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

#ifndef H_BLE_LL_CTRL_
#define H_BLE_LL_CTRL_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * LL control procedures. This "enumeration" is not in the specification;
 * It is used to determine which LL control procedure is currently running
 * in a connection and which ones may be pending.
 */
#define BLE_LL_CTRL_PROC_CONN_UPDATE    (0)
#define BLE_LL_CTRL_PROC_CHAN_MAP_UPD   (1)
#define BLE_LL_CTRL_PROC_ENCRYPT        (2)
#define BLE_LL_CTRL_PROC_FEATURE_XCHG   (3)
#define BLE_LL_CTRL_PROC_VERSION_XCHG   (4)
#define BLE_LL_CTRL_PROC_TERMINATE      (5)
#define BLE_LL_CTRL_PROC_CONN_PARAM_REQ (6)
#define BLE_LL_CTRL_PROC_LE_PING        (7)
#define BLE_LL_CTRL_PROC_DATA_LEN_UPD   (8)
#define BLE_LL_CTRL_PROC_PHY_UPDATE     (9)
#define BLE_LL_CTRL_PROC_SCA_UPDATE     (10)
#define BLE_LL_CTRL_PROC_CIS_CREATE     (11)
#define BLE_LL_CTRL_PROC_NUM            (12)
#define BLE_LL_CTRL_PROC_IDLE           (255)

/* Checks if a particular control procedure is running */
#define IS_PENDING_CTRL_PROC(sm, proc)  (sm->pending_ctrl_procs & (1 << proc))
#define CLR_PENDING_CTRL_PROC(sm, proc) (sm->pending_ctrl_procs &= ~(1 << proc))

/* LL control procedure timeout */
#define BLE_LL_CTRL_PROC_TIMEOUT_MS     (40000) /* ms */

/*
 * LL CTRL PDU format
 *  -> Opcode   (1 byte)
 *  -> Data     (0 - 26 bytes)
 */
#define BLE_LL_CTRL_CONN_UPDATE_IND     (0x00)
#define BLE_LL_CTRL_CHANNEL_MAP_REQ     (0x01)
#define BLE_LL_CTRL_TERMINATE_IND       (0x02)
#define BLE_LL_CTRL_ENC_REQ             (0x03)
#define BLE_LL_CTRL_ENC_RSP             (0x04)
#define BLE_LL_CTRL_START_ENC_REQ       (0x05)
#define BLE_LL_CTRL_START_ENC_RSP       (0x06)
#define BLE_LL_CTRL_UNKNOWN_RSP         (0x07)
#define BLE_LL_CTRL_FEATURE_REQ         (0x08)
#define BLE_LL_CTRL_FEATURE_RSP         (0x09)
#define BLE_LL_CTRL_PAUSE_ENC_REQ       (0x0A)
#define BLE_LL_CTRL_PAUSE_ENC_RSP       (0x0B)
#define BLE_LL_CTRL_VERSION_IND         (0x0C)
#define BLE_LL_CTRL_REJECT_IND          (0x0D)
#define BLE_LL_CTRL_SLAVE_FEATURE_REQ   (0x0E)
#define BLE_LL_CTRL_CONN_PARM_REQ       (0x0F)
#define BLE_LL_CTRL_CONN_PARM_RSP       (0x10)
#define BLE_LL_CTRL_REJECT_IND_EXT      (0x11)
#define BLE_LL_CTRL_PING_REQ            (0x12)
#define BLE_LL_CTRL_PING_RSP            (0x13)
#define BLE_LL_CTRL_LENGTH_REQ          (0x14)
#define BLE_LL_CTRL_LENGTH_RSP          (0x15)
#define BLE_LL_CTRL_PHY_REQ             (0x16)
#define BLE_LL_CTRL_PHY_RSP             (0x17)
#define BLE_LL_CTRL_PHY_UPDATE_IND      (0x18)
#define BLE_LL_CTRL_MIN_USED_CHAN_IND   (0x19)
#define BLE_LL_CTRL_CTE_REQ             (0x1A)
#define BLE_LL_CTRL_CTE_RSP             (0x1B)
#define BLE_LL_CTRL_PERIODIC_SYNC_IND   (0x1C)
#define BLE_LL_CTRL_CLOCK_ACCURACY_REQ  (0x1D)
#define BLE_LL_CTRL_CLOCK_ACCURACY_RSP  (0x1E)
#define BLE_LL_CTRL_CIS_REQ             (0x1F)
#define BLE_LL_CTRL_CIS_RSP             (0x20)
#define BLE_LL_CTRL_CIS_IND             (0x21)
#define BLE_LL_CTRL_CIS_TERMINATE_IND   (0x22)

/* Maximum opcode value */
#define BLE_LL_CTRL_OPCODES             (BLE_LL_CTRL_CIS_TERMINATE_IND + 1)

extern const uint8_t g_ble_ll_ctrl_pkt_lengths[BLE_LL_CTRL_OPCODES];

/* Maximum LL control PDU size */
#if MYNEWT_VAL(BLE_ISO)
#define BLE_LL_CTRL_MAX_PDU_LEN         (42)
#elif MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PERIODIC_ADV_SYNC_TRANSFER)
#define BLE_LL_CTRL_MAX_PDU_LEN         (35)
#else
#define BLE_LL_CTRL_MAX_PDU_LEN         (27)
#endif

/* LL control connection update request */
struct ble_ll_conn_upd_req
{
    uint8_t winsize;
    uint16_t winoffset;
    uint16_t interval;
    uint16_t latency;
    uint16_t timeout;
    uint16_t instant;
};
#define BLE_LL_CTRL_CONN_UPD_REQ_LEN        (11)

/* LL control channel map request */
struct ble_ll_chan_map_req
{
    uint8_t chmap[5];
    uint16_t instant;
};
#define BLE_LL_CTRL_CHAN_MAP_LEN            (7)

/*
 * LL control terminate ind
 *  -> error code (1 byte)
 */
#define BLE_LL_CTRL_TERMINATE_IND_LEN      (1)

/* LL control enc req */
struct ble_ll_enc_req
{
    uint8_t rand[8];
    uint16_t ediv;
    uint8_t skdm[8];
    uint32_t ivm;
};

#define BLE_LL_CTRL_ENC_REQ_LEN             (22)

/* LL control enc rsp */
struct ble_ll_enc_rsp
{
    uint8_t skds[8];
    uint32_t ivs;
};

#define BLE_LL_CTRL_ENC_RSP_LEN             (12)

/* LL control start/pause enc request and response */
#define BLE_LL_CTRL_START_ENC_REQ_LEN       (0)
#define BLE_LL_CTRL_START_ENC_RSP_LEN       (0)
#define BLE_LL_CTRL_PAUSE_ENC_REQ_LEN       (0)
#define BLE_LL_CTRL_PAUSE_ENC_RSP_LEN       (0)

/*
 * LL control unknown response
 *  -> 1 byte which contains the unknown or un-supported opcode.
 */
#define BLE_LL_CTRL_UNK_RSP_LEN             (1)

/*
 * LL control feature req and LL control feature rsp
 *  -> 8 bytes of data containing features supported by device.
 */
#define BLE_LL_CTRL_FEATURE_LEN             (8)

/*
 * LL control version ind
 *  -> version (1 byte):
 *      Contains the version number of the bluetooth controller specification.
 *  -> comp_id (2 bytes)
 *      Contains the company identifier of the manufacturer of the controller.
 *  -> sub_ver_num: Contains a unique value for implementation or revision of
 *      the bluetooth controller.
 */
struct ble_ll_version_ind
{
    uint8_t ble_ctrlr_ver;
    uint16_t company_id;
    uint16_t sub_ver_num;
};

#define BLE_LL_CTRL_VERSION_IND_LEN         (5)

/*
 * LL control reject ind
 *  -> error code (1 byte): contains reason why request was rejected.
 */
#define BLE_LL_CTRL_REJ_IND_LEN             (1)

/*
 * LL control slave feature req
 *  -> 8 bytes of data containing features supported by device.
 */
#define BLE_LL_CTRL_SLAVE_FEATURE_REQ_LEN   (8)

/* LL control connection param req and connection param rsp */
struct ble_ll_conn_params
{
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t latency;
    uint16_t timeout;
    uint8_t pref_periodicity;
    uint16_t ref_conn_event_cnt;
    uint16_t offset0;
    uint16_t offset1;
    uint16_t offset2;
    uint16_t offset3;
    uint16_t offset4;
    uint16_t offset5;
};

#define BLE_LL_CTRL_CONN_PARAMS_LEN     (23)

/* LL control reject ind ext */
struct ble_ll_reject_ind_ext
{
    uint8_t reject_opcode;
    uint8_t err_code;
};

#define BLE_LL_CTRL_REJECT_IND_EXT_LEN  (2)

/* LL control ping req and ping rsp (contain no data) */
#define BLE_LL_CTRL_PING_LEN            (0)

/*
 * LL control length req and length rsp
 *  -> max_rx_bytes (2 bytes): defines connMaxRxOctets. Range 27 to 251
 *  -> max_rx_time (2 bytes): defines connMaxRxTime. Range 328 to 2120 usecs.
 *  -> max_tx_bytes (2 bytes): defines connMaxTxOctets. Range 27 to 251
 *  -> max_tx_time (2 bytes): defines connMaxTxTime. Range 328 to 2120 usecs.
 */
struct ble_ll_len_req
{
    uint16_t max_rx_bytes;
    uint16_t max_rx_time;
    uint16_t max_tx_bytes;
    uint16_t max_tx_time;
};

#define BLE_LL_CTRL_LENGTH_REQ_LEN      (8)

/* PHY request/response */
#define BLE_LL_CTRL_PHY_REQ_LEN         (2)
#define BLE_LL_CTRL_PHY_RSP_LEN         (2)
#define BLE_LL_CTRL_PHY_UPD_IND_LEN     (4)

/* Min used channels */
#define BLE_LL_CTRL_MIN_USED_CHAN_LEN   (2)

/* CTE REQ */
#define BLE_LL_CTRL_CTE_REQ_LEN         (1)

/* CTE RSP (contains no data) */
#define BLE_LL_CTRL_CTE_RSP_LEN     (0)

/* Periodic Sync Transfer IND */
#define BLE_LL_CTRL_PERIODIC_SYNC_IND_LEN   (34)

/* Clock accuracy request/response */
#define BLE_LL_CTRL_CLOCK_ACCURACY_REQ_LEN  (1)
#define BLE_LL_CTRL_CLOCK_ACCURACY_RSP_LEN  (1)

/* BLE ISO */
#define BLE_LL_CTRL_CIS_REQ_LEN         (42)
#define BLE_LL_CTRL_CIS_RSP_LEN         (8)
#define BLE_LL_CTRL_CIS_IND_LEN         (15)
#define BLE_LL_CTRL_CIS_TERMINATE_LEN   (3)

/* API */
struct ble_ll_conn_sm;
void ble_ll_ctrl_proc_start(struct ble_ll_conn_sm *connsm, int ctrl_proc);
void ble_ll_ctrl_proc_stop(struct ble_ll_conn_sm *connsm, int ctrl_proc);
int ble_ll_ctrl_rx_pdu(struct ble_ll_conn_sm *connsm, struct os_mbuf *om);
void ble_ll_ctrl_chk_proc_start(struct ble_ll_conn_sm *connsm);
void ble_ll_ctrl_terminate_start(struct ble_ll_conn_sm *connsm);
int ble_ll_ctrl_is_terminate_ind(uint8_t hdr, uint8_t opcode);
uint8_t ble_ll_ctrl_conn_param_reply(struct ble_ll_conn_sm *connsm,
                                     uint8_t *rsp,
                                     struct ble_ll_conn_params *req);
int ble_ll_ctrl_reject_ind_send(struct ble_ll_conn_sm *connsm,
                                uint8_t rej_opcode, uint8_t err);
int ble_ll_ctrl_start_enc_send(struct ble_ll_conn_sm *connsm);
int ble_ll_ctrl_enc_allowed_pdu_rx(struct os_mbuf *rxpdu);
int ble_ll_ctrl_enc_allowed_pdu_tx(struct os_mbuf_pkthdr *pkthdr);
int ble_ll_ctrl_tx_done(struct os_mbuf *txpdu, struct ble_ll_conn_sm *connsm);
int ble_ll_ctrl_is_start_enc_rsp(struct os_mbuf *txpdu);

void ble_ll_hci_ev_datalen_chg(struct ble_ll_conn_sm *connsm);
void ble_ll_hci_ev_rem_conn_parm_req(struct ble_ll_conn_sm *connsm,
                                     struct ble_ll_conn_params *cp);
void ble_ll_hci_ev_conn_update(struct ble_ll_conn_sm *connsm, uint8_t status);
void ble_ll_hci_ev_rd_rem_used_feat(struct ble_ll_conn_sm *connsm,
                                      uint8_t status);
void ble_ll_hci_ev_rd_rem_ver(struct ble_ll_conn_sm *connsm, uint8_t status);
void ble_ll_hci_ev_encrypt_chg(struct ble_ll_conn_sm *connsm, uint8_t status);
int ble_ll_hci_ev_ltk_req(struct ble_ll_conn_sm *connsm);
int ble_ll_hci_ev_hw_err(uint8_t hw_err);
void ble_ll_hci_ev_databuf_overflow(void);
void ble_ll_hci_ev_le_csa(struct ble_ll_conn_sm *connsm);
void ble_ll_hci_ev_send_scan_req_recv(uint8_t adv_handle, const uint8_t *peer,
                                      uint8_t peer_addr_type);
void ble_ll_hci_ev_send_scan_timeout(void);
void ble_ll_hci_ev_send_adv_set_terminated(uint8_t status, uint8_t adv_handle,
                                           uint16_t conn_handle, uint8_t events);
int ble_ll_hci_ev_phy_update(struct ble_ll_conn_sm *connsm, uint8_t status);
void ble_ll_calc_session_key(struct ble_ll_conn_sm *connsm);
void ble_ll_ctrl_phy_update_proc_complete(struct ble_ll_conn_sm *connsm);
void ble_ll_ctrl_initiate_dle(struct ble_ll_conn_sm *connsm);
void ble_ll_hci_ev_send_vendor_err(const char *file, uint32_t line);

uint8_t ble_ll_ctrl_phy_tx_transition_get(uint8_t phy_mask);
uint8_t ble_ll_ctrl_phy_from_phy_mask(uint8_t phy_mask);

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
void ble_ll_hci_ev_sca_update(struct ble_ll_conn_sm *connsm,
                              uint8_t status, uint8_t peer_sca);
#endif

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_CTRL_ */
