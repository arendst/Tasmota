/* ctr_prng.c - TinyCrypt implementation of CTR-PRNG */

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

#include <tinycrypt/ctr_prng.h>
#include <tinycrypt/utils.h>
#include <tinycrypt/constants.h>
#include <string.h>

/*
 * This PRNG is based on the CTR_DRBG described in Recommendation for Random
 * Number Generation Using Deterministic Random Bit Generators,
 * NIST SP 800-90A Rev. 1.
 *
 * Annotations to particular steps (e.g. 10.2.1.2 Step 1) refer to the steps
 * described in that document.
 *
 */

/**
 *  @brief Array incrementer
 *  Treats the supplied array as one contiguous number (MSB in arr[0]), and
 *  increments it by one
 *  @return none
 *  @param arr IN/OUT -- array to be incremented
 *  @param len IN -- size of arr in bytes
 */
static void arrInc(uint8_t arr[], unsigned int len)
{
	unsigned int i;
	if (0 != arr) {
		for (i = len; i > 0U; i--) {
			if (++arr[i-1] != 0U) {
				break;
			}
		}
	}
}

/**
 *  @brief CTR PRNG update
 *  Updates the internal state of supplied the CTR PRNG context
 *  increments it by one
 *  @return none
 *  @note Assumes: providedData is (TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE) bytes long
 *  @param ctx IN/OUT -- CTR PRNG state
 *  @param providedData IN -- data used when updating the internal state
 */
static void tc_ctr_prng_update(TCCtrPrng_t * const ctx, uint8_t const * const providedData)
{
	if (0 != ctx) {
		/* 10.2.1.2 step 1 */
		uint8_t temp[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE];
		unsigned int len = 0U;

		/* 10.2.1.2 step 2 */
		while (len < sizeof temp) {
			unsigned int blocklen = sizeof(temp) - len;
			uint8_t output_block[TC_AES_BLOCK_SIZE];

			/* 10.2.1.2 step 2.1 */
			arrInc(ctx->V, sizeof ctx->V);

			/* 10.2.1.2 step 2.2 */
			if (blocklen > TC_AES_BLOCK_SIZE) {
				blocklen = TC_AES_BLOCK_SIZE;
			}
			(void)tc_aes_encrypt(output_block, ctx->V, &ctx->key);

			/* 10.2.1.2 step 2.3/step 3 */
			memcpy(&(temp[len]), output_block, blocklen);

			len += blocklen;
		}

		/* 10.2.1.2 step 4 */
		if (0 != providedData) {
			unsigned int i;
			for (i = 0U; i < sizeof temp; i++) {
				temp[i] ^= providedData[i];
			}
		}

		/* 10.2.1.2 step 5 */
		(void)tc_aes128_set_encrypt_key(&ctx->key, temp);
    
		/* 10.2.1.2 step 6 */
		memcpy(ctx->V, &(temp[TC_AES_KEY_SIZE]), TC_AES_BLOCK_SIZE);
	}
}

int tc_ctr_prng_init(TCCtrPrng_t * const ctx, 
		     uint8_t const * const entropy,
		     unsigned int entropyLen, 
		     uint8_t const * const personalization,
		     unsigned int pLen)
{
	int result = TC_CRYPTO_FAIL;	
	unsigned int i;
	uint8_t personalization_buf[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE] = {0U};
	uint8_t seed_material[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE];
	uint8_t zeroArr[TC_AES_BLOCK_SIZE] = {0U};
  
	if (0 != personalization) {
		/* 10.2.1.3.1 step 1 */
		unsigned int len = pLen;
		if (len > sizeof personalization_buf) {
			len = sizeof personalization_buf;
		}

		/* 10.2.1.3.1 step 2 */
		memcpy(personalization_buf, personalization, len);
	}

	if ((0 != ctx) && (0 != entropy) && (entropyLen >= sizeof seed_material)) {
		/* 10.2.1.3.1 step 3 */
		memcpy(seed_material, entropy, sizeof seed_material);
		for (i = 0U; i < sizeof seed_material; i++) {
			seed_material[i] ^= personalization_buf[i];
		}

		/* 10.2.1.3.1 step 4 */
		(void)tc_aes128_set_encrypt_key(&ctx->key, zeroArr);

		/* 10.2.1.3.1 step 5 */
		memset(ctx->V,   0x00, sizeof ctx->V);
    
		/* 10.2.1.3.1 step 6 */    
		tc_ctr_prng_update(ctx, seed_material);

		/* 10.2.1.3.1 step 7 */
		ctx->reseedCount = 1U;

		result = TC_CRYPTO_SUCCESS;
	}
	return result;
}

int tc_ctr_prng_reseed(TCCtrPrng_t * const ctx, 
			uint8_t const * const entropy,
			unsigned int entropyLen,
			uint8_t const * const additional_input,
			unsigned int additionallen)
{
	unsigned int i;
	int result = TC_CRYPTO_FAIL;
	uint8_t additional_input_buf[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE] = {0U};
	uint8_t seed_material[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE];

	if (0 != additional_input) {
		/* 10.2.1.4.1 step 1 */
		unsigned int len = additionallen;
		if (len > sizeof additional_input_buf) {
			len = sizeof additional_input_buf;
		}

		/* 10.2.1.4.1 step 2 */
		memcpy(additional_input_buf, additional_input, len);
	}
	
	unsigned int seedlen = (unsigned int)TC_AES_KEY_SIZE + (unsigned int)TC_AES_BLOCK_SIZE;
	if ((0 != ctx) && (entropyLen >= seedlen)) {
		/* 10.2.1.4.1 step 3 */
		memcpy(seed_material, entropy, sizeof seed_material);
		for (i = 0U; i < sizeof seed_material; i++) {
			seed_material[i] ^= additional_input_buf[i];
		}

		/* 10.2.1.4.1 step 4 */
		tc_ctr_prng_update(ctx, seed_material);

		/* 10.2.1.4.1 step 5 */
		ctx->reseedCount = 1U;

		result = TC_CRYPTO_SUCCESS;
	}
	return result;
}

int tc_ctr_prng_generate(TCCtrPrng_t * const ctx,
			uint8_t const * const additional_input,
			unsigned int additionallen,
			uint8_t * const out,
			unsigned int outlen)
{
	/* 2^48 - see section 10.2.1 */
	static const uint64_t MAX_REQS_BEFORE_RESEED = 0x1000000000000ULL; 

	/* 2^19 bits - see section 10.2.1 */ 
	static const unsigned int MAX_BYTES_PER_REQ = 65536U; 

	unsigned int result = TC_CRYPTO_FAIL;

	if ((0 != ctx) && (0 != out) && (outlen < MAX_BYTES_PER_REQ)) {
		/* 10.2.1.5.1 step 1 */
		if (ctx->reseedCount > MAX_REQS_BEFORE_RESEED) {
			result = TC_CTR_PRNG_RESEED_REQ;
		} else {
			uint8_t additional_input_buf[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE] = {0U};
			if (0 != additional_input) {
				/* 10.2.1.5.1 step 2  */
				unsigned int len = additionallen;
				if (len > sizeof additional_input_buf) {
					len = sizeof additional_input_buf;
				}
				memcpy(additional_input_buf, additional_input, len);
				tc_ctr_prng_update(ctx, additional_input_buf);
			}
      
			/* 10.2.1.5.1 step 3 - implicit */

			/* 10.2.1.5.1 step 4 */
			unsigned int len = 0U;      
			while (len < outlen) {
				unsigned int blocklen = outlen - len;
				uint8_t output_block[TC_AES_BLOCK_SIZE];

				/* 10.2.1.5.1 step 4.1 */
				arrInc(ctx->V, sizeof ctx->V);

				/* 10.2.1.5.1 step 4.2 */
				(void)tc_aes_encrypt(output_block, ctx->V, &ctx->key);
      
				/* 10.2.1.5.1 step 4.3/step 5 */
				if (blocklen > TC_AES_BLOCK_SIZE) {
					blocklen = TC_AES_BLOCK_SIZE;
				}
				memcpy(&(out[len]), output_block, blocklen);

				len += blocklen;
			}
      
			/* 10.2.1.5.1 step 6 */
			tc_ctr_prng_update(ctx, additional_input_buf);

			/* 10.2.1.5.1 step 7 */
			ctx->reseedCount++;

			/* 10.2.1.5.1 step 8 */
			result = TC_CRYPTO_SUCCESS;
		}
	}

	return result;
}

void tc_ctr_prng_uninstantiate(TCCtrPrng_t * const ctx)
{
	if (0 != ctx) {
		memset(ctx->key.words, 0x00, sizeof ctx->key.words);
		memset(ctx->V,         0x00, sizeof ctx->V);
		ctx->reseedCount = 0U;
	}
}




