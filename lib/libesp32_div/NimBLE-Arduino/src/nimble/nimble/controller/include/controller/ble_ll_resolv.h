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

#ifndef H_BLE_LL_RESOLV_
#define H_BLE_LL_RESOLV_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * An entry in the resolving list.
 *      The identity address is stored in little endian format.
 *      The local rpa is stored in little endian format.
 *      The IRKs are stored in big endian format.
 *
 *  Note:
 *  rl_local_irk and rl_peer_irk need to be word aligned
 */
struct ble_ll_resolv_entry
{
    uint8_t rl_addr_type;
    uint8_t rl_priv_mode;
    uint8_t rl_has_local;
    uint8_t rl_has_peer;
    uint8_t rl_local_irk[16];
    uint8_t rl_peer_irk[16];
    uint8_t rl_identity_addr[BLE_DEV_ADDR_LEN];
    uint8_t rl_local_rpa[BLE_DEV_ADDR_LEN];
    uint8_t rl_peer_rpa[BLE_DEV_ADDR_LEN];
};

extern struct ble_ll_resolv_entry g_ble_ll_resolv_list[];

/* Clear the resolving list */
int ble_ll_resolv_list_clr(void);

/* Read the size of the resolving list */
int ble_ll_resolv_list_read_size(uint8_t *rspbuf, uint8_t *rsplen);

/* Add a device to the resolving list */
int ble_ll_resolv_list_add(const uint8_t *cmdbuf, uint8_t len);

/* Remove a device from the resolving list */
int ble_ll_resolv_list_rmv(const uint8_t *cmdbuf, uint8_t len);

/* Address resolution enable command */
int ble_ll_resolv_enable_cmd(const uint8_t *cmdbuf, uint8_t len);

int ble_ll_resolv_peer_addr_rd(const uint8_t *cmdbuf, uint8_t len,
                               uint8_t *rspbuf, uint8_t *rsplen);
int ble_ll_resolv_local_addr_rd(const uint8_t *cmdbuf, uint8_t len,
                                uint8_t *rspbuf, uint8_t *rsplen);

/* Finds 'addr' in resolving list. Doesnt check if address resolution enabled */
struct ble_ll_resolv_entry *
ble_ll_resolv_list_find(const uint8_t *addr, uint8_t addr_type);

/* Returns true if address resolution is enabled */
uint8_t ble_ll_resolv_enabled(void);

/* Reset private address resolution */
void ble_ll_resolv_list_reset(void);

/* Generate local or peer RPA. It is up to caller to make sure required IRK
 * is present on RL
 */
void ble_ll_resolv_get_priv_addr(struct ble_ll_resolv_entry *rl, int local,
                                 uint8_t *addr);

void ble_ll_resolv_set_peer_rpa(int index, uint8_t *rpa);
void ble_ll_resolv_set_local_rpa(int index, uint8_t *rpa);

/* Generate a resolvable private address. */
int ble_ll_resolv_gen_rpa(uint8_t *addr, uint8_t addr_type, uint8_t *rpa,
                          int local);

/* Set the resolvable private address timeout */
int ble_ll_resolv_set_rpa_tmo(const uint8_t *cmdbuf, uint8_t len);

/* Set the privacy mode */
int ble_ll_resolve_set_priv_mode(const uint8_t *cmdbuf, uint8_t len);

/* Get the RPA timeout, in seconds */
uint32_t ble_ll_resolv_get_rpa_tmo(void);

/* Resolve a resolvable private address */
int ble_ll_resolv_rpa(const uint8_t *rpa, const uint8_t *irk);

/* Try to resolve peer RPA and return index on RL if matched */
int ble_ll_resolv_peer_rpa_any(const uint8_t *rpa);

/* Initialize resolv*/
void ble_ll_resolv_init(void);

#ifdef __cplusplus
}
#endif

#endif
