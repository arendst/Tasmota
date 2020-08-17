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

#ifndef H_BLE_HS_LOG_
#define H_BLE_HS_LOG_

#include "modlog/modlog.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

#define BLE_HS_LOG(lvl, ...) \
    MODLOG_ ## lvl(LOG_MODULE_NIMBLE_HOST, __VA_ARGS__)

#define BLE_HS_LOG_ADDR(lvl, addr)                      \
    MODLOG_ ## lvl(LOG_MODULE_NIMBLE_HOST,              \
                   "%02x:%02x:%02x:%02x:%02x:%02x",     \
                   (addr)[5], (addr)[4], (addr)[3],     \
                   (addr)[2], (addr)[1], (addr)[0])

void ble_hs_log_mbuf(const struct os_mbuf *om);
void ble_hs_log_flat_buf(const void *data, int len);

#ifdef __cplusplus
}
#endif

#endif
