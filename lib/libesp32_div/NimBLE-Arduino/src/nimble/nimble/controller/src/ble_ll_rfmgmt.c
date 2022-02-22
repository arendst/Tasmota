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
#include <stddef.h>
#include <assert.h>
#include <stddef.h>
#include "nimble/porting/nimble/include/syscfg/syscfg.h"
#include "nimble/porting/nimble/include/os/os_cputime.h"
#include "../include/controller/ble_phy.h"
#include "../include/controller/ble_ll.h"
#include "../include/controller/ble_ll_sched.h"
#include "../include/controller/ble_ll_rfmgmt.h"

#if MYNEWT_VAL(BLE_LL_RFMGMT_ENABLE_TIME) > 0

enum ble_ll_rfmgmt_state {
    RFMGMT_STATE_OFF = 0,
    RFMGMT_STATE_ENABLING = 1,
    RFMGMT_STATE_ENABLED = 2,
};

struct ble_ll_rfmgmt_data {
    enum ble_ll_rfmgmt_state state;
    uint16_t ticks_to_enabled;

    struct hal_timer timer;
    bool timer_scheduled;
    uint32_t timer_scheduled_at;

    bool enable_scan;
    bool enable_sched;
    uint32_t enable_scan_at;
    uint32_t enable_sched_at;

    uint32_t enabled_at;

    struct ble_npl_event release_ev;
};

static struct ble_ll_rfmgmt_data g_ble_ll_rfmgmt_data;

static void
ble_ll_rfmgmt_enable(void)
{
    OS_ASSERT_CRITICAL();

    if (g_ble_ll_rfmgmt_data.state == RFMGMT_STATE_OFF) {
        g_ble_ll_rfmgmt_data.state = RFMGMT_STATE_ENABLING;
        g_ble_ll_rfmgmt_data.enabled_at = os_cputime_get32();
        ble_phy_rfclk_enable();
    }
}

static void
ble_ll_rfmgmt_disable(void)
{
    OS_ASSERT_CRITICAL();

    if (g_ble_ll_rfmgmt_data.state != RFMGMT_STATE_OFF) {
        ble_phy_rfclk_disable();
        g_ble_ll_rfmgmt_data.state = RFMGMT_STATE_OFF;
    }
}

static void
ble_ll_rfmgmt_timer_reschedule(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    uint32_t enable_at;

    /* Figure out when we need to enable RF */
    if (rfmgmt->enable_scan && rfmgmt->enable_sched) {
        if (CPUTIME_LT(rfmgmt->enable_scan_at, rfmgmt->enable_sched_at)) {
            enable_at = rfmgmt->enable_scan_at;
        } else {
            enable_at = rfmgmt->enable_sched_at;
        }
    } else if (rfmgmt->enable_scan) {
        enable_at = rfmgmt->enable_scan_at;
    } else if (rfmgmt->enable_sched) {
        enable_at = rfmgmt->enable_sched_at;
    } else {
        rfmgmt->timer_scheduled = false;
        os_cputime_timer_stop(&rfmgmt->timer);
        return;
    }

    if (rfmgmt->timer_scheduled) {
        /*
         * If there is timer already scheduled at the same time we do not need
         * to do anything. Otherwise we need to stop timer and schedule it again
         * regardless if it's earlier or later to make sure it fires at the time
         * something expects it.
         */

        if (rfmgmt->timer_scheduled_at == enable_at) {
            return;
        }

        rfmgmt->timer_scheduled = false;
        os_cputime_timer_stop(&rfmgmt->timer);
    }

    /*
     * In case timer was requested to be enabled before current time, just make
     * sure it's enabled and assume caller can deal with this. This will happen
     * if something is scheduled "now" since "enable_at" is in the past, but in
     * such case it's absolutely harmless since we already have clock enabled
     * and this will do nothing.
     */
    if (CPUTIME_LEQ(enable_at, os_cputime_get32())) {
        ble_ll_rfmgmt_enable();
        return;
    }

    rfmgmt->timer_scheduled = true;
    rfmgmt->timer_scheduled_at = enable_at;
    os_cputime_timer_start(&rfmgmt->timer, enable_at);
}

static void
ble_ll_rfmgmt_timer_exp(void *arg)
{
    g_ble_ll_rfmgmt_data.timer_scheduled = false;
    ble_ll_rfmgmt_enable();
}

static void
ble_ll_rfmgmt_release_ev(struct ble_npl_event *ev)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    uint32_t now;
    bool can_disable;
    uint8_t lls;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);

    now = os_cputime_get32();

    can_disable = true;
    lls = ble_ll_state_get();

    if (rfmgmt->enable_scan && CPUTIME_GEQ(now, rfmgmt->enable_scan_at)) {
        /* Blocked by scan */
        can_disable = false;
    } else if (rfmgmt->enable_sched && CPUTIME_GEQ(now, rfmgmt->enable_sched_at)) {
        /* Blocked by scheduler item */
        can_disable = false;
    } else if (lls != BLE_LL_STATE_STANDBY) {
        /* Blocked by LL state */
        can_disable = false;
    }

    if (can_disable) {
        ble_ll_rfmgmt_disable();
    }

    OS_EXIT_CRITICAL(sr);
}

static uint32_t
ble_ll_rfmgmt_ticks_to_enabled(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    uint32_t rem_ticks;
    uint32_t now;

    switch (rfmgmt->state) {
    case RFMGMT_STATE_OFF:
        rem_ticks = rfmgmt->ticks_to_enabled;
        break;
    case RFMGMT_STATE_ENABLING:
        now = os_cputime_get32();
        if (CPUTIME_LT(now, rfmgmt->enabled_at + rfmgmt->ticks_to_enabled)) {
            rem_ticks = rfmgmt->enabled_at + rfmgmt->ticks_to_enabled - now;
            break;
        }
        rfmgmt->state = RFMGMT_STATE_ENABLED;
        /* Else falls through. */
        /* no break */
    case RFMGMT_STATE_ENABLED:
        rem_ticks = 0;
        break;
    default:
        BLE_LL_ASSERT(0);
        rem_ticks = 0;
        break;
    }

    return rem_ticks;
}

void
ble_ll_rfmgmt_init(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;

    rfmgmt->state = RFMGMT_STATE_OFF;

    rfmgmt->ticks_to_enabled =
            ble_ll_usecs_to_ticks_round_up(MYNEWT_VAL(BLE_LL_RFMGMT_ENABLE_TIME));

    rfmgmt->timer_scheduled = false;
    os_cputime_timer_init(&rfmgmt->timer, ble_ll_rfmgmt_timer_exp, NULL);

    ble_npl_event_init(&rfmgmt->release_ev, ble_ll_rfmgmt_release_ev, NULL);
}

void
ble_ll_rfmgmt_reset(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;

    rfmgmt->timer_scheduled = false;
    rfmgmt->timer_scheduled_at = 0;
    os_cputime_timer_stop(&rfmgmt->timer);

    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &rfmgmt->release_ev);

    ble_ll_rfmgmt_disable();

    rfmgmt->enable_scan = false;
    rfmgmt->enable_scan_at = 0;
    rfmgmt->enable_sched = false;
    rfmgmt->enable_sched_at = 0;

    rfmgmt->enabled_at = 0;
}

void
ble_ll_rfmgmt_scan_changed(bool enabled, uint32_t next_window)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);

    rfmgmt->enable_scan = enabled;
    rfmgmt->enable_scan_at = next_window - rfmgmt->ticks_to_enabled;

    ble_ll_rfmgmt_timer_reschedule();

    OS_EXIT_CRITICAL(sr);
}

void
ble_ll_rfmgmt_sched_changed(struct ble_ll_sched_item *first)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);

    rfmgmt->enable_sched = (first != NULL);
    if (first) {
        rfmgmt->enable_sched_at = first->start_time - rfmgmt->ticks_to_enabled;
    }

    ble_ll_rfmgmt_timer_reschedule();

    OS_EXIT_CRITICAL(sr);
}

void
ble_ll_rfmgmt_release(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);

    ble_npl_eventq_remove(&g_ble_ll_data.ll_evq, &rfmgmt->release_ev);

    if (g_ble_ll_rfmgmt_data.state != RFMGMT_STATE_OFF) {
        ble_npl_eventq_put(&g_ble_ll_data.ll_evq, &rfmgmt->release_ev);
    }

    OS_EXIT_CRITICAL(sr);
}

uint32_t
ble_ll_rfmgmt_enable_now(void)
{
    struct ble_ll_rfmgmt_data *rfmgmt = &g_ble_ll_rfmgmt_data;
    uint32_t enabled_at;
    os_sr_t sr;

    OS_ENTER_CRITICAL(sr);

    ble_ll_rfmgmt_enable();

    if (rfmgmt->state == RFMGMT_STATE_ENABLED) {
        enabled_at = os_cputime_get32();
    } else {
        enabled_at = rfmgmt->enabled_at + rfmgmt->ticks_to_enabled + 1;
    }

    OS_EXIT_CRITICAL(sr);

    return enabled_at;
}

bool
ble_ll_rfmgmt_is_enabled(void)
{
    bool ret;

    OS_ASSERT_CRITICAL();

    ret = ble_ll_rfmgmt_ticks_to_enabled() == 0;

    return ret;
}

#else

void
ble_ll_rfmgmt_init(void)
{
    static bool enabled = false;

    if (!enabled) {
        ble_phy_rfclk_enable();
    }

    enabled = true;
}

#endif
#endif
