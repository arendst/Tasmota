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

#ifndef _NIMBLE_PORT_FREERTOS_H
#define _NIMBLE_PORT_FREERTOS_H

#include "nimble/nimble/include/nimble/nimble_npl.h"
#include "nimble/porting/nimble/include/syscfg/syscfg.h"

#ifdef ESP_PLATFORM
#include "esp_err.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ESP_PLATFORM
/**
 * @brief esp_nimble_enable - Initialize the NimBLE host task
 *
 * @param host_task
 * @return esp_err_t
 */
esp_err_t esp_nimble_enable(void *host_task);

/**
 * @brief esp_nimble_disable - Disable the NimBLE host task
 *
 * @return esp_err_t
 */
esp_err_t esp_nimble_disable(void);
#endif

void nimble_port_freertos_init(TaskFunction_t host_task_fn);
void nimble_port_freertos_deinit(void);

#if CONFIG_NIMBLE_STACK_USE_MEM_POOLS
void npl_freertos_funcs_init(void);
void npl_freertos_funcs_deinit(void);
int npl_freertos_mempool_init(void);
struct npl_funcs_t * npl_freertos_funcs_get(void);
#endif

#ifndef ESP_PLATFORM
UBaseType_t nimble_port_freertos_get_ll_hwm(void);
UBaseType_t nimble_port_freertos_get_hs_hwm(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _NIMBLE_PORT_FREERTOS_H */
