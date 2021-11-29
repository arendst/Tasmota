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

#ifndef _OS_H
#define _OS_H

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif

#include "../syscfg/syscfg.h"
#include "nimble/nimble/include/nimble/nimble_npl.h"

#define OS_ALIGN(__n, __a) (                             \
        (((__n) & ((__a) - 1)) == 0)                   ? \
            (__n)                                      : \
            ((__n) + ((__a) - ((__n) & ((__a) - 1))))    \
        )
#define OS_ALIGNMENT    (BLE_NPL_OS_ALIGNMENT)

typedef uint32_t os_sr_t;
#define OS_ENTER_CRITICAL(_sr) (_sr = ble_npl_hw_enter_critical())
#define OS_EXIT_CRITICAL(_sr) (ble_npl_hw_exit_critical(_sr))
#define OS_ASSERT_CRITICAL() assert(ble_npl_hw_is_in_critical())

/* Mynewt components (not abstracted in NPL) */
#include "endian.h"
#include "queue.h"
#include "os_error.h"
#include "os_mbuf.h"
#include "os_mempool.h"

#ifdef __cplusplus
}
#endif

#endif /* _OS_H */
