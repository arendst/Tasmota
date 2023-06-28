/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 * Copyright (c) 2016 Alex Beregszaszi
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

#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "options.h"

// bignum256 are 256 bits stored as 8*30 bit + 1*16 bit
// val[0] are lowest 30 bits, val[8] highest 16 bits
typedef struct {
	uint32_t val[9];
} bignum256;

// read 4 big endian bytes into uint32
uint32_t read_be(const uint8_t *data);

// write 4 big endian bytes
void write_be(uint8_t *data, uint32_t x);

// read 4 little endian bytes into uint32
uint32_t read_le(const uint8_t *data);

// write 4 little endian bytes
void write_le(uint8_t *data, uint32_t x);

void bn_read_be(const uint8_t *in_number, bignum256 *out_number);

void bn_write_be(const bignum256 *in_number, uint8_t *out_number);

void bn_read_le(const uint8_t *in_number, bignum256 *out_number);

void bn_write_le(const bignum256 *in_number, uint8_t *out_number);

void bn_read_uint32(uint32_t in_number, bignum256 *out_number);

void bn_read_uint64(uint64_t in_number, bignum256 *out_number);

static inline uint32_t bn_write_uint32(const bignum256 *in_number)
{
	return in_number->val[0] | (in_number->val[1] << 30);
}

static inline uint64_t bn_write_uint64(const bignum256 *in_number)
{
	uint64_t tmp;
	tmp = in_number->val[2];
	tmp <<= 30;
	tmp |= in_number->val[1];
	tmp <<= 30;
	tmp |= in_number->val[0];
	return tmp;
}

// copies number a to b
static inline void bn_copy(const bignum256 *a, bignum256 *b) {
	*b = *a;
}

int bn_bitcount(const bignum256 *a);

unsigned int bn_digitcount(const bignum256 *a);

void bn_zero(bignum256 *a);

int bn_is_zero(const bignum256 *a);

void bn_one(bignum256 *a);

static inline int bn_is_even(const bignum256 *a) {
	return (a->val[0] & 1) == 0;
}

static inline int bn_is_odd(const bignum256 *a) {
	return (a->val[0] & 1) == 1;
}

int bn_is_less(const bignum256 *a, const bignum256 *b);

int bn_is_equal(const bignum256 *a, const bignum256 *b);

void bn_cmov(bignum256 *res, int cond, const bignum256 *truecase, const bignum256 *falsecase);

void bn_lshift(bignum256 *a);

void bn_rshift(bignum256 *a);

void bn_setbit(bignum256 *a, uint8_t bit);

void bn_clearbit(bignum256 *a, uint8_t bit);

uint32_t bn_testbit(bignum256 *a, uint8_t bit);

void bn_xor(bignum256 *a, const bignum256 *b, const bignum256 *c);

void bn_mult_half(bignum256 *x, const bignum256 *prime);

void bn_mult_k(bignum256 *x, uint8_t k, const bignum256 *prime);

void bn_mod(bignum256 *x, const bignum256 *prime);

void bn_multiply(const bignum256 *k, bignum256 *x, const bignum256 *prime);

void bn_fast_mod(bignum256 *x, const bignum256 *prime);

void bn_sqrt(bignum256 *x, const bignum256 *prime);

void bn_inverse(bignum256 *x, const bignum256 *prime);

void bn_normalize(bignum256 *a);

void bn_add(bignum256 *a, const bignum256 *b);

void bn_addmod(bignum256 *a, const bignum256 *b, const bignum256 *prime);

void bn_addi(bignum256 *a, uint32_t b);

void bn_subi(bignum256 *a, uint32_t b, const bignum256 *prime);

void bn_subtractmod(const bignum256 *a, const bignum256 *b, bignum256 *res, const bignum256 *prime);

void bn_subtract(const bignum256 *a, const bignum256 *b, bignum256 *res);

void bn_divmod58(bignum256 *a, uint32_t *r);

void bn_divmod1000(bignum256 *a, uint32_t *r);

size_t bn_format(const bignum256 *amnt, const char *prefix, const char *suffix, unsigned int decimals, int exponent, bool trailing, char *out, size_t outlen);

static inline size_t bn_format_uint64(uint64_t amount, const char *prefix, const char *suffix, unsigned int decimals, int exponent, bool trailing, char *out, size_t outlen)
{
	bignum256 amnt;
	bn_read_uint64(amount, &amnt);

	return bn_format(&amnt, prefix, suffix, decimals, exponent, trailing, out, outlen);
}

#if USE_BN_PRINT
void bn_print(const bignum256 *a);
void bn_print_raw(const bignum256 *a);
#endif

#ifdef __cplusplus
} /* end of extern "C" */
#endif
    
#endif
