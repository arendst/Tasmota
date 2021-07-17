/**
 * \brief  Multi-precision integer library, ESP32 hardware accelerated parts
 *
 *  based on mbedTLS implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Additions Copyright (C) 2016, Espressif Systems (Shanghai) PTE Ltd
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */
#if __has_include("esp_idf_version.h")
#include "esp_idf_version.h"
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
#warning("IDF is 4 or later")
#include "soc/hwcrypto_periph.h"
#endif
#endif

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/param.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "bignum_impl.h"
//#include "soc/soc_caps.h"

#include <mbedtls/bignum.h>

// wangbin added
#if !defined(SOC_RSA_MAX_BIT_LEN)
#define SOC_RSA_MAX_BIT_LEN    (4096)
#endif


/* Some implementation notes:
 *
 * - Naming convention x_words, y_words, z_words for number of words (limbs) used in a particular
 *   bignum. This number may be less than the size of the bignum
 *
 * - Naming convention hw_words for the hardware length of the operation. This number maybe be rounded up
 *   for targets that requres this (e.g. ESP32), and may be larger than any of the numbers
 *   involved in the calculation.
 *
 * - Timing behaviour of these functions will depend on the length of the inputs. This is fundamentally
 *   the same constraint as the software mbedTLS implementations, and relies on the same
 *   countermeasures (exponent blinding, etc) which are used in mbedTLS.
 */

static const __attribute__((unused)) char *TAG = "bignum";

#define ciL    (sizeof(mbedtls_mpi_uint))         /* chars in limb  */
#define biL    (ciL << 3)                         /* bits  in limb  */


/* Convert bit count to word count
 */
static inline size_t bits_to_words(size_t bits)
{
    return (bits + 31) / 32;
}

/* Return the number of words actually used to represent an mpi
   number.
*/
#if defined(MBEDTLS_MPI_EXP_MOD_ALT)
static size_t mpi_words(const mbedtls_mpi *mpi)
{
    for (size_t i = mpi->n; i > 0; i--) {
        if (mpi->p[i - 1] != 0) {
            return i;
        }
    }
    return 0;
}

#endif //MBEDTLS_MPI_EXP_MOD_ALT

/**
 *
 * There is a need for the value of integer N' such that B^-1(B-1)-N^-1N'=1,
 * where B^-1(B-1) mod N=1. Actually, only the least significant part of
 * N' is needed, hence the definition N0'=N' mod b. We reproduce below the
 * simple algorithm from an article by Dusse and Kaliski to efficiently
 * find N0' from N0 and b
 */
static mbedtls_mpi_uint modular_inverse(const mbedtls_mpi *M)
{
    int i;
    uint64_t t = 1;
    uint64_t two_2_i_minus_1 = 2;   /* 2^(i-1) */
    uint64_t two_2_i = 4;           /* 2^i */
    uint64_t N = M->p[0];

    for (i = 2; i <= 32; i++) {
        if ((mbedtls_mpi_uint) N * t % two_2_i >= two_2_i_minus_1) {
            t += two_2_i_minus_1;
        }

        two_2_i_minus_1 <<= 1;
        two_2_i <<= 1;
    }

    return (mbedtls_mpi_uint)(UINT32_MAX - t + 1);
}

/* Calculate Rinv = RR^2 mod M, where:
 *
 *  R = b^n where b = 2^32, n=num_words,
 *  R = 2^N (where N=num_bits)
 *  RR = R^2 = 2^(2*N) (where N=num_bits=num_words*32)
 *
 * This calculation is computationally expensive (mbedtls_mpi_mod_mpi)
 * so caller should cache the result where possible.
 *
 * DO NOT call this function while holding esp_mpi_enable_hardware_hw_op().
 *
 */
static int calculate_rinv(mbedtls_mpi *Rinv, const mbedtls_mpi *M, int num_words)
{
    int ret;
    size_t num_bits = num_words * 32;
    mbedtls_mpi RR;
    mbedtls_mpi_init(&RR);
    MBEDTLS_MPI_CHK(mbedtls_mpi_set_bit(&RR, num_bits * 2, 1));
    MBEDTLS_MPI_CHK(mbedtls_mpi_mod_mpi(Rinv, &RR, M));

cleanup:
    mbedtls_mpi_free(&RR);

    return ret;
}






/* Z = (X * Y) mod M

   Not an mbedTLS function
*/
int esp_mpi_mul_mpi_mod(mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M)
{
    int ret = 0;

    size_t x_bits = mbedtls_mpi_bitlen(X);
    size_t y_bits = mbedtls_mpi_bitlen(Y);
    size_t m_bits = mbedtls_mpi_bitlen(M);
    size_t z_bits = MIN(m_bits, x_bits + y_bits);
    size_t x_words = bits_to_words(x_bits);
    size_t y_words = bits_to_words(y_bits);
    size_t m_words = bits_to_words(m_bits);
    size_t z_words = bits_to_words(z_bits);
    size_t hw_words = esp_mpi_hardware_words(MAX(x_words, MAX(y_words, m_words))); /* longest operand */
    mbedtls_mpi Rinv;
    mbedtls_mpi_uint Mprime;

    /* Calculate and load the first stage montgomery multiplication */
    mbedtls_mpi_init(&Rinv);
    MBEDTLS_MPI_CHK(calculate_rinv(&Rinv, M, hw_words));
    Mprime = modular_inverse(M);

    esp_mpi_enable_hardware_hw_op();
    /* Load and start a (X * Y) mod M calculation */
    esp_mpi_mul_mpi_mod_hw_op(X, Y, M, &Rinv, Mprime, hw_words);

    MBEDTLS_MPI_CHK(mbedtls_mpi_grow(Z, z_words));

    esp_mpi_read_result_hw_op(Z, z_words);
    Z->s = X->s * Y->s;

cleanup:
    mbedtls_mpi_free(&Rinv);
    esp_mpi_disable_hardware_hw_op();

    return ret;
}

#if defined(MBEDTLS_MPI_EXP_MOD_ALT)

#ifdef ESP_MPI_USE_MONT_EXP
/*
 * Return the most significant one-bit.
 */
static size_t mbedtls_mpi_msb( const mbedtls_mpi *X )
{
    int i, j;
    if (X != NULL && X->n != 0) {
        for (i = X->n - 1; i >= 0; i--) {
            if (X->p[i] != 0) {
                for (j = biL - 1; j >= 0; j--) {
                    if ((X->p[i] & (1 << j)) != 0) {
                        return (i * biL) + j;
                    }
                }
            }
        }
    }
    return 0;
}

/*
 * Montgomery exponentiation: Z = X ^ Y mod M  (HAC 14.94)
 */
static int mpi_montgomery_exp_calc( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M,
                                    mbedtls_mpi *Rinv,
                                    size_t hw_words,
                                    mbedtls_mpi_uint Mprime )
{
    int ret = 0;
    mbedtls_mpi X_, one;

    mbedtls_mpi_init(&X_);
    mbedtls_mpi_init(&one);
    if ( ( ( ret = mbedtls_mpi_grow(&one, hw_words) ) != 0 ) ||
            ( ( ret = mbedtls_mpi_set_bit(&one, 0, 1) )  != 0 ) ) {
        goto cleanup2;
    }

    // Algorithm from HAC 14.94
    {
        // 0 determine t (highest bit set in y)
        int t = mbedtls_mpi_msb(Y);

        esp_mpi_enable_hardware_hw_op();

        // 1.1 x_ = mont(x, R^2 mod m)
        //        = mont(x, rb)
        MBEDTLS_MPI_CHK( esp_mont_hw_op(&X_, X, Rinv, M, Mprime, hw_words, false) );

        // 1.2 z = R mod m
        // now z = R mod m = Mont (R^2 mod m, 1) mod M (as Mont(x) = X&R^-1 mod M)
        MBEDTLS_MPI_CHK( esp_mont_hw_op(Z, Rinv, &one, M, Mprime, hw_words, true) );

        // 2 for i from t down to 0
        for (int i = t; i >= 0; i--) {
            // 2.1 z = mont(z,z)
            if (i != t) { // skip on the first iteration as is still unity
                MBEDTLS_MPI_CHK( esp_mont_hw_op(Z, Z, Z, M, Mprime, hw_words, true) );
            }

            // 2.2 if y[i] = 1 then z = mont(A, x_)
            if (mbedtls_mpi_get_bit(Y, i)) {
                MBEDTLS_MPI_CHK( esp_mont_hw_op(Z, Z, &X_, M, Mprime, hw_words, true) );
            }
        }

        // 3 z = Mont(z, 1)
        MBEDTLS_MPI_CHK( esp_mont_hw_op(Z, Z, &one, M, Mprime, hw_words, true) );
    }

cleanup:
    esp_mpi_disable_hardware_hw_op();

cleanup2:
    mbedtls_mpi_free(&X_);
    mbedtls_mpi_free(&one);
    return ret;
}

#endif //USE_MONT_EXPONENATIATION

/*
 * Z = X ^ Y mod M
 *
 * _Rinv is optional pre-calculated version of Rinv (via calculate_rinv()).
 *
 * (See RSA Accelerator section in Technical Reference for more about Mprime, Rinv)
 *
 */
// wangbin changed mbedtls_mpi_exp_mod -> esp_mpi_exp_mod
int esp_mpi_exp_mod( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M, mbedtls_mpi *_Rinv )
{
    int ret = 0;
    size_t x_words = mpi_words(X);
    size_t y_words = mpi_words(Y);
    size_t m_words = mpi_words(M);


    /* "all numbers must be the same length", so choose longest number
       as cardinal length of operation...
    */
    size_t num_words = esp_mpi_hardware_words(MAX(m_words, MAX(x_words, y_words)));

    mbedtls_mpi Rinv_new; /* used if _Rinv == NULL */
    mbedtls_mpi *Rinv;    /* points to _Rinv (if not NULL) othwerwise &RR_new */
    mbedtls_mpi_uint Mprime;

    if (mbedtls_mpi_cmp_int(M, 0) <= 0 || (M->p[0] & 1) == 0) {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    if (mbedtls_mpi_cmp_int(Y, 0) < 0) {
        return MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    }

    if (mbedtls_mpi_cmp_int(Y, 0) == 0) {
        return mbedtls_mpi_lset(Z, 1);
    }

    if (num_words * 32 > SOC_RSA_MAX_BIT_LEN) {
        return MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
    }

    /* Determine RR pointer, either _RR for cached value
       or local RR_new */
    if (_Rinv == NULL) {
        mbedtls_mpi_init(&Rinv_new);
        Rinv = &Rinv_new;
    } else {
        Rinv = _Rinv;
    }
    if (Rinv->p == NULL) {
        MBEDTLS_MPI_CHK(calculate_rinv(Rinv, M, num_words));
    }

    Mprime = modular_inverse(M);

    // Montgomery exponentiation: Z = X ^ Y mod M  (HAC 14.94)
#ifdef ESP_MPI_USE_MONT_EXP
    ret = mpi_montgomery_exp_calc(Z, X, Y, M, Rinv, num_words, Mprime) ;
    MBEDTLS_MPI_CHK(ret);
#else
    esp_mpi_enable_hardware_hw_op();

    esp_mpi_exp_mpi_mod_hw_op(X, Y, M, Rinv, Mprime, num_words);
    ret = mbedtls_mpi_grow(Z, m_words);
    if (ret != 0) {
        esp_mpi_disable_hardware_hw_op();
        goto cleanup;
    }
    esp_mpi_read_result_hw_op(Z, m_words);
    esp_mpi_disable_hardware_hw_op();
#endif

    // Compensate for negative X
    if (X->s == -1 && (Y->p[0] & 1) != 0) {
        Z->s = -1;
        MBEDTLS_MPI_CHK(mbedtls_mpi_add_mpi(Z, M, Z));
    } else {
        Z->s = 1;
    }

cleanup:
    if (_Rinv == NULL) {
        mbedtls_mpi_free(&Rinv_new);
    }
    return ret;
}

#endif /* MBEDTLS_MPI_EXP_MOD_ALT */



#if defined(MBEDTLS_MPI_MUL_MPI_ALT) /* MBEDTLS_MPI_MUL_MPI_ALT */

static int mpi_mult_mpi_failover_mod_mult( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t z_words);
static int mpi_mult_mpi_overlong(mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t y_words, size_t z_words);

/* Z = X * Y */
int mbedtls_mpi_mul_mpi( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y )
{
    int ret = 0;
    size_t x_bits = mbedtls_mpi_bitlen(X);
    size_t y_bits = mbedtls_mpi_bitlen(Y);
    size_t x_words = bits_to_words(x_bits);
    size_t y_words = bits_to_words(y_bits);
    size_t z_words = bits_to_words(x_bits + y_bits);
    size_t hw_words = esp_mpi_hardware_words(MAX(x_words, y_words)); // length of one operand in hardware

    /* Short-circuit eval if either argument is 0 or 1.

       This is needed as the mpi modular division
       argument will sometimes call in here when one
       argument is too large for the hardware unit, but the other
       argument is zero or one.
    */
    if (x_bits == 0 || y_bits == 0) {
        mbedtls_mpi_lset(Z, 0);
        return 0;
    }
    if (x_bits == 1) {
        ret = mbedtls_mpi_copy(Z, Y);
        Z->s *= X->s;
        return ret;
    }
    if (y_bits == 1) {
        ret = mbedtls_mpi_copy(Z, X);
        Z->s *= Y->s;
        return ret;
    }

    /* Grow Z to result size early, avoid interim allocations */
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow(Z, z_words) );

    /* If either factor is over 2048 bits, we can't use the standard hardware multiplier
       (it assumes result is double longest factor, and result is max 4096 bits.)

       However, we can fail over to mod_mult for up to 4096 bits of result (modulo
       multiplication doesn't have the same restriction, so result is simply the
       number of bits in X plus number of bits in in Y.)
    */
    if (hw_words * 32 > SOC_RSA_MAX_BIT_LEN/2) {
        if (z_words * 32 <= SOC_RSA_MAX_BIT_LEN) {
            /* Note: it's possible to use mpi_mult_mpi_overlong
               for this case as well, but it's very slightly
               slower and requires a memory allocation.
            */
            return mpi_mult_mpi_failover_mod_mult(Z, X, Y, z_words);
        } else {
            /* Still too long for the hardware unit... */
            if (y_words > x_words) {
                return mpi_mult_mpi_overlong(Z, X, Y, y_words, z_words);
            } else {
                return mpi_mult_mpi_overlong(Z, Y, X, x_words, z_words);
            }
        }
    }

    /* Otherwise, we can use the (faster) multiply hardware unit */
    esp_mpi_enable_hardware_hw_op();

    esp_mpi_mul_mpi_hw_op(X, Y, hw_words);
    esp_mpi_read_result_hw_op(Z, z_words);

    esp_mpi_disable_hardware_hw_op();

    Z->s = X->s * Y->s;

cleanup:
    return ret;
}



/* Deal with the case when X & Y are too long for the hardware unit, by splitting one operand
   into two halves.

   Y must be the longer operand

   Slice Y into Yp, Ypp such that:
   Yp = lower 'b' bits of Y
   Ypp = upper 'b' bits of Y (right shifted)

   Such that
   Z = X * Y
   Z = X * (Yp + Ypp<<b)
   Z = (X * Yp) + (X * Ypp<<b)

   Note that this function may recurse multiple times, if both X & Y
   are too long for the hardware multiplication unit.
*/
static int mpi_mult_mpi_overlong(mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t y_words, size_t z_words)
{
    int ret = 0;
    mbedtls_mpi Ztemp;
    /* Rather than slicing in two on bits we slice on limbs (32 bit words) */
    const size_t words_slice = y_words / 2;
    /* Yp holds lower bits of Y (declared to reuse Y's array contents to save on copying) */
    const mbedtls_mpi Yp = {
        .p = Y->p,
        .n = words_slice,
        .s = Y->s
    };
    /* Ypp holds upper bits of Y, right shifted (also reuses Y's array contents) */
    const mbedtls_mpi Ypp = {
        .p = Y->p + words_slice,
        .n = y_words - words_slice,
        .s = Y->s
    };
    mbedtls_mpi_init(&Ztemp);

    /* Get result Ztemp = Yp * X (need temporary variable Ztemp) */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi(&Ztemp, X, &Yp) );

    /* Z = Ypp * Y */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi(Z, X, &Ypp) );

    /* Z = Z << b */
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l(Z, words_slice * 32) );

    /* Z += Ztemp */
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi(Z, Z, &Ztemp) );

cleanup:
    mbedtls_mpi_free(&Ztemp);

    return ret;
}

/* Special-case of mbedtls_mpi_mult_mpi(), where we use hardware montgomery mod
   multiplication to calculate an mbedtls_mpi_mult_mpi result where either
   A or B are >2048 bits so can't use the standard multiplication method.

   Result (number of words, based on A bits + B bits) must still be less than 4096 bits.

   This case is simpler than the general case modulo multiply of
   esp_mpi_mul_mpi_mod() because we can control the other arguments:

   * Modulus is chosen with M=(2^num_bits - 1) (ie M=R-1), so output
   * Mprime and Rinv are therefore predictable as follows:
   isn't actually modulo anything.
   Mprime 1
   Rinv 1

   (See RSA Accelerator section in Technical Reference for more about Mprime, Rinv)
*/

static int mpi_mult_mpi_failover_mod_mult( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t z_words)
{
    int ret;
    size_t hw_words = esp_mpi_hardware_words(z_words);

    esp_mpi_enable_hardware_hw_op();

    esp_mpi_mult_mpi_failover_mod_mult_hw_op(X, Y, hw_words );
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow(Z, hw_words) );
    esp_mpi_read_result_hw_op(Z, hw_words);

    Z->s = X->s * Y->s;
cleanup:
    esp_mpi_disable_hardware_hw_op();
    return ret;
}

#endif /* MBEDTLS_MPI_MUL_MPI_ALT */
