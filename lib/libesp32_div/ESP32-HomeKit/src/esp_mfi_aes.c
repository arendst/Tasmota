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

#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "mbedtls/aes.h"
#include "esp_log.h"

#include "esp_mfi_aes.h"

static const char* TAG = "mfi_aes_adapter";

/*!
 * @group AES 128-bit Counter Mode API

 * @abstract API to encrypt or decrypt using AES-128 in counter mode.
 *
 * Call esp_mfi_aes_ctr_init to initialize the context. Don't use the context until it has been initialized.
 * Call esp_mfi_aes_ctr_update to encrypt or decrypt N bytes of input and generate N bytes of output.
 * Call esp_mfi_aes_ctr_final to finalize the context. After finalizing, you must call AES_CTR_Init to use it again.
*/

typedef struct {
    uint8_t key[MFI_AES_CTR_SIZE];
    uint8_t nonce[MFI_AES_CTR_SIZE];
} aes_ctr_context_t;

/**
 * @bref Create AES context
 *
 * @param none
 *
 * @return the AES context point
 */
esp_mfi_aes_ctr_t esp_mfi_aes_ctr_new(void)
{
    aes_ctr_context_t *context = NULL;
    context = (aes_ctr_context_t *) malloc(sizeof(aes_ctr_context_t));
    return context;
}

/**
 * @bref Initialize AES context, include initialize the key and nonce
 *
 * @param incontext AES context point
 *        inkey AES key
 *        innonce AES nonce
 *
 * @return the result
 *      0 : sucessful
 *      others : failed
 */
int esp_mfi_aes_ctr_init(esp_mfi_aes_ctr_t incontext, const uint8_t inkey[MFI_AES_CTR_SIZE], const uint8_t innonce[MFI_AES_CTR_SIZE])
{
    int ret = 0;

    if (incontext == NULL)
        return -EINVAL;

    aes_ctr_context_t *context = incontext;

    memcpy(context->key, inkey, MFI_AES_CTR_SIZE);
    memcpy(context->nonce, innonce, MFI_AES_CTR_SIZE);

    return ret;
}

/**
 * @bref Update AES context
 *
 * @param incontext AES context point
 *        insrc the data point of update
 *        insrclen the data length
 *        indst the data pint of output
 *
 * @return the result
 *      0 : sucessful
 *      others : failed
 */
int esp_mfi_aes_ctr_update(esp_mfi_aes_ctr_t incontext, const void *insrc, uint16_t insrclen, void *indst)
{
    int ret = 0;

    if (incontext == NULL)
        return -EINVAL;

    aes_ctr_context_t *context = incontext;
    size_t offset = 0;
    uint8_t stream_block[MFI_AES_CTR_SIZE];
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);

    ret = mbedtls_aes_setkey_enc( &ctx, context->key, 128);

    if (ret != 0) {
        ESP_LOGE(TAG, "mfi aes setkey[%d]", ret);
        ret = -EINVAL;
    } else {
        ret = mbedtls_aes_crypt_ctr(&ctx, insrclen, &offset, context->nonce, stream_block, (uint8_t *) insrc, indst);
        if (ret != 0) {
            ESP_LOGE(TAG, "mfi aes crypt[%d]", ret);
            ret = -EINVAL;
        }
    }

    return ret;
}

/**
 * @bref Destory AES context point
 *
 * @param incontext AES context point
 *
 * @return none
 */
void esp_mfi_aes_ctr_final(esp_mfi_aes_ctr_t incontext)
{
    if (incontext) {
        memset((aes_ctr_context_t *) incontext, 0, sizeof(aes_ctr_context_t));
        free(incontext);
    }
}
