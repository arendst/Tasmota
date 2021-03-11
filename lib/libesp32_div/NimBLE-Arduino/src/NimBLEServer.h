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
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEUtils.h"
#include "NimBLEAddress.h"
#include "NimBLEAdvertising.h"
#include "NimBLEService.h"
#include "NimBLESecurity.h"


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
    NimBLEService*         createService(const NimBLEUUID &uuid, uint32_t numHandles=15,
                                         uint8_t inst_id=0);
    void                   removeService(NimBLEService* service, bool deleteSvc = false);
    void                   addService(NimBLEService* service);
    NimBLEAdvertising*     getAdvertising();
    void                   setCallbacks(NimBLEServerCallbacks* pCallbacks,
                                        bool deleteCallbacks = true);
    void                   startAdvertising();
    void                   stopAdvertising();
    void                   start();
    NimBLEService*         getServiceByUUID(const char* uuid, uint16_t instanceId = 0);
    NimBLEService*         getServiceByUUID(const NimBLEUUID &uuid, uint16_t instanceId = 0);
    NimBLEService*         getServiceByHandle(uint16_t handle);
    int                    disconnect(uint16_t connID,
                                      uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    void                   updateConnParams(uint16_t conn_handle,
                                            uint16_t minInterval, uint16_t maxInterval,
                                            uint16_t latency, uint16_t timeout);
    uint16_t               getPeerMTU(uint16_t conn_id);
//    std::vector<uint16_t>  getPeerDevices();
    void                   advertiseOnDisconnect(bool);

private:
    NimBLEServer();
    ~NimBLEServer();
    friend class           NimBLECharacteristic;
    friend class           NimBLEDevice;
    friend class           NimBLEAdvertising;

    bool                   m_gattsStarted;
    bool                   m_advertiseOnDisconnect;
    bool                   m_svcChanged;
    NimBLEServerCallbacks* m_pServerCallbacks;
    bool                   m_deleteCallbacks;
    std::vector<uint16_t>  m_connectedPeersVec;

//    uint16_t               m_svcChgChrHdl; // Future use

    std::vector<NimBLEService*> m_svcVec;
    std::vector<NimBLECharacteristic*> m_notifyChrVec;

    static int             handleGapEvent(struct ble_gap_event *event, void *arg);
    void                   resetGATT();
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
     */
    virtual void onConnect(NimBLEServer* pServer);

    /**
     * @brief Handle a client connection.
     * This is called when a client connects.
     * @param [in] pServer A pointer to the %BLE server that received the client connection.
     * @param [in] desc A pointer to the connection description structure containig information
     * about the connection parameters.
     */
    virtual void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc);

    /**
     * @brief Handle a client disconnection.
     * This is called when a client disconnects.
     * @param [in] pServer A reference to the %BLE server that received the existing client disconnection.
     */
    virtual void onDisconnect(NimBLEServer* pServer);

     /**
     * @brief Handle a client disconnection.
     * This is called when a client discconnects.
     * @param [in] pServer A pointer to the %BLE server that received the client disconnection.
     * @param [in] desc A pointer to the connection description structure containig information
     * about the connection.
     */
    virtual void onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc);

    /**
     * @brief Called when a client requests a passkey for pairing.
     * @return The passkey to be sent to the client.
     */
    virtual uint32_t onPassKeyRequest();

    //virtual void onPassKeyNotify(uint32_t pass_key);
    //virtual bool onSecurityRequest();

    /**
     * @brief Called when the pairing procedure is complete.
     * @param [in] desc A pointer to the struct containing the connection information.\n
     * This can be used to check the status of the connection encryption/pairing.
     */
    virtual void onAuthenticationComplete(ble_gap_conn_desc* desc);

    /**
     * @brief Called when using numeric comparision for pairing.
     * @param [in] pin The pin to compare with the client.
     * @return True to accept the pin.
     */
    virtual bool onConfirmPIN(uint32_t pin);
}; // NimBLEServerCallbacks


#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
#endif /* MAIN_NIMBLESERVER_H_ */
