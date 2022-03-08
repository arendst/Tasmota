/* aes_encrypt.c - TinyCrypt implementation of AES encryption procedure */

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

#include "../include/tinycrypt/aes.h"
#include "../include/tinycrypt/utils.h"
#include "../include/tinycrypt/constants.h"

static const uint8_t sbox[256] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
	0xfe, 0xd7, 0xab, 0x76, 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
	0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 0xb7, 0xfd, 0x93, 0x26,
	0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
	0xeb, 0x27, 0xb2, 0x75, 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
	0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 0x53, 0xd1, 0x00, 0xed,
	0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
	0x50, 0x3c, 0x9f, 0xa8, 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
	0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 0xcd, 0x0c, 0x13, 0xec,
	0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
	0xde, 0x5e, 0x0b, 0xdb, 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
	0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 0xe7, 0xc8, 0x37, 0x6d,
	0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
	0x4b, 0xbd, 0x8b, 0x8a, 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
	0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 0xe1, 0xf8, 0x98, 0x11,
	0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
	0xb0, 0x54, 0xbb, 0x16
};

static inline unsigned int rotword(unsigned int a)
{
	return (((a) >> 24)|((a) << 8));
}

#define subbyte(a, o)(sbox[((a) >> (o))&0xff] << (o))
#define subword(a)(subbyte(a, 24)|subbyte(a, 16)|subbyte(a, 8)|subbyte(a, 0))

int tc_aes128_set_encrypt_key(TCAesKeySched_t s, const uint8_t *k)
{
	const unsigned int rconst[11] = {
		0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
		0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
	};
	unsigned int i;
	unsigned int t;

	if (s == (TCAesKeySched_t) 0) {
		return TC_CRYPTO_FAIL;
	} else if (k == (const uint8_t *) 0) {
		return TC_CRYPTO_FAIL;
	}

	for (i = 0; i < Nk; ++i) {
		s->words[i] = (k[Nb*i]<<24) | (k[Nb*i+1]<<16) |
			      (k[Nb*i+2]<<8) | (k[Nb*i+3]);
	}

	for (; i < (Nb * (Nr + 1)); ++i) {
		t = s->words[i-1];
		if ((i % Nk) == 0) {
			t = subword(rotword(t)) ^ rconst[i/Nk];
		}
		s->words[i] = s->words[i-Nk] ^ t;
	}

	return TC_CRYPTO_SUCCESS;
}

static inline void add_round_key(uint8_t *s, const unsigned int *k)
{
	s[0] ^= (uint8_t)(k[0] >> 24); s[1] ^= (uint8_t)(k[0] >> 16);
	s[2] ^= (uint8_t)(k[0] >> 8); s[3] ^= (uint8_t)(k[0]);
	s[4] ^= (uint8_t)(k[1] >> 24); s[5] ^= (uint8_t)(k[1] >> 16);
	s[6] ^= (uint8_t)(k[1] >> 8); s[7] ^= (uint8_t)(k[1]);
	s[8] ^= (uint8_t)(k[2] >> 24); s[9] ^= (uint8_t)(k[2] >> 16);
	s[10] ^= (uint8_t)(k[2] >> 8); s[11] ^= (uint8_t)(k[2]);
	s[12] ^= (uint8_t)(k[3] >> 24); s[13] ^= (uint8_t)(k[3] >> 16);
	s[14] ^= (uint8_t)(k[3] >> 8); s[15] ^= (uint8_t)(k[3]);
}

static inline void sub_bytes(uint8_t *s)
{
	unsigned int i;

	for (i = 0; i < (Nb * Nk); ++i) {
		s[i] = sbox[s[i]];
	}
}

#define triple(a)(_double_byte(a)^(a))

static inline void mult_row_column(uint8_t *out, const uint8_t *in)
{
	out[0] = _double_byte(in[0]) ^ triple(in[1]) ^ in[2] ^ in[3];
	out[1] = in[0] ^ _double_byte(in[1]) ^ triple(in[2]) ^ in[3];
	out[2] = in[0] ^ in[1] ^ _double_byte(in[2]) ^ triple(in[3]);
	out[3] = triple(in[0]) ^ in[1] ^ in[2] ^ _double_byte(in[3]);
}

static inline void mix_columns(uint8_t *s)
{
	uint8_t t[Nb*Nk];

	mult_row_column(t, s);
	mult_row_column(&t[Nb], s+Nb);
	mult_row_column(&t[2 * Nb], s + (2 * Nb));
	mult_row_column(&t[3 * Nb], s + (3 * Nb));
	(void) _copy(s, sizeof(t), t, sizeof(t));
}

/*
 * This shift_rows also implements the matrix flip required for mix_columns, but
 * performs it here to reduce the number of memory operations.
 */
static inline void shift_rows(uint8_t *s)
{
	uint8_t t[Nb * Nk];

	t[0]  = s[0]; t[1] = s[5]; t[2] = s[10]; t[3] = s[15];
	t[4]  = s[4]; t[5] = s[9]; t[6] = s[14]; t[7] = s[3];
	t[8]  = s[8]; t[9] = s[13]; t[10] = s[2]; t[11] = s[7];
	t[12] = s[12]; t[13] = s[1]; t[14] = s[6]; t[15] = s[11];
	(void) _copy(s, sizeof(t), t, sizeof(t));
}

int tc_aes_encrypt(uint8_t *out, const uint8_t *in, const TCAesKeySched_t s)
{
	uint8_t state[Nk*Nb];
	unsigned int i;

	if (out == (uint8_t *) 0) {
		return TC_CRYPTO_FAIL;
	} else if (in == (const uint8_t *) 0) {
		return TC_CRYPTO_FAIL;
	} else if (s == (TCAesKeySched_t) 0) {
		return TC_CRYPTO_FAIL;
	}

	(void)_copy(state, sizeof(state), in, sizeof(state));
	add_round_key(state, s->words);

	for (i = 0; i < (Nr - 1); ++i) {
		sub_bytes(state);
		shift_rows(state);
		mix_columns(state);
		add_round_key(state, s->words + Nb*(i+1));
	}

	sub_bytes(state);
	shift_rows(state);
	add_round_key(state, s->words + Nb*(i+1));

	(void)_copy(out, sizeof(state), state, sizeof(state));

	/* zeroing out the state buffer */
	_set(state, TC_ZERO_BYTE, sizeof(state));

	return TC_CRYPTO_SUCCESS;
}
