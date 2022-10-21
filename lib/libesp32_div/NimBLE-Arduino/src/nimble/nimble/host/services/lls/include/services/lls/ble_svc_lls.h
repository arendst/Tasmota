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

#ifndef H_BLE_SVC_LLS_
#define H_BLE_SVC_LLS_

#ifdef __cplusplus
extern "C" {
#endif

struct ble_hs_cfg;

#define BLE_SVC_LLS_UUID16                                  0x1803
#define BLE_SVC_LLS_CHR_UUID16_ALERT_LEVEL                  0x2a06

/* Alert level definitions */
#define BLE_SVC_LLS_ALERT_LEVEL_NO_ALERT                    0
#define BLE_SVC_LLS_ALERT_LEVEL_MILD_ALERT                  1
#define BLE_SVC_LLS_ALERT_LEVEL_HIGH_ALERT                  2

typedef int ble_svc_lls_event_fn(uint8_t alert_level);

uint8_t ble_svc_lls_alert_level_get(void);
int ble_svc_lls_alert_level_set(uint8_t alert_level);
void ble_svc_lls_on_gap_disconnect(int reason);

void ble_svc_lls_set_cb(ble_svc_lls_event_fn *cb);
void ble_svc_lls_init(void);

#ifdef __cplusplus
}
#endif

#endif

