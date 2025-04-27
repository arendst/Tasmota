/*
 * Copyright (c) 2021 Daniel Hope (www.floorsense.nz)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 *  list of conditions and the following disclaimer in the documentation and/or
 *  other materials provided with the distribution.
 *
 * 3. Neither the name of "Floorsense Ltd", "Agile Workspace Ltd" nor the names of
 *  its contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Daniel Hope <daniel.hope@smartalock.com>
 */

// AEAD_CHACHA20_POLY1305 as described in https://tools.ietf.org/html/rfc7539
// AEAD_XChaCha20_Poly1305 as described in https://tools.ietf.org/id/draft-arciszewski-xchacha-02.html
#include "chacha20poly1305.h"
#include "chacha20.h"
#include "poly1305-donna.h"

#include <stdlib.h>
#include <stdint.h>
#include "../../crypto.h"

#define POLY1305_KEY_SIZE		32
#define POLY1305_MAC_SIZE		16

static const uint8_t zero[CHACHA20_BLOCK_SIZE] = { 0 };

// 2.6.  Generating the Poly1305 Key Using ChaCha20
static void generate_poly1305_key(struct poly1305_context *poly1305_state, struct chacha20_ctx *chacha20_state, const uint8_t *key, uint64_t nonce) {
	uint8_t block[POLY1305_KEY_SIZE] = {0};

	// The method is to call the block function with the following parameters:
	// - The 256-bit session integrity key is used as the ChaCha20 key.
	// - The block counter is set to zero.
	// - The protocol will specify a 96-bit or 64-bit nonce
	chacha20_init(chacha20_state, key, nonce);

	// We take the first 256 bits or the serialized state, and use those as the one-time Poly1305 key
	chacha20(chacha20_state, block, block, sizeof(block));

	poly1305_init(poly1305_state, block);

	crypto_zero(&block, sizeof(block));
}

// 2.8.  AEAD Construction (Encryption)
void chacha20poly1305_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, uint64_t nonce, const uint8_t *key) {
	struct poly1305_context poly1305_state;
	struct chacha20_ctx chacha20_state;
	uint8_t block[8];
	size_t padded_len;

	// First, a Poly1305 one-time key is generated from the 256-bit key and nonce using the procedure described in Section 2.6.
	generate_poly1305_key(&poly1305_state, &chacha20_state, key, nonce);

	// Next, the ChaCha20 encryption function is called to encrypt the plaintext, using the same key and nonce, and with the initial counter set to 1.
	chacha20(&chacha20_state, dst, src, src_len);

	// Finally, the Poly1305 function is called with the Poly1305 key calculated above, and a message constructed as a concatenation of the following:
	// - The AAD
	poly1305_update(&poly1305_state, ad, ad_len);
	// - padding1 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16
	padded_len = (ad_len + 15) & 0xFFFFFFF0; // Round up to next 16 bytes
	poly1305_update(&poly1305_state, zero, padded_len - ad_len);
	// - The ciphertext
	poly1305_update(&poly1305_state, dst, src_len);
	// - padding2 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16.
	padded_len = (src_len + 15) & 0xFFFFFFF0; // Round up to next 16 bytes
	poly1305_update(&poly1305_state, zero, padded_len - src_len);
	// - The length of the additional data in octets (as a 64-bit little-endian integer)
	U64TO8_LITTLE(block, (uint64_t)ad_len);
	poly1305_update(&poly1305_state, block, sizeof(block));
	// - The length of the ciphertext in octets (as a 64-bit little-endian integer).
	U64TO8_LITTLE(block, (uint64_t)src_len);
	poly1305_update(&poly1305_state, block, sizeof(block));

	// The output from the AEAD is twofold:
	// - A ciphertext of the same length as the plaintext. (above, output of chacha20 into dst)
	// - A 128-bit tag, which is the output of the Poly1305 function. (append to dst)
	poly1305_finish(&poly1305_state, dst + src_len);

	// Make sure we leave nothing sensitive on the stack
	crypto_zero(&chacha20_state, sizeof(chacha20_state));
	crypto_zero(&block, sizeof(block));
}

// 2.8.  AEAD Construction (Decryption)
bool chacha20poly1305_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, uint64_t nonce, const uint8_t *key) {
	struct poly1305_context poly1305_state;
	struct chacha20_ctx chacha20_state;
	uint8_t block[8];
	uint8_t mac[POLY1305_MAC_SIZE];
	size_t padded_len;
	int dst_len;
	bool result = false;

	// Decryption is similar [to encryption] with the following differences:
	// - The roles of ciphertext and plaintext are reversed, so the ChaCha20 encryption function is applied to the ciphertext, producing the plaintext.
	// - The Poly1305 function is still run on the AAD and the ciphertext, not the plaintext.
	// - The calculated tag is bitwise compared to the received tag.  The message is authenticated if and only if the tags match.

	if (src_len >= POLY1305_MAC_SIZE) {
		dst_len = src_len - POLY1305_MAC_SIZE;

		// First, a Poly1305 one-time key is generated from the 256-bit key and nonce using the procedure described in Section 2.6.
		generate_poly1305_key(&poly1305_state, &chacha20_state, key, nonce);

		// Calculate the MAC before attempting decryption

		// the Poly1305 function is called with the Poly1305 key calculated above, and a message constructed as a concatenation of the following:
		// - The AAD
		poly1305_update(&poly1305_state, ad, ad_len);
		// - padding1 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16
		padded_len = (ad_len + 15) & 0xFFFFFFF0; // Round up to next 16 bytes
		poly1305_update(&poly1305_state, zero, padded_len - ad_len);
		// - The ciphertext (note the Poly1305 function is still run on the AAD and the ciphertext, not the plaintext)
		poly1305_update(&poly1305_state, src, dst_len);
		// - padding2 -- the padding is up to 15 zero bytes, and it brings the total length so far to an integral multiple of 16.
		padded_len = (dst_len + 15) & 0xFFFFFFF0; // Round up to next 16 bytes
		poly1305_update(&poly1305_state, zero, padded_len - dst_len);
		// - The length of the additional data in octets (as a 64-bit little-endian integer)
		U64TO8_LITTLE(block, (uint64_t)ad_len);
		poly1305_update(&poly1305_state, block, sizeof(block));
		// - The length of the ciphertext in octets (as a 64-bit little-endian integer).
		U64TO8_LITTLE(block, (uint64_t)dst_len);
		poly1305_update(&poly1305_state, block, sizeof(block));

		// The output from the AEAD is twofold:
		// - A plaintext of the same length as the ciphertext. (below, output of chacha20 into dst)
		// - A 128-bit tag, which is the output of the Poly1305 function. (into mac for checking against passed mac)
		poly1305_finish(&poly1305_state, mac);


		if (crypto_equal(mac, src + dst_len, POLY1305_MAC_SIZE)) {
			// mac is correct - do the decryption
			// Next, the ChaCha20 encryption function is called to decrypt the ciphertext, using the same key and nonce, and with the initial counter set to 1.
			chacha20(&chacha20_state, dst, src, dst_len);
			result = true;
		}
	}
	return result;
}

// AEAD_XChaCha20_Poly1305
// XChaCha20-Poly1305 is a variant of the ChaCha20-Poly1305 AEAD construction as defined in [RFC7539] that uses a 192-bit nonce instead of a 96-bit nonce.
// The algorithm for XChaCha20-Poly1305 is as follows:
// 1. Calculate a subkey from the first 16 bytes of the nonce and the key, using HChaCha20 (Section 2.2).
// 2. Use the subkey and remaining 8 bytes of the nonce (prefixed with 4 NUL bytes) with AEAD_CHACHA20_POLY1305 from [RFC7539] as normal. The definition for XChaCha20 is given in Section 2.3.
void xchacha20poly1305_encrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, const uint8_t *nonce, const uint8_t *key) {
	uint8_t subkey[CHACHA20_KEY_SIZE];
	uint64_t new_nonce;

	new_nonce = U8TO64_LITTLE(nonce + 16);

	hchacha20(subkey, nonce, key);
	chacha20poly1305_encrypt(dst, src, src_len, ad, ad_len, new_nonce, subkey);

	crypto_zero(subkey, sizeof(subkey));
}

bool xchacha20poly1305_decrypt(uint8_t *dst, const uint8_t *src, size_t src_len, const uint8_t *ad, size_t ad_len, const uint8_t *nonce, const uint8_t *key) {
	uint8_t subkey[CHACHA20_KEY_SIZE];
	uint64_t new_nonce;
	bool result;

	new_nonce = U8TO64_LITTLE(nonce + 16);

	hchacha20(subkey, nonce, key);
	result = chacha20poly1305_decrypt(dst, src, src_len, ad, ad_len, new_nonce, subkey);

	crypto_zero(subkey, sizeof(subkey));
	return result;
}
