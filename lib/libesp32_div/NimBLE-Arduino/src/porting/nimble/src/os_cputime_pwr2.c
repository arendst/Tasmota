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

#include "os/os_cputime.h"

/**
 * This module implements cputime functionality for timers for which:
 *     a. freq is a power of 2 Hz, and
 *     b. 256 Hz <= freq < 1 MHz
 */

#if defined(OS_CPUTIME_FREQ_PWR2)

/**
 * @addtogroup OSKernel Operating System Kernel
 * @{
 *   @defgroup OSCPUTime High Resolution Timers
 *   @{
 */

/**
 * os cputime usecs to ticks
 *
 * Converts the given number of microseconds into cputime ticks.
 *
 * @param usecs The number of microseconds to convert to ticks
 *
 * @return uint32_t The number of ticks corresponding to 'usecs'
 */
uint32_t
os_cputime_usecs_to_ticks(uint32_t usecs)
{
    uint64_t ticks;

    /*
     * Faster calculation but could be off 1 full tick since we do not
     * add residual back. Adding back the residual is commented out below, but
     * shown.
     */
    ticks = (1ULL << 32) * MYNEWT_VAL(OS_CPUTIME_FREQ) / 1000000 * usecs;

    /* Residual for 32768 Hz. */
    //ticks += ((uint64_t)usecs * (1526122139+1)) >> 32;

    return ticks >> 32;
}

/**
 * cputime ticks to usecs
 *
 * Convert the given number of ticks into microseconds.
 *
 * @param ticks The number of ticks to convert to microseconds.
 *
 * @return uint32_t The number of microseconds corresponding to 'ticks'
 *
 * NOTE: This calculation will overflow if the value for ticks is greater
 * than 140737488. I am not going to check that here because that many ticks
 * is about 4222 seconds, way more than what this routine should be used for.
 */
uint32_t
os_cputime_ticks_to_usecs(uint32_t ticks)
{
    uint32_t usecs;
    uint32_t shift;
    uint32_t freq;

    /* Given: `freq = 2^n`, calculate `n`. */
    /* Note: this looks like a lot of work, but gcc can optimize it away since
     * `freq` is known at compile time.
     */
    freq = MYNEWT_VAL(OS_CPUTIME_FREQ);
    shift = 0;
    while (freq != 0) {
        freq >>= 1;
        shift++;
    }

    if (shift <= 7) {
        return 0;
    }
    shift -= 7;

    usecs = ((ticks >> shift) * 15625) + (((ticks & 0x1ff) * 15625) >> shift);
    return usecs;
}

/**
 *   @} OSCPUTime
 * @} OSKernel
 */

#endif
