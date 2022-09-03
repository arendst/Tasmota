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

#include <string.h>
#include <errno.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os.h"
#include "nimble/nimble/host/include/host/ble_hs_id.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"

/** At least three channels required per connection (sig, att, sm). */
#define BLE_HS_CONN_MIN_CHANS       3

static SLIST_HEAD(, ble_hs_conn) ble_hs_conns;
static struct os_mempool ble_hs_conn_pool;

static os_membuf_t ble_hs_conn_elem_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_MAX_CONNECTIONS),
                    sizeof (struct ble_hs_conn))
];

static const uint8_t ble_hs_conn_null_addr[6];

int
ble_hs_conn_can_alloc(void)
{
#if !NIMBLE_BLE_CONNECT
    return 0;
#endif

    return ble_hs_conn_pool.mp_num_free >= 1 &&
           ble_l2cap_chan_pool.mp_num_free >= BLE_HS_CONN_MIN_CHANS &&
           ble_gatts_conn_can_alloc();
}

struct ble_l2cap_chan *
ble_hs_conn_chan_find_by_scid(struct ble_hs_conn *conn, uint16_t cid)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_l2cap_chan *chan;

    SLIST_FOREACH(chan, &conn->bhc_channels, next) {
        if (chan->scid == cid) {
            return chan;
        }
        if (chan->scid > cid) {
            return NULL;
        }
    }

    return NULL;
}

struct ble_l2cap_chan *
ble_hs_conn_chan_find_by_dcid(struct ble_hs_conn *conn, uint16_t cid)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_l2cap_chan *chan;

    SLIST_FOREACH(chan, &conn->bhc_channels, next) {
        if (chan->dcid == cid) {
            return chan;
        }
    }

    return NULL;
}

bool
ble_hs_conn_chan_exist(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_l2cap_chan *tmp;

    SLIST_FOREACH(tmp, &conn->bhc_channels, next) {
        if (chan == tmp) {
            return true;
        }
    }

    return false;
}

int
ble_hs_conn_chan_insert(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan)
{
#if !NIMBLE_BLE_CONNECT
    return BLE_HS_ENOTSUP;
#endif

    struct ble_l2cap_chan *prev;
    struct ble_l2cap_chan *cur;

    prev = NULL;
    SLIST_FOREACH(cur, &conn->bhc_channels, next) {
        if (cur->scid == chan->scid) {
            return BLE_HS_EALREADY;
        }
        if (cur->scid > chan->scid) {
            break;
        }

        prev = cur;
    }

    if (prev == NULL) {
        SLIST_INSERT_HEAD(&conn->bhc_channels, chan, next);
    } else {
        SLIST_INSERT_AFTER(prev, chan, next);
    }

    return 0;
}

struct ble_hs_conn *
ble_hs_conn_alloc(uint16_t conn_handle)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_l2cap_chan *chan;
    struct ble_hs_conn *conn;
    int rc;

    conn = os_memblock_get(&ble_hs_conn_pool);
    if (conn == NULL) {
        goto err;
    }
    memset(conn, 0, sizeof *conn);
    conn->bhc_handle = conn_handle;

    SLIST_INIT(&conn->bhc_channels);

    chan = ble_att_create_chan(conn_handle);
    if (chan == NULL) {
        goto err;
    }
    rc = ble_hs_conn_chan_insert(conn, chan);
    if (rc != 0) {
        goto err;
    }

    chan = ble_l2cap_sig_create_chan(conn_handle);
    if (chan == NULL) {
        goto err;
    }
    rc = ble_hs_conn_chan_insert(conn, chan);
    if (rc != 0) {
        goto err;
    }

    /* Create the SM channel even if not configured. We need it to reject SM
     * messages.
     */
    chan = ble_sm_create_chan(conn_handle);
    if (chan == NULL) {
        goto err;
    }
    rc = ble_hs_conn_chan_insert(conn, chan);
    if (rc != 0) {
        goto err;
    }

    rc = ble_gatts_conn_init(&conn->bhc_gatt_svr);
    if (rc != 0) {
        goto err;
    }

    STAILQ_INIT(&conn->bhc_tx_q);

    STATS_INC(ble_hs_stats, conn_create);

    return conn;

err:
    ble_hs_conn_free(conn);
    return NULL;
}

void
ble_hs_conn_delete_chan(struct ble_hs_conn *conn, struct ble_l2cap_chan *chan)
{
    if (conn->bhc_rx_chan == chan) {
        conn->bhc_rx_chan = NULL;
    }

    SLIST_REMOVE(&conn->bhc_channels, chan, ble_l2cap_chan, next);
    ble_l2cap_chan_free(conn, chan);
}

void
ble_hs_conn_foreach(ble_hs_conn_foreach_fn *cb, void *arg)
{
    struct ble_hs_conn *conn;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    SLIST_FOREACH(conn, &ble_hs_conns, bhc_next) {
        if (cb(conn, arg) != 0) {
            return;
        }
    }
}

void
ble_hs_conn_free(struct ble_hs_conn *conn)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    struct ble_l2cap_chan *chan;
    struct os_mbuf_pkthdr *omp;
    int rc;

    if (conn == NULL) {
        return;
    }

    ble_att_svr_prep_clear(&conn->bhc_att_svr.basc_prep_list);

    while ((chan = SLIST_FIRST(&conn->bhc_channels)) != NULL) {
        ble_hs_conn_delete_chan(conn, chan);
    }

    while ((omp = STAILQ_FIRST(&conn->bhc_tx_q)) != NULL) {
        STAILQ_REMOVE_HEAD(&conn->bhc_tx_q, omp_next);
        os_mbuf_free_chain(OS_MBUF_PKTHDR_TO_MBUF(omp));
    }

#if MYNEWT_VAL(BLE_HS_DEBUG)
    memset(conn, 0xff, sizeof *conn);
#endif
    rc = os_memblock_put(&ble_hs_conn_pool, conn);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);

    STATS_INC(ble_hs_stats, conn_delete);
}

void
ble_hs_conn_insert(struct ble_hs_conn *conn)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    BLE_HS_DBG_ASSERT_EVAL(ble_hs_conn_find(conn->bhc_handle) == NULL);
    SLIST_INSERT_HEAD(&ble_hs_conns, conn, bhc_next);
}

void
ble_hs_conn_remove(struct ble_hs_conn *conn)
{
#if !NIMBLE_BLE_CONNECT
    return;
#endif

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    SLIST_REMOVE(&ble_hs_conns, conn, ble_hs_conn, bhc_next);
}

struct ble_hs_conn *
ble_hs_conn_find(uint16_t conn_handle)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_hs_conn *conn;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    SLIST_FOREACH(conn, &ble_hs_conns, bhc_next) {
        if (conn->bhc_handle == conn_handle) {
            return conn;
        }
    }

    return NULL;
}

struct ble_hs_conn *
ble_hs_conn_find_assert(uint16_t conn_handle)
{
    struct ble_hs_conn *conn;

    conn = ble_hs_conn_find(conn_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);

    return conn;
}

struct ble_hs_conn *
ble_hs_conn_find_by_addr(const ble_addr_t *addr)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_hs_conn *conn;
    struct ble_hs_conn_addrs addrs;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    if (!addr) {
        return NULL;
    }

    SLIST_FOREACH(conn, &ble_hs_conns, bhc_next) {
        if (BLE_ADDR_IS_RPA(addr)) {
            if (ble_addr_cmp(&conn->bhc_peer_rpa_addr, addr) == 0) {
                return conn;
            }
        } else {
            if (ble_addr_cmp(&conn->bhc_peer_addr, addr) == 0) {
                return conn;
            }
            if (conn->bhc_peer_addr.type < BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT) {
                continue;
            }
            /*If type 0x02 or 0x03 is used, let's double check if address is good */
            ble_hs_conn_addrs(conn, &addrs);
            if (ble_addr_cmp(&addrs.peer_id_addr, addr) == 0) {
                return conn;
            }
        }
    }

    return NULL;
}

struct ble_hs_conn *
ble_hs_conn_find_by_idx(int idx)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    struct ble_hs_conn *conn;
    int i;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    i = 0;
    SLIST_FOREACH(conn, &ble_hs_conns, bhc_next) {
        if (i == idx) {
            return conn;
        }

        i++;
    }

    return NULL;
}

int
ble_hs_conn_exists(uint16_t conn_handle)
{
#if !NIMBLE_BLE_CONNECT
    return 0;
#endif
    return ble_hs_conn_find(conn_handle) != NULL;
}

/**
 * Retrieves the first connection in the list.
 */
struct ble_hs_conn *
ble_hs_conn_first(void)
{
#if !NIMBLE_BLE_CONNECT
    return NULL;
#endif

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());
    return SLIST_FIRST(&ble_hs_conns);
}

void
ble_hs_conn_addrs(const struct ble_hs_conn *conn,
                  struct ble_hs_conn_addrs *addrs)
{
    const uint8_t *our_id_addr_val;
    int rc;

    /* Determine our address information. */
    addrs->our_id_addr.type =
        ble_hs_misc_own_addr_type_to_id(conn->bhc_our_addr_type);

#if MYNEWT_VAL(BLE_EXT_ADV)
    /* With EA enabled random address for slave connection is per advertising
     * instance and requires special handling here.
     */

    if (!(conn->bhc_flags & BLE_HS_CONN_F_MASTER) &&
            addrs->our_id_addr.type == BLE_ADDR_RANDOM) {
        our_id_addr_val = conn->bhc_our_rnd_addr;
    } else {
        rc = ble_hs_id_addr(addrs->our_id_addr.type, &our_id_addr_val, NULL);
        assert(rc == 0);
    }
#else
    rc = ble_hs_id_addr(addrs->our_id_addr.type, &our_id_addr_val, NULL);
    assert(rc == 0);
#endif

    memcpy(addrs->our_id_addr.val, our_id_addr_val, 6);

    if (memcmp(conn->bhc_our_rpa_addr.val, ble_hs_conn_null_addr, 6) == 0) {
        addrs->our_ota_addr = addrs->our_id_addr;
    } else {
        addrs->our_ota_addr = conn->bhc_our_rpa_addr;
    }

    /* Determine peer address information. */
    addrs->peer_id_addr = conn->bhc_peer_addr;
    addrs->peer_ota_addr = conn->bhc_peer_addr;

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    /* RPA: Override peer address information. */
    ble_addr_t bhc_peer_addr;
    bhc_peer_addr.type = conn->bhc_peer_addr.type;
    memcpy(bhc_peer_addr.val, conn->bhc_peer_addr.val, BLE_DEV_ADDR_LEN);

    struct ble_hs_resolv_entry *rl = NULL;
    rl = ble_hs_resolv_list_find(bhc_peer_addr.val);
    if (rl != NULL) {
        memcpy(addrs->peer_id_addr.val, rl->rl_identity_addr, BLE_DEV_ADDR_LEN);
        addrs->peer_id_addr.type = rl->rl_addr_type;

        if (ble_host_rpa_enabled()) {
            uint8_t *local_id = NULL;
            ble_hs_id_addr(BLE_ADDR_PUBLIC, (const uint8_t **) &local_id, NULL);

            /* RL is present: populate our id addr with public ID */
            memcpy(addrs->our_id_addr.val, local_id, BLE_DEV_ADDR_LEN);
            addrs->our_id_addr.type = BLE_ADDR_PUBLIC;
            BLE_HS_LOG(DEBUG, "Revised our id addr:\n");
            ble_hs_log_flat_buf(our_id_addr_val, BLE_DEV_ADDR_LEN);
            BLE_HS_LOG(DEBUG, "\n");
        }
    }
#endif
    switch (conn->bhc_peer_addr.type) {
    case BLE_ADDR_PUBLIC:
    case BLE_ADDR_RANDOM:
        break;

    case BLE_ADDR_PUBLIC_ID:
        addrs->peer_id_addr.type = BLE_ADDR_PUBLIC;
        addrs->peer_ota_addr = conn->bhc_peer_rpa_addr;
        break;

    case BLE_ADDR_RANDOM_ID:
        addrs->peer_id_addr.type = BLE_ADDR_RANDOM;
        addrs->peer_ota_addr = conn->bhc_peer_rpa_addr;
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        break;
    }
}

int32_t
ble_hs_conn_timer(void)
{
    /* If there are no timeouts configured, then there is nothing to check. */
#if MYNEWT_VAL(BLE_L2CAP_RX_FRAG_TIMEOUT) == 0 && \
    BLE_HS_ATT_SVR_QUEUED_WRITE_TMO == 0

    return BLE_HS_FOREVER;
#endif

    struct ble_hs_conn *conn;
    ble_npl_time_t now;
    int32_t next_exp_in;
    int32_t time_diff;
    uint16_t conn_handle;

    for (;;) {
        conn_handle = BLE_HS_CONN_HANDLE_NONE;
        next_exp_in = BLE_HS_FOREVER;
        now = ble_npl_time_get();

        ble_hs_lock();

        /* This loop performs one of two tasks:
         * 1. Determine if any connections need to be terminated due to timeout.
         *    If so, break out of the loop and terminate the connection.  This
         *    function will need to be executed again.
         * 2. Otherwise, determine when the next timeout will occur.
         */
        SLIST_FOREACH(conn, &ble_hs_conns, bhc_next) {
            if (!(conn->bhc_flags & BLE_HS_CONN_F_TERMINATING)) {

#if MYNEWT_VAL(BLE_L2CAP_RX_FRAG_TIMEOUT) != 0
                /* Check each connection's rx fragment timer.  If too much time
                 * passes after a partial packet is received, the connection is
                 * terminated.
                 */
                if (conn->bhc_rx_chan != NULL) {
                    time_diff = conn->bhc_rx_timeout - now;

                    if (time_diff <= 0) {
                        /* ACL reassembly has timed out.  Remember the connection
                         * handle so it can be terminated after the mutex is
                         * unlocked.
                         */
                        conn_handle = conn->bhc_handle;
                        break;
                    }

                    /* Determine if this connection is the soonest to time out. */
                    if (time_diff < next_exp_in) {
                        next_exp_in = time_diff;
                    }
                }
#endif

#if BLE_HS_ATT_SVR_QUEUED_WRITE_TMO
                /* Check each connection's rx queued write timer.  If too much
                 * time passes after a prep write is received, the queue is
                 * cleared.
                 */
                time_diff = ble_att_svr_ticks_until_tmo(&conn->bhc_att_svr, now);
                if (time_diff <= 0) {
                    /* ACL reassembly has timed out.  Remember the connection
                     * handle so it can be terminated after the mutex is
                     * unlocked.
                     */
                    conn_handle = conn->bhc_handle;
                    break;
                }

                /* Determine if this connection is the soonest to time out. */
                if (time_diff < next_exp_in) {
                    next_exp_in = time_diff;
                }
#endif
            }
        }

        ble_hs_unlock();

        /* If a connection has timed out, terminate it.  We need to repeatedly
         * call this function again to determine when the next timeout is.
         */
        if (conn_handle != BLE_HS_CONN_HANDLE_NONE) {
            ble_gap_terminate(conn_handle, BLE_ERR_REM_USER_CONN_TERM);
            continue;
        }

        return next_exp_in;
    }
}

int
ble_hs_conn_init(void)
{
    int rc;

    rc = os_mempool_init(&ble_hs_conn_pool, MYNEWT_VAL(BLE_MAX_CONNECTIONS),
                         sizeof (struct ble_hs_conn),
                         ble_hs_conn_elem_mem, "ble_hs_conn_pool");
    if (rc != 0) {
        return BLE_HS_EOS;
    }

    SLIST_INIT(&ble_hs_conns);

    return 0;
}
