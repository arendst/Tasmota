/*
 * NimBLERemoteService.h
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLERemoteService.h
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_NIMBLEREMOTESERVICE_H_
#define COMPONENTS_NIMBLEREMOTESERVICE_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEClient.h"
#include "NimBLEUUID.h"
#include "FreeRTOS.h"
#include "NimBLERemoteCharacteristic.h"

#include <map>

class NimBLEClient;
class NimBLERemoteCharacteristic;


/**
 * @brief A model of a remote %BLE service.
 */
class NimBLERemoteService {
public:
    virtual ~NimBLERemoteService();

    // Public methods
    NimBLERemoteCharacteristic* getCharacteristic(const char* uuid);      // Get the specified characteristic reference.
    NimBLERemoteCharacteristic* getCharacteristic(NimBLEUUID uuid);       // Get the specified characteristic reference.
//  BLERemoteCharacteristic* getCharacteristic(uint16_t uuid);      // Get the specified characteristic reference.
    std::map<std::string, NimBLERemoteCharacteristic*>* getCharacteristics();
    std::map<uint16_t, NimBLERemoteCharacteristic*>* getCharacteristicsByHandle();  // Get the characteristics map.
//  void getCharacteristics(std::map<uint16_t, BLERemoteCharacteristic*>* pCharacteristicMap);

    NimBLEClient*            getClient(void);                                           // Get a reference to the client associated with this service.
    uint16_t                 getHandle();                                               // Get the handle of this service.
    NimBLEUUID               getUUID(void);                                             // Get the UUID of this service.
    std::string              getValue(NimBLEUUID characteristicUuid);                      // Get the value of a characteristic.
    bool                     setValue(NimBLEUUID characteristicUuid, std::string value);   // Set the value of a characteristic.
    std::string              toString(void);

private:
    // Private constructor ... never meant to be created by a user application.
    NimBLERemoteService(NimBLEClient* pClient, const struct ble_gatt_svc *service); 

    // Friends
    friend class NimBLEClient;
    friend class NimBLERemoteCharacteristic;

    // Private methods
    bool                retrieveCharacteristics(void);   // Retrieve the characteristics from the BLE Server.
    static int          characteristicDiscCB(uint16_t conn_handle, 
                                const struct ble_gatt_error *error,
                                const struct ble_gatt_chr *chr, void *arg);

    uint16_t            getStartHandle();                // Get the start handle for this service.
    uint16_t            getEndHandle();                  // Get the end handle for this service.
    void                releaseSemaphores();
    void                removeCharacteristics();

    // Properties

    // We maintain a map of characteristics owned by this service keyed by a string representation of the UUID.
    std::map<std::string, NimBLERemoteCharacteristic*> m_characteristicMap;

    // We maintain a map of characteristics owned by this service keyed by a handle.
    std::map<uint16_t, NimBLERemoteCharacteristic*> m_characteristicMapByHandle;

    bool                m_haveCharacteristics; // Have we previously obtained the characteristics.
    NimBLEClient*       m_pClient;
    FreeRTOS::Semaphore m_semaphoreGetCharEvt = FreeRTOS::Semaphore("GetCharEvt");
    NimBLEUUID          m_uuid;             // The UUID of this service.
    uint16_t            m_startHandle;      // The starting handle of this service.
    uint16_t            m_endHandle;        // The ending handle of this service.
}; // BLERemoteService

#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTESERVICE_H_ */
