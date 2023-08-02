Attention when updating library. Changes in lib needed!!

Changes in src/nimble/porting/nimble/include/log_common/log_common.h

```
/*
#define LOG_LEVEL_DEBUG    (0)
#define LOG_LEVEL_INFO     (1)
#define LOG_LEVEL_WARN     (2)
#define LOG_LEVEL_ERROR    (3)
#define LOG_LEVEL_CRITICAL (4)
#define LOG_LEVEL_NONE     (5)
*/
#define LOG_LEVEL_DEBUG    (3)
#define LOG_LEVEL_INFO     (2)
#define LOG_LEVEL_WARN     (4)
#define LOG_LEVEL_ERROR    (1)
#define LOG_LEVEL_CRITICAL (5)
#define LOG_LEVEL_NONE     (0)
```

Change in nimconfig.h

`#define CONFIG_BT_NIMBLE_NVS_PERSIST 1`
to
`#define CONFIG_BT_NIMBLE_NVS_PERSIST 0`

Rename /src/nimble/esp_port/port/src/esp_nimble_mem.c to esp_nimble_mem.cpp

and replace all in the file with

```
/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef ESP_PLATFORM

#include "esp_attr.h"
#include "esp_heap_caps.h"
#include "nimconfig.h"
#include "../include/esp_nimble_mem.h"

//Tasmota Patch
extern void *special_malloc(uint32_t size);
extern void *special_calloc(size_t num, size_t size);


extern "C" {
IRAM_ATTR void *nimble_platform_mem_malloc(size_t size)
{
    return special_malloc((uint32_t)size);
// #ifdef CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL
//     return heap_caps_malloc(size, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
// #elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
//     return heap_caps_malloc(size, MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
// #elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_IRAM_8BIT
//     return heap_caps_malloc_prefer(size, 2, MALLOC_CAP_INTERNAL|MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
// #else
//     return malloc(size);
// #endif
}

IRAM_ATTR void *nimble_platform_mem_calloc(size_t n, size_t size)
{
    return special_calloc(n,size);
// #ifdef CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL
//     return heap_caps_calloc(n, size, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
// #elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
//     return heap_caps_calloc(n, size, MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
// #elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_IRAM_8BIT
//     return heap_caps_calloc_prefer(n, size, 2, MALLOC_CAP_INTERNAL|MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
// #else
//     return calloc(n, size);
// #endif
}

IRAM_ATTR void nimble_platform_mem_free(void *ptr)
{
    heap_caps_free(ptr);
}
} //extern "C"
#endif
```
