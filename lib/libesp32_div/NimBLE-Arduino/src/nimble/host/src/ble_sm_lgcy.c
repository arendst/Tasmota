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
#include "nimble/ble.h"
#include "nimble/nimble_opt.h"
#include "host/ble_sm.h"
#include "ble_hs_priv.h"

#if MYNEWT_VAL(BLE_SM_LEGACY)

/**
 * Create some shortened names for the passkey actions so that the table is
 * easier to read.
 */
#define IOACT_NONE  BLE_SM_IOACT_NONE
#define IOACT_OOB   BLE_SM_IOACT_OOB
#define IOACT_INPUT BLE_SM_IOACT_INPUT
#define IOACT_DISP  BLE_SM_IOACT_DISP

/* This is the initiator passkey action action dpeneding on the io
 * capabilties of both parties
 */
static const uint8_t ble_sm_lgcy_init_ioa[5 /*resp*/ ][5 /*init*/ ] =
{
    {IOACT_NONE,    IOACT_NONE,   IOACT_INPUT, IOACT_NONE, IOACT_INPUT},
    {IOACT_NONE,    IOACT_NONE,   IOACT_INPUT, IOACT_NONE, IOACT_INPUT},
    {IOACT_DISP,    IOACT_DISP,   IOACT_INPUT, IOACT_NONE, IOACT_DISP},
    {IOACT_NONE,    IOACT_NONE,   IOACT_NONE,  IOACT_NONE, IOACT_NONE},
    {IOACT_DISP,    IOACT_DISP,   IOACT_INPUT, IOACT_NONE, IOACT_DISP},
};

/* This is the responder passkey action action depending on the io
 * capabilities of both parties
 */
static const uint8_t ble_sm_lgcy_resp_ioa[5 /*resp*/ ][5 /*init*/ ] =
{
    {IOACT_NONE,    IOACT_NONE,   IOACT_DISP,  IOACT_NONE, IOACT_DISP},
    {IOACT_NONE,    IOACT_NONE,   IOACT_DISP,  IOACT_NONE, IOACT_DISP},
    {IOACT_INPUT,   IOACT_INPUT,  IOACT_INPUT, IOACT_NONE, IOACT_INPUT},
    {IOACT_NONE,    IOACT_NONE,   IOACT_NONE,  IOACT_NONE, IOACT_NONE},
    {IOACT_INPUT,   IOACT_INPUT,  IOACT_DISP,  IOACT_NONE, IOACT_INPUT},
};

int
ble_sm_lgcy_io_action(struct ble_sm_proc *proc, uint8_t *action)
{
    struct ble_sm_pair_cmd *pair_req, *pair_rsp;

    pair_req = (struct ble_sm_pair_cmd *) &proc->pair_req[1];
    pair_rsp = (struct ble_sm_pair_cmd *) &proc->pair_rsp[1];

    if (pair_req->oob_data_flag == BLE_SM_PAIR_OOB_YES &&
        pair_rsp->oob_data_flag == BLE_SM_PAIR_OOB_YES) {
        *action = BLE_SM_IOACT_OOB;
    } else if (!(pair_req->authreq & BLE_SM_PAIR_AUTHREQ_MITM) &&
               !(pair_rsp->authreq & BLE_SM_PAIR_AUTHREQ_MITM)) {

        *action = BLE_SM_IOACT_NONE;
    } else if (pair_req->io_cap >= BLE_SM_IO_CAP_RESERVED ||
               pair_rsp->io_cap >= BLE_SM_IO_CAP_RESERVED) {
        *action = BLE_SM_IOACT_NONE;
    } else if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        *action = ble_sm_lgcy_init_ioa[pair_rsp->io_cap][pair_req->io_cap];
    } else {
        *action = ble_sm_lgcy_resp_ioa[pair_rsp->io_cap][pair_req->io_cap];
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

    default:
        BLE_HS_DBG_ASSERT(0);
        return BLE_HS_EINVAL;
    }

    return 0;
}

void
ble_sm_lgcy_confirm_exec(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    struct ble_sm_pair_confirm *cmd;
    struct os_mbuf *txom;
    uint8_t ia[6];
    uint8_t ra[6];
    uint8_t iat;
    uint8_t rat;
    int rc;

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_CONFIRM, sizeof(*cmd), &txom);
    if (cmd == NULL) {
        rc = BLE_HS_ENOMEM;
        goto err;
    }

    ble_sm_ia_ra(proc, &iat, ia, &rat, ra);

    rc = ble_sm_alg_c1(proc->tk, ble_sm_our_pair_rand(proc), proc->pair_req,
                       proc->pair_rsp, iat, rat, ia, ra, cmd->value);
    if (rc != 0) {
        goto err;
    }

    rc = ble_sm_tx(proc->conn_handle, txom);
    if (rc != 0) {
        goto err;
    }

    if (!(proc->flags & BLE_SM_PROC_F_INITIATOR)) {
        proc->state = BLE_SM_PROC_STATE_RANDOM;
    }

    return;

err:
    if (txom) {
        os_mbuf_free_chain(txom);
    }

    res->app_status = rc;
    res->enc_cb = 1;
    res->sm_err = BLE_SM_ERR_UNSPECIFIED;
}

static int
ble_sm_gen_stk(struct ble_sm_proc *proc)
{
    uint8_t key[16];
    int rc;

    rc = ble_sm_alg_s1(proc->tk, proc->rands, proc->randm, key);
    if (rc != 0) {
        return rc;
    }

    memcpy(proc->ltk, key, proc->key_size);

    /* Ensure proper key size */
    memset(proc->ltk + proc->key_size, 0, sizeof key - proc->key_size);

    return 0;
}

void
ble_sm_lgcy_random_exec(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    struct ble_sm_pair_random *cmd;
    struct os_mbuf *txom;
    int rc;

    cmd = ble_sm_cmd_get(BLE_SM_OP_PAIR_RANDOM, sizeof(*cmd), &txom);
    if (cmd == NULL) {
        res->app_status = BLE_HS_ENOMEM;
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
        proc->state = BLE_SM_PROC_STATE_LTK_START;
    }
}

void
ble_sm_lgcy_random_rx(struct ble_sm_proc *proc, struct ble_sm_result *res)
{
    uint8_t confirm_val[16];
    uint8_t ia[6];
    uint8_t ra[6];
    uint8_t iat;
    uint8_t rat;
    int rc;

    ble_sm_ia_ra(proc, &iat, ia, &rat, ra);

    rc = ble_sm_alg_c1(proc->tk, ble_sm_peer_pair_rand(proc), proc->pair_req,
                       proc->pair_rsp, iat, rat, ia, ra, confirm_val);
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

    /* Generate the key. */
    rc = ble_sm_gen_stk(proc);
    if (rc != 0) {
        res->app_status = rc;
        res->sm_err = BLE_SM_ERR_UNSPECIFIED;
        res->enc_cb = 1;
        return;
    }

    if (proc->flags & BLE_SM_PROC_F_INITIATOR) {
        /* Send the start-encrypt HCI command to the controller.   For
         * short-term key generation, we always set ediv and rand to 0.
         * (Vol. 3, part H, 2.4.4.1).
         */
        proc->state = BLE_SM_PROC_STATE_ENC_START;
    }

    res->execute = 1;
}

#endif
