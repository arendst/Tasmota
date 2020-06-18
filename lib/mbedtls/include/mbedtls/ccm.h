/**
 * \file ccm.h
 *
 * \brief Counter with CBC-MAC (CCM) for 128-bit block ciphers
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_CCM_H
#define MBEDTLS_CCM_H

#include "cipher.h"

#define MBEDTLS_ERR_CCM_BAD_INPUT      -0x000D /**< Bad input parameters to function. */
#define MBEDTLS_ERR_CCM_AUTH_FAILED    -0x000F /**< Authenticated decryption failed. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          CCM context structure
 */
typedef struct {
    mbedtls_cipher_context_t cipher_ctx;    /*!< cipher context used */
}
mbedtls_ccm_context;

/**
 * \brief           Initialize CCM context (just makes references valid)
 *                  Makes the context ready for mbedtls_ccm_setkey() or
 *                  mbedtls_ccm_free().
 *
 * \param ctx       CCM context to initialize
 */
void mbedtls_ccm_init( mbedtls_ccm_context *ctx );

/**
 * \brief           CCM initialization (encryption and decryption)
 *
 * \param ctx       CCM context to be initialized
 * \param cipher    cipher to use (a 128-bit block cipher)
 * \param key       encryption key
 * \param keybits   key size in bits (must be acceptable by the cipher)
 *
 * \return          0 if successful, or a cipher specific error code
 */
int mbedtls_ccm_setkey( mbedtls_ccm_context *ctx,
                        mbedtls_cipher_id_t cipher,
                        const unsigned char *key,
                        unsigned int keybits );

/**
 * \brief           Free a CCM context and underlying cipher sub-context
 *
 * \param ctx       CCM context to free
 */
void mbedtls_ccm_free( mbedtls_ccm_context *ctx );

/**
 * \brief           CCM buffer encryption
 *
 * \param ctx       CCM context
 * \param length    length of the input data in bytes
 * \param iv        nonce (initialization vector)
 * \param iv_len    length of IV in bytes
 *                  must be 2, 3, 4, 5, 6, 7 or 8
 * \param add       additional data
 * \param add_len   length of additional data in bytes
 *                  must be less than 2^16 - 2^8
 * \param input     buffer holding the input data
 * \param output    buffer for holding the output data
 *                  must be at least 'length' bytes wide
 * \param tag       buffer for holding the tag
 * \param tag_len   length of the tag to generate in bytes
 *                  must be 4, 6, 8, 10, 14 or 16
 *
 * \note            The tag is written to a separate buffer. To get the tag
 *                  concatenated with the output as in the CCM spec, use
 *                  tag = output + length and make sure the output buffer is
 *                  at least length + tag_len wide.
 *
 * \return          0 if successful
 */
int mbedtls_ccm_encrypt_and_tag( mbedtls_ccm_context *ctx, size_t length,
                         const unsigned char *iv, size_t iv_len,
                         const unsigned char *add, size_t add_len,
                         const unsigned char *input, unsigned char *output,
                         unsigned char *tag, size_t tag_len );

/**
 * \brief           CCM buffer authenticated decryption
 *
 * \param ctx       CCM context
 * \param length    length of the input data
 * \param iv        initialization vector
 * \param iv_len    length of IV
 * \param add       additional data
 * \param add_len   length of additional data
 * \param input     buffer holding the input data
 * \param output    buffer for holding the output data
 * \param tag       buffer holding the tag
 * \param tag_len   length of the tag
 *
 * \return         0 if successful and authenticated,
 *                 MBEDTLS_ERR_CCM_AUTH_FAILED if tag does not match
 */
int mbedtls_ccm_auth_decrypt( mbedtls_ccm_context *ctx, size_t length,
                      const unsigned char *iv, size_t iv_len,
                      const unsigned char *add, size_t add_len,
                      const unsigned char *input, unsigned char *output,
                      const unsigned char *tag, size_t tag_len );

#if defined(MBEDTLS_SELF_TEST) && defined(MBEDTLS_AES_C)
/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int mbedtls_ccm_self_test( int verbose );
#endif /* MBEDTLS_SELF_TEST && MBEDTLS_AES_C */

#ifdef __cplusplus
}
#endif

#endif /* MBEDTLS_CCM_H */
