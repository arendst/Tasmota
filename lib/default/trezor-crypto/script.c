/**
 * Copyright (c) 2016 Pavol Rusnak
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
#include "base58.h"
#include "script.h"

int script_output_to_address(const uint8_t *script, int scriptlen, char *addr, int addrsize)
{
	uint8_t raw[35];

	// P2PKH
	if (scriptlen == 25 && script[0] == 0x76 && script[1] == 0xA9 && script[2] == 0x14 && script[23] == 0x88 && script[24] == 0xAC) {
		raw[0] = 0x00;
		memcpy(raw + 1, script + 3, 20);
		return base58_encode_check(raw, 1 + 20, HASHER_SHA2, addr, addrsize);
	}

	// P2SH
	if (scriptlen == 23 && script[0] == 0xA9 && script[1] == 0x14 && script[22] == 0x87) {
		raw[0] = 0x05;
		memcpy(raw + 1, script + 2, 20);
		return base58_encode_check(raw, 1 + 20, HASHER_SHA2, addr, addrsize);
	}

	// P2WPKH
	if (scriptlen == 22 && script[0] == 0x00 && script[1] == 0x14) {
		raw[0] = 0x06;
		raw[1] = 0x00;
		raw[2] = 0x00;
		memcpy(raw + 3, script + 2, 20);
		return base58_encode_check(raw, 3 + 20, HASHER_SHA2, addr, addrsize);
	}

	// P2WSH
	if (scriptlen == 34 && script[0] == 0x00 && script[1] == 0x20) {
		raw[0] = 0x0A;
		raw[1] = 0x00;
		raw[2] = 0x00;
		memcpy(raw + 3, script + 2, 32);
		return base58_encode_check(raw, 3 + 32, HASHER_SHA2, addr, addrsize);
	}

	return 0;
}
