/*
 * NimBLEDevice.h
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEDevice.h
 *
 *  Created on: Mar 16, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLEDEVICE_H_
#define MAIN_NIMBLEDEVICE_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#include "NimBLEScan.h"
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#include "NimBLEAdvertising.h"
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#include "NimBLEClient.h"
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#include "NimBLEServer.h"
#endif

#include "NimBLEUtils.h"
#include "NimBLESecurity.h"
#include "NimBLEAddress.h"

#include "esp_bt.h"

#include <map>
#include <string>
#include <list>

#define BLEDevice                       NimBLEDevice
#define BLEClient                       NimBLEClient
#define BLERemoteService                NimBLERemoteService
#define BLERemoteCharacteristic         NimBLERemoteCharacteristic
#define BLERemoteDescriptor             NimBLERemoteDescriptor
#define BLEAdvertisedDevice             NimBLEAdvertisedDevice
#define BLEScan                         NimBLEScan
#define BLEUUID                         NimBLEUUID
#define BLESecurity                     NimBLESecurity
#define BLESecurityCallbacks            NimBLESecurityCallbacks
#define BLEAddress                      NimBLEAddress
#define BLEUtils                        NimBLEUtils
#define BLEClientCallbacks              NimBLEClientCallbacks
#define BLEAdvertisedDeviceCallbacks    NimBLEAdvertisedDeviceCallbacks
#define BLEScanResults                  NimBLEScanResults
#define BLEServer                       NimBLEServer
#define BLEService                      NimBLEService
#define BLECharacteristic               NimBLECharacteristic
#define BLEAdvertising                  NimBLEAdvertising
#define BLEServerCallbacks              NimBLEServerCallbacks
#define BLECharacteristicCallbacks      NimBLECharacteristicCallbacks
#define BLEAdvertisementData            NimBLEAdvertisementData
#define BLEDescriptor                   NimBLEDescriptor
#define BLE2902                         NimBLE2902
#define BLE2904                         NimBLE2904
#define BLEDescriptorCallbacks          NimBLEDescriptorCallbacks
#define BLEBeacon                       NimBLEBeacon
#define BLEEddystoneTLM                 NimBLEEddystoneTLM
#define BLEEddystoneURL                 NimBLEEddystoneURL

#ifdef CONFIG_BT_NIMBLE_MAX_CONNECTIONS
#define NIMBLE_MAX_CONNECTIONS          CONFIG_BT_NIMBLE_MAX_CONNECTIONS
#else
#define NIMBLE_MAX_CONNECTIONS          CONFIG_NIMBLE_MAX_CONNECTIONS
#endif

typedef int (*gap_event_handler)(ble_gap_event *event, void *arg);

extern "C" void ble_store_config_init(void);

/**
 * @brief A model of a %BLE Device from which all the BLE roles are created.
 */
class NimBLEDevice {
public:
    static void             init(const std::string &deviceName);
    static void             deinit(bool clearAll = false);
    static bool             getInitialized();
    static NimBLEAddress    getAddress();
    static std::string      toString();

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
    static NimBLEScan*      getScan();
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    static NimBLEServer*    createServer();
    static NimBLEServer*    getServer();
#endif

    static void             setPower(esp_power_level_t powerLevel, esp_ble_power_type_t powerType=ESP_BLE_PWR_TYPE_DEFAULT);
    static int              getPower(esp_ble_power_type_t powerType=ESP_BLE_PWR_TYPE_DEFAULT);
    static void             setCustomGapHandler(gap_event_handler handler);
    static void             setSecurityAuth(bool bonding, bool mitm, bool sc);
    static void             setSecurityAuth(uint8_t auth_req);
    static void             setSecurityIOCap(uint8_t iocap);
    static void             setSecurityInitKey(uint8_t init_key);
    static void             setSecurityRespKey(uint8_t init_key);
    static void             setSecurityPasskey(uint32_t pin);
    static uint32_t         getSecurityPasskey();
    static void             setSecurityCallbacks(NimBLESecurityCallbacks* pCallbacks);
    static int              startSecurity(uint16_t conn_id);
    static int              setMTU(uint16_t mtu);
    static uint16_t         getMTU();
    static bool             isIgnored(const NimBLEAddress &address);
    static void             addIgnored(const NimBLEAddress &address);
    static void             removeIgnored(const NimBLEAddress &address);

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
    static NimBLEAdvertising* getAdvertising();
    static void               startAdvertising();
    static void               stopAdvertising();
#endif

#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
    static NimBLEClient*    createClient(NimBLEAddress peerAddress = NimBLEAddress(""));
    static bool             deleteClient(NimBLEClient* pClient);
    static NimBLEClient*    getClientByID(uint16_t conn_id);
    static NimBLEClient*    getClientByPeerAddress(const NimBLEAddress &peer_addr);
    static NimBLEClient*    getDisconnectedClient();
    static size_t           getClientListSize();
    static std::list<NimBLEClient*>* getClientList();
#endif

private:
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
    friend class NimBLEClient;
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
    friend class NimBLEScan;
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    friend class NimBLEServer;
    friend class NimBLECharacteristic;
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
    friend class NimBLEAdvertising;
#endif

    static void        onReset(int reason);
    static void        onSync(void);
    static void        host_task(void *param);
    static bool        m_synced;

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
    static NimBLEScan*                m_pScan;
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    static NimBLEServer*              m_pServer;
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
    static NimBLEAdvertising*         m_bleAdvertising;
#endif

#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
    static std::list <NimBLEClient*>  m_cList;
#endif
    static std::list <NimBLEAddress>  m_ignoreList;
    static NimBLESecurityCallbacks*   m_securityCallbacks;
    static uint32_t                   m_passkey;
    static ble_gap_event_listener     m_listener;
    static gap_event_handler          m_customGapHandler;
};


#endif // CONFIG_BT_ENABLED
#endif // MAIN_NIMBLEDEVICE_H_
