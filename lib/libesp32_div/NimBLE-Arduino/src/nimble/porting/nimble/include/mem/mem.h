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

#ifndef H_UTIL_MEM_
#define H_UTIL_MEM_

#ifdef __cplusplus
extern "C" {
#endif

struct os_mempool;
struct os_mbuf_pool;


#if SOC_ESP_NIMBLE_CONTROLLER
int r_mem_malloc_mempool(struct os_mempool *mempool, uint16_t num_blocks,
                       uint32_t block_size, char *name, void **out_buf);
#define mem_malloc_mempool r_mem_malloc_mempool

int r_mem_malloc_mempool_ext(struct os_mempool_ext *mempool, uint16_t num_blocks,
                           uint32_t block_size, char *name, void **out_buf);
#define mem_malloc_mempool_ext r_mem_malloc_mempool_ext


int r_mem_malloc_mbuf_pool(struct os_mempool *mempool,
                         struct os_mbuf_pool *mbuf_pool, uint16_t num_blocks,
                         uint32_t block_size, char *name,
                         void **out_buf);
#define mem_malloc_mbuf_pool r_mem_malloc_mbuf_pool

int r_mem_malloc_mbufpkt_pool(struct os_mempool *mempool,
                            struct os_mbuf_pool *mbuf_pool, int num_blocks,
                            int block_size, char *name,
                            void **out_buf);
#define mem_malloc_mbufpkt_pool r_mem_malloc_mbufpkt_pool

int r_mem_init_mbuf_pool(void *mem, struct os_mempool *mempool,
                       struct os_mbuf_pool *mbuf_pool, int num_blocks,
                       int block_size, const char *name);
#define mem_init_mbuf_pool r_mem_init_mbuf_pool
#else
int mem_malloc_mempool(struct os_mempool *mempool, uint16_t num_blocks,
                       uint32_t block_size, char *name, void **out_buf);
int mem_malloc_mempool_ext(struct os_mempool_ext *mempool, uint16_t num_blocks,
                           uint32_t block_size, char *name, void **out_buf);

int mem_malloc_mbuf_pool(struct os_mempool *mempool,
                         struct os_mbuf_pool *mbuf_pool, uint16_t num_blocks,
                         uint32_t block_size, char *name,
                         void **out_buf);
int mem_malloc_mbufpkt_pool(struct os_mempool *mempool,
                            struct os_mbuf_pool *mbuf_pool, int num_blocks,
                            int block_size, char *name,
                            void **out_buf);
int mem_init_mbuf_pool(void *mem, struct os_mempool *mempool,
                       struct os_mbuf_pool *mbuf_pool, int num_blocks,
                       int block_size, const char *name);
#endif

/**
 * Specifies a function used as a callback.  Functions of this type allocate an
 * mbuf chain meant to hold a packet fragment.  The resulting mbuf must contain
 * a pkthdr.
 *
 * @param frag_size             The number of data bytes that the mbuf will
 *                                  eventually contain.
 * @param arg                   A generic parameter.
 *
 * @return                      An allocated mbuf chain on success;
 *                              NULL on failure.
 */
typedef struct os_mbuf *mem_frag_alloc_fn(uint16_t frag_size, void *arg);


#if SOC_ESP_NIMBLE_CONTROLLER
struct os_mbuf *r_mem_split_frag(struct os_mbuf **om, uint16_t max_frag_sz,
                               mem_frag_alloc_fn *alloc_cb, void *cb_arg);
#define mem_split_frag r_mem_split_frag
void *r_mem_pullup_obj(struct os_mbuf **om, uint16_t len);
#define mem_pullup_obj r_mem_pullup_obj
#else
struct os_mbuf *mem_split_frag(struct os_mbuf **om, uint16_t max_frag_sz,
                               mem_frag_alloc_fn *alloc_cb, void *cb_arg);

void *mem_pullup_obj(struct os_mbuf **om, uint16_t len);
#endif
#ifdef __cplusplus
}
#endif

#endif
