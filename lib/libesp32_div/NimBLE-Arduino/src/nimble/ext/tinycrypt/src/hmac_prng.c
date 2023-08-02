/* hmac_prng.c - TinyCrypt implementation of HMAC-PRNG */

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

#include "../include/tinycrypt/hmac_prng.h"
#include "../include/tinycrypt/hmac.h"
#include "../include/tinycrypt/constants.h"
#include "../include/tinycrypt/utils.h"

/*
 * min bytes in the seed string.
 * MIN_SLEN*8 must be at least the expected security level.
 */
static const unsigned int MIN_SLEN = 32;

/*
 * max bytes in the seed string;
 * SP800-90A specifies a maximum of 2^35 bits (i.e., 2^32 bytes).
 */
static const unsigned int MAX_SLEN = UINT32_MAX;

/*
 * max bytes in the personalization string;
 * SP800-90A specifies a maximum of 2^35 bits (i.e., 2^32 bytes).
 */
static const unsigned int MAX_PLEN = UINT32_MAX;

/*
 * max bytes in the additional_info string;
 * SP800-90A specifies a maximum of 2^35 bits (i.e., 2^32 bytes).
 */
static const unsigned int MAX_ALEN = UINT32_MAX;

/*
 * max number of generates between re-seeds;
 * TinyCrypt accepts up to (2^32 - 1) which is the maximal value of
 * a 32-bit unsigned int variable, while SP800-90A specifies a maximum of 2^48.
 */
static const unsigned int MAX_GENS = UINT32_MAX;

/*
 * maximum bytes per generate call;
 * SP800-90A specifies a maximum up to 2^19.
 */
static const unsigned int  MAX_OUT = (1 << 19);

/*
 * Assumes: prng != NULL, e != NULL, len >= 0.
 */
static void update(TCHmacPrng_t prng, const uint8_t *e, unsigned int len)
{
	const uint8_t separator0 = 0x00;
	const uint8_t separator1 = 0x01;

	/* use current state, e and separator 0 to compute a new prng key: */
	(void)tc_hmac_init(&prng->h);
	(void)tc_hmac_update(&prng->h, prng->v, sizeof(prng->v));
	(void)tc_hmac_update(&prng->h, &separator0, sizeof(separator0));
	(void)tc_hmac_update(&prng->h, e, len);
	(void)tc_hmac_final(prng->key, sizeof(prng->key), &prng->h);
	/* configure the new prng key into the prng's instance of hmac */
	(void)tc_hmac_set_key(&prng->h, prng->key, sizeof(prng->key));

	/* use the new key to compute a new state variable v */
	(void)tc_hmac_init(&prng->h);
	(void)tc_hmac_update(&prng->h, prng->v, sizeof(prng->v));
	(void)tc_hmac_final(prng->v, sizeof(prng->v), &prng->h);

	/* use current state, e and separator 1 to compute a new prng key: */
	(void)tc_hmac_init(&prng->h);
	(void)tc_hmac_update(&prng->h, prng->v, sizeof(prng->v));
	(void)tc_hmac_update(&prng->h, &separator1, sizeof(separator1));
	(void)tc_hmac_update(&prng->h, e, len);
	(void)tc_hmac_final(prng->key, sizeof(prng->key), &prng->h);
	/* configure the new prng key into the prng's instance of hmac */
	(void)tc_hmac_set_key(&prng->h, prng->key, sizeof(prng->key));

	/* use the new key to compute a new state variable v */
	(void)tc_hmac_init(&prng->h);
	(void)tc_hmac_update(&prng->h, prng->v, sizeof(prng->v));
	(void)tc_hmac_final(prng->v, sizeof(prng->v), &prng->h);
}

int tc_hmac_prng_init(TCHmacPrng_t prng,
		      const uint8_t *personalization,
		      unsigned int plen)
{

	/* input sanity check: */
	if (prng == (TCHmacPrng_t) 0 ||
	    personalization == (uint8_t *) 0 ||
	    plen > MAX_PLEN) {
		return TC_CRYPTO_FAIL;
	}

	/* put the generator into a known state: */
	_set(prng->key, 0x00, sizeof(prng->key));
	_set(prng->v, 0x01, sizeof(prng->v));
	tc_hmac_set_key(&prng->h, prng->key, sizeof(prng->key));
	/* update assumes SOME key has been configured into HMAC */

	update(prng, personalization, plen);

	/* force a reseed before allowing tc_hmac_prng_generate to succeed: */
	prng->countdown = 0;

	return TC_CRYPTO_SUCCESS;
}

int tc_hmac_prng_reseed(TCHmacPrng_t prng,
			const uint8_t *seed,
			unsigned int seedlen,
			const uint8_t *additional_input,
			unsigned int additionallen)
{

	/* input sanity check: */
	if (prng == (TCHmacPrng_t) 0 ||
	    seed == (const uint8_t *) 0 ||
	    seedlen < MIN_SLEN ||
	    seedlen > MAX_SLEN) {
		return TC_CRYPTO_FAIL;
	}

	if (additional_input != (const uint8_t *) 0) {
		/*
		 * Abort if additional_input is provided but has inappropriate
		 * length
		 */
		if (additionallen == 0 ||
		    additionallen > MAX_ALEN) {
			return TC_CRYPTO_FAIL;
		} else {
		/* call update for the seed and additional_input */
		update(prng, seed, seedlen);
		update(prng, additional_input, additionallen);
		}
	} else {
		/* call update only for the seed */
		update(prng, seed, seedlen);
	}

	/* ... and enable hmac_prng_generate */
	prng->countdown = MAX_GENS;

	return TC_CRYPTO_SUCCESS;
}

int tc_hmac_prng_generate(uint8_t *out, unsigned int outlen, TCHmacPrng_t prng)
{
	unsigned int bufferlen;

	/* input sanity check: */
	if (out == (uint8_t *) 0 ||
	    prng == (TCHmacPrng_t) 0 ||
	    outlen == 0 ||
	    outlen > MAX_OUT) {
		return TC_CRYPTO_FAIL;
	} else if (prng->countdown == 0) {
		return TC_HMAC_PRNG_RESEED_REQ;
	}

	prng->countdown--;

	while (outlen != 0) {
		/* operate HMAC in OFB mode to create "random" outputs */
		(void)tc_hmac_init(&prng->h);
		(void)tc_hmac_update(&prng->h, prng->v, sizeof(prng->v));
		(void)tc_hmac_final(prng->v, sizeof(prng->v), &prng->h);

		bufferlen = (TC_SHA256_DIGEST_SIZE > outlen) ?
			outlen : TC_SHA256_DIGEST_SIZE;
		(void)_copy(out, bufferlen, prng->v, bufferlen);

		out += bufferlen;
		outlen = (outlen > TC_SHA256_DIGEST_SIZE) ?
			(outlen - TC_SHA256_DIGEST_SIZE) : 0;
	}

	/* block future PRNG compromises from revealing past state */
	update(prng, prng->v, TC_SHA256_DIGEST_SIZE);

	return TC_CRYPTO_SUCCESS;
}
