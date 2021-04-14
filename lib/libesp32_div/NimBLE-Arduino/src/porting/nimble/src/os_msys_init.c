/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include "os/os.h"
#include "mem/mem.h"
#include "esp_nimble_mem.h"

#if MYNEWT_VAL(MSYS_1_BLOCK_COUNT) > 0
#define SYSINIT_MSYS_1_MEMBLOCK_SIZE                \
    OS_ALIGN(MYNEWT_VAL(MSYS_1_BLOCK_SIZE), OS_ALIGNMENT)
#define SYSINIT_MSYS_1_MEMPOOL_SIZE                 \
    OS_MEMPOOL_SIZE(MYNEWT_VAL(MSYS_1_BLOCK_COUNT),  \
                    SYSINIT_MSYS_1_MEMBLOCK_SIZE)
static os_membuf_t *os_msys_init_1_data;
static struct os_mbuf_pool os_msys_init_1_mbuf_pool;
static struct os_mempool os_msys_init_1_mempool;
#endif

#if MYNEWT_VAL(MSYS_2_BLOCK_COUNT) > 0
#define SYSINIT_MSYS_2_MEMBLOCK_SIZE                \
    OS_ALIGN(MYNEWT_VAL(MSYS_2_BLOCK_SIZE), OS_ALIGNMENT)
#define SYSINIT_MSYS_2_MEMPOOL_SIZE                 \
    OS_MEMPOOL_SIZE(MYNEWT_VAL(MSYS_2_BLOCK_COUNT),  \
                    SYSINIT_MSYS_2_MEMBLOCK_SIZE)
static os_membuf_t *os_msys_init_2_data;
static struct os_mbuf_pool os_msys_init_2_mbuf_pool;
static struct os_mempool os_msys_init_2_mempool;
#endif

static void
os_msys_init_once(void *data, struct os_mempool *mempool,
                  struct os_mbuf_pool *mbuf_pool,
                  int block_count, int block_size, const char *name)
{
    int rc;

    rc = mem_init_mbuf_pool(data, mempool, mbuf_pool, block_count, block_size,
                            name);
    assert(rc == 0);

    rc = os_msys_register(mbuf_pool);
    assert(rc == 0);
}

int
os_msys_buf_alloc(void)
{
#if MYNEWT_VAL(MSYS_1_BLOCK_COUNT) > 0
    os_msys_init_1_data = (os_membuf_t *)nimble_platform_mem_calloc(1, (sizeof(os_membuf_t) * SYSINIT_MSYS_1_MEMPOOL_SIZE));
    if (!os_msys_init_1_data) {
        return -1;
    }
#endif

#if MYNEWT_VAL(MSYS_2_BLOCK_COUNT) > 0
    os_msys_init_2_data = (os_membuf_t *)nimble_platform_mem_calloc(1, (sizeof(os_membuf_t) * SYSINIT_MSYS_2_MEMPOOL_SIZE));
    if (!os_msys_init_2_data) {
        return -1;
    }
#endif

    return 0;
}

void
os_msys_buf_free(void)
{
#if MYNEWT_VAL(MSYS_1_BLOCK_COUNT) > 0
    nimble_platform_mem_free(os_msys_init_1_data);
    os_msys_init_1_data = NULL;
#endif

#if MYNEWT_VAL(MSYS_2_BLOCK_COUNT) > 0
    nimble_platform_mem_free(os_msys_init_2_data);
    os_msys_init_2_data = NULL;
#endif

}

void
os_msys_init(void)
{
    os_msys_reset();

    (void)os_msys_init_once;
#if MYNEWT_VAL(MSYS_1_BLOCK_COUNT) > 0
    os_msys_init_once(os_msys_init_1_data,
                      &os_msys_init_1_mempool,
                      &os_msys_init_1_mbuf_pool,
                      MYNEWT_VAL(MSYS_1_BLOCK_COUNT),
                      SYSINIT_MSYS_1_MEMBLOCK_SIZE,
                      "msys_1");
#endif

#if MYNEWT_VAL(MSYS_2_BLOCK_COUNT) > 0
    os_msys_init_once(os_msys_init_2_data,
                      &os_msys_init_2_mempool,
                      &os_msys_init_2_mbuf_pool,
                      MYNEWT_VAL(MSYS_2_BLOCK_COUNT),
                      SYSINIT_MSYS_2_MEMBLOCK_SIZE,
                      "msys_2");
#endif
}
