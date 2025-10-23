/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NIMBLE_CPP_CLIENT_H_
#define NIMBLE_CPP_CLIENT_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_gap.h"
# else
#  include "nimble/nimble/host/include/host/ble_gap.h"
# endif

# include "NimBLEAddress.h"

# include <stdint.h>
# include <vector>
# include <string>

class NimBLEAddress;
class NimBLEUUID;
class NimBLERemoteService;
class NimBLERemoteCharacteristic;
class NimBLEAdvertisedDevice;
class NimBLEAttValue;
class NimBLEClientCallbacks;
class NimBLEConnInfo;
struct NimBLETaskData;

/**
 * @brief A model of a BLE client.
 */
class NimBLEClient {
  public:
# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
    bool connect(const NimBLEAdvertisedDevice* device,
                 bool                          deleteAttributes = true,
                 bool                          asyncConnect     = false,
                 bool                          exchangeMTU      = true);
# endif
    bool connect(const NimBLEAddress& address, bool deleteAttributes = true, bool asyncConnect = false, bool exchangeMTU = true);
    bool           connect(bool deleteAttributes = true, bool asyncConnect = false, bool exchangeMTU = true);
    bool           disconnect(uint8_t reason = BLE_ERR_REM_USER_CONN_TERM);
    bool           cancelConnect() const;
    void           setSelfDelete(bool deleteOnDisconnect, bool deleteOnConnectFail);
    NimBLEAddress  getPeerAddress() const;
    bool           setPeerAddress(const NimBLEAddress& address);
    int            getRssi() const;
    bool           isConnected() const;
    void           setClientCallbacks(NimBLEClientCallbacks* pClientCallbacks, bool deleteCallbacks = true);
    std::string    toString() const;
    uint16_t       getConnHandle() const;
    uint16_t       getMTU() const;
    bool           exchangeMTU();
    bool           secureConnection(bool async = false) const;
    void           setConnectTimeout(uint32_t timeout);
    bool           setDataLen(uint16_t txOctets);
    bool           discoverAttributes();
    NimBLEConnInfo getConnInfo() const;
    int            getLastError() const;
    bool           updateConnParams(uint16_t minInterval, uint16_t maxInterval, uint16_t latency, uint16_t timeout);
    void           setConnectionParams(uint16_t minInterval,
                                       uint16_t maxInterval,
                                       uint16_t latency,
                                       uint16_t timeout,
                                       uint16_t scanInterval = 16,
                                       uint16_t scanWindow   = 16);
    const std::vector<NimBLERemoteService*>&    getServices(bool refresh = false);
    std::vector<NimBLERemoteService*>::iterator begin();
    std::vector<NimBLERemoteService*>::iterator end();
    NimBLERemoteCharacteristic*                 getCharacteristic(uint16_t handle);
    NimBLERemoteService*                        getService(const char* uuid);
    NimBLERemoteService*                        getService(const NimBLEUUID& uuid);
    void                                        deleteServices();
    size_t                                      deleteService(const NimBLEUUID& uuid);
    NimBLEAttValue getValue(const NimBLEUUID& serviceUUID, const NimBLEUUID& characteristicUUID);
    bool           setValue(const NimBLEUUID&     serviceUUID,
                            const NimBLEUUID&     characteristicUUID,
                            const NimBLEAttValue& value,
                            bool                  response = false);

# if CONFIG_BT_NIMBLE_EXT_ADV
    void setConnectPhy(uint8_t phyMask);
# endif
    bool updatePhy(uint8_t txPhysMask, uint8_t rxPhysMask, uint16_t phyOptions = 0);
    bool getPhy(uint8_t* txPhy, uint8_t* rxPhy);

    struct Config {
        uint8_t deleteCallbacks : 1;     // Delete the callback object when the client is deleted.
        uint8_t deleteOnDisconnect : 1;  // Delete the client when disconnected.
        uint8_t deleteOnConnectFail : 1; // Delete the client when a connection attempt fails.
        uint8_t asyncConnect : 1;        // Connect asynchronously.
        uint8_t exchangeMTU : 1;         // Exchange MTU after connection.
    };

    Config getConfig() const;
    void   setConfig(Config config);

  private:
    NimBLEClient(const NimBLEAddress& peerAddress);
    ~NimBLEClient();
    NimBLEClient(const NimBLEClient&)            = delete;
    NimBLEClient& operator=(const NimBLEClient&) = delete;

    bool       retrieveServices(const NimBLEUUID* uuidFilter = nullptr);
    static int handleGapEvent(struct ble_gap_event* event, void* arg);
    static int exchangeMTUCb(uint16_t conn_handle, const ble_gatt_error* error, uint16_t mtu, void* arg);
    static int serviceDiscoveredCB(uint16_t                     connHandle,
                                   const struct ble_gatt_error* error,
                                   const struct ble_gatt_svc*   service,
                                   void*                        arg);

    NimBLEAddress                     m_peerAddress;
    mutable int                       m_lastErr;
    int32_t                           m_connectTimeout;
    mutable NimBLETaskData*           m_pTaskData;
    std::vector<NimBLERemoteService*> m_svcVec;
    NimBLEClientCallbacks*            m_pClientCallbacks;
    uint16_t                          m_connHandle;
    uint8_t                           m_terminateFailCount;
    mutable uint8_t                   m_asyncSecureAttempt;
    Config                            m_config;

# if CONFIG_BT_NIMBLE_EXT_ADV
    uint8_t m_phyMask;
# endif
    ble_gap_conn_params m_connParams;

    friend class NimBLEDevice;
    friend class NimBLEServer;
}; // class NimBLEClient

/**
 * @brief Callbacks associated with a %BLE client.
 */
class NimBLEClientCallbacks {
  public:
    virtual ~NimBLEClientCallbacks() {};

    /**
     * @brief Called after client connects.
     * @param [in] pClient A pointer to the connecting client object.
     */
    virtual void onConnect(NimBLEClient* pClient);

    /**
     * @brief Called when a connection attempt fails.
     * @param [in] pClient A pointer to the connecting client object.
     * @param [in] reason Contains the reason code for the connection failure.
     */
    virtual void onConnectFail(NimBLEClient* pClient, int reason);

    /**
     * @brief Called when disconnected from the server.
     * @param [in] pClient A pointer to the calling client object.
     * @param [in] reason Contains the reason code for the disconnection.
     */
    virtual void onDisconnect(NimBLEClient* pClient, int reason);

    /**
     * @brief Called when server requests to update the connection parameters.
     * @param [in] pClient A pointer to the calling client object.
     * @param [in] params A pointer to the struct containing the connection parameters requested.
     * @return True to accept the parameters.
     */
    virtual bool onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params);

    /**
     * @brief Called when server requests a passkey for pairing.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
     */
    virtual void onPassKeyEntry(NimBLEConnInfo& connInfo);

    /**
     * @brief Called when the pairing procedure is complete.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.\n
     * This can be used to check the status of the connection encryption/pairing.
     */
    virtual void onAuthenticationComplete(NimBLEConnInfo& connInfo);

    /**
     * @brief Called when using numeric comparision for pairing.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
     * @param [in] pin The pin to compare with the server.
     */
    virtual void onConfirmPasskey(NimBLEConnInfo& connInfo, uint32_t pin);

    /**
     * @brief Called when the peer identity address is resolved.
     * @param [in] connInfo A reference to a NimBLEConnInfo instance with information
     */
    virtual void onIdentity(NimBLEConnInfo& connInfo);

    /**
     * @brief Called when the connection MTU changes.
     * @param [in] pClient A pointer to the client that the MTU change is associated with.
     * @param [in] MTU The new MTU value.
     * about the peer connection parameters.
     */
    virtual void onMTUChange(NimBLEClient* pClient, uint16_t MTU);

    /**
     * @brief Called when the PHY update procedure is complete.
     * @param [in] pClient A pointer to the client whose PHY was updated.
     * about the peer connection parameters.
     * @param [in] txPhy The transmit PHY.
     * @param [in] rxPhy The receive PHY.
     * Possible values:
     * * BLE_GAP_LE_PHY_1M
     * * BLE_GAP_LE_PHY_2M
     * * BLE_GAP_LE_PHY_CODED
     */
    virtual void onPhyUpdate(NimBLEClient* pClient, uint8_t txPhy, uint8_t rxPhy);
};

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL
#endif // NIMBLE_CPP_CLIENT_H_
