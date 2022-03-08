/* ec_dh.c - TinyCrypt implementation of EC-DH */

/* 
 * Copyright (c) 2014, Kenneth MacKay
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
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
#include "../include/tinycrypt/ecc_dh.h"
#include <string.h>

#if default_RNG_defined
static uECC_RNG_Function g_rng_function = &default_CSPRNG;
#else
static uECC_RNG_Function g_rng_function = 0;
#endif

int uECC_make_key_with_d(uint8_t *public_key, uint8_t *private_key,
			 unsigned int *d, uECC_Curve curve)
{

	uECC_word_t _private[NUM_ECC_WORDS];
	uECC_word_t _public[NUM_ECC_WORDS * 2];

	/* This function is designed for test purposes-only (such as validating NIST
	 * test vectors) as it uses a provided value for d instead of generating
	 * it uniformly at random. */
	memcpy (_private, d, NUM_ECC_BYTES);

	/* Computing public-key from private: */
	if (EccPoint_compute_public_key(_public, _private, curve)) {

		/* Converting buffers to correct bit order: */
		uECC_vli_nativeToBytes(private_key,
				       BITS_TO_BYTES(curve->num_n_bits),
				       _private);
		uECC_vli_nativeToBytes(public_key,
				       curve->num_bytes,
				       _public);
		uECC_vli_nativeToBytes(public_key + curve->num_bytes,
				       curve->num_bytes,
				       _public + curve->num_words);

		/* erasing temporary buffer used to store secret: */
		memset(_private, 0, NUM_ECC_BYTES);

		return 1;
	}
	return 0;
}

int uECC_make_key(uint8_t *public_key, uint8_t *private_key, uECC_Curve curve)
{

	uECC_word_t _random[NUM_ECC_WORDS * 2];
	uECC_word_t _private[NUM_ECC_WORDS];
	uECC_word_t _public[NUM_ECC_WORDS * 2];
	uECC_word_t tries;

	for (tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries) {
		/* Generating _private uniformly at random: */
		uECC_RNG_Function rng_function = uECC_get_rng();
		if (!rng_function ||
			!rng_function((uint8_t *)_random, 2 * NUM_ECC_WORDS*uECC_WORD_SIZE)) {
        		return 0;
		}

		/* computing modular reduction of _random (see FIPS 186.4 B.4.1): */
		uECC_vli_mmod(_private, _random, curve->n, BITS_TO_WORDS(curve->num_n_bits));

		/* Computing public-key from private: */
		if (EccPoint_compute_public_key(_public, _private, curve)) {

			/* Converting buffers to correct bit order: */
			uECC_vli_nativeToBytes(private_key,
					       BITS_TO_BYTES(curve->num_n_bits),
					       _private);
			uECC_vli_nativeToBytes(public_key,
					       curve->num_bytes,
					       _public);
			uECC_vli_nativeToBytes(public_key + curve->num_bytes,
 					       curve->num_bytes,
					       _public + curve->num_words);

			/* erasing temporary buffer that stored secret: */
			memset(_private, 0, NUM_ECC_BYTES);

      			return 1;
    		}
  	}
	return 0;
}

int uECC_shared_secret(const uint8_t *public_key, const uint8_t *private_key,
		       uint8_t *secret, uECC_Curve curve)
{

	uECC_word_t _public[NUM_ECC_WORDS * 2];
	uECC_word_t _private[NUM_ECC_WORDS];

	uECC_word_t tmp[NUM_ECC_WORDS];
	uECC_word_t *p2[2] = {_private, tmp};
	uECC_word_t *initial_Z = 0;
	uECC_word_t carry;
	wordcount_t num_words = curve->num_words;
	wordcount_t num_bytes = curve->num_bytes;
	int r;

	/* Converting buffers to correct bit order: */
	uECC_vli_bytesToNative(_private,
      			       private_key,
			       BITS_TO_BYTES(curve->num_n_bits));
	uECC_vli_bytesToNative(_public,
      			       public_key,
			       num_bytes);
	uECC_vli_bytesToNative(_public + num_words,
			       public_key + num_bytes,
			       num_bytes);

	/* Regularize the bitcount for the private key so that attackers cannot use a
	 * side channel attack to learn the number of leading zeros. */
	carry = regularize_k(_private, _private, tmp, curve);

	/* If an RNG function was specified, try to get a random initial Z value to
	 * improve protection against side-channel attacks. */
	if (g_rng_function) {
		if (!uECC_generate_random_int(p2[carry], curve->p, num_words)) {
			r = 0;
			goto clear_and_out;
    		}
    		initial_Z = p2[carry];
  	}

	EccPoint_mult(_public, _public, p2[!carry], initial_Z, curve->num_n_bits + 1,
		      curve);

	uECC_vli_nativeToBytes(secret, num_bytes, _public);
	r = !EccPoint_isZero(_public, curve);

clear_and_out:
	/* erasing temporary buffer used to store secret: */
	memset(p2, 0, sizeof(p2));
	__asm__ __volatile__("" :: "g"(p2) : "memory");
	memset(tmp, 0, sizeof(tmp));
	__asm__ __volatile__("" :: "g"(tmp) : "memory");
	memset(_private, 0, sizeof(_private));
	__asm__ __volatile__("" :: "g"(_private) : "memory");

	return r;
}
