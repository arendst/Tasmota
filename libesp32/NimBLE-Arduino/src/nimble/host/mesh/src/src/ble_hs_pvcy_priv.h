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

#ifndef H_BLE_HS_PVCY_PRIV_
#define H_BLE_HS_PVCY_PRIV_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t ble_hs_pvcy_default_irk[16];

int ble_hs_pvcy_set_our_irk(const uint8_t *irk);
int ble_hs_pvcy_our_irk(const uint8_t **out_irk);
int ble_hs_pvcy_remove_entry(uint8_t addr_type, const uint8_t *addr);
int ble_hs_pvcy_add_entry(const uint8_t *addr, uint8_t addrtype,
                          const uint8_t *irk);
int ble_hs_pvcy_ensure_started(void);
int ble_hs_pvcy_set_mode(const ble_addr_t *addr, uint8_t priv_mode);
#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
bool ble_hs_pvcy_enabled(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
