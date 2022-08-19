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
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/include/nimble/hci_common.h"
#include "nimble/nimble/include/nimble/ble_hci_trans.h"
#include "nimble/nimble/host/include/host/ble_gap.h"
#include "nimble/nimble/host/include/host/ble_monitor.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"

#if CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT
static uint16_t reattempt_conn[MYNEWT_VAL(BLE_MAX_CONNECTIONS)];
extern int ble_gap_master_connect_reattempt(uint16_t conn_handle);

#ifdef CONFIG_BT_NIMBLE_MAX_CONN_REATTEMPT
#define MAX_REATTEMPT_ALLOWED CONFIG_BT_NIMBLE_MAX_CONN_REATTEMPT
#else
#define MAX_REATTEMPT_ALLOWED 0
#endif
#endif

_Static_assert(sizeof (struct hci_data_hdr) == BLE_HCI_DATA_HDR_SZ,
               "struct hci_data_hdr must be 4 bytes");

typedef int ble_hs_hci_evt_fn(uint8_t event_code, const void *data,
                              unsigned int len);
static ble_hs_hci_evt_fn ble_hs_hci_evt_hw_error;
static ble_hs_hci_evt_fn ble_hs_hci_evt_num_completed_pkts;
#if NIMBLE_BLE_CONNECT
static ble_hs_hci_evt_fn ble_hs_hci_evt_disconn_complete;
static ble_hs_hci_evt_fn ble_hs_hci_evt_encrypt_change;
static ble_hs_hci_evt_fn ble_hs_hci_evt_enc_key_refresh;
#endif
static ble_hs_hci_evt_fn ble_hs_hci_evt_le_meta;

typedef int ble_hs_hci_evt_le_fn(uint8_t subevent, const void *data,
                                 unsigned int len);
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_adv_rpt;
#if NIMBLE_BLE_CONNECT
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_upd_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_lt_key_req;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_conn_parm_req;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_phy_update_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_enh_conn_complete;
#endif
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_dir_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_ext_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_rd_rem_used_feat_complete;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_scan_timeout;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_adv_set_terminated;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_sync_estab;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_rpt;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_sync_lost;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_scan_req_rcvd;
static ble_hs_hci_evt_le_fn ble_hs_hci_evt_le_periodic_adv_sync_transfer;

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
#if NIMBLE_BLE_CONNECT
    { BLE_HCI_EVCODE_DISCONN_CMP, ble_hs_hci_evt_disconn_complete },
    { BLE_HCI_EVCODE_ENCRYPT_CHG, ble_hs_hci_evt_encrypt_change },
    { BLE_HCI_EVCODE_ENC_KEY_REFRESH, ble_hs_hci_evt_enc_key_refresh },
#endif
    { BLE_HCI_EVCODE_HW_ERROR, ble_hs_hci_evt_hw_error },
};

#define BLE_HS_HCI_EVT_DISPATCH_SZ \
    (sizeof ble_hs_hci_evt_dispatch / sizeof ble_hs_hci_evt_dispatch[0])

static ble_hs_hci_evt_le_fn * const ble_hs_hci_evt_le_dispatch[] = {
#if NIMBLE_BLE_CONNECT
    [BLE_HCI_LE_SUBEV_CONN_COMPLETE] = ble_hs_hci_evt_le_conn_complete,
#endif
    [BLE_HCI_LE_SUBEV_ADV_RPT] = ble_hs_hci_evt_le_adv_rpt,
#if NIMBLE_BLE_CONNECT
    [BLE_HCI_LE_SUBEV_CONN_UPD_COMPLETE] = ble_hs_hci_evt_le_conn_upd_complete,
    [BLE_HCI_LE_SUBEV_LT_KEY_REQ] = ble_hs_hci_evt_le_lt_key_req,
    [BLE_HCI_LE_SUBEV_REM_CONN_PARM_REQ] = ble_hs_hci_evt_le_conn_parm_req,
    [BLE_HCI_LE_SUBEV_ENH_CONN_COMPLETE] = ble_hs_hci_evt_le_enh_conn_complete,
#endif
    [BLE_HCI_LE_SUBEV_DIRECT_ADV_RPT] = ble_hs_hci_evt_le_dir_adv_rpt,
#if NIMBLE_BLE_CONNECT
    [BLE_HCI_LE_SUBEV_PHY_UPDATE_COMPLETE] = ble_hs_hci_evt_le_phy_update_complete,
#endif
    [BLE_HCI_LE_SUBEV_EXT_ADV_RPT] = ble_hs_hci_evt_le_ext_adv_rpt,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_ESTAB] = ble_hs_hci_evt_le_periodic_adv_sync_estab,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_RPT] = ble_hs_hci_evt_le_periodic_adv_rpt,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_LOST] = ble_hs_hci_evt_le_periodic_adv_sync_lost,
    [BLE_HCI_LE_SUBEV_RD_REM_USED_FEAT] = ble_hs_hci_evt_le_rd_rem_used_feat_complete,
    [BLE_HCI_LE_SUBEV_SCAN_TIMEOUT] = ble_hs_hci_evt_le_scan_timeout,
    [BLE_HCI_LE_SUBEV_ADV_SET_TERMINATED] = ble_hs_hci_evt_le_adv_set_terminated,
    [BLE_HCI_LE_SUBEV_SCAN_REQ_RCVD] = ble_hs_hci_evt_le_scan_req_rcvd,
    [BLE_HCI_LE_SUBEV_PERIODIC_ADV_SYNC_TRANSFER] = ble_hs_hci_evt_le_periodic_adv_sync_transfer,
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

#if NIMBLE_BLE_CONNECT
static int
ble_hs_hci_evt_disconn_complete(uint8_t event_code, const void *data,
                                unsigned int len)
{
    const struct ble_hci_ev_disconn_cmp *ev = data;
    const struct ble_hs_conn *conn;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_hs_lock();
    conn = ble_hs_conn_find(le16toh(ev->conn_handle));
    if (conn != NULL) {
        ble_hs_hci_add_avail_pkts(conn->bhc_outstanding_pkts);
    }
    ble_hs_unlock();

#if CONFIG_BT_NIMBLE_ENABLE_CONN_REATTEMPT
    int rc;

    if (ev->reason == BLE_ERR_CONN_ESTABLISHMENT && (conn != NULL)) {
        BLE_HS_LOG(DEBUG, "Reattempt connection; reason = 0x%x, status = %d,"
                          " reattempt count = %d ", ev->reason, ev->status,
                           reattempt_conn[ev->conn_handle]);
        if (conn->bhc_flags & BLE_HS_CONN_F_MASTER) {
            if (reattempt_conn[ev->conn_handle] < MAX_REATTEMPT_ALLOWED) {
                reattempt_conn[ev->conn_handle] += 1;
                rc = ble_gap_master_connect_reattempt(ev->conn_handle);
                if (rc != 0) {
                    BLE_HS_LOG(DEBUG, "Master reconnect attempt failed; rc = %d", rc);
                }
            } else {
                reattempt_conn[ev->conn_handle] = 0;
            }
        }
    } else {
        /* Disconnect completed with some other reason than
         * BLE_ERR_CONN_ESTABLISHMENT, reset the corresponding reattempt count
         * */
        reattempt_conn[ev->conn_handle] = 0;
    }
#endif

    ble_gap_rx_disconn_complete(ev);

    /* The connection termination may have freed up some capacity in the
     * controller for additional ACL data packets.  Wake up any stalled
     * connections.
     */
    ble_hs_wakeup_tx();

    return 0;
}

static int
ble_hs_hci_evt_encrypt_change(uint8_t event_code, const void *data,
                              unsigned int len)
{
    const struct ble_hci_ev_enrypt_chg *ev = data;

    if (len != sizeof (*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_sm_enc_change_rx(ev);

    return 0;
}
#endif
static int
ble_hs_hci_evt_hw_error(uint8_t event_code, const void *data, unsigned int len)
{
    const struct ble_hci_ev_hw_error *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_hs_hw_error(ev->hw_code);

    return 0;
}

#if NIMBLE_BLE_CONNECT
static int
ble_hs_hci_evt_enc_key_refresh(uint8_t event_code, const void *data,
                               unsigned int len)
{
    const struct ble_hci_ev_enc_key_refresh *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_sm_enc_key_refresh_rx(ev);

    return 0;
}
#endif

static int
ble_hs_hci_evt_num_completed_pkts(uint8_t event_code, const void *data,
                                  unsigned int len)
{
    const struct ble_hci_ev_num_comp_pkts *ev = data;
    struct ble_hs_conn *conn;
    uint16_t num_pkts;
    int i;

    if (len != sizeof(*ev) + (ev->count * sizeof(ev->completed[0]))) {
        return BLE_HS_ECONTROLLER;
    }

    for (i = 0; i < ev->count; i++) {
        num_pkts = le16toh(ev->completed[i].packets);

        if (num_pkts > 0) {
            ble_hs_lock();
            conn = ble_hs_conn_find(le16toh(ev->completed[i].handle));
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
ble_hs_hci_evt_le_meta(uint8_t event_code, const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_meta *ev = data;
    ble_hs_hci_evt_le_fn *fn;

    if (len < sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    fn = ble_hs_hci_evt_le_dispatch_find(ev->subevent);
    if (fn) {
        return fn(ev->subevent, data, len);
    }

    return 0;
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static struct ble_gap_conn_complete pend_conn_complete;
#endif

#if NIMBLE_BLE_CONNECT
static int
ble_hs_hci_evt_le_enh_conn_complete(uint8_t subevent, const void *data,
                                    unsigned int len)
{
    const struct ble_hci_ev_le_subev_enh_conn_complete *ev = data;
    struct ble_gap_conn_complete evt;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    memset(&evt, 0, sizeof(evt));

    evt.status = ev->status;

    if (evt.status == BLE_ERR_SUCCESS) {
        evt.connection_handle = le16toh(ev->conn_handle);
        evt.role = ev->role;
        evt.peer_addr_type = ev->peer_addr_type;
        memcpy(evt.peer_addr, ev->peer_addr, BLE_DEV_ADDR_LEN);
        memcpy(evt.local_rpa, ev->local_rpa, BLE_DEV_ADDR_LEN);
        memcpy(evt.peer_rpa,ev->peer_rpa, BLE_DEV_ADDR_LEN);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        /* RPA needs to be resolved here, as controller is not aware of the
         * address is RPA in Host based RPA  */
        if (ble_host_rpa_enabled()) {
            uint8_t *local_id_rpa = ble_hs_get_rpa_local();
            memcpy(evt.local_rpa, local_id_rpa, BLE_DEV_ADDR_LEN);
        }

        struct ble_hs_resolv_entry *rl = NULL;
        ble_rpa_replace_peer_params_with_rl(evt.peer_addr,
                                            &evt.peer_addr_type, &rl);
#endif
        evt.conn_itvl = le16toh(ev->conn_itvl);
        evt.conn_latency = le16toh(ev->conn_latency);
        evt.supervision_timeout = le16toh(ev->supervision_timeout);
        evt.master_clk_acc = ev->mca;
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
ble_hs_hci_evt_le_conn_complete(uint8_t subevent, const void *data,
                                unsigned int len)
{
    const struct ble_hci_ev_le_subev_conn_complete *ev = data;
    struct ble_gap_conn_complete evt;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    memset(&evt, 0, sizeof(evt));

    evt.status = ev->status;

    if (evt.status == BLE_ERR_SUCCESS) {
        evt.connection_handle = le16toh(ev->conn_handle);
        evt.role = ev->role;
        evt.peer_addr_type = ev->peer_addr_type;
        memcpy(evt.peer_addr, ev->peer_addr, BLE_DEV_ADDR_LEN);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        /* RPA needs to be resolved here, as controller is not aware of the
         * address is RPA in Host based RPA  */
        if (ble_host_rpa_enabled()) {
            uint8_t *local_id_rpa = ble_hs_get_rpa_local();
            memcpy(evt.local_rpa, local_id_rpa, BLE_DEV_ADDR_LEN);
        }

        struct ble_hs_resolv_entry *rl = NULL;
        ble_rpa_replace_peer_params_with_rl(evt.peer_addr,
                                            &evt.peer_addr_type, &rl);
#endif
        evt.conn_itvl = le16toh(ev->conn_itvl);
        evt.conn_latency = le16toh(ev->conn_latency);
        evt.supervision_timeout = le16toh(ev->supervision_timeout);
        evt.master_clk_acc = ev->mca;
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
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
static int
ble_hs_hci_evt_le_adv_rpt_first_pass(const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_subev_adv_rpt *ev = data;
    const struct adv_report *rpt;
    int i;

    if (len < sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    len -= sizeof(*ev);
    data += sizeof(*ev);

    if (ev->num_reports < BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN ||
        ev->num_reports > BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX) {
        return BLE_HS_EBADDATA;
    }

    for (i = 0; i < ev->num_reports; i++) {
        /* extra byte for RSSI after adv data */
        if (len < sizeof(*rpt) + 1) {
            return BLE_HS_ECONTROLLER;
        }

        rpt = data;

        len -= sizeof(*rpt) + 1;
        data += sizeof(rpt) + 1;

        if (rpt->data_len > len) {
            return BLE_HS_ECONTROLLER;
        }

        len -= rpt->data_len;
        data += rpt->data_len;
    }

    /* Make sure length was correct */
    if (len) {
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}

static int
ble_hs_hci_evt_le_adv_rpt(uint8_t subevent, const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_subev_adv_rpt *ev = data;
    struct ble_gap_disc_desc desc = {0};
    const struct adv_report *rpt;
    int rc;
    int i;

    /* Validate the event is formatted correctly */
    rc = ble_hs_hci_evt_le_adv_rpt_first_pass(data, len);
    if (rc != 0) {
        return rc;
    }

    data += sizeof(*ev);

    desc.direct_addr = *BLE_ADDR_ANY;

    for (i = 0; i < ev->num_reports; i++) {
        rpt = data;

        data += sizeof(rpt) + rpt->data_len + 1;

        desc.event_type = rpt->type;
        desc.addr.type = rpt->addr_type;
        memcpy(desc.addr.val, rpt->addr, BLE_DEV_ADDR_LEN);
        desc.length_data = rpt->data_len;
        desc.data = rpt->data;
        desc.rssi = rpt->data[rpt->data_len];

        ble_gap_rx_adv_report(&desc);
    }

    return 0;
}
#pragma GCC diagnostic pop

static int
ble_hs_hci_evt_le_dir_adv_rpt(uint8_t subevent, const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_subev_direct_adv_rpt *ev = data;
    struct ble_gap_disc_desc desc = {0};
    int i;

    if (len < sizeof(*ev) || len != ev->num_reports * sizeof(ev->reports[0])) {
        return BLE_HS_ECONTROLLER;
    }

    /* Data fields not present in a direct advertising report. */
    desc.data = NULL;
    desc.length_data = 0;

    for (i = 0; i < ev->num_reports; i++) {
        desc.event_type = ev->reports[i].type;
        desc.addr.type = ev->reports[i].addr_type;
        memcpy(desc.addr.val, ev->reports[i].addr, BLE_DEV_ADDR_LEN);
        desc.direct_addr.type = ev->reports[i].dir_addr_type;
        memcpy(desc.direct_addr.val, ev->reports[i].dir_addr, BLE_DEV_ADDR_LEN);
        desc.rssi = ev->reports[i].rssi;

        ble_gap_rx_adv_report(&desc);
    }

    return 0;
}

static int
ble_hs_hci_evt_le_rd_rem_used_feat_complete(uint8_t subevent, const void *data,
                                            unsigned int len)
{
    const struct ble_hci_ev_le_subev_rd_rem_used_feat *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_gap_rx_rd_rem_sup_feat_complete(ev);

    return 0;
}

#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
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
    case BLE_HCI_LEGACY_ADV_EVTYPE_SCAN_RSP_ADV_SCAN_IND:
        return BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP;
    default:
        return -1;
    }
}
#endif

static int
ble_hs_hci_evt_le_ext_adv_rpt(uint8_t subevent, const void *data,
                              unsigned int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
    const struct ble_hci_ev_le_subev_ext_adv_rpt *ev = data;
    const struct ext_adv_report *report;
    struct ble_gap_ext_disc_desc desc;
    int i;
    int legacy_event_type;

    if (len < sizeof(*ev)) {
        return BLE_HS_EBADDATA;
    }

    if (ev->num_reports < BLE_HCI_LE_ADV_RPT_NUM_RPTS_MIN ||
        ev->num_reports > BLE_HCI_LE_ADV_RPT_NUM_RPTS_MAX) {
        return BLE_HS_EBADDATA;
    }

    /* TODO properly validate len of the event */

    report = &ev->reports[0];
    for (i = 0; i < ev->num_reports; i++) {
        memset(&desc, 0, sizeof(desc));

        desc.props = (report->evt_type) & 0x1F;
        if (desc.props & BLE_HCI_ADV_LEGACY_MASK) {
            legacy_event_type = ble_hs_hci_decode_legacy_type(report->evt_type);
            if (legacy_event_type < 0) {
                report = (const void *) &report->data[report->data_len];
                continue;
            }
            desc.legacy_event_type = legacy_event_type;
            desc.data_status = BLE_GAP_EXT_ADV_DATA_STATUS_COMPLETE;
        } else {
            switch(report->evt_type & BLE_HCI_ADV_DATA_STATUS_MASK) {
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
        desc.addr.type = report->addr_type;
        memcpy(desc.addr.val, report->addr, 6);
        desc.length_data = report->data_len;
        desc.data = report->data;
        desc.rssi = report->rssi;
        desc.tx_power = report->tx_power;
        memcpy(desc.direct_addr.val, report->dir_addr, 6);
        desc.direct_addr.type = report->dir_addr_type;
        desc.sid = report->sid;
        desc.prim_phy = report->pri_phy;
        desc.sec_phy = report->sec_phy;
        desc.periodic_adv_itvl = report->periodic_itvl;

        ble_gap_rx_ext_adv_report(&desc);

        report = (const void *) &report->data[report->data_len];
    }
#endif
    return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_sync_estab(uint8_t subevent, const void *data,
                                          unsigned int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    const struct ble_hci_ev_le_subev_periodic_adv_sync_estab *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_gap_rx_peroidic_adv_sync_estab(ev);
#endif

    return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_rpt(uint8_t subevent, const void *data,
                                   unsigned int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    const struct ble_hci_ev_le_subev_periodic_adv_rpt *ev = data;

    if (len < sizeof(*ev) || len != (sizeof(*ev) + ev->data_len)) {
        return BLE_HS_EBADDATA;
    }

    ble_gap_rx_periodic_adv_rpt(ev);
#endif

return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_sync_lost(uint8_t subevent, const void *data,
                                         unsigned int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    const struct ble_hci_ev_le_subev_periodic_adv_sync_lost *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_EBADDATA;
    }

    ble_gap_rx_periodic_adv_sync_lost(ev);

#endif
    return 0;
}

static int
ble_hs_hci_evt_le_periodic_adv_sync_transfer(uint8_t subevent, const void *data,
                                             unsigned int len)
{
#if MYNEWT_VAL(BLE_PERIODIC_ADV_SYNC_TRANSFER)
    const struct ble_hci_ev_le_subev_periodic_adv_sync_transfer *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_EBADDATA;
    }

    ble_gap_rx_periodic_adv_sync_transfer(ev);

#endif
    return 0;
}

static int
ble_hs_hci_evt_le_scan_timeout(uint8_t subevent, const void *data,
                               unsigned int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
    const struct ble_hci_ev_le_subev_scan_timeout *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_EBADDATA;
    }

    ble_gap_rx_le_scan_timeout();
#endif
    return 0;
}

static int
ble_hs_hci_evt_le_adv_set_terminated(uint8_t subevent, const void *data,
                                     unsigned int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    const struct ble_hci_ev_le_subev_adv_set_terminated *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    if (ev->status == 0) {
        /* ignore return code as we need to terminate advertising set anyway */
        ble_gap_rx_conn_complete(&pend_conn_complete, ev->adv_handle);
    }
    ble_gap_rx_adv_set_terminated(ev);
#endif

    return 0;
}

static int
ble_hs_hci_evt_le_scan_req_rcvd(uint8_t subevent, const void *data,
                                unsigned int len)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    const struct ble_hci_ev_le_subev_scan_req_rcvd *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_gap_rx_scan_req_rcvd(ev);
#endif

    return 0;
}

#if NIMBLE_BLE_CONNECT
static int
ble_hs_hci_evt_le_conn_upd_complete(uint8_t subevent, const void *data,
                                    unsigned int len)
{
    const struct ble_hci_ev_le_subev_conn_upd_complete *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    if (ev->status == 0) {
        BLE_HS_DBG_ASSERT(le16toh(ev->conn_itvl) >= BLE_HCI_CONN_ITVL_MIN);
        BLE_HS_DBG_ASSERT(le16toh(ev->conn_itvl) <= BLE_HCI_CONN_ITVL_MAX);

        BLE_HS_DBG_ASSERT(le16toh(ev->conn_latency) >= BLE_HCI_CONN_LATENCY_MIN);
        BLE_HS_DBG_ASSERT(le16toh(ev->conn_latency) <= BLE_HCI_CONN_LATENCY_MAX);

        BLE_HS_DBG_ASSERT(le16toh(ev->supervision_timeout) >= BLE_HCI_CONN_SPVN_TIMEOUT_MIN);
        BLE_HS_DBG_ASSERT(le16toh(ev->supervision_timeout) <= BLE_HCI_CONN_SPVN_TIMEOUT_MAX);
    }

    ble_gap_rx_update_complete(ev);

    return 0;
}

static int
ble_hs_hci_evt_le_lt_key_req(uint8_t subevent, const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_subev_lt_key_req *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_sm_ltk_req_rx(ev);

    return 0;
}

static int
ble_hs_hci_evt_le_conn_parm_req(uint8_t subevent, const void *data, unsigned int len)
{
    const struct ble_hci_ev_le_subev_rem_conn_param_req *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    BLE_HS_DBG_ASSERT(le16toh(ev->min_interval) >= BLE_HCI_CONN_ITVL_MIN);
    BLE_HS_DBG_ASSERT(le16toh(ev->max_interval) <= BLE_HCI_CONN_ITVL_MAX);
    BLE_HS_DBG_ASSERT(le16toh(ev->max_interval) >= le16toh(ev->min_interval));

    BLE_HS_DBG_ASSERT(le16toh(ev->latency) >= BLE_HCI_CONN_LATENCY_MIN);
    BLE_HS_DBG_ASSERT(le16toh(ev->latency) <= BLE_HCI_CONN_LATENCY_MAX);

    BLE_HS_DBG_ASSERT(le16toh(ev->timeout) >= BLE_HCI_CONN_SPVN_TIMEOUT_MIN);
    BLE_HS_DBG_ASSERT(le16toh(ev->timeout) <= BLE_HCI_CONN_SPVN_TIMEOUT_MAX);

    ble_gap_rx_param_req(ev);

    return 0;
}

static int
ble_hs_hci_evt_le_phy_update_complete(uint8_t subevent, const void *data,
                                      unsigned int len)
{
    const struct ble_hci_ev_le_subev_phy_update_complete *ev = data;

    if (len != sizeof(*ev)) {
        return BLE_HS_ECONTROLLER;
    }

    ble_gap_rx_phy_update_complete(ev);

    return 0;
}
#endif

int
ble_hs_hci_evt_process(const struct ble_hci_ev *ev)
{
    const struct ble_hs_hci_evt_dispatch_entry *entry;
    int rc;

    /* Count events received */
    STATS_INC(ble_hs_stats, hci_event);


    entry = ble_hs_hci_evt_dispatch_find(ev->opcode);
    if (entry == NULL) {
        STATS_INC(ble_hs_stats, hci_unknown_event);
        rc = BLE_HS_ENOTSUP;
    } else {
        rc = entry->cb(ev->opcode, ev->data, ev->length);
    }

    ble_hci_trans_buf_free((uint8_t *) ev);

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
#if NIMBLE_BLE_CONNECT
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
#else
    return BLE_HS_ENOTSUP;
#endif
}
