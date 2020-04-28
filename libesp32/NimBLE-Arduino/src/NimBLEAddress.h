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

typedef enum {
    BLE_ADDR_TYPE_PUBLIC        = 0x00,
    BLE_ADDR_TYPE_RANDOM        = 0x01,
    BLE_ADDR_TYPE_RPA_PUBLIC    = 0x02,
    BLE_ADDR_TYPE_RPA_RANDOM    = 0x03,
} esp_nimble_addr_type_t;

typedef uint8_t esp_ble_addr_type_t ;

/// Bluetooth address length
#define ESP_BD_ADDR_LEN     6

/// Bluetooth device address
typedef uint8_t esp_bd_addr_t[ESP_BD_ADDR_LEN];


/**
 * @brief A %BLE device address.
 *
 * Every %BLE device has a unique address which can be used to identify it and form connections.
 */
class NimBLEAddress {
public:
    NimBLEAddress(ble_addr_t address);
    NimBLEAddress(esp_bd_addr_t address);
    NimBLEAddress(std::string stringAddress);
    bool           equals(NimBLEAddress otherAddress);
    uint8_t*       getNative();
    std::string    toString();

private:
    uint8_t        m_address[6];
};

#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEADDRESS_H_ */
