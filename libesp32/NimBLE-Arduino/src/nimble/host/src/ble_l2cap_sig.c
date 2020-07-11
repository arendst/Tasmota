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

/**
 * L2CAP Signaling (channel ID = 5).
 *
 * Design overview:
 *
 * L2CAP sig procedures are initiated by the application via function calls.
 * Such functions return when either of the following happens:
 *
 * (1) The procedure completes (success or failure).
 * (2) The procedure cannot proceed until a BLE peer responds.
 *
 * For (1), the result of the procedure if fully indicated by the function
 * return code.
 * For (2), the procedure result is indicated by an application-configured
 * callback.  The callback is executed when the procedure completes.
 *
 * Notes on thread-safety:
 * 1. The ble_hs mutex must never be locked when an application callback is
 *    executed.  A callback is free to initiate additional host procedures.
 * 2. The only resource protected by the mutex is the list of active procedures
 *    (ble_l2cap_sig_procs).  Thread-safety is achieved by locking the mutex
 *    during removal and insertion operations.  Procedure objects are only
 *    modified while they are not in the list.
 */

#include <string.h>
#include <errno.h>
#include "nimble/ble.h"
#include "host/ble_monitor.h"
#include "ble_hs_priv.h"

/*****************************************************************************
 * $definitions / declarations                                               *
 *****************************************************************************/

#define BLE_L2CAP_SIG_UNRESPONSIVE_TIMEOUT      30000   /* Milliseconds. */

#define BLE_L2CAP_SIG_PROC_OP_UPDATE            0
#define BLE_L2CAP_SIG_PROC_OP_CONNECT           1
#define BLE_L2CAP_SIG_PROC_OP_DISCONNECT        2
#define BLE_L2CAP_SIG_PROC_OP_MAX               3

struct ble_l2cap_sig_proc {
    STAILQ_ENTRY(ble_l2cap_sig_proc) next;

    ble_npl_time_t exp_os_ticks;
    uint16_t conn_handle;
    uint8_t op;
    uint8_t id;

    union {
        struct {
            ble_l2cap_sig_update_fn *cb;
            void *cb_arg;
        } update;
        struct {
            struct ble_l2cap_chan *chan;
        } connect;
        struct {
            struct ble_l2cap_chan *chan;
        } disconnect;
    };
};

STAILQ_HEAD(ble_l2cap_sig_proc_list, ble_l2cap_sig_proc);

static struct ble_l2cap_sig_proc_list ble_l2cap_sig_procs;

typedef int ble_l2cap_sig_rx_fn(uint16_t conn_handle,
                                struct ble_l2cap_sig_hdr *hdr,
                                struct os_mbuf **om);

static ble_l2cap_sig_rx_fn ble_l2cap_sig_rx_noop;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_update_req_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_update_rsp_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_rx_reject;

#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0
static ble_l2cap_sig_rx_fn ble_l2cap_sig_coc_req_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_coc_rsp_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_disc_rsp_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_disc_req_rx;
static ble_l2cap_sig_rx_fn ble_l2cap_sig_le_credits_rx;
#else
#define ble_l2cap_sig_coc_req_rx    ble_l2cap_sig_rx_noop
#define ble_l2cap_sig_coc_rsp_rx    ble_l2cap_sig_rx_noop
#define ble_l2cap_sig_disc_rsp_rx   ble_l2cap_sig_rx_noop
#define ble_l2cap_sig_disc_req_rx   ble_l2cap_sig_rx_noop
#define ble_l2cap_sig_le_credits_rx   ble_l2cap_sig_rx_noop
#endif

static ble_l2cap_sig_rx_fn * const ble_l2cap_sig_dispatch[] = {
    [BLE_L2CAP_SIG_OP_REJECT]               = ble_l2cap_sig_rx_reject,
    [BLE_L2CAP_SIG_OP_CONNECT_RSP]          = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_CONFIG_RSP]           = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_DISCONN_REQ]          = ble_l2cap_sig_disc_req_rx,
    [BLE_L2CAP_SIG_OP_DISCONN_RSP]          = ble_l2cap_sig_disc_rsp_rx,
    [BLE_L2CAP_SIG_OP_ECHO_RSP]             = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_INFO_RSP]             = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_CREATE_CHAN_RSP]      = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_MOVE_CHAN_RSP]        = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_MOVE_CHAN_CONF_RSP]   = ble_l2cap_sig_rx_noop,
    [BLE_L2CAP_SIG_OP_UPDATE_REQ]           = ble_l2cap_sig_update_req_rx,
    [BLE_L2CAP_SIG_OP_UPDATE_RSP]           = ble_l2cap_sig_update_rsp_rx,
    [BLE_L2CAP_SIG_OP_CREDIT_CONNECT_REQ]   = ble_l2cap_sig_coc_req_rx,
    [BLE_L2CAP_SIG_OP_CREDIT_CONNECT_RSP]   = ble_l2cap_sig_coc_rsp_rx,
    [BLE_L2CAP_SIG_OP_FLOW_CTRL_CREDIT]     = ble_l2cap_sig_le_credits_rx,
};

static uint8_t ble_l2cap_sig_cur_id;

static os_membuf_t ble_l2cap_sig_proc_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_L2CAP_SIG_MAX_PROCS),
                    sizeof (struct ble_l2cap_sig_proc))
];

static struct os_mempool ble_l2cap_sig_proc_pool;

/*****************************************************************************
 * $debug                                                                    *
 *****************************************************************************/

static void
ble_l2cap_sig_dbg_assert_proc_not_inserted(struct ble_l2cap_sig_proc *proc)
{
#if MYNEWT_VAL(BLE_HS_DEBUG)
    struct ble_l2cap_sig_proc *cur;

    STAILQ_FOREACH(cur, &ble_l2cap_sig_procs, next) {
        BLE_HS_DBG_ASSERT(cur != proc);
    }
#endif
}

/*****************************************************************************
 * $misc                                                                     *
 *****************************************************************************/

static uint8_t
ble_l2cap_sig_next_id(void)
{
    ble_l2cap_sig_cur_id++;
    if (ble_l2cap_sig_cur_id == 0) {
        /* An ID of 0 is illegal. */
        ble_l2cap_sig_cur_id = 1;
    }

    return ble_l2cap_sig_cur_id;
}

static ble_l2cap_sig_rx_fn *
ble_l2cap_sig_dispatch_get(uint8_t op)
{
    if (op >= BLE_L2CAP_SIG_OP_MAX) {
        return NULL;
    }

    return ble_l2cap_sig_dispatch[op];
}

/**
 * Allocates a proc entry.
 *
 * @return                      An entry on success; null on failure.
 */
static struct ble_l2cap_sig_proc *
ble_l2cap_sig_proc_alloc(void)
{
    struct ble_l2cap_sig_proc *proc;

    proc = os_memblock_get(&ble_l2cap_sig_proc_pool);
    if (proc != NULL) {
        memset(proc, 0, sizeof *proc);
    }

    return proc;
}

/**
 * Frees the specified proc entry.  No-op if passed a null pointer.
 */
static void
ble_l2cap_sig_proc_free(struct ble_l2cap_sig_proc *proc)
{
    int rc;

    if (proc != NULL) {
        ble_l2cap_sig_dbg_assert_proc_not_inserted(proc);

#if MYNEWT_VAL(BLE_HS_DEBUG)
        memset(proc, 0xff, sizeof *proc);
#endif
        rc = os_memblock_put(&ble_l2cap_sig_proc_pool, proc);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
    }
}

static void
ble_l2cap_sig_proc_insert(struct ble_l2cap_sig_proc *proc)
{
    ble_l2cap_sig_dbg_assert_proc_not_inserted(proc);

    ble_hs_lock();
    STAILQ_INSERT_HEAD(&ble_l2cap_sig_procs, proc, next);
    ble_hs_unlock();
}

/**
 * Tests if a proc entry fits the specified criteria.
 *
 * @param proc                  The procedure to test.
 * @param conn_handle           The connection handle to match against.
 * @param op                    The op code to match against/
 * @param id                    The identifier to match against.
 *                                  0=Ignore this criterion.
 *
 * @return                      1 if the proc matches; 0 otherwise.
 */
static int
ble_l2cap_sig_proc_matches(struct ble_l2cap_sig_proc *proc,
                           uint16_t conn_handle, uint8_t op, uint8_t id)
{
    if (conn_handle != proc->conn_handle) {
        return 0;
    }

    if (op != proc->op) {
        return 0;
    }

    if (id != 0 && id != proc->id) {
        return 0;
    }

    return 1;
}

/**
 * Searches the main proc list for an "expecting" entry whose connection handle
 * and op code match those specified.  If a matching entry is found, it is
 * removed from the list and returned.
 *
 * @param conn_handle           The connection handle to match against.
 * @param op                    The op code to match against.
 * @param identifier            The identifier to match against;
 *                                  0=ignore this criterion.
 *
 * @return                      The matching proc entry on success;
 *                                  null on failure.
 */
static struct ble_l2cap_sig_proc *
ble_l2cap_sig_proc_extract(uint16_t conn_handle, uint8_t op,
                           uint8_t identifier)
{
    struct ble_l2cap_sig_proc *proc;
    struct ble_l2cap_sig_proc *prev;

    ble_hs_lock();

    prev = NULL;
    STAILQ_FOREACH(proc, &ble_l2cap_sig_procs, next) {
        if (ble_l2cap_sig_proc_matches(proc, conn_handle, op, identifier)) {
            if (prev == NULL) {
                STAILQ_REMOVE_HEAD(&ble_l2cap_sig_procs, next);
            } else {
                STAILQ_REMOVE_AFTER(&ble_l2cap_sig_procs, prev, next);
            }
            break;
        }
    }

    ble_hs_unlock();

    return proc;
}

static int
ble_l2cap_sig_rx_noop(uint16_t conn_handle,
                      struct ble_l2cap_sig_hdr *hdr,
                      struct os_mbuf **om)
{
    return BLE_HS_ENOTSUP;
}

static void
ble_l2cap_sig_proc_set_timer(struct ble_l2cap_sig_proc *proc)
{
    proc->exp_os_ticks = ble_npl_time_get() +
                         ble_npl_time_ms_to_ticks32(BLE_L2CAP_SIG_UNRESPONSIVE_TIMEOUT);
    ble_hs_timer_resched();
}

static void
ble_l2cap_sig_process_status(struct ble_l2cap_sig_proc *proc, int status)
{
    if (status == 0) {
        ble_l2cap_sig_proc_set_timer(proc);
        ble_l2cap_sig_proc_insert(proc);
    } else {
        ble_l2cap_sig_proc_free(proc);
    }
}

/*****************************************************************************
 * $update                                                                   *
 *****************************************************************************/

static void
ble_l2cap_sig_update_call_cb(struct ble_l2cap_sig_proc *proc, int status)
{
    BLE_HS_DBG_ASSERT(!ble_hs_locked_by_cur_task());

    if (status != 0) {
        STATS_INC(ble_l2cap_stats, update_fail);
    }

    if (proc->update.cb != NULL) {
        proc->update.cb(proc->conn_handle, status, proc->update.cb_arg);
    }
}

int
ble_l2cap_sig_update_req_rx(uint16_t conn_handle,
                            struct ble_l2cap_sig_hdr *hdr,
                            struct os_mbuf **om)
{
    struct ble_l2cap_sig_update_req *req;
    struct os_mbuf *txom;
    struct ble_l2cap_sig_update_rsp *rsp;
    struct ble_gap_upd_params params;
    ble_hs_conn_flags_t conn_flags;
    uint16_t l2cap_result;
    int sig_err;
    int rc;

    l2cap_result = 0; /* Silence spurious gcc warning. */

    rc = ble_hs_mbuf_pullup_base(om, BLE_L2CAP_SIG_UPDATE_REQ_SZ);
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_atomic_conn_flags(conn_handle, &conn_flags);
    if (rc != 0) {
        return rc;
    }

    /* Only a master can process an update request. */
    sig_err = !(conn_flags & BLE_HS_CONN_F_MASTER);
    if (sig_err) {
        return BLE_HS_EREJECT;
    }

    req = (struct ble_l2cap_sig_update_req *)(*om)->om_data;

    params.itvl_min = le16toh(req->itvl_min);
    params.itvl_max = le16toh(req->itvl_max);
    params.latency = le16toh(req->slave_latency);
    params.supervision_timeout = le16toh(req->timeout_multiplier);
    params.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN;
    params.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN;

    /* Ask application if slave's connection parameters are acceptable. */
    rc = ble_gap_rx_l2cap_update_req(conn_handle, &params);
    if (rc == 0) {
        /* Application agrees to accept parameters; schedule update. */
        rc = ble_gap_update_params(conn_handle, &params);
    }

    if (rc == 0) {
        l2cap_result = BLE_L2CAP_SIG_UPDATE_RSP_RESULT_ACCEPT;
    } else {
        l2cap_result = BLE_L2CAP_SIG_UPDATE_RSP_RESULT_REJECT;
    }

    rsp = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_UPDATE_RSP, hdr->identifier,
                                sizeof(*rsp), &txom);
    if (!rsp) {
        /* No memory for response, lest allow to timeout on remote side */
        return 0;
    }

    rsp->result = htole16(l2cap_result);

    /* Send L2CAP response. */
    ble_l2cap_sig_tx(conn_handle, txom);

    return 0;
}

static int
ble_l2cap_sig_update_rsp_rx(uint16_t conn_handle,
                            struct ble_l2cap_sig_hdr *hdr,
                            struct os_mbuf **om)
{
    struct ble_l2cap_sig_update_rsp *rsp;
    struct ble_l2cap_sig_proc *proc;
    int cb_status;
    int rc;

    proc = ble_l2cap_sig_proc_extract(conn_handle,
                                      BLE_L2CAP_SIG_PROC_OP_UPDATE,
                                      hdr->identifier);
    if (proc == NULL) {
        return 0;
    }

    rc = ble_hs_mbuf_pullup_base(om, BLE_L2CAP_SIG_UPDATE_RSP_SZ);
    if (rc != 0) {
        cb_status = rc;
        goto done;
    }

    rsp = (struct ble_l2cap_sig_update_rsp *)(*om)->om_data;

    switch (le16toh(rsp->result)) {
    case BLE_L2CAP_SIG_UPDATE_RSP_RESULT_ACCEPT:
        cb_status = 0;
        rc = 0;
        break;

    case BLE_L2CAP_SIG_UPDATE_RSP_RESULT_REJECT:
        cb_status = BLE_HS_EREJECT;
        rc = 0;
        break;

    default:
        cb_status = BLE_HS_EBADDATA;
        rc = 0;
        break;
    }

done:
    ble_l2cap_sig_update_call_cb(proc, cb_status);
    ble_l2cap_sig_proc_free(proc);
    return rc;
}

int
ble_l2cap_sig_update(uint16_t conn_handle,
                     struct ble_l2cap_sig_update_params *params,
                     ble_l2cap_sig_update_fn *cb, void *cb_arg)
{
    struct os_mbuf *txom;
    struct ble_l2cap_sig_update_req *req;
    struct ble_l2cap_sig_proc *proc;
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int master;
    int rc;

    proc = NULL;

    STATS_INC(ble_l2cap_stats, update_init);

    ble_hs_lock();
    rc = ble_hs_misc_conn_chan_find_reqd(conn_handle, BLE_L2CAP_CID_SIG,
                                         &conn, &chan);
    if (rc != 0) {
        ble_hs_unlock();
        goto done;
    }

    master = conn->bhc_flags & BLE_HS_CONN_F_MASTER;
    ble_hs_unlock();

    if (master) {
        /* Only the slave can initiate the L2CAP connection update
         * procedure.
         */
        rc = BLE_HS_EINVAL;
        goto done;
    }

    proc = ble_l2cap_sig_proc_alloc();
    if (proc == NULL) {
        STATS_INC(ble_l2cap_stats, update_fail);
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    proc->op = BLE_L2CAP_SIG_PROC_OP_UPDATE;
    proc->id = ble_l2cap_sig_next_id();
    proc->conn_handle = conn_handle;
    proc->update.cb = cb;
    proc->update.cb_arg = cb_arg;

    req = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_UPDATE_REQ, proc->id,
                                sizeof(*req), &txom);
    if (!req) {
        STATS_INC(ble_l2cap_stats, update_fail);
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    req->itvl_min = htole16(params->itvl_min);
    req->itvl_max = htole16(params->itvl_max);
    req->slave_latency = htole16(params->slave_latency);
    req->timeout_multiplier = htole16(params->timeout_multiplier);

    rc = ble_l2cap_sig_tx(conn_handle, txom);

done:
    ble_l2cap_sig_process_status(proc, rc);
    return rc;
}

/*****************************************************************************
 * $connect                                                                  *
 *****************************************************************************/

#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0

static int
ble_l2cap_sig_coc_err2ble_hs_err(uint16_t l2cap_coc_err)
{
    switch (l2cap_coc_err) {
    case BLE_L2CAP_COC_ERR_CONNECTION_SUCCESS:
        return 0;
    case BLE_L2CAP_COC_ERR_UNKNOWN_LE_PSM:
        return BLE_HS_ENOTSUP;
    case BLE_L2CAP_COC_ERR_NO_RESOURCES:
        return BLE_HS_ENOMEM;
    case BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHEN:
        return BLE_HS_EAUTHEN;
    case BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHOR:
        return BLE_HS_EAUTHOR;
    case BLE_L2CAP_COC_ERR_INSUFFICIENT_KEY_SZ:
        return BLE_HS_EENCRYPT_KEY_SZ;
    case BLE_L2CAP_COC_ERR_INSUFFICIENT_ENC:
        return BLE_HS_EENCRYPT;
    case BLE_L2CAP_COC_ERR_INVALID_SOURCE_CID:
        return BLE_HS_EREJECT;
    case BLE_L2CAP_COC_ERR_SOURCE_CID_ALREADY_USED:
        return BLE_HS_EALREADY;
    case BLE_L2CAP_COC_ERR_UNACCEPTABLE_PARAMETERS:
        return BLE_HS_EINVAL;
    default:
        return BLE_HS_EUNKNOWN;
    }
}

static int
ble_l2cap_sig_ble_hs_err2coc_err(uint16_t ble_hs_err)
{
    switch (ble_hs_err) {
    case BLE_HS_ENOTSUP:
        return BLE_L2CAP_COC_ERR_UNKNOWN_LE_PSM;
    case BLE_HS_ENOMEM:
        return BLE_L2CAP_COC_ERR_NO_RESOURCES;
    case BLE_HS_EAUTHEN:
        return BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHEN;
    case BLE_HS_EAUTHOR:
        return BLE_L2CAP_COC_ERR_INSUFFICIENT_AUTHOR;
    case BLE_HS_EENCRYPT:
        return BLE_L2CAP_COC_ERR_INSUFFICIENT_ENC;
    case BLE_HS_EENCRYPT_KEY_SZ:
        return BLE_L2CAP_COC_ERR_INSUFFICIENT_KEY_SZ;
    case BLE_HS_EINVAL:
        return BLE_L2CAP_COC_ERR_UNACCEPTABLE_PARAMETERS;
    default:
        return BLE_L2CAP_COC_ERR_NO_RESOURCES;
    }
}

static void
ble_l2cap_event_coc_connected(struct ble_l2cap_chan *chan, uint16_t status)
{
    struct ble_l2cap_event event = { };

    event.type = BLE_L2CAP_EVENT_COC_CONNECTED;
    event.connect.conn_handle = chan->conn_handle;
    event.connect.chan = chan;
    event.connect.status = status;

    chan->cb(&event, chan->cb_arg);
}

static int
ble_l2cap_event_coc_accept(struct ble_l2cap_chan *chan, uint16_t peer_sdu_size)
{
    struct ble_l2cap_event event = { };

    event.type = BLE_L2CAP_EVENT_COC_ACCEPT;
    event.accept.chan = chan;
    event.accept.conn_handle = chan->conn_handle;
    event.accept.peer_sdu_size = peer_sdu_size;

    return chan->cb(&event, chan->cb_arg);
}

static void
ble_l2cap_sig_coc_connect_cb(struct ble_l2cap_sig_proc *proc, int status)
{
    struct ble_l2cap_chan *chan;

    if (!proc) {
            return;
    }

    chan = proc->connect.chan;
    if (!chan || !chan->cb) {
        return;
    }

    ble_l2cap_event_coc_connected(chan, status);

    if (status) {
        /* Normally in channel free we send disconnected event to application.
         * However in case on error during creation connection we send connected
         * event with error status. To avoid additional disconnected event lets
         * clear callbacks since we don't needed it anymore.*/
        chan->cb = NULL;
        ble_l2cap_chan_free(chan);
    }
}

static int
ble_l2cap_sig_coc_req_rx(uint16_t conn_handle, struct ble_l2cap_sig_hdr *hdr,
                         struct os_mbuf **om)
{
    int rc;
    struct ble_l2cap_sig_le_con_req *req;
    struct os_mbuf *txom;
    struct ble_l2cap_sig_le_con_rsp *rsp;
    struct ble_l2cap_chan *chan = NULL;
    struct ble_hs_conn *conn;
    uint16_t scid;

    rc = ble_hs_mbuf_pullup_base(om, sizeof(req));
    if (rc != 0) {
        return rc;
    }

    rsp = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_CREDIT_CONNECT_RSP,
                                hdr->identifier, sizeof(*rsp), &txom);
    if (!rsp) {
        /* Well, nothing smart we can do if there is no memory for response.
         * Remote will timeout.
         */
        return 0;
    }

    memset(rsp, 0, sizeof(*rsp));

    req = (struct ble_l2cap_sig_le_con_req *)(*om)->om_data;

    ble_hs_lock();
    conn = ble_hs_conn_find_assert(conn_handle);

    /* Verify CID. Note, scid in the request is dcid for out local channel */
    scid = le16toh(req->scid);
    if (scid < BLE_L2CAP_COC_CID_START || scid > BLE_L2CAP_COC_CID_END) {
        rsp->result = htole16(BLE_L2CAP_COC_ERR_INVALID_SOURCE_CID);
        ble_hs_unlock();
        goto failed;
    }

    chan = ble_hs_conn_chan_find_by_dcid(conn, scid);
    if (chan) {
        rsp->result = htole16(BLE_L2CAP_COC_ERR_SOURCE_CID_ALREADY_USED);
        ble_hs_unlock();
        goto failed;
    }

    rc = ble_l2cap_coc_create_srv_chan(conn_handle, le16toh(req->psm), &chan);
    if (rc != 0) {
        uint16_t coc_err = ble_l2cap_sig_ble_hs_err2coc_err(rc);
        rsp->result = htole16(coc_err);
        ble_hs_unlock();
        goto failed;
    }

    /* Fill up remote configuration. Note MPS is the L2CAP MTU*/
    chan->dcid = scid;
    chan->peer_mtu = le16toh(req->mps);
    chan->coc_tx.credits = le16toh(req->credits);
    chan->coc_tx.mtu = le16toh(req->mtu);

    ble_hs_conn_chan_insert(conn, chan);
    ble_hs_unlock();

    rc = ble_l2cap_event_coc_accept(chan, le16toh(req->mtu));
    if (rc != 0) {
        uint16_t coc_err = ble_l2cap_sig_ble_hs_err2coc_err(rc);

        /* Make sure we do not send disconnect event when removing channel */
        chan->cb = NULL;

        ble_hs_lock();
        ble_hs_conn_delete_chan(conn, chan);
        ble_hs_unlock();
        rsp->result = htole16(coc_err);
        goto failed;
    }

    rsp->dcid = htole16(chan->scid);
    rsp->credits = htole16(chan->coc_rx.credits);
    rsp->mps = htole16(chan->my_mtu);
    rsp->mtu = htole16(chan->coc_rx.mtu);
    rsp->result = htole16(BLE_L2CAP_COC_ERR_CONNECTION_SUCCESS);

    rc = ble_l2cap_sig_tx(conn_handle, txom);
    if (rc != 0) {
        ble_hs_lock();
        ble_hs_conn_delete_chan(conn, chan);
        ble_hs_unlock();
        return 0;
    }

    /* Notify user about connection status */
    ble_l2cap_event_coc_connected(chan, rc);

    return 0;

failed:
    ble_l2cap_sig_tx(conn_handle, txom);
    return 0;
}

static int
ble_l2cap_sig_coc_rsp_rx(uint16_t conn_handle, struct ble_l2cap_sig_hdr *hdr,
                          struct os_mbuf **om)
{
    struct ble_l2cap_sig_proc *proc;
    struct ble_l2cap_sig_le_con_rsp *rsp;
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

#if !BLE_MONITOR
    BLE_HS_LOG(DEBUG, "L2CAP LE COC connection response received\n");
#endif

    proc = ble_l2cap_sig_proc_extract(conn_handle,
                                      BLE_L2CAP_SIG_PROC_OP_CONNECT,
                                      hdr->identifier);
    if (!proc) {
        return 0;
    }

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    rsp = (struct ble_l2cap_sig_le_con_rsp *)(*om)->om_data;

    chan = proc->connect.chan;

    if (rsp->result) {
        rc = ble_l2cap_sig_coc_err2ble_hs_err(le16toh(rsp->result));
        goto done;
    }

    /* Fill up remote configuration
     * Note MPS is the L2CAP MTU
     */
    chan->peer_mtu = le16toh(rsp->mps);
    chan->dcid = le16toh(rsp->dcid);
    chan->coc_tx.mtu = le16toh(rsp->mtu);
    chan->coc_tx.credits = le16toh(rsp->credits);

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);
    assert(conn != NULL);
    ble_hs_conn_chan_insert(conn, chan);
    ble_hs_unlock();

    rc = 0;

done:
    ble_l2cap_sig_coc_connect_cb(proc, rc);
    ble_l2cap_sig_proc_free(proc);

    /* Silently ignore errors as this is response signal */
    return 0;
}

int
ble_l2cap_sig_coc_connect(uint16_t conn_handle, uint16_t psm, uint16_t mtu,
                          struct os_mbuf *sdu_rx,
                          ble_l2cap_event_fn *cb, void *cb_arg)
{
    struct ble_hs_conn *conn;
    struct ble_l2cap_sig_proc *proc;
    struct os_mbuf *txom;
    struct ble_l2cap_sig_le_con_req *req;
    struct ble_l2cap_chan *chan = NULL;
    int rc;

    if (!sdu_rx || !cb) {
        return BLE_HS_EINVAL;
    }

    ble_hs_lock();
    conn = ble_hs_conn_find(conn_handle);

    if (!conn) {
        ble_hs_unlock();
        return BLE_HS_ENOTCONN;
    }

    chan = ble_l2cap_coc_chan_alloc(conn_handle, psm, mtu, sdu_rx, cb, cb_arg);
    if (!chan) {
        ble_hs_unlock();
        return BLE_HS_ENOMEM;
    }

    proc = ble_l2cap_sig_proc_alloc();
    if (!proc) {
        ble_l2cap_chan_free(chan);
        ble_hs_unlock();
        return BLE_HS_ENOMEM;
    }

    proc->op = BLE_L2CAP_SIG_PROC_OP_CONNECT;
    proc->id = ble_l2cap_sig_next_id();
    proc->conn_handle = conn_handle;
    proc->connect.chan = chan;

    req = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_CREDIT_CONNECT_REQ, proc->id,
                                sizeof(*req), &txom);
    if (!req) {
        ble_l2cap_chan_free(chan);
        ble_hs_unlock();
        return BLE_HS_ENOMEM;
    }

    req->psm = htole16(psm);
    req->scid = htole16(chan->scid);
    req->mtu = htole16(chan->coc_rx.mtu);
    req->mps = htole16(chan->my_mtu);
    req->credits = htole16(chan->coc_rx.credits);

    ble_hs_unlock();

    rc = ble_l2cap_sig_tx(proc->conn_handle, txom);
    if (rc != 0) {
        ble_l2cap_chan_free(chan);
    }

    ble_l2cap_sig_process_status(proc, rc);

    return rc;
}

/*****************************************************************************
 * $disconnect                                                               *
 *****************************************************************************/

static int
ble_l2cap_sig_disc_req_rx(uint16_t conn_handle, struct ble_l2cap_sig_hdr *hdr,
                          struct os_mbuf **om)
{
    struct ble_l2cap_sig_disc_req *req;
    struct os_mbuf *txom;
    struct ble_l2cap_sig_disc_rsp *rsp;
    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*req));
    if (rc != 0) {
        return rc;
    }

    rsp = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_DISCONN_RSP, hdr->identifier,
                                sizeof(*rsp), &txom);
    if (!rsp) {
        /* Well, nothing smart we can do if there is no memory for response.
         * Remote will timeout.
         */
        return 0;
    }

    ble_hs_lock();
    conn = ble_hs_conn_find_assert(conn_handle);

    req = (struct ble_l2cap_sig_disc_req *) (*om)->om_data;

    /* Let's find matching channel. Note that destination CID in the request
     * is from peer perspective. It is source CID from nimble perspective
     */
    chan = ble_hs_conn_chan_find_by_scid(conn, le16toh(req->dcid));
    if (!chan || (le16toh(req->scid) != chan->dcid)) {
        os_mbuf_free_chain(txom);
        ble_hs_unlock();
        return 0;
    }

    /* Note that in the response destination CID is form peer perspective and
     * it is source CID from nimble perspective.
     */
    rsp->dcid = htole16(chan->scid);
    rsp->scid = htole16(chan->dcid);

    ble_hs_conn_delete_chan(conn, chan);
    ble_hs_unlock();

    ble_l2cap_sig_tx(conn_handle, txom);
    return 0;
}

static void
ble_l2cap_sig_coc_disconnect_cb(struct ble_l2cap_sig_proc *proc, int status)
{
    struct ble_l2cap_chan *chan;
    struct ble_l2cap_event event;
    struct ble_hs_conn *conn;

    if (!proc) {
        return;
    }

    memset(&event, 0, sizeof(event));
    chan = proc->disconnect.chan;

    if (!chan) {
        return;
    }

    if (!chan->cb) {
        goto done;
    }

done:
    ble_hs_lock();
    conn = ble_hs_conn_find(chan->conn_handle);
    if (conn) {
        ble_hs_conn_delete_chan(conn, chan);
    } else {
        ble_l2cap_chan_free(chan);
    }
    ble_hs_unlock();
}

static int
ble_l2cap_sig_disc_rsp_rx(uint16_t conn_handle, struct ble_l2cap_sig_hdr *hdr,
                           struct os_mbuf **om)
{
    struct ble_l2cap_sig_disc_rsp *rsp;
    struct ble_l2cap_sig_proc *proc;
    struct ble_l2cap_chan *chan;
    int rc;

    proc = ble_l2cap_sig_proc_extract(conn_handle,
                                      BLE_L2CAP_SIG_PROC_OP_DISCONNECT,
                                      hdr->identifier);
    if (!proc) {
        return 0;
    }

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*rsp));
    if (rc != 0) {
        goto done;
    }

    chan = proc->disconnect.chan;
    if (!chan) {
        goto done;
    }

    rsp = (struct ble_l2cap_sig_disc_rsp *)(*om)->om_data;
    if (chan->dcid != le16toh(rsp->dcid) || chan->scid != le16toh(rsp->scid)) {
        /* This response is incorrect, lets wait for timeout */
        ble_l2cap_sig_process_status(proc, 0);
        return 0;
    }

    ble_l2cap_sig_coc_disconnect_cb(proc, rc);

done:
    ble_l2cap_sig_proc_free(proc);
    return 0;
}

int
ble_l2cap_sig_disconnect(struct ble_l2cap_chan *chan)
{
    struct os_mbuf *txom;
    struct ble_l2cap_sig_disc_req *req;
    struct ble_l2cap_sig_proc *proc;
    int rc;

    proc = ble_l2cap_sig_proc_alloc();
    if (proc == NULL) {
        return BLE_HS_ENOMEM;
    }

    proc->op = BLE_L2CAP_SIG_PROC_OP_DISCONNECT;
    proc->id = ble_l2cap_sig_next_id();
    proc->conn_handle = chan->conn_handle;
    proc->disconnect.chan = chan;

    req = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_DISCONN_REQ, proc->id,
                                sizeof(*req), &txom);
    if (!req) {
        rc = BLE_HS_ENOMEM;
        goto done;
    }

    req->dcid = htole16(chan->dcid);
    req->scid = htole16(chan->scid);

    rc = ble_l2cap_sig_tx(proc->conn_handle, txom);

done:
    ble_l2cap_sig_process_status(proc, rc);

    return rc;
}

static int
ble_l2cap_sig_le_credits_rx(uint16_t conn_handle, struct ble_l2cap_sig_hdr *hdr,
                            struct os_mbuf **om)
{
    struct ble_l2cap_sig_le_credits *req;
    int rc;

    rc = ble_hs_mbuf_pullup_base(om, sizeof(*req));
    if (rc != 0) {
        return 0;
    }

    req = (struct ble_l2cap_sig_le_credits *) (*om)->om_data;

    /* Ignore when peer sends zero credits */
    if (req->credits == 0) {
            return 0;
    }

    ble_l2cap_coc_le_credits_update(conn_handle, le16toh(req->scid),
                                    le16toh(req->credits));

    return 0;
}

int
ble_l2cap_sig_le_credits(uint16_t conn_handle, uint16_t scid, uint16_t credits)
{
    struct ble_l2cap_sig_le_credits *cmd;
    struct os_mbuf *txom;

    cmd = ble_l2cap_sig_cmd_get(BLE_L2CAP_SIG_OP_FLOW_CTRL_CREDIT,
                                ble_l2cap_sig_next_id(), sizeof(*cmd), &txom);

    if (!cmd) {
        return BLE_HS_ENOMEM;
    }

    cmd->scid = htole16(scid);
    cmd->credits = htole16(credits);

    return ble_l2cap_sig_tx(conn_handle, txom);
}
#endif

static int
ble_l2cap_sig_rx_reject(uint16_t conn_handle,
                        struct ble_l2cap_sig_hdr *hdr,
                        struct os_mbuf **om)
{
    struct ble_l2cap_sig_proc *proc;
    proc = ble_l2cap_sig_proc_extract(conn_handle,
                                         BLE_L2CAP_SIG_PROC_OP_CONNECT,
                                         hdr->identifier);
   if (!proc) {
       return 0;
   }

   switch (proc->id) {
#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0
       case BLE_L2CAP_SIG_PROC_OP_CONNECT:
           ble_l2cap_sig_coc_connect_cb(proc, BLE_HS_EREJECT);
           break;
#endif
       default:
           break;
   }

   ble_l2cap_sig_proc_free(proc);
   return 0;
}
/*****************************************************************************
 * $misc                                                                     *
 *****************************************************************************/

static int
ble_l2cap_sig_rx(struct ble_l2cap_chan *chan)
{
    struct ble_l2cap_sig_hdr hdr;
    ble_l2cap_sig_rx_fn *rx_cb;
    uint16_t conn_handle;
    struct os_mbuf **om;
    int rc;

    conn_handle = chan->conn_handle;
    om = &chan->rx_buf;

    STATS_INC(ble_l2cap_stats, sig_rx);

#if !BLE_MONITOR
    BLE_HS_LOG(DEBUG, "L2CAP - rxed signalling msg: ");
    ble_hs_log_mbuf(*om);
    BLE_HS_LOG(DEBUG, "\n");
#endif

    rc = ble_hs_mbuf_pullup_base(om, BLE_L2CAP_SIG_HDR_SZ);
    if (rc != 0) {
        return rc;
    }

    ble_l2cap_sig_hdr_parse((*om)->om_data, (*om)->om_len, &hdr);

    /* Strip L2CAP sig header from the front of the mbuf. */
    os_mbuf_adj(*om, BLE_L2CAP_SIG_HDR_SZ);

    if (OS_MBUF_PKTLEN(*om) != hdr.length) {
        return BLE_HS_EBADDATA;
    }

    rx_cb = ble_l2cap_sig_dispatch_get(hdr.op);
    if (rx_cb == NULL) {
        rc = BLE_HS_EREJECT;
    } else {
        rc = rx_cb(conn_handle, &hdr, om);
    }

    if (rc) {
        ble_l2cap_sig_reject_tx(conn_handle, hdr.identifier,
                                        BLE_L2CAP_SIG_ERR_CMD_NOT_UNDERSTOOD,
                                        NULL, 0);
    }

    return rc;
}

struct ble_l2cap_chan *
ble_l2cap_sig_create_chan(uint16_t conn_handle)
{
    struct ble_l2cap_chan *chan;

    chan = ble_l2cap_chan_alloc(conn_handle);
    if (chan == NULL) {
        return NULL;
    }

    chan->scid = BLE_L2CAP_CID_SIG;
    chan->dcid = BLE_L2CAP_CID_SIG;
    chan->my_mtu = BLE_L2CAP_SIG_MTU;
    chan->rx_fn = ble_l2cap_sig_rx;

    return chan;
}

/**
 * @return                      The number of ticks until the next expiration
 *                                  occurs.
 */
static int32_t
ble_l2cap_sig_extract_expired(struct ble_l2cap_sig_proc_list *dst_list)
{
    struct ble_l2cap_sig_proc *proc;
    struct ble_l2cap_sig_proc *prev;
    struct ble_l2cap_sig_proc *next;
    ble_npl_time_t now;
    ble_npl_stime_t next_exp_in;
    ble_npl_stime_t time_diff;

    now = ble_npl_time_get();
    STAILQ_INIT(dst_list);

    /* Assume each event is either expired or has infinite duration. */
    next_exp_in = BLE_HS_FOREVER;

    ble_hs_lock();

    prev = NULL;
    proc = STAILQ_FIRST(&ble_l2cap_sig_procs);
    while (proc != NULL) {
        next = STAILQ_NEXT(proc, next);

        time_diff = proc->exp_os_ticks - now;
        if (time_diff <= 0) {
            /* Procedure has expired; move it to the destination list. */
            if (prev == NULL) {
                STAILQ_REMOVE_HEAD(&ble_l2cap_sig_procs, next);
            } else {
                STAILQ_REMOVE_AFTER(&ble_l2cap_sig_procs, prev, next);
            }
            STAILQ_INSERT_TAIL(dst_list, proc, next);
        } else {
            if (time_diff < next_exp_in) {
                next_exp_in = time_diff;
            }
        }

        proc = next;
    }

    ble_hs_unlock();

    return next_exp_in;
}

void
ble_l2cap_sig_conn_broken(uint16_t conn_handle, int reason)
{
    struct ble_l2cap_sig_proc *proc;

    /* Report a failure for each timed out procedure. */
    while ((proc = STAILQ_FIRST(&ble_l2cap_sig_procs)) != NULL) {
        switch(proc->op) {
            case BLE_L2CAP_SIG_PROC_OP_UPDATE:
                ble_l2cap_sig_update_call_cb(proc, reason);
                break;
#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0
            case BLE_L2CAP_SIG_PROC_OP_CONNECT:
                ble_l2cap_sig_coc_connect_cb(proc, reason);
            break;
            case BLE_L2CAP_SIG_PROC_OP_DISCONNECT:
                ble_l2cap_sig_coc_disconnect_cb(proc, reason);
            break;
#endif
            }

            STAILQ_REMOVE_HEAD(&ble_l2cap_sig_procs, next);
            ble_l2cap_sig_proc_free(proc);
    }

}

/**
 * Terminates expired procedures.
 *
 * @return                      The number of ticks until this function should
 *                                  be called again.
 */
int32_t
ble_l2cap_sig_timer(void)
{
    struct ble_l2cap_sig_proc_list temp_list;
    struct ble_l2cap_sig_proc *proc;
    int32_t ticks_until_exp;

    /* Remove timed-out procedures from the main list and insert them into a
     * temporary list.  This function also calculates the number of ticks until
     * the next expiration will occur.
     */
    ticks_until_exp = ble_l2cap_sig_extract_expired(&temp_list);

    /* Report a failure for each timed out procedure. */
    while ((proc = STAILQ_FIRST(&temp_list)) != NULL) {
        STATS_INC(ble_l2cap_stats, proc_timeout);
        switch(proc->op) {
            case BLE_L2CAP_SIG_PROC_OP_UPDATE:
                ble_l2cap_sig_update_call_cb(proc, BLE_HS_ETIMEOUT);
                break;
#if MYNEWT_VAL(BLE_L2CAP_COC_MAX_NUM) != 0
            case BLE_L2CAP_SIG_PROC_OP_CONNECT:
                ble_l2cap_sig_coc_connect_cb(proc, BLE_HS_ETIMEOUT);
            break;
            case BLE_L2CAP_SIG_PROC_OP_DISCONNECT:
                ble_l2cap_sig_coc_disconnect_cb(proc, BLE_HS_ETIMEOUT);
            break;
#endif
        }

        STAILQ_REMOVE_HEAD(&temp_list, next);
        ble_l2cap_sig_proc_free(proc);
    }

    return ticks_until_exp;
}

int
ble_l2cap_sig_init(void)
{
    int rc;

    STAILQ_INIT(&ble_l2cap_sig_procs);

    rc = os_mempool_init(&ble_l2cap_sig_proc_pool,
                         MYNEWT_VAL(BLE_L2CAP_SIG_MAX_PROCS),
                         sizeof (struct ble_l2cap_sig_proc),
                         ble_l2cap_sig_proc_mem,
                         "ble_l2cap_sig_proc_pool");
    if (rc != 0) {
        return rc;
    }

    return 0;
}
