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
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "os/os.h"
#include "nimble/hci_common.h"
#include "nimble/ble_hci_trans.h"
#include "host/ble_gap.h"
#include "host/ble_monitor.h"
#include "ble_hs_priv.h"
#include "ble_hs_dbg_priv.h"
#include "ble_hs_resolv_priv.h"

_Static_assert(sizeof (struct hci_data_hdr) == BLE_HCI_DATA_HDR_SZ,
               "struct hci_data_hdr must be 4 bytes");

typedef int ble_hs_hci_evt_fn(uint8_t event_code, uint8_t *data, int len);
static ble_hs_hci_evt_fn ble_hs_hci_evt_disconn_complete;
static ble_hs_hci_evt_fn ble_hs_hci_evt_encrypt_change;
static ble_hs_hci_evt_fn ble_hs_hci_evt_hw_error;
static ble_hs_hci_evt_fn ble_hs_hci_evt_num_completed_pkts;
static ble_hs_hci_evt_fn ble_hs_hci_evt_enc_key_refresh;
static ble_hs_hci_evt_fn ble_hs_hci_evt_le_meta;

typedef int ble_hs_hci_evt_le_fn(uint8_t subevent, uint8_t *data, int len);
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_upd_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_lt_key_req;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_parm_req;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_dir_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_phy_update_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_ext_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_rd_rem_used_feat_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_scan_timeout;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_adv_set_terminated;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_sync_estab;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_sync_lost;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_scan_req_rcvd;
/* Statistics */
struct host_hci_stats
{
    uint32_t events_rxd;
    uint32_t good_acks_rxd;
    uint32_t bad_acks_rxd;
    uint32_t unknown_events_rxd;
};

#define BLE_HS_HCI_EVT_TIMEOUT        50      /* Milliseconds. */

/** Dispatch table for incoming HCI events.  Sorted by event code field. */
struct ble_hs_hci_evt_dispatch_entry {
    uint8_t event_code;
    ble_hs_hci_evt_fn *cb;
};

static const struct ble_hs_hci_evt_dispatch_entry ble_hs_hci_evt_dispatch[] = {
    { BLE_HCI_EVCODE_LE_META, ble_hs_hci_evt_le_meta },
    { BLE_HCI_EVCODE_NUM_COMP_PKTS, ble_hs_hci_evt_num_completed_pkts },
    { BLE_HCI_EVCODE_DISCONN_CMP, ble_hs_hci_evt_disconn_complete },
    { BLE_HCI_EVCODE_ENCRYPT_CHG, ble_hs_hci_evt_encrypt_change },
    { BLE_HCI_EVCODE_ENC_KEY_REFRESH, ble_hs_hci_evt_enc_key_refresh },
    { BLE_HCI_EVCODE_HW_ERROR, ble_hs_hci_evt_hw_error },
};

#define BLE_HS_HCI_EVT_DISPATCH_SZ \
    (sizeof ble_hs_hci_evt_dispatch / sizeof ble_hs_hci_evt_dispatch[0])

static ble_hs_hci_evt_le_fn * const ble_hs_hci_evt_le_dispatch[] = {
    [BLE_HCI_LE_SUBEV_CONN_COMPLETE] = ble_hs_hci_evt_le_conn_complete,
    [BLE_HCI_LE_SUBEV_ADV_RPT] = ble_hs_hci_evt_le_adv_rpt,
    [BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE] = ble_hs_hci_evt_le_conn_upd_complete,
    [BLE_HCI_LE_SUBEV_LT_KEY_REQ] = ble_hs_hci_evt_le_lt_key_req,
    [BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ] = ble_hs_hci_evt_le_conn_parm_req,
    [BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE] = ble_hs_hci_evt_le_conn_complete,
    [BLE_HCI_LE_SUBEV_DIRECT_ADV_RPT] = ble_hs_hci_evt_le_dir_adv_rpt,
    [BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE] = ble_hs_hci_evt_le_phy_update_complete,
    [BLE_HCI_LE_SUBEV_EXT_ADV_RPT] = ble_hs_hci_evt_le_ext_adv_rpt,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_ESTAB] = ble_hs_hci_evt_le_periodic_adv_sync_estab,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_RPT] = ble_hs_hci_evt_le_periodic_adv_rpt,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_LOST] = ble_hs_hci_evt_le_periodic_adv_sync_lost,
    [BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT] = ble_hs_hci_evt_le_rd_rem_used_feat_complete,
    [BLE_HCI_LE_SUBEV_SCAN_TIMEOUT] = ble_hs_hci_evt_le_scan_timeout,
    [BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED] = ble_hs_hci_evt_le_adv_set_terminated,
    [BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD] = ble_hs_hci_evt_le_scan_req_rcvd,
};

#define BLE_HS_HCI_EVT_LE_DISPATCH_SZ \
    (sizeof ble_hs_hci_evt_le_dispatch / sizeof ble_hs_hci_evt_le_dispatch[0])

static const struct ble_hs_hci_evt_dispatch_entry *
ble_hs_hci_evt_dispatch_find(uint8_t event_code)
{
    const struct ble_hs_hci_evt_dispatch_entry *entry;
    int i;

    for (i = 0; i < BLE_HS_HCI_EVT_DISPATCH_SZ; i++) {
        entry = ble_hs_hci_evt_dispatch + i;
        if (entry->event_code == event_code) {
            return entry;
        }
    }

    return NULL;
}

static ble_hs_hci_evt_le_fn *
ble_hs_hci_evt_le_dispatch_find(uint8_t event_code)
{
    if (event_code >= BLE_HS_HCI_EVT_LE_DISPATCH_SZ) {
        return NULL;
    }

    return ble_hs_hci_evt_le_dispatch[event_code];
}

static int
ble_hs_hci_evt_disconn_complete(uint8_t event_code, uint8_t *data, int len)
{
    struct hci_disconn_complete evt;
    const struct ble_hs_conn *conn;

    if (len < BLE_HCI_EVENT_DISCONN_COMPLETE_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.status = data[2];
    evt.connection_handle = get_le16(data + 3);
    evt.reason = data[5];

    ble_hs_lock();
    conn = ble_hs_conn_find(evt.connection_handle);
    if (conn != NULL) {
        ble_hs_hci_add_avail_pkts(conn->bhc_outstanding_pkts);
    }
    ble_hs_unlock();

    ble_gap_rx_disconn_complete(&evt);

    /* The connection termination may have freed up some capacity in the
     * controller for additional ACL data packets.  Wake up any stalled
     * connections.
     */
    ble_hs_wakeup_tx();

    return 0;
}

static int
ble_hs_hci_evt_encrypt_change(uint8_t event_code, uint8_t *data, int len)
{
    struct hci_encrypt_change evt;

    if (len < BLE_HCI_EVENT_ENCRYPT_CHG_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.status = data[2];
    evt.connection_handle = get_le16(data + 3);
    evt.encryption_enabled = data[5];

    ble_sm_enc_change_rx(&evt);

    return 0;
}

static int
ble_hs_hci_evt_hw_error(uint8_t event_code, uint8_t *data, int len)
{
    uint8_t hw_code;

    if (len < BLE_HCI_EVENT_HW_ERROR_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    hw_code = data[0];
    ble_hs_hw_error(hw_code);

    return 0;
}

static int
ble_hs_hci_evt_enc_key_refresh(uint8_t event_code, uint8_t *data, int len)
{
    struct hci_encrypt_key_refresh evt;

    if (len < BLE_HCI_EVENT_ENC_KEY_REFRESH_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.status = data[2];
    evt.connection_handle = get_le16(data + 3);

    ble_sm_enc_key_refresh_rx(&evt);

    return 0;
}

static int
ble_hs_hci_evt_num_completed_pkts(uint8_t event_code, uint8_t *data,
                                     int len)
{
    struct ble_hs_conn *conn;
    uint16_t num_pkts;
    uint16_t handle;
    uint8_t num_handles;
    int off;
    int i;

    if (len < BLE_HCI_EVENT_HDR_LEN + BLE_HCI_EVENT_NUM_COMP_PKTS_HDR_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    off = BLE_HCI_EVENT_HDR_LEN;
    num_handles = data[off];
    if (len < BLE_HCI_EVENT_NUM_COMP_PKTS_HDR_LEN +
              num_handles * BLE_HCI_EVENT_NUM_COMP_PKTS_ENT_LEN) {
        return BLE_HS_ECONTROLLER;
    }
    off++;

    for (i = 0; i < num_handles; i++) {
        handle = get_le16(data + off);
        num_pkts = get_le16(data + off + 2);
        off += (2 * sizeof(uint16_t));

        if (num_pkts > 0) {
            ble_hs_lock();
            conn = ble_hs_conn_find(handle);
            if (conn != NULL) {
                if (conn->bhc_outstanding_pkts < num_pkts) {
                    ble_hs_sched_reset(BLE_HS_ECONTROLLER);
                } else {
                    conn->bhc_outstanding_pkts -= num_pkts;
                }

                ble_hs_hci_add_avail_pkts(num_pkts);
            }
            ble_hs_unlock();
        }
    }

    /* If any transmissions have stalled, wake them up now. */
    ble_hs_wakeup_tx();

    return 0;
}

static int
ble_hs_hci_evt_le_meta(uint8_t event_code, uint8_t *data, int len)
{
    ble_hs_hci_evt_le_fn *fn;
    uint8_t subevent;
    int rc;

    if (len < BLE_HCI_EVENT_HDR_LEN + BLE_HCI_LE_MIN_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    subevent = data[2];
    fn = ble_hs_hci_evt_le_dispatch_find(subevent);
    if (fn) {
        rc = fn(subevent, data + BLE_HCI_EVENT_HDR_LEN,
                len - BLE_HCI_EVENT_HDR_LEN);
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static struct hci_le_conn_complete pend_conn_complete;
#endif

static int
ble_hs_hci_evt_le_conn_complete(uint8_t subevent, uint8_t *data, int len)
{
    struct hci_le_conn_complete evt;
    int extended_offset = 0;

    if (len < BLE_HCI_LE_CONN_COMPLETE_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    /* this code processes two different events that are really similar */
    if ((subevent == BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE) &&
        ( len < BLE_HCI_LE_ENH_CONN_COMPLETE_LEN)) {
        return BLE_HS_ECONTROLLER;
    }

    memset(&evt, 0, sizeof(evt));

    evt.subevent_code = data[0];
    evt.status = data[1];

    if (evt.status == BLE_ERR_SUCCESS) {
        evt.connection_handle = get_le16(data + 2);
        evt.role = data[4];
        evt.peer_addr_type = data[5];
        memcpy(evt.peer_addr, data + 6, BLE_DEV_ADDR_LEN);

        /* enhanced connection event has the same information with these
         * extra fields stuffed into the middle */
        if (subevent == BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE) {
            memcpy(evt.local_rpa, data + 12, BLE_DEV_ADDR_LEN);
            memcpy(evt.peer_rpa, data + 18, BLE_DEV_ADDR_LEN);
            extended_offset = 12;
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
            /* RPA needs to be resolved here, as controller is not aware of the
             * address is RPA in Host based RPA  */
            if (ble_host_rpa_enabled()) {
                uint8_t *local_id_rpa = ble_hs_get_rpa_local();
                memcpy(evt.local_rpa, local_id_rpa, 6);
            }

            struct ble_hs_resolv_entry *rl = NULL;
            ble_rpa_replace_peer_params_with_rl(evt.peer_addr,
                                                &evt.peer_addr_type, &rl);
#endif
        } else {
            memset(evt.local_rpa, 0, BLE_DEV_ADDR_LEN);
            memset(evt.peer_rpa, 0, BLE_DEV_ADDR_LEN);
        }

        evt.conn_itvl = get_le16(data + 12 + extended_offset);
        evt.conn_latency = get_le16(data + 14 + extended_offset);
        evt.supervision_timeout = get_le16(data + 16 + extended_offset);
        evt.master_clk_acc = data[18 + extended_offset];
    } else {
#if MYNEWT_VAL(BLE_HS_DEBUG)
        evt.connection_handle = BLE_HS_CONN_HANDLE_NONE;
#endif
    }

#if MYNEWT_VAL(BLE_EXT_ADV)
    if (evt.status == BLE_ERR_DIR_ADV_TMO ||
                            evt.role == BLE_HCI_LE_CONN_COMPLETE_ROLE_SLAVE) {
    /* store this until we get set terminated event with adv handle */
        memcpy(&pend_conn_complete, &evt, sizeof(evt));
        return 0;
    }
#endif
    return ble_gap_rx_conn_complete(&evt, 0);
}

static int
ble_hs_hci_evt_le_adv_rpt_first_pass(uint8_t *data, int len)
{
    uint8_t num_reports;
    int off;
    int i;

    if (len < BLE_HCI_LE_ADV_RPT_MIN_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    num_reports = data[1];
    if (num_reports < BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN ||
        num_reports > BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX) {
        return BLE_HS_EBADDATA;
    }

    off = 2; /* Subevent code and num reports. */
    for (i = 0; i < num_reports; i++) {
        /* Move past event type (1), address type (1) and address (6) */
        off += 8;

        /* Add advertising data length (N), length (1) and rssi (1) */
        off += data[off];
        off += 2;

        /* Make sure we are not past length */
        if (off > len) {
            return BLE_HS_ECONTROLLER;
        }
    }

    /* Make sure length was correct */
    if (off != len) {
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}

static int
ble_hs_hci_evt_le_adv_rpt(uint8_t subevent, uint8_t *data, int len)
{
    struct ble_gap_disc_desc desc = {0};
    uint8_t num_reports;
    int off;
    int rc;
    int i;

    /* Validate the event is formatted correctly */
    rc = ble_hs_hci_evt_le_adv_rpt_first_pass(data, len);
    if (rc != 0) {
        return rc;
    }

    desc.direct_addr = *BLE_ADDR_ANY;

    off = 2; /* skip sub-event and num reports */
    num_reports = data[1];
    for (i = 0; i < num_reports; i++) {
        desc.event_type = data[off];
        ++off;

        desc.addr.type = data[off];
        ++off;

        memcpy(desc.addr.val, data + off, 6);
        off += 6;

        desc.length_data = data[off];
        ++off;

        desc.data = data + off;
        off += desc.length_data;

        desc.rssi = data[off];
        ++off;

        ble_gap_rx_adv_report(&desc);
    }

    return 0;
}

static int
ble_hs_hci_evt_le_dir_adv_rpt(uint8_t subevent, uint8_t *data, int len)
{
    struct ble_gap_disc_desc desc = {0};
    uint8_t num_reports;
    int suboff;
    int off;
    int i;

    if (len < BLE_HCI_LE_ADV_DIRECT_RPT_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    num_reports = data[1];
    if (len != 2 + num_reports * BLE_HCI_LE_ADV_DIRECT_RPT_SUB_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    /* Data fields not present in a direct advertising report. */
    desc.data = NULL;
    desc.length_data = 0;

    for (i = 0; i < num_reports; i++) {
        suboff = 0;

        off = 2 + suboff * num_reports + i;
        desc.event_type = data[off];
        suboff++;

        off = 2 + suboff * num_reports + i;
        desc.addr.type = data[off];
        suboff++;

        off = 2 + suboff * num_reports + i * 6;
        memcpy(desc.addr.val, data + off, 6);
        suboff += 6;

        off = 2 + suboff * num_reports + i;
        desc.direct_addr.type = data[off];
        suboff++;

        off = 2 + suboff * num_reports + i * 6;
        memcpy(desc.direct_addr.val, data + off, 6);
        suboff += 6;

        off = 2 + suboff * num_reports + i;
        desc.rssi = data[off];
        suboff++;

        ble_gap_rx_adv_report(&desc);
    }

    return 0;
}

static int
ble_hs_hci_evt_le_rd_rem_used_feat_complete(uint8_t subevent,
                                                 uint8_t *data,
                                                 int len)
{
    struct hci_le_rd_rem_supp_feat_complete evt;

    if (len < BLE_HCI_LE_RD_REM_USED_FEAT_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.status = data[1];
    evt.connection_handle = get_le16(data + 2);
    memcpy(evt.features, data + 4, 8);

    ble_gap_rx_rd_rem_sup_feat_complete(&evt);

    return 0;
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static int
ble_hs_hci_decode_legacy_type(uint16_t evt_type)
{
     switch (evt_type) {
     case BLE_HCI_LEGACY_ADV_EVTYPE_ADV_IND:
         return BLE_HCI_ADV_RPT_EVTYPE_ADV_IND;
     case BLE_HCI_LEGACY_ADV_EVTYPE_ADV_DIRECT_IND:
         return BLE_HCI_ADV_RPT_EVTYPE_DIR_IND;
     case BLE_HCI_LEGACY_ADV_EVTYPE_ADV_SCAN_IND:
         return BLE_HCI_ADV_RPT_EVTYPE_SCAN_IND;
     case BLE_HCI_LEGACY_ADV_EVTYPE_ADV_NONCON_IND:
         return BLE_HCI_ADV_RPT_EVTYPE_NONCONN_IND;
     case BLE_HCI_LEGACY_ADV_EVTYPE_SCAN_RSP_ADV_IND:
         return BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP;
     default:
         return -1;
     }
}
#endif

static int
ble_hs_hci_evt_le_ext_adv_rpt(uint8_t subevent, uint8_t *data, int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    struct ble_gap_ext_disc_desc desc;
    struct hci_ext_adv_report *ext_adv;
    struct hci_ext_adv_report_param *params;
    int num_reports;
    int i;
    int legacy_event_type;

    if (len < sizeof(*ext_adv)) {
        return BLE_HS_EBADDATA;
    }

    ext_adv = (struct hci_ext_adv_report *) data;
    num_reports = ext_adv->num_reports;
    if (num_reports < BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN ||
        num_reports > BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX) {

        return BLE_HS_EBADDATA;
    }

    if (len < (sizeof(*ext_adv) + ext_adv->num_reports * sizeof(*params))) {
        return BLE_HS_ECONTROLLER;
    }

    params = &ext_adv->params[0];
    for (i = 0; i < num_reports; i++) {
        memset(&desc, 0, sizeof(desc));

        desc.props = (params->evt_type) & 0x1F;
        if (desc.props & BLE_HCI_ADV_LEGACY_MASK) {
            legacy_event_type = ble_hs_hci_decode_legacy_type(params->evt_type);
            if (legacy_event_type < 0) {
                params += 1;
                continue;
            }
            desc.legacy_event_type = legacy_event_type;
            desc.data_status = BLE_GAP_EXT_ADV_DATA_STATUS_COMPLETE;
        } else {
            switch(params->evt_type & BLE_HCI_ADV_DATA_STATUS_MASK) {
            case BLE_HCI_ADV_DATA_STATUS_COMPLETE:
                desc.data_status = BLE_GAP_EXT_ADV_DATA_STATUS_COMPLETE;
                break;
            case BLE_HCI_ADV_DATA_STATUS_INCOMPLETE:
                desc.data_status = BLE_GAP_EXT_ADV_DATA_STATUS_INCOMPLETE;
                break;
            case BLE_HCI_ADV_DATA_STATUS_TRUNCATED:
                desc.data_status = BLE_GAP_EXT_ADV_DATA_STATUS_TRUNCATED;
                break;
            default:
                assert(false);
            }
        }
        desc.addr.type = params->addr_type;
        memcpy(desc.addr.val, params->addr, 6);
        desc.length_data = params->adv_data_len;
        desc.data = params->adv_data;
        desc.rssi = params->rssi;
        desc.tx_power = params->tx_power;
        memcpy(desc.direct_addr.val, params->dir_addr, 6);
        desc.direct_addr.type = params->dir_addr_type;
        desc.sid = params->sid;
        desc.prim_phy = params->prim_phy;
        desc.sec_phy = params->sec_phy;
        desc.periodic_adv_itvl = params->per_adv_itvl;
        ble_gap_rx_ext_adv_report(&desc);
        params += 1;
    }
#endif
    return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_sync_estab(uint8_t subevent, uint8_t *data,
                                          int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    struct hci_le_subev_periodic_adv_sync_estab evt;

    if (len < BLE_HCI_LE_PERIODIC_ADV_SYNC_ESTAB_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.status = data[1];
    evt.sync_handle = get_le16(data + 2);
    evt.sid = data[4];
    evt.adv_addr_type = data[5];
    memcpy(evt.adv_addr, &data[6], 6);
    evt.adv_phy = data[12];
    evt.per_adv_ival = get_le16(data + 13);
    evt.adv_clk_accuracy = data[15];

    ble_gap_rx_peroidic_adv_sync_estab(&evt);
#endif

    return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_rpt(uint8_t subevent, uint8_t *data, int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    struct hci_le_subev_periodic_adv_rpt* evt;

    if (len < BLE_HCI_LE_PERIODIC_ADV_RPT_LEN) {
        return BLE_HS_EBADDATA;
    }

    evt = (struct hci_le_subev_periodic_adv_rpt *)(data + 1);
    ble_gap_rx_periodic_adv_rpt(evt);
#endif

return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_sync_lost(uint8_t subevent, uint8_t *data,
                                         int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    struct hci_le_subev_periodic_adv_sync_lost evt;

    if (len < BLE_HCI_LE_PERIODIC_ADV_SYNC_LOST_LEN) {
        return BLE_HS_EBADDATA;
    }

    evt.sync_handle = get_le16(data + 1);

    ble_gap_rx_periodic_adv_sync_lost(&evt);

#endif
    return 0;
}


static int
ble_hs_hci_evt_le_scan_timeout(uint8_t subevent, uint8_t *data, int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
        ble_gap_rx_le_scan_timeout();
#endif
        return 0;
}

static int
ble_hs_hci_evt_le_adv_set_terminated(uint8_t subevent, uint8_t *data, int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    struct hci_le_adv_set_terminated evt;

    if (len < BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.status = data[1];
    evt.adv_handle = data[2];
    evt.conn_handle = get_le16(data + 3);
    evt.completed_events = data[5];

    if (evt.status == 0) {
        /* ignore return code as we need to terminate advertising set anyway */
        ble_gap_rx_conn_complete(&pend_conn_complete, evt.adv_handle);
    }
    ble_gap_rx_adv_set_terminated(&evt);
#endif

    return 0;
}

static int
ble_hs_hci_evt_le_scan_req_rcvd(uint8_t subevent, uint8_t *data, int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    struct hci_le_scan_req_rcvd evt;

    if (len != BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.adv_handle = data[1];
    evt.scan_addr_type = data[2];
    memcpy(evt.scan_addr, data + 3, 6);

    ble_gap_rx_scan_req_rcvd(&evt);
#endif

    return 0;
}

static int
ble_hs_hci_evt_le_conn_upd_complete(uint8_t subevent, uint8_t *data, int len)
{
    struct hci_le_conn_upd_complete evt;

    if (len < BLE_HCI_LE_CONN_UPD_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.status = data[1];
    evt.connection_handle = get_le16(data + 2);
    evt.conn_itvl = get_le16(data + 4);
    evt.conn_latency = get_le16(data + 6);
    evt.supervision_timeout = get_le16(data + 8);

    if (evt.status == 0) {
        if (evt.conn_itvl < BLE_HCI_CONN_ITVL_MIN ||
            evt.conn_itvl > BLE_HCI_CONN_ITVL_MAX) {

            return BLE_HS_EBADDATA;
        }
        if (
#if BLE_HCI_CONN_LATENCY_MIN
	   (evt.conn_latency < BLE_HCI_CONN_LATENCY_MIN) ||
#endif
            evt.conn_latency > BLE_HCI_CONN_LATENCY_MAX) {

            return BLE_HS_EBADDATA;
        }
        if (evt.supervision_timeout < BLE_HCI_CONN_SPVN_TIMEOUT_MIN ||
            evt.supervision_timeout > BLE_HCI_CONN_SPVN_TIMEOUT_MAX) {

            return BLE_HS_EBADDATA;
        }
    }

    ble_gap_rx_update_complete(&evt);

    return 0;
}

static int
ble_hs_hci_evt_le_lt_key_req(uint8_t subevent, uint8_t *data, int len)
{
    struct hci_le_lt_key_req evt;

    if (len < BLE_HCI_LE_LT_KEY_REQ_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.connection_handle = get_le16(data + 1);
    evt.random_number = get_le64(data + 3);
    evt.encrypted_diversifier = get_le16(data + 11);

    ble_sm_ltk_req_rx(&evt);

    return 0;
}

static int
ble_hs_hci_evt_le_conn_parm_req(uint8_t subevent, uint8_t *data, int len)
{
    struct hci_le_conn_param_req evt;

    if (len < BLE_HCI_LE_REM_CONN_PARM_REQ_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.connection_handle = get_le16(data + 1);
    evt.itvl_min = get_le16(data + 3);
    evt.itvl_max = get_le16(data + 5);
    evt.latency = get_le16(data + 7);
    evt.timeout = get_le16(data + 9);

    if (evt.itvl_min < BLE_HCI_CONN_ITVL_MIN ||
        evt.itvl_max > BLE_HCI_CONN_ITVL_MAX ||
        evt.itvl_min > evt.itvl_max) {

        return BLE_HS_EBADDATA;
    }
    if (
#if BLE_HCI_CONN_LATENCY_MIN
	(evt.latency < BLE_HCI_CONN_LATENCY_MIN) ||
#endif
        evt.latency > BLE_HCI_CONN_LATENCY_MAX) {

        return BLE_HS_EBADDATA;
    }
    if (evt.timeout < BLE_HCI_CONN_SPVN_TIMEOUT_MIN ||
        evt.timeout > BLE_HCI_CONN_SPVN_TIMEOUT_MAX) {

        return BLE_HS_EBADDATA;
    }

    ble_gap_rx_param_req(&evt);

    return 0;
}

static int
ble_hs_hci_evt_le_phy_update_complete(uint8_t subevent, uint8_t *data, int len)
{
    struct hci_le_phy_upd_complete evt;

    if (len < BLE_HCI_LE_PHY_UPD_LEN) {
        return BLE_HS_ECONTROLLER;
    }

    evt.subevent_code = data[0];
    evt.status = data[1];
    evt.connection_handle = get_le16(data + 2);
    evt.tx_phy = data[4];
    evt.rx_phy = data[5];

    ble_gap_rx_phy_update_complete(&evt);

    return 0;
}

int
ble_hs_hci_evt_process(uint8_t *data)
{
    const struct ble_hs_hci_evt_dispatch_entry *entry;
    uint8_t event_code;
    uint8_t param_len;
    int event_len;
    int rc;

    /* Count events received */
    STATS_INC(ble_hs_stats, hci_event);

    /* Display to console */
    ble_hs_dbg_event_disp(data);

    /* Process the event */
    event_code = data[0];
    param_len = data[1];

    event_len = param_len + 2;

    entry = ble_hs_hci_evt_dispatch_find(event_code);
    if (entry == NULL) {
        STATS_INC(ble_hs_stats, hci_unknown_event);
        rc = BLE_HS_ENOTSUP;
    } else {
        rc = entry->cb(event_code, data, event_len);
    }

    ble_hci_trans_buf_free(data);

    return rc;
}

/**
 * Called when a data packet is received from the controller.  This function
 * consumes the supplied mbuf, regardless of the outcome.
 *
 * @param om                    The incoming data packet, beginning with the
 *                                  HCI ACL data header.
 *
 * @return                      0 on success; nonzero on failure.
 */
int
ble_hs_hci_evt_acl_process(struct os_mbuf *om)
{
    struct hci_data_hdr hci_hdr;
    struct ble_hs_conn *conn;
    ble_l2cap_rx_fn *rx_cb;
    uint16_t conn_handle;
    int reject_cid;
    int rc;

    rc = ble_hs_hci_util_data_hdr_strip(om, &hci_hdr);
    if (rc != 0) {
        goto err;
    }

#if (BLETEST_THROUGHPUT_TEST == 0)
#if !BLE_MONITOR
    BLE_HS_LOG(DEBUG, "ble_hs_hci_evt_acl_process(): conn_handle=%u pb=%x "
                      "len=%u data=",
               BLE_HCI_DATA_HANDLE(hci_hdr.hdh_handle_pb_bc),
               BLE_HCI_DATA_PB(hci_hdr.hdh_handle_pb_bc),
               hci_hdr.hdh_len);
    ble_hs_log_mbuf(om);
    BLE_HS_LOG(DEBUG, "\n");
#endif
#endif

    if (hci_hdr.hdh_len != OS_MBUF_PKTHDR(om)->omp_len) {
        rc = BLE_HS_EBADDATA;
        goto err;
    }

    conn_handle = BLE_HCI_DATA_HANDLE(hci_hdr.hdh_handle_pb_bc);

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        /* Peer not connected; quietly discard packet. */
        rc = BLE_HS_ENOTCONN;
        reject_cid = -1;
    } else {
        /* Forward ACL data to L2CAP. */
        rc = ble_l2cap_rx(conn, &hci_hdr, om, &rx_cb, &reject_cid);
        om = NULL;
    }

    ble_hs_unlock();

    switch (rc) {
    case 0:
        /* Final fragment received. */
        BLE_HS_DBG_ASSERT(rx_cb != NULL);
        rc = rx_cb(conn->bhc_rx_chan);
        ble_l2cap_remove_rx(conn, conn->bhc_rx_chan);
        break;

    case BLE_HS_EAGAIN:
        /* More fragments on the way. */
        break;

    default:
        if (reject_cid != -1) {
            ble_l2cap_sig_reject_invalid_cid_tx(conn_handle, 0, 0, reject_cid);
        }
        goto err;
    }

    return 0;

err:
    os_mbuf_free_chain(om);
    return rc;
}
