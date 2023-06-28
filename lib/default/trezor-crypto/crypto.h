/*
 * This file is part of the TREZOR project, https://trezor.io/
 *
 * Copyright (C) 2014 Pavol Rusnak <stick@satoshilabs.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ecdsa.h"
#include "bip32.h"
#include "sha2.h"
#include "pb.h"
#include "coins.h"
#include "hasher.h"
#include "types.pb.h"

#define ser_length_size(len) ((len) < 253 ? 1 : (len) < 0x10000 ? 3 : 5)

uint32_t ser_length(uint32_t len, uint8_t *out);

uint32_t ser_length_hash(Hasher *hasher, uint32_t len);

int sshMessageSign(HDNode *node, const uint8_t *message, size_t message_len, uint8_t *signature);

int gpgMessageSign(HDNode *node, const uint8_t *message, size_t message_len, uint8_t *signature);

int cryptoMessageSign(const CoinInfo *coin, HDNode *node, InputScriptType script_type, const uint8_t *message, size_t message_len, uint8_t *signature);

int cryptoMessageVerify(const CoinInfo *coin, const uint8_t *message, size_t message_len, const char *address, const uint8_t *signature);

/* ECIES disabled
int cryptoMessageEncrypt(curve_point *pubkey, const uint8_t *msg, size_t msg_size, bool display_only, uint8_t *nonce, size_t *nonce_len, uint8_t *payload, size_t *payload_len, uint8_t *hmac, size_t *hmac_len, const uint8_t *privkey, const uint8_t *address_raw);

int cryptoMessageDecrypt(curve_point *nonce, uint8_t *payload, size_t payload_len, const uint8_t *hmac, size_t hmac_len, const uint8_t *privkey, uint8_t *msg, size_t *msg_len, bool *display_only, bool *signing, uint8_t *address_raw);
*/

uint8_t *cryptoHDNodePathToPubkey(const HDNodePathType *hdnodepath);

int cryptoMultisigPubkeyIndex(const MultisigRedeemScriptType *multisig, const uint8_t *pubkey);

int cryptoMultisigFingerprint(const MultisigRedeemScriptType *multisig, uint8_t *hash);

int cryptoIdentityFingerprint(const IdentityType *identity, uint8_t *hash);

#endif
