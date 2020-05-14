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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEUtils.h"
#include "NimBLEUUID.h"
#include "NimBLELog.h"

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
 NimBLEUUID::NimBLEUUID(std::string value) {
    m_valueSet = true;
    if (value.length() == 4) {
        m_uuid.u.type         = BLE_UUID_TYPE_16;
        m_uuid.u16.value      = 0;
        for(int i=0;i<value.length();){
            uint8_t MSB = value.c_str()[i];
            uint8_t LSB = value.c_str()[i+1];
            
            if(MSB > '9') MSB -= 7;
            if(LSB > '9') LSB -= 7;
            m_uuid.u16.value += (((MSB&0x0F) <<4) | (LSB & 0x0F))<<(2-i)*4;
            i+=2;   
        }
    }
    else if (value.length() == 8) {
        m_uuid.u.type         = BLE_UUID_TYPE_32;
        m_uuid.u32.value      = 0;
        for(int i=0;i<value.length();){
            uint8_t MSB = value.c_str()[i];
            uint8_t LSB = value.c_str()[i+1];
            
            if(MSB > '9') MSB -= 7; 
            if(LSB > '9') LSB -= 7;
            m_uuid.u32.value += (((MSB&0x0F) <<4) | (LSB & 0x0F))<<(6-i)*4;
            i+=2;
        }       
    }
    else if (value.length() == 16) {  // how we can have 16 byte length string reprezenting 128 bit uuid??? needs to be investigated (lack of time)
        m_uuid.u.type = BLE_UUID_TYPE_128;
        NimBLEUtils::memrcpy(m_uuid.u128.value, (uint8_t*)value.data(), 16);
    }
    else if (value.length() == 36) {
        // If the length of the string is 36 bytes then we will assume it is a long hex string in
        // UUID format.
        m_uuid.u.type = BLE_UUID_TYPE_128;
        int n = 0;
        for(int i=0;i<value.length();){
            if(value.c_str()[i] == '-')
                i++;
            uint8_t MSB = value.c_str()[i];
            uint8_t LSB = value.c_str()[i+1];
            
            if(MSB > '9') MSB -= 7; 
            if(LSB > '9') LSB -= 7;
            m_uuid.u128.value[15-n++] = ((MSB&0x0F) <<4) | (LSB & 0x0F);
            i+=2;   
        }
    }
    else {
        NIMBLE_LOGE(LOG_TAG,"ERROR: UUID value not 2, 4, 16 or 36 bytes");
        m_valueSet = false;
    }
} // NimBLEUUID(std::string)


/**
 * @brief Create a UUID from 16 bytes of memory.
 *
 * @param [in] pData The pointer to the start of the UUID.
 * @param [in] size The size of the data.
 * @param [in] msbFirst Is the MSB first in pData memory?
 */
NimBLEUUID::NimBLEUUID(uint8_t* pData, size_t size, bool msbFirst) {
/*** TODO: change this to use the Nimble function for various lenght UUIDs:
    int ble_uuid_init_from_buf(ble_uuid_any_t *uuid, const void *buf, size_t len);
***/
    if (size != 16) {
        NIMBLE_LOGE(LOG_TAG,"ERROR: UUID length not 16 bytes");
        return;
    }
    m_uuid.u.type = BLE_UUID_TYPE_128;
    if (msbFirst) {
        NimBLEUtils::memrcpy(m_uuid.u128.value, pData, 16);
    } else {
        memcpy(m_uuid.u128.value, pData, 16);
    }
    m_valueSet = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the 16bit value.
 *
 * @param [in] uuid The 16bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint16_t uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_16;
    m_uuid.u16.value     = uuid;
    m_valueSet           = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the 32bit value.
 *
 * @param [in] uuid The 32bit short form UUID.
 */
NimBLEUUID::NimBLEUUID(uint32_t uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_32;
    m_uuid.u32.value     = uuid;
    m_valueSet           = true;
} // NimBLEUUID


/**
 * @brief Create a UUID from the native UUID.
 *
 * @param [in] uuid The native UUID.
 */
 
NimBLEUUID::NimBLEUUID(ble_uuid128_t* uuid) {
    m_uuid.u.type        = BLE_UUID_TYPE_128;
    memcpy(m_uuid.u128.value, uuid->value, 16);
    m_valueSet = true;
} // NimBLEUUID


NimBLEUUID::NimBLEUUID() {
    m_valueSet = false;
} // NimBLEUUID


/**
 * @brief Get the number of bits in this uuid.
 * @return The number of bits in the UUID.  One of 16, 32 or 128.
 */
uint8_t NimBLEUUID::bitSize() {
    if (!m_valueSet) return 0;
    return m_uuid.u.type;
} // bitSize


/**
 * @brief Compare a UUID against this UUID.
 *
 * @param [in] uuid The UUID to compare against.
 * @return True if the UUIDs are equal and false otherwise.
 */
bool NimBLEUUID::equals(NimBLEUUID uuid) {
    if(ble_uuid_cmp(&m_uuid.u, &uuid.getNative()->u) == 0){
        return true;
    }
    return false;
}


/**
 * Create a BLEUUID from a string of the form:
 * 0xNNNN
 * 0xNNNNNNNN
 * 0x<UUID>
 * NNNN
 * NNNNNNNN
 * <UUID>
 */
 
NimBLEUUID NimBLEUUID::fromString(std::string _uuid) {
    uint8_t start = 0;
    if (strstr(_uuid.c_str(), "0x") != nullptr) { // If the string starts with 0x, skip those characters.
        start = 2;
    }
    uint8_t len = _uuid.length() - start; // Calculate the length of the string we are going to use.

    if(len == 4) {
        uint16_t x = strtoul(_uuid.substr(start, len).c_str(), NULL, 16);
        return NimBLEUUID(x);
    } else if (len == 8) {
        uint32_t x = strtoul(_uuid.substr(start, len).c_str(), NULL, 16);
        return NimBLEUUID(x);
    } else if (len == 36) {
        return NimBLEUUID(_uuid);
    }
    return NimBLEUUID();
} // fromString


/**
 * @brief Get the native UUID value.
 *
 * @return The native UUID value or NULL if not set.
 */
ble_uuid_any_t* NimBLEUUID::getNative() {
    if (m_valueSet == false) {
        NIMBLE_LOGD(LOG_TAG,"<< Return of un-initialized UUID!");
        return nullptr;
    }
    return &m_uuid;
} // getNative


/**
 * @brief Convert a UUID to its 128 bit representation.
 *
 * A UUID can be internally represented as 16bit, 32bit or the full 128bit.  This method
 * will convert 16 or 32 bit representations to the full 128bit.
 */
NimBLEUUID NimBLEUUID::to128() {
    // If we either don't have a value or are already a 128 bit UUID, nothing further to do.
    if (!m_valueSet || m_uuid.u.type == BLE_UUID_TYPE_128) {
        return *this;
    }

    // If we are 16 bit or 32 bit, then set the 4 bytes of the variable part of the UUID.
    if (m_uuid.u.type == BLE_UUID_TYPE_16) {
        uint16_t temp = m_uuid.u16.value;
        m_uuid.u128.value[15] = 0;
        m_uuid.u128.value[14] = 0;
        m_uuid.u128.value[13] = (temp >> 8) & 0xff;
        m_uuid.u128.value[12] = temp & 0xff;

    }
    else if (m_uuid.u.type == BLE_UUID_TYPE_32) {
        uint32_t temp = m_uuid.u32.value;
        m_uuid.u128.value[15] = (temp >> 24) & 0xff;
        m_uuid.u128.value[14] = (temp >> 16) & 0xff;
        m_uuid.u128.value[13] = (temp >> 8) & 0xff;
        m_uuid.u128.value[12] = temp & 0xff;
    }

    // Set the fixed parts of the UUID.
    m_uuid.u128.value[11] = 0x00;
    m_uuid.u128.value[10] = 0x00;

    m_uuid.u128.value[9]  = 0x10;
    m_uuid.u128.value[8]  = 0x00;

    m_uuid.u128.value[7]  = 0x80;
    m_uuid.u128.value[6]  = 0x00;

    m_uuid.u128.value[5]  = 0x00;
    m_uuid.u128.value[4]  = 0x80;
    m_uuid.u128.value[3]  = 0x5f;
    m_uuid.u128.value[2]  = 0x9b;
    m_uuid.u128.value[1]  = 0x34;
    m_uuid.u128.value[0]  = 0xfb;

    m_uuid.u.type = BLE_UUID_TYPE_128;
    return *this;
} // to128


/**
 * @brief Get a string representation of the UUID.
 *
 * The format of a string is:
 * 01234567 8901 2345 6789 012345678901
 * 0000180d-0000-1000-8000-00805f9b34fb
 * 0 1 2 3  4 5  6 7  8 9  0 1 2 3 4 5
 *
 * @return A string representation of the UUID.
 */
std::string NimBLEUUID::toString() {
    if (!m_valueSet) return "<NULL>";   // If we have no value, nothing to format.

    char buf[BLE_UUID_STR_LEN];

    return ble_uuid_to_str(&m_uuid.u, buf);
} // toString

#endif /* CONFIG_BT_ENABLED */
