/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef ESP_MFI_DEBUG_H_
#define ESP_MFI_DEBUG_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

#define ESP_MFI_DEBUG_FL "\n"
#define CONFIG_ESP_MFI_DEBUG_ENABLE
#ifdef CONFIG_ESP_MFI_DEBUG_ENABLE
// #define ESP_MFI_DEBUG_ENABLE
#endif /* CONFIG_ESP_MFI_DEBUG_ENABLE */

#define CONFIG_ESP_MFI_ASSERT
#ifdef CONFIG_ESP_MFI_ASSERT
// #define ESP_MFI_ASSERT_ENABLE
#endif /* CONFIG_ESP_MFI_ASSERT */

#define CONFIG_MFI_DEBUG_LEVEL_INIT 0
#define CONFIG_ESP_MFI_ASSERT_BLOCK
#ifdef CONFIG_ESP_MFI_ASSERT_BLOCK
#define ESP_MFI_ASSERT_BLOCK()  while (1)
#else /* CONFIG_ESP_MFI_ASSERT_BLOCK */
#define ESP_MFI_ASSERT_BLOCK()
#endif /* CONFIG_ESP_MFI_ASSERT_BLOCK */

/* debug level with different color */
#define ESP_MFI_DEBUG_INFO      1
#define ESP_MFI_DEBUG_WARN      2
#define ESP_MFI_DEBUG_ERR       3
#define ESP_MFI_DEBUG_ASSERT    4
#define ESP_MFI_DEBUG_BLOCK     5

/**
 * @bref set the MFI debugging level
 *
 * @param level debugging level
 *
 * @return the result
 *     = 0 : OK
 *     others : failed
 */
int esp_mfi_set_debug_level(uint32_t level);

/**
 * @bref get the MFI debugging level and output data color
 *
 * @param level input debug level
 * @param color data color
 *
 * @return MFI debugging level
 */
uint32_t esp_mfi_get_debug_level(uint32_t level, uint32_t *color);

/**
 * @bref format the string and data, then output it
 *
 * @param level if level > MFI_DEBUG_LEVEL then output it
 * @param fmt format string
 * @param ... parameters of format string
 *
 * @return none
 *
 * void ESP_MFI_DEBUG(unsigned int level, const char *fmt, ...);
 */
#ifdef ESP_MFI_DEBUG_ENABLE
/*
#define ESP_MFI_DEBUG(l, fmt, ...)                                                          \
    {                                                                                       \
        uint32_t __color_LINE;                                                              \
        if (l > esp_mfi_get_debug_level(l, &__color_LINE)) {                                \
            printf("\e[1;%dm" fmt "\e[0m" ESP_MFI_DEBUG_FL,                         \
                                __color_LINE,  ##__VA_ARGS__);                              \
        }                                                                                   \
    }
#define ESP_MFI_DEBUG_INTR(l, fmt, ...)                                                          \
    {                                                                                       \
        uint32_t __color_LINE;                                                              \
        if (l > esp_mfi_get_debug_level(l, &__color_LINE)) {                                \
            ets_printf("\e[1;%dm" fmt "\e[0m" ESP_MFI_DEBUG_FL,                         \
                                __color_LINE,  ##__VA_ARGS__);                              \
        }                                                                                   \
    }

#define ESP_MFI_DEBUG(l, fmt, ...)                                                          \
    {                                                                                       \
        uint32_t __color_LINE;                                                              \
        if (l > esp_mfi_get_debug_level(l, &__color_LINE)) {                                \
            printf("[%7lu] " fmt "\n", ((unsigned long) (esp_timer_get_time() / 1000ULL)), ##__VA_ARGS__);                              \
        }                                                                                   \
    }
#define ESP_MFI_DEBUG_INTR(l, fmt, ...)                                                          \
    {                                                                                       \
        uint32_t __color_LINE;                                                              \
        if (l > esp_mfi_get_debug_level(l, &__color_LINE)) {                                \
        	printf("[%7lu] " fmt "\n", ((unsigned long) (esp_timer_get_time() / 1000ULL)), ##__VA_ARGS__);                           \
        }                                                                                   \
    }*/

#define ESP_MFI_DEBUG(l, fmt, ...)                                                          \
    {                                                                                       \
            printf("[%7lu] " fmt "\n", ((unsigned long) (esp_timer_get_time() / 1000ULL)), ##__VA_ARGS__);                              \
    }
#define ESP_MFI_DEBUG_INTR(l, fmt, ...)                                                          \
    {                                                                                       \
        	printf("[%7lu] " fmt "\n", ((unsigned long) (esp_timer_get_time() / 1000ULL)), ##__VA_ARGS__);                           \
    }
#else /* ESP_MFI_DEBUG_ENABLE */
#define ESP_MFI_DEBUG(l, fmt, ...)
#define ESP_MFI_DEBUG_INTR(l, fmt, ...)
#endif /* ESP_MFI_DEBUG_ENABLE */

/**
 * @bref if the signal is false(0) do something depended on configuration
 *
 * @param conditions checking conditions
 *
 * @return none
 *
 * void ESP_MFI_ASSERT(int conditions);
 */
#ifdef ESP_MFI_ASSERT_ENABLE
#define ESP_MFI_ASSERT(cond)                                                                \
{                                                                                           \
    if (!(cond)) {                                                                          \
        printf("\e[1;33m" "ESP_MFI assert file %s line %d" ESP_MFI_DEBUG_FL,        \
                                __FILE__, __LINE__);                                        \
        ESP_MFI_ASSERT_BLOCK();                                                             \
    }                                                                                       \
}
#else
#define ESP_MFI_ASSERT(cond)
#endif

/**
 * @bref check if data length matches given length
 *
 * @param optlen data length
 * @param opttype data
 *
 * @return none
 *
 * void ESP_MFI_CHECK_OPTLEN(int conditions, (all type) data);
 */
#define ESP_MFI_CHECK_OPTLEN(optlen, opttype)                                               \
    do {                                                                                    \
        if ((optlen) > sizeof(opttype)) {                                                   \
            return -1;                                                                      \
        }                                                                                   \
    } while(0)                                                                              \

#ifdef __cplusplus
}
#endif

#endif /* ESP_MFI_DEBUG_H_ */
