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
  * @addtogroup OSKernel
  * @{
  *   @defgroup OSCPUTime High Resolution Timers
  *   @{
  */

#ifndef H_OS_CPUTIME_
#define H_OS_CPUTIME_

#ifdef __cplusplus
extern "C" {
#endif

#include "syscfg/syscfg.h"
#include "hal/hal_timer.h"
#include "os/os.h"

/*
 * NOTE: these definitions allow one to override the cputime frequency used.
 * The reason these definitions exist is to make the code more
 * efficient/smaller when CPUTIME counts at 1 MHz.
 *
 * For those who want a different cputime frequency, you can set the config
 * definition for OS_CPUTIME_FREQ to the desired frequency in your project,
 * target or bsp.
 */
#if (MYNEWT_VAL(OS_CPUTIME_FREQ) == 1000000)

#define OS_CPUTIME_FREQ_1MHZ

#elif MYNEWT_VAL(OS_CPUTIME_FREQ) == 256        ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 512        ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 1024       ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 2048       ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 4096       ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 8192       ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 16384      ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 32768      ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 65536      ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 131072     ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 262144     ||  \
      MYNEWT_VAL(OS_CPUTIME_FREQ) == 524288

#define OS_CPUTIME_FREQ_PWR2

#elif MYNEWT_VAL(OS_CPUTIME_FREQ) > 1000000

#define OS_CPUTIME_FREQ_HIGH

#else

#error "Invalid OS_CPUTIME_FREQ value.  Value must be one of a) a power of 2" \
       ">= 256Hz, or b) any value >= 1MHz"

#endif

#if defined(OS_CPUTIME_FREQ_HIGH)
/* CPUTIME data. */
struct os_cputime_data
{
    uint32_t ticks_per_usec;    /* number of ticks per usec */
};
extern struct os_cputime_data g_os_cputime;
#endif

/* Helpful macros to compare cputimes */
/** evaluates to true if t1 is before t2 in time */
#define CPUTIME_LT(__t1, __t2) ((int32_t)   ((__t1) - (__t2)) < 0)
/** evaluates to true if t1 is after t2 in time */
#define CPUTIME_GT(__t1, __t2) ((int32_t)   ((__t1) - (__t2)) > 0)
/** evaluates to true if t1 is after t2 in time */
#define CPUTIME_GEQ(__t1, __t2) ((int32_t)  ((__t1) - (__t2)) >= 0)
/** evaluates to true if t1 is on or after t2 in time */
#define CPUTIME_LEQ(__t1, __t2) ((int32_t)  ((__t1) - (__t2)) <= 0)

/**
 * Initialize the cputime module. This must be called after os_init is called
 * and before any other timer API are used. This should be called only once
 * and should be called before the hardware timer is used.
 *
 * @param clock_freq The desired cputime frequency, in hertz (Hz).
 *
 * @return int 0 on success; -1 on error.
 */
int os_cputime_init(uint32_t clock_freq);

/**
 * Returns the low 32 bits of cputime.
 *
 * @return uint32_t The lower 32 bits of cputime
 */
uint32_t os_cputime_get32(void);

#if !defined(OS_CPUTIME_FREQ_PWR2)
/**
 * Converts the given number of nanoseconds into cputime ticks.
 * Not defined if OS_CPUTIME_FREQ_PWR2 is defined.
 *
 * @param usecs The number of nanoseconds to convert to ticks
 *
 * @return uint32_t The number of ticks corresponding to 'nsecs'
 */
uint32_t os_cputime_nsecs_to_ticks(uint32_t nsecs);

/**
 * Convert the given number of ticks into nanoseconds.
 * Not defined if OS_CPUTIME_FREQ_PWR2 is defined.
 *
 * @param ticks The number of ticks to convert to nanoseconds.
 *
 * @return uint32_t The number of nanoseconds corresponding to 'ticks'
 */
uint32_t os_cputime_ticks_to_nsecs(uint32_t ticks);

/**
 * Wait until 'nsecs' nanoseconds has elapsed. This is a blocking delay.
 * Not defined if OS_CPUTIME_FREQ_PWR2 is defined.
 *
 *
 * @param nsecs The number of nanoseconds to wait.
 */
void os_cputime_delay_nsecs(uint32_t nsecs);
#endif

#if defined(OS_CPUTIME_FREQ_1MHZ)
#define os_cputime_usecs_to_ticks(x)    (x)
#define os_cputime_ticks_to_usecs(x)    (x)
#else

/**
 * Converts the given number of microseconds into cputime ticks.
 *
 * @param usecs The number of microseconds to convert to ticks
 *
 * @return uint32_t The number of ticks corresponding to 'usecs'
 */
uint32_t os_cputime_usecs_to_ticks(uint32_t usecs);

/**
 * Convert the given number of ticks into microseconds.
 *
 * @param ticks The number of ticks to convert to microseconds.
 *
 * @return uint32_t The number of microseconds corresponding to 'ticks'
 */
uint32_t os_cputime_ticks_to_usecs(uint32_t ticks);
#endif

/**
 * Wait until the number of ticks has elapsed. This is a blocking delay.
 *
 * @param ticks The number of ticks to wait.
 */
void os_cputime_delay_ticks(uint32_t ticks);

/**
 * Wait until 'usecs' microseconds has elapsed. This is a blocking delay.
 *
 * @param usecs The number of usecs to wait.
 */
void os_cputime_delay_usecs(uint32_t usecs);

/**
 * Initialize a CPU timer, using the given HAL timer.
 *
 * @param timer The timer to initialize. Cannot be NULL.
 * @param fp    The timer callback function. Cannot be NULL.
 * @param arg   Pointer to data object to pass to timer.
 */
void os_cputime_timer_init(struct hal_timer *timer, hal_timer_cb fp,
        void *arg);

/**
 * Start a cputimer that will expire at 'cputime'. If cputime has already
 * passed, the timer callback will still be called (at interrupt context).
 *
 * NOTE: This must be called when the timer is stopped.
 *
 * @param timer     Pointer to timer to start. Cannot be NULL.
 * @param cputime   The cputime at which the timer should expire.
 *
 * @return int 0 on success; EINVAL if timer already started or timer struct
 *         invalid
 *
 */
int os_cputime_timer_start(struct hal_timer *timer, uint32_t cputime);

/**
 * Sets a cpu timer that will expire 'usecs' microseconds from the current
 * cputime.
 *
 * NOTE: This must be called when the timer is stopped.
 *
 * @param timer Pointer to timer. Cannot be NULL.
 * @param usecs The number of usecs from now at which the timer will expire.
 *
 * @return int 0 on success; EINVAL if timer already started or timer struct
 *         invalid
 */
int os_cputime_timer_relative(struct hal_timer *timer, uint32_t usecs);

/**
 * Stops a cputimer from running. The timer is removed from the timer queue
 * and interrupts are disabled if no timers are left on the queue. Can be
 * called even if timer is not running.
 *
 * @param timer Pointer to cputimer to stop. Cannot be NULL.
 */
void os_cputime_timer_stop(struct hal_timer *timer);

#ifdef __cplusplus
}
#endif

#endif /* H_OS_CPUTIME_ */

/**
 *   @} OSCPUTime
 * @} OSKernel
 */
