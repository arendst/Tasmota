/*  cmac_mode.h -- interface to a CMAC implementation */

/*
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved
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
 * @brief Interface to a CMAC implementation.
 *
 *  Overview: CMAC is defined NIST in SP 800-38B, and is the standard algorithm
 *            for computing a MAC using a block cipher. It can compute the MAC
 *            for a byte string of any length. It is distinguished from CBC-MAC
 *            in the processing of the final message block; CMAC uses a
 *            different technique to compute the final message block is full
 *            size or only partial, while CBC-MAC uses the same technique for
 *            both. This difference permits CMAC to be applied to variable
 *            length messages, while all messages authenticated by CBC-MAC must
 *            be the same length.
 *
 *  Security: AES128-CMAC mode of operation offers 64 bits of security against
 *            collision attacks. Note however that an external attacker cannot
 *            generate the tags him/herself without knowing the MAC key. In this
 *            sense, to attack the collision property of AES128-CMAC, an
 *            external attacker would need the cooperation of the legal user to
 *            produce an exponentially high number of tags (e.g. 2^64) to
 *            finally be able to look for collisions and benefit from them. As
 *            an extra precaution, the current implementation allows to at most
 *            2^48 calls to the tc_cmac_update function before re-calling
 *            tc_cmac_setup (allowing a new key to be set), as suggested in
 *            Appendix B of SP 800-38B.
 *
 *  Requires: AES-128
 *
 *  Usage:   This implementation provides a "scatter-gather" interface, so that
 *           the CMAC value can be computed incrementally over a message
 *           scattered in different segments throughout memory. Experience shows
 *           this style of interface tends to minimize the burden of programming
 *           correctly. Like all symmetric key operations, it is session
 *           oriented.
 *
 *           To begin a CMAC session, use tc_cmac_setup to initialize a struct
 *           tc_cmac_struct with encryption key and buffer. Our implementation
 *           always assume that the AES key to be the same size as the block
 *           cipher block size. Once setup, this data structure can be used for
 *           many CMAC computations.
 *
 *           Once the state has been setup with a key, computing the CMAC of
 *           some data requires three steps:
 *
 *           (1) first use tc_cmac_init to initialize a new CMAC computation.
 *           (2) next mix all of the data into the CMAC computation state using
 *               tc_cmac_update. If all of the data resides in a single data
 *               segment then only one tc_cmac_update call is needed; if data
 *               is scattered throughout memory in n data segments, then n calls
 *               will be needed. CMAC IS ORDER SENSITIVE, to be able to detect
 *               attacks that swap bytes, so the order in which data is mixed
 *               into the state is critical!
 *           (3) Once all of the data for a message has been mixed, use
 *               tc_cmac_final to compute the CMAC tag value.
 *
 *           Steps (1)-(3) can be repeated as many times as you want to CMAC
 *           multiple messages. A practical limit is 2^48 1K messages before you
 *           have to change the key.
 *
 *           Once you are done computing CMAC with a key, it is a good idea to
 *           destroy the state so an attacker cannot recover the key; use
 *           tc_cmac_erase to accomplish this.
 */

#ifndef __TC_CMAC_MODE_H__
#define __TC_CMAC_MODE_H__

#include "aes.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* padding for last message block */
#define TC_CMAC_PADDING 0x80

/* struct tc_cmac_struct represents the state of a CMAC computation */
typedef struct tc_cmac_struct {
/* initialization vector */
	uint8_t iv[TC_AES_BLOCK_SIZE];
/* used if message length is a multiple of block_size bytes */
	uint8_t K1[TC_AES_BLOCK_SIZE];
/* used if message length isn't a multiple block_size bytes */
	uint8_t K2[TC_AES_BLOCK_SIZE];
/* where to put bytes that didn't fill a block */
	uint8_t leftover[TC_AES_BLOCK_SIZE];
/* identifies the encryption key */
	unsigned int keyid;
/* next available leftover location */
	unsigned int leftover_offset;
/* AES key schedule */
	TCAesKeySched_t sched;
/* calls to tc_cmac_update left before re-key */
	uint64_t countdown;
} *TCCmacState_t;

/**
 * @brief Configures the CMAC state to use the given AES key
 * @return returns TC_CRYPTO_SUCCESS (1) after having configured the CMAC state
 *         returns TC_CRYPTO_FAIL (0) if:
 *              s == NULL or
 *              key == NULL
 *
 * @param s IN/OUT -- the state to set up
 * @param key IN -- the key to use
 * @param sched IN -- AES key schedule
 */
int tc_cmac_setup(TCCmacState_t s, const uint8_t *key,
		      TCAesKeySched_t sched);

/**
 * @brief Erases the CMAC state
 * @return returns TC_CRYPTO_SUCCESS (1) after having configured the CMAC state
 *         returns TC_CRYPTO_FAIL (0) if:
 *              s == NULL
 *
 * @param s IN/OUT -- the state to erase
 */
int tc_cmac_erase(TCCmacState_t s);

/**
 * @brief Initializes a new CMAC computation
 * @return returns TC_CRYPTO_SUCCESS (1) after having initialized the CMAC state
 *         returns TC_CRYPTO_FAIL (0) if:
 *              s == NULL
 *
 * @param s IN/OUT -- the state to initialize
 */
int tc_cmac_init(TCCmacState_t s);

/**
 * @brief Incrementally computes CMAC over the next data segment
 * @return returns TC_CRYPTO_SUCCESS (1) after successfully updating the CMAC state
 *         returns TC_CRYPTO_FAIL (0) if:
 *              s == NULL or
 *              if data == NULL when dlen > 0
 *
 * @param s IN/OUT -- the CMAC state
 * @param data IN -- the next data segment to MAC
 * @param dlen IN -- the length of data in bytes
 */
int tc_cmac_update(TCCmacState_t s, const uint8_t *data, size_t dlen);

/**
 * @brief Generates the tag from the CMAC state
 * @return returns TC_CRYPTO_SUCCESS (1) after successfully generating the tag
 *         returns TC_CRYPTO_FAIL (0) if:
 *              tag == NULL or
 *              s == NULL
 *
 * @param tag OUT -- the CMAC tag
 * @param s IN -- CMAC state
 */
int tc_cmac_final(uint8_t *tag, TCCmacState_t s);

#ifdef __cplusplus
}
#endif

#endif /* __TC_CMAC_MODE_H__ */
