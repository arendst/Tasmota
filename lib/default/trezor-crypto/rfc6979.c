/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 * Copyright (c)      2015 Jochen Hoenicke
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
#include "rfc6979.h"
#include "hmac.h"
#include "macros.h"

void init_rfc6979(const uint8_t *priv_key, const uint8_t *hash, rfc6979_state *state) {
	uint8_t bx[2*32];
	uint8_t buf[32 + 1 + 2*32];

	memcpy(bx, priv_key, 32);
	memcpy(bx+32, hash, 32);

	memset(state->v, 1, sizeof(state->v));
	memset(state->k, 0, sizeof(state->k));

	memcpy(buf, state->v, sizeof(state->v));
	buf[sizeof(state->v)] = 0x00;
	memcpy(buf + sizeof(state->v) + 1, bx, 64);
	hmac_sha256(state->k, sizeof(state->k), buf, sizeof(buf), state->k);
	hmac_sha256(state->k, sizeof(state->k), state->v, sizeof(state->v), state->v);

	memcpy(buf, state->v, sizeof(state->v));
	buf[sizeof(state->v)] = 0x01;
	memcpy(buf + sizeof(state->v) + 1, bx, 64);
	hmac_sha256(state->k, sizeof(state->k), buf, sizeof(buf), state->k);
	hmac_sha256(state->k, sizeof(state->k), state->v, sizeof(state->v), state->v);

	MEMSET_BZERO(bx, sizeof(bx));
	MEMSET_BZERO(buf, sizeof(buf));
}

// generate next number from deterministic random number generator
void generate_rfc6979(uint8_t rnd[32], rfc6979_state *state)
{
	uint8_t buf[32 + 1];

	hmac_sha256(state->k, sizeof(state->k), state->v, sizeof(state->v), state->v);
	memcpy(buf, state->v, sizeof(state->v));
	buf[sizeof(state->v)] = 0x00;
	hmac_sha256(state->k, sizeof(state->k), buf, sizeof(state->v) + 1, state->k);
	hmac_sha256(state->k, sizeof(state->k), state->v, sizeof(state->v), state->v);
	memcpy(rnd, buf, 32);
	MEMSET_BZERO(buf, sizeof(buf));
}

// generate K in a deterministic way, according to RFC6979
// http://tools.ietf.org/html/rfc6979
void generate_k_rfc6979(bignum256 *k, rfc6979_state *state)
{
	uint8_t buf[32];
	generate_rfc6979(buf, state);
	bn_read_be(buf, k);
	MEMSET_BZERO(buf, sizeof(buf));
}
