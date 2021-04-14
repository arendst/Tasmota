#ifndef _ESP_BIGNUM_H_
#define _ESP_BIGNUM_H_

#include <mbedtls/bignum.h>
#include <stdbool.h>

/* Use montgomery exponentiation (HAC 14.94) for calculating X ^ Y mod M,
   this may be faster for some targets. The hardware acceleration support for modular
   exponentiation on the ESP32 is slow for public key operations, so use montgomery
   exponentiation instead.
*/

#define CONFIG_IDF_TARGET_ESP32  1

#if CONFIG_IDF_TARGET_ESP32
#define ESP_MPI_USE_MONT_EXP

#define MBEDTLS_MPI_EXP_MOD_ALT
//#define MBEDTLS_MPI_MUL_MPI_ALT
#endif


int esp_mpi_exp_mod( mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M, mbedtls_mpi *_Rinv );

/**
 * @brief Enable the MPI hardware and acquire the lock
 *
 */
void esp_mpi_enable_hardware_hw_op( void );

/**
 * @brief Disable the MPI hardware and release the lock
 *
 */
void esp_mpi_disable_hardware_hw_op( void );

/**
 * @brief Calculate the number of words needed to represent the input word in hardware
 *
 * @param words The number of words to be represented
 *
 * @return size_t Number of words required
 */
size_t esp_mpi_hardware_words(size_t words);

/**
 * @brief Starts a (X * Y) Mod M calculation in hardware. Rinv and M_prime needs to be precalculated in software.
 *
 */
void esp_mpi_mul_mpi_mod_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M, const mbedtls_mpi *Rinv, mbedtls_mpi_uint Mprime, size_t hw_words);

/**
 * @brief Starts a (X * Y) calculation in hardware.
 *
 */
void esp_mpi_mul_mpi_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t num_words);

/**
 * @brief Special-case of (X * Y), where we use hardware montgomery mod
   multiplication to calculate result where either A or B are >2048 bits so
   can't use the standard multiplication method.
 *
 */
void esp_mpi_mult_mpi_failover_mod_mult_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t num_words);

/**
 * @brief Read out the result from the previous calculation.
 *
 */
void esp_mpi_read_result_hw_op(mbedtls_mpi *Z, size_t z_words);

#ifdef ESP_MPI_USE_MONT_EXP
/**
 * @brief Starts a montgomery multiplication calculation in hardware
 *
 */
int esp_mont_hw_op(mbedtls_mpi* Z, const mbedtls_mpi* X, const mbedtls_mpi* Y, const mbedtls_mpi* M,
                mbedtls_mpi_uint Mprime,
                size_t hw_words,
                bool again);

#else

/**
 * @brief Starts a (X ^ Y) Mod M calculation in hardware. Rinv and M_prime needs to be precalculated in software.
 *
 */
void esp_mpi_exp_mpi_mod_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M, const mbedtls_mpi *Rinv, mbedtls_mpi_uint Mprime, size_t hw_words);

#endif //ESP_MPI_USE_MONT_EXP

#endif
