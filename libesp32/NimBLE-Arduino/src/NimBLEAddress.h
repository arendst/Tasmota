/*
 * NimBLEAddress.h
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEAddress.h
 *
 *  Created on: Jul 2, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEADDRESS_H_
#define COMPONENTS_NIMBLEADDRESS_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimble/ble.h"
/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include <string>
#include <algorithm>

/**
 * @brief A %BLE device address.
 *
 * Every %BLE device has a unique address which can be used to identify it and form connections.
 */
class NimBLEAddress {
public:
    NimBLEAddress(ble_addr_t address);
    NimBLEAddress(uint8_t address[6]);
    NimBLEAddress(const std::string &stringAddress);
    NimBLEAddress(const uint64_t &address);
    bool           equals(const NimBLEAddress &otherAddress) const;
    const uint8_t*       getNative() const;
    std::string    toString() const;

    bool operator   ==(const NimBLEAddress & rhs) const;
    bool operator   !=(const NimBLEAddress & rhs) const;
    operator        std::string() const;

private:
    uint8_t        m_address[6];
};

#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEADDRESS_H_ */
