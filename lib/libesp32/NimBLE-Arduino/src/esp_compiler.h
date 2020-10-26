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
#define likely(x)      __builtin_expect(!!(x), 1) 
#define unlikely(x)    __builtin_expect(!!(x), 0) 
#else
#define likely(x)      (x)
#define unlikely(x)    (x) 
#endif

#endif