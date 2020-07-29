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
 
/* Modifications copyright (C) 2020 Ryan Powell*/

#ifndef _NIMBLE_NPL_H_
#define _NIMBLE_NPL_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "nimconfig.h"
#ifdef __cplusplus
extern "C" {
#endif

struct ble_npl_event;
typedef void ble_npl_event_fn(struct ble_npl_event *ev);

enum ble_npl_error {
    BLE_NPL_OK = 0,
    BLE_NPL_ENOMEM = 1,
    BLE_NPL_EINVAL = 2,
    BLE_NPL_INVALID_PARAM = 3,
    BLE_NPL_MEM_NOT_ALIGNED = 4,
    BLE_NPL_BAD_MUTEX = 5,
    BLE_NPL_TIMEOUT = 6,
    BLE_NPL_ERR_IN_ISR = 7,
    BLE_NPL_ERR_PRIV = 8,
    BLE_NPL_OS_NOT_STARTED = 9,
    BLE_NPL_ENOENT = 10,
    BLE_NPL_EBUSY = 11,
    BLE_NPL_ERROR = 12,
};

typedef enum ble_npl_error ble_npl_error_t;

/* Include OS-specific definitions */
#include "nimble/nimble_npl_os.h"

/*
 * Generic
 */

bool ble_npl_os_started(void);

void *ble_npl_get_current_task_id(void);

/*
 * Event queue
 */

void ble_npl_eventq_init(struct ble_npl_eventq *evq);

void ble_npl_eventq_deinit(struct ble_npl_eventq *evq);

struct ble_npl_event *ble_npl_eventq_get(struct ble_npl_eventq *evq,
                                         ble_npl_time_t tmo);

void ble_npl_eventq_put(struct ble_npl_eventq *evq, struct ble_npl_event *ev);

void ble_npl_eventq_remove(struct ble_npl_eventq *evq,
                           struct ble_npl_event *ev);

void ble_npl_event_init(struct ble_npl_event *ev, ble_npl_event_fn *fn,
                        void *arg);

bool ble_npl_event_is_queued(struct ble_npl_event *ev);

void *ble_npl_event_get_arg(struct ble_npl_event *ev);

void ble_npl_event_set_arg(struct ble_npl_event *ev, void *arg);

bool ble_npl_eventq_is_empty(struct ble_npl_eventq *evq);

void ble_npl_event_run(struct ble_npl_event *ev);

/*
 * Mutexes
 */

ble_npl_error_t ble_npl_mutex_init(struct ble_npl_mutex *mu);

ble_npl_error_t ble_npl_mutex_pend(struct ble_npl_mutex *mu,
                                   ble_npl_time_t timeout);

ble_npl_error_t ble_npl_mutex_release(struct ble_npl_mutex *mu);

ble_npl_error_t ble_npl_mutex_deinit(struct ble_npl_mutex *mu);

/*
 * Semaphores
 */

ble_npl_error_t ble_npl_sem_init(struct ble_npl_sem *sem, uint16_t tokens);

ble_npl_error_t ble_npl_sem_pend(struct ble_npl_sem *sem,
                                 ble_npl_time_t timeout);

ble_npl_error_t ble_npl_sem_release(struct ble_npl_sem *sem);

ble_npl_error_t ble_npl_sem_deinit(struct ble_npl_sem *sem);

uint16_t ble_npl_sem_get_count(struct ble_npl_sem *sem);

/*
 * Callouts
 */

void ble_npl_callout_init(struct ble_npl_callout *co, struct ble_npl_eventq *evq,
                          ble_npl_event_fn *ev_cb, void *ev_arg);

ble_npl_error_t ble_npl_callout_reset(struct ble_npl_callout *co,
                                      ble_npl_time_t ticks);

void ble_npl_callout_stop(struct ble_npl_callout *co);

bool ble_npl_callout_is_active(struct ble_npl_callout *co);

ble_npl_time_t ble_npl_callout_get_ticks(struct ble_npl_callout *co);

ble_npl_time_t ble_npl_callout_remaining_ticks(struct ble_npl_callout *co,
                                               ble_npl_time_t time);

void ble_npl_callout_set_arg(struct ble_npl_callout *co,
                             void *arg);
/*
 * Time functions
 */

ble_npl_time_t ble_npl_time_get(void);

ble_npl_error_t ble_npl_time_ms_to_ticks(uint32_t ms, ble_npl_time_t *out_ticks);

ble_npl_error_t ble_npl_time_ticks_to_ms(ble_npl_time_t ticks, uint32_t *out_ms);

ble_npl_time_t ble_npl_time_ms_to_ticks32(uint32_t ms);

uint32_t ble_npl_time_ticks_to_ms32(ble_npl_time_t ticks);

void ble_npl_time_delay(ble_npl_time_t ticks);

/*
 * Hardware-specific
 *
 * These symbols should be most likely defined by application since they are
 * specific to hardware, not to OS.
 */

#if NIMBLE_CFG_CONTROLLER

void ble_npl_hw_set_isr(int irqn, uint32_t addr);

#endif

uint32_t ble_npl_hw_enter_critical(void);

void ble_npl_hw_exit_critical(uint32_t ctx);

bool ble_npl_hw_is_in_critical(void);

#ifdef __cplusplus
}
#endif

#endif  /* _NIMBLE_NPL_H_ */
