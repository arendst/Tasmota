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

#ifndef H_BLE_SM_PRIV_
#define H_BLE_SM_PRIV_

#include <inttypes.h>
#include "syscfg/syscfg.h"
#include "os/queue.h"
#include "nimble/nimble_opt.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ble_gap_sec_state;
struct hci_le_lt_key_req;
struct hci_encrypt_change;

#define BLE_SM_MTU                  65

#define BLE_SM_OP_PAIR_REQ                      0x01
#define BLE_SM_OP_PAIR_RSP                      0x02
#define BLE_SM_OP_PAIR_CONFIRM                  0x03
#define BLE_SM_OP_PAIR_RANDOM                   0x04
#define BLE_SM_OP_PAIR_FAIL                     0x05
#define BLE_SM_OP_ENC_INFO                      0x06
#define BLE_SM_OP_MASTER_ID                     0x07
#define BLE_SM_OP_IDENTITY_INFO                 0x08
#define BLE_SM_OP_IDENTITY_ADDR_INFO            0x09
#define BLE_SM_OP_SIGN_INFO                     0x0a
#define BLE_SM_OP_SEC_REQ                       0x0b
#define BLE_SM_OP_PAIR_PUBLIC_KEY               0x0c
#define BLE_SM_OP_PAIR_DHKEY_CHECK              0x0d
#define BLE_SM_OP_PAIR_KEYPRESS_NOTIFY          0x0e

struct ble_sm_hdr {
    uint8_t opcode;
    uint8_t data[0];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x01/0x02 [req/rsp])         | 1                 |
 * | IO Capability                      | 1                 |
 * | OOB data flag                      | 1                 |
 * | AuthReq                            | 1                 |
 * | Maximum Encryption Key Size        | 1                 |
 * | Initiator Key Distribution         | 1                 |
 * | Responder Key Distribution         | 1                 |
 */

struct ble_sm_pair_cmd {
    uint8_t io_cap;
    uint8_t oob_data_flag;
    uint8_t authreq;
    uint8_t max_enc_key_size;
    uint8_t init_key_dist;
    uint8_t resp_key_dist;
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x03)                        | 1                 |
 * | Confirm Value                      | 16                |
 */

struct ble_sm_pair_confirm {
    uint8_t value[16];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x04)                        | 1                 |
 * | Random Value                       | 16                |
 */
struct ble_sm_pair_random {
    uint8_t value[16];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x05)                        | 1                 |
 * | Reason                             | 1                 |
 */
struct ble_sm_pair_fail {
    uint8_t reason;
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x06)                        | 1                 |
 * | ltk                                | 16                |
 */
struct ble_sm_enc_info {
    uint8_t ltk[16];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x07)                        | 1                 |
 * | EDIV                               | 2                 |
 * | RAND                               | 8                 |
 */
struct ble_sm_master_id {
    uint16_t ediv;
    uint64_t rand_val;
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x08)                        | 1                 |
 * | irk                                | 16                |
 */
struct ble_sm_id_info {
    uint8_t irk[16];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x09)                        | 1                 |
 * | addr_type                          | 1                 |
 * | address                            | 6                 |
 */
struct ble_sm_id_addr_info {
    uint8_t addr_type;
    uint8_t bd_addr[6];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x0A)                        | 1                 |
 * | csrk                               | 16                |
 */
struct ble_sm_sign_info {
    uint8_t sig_key[16];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x0B)                        | 1                 |
 * | authreq                            | 1                 |
 */
struct ble_sm_sec_req {
    uint8_t authreq;
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x0c)                        | 1                 |
 * | Public Key X                       | 32                |
 * | Public Key Y                       | 32                |
 */
struct ble_sm_public_key {
    uint8_t x[32];
    uint8_t y[32];
} __attribute__((packed));

/**
 * | Parameter                          | Size (octets)     |
 * +------------------------------------+-------------------+
 * | (Code=0x0d)                        | 1                 |
 * | DHKey Check                        | 16                |
 */
struct ble_sm_dhkey_check {
    uint8_t value[16];
} __attribute__((packed));

#if NIMBLE_BLE_SM

#define BLE_SM_PROC_STATE_NONE              ((uint8_t)-1)

#define BLE_SM_PROC_STATE_PAIR              0
#define BLE_SM_PROC_STATE_CONFIRM           1
#define BLE_SM_PROC_STATE_RANDOM            2
#define BLE_SM_PROC_STATE_LTK_START         3
#define BLE_SM_PROC_STATE_LTK_RESTORE       4
#define BLE_SM_PROC_STATE_ENC_START         5
#define BLE_SM_PROC_STATE_ENC_RESTORE       6
#define BLE_SM_PROC_STATE_KEY_EXCH          7
#define BLE_SM_PROC_STATE_SEC_REQ           8
#define BLE_SM_PROC_STATE_PUBLIC_KEY        9
#define BLE_SM_PROC_STATE_DHKEY_CHECK       10
#define BLE_SM_PROC_STATE_CNT               11

#define BLE_SM_PROC_F_INITIATOR             0x01
#define BLE_SM_PROC_F_IO_INJECTED           0x02
#define BLE_SM_PROC_F_ADVANCE_ON_IO         0x04
#define BLE_SM_PROC_F_AUTHENTICATED         0x08
#define BLE_SM_PROC_F_SC                    0x10
#define BLE_SM_PROC_F_BONDING               0x20

#define BLE_SM_KE_F_ENC_INFO                0x01
#define BLE_SM_KE_F_MASTER_ID               0x02
#define BLE_SM_KE_F_ID_INFO                 0x04
#define BLE_SM_KE_F_ADDR_INFO               0x08
#define BLE_SM_KE_F_SIGN_INFO               0x10

typedef uint8_t ble_sm_proc_flags;

struct ble_sm_keys {
    unsigned ltk_valid:1;
    unsigned ediv_rand_valid:1;
    unsigned irk_valid:1;
    unsigned csrk_valid:1;
    unsigned addr_valid:1;
    uint16_t ediv;
    uint64_t rand_val;
    uint8_t addr_type;
    uint8_t key_size;
    uint8_t ltk[16];    /* Little endian. */
    uint8_t irk[16];    /* Little endian. */
    uint8_t csrk[16];   /* Little endian. */
    uint8_t addr[6];    /* Little endian. */
};

struct ble_sm_proc {
    STAILQ_ENTRY(ble_sm_proc) next;

    ble_npl_time_t exp_os_ticks;
    ble_sm_proc_flags flags;
    uint16_t conn_handle;
    uint8_t pair_alg;
    uint8_t state;
    uint8_t rx_key_flags;
    uint8_t key_size;

    uint8_t pair_req[sizeof(struct ble_sm_hdr) + sizeof(struct ble_sm_pair_cmd)];
    uint8_t pair_rsp[sizeof(struct ble_sm_hdr) + sizeof(struct ble_sm_pair_cmd)];
    uint8_t tk[16];
    uint8_t confirm_peer[16];
    uint8_t randm[16];
    uint8_t rands[16];
    uint8_t ltk[16]; /* Little endian. */
    struct ble_sm_keys our_keys;
    struct ble_sm_keys peer_keys;

#if MYNEWT_VAL(BLE_SM_SC)
    /* Secure connections. */
    uint8_t passkey_bits_exchanged;
    uint8_t ri;
    struct ble_sm_public_key pub_key_peer;
    uint8_t mackey[16];
    uint8_t dhkey[32];
#endif
};

struct ble_sm_result {
    int app_status;
    uint8_t sm_err;
    struct ble_gap_passkey_params passkey_params;
    void *state_arg;
    unsigned execute:1;
    unsigned enc_cb:1;
    unsigned persist_keys:1;
    unsigned restore:1;
};

#if MYNEWT_VAL(BLE_HS_DEBUG)
void ble_sm_dbg_set_next_pair_rand(uint8_t *next_pair_rand);
void ble_sm_dbg_set_next_ediv(uint16_t next_ediv);
void ble_sm_dbg_set_next_master_id_rand(uint64_t next_master_id_rand);
void ble_sm_dbg_set_next_ltk(uint8_t *next_ltk);
void ble_sm_dbg_set_next_csrk(uint8_t *next_csrk);
void ble_sm_dbg_set_sc_keys(uint8_t *pubkey, uint8_t *privkey);
#endif

int ble_sm_num_procs(void);

void ble_sm_pair_cmd_log(struct ble_sm_pair_cmd *cmd);
void ble_sm_pair_confirm_log(struct ble_sm_pair_confirm *cmd);
void ble_sm_pair_random_log(struct ble_sm_pair_random *cmd);
void ble_sm_pair_fail_log(struct ble_sm_pair_fail *cmd);
void ble_sm_enc_info_log(struct ble_sm_enc_info *cmd);
void ble_sm_master_id_log(struct ble_sm_master_id *cmd);
void ble_sm_id_info_log(struct ble_sm_id_info *cmd);
void ble_sm_id_addr_info_log(struct ble_sm_id_addr_info *cmd);
void ble_sm_sign_info_log(struct ble_sm_sign_info *cmd);
void ble_sm_sec_req_log(struct ble_sm_sec_req *cmd);
void ble_sm_public_key_log(struct ble_sm_public_key *cmd);
void ble_sm_dhkey_check_log(struct ble_sm_dhkey_check *cmd);

int ble_sm_alg_s1(uint8_t *k, uint8_t *r1, uint8_t *r2, uint8_t *out);
int ble_sm_alg_c1(uint8_t *k, uint8_t *r,
                  uint8_t *preq, uint8_t *pres,
                  uint8_t iat, uint8_t rat,
                  uint8_t *ia, uint8_t *ra,
                  uint8_t *out_enc_data);
int ble_sm_alg_f4(uint8_t *u, uint8_t *v, uint8_t *x, uint8_t z,
                  uint8_t *out_enc_data);
int ble_sm_alg_g2(uint8_t *u, uint8_t *v, uint8_t *x, uint8_t *y,
                  uint32_t *passkey);
int ble_sm_alg_f5(uint8_t *w, uint8_t *n1, uint8_t *n2, uint8_t a1t,
                  uint8_t *a1, uint8_t a2t, uint8_t *a2,
                  uint8_t *mackey, uint8_t *ltk);
int ble_sm_alg_f6(const uint8_t *w, const uint8_t *n1, const uint8_t *n2,
                  const uint8_t *r, const uint8_t *iocap, uint8_t a1t,
                  const uint8_t *a1, uint8_t a2t, const uint8_t *a2,
                  uint8_t *check);
int ble_sm_alg_gen_dhkey(uint8_t *peer_pub_key_x, uint8_t *peer_pub_key_y,
                         uint8_t *our_priv_key, uint8_t *out_dhkey);
int ble_sm_alg_gen_key_pair(uint8_t *pub, uint8_t *priv);
void ble_sm_alg_ecc_init(void);

void ble_sm_enc_change_rx(struct hci_encrypt_change *evt);
void ble_sm_enc_key_refresh_rx(struct hci_encrypt_key_refresh *evt);
int ble_sm_ltk_req_rx(struct hci_le_lt_key_req *evt);

#if MYNEWT_VAL(BLE_SM_LEGACY)
int ble_sm_lgcy_io_action(struct ble_sm_proc *proc, uint8_t *action);
void ble_sm_lgcy_confirm_exec(struct ble_sm_proc *proc,
                              struct ble_sm_result *res);
void ble_sm_lgcy_random_exec(struct ble_sm_proc *proc,
                             struct ble_sm_result *res);
void ble_sm_lgcy_random_rx(struct ble_sm_proc *proc,
                           struct ble_sm_result *res);
#else
#define ble_sm_lgcy_io_action(proc, action) (BLE_HS_ENOTSUP)
#define ble_sm_lgcy_confirm_exec(proc, res)
#define ble_sm_lgcy_random_exec(proc, res)
#define ble_sm_lgcy_random_rx(proc, res)
#endif

#if MYNEWT_VAL(BLE_SM_SC)
int ble_sm_sc_io_action(struct ble_sm_proc *proc, uint8_t *action);
void ble_sm_sc_confirm_exec(struct ble_sm_proc *proc,
                            struct ble_sm_result *res);
void ble_sm_sc_random_exec(struct ble_sm_proc *proc,
                           struct ble_sm_result *res);
void ble_sm_sc_random_rx(struct ble_sm_proc *proc, struct ble_sm_result *res);
void ble_sm_sc_public_key_exec(struct ble_sm_proc *proc,
                               struct ble_sm_result *res,
                               void *arg);
void ble_sm_sc_public_key_rx(uint16_t conn_handle, struct os_mbuf **rxom,
                             struct ble_sm_result *res);
void ble_sm_sc_dhkey_check_exec(struct ble_sm_proc *proc,
                                struct ble_sm_result *res, void *arg);
void ble_sm_sc_dhkey_check_rx(uint16_t conn_handle, struct os_mbuf **rxom,
                              struct ble_sm_result *res);
void ble_sm_sc_init(void);
#else
#define ble_sm_sc_io_action(proc, action) (BLE_HS_ENOTSUP)
#define ble_sm_sc_confirm_exec(proc, res)
#define ble_sm_sc_random_exec(proc, res)
#define ble_sm_sc_random_rx(proc, res)
#define ble_sm_sc_public_key_exec(proc, res, arg)
#define ble_sm_sc_public_key_rx(conn_handle, op, om, res)
#define ble_sm_sc_dhkey_check_exec(proc, res, arg)
#define ble_sm_sc_dhkey_check_rx(conn_handle, op, om, res)
#define ble_sm_sc_init()

#endif

struct ble_sm_proc *ble_sm_proc_find(uint16_t conn_handle, uint8_t state,
                                     int is_initiator,
                                     struct ble_sm_proc **out_prev);
int ble_sm_gen_pair_rand(uint8_t *pair_rand);
uint8_t *ble_sm_our_pair_rand(struct ble_sm_proc *proc);
uint8_t *ble_sm_peer_pair_rand(struct ble_sm_proc *proc);
int ble_sm_ioact_state(uint8_t action);
int ble_sm_proc_can_advance(struct ble_sm_proc *proc);
void ble_sm_process_result(uint16_t conn_handle, struct ble_sm_result *res);
void ble_sm_confirm_advance(struct ble_sm_proc *proc);
void ble_sm_ia_ra(struct ble_sm_proc *proc,
                  uint8_t *out_iat, uint8_t *out_ia,
                  uint8_t *out_rat, uint8_t *out_ra);

int32_t ble_sm_timer(void);
void ble_sm_connection_broken(uint16_t conn_handle);
int ble_sm_pair_initiate(uint16_t conn_handle);
int ble_sm_slave_initiate(uint16_t conn_handle);
int ble_sm_enc_initiate(uint16_t conn_handle, uint8_t key_size,
                        const uint8_t *ltk, uint16_t ediv,
                        uint64_t rand_val, int auth);
int ble_sm_init(void);

#define BLE_SM_LOG_CMD(is_tx, cmd_name, conn_handle, log_cb, cmd) \
    BLE_HS_LOG_CMD((is_tx), "sm", (cmd_name), (conn_handle), (log_cb), (cmd))

#else

#define ble_sm_enc_change_rx(evt) ((void)(evt))
#define ble_sm_ltk_req_rx(evt) ((void)(evt))
#define ble_sm_enc_key_refresh_rx(evt) ((void)(evt))

#define ble_sm_timer() BLE_HS_FOREVER
#define ble_sm_connection_broken(conn_handle)
#define ble_sm_pair_initiate(conn_handle)   BLE_HS_ENOTSUP
#define ble_sm_slave_initiate(conn_handle)  BLE_HS_ENOTSUP
#define ble_sm_enc_initiate(conn_handle, keysize, ltk, ediv, rand_val, auth) \
        BLE_HS_ENOTSUP

#define ble_sm_init() 0

#endif

struct ble_l2cap_chan *ble_sm_create_chan(uint16_t handle);
void *ble_sm_cmd_get(uint8_t opcode, size_t len, struct os_mbuf **txom);
int ble_sm_tx(uint16_t conn_handle, struct os_mbuf *txom);

#ifdef __cplusplus
}
#endif

#endif
