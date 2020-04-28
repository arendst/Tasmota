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

#ifndef H_BLE_SVC_GAP_
#define H_BLE_SVC_GAP_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_SVC_GAP_UUID16                                  0x1800
#define BLE_SVC_GAP_CHR_UUID16_DEVICE_NAME                  0x2a00
#define BLE_SVC_GAP_CHR_UUID16_APPEARANCE                   0x2a01
#define BLE_SVC_GAP_CHR_UUID16_PERIPH_PREF_CONN_PARAMS      0x2a04
#define BLE_SVC_GAP_CHR_UUID16_CENTRAL_ADDRESS_RESOLUTION   0x2aa6

#define BLE_SVC_GAP_APPEARANCE_GEN_UNKNOWN                         0
#define BLE_SVC_GAP_APPEARANCE_GEN_COMPUTER                        128
#define BLE_SVC_GAP_APPEARANCE_CYC_SPEED_AND_CADENCE_SENSOR        1157

typedef void (ble_svc_gap_chr_changed_fn) (uint16_t uuid);

void ble_svc_gap_set_chr_changed_cb(ble_svc_gap_chr_changed_fn *cb);

const char *ble_svc_gap_device_name(void);
int ble_svc_gap_device_name_set(const char *name);
uint16_t ble_svc_gap_device_appearance(void);
int ble_svc_gap_device_appearance_set(uint16_t appearance);

void ble_svc_gap_init(void);

#ifdef __cplusplus
}
#endif

#endif
