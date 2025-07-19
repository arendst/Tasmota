/*
 * NimBLEAddress.cpp
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEAddress.cpp
 *
 *  Created on: Jul 2, 2017
 *      Author: kolban
 */
#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include <algorithm>

#include "NimBLEAddress.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEAddress";

/*************************************************
 * NOTE: NimBLE address bytes are in INVERSE ORDER!
 * We will accomodate that fact in these methods.
*************************************************/

/**
 * @brief Create an address from the native NimBLE representation.
 * @param [in] address The native NimBLE address.
 */
NimBLEAddress::NimBLEAddress(ble_addr_t address) {
    memcpy(m_address, address.val, 6);
    m_addrType = address.type;
} // NimBLEAddress


/**
 * @brief Create a blank address, i.e. 00:00:00:00:00:00, type 0.
 */
NimBLEAddress::NimBLEAddress() {
    NimBLEAddress("");
} // NimBLEAddress


/**
 * @brief Create an address from a hex string
 *
 * A hex string is of the format:
 * ```
 * 00:00:00:00:00:00
 * ```
 * which is 17 characters in length.
 *
 * @param [in] stringAddress The hex string representation of the address.
 * @param [in] type The type of the address.
 */
NimBLEAddress::NimBLEAddress(const std::string &stringAddress, uint8_t type) {
    m_addrType = type;

    if (stringAddress.length() == 0) {
        memset(m_address, 0, 6);
        return;
    }

    if (stringAddress.length() == 6) {
        std::reverse_copy(stringAddress.data(), stringAddress.data() + 6, m_address);
        return;
    }

    if (stringAddress.length() != 17) {
        memset(m_address, 0, sizeof m_address); // "00:00:00:00:00:00" represents an invalid address
        NIMBLE_LOGD(LOG_TAG, "Invalid address '%s'", stringAddress.c_str());
        return;
    }

    int data[6];
    if(sscanf(stringAddress.c_str(), "%x:%x:%x:%x:%x:%x", &data[5], &data[4], &data[3], &data[2], &data[1], &data[0]) != 6) {
        memset(m_address, 0, sizeof m_address); // "00:00:00:00:00:00" represents an invalid address
        NIMBLE_LOGD(LOG_TAG, "Invalid address '%s'", stringAddress.c_str());
    }
    for(size_t index = 0; index < sizeof m_address; index++) {
        m_address[index] = data[index];
    }
} // NimBLEAddress


/**
 * @brief Constructor for compatibility with bluedroid esp library using native ESP representation.
 * @param [in] address A uint8_t[6] or esp_bd_addr_t containing the address.
 * @param [in] type The type of the address.
 */
NimBLEAddress::NimBLEAddress(uint8_t address[6], uint8_t type) {
    std::reverse_copy(address, address + sizeof m_address, m_address);
    m_addrType = type;
} // NimBLEAddress


/**
 * @brief Constructor for address using a hex value.\n
 * Use the same byte order, so use 0xa4c1385def16 for "a4:c1:38:5d:ef:16"
 * @param [in] address uint64_t containing the address.
 * @param [in] type The type of the address.
 */
NimBLEAddress::NimBLEAddress(const uint64_t &address, uint8_t type) {
    memcpy(m_address, &address, sizeof m_address);
    m_addrType = type;
} // NimBLEAddress


/**
 * @brief Determine if this address equals another.
 * @param [in] otherAddress The other address to compare against.
 * @return True if the addresses are equal.
 */
bool NimBLEAddress::equals(const NimBLEAddress &otherAddress) const {
    return *this == otherAddress;
} // equals


/**
 * @brief Get the native representation of the address.
 * @return a pointer to the uint8_t[6] array of the address.
 */
const uint8_t *NimBLEAddress::getNative() const {
    return m_address;
} // getNative


/**
 * @brief Get the address type.
 * @return The address type.
 */
uint8_t NimBLEAddress::getType() const {
    return m_addrType;
} // getType


/**
 * @brief Determine if this address is a Resolvable Private Address.
 * @return True if the address is a RPA.
 */
bool NimBLEAddress::isRpa() const {
    return (m_addrType && ((m_address[5] & 0xc0) == 0x40));
} // isRpa


/**
 * @brief Convert a BLE address to a string.
 *
 * A string representation of an address is in the format:
 *
 * ```
 * xx:xx:xx:xx:xx:xx
 * ```
 *
 * @return The string representation of the address.
 * @deprecated Use std::string() operator instead.
 */
std::string NimBLEAddress::toString() const {
    return std::string(*this);
} // toString


/**
 * @brief Convenience operator to check if this address is equal to another.
 */
bool NimBLEAddress::operator ==(const NimBLEAddress & rhs) const {
    return memcmp(rhs.m_address, m_address, sizeof m_address) == 0;
} // operator ==


/**
 * @brief Convenience operator to check if this address is not equal to another.
 */
bool NimBLEAddress::operator !=(const NimBLEAddress & rhs) const {
    return !this->operator==(rhs);
} // operator !=


/**
 * @brief Convienience operator to convert this address to string representation.
 * @details This allows passing NimBLEAddress to functions
 * that accept std::string and/or or it's methods as a parameter.
 */
NimBLEAddress::operator std::string() const {
    char buffer[18];
    snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x",
                                     m_address[5], m_address[4], m_address[3],
                                     m_address[2], m_address[1], m_address[0]);
    return std::string(buffer);
} // operator std::string


/**
 * @brief Convenience operator to convert the native address representation to uint_64.
 */
NimBLEAddress::operator uint64_t() const {
    uint64_t address = 0;
    memcpy(&address, m_address, sizeof m_address);
    return address;
} // operator uint64_t

#endif
