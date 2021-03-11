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

#ifndef ESP_MFI_SHA_H_
#define ESP_MFI_SHA_H_

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef void* esp_mfi_sha_ctx_t;

/*
 * SHA1 declarations
 */

#define MFI_SHA1_SIZE   20

/**
 * @brief Create SHA1 context
 *
 * @return pointer of the SHA1 context
 */
esp_mfi_sha_ctx_t esp_mfi_sha1_new(void);

/**
 * @brief Init SHA1 context
 *
 * @param ctx pointer of the SHA1 context
 */
void esp_mfi_sha1_init(esp_mfi_sha_ctx_t ctx);

/**
 * @brief Update SHA1 context
 *
 * @param ctx pointer of the SHA1 context
 * @param msg pointer of the input data
 * @param len input data length
 */
void esp_mfi_sha1_update(esp_mfi_sha_ctx_t ctx, const uint8_t *msg, int len);

/**
 * @brief Final SHA1 context
 *
 * @param digest pointer of output data
 * @param ctx pointer of the SHA1 context
 */
void esp_mfi_sha1_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest);

/**
 * @brief Free SHA1 context
 *
 * @param ctx pointer of the SHA1 context
 */
void esp_mfi_sha1_free(esp_mfi_sha_ctx_t ctx);

/*
 * SHA256 declarations
 */

#define MFI_SHA256_SIZE 32

/**
 * @brief Create SHA256 context
 *
 * @return pointer of the SHA256 context
 */
esp_mfi_sha_ctx_t esp_mfi_sha256_new(void);

/**
 * @brief Init SHA256 context
 *
 * @param ctx pointer of the SHA256 context
 */
void esp_mfi_sha256_init(esp_mfi_sha_ctx_t ctx);

/**
 * @brief Update SHA256 context
 *
 * @param ctx pointer of the SHA256 context
 * @param msg pointer of input data
 * @param len input data length
 */
void esp_mfi_sha256_update(esp_mfi_sha_ctx_t ctx, const uint8_t *input, int len);

/**
 * @brief Final SHA256 context
 *
 * @param digest pointer of output data
 * @param ctx pointer of the SHA256 context
 */
void esp_mfi_sha256_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest);

/**
 * @brief Free SHA256 context
 *
 * @param ctx pointer of the SHA256 context
 */
void esp_mfi_sha256_free(esp_mfi_sha_ctx_t ctx);

/*
 * SHA512 declarations
 */

#define MFI_SHA512_SIZE 64

/**
 * @brief Create SHA512 context
 *
 * @return pointer of the SHA512 context
 */
esp_mfi_sha_ctx_t esp_mfi_sha512_new(void);

/**
 * @brief Init SHA512 context
 *
 * @param ctx pointer of the SHA512 context
 */
void esp_mfi_sha512_init(esp_mfi_sha_ctx_t ctx);

/**
 * @brief Update SHA512 context
 *
 * @param ctx pointer of the SHA512 context
 * @param msg pointer of input data
 * @param len input data length
 */
void esp_mfi_sha512_update(esp_mfi_sha_ctx_t ctx, const uint8_t *input, int len);

/**
 * @brief Final SHA512 context
 *
 * @param digest pointer of output data
 * @param ctx pointer of the SHA512 context
 */
void esp_mfi_sha512_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest);

/**
 * @brief Free SHA512 context
 *
 * @param ctx pointer of the SHA512 context
 */
void esp_mfi_sha512_free(esp_mfi_sha_ctx_t ctx);

#ifdef __cplusplus
}
#endif

#endif /* ESP_MFI_SHA_H_ */
