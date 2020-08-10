/*
 * Copyright 2020 Espressif Systems (Shanghai) PTE LTD
 *
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

#ifdef __cplusplus
extern "C" {
#endif

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
/*
 * An entry in the resolving list.
 */
struct ble_hs_resolv_entry {
    uint8_t rl_addr_type;
    uint8_t rl_local_irk[16];
    uint8_t rl_peer_irk[16];
    uint8_t rl_identity_addr[BLE_DEV_ADDR_LEN];
    uint8_t rl_pseudo_id[BLE_DEV_ADDR_LEN];
    uint8_t rl_local_rpa[BLE_DEV_ADDR_LEN];
    uint8_t rl_peer_rpa[BLE_DEV_ADDR_LEN];
};

#if MYNEWT_VAL(BLE_STORE_CONFIG_PERSIST)
/* Persist peer records in NVS. XXX Need to handle this in `store` module */
int ble_store_persist_peer_records(void);
#endif

struct ble_hs_peer_sec {
    ble_addr_t peer_addr;
    uint8_t irk[16];
    uint8_t irk_present: 1;
};
/*
 * BLE host peer device record, this helps in storing peer RPA before bond is
 * created and IRKs are exchanged.
 */
struct ble_hs_dev_records {
    bool rec_used;
    uint8_t pseudo_addr[BLE_DEV_ADDR_LEN];
    uint8_t rand_addr[BLE_DEV_ADDR_LEN];
    uint8_t identity_addr[BLE_DEV_ADDR_LEN];
    struct ble_hs_peer_sec peer_sec;
};

/* Add a device to the resolving list */
int ble_hs_resolv_list_add(uint8_t *cmdbuf);
int ble_hs_gen_own_private_rnd(void);
uint8_t *ble_hs_get_rpa_local(void);

/* Remove a device from the resolving list */
int ble_hs_resolv_list_rmv(uint8_t, uint8_t *);
/* Clear the resolving list and peer dev record */
void ble_hs_resolv_list_clear_all(void);

/* Address resolution enable command */
void ble_hs_resolv_enable(bool);
void ble_hs_resolv_nrpa_enable(void);
void ble_hs_resolv_nrpa_disable(void);

/* Finds 'addr' in resolving list. Doesnt check if address resolution enabled */
struct ble_hs_resolv_entry *
ble_hs_resolv_list_find(uint8_t *addr);

/* Returns true if host based RPA (privacy) is enabled */
bool ble_host_rpa_enabled(void);

/* Searches peer device records (RPA) and fetches matching RL, peer_address
 * into input parameters if RL is found */
void
ble_rpa_replace_peer_params_with_rl(uint8_t *, uint8_t *, struct ble_hs_resolv_entry **);

int ble_rpa_resolv_add_peer_rec(uint8_t *);

struct ble_hs_dev_records *ble_rpa_get_peer_dev_records(void);
int ble_rpa_get_num_peer_dev_records(void);
void ble_rpa_set_num_peer_dev_records(int);
int ble_rpa_remove_peer_dev_rec(struct ble_hs_dev_records *);
struct ble_hs_dev_records *ble_rpa_find_peer_dev_rec(uint8_t *);

/* Set the resolvable private address timeout */
int ble_hs_resolv_set_rpa_tmo(uint16_t);

/* Resolve a resolvable private address */
int ble_hs_resolv_rpa(uint8_t *rpa, uint8_t *irk);

/* Initialize resolv*/
void ble_hs_resolv_init(void);

#ifdef __cplusplus
}
#endif

#endif
