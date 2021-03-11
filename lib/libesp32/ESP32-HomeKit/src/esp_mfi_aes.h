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
#ifndef ESP_MFI_AES_H_
#define ESP_MFI_AES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MFI_AES_CTR_SIZE 16

typedef void* esp_mfi_aes_ctr_t;

/**
 * @brief Create AES context
 *
 * @return pointer of the AES context
 */
esp_mfi_aes_ctr_t esp_mfi_aes_ctr_new(void);

/**
 * @brief Initialize AES context, including the key and nonce
 *
 * @param incontext pointer of the AES context
 * @param inkey AES key
 * @param innonce AES nonce
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_aes_ctr_init(esp_mfi_aes_ctr_t incontext, const uint8_t inkey[MFI_AES_CTR_SIZE], const uint8_t innonce[MFI_AES_CTR_SIZE]);

/**
 * @brief Update AES context
 *
 * @param incontext  AES context point
 * @param insrc  pointer of the source data to be updated
 * @param insrclen  source data length
 * @param indst  pointer of the updated data
 *
 * @return
 *     - 0 : succeed
 *     - others : fail
 */
int esp_mfi_aes_ctr_update(esp_mfi_aes_ctr_t incontext, const void *insrc, uint16_t insrclen, void *indst);

/**
 * @brief Destroy AES context pointer
 *
 * @param incontext  pointer of the AES context
 */
void esp_mfi_aes_ctr_final(esp_mfi_aes_ctr_t incontext);

#ifdef __cplusplus
}
#endif

#endif /* ESP_MFI_AES_H_ */
