// Copyright 2016-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __ESP_COMPILER_H
#define __ESP_COMPILER_H

/*
 * The likely and unlikely macro pairs:
 * These macros are useful to place when application
 * knows the majority ocurrence of a decision paths,
 * placing one of these macros can hint the compiler
 * to reorder instructions producing more optimized
 * code.
 */
#if (CONFIG_COMPILER_OPTIMIZATION_PERF)
#ifndef likely
#define likely(x)      __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)    __builtin_expect(!!(x), 0)
#endif
#else
#ifndef likely
#define likely(x)      (x)
#endif
#ifndef unlikely
#define unlikely(x)    (x)
#endif
#endif

/*
 * Utility macros used for designated initializers, which work differently
 * in C99 and C++ standards mainly for aggregate types.
 * The member separator, comma, is already part of the macro, please omit the trailing comma.
 * Usage example:
 *   struct config_t { char* pchr; char arr[SIZE]; } config = {
 *              ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(pchr)
 *              ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(arr, "Value")
 *          };
 */
#ifdef __cplusplus
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(member, value)  { .member = value },
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(member) .member = { },
#else
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_STR(member, value)  .member = value,
#define ESP_COMPILER_DESIGNATED_INIT_AGGREGATE_TYPE_EMPTY(member)
#endif

#endif
