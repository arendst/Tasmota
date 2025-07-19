/*
 * NimBLEServer.h
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEServer.h
 *
 *  Created on: Apr 16, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLESERVER_H_
#define MAIN_NIMBLESERVER_H_

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#define NIMBLE_ATT_REMOVE_HIDE 1
#define NIMBLE_ATT_REMOVE_DELETE 2

#define onMtuChanged onMTUChange

#include "NimBLEUtils.h"
#include "NimBLEAddress.h"
#if CONFIG_BT_NIMBLE_EXT_ADV
#include "NimBLEExtAdvertising.h"
#else
#include "NimBLEAdvertising.h"
#endif
#include "NimBLEService.h"
#include "NimBLEConnInfo.h"


class NimBLEService;
class NimBLECharacteristic;
class NimBLEServerCallbacks;


/**
 * @brief The model of a %BLE server.
 */
class NimBLEServer {
public:
    size_t                 getConnectedCount();
    NimBLEService*         createService(const char* uuid);
    NimBLEService*         createService(const NimBLEUUID &uuid);
    void                   removeService(NimBLEService* service, bool deleteSvc = false);
    void                   addService(NimBLEService* service);
    void                   setCallbacks(NimBLEServerCallbacks* pCallbacks,
                                        bool deleteCallbacks = true);
#if CONFIG_BT_NIMBLE_EXT_ADV
    NimBLEExtAdvertising*  getAdvertising();
    bool                   startAdvertising(uint8_t inst_id,
                                            int duration = 0,
                                            int max_events = 0);
    bool                   stopAdvertising(uint8_t inst_id);
#endif
#  if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
    NimBLEAdvertising*     getAdvertising();
    bool                   startAdvertising(uint32_t duration = 0);
#endif
    bool                   stopAdvertising();
    void                   start();
    NimBLEService*         getServiceByUUID(const char* uuid, uint16_t instanceId = 0);
    NimBLEService*         getServiceByUUID(const NimBLEUUID &uuid, uint16_t instanceId = 0);
    NimBLEService*         getServiceByHandle(uint16_t handle);
    int                    disconnect(uint16_t connID,
                                      uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    int                    disconnect(const NimBLEConnInfo &connInfo,
                                      uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    void                   updateConnParams(uint16_t conn_handle,
                                            uint16_t minInterval, uint16_t maxInterval,
                                            uint16_t latency, uint16_t timeout);
    void                   setDataLen(uint16_t conn_handle, uint16_t tx_octets);
    uint16_t               getPeerMTU(uint16_t conn_id);
    std::vector<uint16_t>  getPeerDevices();
    NimBLEConnInfo         getPeerInfo(size_t index);
    NimBLEConnInfo         getPeerInfo(const NimBLEAddress& address);
    NimBLEConnInfo         getPeerIDInfo(uint16_t id);
    std::string            getPeerName(const NimBLEConnInfo& connInfo);
    void                   getPeerNameOnConnect(bool enable);
#if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
    void                   advertiseOnDisconnect(bool);
#endif

private:
    NimBLEServer();
    ~NimBLEServer();
    friend class           NimBLECharacteristic;
    friend class           NimBLEService;
    friend class           NimBLEDevice;
    friend class           NimBLEAdvertising;
#if CONFIG_BT_NIMBLE_EXT_ADV
    friend class           NimBLEExtAdvertising;
    friend class           NimBLEExtAdvertisementData;
#endif

    bool                   m_gattsStarted;
#if !CONFIG_BT_NIMBLE_EXT_ADV
    bool                   m_advertiseOnDisconnect;
#endif
    bool                   m_getPeerNameOnConnect;
    bool                   m_svcChanged;
    NimBLEServerCallbacks* m_pServerCallbacks;
    bool                   m_deleteCallbacks;
    uint16_t               m_indWait[CONFIG_BT_NIMBLE_MAX_CONNECTIONS];
    std::vector<uint16_t>  m_connectedPeersVec;

//    uint16_t               m_svcChgChrHdl; // Future use

    std::vector<NimBLEService*> m_svcVec;
    std::vector<NimBLECharacteristic*> m_notifyChrVec;

    static int             handleGapEvent(struct ble_gap_event *event, void *arg);
    static int             peerNameCB(uint16_t conn_handle, const struct ble_gatt_error *error,
                                      struct ble_gatt_attr *attr, void *arg);
    std::string            getPeerNameInternal(uint16_t conn_handle, TaskHandle_t task, int cb_type = -1);
    void                   serviceChanged();
    void                   resetGATT();
    bool                   setIndicateWait(uint16_t conn_handle);
    void                   clearIndicateWait(uint16_t conn_handle);

}; // NimBLEServer


/**
 * @brief Callbacks associated with the operation of a %BLE server.
 */
class NimBLEServerCallbacks {
public:
    virtual ~NimBLEServerCallbacks() {};

    /**
     * @brief Handle a client connection.
     * This is called when a client connects.
     * @param [in] pServer A pointer to the %BLE server that received the client connection.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information.
     * about the peer connection parameters.
     */
    virtual void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo);

    /**
     * @brief Handle a client connection.
     * This is called when a client connects.
     * @param [in] pServer A pointer to the %BLE server that received the client connection.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information.
     * @param [in] name The name of the connected peer device.
     * about the peer connection parameters.
     */
    virtual void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, std::string& name);

    /**
     * @brief Handle a client disconnection.
     * This is called when a client discconnects.
     * @param [in] pServer A pointer to the %BLE server that received the client disconnection.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     * about the peer connection parameters.
     * @param [in] reason The reason code for the disconnection.
     */
    virtual void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason);

    /**
     * @brief Called when the connection MTU changes.
     * @param [in] MTU The new MTU value.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     * about the peer connection parameters.
     */
    virtual void onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo);

    /**
     * @brief Called when a client requests a passkey for pairing (display).
     * @return The passkey to be sent to the client.
     */
    virtual uint32_t onPassKeyDisplay();

    /**
     * @brief Called when using numeric comparision for pairing.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     * Should be passed back to NimBLEDevice::injectConfirmPIN
     * @param [in] pin The pin to compare with the client.
     */
    virtual void onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pin);

    /**
     * @brief Called when the pairing procedure is complete.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     * about the peer connection parameters.
     */
    virtual void onAuthenticationComplete(const NimBLEConnInfo& connInfo);

    /**
     * @brief Called when the pairing procedure is complete.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     * @param [in] name The name of the connected peer device.
     * about the peer connection parameters.
     */
    virtual void onAuthenticationComplete(const NimBLEConnInfo& connInfo, const std::string& name);

    /**
     * @brief Called when the peer identity address is resolved.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     */
    virtual void onIdentity(const NimBLEConnInfo& connInfo);
}; // NimBLEServerCallbacks

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
#endif /* MAIN_NIMBLESERVER_H_ */
