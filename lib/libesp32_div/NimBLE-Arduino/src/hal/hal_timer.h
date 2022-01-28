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


/**
 * @addtogroup HAL
 * @{
 *   @defgroup HALTimer HAL Timer
 *   @{
 */

#ifndef H_HAL_TIMER_
#define H_HAL_TIMER_

#include <inttypes.h>
#include "os/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* HAL timer callback */
typedef void (*hal_timer_cb)(void *arg);

/**
 * The HAL timer structure. The user can declare as many of these structures
 * as desired. They are enqueued on a particular HW timer queue when the user
 * calls the :c:func:`hal_timer_start()` or :c:func:`hal_timer_start_at()` API.
 * The user must have called :c:func:`hal_timer_set_cb()` before starting a
 * timer.
 *
 * NOTE: the user should not have to modify/examine the contents of this
 * structure; the hal timer API should be used.
 */
struct hal_timer {
    /** Internal platform specific pointer */
    void                *bsp_timer;
    /** Callback function */
    hal_timer_cb        cb_func;
    /** Callback argument */
    void                *cb_arg;
    /** Tick at which timer should expire */
    uint32_t            expiry;
    TAILQ_ENTRY(hal_timer) link;    /* Queue linked list structure */
};

/**
 * Initialize a HW timer.
 *
 * @param timer_num The number of the HW timer to initialize
 * @param cfg       Hardware specific timer configuration.  This is
 *                  passed from BSP directly to the MCU specific driver.
 */
int hal_timer_init(int timer_num, void *cfg);

/**
 * Un-initialize a HW timer.
 *
 * @param timer_num The number of the HW timer to un-initialize
 */
int hal_timer_deinit(int timer_num);

/**
 * Config a HW timer at the given frequency and start it. If the exact
 * frequency is not obtainable the closest obtainable frequency is set.
 *
 * @param timer_num The number of the HW timer to configure
 * @param freq_hz   The frequency in Hz to configure the timer at
 *
 * @return 0 on success, non-zero error code on failure
 */
int hal_timer_config(int timer_num, uint32_t freq_hz);

/**
 * Returns the resolution of the HW timer. NOTE: the frequency may not be
 * obtainable so the caller can use this to determine the resolution.
 * Returns resolution in nanoseconds. A return value of 0 indicates an invalid
 * timer was used.
 *
 * @param timer_num The number of the HW timer to get resolution for
 *
 * @return The resolution of the timer
 */
uint32_t hal_timer_get_resolution(int timer_num);

/**
 * Returns the HW timer current tick value
 *
 * @param timer_num The HW timer to read the tick value from
 *
 * @return The current tick value
 */
uint32_t hal_timer_read(int timer_num);

/**
 * Perform a blocking delay for a number of ticks.
 *
 * @param timer_num The timer number to use for the blocking delay
 * @param ticks The number of ticks to delay for
 *
 * @return 0 on success, non-zero error code on failure
 */
int hal_timer_delay(int timer_num, uint32_t ticks);

/**
 * Set the timer structure prior to use. Should not be called if the timer
 * is running. Must be called at least once prior to using timer.
 *
 * @param timer_num The number of the HW timer to configure the callback on
 * @param tmr       The timer structure to use for this timer
 * @param cb_func   The timer callback to call when the timer fires
 * @param arg       An opaque argument to provide the timer callback
 *
 * @return 0  on success, non-zero error code on failure.
 */
int hal_timer_set_cb(int timer_num, struct hal_timer *tmr, hal_timer_cb cb_func,
                     void *arg);

/**
 * Start a timer that will expire in 'ticks' ticks. Ticks cannot be 0
 *
 * @param tmr   The timer to start
 * @param ticks The number of ticks to expire the timer in
 *
 * @return 0 on success, non-zero error code on failure.
 */
int hal_timer_start(struct hal_timer *tmr, uint32_t ticks);

/**
 * Start a timer that will expire when the timer reaches 'tick'. If tick
 * has already passed the timer callback will be called "immediately" (at
 * interrupt context).
 *
 * @param tmr  The timer to start
 * @param tick The absolute tick value to fire the timer at
 *
 * @return 0 on success, non-zero error code on failure.
 */
int hal_timer_start_at(struct hal_timer *tmr, uint32_t tick);

/**
 * Stop a currently running timer; associated callback will NOT be called
 *
 * @param tmr The timer to stop
 */
int hal_timer_stop(struct hal_timer *tmr);

#ifdef __cplusplus
}
#endif

#endif /* H_HAL_TIMER_ */

/**
 *   @} HALTimer
 * @} HAL
 */
