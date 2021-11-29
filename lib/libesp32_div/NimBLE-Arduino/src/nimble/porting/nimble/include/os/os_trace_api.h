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

#ifndef OS_TRACE_API_H
#define OS_TRACE_API_H

#include <stdint.h>

#define OS_TRACE_ID_EVENTQ_PUT                  (40)
#define OS_TRACE_ID_EVENTQ_GET_NO_WAIT          (41)
#define OS_TRACE_ID_EVENTQ_GET                  (42)
#define OS_TRACE_ID_EVENTQ_REMOVE               (43)
#define OS_TRACE_ID_EVENTQ_POLL_0TIMO           (44)
#define OS_TRACE_ID_EVENTQ_POLL                 (45)
#define OS_TRACE_ID_MUTEX_INIT                  (50)
#define OS_TRACE_ID_MUTEX_RELEASE               (51)
#define OS_TRACE_ID_MUTEX_PEND                  (52)
#define OS_TRACE_ID_SEM_INIT                    (60)
#define OS_TRACE_ID_SEM_RELEASE                 (61)
#define OS_TRACE_ID_SEM_PEND                    (62)

static inline void
os_trace_isr_enter(void)
{
}

static inline void
os_trace_isr_exit(void)
{
}

static inline void
os_trace_idle(void)
{
}

static inline void
os_trace_api_void(unsigned id)
{
}

static inline void
os_trace_api_u32(unsigned id, uint32_t p0)
{
}

static inline void
os_trace_api_u32x2(unsigned id, uint32_t p0, uint32_t p1)
{
}

static inline void
os_trace_api_u32x3(unsigned id, uint32_t p0, uint32_t p1, uint32_t p2)
{
}

static inline void
os_trace_api_ret(unsigned id)
{
}

static inline void
os_trace_api_ret_u32(unsigned id, uint32_t return_value)
{
}

#endif
