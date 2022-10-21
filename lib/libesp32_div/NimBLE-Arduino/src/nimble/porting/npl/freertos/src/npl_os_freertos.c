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

#include "nimble/porting/nimble/include/syscfg/syscfg.h"

#if CONFIG_NIMBLE_STACK_USE_MEM_POOLS

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "nimble/console/console.h"

#include "nimble/nimble/include/nimble/nimble_npl.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/portable.h"
#include "../include/nimble/npl_freertos.h"

#include "nimble/porting/nimble/include/os/os_mempool.h"

#ifdef ESP_PLATFORM
#include "esp_log.h"

#include "soc/soc_caps.h"

portMUX_TYPE ble_port_mutex = portMUX_INITIALIZER_UNLOCKED;

#else
#include "nrf.h"

static void *radio_isr_addr;
static void *rng_isr_addr;
static void *rtc0_isr_addr;
#endif

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
static const char *TAG = "Timer";
#endif

#define OS_MEM_ALLOC (1)

#if CONFIG_BT_NIMBLE_ENABLED
#define BT_LE_HCI_EVT_HI_BUF_COUNT MYNEWT_VAL(BLE_HCI_EVT_HI_BUF_COUNT)
#define BT_LE_HCI_EVT_LO_BUF_COUNT MYNEWT_VAL(BLE_HCI_EVT_LO_BUF_COUNT)
#define BT_LE_MAX_EXT_ADV_INSTANCES MYNEWT_VAL(BLE_MULTI_ADV_INSTANCES)
#define BT_LE_MAX_CONNECTIONS MYNEWT_VAL(BLE_MAX_CONNECTIONS)
#else
#include "esp_bt.h"
#define BT_LE_HCI_EVT_HI_BUF_COUNT DEFAULT_BT_LE_HCI_EVT_HI_BUF_COUNT
#define BT_LE_HCI_EVT_LO_BUF_COUNT DEFAULT_BT_LE_HCI_EVT_LO_BUF_COUNT
#define BT_LE_MAX_EXT_ADV_INSTANCES DEFAULT_BT_LE_MAX_EXT_ADV_INSTANCES
#define BT_LE_MAX_CONNECTIONS DEFAULT_BT_LE_MAX_CONNECTIONS
#endif

#define BLE_HS_HCI_EVT_COUNT                    \
    (BT_LE_HCI_EVT_HI_BUF_COUNT +        \
     BT_LE_HCI_EVT_LO_BUF_COUNT)


#define LL_NPL_BASE_EVENT_COUNT     (11)
#define LL_SCAN_EXT_AUX_EVT_CNT     (MYNEWT_VAL(BLE_LL_EXT_ADV_AUX_PTR_CNT))
#define HCI_LL_NPL_EVENT_COUNT      (1)
#define ADV_LL_NPL_EVENT_COUNT      ((BT_LE_MAX_EXT_ADV_INSTANCES+1)*3)
#define SCAN_LL_NPL_EVENT_COUNT     (2)
#define RL_LL_NPL_EVENT_COUNT       (1)
#define SYNC_LL_NPL_EVENT_COUNT     (7)

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_CTRL_TO_HOST_FLOW_CONTROL)
#define LL_CTRL_TO_HOST_FLOW_CTRL_EVT   (1)
#else
#define LL_CTRL_TO_HOST_FLOW_CTRL_EVT   (0)
#endif

#if MYNEWT_VAL(BLE_LL_CFG_FEAT_LE_PING)
#define LL_CFG_FEAT_LE_PING_EVT   (1)
#else
#define LL_CFG_FEAT_LE_PING_EVT   (0)
#endif

#define CONN_MODULE_NPL_EVENT_COUNT (((LL_CFG_FEAT_LE_PING_EVT+2)*BT_LE_MAX_CONNECTIONS)+LL_CTRL_TO_HOST_FLOW_CTRL_EVT)


#define BLE_LL_EV_COUNT (LL_NPL_BASE_EVENT_COUNT +      \
                         LL_SCAN_EXT_AUX_EVT_CNT +      \
                         HCI_LL_NPL_EVENT_COUNT +       \
                         ADV_LL_NPL_EVENT_COUNT +       \
                         SCAN_LL_NPL_EVENT_COUNT +      \
                         RL_LL_NPL_EVENT_COUNT +        \
                         SYNC_LL_NPL_EVENT_COUNT +      \
                         CONN_MODULE_NPL_EVENT_COUNT)

#define BLE_TOTAL_EV_COUNT (BLE_LL_EV_COUNT + BLE_HS_HCI_EVT_COUNT)

#define BLE_TOTAL_EVQ_COUNT (10)

#define BLE_TOTAL_CO_COUNT (40)

#define BLE_TOTAL_SEM_COUNT (10)

#define BLE_TOTAL_MUTEX_COUNT (10)

#if SOC_ESP_NIMBLE_CONTROLLER

struct os_mempool ble_freertos_ev_pool;
static os_membuf_t *ble_freertos_ev_buf = NULL;

struct os_mempool ble_freertos_evq_pool;
static os_membuf_t *ble_freertos_evq_buf = NULL;

struct os_mempool ble_freertos_co_pool;
static os_membuf_t *ble_freertos_co_buf = NULL;

struct os_mempool ble_freertos_sem_pool;
static os_membuf_t *ble_freertos_sem_buf = NULL;

struct os_mempool ble_freertos_mutex_pool;
static os_membuf_t *ble_freertos_mutex_buf = NULL;

#else

struct os_mempool ble_freertos_ev_pool;
static os_membuf_t ble_freertos_ev_buf[
    OS_MEMPOOL_SIZE(BLE_TOTAL_EV_COUNT, sizeof (struct ble_npl_event_freertos))
];

struct os_mempool ble_freertos_evq_pool;
static os_membuf_t ble_freertos_evq_buf[
    OS_MEMPOOL_SIZE(BLE_TOTAL_EVQ_COUNT, sizeof (struct ble_npl_eventq_freertos))
];

struct os_mempool ble_freertos_co_pool;
static os_membuf_t ble_freertos_co_buf[
    OS_MEMPOOL_SIZE(BLE_TOTAL_CO_COUNT, sizeof (struct ble_npl_callout_freertos))
];

struct os_mempool ble_freertos_sem_pool;
static os_membuf_t ble_freertos_sem_buf[
    OS_MEMPOOL_SIZE(BLE_TOTAL_SEM_COUNT, sizeof (struct ble_npl_sem_freertos))
];

struct os_mempool ble_freertos_mutex_pool;
static os_membuf_t ble_freertos_mutex_buf[
    OS_MEMPOOL_SIZE(BLE_TOTAL_MUTEX_COUNT, sizeof (struct ble_npl_mutex_freertos))
];

#endif

bool
IRAM_ATTR npl_freertos_os_started(void)
{
    return xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED;
}

void *
IRAM_ATTR npl_freertos_get_current_task_id(void)
{
    return xTaskGetCurrentTaskHandle();
}

void
IRAM_ATTR npl_freertos_event_init(struct ble_npl_event *ev, ble_npl_event_fn *fn,
                    void *arg)
{
    struct ble_npl_event_freertos *event = NULL;
#if OS_MEM_ALLOC
    if (!os_memblock_from(&ble_freertos_ev_pool,ev->event)) {
        ev->event = os_memblock_get(&ble_freertos_ev_pool);
    }
#else
    if(!ev->event) {
        ev->event = malloc(sizeof(struct ble_npl_event_freertos));
    }
#endif
    event = (struct ble_npl_event_freertos *)ev->event;
    PLATFORM_BLE_LL_ASSERT(event);

    memset(event, 0, sizeof(*event));
    event->fn = fn;
    event->arg = arg;
}

void
IRAM_ATTR npl_freertos_event_deinit(struct ble_npl_event *ev)
{
    PLATFORM_BLE_LL_ASSERT(ev->event);
#if OS_MEM_ALLOC
    os_memblock_put(&ble_freertos_ev_pool,ev->event);
#else
    free(ev->event);
#endif
    ev->event = NULL;
}

void
IRAM_ATTR npl_freertos_event_reset(struct ble_npl_event *ev)
{
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
    PLATFORM_BLE_LL_ASSERT(event);
    event->queued = 0;
}

void
IRAM_ATTR npl_freertos_eventq_init(struct ble_npl_eventq *evq)
{
    struct ble_npl_eventq_freertos *eventq = NULL;
#if OS_MEM_ALLOC
    if (!os_memblock_from(&ble_freertos_evq_pool,evq->eventq)) {
        evq->eventq = os_memblock_get(&ble_freertos_evq_pool);
        eventq = (struct ble_npl_eventq_freertos*)evq->eventq;
        PLATFORM_BLE_LL_ASSERT(eventq);

        memset(eventq, 0, sizeof(*eventq));
        eventq->q = xQueueCreate(BLE_TOTAL_EV_COUNT, sizeof(struct ble_npl_eventq *));
        PLATFORM_BLE_LL_ASSERT(eventq->q);
    }
#else
    if(!evq->eventq) {
        evq->eventq = malloc(sizeof(struct ble_npl_eventq_freertos));
        eventq = (struct ble_npl_eventq_freertos*)evq->eventq;
        PLATFORM_BLE_LL_ASSERT(eventq);

        memset(eventq, 0, sizeof(*eventq));
        eventq->q = xQueueCreate(BLE_TOTAL_EV_COUNT, sizeof(struct ble_npl_eventq *));
        PLATFORM_BLE_LL_ASSERT(eventq->q);
    }
#endif
}

void
IRAM_ATTR npl_freertos_eventq_deinit(struct ble_npl_eventq *evq)
{
    struct ble_npl_eventq_freertos *eventq = (struct ble_npl_eventq_freertos *)evq->eventq;

    PLATFORM_BLE_LL_ASSERT(eventq);
    vQueueDelete(eventq->q);
#if OS_MEM_ALLOC
    os_memblock_put(&ble_freertos_evq_pool,eventq);
#else
    free((void *)eventq);
#endif
    evq->eventq = NULL;
}

void
IRAM_ATTR npl_freertos_callout_mem_reset(struct ble_npl_callout *co)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;

    PLATFORM_BLE_LL_ASSERT(callout);
    PLATFORM_BLE_LL_ASSERT(callout->handle);

    ble_npl_event_reset(&callout->ev);
}

#ifdef ESP_PLATFORM
static inline bool
IRAM_ATTR in_isr(void)
{
    /* XXX hw specific! */
    return xPortInIsrContext() != 0;
}

#else
static inline bool
in_isr(void)
{
    /* XXX hw specific! */
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

void
RADIO_IRQHandler(void)
{
    ((void (*)(void))radio_isr_addr)();
}

void
RNG_IRQHandler(void)
{
    ((void (*)(void))rng_isr_addr)();
}

void
RTC0_IRQHandler(void)
{
    ((void (*)(void))rtc0_isr_addr)();
}

/* This is called by NimBLE radio driver to set interrupt handlers */
void
npl_freertos_hw_set_isr(int irqn, void (*addr)(void))
{
    switch (irqn) {
    case RADIO_IRQn:
        radio_isr_addr = addr;
        break;
    case RNG_IRQn:
        rng_isr_addr = addr;
        break;
    case RTC0_IRQn:
        rtc0_isr_addr = addr;
        break;
    }
}
#endif

struct ble_npl_event *
IRAM_ATTR npl_freertos_eventq_get(struct ble_npl_eventq *evq, ble_npl_time_t tmo)
{
    struct ble_npl_event *ev = NULL;
    struct ble_npl_eventq_freertos *eventq = (struct ble_npl_eventq_freertos *)evq->eventq;
    BaseType_t woken;
    BaseType_t ret;

    if (in_isr()) {
        PLATFORM_BLE_LL_ASSERT(tmo == 0);
        ret = xQueueReceiveFromISR(eventq->q, &ev, &woken);
#ifdef ESP_PLATFORM

        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
        ret = xQueueReceive(eventq->q, &ev, tmo);
    }
    PLATFORM_BLE_LL_ASSERT(ret == pdPASS || ret == errQUEUE_EMPTY);

    if (ev) {
	struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
	if (event) {
            event->queued = false;
	}
    }

    return ev;
}

void
IRAM_ATTR npl_freertos_eventq_put(struct ble_npl_eventq *evq, struct ble_npl_event *ev)
{
    BaseType_t woken;
    BaseType_t ret;
    struct ble_npl_eventq_freertos *eventq = (struct ble_npl_eventq_freertos *)evq->eventq;
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;

    if (event->queued) {
        return;
    }

    event->queued = true;

    if (in_isr()) {
        ret = xQueueSendToBackFromISR(eventq->q, &ev, &woken);
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
        ret = xQueueSendToBack(eventq->q, &ev, portMAX_DELAY);
    }

    PLATFORM_BLE_LL_ASSERT(ret == pdPASS);
}

void
IRAM_ATTR npl_freertos_eventq_remove(struct ble_npl_eventq *evq,
                       struct ble_npl_event *ev)
{
    struct ble_npl_event *tmp_ev;
    BaseType_t ret;
    int i;
    int count;
    BaseType_t woken, woken2;
    struct ble_npl_eventq_freertos *eventq = (struct ble_npl_eventq_freertos *)evq->eventq;
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;

    if (!event->queued) {
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

        count = uxQueueMessagesWaitingFromISR(eventq->q);
        for (i = 0; i < count; i++) {
            ret = xQueueReceiveFromISR(eventq->q, &tmp_ev, &woken2);
            PLATFORM_BLE_LL_ASSERT(ret == pdPASS);
            woken |= woken2;

            if (tmp_ev == ev) {
                continue;
            }

            ret = xQueueSendToBackFromISR(eventq->q, &tmp_ev, &woken2);
            PLATFORM_BLE_LL_ASSERT(ret == pdPASS);
            woken |= woken2;
        }

#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
#ifdef ESP_PLATFORM
        portMUX_TYPE ble_npl_mut = portMUX_INITIALIZER_UNLOCKED;
        portENTER_CRITICAL(&ble_npl_mut);

        count = uxQueueMessagesWaiting(eventq->q);
#else
        vPortEnterCritical();
        count = uxQueueMessagesWaiting(eventq->q);
#endif
        for (i = 0; i < count; i++) {
            ret = xQueueReceive(eventq->q, &tmp_ev, 0);
            PLATFORM_BLE_LL_ASSERT(ret == pdPASS);

            if (tmp_ev == ev) {
                continue;
            }

            ret = xQueueSendToBack(eventq->q, &tmp_ev, 0);
            PLATFORM_BLE_LL_ASSERT(ret == pdPASS);
        }
#ifdef ESP_PLATFORM
        portEXIT_CRITICAL(&ble_npl_mut);
#else
        vPortExitCritical();
#endif
    }

    event->queued = 0;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_mutex_init(struct ble_npl_mutex *mu)
{
    struct ble_npl_mutex_freertos *mutex = NULL;
#if OS_MEM_ALLOC
    if (!os_memblock_from(&ble_freertos_mutex_pool,mu->mutex)) {
        mu->mutex = os_memblock_get(&ble_freertos_mutex_pool);
        mutex = (struct ble_npl_mutex_freertos *)mu->mutex;

        if (!mutex) {
            return BLE_NPL_INVALID_PARAM;
        }

        memset(mutex, 0, sizeof(*mutex));
        mutex->handle = xSemaphoreCreateRecursiveMutex();
        PLATFORM_BLE_LL_ASSERT(mutex->handle);
    }
#else
    if(!mu->mutex) {
        mu->mutex = malloc(sizeof(struct ble_npl_mutex_freertos));
        mutex = (struct ble_npl_mutex_freertos *)mu->mutex;

        if (!mutex) {
            return BLE_NPL_INVALID_PARAM;
        }

        memset(mutex, 0, sizeof(*mutex));
        mutex->handle = xSemaphoreCreateRecursiveMutex();
        PLATFORM_BLE_LL_ASSERT(mutex->handle);
    }
#endif

    return BLE_NPL_OK;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_mutex_deinit(struct ble_npl_mutex *mu)
{
    struct ble_npl_mutex_freertos *mutex = (struct ble_npl_mutex_freertos *)mu->mutex;

    if (!mutex) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(mutex->handle);
    vSemaphoreDelete(mutex->handle);

#if OS_MEM_ALLOC
    os_memblock_put(&ble_freertos_mutex_pool,mutex);
#else
    free((void *)mutex);
#endif
    mu->mutex = NULL;

    return BLE_NPL_OK;
}

void
IRAM_ATTR npl_freertos_event_run(struct ble_npl_event *ev)
{
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
    event->fn(ev);
}

bool
IRAM_ATTR npl_freertos_eventq_is_empty(struct ble_npl_eventq *evq)
{
    struct ble_npl_eventq_freertos *eventq = (struct ble_npl_eventq_freertos *)evq->eventq;
    return xQueueIsQueueEmptyFromISR(eventq->q);
}

bool
IRAM_ATTR npl_freertos_event_is_queued(struct ble_npl_event *ev)
{
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
    return event->queued;
}

void *
IRAM_ATTR npl_freertos_event_get_arg(struct ble_npl_event *ev)
{
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
    return event->arg;
}

void
IRAM_ATTR npl_freertos_event_set_arg(struct ble_npl_event *ev, void *arg)
{
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)ev->event;
    event->arg = arg;
}


ble_npl_error_t
IRAM_ATTR npl_freertos_mutex_pend(struct ble_npl_mutex *mu, ble_npl_time_t timeout)
{
    BaseType_t ret;
    struct ble_npl_mutex_freertos *mutex = (struct ble_npl_mutex_freertos *)mu->mutex;

    if (!mutex) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(mutex->handle);

    if (in_isr()) {
        ret = pdFAIL;
        PLATFORM_BLE_LL_ASSERT(0);
    } else {
        ret = xSemaphoreTakeRecursive(mutex->handle, timeout);
    }

    return ret == pdPASS ? BLE_NPL_OK : BLE_NPL_TIMEOUT;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_mutex_release(struct ble_npl_mutex *mu)
{
    struct ble_npl_mutex_freertos *mutex = (struct ble_npl_mutex_freertos *)mu->mutex;

    if (!mutex) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(mutex->handle);

    if (in_isr()) {
        PLATFORM_BLE_LL_ASSERT(0);
    } else {
        if (xSemaphoreGiveRecursive(mutex->handle) != pdPASS) {
            return BLE_NPL_BAD_MUTEX;
        }
    }

    return BLE_NPL_OK;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_sem_init(struct ble_npl_sem *sem, uint16_t tokens)
{
    struct ble_npl_sem_freertos *semaphor = NULL;
#if OS_MEM_ALLOC
    if (!os_memblock_from(&ble_freertos_sem_pool,sem->sem)) {
        sem->sem = os_memblock_get(&ble_freertos_sem_pool);
        semaphor = (struct ble_npl_sem_freertos *)sem->sem;

        if (!semaphor) {
            return BLE_NPL_INVALID_PARAM;
        }

        memset(semaphor, 0, sizeof(*semaphor));
        semaphor->handle = xSemaphoreCreateCounting(128, tokens);
        PLATFORM_BLE_LL_ASSERT(semaphor->handle);
    }
#else
    if(!sem->sem) {
        sem->sem = malloc(sizeof(struct ble_npl_sem_freertos));
        semaphor = (struct ble_npl_sem_freertos *)sem->sem;

        if (!semaphor) {
            return BLE_NPL_INVALID_PARAM;
        }

        memset(semaphor, 0, sizeof(*semaphor));
        semaphor->handle = xSemaphoreCreateCounting(128, tokens);
        PLATFORM_BLE_LL_ASSERT(semaphor->handle);
    }
#endif

    return BLE_NPL_OK;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_sem_deinit(struct ble_npl_sem *sem)
{
    struct ble_npl_sem_freertos *semaphor = (struct ble_npl_sem_freertos *)sem->sem;

    if (!semaphor) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(semaphor->handle);
    vSemaphoreDelete(semaphor->handle);

#if OS_MEM_ALLOC
    os_memblock_put(&ble_freertos_sem_pool,semaphor);
#else
    free((void *)semaphor);
#endif
    sem->sem = NULL;

    return BLE_NPL_OK;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_sem_pend(struct ble_npl_sem *sem, ble_npl_time_t timeout)
{
    BaseType_t woken;
    BaseType_t ret;
    struct ble_npl_sem_freertos *semaphor = (struct ble_npl_sem_freertos *)sem->sem;

    if (!semaphor) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(semaphor->handle);

    if (in_isr()) {
        PLATFORM_BLE_LL_ASSERT(timeout == 0);
        ret = xSemaphoreTakeFromISR(semaphor->handle, &woken);
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
        ret = xSemaphoreTake(semaphor->handle, timeout);
    }

    return ret == pdPASS ? BLE_NPL_OK : BLE_NPL_TIMEOUT;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_sem_release(struct ble_npl_sem *sem)
{
    BaseType_t ret;
    BaseType_t woken;
    struct ble_npl_sem_freertos *semaphor = (struct ble_npl_sem_freertos *)sem->sem;

    if (!semaphor) {
        return BLE_NPL_INVALID_PARAM;
    }

    PLATFORM_BLE_LL_ASSERT(semaphor->handle);

    if (in_isr()) {
        ret = xSemaphoreGiveFromISR(semaphor->handle, &woken);
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
        ret = xSemaphoreGive(semaphor->handle);
    }

    PLATFORM_BLE_LL_ASSERT(ret == pdPASS);
    return BLE_NPL_OK;
}

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
static void
IRAM_ATTR ble_npl_event_fn_wrapper(void *arg)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)arg;
    PLATFORM_BLE_LL_ASSERT(callout);

    if (callout->evq) {
        ble_npl_eventq_put(callout->evq, &callout->ev);
    } else {
        struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)callout->ev.event;
        event->fn(&callout->ev);
    }
}

static
IRAM_ATTR ble_npl_error_t esp_err_to_npl_error(esp_err_t err)
{
    switch(err) {
    case ESP_ERR_INVALID_ARG:
        return BLE_NPL_INVALID_PARAM;

    case ESP_ERR_INVALID_STATE:
        return BLE_NPL_EINVAL;

    case ESP_OK:
        return BLE_NPL_OK;

   default:
        return BLE_NPL_ERROR;
    }
}
#else

static void
IRAM_ATTR os_callout_timer_cb(TimerHandle_t timer)
{
    struct ble_npl_callout_freertos *callout;

    callout = pvTimerGetTimerID(timer);
    PLATFORM_BLE_LL_ASSERT(callout);

    if (callout->evq) {
        ble_npl_eventq_put(callout->evq, &callout->ev);
    } else {
        struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)callout->ev.event;
        event->fn(&callout->ev);
    }
}
#endif

void
IRAM_ATTR npl_freertos_callout_init(struct ble_npl_callout *co, struct ble_npl_eventq *evq,
                      ble_npl_event_fn *ev_cb, void *ev_arg)
{
    struct ble_npl_callout_freertos *callout = NULL;

#if OS_MEM_ALLOC
    if (!os_memblock_from(&ble_freertos_co_pool, co->co)) {
        co->co = os_memblock_get(&ble_freertos_co_pool);
        callout = (struct ble_npl_callout_freertos *)co->co;
        PLATFORM_BLE_LL_ASSERT(callout);

        memset(callout, 0, sizeof(*callout));
        ble_npl_event_init(&callout->ev, ev_cb, ev_arg);

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
	callout->evq = evq;

	esp_timer_create_args_t create_args = {
		.callback = ble_npl_event_fn_wrapper,
		.arg = callout,
		.name = "nimble_timer"
	};

	ESP_ERROR_CHECK(esp_timer_create(&create_args, &callout->handle));

#else
	callout->handle = xTimerCreate("co", 1, pdFALSE, callout, os_callout_timer_cb);
#endif

	PLATFORM_BLE_LL_ASSERT(callout->handle);
    } else {
	callout = (struct ble_npl_callout_freertos *)co->co;
	PLATFORM_BLE_LL_ASSERT(callout);
	callout->evq = evq;
	ble_npl_event_init(&callout->ev, ev_cb, ev_arg);
    }
#else

    if(!co->co) {
        co->co = malloc(sizeof(struct ble_npl_callout_freertos));
        callout = (struct ble_npl_callout_freertos *)co->co;
        PLATFORM_BLE_LL_ASSERT(callout);

	memset(callout, 0, sizeof(*callout));
        ble_npl_event_init(&callout->ev, ev_cb, ev_arg);

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
	callout->evq = evq;

	esp_timer_create_args_t create_args = {
		.callback = ble_npl_event_fn_wrapper,
		.arg = callout,
		.name = "nimble_timer"
	};

	ESP_ERROR_CHECK(esp_timer_create(&create_args, &callout->handle));
#else
	callout->handle = xTimerCreate("co", 1, pdFALSE, callout, os_callout_timer_cb);
#endif

	PLATFORM_BLE_LL_ASSERT(callout->handle);
    }
    else {
        callout = (struct ble_npl_callout_freertos *)co->co;
        PLATFORM_BLE_LL_ASSERT(callout);
	callout->evq = evq;
	ble_npl_event_init(&callout->ev, ev_cb, ev_arg);
    }
#endif

}

void
IRAM_ATTR npl_freertos_callout_deinit(struct ble_npl_callout *co)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;

    /* Since we dynamically deinit timers, function can be called for NULL timers. Return for such scenarios */
    if (!callout) {
	return;
    }

    PLATFORM_BLE_LL_ASSERT(callout->handle);

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    if(esp_timer_stop(callout->handle))
        ESP_LOGW(TAG, "Timer not stopped");

    if(esp_timer_delete(callout->handle))
        ESP_LOGW(TAG, "Timer not deleted");

#else

    xTimerDelete(callout->handle, portMAX_DELAY);
    ble_npl_event_deinit(&callout->ev);

#if OS_MEM_ALLOC
    os_memblock_put(&ble_freertos_co_pool,callout);
#else
    free((void *)callout);
#endif

#endif
    co->co = NULL;
    memset(co, 0, sizeof(struct ble_npl_callout));
}

uint16_t
IRAM_ATTR npl_freertos_sem_get_count(struct ble_npl_sem *sem)
{
    struct ble_npl_sem_freertos *semaphor = (struct ble_npl_sem_freertos *)sem->sem;
    return uxSemaphoreGetCount(semaphor->handle);
}


ble_npl_error_t
IRAM_ATTR npl_freertos_callout_reset(struct ble_npl_callout *co, ble_npl_time_t ticks)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    esp_timer_stop(callout->handle);

    return esp_err_to_npl_error(esp_timer_start_once(callout->handle, ticks*1000));
#else

    BaseType_t woken1, woken2, woken3;

    if (ticks == 0) {
        ticks = 1;
    }
    if (in_isr()) {
        xTimerStopFromISR(callout->handle, &woken1);
        xTimerChangePeriodFromISR(callout->handle, ticks, &woken2);
        xTimerResetFromISR(callout->handle, &woken3);
#ifdef ESP_PLATFORM
        if( woken1 == pdTRUE || woken2 == pdTRUE || woken3 == pdTRUE) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken1 || woken2 || woken3);
#endif
    } else {
        xTimerStop(callout->handle, portMAX_DELAY);
        xTimerChangePeriod(callout->handle, ticks, portMAX_DELAY);
        xTimerReset(callout->handle, portMAX_DELAY);
    }

    return BLE_NPL_OK;
#endif
}

void
IRAM_ATTR npl_freertos_callout_stop(struct ble_npl_callout *co)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;

    if (!callout) {
	return;
    }

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    esp_timer_stop(callout->handle);
#else
    xTimerStop(callout->handle, portMAX_DELAY);
#endif
}

bool
IRAM_ATTR npl_freertos_callout_is_active(struct ble_npl_callout *co)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    return esp_timer_is_active(callout->handle);
#else
    /* Workaround for bug in xTimerIsTimerActive with FreeRTOS V10.2.0, fixed in V10.4.4
     * See: https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/305
     * Sometimes xTimerIsTimerActive returns pdTRUE even though the timer has expired, so we double check.
     */
    return xTimerIsTimerActive(callout->handle) == pdTRUE &&
           xTimerGetExpiryTime(callout->handle) > xTaskGetTickCountFromISR();
#endif
}

ble_npl_time_t
IRAM_ATTR npl_freertos_callout_get_ticks(struct ble_npl_callout *co)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
   /* Currently, esp_timer does not support an API which gets the expiry time for
    * current timer.
    * Returning 0 from here should not cause any effect.
    * Drawback of this approach is that existing code to reset timer would be called
    * more often (since the if condition to invoke reset timer would always succeed if
    * timer is active).
    */

    return 0;
#else
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;
    return xTimerGetExpiryTime(callout->handle);
#endif
}

ble_npl_time_t
IRAM_ATTR npl_freertos_callout_remaining_ticks(struct ble_npl_callout *co,
                                     ble_npl_time_t now)
{
    ble_npl_time_t rt;
    uint32_t exp = 0;

    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    uint64_t expiry = 0;
    esp_err_t err;

    //Fetch expiry time in microseconds
    err = esp_timer_get_expiry_time((esp_timer_handle_t)(callout->handle), &expiry);
    if (err != ESP_OK) {
        //Error. Could not fetch the expiry time
        return 0;
    }

    //Convert microseconds to ticks
    npl_freertos_time_ms_to_ticks((uint32_t)(expiry / 1000), &exp);
#else
    //esp_timer_get_expiry_time() is only available from IDF 5.0 onwards
    //Set expiry to 0
    exp = 0;
#endif //ESP_IDF_VERSION
#else
    exp = xTimerGetExpiryTime(callout->handle);
#endif

    if (exp > now) {
        rt = exp - now;
    } else {
        rt = 0;
    }

    return rt;
}

void
IRAM_ATTR npl_freertos_callout_set_arg(struct ble_npl_callout *co, void *arg)
{
    struct ble_npl_callout_freertos *callout = (struct ble_npl_callout_freertos *)co->co;
    struct ble_npl_event_freertos *event = (struct ble_npl_event_freertos *)callout->ev.event;
    event->arg = arg;
}

uint32_t
IRAM_ATTR npl_freertos_time_get(void)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    return esp_timer_get_time() / 1000;
#else
    return xTaskGetTickCountFromISR();
#endif
}

ble_npl_error_t
IRAM_ATTR npl_freertos_time_ms_to_ticks(uint32_t ms, ble_npl_time_t *out_ticks)
{
    uint64_t ticks;
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    ticks = (uint64_t)ms;
#else
    ticks = ((uint64_t)ms * configTICK_RATE_HZ) / 1000;
#endif
    if (ticks > UINT32_MAX) {
        return BLE_NPL_EINVAL;
    }

    *out_ticks = ticks;

    return 0;
}

ble_npl_error_t
IRAM_ATTR npl_freertos_time_ticks_to_ms(ble_npl_time_t ticks, uint32_t *out_ms)
{
    uint64_t ms;
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    ms = ((uint64_t)ticks);
#else
    ms = ((uint64_t)ticks * 1000) / configTICK_RATE_HZ;
#endif
    if (ms > UINT32_MAX) {
        return BLE_NPL_EINVAL;
     }

    *out_ms = ms;

    return 0;
}

ble_npl_time_t
IRAM_ATTR npl_freertos_time_ms_to_ticks32(uint32_t ms)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    return ms;
#else
    return ms * configTICK_RATE_HZ / 1000;
#endif
}

uint32_t
IRAM_ATTR npl_freertos_time_ticks_to_ms32(ble_npl_time_t ticks)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    return ticks;
#else
    return ticks * 1000 / configTICK_RATE_HZ;
#endif
}

void
IRAM_ATTR npl_freertos_time_delay(ble_npl_time_t ticks)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    vTaskDelay(ticks / portTICK_PERIOD_MS);
#else
    vTaskDelay(ticks);
#endif
}

#ifdef ESP_PLATFORM
#if NIMBLE_CFG_CONTROLLER || CONFIG_NIMBLE_CONTROLLER_MODE
void
IRAM_ATTR npl_freertos_hw_set_isr(int irqn, uint32_t addr)
{
    //Do nothing
}
#endif
#endif


uint8_t hw_critical_state_status = 0;

uint32_t
IRAM_ATTR npl_freertos_hw_enter_critical(void)
{
    ++hw_critical_state_status;
#ifdef ESP_PLATFORM
    portENTER_CRITICAL(&ble_port_mutex);
#else
    portENTER_CRITICAL();
#endif
    return 0;
}

uint8_t
IRAM_ATTR npl_freertos_hw_is_in_critical(void)
{
    return hw_critical_state_status;
}

void
IRAM_ATTR npl_freertos_hw_exit_critical(uint32_t ctx)
{
    --hw_critical_state_status;
#ifdef ESP_PLATFORM
    portEXIT_CRITICAL(&ble_port_mutex);
#else
    portEXIT_CRITICAL();
#endif


}

uint32_t
IRAM_ATTR npl_freertos_get_time_forever(void)
{
    return portMAX_DELAY;
}

const struct npl_funcs_t npl_funcs_ro = {
    .p_ble_npl_os_started = npl_freertos_os_started,
    .p_ble_npl_get_current_task_id = npl_freertos_get_current_task_id,
    .p_ble_npl_eventq_init = npl_freertos_eventq_init,
    .p_ble_npl_eventq_deinit = npl_freertos_eventq_deinit,
    .p_ble_npl_eventq_get = npl_freertos_eventq_get,
    .p_ble_npl_eventq_put = npl_freertos_eventq_put,
    .p_ble_npl_eventq_remove = npl_freertos_eventq_remove,
    .p_ble_npl_event_run = npl_freertos_event_run,
    .p_ble_npl_eventq_is_empty = npl_freertos_eventq_is_empty,
    .p_ble_npl_event_init = npl_freertos_event_init,
    .p_ble_npl_event_deinit = npl_freertos_event_deinit,
    .p_ble_npl_event_reset = npl_freertos_event_reset,
    .p_ble_npl_event_is_queued = npl_freertos_event_is_queued,
    .p_ble_npl_event_get_arg = npl_freertos_event_get_arg,
    .p_ble_npl_event_set_arg = npl_freertos_event_set_arg,
    .p_ble_npl_mutex_init = npl_freertos_mutex_init,
    .p_ble_npl_mutex_deinit = npl_freertos_mutex_deinit,
    .p_ble_npl_mutex_pend = npl_freertos_mutex_pend,
    .p_ble_npl_mutex_release = npl_freertos_mutex_release,
    .p_ble_npl_sem_init = npl_freertos_sem_init,
    .p_ble_npl_sem_deinit = npl_freertos_sem_deinit,
    .p_ble_npl_sem_pend = npl_freertos_sem_pend,
    .p_ble_npl_sem_release = npl_freertos_sem_release,
    .p_ble_npl_sem_get_count = npl_freertos_sem_get_count,
    .p_ble_npl_callout_init = npl_freertos_callout_init,
    .p_ble_npl_callout_reset = npl_freertos_callout_reset,
    .p_ble_npl_callout_stop = npl_freertos_callout_stop,
    .p_ble_npl_callout_deinit = npl_freertos_callout_deinit,
    .p_ble_npl_callout_mem_reset = npl_freertos_callout_mem_reset,
    .p_ble_npl_callout_is_active = npl_freertos_callout_is_active,
    .p_ble_npl_callout_get_ticks = npl_freertos_callout_get_ticks,
    .p_ble_npl_callout_remaining_ticks = npl_freertos_callout_remaining_ticks,
    .p_ble_npl_callout_set_arg = npl_freertos_callout_set_arg,
    .p_ble_npl_time_get = npl_freertos_time_get,
    .p_ble_npl_time_ms_to_ticks = npl_freertos_time_ms_to_ticks,
    .p_ble_npl_time_ticks_to_ms = npl_freertos_time_ticks_to_ms,
    .p_ble_npl_time_ms_to_ticks32 = npl_freertos_time_ms_to_ticks32,
    .p_ble_npl_time_ticks_to_ms32 = npl_freertos_time_ticks_to_ms32,
    .p_ble_npl_time_delay = npl_freertos_time_delay,
#if NIMBLE_CFG_CONTROLLER || CONFIG_NIMBLE_CONTROLLER_MODE
    .p_ble_npl_hw_set_isr = npl_freertos_hw_set_isr,
#endif
    .p_ble_npl_hw_enter_critical = npl_freertos_hw_enter_critical,
    .p_ble_npl_hw_exit_critical = npl_freertos_hw_exit_critical,
    .p_ble_npl_get_time_forever = npl_freertos_get_time_forever,
    .p_ble_npl_hw_is_in_critical = npl_freertos_hw_is_in_critical
};

struct npl_funcs_t *npl_funcs = NULL;

struct npl_funcs_t * npl_freertos_funcs_get(void)
{
    return npl_funcs;
}

void npl_freertos_funcs_init(void)
{
    npl_funcs = (struct npl_funcs_t *)malloc(sizeof(struct npl_funcs_t));
    if(!npl_funcs) {
        printf("npl funcs init failed\n");
        assert(0);
    }
    memcpy(npl_funcs, &npl_funcs_ro, sizeof(struct npl_funcs_t));
}

int npl_freertos_mempool_init(void)
{
    int rc = -1;

#if SOC_ESP_NIMBLE_CONTROLLER
    ble_freertos_ev_buf  = malloc(OS_MEMPOOL_SIZE(BLE_TOTAL_EV_COUNT, sizeof (struct ble_npl_event_freertos)) * sizeof(os_membuf_t));
    if(!ble_freertos_ev_buf) {
        goto _error;
    }
    ble_freertos_evq_buf  = malloc(OS_MEMPOOL_SIZE(BLE_TOTAL_EVQ_COUNT, sizeof (struct ble_npl_eventq_freertos)) * sizeof(os_membuf_t));
    if(!ble_freertos_evq_buf) {
        goto _error;
    }
    ble_freertos_co_buf  = malloc(OS_MEMPOOL_SIZE(BLE_TOTAL_CO_COUNT, sizeof (struct ble_npl_callout_freertos)) * sizeof(os_membuf_t));
    if(!ble_freertos_co_buf) {
        goto _error;
    }
    ble_freertos_sem_buf  = malloc(OS_MEMPOOL_SIZE(BLE_TOTAL_SEM_COUNT, sizeof (struct ble_npl_sem_freertos)) * sizeof(os_membuf_t));
    if(!ble_freertos_sem_buf) {
        goto _error;
    }
    ble_freertos_mutex_buf  = malloc( OS_MEMPOOL_SIZE(BLE_TOTAL_MUTEX_COUNT, sizeof (struct ble_npl_mutex_freertos)) * sizeof(os_membuf_t));
    if(!ble_freertos_mutex_buf) {
        goto _error;
    }

#endif

    rc = os_mempool_init(&ble_freertos_ev_pool, BLE_TOTAL_EV_COUNT,
                         sizeof (struct ble_npl_event_freertos), ble_freertos_ev_buf,
                         "ble_freertos_ev_pool");
    if(rc != 0) {
        goto _error;
    }

    rc = os_mempool_init(&ble_freertos_evq_pool, BLE_TOTAL_EVQ_COUNT,
                         sizeof (struct ble_npl_eventq_freertos), ble_freertos_evq_buf,
                         "ble_freertos_evq_pool");
    if(rc != 0) {
        goto _error;
    }

    rc = os_mempool_init(&ble_freertos_co_pool, BLE_TOTAL_CO_COUNT,
                         sizeof (struct ble_npl_callout_freertos), ble_freertos_co_buf,
                         "ble_freertos_co_pool");
    if(rc != 0) {
        goto _error;
    }

    rc = os_mempool_init(&ble_freertos_sem_pool, BLE_TOTAL_SEM_COUNT,
                         sizeof (struct ble_npl_sem_freertos), ble_freertos_sem_buf,
                         "ble_freertos_sem_pool");
    if(rc != 0) {
        goto _error;
    }

    rc = os_mempool_init(&ble_freertos_mutex_pool, BLE_TOTAL_MUTEX_COUNT,
                         sizeof (struct ble_npl_mutex_freertos), ble_freertos_mutex_buf,
                         "ble_freertos_mutex_pool");
    if(rc == 0) {
        return rc;
    }
_error:

#if SOC_ESP_NIMBLE_CONTROLLER
    if(ble_freertos_ev_buf) {
        free(ble_freertos_ev_buf);
    }
    if(ble_freertos_evq_buf) {
        free(ble_freertos_evq_buf);
    }
    if(ble_freertos_co_buf) {
        free(ble_freertos_co_buf);
    }
    if(ble_freertos_sem_buf) {
        free(ble_freertos_sem_buf);
    }
    if(ble_freertos_mutex_buf) {
        free(ble_freertos_mutex_buf);
    }
    return -1;
#else

   PLATFORM_BLE_LL_ASSERT(rc == 0);
   return rc;
#endif
}

void npl_freertos_mempool_deinit(void)
{
#if SOC_ESP_NIMBLE_CONTROLLER
    if(ble_freertos_ev_buf) {
        free(ble_freertos_ev_buf);
    }
    if(ble_freertos_evq_buf) {
        free(ble_freertos_evq_buf);
    }
    if(ble_freertos_co_buf) {
        free(ble_freertos_co_buf);
    }
    if(ble_freertos_sem_buf) {
        free(ble_freertos_sem_buf);
    }
    if(ble_freertos_mutex_buf) {
        free(ble_freertos_mutex_buf);
    }
#endif
}

void npl_freertos_funcs_deinit(void)
{
    if (npl_funcs) {
        free(npl_funcs);
    }
    npl_funcs = NULL;
}

#else // Not using MEM pools

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "nimble/nimble/include/nimble/nimble_npl.h"

#ifdef ESP_PLATFORM
#include "freertos/portable.h"
#include "esp_log.h"
portMUX_TYPE ble_port_mutex = portMUX_INITIALIZER_UNLOCKED;
#  if CONFIG_BT_NIMBLE_USE_ESP_TIMER
static const char *TAG = "Timer";
#  endif

#else
#include "nrf.h"
static void *radio_isr_addr;
static void *rng_isr_addr;
static void *rtc0_isr_addr;
#endif

#ifdef ESP_PLATFORM
static inline bool
in_isr(void)
{
    /* XXX hw specific! */
    return xPortInIsrContext() != 0;
}

#else
static inline bool
in_isr(void)
{
    /* XXX hw specific! */
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}

void
RADIO_IRQHandler(void)
{
    ((void (*)(void))radio_isr_addr)();
}

void
RNG_IRQHandler(void)
{
    ((void (*)(void))rng_isr_addr)();
}

void
RTC0_IRQHandler(void)
{
    ((void (*)(void))rtc0_isr_addr)();
}

/* This is called by NimBLE radio driver to set interrupt handlers */
void
npl_freertos_hw_set_isr(int irqn, void (*addr)(void))
{
    switch (irqn) {
    case RADIO_IRQn:
        radio_isr_addr = addr;
        break;
    case RNG_IRQn:
        rng_isr_addr = addr;
        break;
    case RTC0_IRQn:
        rtc0_isr_addr = addr;
        break;
    }
}
#endif

struct ble_npl_event *
npl_freertos_eventq_get(struct ble_npl_eventq *evq, ble_npl_time_t tmo)
{
    struct ble_npl_event *ev = NULL;
    BaseType_t woken;
    BaseType_t ret;

    if (in_isr()) {
        assert(tmo == 0);
        ret = xQueueReceiveFromISR(evq->q, &ev, &woken);
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
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
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
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

#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
#ifdef ESP_PLATFORM
        portMUX_TYPE ble_npl_mut = portMUX_INITIALIZER_UNLOCKED;
        portENTER_CRITICAL(&ble_npl_mut);
#else
        vPortEnterCritical();
#endif
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
#ifdef ESP_PLATFORM
        portEXIT_CRITICAL(&ble_npl_mut);
#else
        vPortExitCritical();
#endif
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
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
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
#ifdef ESP_PLATFORM
        if( woken == pdTRUE ) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken);
#endif
    } else {
        ret = xSemaphoreGive(sem->handle);
    }

    assert(ret == pdPASS);
    return BLE_NPL_OK;
}


#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
static void
ble_npl_event_fn_wrapper(void *arg)
{
    struct ble_npl_callout *co = (struct ble_npl_callout *)arg;

    if (co->evq) {
        ble_npl_eventq_put(co->evq, &co->ev);
    } else {
        co->ev.fn(&co->ev);
    }
}

static
ble_npl_error_t esp_err_to_npl_error(esp_err_t err)
{
    switch(err) {
    case ESP_ERR_INVALID_ARG:
        return BLE_NPL_INVALID_PARAM;

    case ESP_ERR_INVALID_STATE:
        return BLE_NPL_EINVAL;

    case ESP_OK:
        return BLE_NPL_OK;

    default:
        return BLE_NPL_ERROR;
    }
}
#else
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
#endif

void
npl_freertos_callout_init(struct ble_npl_callout *co, struct ble_npl_eventq *evq,
                          ble_npl_event_fn *ev_cb, void *ev_arg)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    co->ev.fn = ev_cb;
    co->ev.arg = ev_arg;
    co->evq = evq;

    esp_timer_create_args_t create_args = {
      .callback = ble_npl_event_fn_wrapper,
      .arg = co,
      .name = "nimble_timer"
    };

    ESP_ERROR_CHECK(esp_timer_create(&create_args, &co->handle));
#else
    if (co->handle == NULL) {
        co->handle = xTimerCreate("co", 1, pdFALSE, co, os_callout_timer_cb);
    }

    co->evq = evq;
    ble_npl_event_init(&co->ev, ev_cb, ev_arg);
#endif
}

void
npl_freertos_callout_deinit(struct ble_npl_callout *co)
{
    if (!co->handle) {
        return;
    }

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    if(esp_timer_stop(co->handle))
	ESP_LOGW(TAG, "Timer not stopped");

    if(esp_timer_delete(co->handle))
	ESP_LOGW(TAG, "Timer not deleted");
#else
    xTimerDelete(co->handle, portMAX_DELAY);
    ble_npl_event_deinit(&co->ev);
#endif
    memset(co, 0, sizeof(struct ble_npl_callout));
}

ble_npl_error_t
npl_freertos_callout_reset(struct ble_npl_callout *co, ble_npl_time_t ticks)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    esp_timer_stop(co->handle);

    return esp_err_to_npl_error(esp_timer_start_once(co->handle, ticks*1000));
#else

    BaseType_t woken1, woken2, woken3;

    if (ticks == 0) {
        ticks = 1;
    }

    if (in_isr()) {
        xTimerStopFromISR(co->handle, &woken1);
        xTimerChangePeriodFromISR(co->handle, ticks, &woken2);
        xTimerResetFromISR(co->handle, &woken3);
#ifdef ESP_PLATFORM
        if( woken1 == pdTRUE || woken2 == pdTRUE || woken3 == pdTRUE) {
            portYIELD_FROM_ISR();
        }
#else
        portYIELD_FROM_ISR(woken1 || woken2 || woken3);
#endif
    } else {
        xTimerStop(co->handle, portMAX_DELAY);
        xTimerChangePeriod(co->handle, ticks, portMAX_DELAY);
        xTimerReset(co->handle, portMAX_DELAY);
    }

    return BLE_NPL_OK;
#endif
}

void
npl_freertos_callout_stop(struct ble_npl_callout *co)
{
    if (!co->handle) {
        return;
    }
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    esp_timer_stop(co->handle);
#else
    xTimerStop(co->handle, portMAX_DELAY);
#endif
}

bool
npl_freertos_callout_is_active(struct ble_npl_callout *co)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
    return esp_timer_is_active(co->handle);
#else
    /* Workaround for bug in xTimerIsTimerActive with FreeRTOS V10.2.0, fixed in V10.4.4
     * See: https://github.com/FreeRTOS/FreeRTOS-Kernel/pull/305
     * Sometimes xTimerIsTimerActive returns pdTRUE even though the timer has expired, so we double check.
     */
    return xTimerIsTimerActive(co->handle) == pdTRUE && xTimerGetExpiryTime(co->handle) > xTaskGetTickCountFromISR();
#endif
}

ble_npl_time_t
npl_freertos_callout_get_ticks(struct ble_npl_callout *co)
{
#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
   /* Currently, esp_timer does not support an API which gets the expiry time for
    * current timer.
    * Returning 0 from here should not cause any effect.
    * Drawback of this approach is that existing code to reset timer would be called
    * more often (since the if condition to invoke reset timer would always succeed if
    * timer is active).
    */

    return 0;
#else
    return xTimerGetExpiryTime(co->handle);
#endif
}

ble_npl_time_t
npl_freertos_callout_remaining_ticks(struct ble_npl_callout *co,
                                     ble_npl_time_t now)
{
    ble_npl_time_t rt;
    uint32_t exp = 0;

#if CONFIG_BT_NIMBLE_USE_ESP_TIMER
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    uint64_t expiry = 0;
    esp_err_t err;

    //Fetch expiry time in microseconds
    err = esp_timer_get_expiry_time((esp_timer_handle_t)(co->handle), &expiry);
    if (err != ESP_OK) {
        //Error. Could not fetch the expiry time
        return 0;
    }

    //Convert microseconds to ticks
    npl_freertos_time_ms_to_ticks((uint32_t)(expiry / 1000), &exp);
#else
    //esp_timer_get_expiry_time() is only available from IDF 5.0 onwards
    //Set expiry to 0
    exp = 0;
#endif //ESP_IDF_VERSION
#else
    exp = xTimerGetExpiryTime(co->handle);
#endif

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

#endif // CONFIG_NIMBLE_STACK_USE_MEM_POOLS
