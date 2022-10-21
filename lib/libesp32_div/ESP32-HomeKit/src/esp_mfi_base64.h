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
#ifndef ESP_MFI_BASE64_H_
#define ESP_MFI_BASE64_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief transform bin data to base64 data
 *
 * @param src input data point
 * @param len input data length
 * @param dest output data point
 * @param dest_len output data buffer length
 * @param out_len output data length
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_base64_encode(const char *src, int len, char *dest, int dest_len, int *out_len);

/**
 * @brief transform base64 data to bin data
 *
 * @param src input data point
 * @param len input data length
 * @param dest output data point
 * @param dest_len output data buffer length
 * @param out_len output data length
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_base64_decode(const char *src, int len, char *dest, int dest_len, int *out_len);

#ifdef __cplusplus
}
#endif

#endif /* ESP_MFI_BASE64_H_ */
