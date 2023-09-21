/*
 * NimBLEService.h
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEService.h
 *
 *  Created on: Mar 25, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLESERVICE_H_
#define MAIN_NIMBLESERVICE_H_

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEServer.h"
#include "NimBLECharacteristic.h"
#include "NimBLEUUID.h"


class NimBLEServer;
class NimBLECharacteristic;


/**
 * @brief The model of a %BLE service.
 *
 */
class NimBLEService {
public:

    NimBLEService(const char* uuid);
    NimBLEService(const NimBLEUUID &uuid);
    ~NimBLEService();

    NimBLEServer*         getServer();

    NimBLEUUID            getUUID();
    uint16_t              getHandle();
    std::string           toString();
    void                  dump();

    bool                  start();

    NimBLECharacteristic* createCharacteristic(const char* uuid,
                                              uint32_t properties =
                                              NIMBLE_PROPERTY::READ |
                                              NIMBLE_PROPERTY::WRITE,
                                              uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);

    NimBLECharacteristic* createCharacteristic(const NimBLEUUID &uuid,
                                               uint32_t properties =
                                               NIMBLE_PROPERTY::READ |
                                               NIMBLE_PROPERTY::WRITE,
                                               uint16_t max_len = BLE_ATT_ATTR_MAX_LEN);

    void                  addCharacteristic(NimBLECharacteristic* pCharacteristic);
    void                  removeCharacteristic(NimBLECharacteristic* pCharacteristic, bool deleteChr = false);
    NimBLECharacteristic* getCharacteristic(const char* uuid, uint16_t instanceId = 0);
    NimBLECharacteristic* getCharacteristic(const NimBLEUUID &uuid, uint16_t instanceId = 0);
    NimBLECharacteristic* getCharacteristicByHandle(uint16_t handle);

    std::vector<NimBLECharacteristic*> getCharacteristics();
    std::vector<NimBLECharacteristic*> getCharacteristics(const char* uuid);
    std::vector<NimBLECharacteristic*> getCharacteristics(const NimBLEUUID &uuid);


private:

    friend class          NimBLEServer;
    friend class          NimBLEDevice;

    uint16_t              m_handle;
    NimBLEUUID            m_uuid;
    ble_gatt_svc_def*     m_pSvcDef;
    uint8_t               m_removed;
    std::vector<NimBLECharacteristic*> m_chrVec;

}; // NimBLEService

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
#endif /* MAIN_NIMBLESERVICE_H_ */
