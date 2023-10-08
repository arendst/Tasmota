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
#ifndef ESP_PLATFORM
 
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/nimble/include/nimble/ble.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_hci.h"
#include "../include/controller/ble_ll_ctrl.h"
#include "ble_ll_conn_priv.h"

#if (BLETEST_CONCURRENT_CONN_TEST == 1)
extern void bletest_ltk_req_reply(uint16_t handle);
#endif

/**
 * Send a data length change event for a connection to the host.
 *
 * @param connsm Pointer to connection state machine
 */
void
ble_ll_hci_ev_datalen_chg(struct ble_ll_conn_sm *connsm)
{
    struct ble_hci_ev_le_subev_data_len_chg *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_DATA_LEN_CHG)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_DATA_LEN_CHG;
            ev->conn_handle = htole16(connsm->conn_handle);

            ev->max_tx_octets = htole16(connsm->eff_max_tx_octets);
            ev->max_tx_time = htole16(connsm->eff_max_tx_time);
            ev->max_rx_octets = htole16(connsm->eff_max_rx_octets);
            ev->max_rx_time = htole16(connsm->eff_max_rx_time);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

/**
 * Send a connection parameter request event for a connection to the host.
 *
 * @param connsm Pointer to connection state machine
 */
void
ble_ll_hci_ev_rem_conn_parm_req(struct ble_ll_conn_sm *connsm,
                                struct ble_ll_conn_params *cp)
{
    struct ble_hci_ev_le_subev_rem_conn_param_req *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->min_interval = htole16(cp->interval_min);
            ev->max_interval = htole16(cp->interval_max);
            ev->latency = htole16(cp->latency);
            ev->timeout = htole16(cp->timeout);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

/**
 * Send a connection update event.
 *
 * @param connsm Pointer to connection state machine
 * @param status The error code.
 */
void
ble_ll_hci_ev_conn_update(struct ble_ll_conn_sm *connsm, uint8_t status)
{
    struct ble_hci_ev_le_subev_conn_upd_complete *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE;
            ev->status = status;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->conn_itvl = htole16(connsm->conn_itvl);
            ev->conn_latency = htole16(connsm->slave_latency);
            ev->supervision_timeout = htole16(connsm->supervision_tmo);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_ENCRYPTION)
void
ble_ll_hci_ev_encrypt_chg(struct ble_ll_conn_sm *connsm, uint8_t status)
{
    struct ble_hci_ev_enc_key_refresh *ev_key_refresh;
    struct ble_hci_ev_enrypt_chg *ev_enc_chf;
    struct ble_hci_ev *hci_ev;

    if (CONN_F_ENC_CHANGE_SENT(connsm) == 0) {
        if (ble_ll_hci_is_event_enabled(BLE_HCI_EVCODE_ENCRYPT_CHG)) {
            hci_ev = (void *)ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
            if (hci_ev) {
                hci_ev->opcode = BLE_HCI_EVCODE_ENCRYPT_CHG;
                hci_ev->length = sizeof(*ev_enc_chf);
                ev_enc_chf = (void *) hci_ev->data;

                ev_enc_chf->status = status;
                ev_enc_chf->connection_handle = htole16(connsm->conn_handle);
                ev_enc_chf->enabled = (status == BLE_ERR_SUCCESS) ? 0x01 : 0x00;

                ble_ll_hci_event_send(hci_ev);
            }
        }

        CONN_F_ENC_CHANGE_SENT(connsm) = 1;
        return;
    }

    if (ble_ll_hci_is_event_enabled(BLE_HCI_EVCODE_ENC_KEY_REFRESH)) {
        hci_ev = (void *)ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_ENC_KEY_REFRESH;
            hci_ev->length = sizeof(*ev_key_refresh);
            ev_key_refresh = (void *) hci_ev->data;

            ev_key_refresh->status = status;
            ev_key_refresh->conn_handle = htole16(connsm->conn_handle);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

/**
 * Send a long term key request event for a connection to the host.
 *
 * @param connsm Pointer to connection state machine
 */
int
ble_ll_hci_ev_ltk_req(struct ble_ll_conn_sm *connsm)
{
    struct ble_hci_ev_le_subev_lt_key_req *ev;
    struct ble_hci_ev *hci_ev;
    int rc;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_LT_KEY_REQ)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_LT_KEY_REQ;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->rand = htole64(connsm->enc_data.host_rand_num);
            ev->div = htole16(connsm->enc_data.enc_div);

            ble_ll_hci_event_send(hci_ev);
        }
        rc = 0;
    } else {
        rc = -1;
    }

#if (BLETEST_CONCURRENT_CONN_TEST == 1)
    if (rc == 0) {
        bletest_ltk_req_reply(connsm->conn_handle);
    }
#endif
    return rc;
}
#endif

void
ble_ll_hci_ev_rd_rem_used_feat(struct ble_ll_conn_sm *connsm, uint8_t status)
{
    struct ble_hci_ev_le_subev_rd_rem_used_feat *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT;
            ev->status = status;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->features[0] = connsm->conn_features;
            memcpy(ev->features + 1, connsm->remote_features, 7);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

void
ble_ll_hci_ev_rd_rem_ver(struct ble_ll_conn_sm *connsm, uint8_t status)
{
    struct ble_hci_ev_rd_rem_ver_info_cmp *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_event_enabled(BLE_HCI_EVCODE_RD_REM_VER_INFO_CMP)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_RD_REM_VER_INFO_CMP;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->status = status;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->version = connsm->vers_nr;
            ev->manufacturer = htole16(connsm->comp_id);
            ev->subversion = htole16(connsm->sub_vers_nr);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

/**
 * Send a HW error to the host.
 *
 * @param hw_err
 *
 * @return int 0: event masked or event sent, -1 otherwise
 */
int
ble_ll_hci_ev_hw_err(uint8_t hw_err)
{
    struct ble_hci_ev_hw_error *ev;
    struct ble_hci_ev *hci_ev;
    int rc;

    rc = 0;
    if (ble_ll_hci_is_event_enabled(BLE_HCI_EVCODE_HW_ERROR)) {
        hci_ev = (void *)ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_HW_ERROR;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->hw_code = hw_err;

            ble_ll_hci_event_send(hci_ev);
        } else {
            rc = -1;
        }
    }
    return rc;
}

void
ble_ll_hci_ev_databuf_overflow(void)
{
    struct ble_hci_ev_data_buf_overflow *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_event_enabled(BLE_HCI_EVCODE_DATA_BUF_OVERFLOW)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_DATA_BUF_OVERFLOW;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->link_type = BLE_HCI_EVENT_ACL_BUF_OVERFLOW;

            ble_ll_hci_event_send(hci_ev);
        }
    }
}

/**
 * Send a LE Channel Selection Algorithm event.
 *
 * @param connsm Pointer to connection state machine
 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_CSA2)
void
ble_ll_hci_ev_le_csa(struct ble_ll_conn_sm *connsm)
{
    struct ble_hci_ev_le_subev_chan_sel_alg *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_CHAN_SEL_ALG)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_CHAN_SEL_ALG;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->csa = connsm->csmflags.cfbit.csa2_supp ? 0x01 : 0x00;

            ble_ll_hci_event_send(hci_ev);
        }
    }
}
#endif

/**
 * Sends the LE Scan Request Received event
 *
 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
void
ble_ll_hci_ev_send_scan_req_recv(uint8_t adv_handle, const uint8_t *peer,
                                 uint8_t peer_addr_type)
{
    struct ble_hci_ev_le_subev_scan_req_rcvd *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD;
            ev->adv_handle = adv_handle;
            ev->peer_addr_type = peer_addr_type;
            memcpy(ev->peer_addr, peer, BLE_DEV_ADDR_LEN);

            ble_ll_hci_event_send(hci_ev);
        }
    }
}
#endif

/**
 * Sends the  LE Scan Timeout Event
 *
 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
void
ble_ll_hci_ev_send_scan_timeout(void)
{
    struct ble_hci_ev_le_subev_scan_timeout *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_SCAN_TIMEOUT)) {
        hci_ev = (void *)ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_SCAN_TIMEOUT;

            ble_ll_hci_event_send(hci_ev);
        }
    }
}
#endif

/**
 * Sends the LE Advertising Set Terminated event
 *
 */
#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_EXT_ADV)
void
ble_ll_hci_ev_send_adv_set_terminated(uint8_t status, uint8_t adv_handle,
                                      uint16_t conn_handle, uint8_t events)
{
    struct ble_hci_ev_le_subev_adv_set_terminated *ev;
    struct ble_hci_ev *hci_ev;

    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED;
            ev->status = status;
            ev->adv_handle = adv_handle;
            ev->conn_handle = htole16(conn_handle);
            ev->num_events = events;

            ble_ll_hci_event_send(hci_ev);
        }
    }
}
#endif

/**
 * Send a PHY update complete event
 *
 * @param connsm Pointer to connection state machine
 * @param status error status of event
 */
#if (BLE_LL_BT5_PHY_SUPPORTED == 1)
int
ble_ll_hci_ev_phy_update(struct ble_ll_conn_sm *connsm, uint8_t status)
{
    struct ble_hci_ev_le_subev_phy_update_complete *ev;
    struct ble_hci_ev *hci_ev;
    int rc;

    rc = 0;
    if (ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE)) {
        hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
        if (hci_ev) {
            hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
            hci_ev->length = sizeof(*ev);
            ev = (void *) hci_ev->data;

            ev->subev_code = BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE;
            ev->status = status;
            ev->conn_handle = htole16(connsm->conn_handle);
            ev->tx_phy = connsm->phy_data.cur_tx_phy;
            ev->rx_phy = connsm->phy_data.cur_rx_phy;

            ble_ll_hci_event_send(hci_ev);
        } else {
            rc = BLE_ERR_MEM_CAPACITY;
        }
    }
    return rc;
}
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_SCA_UPDATE)
void
ble_ll_hci_ev_sca_update(struct ble_ll_conn_sm *connsm, uint8_t status,
                         uint8_t peer_sca)
{
    struct ble_hci_ev_le_subev_peer_sca_complete *ev;
    struct ble_hci_ev *hci_ev;

    if (!ble_ll_hci_is_le_event_enabled(BLE_HCI_LE_SUBEV_REQ_PEER_SCA_COMP)) {
        return;
    }

    hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
    if (!hci_ev) {
        return;
    }

    hci_ev->opcode = BLE_HCI_EVCODE_LE_META;
    hci_ev->length = sizeof(*ev);
    ev = (void *) hci_ev->data;

    ev->subev_code = BLE_HCI_LE_SUBEV_REQ_PEER_SCA_COMP;
    ev->status = status;
    ev->conn_handle = htole16(connsm->conn_handle);
    ev->sca = peer_sca;

    ble_ll_hci_event_send(hci_ev);
}

#endif

void
ble_ll_hci_ev_send_vendor_err(const char *file, uint32_t line)
{
    struct ble_hci_ev_vendor_debug *ev;
    struct ble_hci_ev *hci_ev;
    unsigned int str_len;
    bool skip = true;
    uint8_t digit;
    int max_len;
    int i;

    /* 6 is for line number ":00000" , we assume files have no more than 64k of
     * lines
     */
    max_len = BLE_HCI_MAX_DATA_LEN - sizeof(*ev) - 6;

    hci_ev = (void *) ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
    if (hci_ev) {
        hci_ev->opcode = BLE_HCI_EVCODE_VENDOR_DEBUG;
        hci_ev->length = sizeof(*ev);
        ev = (void *) hci_ev->data;

        /* Debug id for future use */
        ev->id = 0x00;

        /* snprintf would be nicer but this is heavy on flash
         * len = snprintf((char *) ev->data, max_len, "%s:%u", file, line);
         * if (len < 0) {
         *     len = 0;
         * } else if (len > max_len) {
         *     len = max_len;
         * }
         *
         *  hci_ev->length += len;
         */
        str_len = strlen(file);
        if (str_len > max_len) {
            str_len = max_len;
        }

        memcpy(ev->data, file, str_len);
        ev->data[str_len++] = ':';

        for (i = 100000; i >= 10; i /= 10) {
            digit = (line % i) / (i/10);

            if (!digit && skip) {
                continue;
            }

            skip = false;
            ev->data[str_len++] = '0' + digit;
        }

        hci_ev->length += str_len;

        ble_ll_hci_event_send(hci_ev);
    }
}

#endif