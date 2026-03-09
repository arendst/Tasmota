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

#ifndef _CHACHA20POLY1305_H_
#define _CHACHA20POLY1305_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Aead(key, counter, plain text, auth text) ChaCha20Poly1305 AEAD, as specified in RFC7539 [17], with its nonce being composed of 32 bits of zeros followed by the 64-bit little-endian value of counter.
// AEAD_CHACHA20_POLY1305 as described in https://tools.ietf.org/html/rfc7539
void chacha20poly1305_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, uint64_t nonce, const uint8_t *key);
bool chacha20poly1305_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, uint64_t nonce, const uint8_t *key);

// Xaead(key, nonce, plain text, auth text) XChaCha20Poly1305 AEAD, with a 24-byte random nonce, instantiated using HChaCha20 [6] and ChaCha20Poly1305.
// AEAD_XChaCha20_Poly1305 as described in https://tools.ietf.org/id/draft-arciszewski-xchacha-02.html
void xchacha20poly1305_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, const uint8_t *nonce, const uint8_t *key);
bool xchacha20poly1305_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, const uint8_t *nonce, const uint8_t *key);

#ifdef __cplusplus
}
#endif

#endif /* _CHACHA20POLY1305_H_ */
