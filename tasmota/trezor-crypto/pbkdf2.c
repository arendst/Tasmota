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

#include <string.h>
#include "pbkdf2.h"
#include "hmac.h"
#include "sha2.h"
#include "macros.h"

void pbkdf2_hmac_sha256_Init(PBKDF2_HMAC_SHA256_CTX *pctx, const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen)
{
	SHA256_CTX ctx;
	uint32_t blocknr = 1;
#if BYTE_ORDER == LITTLE_ENDIAN
	REVERSE32(blocknr, blocknr);
#endif

	hmac_sha256_prepare(pass, passlen, pctx->odig, pctx->idig);
	memset(pctx->g, 0, sizeof(pctx->g));
	pctx->g[8] = 0x80000000;
	pctx->g[15] = (SHA256_BLOCK_LENGTH + SHA256_DIGEST_LENGTH) * 8;

	memcpy (ctx.state, pctx->idig, sizeof(pctx->idig));
	ctx.bitcount = SHA256_BLOCK_LENGTH * 8;
	sha256_Update(&ctx, salt, saltlen);
	sha256_Update(&ctx, (uint8_t*)&blocknr, sizeof(blocknr));
	sha256_Final(&ctx, (uint8_t*)pctx->g);
#if BYTE_ORDER == LITTLE_ENDIAN
	for (uint32_t k = 0; k < SHA256_DIGEST_LENGTH / sizeof(uint32_t); k++) {
		REVERSE32(pctx->g[k], pctx->g[k]);
	}
#endif
	sha256_Transform(pctx->odig, pctx->g, pctx->g);
	memcpy(pctx->f, pctx->g, SHA256_DIGEST_LENGTH);
	pctx->first = 1;
}

void pbkdf2_hmac_sha256_Update(PBKDF2_HMAC_SHA256_CTX *pctx, uint32_t iterations)
{
	for (uint32_t i = pctx->first; i < iterations; i++) {
		sha256_Transform(pctx->idig, pctx->g, pctx->g);
		sha256_Transform(pctx->odig, pctx->g, pctx->g);
		for (uint32_t j = 0; j < SHA256_DIGEST_LENGTH/sizeof(uint32_t); j++) {
			pctx->f[j] ^= pctx->g[j];
		}
	}
	pctx->first = 0;
}

void pbkdf2_hmac_sha256_Final(PBKDF2_HMAC_SHA256_CTX *pctx, uint8_t *key)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	for (uint32_t k = 0; k < SHA256_DIGEST_LENGTH/sizeof(uint32_t); k++) {
		REVERSE32(pctx->f[k], pctx->f[k]);
	}
#endif
	memcpy(key, pctx->f, SHA256_DIGEST_LENGTH);
	MEMSET_BZERO(pctx, sizeof(PBKDF2_HMAC_SHA256_CTX));
}

void pbkdf2_hmac_sha256(const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen, uint32_t iterations, uint8_t *key)
{
	PBKDF2_HMAC_SHA256_CTX pctx;
	pbkdf2_hmac_sha256_Init(&pctx, pass, passlen, salt, saltlen);
	pbkdf2_hmac_sha256_Update(&pctx, iterations);
	pbkdf2_hmac_sha256_Final(&pctx, key);
}

void pbkdf2_hmac_sha512_Init(PBKDF2_HMAC_SHA512_CTX *pctx, const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen)
{
	SHA512_CTX ctx;
	uint32_t blocknr = 1;
#if BYTE_ORDER == LITTLE_ENDIAN
	REVERSE32(blocknr, blocknr);
#endif

	hmac_sha512_prepare(pass, passlen, pctx->odig, pctx->idig);
	memset(pctx->g, 0, sizeof(pctx->g));
	pctx->g[8] = 0x8000000000000000;
	pctx->g[15] = (SHA512_BLOCK_LENGTH + SHA512_DIGEST_LENGTH) * 8;

	memcpy (ctx.state, pctx->idig, sizeof(pctx->idig));
	ctx.bitcount[0] = SHA512_BLOCK_LENGTH * 8;
	ctx.bitcount[1] = 0;
	sha512_Update(&ctx, salt, saltlen);
	sha512_Update(&ctx, (uint8_t*)&blocknr, sizeof(blocknr));
	sha512_Final(&ctx, (uint8_t*)pctx->g);
#if BYTE_ORDER == LITTLE_ENDIAN
	for (uint32_t k = 0; k < SHA512_DIGEST_LENGTH / sizeof(uint64_t); k++) {
		REVERSE64(pctx->g[k], pctx->g[k]);
	}
#endif
	sha512_Transform(pctx->odig, pctx->g, pctx->g);
	memcpy(pctx->f, pctx->g, SHA512_DIGEST_LENGTH);
	pctx->first = 1;
}

void pbkdf2_hmac_sha512_Update(PBKDF2_HMAC_SHA512_CTX *pctx, uint32_t iterations)
{
	for (uint32_t i = pctx->first; i < iterations; i++) {
		sha512_Transform(pctx->idig, pctx->g, pctx->g);
		sha512_Transform(pctx->odig, pctx->g, pctx->g);
		for (uint32_t j = 0; j < SHA512_DIGEST_LENGTH / sizeof(uint64_t); j++) {
			pctx->f[j] ^= pctx->g[j];
		}
	}
	pctx->first = 0;
}

void pbkdf2_hmac_sha512_Final(PBKDF2_HMAC_SHA512_CTX *pctx, uint8_t *key)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	for (uint32_t k = 0; k < SHA512_DIGEST_LENGTH/sizeof(uint64_t); k++) {
		REVERSE64(pctx->f[k], pctx->f[k]);
	}
#endif
	memcpy(key, pctx->f, SHA512_DIGEST_LENGTH);
	MEMSET_BZERO(pctx, sizeof(PBKDF2_HMAC_SHA512_CTX));
}

void pbkdf2_hmac_sha512(const uint8_t *pass, int passlen, const uint8_t *salt, int saltlen, uint32_t iterations, uint8_t *key)
{
	PBKDF2_HMAC_SHA512_CTX pctx;
	pbkdf2_hmac_sha512_Init(&pctx, pass, passlen, salt, saltlen);
	pbkdf2_hmac_sha512_Update(&pctx, iterations);
	pbkdf2_hmac_sha512_Final(&pctx, key);
}
