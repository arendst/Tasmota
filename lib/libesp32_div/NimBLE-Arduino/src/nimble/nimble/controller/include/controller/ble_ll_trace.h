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

#ifndef H_BLE_LL_TRACE_
#define H_BLE_LL_TRACE_

#include "nimble/porting/nimble/include/os/os_trace_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_LL_TRACE_ID_SCHED                   0
#define BLE_LL_TRACE_ID_RX_START                1
#define BLE_LL_TRACE_ID_RX_END                  2
#define BLE_LL_TRACE_ID_WFR_EXP                 3
#define BLE_LL_TRACE_ID_CTRL_RX                 4
#define BLE_LL_TRACE_ID_CONN_EV_START           5
#define BLE_LL_TRACE_ID_CONN_EV_END             6
#define BLE_LL_TRACE_ID_CONN_END                7
#define BLE_LL_TRACE_ID_CONN_TX                 8
#define BLE_LL_TRACE_ID_CONN_RX                 9
#define BLE_LL_TRACE_ID_ADV_TXDONE              10
#define BLE_LL_TRACE_ID_ADV_HALT                11
#define BLE_LL_TRACE_ID_AUX_REF                 12
#define BLE_LL_TRACE_ID_AUX_UNREF               13

#if MYNEWT_VAL(BLE_LL_SYSVIEW)

extern uint32_t ble_ll_trace_off;

void ble_ll_trace_init(void);

static inline void
ble_ll_trace_u32(unsigned id, uint32_t p1)
{
    os_trace_api_u32(ble_ll_trace_off + id, p1);
}

static inline void
ble_ll_trace_u32x2(unsigned id, uint32_t p1, uint32_t p2)
{
    os_trace_api_u32x2(ble_ll_trace_off + id, p1, p2);
}

static inline void
ble_ll_trace_u32x3(unsigned id, uint32_t p1, uint32_t p2, uint32_t p3)
{
    os_trace_api_u32x3(ble_ll_trace_off + id, p1, p2, p3);
}

#else

static inline void
ble_ll_trace_init(void)
{
}

static inline void
ble_ll_trace_u32(unsigned id, uint32_t p1)
{
}

static inline void
ble_ll_trace_u32x2(unsigned id, uint32_t p1, uint32_t p2)
{
}

static inline void
ble_ll_trace_u32x3(unsigned id, uint32_t p1, uint32_t p2, uint32_t p3)
{
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* H_BLE_LL_TRACE_ */
