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
class NimBLERemoteCharacteristic;
class NimBLEClientCallbacks;
class NimBLEAdvertisedDevice;

/**
 * @brief A model of a %BLE client.
 */
class NimBLEClient {
public:
    bool                                        connect(NimBLEAdvertisedDevice* device, bool deleteAttibutes = true);
    bool                                        connect(const NimBLEAddress &address, bool deleteAttibutes = true);
    bool                                        connect(bool deleteAttibutes = true);
    int                                         disconnect(uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    NimBLEAddress                               getPeerAddress();
    void                                        setPeerAddress(const NimBLEAddress &address);
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
                                                         const std::string &value, bool response = false);
    NimBLERemoteCharacteristic*                 getCharacteristic(const uint16_t handle);
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
    NimBLEClient(const NimBLEAddress &peerAddress);
    ~NimBLEClient();

    friend class            NimBLEDevice;
    friend class            NimBLERemoteService;

    static int              handleGapEvent(struct ble_gap_event *event, void *arg);
    static int              serviceDiscoveredCB(uint16_t conn_handle,
                                                const struct ble_gatt_error *error,
                                                const struct ble_gatt_svc *service,
                                                void *arg);
    static void             dcTimerCb(ble_npl_event *event);
    bool                    retrieveServices(const NimBLEUUID *uuid_filter = nullptr);

    NimBLEAddress           m_peerAddress;
    uint16_t                m_conn_id;
    bool                    m_connEstablished;
    bool                    m_deleteCallbacks;
    int32_t                 m_connectTimeout;
    NimBLEClientCallbacks*  m_pClientCallbacks;
    ble_task_data_t*        m_pTaskData;
    ble_npl_callout         m_dcTimer;

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

    /**
     * @brief Called after client connects.
     * @param [in] pClient A pointer to the calling client object.
     */
    virtual void onConnect(NimBLEClient* pClient);

    /**
     * @brief Called when disconnected from the server.
     * @param [in] pClient A pointer to the calling client object.
     */
    virtual void onDisconnect(NimBLEClient* pClient);

    /**
     * @brief Called when server requests to update the connection parameters.
     * @param [in] pClient A pointer to the calling client object.
     * @param [in] params A pointer to the struct containing the connection parameters requested.
     * @return True to accept the parmeters.
     */
    virtual bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params);

    /**
     * @brief Called when server requests a passkey for pairing.
     * @return The passkey to be sent to the server.
     */
    virtual uint32_t onPassKeyRequest();

    /*virtual void onPassKeyNotify(uint32_t pass_key);
    virtual bool onSecurityRequest();*/

    /**
     * @brief Called when the pairing procedure is complete.
     * @param [in] desc A pointer to the struct containing the connection information.\n
     * This can be used to check the status of the connection encryption/pairing.
     */
    virtual void onAuthenticationComplete(ble_gap_conn_desc* desc);

    /**
     * @brief Called when using numeric comparision for pairing.
     * @param [in] pin The pin to compare with the server.
     * @return True to accept the pin.
     */
    virtual bool onConfirmPIN(uint32_t pin);
};

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif // CONFIG_BT_ENABLED
#endif /* MAIN_NIMBLECLIENT_H_ */
