/*
 * NimBLEUUID.cpp
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEUUID.cpp
 *
 *  Created on: Jun 21, 2017
 *      Author: kolban
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEUtils.h"
#include "NimBLEUUID.h"
#include "NimBLELog.h"

#include <algorithm>

static const char* LOG_TAG = "NimBLEUUID";


/**
 * @brief Create a UUID from a string.
 *
 * Create a UUID from a string.  There will be two possible stories here.  Either the string represents
 * a binary data field or the string represents a hex encoding of a UUID.
 * For the hex encoding, here is an example:
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
 NimBLEUUID::NimBLEUUID(const std::string &value) {
    m_valueSet = true;
    if (value.length() == 4) {
        m_uuid.u.type         = BLE_UUID_TYPE_16;
        m_uuid.u16.value = strtoul(value.c_str(), NULL, 16);
    }
    else if (value.length() == 8) {
        m_uuid.u.type         = BLE_UUID_TYPE_32;
        m_uuid.u32.value = strtoul(value.c_str(), NULL, 16);
    }
    else if (value.length() == 16) {
        *this = NimBLEUUID((uint8_t*)value.data(), 16, true);
    }
    else if (value.length() == 36) {
        // If the length of the string is 36 bytes then we will assume it is a long hex string in
        // UUID format.
        char * position = const_cast<char *>(value.c_str());
        uint32_t first =    strtoul(position, &position, 16);
        uint16_t second =   strtoul(position + 1, &position, 16);
        uint16_t third =    strtoul(position + 1, &position, 16);
        uint16_t fourth =   strtoul(position + 1, &position, 16);
        uint64_t fifth =    strtoull(position + 1, NULL, 16);
        *this = NimBLEUUID(first, second, third, (uint64_t(fourth) << 48) + fifth);
    }
    else {
        m_valueSet = false;
    }
} // NimBLEUUID(std::string)


/**
 * @brief Create a UUID from 2, 4, 16 bytes of memory.
 * @param [in] pData The pointer to the start of the UUID.
 * @param [in] size The size of the data.
 * @param [in] msbFirst Is the MSB first in pData memory?
 */
NimBLEUUID::NimBLEUUID(const uint8_t* pData, size_t size, bool msbFirst) {
    uint8_t *uuidValue = nullptr;

    switch(size) {
        case 2:
            uuidValue = (uint8_t*)&m_uuid.u16.value;
            m_uuid.u.type = BLE_UUID_TYPE_16;
            break;
        case 4:
            uuidValue = (uint8_t*)&m_uuid.u32.value;
            m_uuid.u.type = BLE_UUID_TYPE_32;
            break;
        case 16:
            uuidValue = m_uuid.u128.value;
            m_uuid.u.type = BLE_UUID_TYPE_128;
            break;
        default:
            m_valueSet = false;
            NIMBLE_LOGE(LOG_TAG, "Invalid UUID size");
            return;
    }
    if (msbFirst) {
        std::reverse_copy(pData, pData + size, uuidValue);
    } else {
        memcpy(uuidValue, pData, size);
    }
    m_valueSet = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the 16bit value.
 * @param [in] uuid The 16bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint16_t uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_16;
    m_uuid.u16.value     = uuid;
    m_valueSet           = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the 32bit value.
 * @param [in] uuid The 32bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint32_t uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_32;
    m_uuid.u32.value     = uuid;
    m_valueSet           = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the native UUID.
 * @param [in] uuid The native UUID.
 */
NimBLEUUID::NimBLEUUID(const ble_uuid128_t* uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_128;
    memcpy(m_uuid.u128.value, uuid->value, 16);
    m_valueSet = true;
} // NimBLEUUID


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
    m_uuid.u.type        = BLE_UUID_TYPE_128;
    memcpy(m_uuid.u128.value + 12, &first,  4);
    memcpy(m_uuid.u128.value + 10, &second, 2);
    memcpy(m_uuid.u128.value + 8,  &third,  2);
    memcpy(m_uuid.u128.value,      &fourth, 8);
    m_valueSet = true;
}


/**
 * @brief Creates an empty UUID.
 */
NimBLEUUID::NimBLEUUID() {
    m_valueSet = false;
} // NimBLEUUID


/**
 * @brief Get the number of bits in this uuid.
 * @return The number of bits in the UUID.  One of 16, 32 or 128.
 */
uint8_t NimBLEUUID::bitSize() const {
    if (!m_valueSet) return 0;
    return m_uuid.u.type;
} // bitSize


/**
 * @brief Compare a UUID against this UUID.
 *
 * @param [in] uuid The UUID to compare against.
 * @return True if the UUIDs are equal and false otherwise.
 */
bool NimBLEUUID::equals(const NimBLEUUID &uuid) const {
    return *this == uuid;
}


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
NimBLEUUID NimBLEUUID::fromString(const std::string &uuid) {
    uint8_t start = 0;
    if (strstr(uuid.c_str(), "0x") != nullptr) { // If the string starts with 0x, skip those characters.
        start = 2;
    }
    uint8_t len = uuid.length() - start; // Calculate the length of the string we are going to use.

    if(len == 4) {
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
 * @brief Get the native UUID value.
 * @return The native UUID value or nullptr if not set.
 */
const ble_uuid_any_t* NimBLEUUID::getNative() const {
    if (m_valueSet == false) {
        NIMBLE_LOGD(LOG_TAG,"<< Return of un-initialized UUID!");
        return nullptr;
    }
    return &m_uuid;
} // getNative


/**
 * @brief Convert a UUID to its 128 bit representation.
 * @details A UUID can be internally represented as 16bit, 32bit or the full 128bit.  This method
 * will convert 16 or 32 bit representations to the full 128bit.
 * @return The NimBLEUUID converted to 128bit.
 */
const NimBLEUUID &NimBLEUUID::to128() {
    // If we either don't have a value or are already a 128 bit UUID, nothing further to do.
    if (!m_valueSet || m_uuid.u.type == BLE_UUID_TYPE_128) {
        return *this;
    }

    // If we are 16 bit or 32 bit, then set the other bytes of the UUID.
    if (m_uuid.u.type == BLE_UUID_TYPE_16) {
        *this = NimBLEUUID(m_uuid.u16.value, 0x0000, 0x1000, 0x800000805f9b34fb);
    }
    else if (m_uuid.u.type == BLE_UUID_TYPE_32) {
        *this = NimBLEUUID(m_uuid.u32.value, 0x0000, 0x1000, 0x800000805f9b34fb);
    }

    return *this;
} // to128


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
 * @brief Convienience operator to check if this UUID is equal to another.
 */
bool NimBLEUUID::operator ==(const NimBLEUUID & rhs) const {
    if(m_valueSet && rhs.m_valueSet) {
        if(m_uuid.u.type != rhs.m_uuid.u.type) {
            uint8_t uuidBase[16] = {
                0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80,
                0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
            };

            if(m_uuid.u.type == BLE_UUID_TYPE_128){
                if(rhs.m_uuid.u.type == BLE_UUID_TYPE_16){
                    memcpy(uuidBase+12, &rhs.m_uuid.u16.value, 2);
                } else if (rhs.m_uuid.u.type == BLE_UUID_TYPE_32){
                    memcpy(uuidBase+12, &rhs.m_uuid.u32.value, 4);
                }
                return memcmp(m_uuid.u128.value,uuidBase,16) == 0;

            } else if(rhs.m_uuid.u.type == BLE_UUID_TYPE_128) {
                if(m_uuid.u.type == BLE_UUID_TYPE_16){
                    memcpy(uuidBase+12, &m_uuid.u16.value, 2);
                } else if (m_uuid.u.type == BLE_UUID_TYPE_32){
                    memcpy(uuidBase+12, &m_uuid.u32.value, 4);
                }
                return memcmp(rhs.m_uuid.u128.value,uuidBase,16) == 0;

            } else {
                return false;
            }
        }

        return ble_uuid_cmp(&m_uuid.u, &rhs.m_uuid.u) == 0;
    }

    return m_valueSet == rhs.m_valueSet;
}


/**
 * @brief Convienience operator to check if this UUID is not equal to another.
 */
bool NimBLEUUID::operator !=(const NimBLEUUID & rhs) const {
    return !this->operator==(rhs);
}


/**
 * @brief Convienience operator to convert this UUID to string representation.
 * @details This allows passing NimBLEUUID to functions
 * that accept std::string and/or or it's methods as a parameter.
 */
NimBLEUUID::operator std::string() const {
    if (!m_valueSet) return std::string();   // If we have no value, nothing to format.

    char buf[BLE_UUID_STR_LEN];

    return ble_uuid_to_str(&m_uuid.u, buf);
}


#endif /* CONFIG_BT_ENABLED */
