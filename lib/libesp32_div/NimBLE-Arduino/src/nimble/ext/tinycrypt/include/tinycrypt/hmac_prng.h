/* hmac_prng.h - TinyCrypt interface to an HMAC-PRNG implementation */

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

/**
 * @file
 * @brief Interface to an HMAC-PRNG implementation.
 *
 *  Overview:   A pseudo-random number generator (PRNG) generates a sequence
 *              of numbers that have a distribution close to the one expected
 *              for a sequence of truly random numbers. The NIST Special
 *              Publication 800-90A specifies several mechanisms to generate
 *              sequences of pseudo random numbers, including the HMAC-PRNG one
 *              which is based on HMAC. TinyCrypt implements HMAC-PRNG with
 *              certain modifications from the NIST SP 800-90A spec.
 *
 *  Security:   A cryptographically secure PRNG depends on the existence of an
 *              entropy source to provide a truly random seed as well as the
 *              security of the primitives used as the building blocks (HMAC and
 *              SHA256, for TinyCrypt).
 *
 *              The NIST SP 800-90A standard tolerates a null personalization,
 *              while TinyCrypt requires a non-null personalization. This is
 *              because a personalization string (the host name concatenated
 *              with a time stamp, for example) is easily computed and might be
 *              the last line of defense against failure of the entropy source.
 *
 *  Requires:   - SHA-256
 *              - HMAC
 *
 *  Usage:      1) call tc_hmac_prng_init to set the HMAC key and process the
 *              personalization data.
 *
 *              2) call tc_hmac_prng_reseed to process the seed and additional
 *              input.
 *
 *              3) call tc_hmac_prng_generate to out put the pseudo-random data.
 */

#ifndef __TC_HMAC_PRNG_H__
#define __TC_HMAC_PRNG_H__

#include "sha256.h"
#include "hmac.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TC_HMAC_PRNG_RESEED_REQ -1

struct tc_hmac_prng_struct {
	/* the HMAC instance for this PRNG */
	struct tc_hmac_state_struct h;
	/* the PRNG key */
	uint8_t key[TC_SHA256_DIGEST_SIZE];
	/* PRNG state */
	uint8_t v[TC_SHA256_DIGEST_SIZE];
	/* calls to tc_hmac_prng_generate left before re-seed */
	unsigned int countdown;
};

typedef struct tc_hmac_prng_struct *TCHmacPrng_t;

/**
 *  @brief HMAC-PRNG initialization procedure
 *  Initializes prng with personalization, disables tc_hmac_prng_generate
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                prng == NULL,
 *                personalization == NULL,
 *                plen > MAX_PLEN
 *  @note Assumes: - personalization != NULL.
 *              The personalization is a platform unique string (e.g., the host
 *              name) and is the last line of defense against failure of the
 *              entropy source
 *  @warning    NIST SP 800-90A specifies 3 items as seed material during
 *              initialization: entropy seed, personalization, and an optional
 *              nonce. TinyCrypts requires instead a non-null personalization
 *              (which is easily computed) and indirectly requires an entropy
 *              seed (since the reseed function is mandatorily called after
 *              init)
 *  @param prng IN/OUT -- the PRNG state to initialize
 *  @param personalization IN -- personalization string
 *  @param plen IN -- personalization length in bytes
 */
int tc_hmac_prng_init(TCHmacPrng_t prng,
		      const uint8_t *personalization,
		      unsigned int plen);

/**
 *  @brief HMAC-PRNG reseed procedure
 *  Mixes seed into prng, enables tc_hmac_prng_generate
 *  @return returns  TC_CRYPTO_SUCCESS (1)
 *  	    returns TC_CRYPTO_FAIL (0) if:
 *          prng == NULL,
 *          seed == NULL,
 *          seedlen < MIN_SLEN,
 *          seendlen > MAX_SLEN,
 *          additional_input != (const uint8_t *) 0 && additionallen == 0,
 *          additional_input != (const uint8_t *) 0 && additionallen > MAX_ALEN
 *  @note Assumes:- tc_hmac_prng_init has been called for prng
 *              - seed has sufficient entropy.
 *
 *  @param prng IN/OUT -- the PRNG state
 *  @param seed IN -- entropy to mix into the prng
 *  @param seedlen IN -- length of seed in bytes
 *  @param additional_input IN -- additional input to the prng
 *  @param additionallen IN -- additional input length in bytes
 */
int tc_hmac_prng_reseed(TCHmacPrng_t prng, const uint8_t *seed,
			unsigned int seedlen, const uint8_t *additional_input,
			unsigned int additionallen);

/**
 *  @brief HMAC-PRNG generate procedure
 *  Generates outlen pseudo-random bytes into out buffer, updates prng
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_HMAC_PRNG_RESEED_REQ (-1) if a reseed is needed
 *          returns TC_CRYPTO_FAIL (0) if:
 *                out == NULL,
 *                prng == NULL,
 *                outlen == 0,
 *                outlen >= MAX_OUT
 *  @note Assumes tc_hmac_prng_init has been called for prng
 *  @param out IN/OUT -- buffer to receive output
 *  @param outlen IN -- size of out buffer in bytes
 *  @param prng IN/OUT -- the PRNG state
 */
int tc_hmac_prng_generate(uint8_t *out, unsigned int outlen, TCHmacPrng_t prng);

#ifdef __cplusplus
}
#endif

#endif /* __TC_HMAC_PRNG_H__ */
