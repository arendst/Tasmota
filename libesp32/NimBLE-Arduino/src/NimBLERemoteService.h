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

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLEClient.h"
#include "NimBLEUUID.h"
#include "NimBLERemoteCharacteristic.h"

#include <vector>

class NimBLEClient;
class NimBLERemoteCharacteristic;


/**
 * @brief A model of a remote %BLE service.
 */
class NimBLERemoteService {
public:
    virtual ~NimBLERemoteService();

    // Public methods
    std::vector<NimBLERemoteCharacteristic*>::iterator begin();
    std::vector<NimBLERemoteCharacteristic*>::iterator end();
    NimBLERemoteCharacteristic*               getCharacteristic(const char* uuid);
    NimBLERemoteCharacteristic*               getCharacteristic(const NimBLEUUID &uuid);
    void                                      deleteCharacteristics();
    size_t                                    deleteCharacteristic(const NimBLEUUID &uuid);
    NimBLEClient*                             getClient(void);
    //uint16_t                                  getHandle();
    NimBLEUUID                                getUUID(void);
    std::string                               getValue(const NimBLEUUID &characteristicUuid);
    bool                                      setValue(const NimBLEUUID &characteristicUuid,
                                                       const std::string &value);
    std::string                               toString(void);
    std::vector<NimBLERemoteCharacteristic*>* getCharacteristics(bool refresh = false);

private:
    // Private constructor ... never meant to be created by a user application.
    NimBLERemoteService(NimBLEClient* pClient, const struct ble_gatt_svc *service);

    // Friends
    friend class NimBLEClient;
    friend class NimBLERemoteCharacteristic;

    // Private methods
    bool                retrieveCharacteristics(const NimBLEUUID *uuid_filter = nullptr);
    static int          characteristicDiscCB(uint16_t conn_handle,
                                             const struct ble_gatt_error *error,
                                             const struct ble_gatt_chr *chr,
                                             void *arg);

    uint16_t            getStartHandle();
    uint16_t            getEndHandle();
    void                releaseSemaphores();

    // Properties

    // We maintain a vector of characteristics owned by this service.
    std::vector<NimBLERemoteCharacteristic*> m_characteristicVector;

    NimBLEClient*       m_pClient;
    NimBLEUUID          m_uuid;
    uint16_t            m_startHandle;
    uint16_t            m_endHandle;
}; // NimBLERemoteService

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* COMPONENTS_NIMBLEREMOTESERVICE_H_ */
