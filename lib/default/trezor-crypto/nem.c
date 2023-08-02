/**
 * Copyright (c) 2017 Saleem Rashid
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, E1PRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "nem.h"

#include <string.h>

#include "base32.h"
#include "ed25519-keccak.h"
#include "macros.h"
#include "ripemd160.h"
#include "sha3.h"

const char *nem_network_name(uint8_t network) {
	switch (network) {
	case NEM_NETWORK_MAINNET:
		return "NEM Mainnet";
	case NEM_NETWORK_TESTNET:
		return "NEM Testnet";
	case NEM_NETWORK_MIJIN:
		return "Mijin";
	default:
		return NULL;
	}
}

static inline void nem_write_u32(nem_transaction_ctx *ctx, uint32_t data) {
	ctx->buffer[ctx->offset++] = (data >>  0) & 0xff;
	ctx->buffer[ctx->offset++] = (data >>  8) & 0xff;
	ctx->buffer[ctx->offset++] = (data >> 16) & 0xff;
	ctx->buffer[ctx->offset++] = (data >> 24) & 0xff;
}

static inline void nem_write_u64(nem_transaction_ctx *ctx, uint64_t data) {
	nem_write_u32(ctx, (data >>  0) & 0xffffffff);
	nem_write_u32(ctx, (data >> 32) & 0xffffffff);
}

static inline void nem_write(nem_transaction_ctx *ctx, const uint8_t *data, uint32_t length) {
	nem_write_u32(ctx, length);

	memcpy(&ctx->buffer[ctx->offset], data, length);
	ctx->offset += length;
}

static inline bool nem_can_write(nem_transaction_ctx *ctx, size_t needed) {
	return (ctx->offset + needed) <= ctx->size;
}

static inline bool nem_write_mosaic_str(nem_transaction_ctx *ctx, const char *name, const char *value) {
	uint32_t name_length = strlen(name);
	uint32_t value_length = strlen(value);

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + name_length + sizeof(uint32_t) + value_length) \
	serialize_write((uint8_t *) name, name_length) \
	serialize_write((uint8_t *) value, value_length)

#include "nem_serialize.h"

	return true;
}

static inline bool nem_write_mosaic_bool(nem_transaction_ctx *ctx, const char *name, bool value) {
	return nem_write_mosaic_str(ctx, name, value ? "true" : "false");
}

static inline bool nem_write_mosaic_u64(nem_transaction_ctx *ctx, const char *name, uint64_t value) {
	char buffer[21];

	if (bn_format_uint64(value, NULL, NULL, 0, 0, false, buffer, sizeof(buffer)) == 0) {
		return false;
	}

	return nem_write_mosaic_str(ctx, name, buffer);
}

void nem_get_address_raw(const ed25519_public_key public_key, uint8_t version, uint8_t *address) {
	uint8_t hash[SHA3_256_DIGEST_LENGTH];

	/* 1.  Perform 256-bit Sha3 on the public key */
	keccak_256(public_key, sizeof(ed25519_public_key), hash);

	/* 2.  Perform 160-bit Ripemd of hash resulting from step 1. */
	ripemd160(hash, SHA3_256_DIGEST_LENGTH, &address[1]);

	/* 3.  Prepend version byte to Ripemd hash (either 0x68 or 0x98) */
	address[0] = version;

	/* 4.  Perform 256-bit Sha3 on the result, take the first four bytes as a checksum */
	keccak_256(address, 1 + RIPEMD160_DIGEST_LENGTH, hash);

	/* 5.  Concatenate output of step 3 and the checksum from step 4 */
	memcpy(&address[1 + RIPEMD160_DIGEST_LENGTH], hash, 4);

	MEMSET_BZERO(hash, sizeof(hash));
}

bool nem_get_address(const ed25519_public_key public_key, uint8_t version, char *address) {
	uint8_t pubkeyhash[NEM_ADDRESS_SIZE_RAW];

	nem_get_address_raw(public_key, version, pubkeyhash);

	char *ret = base32_encode(pubkeyhash, sizeof(pubkeyhash), address, NEM_ADDRESS_SIZE + 1, BASE32_ALPHABET_RFC4648);

	MEMSET_BZERO(pubkeyhash, sizeof(pubkeyhash));
	return (ret != NULL);
}

bool nem_validate_address_raw(const uint8_t *address, uint8_t network) {
	if (!nem_network_name(network) || address[0] != network) {
		return false;
	}

	uint8_t hash[SHA3_256_DIGEST_LENGTH];

	keccak_256(address, 1 + RIPEMD160_DIGEST_LENGTH, hash);
	bool valid = (memcmp(&address[1 + RIPEMD160_DIGEST_LENGTH], hash, 4) == 0);

	MEMSET_BZERO(hash, sizeof(hash));
	return valid;
}

bool nem_validate_address(const char *address, uint8_t network) {
	uint8_t pubkeyhash[NEM_ADDRESS_SIZE_RAW];

	if (strlen(address) != NEM_ADDRESS_SIZE) {
		return false;
	}

	uint8_t *ret = base32_decode(address, NEM_ADDRESS_SIZE, pubkeyhash, sizeof(pubkeyhash), BASE32_ALPHABET_RFC4648);
	bool valid = (ret != NULL) && nem_validate_address_raw(pubkeyhash, network);

	MEMSET_BZERO(pubkeyhash, sizeof(pubkeyhash));
	return valid;
}

void nem_transaction_start(nem_transaction_ctx *ctx, const ed25519_public_key public_key, uint8_t *buffer, size_t size) {
	memcpy(ctx->public_key, public_key, sizeof(ctx->public_key));

	ctx->buffer = buffer;
	ctx->offset = 0;
	ctx->size = size;
}

size_t nem_transaction_end(nem_transaction_ctx *ctx, const ed25519_secret_key private_key, ed25519_signature signature) {
	if (private_key != NULL && signature != NULL) {
		ed25519_sign_keccak(ctx->buffer, ctx->offset, private_key, ctx->public_key, signature);
	}

	return ctx->offset;
}

bool nem_transaction_write_common(nem_transaction_ctx *ctx,
	uint32_t type,
	uint32_t version,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline) {

#define NEM_SERIALIZE \
	serialize_u32(type) \
	serialize_u32(version) \
	serialize_u32(timestamp) \
	serialize_write(signer, sizeof(ed25519_public_key)) \
	serialize_u64(fee) \
	serialize_u32(deadline)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_transfer(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *recipient,
	uint64_t amount,
	const uint8_t *payload,
	uint32_t length,
	bool encrypted,
	uint32_t mosaics) {

	if (!signer) {
		signer = ctx->public_key;
	}

	if (!payload) {
		length = 0;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_TRANSFER,
		network << 24 | (mosaics ? 2 : 1),
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

#define NEM_SERIALIZE \
	serialize_write((uint8_t *) recipient, NEM_ADDRESS_SIZE) \
	serialize_u64(amount)

#include "nem_serialize.h"

	if (length) {

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + sizeof(uint32_t) + length) \
	serialize_u32(encrypted ? 0x02 : 0x01) \
	serialize_write(payload, length)

#include "nem_serialize.h"

	} else {

#define NEM_SERIALIZE \
	serialize_u32(0)

#include "nem_serialize.h"

	}

	if (mosaics) {

#define NEM_SERIALIZE \
	serialize_u32(mosaics)

#include "nem_serialize.h"

	}

	return true;
}

bool nem_transaction_write_mosaic(nem_transaction_ctx *ctx,
	const char *namespace,
	const char *mosaic,
	uint64_t quantity) {

	size_t namespace_length = strlen(namespace);
	size_t mosaic_length = strlen(mosaic);
	size_t identifier_length = sizeof(uint32_t) + namespace_length + sizeof(uint32_t) + mosaic_length;

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + sizeof(uint64_t) + identifier_length) \
	serialize_u32(identifier_length) \
	serialize_write((uint8_t *) namespace, namespace_length) \
	serialize_write((uint8_t *) mosaic, mosaic_length) \
	serialize_u64(quantity)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_multisig(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const nem_transaction_ctx *inner) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_MULTISIG,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

#define NEM_SERIALIZE \
	serialize_write(inner->buffer, inner->offset)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_multisig_signature(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const nem_transaction_ctx *inner) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_MULTISIG_SIGNATURE,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

	char address[NEM_ADDRESS_SIZE + 1];
	nem_get_address(inner->public_key, network, address);

	uint8_t hash[SHA3_256_DIGEST_LENGTH];
	keccak_256(inner->buffer, inner->offset, hash);

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + SHA3_256_DIGEST_LENGTH) \
	serialize_write(hash, SHA3_256_DIGEST_LENGTH) \
	serialize_write((uint8_t *) address, NEM_ADDRESS_SIZE)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_provision_namespace(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespace,
	const char *parent,
	const char *rental_sink,
	uint64_t rental_fee) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_PROVISION_NAMESPACE,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

	if (parent) {

#define NEM_SERIALIZE \
	serialize_write((uint8_t *) rental_sink, NEM_ADDRESS_SIZE) \
	serialize_u64(rental_fee) \
	serialize_write((uint8_t *) namespace, strlen(namespace)) \
	serialize_write((uint8_t *) parent, strlen(parent))

#include "nem_serialize.h"

	} else {

#define NEM_SERIALIZE \
	serialize_write((uint8_t *) rental_sink, NEM_ADDRESS_SIZE) \
	serialize_u64(rental_fee) \
	serialize_write((uint8_t *) namespace, strlen(namespace)) \
	serialize_u32(0xffffffff)

#include "nem_serialize.h"

	}

	return true;
}

bool nem_transaction_create_mosaic_creation(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespace,
	const char *mosaic,
	const char *description,
	uint32_t divisibility,
	uint64_t supply,
	bool mutable_supply,
	bool transferable,
	uint32_t levy_type,
	uint64_t levy_fee,
	const char *levy_address,
	const char *levy_namespace,
	const char *levy_mosaic,
	const char *creation_sink,
	uint64_t creation_fee) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_MOSAIC_CREATION,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

	size_t namespace_length = strlen(namespace);
	size_t mosaic_length = strlen(mosaic);
	size_t identifier_length = sizeof(uint32_t) + namespace_length + sizeof(uint32_t) + mosaic_length;

	// This length will be rewritten later on
	nem_transaction_ctx state;
	memcpy(&state, ctx, sizeof(state));

#define NEM_SERIALIZE \
	serialize_u32(0) \
	serialize_write(signer, sizeof(ed25519_public_key)) \
	serialize_u32(identifier_length) \
	serialize_write((uint8_t *) namespace, namespace_length) \
	serialize_write((uint8_t *) mosaic, mosaic_length) \
	serialize_write((uint8_t *) description, strlen(description)) \
	serialize_u32(4) // Number of properties

#include "nem_serialize.h"

	if (!nem_write_mosaic_u64(ctx, "divisibility", divisibility)) return false;
	if (!nem_write_mosaic_u64(ctx, "initialSupply", supply)) return false;
	if (!nem_write_mosaic_bool(ctx, "supplyMutable", mutable_supply)) return false;
	if (!nem_write_mosaic_bool(ctx, "transferable", transferable)) return false;

	if (levy_type) {
		size_t levy_namespace_length = strlen(levy_namespace);
		size_t levy_mosaic_length = strlen(levy_mosaic);
		size_t levy_identifier_length = sizeof(uint32_t) + levy_namespace_length + sizeof(uint32_t) + levy_mosaic_length;

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + sizeof(uint32_t) + NEM_ADDRESS_SIZE + sizeof(uint32_t) + levy_identifier_length + sizeof(uint64_t)) \
	serialize_u32(levy_type) \
	serialize_write((uint8_t *) levy_address, NEM_ADDRESS_SIZE) \
	serialize_u32(levy_identifier_length) \
	serialize_write((uint8_t *) levy_namespace, levy_namespace_length) \
	serialize_write((uint8_t *) levy_mosaic, levy_mosaic_length) \
	serialize_u64(levy_fee)

#include "nem_serialize.h"

	} else {

#define NEM_SERIALIZE \
	serialize_u32(0)

#include "nem_serialize.h"

	}

	// Rewrite length
	nem_write_u32(&state, ctx->offset - state.offset - sizeof(uint32_t));

#define NEM_SERIALIZE \
	serialize_write((uint8_t *) creation_sink, NEM_ADDRESS_SIZE) \
	serialize_u64(creation_fee)

#include "nem_serialize.h"

	return true;

}

bool nem_transaction_create_mosaic_supply_change(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespace,
	const char *mosaic,
	uint32_t type,
	uint64_t delta) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_MOSAIC_SUPPLY_CHANGE,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

	size_t namespace_length = strlen(namespace);
	size_t mosaic_length = strlen(mosaic);
	size_t identifier_length = sizeof(uint32_t) + namespace_length + sizeof(uint32_t) + mosaic_length;

#define NEM_SERIALIZE \
	serialize_u32(identifier_length) \
	serialize_write((uint8_t *) namespace, namespace_length) \
	serialize_write((uint8_t *) mosaic, mosaic_length) \
	serialize_u32(type) \
	serialize_u64(delta)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_aggregate_modification(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	uint32_t modifications,
	bool relative_change) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_AGGREGATE_MODIFICATION,
		network << 24 | (relative_change ? 2 : 1),
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

#define NEM_SERIALIZE \
	serialize_u32(modifications)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_write_cosignatory_modification(nem_transaction_ctx *ctx,
	uint32_t type,
	const ed25519_public_key cosignatory) {

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t) + sizeof(uint32_t) + sizeof(ed25519_public_key)) \
	serialize_u32(type) \
	serialize_write(cosignatory, sizeof(ed25519_public_key))

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_write_minimum_cosignatories(nem_transaction_ctx *ctx,
	int32_t relative_change) {

#define NEM_SERIALIZE \
	serialize_u32(sizeof(uint32_t)) \
	serialize_u32((uint32_t) relative_change)

#include "nem_serialize.h"

	return true;
}

bool nem_transaction_create_importance_transfer(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	uint32_t mode,
	const ed25519_public_key remote) {

	if (!signer) {
		signer = ctx->public_key;
	}

	bool ret = nem_transaction_write_common(ctx,
		NEM_TRANSACTION_TYPE_IMPORTANCE_TRANSFER,
		network << 24 | 1,
		timestamp,
		signer,
		fee,
		deadline);
	if (!ret) return false;

#define NEM_SERIALIZE \
	serialize_u32(mode) \
	serialize_write(remote, sizeof(ed25519_public_key))

#include "nem_serialize.h"

	return true;
}
