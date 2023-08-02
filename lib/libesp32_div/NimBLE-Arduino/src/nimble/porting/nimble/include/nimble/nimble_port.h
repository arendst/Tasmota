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

#ifndef _NIMBLE_PORT_H
#define _NIMBLE_PORT_H

#include "nimble/nimble/include/nimble/nimble_npl.h"

#ifdef ESP_PLATFORM
#include "esp_err.h"
#include "nimconfig.h"
#define NIMBLE_CORE (CONFIG_BT_NIMBLE_PINNED_TO_CORE < portNUM_PROCESSORS ? CONFIG_BT_NIMBLE_PINNED_TO_CORE : tskNO_AFFINITY)
#define NIMBLE_HS_STACK_SIZE CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE
#else
#include "../syscfg/syscfg.h"
#define NIMBLE_HS_STACK_SIZE (CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE / 4)
#endif

#if (CONFIG_IDF_TARGET_ESP32H2 || CONFIG_IDF_TARGET_ESP32C2)
#define NIMBLE_LL_STACK_SIZE CONFIG_BT_LE_CONTROLLER_TASK_STACK_SIZE
#endif

#ifdef __cplusplus
extern "C" {
#endif

void nimble_port_init(void);
void nimble_port_deinit(void);

void nimble_port_run(void);
int nimble_port_stop(void);

#ifdef ESP_PLATFORM
/**
 * @brief esp_nimble_init - Initialize the NimBLE host stack
 *
 * @return esp_err_t
 */
esp_err_t esp_nimble_init(void);

/**
 * @brief esp_nimble_deinit - Deinitialize the NimBLE host stack
 *
 * @return esp_err_t
 */
esp_err_t esp_nimble_deinit(void);
#endif // ESP_PLATFORM

struct ble_npl_eventq *nimble_port_get_dflt_eventq(void);

#if NIMBLE_CFG_CONTROLLER
void nimble_port_ll_task_func(void *arg);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NIMBLE_PORT_H */
