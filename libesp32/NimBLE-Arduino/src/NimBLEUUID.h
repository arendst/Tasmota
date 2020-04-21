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
    NimBLEUUID(std::string uuid);
    NimBLEUUID(uint16_t uuid);
    NimBLEUUID(uint32_t uuid);
    NimBLEUUID(ble_uuid128_t* uuid);
    NimBLEUUID(uint8_t* pData, size_t size, bool msbFirst);
    NimBLEUUID();
    uint8_t        bitSize();   // Get the number of bits in this uuid.
    bool           equals(NimBLEUUID uuid);
    ble_uuid_any_t* getNative();
    NimBLEUUID        to128();
    std::string    toString();
    static NimBLEUUID fromString(std::string uuid);  // Create a NimBLEUUID from a string

private:
    ble_uuid_any_t m_uuid;              // The underlying UUID structure that this class wraps.
    bool           m_valueSet = false;   // Is there a value set for this instance.
}; // NimBLEUUID
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEUUID_H_ */
