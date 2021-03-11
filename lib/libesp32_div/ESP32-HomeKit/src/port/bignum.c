/**
 * \brief  Multi-precision integer library, ESP-IDF hardware accelerated parts
 *
 *  based on mbedTLS implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  Additions Copyright (C) 2016-2020, Espressif Systems (Shanghai) PTE Ltd
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

//#include "soc/hwcrypto_periph.h"
#include "soc/hwcrypto_reg.h"
#include "driver/periph_ctrl.h"
#include <mbedtls/bignum.h>
#include "bignum_impl.h"
#include <sys/param.h>
#include <sys/lock.h>

static _lock_t mpi_lock;

/* Round up number of words to nearest
   512 bit (16 word) block count.
*/
size_t esp_mpi_hardware_words(size_t words)
{
    return (words + 0xF) & ~0xF;
}

void esp_mpi_enable_hardware_hw_op( void )
{
    /* newlib locks lazy initialize on ESP-IDF */
    _lock_acquire(&mpi_lock);

    /* Enable RSA hardware */
    periph_module_enable(PERIPH_RSA_MODULE);
    DPORT_REG_CLR_BIT(DPORT_RSA_PD_CTRL_REG, DPORT_RSA_PD);

    while (DPORT_REG_READ(RSA_CLEAN_REG) != 1)
    { }
    // Note: from enabling RSA clock to here takes about 1.3us
}

void esp_mpi_disable_hardware_hw_op( void )
{
    DPORT_REG_SET_BIT(DPORT_RSA_PD_CTRL_REG, DPORT_RSA_PD);

    /* Disable RSA hardware */
    periph_module_disable(PERIPH_RSA_MODULE);

    _lock_release(&mpi_lock);
}


/* Copy mbedTLS MPI bignum 'mpi' to hardware memory block at 'mem_base'.

   If hw_words is higher than the number of words in the bignum then
   these additional words will be zeroed in the memory buffer.

*/
static inline void mpi_to_mem_block(uint32_t mem_base, const mbedtls_mpi *mpi, size_t hw_words)
{
    uint32_t *pbase = (uint32_t *)mem_base;
    uint32_t copy_words = MIN(hw_words, mpi->n);

    /* Copy MPI data to memory block registers */
    for (int i = 0; i < copy_words; i++) {
        pbase[i] = mpi->p[i];
    }

    /* Zero any remaining memory block data */
    for (int i = copy_words; i < hw_words; i++) {
        pbase[i] = 0;
    }
}

/* Read mbedTLS MPI bignum back from hardware memory block.

   Reads num_words words from block.

   Bignum 'x' should already be grown to at least num_words by caller (can be done while
   calculation is in progress, to save some cycles)
*/
static inline void mem_block_to_mpi(mbedtls_mpi *x, uint32_t mem_base, int num_words)
{
    assert(x->n >= num_words);

    /* Copy data from memory block registers */
    esp_dport_access_read_buffer(x->p, mem_base, num_words);

    /* Zero any remaining limbs in the bignum, if the buffer is bigger
       than num_words */
    for (size_t i = num_words; i < x->n; i++) {
        x->p[i] = 0;
    }
}


/* Begin an RSA operation. op_reg specifies which 'START' register
   to write to.
*/
static inline void start_op(uint32_t op_reg)
{
    /* Clear interrupt status */
    DPORT_REG_WRITE(RSA_INTERRUPT_REG, 1);

    /* Note: above REG_WRITE includes a memw, so we know any writes
       to the memory blocks are also complete. */

    DPORT_REG_WRITE(op_reg, 1);
}

/* Wait for an RSA operation to complete.
*/
static inline void wait_op_complete(void)
{
    while (DPORT_REG_READ(RSA_INTERRUPT_REG) != 1)
    { }

    /* clear the interrupt */
    DPORT_REG_WRITE(RSA_INTERRUPT_REG, 1);
}

/* Read result from last MPI operation */
void esp_mpi_read_result_hw_op(mbedtls_mpi *Z, size_t z_words)
{
    wait_op_complete();
    mem_block_to_mpi(Z, RSA_MEM_Z_BLOCK_BASE, z_words);
}

/* Z = (X * Y) mod M */
void esp_mpi_mul_mpi_mod_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M, const mbedtls_mpi *Rinv, mbedtls_mpi_uint Mprime, size_t hw_words)
{
    /* Load M, X, Rinv, Mprime (Mprime is mod 2^32) */
    mpi_to_mem_block(RSA_MEM_M_BLOCK_BASE, M, hw_words);
    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, X, hw_words);
    mpi_to_mem_block(RSA_MEM_RB_BLOCK_BASE, Rinv, hw_words);
    DPORT_REG_WRITE(RSA_M_DASH_REG, (uint32_t)Mprime);

    /* "mode" register loaded with number of 512-bit blocks, minus 1 */
    DPORT_REG_WRITE(RSA_MULT_MODE_REG, (hw_words / 16) - 1);

    /* Execute first stage montgomery multiplication */
    start_op(RSA_MULT_START_REG);

    wait_op_complete();

    /* execute second stage */
    /* Load Y to X input memory block, rerun */
    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, Y, hw_words);

    start_op(RSA_MULT_START_REG);
}

/* Z = X * Y */
void esp_mpi_mul_mpi_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t hw_words)
{
    /* Copy X (right-extended) & Y (left-extended) to memory block */
    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, X, hw_words);
    mpi_to_mem_block(RSA_MEM_Z_BLOCK_BASE + hw_words * 4, Y, hw_words);
    /* NB: as Y is left-extended, we don't zero the bottom words_mult words of Y block.
       This is OK for now because zeroing is done by hardware when we do esp_mpi_acquire_hardware().
    */

    DPORT_REG_WRITE(RSA_M_DASH_REG, 0);

    /* "mode" register loaded with number of 512-bit blocks in result,
       plus 7 (for range 9-12). (this is ((N~ / 32) - 1) + 8))
    */
    DPORT_REG_WRITE(RSA_MULT_MODE_REG, ((hw_words * 2) / 16) + 7);

    start_op(RSA_MULT_START_REG);

}


int esp_mont_hw_op(mbedtls_mpi *Z, const mbedtls_mpi *X, const mbedtls_mpi *Y, const mbedtls_mpi *M,
                   mbedtls_mpi_uint Mprime,
                   size_t hw_words,
                   bool again)
{
    // Note Z may be the same pointer as X or Y
    int ret = 0;

    // montgomery mult prepare
    if (again == false) {
        mpi_to_mem_block(RSA_MEM_M_BLOCK_BASE, M, hw_words);
        DPORT_REG_WRITE(RSA_M_DASH_REG, Mprime);
        DPORT_REG_WRITE(RSA_MULT_MODE_REG, hw_words / 16 - 1);
    }

    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, X, hw_words);
    mpi_to_mem_block(RSA_MEM_RB_BLOCK_BASE, Y, hw_words);

    start_op(RSA_MULT_START_REG);
    Z->s = 1; // The sign of Z will be = M->s (but M->s is always 1)
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow(Z, hw_words) );

    wait_op_complete();

    /* Read back the result */
    mem_block_to_mpi(Z, RSA_MEM_Z_BLOCK_BASE, hw_words);


    /* from HAC 14.36 - 3. If Z >= M then Z = Z - M */
    if (mbedtls_mpi_cmp_mpi(Z, M) >= 0) {
        MBEDTLS_MPI_CHK(mbedtls_mpi_sub_mpi(Z, Z, M));
    }
cleanup:
    return ret;
}



/* Special-case of mbedtls_mpi_mult_mpi(), where we use hardware montgomery mod
   multiplication to calculate an mbedtls_mpi_mult_mpi result where either
   A or B are >2048 bits so can't use the standard multiplication method.

   Result (z_words, based on A bits + B bits) must still be less than 4096 bits.

   This case is simpler than the general case modulo multiply of
   esp_mpi_mul_mpi_mod() because we can control the other arguments:

   * Modulus is chosen with M=(2^num_bits - 1) (ie M=R-1), so output
   isn't actually modulo anything.
   * Mprime and Rinv are therefore predictable as follows:
   Mprime = 1
   Rinv = 1

   (See RSA Accelerator section in Technical Reference for more about Mprime, Rinv)
*/
void esp_mpi_mult_mpi_failover_mod_mult_hw_op(const mbedtls_mpi *X, const mbedtls_mpi *Y, size_t num_words)
{
    size_t hw_words = num_words;

    /* M = 2^num_words - 1, so block is entirely FF */
    for (int i = 0; i < hw_words; i++) {
        DPORT_REG_WRITE(RSA_MEM_M_BLOCK_BASE + i * 4, UINT32_MAX);
    }
    /* Mprime = 1 */
    DPORT_REG_WRITE(RSA_M_DASH_REG, 1);

    /* "mode" register loaded with number of 512-bit blocks, minus 1 */
    DPORT_REG_WRITE(RSA_MULT_MODE_REG, (hw_words / 16) - 1);

    /* Load X */
    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, X, hw_words);

    /* Rinv = 1, write first word */
    DPORT_REG_WRITE(RSA_MEM_RB_BLOCK_BASE, 1);

    /* Zero out rest of the Rinv words */
    for (int i = 1; i < hw_words; i++) {
        DPORT_REG_WRITE(RSA_MEM_RB_BLOCK_BASE + i * 4, 0);
    }

    start_op(RSA_MULT_START_REG);

    wait_op_complete();

    /* finish the modular multiplication */
    /* Load Y to X input memory block, rerun */
    mpi_to_mem_block(RSA_MEM_X_BLOCK_BASE, Y, hw_words);

    start_op(RSA_MULT_START_REG);

}
