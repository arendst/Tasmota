/*
 * NimBLEClient.h
 *
 *  Created: on Jan 26 2020
 *      Author H2zero
 *
 * Originally:
 * BLEClient.h
 *
 *  Created on: Mar 22, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLECLIENT_H_
#define MAIN_NIMBLECLIENT_H_

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLEAddress.h"
#include "NimBLEUUID.h"
#include "NimBLEUtils.h"
#include "NimBLEAdvertisedDevice.h"
#include "NimBLERemoteService.h"

#include <vector>
#include <string>

class NimBLERemoteService;
class NimBLEClientCallbacks;
class NimBLEAdvertisedDevice;

/**
 * @brief A model of a %BLE client.
 */
class NimBLEClient {
public:
    bool                                        connect(NimBLEAdvertisedDevice* device, bool refreshServices = true);
    bool                                        connect(const NimBLEAddress &address, uint8_t type = BLE_ADDR_PUBLIC,
                                                        bool refreshServices = true);
    int                                         disconnect(uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    NimBLEAddress                               getPeerAddress();
    int                                         getRssi();
    std::vector<NimBLERemoteService*>*          getServices(bool refresh = false);
    std::vector<NimBLERemoteService*>::iterator begin();
    std::vector<NimBLERemoteService*>::iterator end();
    NimBLERemoteService*                        getService(const char* uuid);
    NimBLERemoteService*                        getService(const NimBLEUUID &uuid);
    void                                        deleteServices();
    size_t                                      deleteService(const NimBLEUUID &uuid);
    std::string                                 getValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID);
    bool                                        setValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID,
                                                         const std::string &value);
    bool                                        isConnected();
    void                                        setClientCallbacks(NimBLEClientCallbacks *pClientCallbacks,
                                                                   bool deleteCallbacks = true);
    std::string                                 toString();
    uint16_t                                    getConnId();
    uint16_t                                    getMTU();
    bool                                        secureConnection();
    void                                        setConnectTimeout(uint8_t timeout);
    void                                        setConnectionParams(uint16_t minInterval, uint16_t maxInterval,
                                                                    uint16_t latency, uint16_t timeout,
                                                                    uint16_t scanInterval=16, uint16_t scanWindow=16);
    void                                        updateConnParams(uint16_t minInterval, uint16_t maxInterval,
                                                                 uint16_t latency, uint16_t timeout);
    void                                        discoverAttributes();

private:
    NimBLEClient();
    ~NimBLEClient();

    friend class            NimBLEDevice;
    friend class            NimBLERemoteService;

    static int              handleGapEvent(struct ble_gap_event *event, void *arg);
    static int              serviceDiscoveredCB(uint16_t conn_handle,
                                                const struct ble_gatt_error *error,
                                                const struct ble_gatt_svc *service,
                                                void *arg);
    bool                    retrieveServices(const NimBLEUUID *uuid_filter = nullptr);

    NimBLEAddress           m_peerAddress = NimBLEAddress("");
    uint16_t                m_conn_id;
    bool                    m_isConnected;
    bool                    m_waitingToConnect;
    bool                    m_deleteCallbacks;
    int32_t                 m_connectTimeout;
    NimBLEClientCallbacks*  m_pClientCallbacks;
    ble_task_data_t         *m_pTaskData;

    std::vector<NimBLERemoteService*> m_servicesVector;

private:
    friend class NimBLEClientCallbacks;
    ble_gap_conn_params m_pConnParams;

}; // class NimBLEClient


/**
 * @brief Callbacks associated with a %BLE client.
 */
class NimBLEClientCallbacks {
public:
    virtual ~NimBLEClientCallbacks() {};
    virtual void onConnect(NimBLEClient* pClient);
    virtual void onDisconnect(NimBLEClient* pClient);
    virtual bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params);
    virtual uint32_t onPassKeyRequest();
    virtual void onPassKeyNotify(uint32_t pass_key);
    virtual bool onSecurityRequest();
    virtual void onAuthenticationComplete(ble_gap_conn_desc* desc);
    virtual bool onConfirmPIN(uint32_t pin);
};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif // CONFIG_BT_ENABLED
#endif /* MAIN_NIMBLECLIENT_H_ */
