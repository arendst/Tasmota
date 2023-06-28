/**
 * Copyright (c) 2012-2014 Luke Dashjr
 * Copyright (c) 2013-2014 Pavol Rusnak
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
#include <sys/types.h>
#include "base58.h"
#include "sha2.h"
#include "macros.h"
#include "ripemd160.h"

static const int8_t b58digits_map[] = {
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,-1,-1,-1,-1,
	-1, 9,10,11,12,13,14,15,16,-1,17,18,19,20,21,-1,
	22,23,24,25,26,27,28,29,30,31,32,-1,-1,-1,-1,-1,
	-1,33,34,35,36,37,38,39,40,41,42,43,-1,44,45,46,
	47,48,49,50,51,52,53,54,55,56,57,-1,-1,-1,-1,-1,
};

bool b58tobin(void *bin, size_t *binszp, const char *b58)
{
	size_t binsz = *binszp;
	const unsigned char *b58u = (const unsigned char*)b58;
	unsigned char *binu = bin;
	size_t outisz = (binsz + 3) / 4;
	uint32_t outi[outisz];
	uint64_t t;
	uint32_t c;
	size_t i, j;
	uint8_t bytesleft = binsz % 4;
	uint32_t zeromask = bytesleft ? (0xffffffff << (bytesleft * 8)) : 0;
	unsigned zerocount = 0;
	size_t b58sz;

	b58sz = strlen(b58);

	memset(outi, 0, outisz * sizeof(*outi));

	// Leading zeros, just count
	for (i = 0; i < b58sz && b58u[i] == '1'; ++i)
		++zerocount;

	for ( ; i < b58sz; ++i)
	{
		if (b58u[i] & 0x80)
			// High-bit set on invalid digit
			return false;
		if (b58digits_map[b58u[i]] == -1)
			// Invalid base58 digit
			return false;
		c = (unsigned)b58digits_map[b58u[i]];
		for (j = outisz; j--; )
		{
			t = ((uint64_t)outi[j]) * 58 + c;
			c = (t & 0x3f00000000) >> 32;
			outi[j] = t & 0xffffffff;
		}
		if (c)
			// Output number too big (carry to the next int32)
			return false;
		if (outi[0] & zeromask)
			// Output number too big (last int32 filled too far)
			return false;
	}

	j = 0;
	switch (bytesleft) {
		case 3:
			*(binu++) = (outi[0] &   0xff0000) >> 16;
			//-fallthrough
		case 2:
			*(binu++) = (outi[0] &     0xff00) >>  8;
			//-fallthrough
		case 1:
			*(binu++) = (outi[0] &       0xff);
			++j;
			//-fallthrough
		default:
			break;
	}

	for (; j < outisz; ++j)
	{
		*(binu++) = (outi[j] >> 0x18) & 0xff;
		*(binu++) = (outi[j] >> 0x10) & 0xff;
		*(binu++) = (outi[j] >>    8) & 0xff;
		*(binu++) = (outi[j] >>    0) & 0xff;
	}

	// Count canonical base58 byte count
	binu = bin;
	for (i = 0; i < binsz; ++i)
	{
		if (binu[i])
			break;
		--*binszp;
	}
	*binszp += zerocount;

	return true;
}

int b58check(const void *bin, size_t binsz, HasherType hasher_type, const char *base58str)
{
	unsigned char buf[32];
	const uint8_t *binc = bin;
	unsigned i;
	if (binsz < 4)
		return -4;
	hasher_Raw(hasher_type, bin, binsz - 4, buf);
	hasher_Raw(hasher_type, buf, 32, buf);
	if (memcmp(&binc[binsz - 4], buf, 4))
		return -1;

	// Check number of zeros is correct AFTER verifying checksum (to avoid possibility of accessing base58str beyond the end)
	for (i = 0; binc[i] == '\0' && base58str[i] == '1'; ++i)
	{}  // Just finding the end of zeros, nothing to do in loop
	if (binc[i] == '\0' || base58str[i] == '1')
		return -3;

	return binc[0];
}

static const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz)
{
	const uint8_t *bin = data;
	int carry;
	ssize_t i, j, high, zcount = 0;
	size_t size;

	while (zcount < (ssize_t)binsz && !bin[zcount])
		++zcount;

	size = (binsz - zcount) * 138 / 100 + 1;
	uint8_t buf[size];
	memset(buf, 0, size);

	for (i = zcount, high = size - 1; i < (ssize_t)binsz; ++i, high = j)
	{
		for (carry = bin[i], j = size - 1; (j > high) || carry; --j)
		{
			carry += 256 * buf[j];
			buf[j] = carry % 58;
			carry /= 58;
		}
	}

	for (j = 0; j < (ssize_t)size && !buf[j]; ++j);

	if (*b58sz <= zcount + size - j)
	{
		*b58sz = zcount + size - j + 1;
		return false;
	}

	if (zcount)
		memset(b58, '1', zcount);
	for (i = zcount; j < (ssize_t)size; ++i, ++j)
		b58[i] = b58digits_ordered[buf[j]];
	b58[i] = '\0';
	*b58sz = i + 1;

	return true;
}

int base58_encode_check(const uint8_t *data, int datalen, HasherType hasher_type, char *str, int strsize)
{
	if (datalen > 128) {
		return 0;
	}
	uint8_t buf[datalen + 32];
	uint8_t *hash = buf + datalen;
	memcpy(buf, data, datalen);
	hasher_Raw(hasher_type, data, datalen, hash);
	hasher_Raw(hasher_type, hash, 32, hash);
	size_t res = strsize;
	bool success = b58enc(str, &res, buf, datalen + 4);
	MEMSET_BZERO(buf, sizeof(buf));
	return success ? res : 0;
}

int base58_decode_check(const char *str, HasherType hasher_type, uint8_t *data, int datalen)
{
	if (datalen > 128) {
		return 0;
	}
	uint8_t d[datalen + 4];
	size_t res = datalen + 4;
	if (b58tobin(d, &res, str) != true) {
		return 0;
	}
	uint8_t *nd = d + datalen + 4 - res;
	if (b58check(nd, res, hasher_type, str) < 0) {
		return 0;
	}
	memcpy(data, nd, res - 4);
	return res - 4;
}

#if USE_GRAPHENE
int b58gphcheck(const void *bin, size_t binsz, const char *base58str)
{
	unsigned char buf[32];
	const uint8_t *binc = bin;
	unsigned i;
	if (binsz < 4)
		return -4;
	ripemd160(bin, binsz - 4, buf);  // No double SHA256, but a single RIPEMD160
	if (memcmp(&binc[binsz - 4], buf, 4))
		return -1;

	// Check number of zeros is correct AFTER verifying checksum (to avoid possibility of accessing base58str beyond the end)
	for (i = 0; binc[i] == '\0' && base58str[i] == '1'; ++i)
	{}  // Just finding the end of zeros, nothing to do in loop
	if (binc[i] == '\0' || base58str[i] == '1')
		return -3;

	return binc[0];
}

int base58gph_encode_check(const uint8_t *data, int datalen, char *str, int strsize)
{
	if (datalen > 128) {
		return 0;
	}
	uint8_t buf[datalen + 32];
	uint8_t *hash = buf + datalen;
	memcpy(buf, data, datalen);
	ripemd160(data, datalen, hash);  // No double SHA256, but a single RIPEMD160
	size_t res = strsize;
	bool success = b58enc(str, &res, buf, datalen + 4);
	MEMSET_BZERO(buf, sizeof(buf));
	return success ? res : 0;
}

int base58gph_decode_check(const char *str, uint8_t *data, int datalen)
{
	if (datalen > 128) {
		return 0;
	}
	uint8_t d[datalen + 4];
	size_t res = datalen + 4;
	if (b58tobin(d, &res, str) != true) {
		return 0;
	}
	uint8_t *nd = d + datalen + 4 - res;
	if (b58gphcheck(nd, res, str) < 0) {
		return 0;
	}
	memcpy(data, nd, res - 4);
	return res - 4;
}
#endif
