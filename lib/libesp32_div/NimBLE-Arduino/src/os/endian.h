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

#ifndef H_ENDIAN_
#define H_ENDIAN_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Internal helpers */
#ifndef os_bswap_64
#define os_bswap_64(x)   ((uint64_t)                \
     ((((x) & 0xff00000000000000ull) >> 56) |       \
      (((x) & 0x00ff000000000000ull) >> 40) |       \
      (((x) & 0x0000ff0000000000ull) >> 24) |       \
      (((x) & 0x000000ff00000000ull) >>  8) |       \
      (((x) & 0x00000000ff000000ull) <<  8) |       \
      (((x) & 0x0000000000ff0000ull) << 24) |       \
      (((x) & 0x000000000000ff00ull) << 40) |       \
      (((x) & 0x00000000000000ffull) << 56)))
#endif

#ifndef os_bswap_32
#define os_bswap_32(x)    ((uint32_t)               \
    ((((x) & 0xff000000) >> 24) |                   \
     (((x) & 0x00ff0000) >>  8) |                   \
     (((x) & 0x0000ff00) <<  8) |                   \
     (((x) & 0x000000ff) << 24)))
#endif

#ifndef os_bswap_16
#define os_bswap_16(x)   ((uint16_t)                \
    ((((x) & 0xff00) >> 8) |                        \
     (((x) & 0x00ff) << 8)))
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#ifndef ntohll
#define ntohll(x)  ((uint64_t)(x))
#endif

#ifndef htonll
#define htonll(x)  ((uint64_t)(x))
#endif

#ifndef ntohl
#define ntohl(x)   ((uint32_t)(x))
#endif

#ifndef htonl
#define htonl(x)   ((uint32_t)(x))
#endif

#ifndef ntohs
#define ntohs(x)   ((uint16_t)(x))
#endif

#ifndef htons
#define htons(x)   ((uint16_t)(x))
#endif

#ifndef htobe16
#define htobe16(x) ((uint16_t)(x))
#endif

#ifndef htole16
#define htole16(x) os_bswap_16 (x)
#endif

#ifndef be16toh
#define be16toh(x) ((uint16_t)(x))
#endif

#ifndef le16toh
#define le16toh(x) os_bswap_16 (x)
#endif

#ifndef htobe32
#define htobe32(x) ((uint32_t)(x))
#endif

#ifndef htole32
#define htole32(x) os_bswap_32 (x)
#endif

#ifndef be32toh
#define be32toh(x) ((uint32_t)(x))
#endif

#ifndef le32toh
#define le32toh(x) os_bswap_32 (x)
#endif

#ifndef htobe64
#define htobe64(x) ((uint64_t)(x))
#endif

#ifndef htole64
#define htole64(x) os_bswap_64 (x)
#endif

#ifndef be64toh
#define be64toh(x) ((uint64_t)(x))
#endif

#ifndef le64toh
#define le64toh(x) os_bswap_64 (x)
#endif

#else

#ifndef ntohll
#define ntohll(x)   os_bswap_64(x)
#endif

#ifndef htonll
#define htonll      ntohll
#endif

/* These are not used in NimBLE and ESP-IDF uses them from LwIP */
#if 0
#ifndef ntohl
#define ntohl(x)    os_bswap_32(x)
#endif

#ifndef htonl
#define htonl       ntohl
#endif

#ifndef htons
#define htons(x)    os_bswap_16(x)
#endif

#ifndef ntohs
#define ntohs       htons
#endif
#endif

#ifndef htobe16
#define htobe16(x) os_bswap_16(x)
#endif

#ifndef htole16
#define htole16(x) ((uint16_t)(x))
#endif

#ifndef be16toh
#define be16toh(x) os_bswap_16(x)
#endif

#ifndef le16toh
#define le16toh(x) ((uint16_t)(x))
#endif

#ifndef htobe32
#define htobe32(x) os_bswap_32(x)
#endif

#ifndef htole32
#define htole32(x) ((uint32_t)(x))
#endif

#ifndef be32toh
#define be32toh(x) os_bswap_32(x)
#endif

#ifndef le32toh
#define le32toh(x) ((uint32_t)(x))
#endif

#ifndef htobe64
#define htobe64(x) os_bswap64(x)
#endif

#ifndef htole64
#define htole64(x) ((uint64_t)(x))
#endif

#ifndef be64toh
#define be64toh(x) os_bswap64(x)
#endif

#ifndef le64toh
#define le64toh(x) ((uint64_t)(x))
#endif

#endif

void put_le16(void *buf, uint16_t x);
void put_le24(void *buf, uint32_t x);
void put_le32(void *buf, uint32_t x);
void put_le64(void *buf, uint64_t x);
uint16_t get_le16(const void *buf);
uint32_t get_le24(const void *buf);
uint32_t get_le32(const void *buf);
uint64_t get_le64(const void *buf);
void put_be16(void *buf, uint16_t x);
void put_be24(void *buf, uint32_t x);
void put_be32(void *buf, uint32_t x);
void put_be64(void *buf, uint64_t x);
uint16_t get_be16(const void *buf);
uint32_t get_be24(const void *buf);
uint32_t get_be32(const void *buf);
uint64_t get_be64(const void *buf);
void swap_in_place(void *buf, int len);
void swap_buf(uint8_t *dst, const uint8_t *src, int len);

#ifdef __cplusplus
}
#endif

#endif
