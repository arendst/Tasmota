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

#include "os/endian.h"

void
put_le16(void *buf, uint16_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)x;
    u8ptr[1] = (uint8_t)(x >> 8);
}

void
put_le24(void *buf, uint32_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)x;
    u8ptr[1] = (uint8_t)(x >> 8);
    u8ptr[2] = (uint8_t)(x >> 16);
}

void
put_le32(void *buf, uint32_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)x;
    u8ptr[1] = (uint8_t)(x >> 8);
    u8ptr[2] = (uint8_t)(x >> 16);
    u8ptr[3] = (uint8_t)(x >> 24);
}

void
put_le64(void *buf, uint64_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)x;
    u8ptr[1] = (uint8_t)(x >> 8);
    u8ptr[2] = (uint8_t)(x >> 16);
    u8ptr[3] = (uint8_t)(x >> 24);
    u8ptr[4] = (uint8_t)(x >> 32);
    u8ptr[5] = (uint8_t)(x >> 40);
    u8ptr[6] = (uint8_t)(x >> 48);
    u8ptr[7] = (uint8_t)(x >> 56);
}

uint16_t
get_le16(const void *buf)
{
    const uint8_t *u8ptr;
    uint16_t x;

    u8ptr = buf;
    x = u8ptr[0];
    x |= (uint16_t)u8ptr[1] << 8;

    return x;
}

uint32_t
get_le24(const void *buf)
{
    const uint8_t *u8ptr;
    uint32_t x;

    u8ptr = buf;
    x = u8ptr[0];
    x |= (uint32_t)u8ptr[1] << 8;
    x |= (uint32_t)u8ptr[2] << 16;

    return x;
}

uint32_t
get_le32(const void *buf)
{
    const uint8_t *u8ptr;
    uint32_t x;

    u8ptr = buf;
    x = u8ptr[0];
    x |= (uint32_t)u8ptr[1] << 8;
    x |= (uint32_t)u8ptr[2] << 16;
    x |= (uint32_t)u8ptr[3] << 24;

    return x;
}

uint64_t
get_le64(const void *buf)
{
    const uint8_t *u8ptr;
    uint64_t x;

    u8ptr = buf;
    x = u8ptr[0];
    x |= (uint64_t)u8ptr[1] << 8;
    x |= (uint64_t)u8ptr[2] << 16;
    x |= (uint64_t)u8ptr[3] << 24;
    x |= (uint64_t)u8ptr[4] << 32;
    x |= (uint64_t)u8ptr[5] << 40;
    x |= (uint64_t)u8ptr[6] << 48;
    x |= (uint64_t)u8ptr[7] << 56;

    return x;
}

void
put_be16(void *buf, uint16_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)(x >> 8);
    u8ptr[1] = (uint8_t)x;
}

void
put_be24(void *buf, uint32_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)(x >> 24);
    u8ptr[1] = (uint8_t)(x >> 16);
    u8ptr[2] = (uint8_t)(x >> 8);
}

void
put_be32(void *buf, uint32_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)(x >> 24);
    u8ptr[1] = (uint8_t)(x >> 16);
    u8ptr[2] = (uint8_t)(x >> 8);
    u8ptr[3] = (uint8_t)x;
}

void
put_be64(void *buf, uint64_t x)
{
    uint8_t *u8ptr;

    u8ptr = buf;
    u8ptr[0] = (uint8_t)(x >> 56);
    u8ptr[1] = (uint8_t)(x >> 48);
    u8ptr[2] = (uint8_t)(x >> 40);
    u8ptr[3] = (uint8_t)(x >> 32);
    u8ptr[4] = (uint8_t)(x >> 24);
    u8ptr[5] = (uint8_t)(x >> 16);
    u8ptr[6] = (uint8_t)(x >> 8);
    u8ptr[7] = (uint8_t)x;
}

uint16_t
get_be16(const void *buf)
{
    const uint8_t *u8ptr;
    uint16_t x;

    u8ptr = buf;
    x = (uint16_t)u8ptr[0] << 8;
    x |= u8ptr[1];

    return x;
}

uint32_t
get_be24(const void *buf)
{
    const uint8_t *u8ptr;
    uint32_t x;

    u8ptr = buf;
    x = (uint32_t)u8ptr[0] << 24;
    x |= (uint32_t)u8ptr[1] << 16;
    x |= (uint32_t)u8ptr[2] << 8;

    return x;
}

uint32_t
get_be32(const void *buf)
{
    const uint8_t *u8ptr;
    uint32_t x;

    u8ptr = buf;
    x = (uint32_t)u8ptr[0] << 24;
    x |= (uint32_t)u8ptr[1] << 16;
    x |= (uint32_t)u8ptr[2] << 8;
    x |= u8ptr[3];

    return x;
}

uint64_t
get_be64(const void *buf)
{
    const uint8_t *u8ptr;
    uint64_t x;

    u8ptr = buf;
    x = (uint64_t)u8ptr[0] << 56;
    x |= (uint64_t)u8ptr[1] << 48;
    x |= (uint64_t)u8ptr[2] << 40;
    x |= (uint64_t)u8ptr[3] << 32;
    x |= (uint64_t)u8ptr[4] << 24;
    x |= (uint64_t)u8ptr[5] << 16;
    x |= (uint64_t)u8ptr[6] << 8;
    x |= u8ptr[7];

    return x;
}
void
swap_in_place(void *buf, int len)
{
    uint8_t *u8ptr;
    uint8_t tmp;
    int i;
    int j;

    u8ptr = buf;

    for (i = 0, j = len - 1; i < j; i++, j--) {
        tmp = u8ptr[i];

        u8ptr[i] = u8ptr[j];
        u8ptr[j] = tmp;
    }
}

/* swap octets */
void
swap_buf(uint8_t *dst, const uint8_t *src, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        dst[len - 1 - i] = src[i];
    }
}
