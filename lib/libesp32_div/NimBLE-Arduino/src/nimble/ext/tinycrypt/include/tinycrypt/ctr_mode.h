/* ctr_mode.h - TinyCrypt interface to CTR mode */

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
 * @brief Interface to CTR mode.
 *
 *  Overview:  CTR (pronounced "counter") mode is a NIST approved mode of
 *             operation defined in SP 800-38a. It can be used with any
 *             block cipher to provide confidentiality of strings of any
 *             length. TinyCrypt hard codes AES128 as the block cipher.
 *
 *  Security:  CTR mode achieves confidentiality only if the counter value is
 *             never reused with a same encryption key. If the counter is
 *             repeated, than an adversary might be able to defeat the scheme.
 *
 *             A usual method to ensure different counter values refers to
 *             initialize the counter in a given value (0, for example) and
 *             increases it every time a new block is enciphered. This naturally
 *             leaves to a limitation on the number q of blocks that can be
 *             enciphered using a same key: q < 2^(counter size).
 *
 *             TinyCrypt uses a counter of 32 bits. This means that after 2^32
 *             block encryptions, the counter will be reused (thus losing CBC
 *             security). 2^32 block encryptions should be enough for most of
 *             applications targeting constrained devices. Applications intended
 *             to encrypt a larger number of blocks must replace the key after
 *             2^32 block encryptions.
 *
 *             CTR mode provides NO data integrity.
 *
 *  Requires: AES-128
 *
 *  Usage:     1) call tc_ctr_mode to process the data to encrypt/decrypt.
 *
 */

#ifndef __TC_CTR_MODE_H__
#define __TC_CTR_MODE_H__

#include "aes.h"
#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief CTR mode encryption/decryption procedure.
 *  CTR mode encrypts (or decrypts) inlen bytes from in buffer into out buffer
 *  @return returns TC_CRYPTO_SUCCESS (1)
 *          returns TC_CRYPTO_FAIL (0) if:
 *                out == NULL or
 *                in == NULL or
 *                ctr == NULL or
 *                sched == NULL or
 *                inlen == 0 or
 *                outlen == 0 or
 *                inlen != outlen
 *  @note Assumes:- The current value in ctr has NOT been used with sched
 *              - out points to inlen bytes
 *              - in points to inlen bytes
 *              - ctr is an integer counter in littleEndian format
 *              - sched was initialized by aes_set_encrypt_key
 * @param out OUT -- produced ciphertext (plaintext)
 * @param outlen IN -- length of ciphertext buffer in bytes
 * @param in IN -- data to encrypt (or decrypt)
 * @param inlen IN -- length of input data in bytes
 * @param ctr IN/OUT -- the current counter value
 * @param sched IN -- an initialized AES key schedule
 */
int tc_ctr_mode(uint8_t *out, unsigned int outlen, const uint8_t *in,
		unsigned int inlen, uint8_t *ctr, const TCAesKeySched_t sched);

#ifdef __cplusplus
}
#endif

#endif /* __TC_CTR_MODE_H__ */
