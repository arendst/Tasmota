/* ec_dsa.c - TinyCrypt implementation of EC-DSA */

/* Copyright (c) 2014, Kenneth MacKay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.*/

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "../include/tinycrypt/constants.h"
#include "../include/tinycrypt/ecc.h"
#include "../include/tinycrypt/ecc_dsa.h"

#if default_RNG_defined
static uECC_RNG_Function g_rng_function = &default_CSPRNG;
#else
static uECC_RNG_Function g_rng_function = 0;
#endif

static void bits2int(uECC_word_t *native, const uint8_t *bits,
		     unsigned bits_size, uECC_Curve curve)
{
	unsigned num_n_bytes = BITS_TO_BYTES(curve->num_n_bits);
	unsigned num_n_words = BITS_TO_WORDS(curve->num_n_bits);
	int shift;
	uECC_word_t carry;
	uECC_word_t *ptr;

	if (bits_size > num_n_bytes) {
		bits_size = num_n_bytes;
	}

	uECC_vli_clear(native, num_n_words);
	uECC_vli_bytesToNative(native, bits, bits_size);
	if (bits_size * 8 <= (unsigned)curve->num_n_bits) {
		return;
	}
	shift = bits_size * 8 - curve->num_n_bits;
	carry = 0;
	ptr = native + num_n_words;
	while (ptr-- > native) {
		uECC_word_t temp = *ptr;
		*ptr = (temp >> shift) | carry;
		carry = temp << (uECC_WORD_BITS - shift);
	}

	/* Reduce mod curve_n */
	if (uECC_vli_cmp_unsafe(curve->n, native, num_n_words) != 1) {
		uECC_vli_sub(native, native, curve->n, num_n_words);
	}
}

int uECC_sign_with_k(const uint8_t *private_key, const uint8_t *message_hash,
		     unsigned hash_size, uECC_word_t *k, uint8_t *signature,
		     uECC_Curve curve)
{

	uECC_word_t tmp[NUM_ECC_WORDS];
	uECC_word_t s[NUM_ECC_WORDS];
	uECC_word_t *k2[2] = {tmp, s};
	uECC_word_t p[NUM_ECC_WORDS * 2];
	uECC_word_t carry;
	wordcount_t num_words = curve->num_words;
	wordcount_t num_n_words = BITS_TO_WORDS(curve->num_n_bits);
	bitcount_t num_n_bits = curve->num_n_bits;

	/* Make sure 0 < k < curve_n */
  	if (uECC_vli_isZero(k, num_words) ||
	    uECC_vli_cmp(curve->n, k, num_n_words) != 1) {
		return 0;
	}

	carry = regularize_k(k, tmp, s, curve);
	EccPoint_mult(p, curve->G, k2[!carry], 0, num_n_bits + 1, curve);
	if (uECC_vli_isZero(p, num_words)) {
		return 0;
	}

	/* If an RNG function was specified, get a random number
	to prevent side channel analysis of k. */
	if (!g_rng_function) {
		uECC_vli_clear(tmp, num_n_words);
		tmp[0] = 1;
	}
	else if (!uECC_generate_random_int(tmp, curve->n, num_n_words)) {
		return 0;
	}

	/* Prevent side channel analysis of uECC_vli_modInv() to determine
	bits of k / the private key by premultiplying by a random number */
	uECC_vli_modMult(k, k, tmp, curve->n, num_n_words); /* k' = rand * k */
	uECC_vli_modInv(k, k, curve->n, num_n_words);       /* k = 1 / k' */
	uECC_vli_modMult(k, k, tmp, curve->n, num_n_words); /* k = 1 / k */

	uECC_vli_nativeToBytes(signature, curve->num_bytes, p); /* store r */

	/* tmp = d: */
	uECC_vli_bytesToNative(tmp, private_key, BITS_TO_BYTES(curve->num_n_bits));

	s[num_n_words - 1] = 0;
	uECC_vli_set(s, p, num_words);
	uECC_vli_modMult(s, tmp, s, curve->n, num_n_words); /* s = r*d */

	bits2int(tmp, message_hash, hash_size, curve);
	uECC_vli_modAdd(s, tmp, s, curve->n, num_n_words); /* s = e + r*d */
	uECC_vli_modMult(s, s, k, curve->n, num_n_words);  /* s = (e + r*d) / k */
	if (uECC_vli_numBits(s, num_n_words) > (bitcount_t)curve->num_bytes * 8) {
		return 0;
	}

	uECC_vli_nativeToBytes(signature + curve->num_bytes, curve->num_bytes, s);
	return 1;
}

int uECC_sign(const uint8_t *private_key, const uint8_t *message_hash,
	      unsigned hash_size, uint8_t *signature, uECC_Curve curve)
{
	      uECC_word_t _random[2*NUM_ECC_WORDS];
	      uECC_word_t k[NUM_ECC_WORDS];
	      uECC_word_t tries;

	for (tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries) {
		/* Generating _random uniformly at random: */
		uECC_RNG_Function rng_function = uECC_get_rng();
		if (!rng_function ||
		    !rng_function((uint8_t *)_random, 2*NUM_ECC_WORDS*uECC_WORD_SIZE)) {
			return 0;
		}

		// computing k as modular reduction of _random (see FIPS 186.4 B.5.1):
		uECC_vli_mmod(k, _random, curve->n, BITS_TO_WORDS(curve->num_n_bits));

		if (uECC_sign_with_k(private_key, message_hash, hash_size, k, signature, 
		    curve)) {
			return 1;
		}
	}
	return 0;
}

static bitcount_t smax(bitcount_t a, bitcount_t b)
{
	return (a > b ? a : b);
}

int uECC_verify(const uint8_t *public_key, const uint8_t *message_hash,
		unsigned hash_size, const uint8_t *signature,
	        uECC_Curve curve)
{

	uECC_word_t u1[NUM_ECC_WORDS], u2[NUM_ECC_WORDS];
	uECC_word_t z[NUM_ECC_WORDS];
	uECC_word_t sum[NUM_ECC_WORDS * 2];
	uECC_word_t rx[NUM_ECC_WORDS];
	uECC_word_t ry[NUM_ECC_WORDS];
	uECC_word_t tx[NUM_ECC_WORDS];
	uECC_word_t ty[NUM_ECC_WORDS];
	uECC_word_t tz[NUM_ECC_WORDS];
	const uECC_word_t *points[4];
	const uECC_word_t *point;
	bitcount_t num_bits;
	bitcount_t i;

	uECC_word_t _public[NUM_ECC_WORDS * 2];
	uECC_word_t r[NUM_ECC_WORDS], s[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;
	wordcount_t num_n_words = BITS_TO_WORDS(curve->num_n_bits);

	rx[num_n_words - 1] = 0;
	r[num_n_words - 1] = 0;
	s[num_n_words - 1] = 0;

	uECC_vli_bytesToNative(_public, public_key, curve->num_bytes);
	uECC_vli_bytesToNative(_public + num_words, public_key + curve->num_bytes,
			       curve->num_bytes);
	uECC_vli_bytesToNative(r, signature, curve->num_bytes);
	uECC_vli_bytesToNative(s, signature + curve->num_bytes, curve->num_bytes);

	/* r, s must not be 0. */
	if (uECC_vli_isZero(r, num_words) || uECC_vli_isZero(s, num_words)) {
		return 0;
	}

	/* r, s must be < n. */
	if (uECC_vli_cmp_unsafe(curve->n, r, num_n_words) != 1 ||
	    uECC_vli_cmp_unsafe(curve->n, s, num_n_words) != 1) {
		return 0;
	}

	/* Calculate u1 and u2. */
	uECC_vli_modInv(z, s, curve->n, num_n_words); /* z = 1/s */
	u1[num_n_words - 1] = 0;
	bits2int(u1, message_hash, hash_size, curve);
	uECC_vli_modMult(u1, u1, z, curve->n, num_n_words); /* u1 = e/s */
	uECC_vli_modMult(u2, r, z, curve->n, num_n_words); /* u2 = r/s */

	/* Calculate sum = G + Q. */
	uECC_vli_set(sum, _public, num_words);
	uECC_vli_set(sum + num_words, _public + num_words, num_words);
	uECC_vli_set(tx, curve->G, num_words);
	uECC_vli_set(ty, curve->G + num_words, num_words);
	uECC_vli_modSub(z, sum, tx, curve->p, num_words); /* z = x2 - x1 */
	XYcZ_add(tx, ty, sum, sum + num_words, curve);
	uECC_vli_modInv(z, z, curve->p, num_words); /* z = 1/z */
	apply_z(sum, sum + num_words, z, curve);

	/* Use Shamir's trick to calculate u1*G + u2*Q */
	points[0] = 0;
	points[1] = curve->G;
	points[2] = _public;
	points[3] = sum;
	num_bits = smax(uECC_vli_numBits(u1, num_n_words),
	uECC_vli_numBits(u2, num_n_words));

	point = points[(!!uECC_vli_testBit(u1, num_bits - 1)) |
                       ((!!uECC_vli_testBit(u2, num_bits - 1)) << 1)];
	uECC_vli_set(rx, point, num_words);
	uECC_vli_set(ry, point + num_words, num_words);
	uECC_vli_clear(z, num_words);
	z[0] = 1;

	for (i = num_bits - 2; i >= 0; --i) {
		uECC_word_t index;
		curve->double_jacobian(rx, ry, z, curve);

		index = (!!uECC_vli_testBit(u1, i)) | ((!!uECC_vli_testBit(u2, i)) << 1);
		point = points[index];
		if (point) {
			uECC_vli_set(tx, point, num_words);
			uECC_vli_set(ty, point + num_words, num_words);
			apply_z(tx, ty, z, curve);
			uECC_vli_modSub(tz, rx, tx, curve->p, num_words); /* Z = x2 - x1 */
			XYcZ_add(tx, ty, rx, ry, curve);
			uECC_vli_modMult_fast(z, z, tz, curve);
		}
  	}

	uECC_vli_modInv(z, z, curve->p, num_words); /* Z = 1/Z */
	apply_z(rx, ry, z, curve);

	/* v = x1 (mod n) */
	if (uECC_vli_cmp_unsafe(curve->n, rx, num_n_words) != 1) {
		uECC_vli_sub(rx, rx, curve->n, num_n_words);
	}

	/* Accept only if v == r. */
	return (int)(uECC_vli_equal(rx, r, num_words) == 0);
}

