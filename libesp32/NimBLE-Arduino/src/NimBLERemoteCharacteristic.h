/*
 * NimBLERemoteCharacteristic.h
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLERemoteCharacteristic.h
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_
#define COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

//#include "NimBLEUUID.h"
//#include "FreeRTOS.h"
#include "NimBLERemoteService.h"
#include "NimBLERemoteDescriptor.h"

//#include <string>
#include <map>

class NimBLERemoteService;
class NimBLERemoteDescriptor;


typedef void (*notify_callback)(NimBLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

/**
 * @brief A model of a remote %BLE characteristic.
 */
class NimBLERemoteCharacteristic {
public:
    ~NimBLERemoteCharacteristic();

    // Public member functions
    bool        canBroadcast();
    bool        canIndicate();
    bool        canNotify();
    bool        canRead();
    bool        canWrite();
    bool        canWriteNoResponse();
    NimBLERemoteDescriptor* getDescriptor(NimBLEUUID uuid);
    std::map<std::string, NimBLERemoteDescriptor*>* getDescriptors();
    uint16_t    getHandle();
    uint16_t    getDefHandle();
    NimBLEUUID  getUUID();
    std::string readValue();
    uint8_t     readUInt8();
    uint16_t    readUInt16();
    uint32_t    readUInt32();
    bool        registerForNotify(notify_callback _callback, bool notifications = true, bool response = true);
    bool        writeValue(uint8_t* data, size_t length, bool response = false);
    bool        writeValue(std::string newValue, bool response = false);
    bool        writeValue(uint8_t newValue, bool response = false);
    std::string toString();
    uint8_t*    readRawData();
    NimBLERemoteService* getRemoteService();

private:

    NimBLERemoteCharacteristic(NimBLERemoteService *pRemoteservice, const struct ble_gatt_chr *chr);
    
    friend class NimBLEClient;
    friend class NimBLERemoteService;
    friend class NimBLERemoteDescriptor;

    // Private member functions
    void              removeDescriptors();
    bool              retrieveDescriptors(uint16_t endHdl);
    static int        onReadCB(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);
    static int        onWriteCB(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);
    void              releaseSemaphores();
    static int        descriptorDiscCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                uint16_t chr_val_handle, const struct ble_gatt_dsc *dsc,
                                void *arg);
    
    // Private properties
    NimBLEUUID              m_uuid;
    uint8_t                 m_charProp;
    uint16_t                m_handle;
    uint16_t                m_defHandle;
    NimBLERemoteService*    m_pRemoteService;
    FreeRTOS::Semaphore     m_semaphoreGetDescEvt       = FreeRTOS::Semaphore("GetDescEvt");
    FreeRTOS::Semaphore     m_semaphoreReadCharEvt      = FreeRTOS::Semaphore("ReadCharEvt");
    FreeRTOS::Semaphore     m_semaphoreWriteCharEvt     = FreeRTOS::Semaphore("WriteCharEvt");
    std::string             m_value;
    uint8_t*                m_rawData = nullptr;
    notify_callback         m_notifyCallback;

    // We maintain a map of descriptors owned by this characteristic keyed by a string representation of the UUID.
    std::map<std::string, NimBLERemoteDescriptor*> m_descriptorMap;
}; // BLERemoteCharacteristic
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTECHARACTERISTIC_H_ */
