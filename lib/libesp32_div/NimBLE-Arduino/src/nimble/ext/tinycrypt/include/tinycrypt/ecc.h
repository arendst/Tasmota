/* ecc.h - TinyCrypt interface to common ECC functions */

/* Copyright (c) 2014, Kenneth MacKay
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
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

/**
 * @file
 * @brief -- Interface to common ECC functions.
 *
 *  Overview: This software is an implementation of common functions
 *            necessary to elliptic curve cryptography. This implementation uses
 *            curve NIST p-256.
 *
 *  Security: The curve NIST p-256 provides approximately 128 bits of security.
 *
 */

#ifndef __TC_UECC_H__
#define __TC_UECC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Word size (4 bytes considering 32-bits architectures) */
#define uECC_WORD_SIZE 4

/* setting max number of calls to prng: */
#ifndef uECC_RNG_MAX_TRIES
#define uECC_RNG_MAX_TRIES 64
#endif

/* defining data types to store word and bit counts: */
typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
/* defining data type for comparison result: */
typedef int8_t cmpresult_t;
/* defining data type to store ECC coordinate/point in 32bits words: */
typedef unsigned int uECC_word_t;
/* defining data type to store an ECC coordinate/point in 64bits words: */
typedef uint64_t uECC_dword_t;

/* defining masks useful for ecc computations: */
#define HIGH_BIT_SET 0x80000000
#define uECC_WORD_BITS 32
#define uECC_WORD_BITS_SHIFT 5
#define uECC_WORD_BITS_MASK 0x01F

/* Number of words of 32 bits to represent an element of the the curve p-256: */
#define NUM_ECC_WORDS 8
/* Number of bytes to represent an element of the the curve p-256: */
#define NUM_ECC_BYTES (uECC_WORD_SIZE*NUM_ECC_WORDS)

/* structure that represents an elliptic curve (e.g. p256):*/
struct uECC_Curve_t;
typedef const struct uECC_Curve_t * uECC_Curve;
struct uECC_Curve_t {
  wordcount_t num_words;
  wordcount_t num_bytes;
  bitcount_t num_n_bits;
  uECC_word_t p[NUM_ECC_WORDS];
  uECC_word_t n[NUM_ECC_WORDS];
  uECC_word_t G[NUM_ECC_WORDS * 2];
  uECC_word_t b[NUM_ECC_WORDS];
  void (*double_jacobian)(uECC_word_t * X1, uECC_word_t * Y1, uECC_word_t * Z1,
	uECC_Curve curve);
  void (*x_side)(uECC_word_t *result, const uECC_word_t *x, uECC_Curve curve);
  void (*mmod_fast)(uECC_word_t *result, uECC_word_t *product);
};

/*
 * @brief computes doubling of point ion jacobian coordinates, in place.
 * @param X1 IN/OUT -- x coordinate
 * @param Y1 IN/OUT -- y coordinate
 * @param Z1 IN/OUT -- z coordinate
 * @param curve IN -- elliptic curve
 */
void double_jacobian_default(uECC_word_t * X1, uECC_word_t * Y1,
			     uECC_word_t * Z1, uECC_Curve curve);

/*
 * @brief Computes x^3 + ax + b. result must not overlap x.
 * @param result OUT -- x^3 + ax + b
 * @param x IN -- value of x
 * @param curve IN -- elliptic curve
 */
void x_side_default(uECC_word_t *result, const uECC_word_t *x,
		    uECC_Curve curve);

/*
 * @brief Computes result = product % curve_p
 * from http://www.nsa.gov/ia/_files/nist-routines.pdf
 * @param result OUT -- product % curve_p
 * @param product IN -- value to be reduced mod curve_p
 */
void vli_mmod_fast_secp256r1(unsigned int *result, unsigned int *product);

/* Bytes to words ordering: */
#define BYTES_TO_WORDS_8(a, b, c, d, e, f, g, h) 0x##d##c##b##a, 0x##h##g##f##e
#define BYTES_TO_WORDS_4(a, b, c, d) 0x##d##c##b##a
#define BITS_TO_WORDS(num_bits) \
	((num_bits + ((uECC_WORD_SIZE * 8) - 1)) / (uECC_WORD_SIZE * 8))
#define BITS_TO_BYTES(num_bits) ((num_bits + 7) / 8)

/* definition of curve NIST p-256: */
static const struct uECC_Curve_t curve_secp256r1 = {
	NUM_ECC_WORDS,
	NUM_ECC_BYTES,
	256, /* num_n_bits */ {
		BYTES_TO_WORDS_8(FF, FF, FF, FF, FF, FF, FF, FF),
		BYTES_TO_WORDS_8(FF, FF, FF, FF, 00, 00, 00, 00),
        	BYTES_TO_WORDS_8(00, 00, 00, 00, 00, 00, 00, 00),
        	BYTES_TO_WORDS_8(01, 00, 00, 00, FF, FF, FF, FF)
	}, {
		BYTES_TO_WORDS_8(51, 25, 63, FC, C2, CA, B9, F3),
            	BYTES_TO_WORDS_8(84, 9E, 17, A7, AD, FA, E6, BC),
            	BYTES_TO_WORDS_8(FF, FF, FF, FF, FF, FF, FF, FF),
            	BYTES_TO_WORDS_8(00, 00, 00, 00, FF, FF, FF, FF)
	}, {
		BYTES_TO_WORDS_8(96, C2, 98, D8, 45, 39, A1, F4),
                BYTES_TO_WORDS_8(A0, 33, EB, 2D, 81, 7D, 03, 77),
                BYTES_TO_WORDS_8(F2, 40, A4, 63, E5, E6, BC, F8),
                BYTES_TO_WORDS_8(47, 42, 2C, E1, F2, D1, 17, 6B),

                BYTES_TO_WORDS_8(F5, 51, BF, 37, 68, 40, B6, CB),
                BYTES_TO_WORDS_8(CE, 5E, 31, 6B, 57, 33, CE, 2B),
                BYTES_TO_WORDS_8(16, 9E, 0F, 7C, 4A, EB, E7, 8E),
                BYTES_TO_WORDS_8(9B, 7F, 1A, FE, E2, 42, E3, 4F)
	}, {
		BYTES_TO_WORDS_8(4B, 60, D2, 27, 3E, 3C, CE, 3B),
                BYTES_TO_WORDS_8(F6, B0, 53, CC, B0, 06, 1D, 65),
                BYTES_TO_WORDS_8(BC, 86, 98, 76, 55, BD, EB, B3),
                BYTES_TO_WORDS_8(E7, 93, 3A, AA, D8, 35, C6, 5A)
	},
        &double_jacobian_default,
        &x_side_default,
        &vli_mmod_fast_secp256r1
};

uECC_Curve uECC_secp256r1(void);

/*
 * @brief Generates a random integer in the range 0 < random < top.
 * Both random and top have num_words words.
 * @param random OUT -- random integer in the range 0 < random < top
 * @param top IN -- upper limit
 * @param num_words IN -- number of words
 * @return a random integer in the range 0 < random < top
 */
int uECC_generate_random_int(uECC_word_t *random, const uECC_word_t *top,
			     wordcount_t num_words);


/* uECC_RNG_Function type
 * The RNG function should fill 'size' random bytes into 'dest'. It should
 * return 1 if 'dest' was filled with random data, or 0 if the random data could
 * not be generated. The filled-in values should be either truly random, or from
 * a cryptographically-secure PRNG.
 *
 * A correctly functioning RNG function must be set (using uECC_set_rng())
 * before calling uECC_make_key() or uECC_sign().
 *
 * Setting a correctly functioning RNG function improves the resistance to
 * side-channel attacks for uECC_shared_secret().
 *
 * A correct RNG function is set by default. If you are building on another
 * POSIX-compliant system that supports /dev/random or /dev/urandom, you can
 * define uECC_POSIX to use the predefined RNG.
 */
typedef int(*uECC_RNG_Function)(uint8_t *dest, unsigned int size);

/*
 * @brief Set the function that will be used to generate random bytes. The RNG
 * function should return 1 if the random data was generated, or 0 if the random
 * data could not be generated.
 *
 * @note On platforms where there is no predefined RNG function, this must be
 * called before uECC_make_key() or uECC_sign() are used.
 *
 * @param rng_function IN -- function that will be used to generate random bytes
 */
void uECC_set_rng(uECC_RNG_Function rng_function);

/*
 * @brief provides current uECC_RNG_Function.
 * @return Returns the function that will be used to generate random bytes.
 */
uECC_RNG_Function uECC_get_rng(void);

/*
 * @brief computes the size of a private key for the curve in bytes.
 * @param curve IN -- elliptic curve
 * @return size of a private key for the curve in bytes.
 */
int uECC_curve_private_key_size(uECC_Curve curve);

/*
 * @brief computes the size of a public key for the curve in bytes.
 * @param curve IN -- elliptic curve
 * @return the size of a public key for the curve in bytes.
 */
int uECC_curve_public_key_size(uECC_Curve curve);

/*
 * @brief Compute the corresponding public key for a private key.
 * @param private_key IN -- The private key to compute the public key for
 * @param public_key OUT -- Will be filled in with the corresponding public key
 * @param curve
 * @return Returns 1 if key was computed successfully, 0 if an error occurred.
 */
int uECC_compute_public_key(const uint8_t *private_key,
			    uint8_t *public_key, uECC_Curve curve);

/*
 * @brief Compute public-key.
 * @return corresponding public-key.
 * @param result OUT -- public-key
 * @param private_key IN -- private-key
 * @param curve IN -- elliptic curve
 */
uECC_word_t EccPoint_compute_public_key(uECC_word_t *result,
					uECC_word_t *private_key, uECC_Curve curve);

/*
 * @brief Regularize the bitcount for the private key so that attackers cannot
 * use a side channel attack to learn the number of leading zeros.
 * @return Regularized k
 * @param k IN -- private-key
 * @param k0 IN/OUT -- regularized k
 * @param k1 IN/OUT -- regularized k
 * @param curve IN -- elliptic curve
 */
uECC_word_t regularize_k(const uECC_word_t * const k, uECC_word_t *k0,
			 uECC_word_t *k1, uECC_Curve curve);

/*
 * @brief Point multiplication algorithm using Montgomery's ladder with co-Z
 * coordinates. See http://eprint.iacr.org/2011/338.pdf.
 * @note Result may overlap point.
 * @param result OUT -- returns scalar*point
 * @param point IN -- elliptic curve point
 * @param scalar IN -- scalar
 * @param initial_Z IN -- initial value for z
 * @param num_bits IN -- number of bits in scalar
 * @param curve IN -- elliptic curve
 */
void EccPoint_mult(uECC_word_t * result, const uECC_word_t * point,
		   const uECC_word_t * scalar, const uECC_word_t * initial_Z,
		   bitcount_t num_bits, uECC_Curve curve);

/*
 * @brief Constant-time comparison to zero - secure way to compare long integers
 * @param vli IN -- very long integer
 * @param num_words IN -- number of words in the vli
 * @return 1 if vli == 0, 0 otherwise.
 */
uECC_word_t uECC_vli_isZero(const uECC_word_t *vli, wordcount_t num_words);

/*
 * @brief Check if 'point' is the point at infinity
 * @param point IN -- elliptic curve point
 * @param curve IN -- elliptic curve
 * @return if 'point' is the point at infinity, 0 otherwise.
 */
uECC_word_t EccPoint_isZero(const uECC_word_t *point, uECC_Curve curve);

/*
 * @brief computes the sign of left - right, in constant time.
 * @param left IN -- left term to be compared
 * @param right IN -- right term to be compared
 * @param num_words IN -- number of words
 * @return the sign of left - right
 */
cmpresult_t uECC_vli_cmp(const uECC_word_t *left, const uECC_word_t *right,
			 wordcount_t num_words);

/*
 * @brief computes sign of left - right, not in constant time.
 * @note should not be used if inputs are part of a secret
 * @param left IN -- left term to be compared
 * @param right IN -- right term to be compared
 * @param num_words IN -- number of words
 * @return the sign of left - right
 */
cmpresult_t uECC_vli_cmp_unsafe(const uECC_word_t *left, const uECC_word_t *right,
				wordcount_t num_words);

/*
 * @brief Computes result = (left - right) % mod.
 * @note Assumes that (left < mod) and (right < mod), and that result does not
 * overlap mod.
 * @param result OUT -- (left - right) % mod
 * @param left IN -- leftright term in modular subtraction
 * @param right IN -- right term in modular subtraction
 * @param mod IN -- mod
 * @param num_words IN -- number of words
 */
void uECC_vli_modSub(uECC_word_t *result, const uECC_word_t *left,
		     const uECC_word_t *right, const uECC_word_t *mod,
		     wordcount_t num_words);

/*
 * @brief Computes P' = (x1', y1', Z3), P + Q = (x3, y3, Z3) or
 * P => P', Q => P + Q
 * @note assumes Input P = (x1, y1, Z), Q = (x2, y2, Z)
 * @param X1 IN -- x coordinate of P
 * @param Y1 IN -- y coordinate of P
 * @param X2 IN -- x coordinate of Q
 * @param Y2 IN -- y coordinate of Q
 * @param curve IN -- elliptic curve
 */
void XYcZ_add(uECC_word_t * X1, uECC_word_t * Y1, uECC_word_t * X2,
	      uECC_word_t * Y2, uECC_Curve curve);

/*
 * @brief Computes (x1 * z^2, y1 * z^3)
 * @param X1 IN -- previous x1 coordinate
 * @param Y1 IN -- previous y1 coordinate
 * @param Z IN -- z value
 * @param curve IN -- elliptic curve
 */
void apply_z(uECC_word_t * X1, uECC_word_t * Y1, const uECC_word_t * const Z,
	     uECC_Curve curve);

/*
 * @brief Check if bit is set.
 * @return Returns nonzero if bit 'bit' of vli is set.
 * @warning It is assumed that the value provided in 'bit' is within the
 * boundaries of the word-array 'vli'.
 * @note The bit ordering layout assumed for vli is: {31, 30, ..., 0},
 * {63, 62, ..., 32}, {95, 94, ..., 64}, {127, 126,..., 96} for a vli consisting
 * of 4 uECC_word_t elements.
 */
uECC_word_t uECC_vli_testBit(const uECC_word_t *vli, bitcount_t bit);

/*
 * @brief Computes result = product % mod, where product is 2N words long.
 * @param result OUT -- product % mod
 * @param mod IN -- module
 * @param num_words IN -- number of words
 * @warning Currently only designed to work for curve_p or curve_n.
 */
void uECC_vli_mmod(uECC_word_t *result, uECC_word_t *product,
		   const uECC_word_t *mod, wordcount_t num_words);

/*
 * @brief Computes modular product (using curve->mmod_fast)
 * @param result OUT -- (left * right) mod % curve_p
 * @param left IN -- left term in product
 * @param right IN -- right term in product
 * @param curve IN -- elliptic curve
 */
void uECC_vli_modMult_fast(uECC_word_t *result, const uECC_word_t *left,
			   const uECC_word_t *right, uECC_Curve curve);

/*
 * @brief Computes result = left - right.
 * @note Can modify in place.
 * @param result OUT -- left - right
 * @param left IN -- left term in subtraction
 * @param right IN -- right term in subtraction
 * @param num_words IN -- number of words
 * @return borrow
 */
uECC_word_t uECC_vli_sub(uECC_word_t *result, const uECC_word_t *left,
			 const uECC_word_t *right, wordcount_t num_words);

/*
 * @brief Constant-time comparison function(secure way to compare long ints)
 * @param left IN -- left term in comparison
 * @param right IN -- right term in comparison
 * @param num_words IN -- number of words
 * @return Returns 0 if left == right, 1 otherwise.
 */
uECC_word_t uECC_vli_equal(const uECC_word_t *left, const uECC_word_t *right,
			   wordcount_t num_words);

/*
 * @brief Computes (left * right) % mod
 * @param result OUT -- (left * right) % mod
 * @param left IN -- left term in product
 * @param right IN -- right term in product
 * @param mod IN -- mod
 * @param num_words IN -- number of words
 */
void uECC_vli_modMult(uECC_word_t *result, const uECC_word_t *left,
		      const uECC_word_t *right, const uECC_word_t *mod,
	              wordcount_t num_words);

/*
 * @brief Computes (1 / input) % mod
 * @note All VLIs are the same size.
 * @note See "Euclid's GCD to Montgomery Multiplication to the Great Divide"
 * @param result OUT -- (1 / input) % mod
 * @param input IN -- value to be modular inverted
 * @param mod IN -- mod
 * @param num_words -- number of words
 */
void uECC_vli_modInv(uECC_word_t *result, const uECC_word_t *input,
		     const uECC_word_t *mod, wordcount_t num_words);

/*
 * @brief Sets dest = src.
 * @param dest OUT -- destination buffer
 * @param src IN --  origin buffer
 * @param num_words IN -- number of words
 */
void uECC_vli_set(uECC_word_t *dest, const uECC_word_t *src,
		  wordcount_t num_words);

/*
 * @brief Computes (left + right) % mod.
 * @note Assumes that (left < mod) and right < mod), and that result does not
 * overlap mod.
 * @param result OUT -- (left + right) % mod.
 * @param left IN -- left term in addition
 * @param right IN -- right term in addition
 * @param mod IN -- mod
 * @param num_words IN -- number of words
 */
void uECC_vli_modAdd(uECC_word_t *result,  const uECC_word_t *left,
    		     const uECC_word_t *right, const uECC_word_t *mod,
   		     wordcount_t num_words);

/*
 * @brief Counts the number of bits required to represent vli.
 * @param vli IN -- very long integer
 * @param max_words IN -- number of words
 * @return number of bits in given vli
 */
bitcount_t uECC_vli_numBits(const uECC_word_t *vli,
			    const wordcount_t max_words);

/*
 * @brief Erases (set to 0) vli
 * @param vli IN -- very long integer
 * @param num_words IN -- number of words
 */
void uECC_vli_clear(uECC_word_t *vli, wordcount_t num_words);

/*
 * @brief check if it is a valid point in the curve
 * @param point IN -- point to be checked
 * @param curve IN -- elliptic curve
 * @return 0 if point is valid
 * @exception returns -1 if it is a point at infinity
 * @exception returns -2 if x or y is smaller than p,
 * @exception returns -3 if y^2 != x^3 + ax + b.
 */
int uECC_valid_point(const uECC_word_t *point, uECC_Curve curve);

/*
 * @brief Check if a public key is valid.
 * @param public_key IN -- The public key to be checked.
 * @return returns 0 if the public key is valid
 * @exception returns -1 if it is a point at infinity
 * @exception returns -2 if x or y is smaller than p,
 * @exception returns -3 if y^2 != x^3 + ax + b.
 * @exception returns -4 if public key is the group generator.
 *
 * @note Note that you are not required to check for a valid public key before
 * using any other uECC functions. However, you may wish to avoid spending CPU
 * time computing a shared secret or verifying a signature using an invalid
 * public key.
 */
int uECC_valid_public_key(const uint8_t *public_key, uECC_Curve curve);

 /*
  * @brief Converts an integer in uECC native format to big-endian bytes.
  * @param bytes OUT -- bytes representation
  * @param num_bytes IN -- number of bytes
  * @param native IN -- uECC native representation
  */
void uECC_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
    			    const unsigned int *native);

/*
 * @brief Converts big-endian bytes to an integer in uECC native format.
 * @param native OUT -- uECC native representation
 * @param bytes IN -- bytes representation
 * @param num_bytes IN -- number of bytes
 */
void uECC_vli_bytesToNative(unsigned int *native, const uint8_t *bytes,
			    int num_bytes);

#ifdef __cplusplus
}
#endif

#endif /* __TC_UECC_H__ */
