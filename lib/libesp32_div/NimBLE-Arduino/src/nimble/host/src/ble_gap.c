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

#include <assert.h>
#include <string.h>
#include <errno.h>
#include "nimble/nimble_opt.h"
#include "host/ble_hs_adv.h"
#include "host/ble_hs_hci.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"

#if MYNEWT
#include "bsp/bsp.h"
#else
#define bssnz_t
#endif

/**
 * GAP - Generic Access Profile.
 *
 * Design overview:
 *
 * GAP procedures are initiated by the application via function calls.  Such
 * functions return when either of the following happens:
 *
 * (1) The procedure completes (success or failure).
 * (2) The procedure cannot proceed until a BLE peer responds.
 *
 * For (1), the result of the procedure if fully indicated by the function
 * return code.
 * For (2), the procedure result is indicated by an application-configured
 * callback.  The callback is executed when the procedure completes.
 *
 * The GAP is always in one of two states:
 * 1. Free
 * 2. Preempted
 *
 * While GAP is in the free state, new procedures can be started at will.
 * While GAP is in the preempted state, no new procedures are allowed.  The
 * host sets GAP to the preempted state when it needs to ensure no ongoing
 * procedures, a condition required for some HCI commands to succeed.  The host
 * must take care to take GAP out of the preempted state as soon as possible.
 *
 * Notes on thread-safety:
 * 1. The ble_hs mutex must always be unlocked when an application callback is
 *    executed.  The purpose of this requirement is to allow callbacks to
 *    initiate additional host procedures, which may require locking of the
 *    mutex.
 * 2. Functions called directly by the application never call callbacks.
 *    Generally, these functions lock the ble_hs mutex at the start, and only
 *    unlock it at return.
 * 3. Functions which do call callbacks (receive handlers and timer
 *    expirations) generally only lock the mutex long enough to modify
 *    affected state and make copies of data needed for the callback.  A copy
 *    of various pieces of data is called a "snapshot" (struct
 *    ble_gap_snapshot).  The sole purpose of snapshots is to allow callbacks
 *    to be executed after unlocking the mutex.
 */

/** GAP procedure op codes. */
#define BLE_GAP_OP_NULL             0
#define BLE_GAP_OP_M_DISC           1
#define BLE_GAP_OP_M_CONN           2
#define BLE_GAP_OP_S_ADV            1
#define BLE_GAP_OP_S_PERIODIC_ADV   2
#define BLE_GAP_OP_SYNC             1

/**
 * If an attempt to cancel an active procedure fails, the attempt is retried
 * at this rate (ms).
 */
#define BLE_GAP_CANCEL_RETRY_TIMEOUT_MS         100 /* ms */

#define BLE_GAP_UPDATE_TIMEOUT_MS               40000 /* ms */

static const struct ble_gap_conn_params ble_gap_conn_params_dflt = {
    .scan_itvl = 0x0010,
    .scan_window = 0x0010,
    .itvl_min = BLE_GAP_INITIAL_CONN_ITVL_MIN,
    .itvl_max = BLE_GAP_INITIAL_CONN_ITVL_MAX,
    .latency = BLE_GAP_INITIAL_CONN_LATENCY,
    .supervision_timeout = BLE_GAP_INITIAL_SUPERVISION_TIMEOUT,
    .min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN,
    .max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN,
};

/**
 * The state of the in-progress master connection.  If no master connection is
 * currently in progress, then the op field is set to BLE_GAP_OP_NULL.
 */
struct ble_gap_master_state {
    uint8_t op;

    uint8_t exp_set:1;
    ble_npl_time_t exp_os_ticks;

    ble_gap_event_fn *cb;
    void *cb_arg;

    /**
     * Indicates the type of master procedure that was preempted, or
     * BLE_GAP_OP_NULL if no procedure was preempted.
     */
    uint8_t preempted_op;

    union {
        struct {
            uint8_t using_wl:1;
            uint8_t our_addr_type:2;
            uint8_t cancel:1;
        } conn;

        struct {
            uint8_t limited:1;
        } disc;
    };
};
static bssnz_t struct ble_gap_master_state ble_gap_master;

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
/**
 * The state of the in-progress sync creation. If no sync creation connection is
 * currently in progress, then the op field is set to BLE_GAP_OP_NULL.
 */
struct ble_gap_sync_state {
    uint8_t op;
    struct ble_hs_periodic_sync *psync;

    ble_gap_event_fn *cb;
    void *cb_arg;
};

static bssnz_t struct ble_gap_sync_state ble_gap_sync;
#endif

/**
 * The state of the in-progress slave connection.  If no slave connection is
 * currently in progress, then the op field is set to BLE_GAP_OP_NULL.
 */
struct ble_gap_slave_state {
    uint8_t op;

    unsigned int our_addr_type:2;
    unsigned int preempted:1;  /** Set to 1 if advertising was preempted. */
    unsigned int connectable:1;

#if MYNEWT_VAL(BLE_EXT_ADV)
    unsigned int configured:1; /** If instance is configured */
    unsigned int scannable:1;
    unsigned int directed:1;
    unsigned int legacy_pdu:1;
    unsigned int rnd_addr_set:1;
#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    unsigned int periodic_configured:1;
    uint8_t      periodic_op;
#endif
    uint8_t rnd_addr[6];
#else
/* timer is used only with legacy advertising */
    unsigned int exp_set:1;
    ble_npl_time_t exp_os_ticks;
#endif

    ble_gap_event_fn *cb;
    void *cb_arg;
};

static bssnz_t struct ble_gap_slave_state ble_gap_slave[BLE_ADV_INSTANCES];

struct ble_gap_update_entry {
    SLIST_ENTRY(ble_gap_update_entry) next;
    struct ble_gap_upd_params params;
    ble_npl_time_t exp_os_ticks;
    uint16_t conn_handle;
};
SLIST_HEAD(ble_gap_update_entry_list, ble_gap_update_entry);

struct ble_gap_snapshot {
    struct ble_gap_conn_desc *desc;
    ble_gap_event_fn *cb;
    void *cb_arg;
};

static SLIST_HEAD(ble_gap_hook_list, ble_gap_event_listener) ble_gap_event_listener_list;
static os_membuf_t ble_gap_update_entry_mem[
                        OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_GAP_MAX_PENDING_CONN_PARAM_UPDATE),
                                        sizeof (struct ble_gap_update_entry))];
static struct os_mempool ble_gap_update_entry_pool;
static struct ble_gap_update_entry_list ble_gap_update_entries;

static void ble_gap_update_entry_free(struct ble_gap_update_entry *entry);
static struct ble_gap_update_entry *
ble_gap_update_entry_find(uint16_t conn_handle,
                          struct ble_gap_update_entry **out_prev);
static struct ble_gap_update_entry *
ble_gap_update_entry_remove(uint16_t conn_handle);
static void
ble_gap_update_l2cap_cb(uint16_t conn_handle, int status, void *arg);

static int ble_gap_adv_enable_tx(int enable);
static int ble_gap_conn_cancel_tx(void);

#if NIMBLE_BLE_SCAN && !MYNEWT_VAL(BLE_EXT_ADV)
static int ble_gap_disc_enable_tx(int enable, int filter_duplicates);
#endif

STATS_SECT_DECL(ble_gap_stats) ble_gap_stats;
STATS_NAME_START(ble_gap_stats)
    STATS_NAME(ble_gap_stats, wl_set)
    STATS_NAME(ble_gap_stats, wl_set_fail)
    STATS_NAME(ble_gap_stats, adv_stop)
    STATS_NAME(ble_gap_stats, adv_stop_fail)
    STATS_NAME(ble_gap_stats, adv_start)
    STATS_NAME(ble_gap_stats, adv_start_fail)
    STATS_NAME(ble_gap_stats, adv_set_data)
    STATS_NAME(ble_gap_stats, adv_set_data_fail)
    STATS_NAME(ble_gap_stats, adv_rsp_set_data)
    STATS_NAME(ble_gap_stats, adv_rsp_set_data_fail)
    STATS_NAME(ble_gap_stats, discover)
    STATS_NAME(ble_gap_stats, discover_fail)
    STATS_NAME(ble_gap_stats, initiate)
    STATS_NAME(ble_gap_stats, initiate_fail)
    STATS_NAME(ble_gap_stats, terminate)
    STATS_NAME(ble_gap_stats, terminate_fail)
    STATS_NAME(ble_gap_stats, cancel)
    STATS_NAME(ble_gap_stats, cancel_fail)
    STATS_NAME(ble_gap_stats, update)
    STATS_NAME(ble_gap_stats, update_fail)
    STATS_NAME(ble_gap_stats, connect_mst)
    STATS_NAME(ble_gap_stats, connect_slv)
    STATS_NAME(ble_gap_stats, disconnect)
    STATS_NAME(ble_gap_stats, rx_disconnect)
    STATS_NAME(ble_gap_stats, rx_update_complete)
    STATS_NAME(ble_gap_stats, rx_adv_report)
    STATS_NAME(ble_gap_stats, rx_conn_complete)
    STATS_NAME(ble_gap_stats, discover_cancel)
    STATS_NAME(ble_gap_stats, discover_cancel_fail)
    STATS_NAME(ble_gap_stats, security_initiate)
    STATS_NAME(ble_gap_stats, security_initiate_fail)
STATS_NAME_END(ble_gap_stats)

/*****************************************************************************
 * $debug                                                                    *
 *****************************************************************************/

#if MYNEWT_VAL(BLE_HS_DEBUG)
int
ble_gap_dbg_update_active(uint16_t conn_handle)
{
    const struct ble_gap_update_entry *entry;

    ble_hs_lock();
    entry = ble_gap_update_entry_find(conn_handle, NULL);
    ble_hs_unlock();

    return entry != NULL;
}
#endif

/*****************************************************************************
 * $log                                                                      *
 *****************************************************************************/

#if NIMBLE_BLE_SCAN && !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_log_duration(int32_t duration_ms)
{
    if (duration_ms == BLE_HS_FOREVER) {
        BLE_HS_LOG(INFO, "duration=forever");
    } else {
        BLE_HS_LOG(INFO, "duration=%dms", duration_ms);
    }
}
#endif

#if !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_log_conn(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                 const struct ble_gap_conn_params *params)
{
    if (peer_addr != NULL) {
        BLE_HS_LOG(INFO, "peer_addr_type=%d peer_addr=", peer_addr->type);
        BLE_HS_LOG_ADDR(INFO, peer_addr->val);
    }

    BLE_HS_LOG(INFO, " scan_itvl=%d scan_window=%d itvl_min=%d itvl_max=%d "
                     "latency=%d supervision_timeout=%d min_ce_len=%d "
                     "max_ce_len=%d own_addr_type=%d",
               params->scan_itvl, params->scan_window, params->itvl_min,
               params->itvl_max, params->latency, params->supervision_timeout,
               params->min_ce_len, params->max_ce_len, own_addr_type);
}
#endif

#if NIMBLE_BLE_SCAN && !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_log_disc(uint8_t own_addr_type, int32_t duration_ms,
                 const struct ble_gap_disc_params *disc_params)
{
    BLE_HS_LOG(INFO, "own_addr_type=%d filter_policy=%d passive=%d limited=%d "
                     "filter_duplicates=%d ",
               own_addr_type, disc_params->filter_policy, disc_params->passive,
               disc_params->limited, disc_params->filter_duplicates);
    ble_gap_log_duration(duration_ms);
}
#endif

static void
ble_gap_log_update(uint16_t conn_handle,
                   const struct ble_gap_upd_params *params)
{
    BLE_HS_LOG(INFO, "connection parameter update; "
                     "conn_handle=%d itvl_min=%d itvl_max=%d latency=%d "
                     "supervision_timeout=%d min_ce_len=%d max_ce_len=%d",
               conn_handle, params->itvl_min, params->itvl_max,
               params->latency, params->supervision_timeout,
               params->min_ce_len, params->max_ce_len);
}

static void
ble_gap_log_wl(const ble_addr_t *addr, uint8_t white_list_count)
{
    int i;

    BLE_HS_LOG(INFO, "count=%d ", white_list_count);

    for (i = 0; i < white_list_count; i++, addr++) {
        BLE_HS_LOG(INFO, "entry-%d={addr_type=%d addr=", i, addr->type);
        BLE_HS_LOG_ADDR(INFO, addr->val);
        BLE_HS_LOG(INFO, "} ");
    }
}

#if NIMBLE_BLE_ADVERTISE && !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_log_adv(uint8_t own_addr_type, const ble_addr_t *direct_addr,
                const struct ble_gap_adv_params *adv_params)
{
    BLE_HS_LOG(INFO, "disc_mode=%d", adv_params->disc_mode);
    if (direct_addr) {
        BLE_HS_LOG(INFO, " direct_addr_type=%d direct_addr=",
                   direct_addr->type);
        BLE_HS_LOG_ADDR(INFO, direct_addr->val);
    }
    BLE_HS_LOG(INFO, " adv_channel_map=%d own_addr_type=%d "
                     "adv_filter_policy=%d adv_itvl_min=%d adv_itvl_max=%d",
               adv_params->channel_map,
               own_addr_type,
               adv_params->filter_policy,
               adv_params->itvl_min,
               adv_params->itvl_max);
}
#endif

/*****************************************************************************
 * $snapshot                                                                 *
 *****************************************************************************/

static void
ble_gap_fill_conn_desc(struct ble_hs_conn *conn,
                       struct ble_gap_conn_desc *desc)
{
    struct ble_hs_conn_addrs addrs;

    ble_hs_conn_addrs(conn, &addrs);

    desc->our_id_addr = addrs.our_id_addr;
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    /* Check if the privacy is enabled, change the address type accordingly
     * */
    if (ble_host_rpa_enabled())
    {
        uint8_t *local_id = NULL;
        struct ble_hs_resolv_entry *rl = NULL;
        rl = ble_hs_resolv_list_find(conn->bhc_peer_addr.val);

        if (rl != NULL) {
            /* Get public ID address here */
            ble_hs_id_addr(BLE_ADDR_PUBLIC, (const uint8_t **) &local_id, NULL);
            memcpy(desc->our_id_addr.val, local_id, BLE_DEV_ADDR_LEN);
            desc->our_id_addr.type = BLE_ADDR_PUBLIC;
        }
    }
#endif
    desc->peer_id_addr = addrs.peer_id_addr;
    desc->our_ota_addr = addrs.our_ota_addr;
    desc->peer_ota_addr = addrs.peer_ota_addr;

    desc->conn_handle = conn->bhc_handle;
    desc->conn_itvl = conn->bhc_itvl;
    desc->conn_latency = conn->bhc_latency;
    desc->supervision_timeout = conn->bhc_supervision_timeout;
    desc->master_clock_accuracy = conn->bhc_master_clock_accuracy;
    desc->sec_state = conn->bhc_sec_state;

    if (conn->bhc_flags & BLE_HS_CONN_F_MASTER) {
        desc->role = BLE_GAP_ROLE_MASTER;
    } else {
        desc->role = BLE_GAP_ROLE_SLAVE;
    }
}

static void
ble_gap_conn_to_snapshot(struct ble_hs_conn *conn,
                         struct ble_gap_snapshot *snap)
{
    ble_gap_fill_conn_desc(conn, snap->desc);
    snap->cb = conn->bhc_cb;
    snap->cb_arg = conn->bhc_cb_arg;
}

static int
ble_gap_find_snapshot(uint16_t handle, struct ble_gap_snapshot *snap)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find(handle);
    if (conn != NULL) {
        ble_gap_conn_to_snapshot(conn, snap);
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    } else {
        return 0;
    }
}

int
ble_gap_conn_find(uint16_t handle, struct ble_gap_conn_desc *out_desc)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find(handle);
    if (conn != NULL && out_desc != NULL) {
        ble_gap_fill_conn_desc(conn, out_desc);
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    } else {
        return 0;
    }
}

int
ble_gap_conn_find_by_addr(const ble_addr_t *addr,
                          struct ble_gap_conn_desc *out_desc)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find_by_addr(addr);
    if (conn != NULL && out_desc != NULL) {
        ble_gap_fill_conn_desc(conn, out_desc);
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    }

    return 0;
}

static int
ble_gap_extract_conn_cb(uint16_t conn_handle,
                        ble_gap_event_fn **out_cb, void **out_cb_arg)
{
    const struct ble_hs_conn *conn;

    BLE_HS_DBG_ASSERT(conn_handle <= BLE_HCI_LE_CONN_HANDLE_MAX);

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        *out_cb = conn->bhc_cb;
        *out_cb_arg = conn->bhc_cb_arg;
    } else {
        *out_cb = NULL;
        *out_cb_arg = NULL;
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    } else {
        return 0;
    }
}

int
ble_gap_set_priv_mode(const ble_addr_t *peer_addr, uint8_t priv_mode)
{
    return ble_hs_pvcy_set_mode(peer_addr, priv_mode);
}

int
ble_gap_read_le_phy(uint16_t conn_handle, uint8_t *tx_phy, uint8_t *rx_phy)
{
    struct ble_hs_conn *conn;
    uint8_t buf[BLE_HCI_LE_RD_PHY_LEN];
    uint8_t rspbuf[4];
    uint8_t rsplen;
    int rc;

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    }

    rc = ble_hs_hci_cmd_build_le_read_phy(conn_handle, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_RD_PHY),
                           buf, sizeof(buf), rspbuf, sizeof(rspbuf), &rsplen);
    if (rc != 0) {
        return rc;
    }

    if (rsplen != sizeof(rspbuf)) {
        return BLE_HS_ECONTROLLER;
    }

    /* First two octets is conn_handle. We can ignore it */

    *tx_phy = rspbuf[2];
    *rx_phy = rspbuf[3];

    return 0;
}

int
ble_gap_set_prefered_default_le_phy(uint8_t tx_phys_mask, uint8_t rx_phys_mask)
{
    uint8_t buf[BLE_HCI_LE_SET_DEFAULT_PHY_LEN];
    int rc;

    rc = ble_hs_hci_cmd_build_le_set_default_phy(tx_phys_mask, rx_phys_mask,
                                                 buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_SET_DEFAULT_PHY),
                            buf, sizeof(buf), NULL, 0, NULL);
}

int
ble_gap_set_prefered_le_phy(uint16_t conn_handle, uint8_t tx_phys_mask,
                   uint8_t rx_phys_mask, uint16_t phy_opts)
{
    struct ble_hs_conn *conn;
    uint8_t buf[BLE_HCI_LE_SET_PHY_LEN];
    int rc;

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    }

    rc = ble_hs_hci_cmd_build_le_set_phy(conn_handle, tx_phys_mask,
                                         rx_phys_mask, phy_opts, buf,
                                         sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return ble_hs_hci_cmd_tx(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_PHY),
        buf, sizeof(buf), NULL, 0, NULL);
}

/*****************************************************************************
 * $misc                                                                     *
 *****************************************************************************/

static int
ble_gap_event_listener_call(struct ble_gap_event *event);

static int
ble_gap_call_event_cb(struct ble_gap_event *event,
                      ble_gap_event_fn *cb, void *cb_arg)
{
    int rc;

    BLE_HS_DBG_ASSERT(!ble_hs_locked_by_cur_task());

    if (cb != NULL) {
        rc = cb(event, cb_arg);
    } else {
        if (event->type == BLE_GAP_EVENT_CONN_UPDATE_REQ) {
            /* Just copy peer parameters back into the reply. */
            *event->conn_update_req.self_params =
                *event->conn_update_req.peer_params;
        }
        rc = 0;
    }

    return rc;
}


static int
ble_gap_call_conn_event_cb(struct ble_gap_event *event, uint16_t conn_handle)
{
    ble_gap_event_fn *cb;
    void *cb_arg;
    int rc;

    rc = ble_gap_extract_conn_cb(conn_handle, &cb, &cb_arg);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gap_call_event_cb(event, cb, cb_arg);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static bool
ble_gap_is_preempted(void)
{
    int i;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    if (ble_gap_master.preempted_op != BLE_GAP_OP_NULL) {
        return true;
    }

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (ble_gap_slave[i].preempted) {
            return true;
        }
    }

    return false;
}

static void
ble_gap_master_reset_state(void)
{
    ble_gap_master.op = BLE_GAP_OP_NULL;
    ble_gap_master.exp_set = 0;
    ble_gap_master.conn.cancel = 0;

    ble_hs_timer_resched();
}

static void
ble_gap_slave_reset_state(uint8_t instance)
{
    ble_gap_slave[instance].op = BLE_GAP_OP_NULL;

#if !MYNEWT_VAL(BLE_EXT_ADV)
    ble_gap_slave[instance].exp_set = 0;
    ble_hs_timer_resched();
#endif
}

static bool
ble_gap_has_client(struct ble_gap_master_state *out_state)
{
    if (!out_state) {
        return 0;
    }

    return out_state->cb;
}

static void
ble_gap_master_extract_state(struct ble_gap_master_state *out_state,
                             int reset_state)
{
    ble_hs_lock();

    *out_state = ble_gap_master;

    if (reset_state) {
        ble_gap_master_reset_state();
        ble_gap_master.preempted_op = BLE_GAP_OP_NULL;
    }

    ble_hs_unlock();
}

static void
ble_gap_slave_extract_cb(uint8_t instance,
                         ble_gap_event_fn **out_cb, void **out_cb_arg)
{
    ble_hs_lock();

    *out_cb = ble_gap_slave[instance].cb;
    *out_cb_arg = ble_gap_slave[instance].cb_arg;
    ble_gap_slave_reset_state(instance);

    ble_hs_unlock();
}

static void
ble_gap_adv_finished(uint8_t instance, int reason, uint16_t conn_handle,
                     uint8_t num_events)
{
    struct ble_gap_event event;
    ble_gap_event_fn *cb;
    void *cb_arg;

    ble_gap_slave_extract_cb(instance, &cb, &cb_arg);
    if (cb != NULL) {
        memset(&event, 0, sizeof event);
        event.type = BLE_GAP_EVENT_ADV_COMPLETE;
        event.adv_complete.reason = reason;
#if MYNEWT_VAL(BLE_EXT_ADV)
        event.adv_complete.instance = instance;
        event.adv_complete.conn_handle = conn_handle;
        event.adv_complete.num_ext_adv_events = num_events;
#endif
        cb(&event, cb_arg);
    }
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_scan_req_rcvd(uint8_t instance, uint8_t scan_addr_type,
                      uint8_t *scan_addr)
{
    struct ble_gap_event event;
    ble_gap_event_fn *cb;
    void *cb_arg;

    ble_gap_slave_extract_cb(instance, &cb, &cb_arg);
    if (cb != NULL) {
        memset(&event, 0, sizeof event);
        event.type = BLE_GAP_EVENT_SCAN_REQ_RCVD;
        event.scan_req_rcvd.instance = instance;
        event.scan_req_rcvd.scan_addr.type = scan_addr_type;
        memcpy(event.scan_req_rcvd.scan_addr.val, scan_addr, 6);
        cb(&event, cb_arg);
    }
}
#endif

static int
ble_gap_master_connect_failure(int status)
{
    struct ble_gap_master_state state;
    struct ble_gap_event event;
    int rc;

    ble_gap_master_extract_state(&state, 1);
    if (ble_gap_has_client(&state)) {
        memset(&event, 0, sizeof event);
        event.type = BLE_GAP_EVENT_CONNECT;
        event.connect.status = status;

        rc = state.cb(&event, state.cb_arg);
    } else {
        rc = 0;
    }

    return rc;
}

static void
ble_gap_master_connect_cancelled(void)
{
    struct ble_gap_master_state state;
    struct ble_gap_event event;

    ble_gap_master_extract_state(&state, 1);
    if (state.cb != NULL) {
        memset(&event, 0, sizeof event);
        event.type = BLE_GAP_EVENT_CONNECT;
        event.connect.conn_handle = BLE_HS_CONN_HANDLE_NONE;
        if (state.conn.cancel) {
            /* Connect procedure successfully cancelled. */
            event.connect.status = BLE_HS_EAPP;
        } else {
            /* Connect procedure timed out. */
            event.connect.status = BLE_HS_ETIMEOUT;
        }
        state.cb(&event, state.cb_arg);
    }
}

static void
ble_gap_disc_report(void *desc)
{
    struct ble_gap_master_state state;
    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
#if MYNEWT_VAL(BLE_EXT_ADV)
    event.type = BLE_GAP_EVENT_EXT_DISC;
    event.ext_disc = *((struct ble_gap_ext_disc_desc *)desc);
#else
    event.type = BLE_GAP_EVENT_DISC;
    event.disc = *((struct ble_gap_disc_desc *)desc);
#endif

    ble_gap_master_extract_state(&state, 0);
    if (ble_gap_has_client(&state)) {
        state.cb(&event, state.cb_arg);
    }

    ble_gap_event_listener_call(&event);
}

#if NIMBLE_BLE_SCAN
static void
ble_gap_disc_complete(void)
{
    struct ble_gap_master_state state;
    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_DISC_COMPLETE;
    event.disc_complete.reason = 0;

    ble_gap_master_extract_state(&state, 1);
    if (ble_gap_has_client(&state)) {
        ble_gap_call_event_cb(&event, state.cb, state.cb_arg);
    }

    ble_gap_event_listener_call(&event);
}
#endif

static void
ble_gap_update_notify(uint16_t conn_handle, int status)
{
    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_CONN_UPDATE;
    event.conn_update.conn_handle = conn_handle;
    event.conn_update.status = status;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, conn_handle);

    /* Terminate the connection on procedure timeout. */
    if (status == BLE_HS_ETIMEOUT) {
        ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM);
    }
}

static uint32_t
ble_gap_master_ticks_until_exp(void)
{
    ble_npl_stime_t ticks;

    if (ble_gap_master.op == BLE_GAP_OP_NULL || !ble_gap_master.exp_set) {
        /* Timer not set; infinity ticks until next event. */
        return BLE_HS_FOREVER;
    }

    ticks = ble_gap_master.exp_os_ticks - ble_npl_time_get();
    if (ticks > 0) {
        /* Timer not expired yet. */
        return ticks;
    }

    /* Timer just expired. */
    return 0;
}

#if !MYNEWT_VAL(BLE_EXT_ADV)
static uint32_t
ble_gap_slave_ticks_until_exp(void)
{
    ble_npl_stime_t ticks;

    if (ble_gap_slave[0].op == BLE_GAP_OP_NULL || !ble_gap_slave[0].exp_set) {
        /* Timer not set; infinity ticks until next event. */
        return BLE_HS_FOREVER;
    }

    ticks = ble_gap_slave[0].exp_os_ticks - ble_npl_time_get();
    if (ticks > 0) {
        /* Timer not expired yet. */
        return ticks;
    }

    /* Timer just expired. */
    return 0;
}
#endif

/**
 * Finds the update procedure that expires soonest.
 *
 * @param out_ticks_from_now    On success, the ticks until the update
 *                                  procedure's expiry time gets written here.
 *
 * @return                      The connection handle of the update procedure
 *                                  that expires soonest, or
 *                                  BLE_HS_CONN_HANDLE_NONE if there are no
 *                                  active update procedures.
 */
static uint16_t
ble_gap_update_next_exp(int32_t *out_ticks_from_now)
{
    struct ble_gap_update_entry *entry;
    ble_npl_time_t now;
    uint16_t conn_handle;
    int32_t best_ticks;
    int32_t ticks;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    conn_handle = BLE_HS_CONN_HANDLE_NONE;
    best_ticks = BLE_HS_FOREVER;
    now = ble_npl_time_get();

    SLIST_FOREACH(entry, &ble_gap_update_entries, next) {
        ticks = entry->exp_os_ticks - now;
        if (ticks <= 0) {
            ticks = 0;
        }

        if (ticks < best_ticks) {
            conn_handle = entry->conn_handle;
            best_ticks = ticks;
        }
    }

    if (out_ticks_from_now != NULL) {
        *out_ticks_from_now = best_ticks;
    }

    return conn_handle;

}

static void
ble_gap_master_set_timer(uint32_t ticks_from_now)
{
    ble_gap_master.exp_os_ticks = ble_npl_time_get() + ticks_from_now;
    ble_gap_master.exp_set = 1;

    ble_hs_timer_resched();
}

#if NIMBLE_BLE_ADVERTISE && !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_slave_set_timer(uint32_t ticks_from_now)
{
    ble_gap_slave[0].exp_os_ticks = ble_npl_time_get() + ticks_from_now;
    ble_gap_slave[0].exp_set = 1;

    ble_hs_timer_resched();
}
#endif

/**
 * Called when an error is encountered while the master-connection-fsm is
 * active.
 */
static void
ble_gap_master_failed(int status)
{
    switch (ble_gap_master.op) {
    case BLE_GAP_OP_M_CONN:
        STATS_INC(ble_gap_stats, initiate_fail);
        ble_gap_master_connect_failure(status);
        break;

#if NIMBLE_BLE_SCAN
    case BLE_GAP_OP_M_DISC:
        STATS_INC(ble_gap_stats, initiate_fail);
        ble_gap_disc_complete();
        ble_gap_master_reset_state();
        break;
#endif

    default:
        //BLE_HS_DBG_ASSERT(0);
        break;
    }
}

static void
ble_gap_update_failed(uint16_t conn_handle, int status)
{
    struct ble_gap_update_entry *entry;

    STATS_INC(ble_gap_stats, update_fail);

    ble_hs_lock();
    entry = ble_gap_update_entry_remove(conn_handle);
    ble_hs_unlock();

    ble_gap_update_entry_free(entry);

    ble_gap_update_notify(conn_handle, status);
}

void
ble_gap_conn_broken(uint16_t conn_handle, int reason)
{
    struct ble_gap_update_entry *entry;
    struct ble_gap_snapshot snap;
    struct ble_gap_event event;
    int rc;

    memset(&event, 0, sizeof event);
    snap.desc = &event.disconnect.conn;

    rc = ble_gap_find_snapshot(conn_handle, &snap);
    if (rc != 0) {
        /* No longer connected. */
        return;
    }

    /* If there was a connection update in progress, indicate to the
     * application that it did not complete.
     */
    ble_hs_lock();
    entry = ble_gap_update_entry_remove(conn_handle);
    ble_hs_unlock();

    if (entry != NULL) {
        ble_gap_update_notify(conn_handle, reason);
        ble_gap_update_entry_free(entry);
    }

    /* Indicate the connection termination to each module.  The order matters
     * here: gatts must come before gattc to ensure the application does not
     * get informed of spurious notify-tx events.
     */
    ble_l2cap_sig_conn_broken(conn_handle, reason);
    ble_sm_connection_broken(conn_handle);
    ble_gatts_connection_broken(conn_handle);
    ble_gattc_connection_broken(conn_handle);
    ble_hs_flow_connection_broken(conn_handle);;

    ble_hs_atomic_conn_delete(conn_handle);

    event.type = BLE_GAP_EVENT_DISCONNECT;
    event.disconnect.reason = reason;

    ble_gap_event_listener_call(&event);
    ble_gap_call_event_cb(&event, snap.cb, snap.cb_arg);

    STATS_INC(ble_gap_stats, disconnect);
}

static void
ble_gap_update_to_l2cap(const struct ble_gap_upd_params *params,
                        struct ble_l2cap_sig_update_params *l2cap_params)
{
    l2cap_params->itvl_min = params->itvl_min;
    l2cap_params->itvl_max = params->itvl_max;
    l2cap_params->slave_latency = params->latency;
    l2cap_params->timeout_multiplier = params->supervision_timeout;
}

void
ble_gap_rx_disconn_complete(struct hci_disconn_complete *evt)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    struct ble_gap_event event;

    STATS_INC(ble_gap_stats, rx_disconnect);

    if (evt->status == 0) {
        ble_gap_conn_broken(evt->connection_handle,
                            BLE_HS_HCI_ERR(evt->reason));
    } else {
        memset(&event, 0, sizeof event);
        event.type = BLE_GAP_EVENT_TERM_FAILURE;
        event.term_failure.conn_handle = evt->connection_handle;
        event.term_failure.status = BLE_HS_HCI_ERR(evt->status);

        ble_gap_event_listener_call(&event);
        ble_gap_call_conn_event_cb(&event, evt->connection_handle);
    }
}

void
ble_gap_rx_update_complete(struct hci_le_conn_upd_complete *evt)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    struct ble_gap_update_entry *entry;
    struct ble_l2cap_sig_update_params l2cap_params;
    struct ble_gap_event event;
    struct ble_hs_conn *conn;
    int cb_status;
    int call_cb;
    int rc;

    STATS_INC(ble_gap_stats, rx_update_complete);

    memset(&event, 0, sizeof event);
    memset(&l2cap_params, 0, sizeof l2cap_params);

    ble_hs_lock();

    conn = ble_hs_conn_find(evt->connection_handle);
    if (conn != NULL) {
        switch (evt->status) {
        case 0:
            /* Connection successfully updated. */
            conn->bhc_itvl = evt->conn_itvl;
            conn->bhc_latency = evt->conn_latency;
            conn->bhc_supervision_timeout = evt->supervision_timeout;
            break;

        case BLE_ERR_UNSUPP_REM_FEATURE:
            /* Peer reports that it doesn't support the procedure.  This should
             * only happen if our controller sent the 4.1 Connection Parameters
             * Request Procedure.  If we are the slave, fail over to the L2CAP
             * update procedure.
             */
            entry = ble_gap_update_entry_find(evt->connection_handle, NULL);
            if (entry != NULL && !(conn->bhc_flags & BLE_HS_CONN_F_MASTER)) {
                ble_gap_update_to_l2cap(&entry->params, &l2cap_params);
                entry->exp_os_ticks = ble_npl_time_get() +
                                      ble_npl_time_ms_to_ticks32(BLE_GAP_UPDATE_TIMEOUT_MS);
            }
            break;

        default:
            break;
        }
    }

    /* We aren't failing over to L2CAP, the update procedure is complete. */
    if (l2cap_params.itvl_min == 0) {
        entry = ble_gap_update_entry_remove(evt->connection_handle);
        ble_gap_update_entry_free(entry);
    }

    ble_hs_unlock();

    if (l2cap_params.itvl_min != 0) {
        rc = ble_l2cap_sig_update(evt->connection_handle,
                                  &l2cap_params,
                                  ble_gap_update_l2cap_cb, NULL);
        if (rc == 0) {
            call_cb = 0;
        } else {
            call_cb = 1;
            cb_status = rc;
        }
    } else {
        call_cb = 1;
        cb_status = BLE_HS_HCI_ERR(evt->status);
    }

    if (call_cb) {
        ble_gap_update_notify(evt->connection_handle, cb_status);
    }
}

/**
 * Tells you if there is an active central GAP procedure (connect or discover).
 */
int
ble_gap_master_in_progress(void)
{
    return ble_gap_master.op != BLE_GAP_OP_NULL;
}

static int
ble_gap_accept_master_conn(void)
{
    int rc;

    switch (ble_gap_master.op) {
    case BLE_GAP_OP_NULL:
    case BLE_GAP_OP_M_DISC:
        rc = BLE_HS_ENOENT;
        break;

    case BLE_GAP_OP_M_CONN:
        rc = 0;
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        rc = BLE_HS_ENOENT;
        break;
    }

    if (rc == 0) {
        STATS_INC(ble_gap_stats, connect_mst);
    }

    return rc;
}

static int
ble_gap_adv_active_instance(uint8_t instance)
{
    /* Assume read is atomic; mutex not necessary. */
    return ble_gap_slave[instance].op == BLE_GAP_OP_S_ADV;
}

/**
 * Clears advertisement and discovery state.  This function is necessary
 * when the controller loses its active state (e.g. on host stack reset).
 */
void
ble_gap_reset_state(int reason)
{
    uint16_t conn_handle;

    while (1) {
        conn_handle = ble_hs_atomic_first_conn_handle();
        if (conn_handle == BLE_HS_CONN_HANDLE_NONE) {
            break;
        }

        ble_gap_conn_broken(conn_handle, reason);
    }

#if NIMBLE_BLE_ADVERTISE
#if MYNEWT_VAL(BLE_EXT_ADV)
    uint8_t i;
    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (ble_gap_adv_active_instance(i)) {
            /* Indicate to application that advertising has stopped. */
            ble_gap_adv_finished(i, reason, 0, 0);
        }
    }
#else
    if (ble_gap_adv_active_instance(0)) {
        /* Indicate to application that advertising has stopped. */
        ble_gap_adv_finished(0, reason, 0, 0);
    }
#endif
#endif

#if (NIMBLE_BLE_SCAN || NIMBLE_BLE_CONNECT)
    ble_gap_master_failed(reason);
#endif
}

static int
ble_gap_accept_slave_conn(uint8_t instance)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
       rc = BLE_HS_ENOENT;
    } else if (!ble_gap_adv_active_instance(instance)) {
        rc = BLE_HS_ENOENT;
    } else {
        if (ble_gap_slave[instance].connectable) {
            rc = 0;
        } else {
            rc = BLE_HS_ENOENT;
        }
    }

    if (rc == 0) {
        STATS_INC(ble_gap_stats, connect_slv);
    }

    return rc;
}

static int
ble_gap_rx_adv_report_sanity_check(uint8_t *adv_data, uint8_t adv_data_len)
{
    const struct ble_hs_adv_field *flags;
    int rc;

    STATS_INC(ble_gap_stats, rx_adv_report);

    if (ble_gap_master.op != BLE_GAP_OP_M_DISC) {
        return -1;
    }

    /* If a limited discovery procedure is active, discard non-limited
     * advertisements.
     */
    if (ble_gap_master.disc.limited) {
        rc = ble_hs_adv_find_field(BLE_HS_ADV_TYPE_FLAGS, adv_data,
                                   adv_data_len, &flags);
        if ((rc == 0) && (flags->length == 2) &&
            !(flags->value[0] & BLE_HS_ADV_F_DISC_LTD)) {
            return -1;
        }
    }

    return 0;
}

void
ble_gap_rx_adv_report(struct ble_gap_disc_desc *desc)
{
#if !NIMBLE_BLE_SCAN
    return;
#endif

    if (ble_gap_rx_adv_report_sanity_check(desc->data, desc->length_data)) {
        return;
    }

    ble_gap_disc_report(desc);
}

#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
void
ble_gap_rx_le_scan_timeout(void)
{
    ble_gap_disc_complete();
}
#endif

#if MYNEWT_VAL(BLE_EXT_ADV)
void
ble_gap_rx_ext_adv_report(struct ble_gap_ext_disc_desc *desc)
{
    if (ble_gap_rx_adv_report_sanity_check(desc->data, desc->length_data)) {
        return;
    }

    ble_gap_disc_report(desc);
}

void
ble_gap_rx_adv_set_terminated(struct hci_le_adv_set_terminated *evt)
{
    uint16_t conn_handle;
    int reason;

    /* Currently spec allows only 0x3c and 0x43 when advertising was stopped
     * due to timeout or events limit, mp this for timeout error for now */
    if (evt->status) {
        reason = BLE_HS_ETIMEOUT;
        conn_handle = 0;
    } else {
        reason = 0;
        conn_handle = evt->conn_handle;
    }

    ble_gap_adv_finished(evt->adv_handle, reason, conn_handle,
                         evt->completed_events);
}

void
ble_gap_rx_scan_req_rcvd(struct hci_le_scan_req_rcvd *evt)
{
    ble_gap_scan_req_rcvd(evt->adv_handle, evt->scan_addr_type, evt->scan_addr);
}
#endif

/* Periodic adv events */
#if MYNEWT_VAL(BLE_PERIODIC_ADV)

void
ble_gap_rx_peroidic_adv_sync_estab(struct hci_le_subev_periodic_adv_sync_estab *evt)
{
    struct ble_gap_event event;
    ble_gap_event_fn *cb;
    void *cb_arg;

    memset(&event, 0, sizeof event);

    event.type = BLE_GAP_EVENT_PERIODIC_SYNC;
    event.periodic_sync.status = evt->status;

    ble_hs_lock();

    BLE_HS_DBG_ASSERT(ble_gap_sync.psync);

    if (!evt->status) {
        ble_gap_sync.psync->sync_handle  = evt->sync_handle;
        ble_gap_sync.psync->adv_sid = evt->sid;
        memcpy(ble_gap_sync.psync->advertiser_addr.val, evt->adv_addr, 6);
        ble_gap_sync.psync->advertiser_addr.type = evt->adv_addr_type;

        ble_gap_sync.psync->cb = ble_gap_sync.cb;
        ble_gap_sync.psync->cb_arg = ble_gap_sync.cb_arg;

        event.periodic_sync.sync_handle = evt->sync_handle;
        event.periodic_sync.sid = evt->sid;
        event.periodic_sync.adv_addr = ble_gap_sync.psync->advertiser_addr;
        event.periodic_sync.adv_phy = evt->adv_phy;
        event.periodic_sync.per_adv_ival = evt->per_adv_ival;
        event.periodic_sync.adv_clk_accuracy = evt->adv_clk_accuracy;

        ble_hs_periodic_sync_insert(ble_gap_sync.psync);
    } else {
        ble_hs_periodic_sync_free(ble_gap_sync.psync);
    }

    cb = ble_gap_sync.cb;
    cb_arg = ble_gap_sync.cb_arg;

    ble_gap_sync.op = BLE_GAP_OP_NULL;
    ble_gap_sync.cb_arg = NULL;
    ble_gap_sync.cb_arg = NULL;
    ble_gap_sync.psync = NULL;

    ble_hs_unlock();

    ble_gap_event_listener_call(&event);
    if (cb) {
        cb(&event, cb_arg);
    }
}

void
ble_gap_rx_periodic_adv_rpt(struct hci_le_subev_periodic_adv_rpt *evt)
{
    struct ble_hs_periodic_sync *psync;
    struct ble_gap_event event;
    ble_gap_event_fn *cb = NULL;
    void *cb_arg = NULL;

    ble_hs_lock();
    psync = ble_hs_periodic_sync_find_by_handle(evt->sync_handle);
    if (psync) {
        cb = psync->cb;
        cb_arg = psync->cb_arg;
    }
    ble_hs_unlock();

    if (!psync || !cb) {
        return;
    }

    memset(&event, 0, sizeof event);

    event.type = BLE_GAP_EVENT_PERIODIC_REPORT;
    event.periodic_report.sync_handle = evt->sync_handle;
    event.periodic_report.tx_power = evt->tx_power;
    event.periodic_report.rssi = evt->rssi;
    event.periodic_report.data_status = evt->data_status;
    event.periodic_report.data_length = evt->data_length;
    event.periodic_report.data = evt->data;

    /* TODO should we allow for listener too? this can be spammy and is more
     * like ACL data, not general event
     */
     cb(&event, cb_arg);
}

void
ble_gap_rx_periodic_adv_sync_lost(struct hci_le_subev_periodic_adv_sync_lost *evt)
{
    struct ble_hs_periodic_sync *psync;
    struct ble_gap_event event;
    ble_gap_event_fn *cb;
    void *cb_arg;

    memset(&event, 0, sizeof event);

    event.type = BLE_GAP_EVENT_PERIODIC_SYNC_LOST;
    event.periodic_sync_lost.sync_handle = evt->sync_handle;
    event.periodic_sync_lost.reason = BLE_HS_ETIMEOUT;

    ble_hs_lock();
    /* The handle must be in the list */
    psync = ble_hs_periodic_sync_find_by_handle(evt->sync_handle);
    BLE_HS_DBG_ASSERT(psync);

    cb = psync->cb;
    cb_arg = psync->cb_arg;

    /* Remove the handle from the list */
    ble_hs_periodic_sync_remove(psync);
    ble_hs_unlock();

    /* remove any sync_lost event from queue */
    ble_npl_eventq_remove(ble_hs_evq_get(), &psync->lost_ev);

    /* Free the memory occupied by psync as it is no longer needed */
    ble_hs_periodic_sync_free(psync);

    ble_gap_event_listener_call(&event);
    if (cb) {
        cb(&event, cb_arg);
    }
}
#endif

static int
ble_gap_rd_rem_sup_feat_tx(uint16_t handle)
{
    uint8_t buf[BLE_HCI_CONN_RD_REM_FEAT_LEN];
    int rc;

    rc = ble_hs_hci_cmd_build_le_read_remote_feat(handle, buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_EUNKNOWN;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_RD_REM_FEAT),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/**
 * Processes an incoming connection-complete HCI event.
 * instance parameter is valid only for slave connection.
 */
int
ble_gap_rx_conn_complete(struct hci_le_conn_complete *evt, uint8_t instance)
{
#if !NIMBLE_BLE_CONNECT
    return BLE_HS_ENOTSUP;
#endif

    struct ble_gap_event event;
    struct ble_hs_conn *conn;
    int rc;

    STATS_INC(ble_gap_stats, rx_conn_complete);

    /* in that case *only* status field is valid so we determine role
     * based on error code
     */
    if (evt->status != BLE_ERR_SUCCESS) {
        switch (evt->status) {
        case BLE_ERR_DIR_ADV_TMO:
            /* slave role (HD directed advertising)
             *
             * with ext advertising this is send from set terminated event
             */
#if !MYNEWT_VAL(BLE_EXT_ADV)
            if (ble_gap_adv_active()) {
                ble_gap_adv_finished(0, 0, 0, 0);
            }
#endif
            break;
        case BLE_ERR_UNK_CONN_ID:
            /* master role */
            if (ble_gap_master_in_progress()) {
                /* Connect procedure successfully cancelled. */
                if (ble_gap_master.preempted_op == BLE_GAP_OP_M_CONN) {
                    ble_gap_master_failed(BLE_HS_EPREEMPTED);
                } else {
                    ble_gap_master_connect_cancelled();
                }
            }
            break;
        default:
            /* this should never happen, unless controller is broken */
            BLE_HS_LOG(INFO, "controller reported invalid error code in conn"
                             "complete event: %u", evt->status);
            assert(0);
            break;
        }
        return 0;
    }

    /* Apply the event to the existing connection if it exists. */
    if (ble_hs_atomic_conn_flags(evt->connection_handle, NULL) == 0) {
        /* XXX: Does this ever happen? */
        return 0;
    }

    /* This event refers to a new connection. */

    switch (evt->role) {
    case BLE_HCI_LE_CONN_COMPLETE_ROLE_MASTER:
        rc = ble_gap_accept_master_conn();
        if (rc != 0) {
            return rc;
        }
        break;

    case BLE_HCI_LE_CONN_COMPLETE_ROLE_SLAVE:
        rc = ble_gap_accept_slave_conn(instance);
        if (rc != 0) {
            return rc;
        }
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }

    /* We verified that there is a free connection when the procedure began. */
    conn = ble_hs_conn_alloc(evt->connection_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);

    conn->bhc_itvl = evt->conn_itvl;
    conn->bhc_latency = evt->conn_latency;
    conn->bhc_supervision_timeout = evt->supervision_timeout;
    conn->bhc_master_clock_accuracy = evt->master_clk_acc;
    if (evt->role == BLE_HCI_LE_CONN_COMPLETE_ROLE_MASTER) {
        conn->bhc_cb = ble_gap_master.cb;
        conn->bhc_cb_arg = ble_gap_master.cb_arg;
        conn->bhc_flags |= BLE_HS_CONN_F_MASTER;
        conn->bhc_our_addr_type = ble_gap_master.conn.our_addr_type;
        ble_gap_master_reset_state();
    } else {
        conn->bhc_cb = ble_gap_slave[instance].cb;
        conn->bhc_cb_arg = ble_gap_slave[instance].cb_arg;
        conn->bhc_our_addr_type = ble_gap_slave[instance].our_addr_type;
#if MYNEWT_VAL(BLE_EXT_ADV)
        memcpy(conn->bhc_our_rnd_addr, ble_gap_slave[instance].rnd_addr, 6);
#endif
        ble_gap_slave_reset_state(instance);
    }

    conn->bhc_peer_addr.type = evt->peer_addr_type;
    memcpy(conn->bhc_peer_addr.val, evt->peer_addr, 6);

    conn->bhc_our_rpa_addr.type = BLE_ADDR_RANDOM;
    memcpy(conn->bhc_our_rpa_addr.val, evt->local_rpa, 6);

    /* If peer RPA is not set in the event and peer address
     * is RPA then store the peer RPA address so when the peer
     * address is resolved, the RPA is not forgotten.
     */
    if (memcmp(BLE_ADDR_ANY->val, evt->peer_rpa, 6) == 0) {
        if (BLE_ADDR_IS_RPA(&conn->bhc_peer_addr)) {
            conn->bhc_peer_rpa_addr = conn->bhc_peer_addr;
        }
    } else {
        conn->bhc_peer_rpa_addr.type = BLE_ADDR_RANDOM;
        memcpy(conn->bhc_peer_rpa_addr.val, evt->peer_rpa, 6);
    }

    ble_hs_lock();

    memset(&event, 0, sizeof event);
    ble_hs_conn_insert(conn);

    ble_hs_unlock();

    event.type = BLE_GAP_EVENT_CONNECT;
    event.connect.conn_handle = evt->connection_handle;
    event.connect.status = 0;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, evt->connection_handle);

    ble_gap_rd_rem_sup_feat_tx(evt->connection_handle);

    return 0;
}

void
ble_gap_rx_rd_rem_sup_feat_complete(
    struct hci_le_rd_rem_supp_feat_complete *evt)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find(evt->connection_handle);
    if (conn != NULL && evt->status == 0) {
            conn->supported_feat = get_le32(evt->features);
    }

    ble_hs_unlock();
}

int
ble_gap_rx_l2cap_update_req(uint16_t conn_handle,
                            struct ble_gap_upd_params *params)
{
    struct ble_gap_event event;
    int rc;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_L2CAP_UPDATE_REQ;
    event.conn_update_req.conn_handle = conn_handle;
    event.conn_update_req.peer_params = params;

    rc = ble_gap_call_conn_event_cb(&event, conn_handle);
    return rc;
}

void
ble_gap_rx_phy_update_complete(struct hci_le_phy_upd_complete *evt)
{
    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_PHY_UPDATE_COMPLETE;
    event.phy_updated.status = evt->status;
    event.phy_updated.conn_handle = evt->connection_handle;
    event.phy_updated.tx_phy = evt->tx_phy;
    event.phy_updated.rx_phy = evt->rx_phy;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, evt->connection_handle);
}

static int32_t
ble_gap_master_timer(void)
{
    uint32_t ticks_until_exp;
    int rc;

    ticks_until_exp = ble_gap_master_ticks_until_exp();
    if (ticks_until_exp != 0) {
        /* Timer not expired yet. */
        return ticks_until_exp;
    }

    /*** Timer expired; process event. */

    switch (ble_gap_master.op) {
    case BLE_GAP_OP_M_CONN:
        rc = ble_gap_conn_cancel_tx();
        if (rc != 0) {
            /* Failed to stop connecting; try again in 100 ms. */
            return ble_npl_time_ms_to_ticks32(BLE_GAP_CANCEL_RETRY_TIMEOUT_MS);
        } else {
            /* Stop the timer now that the cancel command has been acked. */
            ble_gap_master.exp_set = 0;

            /* Timeout gets reported when we receive a connection complete
             * event indicating the connect procedure has been cancelled.
             */
            /* XXX: Set a timer to reset the controller if a connection
             * complete event isn't received within a reasonable interval.
             */
        }
        break;

    case BLE_GAP_OP_M_DISC:
#if NIMBLE_BLE_SCAN && !MYNEWT_VAL(BLE_EXT_ADV)
        /* When a discovery procedure times out, it is not a failure. */
        rc = ble_gap_disc_enable_tx(0, 0);
        if (rc != 0) {
            /* Failed to stop discovery; try again in 100 ms. */
            return ble_npl_time_ms_to_ticks32(BLE_GAP_CANCEL_RETRY_TIMEOUT_MS);
        }

        ble_gap_disc_complete();
#else
        assert(0);
#endif
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }

    return BLE_HS_FOREVER;
}

#if !MYNEWT_VAL(BLE_EXT_ADV)
static int32_t
ble_gap_slave_timer(void)
{
    uint32_t ticks_until_exp;
    int rc;

    ticks_until_exp = ble_gap_slave_ticks_until_exp();
    if (ticks_until_exp != 0) {
        /* Timer not expired yet. */
        return ticks_until_exp;
    }

    /*** Timer expired; process event. */

    /* Stop advertising. */
    rc = ble_gap_adv_enable_tx(0);
    if (rc != 0) {
        /* Failed to stop advertising; try again in 100 ms. */
        return 100;
    }

    /* Clear the timer and cancel the current procedure. */
    ble_gap_slave_reset_state(0);

    /* Indicate to application that advertising has stopped. */
    ble_gap_adv_finished(0, BLE_HS_ETIMEOUT, 0, 0);

    return BLE_HS_FOREVER;
}
#endif

static int32_t
ble_gap_update_timer(void)
{
    struct ble_gap_update_entry *entry;
    int32_t ticks_until_exp;
    uint16_t conn_handle;

    do {
        ble_hs_lock();

        conn_handle = ble_gap_update_next_exp(&ticks_until_exp);
        if (ticks_until_exp == 0) {
            entry = ble_gap_update_entry_remove(conn_handle);
        } else {
            entry = NULL;
        }

        ble_hs_unlock();

        if (entry != NULL) {
            ble_gap_update_entry_free(entry);
        }
    } while (entry != NULL);

    return ticks_until_exp;
}

int
ble_gap_set_event_cb(uint16_t conn_handle, ble_gap_event_fn *cb, void *cb_arg)
{
    struct ble_hs_conn *conn;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        conn->bhc_cb = cb;
        conn->bhc_cb_arg = cb_arg;
    }

    ble_hs_unlock();

    if (conn == NULL) {
        return BLE_HS_ENOTCONN;
    }

    return 0;
}

/**
 * Handles timed-out GAP procedures.
 *
 * @return                      The number of ticks until this function should
 *                                  be called again.
 */
int32_t
ble_gap_timer(void)
{
    int32_t update_ticks;
    int32_t master_ticks;
    int32_t min_ticks;

    master_ticks = ble_gap_master_timer();
    update_ticks = ble_gap_update_timer();

    min_ticks = min(master_ticks, update_ticks);

#if !MYNEWT_VAL(BLE_EXT_ADV)
    min_ticks = min(min_ticks, ble_gap_slave_timer());
#endif

    return min_ticks;
}

/*****************************************************************************
 * $white list                                                               *
 *****************************************************************************/

static int
ble_gap_wl_busy(void)
{
#if !MYNEWT_VAL(BLE_WHITELIST)
    return BLE_HS_ENOTSUP;
#endif

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    if (ble_host_rpa_enabled()) {
        return BLE_HS_ENOTSUP;
    }
#endif
    /* Check if an auto or selective connection establishment procedure is in
     * progress.
     */
    return ble_gap_master.op == BLE_GAP_OP_M_CONN &&
           ble_gap_master.conn.using_wl;
}

static int
ble_gap_wl_tx_add(const ble_addr_t *addr)
{
    uint8_t buf[BLE_HCI_ADD_WHITE_LIST_LEN];
    int rc;

    rc = ble_hs_hci_cmd_build_le_add_to_whitelist(addr->val, addr->type,
                                                  buf, sizeof buf);
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_ADD_WHITE_LIST),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_wl_tx_clear(void)
{
    int rc;

    rc = ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_CLEAR_WHITE_LIST),
        NULL, 0);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_gap_wl_set(const ble_addr_t *addrs, uint8_t white_list_count)
{
#if !MYNEWT_VAL(BLE_WHITELIST)
    return BLE_HS_ENOTSUP;
#endif

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    if (ble_host_rpa_enabled()) {
        return BLE_HS_ENOTSUP;
    }
#endif
    int rc;
    int i;

    STATS_INC(ble_gap_stats, wl_set);

    ble_hs_lock();

    if (white_list_count == 0) {
        rc = BLE_HS_EINVAL;
        goto done;
    }

    for (i = 0; i < white_list_count; i++) {
        if (addrs[i].type != BLE_ADDR_PUBLIC &&
            addrs[i].type != BLE_ADDR_RANDOM) {

            rc = BLE_HS_EINVAL;
            goto done;
        }
    }

    if (ble_gap_wl_busy()) {
        rc = BLE_HS_EBUSY;
        goto done;
    }

    BLE_HS_LOG(INFO, "GAP procedure initiated: set whitelist; ");
    ble_gap_log_wl(addrs, white_list_count);
    BLE_HS_LOG(INFO, "\n");

    rc = ble_gap_wl_tx_clear();
    if (rc != 0) {
        goto done;
    }

    for (i = 0; i < white_list_count; i++) {
        rc = ble_gap_wl_tx_add(addrs + i);
        if (rc != 0) {
            goto done;
        }
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, wl_set_fail);
    }
    return rc;
}

/*****************************************************************************
 * $stop advertise                                                           *
 *****************************************************************************/

static int
ble_gap_adv_enable_tx(int enable)
{
    uint8_t buf[BLE_HCI_SET_ADV_ENABLE_LEN];
    uint16_t opcode;
    int rc;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_ADV_ENABLE);
    ble_hs_hci_cmd_build_le_set_adv_enable(!!enable, buf, sizeof buf);

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_adv_stop_no_lock(void)
{
#if !NIMBLE_BLE_ADVERTISE
    return BLE_HS_ENOTSUP;
#endif

    bool active;
    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    STATS_INC(ble_gap_stats, adv_stop);

    active = ble_gap_adv_active();

    BLE_HS_LOG(INFO, "GAP procedure initiated: stop advertising.\n");

    rc = ble_gap_adv_enable_tx(0);
    if (rc != 0) {
        goto done;
    }

    ble_gap_slave_reset_state(0);

    if (!active) {
        rc = BLE_HS_EALREADY;
    } else {
        rc = 0;
    }

done:
    if (rc != 0) {
        STATS_INC(ble_gap_stats, adv_stop_fail);
    }

    return rc;
}

int
ble_gap_adv_stop(void)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#endif

    int rc;

    ble_hs_lock();
    rc = ble_gap_adv_stop_no_lock();
    ble_hs_unlock();

    return rc;
}

/*****************************************************************************
 * $advertise                                                                *
 *****************************************************************************/
#if NIMBLE_BLE_ADVERTISE && !MYNEWT_VAL(BLE_EXT_ADV)
static int
ble_gap_adv_type(const struct ble_gap_adv_params *adv_params)
{
    switch (adv_params->conn_mode) {
    case BLE_GAP_CONN_MODE_NON:
        if (adv_params->disc_mode == BLE_GAP_DISC_MODE_NON) {
            return BLE_HCI_ADV_TYPE_ADV_NONCONN_IND;
        } else {
            return BLE_HCI_ADV_TYPE_ADV_SCAN_IND;
        }

    case BLE_GAP_CONN_MODE_UND:
        return BLE_HCI_ADV_TYPE_ADV_IND;

    case BLE_GAP_CONN_MODE_DIR:
        if (adv_params->high_duty_cycle) {
            return BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD;
        } else {
            return BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD;
        }

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_HCI_ADV_TYPE_ADV_IND;
    }
}

static void
ble_gap_adv_dflt_itvls(uint8_t conn_mode,
                       uint16_t *out_itvl_min, uint16_t *out_itvl_max)
{
    switch (conn_mode) {
    case BLE_GAP_CONN_MODE_NON:
        *out_itvl_min = BLE_GAP_ADV_FAST_INTERVAL2_MIN;
        *out_itvl_max = BLE_GAP_ADV_FAST_INTERVAL2_MAX;
        break;

    case BLE_GAP_CONN_MODE_UND:
        *out_itvl_min = BLE_GAP_ADV_FAST_INTERVAL1_MIN;
        *out_itvl_max = BLE_GAP_ADV_FAST_INTERVAL1_MAX;
        break;

    case BLE_GAP_CONN_MODE_DIR:
        *out_itvl_min = BLE_GAP_ADV_FAST_INTERVAL1_MIN;
        *out_itvl_max = BLE_GAP_ADV_FAST_INTERVAL1_MAX;
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }
}

static int
ble_gap_adv_params_tx(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                      const struct ble_gap_adv_params *adv_params)

{
    const ble_addr_t *peer_any = BLE_ADDR_ANY;
    struct hci_adv_params hci_adv_params;
    uint8_t buf[BLE_HCI_SET_ADV_PARAM_LEN];
    int rc;

    if (peer_addr == NULL) {
        peer_addr = peer_any;
    }

    hci_adv_params.own_addr_type = own_addr_type;
    hci_adv_params.peer_addr_type = peer_addr->type;
    memcpy(hci_adv_params.peer_addr, peer_addr->val,
           sizeof hci_adv_params.peer_addr);

    /* Fill optional fields if application did not specify them. */
    if (adv_params->itvl_min == 0 && adv_params->itvl_max == 0) {
        ble_gap_adv_dflt_itvls(adv_params->conn_mode,
                               &hci_adv_params.adv_itvl_min,
                               &hci_adv_params.adv_itvl_max);
    } else {
        hci_adv_params.adv_itvl_min = adv_params->itvl_min;
        hci_adv_params.adv_itvl_max = adv_params->itvl_max;
    }
    if (adv_params->channel_map == 0) {
        hci_adv_params.adv_channel_map = BLE_GAP_ADV_DFLT_CHANNEL_MAP;
    } else {
        hci_adv_params.adv_channel_map = adv_params->channel_map;
    }

    /* Zero is the default value for filter policy and high duty cycle */
    hci_adv_params.adv_filter_policy = adv_params->filter_policy;

    hci_adv_params.adv_type = ble_gap_adv_type(adv_params);
    rc = ble_hs_hci_cmd_build_le_set_adv_params(&hci_adv_params,
                                                buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_EINVAL;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_SET_ADV_PARAMS),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_adv_validate(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                     const struct ble_gap_adv_params *adv_params)
{
    if (adv_params == NULL) {
        return BLE_HS_EINVAL;
    }

    if (own_addr_type > BLE_HCI_ADV_OWN_ADDR_MAX) {
        return BLE_HS_EINVAL;
    }

    if (adv_params->disc_mode >= BLE_GAP_DISC_MODE_MAX) {
        return BLE_HS_EINVAL;
    }

    if (ble_gap_slave[0].op != BLE_GAP_OP_NULL) {
        return BLE_HS_EALREADY;
    }

    switch (adv_params->conn_mode) {
    case BLE_GAP_CONN_MODE_NON:
        /* High duty cycle only allowed for directed advertising. */
        if (adv_params->high_duty_cycle) {
            return BLE_HS_EINVAL;
        }
        break;

    case BLE_GAP_CONN_MODE_UND:
        /* High duty cycle only allowed for directed advertising. */
        if (adv_params->high_duty_cycle) {
            return BLE_HS_EINVAL;
        }

        /* Don't allow connectable advertising if we won't be able to allocate
         * a new connection.
         */
        if (!ble_hs_conn_can_alloc()) {
            return BLE_HS_ENOMEM;
        }
        break;

    case BLE_GAP_CONN_MODE_DIR:
        if (peer_addr == NULL) {
            return BLE_HS_EINVAL;
        }

        if (peer_addr->type != BLE_ADDR_PUBLIC &&
            peer_addr->type != BLE_ADDR_RANDOM &&
            peer_addr->type != BLE_ADDR_PUBLIC_ID &&
            peer_addr->type != BLE_ADDR_RANDOM_ID) {

            return BLE_HS_EINVAL;
        }

        /* Don't allow connectable advertising if we won't be able to allocate
         * a new connection.
         */
        if (!ble_hs_conn_can_alloc()) {
            return BLE_HS_ENOMEM;
        }
        break;

    default:
        return BLE_HS_EINVAL;
    }

    return 0;
}
#endif

int
ble_gap_adv_start(uint8_t own_addr_type, const ble_addr_t *direct_addr,
                  int32_t duration_ms,
                  const struct ble_gap_adv_params *adv_params,
                  ble_gap_event_fn *cb, void *cb_arg)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#else
    uint32_t duration_ticks;
    int rc;

    STATS_INC(ble_gap_stats, adv_start);

    ble_hs_lock();

    rc = ble_gap_adv_validate(own_addr_type, direct_addr, adv_params);
    if (rc != 0) {
        goto done;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        rc = ble_npl_time_ms_to_ticks(duration_ms, &duration_ticks);
        if (rc != 0) {
            /* Duration too great. */
            rc = BLE_HS_EINVAL;
            goto done;
        }
    }

    if (!ble_hs_is_enabled()) {
        rc = BLE_HS_EDISABLED;
        goto done;
    }

    if (ble_gap_is_preempted()) {
        rc = BLE_HS_EPREEMPTED;
        goto done;
    }

    rc = ble_hs_id_use_addr(own_addr_type);
    if (rc != 0) {
        goto done;
    }

    BLE_HS_LOG(INFO, "GAP procedure initiated: advertise; ");
    ble_gap_log_adv(own_addr_type, direct_addr, adv_params);
    BLE_HS_LOG(INFO, "\n");

    ble_gap_slave[0].cb = cb;
    ble_gap_slave[0].cb_arg = cb_arg;
    ble_gap_slave[0].our_addr_type = own_addr_type;

    if (adv_params->conn_mode != BLE_GAP_CONN_MODE_NON) {
        ble_gap_slave[0].connectable = 1;
    } else {
        ble_gap_slave[0].connectable = 0;
    }

    rc = ble_gap_adv_params_tx(own_addr_type, direct_addr, adv_params);
    if (rc != 0) {
        goto done;
    }

    ble_gap_slave[0].op = BLE_GAP_OP_S_ADV;

    rc = ble_gap_adv_enable_tx(1);
    if (rc != 0) {
        ble_gap_slave_reset_state(0);
        goto done;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        ble_gap_slave_set_timer(duration_ticks);
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, adv_start_fail);
    }
    return rc;
#endif
}

int
ble_gap_adv_set_data(const uint8_t *data, int data_len)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#endif

    uint8_t buf[BLE_HCI_SET_ADV_DATA_LEN];
    uint16_t opcode;
    int rc;

    STATS_INC(ble_gap_stats, adv_set_data);

    ble_hs_lock();

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_ADV_DATA);
    rc = ble_hs_hci_cmd_build_le_set_adv_data(data, data_len, buf,
                                              sizeof(buf));
    if (rc != 0) {
        goto done;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    ble_hs_unlock();
    return rc;
}

int
ble_gap_adv_rsp_set_data(const uint8_t *data, int data_len)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#endif

    uint8_t buf[BLE_HCI_SET_SCAN_RSP_DATA_LEN];
    uint16_t opcode;
    int rc;

    ble_hs_lock();

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_SCAN_RSP_DATA);
    rc = ble_hs_hci_cmd_build_le_set_scan_rsp_data(data, data_len,
                                                   buf, sizeof(buf));
    if (rc != 0) {
        rc = BLE_HS_HCI_ERR(rc);
        goto done;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        goto done;
    }

    rc = 0;

done:
    ble_hs_unlock();
    return rc;
}

int
ble_gap_adv_set_fields(const struct ble_hs_adv_fields *adv_fields)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#endif

    uint8_t buf[BLE_HS_ADV_MAX_SZ];
    uint8_t buf_sz;
    int rc;

    rc = ble_hs_adv_set_fields(adv_fields, buf, &buf_sz, sizeof buf);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gap_adv_set_data(buf, buf_sz);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_gap_adv_rsp_set_fields(const struct ble_hs_adv_fields *rsp_fields)
{
#if !NIMBLE_BLE_ADVERTISE || MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#endif

    uint8_t buf[BLE_HS_ADV_MAX_SZ];
    uint8_t buf_sz;
    int rc;

    rc = ble_hs_adv_set_fields(rsp_fields, buf, &buf_sz, sizeof buf);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gap_adv_rsp_set_data(buf, buf_sz);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_gap_adv_active(void)
{
    return ble_gap_adv_active_instance(0);
}

#if MYNEWT_VAL(BLE_EXT_ADV)
static int
ble_gap_ext_adv_params_tx(uint8_t instance,
                          const struct ble_gap_ext_adv_params *params,
                          int8_t *selected_tx_power)

{
    struct hci_ext_adv_params hci_adv_params;
    uint8_t buf[BLE_HCI_LE_SET_EXT_ADV_PARAM_LEN];
    uint8_t rsp;
    int rc;

    memset(&hci_adv_params, 0, sizeof(hci_adv_params));

    if (params->connectable) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_CONNECTABLE;
    }
    if (params->scannable) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_SCANNABLE;
    }
    if (params->directed) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_DIRECTED;
        hci_adv_params.peer_addr_type = params->peer.type;
        memcpy(hci_adv_params.peer_addr, params->peer.val, 6);
    }
    if (params->high_duty_directed) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_HD_DIRECTED;
    }
    if (params->legacy_pdu) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_LEGACY;
    }
    if (params->anonymous) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_ANON_ADV;
    }
    if (params->include_tx_power) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_EXT_ADV_PROP_INC_TX_PWR;
    }

    /* Fill optional fields if application did not specify them. */
    if (params->itvl_min == 0 && params->itvl_max == 0) {
        /* TODO for now limited to legacy values*/
        hci_adv_params.min_interval = BLE_GAP_ADV_FAST_INTERVAL1_MIN;
        hci_adv_params.max_interval = BLE_GAP_ADV_FAST_INTERVAL2_MAX;

    } else {
        hci_adv_params.min_interval = params->itvl_min;
        hci_adv_params.max_interval = params->itvl_max;
    }

    if (params->channel_map == 0) {
        hci_adv_params.chan_map = BLE_GAP_ADV_DFLT_CHANNEL_MAP;
    } else {
        hci_adv_params.chan_map = params->channel_map;
    }

    /* Zero is the default value for filter policy and high duty cycle */
    hci_adv_params.filter_policy = params->filter_policy;
    hci_adv_params.tx_power = params->tx_power;

    if (params->legacy_pdu) {
        hci_adv_params.primary_phy = BLE_HCI_LE_PHY_1M;
        hci_adv_params.secondary_phy = BLE_HCI_LE_PHY_1M;
    } else {
        hci_adv_params.primary_phy = params->primary_phy;
        hci_adv_params.secondary_phy = params->secondary_phy;
    }

    hci_adv_params.own_addr_type = params->own_addr_type;
    hci_adv_params.max_skip = 0;
    hci_adv_params.sid = params->sid;
    hci_adv_params.scan_req_notif = params->scan_req_notif;

    rc = ble_hs_hci_cmd_build_le_ext_adv_params(instance, &hci_adv_params,
                                                buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx(
            BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_EXT_ADV_PARAM),
            buf, sizeof(buf), &rsp, 1, NULL);

    if (rc != 0) {
        return rc;
    }

    if (selected_tx_power) {
        *selected_tx_power = rsp;
    }

    return 0;
}

static int
ble_gap_ext_adv_params_validate(const struct ble_gap_ext_adv_params *params)
{
    if (!params) {
        return BLE_HS_EINVAL;
    }

    if (params->own_addr_type > BLE_HCI_ADV_OWN_ADDR_MAX) {
        return BLE_HS_EINVAL;
    }

    /* Don't allow connectable advertising if we won't be able to allocate
     * a new connection.
     */
    if (params->connectable && !ble_hs_conn_can_alloc()) {
        return BLE_HS_ENOMEM;
    }

    if (params->legacy_pdu) {
        /* not allowed for legacy PDUs */
        if (params->anonymous || params->include_tx_power) {
            return BLE_HS_EINVAL;
        }
    }

    if (params->directed) {
        if (params->scannable && params->connectable) {
            return BLE_HS_EINVAL;
        }
    }

    if (!params->legacy_pdu) {
        /* not allowed for extended advertising PDUs */
        if (params->connectable && params->scannable) {
            return BLE_HS_EINVAL;
        }

        /* HD directed advertising allowed only for legacy PDUs */
        if (params->high_duty_directed) {
            return BLE_HS_EINVAL;
        }
    }

    return 0;
}

int
ble_gap_ext_adv_configure(uint8_t instance,
                          const struct ble_gap_ext_adv_params *params,
                          int8_t *selected_tx_power,
                          ble_gap_event_fn *cb, void *cb_arg)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    rc = ble_gap_ext_adv_params_validate(params);
    if (rc) {
        return rc;
    }

    ble_hs_lock();

    if (ble_gap_adv_active_instance(instance)) {
        ble_hs_unlock();
        return BLE_HS_EBUSY;
    }

    rc = ble_gap_ext_adv_params_tx(instance, params, selected_tx_power);
    if (rc) {
        ble_hs_unlock();
        return rc;
    }

    ble_gap_slave[instance].configured = 1;
    ble_gap_slave[instance].cb = cb;
    ble_gap_slave[instance].cb_arg = cb_arg;
    ble_gap_slave[instance].our_addr_type = params->own_addr_type;

    ble_gap_slave[instance].connectable = params->connectable;
    ble_gap_slave[instance].scannable = params->scannable;
    ble_gap_slave[instance].directed = params->directed;
    ble_gap_slave[instance].legacy_pdu = params->legacy_pdu;

    ble_hs_unlock();
    return 0;
}

static int
ble_gap_ext_adv_set_addr_no_lock(uint8_t instance, const uint8_t *addr)
{
    uint8_t buf[BLE_HCI_LE_SET_ADV_SET_RND_ADDR_LEN];
    int rc;

    rc = ble_hs_hci_cmd_build_le_ext_adv_set_random_addr(instance, addr, buf,
                                                         sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                     BLE_HCI_OCF_LE_SET_ADV_SET_RND_ADDR),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    ble_gap_slave[instance].rnd_addr_set = 1;
    memcpy(ble_gap_slave[instance].rnd_addr, addr, 6);

    return 0;
}

int
ble_gap_ext_adv_set_addr(uint8_t instance, const ble_addr_t *addr)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES || addr->type != BLE_ADDR_RANDOM) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    rc = ble_gap_ext_adv_set_addr_no_lock(instance, addr->val);
    ble_hs_unlock();

    return rc;
}

int
ble_gap_ext_adv_start(uint8_t instance, int duration, int max_events)
{
    const uint8_t *rnd_addr;
    uint8_t buf[6];
    struct hci_ext_adv_set set;
    uint16_t opcode;
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    if (!ble_gap_slave[instance].configured) {
        ble_hs_unlock();
        return BLE_HS_EINVAL;
    }

    if (ble_gap_slave[instance].op != BLE_GAP_OP_NULL) {
        ble_hs_unlock();
        return  BLE_HS_EALREADY;
    }

    if (ble_gap_slave[instance].directed && duration > 1280) {
        ble_hs_unlock();
        return BLE_HS_EINVAL;
    }

    /* verify own address type if random address for instance wasn't explicitly
     * set
     */
    switch (ble_gap_slave[instance].our_addr_type) {
    case BLE_OWN_ADDR_RANDOM:
    case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
        if (ble_gap_slave[instance].rnd_addr_set) {
            break;
        }
    /* fall through */
    case BLE_OWN_ADDR_PUBLIC:
    case BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT:
    default:
        rc = ble_hs_id_use_addr(ble_gap_slave[instance].our_addr_type);
        if (rc) {
            ble_hs_unlock();
            return BLE_HS_EINVAL;
        }
        break;
    }

    /* fallback to ID static random address if using random address and instance
     * wasn't configured with own address
     */
    if (!ble_gap_slave[instance].rnd_addr_set) {
        switch (ble_gap_slave[instance].our_addr_type) {
        case BLE_OWN_ADDR_RANDOM:
        case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
            rc = ble_hs_id_addr(BLE_ADDR_RANDOM, &rnd_addr, NULL);
            if (rc != 0) {
                ble_hs_unlock();
                return rc;
            }

            rc = ble_gap_ext_adv_set_addr_no_lock(instance, rnd_addr);
            if (rc != 0) {
                ble_hs_unlock();
                return rc;
            }
            break;
        default:
            break;
        }
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE);

    set.handle = instance;
    set.duration = duration;
    set.events = max_events;

    rc = ble_hs_hci_cmd_build_le_ext_adv_enable(1, 1, &set, buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    ble_gap_slave[instance].op = BLE_GAP_OP_S_ADV;

    ble_hs_unlock();
    return 0;
}

static int
ble_gap_ext_adv_stop_no_lock(uint8_t instance)
{
    uint8_t buf[6];
    struct hci_ext_adv_set set;
    uint16_t opcode;
    bool active;
    int rc;

    if (!ble_gap_slave[instance].configured) {
        return BLE_HS_EINVAL;
    }

    active = ble_gap_adv_active_instance(instance);

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_EXT_ADV_ENABLE);

    set.handle = instance;
    set.duration = 0;
    set.events = 0;

    rc = ble_hs_hci_cmd_build_le_ext_adv_enable(0, 1, &set, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    ble_gap_slave[instance].op = BLE_GAP_OP_NULL;

    if (!active) {
        return BLE_HS_EALREADY;
    } else {
        return 0;
    }
}

int
ble_gap_ext_adv_stop(uint8_t instance)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    rc = ble_gap_ext_adv_stop_no_lock(instance);
    ble_hs_unlock();

    return rc;
}


static int
ble_gap_ext_adv_set_data_validate(uint8_t instance, struct os_mbuf *data)
{
    uint16_t len = OS_MBUF_PKTLEN(data);

    if (!ble_gap_slave[instance].configured) {
        return BLE_HS_EINVAL;
    }

    /* not allowed with directed advertising for legacy*/
    if (ble_gap_slave[instance].legacy_pdu && ble_gap_slave[instance].directed) {
        return BLE_HS_EINVAL;
    }

    /* always allowed with legacy PDU but limited to legacy length */
    if (ble_gap_slave[instance].legacy_pdu) {
        if (len > BLE_HS_ADV_MAX_SZ) {
            return BLE_HS_EINVAL;
        }

        return 0;
    }

    /* if already advertising, data must fit in single HCI command
     * as per BT 5.0 Vol 2, Part E, 7.8.54. Don't bother Controller with such
     * a request.
     */
    if (ble_gap_slave[instance].op == BLE_GAP_OP_S_ADV) {
        if (len > min(MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE), 251)) {
            return BLE_HS_EINVAL;
        }
    }

    /* not allowed with scannable advertising */
    if (ble_gap_slave[instance].scannable) {
        return BLE_HS_EINVAL;
    }

    return 0;
}

static int
ble_gap_ext_adv_set(uint8_t instance, uint16_t opcode, struct os_mbuf **data)
{
    /* in that case we always fit all data in single HCI command */
#if MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE) <= BLE_HCI_MAX_EXT_ADV_DATA_LEN
    static uint8_t buf[BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN + \
                       MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE)];
    uint16_t len = OS_MBUF_PKTLEN(*data);
    int rc;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, opcode);

    rc = ble_hs_hci_cmd_build_le_ext_adv_data(instance,
                                    BLE_HCI_LE_SET_DATA_OPER_COMPLETE,
                                    0, *data, len, buf, sizeof(buf));
    if (rc) {
        return rc;
    }

    os_mbuf_adj(*data, MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE));
    *data = os_mbuf_trim_front(*data);

    return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
                                       BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN + len);
#else
    static uint8_t buf[BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN + \
                       BLE_HCI_MAX_EXT_ADV_DATA_LEN];
    uint16_t len = OS_MBUF_PKTLEN(*data);
    uint8_t op;
    int rc;

    opcode =  BLE_HCI_OP(BLE_HCI_OGF_LE, opcode);

    /* complete data */
    if (len <= BLE_HCI_MAX_EXT_ADV_DATA_LEN) {
        rc = ble_hs_hci_cmd_build_le_ext_adv_data(instance,
                                    BLE_HCI_LE_SET_DATA_OPER_COMPLETE,
                                    0, *data, len, buf,sizeof(buf));
        if (rc) {
            return rc;
        }

        os_mbuf_adj(*data, len);
        *data = os_mbuf_trim_front(*data);

        return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
                                        BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN + len);
    }

    /* first fragment  */
    op = BLE_HCI_LE_SET_DATA_OPER_FIRST;

    do {
        rc = ble_hs_hci_cmd_build_le_ext_adv_data(instance, op, 0, *data,
                                                  BLE_HCI_MAX_EXT_ADV_DATA_LEN,
                                                  buf, sizeof(buf));
        if (rc) {
            return rc;
        }

        os_mbuf_adj(*data, BLE_HCI_MAX_EXT_ADV_DATA_LEN);
        *data = os_mbuf_trim_front(*data);

        rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
        if (rc) {
            return rc;
        }

        len -= BLE_HCI_MAX_EXT_ADV_DATA_LEN;
        op = BLE_HCI_LE_SET_DATA_OPER_INT;
    } while (len > BLE_HCI_MAX_EXT_ADV_DATA_LEN);

    /* last fragment */
    rc = ble_hs_hci_cmd_build_le_ext_adv_data(instance,
                                        BLE_HCI_LE_SET_DATA_OPER_LAST,
                                        0, *data, len, buf, sizeof(buf));
    if (rc) {
        return rc;
    }

    os_mbuf_adj(*data, len);
    *data = os_mbuf_trim_front(*data);

    return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
                                       BLE_HCI_SET_EXT_ADV_DATA_HDR_LEN + len);
#endif
}

int
ble_gap_ext_adv_set_data(uint8_t instance, struct os_mbuf *data)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        rc = BLE_HS_EINVAL;
        goto done;
    }

    ble_hs_lock();
    rc = ble_gap_ext_adv_set_data_validate(instance, data);
    if (rc != 0) {
        ble_hs_unlock();
        goto done;
    }

    rc = ble_gap_ext_adv_set(instance, BLE_HCI_OCF_LE_SET_EXT_ADV_DATA, &data);

    ble_hs_unlock();

done:
    os_mbuf_free_chain(data);
    return rc;
}

static int
ble_gap_ext_adv_rsp_set_validate(uint8_t instance,  struct os_mbuf *data)
{
    uint16_t len = OS_MBUF_PKTLEN(data);

    if (!ble_gap_slave[instance].configured) {
        return BLE_HS_EINVAL;
    }

    /* not allowed with directed advertising */
    if (ble_gap_slave[instance].directed && ble_gap_slave[instance].connectable) {
        return BLE_HS_EINVAL;
    }

    /* only allowed with scannable advertising */
    if (!ble_gap_slave[instance].scannable) {
        return BLE_HS_EINVAL;
    }

    /* with legacy PDU limited to legacy length */
    if (ble_gap_slave[instance].legacy_pdu) {
        if (len > BLE_HS_ADV_MAX_SZ) {
            return BLE_HS_EINVAL;
        }

        return 0;
    }

    /* if already advertising, data must fit in single HCI command
     * as per BT 5.0 Vol 2, Part E, 7.8.55. Don't bother Controller with such
     * a request.
     */
    if (ble_gap_slave[instance].op == BLE_GAP_OP_S_ADV) {
        if (len > min(MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE), 251)) {
            return BLE_HS_EINVAL;
        }
    }

    return 0;
}

int
ble_gap_ext_adv_rsp_set_data(uint8_t instance, struct os_mbuf *data)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        rc = BLE_HS_EINVAL;
        goto done;
    }

    ble_hs_lock();
    rc = ble_gap_ext_adv_rsp_set_validate(instance, data);
    if (rc != 0) {
        ble_hs_unlock();
        goto done;
    }

    rc = ble_gap_ext_adv_set(instance, BLE_HCI_OCF_LE_SET_EXT_SCAN_RSP_DATA,
                             &data);

    ble_hs_unlock();

done:
    os_mbuf_free_chain(data);
    return rc;
}

int
ble_gap_ext_adv_remove(uint8_t instance)
{
    uint8_t buf[BLE_HCI_LE_REMOVE_ADV_SET_LEN];
    uint16_t opcode;
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    if (!ble_gap_slave[instance].configured) {
        ble_hs_unlock();
        return BLE_HS_EALREADY;
    }

    if (ble_gap_slave[instance].op == BLE_GAP_OP_S_ADV) {
        ble_hs_unlock();
        return BLE_HS_EBUSY;
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_REMOVE_ADV_SET);

    rc = ble_hs_hci_cmd_build_le_ext_adv_remove(instance, buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    memset(&ble_gap_slave[instance], 0, sizeof(struct ble_gap_slave_state));
    ble_hs_unlock();

    return 0;
}

int
ble_gap_ext_adv_clear(void)
{
    int rc;
    uint8_t instance;
    uint16_t opcode;

    ble_hs_lock();

    for (instance = 0; instance < BLE_ADV_INSTANCES; instance++) {
        /* If there is an active instance or periodic adv instance,
         * Don't send the command
         * */
        if ((ble_gap_slave[instance].op == BLE_GAP_OP_S_ADV)) {
            ble_hs_unlock();
            return BLE_HS_EBUSY;
        }

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
        if (ble_gap_slave[instance].periodic_op == BLE_GAP_OP_S_PERIODIC_ADV) {
            ble_hs_unlock();
            return BLE_HS_EBUSY;
        }
#endif
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_CLEAR_ADV_SETS);

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, NULL, 0);
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    memset(ble_gap_slave, 0, sizeof(ble_gap_slave));
    ble_hs_unlock();

    return 0;
}

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
static int
ble_gap_periodic_adv_params_tx(uint8_t instance,
        const struct ble_gap_periodic_adv_params *params)

{
    struct hci_periodic_adv_params hci_adv_params;
    uint8_t buf[BLE_HCI_LE_SET_PERIODIC_ADV_PARAMS_LEN];
    int rc;

    memset(&hci_adv_params, 0, sizeof(hci_adv_params));

    if (params->include_tx_power) {
        hci_adv_params.properties |= BLE_HCI_LE_SET_PERIODIC_ADV_PROP_INC_TX_PWR;
    }

    /* Fill optional fields if application did not specify them. */
    if (params->itvl_min == 0 && params->itvl_max == 0) {
        hci_adv_params.min_interval = 30 / 1.25;   //30 ms
        hci_adv_params.max_interval = 60 / 1.25;   //150 ms

    } else {
        hci_adv_params.min_interval = params->itvl_min;
        hci_adv_params.max_interval = params->itvl_max;
    }

    rc = ble_hs_hci_cmd_build_le_periodic_adv_params(instance, &hci_adv_params, buf,
            sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(
            BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_PERIODIC_ADV_PARAMS), buf,
            sizeof(buf));

    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_periodic_adv_params_validate(
        const struct ble_gap_periodic_adv_params *params)
{
    if (!params) {
        return BLE_HS_EINVAL;
    }

    if (params->itvl_min && params->itvl_min < 6) {
        return BLE_HS_EINVAL;
    }
    if (params->itvl_max && params->itvl_max < 6) {
            return BLE_HS_EINVAL;
    }
    return 0;
}

int
ble_gap_periodic_adv_configure(uint8_t instance,
        const struct ble_gap_periodic_adv_params *params)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    rc = ble_gap_periodic_adv_params_validate(params);
    if (rc) {
        return rc;
    }

    ble_hs_lock();

    /* The corresponding extended advertising instance should be configured */
    if (!ble_gap_slave[instance].configured) {
        ble_hs_unlock();
        return ENOMEM;
    }

    /* Periodic advertising shall not be configured while it is already
     * running.
     * Bluetooth Core Specification, Section 7.8.61
     */
    if (ble_gap_slave[instance].periodic_op == BLE_GAP_OP_S_PERIODIC_ADV) {
        ble_hs_unlock();
        return BLE_HS_EINVAL;
    }

    rc = ble_gap_periodic_adv_params_tx(instance, params);
    if (rc) {
        ble_hs_unlock();
        return rc;
    }

    ble_gap_slave[instance].periodic_configured = 1;

    ble_hs_unlock();

    return 0;
}

int
ble_gap_periodic_adv_start(uint8_t instance)
{
    uint8_t buf[BLE_HCI_LE_SET_PERIODIC_ADV_ENABLE_LEN];
    uint16_t opcode;
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();

    /* Periodic advertising cannot start unless it is configured before */
    if (!ble_gap_slave[instance].periodic_configured) {
        ble_hs_unlock();
        return BLE_HS_EINVAL;
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE,
                                  BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE);
    rc = ble_hs_hci_cmd_build_le_periodic_adv_enable(1, instance, buf,
                                                            sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    ble_gap_slave[instance].periodic_op = BLE_GAP_OP_S_PERIODIC_ADV;

    ble_hs_unlock();
    return 0;
}

static int
ble_gap_periodic_adv_set(uint8_t instance, uint16_t opcode,
                         struct os_mbuf **data)
{
    /* In that case we always fit all data in single HCI command */
#if MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE) <= BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN

    static uint8_t buf[BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN +
                       MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE)];
    uint16_t len = OS_MBUF_PKTLEN(*data);
    int rc;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, opcode);

    rc = ble_hs_hci_cmd_build_le_periodic_adv_data(
            instance,
            BLE_HCI_LE_SET_DATA_OPER_COMPLETE,
            *data, len,
            buf, sizeof(buf));

    if (rc) {
        return rc;
    }

    os_mbuf_adj(*data, MYNEWT_VAL(BLE_EXT_ADV_MAX_SIZE));
    *data = os_mbuf_trim_front(*data);

    return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
            BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN + len);
#else
    static uint8_t buf[BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN +
                       BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN];
    uint16_t len = OS_MBUF_PKTLEN(*data);
    uint8_t op;
    int rc;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, opcode);

    /* Complete data */
    if (len <= BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN) {
        rc = ble_hs_hci_cmd_build_le_periodic_adv_data(instance,
                BLE_HCI_LE_SET_DATA_OPER_COMPLETE, *data, len, buf,
                sizeof(buf));
        if (rc) {
            return rc;
        }

        os_mbuf_adj(*data, len);
        *data = os_mbuf_trim_front(*data);

        return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
                BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN + len);
    }

    /* If the periodic advertising is already enabled, the periodic advertising
     * the op code shall be nothing but 0x03
     * Bluetooth Core Specification, section 7.8.62
     */
    if (ble_gap_slave[instance].periodic_op == BLE_GAP_OP_S_PERIODIC_ADV) {
        return BLE_HS_EINVAL;
    }

    /* First fragment  */
    op = BLE_HCI_LE_SET_DATA_OPER_FIRST;

    do{
        rc = ble_hs_hci_cmd_build_le_periodic_adv_data(instance, op, *data,
                BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN, buf, sizeof(buf));
        if (rc) {
            return rc;
        }

        os_mbuf_adj(*data, BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN);
        *data = os_mbuf_trim_front(*data);

        rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
        if (rc) {
            return rc;
        }

        len -= BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN;
        op = BLE_HCI_LE_SET_DATA_OPER_INT;
    } while (len > BLE_HCI_MAX_PERIODIC_ADV_DATA_LEN);

    /* Last fragment */
    rc = ble_hs_hci_cmd_build_le_periodic_adv_data(
            instance,
            BLE_HCI_LE_SET_DATA_OPER_LAST,
            *data, len, buf, sizeof(buf));
    if (rc) {
        return rc;
    }

    os_mbuf_adj(*data, len);
    *data = os_mbuf_trim_front(*data);

    return ble_hs_hci_cmd_tx_empty_ack(opcode, buf,
            BLE_HCI_SET_PERIODIC_ADV_DATA_HDR_LEN + len);
#endif
}

static int
ble_gap_periodic_adv_set_data_validate(uint8_t instance,
                                       struct os_mbuf *data)
{
    /* The corresponding extended advertising instance should be configured */
    if (!ble_gap_slave[instance].configured) {
        return BLE_HS_EINVAL;
    }

    if (ble_gap_slave[instance].legacy_pdu) {
        return BLE_HS_EINVAL;
    }

    /* One more check states that if the periodic advertising is already
     * enabled, the operation shall be 0x03 (Complete).
     * This check is handled during sending the data to the controller, as the
     * length checks are already checked there, so this saves duplicate code
     */

    return 0;
}

int
ble_gap_periodic_adv_set_data(uint8_t instance, struct os_mbuf *data)
{
    int rc;
    if (instance >= BLE_ADV_INSTANCES) {
        rc = BLE_HS_EINVAL;
        goto done;
    }

    ble_hs_lock();

    rc = ble_gap_periodic_adv_set_data_validate(instance, data);
    if (rc != 0) {
        ble_hs_unlock();
        goto done;
    }

    rc = ble_gap_periodic_adv_set(instance,
                                  BLE_HCI_OCF_LE_SET_PERIODIC_ADV_DATA, &data);

    ble_hs_unlock();

done:
    os_mbuf_free_chain(data);
    return rc;
}

static int
ble_gap_periodic_adv_stop_no_lock(uint8_t instance)
{
    uint8_t buf[BLE_HCI_LE_SET_PERIODIC_ADV_ENABLE_LEN];
    uint16_t opcode;
    int rc;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_PERIODIC_ADV_ENABLE);

    rc = ble_hs_hci_cmd_build_le_periodic_adv_enable(0, instance, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    ble_gap_slave[instance].periodic_op = BLE_GAP_OP_NULL;

    return 0;
}

int
ble_gap_periodic_adv_stop(uint8_t instance)
{
    int rc;

    if (instance >= BLE_ADV_INSTANCES) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    rc = ble_gap_periodic_adv_stop_no_lock(instance);
    ble_hs_unlock();

    return rc;
}

static void
ble_gap_npl_sync_lost(struct ble_npl_event *ev)
{
    struct ble_hs_periodic_sync *psync;
    struct ble_gap_event event;
    ble_gap_event_fn *cb;
    void *cb_arg;

    /* this psync is no longer on list so no lock needed */
    psync = ble_npl_event_get_arg(ev);
    cb = psync->cb;
    cb_arg = psync->cb_arg;

    memset(&event, 0, sizeof event);

    event.type = BLE_GAP_EVENT_PERIODIC_SYNC_LOST;
    event.periodic_sync_lost.sync_handle = psync->sync_handle;
    event.periodic_sync_lost.reason = BLE_HS_EDONE;

    /* Free the memory occupied by psync as it is no longer needed */
    ble_hs_periodic_sync_free(psync);

    ble_gap_event_listener_call(&event);
    if (cb) {
        cb(&event, cb_arg);
    }
}

int
ble_gap_periodic_adv_create_sync(const ble_addr_t *addr, uint8_t adv_sid,
                                 const struct ble_gap_periodic_sync_params *params,
                                 ble_gap_event_fn *cb, void *cb_arg)
{
    uint8_t buf[BLE_HCI_LE_PERIODIC_ADV_CREATE_SYNC_LEN];
    struct ble_hs_periodic_sync *psync;
    uint16_t opcode;
    int rc = 0;

    ble_hs_lock();

    /* No sync can be created if another sync is still pending */
    if (ble_gap_sync.op == BLE_GAP_OP_SYNC) {
        ble_hs_unlock();
        return BLE_HS_EBUSY;
    }

    /* cannot create another sync if already synchronized */
    if (ble_hs_periodic_sync_find(addr, adv_sid)) {
        ble_hs_unlock();
        return BLE_HS_EALREADY;
    }

    /* preallocate sync element */
    psync = ble_hs_periodic_sync_alloc();
    if (!psync) {
        ble_hs_unlock();
        return BLE_HS_ENOMEM;
    }

    ble_npl_event_init(&psync->lost_ev, ble_gap_npl_sync_lost, psync);

    if (addr) {
        rc = ble_hs_hci_cmd_build_le_periodic_adv_create_sync(0x00, adv_sid,
                                        addr->type, addr->val, params->skip,
                                        params->sync_timeout, buf, sizeof(buf));
    } else {
        rc = ble_hs_hci_cmd_build_le_periodic_adv_create_sync(0x01, adv_sid,
                            BLE_ADDR_ANY->type, BLE_ADDR_ANY->val, params->skip,
                            params->sync_timeout, buf, sizeof(buf));
    }

    if (rc != 0) {
        ble_hs_periodic_sync_free(psync);
        ble_hs_unlock();
        return rc;
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC);
    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));

    if (!rc) {
        /* This shall be reset upon receiving sync_established event,
         * or if the sync is cancelled before receiving that event.
         */
        ble_gap_sync.op = BLE_GAP_OP_SYNC;
        ble_gap_sync.cb = cb;
        ble_gap_sync.cb_arg = cb_arg;
        ble_gap_sync.psync = psync;
    } else {
        ble_hs_periodic_sync_free(psync);
    }

    ble_hs_unlock();

    return rc;
}

int
ble_gap_periodic_adv_create_sync_cancel(void)
{
    uint16_t opcode;
    int rc = 0;

    ble_hs_lock();

    if (ble_gap_sync.op != BLE_GAP_OP_SYNC) {
        ble_hs_unlock();
        return BLE_HS_EBUSY;
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE,
                        BLE_HCI_OCF_LE_PERIODIC_ADV_CREATE_SYNC_CANCEL);

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, NULL, 0);

    if (!rc) {
        ble_gap_sync.op = BLE_GAP_OP_NULL;
        ble_gap_sync.cb = NULL;
        ble_gap_sync.cb_arg = NULL;
        ble_hs_periodic_sync_free(ble_gap_sync.psync);
        ble_gap_sync.psync = NULL;
    }

    ble_hs_unlock();

    return rc;
}

int
ble_gap_periodic_adv_terminate_sync(uint16_t sync_handle)
{
    uint8_t buf[BLE_HCI_LE_PERIODIC_ADV_TERM_SYNC_LEN];
    uint16_t opcode;
    struct ble_hs_periodic_sync *psync;
    int rc = 0;

    ble_hs_lock();

    if (ble_gap_sync.op == BLE_GAP_OP_SYNC) {
        ble_hs_unlock();
        return BLE_HS_EBUSY;
    }

    /* The handle must be in the list. If it doesn't exist, it means
     * that the sync may have been lost at the same moment in which
     * the app wants to terminate that sync handle
     */
    psync = ble_hs_periodic_sync_find_by_handle(sync_handle);
    if (!psync) {
        /* Sync already terminated.*/
        ble_hs_unlock();
        return BLE_HS_ENOTCONN;
    }

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_PERIODIC_ADV_TERM_SYNC);

    rc = ble_hs_hci_cmd_build_le_periodic_adv_terminate_sync(sync_handle,
                                                             buf, sizeof(buf));
    if (rc != 0) {
        ble_hs_unlock();
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));

    if (rc == 0) {
        /* Remove the handle from the list */
        ble_hs_periodic_sync_remove(psync);

        /* send sync_lost event, this is to mimic connection behavior and thus
         * simplify application error handling
         */
        ble_npl_eventq_put(ble_hs_evq_get(), &psync->lost_ev);
    }

    ble_hs_unlock();

    return rc;
}

int
ble_gap_add_dev_to_periodic_adv_list(const ble_addr_t *peer_addr,
                                     uint8_t adv_sid)
{
    uint8_t buf[BLE_HCI_LE_ADD_DEV_TO_PERIODIC_ADV_LIST_LEN];
    uint16_t opcode;
    int rc = 0;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE,
                        BLE_HCI_OCF_LE_ADD_DEV_TO_PERIODIC_ADV_LIST);

    rc = ble_hs_hci_cmd_build_le_add_dev_to_periodic_adv_list(peer_addr->type,
                                                              peer_addr->val,
                                                              adv_sid, buf,
                                                              sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));

    return rc;
}

int
ble_gap_rem_dev_from_periodic_adv_list(const ble_addr_t *peer_addr, uint8_t adv_sid)
{
    uint8_t buf[BLE_HCI_LE_REM_DEV_FROM_PERIODIC_ADV_LIST_LEN];
    uint16_t opcode;
    int rc = 0;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE,
                        BLE_HCI_OCF_LE_REM_DEV_FROM_PERIODIC_ADV_LIST);

    rc = ble_hs_hci_cmd_build_le_rem_dev_from_periodic_adv_list(peer_addr->type,
                                                                peer_addr->val,
                                                                adv_sid, buf,
                                                                sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, buf, sizeof(buf));

    return rc;
}

int
ble_gap_clear_periodic_adv_list(void)
{
    uint16_t opcode;
    int rc = 0;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_CLEAR_PERIODIC_ADV_LIST);

    rc = ble_hs_hci_cmd_tx_empty_ack(opcode, NULL, 0);

    return rc;
}

int
ble_gap_read_periodic_adv_list_size(uint8_t *per_adv_list_size)
{
    uint8_t rspbuf[1];
    uint8_t rsplen;
    uint16_t opcode;
    int rc = 0;

    opcode = BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_RD_PERIODIC_ADV_LIST_SIZE);

    rc = ble_hs_hci_cmd_tx(opcode, NULL, 0, rspbuf, sizeof(rspbuf), &rsplen);
    if (rc != 0) {
        return rc;
    }

    if (rsplen != sizeof(rspbuf)) {
        return BLE_HS_ECONTROLLER;
    }

    *per_adv_list_size = rspbuf[0];

    return 0;
}
#endif

/*****************************************************************************
 * $discovery procedures                                                     *
 *****************************************************************************/

#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
static int
ble_gap_ext_disc_tx_params(uint8_t own_addr_type, uint8_t filter_policy,
                       const struct ble_hs_hci_ext_scan_param *uncoded_params,
                       const struct ble_hs_hci_ext_scan_param *coded_params)
{
    uint8_t buf[BLE_HCI_LE_EXT_SCAN_BASE_LEN +
                2 * BLE_HCI_LE_EXT_SCAN_SINGLE_PARAM_LEN];
    uint8_t phy_mask = 0;
    struct ble_hs_hci_ext_scan_param param[2] = {{0}};
    struct ble_hs_hci_ext_scan_param *p = param;
    int phy_count = 0;
    uint8_t len;
    int rc;

    if (uncoded_params) {
        phy_mask |= BLE_HCI_LE_PHY_1M_PREF_MASK;
        memcpy(&param[phy_count], uncoded_params, sizeof(*uncoded_params));
        phy_count++;
    }

    if (coded_params) {
        phy_mask |= BLE_HCI_LE_PHY_CODED_PREF_MASK;
        memcpy(&param[phy_count], coded_params, sizeof(*coded_params));
        phy_count++;
    }

    rc = ble_hs_hci_cmd_build_le_set_ext_scan_params(own_addr_type,
                                                     filter_policy,
                                                     phy_mask,
                                                     phy_count,
                                                     &p, buf, sizeof(buf));

    if (rc != 0) {
        return BLE_HS_EINVAL;
    }

    len = BLE_HCI_LE_EXT_SCAN_BASE_LEN +
          BLE_HCI_LE_EXT_SCAN_SINGLE_PARAM_LEN * phy_count;

    return ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                            BLE_HCI_OCF_LE_SET_EXT_SCAN_PARAM),
                                       buf, len);
}

static int
ble_gap_ext_disc_enable_tx(uint8_t enable, uint8_t filter_duplicates,
                           uint16_t duration, uint16_t period)
{
    uint8_t buf[BLE_HCI_LE_SET_EXT_SCAN_ENABLE_LEN];

    ble_hs_hci_cmd_build_le_set_ext_scan_enable(enable, filter_duplicates,
                                                duration, period,
                                                buf, sizeof buf);

    return ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_EXT_SCAN_ENABLE),
        buf, sizeof(buf));
}
#endif
#endif
#if NIMBLE_BLE_SCAN
#if !MYNEWT_VAL(BLE_EXT_ADV)
static int
ble_gap_disc_enable_tx(int enable, int filter_duplicates)
{
    uint8_t buf[BLE_HCI_SET_SCAN_ENABLE_LEN];
    int rc;

    ble_hs_hci_cmd_build_le_set_scan_enable(!!enable, !!filter_duplicates,
                                            buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_SCAN_ENABLE),
        buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_disc_tx_params(uint8_t own_addr_type,
                       const struct ble_gap_disc_params *disc_params)
{
    uint8_t buf[BLE_HCI_SET_SCAN_PARAM_LEN];
    uint8_t scan_type;
    int rc;

    if (disc_params->passive) {
        scan_type = BLE_HCI_SCAN_TYPE_PASSIVE;
    } else {
        scan_type = BLE_HCI_SCAN_TYPE_ACTIVE;
    }

    rc = ble_hs_hci_cmd_build_le_set_scan_params(scan_type,
                                                 disc_params->itvl,
                                                 disc_params->window,
                                                 own_addr_type,
                                                 disc_params->filter_policy,
                                                 buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_EINVAL;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_SET_SCAN_PARAMS),
        buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}
#endif

static int
ble_gap_disc_disable_tx(void)
{
#if MYNEWT_VAL(BLE_EXT_ADV)
    return ble_gap_ext_disc_enable_tx(0, 0, 0, 0);
#else
    return ble_gap_disc_enable_tx(0, 0);
#endif
}

static int
ble_gap_disc_cancel_no_lock(void)
{
    int rc;

    STATS_INC(ble_gap_stats, discover_cancel);

    if (!ble_gap_disc_active()) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    rc = ble_gap_disc_disable_tx();
    if (rc != 0) {
        goto done;
    }

    ble_gap_master_reset_state();

done:
    if (rc != 0) {
        STATS_INC(ble_gap_stats, discover_cancel_fail);
    }

    return rc;
}
#endif

int
ble_gap_disc_cancel(void)
{
#if !NIMBLE_BLE_SCAN
    return BLE_HS_ENOTSUP;
#else

    int rc;

    ble_hs_lock();
    rc = ble_gap_disc_cancel_no_lock();
    ble_hs_unlock();

    return rc;
#endif
}

#if NIMBLE_BLE_SCAN
static int
ble_gap_disc_ext_validate(uint8_t own_addr_type)
{
    if (own_addr_type > BLE_HCI_ADV_OWN_ADDR_MAX) {
        return BLE_HS_EINVAL;
    }

    if (ble_gap_conn_active()) {
        return BLE_HS_EBUSY;
    }

    if (ble_gap_disc_active()) {
        return BLE_HS_EALREADY;
    }

    if (!ble_hs_is_enabled()) {
        return BLE_HS_EDISABLED;
    }

    if (ble_gap_is_preempted()) {
        return BLE_HS_EPREEMPTED;
    }

    return 0;
}
#endif

#if MYNEWT_VAL(BLE_EXT_ADV) && NIMBLE_BLE_SCAN
static void
ble_gap_ext_disc_fill_dflts(uint8_t limited,
                            struct ble_hs_hci_ext_scan_param *disc_params)
{
   if (disc_params->scan_itvl == 0) {
        if (limited) {
            disc_params->scan_itvl = BLE_GAP_LIM_DISC_SCAN_INT;
        } else {
            disc_params->scan_itvl = BLE_GAP_SCAN_FAST_INTERVAL_MIN;
        }
    }

    if (disc_params->scan_window == 0) {
        if (limited) {
            disc_params->scan_window = BLE_GAP_LIM_DISC_SCAN_WINDOW;
        } else {
            disc_params->scan_window = BLE_GAP_SCAN_FAST_WINDOW;
        }
    }
}

static void
ble_gap_ext_scan_params_to_hci(const struct ble_gap_ext_disc_params *params,
                               struct ble_hs_hci_ext_scan_param *hci_params)
{

    memset(hci_params, 0, sizeof(*hci_params));

    if (params->passive) {
        hci_params->scan_type =  BLE_HCI_SCAN_TYPE_PASSIVE;
    } else {
        hci_params->scan_type = BLE_HCI_SCAN_TYPE_ACTIVE;
    }

    hci_params->scan_itvl = params->itvl;
    hci_params->scan_window = params->window;
}
#endif

int
ble_gap_ext_disc(uint8_t own_addr_type, uint16_t duration, uint16_t period,
                 uint8_t filter_duplicates, uint8_t filter_policy,
                 uint8_t limited,
                 const struct ble_gap_ext_disc_params *uncoded_params,
                 const struct ble_gap_ext_disc_params *coded_params,
                 ble_gap_event_fn *cb, void *cb_arg)
{
#if !NIMBLE_BLE_SCAN || !MYNEWT_VAL(BLE_EXT_ADV)
    return BLE_HS_ENOTSUP;
#else

    struct ble_hs_hci_ext_scan_param ucp;
    struct ble_hs_hci_ext_scan_param cp;
    int rc;

    STATS_INC(ble_gap_stats, discover);

    ble_hs_lock();

    rc = ble_gap_disc_ext_validate(own_addr_type);
    if (rc != 0) {
        goto done;
    }

    /* Make a copy of the parameter structure and fill unspecified values with
     * defaults.
     */

    if (uncoded_params) {
        ble_gap_ext_scan_params_to_hci(uncoded_params, &ucp);
        ble_gap_ext_disc_fill_dflts(limited, &ucp);

        /* XXX: We should do it only once */
        if (!uncoded_params->passive) {
            rc = ble_hs_id_use_addr(own_addr_type);
            if (rc != 0) {
                goto done;
            }
        }
    }

    if (coded_params) {
        ble_gap_ext_scan_params_to_hci(coded_params, &cp);
        ble_gap_ext_disc_fill_dflts(limited, &cp);

        /* XXX: We should do it only once */
        if (!coded_params->passive) {
            rc = ble_hs_id_use_addr(own_addr_type);
            if (rc != 0) {
                goto done;
            }
        }
    }

    ble_gap_master.disc.limited = limited;
    ble_gap_master.cb = cb;
    ble_gap_master.cb_arg = cb_arg;

    rc = ble_gap_ext_disc_tx_params(own_addr_type, filter_policy,
                                    uncoded_params ? &ucp : NULL,
                                    coded_params ? &cp : NULL);
    if (rc != 0) {
        goto done;
    }

    ble_gap_master.op = BLE_GAP_OP_M_DISC;

    rc = ble_gap_ext_disc_enable_tx(1, filter_duplicates, duration, period);
    if (rc != 0) {
        ble_gap_master_reset_state();
        goto done;
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, discover_fail);
    }
    return rc;
#endif
}

#if NIMBLE_BLE_SCAN && !MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_disc_fill_dflts(struct ble_gap_disc_params *disc_params)
{
   if (disc_params->itvl == 0) {
        if (disc_params->limited) {
            disc_params->itvl = BLE_GAP_LIM_DISC_SCAN_INT;
        } else {
            disc_params->itvl = BLE_GAP_SCAN_FAST_INTERVAL_MIN;
        }
    }

    if (disc_params->window == 0) {
        if (disc_params->limited) {
            disc_params->window = BLE_GAP_LIM_DISC_SCAN_WINDOW;
        } else {
            disc_params->window = BLE_GAP_SCAN_FAST_WINDOW;
        }
    }
}

static int
ble_gap_disc_validate(uint8_t own_addr_type,
                      const struct ble_gap_disc_params *disc_params)
{
    if (disc_params == NULL) {
        return BLE_HS_EINVAL;
    }

    return ble_gap_disc_ext_validate(own_addr_type);
}
#endif

int
ble_gap_disc(uint8_t own_addr_type, int32_t duration_ms,
             const struct ble_gap_disc_params *disc_params,
             ble_gap_event_fn *cb, void *cb_arg)
{
#if !NIMBLE_BLE_SCAN
    return BLE_HS_ENOTSUP;
#else

#if MYNEWT_VAL(BLE_EXT_ADV)
    struct ble_gap_ext_disc_params p = {0};

    p.itvl = disc_params->itvl;
    p.passive = disc_params->passive;
    p.window = disc_params->window;

    return ble_gap_ext_disc(own_addr_type, duration_ms/10, 0,
                          disc_params->filter_duplicates,
                          disc_params->filter_policy, disc_params->limited,
                          &p, NULL, cb, cb_arg);
#else
    struct ble_gap_disc_params params;
    uint32_t duration_ticks = 0;
    int rc;

    STATS_INC(ble_gap_stats, discover);

    ble_hs_lock();

    /* Make a copy of the parameter strcuture and fill unspecified values with
     * defaults.
     */
    params = *disc_params;
    ble_gap_disc_fill_dflts(&params);

    rc = ble_gap_disc_validate(own_addr_type, &params);
    if (rc != 0) {
        goto done;
    }

    if (duration_ms == 0) {
        duration_ms = BLE_GAP_DISC_DUR_DFLT;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        rc = ble_npl_time_ms_to_ticks(duration_ms, &duration_ticks);
        if (rc != 0) {
            /* Duration too great. */
            rc = BLE_HS_EINVAL;
            goto done;
        }
    }

    if (!params.passive) {
        rc = ble_hs_id_use_addr(own_addr_type);
        if (rc != 0) {
            goto done;
        }
    }

    ble_gap_master.disc.limited = params.limited;
    ble_gap_master.cb = cb;
    ble_gap_master.cb_arg = cb_arg;

    BLE_HS_LOG(INFO, "GAP procedure initiated: discovery; ");
    ble_gap_log_disc(own_addr_type, duration_ms, &params);
    BLE_HS_LOG(INFO, "\n");

    rc = ble_gap_disc_tx_params(own_addr_type, &params);
    if (rc != 0) {
        goto done;
    }

    ble_gap_master.op = BLE_GAP_OP_M_DISC;

    rc = ble_gap_disc_enable_tx(1, params.filter_duplicates);
    if (rc != 0) {
        ble_gap_master_reset_state();
        goto done;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        ble_gap_master_set_timer(duration_ticks);
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, discover_fail);
    }
    return rc;
#endif
#endif
}

int
ble_gap_disc_active(void)
{
    /* Assume read is atomic; mutex not necessary. */
    return ble_gap_master.op == BLE_GAP_OP_M_DISC;
}

#if !MYNEWT_VAL(BLE_EXT_ADV)
/*****************************************************************************
 * $connection establishment procedures                                      *
 *****************************************************************************/

static int
ble_gap_conn_create_tx(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                       const struct ble_gap_conn_params *params)
{
    uint8_t buf[BLE_HCI_CREATE_CONN_LEN];
    struct hci_create_conn hcc;
    int rc;

    hcc.scan_itvl = params->scan_itvl;
    hcc.scan_window = params->scan_window;

    if (peer_addr == NULL) {
        /* Application wants to connect to any device in the white list.  The
         * peer address type and peer address fields are ignored by the
         * controller; fill them with dummy values.
         */
        hcc.filter_policy = BLE_HCI_CONN_FILT_USE_WL;
        hcc.peer_addr_type = 0;
        memset(hcc.peer_addr, 0, sizeof hcc.peer_addr);
    } else {
        hcc.filter_policy = BLE_HCI_CONN_FILT_NO_WL;
        hcc.peer_addr_type = peer_addr->type;
        memcpy(hcc.peer_addr, peer_addr->val, sizeof hcc.peer_addr);
    }

    hcc.own_addr_type = own_addr_type;
    hcc.conn_itvl_min = params->itvl_min;
    hcc.conn_itvl_max = params->itvl_max;
    hcc.conn_latency = params->latency;
    hcc.supervision_timeout = params->supervision_timeout;
    hcc.min_ce_len = params->min_ce_len;
    hcc.max_ce_len = params->max_ce_len;

    rc = ble_hs_hci_cmd_build_le_create_connection(&hcc, buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_EUNKNOWN;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_CREATE_CONN),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}
#endif

#if MYNEWT_VAL(BLE_EXT_ADV)
static void
ble_gap_copy_params(struct hci_ext_conn_params *hcc_params,
                    const struct ble_gap_conn_params *gap_params)
{
    hcc_params->scan_itvl = gap_params->scan_itvl;
    hcc_params->scan_window = gap_params->scan_window;
    hcc_params->conn_itvl_max = gap_params->itvl_max;
    hcc_params->conn_itvl_min = gap_params->itvl_min;
    hcc_params->max_ce_len = gap_params->max_ce_len;
    hcc_params->min_ce_len = gap_params->min_ce_len;
    hcc_params->conn_latency = gap_params->latency;
    hcc_params->supervision_timeout = gap_params->supervision_timeout;
}

static int
ble_gap_ext_conn_create_tx(
    uint8_t own_addr_type, const ble_addr_t *peer_addr, uint8_t phy_mask,
    const struct ble_gap_conn_params *phy_1m_conn_params,
    const struct ble_gap_conn_params *phy_2m_conn_params,
    const struct ble_gap_conn_params *phy_coded_conn_params)
{
    uint8_t buf[sizeof(struct hci_ext_create_conn)];
    struct hci_ext_create_conn hcc = {0};
    int rc;

    if (peer_addr == NULL) {
        /* Application wants to connect to any device in the white list.  The
         * peer address type and peer address fields are ignored by the
         * controller; fill them with dummy values.
         */
        hcc.filter_policy = BLE_HCI_CONN_FILT_USE_WL;
        hcc.peer_addr_type = 0;
        memset(hcc.peer_addr, 0, sizeof hcc.peer_addr);
    } else {
        hcc.filter_policy = BLE_HCI_CONN_FILT_NO_WL;
        hcc.peer_addr_type = peer_addr->type;;
        memcpy(hcc.peer_addr, peer_addr->val, sizeof hcc.peer_addr);
    }

    hcc.own_addr_type = own_addr_type;

    hcc.init_phy_mask = phy_mask;

    if (phy_mask & BLE_GAP_LE_PHY_1M_MASK) {
        /* XXX same structs */
        ble_gap_copy_params(&hcc.params[0], phy_1m_conn_params);
    }

    if (phy_mask & BLE_GAP_LE_PHY_2M_MASK) {
        /* XXX same structs */
        ble_gap_copy_params(&hcc.params[1], phy_2m_conn_params);
    }

    if (phy_mask & BLE_GAP_LE_PHY_CODED_MASK) {
        /* XXX same structs */
        ble_gap_copy_params(&hcc.params[2], phy_coded_conn_params);
    }

    rc = ble_hs_hci_cmd_build_le_ext_create_conn(&hcc, buf, sizeof buf);
    if (rc != 0) {
        return BLE_HS_EUNKNOWN;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_EXT_CREATE_CONN),
        buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/**
 * Initiates a connect procedure.
 *
 * @param own_addr_type         The type of address the stack should use for
 *                                  itself during connection establishment.
 *                                      o BLE_OWN_ADDR_PUBLIC
 *                                      o BLE_OWN_ADDR_RANDOM
 *                                      o BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
 *                                      o BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
 * @param peer_addr             The address of the peer to connect to.
 *                                  If this parameter is NULL, the white list
 *                                  is used.
 * @param duration_ms           The duration of the discovery procedure.
 *                                  On expiration, the procedure ends and a
 *                                  BLE_GAP_EVENT_DISC_COMPLETE event is
 *                                  reported.  Units are milliseconds.
 * @param phy_mask              Define on which PHYs connection attempt should
 *                                  be done
 * @param phy_1m_conn_params     Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_1M_MASK is set in phy_mask
 *                                  this parameter can be specify to null for
 *                                  default values.
 * @param phy_2m_conn_params     Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_2M_MASK is set in phy_mask
 *                                  this parameter can be specify to null for
 *                                  default values.
 * @param phy_coded_conn_params  Additional arguments specifying the
 *                                  particulars of the connect procedure. When
 *                                  BLE_GAP_LE_PHY_CODED_MASK is set in
 *                                  phy_mask this parameter can be specify to
 *                                  null for default values.
 * @param cb                    The callback to associate with this connect
 *                                  procedure.  When the connect procedure
 *                                  completes, the result is reported through
 *                                  this callback.  If the connect procedure
 *                                  succeeds, the connection inherits this
 *                                  callback as its event-reporting mechanism.
 * @param cb_arg                The optional argument to pass to the callback
 *                                  function.
 *
 * @return                      0 on success;
 *                              BLE_HS_EALREADY if a connection attempt is
 *                                  already in progress;
 *                              BLE_HS_EBUSY if initiating a connection is not
 *                                  possible because scanning is in progress;
 *                              BLE_HS_EDONE if the specified peer is already
 *                                  connected;
 *                              Other nonzero on error.
 */
int
ble_gap_ext_connect(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                int32_t duration_ms, uint8_t phy_mask,
                const struct ble_gap_conn_params *phy_1m_conn_params,
                const struct ble_gap_conn_params *phy_2m_conn_params,
                const struct ble_gap_conn_params *phy_coded_conn_params,
                ble_gap_event_fn *cb, void *cb_arg)
{
#if !MYNEWT_VAL(BLE_ROLE_CENTRAL)
    return BLE_HS_ENOTSUP;
#endif

    ble_npl_time_t duration_ticks;
    int rc;

    STATS_INC(ble_gap_stats, initiate);

    ble_hs_lock();

    if (ble_gap_conn_active()) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    if (ble_gap_disc_active()) {
        rc = BLE_HS_EBUSY;
        goto done;
    }

    if (!ble_hs_is_enabled()) {
        return BLE_HS_EDISABLED;
    }

    if (ble_gap_is_preempted()) {
        rc = BLE_HS_EPREEMPTED;
        goto done;
    }

    if (!ble_hs_conn_can_alloc()) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    if (peer_addr &&
        peer_addr->type != BLE_ADDR_PUBLIC &&
        peer_addr->type != BLE_ADDR_RANDOM &&
        peer_addr->type != BLE_ADDR_PUBLIC_ID &&
        peer_addr->type != BLE_ADDR_RANDOM_ID) {

        rc = BLE_HS_EINVAL;
        goto done;
    }

    if ((phy_mask & BLE_GAP_LE_PHY_1M_MASK) && phy_1m_conn_params == NULL) {
        phy_1m_conn_params = &ble_gap_conn_params_dflt;
    }

    if ((phy_mask & BLE_GAP_LE_PHY_2M_MASK) && phy_2m_conn_params == NULL) {
        phy_2m_conn_params = &ble_gap_conn_params_dflt;
    }

    if ((phy_mask & BLE_GAP_LE_PHY_CODED_MASK) &&
        phy_coded_conn_params == NULL) {

        phy_coded_conn_params = &ble_gap_conn_params_dflt;
    }

    if (duration_ms == 0) {
        duration_ms = BLE_GAP_CONN_DUR_DFLT;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        rc = ble_npl_time_ms_to_ticks(duration_ms, &duration_ticks);
        if (rc != 0) {
            /* Duration too great. */
            rc = BLE_HS_EINVAL;
            goto done;
        }
    }

    /* Verify peer not already connected. */
    if (ble_hs_conn_find_by_addr(peer_addr) != NULL) {
        rc = BLE_HS_EDONE;
        goto done;
    }

    /* XXX: Verify conn_params. */

    rc = ble_hs_id_use_addr(own_addr_type);
    if (rc != 0) {
        goto done;
    }

    ble_gap_master.cb = cb;
    ble_gap_master.cb_arg = cb_arg;
    ble_gap_master.conn.using_wl = peer_addr == NULL;
    ble_gap_master.conn.our_addr_type = own_addr_type;

    ble_gap_master.op = BLE_GAP_OP_M_CONN;

    rc = ble_gap_ext_conn_create_tx(own_addr_type, peer_addr, phy_mask,
                                    phy_1m_conn_params, phy_2m_conn_params,
                                    phy_coded_conn_params);
    if (rc != 0) {
        ble_gap_master_reset_state();
        goto done;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        ble_gap_master_set_timer(duration_ticks);
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, initiate_fail);
    }
    return rc;
}
#endif

int
ble_gap_connect(uint8_t own_addr_type, const ble_addr_t *peer_addr,
                int32_t duration_ms,
                const struct ble_gap_conn_params *conn_params,
                ble_gap_event_fn *cb, void *cb_arg)
{
#if !MYNEWT_VAL(BLE_ROLE_CENTRAL)
    return BLE_HS_ENOTSUP;
#endif

#if MYNEWT_VAL(BLE_EXT_ADV)
    return ble_gap_ext_connect(own_addr_type, peer_addr, duration_ms,
                               BLE_GAP_LE_PHY_1M_MASK,
                               conn_params, NULL, NULL, cb, cb_arg);
#else
    uint32_t duration_ticks;
    int rc;

    STATS_INC(ble_gap_stats, initiate);

    ble_hs_lock();

    if (ble_gap_conn_active()) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    if (ble_gap_disc_active()) {
        rc = BLE_HS_EBUSY;
        goto done;
    }

    if (!ble_hs_is_enabled()) {
        rc = BLE_HS_EDISABLED;
        goto done;
    }

    if (ble_gap_is_preempted()) {
        rc = BLE_HS_EPREEMPTED;
        goto done;
    }

    if (!ble_hs_conn_can_alloc()) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    if (peer_addr &&
        peer_addr->type != BLE_ADDR_PUBLIC &&
        peer_addr->type != BLE_ADDR_RANDOM &&
        peer_addr->type != BLE_ADDR_PUBLIC_ID &&
        peer_addr->type != BLE_ADDR_RANDOM_ID) {

        rc = BLE_HS_EINVAL;
        goto done;
    }

    if (conn_params == NULL) {
        conn_params = &ble_gap_conn_params_dflt;
    }

    if (duration_ms == 0) {
        duration_ms = BLE_GAP_CONN_DUR_DFLT;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        rc = ble_npl_time_ms_to_ticks(duration_ms, &duration_ticks);
        if (rc != 0) {
            /* Duration too great. */
            rc = BLE_HS_EINVAL;
            goto done;
        }
    }

    /* Verify peer not already connected. */
    if (ble_hs_conn_find_by_addr(peer_addr) != NULL) {
        rc = BLE_HS_EDONE;
        goto done;
    }

    /* XXX: Verify conn_params. */

    rc = ble_hs_id_use_addr(own_addr_type);
    if (rc != 0) {
        goto done;
    }

    BLE_HS_LOG(INFO, "GAP procedure initiated: connect; ");
    ble_gap_log_conn(own_addr_type, peer_addr, conn_params);
    BLE_HS_LOG(INFO, "\n");

    ble_gap_master.cb = cb;
    ble_gap_master.cb_arg = cb_arg;
    ble_gap_master.conn.using_wl = peer_addr == NULL;
    ble_gap_master.conn.our_addr_type = own_addr_type;

    ble_gap_master.op = BLE_GAP_OP_M_CONN;

    rc = ble_gap_conn_create_tx(own_addr_type, peer_addr,
                                conn_params);
    if (rc != 0) {
        ble_gap_master_reset_state();
        goto done;
    }

    if (duration_ms != BLE_HS_FOREVER) {
        ble_gap_master_set_timer(duration_ticks);
    }

    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, initiate_fail);
    }
    return rc;
#endif
}

int
ble_gap_conn_active(void)
{
    /* Assume read is atomic; mutex not necessary. */
    return ble_gap_master.op == BLE_GAP_OP_M_CONN;
}

/*****************************************************************************
 * $terminate connection procedure                                           *
 *****************************************************************************/

int
ble_gap_terminate(uint16_t conn_handle, uint8_t hci_reason)
{
    uint8_t buf[BLE_HCI_DISCONNECT_CMD_LEN];
    struct ble_hs_conn *conn;
    int rc;

    STATS_INC(ble_gap_stats, terminate);

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        rc = BLE_HS_ENOTCONN;
        goto done;
    }

    if (conn->bhc_flags & BLE_HS_CONN_F_TERMINATING) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    BLE_HS_LOG(INFO, "GAP procedure initiated: terminate connection; "
                     "conn_handle=%d hci_reason=%d\n",
               conn_handle, hci_reason);

    ble_hs_hci_cmd_build_disconnect(conn_handle, hci_reason,
                                    buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LINK_CTRL,
                                                BLE_HCI_OCF_DISCONNECT_CMD),
                                     buf, sizeof(buf));
    if (rc != 0) {
        goto done;
    }

    conn->bhc_flags |= BLE_HS_CONN_F_TERMINATING;
    rc = 0;

done:
    ble_hs_unlock();

    if (rc != 0) {
        STATS_INC(ble_gap_stats, terminate_fail);
    }
    return rc;
}

/*****************************************************************************
 * $cancel                                                                   *
 *****************************************************************************/

static int
ble_gap_conn_cancel_tx(void)
{
    int rc;

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                            BLE_HCI_OCF_LE_CREATE_CONN_CANCEL),
                                     NULL, 0);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_conn_cancel_no_lock(void)
{
#if !MYNEWT_VAL(BLE_ROLE_CENTRAL)
    return BLE_HS_ENOTSUP;
#endif

    int rc;

    STATS_INC(ble_gap_stats, cancel);

    if (!ble_gap_conn_active()) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    BLE_HS_LOG(INFO, "GAP procedure initiated: cancel connection\n");

    rc = ble_gap_conn_cancel_tx();
    if (rc != 0) {
        goto done;
    }

    ble_gap_master.conn.cancel = 1;
    rc = 0;

done:
    if (rc != 0) {
        STATS_INC(ble_gap_stats, cancel_fail);
    }

    return rc;
}

int
ble_gap_conn_cancel(void)
{
#if !MYNEWT_VAL(BLE_ROLE_CENTRAL)
    return BLE_HS_ENOTSUP;
#endif

    int rc;

    ble_hs_lock();
    rc = ble_gap_conn_cancel_no_lock();
    ble_hs_unlock();

    return rc;
}

/*****************************************************************************
 * $update connection parameters                                             *
 *****************************************************************************/

static struct ble_gap_update_entry *
ble_gap_update_entry_alloc(void)
{
    struct ble_gap_update_entry *entry;

    entry = os_memblock_get(&ble_gap_update_entry_pool);
    if (entry != NULL) {
        memset(entry, 0, sizeof *entry);
    }

    return entry;
}

static void
ble_gap_update_entry_free(struct ble_gap_update_entry *entry)
{
    int rc;

    if (entry != NULL) {
#if MYNEWT_VAL(BLE_HS_DEBUG)
        memset(entry, 0xff, sizeof *entry);
#endif
        rc = os_memblock_put(&ble_gap_update_entry_pool, entry);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
    }
}

static struct ble_gap_update_entry *
ble_gap_update_entry_find(uint16_t conn_handle,
                          struct ble_gap_update_entry **out_prev)
{
    struct ble_gap_update_entry *entry;
    struct ble_gap_update_entry *prev;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    prev = NULL;
    SLIST_FOREACH(entry, &ble_gap_update_entries, next) {
        if (entry->conn_handle == conn_handle) {
            break;
        }

        prev = entry;
    }

    if (out_prev != NULL) {
        *out_prev = prev;
    }

    return entry;
}

static struct ble_gap_update_entry *
ble_gap_update_entry_remove(uint16_t conn_handle)
{
    struct ble_gap_update_entry *entry;
    struct ble_gap_update_entry *prev;

    entry = ble_gap_update_entry_find(conn_handle, &prev);
    if (entry != NULL) {
        if (prev == NULL) {
            SLIST_REMOVE_HEAD(&ble_gap_update_entries, next);
        } else {
            SLIST_NEXT(prev, next) = SLIST_NEXT(entry, next);
        }
        ble_hs_timer_resched();
    }

    return entry;
}

static void
ble_gap_update_l2cap_cb(uint16_t conn_handle, int status, void *arg)
{
    struct ble_gap_update_entry *entry;

    /* Report failures and rejections.  Success gets reported when the
     * controller sends the connection update complete event.
     */

    ble_hs_lock();
    entry = ble_gap_update_entry_remove(conn_handle);
    ble_hs_unlock();

    if (entry != NULL) {
        ble_gap_update_entry_free(entry);
        if (status != 0) {
            ble_gap_update_notify(conn_handle, status);
        }
        /* On success let's wait for the controller to notify about update */
    }
}

static int
ble_gap_tx_param_pos_reply(uint16_t conn_handle,
                           struct ble_gap_upd_params *params)
{
    uint8_t buf[BLE_HCI_CONN_PARAM_REPLY_LEN];
    struct hci_conn_param_reply pos_reply;
    int rc;

    pos_reply.handle = conn_handle;
    pos_reply.conn_itvl_min = params->itvl_min;
    pos_reply.conn_itvl_max = params->itvl_max;
    pos_reply.conn_latency = params->latency;
    pos_reply.supervision_timeout = params->supervision_timeout;
    pos_reply.min_ce_len = params->min_ce_len;
    pos_reply.max_ce_len = params->max_ce_len;

    ble_hs_hci_cmd_build_le_conn_param_reply(&pos_reply, buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx_empty_ack(
        BLE_HCI_OP(BLE_HCI_OGF_LE, BLE_HCI_OCF_LE_REM_CONN_PARAM_RR),
        buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_gap_tx_param_neg_reply(uint16_t conn_handle, uint8_t reject_reason)
{
    uint8_t buf[BLE_HCI_CONN_PARAM_NEG_REPLY_LEN];
    struct hci_conn_param_neg_reply neg_reply;
    int rc;

    neg_reply.handle = conn_handle;
    neg_reply.reason = reject_reason;

    ble_hs_hci_cmd_build_le_conn_param_neg_reply(&neg_reply, buf, sizeof buf);
    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                         BLE_HCI_OCF_LE_REM_CONN_PARAM_NRR),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

void
ble_gap_rx_param_req(struct hci_le_conn_param_req *evt)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    struct ble_gap_upd_params peer_params;
    struct ble_gap_upd_params self_params;
    struct ble_gap_event event;
    uint8_t reject_reason;
    int rc;

    reject_reason = 0; /* Silence warning. */

    memset(&event, 0, sizeof event);

    peer_params.itvl_min = evt->itvl_min;
    peer_params.itvl_max = evt->itvl_max;
    peer_params.latency = evt->latency;
    peer_params.supervision_timeout = evt->timeout;
    peer_params.min_ce_len = 0;
    peer_params.max_ce_len = 0;

    /* Copy the peer params into the self params to make it easy on the
     * application.  The application callback will change only the fields which
     * it finds unsuitable.
     */
    self_params = peer_params;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_CONN_UPDATE_REQ;
    event.conn_update_req.conn_handle = evt->connection_handle;
    event.conn_update_req.self_params = &self_params;
    event.conn_update_req.peer_params = &peer_params;
    rc = ble_gap_call_conn_event_cb(&event, evt->connection_handle);
    if (rc != 0) {
        reject_reason = rc;
    }

    if (rc == 0) {
        rc = ble_gap_tx_param_pos_reply(evt->connection_handle, &self_params);
        if (rc != 0) {
            ble_gap_update_failed(evt->connection_handle, rc);
        }
    } else {
        ble_gap_tx_param_neg_reply(evt->connection_handle, reject_reason);
    }
}

static int
ble_gap_update_tx(uint16_t conn_handle,
                  const struct ble_gap_upd_params *params)
{
    uint8_t buf[BLE_HCI_CONN_UPDATE_LEN];
    struct hci_conn_update cmd;
    int rc;

    cmd.handle = conn_handle;
    cmd.conn_itvl_min = params->itvl_min;
    cmd.conn_itvl_max = params->itvl_max;
    cmd.conn_latency = params->latency;
    cmd.supervision_timeout = params->supervision_timeout;
    cmd.min_ce_len = params->min_ce_len;
    cmd.max_ce_len = params->max_ce_len;

    rc = ble_hs_hci_cmd_build_le_conn_update(&cmd, buf, sizeof buf);
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx_empty_ack(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                                BLE_HCI_OCF_LE_CONN_UPDATE),
                                     buf, sizeof(buf));
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static bool
ble_gap_validate_conn_params(const struct ble_gap_upd_params *params)
{

    /* Requirements from Bluetooth spec. v4.2 [Vol 2, Part E], 7.8.18 */
    if (params->itvl_min > params->itvl_max) {
        return false;
    }

    if (params->itvl_min < 0x0006 || params->itvl_max > 0x0C80) {
        return false;
    }

    if (params->latency > 0x01F3) {
        return false;
    }

    /* According to specification mentioned above we should make sure that:
     * supervision_timeout_ms > (1 + latency) * 2 * max_interval_ms
     *    =>
     * supervision_timeout * 10 ms > (1 + latency) * 2 * itvl_max * 1.25ms
     */
    if (params->supervision_timeout <=
                   (((1 + params->latency) * params->itvl_max) / 4)) {
        return false;
    }

    return true;
}

int
ble_gap_update_params(uint16_t conn_handle,
                      const struct ble_gap_upd_params *params)
{
#if !NIMBLE_BLE_CONNECT
    return BLE_HS_ENOTSUP;
#endif

    struct ble_l2cap_sig_update_params l2cap_params;
    struct ble_gap_update_entry *entry;
    struct ble_gap_update_entry *dup;
    struct ble_hs_conn *conn;
    int l2cap_update;
    int rc;

    l2cap_update = 0;

    /* Validate parameters with a spec */
    if (!ble_gap_validate_conn_params(params)) {
        return BLE_HS_EINVAL;
    }

    STATS_INC(ble_gap_stats, update);
    memset(&l2cap_params, 0, sizeof l2cap_params);
    entry = NULL;

    ble_hs_lock();

    conn = ble_hs_conn_find(conn_handle);
    if (conn == NULL) {
        rc = BLE_HS_ENOTCONN;
        goto done;
    }

    /* Don't allow two concurrent updates to the same connection. */
    dup = ble_gap_update_entry_find(conn_handle, NULL);
    if (dup != NULL) {
        rc = BLE_HS_EALREADY;
        goto done;
    }

    entry = ble_gap_update_entry_alloc();
    if (entry == NULL) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    entry->conn_handle = conn_handle;
    entry->params = *params;

    entry->exp_os_ticks = ble_npl_time_get() +
                          ble_npl_time_ms_to_ticks32(BLE_GAP_UPDATE_TIMEOUT_MS);

    BLE_HS_LOG(INFO, "GAP procedure initiated: ");
    ble_gap_log_update(conn_handle, params);
    BLE_HS_LOG(INFO, "\n");

    /*
     * If LL update procedure is not supported on this connection and we are
     * the slave, fail over to the L2CAP update procedure.
     */
    if ((conn->supported_feat & BLE_HS_HCI_LE_FEAT_CONN_PARAM_REQUEST) == 0 &&
            !(conn->bhc_flags & BLE_HS_CONN_F_MASTER)) {
        l2cap_update = 1;
        rc = 0;
    } else {
        rc = ble_gap_update_tx(conn_handle, params);
    }

done:
    ble_hs_unlock();

    if (!l2cap_update) {
        ble_hs_timer_resched();
    } else {
        ble_gap_update_to_l2cap(params, &l2cap_params);

        rc = ble_l2cap_sig_update(conn_handle, &l2cap_params,
                                              ble_gap_update_l2cap_cb, NULL);
    }

    ble_hs_lock();
    if (rc == 0) {
        SLIST_INSERT_HEAD(&ble_gap_update_entries, entry, next);
    } else {
        ble_gap_update_entry_free(entry);
        STATS_INC(ble_gap_stats, update_fail);
    }
    ble_hs_unlock();

    return rc;
}

/*****************************************************************************
 * $security                                                                 *
 *****************************************************************************/
int
ble_gap_security_initiate(uint16_t conn_handle)
{
#if !NIMBLE_BLE_SM
    return BLE_HS_ENOTSUP;
#endif

    struct ble_store_value_sec value_sec;
    struct ble_store_key_sec key_sec;
    struct ble_hs_conn_addrs addrs;
    ble_hs_conn_flags_t conn_flags;
    struct ble_hs_conn *conn;
    int rc;

    STATS_INC(ble_gap_stats, security_initiate);

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        conn_flags = conn->bhc_flags;
        ble_hs_conn_addrs(conn, &addrs);

        memset(&key_sec, 0, sizeof key_sec);
        key_sec.peer_addr = addrs.peer_id_addr;
    }
    ble_hs_unlock();

    if (conn == NULL) {
        rc = BLE_HS_ENOTCONN;
        goto done;
    }

    if (conn_flags & BLE_HS_CONN_F_MASTER) {
        /* Search the security database for an LTK for this peer.  If one
         * is found, perform the encryption procedure rather than the pairing
         * procedure.
         */
        rc = ble_store_read_peer_sec(&key_sec, &value_sec);
        if (rc == 0 && value_sec.ltk_present) {
            rc = ble_sm_enc_initiate(conn_handle, value_sec.key_size,
                                     value_sec.ltk, value_sec.ediv,
                                     value_sec.rand_num,
                                     value_sec.authenticated);
            if (rc != 0) {
                goto done;
            }
        } else {
            rc = ble_sm_pair_initiate(conn_handle);
            if (rc != 0) {
                goto done;
            }
        }
    } else {
        rc = ble_sm_slave_initiate(conn_handle);
        if (rc != 0) {
            goto done;
        }
    }

    rc = 0;

done:
    if (rc != 0) {
        STATS_INC(ble_gap_stats, security_initiate_fail);
    }

    return rc;
}

int
ble_gap_pair_initiate(uint16_t conn_handle)
{
    int rc;

    rc = ble_sm_pair_initiate(conn_handle);

    return rc;
}

int
ble_gap_encryption_initiate(uint16_t conn_handle,
                            uint8_t key_size,
                            const uint8_t *ltk,
                            uint16_t ediv,
                            uint64_t rand_val,
                            int auth)
{
#if !NIMBLE_BLE_SM
    return BLE_HS_ENOTSUP;
#endif

    ble_hs_conn_flags_t conn_flags;
    int rc;

    rc = ble_hs_atomic_conn_flags(conn_handle, &conn_flags);
    if (rc != 0) {
        return rc;
    }

    if (!(conn_flags & BLE_HS_CONN_F_MASTER)) {
        return BLE_HS_EROLE;
    }

    rc = ble_sm_enc_initiate(conn_handle, key_size, ltk,
                             ediv, rand_val, auth);
    return rc;
}

int
ble_gap_unpair(const ble_addr_t *peer_addr)
{
    struct ble_hs_conn *conn;

    if (ble_addr_cmp(peer_addr, BLE_ADDR_ANY) == 0) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();

    conn = ble_hs_conn_find_by_addr(peer_addr);
    if (conn != NULL) {
        ble_gap_terminate(conn->bhc_handle, BLE_ERR_REM_USER_CONN_TERM);
    }

    ble_hs_unlock();

    ble_hs_pvcy_remove_entry(peer_addr->type,
                             peer_addr->val);

    return ble_store_util_delete_peer(peer_addr);
}

int
ble_gap_unpair_oldest_peer(void)
{
    ble_addr_t oldest_peer_id_addr;
    int num_peers;
    int rc;

    rc = ble_store_util_bonded_peers(
            &oldest_peer_id_addr, &num_peers, 1);
    if (rc != 0) {
        return rc;
    }

    if (num_peers == 0) {
        return 0;
    }

    rc = ble_gap_unpair(&oldest_peer_id_addr);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

int
ble_gap_unpair_oldest_except(const ble_addr_t *peer_addr)
{
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_peers;
    int rc, i;

    rc = ble_store_util_bonded_peers(
            &peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return rc;
    }

    if (num_peers == 0) {
        return BLE_HS_ENOENT;
    }

    for (i = 0; i < num_peers; i++) {
        if (ble_addr_cmp(peer_addr, &peer_id_addrs[i]) != 0) {
            break;
        }
    }

    if (i >= num_peers) {
        return BLE_HS_ENOMEM;
    }

    return ble_gap_unpair(&peer_id_addrs[i]);
}

void
ble_gap_passkey_event(uint16_t conn_handle,
                      struct ble_gap_passkey_params *passkey_params)
{
#if !NIMBLE_BLE_SM
    return;
#endif

    struct ble_gap_event event;

    BLE_HS_LOG(DEBUG, "send passkey action request %d\n",
               passkey_params->action);

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_PASSKEY_ACTION;
    event.passkey.conn_handle = conn_handle;
    event.passkey.params = *passkey_params;
    ble_gap_call_conn_event_cb(&event, conn_handle);
}

void
ble_gap_enc_event(uint16_t conn_handle, int status,
                  int security_restored, int bonded)
{
#if !NIMBLE_BLE_SM
    return;
#endif

    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_ENC_CHANGE;
    event.enc_change.conn_handle = conn_handle;
    event.enc_change.status = status;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, conn_handle);

    if (status != 0) {
        return;
    }

    /* If encryption succeded and encryption has been restored for bonded device,
     * notify gatt server so it has chance to send notification/indication if needed.
     */
    if (security_restored) {
        ble_gatts_bonding_restored(conn_handle);
        return;
    }

    /* If this is fresh pairing and bonding has been established,
     * notify gatt server about that so previous subscriptions (before bonding)
     * can be stored.
     */
    if (bonded) {
        ble_gatts_bonding_established(conn_handle);
    }
}

void
ble_gap_identity_event(uint16_t conn_handle)
{
#if !NIMBLE_BLE_SM
    return;
#endif

    struct ble_gap_event event;

    BLE_HS_LOG(DEBUG, "send identity changed");

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_IDENTITY_RESOLVED;
    event.identity_resolved.conn_handle = conn_handle;
    ble_gap_call_conn_event_cb(&event, conn_handle);
}

int
ble_gap_repeat_pairing_event(const struct ble_gap_repeat_pairing *rp)
{
#if !NIMBLE_BLE_SM
    return 0;
#endif

    struct ble_gap_event event;
    int rc;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_REPEAT_PAIRING;
    event.repeat_pairing = *rp;
    rc = ble_gap_call_conn_event_cb(&event, rp->conn_handle);
    return rc;
}

/*****************************************************************************
 * $rssi                                                                     *
 *****************************************************************************/

int
ble_gap_conn_rssi(uint16_t conn_handle, int8_t *out_rssi)
{
    int rc;

    rc = ble_hs_hci_util_read_rssi(conn_handle, out_rssi);
    return rc;
}

/*****************************************************************************
 * $notify                                                                   *
 *****************************************************************************/

void
ble_gap_notify_rx_event(uint16_t conn_handle, uint16_t attr_handle,
                        struct os_mbuf *om, int is_indication)
{
#if !MYNEWT_VAL(BLE_GATT_NOTIFY) && !MYNEWT_VAL(BLE_GATT_INDICATE)
    return;
#endif

    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_NOTIFY_RX;
    event.notify_rx.conn_handle = conn_handle;
    event.notify_rx.attr_handle = attr_handle;
    event.notify_rx.om = om;
    event.notify_rx.indication = is_indication;
    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, conn_handle);

    os_mbuf_free_chain(event.notify_rx.om);
}

void
ble_gap_notify_tx_event(int status, uint16_t conn_handle, uint16_t attr_handle,
                        int is_indication)
{
#if !MYNEWT_VAL(BLE_GATT_NOTIFY) && !MYNEWT_VAL(BLE_GATT_INDICATE)
    return;
#endif

    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_NOTIFY_TX;
    event.notify_tx.conn_handle = conn_handle;
    event.notify_tx.status = status;
    event.notify_tx.attr_handle = attr_handle;
    event.notify_tx.indication = is_indication;
    ble_gap_call_conn_event_cb(&event, conn_handle);
}

/*****************************************************************************
 * $subscribe                                                                *
 *****************************************************************************/

void
ble_gap_subscribe_event(uint16_t conn_handle, uint16_t attr_handle,
                        uint8_t reason,
                        uint8_t prev_notify, uint8_t cur_notify,
                        uint8_t prev_indicate, uint8_t cur_indicate)
{
    struct ble_gap_event event;

    BLE_HS_DBG_ASSERT(prev_notify != cur_notify ||
                      prev_indicate != cur_indicate);
    BLE_HS_DBG_ASSERT(reason == BLE_GAP_SUBSCRIBE_REASON_WRITE ||
                      reason == BLE_GAP_SUBSCRIBE_REASON_TERM  ||
                      reason == BLE_GAP_SUBSCRIBE_REASON_RESTORE);

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_SUBSCRIBE;
    event.subscribe.conn_handle = conn_handle;
    event.subscribe.attr_handle = attr_handle;
    event.subscribe.reason = reason;
    event.subscribe.prev_notify = !!prev_notify;
    event.subscribe.cur_notify = !!cur_notify;
    event.subscribe.prev_indicate = !!prev_indicate;
    event.subscribe.cur_indicate = !!cur_indicate;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, conn_handle);
}

/*****************************************************************************
 * $mtu                                                                      *
 *****************************************************************************/

void
ble_gap_mtu_event(uint16_t conn_handle, uint16_t cid, uint16_t mtu)
{
    struct ble_gap_event event;

    memset(&event, 0, sizeof event);
    event.type = BLE_GAP_EVENT_MTU;
    event.mtu.conn_handle = conn_handle;
    event.mtu.channel_id = cid;
    event.mtu.value = mtu;

    ble_gap_event_listener_call(&event);
    ble_gap_call_conn_event_cb(&event, conn_handle);
}

/*****************************************************************************
 * $preempt                                                                  *
 *****************************************************************************/

void
ble_gap_preempt_no_lock(void)
{
    int rc;
    int i;

    (void)rc;
    (void)i;

#if NIMBLE_BLE_ADVERTISE
#if MYNEWT_VAL(BLE_EXT_ADV)
    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        rc = ble_gap_ext_adv_stop_no_lock(i);
        if (rc == 0) {
            ble_gap_slave[i].preempted = 1;
        }
    }
#else
    rc = ble_gap_adv_stop_no_lock();
    if (rc == 0) {
        ble_gap_slave[0].preempted = 1;
    }
#endif
#endif

#if NIMBLE_BLE_CONNECT
    rc = ble_gap_conn_cancel_no_lock();
    if (rc == 0) {
        ble_gap_master.preempted_op = BLE_GAP_OP_M_CONN;
    }
#endif

#if NIMBLE_BLE_SCAN
    rc = ble_gap_disc_cancel_no_lock();
    if (rc == 0) {
        ble_gap_master.preempted_op = BLE_GAP_OP_M_DISC;
    }
#endif
}

/**
 * @brief Preempts the GAP if it is not already preempted.
 *
 * Aborts all active GAP procedures and prevents new ones from being started.
 * This function is used to ensure an idle GAP so that the controller's
 * resolving list can be modified.  When done accessing the resolving list, the
 * caller must call `ble_gap_preempt_done()` to permit new GAP procedures.
 *
 * On preemption, all aborted GAP procedures are reported with a status or
 * reason code of BLE_HS_EPREEMPTED.  An attempt to initiate a new GAP
 * procedure during preemption fails with a return code of BLE_HS_EPREEMPTED.
 */
void
ble_gap_preempt(void)
{
    ble_hs_lock();

    if (!ble_gap_is_preempted()) {
        ble_gap_preempt_no_lock();
    }

    ble_hs_unlock();
}

/**
 * Takes GAP out of the preempted state, allowing new GAP procedures to be
 * initiated.  This function should only be called after a call to
 * `ble_gap_preempt()`.
 */

static struct ble_npl_mutex preempt_done_mutex;

void
ble_gap_preempt_done(void)
{
    struct ble_gap_event event;
    ble_gap_event_fn *master_cb;
    void *master_arg;
    int disc_preempted;
    int i;
    static struct {
        ble_gap_event_fn *cb;
        void *arg;
    } slaves[BLE_ADV_INSTANCES];

    disc_preempted = 0;

    /* Protects slaves from accessing by multiple threads */
    ble_npl_mutex_pend(&preempt_done_mutex, 0xFFFFFFFF);
    memset(slaves, 0, sizeof(slaves));

    ble_hs_lock();

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (ble_gap_slave[i].preempted) {
            ble_gap_slave[i].preempted = 0;
            slaves[i].cb = ble_gap_slave[i].cb;
            slaves[i].arg = ble_gap_slave[i].cb_arg;
        }
    }

    if (ble_gap_master.preempted_op == BLE_GAP_OP_M_DISC) {
        ble_gap_master.preempted_op = BLE_GAP_OP_NULL;
        disc_preempted = 1;
        master_cb = ble_gap_master.cb;
        master_arg = ble_gap_master.cb_arg;
    }

    ble_hs_unlock();

    event.type = BLE_GAP_EVENT_ADV_COMPLETE;
    event.adv_complete.reason = BLE_HS_EPREEMPTED;

    for (i = 0; i < BLE_ADV_INSTANCES; i++) {
        if (slaves[i].cb) {
#if MYNEWT_VAL(BLE_EXT_ADV)
            event.adv_complete.instance = i;
            event.adv_complete.conn_handle = i;
#endif
            ble_gap_call_event_cb(&event, slaves[i].cb, slaves[i].arg);
        }
    }
    ble_npl_mutex_release(&preempt_done_mutex);

    if (disc_preempted) {
        event.type = BLE_GAP_EVENT_DISC_COMPLETE;
        event.disc_complete.reason = BLE_HS_EPREEMPTED;
        ble_gap_call_event_cb(&event, master_cb, master_arg);
    }
}

int
ble_gap_event_listener_register(struct ble_gap_event_listener *listener,
                                ble_gap_event_fn *fn, void *arg)
{
    struct ble_gap_event_listener *evl = NULL;
    int rc;

    SLIST_FOREACH(evl, &ble_gap_event_listener_list, link) {
        if (evl == listener) {
            break;
        }
    }

    if (!evl) {
        if (fn) {
            memset(listener, 0, sizeof(*listener));
            listener->fn = fn;
            listener->arg = arg;
            SLIST_INSERT_HEAD(&ble_gap_event_listener_list, listener, link);
            rc = 0;
        } else {
            rc = BLE_HS_EINVAL;
        }
    } else {
        rc = BLE_HS_EALREADY;
    }

    return rc;
}

int
ble_gap_event_listener_unregister(struct ble_gap_event_listener *listener)
{
    struct ble_gap_event_listener *evl = NULL;
    int rc;

    /*
     * We check if element exists on the list only for sanity to let caller
     * know whether it registered its listener before.
     */

    SLIST_FOREACH(evl, &ble_gap_event_listener_list, link) {
        if (evl == listener) {
            break;
        }
    }

    if (!evl) {
        rc = BLE_HS_ENOENT;
    } else {
        SLIST_REMOVE(&ble_gap_event_listener_list, listener,
                     ble_gap_event_listener, link);
        rc = 0;
    }

    return rc;
}

static int
ble_gap_event_listener_call(struct ble_gap_event *event)
{
    struct ble_gap_event_listener *evl = NULL;

    SLIST_FOREACH(evl, &ble_gap_event_listener_list, link) {
        evl->fn(event, evl->arg);
    }

    return 0;
}

/*****************************************************************************
 * $init                                                                     *
 *****************************************************************************/

int
ble_gap_init(void)
{
    int rc;

    memset(&ble_gap_master, 0, sizeof(ble_gap_master));
    memset(ble_gap_slave, 0, sizeof(ble_gap_slave));

#if MYNEWT_VAL(BLE_PERIODIC_ADV)
    memset(&ble_gap_sync, 0, sizeof(ble_gap_sync));
#endif

    ble_npl_mutex_init(&preempt_done_mutex);

    SLIST_INIT(&ble_gap_update_entries);
    SLIST_INIT(&ble_gap_event_listener_list);

    rc = os_mempool_init(&ble_gap_update_entry_pool,
                         MYNEWT_VAL(BLE_GAP_MAX_PENDING_CONN_PARAM_UPDATE),
                         sizeof (struct ble_gap_update_entry),
                         ble_gap_update_entry_mem,
                         "ble_gap_update");
    switch (rc) {
    case 0:
        break;
    case OS_ENOMEM:
        rc = BLE_HS_ENOMEM;
        goto err;
    default:
        rc = BLE_HS_EOS;
        goto err;
    }

    rc = stats_init_and_reg(
        STATS_HDR(ble_gap_stats), STATS_SIZE_INIT_PARMS(ble_gap_stats,
        STATS_SIZE_32), STATS_NAME_INIT_PARMS(ble_gap_stats), "ble_gap");
    if (rc != 0) {
        goto err;
    }

    return 0;

err:
    return rc;
}

void
ble_gap_deinit(void)
{
    ble_npl_mutex_deinit(&preempt_done_mutex);
}
