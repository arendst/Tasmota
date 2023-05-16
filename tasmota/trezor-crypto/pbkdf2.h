/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __PBKDF2_H__
#define __PBKDF2_H__

#include <stdint.h>
#include "sha2.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _PBKDF2_HMAC_SHA256_CTX {
	uint32_t odig[SHA256_DIGEST_LENGTH / sizeof(uint32_t)];
	uint32_t idig[SHA256_DIGEST_LENGTH / sizeof(uint32_t)];
	uint32_t f[SHA256_DIGEST_LENGTH / sizeof(uint32_t)];
	uint32_t g[SHA256_BLOCK_LENGTH / sizeof(uint32_t)];
	char first;
} PBKDF2_HMAC_SHA256_CTX;

typedef struct _PBKDF2_HMAC_SHA512_CTX {
	uint64_t odig[SHA512_DIGEST_LENGTH / sizeof(uint64_t)];
	uint64_t idig[SHA512_DIGEST_LENGTH / sizeof(uint64_t)];
	uint64_t f[SHA512_DIGEST_LENGTH / sizeof(uint64_t)];
	uint64_t g[SHA512_BLOCK_LENGTH / sizeof(uint64_t)];
	char first;
} PBKDF2_HMAC_SHA512_CTX;

void pbkdf2_hmac_sha256_Init(PBKDF2_HMAC_SHA256_CTX *pctx, const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen);
void pbkdf2_hmac_sha256_Update(PBKDF2_HMAC_SHA256_CTX *pctx, uint32_t iterations);
void pbkdf2_hmac_sha256_Final(PBKDF2_HMAC_SHA256_CTX *pctx, uint8_t *key);
void pbkdf2_hmac_sha256(const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen, uint32_t iterations, uint8_t *key);

void pbkdf2_hmac_sha512_Init(PBKDF2_HMAC_SHA512_CTX *pctx, const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen);
void pbkdf2_hmac_sha512_Update(PBKDF2_HMAC_SHA512_CTX *pctx, uint32_t iterations);
void pbkdf2_hmac_sha512_Final(PBKDF2_HMAC_SHA512_CTX *pctx, uint8_t *key);
void pbkdf2_hmac_sha512(const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen, uint32_t iterations, uint8_t *key);
    
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
