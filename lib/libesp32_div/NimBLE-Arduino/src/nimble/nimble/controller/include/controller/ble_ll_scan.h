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

#ifndef H_BLE_LL_SCAN_
#define H_BLE_LL_SCAN_

#include "ble_ll_sched.h"
#include "nimble/porting/nimble/include/hal/hal_timer.h"
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/nimble/include/nimble/nimble_npl.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SCAN_REQ
 *      -> ScanA    (6 bytes)
 *      -> AdvA     (6 bytes)
 *
 *  ScanA is the scanners public (TxAdd=0) or random (TxAdd = 1) address
 *  AdvaA is the advertisers public (RxAdd=0) or random (RxAdd=1) address.
 *
 * Sent by the LL in the Scanning state; received by the LL in the advertising
 * state. The advertising address is the intended recipient of this frame.
 */
#define BLE_SCAN_REQ_LEN                (12)

/*
 * SCAN_RSP
 *      -> AdvA         (6 bytes)
 *      -> ScanRspData  (0 - 31 bytes)
 *
 *  AdvaA is the advertisers public (TxAdd=0) or random (TxAdd=1) address.
 *  ScanRspData may contain any data from the advertisers host.
 *
 * Sent by the LL in the advertising state; received by the LL in the
 * scanning state.
 */
#define BLE_SCAN_RSP_LEGACY_DATA_MAX_LEN       (31)
#define BLE_SCAN_LEGACY_MAX_PKT_LEN            (37)

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
#define BLE_SCAN_RSP_DATA_MAX_LEN       MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE)

/* For Bluetooth 5.0 we need state machine for two PHYs*/
#define BLE_LL_SCAN_PHY_NUMBER          (2)
#else
#define BLE_LL_SCAN_PHY_NUMBER          (1)
#define BLE_SCAN_RSP_DATA_MAX_LEN       BLE_SCAN_RSP_LEGACY_DATA_MAX_LEN
#endif

#define PHY_UNCODED                    (0)
#define PHY_CODED                      (1)

#define BLE_LL_EXT_ADV_MODE_NON_CONN    (0x00)
#define BLE_LL_EXT_ADV_MODE_CONN        (0x01)
#define BLE_LL_EXT_ADV_MODE_SCAN        (0x02)

/* All values are stored as ticks */
struct ble_ll_scan_timing {
    uint32_t interval;
    uint32_t window;
    uint32_t start_time;
};

struct ble_ll_scan_params
{
    uint8_t phy;
    uint8_t own_addr_type;
    uint8_t scan_filt_policy;
    uint8_t configured;
    uint8_t scan_type;
    uint8_t scan_chan;
    struct ble_ll_scan_timing timing;
};

#define BLE_LL_AUX_HAS_ADVA                     0x01
#define BLE_LL_AUX_HAS_TARGETA                  0x02
#define BLE_LL_AUX_HAS_ADI                      0x04
#define BLE_LL_AUX_IS_MATCHED                   0x08
#define BLE_LL_AUX_IS_TARGETA_RESOLVED          0x10

#define BLE_LL_AUX_FLAG_HCI_SENT_ANY            0x02
#define BLE_LL_AUX_FLAG_HCI_SENT_COMPLETED      0x04
#define BLE_LL_AUX_FLAG_HCI_SENT_TRUNCATED      0x08
#define BLE_LL_AUX_FLAG_SCAN_COMPLETE           0x10
#define BLE_LL_AUX_FLAG_SCAN_ERROR              0x20
#define BLE_LL_AUX_FLAG_AUX_ADV_RECEIVED        0x40
#define BLE_LL_AUX_FLAG_AUX_CHAIN_RECEIVED      0x80

struct ble_ll_aux_data {
    uint8_t flags;

    /*
     * Since aux_data can be accessed from ISR and LL, we have separate copies
     * of flags to make sure that ISR does not modify flags while LL uses them.
     * ISR updates 'flags_isr' and LL adds these to 'flags_ll' which it then
     * uses for further processing allowing to update 'flags_isr' if another
     * scan for given 'aux_data' is scheduled. Note that flags must not be unset
     * while aux_data is valid.
     */
    uint8_t flags_isr;
    uint8_t flags_ll;

    uint8_t ref_cnt;
    uint8_t chan;
    uint8_t aux_phy;
    uint8_t aux_primary_phy;
    uint8_t mode;
    uint8_t scanning;
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    int8_t rpa_index;
#endif
    uint16_t adi;
    uint32_t offset;
    uint8_t offset_units;
    uint8_t adva[6];
    uint8_t adva_type;
    uint8_t targeta[6];
    uint8_t targeta_type;
    uint16_t evt_type;
    struct ble_ll_sched_item sch;
    struct ble_hci_ev *evt;
    struct ble_npl_event ev;
};

struct ble_ll_scan_pdu_data {
    uint8_t hdr_byte;
    /* ScanA for SCAN_REQ and InitA for CONNECT_IND */
    union {
        uint8_t scana[BLE_DEV_ADDR_LEN];
        uint8_t inita[BLE_DEV_ADDR_LEN];
    };
    uint8_t adva[BLE_DEV_ADDR_LEN];
};

struct ble_ll_scan_sm
{
    uint8_t scan_enabled;
    uint8_t own_addr_type;
    uint8_t scan_filt_dups;
    uint8_t scan_rsp_pending;
    uint8_t scan_rsp_cons_fails;
    uint8_t scan_rsp_cons_ok;
    uint8_t scan_peer_rpa[BLE_DEV_ADDR_LEN];
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
    ble_npl_time_t scan_nrpa_timer;
    uint8_t scan_nrpa[BLE_DEV_ADDR_LEN];
#endif
    struct ble_ll_scan_pdu_data pdu_data;

    /* XXX: Shall we count backoff per phy? */
    uint16_t upper_limit;
    uint16_t backoff_count;
    uint32_t scan_win_start_time;
    struct ble_npl_event scan_sched_ev;
    struct hal_timer scan_timer;
    struct ble_npl_event scan_interrupted_ev;

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
    struct hal_timer duration_timer;
    struct hal_timer period_timer;
    uint32_t duration_ticks;
    uint32_t period_ticks;
    uint8_t ext_scanning;
#endif

    uint8_t restart_timer_needed;
    struct ble_ll_aux_data *cur_aux_data;

    struct ble_ll_scan_params *scanp;
    struct ble_ll_scan_params *scanp_next;
    struct ble_ll_scan_params scanp_phys[BLE_LL_SCAN_PHY_NUMBER];
};

/* Scan types */
#define BLE_SCAN_TYPE_PASSIVE   (BLE_HCI_SCAN_TYPE_PASSIVE)
#define BLE_SCAN_TYPE_ACTIVE    (BLE_HCI_SCAN_TYPE_ACTIVE)
#define BLE_SCAN_TYPE_INITIATE  (2)

/*---- HCI ----*/
/* Set scanning parameters */
int ble_ll_scan_set_scan_params(const uint8_t *cmdbuf, uint8_t len);

/* Turn scanning on/off */
int ble_ll_hci_scan_set_enable(const uint8_t *cmdbuf, uint8_t len);
int ble_ll_hci_ext_scan_set_enable(const uint8_t *cmdbuf, uint8_t len);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
int ble_ll_set_ext_scan_params(const uint8_t *cmdbuf, uint8_t len);
#endif

/*--- Controller Internal API ---*/
/* Initialize the scanner */
void ble_ll_scan_init(void);

/* Reset the scanner */
void ble_ll_scan_reset(void);

/* Called when Link Layer starts to receive a PDU and is in scanning state */
int ble_ll_scan_rx_isr_start(uint8_t pdu_type, uint16_t *rxflags);

/* Called when Link Layer has finished receiving a PDU while scanning */
int ble_ll_scan_rx_isr_end(struct os_mbuf *rxpdu, uint8_t crcok);

/* Process a scan response PDU */
void ble_ll_scan_rx_pkt_in(uint8_t pdu_type, struct os_mbuf *om,
                           struct ble_mbuf_hdr *hdr);

/* Boolean function denoting whether or not the whitelist can be changed */
int ble_ll_scan_can_chg_whitelist(void);

/* Boolean function returning true if scanning enabled */
int ble_ll_scan_enabled(void);

/* Boolean function returns true if whitelist is enabled for scanning */
int ble_ll_scan_whitelist_enabled(void);

/* Initialize the scanner when we start initiating */
struct hci_create_conn;
int ble_ll_scan_initiator_start(struct hci_create_conn *hcc,
                                struct ble_ll_scan_sm **sm);

/* Returns storage for PDU data (for SCAN_REQ or CONNECT_IND) */
struct ble_ll_scan_pdu_data *ble_ll_scan_get_pdu_data(void);

/* Called to set the resolvable private address of the last connected peer */
void ble_ll_scan_set_peer_rpa(uint8_t *rpa);

/* Returns peer RPA of last connection made */
uint8_t *ble_ll_scan_get_peer_rpa(void);

/* Returns the local RPA used by the scanner/initiator */
uint8_t *ble_ll_scan_get_local_rpa(void);

/* Stop the scanning state machine */
void ble_ll_scan_sm_stop(int chk_disable);

/* Resume scanning */
void ble_ll_scan_chk_resume(void);

/* Called when wait for response timer expires in scanning mode */
void ble_ll_scan_wfr_timer_exp(void);

/* Called when scan could be interrupted  */
void ble_ll_scan_interrupted(struct ble_ll_scan_sm *scansm);

int ble_ll_scan_adv_decode_addr(uint8_t pdu_type, uint8_t *rxbuf,
                                struct ble_mbuf_hdr *ble_hdr,
                                uint8_t **addr, uint8_t *addr_type,
                                uint8_t **inita, uint8_t *init_addr_type,
                                int *ext_mode);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
int ble_ll_scan_update_aux_data(struct ble_mbuf_hdr *ble_hdr, uint8_t *rxbuf,
                                bool *adva_present);

/* Initialize the extended scanner when we start initiating */
struct hci_ext_create_conn;
int ble_ll_scan_ext_initiator_start(struct hci_ext_create_conn *hcc,
                                    struct ble_ll_scan_sm **sm);

/* Called to parse extended advertising*/
struct ble_ll_aux_data *ble_ll_scan_aux_data_ref(struct ble_ll_aux_data *aux_scan);
void ble_ll_scan_aux_data_unref(struct ble_ll_aux_data *aux_scan);
void ble_ll_scan_end_adv_evt(struct ble_ll_aux_data *aux_data);
#endif

/* Called to halt currently running scan */
void ble_ll_scan_halt(void);

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_SCAN_ */
