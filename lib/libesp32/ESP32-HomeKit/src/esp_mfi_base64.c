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

#include <sys/errno.h>

#include "mbedtls/base64.h"

/**
 * @brief transform bin data to base64 data
 */
int esp_mfi_base64_encode(const char *src, int len, char *dest, int dest_len, int *out_len)
{
    int ret = mbedtls_base64_encode((unsigned char *)dest, dest_len, (size_t *)out_len, (unsigned char *)src, len);
    if (ret != 0){
        return -EINVAL;
    } else{
        return 0;
    }
}

/**
 * @brief transform base64 data to bin data
 */
int esp_mfi_base64_decode(const char *src, int len, char *dest, int dest_len, int *out_len)
{
    int ret = 0;
    ret = mbedtls_base64_decode((unsigned char *)dest, dest_len, (size_t *)out_len, (unsigned char *)src, len);
    if (ret != 0) {
        return -EINVAL;
    } else {
        return 0;
    }
}
