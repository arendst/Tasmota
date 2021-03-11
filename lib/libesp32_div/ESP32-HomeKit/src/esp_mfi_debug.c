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

#include <stdio.h>
#include <hap.h>
#include <esp_mfi_debug.h>

#define RED_CHAR    31
#define GREEN_CHAR  32
#define YELLOW_CHAR 33
#define BLUE_CHAR   34
#define CYAN_CHAR   36
#define WHITE_CHAR  37

#ifdef CONFIG_MFI_DEBUG_LEVEL_INIT
#define MFI_DEBUG_LEVEL_INIT CONFIG_MFI_DEBUG_LEVEL_INIT
#else /* CONFIG_MFI_DEBUG_LEVEL_INIT */
#define MFI_DEBUG_LEVEL_INIT 0
#endif /* CONFIG_MFI_DEBUG_LEVEL_INIT */

static uint32_t mfi_debug_level = MFI_DEBUG_LEVEL_INIT;

/**
 * @bref set the MFI debugging level
 */
int esp_mfi_set_debug_level(uint32_t level)
{
    mfi_debug_level = level;
    return 0;
}

/**
 * @bref get the MFI debugging level and output data color
 */
uint32_t esp_mfi_get_debug_level(uint32_t level, uint32_t *color)
{
    const uint32_t prospect_table[] = {
        0,              /* no                                   */
        WHITE_CHAR,     /* information: 1, color(37: white)     */
        GREEN_CHAR,     /* warn       : 3, color(32: green)     */
        RED_CHAR,       /* error      : 3, color(31: red)       */
        YELLOW_CHAR,    /* assert     : 4, color(33: yellow)    */
        CYAN_CHAR       /* block      : 5, color(36: cyan)      */
                        /* others     : n, color(34: blue)      */
    };
    const uint32_t prospect_table_size = sizeof(prospect_table) / sizeof(prospect_table[0]);

    if (level < prospect_table_size)
        *color = prospect_table[level];
    else
        *color = BLUE_CHAR;

    return mfi_debug_level;
}

void hap_set_debug_level(hap_debug_level_t level)
{
    esp_mfi_set_debug_level(level);
}
