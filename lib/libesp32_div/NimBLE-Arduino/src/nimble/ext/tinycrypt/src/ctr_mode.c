/* ctr_mode.c - TinyCrypt CTR mode implementation */

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
#include "../include/tinycrypt/ctr_mode.h"
#include "../include/tinycrypt/utils.h"

int tc_ctr_mode(uint8_t *out, unsigned int outlen, const uint8_t *in,
		unsigned int inlen, uint8_t *ctr, const TCAesKeySched_t sched)
{

	uint8_t buffer[TC_AES_BLOCK_SIZE];
	uint8_t nonce[TC_AES_BLOCK_SIZE];
	unsigned int block_num;
	unsigned int i;

	/* input sanity check: */
	if (out == (uint8_t *) 0 ||
	    in == (uint8_t *) 0 ||
	    ctr == (uint8_t *) 0 ||
	    sched == (TCAesKeySched_t) 0 ||
	    inlen == 0 ||
	    outlen == 0 ||
	    outlen != inlen) {
		return TC_CRYPTO_FAIL;
	}

	/* copy the ctr to the nonce */
	(void)_copy(nonce, sizeof(nonce), ctr, sizeof(nonce));

	/* select the last 4 bytes of the nonce to be incremented */
	block_num = (nonce[12] << 24) | (nonce[13] << 16) |
		    (nonce[14] << 8) | (nonce[15]);
	for (i = 0; i < inlen; ++i) {
		if ((i % (TC_AES_BLOCK_SIZE)) == 0) {
			/* encrypt data using the current nonce */
			if (tc_aes_encrypt(buffer, nonce, sched)) {
				block_num++;
				nonce[12] = (uint8_t)(block_num >> 24);
				nonce[13] = (uint8_t)(block_num >> 16);
				nonce[14] = (uint8_t)(block_num >> 8);
				nonce[15] = (uint8_t)(block_num);
			} else {
				return TC_CRYPTO_FAIL;
			}
		}
		/* update the output */
		*out++ = buffer[i%(TC_AES_BLOCK_SIZE)] ^ *in++;
	}

	/* update the counter */
	ctr[12] = nonce[12]; ctr[13] = nonce[13];
	ctr[14] = nonce[14]; ctr[15] = nonce[15];

	return TC_CRYPTO_SUCCESS;
}
