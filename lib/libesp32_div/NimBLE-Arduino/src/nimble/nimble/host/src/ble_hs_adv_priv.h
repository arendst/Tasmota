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

#ifndef H_BLE_HS_ADV_PRIV_
#define H_BLE_HS_ADV_PRIV_

#ifdef __cplusplus
extern "C" {
#endif

int ble_hs_adv_set_flat(uint8_t type, int data_len, const void *data,
                        uint8_t *dst, uint8_t *dst_len, uint8_t max_len);
int ble_hs_adv_find_field(uint8_t type, const uint8_t *data, uint8_t length,
                          const struct ble_hs_adv_field **out);

#ifdef __cplusplus
}
#endif

#endif
