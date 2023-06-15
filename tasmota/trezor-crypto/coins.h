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

#ifndef __COINS_H__
#define __COINS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "bip32.h"
#include "coins_count.h"
#include "hasher.h"

typedef struct _CoinInfo {
	const char *coin_name;
	const char *coin_shortcut;
	uint64_t maxfee_kb;
	const char *signed_message_header;
	bool has_address_type;
	bool has_address_type_p2sh;
	bool has_segwit;
	bool has_forkid;
	bool force_bip143;
	// address types > 0xFF represent a two-byte prefix in big-endian order
	uint32_t address_type;
	uint32_t address_type_p2sh;
	uint32_t xpub_magic;
	uint32_t xprv_magic;
	uint32_t forkid;
	const char *bech32_prefix;
	uint32_t coin_type;
	const char *curve_name;
	const curve_info *curve;
} CoinInfo;

extern const CoinInfo coins[COINS_COUNT];

const CoinInfo *coinByName(const char *name);
const CoinInfo *coinByAddressType(uint32_t address_type);
const CoinInfo *coinByCoinType(uint32_t coin_type);
bool coinExtractAddressType(const CoinInfo *coin, const char *addr, uint32_t *address_type);
bool coinExtractAddressTypeRaw(const CoinInfo *coin, const uint8_t *addr_raw, uint32_t *address_type);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
