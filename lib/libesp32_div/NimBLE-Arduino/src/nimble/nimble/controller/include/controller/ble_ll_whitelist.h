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

#ifndef H_BLE_LL_WHITELIST_
#define H_BLE_LL_WHITELIST_

#ifdef __cplusplus
extern "C" {
#endif

/* Clear the whitelist */
int ble_ll_whitelist_clear(void);

/* Read the size of the whitelist */
int ble_ll_whitelist_read_size(uint8_t *rspbuf, uint8_t *rsplen);

/* Add a device to the whitelist */
int ble_ll_whitelist_add(const uint8_t *cmdbuf, uint8_t len);

/* Remove a device fromthe whitelist */
int ble_ll_whitelist_rmv(const uint8_t *cmdbuf, uint8_t len);

/* Enable whitelisting */
void ble_ll_whitelist_enable(void);

/* Disable whitelisting */
void ble_ll_whitelist_disable(void);

/* Boolean function returning true if address matches a whitelist entry */
int ble_ll_whitelist_match(uint8_t *addr, uint8_t addr_type, int is_ident);

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_WHITELIST_ */
