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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __NEM_H__
#define __NEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "bip32.h"
#include "ed25519.h"

#define NEM_LEVY_PERCENTILE_DIVISOR 4
#define NEM_MAX_DIVISIBILITY        6
#define NEM_MAX_SUPPLY              9000000000

#define NEM_NETWORK_MAINNET 0x68
#define NEM_NETWORK_TESTNET 0x98
#define NEM_NETWORK_MIJIN   0x60

#define NEM_ADDRESS_SIZE 40
#define NEM_ADDRESS_SIZE_RAW 25

#define NEM_TRANSACTION_TYPE_TRANSFER                0x0101
#define NEM_TRANSACTION_TYPE_IMPORTANCE_TRANSFER     0x0801
#define NEM_TRANSACTION_TYPE_AGGREGATE_MODIFICATION  0x1001
#define NEM_TRANSACTION_TYPE_MULTISIG_SIGNATURE      0x1002
#define NEM_TRANSACTION_TYPE_MULTISIG                0x1004
#define NEM_TRANSACTION_TYPE_PROVISION_NAMESPACE     0x2001
#define NEM_TRANSACTION_TYPE_MOSAIC_CREATION         0x4001
#define NEM_TRANSACTION_TYPE_MOSAIC_SUPPLY_CHANGE    0x4002

#define NEM_SALT_SIZE sizeof(ed25519_public_key)

#define NEM_ENCRYPTED_SIZE(size)         (((size) + AES_BLOCK_SIZE) / AES_BLOCK_SIZE * AES_BLOCK_SIZE)
#define NEM_ENCRYPTED_PAYLOAD_SIZE(size) (AES_BLOCK_SIZE + NEM_SALT_SIZE + NEM_ENCRYPTED_SIZE(size))

#define _NEM_PADDING_SIZE(buffer, size)  ((buffer)[(size) - 1])
#define NEM_PADDING_SIZE(buffer, size)   (_NEM_PADDING_SIZE(buffer, size) > (size) ? (size) : _NEM_PADDING_SIZE(buffer, size))

#define NEM_DECRYPTED_SIZE(buffer, size) ((size) - NEM_PADDING_SIZE(buffer, size))

typedef struct {
	ed25519_public_key public_key;
	uint8_t *buffer;
	size_t offset;
	size_t size;
} nem_transaction_ctx;

const char *nem_network_name(uint8_t network);

void nem_get_address_raw(const ed25519_public_key public_key, uint8_t version, uint8_t *address);
bool nem_get_address(const ed25519_public_key public_key, uint8_t version, char *address);

bool nem_validate_address_raw(const uint8_t *address, uint8_t network);
bool nem_validate_address(const char *address, uint8_t network);

void nem_transaction_start(nem_transaction_ctx *ctx, const ed25519_public_key public_key, uint8_t *buffer, size_t size);
size_t nem_transaction_end(nem_transaction_ctx *ctx, const ed25519_secret_key private_key, ed25519_signature signature);

bool nem_transaction_write_common(nem_transaction_ctx *context,
	uint32_t type,
	uint32_t version,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline);

bool nem_transaction_create_transfer(nem_transaction_ctx *context,
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
	uint32_t mosaics);

bool nem_transaction_write_mosaic(nem_transaction_ctx *ctx,
	const char *namespaces,
	const char *mosaic,
	uint64_t quantity);

bool nem_transaction_create_multisig(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const nem_transaction_ctx *inner);

bool nem_transaction_create_multisig_signature(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const nem_transaction_ctx *inner);

bool nem_transaction_create_provision_namespace(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespaces,
	const char *parent,
	const char *rental_sink,
	uint64_t rental_fee);

bool nem_transaction_create_mosaic_creation(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespaces,
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
	uint64_t creation_fee);

bool nem_transaction_create_mosaic_supply_change(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	const char *namespaces,
	const char *mosaic,
	uint32_t type,
	uint64_t delta);

bool nem_transaction_create_aggregate_modification(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	uint32_t modifications,
	bool relative_change);

bool nem_transaction_write_cosignatory_modification(nem_transaction_ctx *ctx,
	uint32_t type,
	const ed25519_public_key cosignatory);

bool nem_transaction_write_minimum_cosignatories(nem_transaction_ctx *ctx,
	int32_t relative_change);

bool nem_transaction_create_importance_transfer(nem_transaction_ctx *ctx,
	uint8_t  network,
	uint32_t timestamp,
	const ed25519_public_key signer,
	uint64_t fee,
	uint32_t deadline,
	uint32_t mode,
	const ed25519_public_key remote);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
