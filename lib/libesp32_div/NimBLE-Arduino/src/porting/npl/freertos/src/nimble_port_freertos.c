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

#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nimble/nimble_port.h"

#if NIMBLE_CFG_CONTROLLER
static TaskHandle_t ll_task_h;
#endif
static TaskHandle_t host_task_h;

void
nimble_port_freertos_init(TaskFunction_t host_task_fn)
{
#if NIMBLE_CFG_CONTROLLER
    /*
     * Create task where NimBLE LL will run. This one is required as LL has its
     * own event queue and should have highest priority. The task function is
     * provided by NimBLE and in case of FreeRTOS it does not need to be wrapped
     * since it has compatible prototype.
     */
    xTaskCreate(nimble_port_ll_task_func, "ll", configMINIMAL_STACK_SIZE + 400,
                NULL, configMAX_PRIORITIES - 1, &ll_task_h);
#endif

    /*
     * Create task where NimBLE host will run. It is not strictly necessary to
     * have separate task for NimBLE host, but since something needs to handle
     * default queue it is just easier to make separate task which does this.
     */
    xTaskCreatePinnedToCore(host_task_fn, "ble", NIMBLE_STACK_SIZE,
                NULL, (configMAX_PRIORITIES - 4), &host_task_h, NIMBLE_CORE);
}

void
nimble_port_freertos_deinit(void)
{
    if (host_task_h) {
        vTaskDelete(host_task_h);
    }
}
