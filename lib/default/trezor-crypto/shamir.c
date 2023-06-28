/*
 * Implementation of the hazardous parts of the SSS library
 *
 * Copyright (c) 2017 Daan Sprenkels <hello@dsprenkels.com>
 * Copyright (c) 2019 SatoshiLabs
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
 *
 * This code contains the actual Shamir secret sharing functionality. The
 * implementation of this code is based on the idea that the user likes to
 * generate/combine 32 shares (in GF(2^8)) at the same time, because a 256 bit
 * key will be exactly 32 bytes. Therefore we bitslice all the input and
 * unbitslice the output right before returning.
 *
 * This bitslice approach optimizes natively on all architectures that are 32
 * bit or more. Care is taken to use not too many registers, to ensure that no
 * values have to be leaked to the stack.
 *
 * All functions in this module are implemented constant time and constant
 * lookup operations, as all proper crypto code must be.
 */

#include "shamir.h"
#include <string.h>
#include "memzero.h"

static void bitslice(uint32_t r[8], const uint8_t *x, size_t len) {
  size_t bit_idx, arr_idx;
  uint32_t cur;

  memset(r, 0, sizeof(uint32_t[8]));
  for (arr_idx = 0; arr_idx < len; arr_idx++) {
    cur = (uint32_t)x[arr_idx];
    for (bit_idx = 0; bit_idx < 8; bit_idx++) {
      r[bit_idx] |= ((cur & (1 << bit_idx)) >> bit_idx) << arr_idx;
    }
  }
}

static void unbitslice(uint8_t *r, const uint32_t x[8], size_t len) {
  size_t bit_idx, arr_idx;
  uint32_t cur;

  memset(r, 0, sizeof(uint8_t) * len);
  for (bit_idx = 0; bit_idx < 8; bit_idx++) {
    cur = (uint32_t)x[bit_idx];
    for (arr_idx = 0; arr_idx < len; arr_idx++) {
      r[arr_idx] |= ((cur & (1 << arr_idx)) >> arr_idx) << bit_idx;
    }
  }
}

static void bitslice_setall(uint32_t r[8], const uint8_t x) {
  size_t idx;
  for (idx = 0; idx < 8; idx++) {
    r[idx] = -((x >> idx) & 1);
  }
}

/*
 * Add (XOR) `r` with `x` and store the result in `r`.
 */
static void gf256_add(uint32_t r[8], const uint32_t x[8]) {
  size_t idx;
  for (idx = 0; idx < 8; idx++) r[idx] ^= x[idx];
}

/*
 * Safely multiply two bitsliced polynomials in GF(2^8) reduced by
 * x^8 + x^4 + x^3 + x + 1. `r` and `a` may overlap, but overlapping of `r`
 * and `b` will produce an incorrect result! If you need to square a polynomial
 * use `gf256_square` instead.
 */
static void gf256_mul(uint32_t r[8], const uint32_t a[8], const uint32_t b[8]) {
  /* This function implements Russian Peasant multiplication on two
   * bitsliced polynomials.
   *
   * I personally think that these kinds of long lists of operations
   * are often a bit ugly. A double for loop would be nicer and would
   * take up a lot less lines of code.
   * However, some compilers seem to fail in optimizing these kinds of
   * loops. So we will just have to do this by hand.
   */
  uint32_t a2[8];
  memcpy(a2, a, sizeof(uint32_t[8]));

  r[0] = a2[0] & b[0]; /* add (assignment, because r is 0) */
  r[1] = a2[1] & b[0];
  r[2] = a2[2] & b[0];
  r[3] = a2[3] & b[0];
  r[4] = a2[4] & b[0];
  r[5] = a2[5] & b[0];
  r[6] = a2[6] & b[0];
  r[7] = a2[7] & b[0];
  a2[0] ^= a2[7]; /* reduce */
  a2[2] ^= a2[7];
  a2[3] ^= a2[7];

  r[0] ^= a2[7] & b[1]; /* add */
  r[1] ^= a2[0] & b[1];
  r[2] ^= a2[1] & b[1];
  r[3] ^= a2[2] & b[1];
  r[4] ^= a2[3] & b[1];
  r[5] ^= a2[4] & b[1];
  r[6] ^= a2[5] & b[1];
  r[7] ^= a2[6] & b[1];
  a2[7] ^= a2[6]; /* reduce */
  a2[1] ^= a2[6];
  a2[2] ^= a2[6];

  r[0] ^= a2[6] & b[2]; /* add */
  r[1] ^= a2[7] & b[2];
  r[2] ^= a2[0] & b[2];
  r[3] ^= a2[1] & b[2];
  r[4] ^= a2[2] & b[2];
  r[5] ^= a2[3] & b[2];
  r[6] ^= a2[4] & b[2];
  r[7] ^= a2[5] & b[2];
  a2[6] ^= a2[5]; /* reduce */
  a2[0] ^= a2[5];
  a2[1] ^= a2[5];

  r[0] ^= a2[5] & b[3]; /* add */
  r[1] ^= a2[6] & b[3];
  r[2] ^= a2[7] & b[3];
  r[3] ^= a2[0] & b[3];
  r[4] ^= a2[1] & b[3];
  r[5] ^= a2[2] & b[3];
  r[6] ^= a2[3] & b[3];
  r[7] ^= a2[4] & b[3];
  a2[5] ^= a2[4]; /* reduce */
  a2[7] ^= a2[4];
  a2[0] ^= a2[4];

  r[0] ^= a2[4] & b[4]; /* add */
  r[1] ^= a2[5] & b[4];
  r[2] ^= a2[6] & b[4];
  r[3] ^= a2[7] & b[4];
  r[4] ^= a2[0] & b[4];
  r[5] ^= a2[1] & b[4];
  r[6] ^= a2[2] & b[4];
  r[7] ^= a2[3] & b[4];
  a2[4] ^= a2[3]; /* reduce */
  a2[6] ^= a2[3];
  a2[7] ^= a2[3];

  r[0] ^= a2[3] & b[5]; /* add */
  r[1] ^= a2[4] & b[5];
  r[2] ^= a2[5] & b[5];
  r[3] ^= a2[6] & b[5];
  r[4] ^= a2[7] & b[5];
  r[5] ^= a2[0] & b[5];
  r[6] ^= a2[1] & b[5];
  r[7] ^= a2[2] & b[5];
  a2[3] ^= a2[2]; /* reduce */
  a2[5] ^= a2[2];
  a2[6] ^= a2[2];

  r[0] ^= a2[2] & b[6]; /* add */
  r[1] ^= a2[3] & b[6];
  r[2] ^= a2[4] & b[6];
  r[3] ^= a2[5] & b[6];
  r[4] ^= a2[6] & b[6];
  r[5] ^= a2[7] & b[6];
  r[6] ^= a2[0] & b[6];
  r[7] ^= a2[1] & b[6];
  a2[2] ^= a2[1]; /* reduce */
  a2[4] ^= a2[1];
  a2[5] ^= a2[1];

  r[0] ^= a2[1] & b[7]; /* add */
  r[1] ^= a2[2] & b[7];
  r[2] ^= a2[3] & b[7];
  r[3] ^= a2[4] & b[7];
  r[4] ^= a2[5] & b[7];
  r[5] ^= a2[6] & b[7];
  r[6] ^= a2[7] & b[7];
  r[7] ^= a2[0] & b[7];

  //memzero(a2, sizeof(a2));
}

/*
 * Square `x` in GF(2^8) and write the result to `r`. `r` and `x` may overlap.
 */
static void gf256_square(uint32_t r[8], const uint32_t x[8]) {
  uint32_t r8, r10, r12, r14;
  /* Use the Freshman's Dream rule to square the polynomial
   * Assignments are done from 7 downto 0, because this allows the user
   * to execute this function in-place (e.g. `gf256_square(r, r);`).
   */
  r14 = x[7];
  r12 = x[6];
  r10 = x[5];
  r8 = x[4];
  r[6] = x[3];
  r[4] = x[2];
  r[2] = x[1];
  r[0] = x[0];

  /* Reduce with  x^8 + x^4 + x^3 + x + 1 until order is less than 8 */
  r[7] = r14; /* r[7] was 0 */
  r[6] ^= r14;
  r10 ^= r14;
  /* Skip, because r13 is always 0 */
  r[4] ^= r12;
  r[5] = r12; /* r[5] was 0 */
  r[7] ^= r12;
  r8 ^= r12;
  /* Skip, because r11 is always 0 */
  r[2] ^= r10;
  r[3] = r10; /* r[3] was 0 */
  r[5] ^= r10;
  r[6] ^= r10;
  r[1] = r14;  /* r[1] was 0 */
  r[2] ^= r14; /* Substitute r9 by r14 because they will always be equal*/
  r[4] ^= r14;
  r[5] ^= r14;
  r[0] ^= r8;
  r[1] ^= r8;
  r[3] ^= r8;
  r[4] ^= r8;
}

/*
 * Invert `x` in GF(2^8) and write the result to `r`
 */
static void gf256_inv(uint32_t r[8], uint32_t x[8]) {
  uint32_t y[8], z[8];

  gf256_square(y, x);  // y = x^2
  gf256_square(y, y);  // y = x^4
  gf256_square(r, y);  // r = x^8
  gf256_mul(z, r, x);  // z = x^9
  gf256_square(r, r);  // r = x^16
  gf256_mul(r, r, z);  // r = x^25
  gf256_square(r, r);  // r = x^50
  gf256_square(z, r);  // z = x^100
  gf256_square(z, z);  // z = x^200
  gf256_mul(r, r, z);  // r = x^250
  gf256_mul(r, r, y);  // r = x^254

  //memzero(y, sizeof(y));
  //memzero(z, sizeof(z));
}

bool shamir_interpolate(uint8_t *result, uint8_t result_index,
                        const uint8_t *share_indices,
                        const uint8_t **share_values, uint8_t share_count,
                        size_t len) {
  size_t i, j;
  uint32_t x[8];
  uint32_t xs[share_count][8];
  uint32_t ys[share_count][8];
  uint32_t num[8] = {~0}; /* num is the numerator (=1) */
  uint32_t denom[8];
  uint32_t tmp[8];
  uint32_t secret[8] = {0};
  bool ret = true;

  if (len > SHAMIR_MAX_LEN) return false;

  /* Collect the x and y values */
  for (i = 0; i < share_count; i++) {
    bitslice_setall(xs[i], share_indices[i]);
    bitslice(ys[i], share_values[i], len);
  }
  bitslice_setall(x, result_index);

  for (i = 0; i < share_count; i++) {
    memcpy(tmp, x, sizeof(uint32_t[8]));
    gf256_add(tmp, xs[i]);
    gf256_mul(num, num, tmp);
  }

  /* Use Lagrange basis polynomials to calculate the secret coefficient */
  for (i = 0; i < share_count; i++) {
    /* The code below assumes that none of the share_indices are equal to
     * result_index. We need to treat that as a special case. */
    if (share_indices[i] != result_index) {
      memcpy(denom, x, sizeof(denom));
      gf256_add(denom, xs[i]);
    } else {
      bitslice_setall(denom, 1);
      gf256_add(secret, ys[i]);
    }
    for (j = 0; j < share_count; j++) {
      if (i == j) continue;
      memcpy(tmp, xs[i], sizeof(uint32_t[8]));
      gf256_add(tmp, xs[j]);
      gf256_mul(denom, denom, tmp);
    }
    if ((denom[0] | denom[1] | denom[2] | denom[3] | denom[4] | denom[5] |
         denom[6] | denom[7]) == 0) {
      /* The share_indices are not unique. */
      ret = false;
      break;
    }
    gf256_inv(tmp, denom);      /* inverted denominator */
    gf256_mul(tmp, tmp, num);   /* basis polynomial */
    gf256_mul(tmp, tmp, ys[i]); /* scaled coefficient */
    gf256_add(secret, tmp);
  }

  if (ret == true) {
    unbitslice(result, secret, len);
  }

  memzero(x, sizeof(x));
  memzero(xs, sizeof(xs));
  memzero(ys, sizeof(ys));
  memzero(num, sizeof(num));
  memzero(denom, sizeof(denom));
  memzero(tmp, sizeof(tmp));
  memzero(secret, sizeof(secret));
  return ret;
}
