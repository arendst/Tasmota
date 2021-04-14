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

#include "nimble/nimble_opt.h"
#include "host/ble_sm.h"
#include "ble_hs_priv.h"
#include "ble_sm_priv.h"

#if MYNEWT_VAL(BLE_SM_SC)

#define BLE_SM_SC_PASSKEY_BYTES     4
#define BLE_SM_SC_PASSKEY_BITS      20

static uint8_t ble_sm_sc_pub_key[64];
static uint8_t ble_sm_sc_priv_key[32];

/**
 * Whether our public-private key pair has been generated.  We generate it on
 * startup for now until we have a non-volatile storage mechanism.
 */
static uint8_t ble_sm_sc_keys_generated;

/**
 * Create some shortened names for the passkey actions so that the table is
 * easier to read.
 */
#define IOACT_NONE      BLE_SM_IOACT_NONE
#define IOACT_OOB       BLE_SM_IOACT_OOB
#define IOACT_INPUT     BLE_SM_IOACT_INPUT
#define IOACT_DISP      BLE_SM_IOACT_DISP
#define IOACT_NUMCMP    BLE_SM_IOACT_NUMCMP

/**
 * This table expresses the required initiator IO action.  Inputs are:
 *     o Responder IO capabilities (from pair response).
 *     o Initiator IO capabilities (from pair request).
 */
static const uint8_t ble_sm_sc_init_ioa[5 /*resp*/ ][5 /*init*/ ] =
{
      /* init */
/*r*/ {IOACT_NONE,    IOACT_NONE,   IOACT_INPUT, IOACT_NONE, IOACT_INPUT},
/*e*/ {IOACT_NONE,    IOACT_NUMCMP, IOACT_INPUT, IOACT_NONE, IOACT_NUMCMP},
/*s*/ {IOACT_DISP,    IOACT_DISP,   IOACT_INPUT, IOACT_NONE, IOACT_DISP},
/*p*/ {IOACT_NONE,    IOACT_NONE,   IOACT_NONE,  IOACT_NONE, IOACT_NONE},
      {IOACT_DISP,    IOACT_NUMCMP, IOACT_INPUT, IOACT_NONE, IOACT_NUMCMP},
};

/**
 * This table expresses the required responder IO action.  Inputs are:
 *     o Responder IO capabilities (from pair response).
 *     o Initiator IO capabilities (from pair request).
 */
static const uint8_t ble_sm_sc_resp_ioa[5 /*resp*/ ][5 /*init*/ ] =
{
      /* init */
/*r*/ {IOACT_NONE,    IOACT_NONE,   IOACT_DISP,  IOACT_NONE, IOACT_DISP},
/*e*/ {IOACT_NONE,    IOACT_NUMCMP, IOACT_DISP,  IOACT_NONE, IOACT_NUMCMP},
/*s*/ {IOACT_INPUT,   IOACT_INPUT,  IOACT_INPUT, IOACT_NONE, IOACT_INPUT},
/*p*/ {IOACT_NONE,    IOACT_NONE,   IOACT_NONE,  IOACT_NONE, IOACT_NONE},
      {IOACT_INPUT,   IOACT_NUMCMP, IOACT_DISP,  IOACT_NONE, IOACT_NUMCMP},
};

#if MYNEWT_VAL(BLE_HS_DEBUG)

static uint8_t ble_sm_dbg_sc_pub_key[64];
static uint8_t ble_sm_dbg_sc_priv_key[32];
static uint8_t ble_sm_dbg_sc_keys_set;

void
ble_sm_dbg_set_sc_keys(uint8_t *pubkey, uint8_t *privkey)
{
    memcpy(ble_sm_dbg_sc_pub_key, pubkey,
           sizeof ble_sm_dbg_sc_pub_key);
    memcpy(ble_sm_dbg_sc_priv_key, privkey,
           sizeof ble_sm_dbg_sc_priv_key);
    ble_sm_dbg_sc_keys_set = 1;
}

#endif

int
ble_sm_sc_io_action(struct ble_sm_proc *proc, uint8_t *action)
{
    struct ble_sm_pair_cmd *pair_req, *pair_rsp;

    pair_req = (struct ble_sm_pair_cmd *) &proc->pair_req[1];
    pair_rsp = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];

    if (pair_req->oob_data_flag == BLE_SM_PAIR_OOB_YES ||
        pair_rsp->oob_data_flag == BLE_SM_PAIR_OOB_YES) {
        *action = BLE_SM_IOACT_OOB;
    } else if (!(pair_req->authreq & BLE_SM_PAIR_AUTHREQ_MITM) &&
               !(pair_rsp->authreq & BLE_SM_PAIR_AUTHREQ_MITM)) {

        *action = BLE_SM_IOACT_NONE;
    } else if (pair_req->io_cap >= BLE_SM_IO_CAP_RESERVED ||
               pair_rsp->io_cap >= BLE_SM_IO_CAP_RESERVED) {
        *action = BLE_SM_IOACT_NONE;
    } else if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        *action = ble_sm_sc_init_ioa[pair_rsp->io_cap][pair_req->io_cap];
    } else {
        *action = ble_sm_sc_resp_ioa[pair_rsp->io_cap][pair_req->io_cap];
    }

    switch (*action) {
    case BLE_SM_IOACT_NONE:
        proc->pair_alg = BLE_SM_PAIR_ALG_JW;
        break;

    case BLE_SM_IOACT_OOB:
        proc->pair_alg = BLE_SM_PAIR_ALG_OOB;
        proc->flags |= BLE_SM_PROC_F_AUTHENTICATED;
        break;

    case BLE_SM_IOACT_INPUT:
    case BLE_SM_IOACT_DISP:
        proc->pair_alg = BLE_SM_PAIR_ALG_PASSKEY;
        proc->flags |= BLE_SM_PROC_F_AUTHENTICATED;
        break;

    case BLE_SM_IOACT_NUMCMP:
        proc->pair_alg = BLE_SM_PAIR_ALG_NUMCMP;
        proc->flags |= BLE_SM_PROC_F_AUTHENTICATED;
        break;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_HS_EINVAL;
    }

    return 0;
}

static int
ble_sm_gen_pub_priv(uint8_t *pub, uint8_t *priv)
{
    int rc;

#if MYNEWT_VAL(BLE_HS_DEBUG)
    if (ble_sm_dbg_sc_keys_set) {
        ble_sm_dbg_sc_keys_set = 0;
        memcpy(pub, ble_sm_dbg_sc_pub_key, sizeof ble_sm_dbg_sc_pub_key);
        memcpy(priv, ble_sm_dbg_sc_priv_key, sizeof ble_sm_dbg_sc_priv_key);
        return 0;
    }
#endif

    rc = ble_sm_alg_gen_key_pair(pub, priv);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

static int
ble_sm_sc_ensure_keys_generated(void)
{
    int rc;

    if (!ble_sm_sc_keys_generated) {
        rc = ble_sm_gen_pub_priv(ble_sm_sc_pub_key, ble_sm_sc_priv_key);
        if (rc != 0) {
            return rc;
        }

        ble_sm_sc_keys_generated = 1;
    }

    BLE_HS_LOG(DEBUG, "our pubkey=");
    ble_hs_log_flat_buf(&ble_sm_sc_pub_key, 64);
    BLE_HS_LOG(DEBUG, "\n");
    BLE_HS_LOG(DEBUG, "our privkey=");
    ble_hs_log_flat_buf(&ble_sm_sc_priv_key, 32);
    BLE_HS_LOG(DEBUG, "\n");

    return 0;
}

/* Initiator does not send a confirm when pairing algorithm is any of:
 *     o just works
 *     o numeric comparison
 * (vol. 3, part H, 2.3.5.6.2)
 */
static int
ble_sm_sc_initiator_txes_confirm(struct ble_sm_proc *proc)
{
    BLE_HS_DBG_ASSERT(proc->flags & BLE_SM_PROC_F_SC);

    return proc->pair_alg != BLE_SM_PAIR_ALG_JW &&
           proc->pair_alg != BLE_SM_PAIR_ALG_NUMCMP;
}

/* Responder does not verify the initiator's random number when pairing
 * algorithm is any of:
 *     o just works
 *     o numeric comparison
 * (vol. 3, part H, 2.3.5.6.2)
 */
static int
ble_sm_sc_responder_verifies_random(struct ble_sm_proc *proc)
{
    BLE_HS_DBG_ASSERT(proc->flags & BLE_SM_PROC_F_SC);

    return proc->pair_alg != BLE_SM_PAIR_ALG_JW &&
           proc->pair_alg != BLE_SM_PAIR_ALG_NUMCMP;
}

/**
 * Generates the Ri byte used in the confirm message.  On success, the byte is
 * written to the supplied procedure object.
 */
static int
ble_sm_sc_gen_ri(struct ble_sm_proc *proc)
{
    int byte;
    int bit;
    int rc;

    switch (proc->pair_alg) {
    case BLE_SM_PAIR_ALG_JW:
    case BLE_SM_PAIR_ALG_NUMCMP:
        proc->ri = 0;
        return 0;

    case BLE_SM_PAIR_ALG_PASSKEY:
        BLE_HS_DBG_ASSERT(proc->passkey_bits_exchanged <
                          BLE_SM_SC_PASSKEY_BITS);

        byte = proc->passkey_bits_exchanged / 8;
        bit = proc->passkey_bits_exchanged % 8;
        proc->ri = 0x80 | !!(proc->tk[byte] & (1 << bit));

        proc->passkey_bits_exchanged++;

        return 0;

    case BLE_SM_PAIR_ALG_OOB:
        rc = ble_hs_hci_util_rand(&proc->ri, 1);
        return rc;

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_HS_EUNKNOWN;
    }
}

void
ble_sm_sc_confirm_exec(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    struct ble_sm_pair_confirm *cmd;
    struct os_mbuf *txom;
    int rc;

    rc = ble_sm_sc_gen_ri(proc);
    if (rc != 0) {
        res->app_status = rc;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_CONFIRM, sizeof(*cmd), &txom);
    if (cmd == NULL) {
        rc = BLE_HS_ENOMEM;
        res->app_status = rc;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    rc = ble_sm_alg_f4(ble_sm_sc_pub_key, proc->pub_key_peer.x,
                       ble_sm_our_pair_rand(proc), proc->ri, cmd->value);
    if (rc != 0) {
        os_mbuf_free_chain(txom);
        res->app_status = rc;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        res->app_status = rc;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    if (!(proc->flags & BLE_SM_PROC_F_INITIATOR)) {
        proc->state = BLE_SM_PROC_STATE_RANDOM;
    }
}

static void
ble_sm_sc_gen_numcmp(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    uint8_t *pka;
    uint8_t *pkb;

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        pka = ble_sm_sc_pub_key;
        pkb = proc->pub_key_peer.x;
    } else {
        pka = proc->pub_key_peer.x;
        pkb = ble_sm_sc_pub_key;
    }
    res->app_status = ble_sm_alg_g2(pka, pkb, proc->randm, proc->rands,
                                    &res->passkey_params.numcmp);
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
    }
}

/**
 * Advances the supplied procedure object to the next state after it has
 * completed the random state.
 */
static int
ble_sm_sc_random_advance(struct ble_sm_proc *proc)
{
    int rc;

    if (proc->pair_alg != BLE_SM_PAIR_ALG_PASSKEY ||
        proc->passkey_bits_exchanged >= BLE_SM_SC_PASSKEY_BITS) {

        proc->state = BLE_SM_PROC_STATE_DHKEY_CHECK;
    } else {
        proc->state = BLE_SM_PROC_STATE_CONFIRM;
        rc = ble_sm_gen_pair_rand(ble_sm_our_pair_rand(proc));
        if (rc != 0) {
            return rc;
        }
    }

    return 0;
}

void
ble_sm_sc_random_exec(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    struct ble_sm_pair_random *cmd;
    struct os_mbuf *txom;
    uint8_t ioact;
    int rc;

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_RANDOM, sizeof(*cmd), &txom);
    if (cmd == NULL) {
        rc = BLE_HS_ENOMEM;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    memcpy(cmd->value, ble_sm_our_pair_rand(proc), 16);

    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        res->app_status = rc;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    if (!(proc->flags & BLE_SM_PROC_F_INITIATOR)) {
        rc = ble_sm_sc_random_advance(proc);
        if (rc != 0) {
            res->app_status = rc;
            res->enc_cb = 1;
            res->sm_err = BLE_SM_ERR_UNSPECIFIED;
            return;
        }

        rc = ble_sm_sc_io_action(proc, &ioact);
        BLE_HS_DBG_ASSERT(rc == 0);

        if (ble_sm_ioact_state(ioact) == proc->state &&
            !(proc->flags & BLE_SM_PROC_F_IO_INJECTED)) {

            res->passkey_params.action = ioact;
            BLE_HS_DBG_ASSERT(ioact == BLE_SM_IOACT_NUMCMP);
            ble_sm_sc_gen_numcmp(proc, res);
        }
    }
}

void
ble_sm_sc_random_rx(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    uint8_t confirm_val[16];
    uint8_t ia[6];
    uint8_t ra[6];
    uint8_t ioact;
    uint8_t iat;
    uint8_t rat;
    int rc;

    if (proc->flags & BLE_SM_PROC_F_INITIATOR ||
        ble_sm_sc_responder_verifies_random(proc)) {

        BLE_HS_LOG(DEBUG, "tk=");
        ble_hs_log_flat_buf(proc->tk, 16);
        BLE_HS_LOG(DEBUG, "\n");

        rc = ble_sm_alg_f4(proc->pub_key_peer.x, ble_sm_sc_pub_key,
                           ble_sm_peer_pair_rand(proc), proc->ri,
                           confirm_val);
        if (rc != 0) {
            res->app_status = rc;
            res->sm_err = BLE_SM_ERR_UNSPECIFIED;
            res->enc_cb = 1;
            return;
        }

        if (memcmp(proc->confirm_peer, confirm_val, 16) != 0) {
            /* Random number mismatch. */
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_CONFIRM_MISMATCH);
            res->sm_err = BLE_SM_ERR_CONFIRM_MISMATCH;
            res->enc_cb = 1;
            return;
        }
    }

    /* Calculate the mac key and ltk. */
    ble_sm_ia_ra(proc, &iat, ia, &rat, ra);
    rc = ble_sm_alg_f5(proc->dhkey, proc->randm, proc->rands,
                       iat, ia, rat, ra, proc->mackey, proc->ltk);
    if (rc != 0) {
        res->app_status = rc;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    /* Ensure proper key size */
    memset(proc->ltk + proc->key_size, 0, sizeof proc->ltk - proc->key_size);

    /* Ensure the ltk gets persisted when the pairing procedure succeeds. */
    memcpy(proc->our_keys.ltk, proc->ltk, sizeof proc->our_keys.ltk);
    proc->our_keys.ltk_valid = 1;
    proc->our_keys.ediv = 0;
    proc->our_keys.rand_val = 0;
    proc->our_keys.ediv_rand_valid = 1;
    proc->our_keys.key_size = proc->key_size;

    memcpy(proc->peer_keys.ltk, proc->ltk, sizeof proc->peer_keys.ltk);
    proc->peer_keys.ltk_valid = 1;
    proc->peer_keys.ediv = 0;
    proc->peer_keys.rand_val = 0;
    proc->peer_keys.ediv_rand_valid = 1;
    proc->peer_keys.key_size = proc->key_size;

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        ble_sm_sc_random_advance(proc);

        rc = ble_sm_sc_io_action(proc, &ioact);
        if (rc != 0) {
            BLE_HS_DBG_ASSERT(0);
        }

        if (ble_sm_ioact_state(ioact) == proc->state &&
            !(proc->flags & BLE_SM_PROC_F_IO_INJECTED)) {

            res->passkey_params.action = ioact;
            BLE_HS_DBG_ASSERT(ioact == BLE_SM_IOACT_NUMCMP);
            ble_sm_sc_gen_numcmp(proc, res);
        } else {
            res->execute = 1;
        }
    } else {
        res->execute = 1;
    }
}

void
ble_sm_sc_public_key_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                          void *arg)
{
    struct ble_sm_public_key *cmd;
    struct os_mbuf *txom;
    uint8_t ioact;
    int rc;

    res->app_status = ble_sm_sc_ensure_keys_generated();
    if (res->app_status != 0) {
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_PUBLIC_KEY, sizeof(*cmd), &txom);
    if (!cmd) {
        res->app_status = BLE_HS_ENOMEM;
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    memcpy(cmd->x, ble_sm_sc_pub_key + 0, 32);
    memcpy(cmd->y, ble_sm_sc_pub_key + 32, 32);

    res->app_status = ble_sm_tx(proc->conn_handle, txom);
    if (res->app_status != 0) {
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    if (!(proc->flags & BLE_SM_PROC_F_INITIATOR)) {
        proc->state = BLE_SM_PROC_STATE_CONFIRM;

        rc = ble_sm_sc_io_action(proc, &ioact);
        if (rc != 0) {
            BLE_HS_DBG_ASSERT(0);
        }

        if (ble_sm_ioact_state(ioact) == proc->state) {
            res->passkey_params.action = ioact;
        }

        if (ble_sm_proc_can_advance(proc) &&
            !ble_sm_sc_initiator_txes_confirm(proc)) {

            res->execute = 1;
        }
    }
}

void
ble_sm_sc_public_key_rx(uint16_t conn_handle, struct os_mbuf **om,
                        struct ble_sm_result *res)
{
    struct ble_sm_public_key *cmd;
    struct ble_sm_proc *proc;
    uint8_t ioact;
    int rc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->enc_cb = 1;
        return;
    }

    res->app_status = ble_sm_sc_ensure_keys_generated();
    if (res->app_status != 0) {
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    cmd = (struct ble_sm_public_key *)(*om)->om_data;
    BLE_SM_LOG_CMD(0, "public key", conn_handle, ble_sm_public_key_log, cmd);

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_PUBLIC_KEY, -1,
                            NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
    } else {
        memcpy(&proc->pub_key_peer, cmd, sizeof(*cmd));
        rc = ble_sm_alg_gen_dhkey(proc->pub_key_peer.x,
                                  proc->pub_key_peer.y,
                                  ble_sm_sc_priv_key,
                                  proc->dhkey);
        if (rc != 0) {
            res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_DHKEY);
            res->sm_err = BLE_SM_ERR_DHKEY;
            res->enc_cb = 1;
        } else {
            if (proc->flags & BLE_SM_PROC_F_INITIATOR) {

                proc->state = BLE_SM_PROC_STATE_CONFIRM;

                rc = ble_sm_sc_io_action(proc, &ioact);
                if (rc != 0) {
                        BLE_HS_DBG_ASSERT(0);
                }

                if (ble_sm_ioact_state(ioact) == proc->state) {
                    res->passkey_params.action = ioact;
                }

                if (ble_sm_proc_can_advance(proc) &&
                    ble_sm_sc_initiator_txes_confirm(proc)) {

                    res->execute = 1;
                }
            } else {
                res->execute = 1;
            }
        }
    }
    ble_hs_unlock();
}

static void
ble_sm_sc_dhkey_addrs(struct ble_sm_proc *proc, ble_addr_t *our_addr,
                      ble_addr_t *peer_addr)
{
    struct ble_hs_conn_addrs addrs;
    struct ble_hs_conn *conn;

    conn = ble_hs_conn_find_assert(proc->conn_handle);

    ble_hs_conn_addrs(conn, &addrs);

    *our_addr = addrs.our_ota_addr;
    *peer_addr = addrs.peer_ota_addr;
}

void
ble_sm_sc_dhkey_check_exec(struct ble_sm_proc *proc, struct ble_sm_result *res,
                           void *arg)
{
    struct ble_sm_dhkey_check *cmd;
    ble_addr_t our_addr;
    ble_addr_t peer_addr;
    struct os_mbuf *txom;
    uint8_t *iocap;
    int rc;

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        struct ble_sm_pair_cmd *pair_req;

        pair_req  = (struct ble_sm_pair_cmd *) &proc->pair_req[1];
        iocap = &pair_req->io_cap;
    } else {
        struct ble_sm_pair_cmd *pair_rsp;

        pair_rsp  = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];
        iocap = &pair_rsp->io_cap;
    }

    ble_sm_sc_dhkey_addrs(proc, &our_addr, &peer_addr);

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_DHKEY_CHECK, sizeof(*cmd), &txom);
    if (!cmd) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    rc = ble_sm_alg_f6(proc->mackey, ble_sm_our_pair_rand(proc),
                       ble_sm_peer_pair_rand(proc), proc->tk, iocap,
                       our_addr.type, our_addr.val, peer_addr.type,
                       peer_addr.val, cmd->value);
    if (rc != 0) {
        os_mbuf_free_chain(txom);
        goto err;
    }

    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        goto err;
    }

    if (!(proc->flags & BLE_SM_PROC_F_INITIATOR)) {
        proc->state = BLE_SM_PROC_STATE_LTK_START;
    }

    return;

err:
    res->app_status = rc;
    res->enc_cb = 1;
    res->sm_err = BLE_SM_ERR_UNSPECIFIED;
}

static void
ble_sm_dhkey_check_process(struct ble_sm_proc *proc,
                           struct ble_sm_dhkey_check *cmd,
                           struct ble_sm_result *res)
{
    uint8_t exp_value[16];
    ble_addr_t our_addr;
    ble_addr_t peer_addr;
    uint8_t *iocap;
    uint8_t ioact;
    int rc;

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        struct ble_sm_pair_cmd *pair_rsp;

        pair_rsp  = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];
        iocap = &pair_rsp->io_cap;
    } else {
        struct ble_sm_pair_cmd *pair_req;

        pair_req  = (struct ble_sm_pair_cmd *) &proc->pair_req[1];
        iocap = &pair_req->io_cap;
    }

    ble_sm_sc_dhkey_addrs(proc, &our_addr, &peer_addr);
    BLE_HS_LOG(DEBUG, "tk=");
    ble_hs_log_flat_buf(proc->tk, 16);
    BLE_HS_LOG(DEBUG, "\n");

    res->app_status = ble_sm_alg_f6(proc->mackey,
                                    ble_sm_peer_pair_rand(proc),
                                    ble_sm_our_pair_rand(proc),
                                    proc->tk, iocap,
                                    peer_addr.type, peer_addr.val,
                                    our_addr.type, our_addr.val,
                                    exp_value);
    if (res->app_status != 0) {
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    if (memcmp(cmd->value, exp_value, 16) != 0) {
        /* Random number mismatch. */
        res->sm_err = BLE_SM_ERR_DHKEY;
        res->app_status = BLE_HS_SM_US_ERR(BLE_SM_ERR_DHKEY);
        res->enc_cb = 1;
        return;
    }

    rc = ble_sm_sc_io_action(proc, &ioact);
    if (rc != 0) {
        BLE_HS_DBG_ASSERT(0);
    }

    if (ble_sm_ioact_state(ioact) == proc->state) {
        proc->flags |= BLE_SM_PROC_F_ADVANCE_ON_IO;
    }

    if (ble_sm_proc_can_advance(proc)) {
        if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
            proc->state = BLE_SM_PROC_STATE_ENC_START;
        }

        res->execute = 1;
    }
}

void
ble_sm_sc_dhkey_check_rx(uint16_t conn_handle, struct os_mbuf **om,
                         struct ble_sm_result *res)
{
    struct ble_sm_dhkey_check *cmd;
    struct ble_sm_proc *proc;

    res->app_status = ble_hs_mbuf_pullup_base(om, sizeof(*cmd));
    if (res->app_status != 0) {
        res->enc_cb = 1;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        return;
    }

    cmd = (struct ble_sm_dhkey_check *)(*om)->om_data;
    BLE_SM_LOG_CMD(0, "dhkey check", conn_handle, ble_sm_dhkey_check_log, cmd);

    ble_hs_lock();
    proc = ble_sm_proc_find(conn_handle, BLE_SM_PROC_STATE_DHKEY_CHECK, -1,
                            NULL);
    if (proc == NULL) {
        res->app_status = BLE_HS_ENOENT;
    } else {
        ble_sm_dhkey_check_process(proc, cmd, res);
    }
    ble_hs_unlock();
}

void
ble_sm_sc_init(void)
{
    ble_sm_alg_ecc_init();
    ble_sm_sc_keys_generated = 0;
}

#endif  /* MYNEWT_VAL(BLE_SM_SC) */
