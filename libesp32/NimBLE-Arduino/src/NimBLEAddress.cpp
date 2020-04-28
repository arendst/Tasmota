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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEAddress.h"
#include "NimBLEUtils.h"


/*************************************************
NOTE: NimBLE addresses are in INVERSE ORDER!
We will accomodate that fact in these methods.
*************************************************/

/**
 * @brief Create an address from the native ESP32 representation.
 * @param [in] address The native representation.
 */
NimBLEAddress::NimBLEAddress(ble_addr_t address) {
    memcpy(m_address, address.val, 6);
} // BLEAddress


/**
 * @brief Create an address from a hex string
 *
 * A hex string is of the format:
 * ```
 * 00:00:00:00:00:00
 * ```
 * which is 17 characters in length.
 *
 * @param [in] stringAddress The hex representation of the address.
 */
NimBLEAddress::NimBLEAddress(std::string stringAddress) {
    if (stringAddress.length() != 17) return;

    int data[6];
    sscanf(stringAddress.c_str(), "%x:%x:%x:%x:%x:%x", &data[5], &data[4], &data[3], &data[2], &data[1], &data[0]);
    m_address[0] = (uint8_t) data[0];
    m_address[1] = (uint8_t) data[1];
    m_address[2] = (uint8_t) data[2];
    m_address[3] = (uint8_t) data[3];
    m_address[4] = (uint8_t) data[4];
    m_address[5] = (uint8_t) data[5];
} // BLEAddress


/**
 * @brief Constructor for compatibility with bluedrioid esp library.
 * @param [in] esp_bd_addr_t struct containing the address.
 */
NimBLEAddress::NimBLEAddress(esp_bd_addr_t address) {
    NimBLEUtils::memrcpy(m_address, address, 6);
} // NimBLEAddress


/**
 * @brief Determine if this address equals another.
 * @param [in] otherAddress The other address to compare against.
 * @return True if the addresses are equal.
 */
bool NimBLEAddress::equals(NimBLEAddress otherAddress) {
    return memcmp(otherAddress.getNative(), m_address, 6) == 0;
} // equals


/**
 * @brief Return the native representation of the address.
 * @return The native representation of the address.
 */
uint8_t *NimBLEAddress::getNative() {
    return m_address;
} // getNative


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
 */
std::string NimBLEAddress::toString() {
    auto size = 18;
    char *res = (char*)malloc(size);
    snprintf(res, size, "%02x:%02x:%02x:%02x:%02x:%02x", m_address[5], m_address[4], m_address[3], m_address[2], m_address[1], m_address[0]);
    std::string ret(res);
    free(res);
    return ret;
    
} // toString
#endif
