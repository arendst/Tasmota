/*
 * NimBLEUUID.h
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEUUID.h
 *
 *  Created on: Jun 21, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEUUID_H_
#define COMPONENTS_NIMBLEUUID_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "host/ble_uuid.h"
/****  FIX COMPILATION ****/
#undef min
#undef max
/**************************/

#include <string>

/**
 * @brief A model of a %BLE UUID.
 */
class NimBLEUUID {
public:
    NimBLEUUID(const std::string &uuid);
    NimBLEUUID(uint16_t uuid);
    NimBLEUUID(uint32_t uuid);
    NimBLEUUID(const ble_uuid128_t* uuid);
    NimBLEUUID(const uint8_t* pData, size_t size, bool msbFirst);
    NimBLEUUID(uint32_t first, uint16_t second, uint16_t third, uint64_t fourth);
    NimBLEUUID();

    uint8_t               bitSize() const;
    bool                  equals(const NimBLEUUID &uuid) const;
    const ble_uuid_any_t* getNative() const;
    const NimBLEUUID &    to128();
    std::string           toString() const;
    static NimBLEUUID     fromString(const std::string &uuid);

    bool operator ==(const NimBLEUUID & rhs) const;
    bool operator !=(const NimBLEUUID & rhs) const;
    operator std::string() const;

private:
    ble_uuid_any_t m_uuid;
    bool           m_valueSet = false;
}; // NimBLEUUID
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEUUID_H_ */
