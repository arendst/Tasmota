/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NimBLEUUID.h"
#if CONFIG_BT_ENABLED

# include "NimBLEUtils.h"
# include "NimBLELog.h"

/****  FIX COMPILATION ****/
# undef min
# undef max
/**************************/

# include <algorithm>

static const char*   LOG_TAG         = "NimBLEUUID";
static const uint8_t ble_base_uuid[] = {
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/**
 * @brief Create a UUID from the native UUID.
 * @param [in] uuid The native UUID.
 */
NimBLEUUID::NimBLEUUID(const ble_uuid_any_t& uuid) : m_uuid{uuid} {}

/**
 * @brief Create a UUID from a string.
 *
 * Create a UUID from a string.  There will be two possible stories here. Either
 * the string represents a binary data field or the string represents a hex
 * encoding of a UUID. For the hex encoding, here is an example:
 *
 * ```
 * "beb5483e-36e1-4688-b7f5-ea07361b26a8"
 *  0 1 2 3  4 5  6 7  8 9  0 1 2 3 4 5
 *  12345678-90ab-cdef-1234-567890abcdef
 * ```
 *
 * This has a length of 36 characters.  We need to parse this into 16 bytes.
 *
 * @param [in] value The string to build a UUID from.
 */
NimBLEUUID::NimBLEUUID(const std::string& value) {
    if (value.length() == 4) {
        m_uuid.u.type    = BLE_UUID_TYPE_16;
        m_uuid.u16.value = strtoul(value.c_str(), nullptr, 16);
    } else if (value.length() == 8) {
        m_uuid.u.type    = BLE_UUID_TYPE_32;
        m_uuid.u32.value = strtoul(value.c_str(), nullptr, 16);
    } else if (value.length() == 16) {
        memcpy(m_uuid.u128.value, &value[0], 16);
        m_uuid.u.type = BLE_UUID_TYPE_128;
    } else if (value.length() == 36) {
        char*    position;
        uint64_t first_half = (strtoull(&value[0], &position, 16) << 32) +
                              (strtoull(position + 1, &position, 16) << 16) + strtoull(position + 1, &position, 16);
        uint64_t second_half = (strtoull(position + 1, &position, 16) << 48) + strtoull(position + 1, nullptr, 16);
        memcpy(m_uuid.u128.value + 8, &first_half, 8);
        memcpy(m_uuid.u128.value, &second_half, 8);
        m_uuid.u.type = BLE_UUID_TYPE_128;
    } else {
        NIMBLE_LOGE(LOG_TAG, "Invalid UUID length");
        m_uuid.u.type = 0;
    }
} // NimBLEUUID(std::string)

/**
 * @brief Create a UUID from 2, 4, 16 bytes of memory.
 * @param [in] pData The pointer to the start of the UUID.
 * @param [in] size The size of the data.
 */
NimBLEUUID::NimBLEUUID(const uint8_t* pData, size_t size) {
    if (ble_uuid_init_from_buf(&m_uuid, pData, size)) {
        NIMBLE_LOGE(LOG_TAG, "Invalid UUID size");
        m_uuid.u.type = 0;
    }
} // NimBLEUUID(const uint8_t* pData, size_t size)

/**
 * @brief Create a UUID from the 16bit value.
 * @param [in] uuid The 16bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint16_t uuid) {
    m_uuid.u.type    = BLE_UUID_TYPE_16;
    m_uuid.u16.value = uuid;
} // NimBLEUUID(uint16_t uuid)

/**
 * @brief Create a UUID from the 32bit value.
 * @param [in] uuid The 32bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint32_t uuid) {
    m_uuid.u.type    = BLE_UUID_TYPE_32;
    m_uuid.u32.value = uuid;
} // NimBLEUUID(uint32_t uuid)

/**
 * @brief Create a UUID from the native UUID.
 * @param [in] uuid The native UUID.
 */
NimBLEUUID::NimBLEUUID(const ble_uuid128_t* uuid) {
    m_uuid.u.type = BLE_UUID_TYPE_128;
    memcpy(m_uuid.u128.value, uuid->value, 16);
} // NimBLEUUID(const ble_uuid128_t* uuid)

/**
 * @brief Create a UUID from the 128bit value using hex parts instead of string,
 * instead of NimBLEUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6"), it becomes
 * NimBLEUUID(0xebe0ccb0, 0x7a0a, 0x4b0c, 0x8a1a6ff2997da3a6)
 *
 * @param [in] first  The first 32bit of the UUID.
 * @param [in] second The next 16bit of the UUID.
 * @param [in] third  The next 16bit of the UUID.
 * @param [in] fourth The last 64bit of the UUID, combining the last 2 parts of the string equivalent
 */
NimBLEUUID::NimBLEUUID(uint32_t first, uint16_t second, uint16_t third, uint64_t fourth) {
    m_uuid.u.type = BLE_UUID_TYPE_128;
    memcpy(m_uuid.u128.value + 12, &first, 4);
    memcpy(m_uuid.u128.value + 10, &second, 2);
    memcpy(m_uuid.u128.value + 8, &third, 2);
    memcpy(m_uuid.u128.value, &fourth, 8);
} // NimBLEUUID(uint32_t first, uint16_t second, uint16_t third, uint64_t fourth)

/**
 * @brief Get the bit size of the UUID, 16, 32 or 128.
 * @return The bit size of the UUID or 0 if not initialized.
 */
uint8_t NimBLEUUID::bitSize() const {
    return this->m_uuid.u.type;
} // bitSize

/**
 * @brief Get the uuid value.
 * @return A pointer to the UUID value or nullptr if not initialized.
 * @note This should be checked with `bitSize()` before accessing.
 */
const uint8_t* NimBLEUUID::getValue() const {
    switch (bitSize()) {
        case BLE_UUID_TYPE_16:
            return reinterpret_cast<const uint8_t*>(&m_uuid.u16.value);
        case BLE_UUID_TYPE_32:
            return reinterpret_cast<const uint8_t*>(&m_uuid.u32.value);
        case BLE_UUID_TYPE_128:
            return m_uuid.u128.value;
        default:
            return nullptr;
    }
} // getValue

/**
 * @brief Get a pointer to the NimBLE UUID base structure.
 * @return A Read-only pointer to the NimBLE UUID base structure.
 * @note The type value should be checked, if no 16, 32 or 128 then the UUID is not initialized.
 */
const ble_uuid_t* NimBLEUUID::getBase() const {
    return &this->m_uuid.u;
} // getBase

/**
 * @brief Compare a UUID against this UUID.
 *
 * @param [in] uuid The UUID to compare against.
 * @return True if the UUIDs are equal and false otherwise.
 */
bool NimBLEUUID::equals(const NimBLEUUID& uuid) const {
    return *this == uuid;
} // equals

/**
 * Create a NimBLEUUID from a string of the form:
 * 0xNNNN
 * 0xNNNNNNNN
 * 0x<UUID\>
 * NNNN
 * NNNNNNNN
 * <UUID\>
 *
 * @param [in] uuid The string to create the UUID from.
 */
NimBLEUUID NimBLEUUID::fromString(const std::string& uuid) {
    uint8_t start = 0;
    if (strstr(uuid.c_str(), "0x") != nullptr) { // If the string starts with 0x, skip those characters.
        start = 2;
    }

    uint8_t len = uuid.length() - start; // Calculate the length of the string we are going to use.
    if (len == 4) {
        uint16_t x = strtoul(uuid.substr(start, len).c_str(), NULL, 16);
        return NimBLEUUID(x);
    } else if (len == 8) {
        uint32_t x = strtoul(uuid.substr(start, len).c_str(), NULL, 16);
        return NimBLEUUID(x);
    } else if (len == 36) {
        return NimBLEUUID(uuid);
    }

    return NimBLEUUID();
} // fromString

/**
 * @brief Convert a UUID to its 128 bit representation.
 * @details A UUID can be internally represented as 16bit, 32bit or the full 128bit.
 * This method will convert 16 or 32bit representations to the full 128bit.
 * @return The NimBLEUUID converted to 128bit.
 */
const NimBLEUUID& NimBLEUUID::to128() {
    // If we either don't have a value or are already a 128 bit UUID, nothing further to do.
    if (bitSize() != BLE_UUID_TYPE_128) {
        uint32_t val = bitSize() == BLE_UUID_TYPE_16 ? *reinterpret_cast<const uint16_t*>(getValue())
                                                     : *reinterpret_cast<const uint32_t*>(getValue());
        memcpy(m_uuid.u128.value, &ble_base_uuid, sizeof(ble_base_uuid) - 4);
        memcpy(m_uuid.u128.value + 12, &val, 4);
        m_uuid.u.type = BLE_UUID_TYPE_128;
    }

    return *this;
} // to128

/**
 * @brief Convert 128 bit UUID to its 16 bit representation.
 * @details A UUID can be internally represented as 16bit, 32bit or the full 128bit.
 * This method will convert a 128bit uuid to 16bit if it contains the ble base uuid.
 * @return The NimBLEUUID converted to 16bit if successful, otherwise the original uuid.
 */
const NimBLEUUID& NimBLEUUID::to16() {
    if (bitSize() == BLE_UUID_TYPE_128) {
        const uint8_t* val = getValue();
        if (memcmp(val, ble_base_uuid, sizeof(ble_base_uuid) - 4) == 0) {
            m_uuid.u16.value = *reinterpret_cast<const uint16_t*>(val + 12);
            m_uuid.u.type    = BLE_UUID_TYPE_16;
        }
    }

    return *this;
} // to16

/**
 * @brief Get a string representation of the UUID.
 * @details
 * The format of a string is:
 * 01234567 8901 2345 6789 012345678901
 * 0000180d-0000-1000-8000-00805f9b34fb
 * 0 1 2 3  4 5  6 7  8 9  0 1 2 3 4 5
 *
 * @return A string representation of the UUID.
 * @deprecated Use std::string() operator instead.
 */
std::string NimBLEUUID::toString() const {
    return std::string(*this);
} // toString

/**
 * @brief Reverse the byte order of the UUID.
 * @return The NimBLEUUID with the byte order reversed.
 * @details This is useful when comparing UUIDs or when the advertisement data is reversed.
 */
const NimBLEUUID& NimBLEUUID::reverseByteOrder() {
    if (bitSize() == BLE_UUID_TYPE_128) {
        std::reverse(m_uuid.u128.value, m_uuid.u128.value + 16);
    } else if (bitSize() == BLE_UUID_TYPE_32) {
        m_uuid.u32.value = __builtin_bswap32(m_uuid.u32.value);
    } else if (bitSize() == BLE_UUID_TYPE_16) {
        m_uuid.u16.value = __builtin_bswap16(m_uuid.u16.value);
    }

    return *this;
} // reverseByteOrder

/**
 * @brief Convenience operator to check if this UUID is equal to another.
 */
bool NimBLEUUID::operator==(const NimBLEUUID& rhs) const {
    if (!this->bitSize() || !rhs.bitSize()) {
        return false;
    }

    if (this->bitSize() != rhs.bitSize()) {
        uint8_t uuid128[sizeof(ble_base_uuid)];
        memcpy(uuid128, &ble_base_uuid, sizeof(ble_base_uuid));
        if (this->bitSize() == BLE_UUID_TYPE_128) {
            memcpy(uuid128 + 12, rhs.getValue(), rhs.bitSize() == BLE_UUID_TYPE_16 ? 2 : 4);
            return memcmp(this->getValue(), uuid128, sizeof(ble_base_uuid)) == 0;

        } else if (rhs.bitSize() == BLE_UUID_TYPE_128) {
            memcpy(uuid128 + 12, getValue(), this->bitSize() == BLE_UUID_TYPE_16 ? 2 : 4);
            return memcmp(rhs.getValue(), uuid128, sizeof(ble_base_uuid)) == 0;
        } else {
            return false;
        }
    }

    if (this->bitSize() == BLE_UUID_TYPE_16) {
        return *reinterpret_cast<const uint16_t*>(this->getValue()) == *reinterpret_cast<const uint16_t*>(rhs.getValue());
    }

    if (this->bitSize() == BLE_UUID_TYPE_32) {
        return *reinterpret_cast<const uint32_t*>(this->getValue()) == *reinterpret_cast<const uint32_t*>(rhs.getValue());
    }

    if (this->bitSize() == BLE_UUID_TYPE_128) {
        return memcmp(this->getValue(), rhs.getValue(), 16) == 0;
    }

    return false;
} // operator==

/**
 * @brief Convenience operator to check if this UUID is not equal to another.
 */
bool NimBLEUUID::operator!=(const NimBLEUUID& rhs) const {
    return !this->operator==(rhs);
} // operator!=

/**
 * @brief Convenience operator to convert this UUID to string representation.
 * @details This allows passing NimBLEUUID to functions
 * that accept std::string and/or or it's methods as a parameter.
 */
NimBLEUUID::operator std::string() const {
    char buf[BLE_UUID_STR_LEN];
    return ble_uuid_to_str(&m_uuid.u, buf);
} // operator std::string

#endif /* CONFIG_BT_ENABLED */
