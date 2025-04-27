/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 * 3. Neither the name of "Floorsense Ltd", "Agile Workspace Ltd" nor the names of
 *  its contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Daniel Hope <daniel.hope@smartalock.com>
 */

// RFC7539 implementation of ChaCha20 with modified nonce size for WireGuard
// https://tools.ietf.org/html/rfc7539
// Adapted from https://cr.yp.to/streamciphers/timings/estreambench/submissions/salsa20/chacha8/ref/chacha.c by D. J. Bernstein (Public Domain)
// HChaCha20 is described here: https://tools.ietf.org/id/draft-arciszewski-xchacha-02.html
#ifndef _CHACHA20_H_
#define _CHACHA20_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CHACHA20_BLOCK_SIZE		(64)
#define CHACHA20_KEY_SIZE		(32)

struct chacha20_ctx {
	uint32_t state[16];
};

void chacha20_init(struct chacha20_ctx *ctx, const uint8_t *key, const uint64_t nonce);
void chacha20(struct chacha20_ctx *ctx, uint8_t *out, const uint8_t *in, uint32_t len);
void hchacha20(uint8_t *out, const uint8_t *nonce, const uint8_t *key);

#ifdef __cplusplus
}
#endif

#endif /* _CHACHA20_H_ */
