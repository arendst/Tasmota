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

#ifndef H_BLE_UUID_
#define H_BLE_UUID_

/**
 * @brief Bluetooth UUID
 * @defgroup bt_uuid Bluetooth UUID
 * @ingroup bt_host
 * @{
 */

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct os_mbuf;

/** Type of UUID */
enum {
    /** 16-bit UUID (BT SIG assigned) */
    BLE_UUID_TYPE_16 = 16,

    /** 32-bit UUID (BT SIG assigned) */
    BLE_UUID_TYPE_32 = 32,

    /** 128-bit UUID */
    BLE_UUID_TYPE_128 = 128,
};

/** Generic UUID type, to be used only as a pointer */
typedef struct {
    /** Type of the UUID */
    uint8_t type;
} ble_uuid_t;

/** 16-bit UUID */
typedef struct {
    ble_uuid_t u;
    uint16_t value;
} ble_uuid16_t;

/** 32-bit UUID */
typedef struct {
    ble_uuid_t u;
    uint32_t value;
} ble_uuid32_t;

/** 128-bit UUID */
typedef struct {
    ble_uuid_t u;
    uint8_t value[16];
} ble_uuid128_t;

/** Universal UUID type, to be used for any-UUID static allocation */
typedef union {
    ble_uuid_t u;
    ble_uuid16_t u16;
    ble_uuid32_t u32;
    ble_uuid128_t u128;
} ble_uuid_any_t;

#define BLE_UUID16_INIT(uuid16)         \
    {                                   \
        .u.type = BLE_UUID_TYPE_16,     \
        .value = (uuid16),              \
    }

#define BLE_UUID32_INIT(uuid32)         \
    {                                   \
        .u.type = BLE_UUID_TYPE_32,     \
        .value = (uuid32),              \
    }

#define BLE_UUID128_INIT(uuid128...)    \
    {                                   \
        .u.type = BLE_UUID_TYPE_128,    \
        .value = { uuid128 },           \
    }

#define BLE_UUID16_DECLARE(uuid16) \
    ((ble_uuid_t *) (&(ble_uuid16_t) BLE_UUID16_INIT(uuid16)))

#define BLE_UUID32_DECLARE(uuid32) \
    ((ble_uuid_t *) (&(ble_uuid32_t) BLE_UUID32_INIT(uuid32)))

#define BLE_UUID128_DECLARE(uuid128...) \
    ((ble_uuid_t *) (&(ble_uuid128_t) BLE_UUID128_INIT(uuid128)))

#define BLE_UUID16(u) \
    ((ble_uuid16_t *) (u))

#define BLE_UUID32(u) \
    ((ble_uuid32_t *) (u))

#define BLE_UUID128(u) \
    ((ble_uuid128_t *) (u))

/** Size of buffer needed to store UUID as a string.
 *  Includes trailing \0.
 */
#define BLE_UUID_STR_LEN (37)

/** @brief Constructs a UUID object from a byte array.
 *
 * @param uuid  On success, this gets populated with the constructed UUID.
 * @param buf   The source buffer to parse.
 * @param len   The size of the buffer, in bytes.
 *
 * @return      0 on success, BLE_HS_EINVAL if the source buffer does not contain
 *              a valid UUID.
 */
int ble_uuid_init_from_buf(ble_uuid_any_t *uuid, const void *buf, size_t len);

/** @brief Compares two Bluetooth UUIDs.
 *
 * @param uuid1  The first UUID to compare.
 * @param uuid2  The second UUID to compare.
 *
 * @return       0 if the two UUIDs are equal, nonzero if the UUIDs differ.
 */
int ble_uuid_cmp(const ble_uuid_t *uuid1, const ble_uuid_t *uuid2);

/** @brief Copy Bluetooth UUID
 *
 * @param dst    Destination UUID.
 * @param src    Source UUID.
 */
void ble_uuid_copy(ble_uuid_any_t *dst, const ble_uuid_t *src);

/** @brief Converts the specified UUID to its string representation.
 *
 * Example string representations:
 *     o 16-bit:  0x1234
 *     o 32-bit:  0x12345678
 *     o 128-bit: 12345678-1234-1234-1234-123456789abc
 *
 * @param uuid   The source UUID to convert.
 * @param dst    The destination buffer.
 *
 * @return       A pointer to the supplied destination buffer.
 */
char *ble_uuid_to_str(const ble_uuid_t *uuid, char *dst);

/** @brief Converts the specified 16-bit UUID to a uint16_t.
 *
 * @param uuid   The source UUID to convert.
 *
 * @return       The converted integer on success, NULL if the specified UUID is
 *               not 16 bits.
 */
uint16_t ble_uuid_u16(const ble_uuid_t *uuid);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* _BLE_HOST_UUID_H */
