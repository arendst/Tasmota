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

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "nimble/nimble_npl.h"
#include "freertos/portable.h"

portMUX_TYPE ble_port_mutex = portMUX_INITIALIZER_UNLOCKED;

static inline bool
in_isr(void)
{
    /* XXX hw specific! */
    return xPortInIsrContext() != 0;
}

struct ble_npl_event *
npl_freertos_eventq_get(struct ble_npl_eventq *evq, ble_npl_time_t tmo)
{
    struct ble_npl_event *ev = NULL;
    BaseType_t woken;
    BaseType_t ret;

    if (in_isr()) {
        assert(tmo == 0);
        ret = xQueueReceiveFromISR(evq->q, &ev, &woken);
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
    } else {
        ret = xQueueReceive(evq->q, &ev, tmo);
    }
    assert(ret == pdPASS || ret == errQUEUE_EMPTY);

    if (ev) {
        ev->queued = false;
    }

    return ev;
}

void
npl_freertos_eventq_put(struct ble_npl_eventq *evq, struct ble_npl_event *ev)
{
    BaseType_t woken;
    BaseType_t ret;

    if (ev->queued) {
        return;
    }

    ev->queued = true;

    if (in_isr()) {
        ret = xQueueSendToBackFromISR(evq->q, &ev, &woken);
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
    } else {
        ret = xQueueSendToBack(evq->q, &ev, portMAX_DELAY);
    }

    assert(ret == pdPASS);
}

void
npl_freertos_eventq_remove(struct ble_npl_eventq *evq,
                      struct ble_npl_event *ev)
{
    struct ble_npl_event *tmp_ev;
    BaseType_t ret;
    int i;
    int count;
    BaseType_t woken, woken2;

    if (!ev->queued) {
        return;
    }

    /*
     * XXX We cannot extract element from inside FreeRTOS queue so as a quick
     * workaround we'll just remove all elements and add them back except the
     * one we need to remove. This is silly, but works for now - we probably
     * better use counting semaphore with os_queue to handle this in future.
     */

    if (in_isr()) {
        woken = pdFALSE;

        count = uxQueueMessagesWaitingFromISR(evq->q);
        for (i = 0; i < count; i++) {
            ret = xQueueReceiveFromISR(evq->q, &tmp_ev, &woken2);
            assert(ret == pdPASS);
            woken |= woken2;

            if (tmp_ev == ev) {
                continue;
            }

            ret = xQueueSendToBackFromISR(evq->q, &tmp_ev, &woken2);
            assert(ret == pdPASS);
            woken |= woken2;
        }

        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
    } else {
        portMUX_TYPE ble_npl_mut = portMUX_INITIALIZER_UNLOCKED;
        portENTER_CRITICAL(&ble_npl_mut);

        count = uxQueueMessagesWaiting(evq->q);
        for (i = 0; i < count; i++) {
            ret = xQueueReceive(evq->q, &tmp_ev, 0);
            assert(ret == pdPASS);

            if (tmp_ev == ev) {
                continue;
            }

            ret = xQueueSendToBack(evq->q, &tmp_ev, 0);
            assert(ret == pdPASS);
        }

        portEXIT_CRITICAL(&ble_npl_mut);
    }

    ev->queued = 0;
}

ble_npl_error_t
npl_freertos_mutex_init(struct ble_npl_mutex *mu)
{
    if (!mu) {
        return BLE_NPL_INVALID_PARAM;
    }

    mu->handle = xSemaphoreCreateRecursiveMutex();
    assert(mu->handle);

    return BLE_NPL_OK;
}

ble_npl_error_t
npl_freertos_mutex_deinit(struct ble_npl_mutex *mu)
{
    if (!mu) {
        return BLE_NPL_INVALID_PARAM;
    }

    if (mu->handle) {
        vSemaphoreDelete(mu->handle);
    }

    return BLE_NPL_OK;
}

ble_npl_error_t
npl_freertos_mutex_pend(struct ble_npl_mutex *mu, ble_npl_time_t timeout)
{
    BaseType_t ret;

    if (!mu) {
        return BLE_NPL_INVALID_PARAM;
    }

    assert(mu->handle);

    if (in_isr()) {
        ret = pdFAIL;
        assert(0);
    } else {
        ret = xSemaphoreTakeRecursive(mu->handle, timeout);
    }

    return ret == pdPASS ? BLE_NPL_OK : BLE_NPL_TIMEOUT;
}

ble_npl_error_t
npl_freertos_mutex_release(struct ble_npl_mutex *mu)
{
    if (!mu) {
        return BLE_NPL_INVALID_PARAM;
    }

    assert(mu->handle);

    if (in_isr()) {
        assert(0);
    } else {
        if (xSemaphoreGiveRecursive(mu->handle) != pdPASS) {
            return BLE_NPL_BAD_MUTEX;
        }
    }

    return BLE_NPL_OK;
}

ble_npl_error_t
npl_freertos_sem_init(struct ble_npl_sem *sem, uint16_t tokens)
{
    if (!sem) {
        return BLE_NPL_INVALID_PARAM;
    }

    sem->handle = xSemaphoreCreateCounting(128, tokens);
    assert(sem->handle);

    return BLE_NPL_OK;
}

ble_npl_error_t
npl_freertos_sem_deinit(struct ble_npl_sem *sem)
{
    if (!sem) {
        return BLE_NPL_INVALID_PARAM;
    }

    if (sem->handle) {
        vSemaphoreDelete(sem->handle);
    }

    return BLE_NPL_OK;
}

ble_npl_error_t
npl_freertos_sem_pend(struct ble_npl_sem *sem, ble_npl_time_t timeout)
{
    BaseType_t woken;
    BaseType_t ret;

    if (!sem) {
        return BLE_NPL_INVALID_PARAM;
    }

    assert(sem->handle);

    if (in_isr()) {
        assert(timeout == 0);
        ret = xSemaphoreTakeFromISR(sem->handle, &woken);
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
    } else {
        ret = xSemaphoreTake(sem->handle, timeout);
    }

    return ret == pdPASS ? BLE_NPL_OK : BLE_NPL_TIMEOUT;
}

ble_npl_error_t
npl_freertos_sem_release(struct ble_npl_sem *sem)
{
    BaseType_t ret;
    BaseType_t woken;

    if (!sem) {
        return BLE_NPL_INVALID_PARAM;
    }

    assert(sem->handle);

    if (in_isr()) {
        ret = xSemaphoreGiveFromISR(sem->handle, &woken);
        
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
    } else {
        ret = xSemaphoreGive(sem->handle);
    }

    assert(ret == pdPASS);
    return BLE_NPL_OK;
}

static void
os_callout_timer_cb(TimerHandle_t timer)
{
    struct ble_npl_callout *co;

    co = pvTimerGetTimerID(timer);
    assert(co);

    if (co->evq) {
        ble_npl_eventq_put(co->evq, &co->ev);
    } else {
        co->ev.fn(&co->ev);
    }
}

void
npl_freertos_callout_init(struct ble_npl_callout *co, struct ble_npl_eventq *evq,
                     ble_npl_event_fn *ev_cb, void *ev_arg)
{
    if (co->handle == NULL) {
        co->handle = xTimerCreate("co", 1, pdFALSE, co, os_callout_timer_cb);
    }
    co->evq = evq;
    ble_npl_event_init(&co->ev, ev_cb, ev_arg);
}
void
npl_freertos_callout_deinit(struct ble_npl_callout *co)
{
    if (co->handle) {
        xTimerDelete(co->handle, portMAX_DELAY);
        co->handle = NULL;
    }
}

ble_npl_error_t
npl_freertos_callout_reset(struct ble_npl_callout *co, ble_npl_time_t ticks)
{
    BaseType_t woken1, woken2, woken3;

    if (ticks == 0) {
        ticks = 1;
    }

    if (in_isr()) {
        xTimerStopFromISR(co->handle, &woken1);
        xTimerChangePeriodFromISR(co->handle, ticks, &woken2);
        xTimerResetFromISR(co->handle, &woken3);

        if( woken1 == pdTRUE || woken2 == pdTRUE || woken3 == pdTRUE) {
            portYIELD_FROM_ISR();
        }
    } else {
        xTimerStop(co->handle, portMAX_DELAY);
        xTimerChangePeriod(co->handle, ticks, portMAX_DELAY);
        xTimerReset(co->handle, portMAX_DELAY);
    }

    return BLE_NPL_OK;
}

ble_npl_time_t
npl_freertos_callout_remaining_ticks(struct ble_npl_callout *co,
                                     ble_npl_time_t now)
{
    ble_npl_time_t rt;
    uint32_t exp;

    exp = xTimerGetExpiryTime(co->handle);

    if (exp > now) {
        rt = exp - now;
    } else {
        rt = 0;
    }

    return rt;
}

ble_npl_error_t
npl_freertos_time_ms_to_ticks(uint32_t ms, ble_npl_time_t *out_ticks)
{
    uint64_t ticks;

    ticks = ((uint64_t)ms * configTICK_RATE_HZ) / 1000;
    if (ticks > UINT32_MAX) {
        return BLE_NPL_EINVAL;
    }

    *out_ticks = ticks;

    return 0;
}

ble_npl_error_t
npl_freertos_time_ticks_to_ms(ble_npl_time_t ticks, uint32_t *out_ms)
{
    uint64_t ms;

    ms = ((uint64_t)ticks * 1000) / configTICK_RATE_HZ;
    if (ms > UINT32_MAX) {
        return BLE_NPL_EINVAL;
     }

    *out_ms = ms;

    return 0;
}
