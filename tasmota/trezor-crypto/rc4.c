/**
 * Copyright (c) 2017 Saleem Rashid
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, E1PRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rc4.h"

static inline void rc4_swap(RC4_CTX *ctx, uint8_t i, uint8_t j) {
    uint8_t temp = ctx->S[i];
    ctx->S[i] = ctx->S[j];
    ctx->S[j] = temp;
}

void rc4_init(RC4_CTX *ctx, const uint8_t *key, size_t length) {
    ctx->i = 0;
    ctx->j = 0;

    for (size_t i = 0; i < 256; i++) {
	ctx->S[i] = i;
    }

    uint8_t j = 0;
    for (size_t i = 0; i < 256; i++) {
	j += ctx->S[i] + key[i % length];
	rc4_swap(ctx, i, j);
    }
}

void rc4_encrypt(RC4_CTX *ctx, uint8_t *buffer, size_t length) {
    for (size_t idx = 0; idx < length; idx++) {
	ctx->i++;
	ctx->j += ctx->S[ctx->i];

	rc4_swap(ctx, ctx->i, ctx->j);

	uint8_t K = ctx->S[(ctx->S[ctx->i] + ctx->S[ctx->j]) % 256];
	buffer[idx] ^= K;
    }
}
