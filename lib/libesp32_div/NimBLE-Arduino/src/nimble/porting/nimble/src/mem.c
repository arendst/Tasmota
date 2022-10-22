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

#include <stdlib.h>
#include "../include/os/os.h"
#include "../include/mem/mem.h"

#ifdef ESP_PLATFORM
#include "nimble/esp_port/port/include/esp_nimble_mem.h"
#endif

/**
 * Generic mempool allocation function.  Used with basic and extended mempools.
 */
static int
mem_malloc_mempool_gen(uint16_t num_blocks, uint32_t block_size,
                       void **out_buf)
{
    block_size = OS_ALIGN(block_size, OS_ALIGNMENT);

    if (num_blocks > 0) {
#ifdef ESP_PLATFORM
        *out_buf = nimble_platform_mem_malloc(OS_MEMPOOL_BYTES(num_blocks, block_size));
#else
        *out_buf = malloc(OS_MEMPOOL_BYTES(num_blocks, block_size));
#endif
        if (*out_buf == NULL) {
            return OS_ENOMEM;
        }
    } else {
        *out_buf = NULL;
    }

    return 0;
}

/**
 * Mallocs a block of memory and initializes a mempool to use it.
 *
 * @param mempool               The mempool to initialize.
 * @param num_blocks            The total number of memory blocks in the
 *                                  mempool.
 * @param block_size            The size of each mempool entry.
 * @param name                  The name to give the mempool.
 * @param out_buf               On success, this points to the malloced memory.
 *                                  Pass NULL if you don't need this
 *                                  information.
 *
 * @return                      0 on success;
 *                              OS_ENOMEM on malloc failure;
 *                              Other OS code on unexpected error.
 */
int
mem_malloc_mempool(struct os_mempool *mempool, uint16_t num_blocks,
                   uint32_t block_size, char *name, void **out_buf)
{
    void *buf;
    int rc;

    rc = mem_malloc_mempool_gen(num_blocks, block_size, &buf);
    if (rc != 0) {
        return rc;
    }

    rc = os_mempool_init(mempool, num_blocks, block_size, buf, name);
    if (rc != 0) {
#ifdef ESP_PLATFORM
        nimble_platform_mem_free(buf);
#else
        free(buf);
#endif
        return rc;
    }

    if (out_buf != NULL) {
        *out_buf = buf;
    }

    return 0;
}

/**
 * Mallocs a block of memory and initializes an extended mempool to use it.
 *
 * @param mpe                   The extended mempool to initialize.
 * @param num_blocks            The total number of memory blocks in the
 *                                  mempool.
 * @param block_size            The size of each mempool entry.
 * @param name                  The name to give the mempool.
 * @param out_buf               On success, this points to the malloced memory.
 *                                  Pass NULL if you don't need this
 *                                  information.
 *
 * @return                      0 on success;
 *                              OS_ENOMEM on malloc failure;
 *                              Other OS code on unexpected error.
 */
int
mem_malloc_mempool_ext(struct os_mempool_ext *mpe, uint16_t num_blocks,
                       uint32_t block_size, char *name, void **out_buf)
{
    void *buf;
    int rc;

    rc = mem_malloc_mempool_gen(num_blocks, block_size, &buf);
    if (rc != 0) {
        return rc;
    }

    rc = os_mempool_ext_init(mpe, num_blocks, block_size, buf, name);
    if (rc != 0) {
#ifdef ESP_PLATFORM
        nimble_platform_mem_free(buf);
#else
        free(buf);
#endif
        return rc;
    }

    if (out_buf != NULL) {
        *out_buf = buf;
    }

    return 0;
}

/**
 * Mallocs a block of memory and initializes an mbuf pool to use it.  The
 * specified block_size indicates the size of an mbuf acquired from the pool if
 * it does not contain a pkthdr.
 *
 * @param mempool               The mempool to initialize.
 * @param mbuf_pool             The mbuf pool to initialize.
 * @param num_blocks            The total number of mbufs in the pool.
 * @param block_size            The size of each mbuf.
 * @param name                  The name to give the mempool.
 * @param out_buf               On success, this points to the malloced memory.
 *                                  Pass NULL if you don't need this
 *                                  information.
 *
 * @return                      0 on success;
 *                              OS_ENOMEM on malloc failure;
 *                              Other OS code on unexpected error.
 */
int
mem_malloc_mbuf_pool(struct os_mempool *mempool,
                     struct os_mbuf_pool *mbuf_pool, uint16_t num_blocks,
                     uint32_t block_size, char *name,
                     void **out_buf)
{
    void *buf;
    int rc;

    block_size = OS_ALIGN(block_size + sizeof (struct os_mbuf), OS_ALIGNMENT);

    rc = mem_malloc_mempool(mempool, num_blocks, block_size, name, &buf);
    if (rc != 0) {
        return rc;
    }

    rc = os_mbuf_pool_init(mbuf_pool, mempool, block_size, num_blocks);
    if (rc != 0) {
#ifdef ESP_PLATFORM
        nimble_platform_mem_free(buf);
#else
        free(buf);
#endif
        return rc;
    }

    if (out_buf != NULL) {
        *out_buf = buf;
    }

    return 0;
}

/**
 * Mallocs a block of memory and initializes an mbuf pool to use it.  The
 * specified block_size indicates the size of an mbuf acquired from the pool if
 * it contains a pkthdr.
 *
 * @param mempool               The mempool to initialize.
 * @param mbuf_pool             The mbuf pool to initialize.
 * @param num_blocks            The total number of mbufs in the pool.
 * @param block_size            The size of each mbuf.
 * @param name                  The name to give the mempool.
 * @param out_buf               On success, this points to the malloced memory.
 *                                  Pass NULL if you don't need this
 *                                  information.
 *
 * @return                      0 on success;
 *                              OS_ENOMEM on malloc failure;
 *                              Other OS code on unexpected error.
 */
int
mem_malloc_mbufpkt_pool(struct os_mempool *mempool,
                        struct os_mbuf_pool *mbuf_pool, int num_blocks,
                        int block_size, char *name,
                        void **out_buf)
{
    int rc;

    rc = mem_malloc_mbuf_pool(mempool, mbuf_pool, num_blocks,
                              block_size + sizeof (struct os_mbuf_pkthdr),
                              name, out_buf);
    return rc;
}

int
mem_init_mbuf_pool(void *mem, struct os_mempool *mempool,
                   struct os_mbuf_pool *mbuf_pool, int num_blocks,
                   int block_size, const char *name)
{
    int rc;

    rc = os_mempool_init(mempool, num_blocks, block_size, mem, name);
    if (rc != 0) {
        return rc;
    }

    rc = os_mbuf_pool_init(mbuf_pool, mempool, block_size, num_blocks);
    if (rc != 0) {
        return rc;
    }

    return 0;
}

/*
 * Splits an appropriately-sized fragment from the front of an mbuf chain, as
 * neeeded.  If the length of the mbuf chain greater than specified maximum
 * fragment size, a new mbuf is allocated, and data is moved from the source
 * mbuf to the new mbuf.  If the mbuf chain is small enough to fit in a single
 * fragment, the source mbuf itself is returned unmodified, and the suplied
 * pointer is set to NULL.
 *
 * This function is expected to be called in a loop until the entire mbuf chain
 * has been consumed.  For example:
 *
 *     struct os_mbuf *frag;
 *     struct os_mbuf *rsp;
 *     // [...]
 *     while (rsp != NULL) {
 *         frag = mem_split_frag(&rsp, get_mtu(), frag_alloc, NULL);
 *         if (frag == NULL) {
 *             os_mbuf_free_chain(rsp);
 *             return OS_ENOMEM;
 *         }
 *         send_packet(frag)
 *     }
 *
 * @param om                    The packet to fragment.  Upon fragmentation,
 *                                  this mbuf is adjusted such that the
 *                                  fragment data is removed.  If the packet
 *                                  constitutes a single fragment, this gets
 *                                  set to NULL on success.
 * @param max_frag_sz           The maximum payload size of a fragment.
 *                                  Typically this is the MTU of the
 *                                  connection.
 * @param alloc_cb              Points to a function that allocates an mbuf to
 *                                  hold a fragment.  This function gets called
 *                                  before the source mbuf chain is modified,
 *                                  so it can safely inspect it.
 * @param cb_arg                Generic parameter that gets passed to the
 *                                  callback function.
 *
 * @return                      The next fragment to send on success;
 *                              NULL on failure.
 */
struct os_mbuf *
mem_split_frag(struct os_mbuf **om, uint16_t max_frag_sz,
               mem_frag_alloc_fn *alloc_cb, void *cb_arg)
{
    struct os_mbuf *frag;
    int rc;

    if (OS_MBUF_PKTLEN(*om) <= max_frag_sz) {
        /* Final fragment. */
        frag = *om;
        *om = NULL;
        return frag;
    }

    /* Packet needs to be split.  Allocate a new buffer for the fragment. */
    frag = alloc_cb(max_frag_sz, cb_arg);
    if (frag == NULL) {
        goto err;
    }

    /* Move data from the front of the packet into the fragment mbuf. */
    rc = os_mbuf_appendfrom(frag, *om, 0, max_frag_sz);
    if (rc != 0) {
        goto err;
    }
    os_mbuf_adj(*om, max_frag_sz);

    /* Free unused portion of of source mbuf chain, if possible. */
    *om = os_mbuf_trim_front(*om);

    return frag;

err:
    os_mbuf_free_chain(frag);
    return NULL;
}

/**
 * Applies a pullup operation to the supplied mbuf and returns a pointer to the
 * start of the mbuf data.  This is simply a convenience function which allows
 * the user to access the mbuf data without a cast.  On failure, the provided
 * mbuf is freed.
 *
 * @param om                    The mbuf to pull up.
 * @param len                   The size of the object to pull up.
 *
 * @return                      The start of the pulled-up mbuf data.
 */
void *
mem_pullup_obj(struct os_mbuf **om, uint16_t len)
{
    *om = os_mbuf_pullup(*om, len);
    if (*om == NULL) {
        return NULL;
    }

    return (*om)->om_data;

}
