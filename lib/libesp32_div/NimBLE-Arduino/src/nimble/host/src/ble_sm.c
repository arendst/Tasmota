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
 * L2CAP Security Manager (channel ID = 6).
 *
 * Design overview:
 *
 * L2CAP sm procedures are initiated by the application via function calls.
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
 * 2. Keep the host mutex locked whenever:
 *      o A proc entry is read from or written to.
 *      o The proc list is read or modified.
 */

#include <string.h>
#include <errno.h>
#include "nimble/ble.h"
#include "nimble/nimble_opt.h"
#include "host/ble_sm.h"
#include "ble_hs_priv.h"
#include "ble_hs_resolv_priv.h"
#include "../store/config/src/ble_store_config_priv.h"

#if NIMBLE_BLE_SM

/** Procedure timeout; 30 seconds. */
#define BLE_SM_TIMEOUT_MS             (30000)

STAILQ_HEAD(ble_sm_proc_list, ble_sm_proc);

typedef void ble_sm_rx_fn(uint16_t conn_handle, struct os_mbuf **om,
                          struct ble_sm_result *res);

static ble_sm_rx_fn ble_sm_rx_noop;
static ble_sm_rx_fn ble_sm_pair_req_rx;
static ble_sm_rx_fn ble_sm_pair_rsp_rx;
static ble_sm_rx_fn ble_sm_confirm_rx;
static ble_sm_rx_fn ble_sm_random_rx;
static ble_sm_rx_fn ble_sm_fail_rx;
static ble_sm_rx_fn ble_sm_enc_info_rx;
static ble_sm_rx_fn ble_sm_master_id_rx;
static ble_sm_rx_fn ble_sm_id_info_rx;
static ble_sm_rx_fn ble_sm_id_addr_info_rx;
static ble_sm_rx_fn ble_sm_sign_info_rx;
static ble_sm_rx_fn ble_sm_sec_req_rx;

static ble_sm_rx_fn * const ble_sm_dispatch[] = {
   [BLE_SM_OP_PAIR_REQ] = ble_sm_pair_req_rx,
   [BLE_SM_OP_PAIR_RSP] = ble_sm_pair_rsp_rx,
   [BLE_SM_OP_PAIR_CONFIRM] = ble_sm_confirm_rx,
   [BLE_SM_OP_PAIR_RANDOM] = ble_sm_random_rx,
   [BLE_SM_OP_PAIR_FAIL] = ble_sm_fail_rx,
   [BLE_SM_OP_ENC_INFO] = ble_sm_enc_info_rx,
   [BLE_SM_OP_MASTER_ID] = ble_sm_master_id_rx,
   [BLE_SM_OP_IDENTITY_INFO] = ble_sm_id_info_rx,
   [BLE_SM_OP_IDENTITY_ADDR_INFO] = ble_sm_id_addr_info_rx,
   [BLE_SM_OP_SIGN_INFO] = ble_sm_sign_info_rx,
   [BLE_SM_OP_SEC_REQ] = ble_sm_sec_req_rx,
   [BLE_SM_OP_PAIR_KEYPRESS_NOTIFY] = ble_sm_rx_noop,
#if MYNEWT_VAL(BLE_SM_SC)
   [BLE_SM_OP_PAIR_PUBLIC_KEY] = ble_sm_sc_public_key_rx,
   [BLE_SM_OP_PAIR_DHKEY_CHECK] = ble_sm_sc_dhkey_check_rx,
#else
   [BLE_SM_OP_PAIR_PUBLIC_KEY] = ble_sm_rx_noop,
   [BLE_SM_OP_PAIR_DHKEY_CHECK] = ble_sm_rx_noop,
#endif
};

struct hci_start_encrypt
{
    uint16_t connection_handle;
    uint16_t encrypted_diversifier;
    uint64_t random_number;
    uint8_t long_term_key[16];
};

typedef void ble_sm_state_fn(struct ble_sm_proc *proc,
                             struct ble_sm_result *res, void *arg);

static ble_sm_state_fn ble_sm_pair_exec;
static ble_sm_state_fn ble_sm_confirm_exec;
static ble_sm_state_fn ble_sm_random_exec;
static ble_sm_state_fn ble_sm_ltk_start_exec;
static ble_sm_state_fn ble_sm_ltk_restore_exec;
static ble_sm_state_fn ble_sm_enc_start_exec;
static ble_sm_state_fn ble_sm_enc_restore_exec;
static ble_sm_state_fn ble_sm_key_exch_exec;
static ble_sm_state_fn ble_sm_sec_req_exec;

static ble_sm_state_fn * const
ble_sm_state_dispatch[BLE_SM_PROC_STATE_CNT] = {
    [BLE_SM_PROC_STATE_PAIR]          = ble_sm_pair_exec,
    [BLE_SM_PROC_STATE_CONFIRM]       = ble_sm_confirm_exec,
    [BLE_SM_PROC_STATE_RANDOM]        = ble_sm_random_exec,
    [BLE_SM_PROC_STATE_LTK_START]     = ble_sm_ltk_start_exec,
    [BLE_SM_PROC_STATE_LTK_RESTORE]   = ble_sm_ltk_restore_exec,
    [BLE_SM_PROC_STATE_ENC_START]     = ble_sm_enc_start_exec,
    [BLE_SM_PROC_STATE_ENC_RESTORE]   = ble_sm_enc_restore_exec,
    [BLE_SM_PROC_STATE_KEY_EXCH]      = ble_sm_key_exch_exec,
    [BLE_SM_PROC_STATE_SEC_REQ]       = ble_sm_sec_req_exec,
#if MYNEWT_VAL(BLE_SM_SC)
    [BLE_SM_PROC_STATE_PUBLIC_KEY]    = ble_sm_sc_public_key_exec,
    [BLE_SM_PROC_STATE_DHKEY_CHECK]   = ble_sm_sc_dhkey_check_exec,
#else
    [BLE_SM_PROC_STATE_PUBLIC_KEY]    = NULL,
    [BLE_SM_PROC_STATE_DHKEY_CHECK]   = NULL,
#endif
};

static os_membuf_t ble_sm_proc_mem[
    OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_SM_MAX_PROCS),
                    sizeof (struct ble_sm_proc))
];

static struct os_mempool ble_sm_proc_pool;

/* Maintains the list of active security manager procedures. */
static struct ble_sm_proc_list ble_sm_procs;

static void ble_sm_pair_cfg(struct ble_sm_proc *proc);


/*****************************************************************************
 * $debug                                                                    *
 *****************************************************************************/

#if MYNEWT_VAL(BLE_HS_DEBUG)

static uint8_t ble_sm_dbg_next_pair_rand[16];
static uint8_t ble_sm_dbg_next_pair_rand_set;
static uint16_t ble_sm_dbg_next_ediv;
static uint8_t ble_sm_dbg_next_ediv_set;
static uint64_t ble_sm_dbg_next_master_id_rand;
static uint8_t ble_sm_dbg_next_master_id_rand_set;
static uint8_t ble_sm_dbg_next_ltk[16];
static uint8_t ble_sm_dbg_next_ltk_set;
static uint8_t ble_sm_dbg_next_csrk[16];
static uint8_t ble_sm_dbg_next_csrk_set;

void
ble_sm_dbg_set_next_pair_rand(uint8_t *next_pair_rand)
{
    memcpy(ble_sm_dbg_next_pair_rand, next_pair_rand,
           sizeof ble_sm_dbg_next_pair_rand);
    ble_sm_dbg_next_pair_rand_set = 1;
}

void
ble_sm_dbg_set_next_ediv(uint16_t next_ediv)
{
    ble_sm_dbg_next_ediv = next_ediv;
    ble_sm_dbg_next_ediv_set = 1;
}

void
ble_sm_dbg_set_next_master_id_rand(uint64_t next_master_id_rand)
{
    ble_sm_dbg_next_master_id_rand = next_master_id_rand;
    ble_sm_dbg_next_master_id_rand_set = 1;
}

void
ble_sm_dbg_set_next_ltk(uint8_t *next_ltk)
{
    memcpy(ble_sm_dbg_next_ltk, next_ltk,
           sizeof ble_sm_dbg_next_ltk);
    ble_sm_dbg_next_ltk_set = 1;
}

void
ble_sm_dbg_set_next_csrk(uint8_t *next_csrk)
{
    memcpy(ble_sm_dbg_next_csrk, next_csrk,
           sizeof ble_sm_dbg_next_csrk);
    ble_sm_dbg_next_csrk_set = 1;
}

#endif

static void
ble_sm_dbg_assert_no_cycles(void)
{
#if MYNEWT_VAL(BLE_HS_DEBUG)
    ble_sm_num_procs();
#endif
}

static void
ble_sm_dbg_assert_not_inserted(struct ble_sm_proc *proc)
{
#if MYNEWT_VAL(BLE_HS_DEBUG)
    struct ble_sm_proc *cur;

    STAILQ_FOREACH(cur, &ble_sm_procs, next) {
        BLE_HS_DBG_ASSERT(cur != proc);
    }
#endif
}

/*****************************************************************************
 * $misc                                                                     *
 *****************************************************************************/

/**
 * Calculates the number of active SM procedures.
 */
int
ble_sm_num_procs(void)
{
    struct ble_sm_proc *proc;
    int cnt;

    cnt = 0;
    STAILQ_FOREACH(proc, &ble_sm_procs, next) {
        BLE_HS_DBG_ASSERT(cnt < MYNEWT_VAL(BLE_SM_MAX_PROCS));
        cnt++;
    }

    return cnt;
}

int
ble_sm_gen_pair_rand(uint8_t *pair_rand)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_next_pair_rand_set) {
        ble_sm_dbg_next_pair_rand_set = 0;
        memcpy(pair_rand, ble_sm_dbg_next_pair_rand,
               sizeof ble_sm_dbg_next_pair_rand);
        return 0;
    }
#endif

    rc = ble_hs_hci_util_rand(pair_rand, 16);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_sm_gen_ediv(struct ble_sm_master_id *master_id)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_next_ediv_set) {
        ble_sm_dbg_next_ediv_set = 0;
        master_id->ediv = ble_sm_dbg_next_ediv;
        return 0;
    }
#endif

    rc = ble_hs_hci_util_rand(&master_id->ediv, sizeof master_id->ediv);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_sm_gen_master_id_rand(struct ble_sm_master_id *master_id)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_next_master_id_rand_set) {
        ble_sm_dbg_next_master_id_rand_set = 0;
        master_id->rand_val = ble_sm_dbg_next_master_id_rand;
        return 0;
    }
#endif

    rc = ble_hs_hci_util_rand(&master_id->rand_val, sizeof master_id->rand_val);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_sm_gen_ltk(struct ble_sm_proc *proc, uint8_t *ltk)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_next_ltk_set) {
        ble_sm_dbg_next_ltk_set = 0;
        memcpy(ltk, ble_sm_dbg_next_ltk,
               sizeof ble_sm_dbg_next_ltk);
        return 0;
    }
#endif

    rc = ble_hs_hci_util_rand(ltk, proc->key_size);
    if (rc != 0) {
        return rc;
    }

    /* Ensure proper key size */
    memset(ltk + proc->key_size, 0, sizeof proc->ltk - proc->key_size);

    return 0;
}

static int
ble_sm_gen_csrk(struct ble_sm_proc *proc, uint8_t *csrk)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_next_csrk_set) {
        ble_sm_dbg_next_csrk_set = 0;
        memcpy(csrk, ble_sm_dbg_next_csrk,
               sizeof ble_sm_dbg_next_csrk);
        return 0;
    }
#endif

    rc = ble_hs_hci_util_rand(csrk, 16);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static void
ble_sm_proc_set_timer(struct ble_sm_proc *proc)
{
    proc->exp_os_ticks = ble_npl_time_get() +
                         ble_npl_time_ms_to_ticks32(BLE_SM_TIMEOUT_MS);
    ble_hs_timer_resched();
}

static ble_sm_rx_fn *
ble_sm_dispatch_get(uint8_t op)
{
    if (op >= sizeof ble_sm_dispatch / sizeof ble_sm_dispatch[0]) {
        return NULL;
    }

    return ble_sm_dispatch[op];
}

/**
 * Allocates a proc entry.
 *
 * @return                      An entry on success; null on failure.
 */
static struct ble_sm_proc *
ble_sm_proc_alloc(void)
{
    struct ble_sm_proc *proc;

    proc = os_memblock_get(&ble_sm_proc_pool);
    if (proc != NULL) {
        memset(proc, 0, sizeof *proc);
    }

    return proc;
}

/**
 * Frees the specified proc entry.  No-state if passed a null pointer.
 */
static void
ble_sm_proc_free(struct ble_sm_proc *proc)
{
    int rc;

    if (proc != NULL) {
        ble_sm_dbg_assert_not_inserted(proc);
#if MYNEWT_VAL(BLE_HS_DEBUG)
        memset(proc, 0xff, sizeof *proc);
#endif
        rc = os_memblock_put(&ble_sm_proc_pool, proc);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
    }
}

static void
ble_sm_proc_remove(struct ble_sm_proc *proc,
                         struct ble_sm_proc *prev)
{
    if (prev == NULL) {
        BLE_HS_DBG_ASSERT(STAILQ_FIRST(&ble_sm_procs) == proc);
        STAILQ_REMOVE_HEAD(&ble_sm_procs, next);
    } else {
        BLE_HS_DBG_ASSERT(STAILQ_NEXT(prev, next) == proc);
        STAILQ_REMOVE_AFTER(&ble_sm_procs, prev, next);
    }

    ble_sm_dbg_assert_no_cycles();
}

static void
ble_sm_update_sec_state(uint16_t conn_handle, int encrypted,
                        int authenticated, int bonded, int key_size)
{
    struct ble_hs_conn *conn;

    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        conn->bhc_sec_state.encrypted = encrypted;

        /* Authentication and bonding are never revoked from a secure link */
        if (authenticated) {
            conn->bhc_sec_state.authenticated = 1;
        }
        if (bonded) {
            conn->bhc_sec_state.bonded = 1;
        }

        if (key_size) {
            conn->bhc_sec_state.key_size = key_size;
        }
    }
}

static void
ble_sm_fill_store_value(const ble_addr_t *peer_addr,
                        int authenticated,
                        int sc,
                        struct ble_sm_keys *keys,
                        struct ble_store_value_sec *value_sec)
{
    memset(value_sec, 0, sizeof *value_sec);

    value_sec->peer_addr = *peer_addr;

    if (keys->ediv_rand_valid && keys->ltk_valid) {
        value_sec->key_size = keys->key_size;
        value_sec->ediv = keys->ediv;
        value_sec->rand_num = keys->rand_val;

        memcpy(value_sec->ltk, keys->ltk, sizeof value_sec->ltk);
        value_sec->ltk_present = 1;

        value_sec->authenticated = !!authenticated;
        value_sec->sc = !!sc;
    }

    if (keys->irk_valid) {
        memcpy(value_sec->irk, keys->irk, sizeof value_sec->irk);
        value_sec->irk_present = 1;
    }

    if (keys->csrk_valid) {
        memcpy(value_sec->csrk, keys->csrk, sizeof value_sec->csrk);
        value_sec->csrk_present = 1;
    }
}

void
ble_sm_ia_ra(struct ble_sm_proc *proc,
             uint8_t *out_iat, uint8_t *out_ia,
             uint8_t *out_rat, uint8_t *out_ra)
{
    struct ble_hs_conn_addrs addrs;
    struct ble_hs_conn *conn;

    conn = ble_hs_conn_find_assert(proc->conn_handle);

    ble_hs_conn_addrs(conn, &addrs);

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        *out_iat = addrs.our_ota_addr.type;
        memcpy(out_ia, addrs.our_ota_addr.val, 6);

        *out_rat = addrs.peer_ota_addr.type;
        memcpy(out_ra, addrs.peer_ota_addr.val, 6);
    } else {
        *out_iat = addrs.peer_ota_addr.type;
        memcpy(out_ia, addrs.peer_ota_addr.val, 6);

        *out_rat = addrs.our_ota_addr.type;
        memcpy(out_ra, addrs.our_ota_addr.val, 6);
    }
}

static void
ble_sm_persist_keys(struct ble_sm_proc *proc)
{
    struct ble_store_value_sec value_sec;
    struct ble_hs_conn *conn;
    ble_addr_t peer_addr;
    int authenticated;
    int identity_ev = 0;
    int sc;

    ble_hs_lock();

    conn = ble_hs_conn_find(proc->conn_handle);
    BLE_HS_DBG_ASSERT(conn != NULL);

    /* If we got an identity address, use that for key storage. */
    if (proc->peer_keys.addr_valid) {
        peer_addr.type = proc->peer_keys.addr_type;
        memcpy(peer_addr.val, proc->peer_keys.addr, sizeof peer_addr.val);

        conn->bhc_peer_addr = peer_addr;

        /* Update identity address in conn.
         * If peer's rpa address is set then it means that the peer's address
         * is an identity address. The peer's address type has to be
         * set as 'ID' to allow resolve 'id' and 'ota' addresses properly in
         * conn info.
         */
        if (memcmp(BLE_ADDR_ANY->val, &conn->bhc_peer_rpa_addr.val, 6) != 0) {
            switch (peer_addr.type) {
            case BLE_ADDR_PUBLIC:
            case BLE_ADDR_PUBLIC_ID:
                conn->bhc_peer_addr.type = BLE_ADDR_PUBLIC_ID;
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
                /* In case of Host based privacy, we should not be changing
                 * peer address type to BLE_ADDR_PUBLIC_ID */
                conn->bhc_peer_addr.type = BLE_ADDR_PUBLIC;
#endif
                break;

            case BLE_ADDR_RANDOM:
            case BLE_ADDR_RANDOM_ID:
                conn->bhc_peer_addr.type = BLE_ADDR_RANDOM_ID;
                break;
            }

            identity_ev = 1;
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
            struct ble_hs_dev_records *p_dev_rec =
                                      ble_rpa_find_peer_dev_rec(conn->bhc_peer_rpa_addr.val);
            if (p_dev_rec == NULL) {
                if (!ble_rpa_resolv_add_peer_rec(conn->bhc_peer_rpa_addr.val)) {
                    p_dev_rec = ble_rpa_find_peer_dev_rec(conn->bhc_peer_rpa_addr.val);
                }
            }

            if (p_dev_rec != NULL) {
                /* Once bonded, copy the peer device records */
                swap_buf(p_dev_rec->peer_sec.irk, proc->peer_keys.irk, 16);
                p_dev_rec->peer_sec.irk_present = proc->peer_keys.irk_valid;
                memcpy(p_dev_rec->peer_sec.peer_addr.val,
                       proc->peer_keys.addr, 6);
                p_dev_rec->peer_sec.peer_addr.type = proc->peer_keys.addr_type;

                ble_store_persist_peer_records();
            }
#endif
        }
    } else {
        peer_addr = conn->bhc_peer_addr;
        peer_addr.type =
            ble_hs_misc_peer_addr_type_to_id(conn->bhc_peer_addr.type);
    }

    ble_hs_unlock();

    if (identity_ev) {
        ble_gap_identity_event(proc->conn_handle);
    }

    authenticated = proc->flags & BLE_SM_PROC_F_AUTHENTICATED;
    sc = proc->flags & BLE_SM_PROC_F_SC;

    ble_sm_fill_store_value(&peer_addr, authenticated, sc, &proc->our_keys,
                            &value_sec);
    ble_store_write_our_sec(&value_sec);

    ble_sm_fill_store_value(&peer_addr, authenticated, sc, &proc->peer_keys,
                            &value_sec);
    ble_store_write_peer_sec(&value_sec);
}

static int
ble_sm_proc_matches(struct ble_sm_proc *proc, uint16_t conn_handle,
                    uint8_t state, int is_initiator)
{
    int proc_is_initiator;

    if (conn_handle != proc->conn_handle) {
        return 0;
    }

    if (state != BLE_SM_PROC_STATE_NONE && state != proc->state) {
        return 0;
    }

    proc_is_initiator = !!(proc->flags & BLE_SM_PROC_F_INITIATOR);
    if (is_initiator != -1 && is_initiator != proc_is_initiator) {
        return 0;
    }

    return 1;
}

/**
 * Searches the main proc list for an entry whose connection handle and state
 * code match those specified.
 *
 * @param conn_handle           The connection handle to match against.
 * @param state                 The state code to match against.
 * @param is_initiator          Matches on the proc's initiator flag:
 *                                   0=non-initiator only
 *                                   1=initiator only
 *                                  -1=don't care
 * @param out_prev              On success, the entry previous to the result is
 *                                  written here.
 *
 * @return                      The matching proc entry on success;
 *                                  null on failure.
 */
struct ble_sm_proc *
ble_sm_proc_find(uint16_t conn_handle, uint8_t state, int is_initiator,
                 struct ble_sm_proc **out_prev)
{
    struct ble_sm_proc *proc;
    struct ble_sm_proc *prev;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    prev = NULL;
    STAILQ_FOREACH(proc, &ble_sm_procs, next) {
        if (ble_sm_proc_matches(proc, conn_handle, state, is_initiator)) {
            if (out_prev != NULL) {
                *out_prev = prev;
            }
            break;
        }

        prev = proc;
    }

    return proc;
}

static void
ble_sm_insert(struct ble_sm_proc *proc)
{
#if MYNEWT_VAL(BLE_HS_DEBUG)
    struct ble_sm_proc *cur;

    STAILQ_FOREACH(cur, &ble_sm_procs, next) {
        BLE_HS_DBG_ASSERT(cur != proc);
    }
#endif

    STAILQ_INSERT_HEAD(&ble_sm_procs, proc, next);
}

static int32_t
ble_sm_extract_expired(struct ble_sm_proc_list *dst_list)
{
    struct ble_sm_proc *proc;
    struct ble_sm_proc *prev;
    struct ble_sm_proc *next;
    ble_npl_time_t now;
    ble_npl_stime_t next_exp_in;
    ble_npl_stime_t time_diff;

    now = ble_npl_time_get();
    STAILQ_INIT(dst_list);

    /* Assume each event is either expired or has infinite duration. */
    next_exp_in = BLE_HS_FOREVER;

    ble_hs_lock();

    prev = NULL;
    proc = STAILQ_FIRST(&ble_sm_procs);
    while (proc != NULL) {
        next = STAILQ_NEXT(proc, next);

        time_diff = proc->exp_os_ticks - now;
        if (time_diff <= 0) {
            /* Procedure has expired; move it to the destination list. */
            if (prev == NULL) {
                STAILQ_REMOVE_HEAD(&ble_sm_procs, next);
            } else {
                STAILQ_REMOVE_AFTER(&ble_sm_procs, prev, next);
            }
            STAILQ_INSERT_HEAD(dst_list, proc, next);
        } else {
            if (time_diff < next_exp_in) {
                next_exp_in = time_diff;
            }
        }

        prev = proc;
        proc = next;
    }

    ble_sm_dbg_assert_no_cycles();

    ble_hs_unlock();

    return next_exp_in;
}

static void
ble_sm_rx_noop(uint16_t conn_handle, struct os_mbuf **om,
               struct ble_sm_result *res)
{
    res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_CMD_NOT_SUPP);
    res->sm_err = BLE_SM_ERR_CMD_NOT_SUPP;
}

static uint8_t
ble_sm_build_authreq(void)
{
    return ble_hs_cfg.sm_bonding << 0  |
           ble_hs_cfg.sm_mitm << 2     |
           ble_hs_cfg.sm_sc << 3       |
           ble_hs_cfg.sm_keypress << 4;
}

static int
ble_sm_io_action(struct ble_sm_proc *proc, uint8_t *action)
{
    if (proc->flags & BLE_SM_PROC_F_SC) {
        return ble_sm_sc_io_action(proc, action);
    } else {
        return ble_sm_lgcy_io_action(proc, action);
    }
}

int
ble_sm_ioact_state(uint8_t action)
{
    switch (action) {
    case BLE_SM_IOACT_NONE:
        return BLE_SM_PROC_STATE_NONE;

    case BLE_SM_IOACT_NUMCMP:
        return BLE_SM_PROC_STATE_DHKEY_CHECK;

    case BLE_SM_IOACT_OOB_SC:
        return BLE_SM_PROC_STATE_RANDOM;

    case BLE_SM_IOACT_OOB:
    case BLE_SM_IOACT_INPUT:
    case BLE_SM_IOACT_DISP:
        return BLE_SM_PROC_STATE_CONFIRM;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_SM_PROC_STATE_NONE;
    }
}

int
ble_sm_proc_can_advance(struct ble_sm_proc *proc)
{
    uint8_t ioact;
    int rc;

    rc = ble_sm_io_action(proc, &ioact);
    if (rc != 0) {
        BLE_HS_DBG_ASSERT(0);
    }

    if (ble_sm_ioact_state(ioact) != proc->state) {
        return 1;
    }

    if (proc->flags & BLE_SM_PROC_F_IO_INJECTED &&
        proc->flags & BLE_SM_PROC_F_ADVANCE_ON_IO) {

        return 1;
    }

    return 0;
}

static void
ble_sm_exec(struct ble_sm_proc *proc, struct ble_sm_result *res, void *arg)
{
    ble_sm_state_fn *cb;

    memset(res, 0, sizeof *res);

    if (!ble_hs_conn_exists(proc->conn_handle)) {
        res->app_status = BLE_HS_ENOTCONN;
    } else {
        BLE_HS_DBG_ASSERT(proc->state < BLE_SM_PROC_STATE_CNT);
        cb = ble_sm_state_dispatch[proc->state];
        BLE_HS_DBG_ASSERT(cb != NULL);
        cb(proc, res, arg);
    }
}

static void
ble_sm_pair_fail_tx(uint16_t conn_handle, uint8_t reason)
{
    struct ble_sm_pair_fail *cmd;
    struct os_mbuf *txom;
    int rc;

    BLE_HS_DBG_ASSERT(reason > 0 && reason < BLE_SM_ERR_MAX_PLUS_1);

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_FAIL, sizeof(*cmd), &txom);
    if (cmd) {
        cmd->reason = reason;
        rc = ble_sm_tx(conn_handle, txom);
        if (rc) {
            BLE_HS_LOG(ERROR, "ble_sm_pair_fail_tx failed, rc = %d\n", rc);
        }
    }
}

/**
 * Reads a bond from storage.
 */
static int
ble_sm_read_bond(uint16_t conn_handle, struct ble_store_value_sec *out_bond)
{
    struct ble_store_key_sec key_sec;
    struct ble_gap_conn_desc desc;
    int rc;

    rc = ble_gap_conn_find(conn_handle, &desc);
    if (rc != 0) {
        return rc;
    }

    memset(&key_sec, 0, sizeof key_sec);
    key_sec.peer_addr = desc.peer_id_addr;

    rc = ble_store_read_peer_sec(&key_sec, out_bond);
    return rc;
}

/**
 * Checks if the specified peer is already bonded.  If it is, the application
 * is queried about how to proceed: retry or ignore.  The application should
 * only indicate a retry if it deleted the old bond.
 *
 * @param conn_handle           The handle of the connection over which the
 *                                  pairing request was received.
 * @param proc_flags            The security flags associated with the
 *                                  conflicting SM procedure.
 * @param key_size              The key size of the conflicting SM procedure.
 *
 * @return                      0 if the procedure should continue;
 *                              nonzero if the request should be ignored.
 */
static int
ble_sm_chk_repeat_pairing(uint16_t conn_handle,
                          ble_sm_proc_flags proc_flags,
                          uint8_t key_size)
{
    struct ble_gap_repeat_pairing rp;
    struct ble_store_value_sec bond;
    int rc;

    do {
        /* If the peer isn't bonded, indicate that the pairing procedure should
         * continue.
         */
        rc = ble_sm_read_bond(conn_handle, &bond);
        switch (rc) {
        case 0:
            break;
        case BLE_HS_ENOENT:
            return 0;
        default:
            return rc;
        }

        /* Peer is already bonded.  Ask the application what to do about it. */
        rp.conn_handle = conn_handle;
        rp.cur_key_size = bond.key_size;
        rp.cur_authenticated = bond.authenticated;
        rp.cur_sc = bond.sc;

        rp.new_key_size = key_size;
        rp.new_authenticated = !!(proc_flags & BLE_SM_PROC_F_AUTHENTICATED);
        rp.new_sc = !!(proc_flags & BLE_SM_PROC_F_SC);
        rp.new_bonding = !!(proc_flags & BLE_SM_PROC_F_BONDING);

        rc = ble_gap_repeat_pairing_event(&rp);
    } while (rc == BLE_GAP_REPEAT_PAIRING_RETRY);

    BLE_HS_LOG(DEBUG, "silently ignoring pair request from bonded peer");

    return BLE_HS_EALREADY;
}

void
ble_sm_process_result(uint16_t conn_handle, struct ble_sm_result *res)
{
    struct ble_sm_proc *prev;
    struct ble_sm_proc *proc;
    int rm;

    rm = 0;

    while (1) {
        ble_hs_lock();
        proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1,
                                &prev);

        if (proc != NULL) {
            if (res->execute) {
                ble_sm_exec(proc, res, res->state_arg);
            }

            if (res->app_status != 0) {
                rm = 1;
            }

            if (proc->state == BLE_SM_PROC_STATE_NONE) {
                rm = 1;
            }

            if (rm) {
                ble_sm_proc_remove(proc, prev);
            } else {
                ble_sm_proc_set_timer(proc);
            }
        }

        if (res->sm_err != 0) {
            ble_sm_pair_fail_tx(conn_handle, res->sm_err);
        }

        ble_hs_unlock();

        if (proc == NULL) {
            break;
        }

        if (res->enc_cb) {
            BLE_HS_DBG_ASSERT(proc == NULL || rm);
            ble_gap_enc_event(conn_handle, res->app_status, res->restore, res->bonded);
        }

        if (res->app_status == 0 &&
            res->passkey_params.action != BLE_SM_IOACT_NONE) {

            ble_gap_passkey_event(conn_handle, &res->passkey_params);
        }

        /* Persist keys if bonding has successfully completed. */
        if (res->app_status == 0    &&
            rm                      &&
            proc->flags & BLE_SM_PROC_F_BONDING) {

            ble_sm_persist_keys(proc);
        }

        if (rm) {
            ble_sm_proc_free(proc);
            break;
        }

        if (!res->execute) {
            break;
        }

        memset(res, 0, sizeof *res);
        res->execute = 1;
    }
}

static void
ble_sm_key_dist(struct ble_sm_proc *proc,
                uint8_t *out_init_key_dist, uint8_t *out_resp_key_dist)
{
    struct ble_sm_pair_cmd *pair_rsp;

    pair_rsp = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];

    *out_init_key_dist = pair_rsp->init_key_dist;
    *out_resp_key_dist = pair_rsp->resp_key_dist;

    /* Encryption info and master ID are only sent in legacy pairing. */
    if (proc->flags & BLE_SM_PROC_F_SC) {
        *out_init_key_dist &= ~BLE_SM_PAIR_KEY_DIST_ENC;
        *out_resp_key_dist &= ~BLE_SM_PAIR_KEY_DIST_ENC;
    }
}

static int
ble_sm_chk_store_overflow_by_type(int obj_type, uint16_t conn_handle)
{
#if !MYNEWT_VAL(BLE_SM_BONDING)
    return 0;
#endif

    int count;
    int rc;

    rc = ble_store_util_count(obj_type, &count);
    if (rc != 0) {
        return rc;
    }

    /* Pessimistically assume all active procs will persist bonds. */
    ble_hs_lock();
    count += ble_sm_num_procs();
    ble_hs_unlock();

    if (count < MYNEWT_VAL(BLE_STORE_MAX_BONDS)) {
        /* There is sufficient capacity for another bond. */
        return 0;
    }

    /* No capacity for an additional bond.  Tell the application to make
     * room.
     */
    rc = ble_store_full_event(obj_type, conn_handle);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_sm_chk_store_overflow(uint16_t conn_handle)
{
    int rc;

    rc = ble_sm_chk_store_overflow_by_type(BLE_STORE_OBJ_TYPE_PEER_SEC,
                                           conn_handle);
    if (rc != 0) {
        return rc;
    }

    rc = ble_sm_chk_store_overflow_by_type(BLE_STORE_OBJ_TYPE_OUR_SEC,
                                           conn_handle);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/*****************************************************************************
 * $enc                                                                      *
 *****************************************************************************/

static int
ble_sm_start_encrypt_tx(struct hci_start_encrypt *params)
{
    struct ble_hci_le_start_encrypt_cp cmd;

    cmd.conn_handle = htole16(params->connection_handle);
    cmd.div = htole16(params->encrypted_diversifier);
    cmd.rand = htole64(params->random_number);
    memcpy(cmd.ltk, params->long_term_key, sizeof(cmd.ltk));

    return ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                        BLE_HCI_OCF_LE_START_ENCRYPT),
                             &cmd, sizeof(cmd), NULL, 0);
}

static void
ble_sm_enc_start_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                      void *arg)
{
    struct hci_start_encrypt cmd;
    int rc;

    BLE_HS_DBG_ASSERT(proc->flags & BLE_SM_PROC_F_INITIATOR);

    cmd.connection_handle = proc->conn_handle;
    cmd.encrypted_diversifier = 0;
    cmd.random_number = 0;
    memcpy(cmd.long_term_key, proc->ltk, sizeof cmd.long_term_key);

    rc = ble_sm_start_encrypt_tx(&cmd);
    if (rc != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->app_status = rc;
        res->enc_cb = 1;
    }
}

static void
ble_sm_enc_restore_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                        void *arg)
{
    struct hci_start_encrypt *cmd;

    BLE_HS_DBG_ASSERT(proc->flags & BLE_SM_PROC_F_INITIATOR);

    cmd = arg;
    BLE_HS_DBG_ASSERT(cmd != NULL);

    res->app_status = ble_sm_start_encrypt_tx(cmd);
}

static void
ble_sm_enc_event_rx(uint16_t conn_handle, uint8_t evt_status, int encrypted)
{
    struct ble_sm_result res;
    struct ble_sm_proc *proc;
    int authenticated;
    int bonded;
    int key_size;

    memset(&res, 0, sizeof res);

    /* Assume no change in authenticated and bonded statuses. */
    authenticated = 0;
    bonded = 0;
    key_size = 0;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, NULL);
    if (proc != NULL) {
        switch (proc->state) {
        case BLE_SM_PROC_STATE_ENC_START:
            /* We are completing a pairing procedure; keys may need to be
             * exchanged.
             */
            if (evt_status == 0) {
                /* If the responder has any keys to send, it sends them
                 * first.
                 */
                proc->state = BLE_SM_PROC_STATE_KEY_EXCH;
                if (!(proc->flags & BLE_SM_PROC_F_INITIATOR) ||
                    proc->rx_key_flags == 0) {

                    res.execute = 1;
                }

                key_size = proc->key_size;
            } else {
                /* Failure or no keys to exchange; procedure is complete. */
                proc->state = BLE_SM_PROC_STATE_NONE;
            }
            if (proc->flags & BLE_SM_PROC_F_AUTHENTICATED) {
                authenticated = 1;
            }
            break;

        case BLE_SM_PROC_STATE_ENC_RESTORE:
            /* A secure link is being restored via the encryption
             * procedure.  Keys were exchanged during pairing; they don't
             * get exchanged again now.  Procedure is complete.
             */
            BLE_HS_DBG_ASSERT(proc->rx_key_flags == 0);
            proc->state = BLE_SM_PROC_STATE_NONE;
            if (proc->flags & BLE_SM_PROC_F_AUTHENTICATED) {
                authenticated = 1;
            }
            bonded = 1;
            res.restore = 1;

            key_size = proc->key_size;
            break;

        default:
            /* The encryption change event is unexpected.  We take the
             * controller at its word that the state has changed and we
             * terminate the procedure.
             */
            proc->state = BLE_SM_PROC_STATE_NONE;
            res.sm_err = BLE_SM_ERR_UNSPECIFIED;
            break;
        }
    }

    if (evt_status == 0) {
        /* Set the encrypted state of the connection as indicated in the
         * event.
         */
        ble_sm_update_sec_state(conn_handle, encrypted, authenticated, bonded,
                                key_size);
    }

    /* Unless keys need to be exchanged, notify the application of the security
     * change.  If key exchange is pending, the application callback is
     * triggered after exchange completes.
     */
    if (proc == NULL || proc->state == BLE_SM_PROC_STATE_NONE) {
        res.enc_cb = 1;
        res.app_status = BLE_HS_HCI_ERR(evt_status);
    }

    ble_hs_unlock();

    res.bonded = bonded;
    ble_sm_process_result(conn_handle, &res);
}

void
ble_sm_enc_change_rx(const struct ble_hci_ev_enrypt_chg *ev)
{
    /* For encrypted state: read LE-encryption bit; ignore BR/EDR and reserved
     * bits.
     */
    ble_sm_enc_event_rx(le16toh(ev->connection_handle), ev->status,
                        ev->enabled & 0x01);
}

void
ble_sm_enc_key_refresh_rx(const struct ble_hci_ev_enc_key_refresh *ev)
{
    ble_sm_enc_event_rx(le16toh(ev->conn_handle), ev->status, 1);
}

/*****************************************************************************
 * $ltk                                                                      *
 *****************************************************************************/

static int
ble_sm_retrieve_ltk(uint16_t ediv, uint64_t rand, uint8_t peer_addr_type,
                    uint8_t *peer_addr, struct ble_store_value_sec *value_sec)
{
    struct ble_store_key_sec key_sec;
    int rc;

    /* Tell applicaiton to look up LTK by peer address and ediv/rand pair. */
    memset(&key_sec, 0, sizeof key_sec);
    key_sec.peer_addr.type = peer_addr_type;
    memcpy(key_sec.peer_addr.val, peer_addr, 6);
    key_sec.ediv = ediv;
    key_sec.rand_num = rand;
    key_sec.ediv_rand_present = 1;

    rc = ble_store_read_our_sec(&key_sec, value_sec);
    return rc;
}

static int
ble_sm_ltk_req_reply_tx(uint16_t conn_handle, const uint8_t *ltk)
{
    struct ble_hci_le_lt_key_req_reply_cp cmd;
    struct ble_hci_le_lt_key_req_reply_rp rsp;
    int rc;

    cmd.conn_handle = htole16(conn_handle);
    memcpy(cmd.ltk, ltk, 16);

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_LT_KEY_REQ_REPLY),
                           &cmd, sizeof(cmd), &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    if (le16toh(rsp.conn_handle) != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}

static int
ble_sm_ltk_req_neg_reply_tx(uint16_t conn_handle)
{
    struct ble_hci_le_lt_key_req_neg_reply_cp cmd;
    struct ble_hci_le_lt_key_req_neg_reply_cp rsp;
    int rc;

    cmd.conn_handle = htole16(conn_handle);
    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_LE,
                                      BLE_HCI_OCF_LE_LT_KEY_REQ_NEG_REPLY),
                           &cmd, sizeof(cmd), &rsp, sizeof(rsp));
    if (rc != 0) {
        return rc;
    }

    if (le16toh(rsp.conn_handle) != conn_handle) {
        return BLE_HS_ECONTROLLER;
    }

    return 0;
}

static void
ble_sm_ltk_start_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                      void *arg)
{
    BLE_HS_DBG_ASSERT(!(proc->flags & BLE_SM_PROC_F_INITIATOR));

    res->app_status = ble_sm_ltk_req_reply_tx(proc->conn_handle, proc->ltk);
    if (res->app_status == 0) {
        proc->state = BLE_SM_PROC_STATE_ENC_START;
    } else {
        res->enc_cb = 1;
    }
}

static void
ble_sm_ltk_restore_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                        void *arg)
{
    struct ble_store_value_sec *value_sec;

    BLE_HS_DBG_ASSERT(!(proc->flags & BLE_SM_PROC_F_INITIATOR));

    value_sec = arg;

    if (value_sec != NULL) {
        /* Store provided a key; send it to the controller. */
        res->app_status = ble_sm_ltk_req_reply_tx(
            proc->conn_handle, value_sec->ltk);

        if (res->app_status == 0) {
            proc->key_size = value_sec->key_size;
            if (value_sec->authenticated) {
                proc->flags |= BLE_SM_PROC_F_AUTHENTICATED;
            }
        } else {
            /* Notify the app if it provided a key and the procedure failed. */
            res->enc_cb = 1;
        }
    } else {
        /* Application does not have the requested key in its database.  Send a
         * negative reply to the controller.
         */
        ble_sm_ltk_req_neg_reply_tx(proc->conn_handle);
        res->app_status = BLE_HS_ENOENT;
    }

    if (res->app_status == 0) {
        proc->state = BLE_SM_PROC_STATE_ENC_RESTORE;
    }
}

int
ble_sm_ltk_req_rx(const struct ble_hci_ev_le_subev_lt_key_req *ev)
{
    struct ble_store_value_sec value_sec;
    struct ble_hs_conn_addrs addrs;
    struct ble_sm_result res;
    struct ble_sm_proc *proc;
    struct ble_hs_conn *conn;
    uint8_t peer_id_addr[6];
    int store_rc;
    int restore;

    uint16_t conn_handle = le16toh(ev->conn_handle);

    memset(&res, 0, sizeof res);

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, 0, NULL);
    if (proc == NULL) {
        /* The peer is attempting to restore a encrypted connection via the
         * encryption procedure.  Create a proc entry to indicate that security
         * establishment is in progress and execute the procedure after the
         * mutex gets unlocked.
         */
        restore = 1;
        proc = ble_sm_proc_alloc();
        if (proc == NULL) {
            res.app_status = BLE_HS_ENOMEM;
        } else {
            proc->conn_handle = conn_handle;
            proc->state = BLE_SM_PROC_STATE_LTK_RESTORE;
            ble_sm_insert(proc);

            res.execute = 1;
        }
    } else if (proc->state == BLE_SM_PROC_STATE_SEC_REQ) {
        /* Same as above, except we solicited the encryption procedure by
         * sending a security request.
         */
        restore = 1;
        proc->state = BLE_SM_PROC_STATE_LTK_RESTORE;
        res.execute = 1;
    } else if (proc->state == BLE_SM_PROC_STATE_LTK_START) {
        /* Legacy pairing just completed.  Send the short term key to the
         * controller.
         */
        restore = 0;
        res.execute = 1;
    } else {
        /* The request is unexpected; nack and forget. */
        restore = 0;
        ble_sm_ltk_req_neg_reply_tx(conn_handle);
        proc = NULL;
    }

    if (restore) {
        conn = ble_hs_conn_find_assert(conn_handle);
        ble_hs_conn_addrs(conn, &addrs);
        memcpy(peer_id_addr, addrs.peer_id_addr.val, 6);
    }

    ble_hs_unlock();

    if (proc == NULL) {
        return res.app_status;
    }

    if (res.app_status == 0) {
        if (restore) {
            store_rc = ble_sm_retrieve_ltk(le16toh(ev->div), le64toh(ev->rand),
                                            addrs.peer_id_addr.type,
                                           peer_id_addr, &value_sec);
            if (store_rc == 0) {
                /* Send the key to the controller. */
                res.state_arg = &value_sec;
            } else {
                /* Send a nack to the controller. */
                res.state_arg = NULL;
            }
        }
    }

    ble_sm_process_result(conn_handle, &res);

    return 0;
}

/*****************************************************************************
 * $random                                                                   *
 *****************************************************************************/

uint8_t *
ble_sm_our_pair_rand(struct ble_sm_proc *proc)
{
    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        return proc->randm;
    } else {
        return proc->rands;
    }
}

uint8_t *
ble_sm_peer_pair_rand(struct ble_sm_proc *proc)
{
    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        return proc->rands;
    } else {
        return proc->randm;
    }
}

static void
ble_sm_random_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                   void *arg)
{
    if (proc->flags & BLE_SM_PROC_F_SC) {
        ble_sm_sc_random_exec(proc, res);
    } else {
        ble_sm_lgcy_random_exec(proc, res);
    }
}

static void
ble_sm_random_rx(uint16_t conn_handle, struct os_mbuf **om,
                 struct ble_sm_result *res)
{
    struct ble_sm_pair_random *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_pair_random *)(*om)->om_data;

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_RANDOM, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
    } else {
        memcpy(ble_sm_peer_pair_rand(proc), cmd->value, 16);

        if (proc->flags & BLE_SM_PROC_F_SC) {
            ble_sm_sc_random_rx(proc, res);
        } else {
            ble_sm_lgcy_random_rx(proc, res);
        }
    }
    ble_hs_unlock();
}

/*****************************************************************************
 * $confirm                                                                  *
 *****************************************************************************/

static void
ble_sm_confirm_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                    void *arg)
{
    if (!(proc->flags & BLE_SM_PROC_F_SC)) {
        ble_sm_lgcy_confirm_exec(proc, res);
    } else {
        ble_sm_sc_confirm_exec(proc, res);
    }
}

static void
ble_sm_confirm_rx(uint16_t conn_handle, struct os_mbuf **om,
                  struct ble_sm_result *res)
{
    struct ble_sm_pair_confirm *cmd;
    struct ble_sm_proc *proc;
    uint8_t ioact;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_pair_confirm *)(*om)->om_data;

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_CONFIRM, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
    } else {
        memcpy(proc->confirm_peer, cmd->value, 16);

        if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
            proc->state = BLE_SM_PROC_STATE_RANDOM;
            res->execute = 1;
        } else {
            int rc;

            rc = ble_sm_io_action(proc, &ioact);
            if (rc != 0) {
                BLE_HS_DBG_ASSERT(0);
            }

            if (ble_sm_ioact_state(ioact) == proc->state) {
                proc->flags |= BLE_SM_PROC_F_ADVANCE_ON_IO;
            }
            if (ble_sm_proc_can_advance(proc)) {
                res->execute = 1;
            }
        }
    }
    ble_hs_unlock();
}

/*****************************************************************************
 * $pair                                                                     *
 *****************************************************************************/

static uint8_t
ble_sm_state_after_pair(struct ble_sm_proc *proc)
{
    if (proc->flags & BLE_SM_PROC_F_SC) {
        return BLE_SM_PROC_STATE_PUBLIC_KEY;
    } else {
        return BLE_SM_PROC_STATE_CONFIRM;
    }
}

static void
ble_sm_pair_cfg(struct ble_sm_proc *proc)
{
    struct ble_sm_pair_cmd *pair_req, *pair_rsp;
    uint8_t init_key_dist;
    uint8_t resp_key_dist;
    uint8_t rx_key_dist;
    uint8_t ioact;
    int rc;

    pair_req = (struct ble_sm_pair_cmd *) &proc->pair_req[1];
    pair_rsp = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];

    if (pair_req->authreq & BLE_SM_PAIR_AUTHREQ_SC &&
        pair_rsp->authreq & BLE_SM_PAIR_AUTHREQ_SC) {

        proc->flags |= BLE_SM_PROC_F_SC;
    }

    ble_sm_key_dist(proc, &init_key_dist, &resp_key_dist);
    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        rx_key_dist = resp_key_dist;
    } else {
        rx_key_dist = init_key_dist;
    }

    if (pair_req->authreq & BLE_SM_PAIR_AUTHREQ_BOND &&
        pair_rsp->authreq & BLE_SM_PAIR_AUTHREQ_BOND) {

        proc->flags |= BLE_SM_PROC_F_BONDING;
    }

    /* In legacy mode, bonding requires the exchange of keys
     * at least from one side.  If no key exchange was specified,
     * pretend bonding is not enabled.
     */
    if (!(proc->flags & BLE_SM_PROC_F_SC) &&
        (init_key_dist == 0 && resp_key_dist == 0)) {

        proc->flags &= ~BLE_SM_PROC_F_BONDING;
    }

    proc->rx_key_flags = 0;
    if (rx_key_dist & BLE_SM_PAIR_KEY_DIST_ENC) {
        proc->rx_key_flags |= BLE_SM_KE_F_ENC_INFO |
                              BLE_SM_KE_F_MASTER_ID;
    }
    if (rx_key_dist & BLE_SM_PAIR_KEY_DIST_ID) {
        proc->rx_key_flags |= BLE_SM_KE_F_ID_INFO |
                              BLE_SM_KE_F_ADDR_INFO;
    }
    if (rx_key_dist & BLE_SM_PAIR_KEY_DIST_SIGN) {
        proc->rx_key_flags |= BLE_SM_KE_F_SIGN_INFO;
    }

    proc->key_size = min(pair_req->max_enc_key_size,
                         pair_rsp->max_enc_key_size);

    rc = ble_sm_io_action(proc, &ioact);
    BLE_HS_DBG_ASSERT_EVAL(rc == 0);
}

static void
ble_sm_pair_base_fill(struct ble_sm_pair_cmd *cmd)
{
    cmd->io_cap = ble_hs_cfg.sm_io_cap;
    cmd->oob_data_flag = ble_hs_cfg.sm_oob_data_flag;
    cmd->authreq = ble_sm_build_authreq();
    cmd->max_enc_key_size = BLE_SM_PAIR_KEY_SZ_MAX;
}

static void
ble_sm_pair_req_fill(struct ble_sm_proc *proc)
{
    struct ble_sm_pair_cmd *req;

    req = (void *)(proc->pair_req + 1);

    proc->pair_req[0] = BLE_SM_OP_PAIR_REQ;
    ble_sm_pair_base_fill(req);
    req->init_key_dist = ble_hs_cfg.sm_our_key_dist;
    req->resp_key_dist = ble_hs_cfg.sm_their_key_dist;
}

static void
ble_sm_pair_rsp_fill(struct ble_sm_proc *proc)
{
    const struct ble_sm_pair_cmd *req;
    struct ble_sm_pair_cmd *rsp;

    req = (void *)(proc->pair_req + 1);
    rsp = (void *)(proc->pair_rsp + 1);

    proc->pair_rsp[0] = BLE_SM_OP_PAIR_RSP;
    ble_sm_pair_base_fill(rsp);

    /* The response's key distribution flags field is the intersection of
     * the peer's preferences and our capabilities.
     */
    rsp->init_key_dist = req->init_key_dist &
                         ble_hs_cfg.sm_their_key_dist;
    rsp->resp_key_dist = req->resp_key_dist &
                         ble_hs_cfg.sm_our_key_dist;
}

static void
ble_sm_pair_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                 void *arg)
{
    struct ble_sm_pair_cmd *cmd;
    struct os_mbuf *txom;
    uint8_t ioact;
    int is_req;
    int rc;

    is_req = proc->flags & BLE_SM_PROC_F_INITIATOR;

    cmd = ble_sm_cmd_get(is_req ? BLE_SM_OP_PAIR_REQ : BLE_SM_OP_PAIR_RSP,
                         sizeof(*cmd), &txom);
    if (cmd == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    if (is_req) {
        ble_sm_pair_req_fill(proc);
        memcpy(cmd, proc->pair_req + 1, sizeof(*cmd));
    } else {
        /* The response was already generated when we processed the incoming
         * request.
         */
        memcpy(cmd, proc->pair_rsp + 1, sizeof(*cmd));

        proc->state = ble_sm_state_after_pair(proc);

        rc = ble_sm_io_action(proc, &ioact);
        BLE_HS_DBG_ASSERT(rc == 0);

        if (ble_sm_ioact_state(ioact) == proc->state) {
            res->passkey_params.action = ioact;
        }
    }

    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        goto err;
    }

    res->app_status = ble_sm_gen_pair_rand(ble_sm_our_pair_rand(proc));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    return;

err:
    res->app_status = rc;

    if (!is_req) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    }
}

static bool
ble_sm_verify_auth_requirements(uint8_t authreq)
{
    /* For now we check only SC only mode. I.e.: when remote indicates
     * to not support SC pairing, let us make sure legacy pairing is supported
     * on our side. If not, we can fail right away.
     */
    if (!(authreq & BLE_SM_PAIR_AUTHREQ_SC)) {
        if (MYNEWT_VAL(BLE_SM_LEGACY) == 0) {
            return false;
        }
    }
    return true;
}

static void
ble_sm_pair_req_rx(uint16_t conn_handle, struct os_mbuf **om,
                   struct ble_sm_result *res)
{
    struct ble_sm_pair_cmd *req;
    struct ble_sm_proc *proc;
    struct ble_sm_proc *prev;
    struct ble_hs_conn *conn;
    ble_sm_proc_flags proc_flags;
    uint8_t key_size;
    int rc;

    /* Silence spurious unused-variable warnings. */
    proc_flags = 0;
    key_size = 0;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*req));
    if (res->app_status != 0) {
        return;
    }

    req = (struct ble_sm_pair_cmd *)(*om)->om_data;

    ble_hs_lock();

    /* XXX: Check connection state; reject if not appropriate. */
    /* XXX: Ensure enough time has passed since the previous failed pairing
     * attempt.
     */
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, &prev);
    if (proc != NULL) {
        /* Fail if procedure is in progress unless we sent a slave security
         * request to peer.
         */
        if (proc->state != BLE_SM_PROC_STATE_SEC_REQ) {
            res->sm_err = BLE_SM_ERR_UNSPECIFIED;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_UNSPECIFIED);
            ble_hs_unlock();
            return;
        }

        /* Remove the procedure because it was allocated when
         * sending the Slave Security Request and it will be allocated
         * again later in this method. We should probably refactor this
         * in the future.
         */
        ble_sm_proc_remove(proc, prev);
        ble_sm_proc_free(proc);
    }

    ble_hs_unlock();

    /* Check if there is storage capacity for a new bond.  If there isn't, ask
     * the application to make room.
     */
    rc = ble_sm_chk_store_overflow(conn_handle);
    if (rc != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->app_status = rc;
        return;
    }

    ble_hs_lock();

    proc = ble_sm_proc_alloc();
    if (proc != NULL) {
        proc->conn_handle = conn_handle;
        proc->state = BLE_SM_PROC_STATE_PAIR;
        ble_sm_insert(proc);

        proc->pair_req[0] = BLE_SM_OP_PAIR_REQ;
        memcpy(proc->pair_req + 1, req, sizeof(*req));

        conn = ble_hs_conn_find_assert(proc->conn_handle);
        if (conn->bhc_flags & BLE_HS_CONN_F_MASTER) {
            res->sm_err = BLE_SM_ERR_CMD_NOT_SUPP;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_CMD_NOT_SUPP);
        } else if (req->max_enc_key_size < BLE_SM_PAIR_KEY_SZ_MIN) {
            res->sm_err = BLE_SM_ERR_ENC_KEY_SZ;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_ENC_KEY_SZ);
        } else if (req->max_enc_key_size > BLE_SM_PAIR_KEY_SZ_MAX) {
            res->sm_err = BLE_SM_ERR_INVAL;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_INVAL);
        } else if (!ble_sm_verify_auth_requirements(req->authreq)) {
            res->sm_err = BLE_SM_ERR_AUTHREQ;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_AUTHREQ);
        } else {
            /* The request looks good.  Precalculate our pairing response and
             * determine some properties of the imminent link.  We need this
             * information in case this is a repeated pairing attempt (i.e., we
             * are already bonded to this peer).  In that case, we include the
             * information in a notification to the app.
             */
            ble_sm_pair_rsp_fill(proc);
            ble_sm_pair_cfg(proc);

            proc_flags = proc->flags;
            key_size = proc->key_size;
            res->execute = 1;
        }
    }

    ble_hs_unlock();

    /* Check if we are already bonded to this peer.  If so, give the
     * application an opportunity to delete the old bond.
     */
    if (res->app_status == 0) {
        rc = ble_sm_chk_repeat_pairing(conn_handle, proc_flags, key_size);
        if (rc != 0) {
            /* The app indicated that the pairing request should be ignored. */
            res->app_status = rc;
            res->execute = 0;
        }
    }
}

static void
ble_sm_pair_rsp_rx(uint16_t conn_handle, struct os_mbuf **om,
                   struct ble_sm_result *res)
{
    struct ble_sm_pair_cmd *rsp;
    struct ble_sm_proc *proc;
    uint8_t ioact;
    int rc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*rsp));
    if (res->app_status != 0) {
        res->enc_cb = 1;
        return;
    }

    rsp = (struct ble_sm_pair_cmd *)(*om)->om_data;

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_PAIR, 1, NULL);
    if (proc != NULL) {
        proc->pair_rsp[0] = BLE_SM_OP_PAIR_RSP;
        memcpy(proc->pair_rsp + 1, rsp, sizeof(*rsp));

        if (rsp->max_enc_key_size < BLE_SM_PAIR_KEY_SZ_MIN) {
            res->sm_err = BLE_SM_ERR_ENC_KEY_SZ;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_ENC_KEY_SZ);
        } else if (rsp->max_enc_key_size > BLE_SM_PAIR_KEY_SZ_MAX) {
            res->sm_err = BLE_SM_ERR_INVAL;
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_INVAL);
        } else {
            ble_sm_pair_cfg(proc);

            rc = ble_sm_io_action(proc, &ioact);
            if (rc != 0) {
                res->sm_err = BLE_SM_ERR_AUTHREQ;
                res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_AUTHREQ);
                res->enc_cb = 1;
            } else {
                proc->state = ble_sm_state_after_pair(proc);
                if (ble_sm_ioact_state(ioact) == proc->state) {
                    res->passkey_params.action = ioact;
                }
                if (ble_sm_proc_can_advance(proc)) {
                    res->execute = 1;
                }
            }
        }
    }

    ble_hs_unlock();
}

/*****************************************************************************
 * $security request                                                         *
 *****************************************************************************/

static void
ble_sm_sec_req_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                    void *arg)
{
    struct ble_sm_sec_req *cmd;
    struct os_mbuf *txom;
    int rc;

    cmd = ble_sm_cmd_get(BLE_SM_OP_SEC_REQ, sizeof(*cmd), &txom);
    if (!cmd) {
        res->app_status = BLE_HS_ENOMEM;
        return;
    }

    cmd->authreq = ble_sm_build_authreq();
    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        res->app_status = rc;
        return;
    }
}

static void
ble_sm_sec_req_rx(uint16_t conn_handle, struct os_mbuf **om,
                  struct ble_sm_result *res)
{
    struct ble_store_value_sec value_sec;
    struct ble_store_key_sec key_sec;
    struct ble_hs_conn_addrs addrs;
    struct ble_sm_sec_req *cmd;
    struct ble_hs_conn *conn;
    int authreq_mitm;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        return;
    }

    cmd = (struct ble_sm_sec_req *)(*om)->om_data;

    /* XXX: Reject if:
     *     o authreq-reserved flags set?
     */

    ble_hs_lock();

    conn = ble_hs_conn_find_assert(conn_handle);
    if (!(conn->bhc_flags & BLE_HS_CONN_F_MASTER)) {
        res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_CMD_NOT_SUPP);
        res->sm_err = BLE_SM_ERR_CMD_NOT_SUPP;
    } else {
        /* We will be querying the SM database for a key corresponding to the
         * sender; remember the sender's address while the connection list is
         * locked.
         */
        ble_hs_conn_addrs(conn, &addrs);
        memset(&key_sec, 0, sizeof key_sec);
        key_sec.peer_addr = addrs.peer_id_addr;
    }

    ble_hs_unlock();

    if (res->app_status == 0) {
        /* If the peer is requesting a bonded connection, query database for an
         * LTK corresponding to the sender.
         */
        if (cmd->authreq & BLE_SM_PAIR_AUTHREQ_BOND) {
            res->app_status = ble_store_read_peer_sec(&key_sec, &value_sec);
        } else {
            res->app_status = BLE_HS_ENOENT;
        }
        if (res->app_status == 0) {
            /* Found a key corresponding to this peer.  Make sure it meets the
             * requested minimum authreq.
             */
            authreq_mitm = cmd->authreq & BLE_SM_PAIR_AUTHREQ_MITM;
            if (authreq_mitm && !value_sec.authenticated) {
                res->app_status = BLE_HS_EREJECT;
            }
        }

        if (res->app_status == 0) {
            res->app_status = ble_sm_enc_initiate(conn_handle,
                                                  value_sec.key_size,
                                                  value_sec.ltk,
                                                  value_sec.ediv,
                                                  value_sec.rand_num,
                                                  value_sec.authenticated);
        } else {
            res->app_status = ble_sm_pair_initiate(conn_handle);
        }
    }
}

/*****************************************************************************
 * $key exchange                                                             *
 *****************************************************************************/

static void
ble_sm_key_exch_success(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    /* The procedure is now complete.  Update connection bonded state and
     * terminate procedure.
     */
    ble_sm_update_sec_state(proc->conn_handle, 1,
                            !!(proc->flags & BLE_SM_PROC_F_AUTHENTICATED),
                            !!(proc->flags & BLE_SM_PROC_F_BONDING),
                            proc->key_size);
    proc->state = BLE_SM_PROC_STATE_NONE;

    res->app_status = 0;
    res->enc_cb = 1;
}

static void
ble_sm_key_exch_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                     void *arg)
{
    struct ble_sm_id_addr_info *addr_info;
    struct ble_hs_conn_addrs addrs;
    struct ble_sm_sign_info *sign_info;
    struct ble_sm_master_id *master_id;
    struct ble_sm_enc_info *enc_info;
    struct ble_sm_id_info *id_info;
    struct ble_hs_conn *conn;
    uint8_t init_key_dist;
    uint8_t resp_key_dist;
    uint8_t our_key_dist;
    struct os_mbuf *txom;
    const uint8_t *irk;
    int rc;

    ble_sm_key_dist(proc, &init_key_dist, &resp_key_dist);
    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        our_key_dist = init_key_dist;
    } else {
        our_key_dist = resp_key_dist;
    }

    if (our_key_dist & BLE_SM_PAIR_KEY_DIST_ENC) {
        /* Send encryption information. */
        enc_info = ble_sm_cmd_get(BLE_SM_OP_ENC_INFO, sizeof(*enc_info), &txom);
        if (!enc_info) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        rc = ble_sm_gen_ltk(proc, enc_info->ltk);
        if (rc != 0) {
            os_mbuf_free_chain(txom);
            goto err;
        }

        /* store LTK before sending since ble_sm_tx consumes tx mbuf */
        memcpy(proc->our_keys.ltk, enc_info->ltk, 16);
        proc->our_keys.key_size = proc->key_size;
        proc->our_keys.ltk_valid = 1;

        rc = ble_sm_tx(proc->conn_handle, txom);
        if (rc != 0) {
            goto err;
        }

        /* Send master identification. */
        master_id = ble_sm_cmd_get(BLE_SM_OP_MASTER_ID, sizeof(*master_id),
                                   &txom);
        if (!master_id) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        rc = ble_sm_gen_ediv(master_id);
        if (rc != 0) {
            os_mbuf_free_chain(txom);
            goto err;
        }
        rc = ble_sm_gen_master_id_rand(master_id);
        if (rc != 0) {
            os_mbuf_free_chain(txom);
            goto err;
        }

        proc->our_keys.ediv_rand_valid = 1;
        proc->our_keys.rand_val = master_id->rand_val;
        proc->our_keys.ediv = master_id->ediv;

        rc = ble_sm_tx(proc->conn_handle, txom);
        if (rc != 0) {
            goto err;
        }
    }

    if (our_key_dist & BLE_SM_PAIR_KEY_DIST_ID) {
        /* Send identity information. */
        id_info = ble_sm_cmd_get(BLE_SM_OP_IDENTITY_INFO, sizeof(*id_info),
                                 &txom);
        if (!id_info) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        rc = ble_hs_pvcy_our_irk(&irk);
        if (rc != 0) {
            os_mbuf_free_chain(txom);
            goto err;
        }

        memcpy(id_info->irk, irk, 16);
        proc->our_keys.irk_valid = 1;

        rc = ble_sm_tx(proc->conn_handle, txom);
        if (rc != 0) {
            goto err;
        }

        /* Send identity address information. */
        addr_info = ble_sm_cmd_get(BLE_SM_OP_IDENTITY_ADDR_INFO,
                                   sizeof(*addr_info), &txom);
        if (!addr_info) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        conn = ble_hs_conn_find_assert(proc->conn_handle);
        ble_hs_conn_addrs(conn, &addrs);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
        /* Check if the privacy is enabled, change the address accordingly
         * Overriding the addrs field to get the ID address remain unique
         * (public address) */
        if (ble_host_rpa_enabled())
        {
            uint8_t *local_id_rpa = NULL;

            ble_hs_id_addr(BLE_ADDR_PUBLIC, (void *) &local_id_rpa, NULL);
            memcpy(addrs.our_id_addr.val, local_id_rpa, 6);
            addrs.our_id_addr.type = BLE_ADDR_PUBLIC;
        }
#endif
        addr_info->addr_type = addrs.our_id_addr.type;
        memcpy(addr_info->bd_addr, addrs.our_id_addr.val, 6);

        proc->our_keys.addr_valid = 1;
        memcpy(proc->our_keys.irk, irk, 16);
        proc->our_keys.addr_type = addr_info->addr_type;
        memcpy(proc->our_keys.addr, addr_info->bd_addr, 6);

        rc = ble_sm_tx(proc->conn_handle, txom);
        if (rc != 0) {
            goto err;
        }
    }

    if (our_key_dist & BLE_SM_PAIR_KEY_DIST_SIGN) {
        /* Send signing information. */
        sign_info = ble_sm_cmd_get(BLE_SM_OP_SIGN_INFO, sizeof(*sign_info),
                                   &txom);
        if (!sign_info) {
            rc = BLE_HS_ENOMEM;
            goto err;
        }

        rc = ble_sm_gen_csrk(proc, sign_info->sig_key);
        if (rc != 0) {
            os_mbuf_free_chain(txom);
            goto err;
        }

        proc->our_keys.csrk_valid = 1;
        memcpy(proc->our_keys.csrk, sign_info->sig_key, 16);

        rc = ble_sm_tx(proc->conn_handle, txom);
        if (rc != 0) {
            goto err;
        }
    }

    if (proc->flags & BLE_SM_PROC_F_INITIATOR || proc->rx_key_flags == 0) {
        /* The procedure is now complete. */
        ble_sm_key_exch_success(proc, res);
    }

    return;

err:
    res->app_status = rc;
    res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    res->enc_cb = 1;
}

static void
ble_sm_key_rxed(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    BLE_HS_LOG(DEBUG, "rx_key_flags=0x%02x\n", proc->rx_key_flags);

    if (proc->rx_key_flags == 0) {
        /* The peer is done sending keys.  If we are the initiator, we need to
         * send ours.  If we are the responder, the procedure is complete.
         */
        if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
            res->execute = 1;
        } else {
            ble_sm_key_exch_success(proc, res);
        }
    }
}

static void
ble_sm_enc_info_rx(uint16_t conn_handle, struct os_mbuf **om,
                   struct ble_sm_result *res)
{
    struct ble_sm_enc_info *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_enc_info *)(*om)->om_data;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_KEY_EXCH, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        proc->rx_key_flags &= ~BLE_SM_KE_F_ENC_INFO;
        proc->peer_keys.ltk_valid = 1;
        memcpy(proc->peer_keys.ltk, cmd->ltk, 16);
        proc->peer_keys.key_size = proc->key_size;

        ble_sm_key_rxed(proc, res);
    }

    ble_hs_unlock();
}

static void
ble_sm_master_id_rx(uint16_t conn_handle, struct os_mbuf **om,
                    struct ble_sm_result *res)
{
    struct ble_sm_master_id *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_master_id *)(*om)->om_data;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_KEY_EXCH, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        proc->rx_key_flags &= ~BLE_SM_KE_F_MASTER_ID;
        proc->peer_keys.ediv_rand_valid = 1;

        proc->peer_keys.ediv = le16toh(cmd->ediv);
        proc->peer_keys.rand_val = le64toh(cmd->rand_val);

        ble_sm_key_rxed(proc, res);
    }

    ble_hs_unlock();
}

static void
ble_sm_id_info_rx(uint16_t conn_handle, struct os_mbuf **om,
                  struct ble_sm_result *res)
{
    struct ble_sm_id_info *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_id_info *)(*om)->om_data;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_KEY_EXCH, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        proc->rx_key_flags &= ~BLE_SM_KE_F_ID_INFO;

        memcpy(proc->peer_keys.irk, cmd->irk, 16);
        proc->peer_keys.irk_valid = 1;

        ble_sm_key_rxed(proc, res);
    }

    ble_hs_unlock();
}

static void
ble_sm_id_addr_info_rx(uint16_t conn_handle, struct os_mbuf **om,
                       struct ble_sm_result *res)
{
    struct ble_sm_id_addr_info *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_id_addr_info *)(*om)->om_data;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_KEY_EXCH, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        proc->rx_key_flags &= ~BLE_SM_KE_F_ADDR_INFO;
        proc->peer_keys.addr_valid = 1;
        proc->peer_keys.addr_type = cmd->addr_type;
        memcpy(proc->peer_keys.addr, cmd->bd_addr, 6);

        ble_sm_key_rxed(proc, res);
    }

    ble_hs_unlock();
}

static void
ble_sm_sign_info_rx(uint16_t conn_handle, struct os_mbuf **om,
                    struct ble_sm_result *res)
{
    struct ble_sm_sign_info *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    cmd = (struct ble_sm_sign_info *)(*om)->om_data;

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_KEY_EXCH, -1, NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        proc->rx_key_flags &= ~BLE_SM_KE_F_SIGN_INFO;

        memcpy(proc->peer_keys.csrk, cmd->sig_key, 16);
        proc->peer_keys.csrk_valid = 1;

        ble_sm_key_rxed(proc, res);
    }

    ble_hs_unlock();
}

/*****************************************************************************
 * $fail                                                                     *
 *****************************************************************************/

static void
ble_sm_fail_rx(uint16_t conn_handle, struct os_mbuf **om,
               struct ble_sm_result *res)
{
    struct ble_sm_pair_fail *cmd;

    res->enc_cb = 1;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status == 0) {
        cmd = (struct ble_sm_pair_fail *)(*om)->om_data;

        res->app_status = BLE_HS_SM_PEER_ERR(cmd->reason);
    }
}

/*****************************************************************************
 * $api                                                                      *
 *****************************************************************************/

/**
 * Times out expired SM procedures.
 *
 * @return                      The number of ticks until this function should
 *                                  be called again.
 */
int32_t
ble_sm_timer(void)
{
    struct ble_sm_proc_list exp_list;
    struct ble_sm_proc *proc;
    int32_t ticks_until_exp;

    /* Remove timed-out procedures from the main list and insert them into a
     * temporary list.  This function also calculates the number of ticks until
     * the next expiration will occur.
     */
    ticks_until_exp = ble_sm_extract_expired(&exp_list);

    /* Notify application of each failure and free the corresponding procedure
     * object.
     * XXX: Mark connection as tainted; don't allow any subsequent SMP
     * procedures without reconnect.
     */
    while ((proc = STAILQ_FIRST(&exp_list)) != NULL) {
        ble_gap_enc_event(proc->conn_handle, BLE_HS_ETIMEOUT, 0, 0);

        STAILQ_REMOVE_HEAD(&exp_list, next);
        ble_sm_proc_free(proc);
    }

    return ticks_until_exp;
}

/**
 * Initiates the pairing procedure for the specified connection.
 */
int
ble_sm_pair_initiate(uint16_t conn_handle)
{
    struct ble_sm_result res;
    struct ble_sm_proc *proc;
    int rc;

    memset(&res, 0, sizeof(res));

    /* Make sure a procedure isn't already in progress for this connection. */
    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, NULL);
    ble_hs_unlock();

    if (proc != NULL) {
        res.app_status = BLE_HS_EALREADY;
        return BLE_HS_EALREADY;
    }

    /* Check if there is storage capacity for a new bond.  If there isn't, ask
     * the application to make room.
     */
    rc = ble_sm_chk_store_overflow(conn_handle);
    if (rc != 0) {
        return rc;
    }

    proc = ble_sm_proc_alloc();
    if (proc == NULL) {
        res.app_status = BLE_HS_ENOMEM;
    } else {
        proc->conn_handle = conn_handle;
        proc->state = BLE_SM_PROC_STATE_PAIR;
        proc->flags |= BLE_SM_PROC_F_INITIATOR;

        ble_hs_lock();
        ble_sm_insert(proc);
        ble_hs_unlock();

        res.execute = 1;
    }

    if (proc != NULL) {
        ble_sm_process_result(conn_handle, &res);
    }

    return res.app_status;
}

int
ble_sm_slave_initiate(uint16_t conn_handle)
{
    struct ble_sm_result res;
    struct ble_sm_proc *proc;

    memset(&res, 0, sizeof(res));

    ble_hs_lock();

    /* Make sure a procedure isn't already in progress for this connection. */
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, NULL);
    if (proc != NULL) {
        res.app_status = BLE_HS_EALREADY;

        /* Set pointer to null so that existing entry doesn't get freed. */
        proc = NULL;
    } else {
        proc = ble_sm_proc_alloc();
        if (proc == NULL) {
            res.app_status = BLE_HS_ENOMEM;
        } else {
            proc->conn_handle = conn_handle;
            proc->state = BLE_SM_PROC_STATE_SEC_REQ;
            ble_sm_insert(proc);

            res.execute = 1;
        }
    }

    ble_hs_unlock();

    if (proc != NULL) {
        ble_sm_process_result(conn_handle, &res);
    }

    return res.app_status;
}

/**
 * Initiates the encryption procedure for the specified connection.
 */
int
ble_sm_enc_initiate(uint16_t conn_handle, uint8_t key_size,
                    const uint8_t *ltk, uint16_t ediv,
                    uint64_t rand_val, int auth)
{
    struct ble_sm_result res;
    struct ble_sm_proc *proc;
    struct hci_start_encrypt cmd;

    memset(&res, 0, sizeof res);

    /* Make sure a procedure isn't already in progress for this connection. */
    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, NULL);
    if (proc != NULL) {
        res.app_status = BLE_HS_EALREADY;

        /* Set pointer to null so that existing entry doesn't get freed. */
        proc = NULL;
    } else {
        proc = ble_sm_proc_alloc();
        if (proc == NULL) {
            res.app_status = BLE_HS_ENOMEM;
        } else {
            proc->conn_handle = conn_handle;
            proc->key_size = key_size;
            proc->state = BLE_SM_PROC_STATE_ENC_RESTORE;
            proc->flags |= BLE_SM_PROC_F_INITIATOR;
            if (auth) {
                proc->flags |= BLE_SM_PROC_F_AUTHENTICATED;
            }
            ble_sm_insert(proc);

            cmd.connection_handle = conn_handle;
            cmd.encrypted_diversifier = ediv;
            cmd.random_number = rand_val;
            memcpy(cmd.long_term_key, ltk, sizeof cmd.long_term_key);

            res.execute = 1;
            res.state_arg = &cmd;
        }
    }

    ble_hs_unlock();

    ble_sm_process_result(conn_handle, &res);

    return res.app_status;
}

static int
ble_sm_rx(struct ble_l2cap_chan *chan)
{
    struct ble_sm_result res;
    ble_sm_rx_fn *rx_cb;
    uint8_t op;
    uint16_t conn_handle;
    struct os_mbuf **om;
    int rc;

    STATS_INC(ble_l2cap_stats, sm_rx);

    conn_handle = ble_l2cap_get_conn_handle(chan);
    if (conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return BLE_HS_ENOTCONN;
    }

    om = &chan->rx_buf;
    BLE_HS_DBG_ASSERT(*om != NULL);

    rc = os_mbuf_copydata(*om, 0, 1, &op);
    if (rc != 0) {
        return BLE_HS_EBADDATA;
    }

    /* Strip L2CAP SM header from the front of the mbuf. */
    os_mbuf_adj(*om, 1);

    rx_cb = ble_sm_dispatch_get(op);
    if (rx_cb != NULL) {
        memset(&res, 0, sizeof res);

        rx_cb(conn_handle, om, &res);
        ble_sm_process_result(conn_handle, &res);
        rc = res.app_status;
    } else {
        rc = BLE_HS_ENOTSUP;
    }

    return rc;
}

int
ble_sm_inject_io(uint16_t conn_handle, struct ble_sm_io *pkey)
{
    struct ble_sm_result res;
    struct ble_sm_proc *proc;
    int rc;
    uint8_t action;

    memset(&res, 0, sizeof res);

    ble_hs_lock();

    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_NONE, -1, NULL);
    if (proc == NULL) {
        rc = BLE_HS_ENOENT;
    } else if (proc->flags & BLE_SM_PROC_F_IO_INJECTED) {
        rc = BLE_HS_EALREADY;
    } else if ((ble_sm_io_action(proc, &action) == 0) && pkey->action != action) {
        /* Application provided incorrect IO type. */
        rc = BLE_HS_EINVAL;
    } else if (ble_sm_ioact_state(pkey->action) != proc->state) {
        /* Procedure is not ready for user input. */
        rc = BLE_HS_EINVAL;
    } else {
        /* Assume valid input. */
        rc = 0;

        switch (pkey->action) {
        case BLE_SM_IOACT_OOB:
            proc->flags |= BLE_SM_PROC_F_IO_INJECTED;
            memcpy(proc->tk, pkey->oob, 16);
            if ((proc->flags & BLE_SM_PROC_F_INITIATOR) ||
                (proc->flags & BLE_SM_PROC_F_ADVANCE_ON_IO)) {

                res.execute = 1;
            }
            break;

        case BLE_SM_IOACT_INPUT:
        case BLE_SM_IOACT_DISP:
            if (pkey->passkey > 999999) {
                rc = BLE_HS_EINVAL;
            } else {
                proc->flags |= BLE_SM_PROC_F_IO_INJECTED;
                memset(proc->tk, 0, 16);
                proc->tk[0] = (pkey->passkey >> 0) & 0xff;
                proc->tk[1] = (pkey->passkey >> 8) & 0xff;
                proc->tk[2] = (pkey->passkey >> 16) & 0xff;
                proc->tk[3] = (pkey->passkey >> 24) & 0xff;
                if ((proc->flags & BLE_SM_PROC_F_INITIATOR) ||
                    (proc->flags & BLE_SM_PROC_F_ADVANCE_ON_IO)) {

                    res.execute = 1;
                }
            }
            break;

        case BLE_SM_IOACT_NUMCMP:
            if (!pkey->numcmp_accept) {
                res.app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_NUMCMP);
                res.sm_err = BLE_SM_ERR_NUMCMP;
            } else {
                proc->flags |= BLE_SM_PROC_F_IO_INJECTED;
                if (proc->flags & BLE_SM_PROC_F_INITIATOR ||
                    proc->flags & BLE_SM_PROC_F_ADVANCE_ON_IO) {

                    res.execute = 1;
                }
            }
            break;

#if MYNEWT_VAL(BLE_SM_SC)
        case BLE_SM_IOACT_OOB_SC:
            if (!ble_sm_sc_oob_data_check(proc,
                                          (pkey->oob_sc_data.local != NULL),
                                          (pkey->oob_sc_data.remote != NULL))) {
                res.app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_OOB);
                res.sm_err = BLE_SM_ERR_OOB;
            } else {
                proc->flags |= BLE_SM_PROC_F_IO_INJECTED;
                proc->oob_data_local = pkey->oob_sc_data.local;
                proc->oob_data_remote = pkey->oob_sc_data.remote;

                /* Execute Confirm step */
                ble_sm_sc_oob_confirm(proc, &res);
            }
            break;
#endif

        default:
            BLE_HS_DBG_ASSERT(0);
            rc = BLE_HS_EINVAL;
            break;
        }
    }

    ble_hs_unlock();

    /* If application provided invalid input, return error without modifying
     * SMP state.
     */
    if (rc != 0) {
        return rc;
    }

    ble_sm_process_result(conn_handle, &res);
    return res.app_status;
}

void
ble_sm_connection_broken(uint16_t conn_handle)
{
    struct ble_sm_result res;

    memset(&res, 0, sizeof res);
    res.app_status = BLE_HS_ENOTCONN;
    res.enc_cb = 1;

    ble_sm_process_result(conn_handle, &res);
}

int
ble_sm_init(void)
{
    int rc;

    STAILQ_INIT(&ble_sm_procs);

    rc = os_mempool_init(&ble_sm_proc_pool,
                         MYNEWT_VAL(BLE_SM_MAX_PROCS),
                         sizeof (struct ble_sm_proc),
                         ble_sm_proc_mem,
                         "ble_sm_proc_pool");
    if (rc != 0) {
        return rc;
    }

    ble_sm_sc_init();

    return 0;
}
#else
/* if pairing is not supported it is only needed to reply with Pairing
 * Failed with 'Pairing not Supported' reason so this function can be very
 * simple
 */
static int
ble_sm_rx(struct ble_l2cap_chan *chan)
{
    struct ble_sm_pair_fail *cmd;
    struct os_mbuf *txom;
    uint16_t handle;
    int rc;

    handle = ble_l2cap_get_conn_handle(chan);
    if (!handle) {
        return BLE_HS_ENOTCONN;
    }

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_FAIL, sizeof(*cmd), &txom);
    if (cmd == NULL) {
        return BLE_HS_ENOMEM;
    }

    cmd->reason = BLE_SM_ERR_PAIR_NOT_SUPP;

    ble_hs_lock();
    rc = ble_sm_tx(handle, txom);
    ble_hs_unlock();

    return rc;
}
#endif

struct ble_l2cap_chan *
ble_sm_create_chan(uint16_t conn_handle)
{
    struct ble_l2cap_chan *chan;

    chan = ble_l2cap_chan_alloc(conn_handle);
    if (chan == NULL) {
        return NULL;
    }

    chan->scid = BLE_L2CAP_CID_SM;
    chan->dcid = BLE_L2CAP_CID_SM;
    chan->my_mtu = BLE_SM_MTU;
    chan->rx_fn = ble_sm_rx;

    return chan;
}
