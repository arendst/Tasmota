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
#ifndef ESP_PLATFORM
#include <stdint.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os_trace_api.h"

#if MYNEWT_VAL(BLE_LL_SYSVIEW)

static os_trace_module_t g_ble_ll_trace_mod;
uint32_t ble_ll_trace_off;

static void
ble_ll_trace_module_send_desc(void)
{
    os_trace_module_desc(&g_ble_ll_trace_mod, "0 ll_sched lls=%u cputime=%u start_time=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "1 ll_rx_start lls=%u pdu_type=%x");
    os_trace_module_desc(&g_ble_ll_trace_mod, "2 ll_rx_end pdu_type=%x len=%u flags=%x");
    os_trace_module_desc(&g_ble_ll_trace_mod, "3 ll_wfr_timer_exp lls=%u xcvr=%u rx_start=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "4 ll_ctrl_rx opcode=%u len=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "5 ll_conn_ev_start conn_handle=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "6 ll_conn_ev_end conn_handle=%u event_cntr=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "7 ll_conn_end conn_handle=%u event_cntr=%u err=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "8 ll_conn_tx len=%u offset=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "9 ll_conn_rx conn_sn=%u pdu_nesn=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "10 ll_adv_txdone inst=%u chanset=%x");
    os_trace_module_desc(&g_ble_ll_trace_mod, "11 ll_adv_halt inst=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "12 ll_aux_ref aux=%p ref=%u");
    os_trace_module_desc(&g_ble_ll_trace_mod, "13 ll_aux_unref aux=%p ref=%u");
}

void
ble_ll_trace_init(void)
{
    ble_ll_trace_off =
            os_trace_module_register(&g_ble_ll_trace_mod, "ble_ll", 12,
                                     ble_ll_trace_module_send_desc);
}
#endif
#endif
