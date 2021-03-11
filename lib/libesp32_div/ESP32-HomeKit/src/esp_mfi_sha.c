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

#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"
#include "mbedtls/sha512.h"

#include "esp_mfi_sha.h"

#ifdef CONFIG_IDF_TARGET_ESP8266
#define mbedtls_sha512_starts mbedtls_sha512_starts_ret
#define mbedtls_sha512_update mbedtls_sha512_update_ret
#define mbedtls_sha512_finish mbedtls_sha512_finish_ret
#endif

/**
 * @bref Create SHA1 context
 *
 * @param none
 *
 * @return the SHA1 context point
 */
esp_mfi_sha_ctx_t esp_mfi_sha1_new(void)
{
    mbedtls_sha1_context *context = NULL;
    context = (mbedtls_sha1_context *) malloc(sizeof(mbedtls_sha1_context));
    mbedtls_sha1_init(context);
    return context;
}

/**
 * @bref Free SHA1 context
 *
 * @param ctx the SHA1 context point
 *
 * @return none
 */
void esp_mfi_sha1_free(esp_mfi_sha_ctx_t ctx)
{
    if (ctx) {
        mbedtls_sha1_free( (mbedtls_sha1_context *)ctx);
        free(ctx);
    }
}

/**
 * @bref Init SHA1 context
 *
 * @param ctx the SHA1 context point
 *
 * @return none
 */
void esp_mfi_sha1_init(esp_mfi_sha_ctx_t ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_sha1_starts((mbedtls_sha1_context *) ctx);
}

/**
 * @bref Update SHA1 context
 *
 * @param ctx the SHA1 context point
 *        msg input data point
 *        len input data length
 *
 * @return none
 */
void esp_mfi_sha1_update(esp_mfi_sha_ctx_t ctx, const uint8_t * msg, int len)
{
    if (ctx == NULL || msg == NULL)
        return;

    mbedtls_sha1_update((mbedtls_sha1_context *) ctx, msg, len);
}

/**
 * @bref Final SHA1 context
 *
 * @param digest output data point
 *        ctx the SHA1 context point
 *
 * @return none
 */
void esp_mfi_sha1_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest)
{
    if (ctx == NULL || digest == NULL)
        return;

    mbedtls_sha1_finish((mbedtls_sha1_context *) ctx, digest);
}

/*
 * SHA256 declarations
 */

/**
 * @bref Create SHA256 context
 *
 * @param none
 *
 * @return the SHA256 context point
 */
esp_mfi_sha_ctx_t esp_mfi_sha256_new(void)
{
    mbedtls_sha256_context *context = NULL;
    context = (mbedtls_sha256_context *) malloc(sizeof(mbedtls_sha256_context));
    mbedtls_sha256_init(context);
    return context;
}

/**
 * @bref Free SHA256 context
 *
 * @param ctx the SHA256 context point
 *
 * @return none
 */
void esp_mfi_sha256_free(esp_mfi_sha_ctx_t ctx)
{
    if (ctx) {
        mbedtls_sha256_free((mbedtls_sha256_context *)ctx);
        free(ctx);
    }
}

/**
 * @bref Init SHA256 context
 *
 * @param ctx the SHA256 context point
 *
 * @return none
 */
void esp_mfi_sha256_init(esp_mfi_sha_ctx_t ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_sha256_starts((mbedtls_sha256_context *) ctx, 0);
}

/**
 * @bref Update SHA256 context
 *
 * @param ctx the SHA256 context point
 *        msg input data point
 *        len input data length
 *
 * @return none
 */
void esp_mfi_sha256_update(esp_mfi_sha_ctx_t ctx, const uint8_t *input, int len)
{
    if (ctx == NULL || input == NULL)
        return;

    mbedtls_sha256_update((mbedtls_sha256_context *) ctx, input, len);
}

/**
 * @bref Final SHA256 context
 *
 * @param digest output data point
 *        ctx the SHA256 context point
 *
 * @return none
 */
void esp_mfi_sha256_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest)
{
    if (ctx == NULL || digest == NULL)
        return;

    mbedtls_sha256_finish((mbedtls_sha256_context *) ctx, digest);
}

/*
 * SHA512 declarations
 */

/**
 * @bref Create SHA512 context
 *
 * @param none
 *
 * @return the SHA512 context point
 */
esp_mfi_sha_ctx_t esp_mfi_sha512_new(void)
{
    mbedtls_sha512_context *context = NULL;
    context = (mbedtls_sha512_context *) malloc(sizeof(mbedtls_sha512_context));
    mbedtls_sha512_init(context);
    return context;
}

/**
 * @bref Free SHA512 context
 *
 * @param ctx the SHA512 context point
 *
 * @return none
 */
void esp_mfi_sha512_free(esp_mfi_sha_ctx_t ctx)
{
    if (ctx) {
        mbedtls_sha512_free((mbedtls_sha512_context *)ctx);
        free(ctx);
    }
}

/**
 * @bref Init SHA512 context
 *
 * @param ctx the SHA512 context point
 *
 * @return none
 */
void esp_mfi_sha512_init(esp_mfi_sha_ctx_t ctx)
{
    if (ctx == NULL)
        return;

    mbedtls_sha512_starts((mbedtls_sha512_context *) ctx, 0);
}

/**
 * @bref Update SHA512 context
 *
 * @param ctx the SHA512 context point
 *        msg input data point
 *        len input data length
 *
 * @return none
 */
void esp_mfi_sha512_update(esp_mfi_sha_ctx_t ctx, const uint8_t *input, int len)
{
    if (ctx == NULL || input == NULL)
        return;

    mbedtls_sha512_update((mbedtls_sha512_context *) ctx, input, len);
}

/**
 * @bref Final SHA512 context
 *
 * @param digest output data point
 *        ctx the SHA512 context point
 *
 * @return none
 */
void esp_mfi_sha512_final(esp_mfi_sha_ctx_t ctx, uint8_t *digest)
{
    if (ctx == NULL || digest == NULL)
        return;

    mbedtls_sha512_finish((mbedtls_sha512_context *) ctx, digest);
}
