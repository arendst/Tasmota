/* ecc_dh.h - TinyCrypt interface to EC-DH implementation */

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

/* Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
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
 * @brief -- Interface to EC-DH implementation.
 *
 *  Overview: This software is an implementation of EC-DH. This implementation
 *            uses curve NIST p-256.
 *
 *  Security: The curve NIST p-256 provides approximately 128 bits of security.
 */

#ifndef __TC_ECC_DH_H__
#define __TC_ECC_DH_H__

#include "ecc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Create a public/private key pair.
 * @return returns TC_CRYPTO_SUCCESS (1) if the key pair was generated successfully
 *         returns TC_CRYPTO_FAIL (0) if error while generating key pair
 *
 * @param p_public_key OUT -- Will be filled in with the public key. Must be at
 * least 2 * the curve size (in bytes) long. For curve secp256r1, p_public_key
 * must be 64 bytes long.
 * @param p_private_key OUT -- Will be filled in with the private key. Must be as
 * long as the curve order (for secp256r1, p_private_key must be 32 bytes long).
 *
 * @note side-channel countermeasure: algorithm strengthened against timing
 * attack.
 * @warning A cryptographically-secure PRNG function must be set (using
 * uECC_set_rng()) before calling uECC_make_key().
 */
int uECC_make_key(uint8_t *p_public_key, uint8_t *p_private_key, uECC_Curve curve);

#ifdef ENABLE_TESTS

/**
 * @brief Create a public/private key pair given a specific d.
 *
 * @note THIS FUNCTION SHOULD BE CALLED ONLY FOR TEST PURPOSES. Refer to
 * uECC_make_key() function for real applications.
 */
int uECC_make_key_with_d(uint8_t *p_public_key, uint8_t *p_private_key,
    			 unsigned int *d, uECC_Curve curve);
#endif

/**
 * @brief Compute a shared secret given your secret key and someone else's
 * public key.
 * @return returns TC_CRYPTO_SUCCESS (1) if the shared secret was computed successfully
 *         returns TC_CRYPTO_FAIL (0) otherwise
 *
 * @param p_secret OUT -- Will be filled in with the shared secret value. Must be
 * the same size as the curve size (for curve secp256r1, secret must be 32 bytes
 * long.
 * @param p_public_key IN -- The public key of the remote party.
 * @param p_private_key IN -- Your private key.
 *
 * @warning It is recommended to use the output of uECC_shared_secret() as the
 * input of a recommended Key Derivation Function (see NIST SP 800-108) in
 * order to produce a cryptographically secure symmetric key.
 */
int uECC_shared_secret(const uint8_t *p_public_key, const uint8_t *p_private_key,
		       uint8_t *p_secret, uECC_Curve curve);

#ifdef __cplusplus
}
#endif

#endif /* __TC_ECC_DH_H__ */
