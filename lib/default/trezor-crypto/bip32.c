/**
 * Copyright (c) 2013-2016 Tomas Dzetkulic
 * Copyright (c) 2013-2016 Pavol Rusnak
 * Copyright (c) 2015-2016 Jochen Hoenicke
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <stdbool.h>

#include "aes/aes.h"
#include "address.h"
#include "bignum.h"
#include "hmac.h"
#include "ecdsa.h"
#include "bip32.h"
#include "sha2.h"
#include "sha3.h"
#include "ripemd160.h"
#include "base58.h"
#include "macros.h"
#include "curves.h"
#include "secp256k1.h"
#include "nist256p1.h"
#include "ed25519-donna/ed25519.h"
#include "ed25519-donna/ed25519-sha3.h"
#if USE_KECCAK
#include "ed25519-donna/ed25519-keccak.h"
#endif
#if USE_NEM
#include "nem.h"
#endif

const curve_info ed25519_info = {
	.bip32_name = "ed25519 seed",
	.params = NULL,
	.hasher_type = HASHER_SHA2,
};

const curve_info ed25519_sha3_info = {
	.bip32_name = "ed25519-sha3 seed",
	.params = NULL,
	.hasher_type = HASHER_SHA2,
};

#if USE_KECCAK
const curve_info ed25519_keccak_info = {
	.bip32_name = "ed25519-keccak seed",
	.params = NULL,
	.hasher_type = HASHER_SHA2,
};
#endif

const curve_info curve25519_info = {
	.bip32_name = "curve25519 seed",
	.params = NULL,
	.hasher_type = HASHER_SHA2,
};

int hdnode_from_xpub(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *public_key, const char* curve, HDNode *out)
{
	const curve_info *info = get_curve_by_name(curve);
	if (info == 0) {
		return 0;
	}
	if (public_key[0] != 0x02 && public_key[0] != 0x03) { // invalid pubkey
		return 0;
	}
	out->curve = info;
	out->depth = depth;
	out->child_num = child_num;
	memcpy(out->chain_code, chain_code, 32);
	MEMSET_BZERO(out->private_key, 32);
	memcpy(out->public_key, public_key, 33);
	return 1;
}

int hdnode_from_xprv(uint32_t depth, uint32_t child_num, const uint8_t *chain_code, const uint8_t *private_key, const char* curve, HDNode *out)
{
	bool failed = false;
	const curve_info *info = get_curve_by_name(curve);
	if (info == 0) {
		failed = true;
	} else if (info->params) {
		bignum256 a;
		bn_read_be(private_key, &a);
		if (bn_is_zero(&a)) { // == 0
			failed = true;
		} else {
			if (!bn_is_less(&a, &info->params->order)) { // >= order
				failed = true;
			}
		}
		MEMSET_BZERO(&a, sizeof(a));
	}

	if (failed) {
		return 0;
	}

	out->curve = info;
	out->depth = depth;
	out->child_num = child_num;
	memcpy(out->chain_code, chain_code, 32);
	memcpy(out->private_key, private_key, 32);
	MEMSET_BZERO(out->public_key, sizeof(out->public_key));
	return 1;
}

int hdnode_from_seed(const uint8_t *seed, int seed_len, const char* curve, HDNode *out)
{
	static CONFIDENTIAL uint8_t I[32 + 32];
	memset(out, 0, sizeof(HDNode));
	out->depth = 0;
	out->child_num = 0;
	out->curve = get_curve_by_name(curve);
	if (out->curve == 0) {
		return 0;
	}
	static CONFIDENTIAL HMAC_SHA512_CTX ctx;
	hmac_sha512_Init(&ctx, (const uint8_t*) out->curve->bip32_name, strlen(out->curve->bip32_name));
	hmac_sha512_Update(&ctx, seed, seed_len);
	hmac_sha512_Final(&ctx, I);

	if (out->curve->params) {
		bignum256 a;
		while (true) {
			bn_read_be(I, &a);
			if (!bn_is_zero(&a) // != 0
				&& bn_is_less(&a, &out->curve->params->order)) { // < order
				break;
			}
			hmac_sha512_Init(&ctx, (const uint8_t*) out->curve->bip32_name, strlen(out->curve->bip32_name));
			hmac_sha512_Update(&ctx, I, sizeof(I));
			hmac_sha512_Final(&ctx, I);
		}
		MEMSET_BZERO(&a, sizeof(a));
	}
	memcpy(out->private_key, I, 32);
	memcpy(out->chain_code, I + 32, 32);
	MEMSET_BZERO(out->public_key, sizeof(out->public_key));
	MEMSET_BZERO(I, sizeof(I));
	return 1;
}

uint32_t hdnode_fingerprint(HDNode *node)
{
	uint8_t digest[32];
	uint32_t fingerprint;

	hdnode_fill_public_key(node);
	hasher_Raw(node->curve->hasher_type, node->public_key, 33, digest);
	ripemd160(digest, 32, digest);
	fingerprint = (digest[0] << 24) + (digest[1] << 16) + (digest[2] << 8) + digest[3];
	MEMSET_BZERO(digest, sizeof(digest));
	return fingerprint;
}

int hdnode_private_ckd(HDNode *inout, uint32_t i)
{
	static CONFIDENTIAL uint8_t data[1 + 32 + 4];
	static CONFIDENTIAL uint8_t I[32 + 32];
	static CONFIDENTIAL bignum256 a, b;

	if (i & 0x80000000) { // private derivation
		data[0] = 0;
		memcpy(data + 1, inout->private_key, 32);
	} else { // public derivation
		if (!inout->curve->params) {
			return 0;
		}
		hdnode_fill_public_key(inout);
		memcpy(data, inout->public_key, 33);
	}
	write_be(data + 33, i);

	bn_read_be(inout->private_key, &a);

	static CONFIDENTIAL HMAC_SHA512_CTX ctx;
	hmac_sha512_Init(&ctx, inout->chain_code, 32);
	hmac_sha512_Update(&ctx, data, sizeof(data));
	hmac_sha512_Final(&ctx, I);

	if (inout->curve->params) {
		while (true) {
			bool failed = false;
			bn_read_be(I, &b);
			if (!bn_is_less(&b, &inout->curve->params->order)) { // >= order
				failed = true;
			} else {
				bn_add(&b, &a);
				bn_mod(&b, &inout->curve->params->order);
				if (bn_is_zero(&b)) {
					failed = true;
				}
			}

			if (!failed) {
				bn_write_be(&b, inout->private_key);
				break;
			}

			data[0] = 1;
			memcpy(data + 1, I + 32, 32);
			hmac_sha512_Init(&ctx, inout->chain_code, 32);
			hmac_sha512_Update(&ctx, data, sizeof(data));
			hmac_sha512_Final(&ctx, I);
		}
	} else {
		memcpy(inout->private_key, I, 32);
	}

	memcpy(inout->chain_code, I + 32, 32);
	inout->depth++;
	inout->child_num = i;
	MEMSET_BZERO(inout->public_key, sizeof(inout->public_key));

	// making sure to wipe our memory
	MEMSET_BZERO(&a, sizeof(a));
	MEMSET_BZERO(&b, sizeof(b));
	MEMSET_BZERO(I, sizeof(I));
	MEMSET_BZERO(data, sizeof(data));
	return 1;
}

int hdnode_public_ckd_cp(const ecdsa_curve *curve, const curve_point *parent, const uint8_t *parent_chain_code, uint32_t i, curve_point *child, uint8_t *child_chain_code) {
	uint8_t data[1 + 32 + 4];
	uint8_t I[32 + 32];
	bignum256 c;

	if (i & 0x80000000) { // private derivation
		return 0;
	}

	data[0] = 0x02 | (parent->y.val[0] & 0x01);
	bn_write_be(&parent->x, data + 1);
	write_be(data + 33, i);

	while (true) {
		hmac_sha512(parent_chain_code, 32, data, sizeof(data), I);
		bn_read_be(I, &c);
		if (bn_is_less(&c, &curve->order)) { // < order
			scalar_multiply(curve, &c, child); // b = c * G
			point_add(curve, parent, child);       // b = a + b
			if (!point_is_infinity(child)) {
				if (child_chain_code) {
					memcpy(child_chain_code, I + 32, 32);
				}

				// Wipe all stack data.
				MEMSET_BZERO(data, sizeof(data));
				MEMSET_BZERO(I, sizeof(I));
				MEMSET_BZERO(&c, sizeof(c));
				return 1;
			}
		}

		data[0] = 1;
		memcpy(data + 1, I + 32, 32);
	}
}

int hdnode_public_ckd(HDNode *inout, uint32_t i)
{
	curve_point parent, child;

	if (!ecdsa_read_pubkey(inout->curve->params, inout->public_key, &parent)) {
		return 0;
	}
	if (!hdnode_public_ckd_cp(inout->curve->params, &parent, inout->chain_code, i, &child, inout->chain_code)) {
		return 0;
	}
	memset(inout->private_key, 0, 32);
	inout->depth++;
	inout->child_num = i;
	inout->public_key[0] = 0x02 | (child.y.val[0] & 0x01);
	bn_write_be(&child.x, inout->public_key + 1);

	// Wipe all stack data.
	MEMSET_BZERO(&parent, sizeof(parent));
	MEMSET_BZERO(&child, sizeof(child));

	return 1;
}

void hdnode_public_ckd_address_optimized(const curve_point *pub, const uint8_t *chain_code, uint32_t i, uint32_t version, HasherType hasher_type, char *addr, int addrsize, int addrformat)
{
	uint8_t child_pubkey[33];
	curve_point b;

	hdnode_public_ckd_cp(&secp256k1, pub, chain_code, i, &b, NULL);
	child_pubkey[0] = 0x02 | (b.y.val[0] & 0x01);
	bn_write_be(&b.x, child_pubkey + 1);

	switch (addrformat) {
		case 1: // Segwit-in-P2SH
			ecdsa_get_address_segwit_p2sh(child_pubkey, version, hasher_type, addr, addrsize);
			break;
		default: // normal address
			ecdsa_get_address(child_pubkey, version, hasher_type, addr, addrsize);
			break;
	}
}

#if USE_BIP32_CACHE
static bool private_ckd_cache_root_set = false;
static CONFIDENTIAL HDNode private_ckd_cache_root;
static int private_ckd_cache_index = 0;

static CONFIDENTIAL struct {
	bool set;
	size_t depth;
	uint32_t i[BIP32_CACHE_MAXDEPTH];
	HDNode node;
} private_ckd_cache[BIP32_CACHE_SIZE];

int hdnode_private_ckd_cached(HDNode *inout, const uint32_t *i, size_t i_count, uint32_t *fingerprint)
{
	if (i_count == 0) {
		// no way how to compute parent fingerprint
		return 1;
	}
	if (i_count == 1) {
		if (fingerprint) {
			*fingerprint = hdnode_fingerprint(inout);
		}
		if (hdnode_private_ckd(inout, i[0]) == 0) return 0;
		return 1;
	}

	bool found = false;
	// if root is not set or not the same
	if (!private_ckd_cache_root_set || memcmp(&private_ckd_cache_root, inout, sizeof(HDNode)) != 0) {
		// clear the cache
		private_ckd_cache_index = 0;
		memset(private_ckd_cache, 0, sizeof(private_ckd_cache));
		// setup new root
		memcpy(&private_ckd_cache_root, inout, sizeof(HDNode));
		private_ckd_cache_root_set = true;
	} else {
		// try to find parent
		int j;
		for (j = 0; j < BIP32_CACHE_SIZE; j++) {
			if (private_ckd_cache[j].set &&
			    private_ckd_cache[j].depth == i_count - 1 &&
			    memcmp(private_ckd_cache[j].i, i, (i_count - 1) * sizeof(uint32_t)) == 0 &&
				private_ckd_cache[j].node.curve == inout->curve) {
				memcpy(inout, &(private_ckd_cache[j].node), sizeof(HDNode));
				found = true;
				break;
			}
		}
	}

	// else derive parent
	if (!found) {
		size_t k;
		for (k = 0; k < i_count - 1; k++) {
			if (hdnode_private_ckd(inout, i[k]) == 0) return 0;
		}
		// and save it
		memset(&(private_ckd_cache[private_ckd_cache_index]), 0, sizeof(private_ckd_cache[private_ckd_cache_index]));
		private_ckd_cache[private_ckd_cache_index].set = true;
		private_ckd_cache[private_ckd_cache_index].depth = i_count - 1;
		memcpy(private_ckd_cache[private_ckd_cache_index].i, i, (i_count - 1) * sizeof(uint32_t));
		memcpy(&(private_ckd_cache[private_ckd_cache_index].node), inout, sizeof(HDNode));
		private_ckd_cache_index = (private_ckd_cache_index + 1) % BIP32_CACHE_SIZE;
	}

	if (fingerprint) {
		*fingerprint = hdnode_fingerprint(inout);
	}
	if (hdnode_private_ckd(inout, i[i_count - 1]) == 0) return 0;

	return 1;
}
#endif

void hdnode_get_address_raw(HDNode *node, uint32_t version, uint8_t *addr_raw)
{
	hdnode_fill_public_key(node);
	ecdsa_get_address_raw(node->public_key, version, node->curve->hasher_type, addr_raw);
}

void hdnode_get_address(HDNode *node, uint32_t version, char *addr, int addrsize)
{
	hdnode_fill_public_key(node);
	ecdsa_get_address(node->public_key, version, node->curve->hasher_type, addr, addrsize);
}

void hdnode_fill_public_key(HDNode *node)
{
	if (node->public_key[0] != 0)
		return;
	if (node->curve->params) {
		ecdsa_get_public_key33(node->curve->params, node->private_key, node->public_key);
	} /*else {
		node->public_key[0] = 1;
		if (node->curve == &ed25519_info) {
			ed25519_publickey(node->private_key, node->public_key + 1);
		} else if (node->curve == &ed25519_sha3_info) {
			ed25519_publickey_sha3(node->private_key, node->public_key + 1);
#if USE_KECCAK
		} else if (node->curve == &ed25519_keccak_info) {
			ed25519_publickey_keccak(node->private_key, node->public_key + 1);
#endif
		} else if (node->curve == &curve25519_info) {
			curve25519_scalarmult_basepoint(node->public_key + 1, node->private_key);
		}
	} */
}

#if USE_ETHEREUM
int hdnode_get_ethereum_pubkeyhash(const HDNode *node, uint8_t *pubkeyhash)
{
	uint8_t buf[65];
	SHA3_CTX ctx;

	/* get uncompressed public key */
	ecdsa_get_public_key65(node->curve->params, node->private_key, buf);

	/* compute sha3 of x and y coordinate without 04 prefix */
	sha3_256_Init(&ctx);
	sha3_Update(&ctx, buf + 1, 64);
	keccak_Final(&ctx, buf);

	/* result are the least significant 160 bits */
	memcpy(pubkeyhash, buf + 12, 20);

	return 1;
}
#endif

#if USE_NEM
int hdnode_get_nem_address(HDNode *node, uint8_t version, char *address) {
	if (node->curve != &ed25519_keccak_info) {
		return 0;
	}

	hdnode_fill_public_key(node);
	return nem_get_address(&node->public_key[1], version, address);
}

int hdnode_get_nem_shared_key(const HDNode *node, const ed25519_public_key peer_public_key, const uint8_t *salt, ed25519_public_key mul, uint8_t *shared_key) {
	if (node->curve != &ed25519_keccak_info) {
		return 0;
	}

	// sizeof(ed25519_public_key) == SHA3_256_DIGEST_LENGTH
	if (mul == NULL) mul = shared_key;

	if (ed25519_scalarmult_keccak(mul, node->private_key, peer_public_key)) {
		return 0;
	}

	for (size_t i = 0; i < 32; i++) {
		shared_key[i] = mul[i] ^ salt[i];
	}

	keccak_256(shared_key, 32, shared_key);
	return 1;
}

int hdnode_nem_encrypt(const HDNode *node, const ed25519_public_key public_key, uint8_t *iv, const uint8_t *salt, const uint8_t *payload, size_t size, uint8_t *buffer) {
	uint8_t last_block[AES_BLOCK_SIZE];
	uint8_t remainder = size % AES_BLOCK_SIZE;

	// Round down to last whole block
	size -= remainder;
	// Copy old last block
	memcpy(last_block, &payload[size], remainder);
	// Pad new last block with number of missing bytes
	memset(&last_block[remainder], AES_BLOCK_SIZE - remainder, AES_BLOCK_SIZE - remainder);

	uint8_t shared_key[SHA3_256_DIGEST_LENGTH];
	if (!hdnode_get_nem_shared_key(node, public_key, salt, NULL, shared_key)) {
		return 0;
	}

	trzr_aes_encrypt_ctx ctx;

	int ret = trzr_aes_encrypt_key256(shared_key, &ctx);
	MEMSET_BZERO(shared_key, sizeof(shared_key));

	if (ret != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_encrypt(payload, buffer, size, iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_encrypt(last_block, &buffer[size], sizeof(last_block), iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	return 1;
}

int hdnode_nem_decrypt(const HDNode *node, const ed25519_public_key public_key, uint8_t *iv, const uint8_t *salt, const uint8_t *payload, size_t size, uint8_t *buffer) {
	uint8_t shared_key[SHA3_256_DIGEST_LENGTH];

	if (!hdnode_get_nem_shared_key(node, public_key, salt, NULL, shared_key)) {
		return 0;
	}

	trzr_aes_decrypt_ctx ctx;

	int ret = trzr_aes_decrypt_key256(shared_key, &ctx);
	MEMSET_BZERO(shared_key, sizeof(shared_key));

	if (ret != EXIT_SUCCESS) {
		return 0;
	}

	if (aes_cbc_decrypt(payload, buffer, size, iv, &ctx) != EXIT_SUCCESS) {
		return 0;
	}

	return 1;
}
#endif

// msg is a data to be signed
// msg_len is the message length
int hdnode_sign(HDNode *node, const uint8_t *msg, uint32_t msg_len, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]))
{
	if (node->curve->params) {
		return ecdsa_sign(node->curve->params, node->curve->hasher_type, node->private_key, msg, msg_len, sig, pby, is_canonical);
	} else if (node->curve == &curve25519_info) {
		return 1;  // signatures are not supported
	} else {
		hdnode_fill_public_key(node);
		if (node->curve == &ed25519_info) {
			ed25519_sign(msg, msg_len, node->private_key, node->public_key + 1, sig);
		} else if (node->curve == &ed25519_sha3_info) {
			ed25519_sign_sha3(msg, msg_len, node->private_key, node->public_key + 1, sig);
#if USE_KECCAK
		} else if (node->curve == &ed25519_keccak_info) {
			ed25519_sign_keccak(msg, msg_len, node->private_key, node->public_key + 1, sig);
#endif
		}
		return 0;
	}
}

int hdnode_sign_digest(HDNode *node, const uint8_t *digest, uint8_t *sig, uint8_t *pby, int (*is_canonical)(uint8_t by, uint8_t sig[64]))
{
	if (node->curve->params) {
		return ecdsa_sign_digest(node->curve->params, node->private_key, digest, sig, pby, is_canonical);
	} else if (node->curve == &curve25519_info) {
		return 1;  // signatures are not supported
	} else {
		return hdnode_sign(node, digest, 32, sig, pby, is_canonical);
	}
}

int hdnode_get_shared_key(const HDNode *node, const uint8_t *peer_public_key, uint8_t *session_key, int *result_size)
{
	// Use elliptic curve Diffie-Helman to compute shared session key
	if (node->curve->params) {
		if (ecdh_multiply(node->curve->params, node->private_key, peer_public_key, session_key) != 0) {
			return 1;
		}
		*result_size = 65;
		return 0;
	} else if (node->curve == &curve25519_info) {
		session_key[0] = 0x04;
		if (peer_public_key[0] != 0x40) {
			return 1;  // Curve25519 public key should start with 0x40 byte.
		}
		curve25519_scalarmult(session_key + 1, node->private_key, peer_public_key + 1);
		*result_size = 33;
		return 0;
	} else {
		*result_size = 0;
		return 1;  // ECDH is not supported
	}
}

static int hdnode_serialize(const HDNode *node, uint32_t fingerprint, uint32_t version, char use_public, char *str, int strsize)
{
	uint8_t node_data[78];
	write_be(node_data, version);
	node_data[4] = node->depth;
	write_be(node_data + 5, fingerprint);
	write_be(node_data + 9, node->child_num);
	memcpy(node_data + 13, node->chain_code, 32);
	if (use_public) {
		memcpy(node_data + 45, node->public_key, 33);
	} else {
		node_data[45] = 0;
		memcpy(node_data + 46, node->private_key, 32);
	}
	int ret = base58_encode_check(node_data, sizeof(node_data), node->curve->hasher_type, str, strsize);
	MEMSET_BZERO(node_data, sizeof(node_data));
	return ret;
}

int hdnode_serialize_public(const HDNode *node, uint32_t fingerprint, uint32_t version, char *str, int strsize)
{
	return hdnode_serialize(node, fingerprint, version, 1, str, strsize);
}

int hdnode_serialize_private(const HDNode *node, uint32_t fingerprint, uint32_t version, char *str, int strsize)
{
	return hdnode_serialize(node, fingerprint, version, 0, str, strsize);
}

// check for validity of curve point in case of public data not performed
int hdnode_deserialize(const char *str, uint32_t version_public, uint32_t version_private, const char *curve, HDNode *node, uint32_t *fingerprint)
{
	uint8_t node_data[78];
	memset(node, 0, sizeof(HDNode));
	node->curve = get_curve_by_name(curve);
	if (base58_decode_check(str, node->curve->hasher_type, node_data, sizeof(node_data)) != sizeof(node_data)) {
		return -1;
	}
	uint32_t version = read_be(node_data);
	if (version == version_public) {
		MEMSET_BZERO(node->private_key, sizeof(node->private_key));
		memcpy(node->public_key, node_data + 45, 33);
	} else if (version == version_private) { // private node
		if (node_data[45]) { // invalid data
			return -2;
		}
		memcpy(node->private_key, node_data + 46, 32);
		MEMSET_BZERO(node->public_key, sizeof(node->public_key));
	} else {
		return -3; // invalid version
	}
	node->depth = node_data[4];
	if (fingerprint) {
		*fingerprint = read_be(node_data + 5);
	}
	node->child_num = read_be(node_data + 9);
	memcpy(node->chain_code, node_data + 13, 32);
	return 0;
}

const curve_info *get_curve_by_name(const char *curve_name) {
	if (curve_name == 0) {
		return 0;
	}
	if (strcmp(curve_name, SECP256K1_NAME) == 0) {
		return &secp256k1_info;
	}
	if (strcmp(curve_name, SECP256K1_DECRED_NAME) == 0) {
		return &secp256k1_decred_info;
	}
	if (strcmp(curve_name, NIST256P1_NAME) == 0) {
		return &nist256p1_info;
	}
	if (strcmp(curve_name, ED25519_NAME) == 0) {
		return &ed25519_info;
	}
	if (strcmp(curve_name, ED25519_SHA3_NAME) == 0) {
		return &ed25519_sha3_info;
	}
#if USE_KECCAK
	if (strcmp(curve_name, ED25519_KECCAK_NAME) == 0) {
		return &ed25519_keccak_info;
	}
#endif
	if (strcmp(curve_name, CURVE25519_NAME) == 0) {
		return &curve25519_info;
	}
	return 0;
}
