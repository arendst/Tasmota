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

#include <string.h>
#include "transaction.h"
#include "ecdsa.h"
#include "coins.h"
#include "util.h"
#include "debug.h"
//#include "protect.h"
//#include "layout2.h"
#include "crypto.h"
#include "ripemd160.h"
#include "base58.h"
#include "address.h"
//#include "messages.pb.h"
#include "types.pb.h"
#include "segwit_addr.h"

#define SEGWIT_VERSION_0 0

/* transaction input size (without script): 32 prevhash, 4 idx, 4 sequence */
#define TXSIZE_INPUT 40
/* transaction output size (without script): 8 amount */
#define TXSIZE_OUTPUT 8
/* size of a pubkey */
#define TXSIZE_PUBKEY 33
/* size of a DER signature (3 type bytes, 3 len bytes, 33 R, 32 S, 1 sighash */
#define TXSIZE_SIGNATURE 72
/* size of a multiscript without pubkey (1 M, 1 N, 1 checksig) */
#define TXSIZE_MULTISIGSCRIPT 3
/* size of a p2wpkh script (1 version, 1 push, 20 hash) */
#define TXSIZE_WITNESSPKHASH 22
/* size of a p2wsh script (1 version, 1 push, 32 hash) */
#define TXSIZE_WITNESSSCRIPT 34
/* size of a p2pkh script (dup, hash, push, 20 pubkeyhash, equal, checksig) */
#define TXSIZE_P2PKHASH 25
/* size of a p2sh script (hash, push, 20 scripthash, equal) */
#define TXSIZE_P2SCRIPT 23

static const uint8_t segwit_header[2] = {0,1};

static inline uint32_t op_push_size(uint32_t i) {
	if (i < 0x4C) {
		return 1;
	}
	if (i < 0x100) {
		return 2;
	}
	if (i < 0x10000) {
		return 3;
	}
	return 5;
}

uint32_t op_push(uint32_t i, uint8_t *out) {
	if (i < 0x4C) {
		out[0] = i & 0xFF;
		return 1;
	}
	if (i < 0x100) {
		out[0] = 0x4C;
		out[1] = i & 0xFF;
		return 2;
	}
	if (i < 0x10000) {
		out[0] = 0x4D;
		out[1] = i & 0xFF;
		out[2] = (i >> 8) & 0xFF;
		return 3;
	}
	out[0] = 0x4E;
	out[1] = i & 0xFF;
	out[2] = (i >> 8) & 0xFF;
	out[3] = (i >> 16) & 0xFF;
	out[4] = (i >> 24) & 0xFF;
	return 5;
}

bool compute_address(const CoinInfo *coin,
					 InputScriptType script_type,
					 const HDNode *node,
					 bool has_multisig, const MultisigRedeemScriptType *multisig,
					 char address[MAX_ADDR_SIZE]) {

	uint8_t raw[MAX_ADDR_RAW_SIZE];
	uint8_t digest[32];
	size_t prelen;

	if (has_multisig) {
		if (cryptoMultisigPubkeyIndex(multisig, node->public_key) < 0) {
			return 0;
		}
		if (compile_script_multisig_hash(multisig, coin->curve->hasher_type, digest) == 0) {
			return 0;
		}
		if (script_type == InputScriptType_SPENDWITNESS) {
			// segwit p2wsh:  script hash is single sha256
			if (!coin->has_segwit || !coin->bech32_prefix) {
				return 0;
			}
			if (!segwit_addr_encode(address, coin->bech32_prefix, SEGWIT_VERSION_0, digest, 32)) {
				return 0;
			}
		} else if (script_type == InputScriptType_SPENDP2SHWITNESS) {
			// segwit p2wsh encapsuled in p2sh address
			if (!coin->has_segwit) {
				return 0;
			}
			if (!coin->has_address_type_p2sh) {
				return 0;
			}
			raw[0] = 0; // push version
			raw[1] = 32; // push 32 bytes
			memcpy(raw+2, digest, 32); // push hash
			hasher_Raw(coin->curve->hasher_type, raw, 34, digest);
			prelen = address_prefix_bytes_len(coin->address_type_p2sh);
			address_write_prefix_bytes(coin->address_type_p2sh, raw);
			ripemd160(digest, 32, raw + prelen);
			if (!base58_encode_check(raw, prelen + 20, coin->curve->hasher_type, address, MAX_ADDR_SIZE)) {
				return 0;
			}
		} else {
			// non-segwit p2sh multisig
			prelen = address_prefix_bytes_len(coin->address_type_p2sh);
			address_write_prefix_bytes(coin->address_type_p2sh, raw);
			ripemd160(digest, 32, raw + prelen);
			if (!base58_encode_check(raw, prelen + 20, coin->curve->hasher_type, address, MAX_ADDR_SIZE)) {
				return 0;
			}
		}
	} else if (script_type == InputScriptType_SPENDWITNESS) {
		// segwit p2wpkh:  pubkey hash is ripemd160 of sha256
		if (!coin->has_segwit || !coin->bech32_prefix) {
			return 0;
		}
		ecdsa_get_pubkeyhash(node->public_key, coin->curve->hasher_type, digest);
		if (!segwit_addr_encode(address, coin->bech32_prefix, SEGWIT_VERSION_0, digest, 20)) {
			return 0;
		}
	} else if (script_type == InputScriptType_SPENDP2SHWITNESS) {
		// segwit p2wpkh embedded in p2sh
		if (!coin->has_segwit) {
			return 0;
		}
		if (!coin->has_address_type_p2sh) {
			return 0;
		}
		ecdsa_get_address_segwit_p2sh(node->public_key, coin->address_type_p2sh, coin->curve->hasher_type, address, MAX_ADDR_SIZE);
	} else {
		ecdsa_get_address(node->public_key, coin->address_type, coin->curve->hasher_type, address, MAX_ADDR_SIZE);
	}
	return 1;
}

int compile_output(const CoinInfo *coin, const HDNode *root, TxOutputType *in, TxOutputBinType *out, bool needs_confirm)
{
	memset(out, 0, sizeof(TxOutputBinType));
	out->amount = in->amount;
	uint8_t addr_raw[MAX_ADDR_RAW_SIZE];
	size_t addr_raw_len;

	if (in->script_type == OutputScriptType_PAYTOOPRETURN) {
		// only 0 satoshi allowed for OP_RETURN
		if (in->amount != 0) {
			return 0; // failed to compile output
		}
		if (needs_confirm) {
			//layoutConfirmOpReturn(in->op_return_data.bytes, in->op_return_data.size);
			/*
			if (!protectButton(ButtonRequestType_ButtonRequest_ConfirmOutput, false)) {
				return -1; // user aborted
			}
			*/
		}
		uint32_t r = 0;
		out->script_pubkey.bytes[0] = 0x6A; r++; // OP_RETURN
		r += op_push(in->op_return_data.size, out->script_pubkey.bytes + r);
		memcpy(out->script_pubkey.bytes + r, in->op_return_data.bytes, in->op_return_data.size); r += in->op_return_data.size;
		out->script_pubkey.size = r;
		return r;
	}

	if (in->address_n_count > 0) {
		static CONFIDENTIAL HDNode node;
		InputScriptType input_script_type;

		switch (in->script_type) {
			case OutputScriptType_PAYTOADDRESS:
				input_script_type = InputScriptType_SPENDADDRESS;
				break;
			case OutputScriptType_PAYTOMULTISIG:
				input_script_type = InputScriptType_SPENDMULTISIG;
				break;
			case OutputScriptType_PAYTOWITNESS:
				input_script_type = InputScriptType_SPENDWITNESS;
				break;
			case OutputScriptType_PAYTOP2SHWITNESS:
				input_script_type = InputScriptType_SPENDP2SHWITNESS;
				break;
			default:
				return 0; // failed to compile output
		}
		memcpy(&node, root, sizeof(HDNode));
		if (hdnode_private_ckd_cached(&node, in->address_n, in->address_n_count, NULL) == 0) {
			return 0; // failed to compile output
		}
		hdnode_fill_public_key(&node);
		if (!compute_address(coin, input_script_type, &node,
							 in->has_multisig, &in->multisig,
							 in->address)) {
			return 0; // failed to compile output
		}
	} else if (!in->has_address) {
		return 0; // failed to compile output
	}

	addr_raw_len = base58_decode_check(in->address, coin->curve->hasher_type, addr_raw, MAX_ADDR_RAW_SIZE);
	size_t prefix_len;
	if (coin->has_address_type                                  // p2pkh
		&& addr_raw_len == 20 + (prefix_len = address_prefix_bytes_len(coin->address_type))
		&& address_check_prefix(addr_raw, coin->address_type)) {
		out->script_pubkey.bytes[0] = 0x76; // OP_DUP
		out->script_pubkey.bytes[1] = 0xA9; // OP_HASH_160
		out->script_pubkey.bytes[2] = 0x14; // pushing 20 bytes
		memcpy(out->script_pubkey.bytes + 3, addr_raw + prefix_len, 20);
		out->script_pubkey.bytes[23] = 0x88; // OP_EQUALVERIFY
		out->script_pubkey.bytes[24] = 0xAC; // OP_CHECKSIG
		out->script_pubkey.size = 25;
	} else if (coin->has_address_type_p2sh                      // p2sh
			   && addr_raw_len == 20 + (prefix_len = address_prefix_bytes_len(coin->address_type_p2sh))
			   && address_check_prefix(addr_raw, coin->address_type_p2sh)) {
		out->script_pubkey.bytes[0] = 0xA9; // OP_HASH_160
		out->script_pubkey.bytes[1] = 0x14; // pushing 20 bytes
		memcpy(out->script_pubkey.bytes + 2, addr_raw + prefix_len, 20);
		out->script_pubkey.bytes[22] = 0x87; // OP_EQUAL
		out->script_pubkey.size = 23;
	} else if (coin->bech32_prefix) {
		int witver;
		if (!segwit_addr_decode(&witver, addr_raw, &addr_raw_len, coin->bech32_prefix, in->address)) {
			return 0;
		}
		// segwit:
		// push 1 byte version id (opcode OP_0 = 0, OP_i = 80+i)
		// push addr_raw (segwit_addr_decode makes sure addr_raw_len is at most 40)
		out->script_pubkey.bytes[0] = witver == 0 ? 0 : 80 + witver;
		out->script_pubkey.bytes[1] = addr_raw_len;
		memcpy(out->script_pubkey.bytes + 2, addr_raw, addr_raw_len);
		out->script_pubkey.size = addr_raw_len + 2;
	} else {
		return 0;
	}

	if (needs_confirm) {
		//layoutConfirmOutput(coin, in);
		/*
		if (!protectButton(ButtonRequestType_ButtonRequest_ConfirmOutput, false)) {
			return -1; // user aborted
		}
		*/
	}

	return out->script_pubkey.size;
}

uint32_t compile_script_sig(uint32_t address_type, const uint8_t *pubkeyhash, uint8_t *out)
{
	if (coinByAddressType(address_type)) { // valid coin type
		out[0] = 0x76; // OP_DUP
		out[1] = 0xA9; // OP_HASH_160
		out[2] = 0x14; // pushing 20 bytes
		memcpy(out + 3, pubkeyhash, 20);
		out[23] = 0x88; // OP_EQUALVERIFY
		out[24] = 0xAC; // OP_CHECKSIG
		return 25;
	} else {
		return 0; // unsupported
	}
}

// if out == NULL just compute the length
uint32_t compile_script_multisig(const MultisigRedeemScriptType *multisig, uint8_t *out)
{
	if (!multisig->has_m) return 0;
	const uint32_t m = multisig->m;
	const uint32_t n = multisig->pubkeys_count;
	if (m < 1 || m > 15) return 0;
	if (n < 1 || n > 15) return 0;
	uint32_t r = 0;
	if (out) {
		out[r] = 0x50 + m; r++;
		for (uint32_t i = 0; i < n; i++) {
			out[r] = 33; r++; // OP_PUSH 33
			const uint8_t *pubkey = cryptoHDNodePathToPubkey(&(multisig->pubkeys[i]));
			if (!pubkey) return 0;
			memcpy(out + r, pubkey, 33); r += 33;
		}
		out[r] = 0x50 + n; r++;
		out[r] = 0xAE; r++; // OP_CHECKMULTISIG
	} else {
		r = 1 + 34 * n + 2;
	}
	return r;
}

uint32_t compile_script_multisig_hash(const MultisigRedeemScriptType *multisig, HasherType hasher_type, uint8_t *hash)
{
	if (!multisig->has_m) return 0;
	const uint32_t m = multisig->m;
	const uint32_t n = multisig->pubkeys_count;
	if (m < 1 || m > 15) return 0;
	if (n < 1 || n > 15) return 0;

	Hasher hasher;
	hasher_Init(&hasher, hasher_type);

	uint8_t d[2];
	d[0] = 0x50 + m; hasher_Update(&hasher, d, 1);
	for (uint32_t i = 0; i < n; i++) {
		d[0] = 33; hasher_Update(&hasher, d, 1); // OP_PUSH 33
		const uint8_t *pubkey = cryptoHDNodePathToPubkey(&(multisig->pubkeys[i]));
		if (!pubkey) return 0;
		hasher_Update(&hasher, pubkey, 33);
	}
	d[0] = 0x50 + n;
	d[1] = 0xAE;
	hasher_Update(&hasher, d, 2);

	hasher_Final(&hasher, hash);

	return 1;
}

uint32_t serialize_script_sig(const uint8_t *signature, uint32_t signature_len, const uint8_t *pubkey, uint32_t pubkey_len, uint8_t sighash, uint8_t *out)
{
	uint32_t r = 0;
	r += op_push(signature_len + 1, out + r);
	memcpy(out + r, signature, signature_len); r += signature_len;
	out[r] = sighash; r++;
	r += op_push(pubkey_len, out + r);
	memcpy(out + r, pubkey, pubkey_len); r += pubkey_len;
	return r;
}

uint32_t serialize_script_multisig(const MultisigRedeemScriptType *multisig, uint8_t sighash, uint8_t *out)
{
	uint32_t r = 0;
	out[r] = 0x00; r++;
	for (uint32_t i = 0; i < multisig->signatures_count; i++) {
		if (multisig->signatures[i].size == 0) {
			continue;
		}
		r += op_push(multisig->signatures[i].size + 1, out + r);
		memcpy(out + r, multisig->signatures[i].bytes, multisig->signatures[i].size); r += multisig->signatures[i].size;
		out[r] = sighash; r++;
	}
	uint32_t script_len = compile_script_multisig(multisig, 0);
	if (script_len == 0) {
		return 0;
	}
	r += op_push(script_len, out + r);
	r += compile_script_multisig(multisig, out + r);
	return r;
}

// tx methods

uint32_t tx_prevout_hash(Hasher *hasher, const TxInputType *input)
{
	for (int i = 0; i < 32; i++) {
		hasher_Update(hasher, &(input->prev_hash.bytes[31 - i]), 1);
	}
	hasher_Update(hasher, (const uint8_t *)&input->prev_index, 4);
	return 36;
}

uint32_t tx_script_hash(Hasher *hasher, uint32_t size, const uint8_t *data)
{
	int r = ser_length_hash(hasher, size);
	hasher_Update(hasher, data, size);
	return r + size;
}

uint32_t tx_sequence_hash(Hasher *hasher, const TxInputType *input)
{
	hasher_Update(hasher, (const uint8_t *)&input->sequence, 4);
	return 4;
}

uint32_t tx_output_hash(Hasher *hasher, const TxOutputBinType *output)
{
	uint32_t r = 0;
	hasher_Update(hasher, (const uint8_t *)&output->amount, 8); r += 8;
	r += tx_script_hash(hasher, output->script_pubkey.size, output->script_pubkey.bytes);
	return r;
}

uint32_t tx_serialize_script(uint32_t size, const uint8_t *data, uint8_t *out)
{
	int r = ser_length(size, out);
	memcpy(out + r, data, size);
	return r + size;
}

uint32_t tx_serialize_header(TxStruct *tx, uint8_t *out)
{
	int r = 4;
	memcpy(out, &(tx->version), 4);
	if (tx->is_segwit) {
		memcpy(out + r, segwit_header, 2);
		r += 2;
	}
	return r + ser_length(tx->inputs_len, out + r);
}

uint32_t tx_serialize_header_hash(TxStruct *tx)
{
	int r = 4;
	hasher_Update(&(tx->hasher), (const uint8_t *)&(tx->version), 4);
	if (tx->is_segwit) {
		hasher_Update(&(tx->hasher), segwit_header, 2);
		r += 2;
	}
	return r + ser_length_hash(&(tx->hasher), tx->inputs_len);
}

uint32_t tx_serialize_input(TxStruct *tx, const TxInputType *input, uint8_t *out)
{
	if (tx->have_inputs >= tx->inputs_len) {
		// already got all inputs
		return 0;
	}
	uint32_t r = 0;
	if (tx->have_inputs == 0) {
		r += tx_serialize_header(tx, out + r);
	}
	for (int i = 0; i < 32; i++) {
		*(out + r + i) = input->prev_hash.bytes[31 - i];
	}
	r += 32;
	memcpy(out + r, &input->prev_index, 4); r += 4;
	r += tx_serialize_script(input->script_sig.size, input->script_sig.bytes, out + r);
	memcpy(out + r, &input->sequence, 4); r += 4;

	tx->have_inputs++;
	tx->size += r;

	return r;
}

uint32_t tx_serialize_input_hash(TxStruct *tx, const TxInputType *input)
{
	if (tx->have_inputs >= tx->inputs_len) {
		// already got all inputs
		return 0;
	}
	uint32_t r = 0;
	if (tx->have_inputs == 0) {
		r += tx_serialize_header_hash(tx);
	}
	r += tx_prevout_hash(&(tx->hasher), input);
	r += tx_script_hash(&(tx->hasher), input->script_sig.size, input->script_sig.bytes);
	r += tx_sequence_hash(&(tx->hasher), input);

	tx->have_inputs++;
	tx->size += r;

	return r;
}

uint32_t tx_serialize_middle(TxStruct *tx, uint8_t *out)
{
	return ser_length(tx->outputs_len, out);
}

uint32_t tx_serialize_middle_hash(TxStruct *tx)
{
	return ser_length_hash(&(tx->hasher), tx->outputs_len);
}

uint32_t tx_serialize_footer(TxStruct *tx, uint8_t *out)
{
	memcpy(out, &(tx->lock_time), 4);
	return 4;
}

uint32_t tx_serialize_footer_hash(TxStruct *tx)
{
	hasher_Update(&(tx->hasher), (const uint8_t *)&(tx->lock_time), 4);
	return 4;
}

uint32_t tx_serialize_output(TxStruct *tx, const TxOutputBinType *output, uint8_t *out)
{
	if (tx->have_inputs < tx->inputs_len) {
		// not all inputs provided
		return 0;
	}
	if (tx->have_outputs >= tx->outputs_len) {
		// already got all outputs
		return 0;
	}
	uint32_t r = 0;
	if (tx->have_outputs == 0) {
		r += tx_serialize_middle(tx, out + r);
	}
	memcpy(out + r, &output->amount, 8); r += 8;
	r += tx_serialize_script(output->script_pubkey.size, output->script_pubkey.bytes, out + r);
	tx->have_outputs++;
	if (tx->have_outputs == tx->outputs_len
		&& !tx->is_segwit) {
		r += tx_serialize_footer(tx, out + r);
	}
	tx->size += r;
	return r;
}

uint32_t tx_serialize_output_hash(TxStruct *tx, const TxOutputBinType *output)
{
	if (tx->have_inputs < tx->inputs_len) {
		// not all inputs provided
		return 0;
	}
	if (tx->have_outputs >= tx->outputs_len) {
		// already got all outputs
		return 0;
	}
	uint32_t r = 0;
	if (tx->have_outputs == 0) {
		r += tx_serialize_middle_hash(tx);
	}
	r += tx_output_hash(&(tx->hasher), output);
	tx->have_outputs++;
	if (tx->have_outputs == tx->outputs_len
		&& !tx->is_segwit) {
		r += tx_serialize_footer_hash(tx);
	}
	tx->size += r;
	return r;
}

uint32_t tx_serialize_extra_data_hash(TxStruct *tx, const uint8_t *data, uint32_t datalen)
{
	if (tx->have_inputs < tx->inputs_len) {
		// not all inputs provided
		return 0;
	}
	if (tx->have_outputs < tx->outputs_len) {
		// not all inputs provided
		return 0;
	}
	if (tx->extra_data_received + datalen > tx->extra_data_len) {
		// we are receiving too much data
		return 0;
	}
	hasher_Update(&(tx->hasher), data, datalen);
	tx->extra_data_received += datalen;
	tx->size += datalen;
	return datalen;
}

void tx_init(TxStruct *tx, uint32_t inputs_len, uint32_t outputs_len, uint32_t version, uint32_t lock_time, uint32_t extra_data_len, HasherType hasher_type)
{
	tx->inputs_len = inputs_len;
	tx->outputs_len = outputs_len;
	tx->version = version;
	tx->lock_time = lock_time;
	tx->have_inputs = 0;
	tx->have_outputs = 0;
	tx->extra_data_len = extra_data_len;
	tx->extra_data_received = 0;
	tx->size = 0;
	tx->is_segwit = false;
	hasher_Init(&(tx->hasher), hasher_type);
}

void tx_hash_final(TxStruct *t, uint8_t *hash, bool reverse)
{
	hasher_Double(&(t->hasher), hash);
	if (!reverse) return;
	for (uint8_t i = 0; i < 16; i++) {
		uint8_t k = hash[31 - i];
		hash[31 - i] = hash[i];
		hash[i] = k;
	}
}

uint32_t tx_input_weight(const TxInputType *txinput) {
	uint32_t input_script_size;
	if (txinput->has_multisig) {
		uint32_t multisig_script_size = TXSIZE_MULTISIGSCRIPT
			+ txinput->multisig.pubkeys_count * (1 + TXSIZE_PUBKEY);
		input_script_size = 1 // the OP_FALSE bug in multisig
			+ txinput->multisig.m * (1 + TXSIZE_SIGNATURE)
			+ op_push_size(multisig_script_size) + multisig_script_size;
	} else {
		input_script_size = (1 + TXSIZE_SIGNATURE + 1 + TXSIZE_PUBKEY);
	}
	uint32_t weight = 4 * TXSIZE_INPUT;
	if (txinput->script_type == InputScriptType_SPENDADDRESS
		|| txinput->script_type == InputScriptType_SPENDMULTISIG) {
		input_script_size += ser_length_size(input_script_size);
		weight += 4 * input_script_size;
	} else if (txinput->script_type == InputScriptType_SPENDWITNESS
			   || txinput->script_type == InputScriptType_SPENDP2SHWITNESS) {
		if (txinput->script_type == InputScriptType_SPENDP2SHWITNESS) {
			weight += 4 * (2 + (txinput->has_multisig
								? TXSIZE_WITNESSSCRIPT : TXSIZE_WITNESSPKHASH));
		} else {
			weight += 4; // empty input script
		}
		weight += input_script_size; // discounted witness
	}
	return weight;
}

uint32_t tx_output_weight(const CoinInfo *coin, const TxOutputType *txoutput) {
	uint32_t output_script_size = 0;
	if (txoutput->script_type == OutputScriptType_PAYTOOPRETURN) {
		output_script_size = 1 + op_push_size(txoutput->op_return_data.size)
			+ txoutput->op_return_data.size;
	} else if (txoutput->address_n_count > 0) {
		if (txoutput->script_type == OutputScriptType_PAYTOWITNESS) {
			output_script_size = txoutput->has_multisig
				? TXSIZE_WITNESSSCRIPT : TXSIZE_WITNESSPKHASH;
		} else if (txoutput->script_type == OutputScriptType_PAYTOP2SHWITNESS) {
			output_script_size = TXSIZE_P2SCRIPT;
		} else {
			output_script_size = txoutput->has_multisig
				? TXSIZE_P2SCRIPT : TXSIZE_P2PKHASH;
		}
	} else {
		uint8_t addr_raw[MAX_ADDR_RAW_SIZE];
		int witver;
		size_t addr_raw_len;
		if (coin->bech32_prefix
			&& segwit_addr_decode(&witver, addr_raw, &addr_raw_len, coin->bech32_prefix, txoutput->address)) {
			output_script_size = 2 + addr_raw_len;
		} else {
			addr_raw_len = base58_decode_check(txoutput->address, coin->curve->hasher_type, addr_raw, MAX_ADDR_RAW_SIZE);
			if (coin->has_address_type
				&& address_check_prefix(addr_raw, coin->address_type)) {
				output_script_size = TXSIZE_P2PKHASH;
			} else if (coin->has_address_type_p2sh
					   && address_check_prefix(addr_raw, coin->address_type_p2sh)) {
				output_script_size = TXSIZE_P2SCRIPT;
			}
		}
	}
	output_script_size += ser_length_size(output_script_size);
	return 4 * (TXSIZE_OUTPUT + output_script_size);
}
