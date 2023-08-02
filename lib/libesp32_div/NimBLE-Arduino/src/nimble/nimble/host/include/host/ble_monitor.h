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

#ifndef H_BLE_MONITOR_
#define H_BLE_MONITOR_

#include "nimble/porting/nimble/include/syscfg/syscfg.h"

#undef BLE_MONITOR
#define BLE_MONITOR (MYNEWT_VAL(BLE_MONITOR_UART) || MYNEWT_VAL(BLE_MONITOR_RTT))

#ifdef __cplusplus
extern "C" {
#endif

int ble_monitor_log(int level, const char *fmt, ...);

int ble_monitor_out(int c);
void ble_monitor_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
