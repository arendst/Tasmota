/* cbc_mode.c - TinyCrypt implementation of CBC mode encryption & decryption */

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

#include "../include/tinycrypt/cbc_mode.h"
#include "../include/tinycrypt/constants.h"
#include "../include/tinycrypt/utils.h"

int tc_cbc_mode_encrypt(uint8_t *out, unsigned int outlen, const uint8_t *in,
			    unsigned int inlen, const uint8_t *iv,
			    const TCAesKeySched_t sched)
{

	uint8_t buffer[TC_AES_BLOCK_SIZE];
	unsigned int n, m;

	/* input sanity check: */
	if (out == (uint8_t *) 0 ||
	    in == (const uint8_t *) 0 ||
	    sched == (TCAesKeySched_t) 0 ||
	    inlen == 0 ||
	    outlen == 0 ||
	    (inlen % TC_AES_BLOCK_SIZE) != 0 ||
	    (outlen % TC_AES_BLOCK_SIZE) != 0 ||
	    outlen != inlen + TC_AES_BLOCK_SIZE) {
		return TC_CRYPTO_FAIL;
	}

	/* copy iv to the buffer */
	(void)_copy(buffer, TC_AES_BLOCK_SIZE, iv, TC_AES_BLOCK_SIZE);
	/* copy iv to the output buffer */
	(void)_copy(out, TC_AES_BLOCK_SIZE, iv, TC_AES_BLOCK_SIZE);
	out += TC_AES_BLOCK_SIZE;

	for (n = m = 0; n < inlen; ++n) {
		buffer[m++] ^= *in++;
		if (m == TC_AES_BLOCK_SIZE) {
			(void)tc_aes_encrypt(buffer, buffer, sched);
			(void)_copy(out, TC_AES_BLOCK_SIZE,
				    buffer, TC_AES_BLOCK_SIZE);
			out += TC_AES_BLOCK_SIZE;
			m = 0;
		}
	}

	return TC_CRYPTO_SUCCESS;
}

int tc_cbc_mode_decrypt(uint8_t *out, unsigned int outlen, const uint8_t *in,
			    unsigned int inlen, const uint8_t *iv,
			    const TCAesKeySched_t sched)
{

	uint8_t buffer[TC_AES_BLOCK_SIZE];
	const uint8_t *p;
	unsigned int n, m;

	/* sanity check the inputs */
	if (out == (uint8_t *) 0 ||
	    in == (const uint8_t *) 0 ||
	    sched == (TCAesKeySched_t) 0 ||
	    inlen == 0 ||
	    outlen == 0 ||
	    (inlen % TC_AES_BLOCK_SIZE) != 0 ||
	    (outlen % TC_AES_BLOCK_SIZE) != 0 ||
	    outlen != inlen - TC_AES_BLOCK_SIZE) {
		return TC_CRYPTO_FAIL;
	}

	/*
	 * Note that in == iv + ciphertext, i.e. the iv and the ciphertext are
	 * contiguous. This allows for a very efficient decryption algorithm
	 * that would not otherwise be possible.
	 */
	p = iv;
	for (n = m = 0; n < inlen; ++n) {
		if ((n % TC_AES_BLOCK_SIZE) == 0) {
			(void)tc_aes_decrypt(buffer, in, sched);
			in += TC_AES_BLOCK_SIZE;
			m = 0;
		}
		*out++ = buffer[m++] ^ *p++;
	}

	return TC_CRYPTO_SUCCESS;
}
