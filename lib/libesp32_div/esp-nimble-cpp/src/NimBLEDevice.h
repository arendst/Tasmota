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

#ifndef NIMBLE_CPP_DEVICE_H_
#define NIMBLE_CPP_DEVICE_H_

#include "nimconfig.h"
#if CONFIG_BT_ENABLED
# ifdef ESP_PLATFORM
#  ifndef CONFIG_IDF_TARGET_ESP32P4
#   include <esp_bt.h>
#  endif
# endif

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include <host/ble_gap.h>
# else
#  include <nimble/nimble/host/include/host/ble_gap.h>
# endif

/****  FIX COMPILATION ****/
# undef min
# undef max
/**************************/

# include <string>
# include <vector>

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
#  include <array>
class NimBLEClient;
# endif

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
class NimBLEScan;
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
class NimBLEExtAdvertising;
#  else
class NimBLEAdvertising;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
class NimBLEServer;
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM > 0
class NimBLEL2CAPServer;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL || CONFIG_BT_NIMBLE_ROLE_CENTRAL
class NimBLEConnInfo;
# endif

class NimBLEAddress;
class NimBLEDeviceCallbacks;

# define BLEDevice                    NimBLEDevice
# define BLEClient                    NimBLEClient
# define BLERemoteService             NimBLERemoteService
# define BLERemoteCharacteristic      NimBLERemoteCharacteristic
# define BLERemoteDescriptor          NimBLERemoteDescriptor
# define BLEAdvertisedDevice          NimBLEAdvertisedDevice
# define BLEScan                      NimBLEScan
# define BLEUUID                      NimBLEUUID
# define BLEAddress                   NimBLEAddress
# define BLEUtils                     NimBLEUtils
# define BLEClientCallbacks           NimBLEClientCallbacks
# define BLEAdvertisedDeviceCallbacks NimBLEScanCallbacks
# define BLEScanResults               NimBLEScanResults
# define BLEServer                    NimBLEServer
# define BLEService                   NimBLEService
# define BLECharacteristic            NimBLECharacteristic
# define BLEAdvertising               NimBLEAdvertising
# define BLEServerCallbacks           NimBLEServerCallbacks
# define BLECharacteristicCallbacks   NimBLECharacteristicCallbacks
# define BLEAdvertisementData         NimBLEAdvertisementData
# define BLEDescriptor                NimBLEDescriptor
# define BLE2904                      NimBLE2904
# define BLEDescriptorCallbacks       NimBLEDescriptorCallbacks
# define BLEBeacon                    NimBLEBeacon
# define BLEEddystoneTLM              NimBLEEddystoneTLM
# define BLEEddystoneURL              NimBLEEddystoneURL
# define BLEConnInfo                  NimBLEConnInfo
# define BLEL2CAPServer               NimBLEL2CAPServer
# define BLEL2CAPService              NimBLEL2CAPService
# define BLEL2CAPServiceCallbacks     NimBLEL2CAPServiceCallbacks
# define BLEL2CAPClient               NimBLEL2CAPClient
# define BLEL2CAPClientCallbacks      NimBLEL2CAPClientCallbacks
# define BLEL2CAPChannel              NimBLEL2CAPChannel
# define BLEL2CAPChannelCallbacks     NimBLEL2CAPChannelCallbacks

# ifdef CONFIG_BT_NIMBLE_MAX_CONNECTIONS
#  define NIMBLE_MAX_CONNECTIONS CONFIG_BT_NIMBLE_MAX_CONNECTIONS
# else
#  define NIMBLE_MAX_CONNECTIONS CONFIG_NIMBLE_MAX_CONNECTIONS
# endif

enum class NimBLETxPowerType { All = 0, Advertise = 1, Scan = 2, Connection = 3 };

typedef int (*gap_event_handler)(ble_gap_event* event, void* arg);

/**
 * @brief A model of a BLE Device from which all the BLE roles are created.
 */
class NimBLEDevice {
  public:
    static bool          init(const std::string& deviceName);
    static bool          deinit(bool clearAll = false);
    static bool          setDeviceName(const std::string& deviceName);
    static bool          isInitialized();
    static NimBLEAddress getAddress();
    static std::string   toString();
    static bool          whiteListAdd(const NimBLEAddress& address);
    static bool          whiteListRemove(const NimBLEAddress& address);
    static bool          onWhiteList(const NimBLEAddress& address);
    static size_t        getWhiteListCount();
    static NimBLEAddress getWhiteListAddress(size_t index);
    static bool          setOwnAddrType(uint8_t type);
    static bool          setOwnAddr(const NimBLEAddress& addr);
    static bool          setOwnAddr(const uint8_t* addr);
    static void          setDeviceCallbacks(NimBLEDeviceCallbacks* cb);
    static void          setScanDuplicateCacheSize(uint16_t cacheSize);
    static void          setScanFilterMode(uint8_t type);
    static void          setScanDuplicateCacheResetTime(uint16_t time);
    static bool          setCustomGapHandler(gap_event_handler handler);
    static void          setSecurityAuth(bool bonding, bool mitm, bool sc);
    static void          setSecurityAuth(uint8_t auth);
    static void          setSecurityIOCap(uint8_t iocap);
    static void          setSecurityInitKey(uint8_t initKey);
    static void          setSecurityRespKey(uint8_t respKey);
    static void          setSecurityPasskey(uint32_t passKey);
    static uint32_t      getSecurityPasskey();
    static bool          startSecurity(uint16_t connHandle, int* rcPtr = nullptr);
    static bool          setMTU(uint16_t mtu);
    static uint16_t      getMTU();
    static void          onReset(int reason);
    static void          onSync(void);
    static void          host_task(void* param);
    static int           getPower(NimBLETxPowerType type = NimBLETxPowerType::All);
    static bool          setPower(int8_t dbm, NimBLETxPowerType type = NimBLETxPowerType::All);
    static bool          setDefaultPhy(uint8_t txPhyMask, uint8_t rxPhyMask);

# ifdef ESP_PLATFORM
#  ifndef CONFIG_IDF_TARGET_ESP32P4
    static esp_power_level_t getPowerLevel(esp_ble_power_type_t powerType = ESP_BLE_PWR_TYPE_DEFAULT);
    static bool setPowerLevel(esp_power_level_t powerLevel, esp_ble_power_type_t powerType = ESP_BLE_PWR_TYPE_DEFAULT);
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
    static NimBLEScan* getScan();
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    static NimBLEServer* createServer();
    static NimBLEServer* getServer();
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM > 0
    static NimBLEL2CAPServer* createL2CAPServer();
    static NimBLEL2CAPServer* getL2CAPServer();
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL || CONFIG_BT_NIMBLE_ROLE_CENTRAL
    static bool injectConfirmPasskey(const NimBLEConnInfo& peerInfo, bool accept);
    static bool injectPassKey(const NimBLEConnInfo& peerInfo, uint32_t pin);
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
    static NimBLEExtAdvertising* getAdvertising();
    static bool                  startAdvertising(uint8_t instId, int duration = 0, int maxEvents = 0);
    static bool                  stopAdvertising(uint8_t instId);
    static bool                  stopAdvertising();
#  endif
#  if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
    static NimBLEAdvertising* getAdvertising();
    static bool               startAdvertising(uint32_t duration = 0);
    static bool               stopAdvertising();
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
    static NimBLEClient*              createClient();
    static NimBLEClient*              createClient(const NimBLEAddress& peerAddress);
    static bool                       deleteClient(NimBLEClient* pClient);
    static NimBLEClient*              getClientByHandle(uint16_t connHandle);
    static NimBLEClient*              getClientByPeerAddress(const NimBLEAddress& peerAddress);
    static NimBLEClient*              getDisconnectedClient();
    static size_t                     getCreatedClientCount();
    static std::vector<NimBLEClient*> getConnectedClients();
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL || CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    static bool          deleteBond(const NimBLEAddress& address);
    static int           getNumBonds();
    static bool          isBonded(const NimBLEAddress& address);
    static bool          deleteAllBonds();
    static NimBLEAddress getBondedAddress(int index);
# endif

  private:
    static bool                       m_synced;
    static bool                       m_initialized;
    static uint32_t                   m_passkey;
    static ble_gap_event_listener     m_listener;
    static uint8_t                    m_ownAddrType;
    static std::vector<NimBLEAddress> m_whiteList;
    static NimBLEDeviceCallbacks*     m_pDeviceCallbacks;
    static NimBLEDeviceCallbacks      defaultDeviceCallbacks;

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
    static NimBLEScan* m_pScan;
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    static NimBLEServer* m_pServer;
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM > 0
    static NimBLEL2CAPServer* m_pL2CAPServer;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
    static NimBLEExtAdvertising* m_bleAdvertising;
#  else
    static NimBLEAdvertising* m_bleAdvertising;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
    static std::array<NimBLEClient*, NIMBLE_MAX_CONNECTIONS> m_pClients;
# endif

# ifdef ESP_PLATFORM
#  if CONFIG_BTDM_BLE_SCAN_DUPL || CONFIG_BT_LE_SCAN_DUPL
    static uint16_t m_scanDuplicateSize;
    static uint8_t  m_scanFilterMode;
    static uint16_t m_scanDuplicateResetTime;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
    friend class NimBLEClient;
# endif

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
    friend class NimBLEScan;
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    friend class NimBLEServer;
    friend class NimBLECharacteristic;
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
    friend class NimBLEAdvertising;
#  if CONFIG_BT_NIMBLE_EXT_ADV
    friend class NimBLEExtAdvertising;
    friend class NimBLEExtAdvertisement;
#  endif
# endif
};

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
#  include "NimBLEClient.h"
#  include "NimBLERemoteService.h"
#  include "NimBLERemoteCharacteristic.h"
#  include "NimBLERemoteDescriptor.h"
# endif

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
#  include "NimBLEScan.h"
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#  include "NimBLEServer.h"
#  include "NimBLEService.h"
#  include "NimBLECharacteristic.h"
#  include "NimBLEDescriptor.h"
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
#   include "NimBLEL2CAPServer.h"
#   include "NimBLEL2CAPChannel.h"
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
#   include "NimBLEExtAdvertising.h"
#  else
#   include "NimBLEAdvertising.h"
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL || CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
#  include "NimBLEConnInfo.h"
# endif

# include "NimBLEAddress.h"
# include "NimBLEUtils.h"

/**
 * @brief Callbacks associated with a BLE device.
 */
class NimBLEDeviceCallbacks {
  public:
    virtual ~NimBLEDeviceCallbacks() {};

    /**
     * @brief Indicates an inability to perform a store operation.
     * This callback should do one of two things:
     *     -Address the problem and return 0, indicating that the store operation
     *      should proceed.
     *     -Return nonzero to indicate that the store operation should be aborted.
     * @param event     Describes the store event being reported.
     *                      BLE_STORE_EVENT_FULL; or
     *                      BLE_STORE_EVENT_OVERFLOW
     * @return          0 if the store operation should proceed;
     *                  nonzero if the store operation should be aborted.
     */
    virtual int onStoreStatus(struct ble_store_status_event* event, void* arg);
};

#endif // CONFIG_BT_ENABLED
#endif // NIMBLE_CPP_DEVICE_H_
