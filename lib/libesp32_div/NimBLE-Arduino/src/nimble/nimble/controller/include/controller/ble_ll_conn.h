/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_BLE_LL_CONN_
#define H_BLE_LL_CONN_

#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/nimble_npl.h"
#include "ble_ll_sched.h"
#include "ble_ll_ctrl.h"
#include "ble_phy.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Roles */
#define BLE_LL_CONN_ROLE_NONE           (0)
#define BLE_LL_CONN_ROLE_MASTER         (1)
#define BLE_LL_CONN_ROLE_SLAVE          (2)

/* Connection states */
#define BLE_LL_CONN_STATE_IDLE          (0)
#define BLE_LL_CONN_STATE_CREATED       (1)
#define BLE_LL_CONN_STATE_ESTABLISHED   (2)

/* Channel map size */
#define BLE_LL_CONN_CHMAP_LEN           (5)

/* Definitions for source clock accuracy */
#define BLE_MASTER_SCA_251_500_PPM      (0)
#define BLE_MASTER_SCA_151_250_PPM      (1)
#define BLE_MASTER_SCA_101_150_PPM      (2)
#define BLE_MASTER_SCA_76_100_PPM       (3)
#define BLE_MASTER_SCA_51_75_PPM        (4)
#define BLE_MASTER_SCA_31_50_PPM        (5)
#define BLE_MASTER_SCA_21_30_PPM        (6)
#define BLE_MASTER_SCA_0_20_PPM         (7)

/* Definition for RSSI when the RSSI is unknown */
#define BLE_LL_CONN_UNKNOWN_RSSI        (127)

#define BLE_LL_CONN_HANDLE_ISO_OFFSET   (0x0100)

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
/*
 * Encryption states for a connection
 *
 * NOTE: the states are ordered so that we can check to see if the state
 * is greater than ENCRYPTED. If so, it means that the start or pause
 * encryption procedure is running and we should not send data pdu's.
 */
enum conn_enc_state {
    CONN_ENC_S_UNENCRYPTED = 1,
    CONN_ENC_S_ENCRYPTED,
    CONN_ENC_S_ENC_RSP_TO_BE_SENT,
    CONN_ENC_S_ENC_RSP_WAIT,
    CONN_ENC_S_PAUSE_ENC_RSP_WAIT,
    CONN_ENC_S_PAUSED,
    CONN_ENC_S_START_ENC_REQ_WAIT,
    CONN_ENC_S_START_ENC_RSP_WAIT,
    CONN_ENC_S_LTK_REQ_WAIT,
    CONN_ENC_S_LTK_NEG_REPLY
};

/*
 * Note that the LTK is the key, the SDK is the plain text, and the
 * session key is the cipher text portion of the encryption block.
 *
 * NOTE: we have intentionally violated the specification by making the
 * transmit and receive packet counters 32-bits as opposed to 39 (as per the
 * specification). We do this to save code space, ram and calculation time. The
 * only drawback is that any encrypted connection that sends more than 2^32
 * packets will suffer a MIC failure and thus be disconnected.
 */
struct ble_ll_conn_enc_data
{
    uint8_t enc_state;
    uint8_t tx_encrypted;
    uint16_t enc_div;
    uint32_t tx_pkt_cntr;
    uint32_t rx_pkt_cntr;
    uint64_t host_rand_num;
    uint8_t iv[8];
    struct ble_encryption_block enc_block;
};
#endif

/* Connection state machine flags. */
union ble_ll_conn_sm_flags {
    struct {
        uint32_t pkt_rxd:1;
        uint32_t terminate_ind_txd:1;
        uint32_t terminate_ind_rxd:1;
        uint32_t terminate_ind_rxd_acked:1;
        uint32_t allow_slave_latency:1;
        uint32_t slave_set_last_anchor:1;
        uint32_t awaiting_host_reply:1;
        uint32_t terminate_started:1;
        uint32_t conn_update_sched:1;
        uint32_t host_expects_upd_event:1;
        uint32_t version_ind_sent:1;
        uint32_t rxd_version_ind:1;
        uint32_t chanmap_update_scheduled:1;
        uint32_t conn_empty_pdu_txd:1;
        uint32_t last_txd_md:1;
        uint32_t conn_req_txd:1;
        uint32_t send_ltk_req:1;
        uint32_t encrypted:1;
        uint32_t encrypt_chg_sent:1;
        uint32_t le_ping_supp:1;
        uint32_t csa2_supp:1;
        uint32_t host_phy_update: 1;
        uint32_t phy_update_sched: 1;
        uint32_t ctrlr_phy_update: 1;
        uint32_t phy_update_event: 1;
        uint32_t peer_phy_update: 1; /* XXX:combine with ctrlr udpate bit? */
        uint32_t aux_conn_req: 1;
        uint32_t rxd_features:1;
        uint32_t pending_hci_rd_features:1;
        uint32_t pending_initiate_dle:1;
    } cfbit;
    uint32_t conn_flags;
} __attribute__((packed));

/**
 * Structure used for PHY data inside a connection.
 *
 * NOTE: the new phy's are the phys we will change to when a phy update
 * procedure is ongoing and the event counter hits the instant.
 *
 * tx_phy_mode: chip specific phy mode for tx
 * rx_phy_mode: chip specific phy mode for rx
 * cur_tx_phy: value denoting current tx_phy (not a bitmask!)
 * cur_rx_phy: value denoting current rx phy (not a bitmask!)
 * new_tx_phy: value denoting new tx_phy (not a bitmask!)
 * new_rx_phy: value denoting new rx phy (not a bitmask!)
 * req_pref_tx_phy: tx phy sent in a phy request (may be different than host)
 * req_pref_rx_phy: rx phy sent in a phy request (may be different than host)
 * host_pref_tx_phys: bitmask of preferred transmit PHYs sent by host
 * host_pref_rx_phys: bitmask of preferred receive PHYs sent by host
 * phy_options: preferred phy options for coded phy
 */
struct ble_ll_conn_phy_data
{
    uint32_t tx_phy_mode: 2;
    uint32_t rx_phy_mode: 2;
    uint32_t cur_tx_phy: 2;
    uint32_t cur_rx_phy: 2;
    uint32_t new_tx_phy: 2;
    uint32_t new_rx_phy: 2;
    uint32_t host_pref_tx_phys_mask: 3;
    uint32_t host_pref_rx_phys_mask: 3;
    uint32_t req_pref_tx_phys_mask: 3;
    uint32_t req_pref_rx_phys_mask: 3;
    uint32_t phy_options: 2;
}  __attribute__((packed));

#define CONN_CUR_TX_PHY_MASK(csm)   (1 << ((csm)->phy_data.cur_tx_phy - 1))
#define CONN_CUR_RX_PHY_MASK(csm)   (1 << ((csm)->phy_data.cur_rx_phy - 1))

struct hci_conn_update
{
    uint16_t handle;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

struct hci_ext_conn_params
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

struct hci_ext_create_conn
{
    uint8_t filter_policy;
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint8_t init_phy_mask;
    struct hci_ext_conn_params params[3];
};

/* Connection state machine */
struct ble_ll_conn_sm
{
    /* Connection state machine flags */
    union ble_ll_conn_sm_flags csmflags;

    /* Current connection handle, state and role */
    uint16_t conn_handle;
    uint8_t conn_state;
    uint8_t conn_role;          /* Can possibly be 1 bit */

    /* RSSI */
    int8_t conn_rssi;

    /* For privacy */
    int8_t rpa_index;

    /* Connection data length management */
    uint8_t max_tx_octets;
    uint8_t max_rx_octets;
    uint8_t rem_max_tx_octets;
    uint8_t rem_max_rx_octets;
    uint8_t eff_max_tx_octets;
    uint8_t eff_max_rx_octets;
    uint16_t max_tx_time;
    uint16_t max_rx_time;
    uint16_t rem_max_tx_time;
    uint16_t rem_max_rx_time;
    uint16_t eff_max_tx_time;
    uint16_t eff_max_rx_time;
    uint8_t max_tx_octets_phy_mode[BLE_PHY_NUM_MODE];
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CODED_PHY)
    uint16_t host_req_max_tx_time;
#endif

#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
    struct ble_ll_conn_phy_data phy_data;
    uint16_t phy_instant;
    uint8_t phy_tx_transition;
#endif

    /* Used to calculate data channel index for connection */
    uint8_t chanmap[BLE_LL_CONN_CHMAP_LEN];
    uint8_t req_chanmap[BLE_LL_CONN_CHMAP_LEN];
    uint16_t chanmap_instant;
    uint16_t channel_id; /* TODO could be union with hop and last chan used */
    uint8_t hop_inc;
    uint8_t data_chan_index;
    uint8_t last_unmapped_chan;
    uint8_t num_used_chans;

#if MYNEWT_VAL(BLE_LL_STRICT_CONN_SCHEDULING)
    uint8_t period_occ_mask;    /* mask: period 0 = 0x01, period 3 = 0x08 */
#endif

    /* Ack/Flow Control */
    uint8_t tx_seqnum;          /* note: can be 1 bit */
    uint8_t next_exp_seqnum;    /* note: can be 1 bit */
    uint8_t cons_rxd_bad_crc;   /* note: can be 1 bit */
    uint8_t last_rxd_sn;        /* note: cant be 1 bit given current code */
    uint8_t last_rxd_hdr_byte;  /* note: possibly can make 1 bit since we
                                   only use the MD bit now */

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_CTRL_TO_HOST_FLOW_CONTROL)
    uint16_t cth_flow_pending;
#endif

    /* connection event mgmt */
    uint8_t reject_reason;
    uint8_t host_reply_opcode;
    uint8_t master_sca;
    uint8_t tx_win_size;
    uint8_t cur_ctrl_proc;
    uint8_t disconnect_reason;
    uint8_t rxd_disconnect_reason;
    uint8_t vers_nr;
    uint8_t conn_features;
    uint8_t remote_features[7];
    uint16_t pending_ctrl_procs;
    uint16_t event_cntr;
    uint16_t completed_pkts;
    uint16_t comp_id;
    uint16_t sub_vers_nr;
    uint16_t auth_pyld_tmo;         /* could be ifdef'd. 10 msec units */

    uint32_t access_addr;
    uint32_t crcinit;               /* only low 24 bits used */
    /* XXX: do we need ce_end_time? Cant this be sched end time? */
    uint32_t ce_end_time;   /* cputime at which connection event should end */
    uint32_t terminate_timeout;
    uint32_t last_scheduled;

    /* Connection timing */
    uint16_t conn_itvl;
    uint16_t slave_latency;
    uint16_t supervision_tmo;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
    uint16_t tx_win_off;
    uint32_t anchor_point;
    uint8_t anchor_point_usecs;     /* XXX: can this be uint8_t ?*/
    uint8_t conn_itvl_usecs;
    uint32_t conn_itvl_ticks;
    uint32_t last_anchor_point;     /* Slave only */
    uint32_t slave_cur_tx_win_usecs;
    uint32_t slave_cur_window_widening;
    uint32_t last_rxd_pdu_cputime;  /* Used exclusively for supervision timer */

    /*
     * Used to mark that identity address was used as InitA
     */
    uint8_t inita_identity_used;

    /* address information */
    uint8_t own_addr_type;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];

    /*
     * XXX: TODO. Could save memory. Have single event at LL and put these
     * on a singly linked list. Only would need list pointer here.
     */
    /* Connection end event */
    struct ble_npl_event conn_ev_end;

    /* Packet transmit queue */
    struct os_mbuf *cur_tx_pdu;
    STAILQ_HEAD(conn_txq_head, os_mbuf_pkthdr) conn_txq;

    /* List entry for active/free connection pools */
    union {
        SLIST_ENTRY(ble_ll_conn_sm) act_sle;
        STAILQ_ENTRY(ble_ll_conn_sm) free_stqe;
    };

    /* LL control procedure response timer */
    struct ble_npl_callout ctrl_proc_rsp_timer;

    /* For scheduling connections */
    struct ble_ll_sched_item conn_sch;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
    struct ble_npl_callout auth_pyld_timer;
#endif

    /*
     * XXX: a note on all these structures for control procedures. First off,
     * all of these need to be ifdef'd to save memory. Another thing to
     * consider is this: since most control procedures can only run when no
     * others are running, can I use just one structure (a union)? Should I
     * allocate these from a pool? Not sure what to do. For now, I just use
     * a large chunk of memory per connection.
     */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
    struct ble_ll_conn_enc_data enc_data;
#endif
    /*
     * For connection update procedure. XXX: can make this a pointer and
     * malloc it if we want to save space.
     */
    struct hci_conn_update conn_param_req;

    /* For connection update procedure */
    struct ble_ll_conn_upd_req conn_update_req;

    /* XXX: for now, just store them all */
    struct ble_ll_conn_params conn_cp;

    struct ble_ll_scan_sm *scansm;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct hci_ext_create_conn initial_params;
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
    uint8_t  sync_transfer_mode;
    uint16_t sync_transfer_skip;
    uint32_t sync_transfer_sync_timeout;
#endif
};

/* Flags */
#define CONN_F_UPDATE_SCHED(csm)    ((csm)->csmflags.cfbit.conn_update_sched)
#define CONN_F_EMPTY_PDU_TXD(csm)   ((csm)->csmflags.cfbit.conn_empty_pdu_txd)
#define CONN_F_LAST_TXD_MD(csm)     ((csm)->csmflags.cfbit.last_txd_md)
#define CONN_F_CONN_REQ_TXD(csm)    ((csm)->csmflags.cfbit.conn_req_txd)
#define CONN_F_ENCRYPTED(csm)       ((csm)->csmflags.cfbit.encrypted)
#define CONN_F_ENC_CHANGE_SENT(csm) ((csm)->csmflags.cfbit.encrypt_chg_sent)
#define CONN_F_LE_PING_SUPP(csm)    ((csm)->csmflags.cfbit.le_ping_supp)
#define CONN_F_TERMINATE_STARTED(csm) ((csm)->csmflags.cfbit.terminate_started)
#define CONN_F_CSA2_SUPP(csm)       ((csm)->csmflags.cfbit.csa2_supp)
#define CONN_F_HOST_PHY_UPDATE(csm) ((csm)->csmflags.cfbit.host_phy_update)
#define CONN_F_PHY_UPDATE_SCHED(csm) ((csm)->csmflags.cfbit.phy_update_sched)
#define CONN_F_CTRLR_PHY_UPDATE(csm) ((csm)->csmflags.cfbit.ctrlr_phy_update)
#define CONN_F_PHY_UPDATE_EVENT(csm) ((csm)->csmflags.cfbit.phy_update_event)
#define CONN_F_PEER_PHY_UPDATE(csm)  ((csm)->csmflags.cfbit.peer_phy_update)
#define CONN_F_AUX_CONN_REQ(csm)  ((csm)->csmflags.cfbit.aux_conn_req)

/* Role */
#define CONN_IS_MASTER(csm)         (csm->conn_role == BLE_LL_CONN_ROLE_MASTER)
#define CONN_IS_SLAVE(csm)          (csm->conn_role == BLE_LL_CONN_ROLE_SLAVE)

/*
 * Given a handle, returns an active connection state machine (or NULL if the
 * handle does not exist
 *
 */
struct ble_ll_conn_sm *ble_ll_conn_find_active_conn(uint16_t handle);

/* required for unit testing */
uint8_t ble_ll_conn_calc_dci(struct ble_ll_conn_sm *conn, uint16_t latency);

/* used to get anchor point for connection event specified */
void ble_ll_conn_get_anchor(struct ble_ll_conn_sm *connsm, uint16_t conn_event,
                            uint32_t *anchor, uint8_t *anchor_usecs);

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_CONN_ */
