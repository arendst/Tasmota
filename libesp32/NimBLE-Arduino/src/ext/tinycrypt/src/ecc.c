/* ecc.c - TinyCrypt implementation of common ECC functions */

/*
 * Copyright (c) 2014, Kenneth MacKay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright (C) 2017 by Intel Corporation, All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <tinycrypt/ecc.h>
#include <tinycrypt/ecc_platform_specific.h>
#include <string.h>

/* IMPORTANT: Make sure a cryptographically-secure PRNG is set and the platform
 * has access to enough entropy in order to feed the PRNG regularly. */
#if default_RNG_defined
static uECC_RNG_Function g_rng_function = &default_CSPRNG;
#else
static uECC_RNG_Function g_rng_function = 0;
#endif

void uECC_set_rng(uECC_RNG_Function rng_function)
{
	g_rng_function = rng_function;
}

uECC_RNG_Function uECC_get_rng(void)
{
	return g_rng_function;
}

int uECC_curve_private_key_size(uECC_Curve curve)
{
	return BITS_TO_BYTES(curve->num_n_bits);
}

int uECC_curve_public_key_size(uECC_Curve curve)
{
	return 2 * curve->num_bytes;
}

void uECC_vli_clear(uECC_word_t *vli, wordcount_t num_words)
{
	wordcount_t i;
	for (i = 0; i < num_words; ++i) {
		 vli[i] = 0;
	}
}

uECC_word_t uECC_vli_isZero(const uECC_word_t *vli, wordcount_t num_words)
{
	uECC_word_t bits = 0;
	wordcount_t i;
	for (i = 0; i < num_words; ++i) {
		bits |= vli[i];
	}
	return (bits == 0);
}

uECC_word_t uECC_vli_testBit(const uECC_word_t *vli, bitcount_t bit)
{
	return (vli[bit >> uECC_WORD_BITS_SHIFT] &
		((uECC_word_t)1 << (bit & uECC_WORD_BITS_MASK)));
}

/* Counts the number of words in vli. */
static wordcount_t vli_numDigits(const uECC_word_t *vli,
				 const wordcount_t max_words)
{

	wordcount_t i;
	/* Search from the end until we find a non-zero digit. We do it in reverse
	 * because we expect that most digits will be nonzero. */
	for (i = max_words - 1; i >= 0 && vli[i] == 0; --i) {
	}

	return (i + 1);
}

bitcount_t uECC_vli_numBits(const uECC_word_t *vli,
			    const wordcount_t max_words)
{

	uECC_word_t i;
	uECC_word_t digit;

	wordcount_t num_digits = vli_numDigits(vli, max_words);
	if (num_digits == 0) {
		return 0;
	}

	digit = vli[num_digits - 1];
	for (i = 0; digit; ++i) {
		digit >>= 1;
	}

	return (((bitcount_t)(num_digits - 1) << uECC_WORD_BITS_SHIFT) + i);
}

void uECC_vli_set(uECC_word_t *dest, const uECC_word_t *src,
		  wordcount_t num_words)
{
	wordcount_t i;

	for (i = 0; i < num_words; ++i) {
		dest[i] = src[i];
  	}
}

cmpresult_t uECC_vli_cmp_unsafe(const uECC_word_t *left,
				const uECC_word_t *right,
				wordcount_t num_words)
{
	wordcount_t i;

	for (i = num_words - 1; i >= 0; --i) {
		if (left[i] > right[i]) {
			return 1;
		} else if (left[i] < right[i]) {
			return -1;
		}
	}
	return 0;
}

uECC_word_t uECC_vli_equal(const uECC_word_t *left, const uECC_word_t *right,
			   wordcount_t num_words)
{

	uECC_word_t diff = 0;
	wordcount_t i;

	for (i = num_words - 1; i >= 0; --i) {
		diff |= (left[i] ^ right[i]);
	}
	return !(diff == 0);
}

uECC_word_t cond_set(uECC_word_t p_true, uECC_word_t p_false, unsigned int cond)
{
	return (p_true*(cond)) | (p_false*(!cond));
}

/* Computes result = left - right, returning borrow, in constant time.
 * Can modify in place. */
uECC_word_t uECC_vli_sub(uECC_word_t *result, const uECC_word_t *left,
			 const uECC_word_t *right, wordcount_t num_words)
{
	uECC_word_t borrow = 0;
	wordcount_t i;
	for (i = 0; i < num_words; ++i) {
		uECC_word_t diff = left[i] - right[i] - borrow;
		uECC_word_t val = (diff > left[i]);
		borrow = cond_set(val, borrow, (diff != left[i]));

		result[i] = diff;
	}
	return borrow;
}

/* Computes result = left + right, returning carry, in constant time.
 * Can modify in place. */
static uECC_word_t uECC_vli_add(uECC_word_t *result, const uECC_word_t *left,
				const uECC_word_t *right, wordcount_t num_words)
{
	uECC_word_t carry = 0;
	wordcount_t i;
	for (i = 0; i < num_words; ++i) {
		uECC_word_t sum = left[i] + right[i] + carry;
		uECC_word_t val = (sum < left[i]);
		carry = cond_set(val, carry, (sum != left[i]));
		result[i] = sum;
	}
	return carry;
}

cmpresult_t uECC_vli_cmp(const uECC_word_t *left, const uECC_word_t *right,
			 wordcount_t num_words)
{
	uECC_word_t tmp[NUM_ECC_WORDS];
	uECC_word_t neg = !!uECC_vli_sub(tmp, left, right, num_words);
	uECC_word_t equal = uECC_vli_isZero(tmp, num_words);
	return (!equal - 2 * neg);
}

/* Computes vli = vli >> 1. */
static void uECC_vli_rshift1(uECC_word_t *vli, wordcount_t num_words)
{
	uECC_word_t *end = vli;
	uECC_word_t carry = 0;

	vli += num_words;
	while (vli-- > end) {
		uECC_word_t temp = *vli;
		*vli = (temp >> 1) | carry;
		carry = temp << (uECC_WORD_BITS - 1);
	}
}

static void muladd(uECC_word_t a, uECC_word_t b, uECC_word_t *r0,
		   uECC_word_t *r1, uECC_word_t *r2)
{

	uECC_dword_t p = (uECC_dword_t)a * b;
	uECC_dword_t r01 = ((uECC_dword_t)(*r1) << uECC_WORD_BITS) | *r0;
	r01 += p;
	*r2 += (r01 < p);
	*r1 = r01 >> uECC_WORD_BITS;
	*r0 = (uECC_word_t)r01;

}

/* Computes result = left * right. Result must be 2 * num_words long. */
static void uECC_vli_mult(uECC_word_t *result, const uECC_word_t *left,
			  const uECC_word_t *right, wordcount_t num_words)
{

	uECC_word_t r0 = 0;
	uECC_word_t r1 = 0;
	uECC_word_t r2 = 0;
	wordcount_t i, k;

	/* Compute each digit of result in sequence, maintaining the carries. */
	for (k = 0; k < num_words; ++k) {

		for (i = 0; i <= k; ++i) {
			muladd(left[i], right[k - i], &r0, &r1, &r2);
		}

		result[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}

	for (k = num_words; k < num_words * 2 - 1; ++k) {

		for (i = (k + 1) - num_words; i < num_words; ++i) {
			muladd(left[i], right[k - i], &r0, &r1, &r2);
		}
		result[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}
	result[num_words * 2 - 1] = r0;
}

void uECC_vli_modAdd(uECC_word_t *result, const uECC_word_t *left,
		     const uECC_word_t *right, const uECC_word_t *mod,
		     wordcount_t num_words)
{
	uECC_word_t carry = uECC_vli_add(result, left, right, num_words);
	if (carry || uECC_vli_cmp_unsafe(mod, result, num_words) != 1) {
	/* result > mod (result = mod + remainder), so subtract mod to get
	 * remainder. */
		uECC_vli_sub(result, result, mod, num_words);
	}
}

void uECC_vli_modSub(uECC_word_t *result, const uECC_word_t *left,
		     const uECC_word_t *right, const uECC_word_t *mod,
		     wordcount_t num_words)
{
	uECC_word_t l_borrow = uECC_vli_sub(result, left, right, num_words);
	if (l_borrow) {
		/* In this case, result == -diff == (max int) - diff. Since -x % d == d - x,
		 * we can get the correct result from result + mod (with overflow). */
		uECC_vli_add(result, result, mod, num_words);
	}
}

/* Computes result = product % mod, where product is 2N words long. */
/* Currently only designed to work for curve_p or curve_n. */
void uECC_vli_mmod(uECC_word_t *result, uECC_word_t *product,
    		   const uECC_word_t *mod, wordcount_t num_words)
{
	uECC_word_t mod_multiple[2 * NUM_ECC_WORDS];
	uECC_word_t tmp[2 * NUM_ECC_WORDS];
	uECC_word_t *v[2] = {tmp, product};
	uECC_word_t index;

	/* Shift mod so its highest set bit is at the maximum position. */
	bitcount_t shift = (num_words * 2 * uECC_WORD_BITS) -
			   uECC_vli_numBits(mod, num_words);
	wordcount_t word_shift = shift / uECC_WORD_BITS;
	wordcount_t bit_shift = shift % uECC_WORD_BITS;
	uECC_word_t carry = 0;
	uECC_vli_clear(mod_multiple, word_shift);
	if (bit_shift > 0) {
		for(index = 0; index < (uECC_word_t)num_words; ++index) {
			mod_multiple[word_shift + index] = (mod[index] << bit_shift) | carry;
			carry = mod[index] >> (uECC_WORD_BITS - bit_shift);
		}
	} else {
		uECC_vli_set(mod_multiple + word_shift, mod, num_words);
	}

	for (index = 1; shift >= 0; --shift) {
		uECC_word_t borrow = 0;
		wordcount_t i;
		for (i = 0; i < num_words * 2; ++i) {
			uECC_word_t diff = v[index][i] - mod_multiple[i] - borrow;
			if (diff != v[index][i]) {
				borrow = (diff > v[index][i]);
			}
			v[1 - index][i] = diff;
		}
		/* Swap the index if there was no borrow */
		index = !(index ^ borrow);
		uECC_vli_rshift1(mod_multiple, num_words);
		mod_multiple[num_words - 1] |= mod_multiple[num_words] <<
					       (uECC_WORD_BITS - 1);
		uECC_vli_rshift1(mod_multiple + num_words, num_words);
	}
	uECC_vli_set(result, v[index], num_words);
}

void uECC_vli_modMult(uECC_word_t *result, const uECC_word_t *left,
		      const uECC_word_t *right, const uECC_word_t *mod,
		      wordcount_t num_words)
{
	uECC_word_t product[2 * NUM_ECC_WORDS];
	uECC_vli_mult(product, left, right, num_words);
	uECC_vli_mmod(result, product, mod, num_words);
}

void uECC_vli_modMult_fast(uECC_word_t *result, const uECC_word_t *left,
			   const uECC_word_t *right, uECC_Curve curve)
{
	uECC_word_t product[2 * NUM_ECC_WORDS];
	uECC_vli_mult(product, left, right, curve->num_words);

	curve->mmod_fast(result, product);
}

static void uECC_vli_modSquare_fast(uECC_word_t *result,
				    const uECC_word_t *left,
				    uECC_Curve curve)
{
	uECC_vli_modMult_fast(result, left, left, curve);
}


#define EVEN(vli) (!(vli[0] & 1))

static void vli_modInv_update(uECC_word_t *uv,
			      const uECC_word_t *mod,
			      wordcount_t num_words)
{

	uECC_word_t carry = 0;

	if (!EVEN(uv)) {
		carry = uECC_vli_add(uv, uv, mod, num_words);
	}
	uECC_vli_rshift1(uv, num_words);
	if (carry) {
		uv[num_words - 1] |= HIGH_BIT_SET;
	}
}

void uECC_vli_modInv(uECC_word_t *result, const uECC_word_t *input,
		     const uECC_word_t *mod, wordcount_t num_words)
{
	uECC_word_t a[NUM_ECC_WORDS], b[NUM_ECC_WORDS];
	uECC_word_t u[NUM_ECC_WORDS], v[NUM_ECC_WORDS];
	cmpresult_t cmpResult;

	if (uECC_vli_isZero(input, num_words)) {
		uECC_vli_clear(result, num_words);
		return;
	}

	uECC_vli_set(a, input, num_words);
	uECC_vli_set(b, mod, num_words);
	uECC_vli_clear(u, num_words);
	u[0] = 1;
	uECC_vli_clear(v, num_words);
	while ((cmpResult = uECC_vli_cmp_unsafe(a, b, num_words)) != 0) {
		if (EVEN(a)) {
			uECC_vli_rshift1(a, num_words);
      			vli_modInv_update(u, mod, num_words);
    		} else if (EVEN(b)) {
			uECC_vli_rshift1(b, num_words);
			vli_modInv_update(v, mod, num_words);
		} else if (cmpResult > 0) {
			uECC_vli_sub(a, a, b, num_words);
			uECC_vli_rshift1(a, num_words);
			if (uECC_vli_cmp_unsafe(u, v, num_words) < 0) {
        			uECC_vli_add(u, u, mod, num_words);
      			}
      			uECC_vli_sub(u, u, v, num_words);
      			vli_modInv_update(u, mod, num_words);
    		} else {
      			uECC_vli_sub(b, b, a, num_words);
      			uECC_vli_rshift1(b, num_words);
      			if (uECC_vli_cmp_unsafe(v, u, num_words) < 0) {
        			uECC_vli_add(v, v, mod, num_words);
      			}
      			uECC_vli_sub(v, v, u, num_words);
      			vli_modInv_update(v, mod, num_words);
    		}
  	}
  	uECC_vli_set(result, u, num_words);
}

/* ------ Point operations ------ */

void double_jacobian_default(uECC_word_t * X1, uECC_word_t * Y1,
			     uECC_word_t * Z1, uECC_Curve curve)
{
	/* t1 = X, t2 = Y, t3 = Z */
	uECC_word_t t4[NUM_ECC_WORDS];
	uECC_word_t t5[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;

	if (uECC_vli_isZero(Z1, num_words)) {
		return;
	}

	uECC_vli_modSquare_fast(t4, Y1, curve);   /* t4 = y1^2 */
	uECC_vli_modMult_fast(t5, X1, t4, curve); /* t5 = x1*y1^2 = A */
	uECC_vli_modSquare_fast(t4, t4, curve);   /* t4 = y1^4 */
	uECC_vli_modMult_fast(Y1, Y1, Z1, curve); /* t2 = y1*z1 = z3 */
	uECC_vli_modSquare_fast(Z1, Z1, curve);   /* t3 = z1^2 */

	uECC_vli_modAdd(X1, X1, Z1, curve->p, num_words); /* t1 = x1 + z1^2 */
	uECC_vli_modAdd(Z1, Z1, Z1, curve->p, num_words); /* t3 = 2*z1^2 */
	uECC_vli_modSub(Z1, X1, Z1, curve->p, num_words); /* t3 = x1 - z1^2 */
	uECC_vli_modMult_fast(X1, X1, Z1, curve); /* t1 = x1^2 - z1^4 */

	uECC_vli_modAdd(Z1, X1, X1, curve->p, num_words); /* t3 = 2*(x1^2 - z1^4) */
	uECC_vli_modAdd(X1, X1, Z1, curve->p, num_words); /* t1 = 3*(x1^2 - z1^4) */
	if (uECC_vli_testBit(X1, 0)) {
		uECC_word_t l_carry = uECC_vli_add(X1, X1, curve->p, num_words);
		uECC_vli_rshift1(X1, num_words);
		X1[num_words - 1] |= l_carry << (uECC_WORD_BITS - 1);
	} else {
		uECC_vli_rshift1(X1, num_words);
	}

	/* t1 = 3/2*(x1^2 - z1^4) = B */
	uECC_vli_modSquare_fast(Z1, X1, curve); /* t3 = B^2 */
	uECC_vli_modSub(Z1, Z1, t5, curve->p, num_words); /* t3 = B^2 - A */
	uECC_vli_modSub(Z1, Z1, t5, curve->p, num_words); /* t3 = B^2 - 2A = x3 */
	uECC_vli_modSub(t5, t5, Z1, curve->p, num_words); /* t5 = A - x3 */
	uECC_vli_modMult_fast(X1, X1, t5, curve); /* t1 = B * (A - x3) */
	/* t4 = B * (A - x3) - y1^4 = y3: */
	uECC_vli_modSub(t4, X1, t4, curve->p, num_words);

	uECC_vli_set(X1, Z1, num_words);
	uECC_vli_set(Z1, Y1, num_words);
	uECC_vli_set(Y1, t4, num_words);
}

void x_side_default(uECC_word_t *result,
		    const uECC_word_t *x,
		    uECC_Curve curve)
{
	uECC_word_t _3[NUM_ECC_WORDS] = {3}; /* -a = 3 */
	wordcount_t num_words = curve->num_words;

	uECC_vli_modSquare_fast(result, x, curve); /* r = x^2 */
	uECC_vli_modSub(result, result, _3, curve->p, num_words); /* r = x^2 - 3 */
	uECC_vli_modMult_fast(result, result, x, curve); /* r = x^3 - 3x */
	/* r = x^3 - 3x + b: */
	uECC_vli_modAdd(result, result, curve->b, curve->p, num_words);
}

uECC_Curve uECC_secp256r1(void)
{
	return &curve_secp256r1;
}

void vli_mmod_fast_secp256r1(unsigned int *result, unsigned int*product)
{
	unsigned int tmp[NUM_ECC_WORDS];
	int carry;

	/* t */
	uECC_vli_set(result, product, NUM_ECC_WORDS);

	/* s1 */
	tmp[0] = tmp[1] = tmp[2] = 0;
	tmp[3] = product[11];
	tmp[4] = product[12];
	tmp[5] = product[13];
	tmp[6] = product[14];
	tmp[7] = product[15];
	carry = uECC_vli_add(tmp, tmp, tmp, NUM_ECC_WORDS);
	carry += uECC_vli_add(result, result, tmp, NUM_ECC_WORDS);

	/* s2 */
	tmp[3] = product[12];
	tmp[4] = product[13];
	tmp[5] = product[14];
	tmp[6] = product[15];
	tmp[7] = 0;
	carry += uECC_vli_add(tmp, tmp, tmp, NUM_ECC_WORDS);
	carry += uECC_vli_add(result, result, tmp, NUM_ECC_WORDS);

	/* s3 */
	tmp[0] = product[8];
	tmp[1] = product[9];
	tmp[2] = product[10];
	tmp[3] = tmp[4] = tmp[5] = 0;
	tmp[6] = product[14];
	tmp[7] = product[15];
  	carry += uECC_vli_add(result, result, tmp, NUM_ECC_WORDS);

	/* s4 */
	tmp[0] = product[9];
	tmp[1] = product[10];
	tmp[2] = product[11];
	tmp[3] = product[13];
	tmp[4] = product[14];
	tmp[5] = product[15];
	tmp[6] = product[13];
	tmp[7] = product[8];
	carry += uECC_vli_add(result, result, tmp, NUM_ECC_WORDS);

	/* d1 */
	tmp[0] = product[11];
	tmp[1] = product[12];
	tmp[2] = product[13];
	tmp[3] = tmp[4] = tmp[5] = 0;
	tmp[6] = product[8];
	tmp[7] = product[10];
	carry -= uECC_vli_sub(result, result, tmp, NUM_ECC_WORDS);

	/* d2 */
	tmp[0] = product[12];
	tmp[1] = product[13];
	tmp[2] = product[14];
	tmp[3] = product[15];
	tmp[4] = tmp[5] = 0;
	tmp[6] = product[9];
	tmp[7] = product[11];
	carry -= uECC_vli_sub(result, result, tmp, NUM_ECC_WORDS);

	/* d3 */
	tmp[0] = product[13];
	tmp[1] = product[14];
	tmp[2] = product[15];
	tmp[3] = product[8];
	tmp[4] = product[9];
	tmp[5] = product[10];
	tmp[6] = 0;
	tmp[7] = product[12];
	carry -= uECC_vli_sub(result, result, tmp, NUM_ECC_WORDS);

	/* d4 */
	tmp[0] = product[14];
	tmp[1] = product[15];
	tmp[2] = 0;
	tmp[3] = product[9];
	tmp[4] = product[10];
	tmp[5] = product[11];
	tmp[6] = 0;
	tmp[7] = product[13];
	carry -= uECC_vli_sub(result, result, tmp, NUM_ECC_WORDS);

	if (carry < 0) {
		do {
			carry += uECC_vli_add(result, result, curve_secp256r1.p, NUM_ECC_WORDS);
		}
		while (carry < 0);
	} else  {
		while (carry || 
		       uECC_vli_cmp_unsafe(curve_secp256r1.p, result, NUM_ECC_WORDS) != 1) {
			carry -= uECC_vli_sub(result, result, curve_secp256r1.p, NUM_ECC_WORDS);
		}
	}
}

uECC_word_t EccPoint_isZero(const uECC_word_t *point, uECC_Curve curve)
{
	return uECC_vli_isZero(point, curve->num_words * 2);
}

void apply_z(uECC_word_t * X1, uECC_word_t * Y1, const uECC_word_t * const Z,
	     uECC_Curve curve)
{
	uECC_word_t t1[NUM_ECC_WORDS];

	uECC_vli_modSquare_fast(t1, Z, curve);    /* z^2 */
	uECC_vli_modMult_fast(X1, X1, t1, curve); /* x1 * z^2 */
	uECC_vli_modMult_fast(t1, t1, Z, curve);  /* z^3 */
	uECC_vli_modMult_fast(Y1, Y1, t1, curve); /* y1 * z^3 */
}

/* P = (x1, y1) => 2P, (x2, y2) => P' */
static void XYcZ_initial_double(uECC_word_t * X1, uECC_word_t * Y1,
				uECC_word_t * X2, uECC_word_t * Y2,
				const uECC_word_t * const initial_Z,
				uECC_Curve curve)
{
	uECC_word_t z[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;
	if (initial_Z) {
		uECC_vli_set(z, initial_Z, num_words);
	} else {
		uECC_vli_clear(z, num_words);
		z[0] = 1;
	}

	uECC_vli_set(X2, X1, num_words);
	uECC_vli_set(Y2, Y1, num_words);

	apply_z(X1, Y1, z, curve);
	curve->double_jacobian(X1, Y1, z, curve);
	apply_z(X2, Y2, z, curve);
}

void XYcZ_add(uECC_word_t * X1, uECC_word_t * Y1,
	      uECC_word_t * X2, uECC_word_t * Y2,
	      uECC_Curve curve)
{
	/* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
	uECC_word_t t5[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;

	uECC_vli_modSub(t5, X2, X1, curve->p, num_words); /* t5 = x2 - x1 */
	uECC_vli_modSquare_fast(t5, t5, curve); /* t5 = (x2 - x1)^2 = A */
	uECC_vli_modMult_fast(X1, X1, t5, curve); /* t1 = x1*A = B */
	uECC_vli_modMult_fast(X2, X2, t5, curve); /* t3 = x2*A = C */
	uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words); /* t4 = y2 - y1 */
	uECC_vli_modSquare_fast(t5, Y2, curve); /* t5 = (y2 - y1)^2 = D */

	uECC_vli_modSub(t5, t5, X1, curve->p, num_words); /* t5 = D - B */
	uECC_vli_modSub(t5, t5, X2, curve->p, num_words); /* t5 = D - B - C = x3 */
	uECC_vli_modSub(X2, X2, X1, curve->p, num_words); /* t3 = C - B */
	uECC_vli_modMult_fast(Y1, Y1, X2, curve); /* t2 = y1*(C - B) */
	uECC_vli_modSub(X2, X1, t5, curve->p, num_words); /* t3 = B - x3 */
	uECC_vli_modMult_fast(Y2, Y2, X2, curve); /* t4 = (y2 - y1)*(B - x3) */
	uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words); /* t4 = y3 */

	uECC_vli_set(X2, t5, num_words);
}

/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P + Q = (x3, y3, Z3), P - Q = (x3', y3', Z3)
   or P => P - Q, Q => P + Q
 */
static void XYcZ_addC(uECC_word_t * X1, uECC_word_t * Y1,
		      uECC_word_t * X2, uECC_word_t * Y2,
		      uECC_Curve curve)
{
	/* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
	uECC_word_t t5[NUM_ECC_WORDS];
	uECC_word_t t6[NUM_ECC_WORDS];
	uECC_word_t t7[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;

	uECC_vli_modSub(t5, X2, X1, curve->p, num_words); /* t5 = x2 - x1 */
	uECC_vli_modSquare_fast(t5, t5, curve); /* t5 = (x2 - x1)^2 = A */
	uECC_vli_modMult_fast(X1, X1, t5, curve); /* t1 = x1*A = B */
	uECC_vli_modMult_fast(X2, X2, t5, curve); /* t3 = x2*A = C */
	uECC_vli_modAdd(t5, Y2, Y1, curve->p, num_words); /* t5 = y2 + y1 */
	uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words); /* t4 = y2 - y1 */

	uECC_vli_modSub(t6, X2, X1, curve->p, num_words); /* t6 = C - B */
	uECC_vli_modMult_fast(Y1, Y1, t6, curve); /* t2 = y1 * (C - B) = E */
	uECC_vli_modAdd(t6, X1, X2, curve->p, num_words); /* t6 = B + C */
	uECC_vli_modSquare_fast(X2, Y2, curve); /* t3 = (y2 - y1)^2 = D */
	uECC_vli_modSub(X2, X2, t6, curve->p, num_words); /* t3 = D - (B + C) = x3 */

	uECC_vli_modSub(t7, X1, X2, curve->p, num_words); /* t7 = B - x3 */
	uECC_vli_modMult_fast(Y2, Y2, t7, curve); /* t4 = (y2 - y1)*(B - x3) */
	/* t4 = (y2 - y1)*(B - x3) - E = y3: */
	uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words);

	uECC_vli_modSquare_fast(t7, t5, curve); /* t7 = (y2 + y1)^2 = F */
	uECC_vli_modSub(t7, t7, t6, curve->p, num_words); /* t7 = F - (B + C) = x3' */
	uECC_vli_modSub(t6, t7, X1, curve->p, num_words); /* t6 = x3' - B */
	uECC_vli_modMult_fast(t6, t6, t5, curve); /* t6 = (y2+y1)*(x3' - B) */
	/* t2 = (y2+y1)*(x3' - B) - E = y3': */
	uECC_vli_modSub(Y1, t6, Y1, curve->p, num_words);

	uECC_vli_set(X1, t7, num_words);
}

void EccPoint_mult(uECC_word_t * result, const uECC_word_t * point,
		   const uECC_word_t * scalar,
		   const uECC_word_t * initial_Z,
		   bitcount_t num_bits, uECC_Curve curve) 
{
	/* R0 and R1 */
	uECC_word_t Rx[2][NUM_ECC_WORDS];
	uECC_word_t Ry[2][NUM_ECC_WORDS];
	uECC_word_t z[NUM_ECC_WORDS];
	bitcount_t i;
	uECC_word_t nb;
	wordcount_t num_words = curve->num_words;

	uECC_vli_set(Rx[1], point, num_words);
  	uECC_vli_set(Ry[1], point + num_words, num_words);

	XYcZ_initial_double(Rx[1], Ry[1], Rx[0], Ry[0], initial_Z, curve);

	for (i = num_bits - 2; i > 0; --i) {
		nb = !uECC_vli_testBit(scalar, i);
		XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb], curve);
		XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb], curve);
	}

	nb = !uECC_vli_testBit(scalar, 0);
	XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb], curve);

	/* Find final 1/Z value. */
	uECC_vli_modSub(z, Rx[1], Rx[0], curve->p, num_words); /* X1 - X0 */
	uECC_vli_modMult_fast(z, z, Ry[1 - nb], curve); /* Yb * (X1 - X0) */
	uECC_vli_modMult_fast(z, z, point, curve); /* xP * Yb * (X1 - X0) */
	uECC_vli_modInv(z, z, curve->p, num_words); /* 1 / (xP * Yb * (X1 - X0))*/
	/* yP / (xP * Yb * (X1 - X0)) */
	uECC_vli_modMult_fast(z, z, point + num_words, curve);
	/* Xb * yP / (xP * Yb * (X1 - X0)) */
	uECC_vli_modMult_fast(z, z, Rx[1 - nb], curve);
	/* End 1/Z calculation */

	XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb], curve);
	apply_z(Rx[0], Ry[0], z, curve);

	uECC_vli_set(result, Rx[0], num_words);
	uECC_vli_set(result + num_words, Ry[0], num_words);
}

uECC_word_t regularize_k(const uECC_word_t * const k, uECC_word_t *k0,
			 uECC_word_t *k1, uECC_Curve curve)
{

	wordcount_t num_n_words = BITS_TO_WORDS(curve->num_n_bits);

	bitcount_t num_n_bits = curve->num_n_bits;

	uECC_word_t carry = uECC_vli_add(k0, k, curve->n, num_n_words) ||
			     (num_n_bits < ((bitcount_t)num_n_words * uECC_WORD_SIZE * 8) &&
			     uECC_vli_testBit(k0, num_n_bits));

	uECC_vli_add(k1, k0, curve->n, num_n_words);

	return carry;
}

uECC_word_t EccPoint_compute_public_key(uECC_word_t *result,
					uECC_word_t *private_key,
					uECC_Curve curve)
{

	uECC_word_t tmp1[NUM_ECC_WORDS];
 	uECC_word_t tmp2[NUM_ECC_WORDS];
	uECC_word_t *p2[2] = {tmp1, tmp2};
	uECC_word_t carry;

	/* Regularize the bitcount for the private key so that attackers cannot
	 * use a side channel attack to learn the number of leading zeros. */
	carry = regularize_k(private_key, tmp1, tmp2, curve);

	EccPoint_mult(result, curve->G, p2[!carry], 0, curve->num_n_bits + 1, curve);

	if (EccPoint_isZero(result, curve)) {
		return 0;
	}
	return 1;
}

/* Converts an integer in uECC native format to big-endian bytes. */
void uECC_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
			    const unsigned int *native)
{
	wordcount_t i;
	for (i = 0; i < num_bytes; ++i) {
		unsigned b = num_bytes - 1 - i;
		bytes[i] = native[b / uECC_WORD_SIZE] >> (8 * (b % uECC_WORD_SIZE));
	}
}

/* Converts big-endian bytes to an integer in uECC native format. */
void uECC_vli_bytesToNative(unsigned int *native, const uint8_t *bytes,
			    int num_bytes)
{
	wordcount_t i;
	uECC_vli_clear(native, (num_bytes + (uECC_WORD_SIZE - 1)) / uECC_WORD_SIZE);
	for (i = 0; i < num_bytes; ++i) {
		unsigned b = num_bytes - 1 - i;
		native[b / uECC_WORD_SIZE] |=
			(uECC_word_t)bytes[i] << (8 * (b % uECC_WORD_SIZE));
  	}
}

int uECC_generate_random_int(uECC_word_t *random, const uECC_word_t *top,
			     wordcount_t num_words)
{
	uECC_word_t mask = (uECC_word_t)-1;
	uECC_word_t tries;
	bitcount_t num_bits = uECC_vli_numBits(top, num_words);

	if (!g_rng_function) {
		return 0;
	}

	for (tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries) {
		if (!g_rng_function((uint8_t *)random, num_words * uECC_WORD_SIZE)) {
      			return 0;
    		}
		random[num_words - 1] &=
        		mask >> ((bitcount_t)(num_words * uECC_WORD_SIZE * 8 - num_bits));
		if (!uECC_vli_isZero(random, num_words) &&
			uECC_vli_cmp(top, random, num_words) == 1) {
			return 1;
		}
	}
	return 0;
}


int uECC_valid_point(const uECC_word_t *point, uECC_Curve curve)
{
	uECC_word_t tmp1[NUM_ECC_WORDS];
	uECC_word_t tmp2[NUM_ECC_WORDS];
	wordcount_t num_words = curve->num_words;

	/* The point at infinity is invalid. */
	if (EccPoint_isZero(point, curve)) {
		return -1;
	}

	/* x and y must be smaller than p. */
	if (uECC_vli_cmp_unsafe(curve->p, point, num_words) != 1 ||
		uECC_vli_cmp_unsafe(curve->p, point + num_words, num_words) != 1) {
		return -2;
	}

	uECC_vli_modSquare_fast(tmp1, point + num_words, curve);
	curve->x_side(tmp2, point, curve); /* tmp2 = x^3 + ax + b */

	/* Make sure that y^2 == x^3 + ax + b */
	if (uECC_vli_equal(tmp1, tmp2, num_words) != 0)
		return -3;

	return 0;
}

int uECC_valid_public_key(const uint8_t *public_key, uECC_Curve curve)
{

	uECC_word_t _public[NUM_ECC_WORDS * 2];

	uECC_vli_bytesToNative(_public, public_key, curve->num_bytes);
	uECC_vli_bytesToNative(
	_public + curve->num_words,
	public_key + curve->num_bytes,
	curve->num_bytes);

	if (uECC_vli_cmp_unsafe(_public, curve->G, NUM_ECC_WORDS * 2) == 0) {
		return -4;
	}

	return uECC_valid_point(_public, curve);
}

int uECC_compute_public_key(const uint8_t *private_key, uint8_t *public_key,
			    uECC_Curve curve)
{

	uECC_word_t _private[NUM_ECC_WORDS];
	uECC_word_t _public[NUM_ECC_WORDS * 2];

	uECC_vli_bytesToNative(
	_private,
	private_key,
	BITS_TO_BYTES(curve->num_n_bits));

	/* Make sure the private key is in the range [1, n-1]. */
	if (uECC_vli_isZero(_private, BITS_TO_WORDS(curve->num_n_bits))) {
		return 0;
	}

	if (uECC_vli_cmp(curve->n, _private, BITS_TO_WORDS(curve->num_n_bits)) != 1) {
		return 0;
	}

	/* Compute public key. */
	if (!EccPoint_compute_public_key(_public, _private, curve)) {
		return 0;
	}

	uECC_vli_nativeToBytes(public_key, curve->num_bytes, _public);
	uECC_vli_nativeToBytes(
	public_key +
	curve->num_bytes, curve->num_bytes, _public + curve->num_words);
	return 1;
}



