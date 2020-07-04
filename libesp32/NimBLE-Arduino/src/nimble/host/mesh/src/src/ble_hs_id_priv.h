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

#ifndef H_BLE_HS_ID_PRIV_
#define H_BLE_HS_ID_PRIV_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void ble_hs_id_set_pub(const uint8_t *pub_addr);
int ble_hs_id_addr(uint8_t id_addr_type, const uint8_t **out_id_addr,
                   int *out_is_nrpa);
int ble_hs_id_use_addr(uint8_t addr_type);
void ble_hs_id_reset(void);
void ble_hs_id_rnd_reset(void);

#if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
bool ble_hs_is_rpa(uint8_t *addr, uint8_t addr_type);
int ble_hs_id_set_pseudo_rnd(const uint8_t *);
#endif
#ifdef __cplusplus
}
#endif

#endif
