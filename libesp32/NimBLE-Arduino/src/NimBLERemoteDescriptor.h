/*
 * NimBLERemoteDescriptor.h
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLERemoteDescriptor.h
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_
#define COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLERemoteCharacteristic.h"

class NimBLERemoteCharacteristic;
/**
 * @brief A model of remote %BLE descriptor.
 */
class NimBLERemoteDescriptor {
public:
    uint16_t    getHandle();
    NimBLERemoteCharacteristic* getRemoteCharacteristic();
    NimBLEUUID     getUUID();
    std::string readValue(void);
    uint8_t     readUInt8(void);
    uint16_t    readUInt16(void);
    uint32_t    readUInt32(void);
    std::string toString(void);
    bool        writeValue(uint8_t* data, size_t length, bool response = false);
    bool        writeValue(std::string newValue, bool response = false);
    bool        writeValue(uint8_t newValue, bool response = false);


private:
    friend class NimBLERemoteCharacteristic;
    NimBLERemoteDescriptor(NimBLERemoteCharacteristic* pRemoteCharacteristic, const struct ble_gatt_dsc *dsc);
    static int  onWriteCB(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);
    static int  onReadCB(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);
    void        releaseSemaphores();

    uint16_t                    m_handle;                  // Server handle of this descriptor.
    NimBLEUUID                  m_uuid;                    // UUID of this descriptor.
    std::string                 m_value;                   // Last received value of the descriptor.
    NimBLERemoteCharacteristic* m_pRemoteCharacteristic;   // Reference to the Remote characteristic of which this descriptor is associated.
    FreeRTOS::Semaphore         m_semaphoreReadDescrEvt  = FreeRTOS::Semaphore("ReadDescrEvt");
    FreeRTOS::Semaphore         m_semaphoreDescWrite     = FreeRTOS::Semaphore("WriteDescEvt");


};
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTEDESCRIPTOR_H_ */
