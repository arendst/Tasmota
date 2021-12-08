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

#ifndef H_BLE_LL_CONN_PRIV_
#define H_BLE_LL_CONN_PRIV_

#include "../include/controller/ble_ll_conn.h"
#include "../include/controller/ble_ll_hci.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definitions for min/max RX/TX time/bytes values allowed for connections.
 * Source: Core 5.0 specification, Vol 6, Part B, section 4.5.10
 */
#define BLE_LL_CONN_SUPP_TIME_MIN           (328)   /* usecs */
#define BLE_LL_CONN_SUPP_TIME_MAX           (17040) /* usecs */
#define BLE_LL_CONN_SUPP_TIME_MIN_UNCODED   (328)   /* usecs */
#define BLE_LL_CONN_SUPP_TIME_MAX_UNCODED   (2120)  /* usecs */
#define BLE_LL_CONN_SUPP_TIME_MIN_CODED     (2704)  /* usecs */
#define BLE_LL_CONN_SUPP_TIME_MAX_CODED     (17040) /* usecs */
#define BLE_LL_CONN_SUPP_BYTES_MIN          (27)    /* bytes */
#define BLE_LL_CONN_SUPP_BYTES_MAX          (251)   /* bytes */

/* Connection event timing */
#define BLE_LL_CONN_INITIAL_OFFSET          (1250)
#define BLE_LL_CONN_ITVL_USECS              (1250)
#define BLE_LL_CONN_TX_WIN_USECS            (1250)
#define BLE_LL_CONN_TX_OFF_USECS            (1250)
#define BLE_LL_CONN_CE_USECS                (625)
#define BLE_LL_CONN_TX_WIN_MIN              (1)         /* in tx win units */
#define BLE_LL_CONN_SLAVE_LATENCY_MAX       (499)

/* Connection handle range */
#define BLE_LL_CONN_MAX_CONN_HANDLE         (0x0EFF)

/* Offset (in bytes) of advertising address in connect request */
#define BLE_LL_CONN_REQ_ADVA_OFF    (BLE_LL_PDU_HDR_LEN + BLE_DEV_ADDR_LEN)

/* Default authenticated payload timeout (30 seconds; in 10 msecs increments) */
#define BLE_LL_CONN_DEF_AUTH_PYLD_TMO       (3000)
#define BLE_LL_CONN_AUTH_PYLD_OS_TMO(x)     ble_npl_time_ms_to_ticks32((x) * 10)

/* Global Link Layer connection parameters */
struct ble_ll_conn_global_params
{
    uint8_t master_chan_map[BLE_LL_CONN_CHMAP_LEN];
    uint8_t num_used_chans;
    uint8_t supp_max_tx_octets;
    uint8_t supp_max_rx_octets;
    uint8_t conn_init_max_tx_octets;
    uint8_t sugg_tx_octets;
    uint16_t sugg_tx_time;
    uint16_t conn_init_max_tx_time;
    uint16_t conn_init_max_tx_time_uncoded;
    uint16_t conn_init_max_tx_time_coded;
    uint16_t supp_max_tx_time;
    uint16_t supp_max_rx_time;
};
extern struct ble_ll_conn_global_params g_ble_ll_conn_params;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
struct ble_ll_conn_sync_transfer_params
{
    uint32_t sync_timeout_us;
    uint16_t max_skip;
    uint8_t  mode;
};
extern struct ble_ll_conn_sync_transfer_params g_ble_ll_conn_sync_transfer_params;
#endif

/* Some data structures used by other LL routines */
SLIST_HEAD(ble_ll_conn_active_list, ble_ll_conn_sm);
STAILQ_HEAD(ble_ll_conn_free_list, ble_ll_conn_sm);
extern struct ble_ll_conn_active_list g_ble_ll_conn_active_list;
extern struct ble_ll_conn_free_list g_ble_ll_conn_free_list;

/* Pointer to connection state machine we are trying to create */
extern struct ble_ll_conn_sm *g_ble_ll_conn_create_sm;

/* Generic interface */
struct ble_ll_len_req;
struct ble_mbuf_hdr;
struct ble_ll_adv_sm;

struct hci_create_conn
{
    uint16_t scan_itvl;
    uint16_t scan_window;
    uint8_t filter_policy;
    uint8_t peer_addr_type;
    uint8_t peer_addr[BLE_DEV_ADDR_LEN];
    uint8_t own_addr_type;
    uint16_t conn_itvl_min;
    uint16_t conn_itvl_max;
    uint16_t conn_latency;
    uint16_t supervision_timeout;
    uint16_t min_ce_len;
    uint16_t max_ce_len;
};

void ble_ll_conn_sm_new(struct ble_ll_conn_sm *connsm);
void ble_ll_conn_end(struct ble_ll_conn_sm *connsm, uint8_t ble_err);
void ble_ll_conn_enqueue_pkt(struct ble_ll_conn_sm *connsm, struct os_mbuf *om,
                             uint8_t hdr_byte, uint8_t length);
struct ble_ll_conn_sm *ble_ll_conn_sm_get(void);
void ble_ll_conn_master_init(struct ble_ll_conn_sm *connsm,
                             struct hci_create_conn *hcc);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
void ble_ll_conn_ext_master_init(struct ble_ll_conn_sm *connsm,
                                 struct hci_ext_create_conn *hcc);

void ble_ll_conn_ext_set_params(struct ble_ll_conn_sm *connsm,
                                struct hci_ext_conn_params *hcc_params,
                                int phy);
#endif

struct ble_ll_conn_sm *ble_ll_conn_find_active_conn(uint16_t handle);
void ble_ll_conn_update_eff_data_len(struct ble_ll_conn_sm *connsm);

/* Advertising interface */
int ble_ll_conn_slave_start(uint8_t *rxbuf, uint8_t pat,
                            struct ble_mbuf_hdr *rxhdr, bool force_csa2);

/* Link Layer interface */
void ble_ll_conn_module_init(void);
void ble_ll_conn_set_global_chanmap(uint8_t num_used_chans, const uint8_t *chanmap);
void ble_ll_conn_module_reset(void);
void ble_ll_conn_tx_pkt_in(struct os_mbuf *om, uint16_t handle, uint16_t len);
int ble_ll_conn_rx_isr_start(struct ble_mbuf_hdr *rxhdr, uint32_t aa);
int ble_ll_conn_rx_isr_end(uint8_t *rxbuf, struct ble_mbuf_hdr *rxhdr);
void ble_ll_conn_rx_data_pdu(struct os_mbuf *rxpdu, struct ble_mbuf_hdr *hdr);
void ble_ll_init_rx_pkt_in(uint8_t pdu_type, uint8_t *rxbuf,
                           struct ble_mbuf_hdr *ble_hdr);
int ble_ll_init_rx_isr_start(uint8_t pdu_type, struct ble_mbuf_hdr *ble_hdr);
int ble_ll_init_rx_isr_end(uint8_t *rxbuf, uint8_t crcok,
                           struct ble_mbuf_hdr *ble_hdr);
void ble_ll_conn_wfr_timer_exp(void);
void ble_ll_conn_init_wfr_timer_exp(void);
int ble_ll_conn_is_lru(struct ble_ll_conn_sm *s1, struct ble_ll_conn_sm *s2);
uint32_t ble_ll_conn_get_ce_end_time(void);
void ble_ll_conn_event_halt(void);
void ble_ll_conn_reset_pending_aux_conn_rsp(void);
bool ble_ll_conn_init_pending_aux_conn_rsp(void);
/* HCI */
void ble_ll_disconn_comp_event_send(struct ble_ll_conn_sm *connsm,
                                    uint8_t reason);
void ble_ll_auth_pyld_tmo_event_send(struct ble_ll_conn_sm *connsm);
int ble_ll_conn_hci_disconnect_cmd(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_rd_rem_ver_cmd(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_create(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_update(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_set_chan_class(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_param_rr(const uint8_t *cmdbuf, uint8_t len,
                             uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_param_nrr(const uint8_t *cmdbuf, uint8_t len,
                             uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_create_cancel(ble_ll_hci_post_cmd_complete_cb *post_cmd_cb);
void ble_ll_conn_num_comp_pkts_event_send(struct ble_ll_conn_sm *connsm);
void ble_ll_conn_comp_event_send(struct ble_ll_conn_sm *connsm, uint8_t status,
                                 uint8_t *evbuf, struct ble_ll_adv_sm *advsm);
void ble_ll_conn_timeout(struct ble_ll_conn_sm *connsm, uint8_t ble_err);
int ble_ll_conn_hci_chk_conn_params(uint16_t itvl_min, uint16_t itvl_max,
                                    uint16_t latency, uint16_t spvn_tmo);
int ble_ll_conn_hci_read_rem_features(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_rd_rssi(const uint8_t *cmdbuf, uint8_t len, uint8_t *rspbuf,
                            uint8_t *rsplen);
int ble_ll_conn_hci_rd_chan_map(const uint8_t *cmdbuf, uint8_t len,
                                uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_set_data_len(const uint8_t *cmdbuf, uint8_t len,
                                 uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_le_start_encrypt(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_hci_le_ltk_reply(const uint8_t *cmdbuf, uint8_t len,
                                 uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_le_ltk_neg_reply(const uint8_t *cmdbuf, uint8_t len,
                                     uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_wr_auth_pyld_tmo(const uint8_t *cmdbuf, uint8_t len,
                                     uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_conn_hci_rd_auth_pyld_tmo(const uint8_t *cmdbuf, uint8_t len,
                                     uint8_t *rspbuf, uint8_t *rsplen);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
void ble_ll_conn_auth_pyld_timer_start(struct ble_ll_conn_sm *connsm);
#else
#define ble_ll_conn_auth_pyld_timer_start(x)
#endif

int ble_ll_hci_cmd_rx(uint8_t *cmd, void *arg);
int ble_ll_hci_acl_rx(struct os_mbuf *om, void *arg);

int ble_ll_conn_hci_le_rd_phy(const uint8_t *cmdbuf, uint8_t len,
                              uint8_t *rsp, uint8_t *rsplen);
int ble_ll_conn_hci_le_set_phy(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_conn_chk_phy_upd_start(struct ble_ll_conn_sm *connsm);
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
int ble_ll_ext_conn_create(const uint8_t *cmdbuf, uint8_t cmdlen);
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
int ble_ll_set_sync_transfer_params(const uint8_t *cmdbuf, uint8_t len,
                                    uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_set_default_sync_transfer_params(const uint8_t *cmdbuf, uint8_t len);
#endif

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_CONN_PRIV_ */
