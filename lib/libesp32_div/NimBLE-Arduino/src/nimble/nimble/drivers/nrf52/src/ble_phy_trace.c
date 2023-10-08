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

#if defined(ARDUINO_ARCH_NRF5) && defined(NRF52_SERIES)

#include <stdint.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os_trace_api.h"

#if MYNEWT_VAL(BLE_PHY_SYSVIEW)

static os_trace_module_t g_ble_phy_trace_mod;
uint32_t ble_phy_trace_off;

static void
ble_phy_trace_module_send_desc(void)
{
    os_trace_module_desc(&g_ble_phy_trace_mod, "0 phy_set_tx cputime=%u usecs=%u");
    os_trace_module_desc(&g_ble_phy_trace_mod, "1 phy_set_rx cputime=%u usecs=%u");
    os_trace_module_desc(&g_ble_phy_trace_mod, "2 phy_disable");
}

void
ble_phy_trace_init(void)
{
    ble_phy_trace_off =
            os_trace_module_register(&g_ble_phy_trace_mod, "ble_phy", 3,
                                     ble_phy_trace_module_send_desc);
}
#endif
#endif