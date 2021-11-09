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

#if CONFIG_IDF_TARGET_ESP32C3
#if __has_include("esp_idf_version.h")
#include "esp_idf_version.h"
#endif

#include <sys/param.h>
#include "soc/hwcrypto_periph.h"
#include "driver/periph_ctrl.h"
#include "mbedtls/bignum.h"
#include "bignum_impl.h"
#include "soc/system_reg.h"
#include "soc/periph_defs.h"
#include "esp_crypto_lock.h"

/* Copy mbedTLS MPI bignum 'mpi' to hardware memory block at 'mem_base'.

   If hw_words is higher than the number of words in the bignum then
   these additional words will be zeroed in the memory buffer.

*/
static inline void mpi_to_mem_block(uint32_t mem_base, const mbedtls_mpi *mpi, size_t num_words)
{
    uint32_t *pbase = (uint32_t *)mem_base;
    uint32_t copy_words = MIN(num_words, mpi->n);

    /* Copy MPI data to memory block registers */
    for (int i = 0; i < copy_words; i++) {
        pbase[i] = mpi->p[i];
    }

    /* Zero any remaining memory block data */
    for (int i = copy_words; i < num_words; i++) {
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

    /* Copy data from memory block registers */
    const size_t REG_WIDTH = sizeof(uint32_t);
    for (size_t i = 0; i < num_words; i++) {
        x->p[i] = REG_READ(mem_base + (i * REG_WIDTH));
    }
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
    REG_WRITE(RSA_CLEAR_INTERRUPT_REG, 1);

    /* Note: above REG_WRITE includes a memw, so we know any writes
       to the memory blocks are also complete. */

    REG_WRITE(op_reg, 1);
}

/* Wait for an RSA operation to complete.
*/
static inline void wait_op_complete(void)
{
    while (REG_READ(RSA_QUERY_INTERRUPT_REG) != 1)
    { }

    /* clear the interrupt */
    REG_WRITE(RSA_CLEAR_INTERRUPT_REG, 1);
}

#endif //CONFIG_IDF_TARGET_ESP32C3
