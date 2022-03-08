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

IRAM_ATTR void *nimble_platform_mem_malloc(size_t size)
{
#ifdef CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL
    return heap_caps_malloc(size, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
#elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_IRAM_8BIT
    return heap_caps_malloc_prefer(size, 2, MALLOC_CAP_INTERNAL|MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#else
    return malloc(size);
#endif
}

IRAM_ATTR void *nimble_platform_mem_calloc(size_t n, size_t size)
{
#ifdef CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_INTERNAL
    return heap_caps_calloc(n, size, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_EXTERNAL
    return heap_caps_calloc(n, size, MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT);
#elif CONFIG_BT_NIMBLE_MEM_ALLOC_MODE_IRAM_8BIT
    return heap_caps_calloc_prefer(n, size, 2, MALLOC_CAP_INTERNAL|MALLOC_CAP_IRAM_8BIT, MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT);
#else
    return calloc(n, size);
#endif
}

IRAM_ATTR void nimble_platform_mem_free(void *ptr)
{
    heap_caps_free(ptr);
}
#endif
