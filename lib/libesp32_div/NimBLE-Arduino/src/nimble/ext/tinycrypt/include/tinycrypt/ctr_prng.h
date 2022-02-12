/* ctr_prng.h - TinyCrypt interface to a CTR-PRNG implementation */

/*
 * Copyright (c) 2016, Chris Morrison
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
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

/**
 * @file
 * @brief Interface to a CTR-PRNG implementation.
 *
 *  Overview:   A pseudo-random number generator (PRNG) generates a sequence
 *              of numbers that have a distribution close to the one expected
 *              for a sequence of truly random numbers. The NIST Special
 *              Publication 800-90A specifies several mechanisms to generate
 *              sequences of pseudo random numbers, including the CTR-PRNG one
 *              which is based on AES. TinyCrypt implements CTR-PRNG with
 *              AES-128.
 *
 *  Security:   A cryptographically secure PRNG depends on the existence of an
 *              entropy source to provide a truly random seed as well as the
 *              security of the primitives used as the building blocks (AES-128
 *              in this instance).
 *
 *  Requires:   - AES-128
 *
 *  Usage:      1) call tc_ctr_prng_init to seed the prng context
 *
 *              2) call tc_ctr_prng_reseed to mix in additional entropy into
 *              the prng context
 *
 *              3) call tc_ctr_prng_generate to output the pseudo-random data
 *
 *              4) call tc_ctr_prng_uninstantiate to zero out the prng context
 */

#ifndef __TC_CTR_PRNG_H__
#define __TC_CTR_PRNG_H__

#include "aes.h"

#define TC_CTR_PRNG_RESEED_REQ -1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	/* updated each time another BLOCKLEN_BYTES bytes are produced */
	uint8_t V[TC_AES_BLOCK_SIZE]; 

	/* updated whenever the PRNG is reseeded */
	struct tc_aes_key_sched_struct key;

	/* number of requests since initialization/reseeding */
	uint64_t reseedCount;
} TCCtrPrng_t;


/**
 *  @brief CTR-PRNG initialization procedure
 *  Initializes prng context with entropy and personalization string (if any)
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                ctx == NULL,
 *                entropy == NULL,
 *                entropyLen < (TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE)
 *  @note       Only the first (TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE) bytes of
 *              both the entropy and personalization inputs are used -
 *              supplying additional bytes has no effect.
 *  @param ctx IN/OUT -- the PRNG context to initialize
 *  @param entropy IN -- entropy used to seed the PRNG
 *  @param entropyLen IN -- entropy length in bytes
 *  @param personalization IN -- personalization string used to seed the PRNG
 *  (may be null)
 *  @param plen IN -- personalization length in bytes
 *
 */
int tc_ctr_prng_init(TCCtrPrng_t * const ctx, 
		     uint8_t const * const entropy,
		     unsigned int entropyLen, 
		     uint8_t const * const personalization,
		     unsigned int pLen);

/**
 *  @brief CTR-PRNG reseed procedure
 *  Mixes entropy and additional_input into the prng context
 *  @return returns  TC_CRYPTO_SUCCESS (1)
 *  returns TC_CRYPTO_FAIL (0) if:
 *          ctx == NULL,
 *          entropy == NULL,
 *          entropylen < (TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE)
 *  @note It is better to reseed an existing prng context rather than
 *        re-initialise, so that any existing entropy in the context is
 *        presereved.  This offers some protection against undetected failures
 *        of the entropy source.
 *  @note Assumes tc_ctr_prng_init has been called for ctx
 *  @param ctx IN/OUT -- the PRNG state
 *  @param entropy IN -- entropy to mix into the prng
 *  @param entropylen IN -- length of entropy in bytes
 *  @param additional_input IN -- additional input to the prng (may be null)
 *  @param additionallen IN -- additional input length in bytes
 */
int tc_ctr_prng_reseed(TCCtrPrng_t * const ctx, 
		       uint8_t const * const entropy,
		       unsigned int entropyLen,
		       uint8_t const * const additional_input,
		       unsigned int additionallen);

/**
 *  @brief CTR-PRNG generate procedure
 *  Generates outlen pseudo-random bytes into out buffer, updates prng
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CTR_PRNG_RESEED_REQ (-1) if a reseed is needed
 *             returns TC_CRYPTO_FAIL (0) if:
 *                ctx == NULL,
 *                out == NULL,
 *                outlen >= 2^16
 *  @note Assumes tc_ctr_prng_init has been called for ctx
 *  @param ctx IN/OUT -- the PRNG context
 *  @param additional_input IN -- additional input to the prng (may be null)
 *  @param additionallen IN -- additional input length in bytes
 *  @param out IN/OUT -- buffer to receive output
 *  @param outlen IN -- size of out buffer in bytes
 */
int tc_ctr_prng_generate(TCCtrPrng_t * const ctx,
			 uint8_t const * const additional_input,
			 unsigned int additionallen,
			 uint8_t * const out,
			 unsigned int outlen);

/**
 *  @brief CTR-PRNG uninstantiate procedure
 *  Zeroes the internal state of the supplied prng context
 *  @return none
 *  @param ctx IN/OUT -- the PRNG context
 */
void tc_ctr_prng_uninstantiate(TCCtrPrng_t * const ctx);

#ifdef __cplusplus
}
#endif

#endif /* __TC_CTR_PRNG_H__ */
