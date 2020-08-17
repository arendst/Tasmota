/* ecc_dh.h - TinyCrypt interface to EC-DSA implementation */

/*
 * Copyright (c) 2014, Kenneth MacKay
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

/*
 * Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
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
 * @brief -- Interface to EC-DSA implementation.
 *
 *  Overview: This software is an implementation of EC-DSA. This implementation
 *            uses curve NIST p-256.
 *
 *  Security: The curve NIST p-256 provides approximately 128 bits of security.
 *
 *  Usage:  - To sign: Compute a hash of the data you wish to sign (SHA-2 is
 *          recommended) and pass it in to ecdsa_sign function along with your
 *          private key and a random number. You must use a new non-predictable
 *          random number to generate each new signature.
 *          - To verify a signature: Compute the hash of the signed data using
 *          the same hash as the signer and pass it to this function along with
 *          the signer's public key and the signature values (r and s).
 */

#ifndef __TC_ECC_DSA_H__
#define __TC_ECC_DSA_H__

#include <tinycrypt/ecc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate an ECDSA signature for a given hash value.
 * @return returns TC_CRYPTO_SUCCESS (1) if the signature generated successfully
 *         returns TC_CRYPTO_FAIL (0) if an error occurred.
 *
 * @param p_private_key IN -- Your private key.
 * @param p_message_hash IN -- The hash of the message to sign.
 * @param p_hash_size IN -- The size of p_message_hash in bytes.
 * @param p_signature OUT -- Will be filled in with the signature value. Must be
 * at least 2 * curve size long (for secp256r1, signature must be 64 bytes long).
 *
 * @warning A cryptographically-secure PRNG function must be set (using
 * uECC_set_rng()) before calling uECC_sign().
 * @note Usage: Compute a hash of the data you wish to sign (SHA-2 is
 * recommended) and pass it in to this function along with your private key.
 * @note side-channel countermeasure: algorithm strengthened against timing
 * attack.
 */
int uECC_sign(const uint8_t *p_private_key, const uint8_t *p_message_hash,
	      unsigned p_hash_size, uint8_t *p_signature, uECC_Curve curve);

#ifdef ENABLE_TESTS
/*
 * THIS FUNCTION SHOULD BE CALLED FOR TEST PURPOSES ONLY.
 * Refer to uECC_sign() function for real applications.
 */
int uECC_sign_with_k(const uint8_t *private_key, const uint8_t *message_hash,
		     unsigned int hash_size, uECC_word_t *k, uint8_t *signature,
		     uECC_Curve curve);
#endif

/**
 * @brief Verify an ECDSA signature.
 * @return returns TC_SUCCESS (1) if the signature is valid
 * 	   returns TC_FAIL (0) if the signature is invalid.
 *
 * @param p_public_key IN -- The signer's public key.
 * @param p_message_hash IN -- The hash of the signed data.
 * @param p_hash_size IN -- The size of p_message_hash in bytes.
 * @param p_signature IN -- The signature values.
 *
 * @note Usage: Compute the hash of the signed data using the same hash as the
 * signer and pass it to this function along with the signer's public key and
 * the signature values (hash_size and signature).
 */
int uECC_verify(const uint8_t *p_public_key, const uint8_t *p_message_hash,
		unsigned int p_hash_size, const uint8_t *p_signature, uECC_Curve curve);

#ifdef __cplusplus
}
#endif

#endif /* __TC_ECC_DSA_H__ */
