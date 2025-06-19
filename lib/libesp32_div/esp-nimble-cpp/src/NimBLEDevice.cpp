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

#include "NimBLEDevice.h"
#if CONFIG_BT_ENABLED

# ifdef ESP_PLATFORM
#  include "esp_err.h"
#  ifndef CONFIG_IDF_TARGET_ESP32P4
#   include "esp_bt.h"
#  endif
#  include "nvs_flash.h"
#  if defined(CONFIG_NIMBLE_CPP_IDF)
#   if (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0) || CONFIG_BT_NIMBLE_LEGACY_VHCI_ENABLE)
#    include "esp_nimble_hci.h"
#   endif
#   include "nimble/nimble_port.h"
#   include "nimble/nimble_port_freertos.h"
#   include "host/ble_hs.h"
#   include "host/ble_hs_pvcy.h"
#   include "host/util/util.h"
#   include "services/gap/ble_svc_gap.h"
#   include "services/gatt/ble_svc_gatt.h"
#  else
#   include "nimble/esp_port/esp-hci/include/esp_nimble_hci.h"
#  endif
# else
#  include "nimble/nimble/controller/include/controller/ble_phy.h"
# endif

# ifndef CONFIG_NIMBLE_CPP_IDF
#  include "nimble/porting/nimble/include/nimble/nimble_port.h"
#  include "nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h"
#  include "nimble/nimble/host/include/host/ble_hs.h"
#  include "nimble/nimble/host/include/host/ble_hs_pvcy.h"
#  include "nimble/nimble/host/util/include/host/util/util.h"
#  include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
#  include "nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h"
# endif

# if defined(ESP_PLATFORM) && defined(CONFIG_ENABLE_ARDUINO_DEPENDS)
#  include "esp32-hal-bt.h"
# endif

# include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEDevice";

extern "C" void ble_store_config_init(void);

/**
 * Singletons for the NimBLEDevice.
 */
NimBLEDeviceCallbacks  NimBLEDevice::defaultDeviceCallbacks{};
NimBLEDeviceCallbacks* NimBLEDevice::m_pDeviceCallbacks = &defaultDeviceCallbacks;

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
NimBLEScan* NimBLEDevice::m_pScan = nullptr;
# endif

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
NimBLEServer* NimBLEDevice::m_pServer = nullptr;
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM > 0
NimBLEL2CAPServer* NimBLEDevice::m_pL2CAPServer = nullptr;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
NimBLEExtAdvertising* NimBLEDevice::m_bleAdvertising = nullptr;
#  else
NimBLEAdvertising* NimBLEDevice::m_bleAdvertising = nullptr;
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
std::array<NimBLEClient*, NIMBLE_MAX_CONNECTIONS> NimBLEDevice::m_pClients{};
# endif

bool                       NimBLEDevice::m_initialized{false};
uint32_t                   NimBLEDevice::m_passkey{123456};
bool                       NimBLEDevice::m_synced{false};
ble_gap_event_listener     NimBLEDevice::m_listener{};
std::vector<NimBLEAddress> NimBLEDevice::m_whiteList{};
uint8_t                    NimBLEDevice::m_ownAddrType{BLE_OWN_ADDR_PUBLIC};

# ifdef ESP_PLATFORM
#  if CONFIG_BTDM_BLE_SCAN_DUPL
uint16_t NimBLEDevice::m_scanDuplicateSize{CONFIG_BTDM_SCAN_DUPL_CACHE_SIZE};
uint8_t  NimBLEDevice::m_scanFilterMode{CONFIG_BTDM_SCAN_DUPL_TYPE};
uint16_t NimBLEDevice::m_scanDuplicateResetTime{0};
#  elif CONFIG_BT_LE_SCAN_DUPL
uint16_t       NimBLEDevice::m_scanDuplicateSize{CONFIG_BT_LE_LL_DUP_SCAN_LIST_COUNT};
uint8_t        NimBLEDevice::m_scanFilterMode{CONFIG_BT_LE_SCAN_DUPL_TYPE};
uint16_t       NimBLEDevice::m_scanDuplicateResetTime{0};
extern "C" int ble_vhci_disc_duplicate_set_max_cache_size(int max_cache_size);
extern "C" int ble_vhci_disc_duplicate_set_period_refresh_time(int refresh_period_time);
extern "C" int ble_vhci_disc_duplicate_mode_disable(int mode);
extern "C" int ble_vhci_disc_duplicate_mode_enable(int mode);
#  endif
# endif

/* -------------------------------------------------------------------------- */
/*                              SERVER FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
/**
 * @brief Create an instance of a server.
 * @return A pointer to the instance of the server.
 */
NimBLEServer* NimBLEDevice::createServer() {
    if (NimBLEDevice::m_pServer == nullptr) {
        NimBLEDevice::m_pServer = new NimBLEServer();
        ble_gatts_reset();
        ble_svc_gap_init();
        ble_svc_gatt_init();
    }

    return m_pServer;
} // createServer

/**
 * @brief Get the instance of the server.
 * @return A pointer to the server instance or nullptr if none have been created.
 */
NimBLEServer* NimBLEDevice::getServer() {
    return m_pServer;
} // getServer

#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
/**
 * @brief Create an instance of a L2CAP server.
 * @return A pointer to the instance of the L2CAP server.
 */
NimBLEL2CAPServer* NimBLEDevice::createL2CAPServer() {
    if (NimBLEDevice::m_pL2CAPServer == nullptr) {
        NimBLEDevice::m_pL2CAPServer = new NimBLEL2CAPServer();
    }
    return m_pL2CAPServer;
} // createL2CAPServer

/**
 * @brief Get the instance of the L2CAP server.
 * @return A pointer to the L2CAP server instance or nullptr if none have been created.
 */
NimBLEL2CAPServer* NimBLEDevice::getL2CAPServer() {
    return m_pL2CAPServer;
} // getL2CAPServer
#  endif // #if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
# endif  // #if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

/* -------------------------------------------------------------------------- */
/*                            ADVERTISING FUNCTIONS                           */
/* -------------------------------------------------------------------------- */

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
#  if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Get the instance of the extended advertising object.
 * @return A pointer to the extended advertising object.
 */
NimBLEExtAdvertising* NimBLEDevice::getAdvertising() {
    if (m_bleAdvertising == nullptr) {
        m_bleAdvertising = new NimBLEExtAdvertising();
    }

    return m_bleAdvertising;
}

/**
 * @brief Convenience function to begin advertising.
 * @param [in] instId The extended advertisement instance ID to start.
 * @param [in] duration How long to advertise for in milliseconds, 0 = forever (default).
 * @param [in] maxEvents Maximum number of advertisement events to send, 0 = no limit (default).
 * @return True if advertising started successfully.
 */
bool NimBLEDevice::startAdvertising(uint8_t instId, int duration, int maxEvents) {
    return getAdvertising()->start(instId, duration, maxEvents);
} // startAdvertising

/**
 * @brief Convenience function to stop advertising a data set.
 * @param [in] instId The extended advertisement instance ID to stop advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEDevice::stopAdvertising(uint8_t instId) {
    return getAdvertising()->stop(instId);
} // stopAdvertising
#  endif

#  if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
/**
 * @brief Get the instance of the advertising object.
 * @return A pointer to the advertising object.
 */
NimBLEAdvertising* NimBLEDevice::getAdvertising() {
    if (m_bleAdvertising == nullptr) {
        m_bleAdvertising = new NimBLEAdvertising();
    }
    return m_bleAdvertising;
}

/**
 * @brief Convenience function to begin advertising.
 * @param [in] duration The duration in milliseconds to advertise for, default = forever.
 * @return True if advertising started successfully.
 */
bool NimBLEDevice::startAdvertising(uint32_t duration) {
    return getAdvertising()->start(duration);
} // startAdvertising
#  endif

/**
 * @brief Convenience function to stop all advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEDevice::stopAdvertising() {
    return getAdvertising()->stop();
} // stopAdvertising
# endif // #if CONFIG_BT_NIMBLE_ROLE_BROADCASTER

/* -------------------------------------------------------------------------- */
/*                               SCAN FUNCTIONS                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Retrieve the Scan object that we use for scanning.
 * @return The scanning object reference.  This is a singleton object.  The caller should not
 * try and release/delete it.
 */
# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
NimBLEScan* NimBLEDevice::getScan() {
    if (m_pScan == nullptr) {
        m_pScan = new NimBLEScan();
    }

    return m_pScan;
} // getScan

#  ifdef ESP_PLATFORM
#   if CONFIG_BTDM_BLE_SCAN_DUPL || CONFIG_BT_LE_SCAN_DUPL
/**
 * @brief Set the duplicate filter cache size for filtering scanned devices.
 * @param [in] size The number of advertisements filtered before the cache is reset.\n
 * Range is 10-1000, a larger value will reduce how often the same devices are reported.
 * @details Must only be called before calling NimBLEDevice::init.
 */
void NimBLEDevice::setScanDuplicateCacheSize(uint16_t size) {
    if (m_initialized) {
        NIMBLE_LOGE(LOG_TAG, "Cannot change scan cache size while initialized");
        return;
    } else {
        if (size > 1000) {
            size = 1000;
        } else if (size < 10) {
            size = 10;
        }
    }

    NIMBLE_LOGD(LOG_TAG, "Set duplicate cache size to: %u", size);
    m_scanDuplicateSize = size;
}

/**
 * @brief Set the duplicate filter mode for filtering scanned devices.
 * @param [in] mode One of three possible options:
 * * CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE (0) (default)\n
     Filter by device address only, advertisements from the same address will be reported only once.
 * * CONFIG_BTDM_SCAN_DUPL_TYPE_DATA (1)\n
     Filter by data only, advertisements with the same data will only be reported once,\n
     even from different addresses.
 * * CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE (2)\n
     Filter by address and data, advertisements from the same address will be reported only once,\n
     except if the data in the advertisement has changed, then it will be reported again.
 * @details Must only be called before calling NimBLEDevice::init.
 */
void NimBLEDevice::setScanFilterMode(uint8_t mode) {
    if (m_initialized) {
        NIMBLE_LOGE(LOG_TAG, "Cannot change scan duplicate type while initialized");
        return;
    } else if (mode > 2) {
        NIMBLE_LOGE(LOG_TAG, "Invalid scan duplicate type");
        return;
    }

    m_scanFilterMode = mode;
}

/**
 * @brief Set the time in seconds to reset the duplicate cache.
 * @param [in] time The time in seconds to reset the cache.
 * @details When the cache is reset all scanned devices will be reported again
 * even if already seen in the current scan. If set to 0 the cache will never be reset.
 */
void NimBLEDevice::setScanDuplicateCacheResetTime(uint16_t time) {
    if (m_initialized) {
        NIMBLE_LOGE(LOG_TAG, "Cannot change scan cache reset time while initialized");
        return;
    } else if (time > 1000) {
        NIMBLE_LOGE(LOG_TAG, "Invalid scan cache reset time");
        return;
    }

    NIMBLE_LOGD(LOG_TAG, "Set duplicate cache reset time to: %u", time);
    m_scanDuplicateResetTime = time;
}
#   endif // CONFIG_BTDM_BLE_SCAN_DUPL || CONFIG_BT_LE_SCAN_DUPL
#  endif  // ESP_PLATFORM
# endif   // CONFIG_BT_NIMBLE_ROLE_OBSERVER

/* -------------------------------------------------------------------------- */
/*                              CLIENT FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
/**
 * @brief Creates a new client object, each client can connect to 1 peripheral device.
 * @return A pointer to the new client object, or nullptr on error.
 */
NimBLEClient* NimBLEDevice::createClient() {
    return createClient(NimBLEAddress{});
} // createClient

/**
 * @brief Creates a new client object, each client can connect to 1 peripheral device.
 * @param [in] peerAddress A peer address reference that is copied to the new client
 * object, allows for calling NimBLEClient::connect(bool) without a device or address parameter.
 * @return A pointer to the new client object, or nullptr on error.
 */
NimBLEClient* NimBLEDevice::createClient(const NimBLEAddress& peerAddress) {
    for (auto& clt : m_pClients) {
        if (clt == nullptr) {
            clt = new NimBLEClient(peerAddress);
            return clt;
        }
    }

    NIMBLE_LOGE(LOG_TAG, "Unable to create client; already at max: %d", NIMBLE_MAX_CONNECTIONS);
    return nullptr;
} // createClient

/**
 * @brief Delete the client object and remove it from the list.\n
 * Checks if it is connected or trying to connect and disconnects/stops it first.
 * @param [in] pClient A pointer to the client object.
 */
bool NimBLEDevice::deleteClient(NimBLEClient* pClient) {
    if (pClient == nullptr) {
        return false;
    }

    for (auto& clt : m_pClients) {
        if (clt == pClient) {
            if (clt->isConnected()) {
                clt->m_config.deleteOnDisconnect = true;
                if (!clt->disconnect()) {
                    break;
                }
            } else if (pClient->m_pTaskData != nullptr) {
                clt->m_config.deleteOnConnectFail = true;
                if (!clt->cancelConnect()) {
                    break;
                }
            } else {
                delete clt;
                clt = nullptr;
            }

            return true;
        }
    }

    return false;
} // deleteClient

/**
 * @brief Get the number of created client objects.
 * @return Number of client objects created.
 */
size_t NimBLEDevice::getCreatedClientCount() {
    size_t count = 0;
    for (const auto clt : m_pClients) {
        if (clt != nullptr) {
            count++;
        }
    }

    return count;
} // getCreatedClientCount

/**
 * @brief Get a reference to a client by connection handle.
 * @param [in] connHandle The client connection handle to search for.
 * @return A pointer to the client object with the specified connection handle or nullptr.
 */
NimBLEClient* NimBLEDevice::getClientByHandle(uint16_t connHandle) {
    for (const auto clt : m_pClients) {
        if (clt != nullptr && clt->getConnHandle() == connHandle) {
            return clt;
        }
    }

    return nullptr;
} // getClientByHandle

/**
 * @brief Get a reference to a client by peer address.
 * @param [in] addr The address of the peer to search for.
 * @return A pointer to the client object with the peer address or nullptr.
 */
NimBLEClient* NimBLEDevice::getClientByPeerAddress(const NimBLEAddress& addr) {
    for (const auto clt : m_pClients) {
        if (clt != nullptr && clt->getPeerAddress() == addr) {
            return clt;
        }
    }

    return nullptr;
} // getClientPeerAddress

/**
 * @brief Finds the first disconnected client available.
 * @return A pointer to the first client object that is not connected to a peer or nullptr.
 */
NimBLEClient* NimBLEDevice::getDisconnectedClient() {
    for (const auto clt : m_pClients) {
        if (clt != nullptr && !clt->isConnected()) {
            return clt;
        }
    }

    return nullptr;
} // getDisconnectedClient

/**
 * @brief Get a list of connected clients.
 * @return A vector of connected client objects.
 */
std::vector<NimBLEClient*> NimBLEDevice::getConnectedClients() {
    std::vector<NimBLEClient*> clients;
    for (const auto clt : m_pClients) {
        if (clt != nullptr && clt->isConnected()) {
            clients.push_back(clt);
        }
    }

    return clients;
} // getConnectedClients

# endif // CONFIG_BT_NIMBLE_ROLE_CENTRAL

/* -------------------------------------------------------------------------- */
/*                               TRANSMIT POWER                               */
/* -------------------------------------------------------------------------- */

# ifdef ESP_PLATFORM
#  ifndef CONFIG_IDF_TARGET_ESP32P4
/**
 * @brief Get the transmission power.
 * @return The power level currently used in esp_power_level_t or a negative value on error.
 */
esp_power_level_t NimBLEDevice::getPowerLevel(esp_ble_power_type_t powerType) {
    esp_power_level_t pwr = esp_ble_tx_power_get(powerType);

#   if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
    // workaround for bug when "enhanced tx power" was added
    if (pwr == 0xFF) {
        pwr = esp_ble_tx_power_get(ESP_BLE_PWR_TYPE_CONN_HDL3);
    }
#   endif

    return pwr;
} // getPowerLevel

/**
 * @brief Set the transmission power.
 * @param [in] powerLevel The power level to set in esp_power_level_t.
 * @return True if the power level was set successfully.
 */
bool NimBLEDevice::setPowerLevel(esp_power_level_t powerLevel, esp_ble_power_type_t powerType) {
    esp_err_t errRc = esp_ble_tx_power_set(powerType, powerLevel);
    if (errRc != ESP_OK) {
        NIMBLE_LOGE(LOG_TAG, "esp_ble_tx_power_set: rc=%d", errRc);
    }

    return errRc == ESP_OK;
} // setPowerLevel
#  endif // !CONFIG_IDF_TARGET_ESP32P4
# endif  // ESP_PLATFORM

/**
 * @brief Set the transmission power.
 * @param [in] dbm The power level to set in dBm.
 * @return True if the power level was set successfully.
 */
bool NimBLEDevice::setPower(int8_t dbm, NimBLETxPowerType type) {
# ifdef ESP_PLATFORM
#  ifdef CONFIG_IDF_TARGET_ESP32P4
    return false; // CONFIG_IDF_TARGET_ESP32P4 does not support esp_ble_tx_power_set
#  else
    if (dbm % 3 == 2) {
        dbm++; // round up to the next multiple of 3 to be able to target 20dbm
    }

    bool              success = false;
    esp_power_level_t espPwr  = static_cast<esp_power_level_t>(dbm / 3 + ESP_PWR_LVL_N0);
    if (type == NimBLETxPowerType::All) {
        success  = setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_ADV);
        success &= setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_SCAN);
        success &= setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_DEFAULT);
    } else if (type == NimBLETxPowerType::Advertise) {
        success = setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_ADV);
    } else if (type == NimBLETxPowerType::Scan) {
        success = setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_SCAN);
    } else if (type == NimBLETxPowerType::Connection) {
        success = setPowerLevel(espPwr, ESP_BLE_PWR_TYPE_DEFAULT);
    }

    return success;
#  endif
# else
    (void)type; // unused
    NIMBLE_LOGD(LOG_TAG, ">> setPower: %d", dbm);
    int rc = ble_phy_tx_power_set(dbm);
    if (rc) {
        NIMBLE_LOGE(LOG_TAG, "failed to set TX power, rc: %04x\n", rc);
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "TX power set to %d dBm\n", dbm);
    return true;
# endif
} // setPower

/**
 * @brief Get the transmission power.
 * @return The power level currently used in dbm or 0xFF on error.
 */
int NimBLEDevice::getPower(NimBLETxPowerType type) {
# ifdef ESP_PLATFORM
#  ifdef CONFIG_IDF_TARGET_ESP32P4
    return 0xFF; // CONFIG_IDF_TARGET_ESP32P4 does not support esp_ble_tx_power_get
#  else
    esp_ble_power_type_t espPwr = type == NimBLETxPowerType::Advertise ? ESP_BLE_PWR_TYPE_ADV
                                  : type == NimBLETxPowerType::Scan    ? ESP_BLE_PWR_TYPE_SCAN
                                                                       : ESP_BLE_PWR_TYPE_DEFAULT;

    int pwr = getPowerLevel(espPwr);
    if (pwr < 0) {
        NIMBLE_LOGE(LOG_TAG, "esp_ble_tx_power_get failed rc=%d", pwr);
        return 0xFF;
    }

    if (pwr < ESP_PWR_LVL_N0) {
        return -3 * (ESP_PWR_LVL_N0 - pwr);
    }

    if (pwr > ESP_PWR_LVL_N0) {
        return std::min<int>((pwr - ESP_PWR_LVL_N0) * 3, 20);
    }

    return 0;
#  endif
# else
    (void)type; // unused
    return ble_phy_tx_power_get();
# endif
} // getPower

/* -------------------------------------------------------------------------- */
/*                                MTU FUNCTIONS                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Setup local mtu that will be used to negotiate mtu during request from client peer.
 * @param [in] mtu Value to set local mtu:
 * * This should be larger than 23 and lower or equal to BLE_ATT_MTU_MAX = 527.
 * @return True if the mtu was set successfully.
 */
bool NimBLEDevice::setMTU(uint16_t mtu) {
    int rc = ble_att_set_preferred_mtu(mtu);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Could not set local mtu value to: %d, rc: %d", mtu, rc);
    }

    return rc == 0;
} // setMTU

/**
 * @brief Get local MTU value set.
 * @return The current preferred MTU setting.
 */
uint16_t NimBLEDevice::getMTU() {
    return ble_att_preferred_mtu();
}

/* -------------------------------------------------------------------------- */
/*                               BOND MANAGEMENT                              */
/* -------------------------------------------------------------------------- */

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL || CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
/**
 * @brief Gets the number of bonded peers stored
 */
int NimBLEDevice::getNumBonds() {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int        num_peers, rc;
    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return 0;
    }

    return num_peers;
}

/**
 * @brief Deletes all bonding information.
 * @returns True on success.
 */
bool NimBLEDevice::deleteAllBonds() {
    int rc = ble_store_clear();
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed to delete all bonds; rc=%d", rc);
        return false;
    }
    return true;
}

/**
 * @brief Deletes a peer bond.
 * @param [in] address The address of the peer with which to delete bond info.
 * @returns True on success.
 */
bool NimBLEDevice::deleteBond(const NimBLEAddress& address) {
    return ble_gap_unpair(address.getBase()) == 0;
}

/**
 * @brief Checks if a peer device is bonded.
 * @param [in] address The address to check for bonding.
 * @returns True if bonded.
 */
bool NimBLEDevice::isBonded(const NimBLEAddress& address) {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int        num_peers, rc;

    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return false;
    }

    for (int i = 0; i < num_peers; i++) {
        NimBLEAddress storedAddr(peer_id_addrs[i]);
        if (storedAddr == address) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Get the address of a bonded peer device by index.
 * @param [in] index The index to retrieve the peer address of.
 * @returns NimBLEAddress of the found bonded peer or null address if not found.
 */
NimBLEAddress NimBLEDevice::getBondedAddress(int index) {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int        num_peers, rc;
    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0 || index > num_peers || index < 0) {
        return NimBLEAddress{};
    }

    return NimBLEAddress(peer_id_addrs[index]);
}
# endif

/* -------------------------------------------------------------------------- */
/*                                  WHITELIST                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Checks if a peer device is whitelisted.
 * @param [in] address The address to check for in the whitelist.
 * @returns True if the address is in the whitelist.
 */
bool NimBLEDevice::onWhiteList(const NimBLEAddress& address) {
    for (const auto& addr : m_whiteList) {
        if (addr == address) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Add a peer address to the whitelist.
 * @param [in] address The address to add to the whitelist.
 * @returns True if successful.
 */
bool NimBLEDevice::whiteListAdd(const NimBLEAddress& address) {
    if (!NimBLEDevice::onWhiteList(address)) {
        m_whiteList.push_back(address);
        int rc = ble_gap_wl_set(reinterpret_cast<ble_addr_t*>(&m_whiteList[0]), m_whiteList.size());
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Failed adding to whitelist rc=%d", rc);
            m_whiteList.pop_back();
            return false;
        }
    }

    return true;
}

/**
 * @brief Remove a peer address from the whitelist.
 * @param [in] address The address to remove from the whitelist.
 * @returns True if successful.
 */
bool NimBLEDevice::whiteListRemove(const NimBLEAddress& address) {
    for (auto it = m_whiteList.begin(); it < m_whiteList.end(); ++it) {
        if (*it == address) {
            m_whiteList.erase(it);
            int rc = ble_gap_wl_set(reinterpret_cast<ble_addr_t*>(&m_whiteList[0]), m_whiteList.size());
            if (rc != 0) {
                m_whiteList.push_back(address);
                NIMBLE_LOGE(LOG_TAG, "Failed removing from whitelist rc=%d", rc);
                return false;
            }

            std::vector<NimBLEAddress>(m_whiteList).swap(m_whiteList);
        }
    }

    return true;
}

/**
 * @brief Gets the count of addresses in the whitelist.
 * @returns The number of addresses in the whitelist.
 */
size_t NimBLEDevice::getWhiteListCount() {
    return m_whiteList.size();
}

/**
 * @brief Gets the address at the vector index.
 * @param [in] index The vector index to retrieve the address from.
 * @returns The NimBLEAddress at the whitelist index or null address if not found.
 */
NimBLEAddress NimBLEDevice::getWhiteListAddress(size_t index) {
    if (index > m_whiteList.size()) {
        NIMBLE_LOGE(LOG_TAG, "Invalid index; %u", index);
        return NimBLEAddress{};
    }

    return m_whiteList[index];
}

/* -------------------------------------------------------------------------- */
/*                               STACK FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Set the preferred default phy to use for connections.
 * @param [in] txPhyMask TX PHY. Can be mask of following:
 * - BLE_GAP_LE_PHY_1M_MASK
 * - BLE_GAP_LE_PHY_2M_MASK
 * - BLE_GAP_LE_PHY_CODED_MASK
 * - BLE_GAP_LE_PHY_ANY_MASK
 * @param [in] rxPhyMask RX PHY. Can be mask of following:
 * - BLE_GAP_LE_PHY_1M_MASK
 * - BLE_GAP_LE_PHY_2M_MASK
 * - BLE_GAP_LE_PHY_CODED_MASK
 * - BLE_GAP_LE_PHY_ANY_MASK
 * @return True if successful.
 */
bool NimBLEDevice::setDefaultPhy(uint8_t txPhyMask, uint8_t rxPhyMask) {
    int rc = ble_gap_set_prefered_default_le_phy(txPhyMask, rxPhyMask);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed to set default phy; rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    return rc == 0;
}

/**
 * @brief Host reset, we pass the message so we don't make calls until re-synced.
 * @param [in] reason The reason code for the reset.
 */
void NimBLEDevice::onReset(int reason) {
    if (!m_synced) {
        return;
    }

    m_synced = false;

    NIMBLE_LOGE(LOG_TAG, "Host reset; reason=%d, %s", reason, NimBLEUtils::returnCodeToString(reason));
} // onReset

/**
 * @brief Host synced with controller, all clear to make calls to the stack.
 */
void NimBLEDevice::onSync(void) {
    NIMBLE_LOGI(LOG_TAG, "NimBle host synced.");
    // This check is needed due to potentially being called multiple times in succession
    // If this happens, the call to scan start may get stuck or cause an advertising fault.
    if (m_synced) {
        return;
    }

    // Get the public and random address for the device.
    int rc = ble_hs_util_ensure_addr(0);
    if (rc == 0) {
        rc = ble_hs_util_ensure_addr(1);
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "error ensuring address; rc=%d", rc);
        return;
    }

    // start with using the public address if available, if not then use random.
    rc = ble_hs_id_copy_addr(BLE_OWN_ADDR_PUBLIC, NULL, NULL);
    if (rc != 0) {
        m_ownAddrType = BLE_OWN_ADDR_RANDOM;
    }

    // Yield for housekeeping tasks before returning to operations.
    // Occasionally triggers exception without.
    ble_npl_time_delay(1);

    m_synced = true;

    if (m_initialized) {
# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
        if (m_pScan != nullptr) {
            m_pScan->onHostSync();
        }
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
        if (m_bleAdvertising != nullptr) {
            m_bleAdvertising->onHostSync();
        }
# endif
    }
} // onSync

/**
 * @brief The main host task.
 */
void NimBLEDevice::host_task(void* param) {
    NIMBLE_LOGI(LOG_TAG, "BLE Host Task Started");
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
    nimble_port_freertos_deinit();
} // host_task

/**
 * @brief Initialize the BLE environment.
 * @param [in] deviceName The device name of the device.
 */
bool NimBLEDevice::init(const std::string& deviceName) {
    if (!m_initialized) {
# ifdef ESP_PLATFORM

#  if defined(CONFIG_ENABLE_ARDUINO_DEPENDS) && SOC_BT_SUPPORTED
        // make sure the linker includes esp32-hal-bt.c so Arduino init doesn't release BLE memory.
        btStarted();
#  endif

        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            err = nvs_flash_erase();
            if (err == ESP_OK) {
                err = nvs_flash_init();
            }
        }

        if (err != ESP_OK) {
            NIMBLE_LOGE(LOG_TAG, "nvs_flash_init() failed; err=%d", err);
            return false;
        }

#  if CONFIG_IDF_TARGET_ESP32
        esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
#  endif

#  if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0) || !defined(CONFIG_NIMBLE_CPP_IDF)
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
#   if defined(CONFIG_IDF_TARGET_ESP32)
        bt_cfg.mode         = ESP_BT_MODE_BLE;
        bt_cfg.ble_max_conn = CONFIG_BT_NIMBLE_MAX_CONNECTIONS;
#   elif defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
        bt_cfg.ble_max_act = CONFIG_BT_NIMBLE_MAX_CONNECTIONS;
#   else
        bt_cfg.nimble_max_connections = CONFIG_BT_NIMBLE_MAX_CONNECTIONS;
#   endif

#   if CONFIG_BTDM_BLE_SCAN_DUPL
        bt_cfg.normal_adv_size         = m_scanDuplicateSize;
        bt_cfg.scan_duplicate_type     = m_scanFilterMode;
        bt_cfg.dup_list_refresh_period = m_scanDuplicateResetTime;
#   elif CONFIG_BT_LE_SCAN_DUPL
        bt_cfg.ble_ll_rsp_dup_list_count = m_scanDuplicateSize;
        bt_cfg.ble_ll_adv_dup_list_count = m_scanDuplicateSize;
#   endif
        err = esp_bt_controller_init(&bt_cfg);
        if (err != ESP_OK) {
            NIMBLE_LOGE(LOG_TAG, "esp_bt_controller_init() failed; err=%d", err);
            return false;
        }

#   if CONFIG_BT_LE_SCAN_DUPL
        int mode = (1UL << 4); // FILTER_DUPLICATE_EXCEPTION_FOR_MESH
        switch (m_scanFilterMode) {
            case 1:
                mode |= (1UL << 3); // FILTER_DUPLICATE_ADVDATA
                break;
            case 2:
                mode |= ((1UL << 2) | (1UL << 3)); // FILTER_DUPLICATE_ADDRESS | FILTER_DUPLICATE_ADVDATA
                break;
            default:
                mode |= (1UL << 0) | (1UL << 2); // FILTER_DUPLICATE_PDUTYPE | FILTER_DUPLICATE_ADDRESS
        }

        ble_vhci_disc_duplicate_mode_disable(0xFFFFFFFF);
        ble_vhci_disc_duplicate_mode_enable(mode);
        ble_vhci_disc_duplicate_set_max_cache_size(m_scanDuplicateSize);
        ble_vhci_disc_duplicate_set_period_refresh_time(m_scanDuplicateResetTime);
#   endif

        err = esp_bt_controller_enable(ESP_BT_MODE_BLE);
        if (err != ESP_OK) {
            NIMBLE_LOGE(LOG_TAG, "esp_bt_controller_enable() failed; err=%d", err);
            return false;
        }

#   if CONFIG_BT_NIMBLE_LEGACY_VHCI_ENABLE
        err = esp_nimble_hci_init();
        if (err != ESP_OK) {
            NIMBLE_LOGE(LOG_TAG, "esp_nimble_hci_init() failed; err=%d", err);
            return false;
        }
#   endif
#  endif
# endif
        nimble_port_init();

        // Setup callbacks for host events
        ble_hs_cfg.reset_cb        = NimBLEDevice::onReset;
        ble_hs_cfg.sync_cb         = NimBLEDevice::onSync;
        ble_hs_cfg.store_status_cb = [](struct ble_store_status_event* event, void* arg) {
            return m_pDeviceCallbacks->onStoreStatus(event, arg);
        };

        // Set initial security capabilities
        ble_hs_cfg.sm_io_cap         = BLE_HS_IO_NO_INPUT_OUTPUT;
        ble_hs_cfg.sm_bonding        = 0;
        ble_hs_cfg.sm_mitm           = 0;
        ble_hs_cfg.sm_sc             = 1;
        ble_hs_cfg.sm_our_key_dist   = BLE_SM_PAIR_KEY_DIST_ENC;
        ble_hs_cfg.sm_their_key_dist = BLE_SM_PAIR_KEY_DIST_ENC;
# if MYNEWT_VAL(BLE_LL_CFG_FEAT_LL_PRIVACY)
        ble_hs_cfg.sm_our_key_dist   |= BLE_SM_PAIR_KEY_DIST_ID;
        ble_hs_cfg.sm_their_key_dist |= BLE_SM_PAIR_KEY_DIST_ID;
# endif

        setDeviceName(deviceName);
        ble_store_config_init();
        nimble_port_freertos_init(NimBLEDevice::host_task);
    }

    // Wait for host and controller to sync before returning and accepting new tasks
    while (!m_synced) {
        ble_npl_time_delay(1);
    }

    m_initialized = true; // Set the initialization flag to ensure we are only initialized once.
    return true;
} // init

/**
 * @brief Shutdown the NimBLE stack/controller.
 * @param [in] clearAll If true, deletes all server/advertising/scan/client objects after de-initializing.
 * @note If clearAll is true when called all objects created will be deleted and any references to the created objects become invalid.
 * If clearAll is false, the objects will remain and can be used again after re-initializing the stack.
 * If the stack was not already initialized, the objects created can be deleted when clearAll is true with no effect on the stack.
 */
bool NimBLEDevice::deinit(bool clearAll) {
    int rc = 0;
    if (m_initialized) {
        rc = nimble_port_stop();
        if (rc == 0) {
            nimble_port_deinit();
# ifdef CONFIG_NIMBLE_CPP_IDF
#  if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            rc = esp_nimble_hci_and_controller_deinit();
            if (rc != ESP_OK) {
                NIMBLE_LOGE(LOG_TAG, "esp_nimble_hci_and_controller_deinit() failed with error: %d", rc);
            }
#  endif
# endif
            m_initialized = false;
            m_synced      = false;
        }
    }

    if (clearAll) {
# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
        if (NimBLEDevice::m_pServer != nullptr) {
            delete NimBLEDevice::m_pServer;
            NimBLEDevice::m_pServer = nullptr;
        }
#  if CONFIG_BT_NIMBLE_L2CAP_COC_MAX_NUM
        if (NimBLEDevice::m_pL2CAPServer != nullptr) {
            delete NimBLEDevice::m_pL2CAPServer;
            NimBLEDevice::m_pL2CAPServer = nullptr;
        }
#  endif
# endif

# if CONFIG_BT_NIMBLE_ROLE_BROADCASTER
        if (NimBLEDevice::m_bleAdvertising != nullptr) {
            delete NimBLEDevice::m_bleAdvertising;
            NimBLEDevice::m_bleAdvertising = nullptr;
        }
# endif

# if CONFIG_BT_NIMBLE_ROLE_OBSERVER
        if (NimBLEDevice::m_pScan != nullptr) {
            delete NimBLEDevice::m_pScan;
            NimBLEDevice::m_pScan = nullptr;
        }
# endif

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
        for (auto clt : m_pClients) {
            deleteClient(clt);
        }
# endif
    }

    return rc == 0;
} // deinit

/**
 * @brief Check if the initialization is complete.
 * @return true if initialized.
 */
bool NimBLEDevice::isInitialized() {
    return m_initialized;
} // getInitialized

/* -------------------------------------------------------------------------- */
/*                             ADDRESS MANAGEMENT                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Get our device address.
 * @return A NimBLEAddress object with the currently used address, or a NULL address if not set.
 */
NimBLEAddress NimBLEDevice::getAddress() {
    ble_addr_t addr{};
    uint8_t    type = m_ownAddrType & 1; // input must be random or public, odd values are random
    int        rc   = ble_hs_id_copy_addr(type, addr.val, NULL);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "No address, rc: %d", rc);
    } else {
        addr.type = type;
    }

    return NimBLEAddress{addr};
} // getAddress

/**
 * @brief Sets the address type to use.
 * @param [in] type Bluetooth Device address type.
 * The available types are defined as:
 * * 0x00: BLE_OWN_ADDR_PUBLIC - Public address; Uses the hardware static address.
 * * 0x01: BLE_OWN_ADDR_RANDOM - Random static address; Uses the hardware or generated random static address.
 * * 0x02: BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT - Resolvable private address, defaults to public if no RPA available.
 * * 0x03: BLE_OWN_ADDR_RPA_RANDOM_DEFAULT - Resolvable private address, defaults to random static if no RPA available.
 */
bool NimBLEDevice::setOwnAddrType(uint8_t type) {
    int rc = ble_hs_id_copy_addr(type & 1, NULL, NULL); // Odd values are random
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Unable to set address type %d, rc=%d", type, rc);
        return false;
    }

    m_ownAddrType = type;

# if MYNEWT_VAL(BLE_HOST_BASED_PRIVACY)
    if (type == BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT || type == BLE_OWN_ADDR_RPA_RANDOM_DEFAULT) {
        // esp32 controller does not support RPA so we must use the random static for calls to the stack
        // the host will take care of the random private address generation/setting.
        m_ownAddrType = BLE_OWN_ADDR_RANDOM;
        rc            = ble_hs_pvcy_rpa_config(NIMBLE_HOST_ENABLE_RPA);
    } else {
        rc = ble_hs_pvcy_rpa_config(NIMBLE_HOST_DISABLE_PRIVACY);
    }
# endif

    return rc == 0;
} // setOwnAddrType

/**
 * @brief Set the device address to use.
 * @param [in] addr The address to set.
 * @return True if the address was set successfully.
 * @details To use the address generated the address type must be set to random with `setOwnAddrType`.
 */
bool NimBLEDevice::setOwnAddr(const NimBLEAddress& addr) {
    return setOwnAddr(addr.getBase()->val);
} // setOwnAddr

/**
 * @brief Set the device address to use.
 * @param [in] addr The address to set.
 * @return True if the address was set successfully.
 * @details To use the address generated the address type must be set to random with `setOwnAddrType`.
 */
bool NimBLEDevice::setOwnAddr(const uint8_t* addr) {
    int rc = ble_hs_id_set_rnd(addr);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed to set address, rc=%d", rc);
        return false;
    }

    return true;
} // setOwnAddr

/* -------------------------------------------------------------------------- */
/*                                  SECURITY                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Set the authorization mode for this device.
 * @param bonding If true we allow bonding, false no bonding will be performed.
 * @param mitm If true we are capable of man in the middle protection, false if not.
 * @param sc If true we will perform secure connection pairing, false we will use legacy pairing.
 */
void NimBLEDevice::setSecurityAuth(bool bonding, bool mitm, bool sc) {
    NIMBLE_LOGD(LOG_TAG, "Setting bonding: %d, mitm: %d, sc: %d", bonding, mitm, sc);
    ble_hs_cfg.sm_bonding = bonding;
    ble_hs_cfg.sm_mitm    = mitm;
    ble_hs_cfg.sm_sc      = sc;
} // setSecurityAuth

/**
 * @brief Set the authorization mode for this device.
 * @param auth_req A bitmap indicating what modes are supported.\n
 * The available bits are defined as:
 * * 0x01 BLE_SM_PAIR_AUTHREQ_BOND
 * * 0x04 BLE_SM_PAIR_AUTHREQ_MITM
 * * 0x08 BLE_SM_PAIR_AUTHREQ_SC
 * * 0x10 BLE_SM_PAIR_AUTHREQ_KEYPRESS  - not yet supported.
 */
void NimBLEDevice::setSecurityAuth(uint8_t auth_req) {
    NimBLEDevice::setSecurityAuth(auth_req & BLE_SM_PAIR_AUTHREQ_BOND,
                                  auth_req & BLE_SM_PAIR_AUTHREQ_MITM,
                                  auth_req & BLE_SM_PAIR_AUTHREQ_SC);
} // setSecurityAuth

/**
 * @brief Set the Input/Output capabilities of this device.
 * @param iocap One of the following values:
 * * 0x00 BLE_HS_IO_DISPLAY_ONLY         DisplayOnly IO capability
 * * 0x01 BLE_HS_IO_DISPLAY_YESNO        DisplayYesNo IO capability
 * * 0x02 BLE_HS_IO_KEYBOARD_ONLY        KeyboardOnly IO capability
 * * 0x03 BLE_HS_IO_NO_INPUT_OUTPUT      NoInputNoOutput IO capability
 * * 0x04 BLE_HS_IO_KEYBOARD_DISPLAY     KeyboardDisplay Only IO capability
 */
void NimBLEDevice::setSecurityIOCap(uint8_t iocap) {
    ble_hs_cfg.sm_io_cap = iocap;
} // setSecurityIOCap

/**
 * @brief If we are the initiator of the security procedure this sets the keys we will distribute.
 * @param initKey A bitmap indicating which keys to distribute during pairing.\n
 * The available bits are defined as:
 * * 0x01: BLE_SM_PAIR_KEY_DIST_ENC  - Distribute the encryption key.
 * * 0x02: BLE_SM_PAIR_KEY_DIST_ID   - Distribute the ID key (IRK).
 * * 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 * * 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
void NimBLEDevice::setSecurityInitKey(uint8_t initKey) {
    ble_hs_cfg.sm_our_key_dist = initKey;
} // setsSecurityInitKey

/**
 * @brief Set the keys we are willing to accept during pairing.
 * @param respKey A bitmap indicating which keys to accept during pairing.
 * The available bits are defined as:
 * * 0x01: BLE_SM_PAIR_KEY_DIST_ENC  -  Accept the encryption key.
 * * 0x02: BLE_SM_PAIR_KEY_DIST_ID   -  Accept the ID key (IRK).
 * * 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 * * 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
void NimBLEDevice::setSecurityRespKey(uint8_t respKey) {
    ble_hs_cfg.sm_their_key_dist = respKey;
} // setsSecurityRespKey

/**
 * @brief Set the passkey the server will ask for when pairing.
 * @param [in] passkey The passkey to use.
 */
void NimBLEDevice::setSecurityPasskey(uint32_t passkey) {
    m_passkey = passkey;
} // setSecurityPasskey

/**
 * @brief Get the current passkey used for pairing.
 * @return The current passkey.
 */
uint32_t NimBLEDevice::getSecurityPasskey() {
    return m_passkey;
} // getSecurityPasskey

/**
 * @brief Start the connection securing and authorization for this connection.
 * @param connHandle The connection handle of the peer device.
 * @param rcPtr Optional pointer to pass the return code to the caller.
 * @returns True if successfully started, success = 0 or BLE_HS_EALREADY.
 */
bool NimBLEDevice::startSecurity(uint16_t connHandle, int* rcPtr) {
    int rc = ble_gap_security_initiate(connHandle);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_security_initiate: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }
    if (rcPtr) {
        *rcPtr = rc;
    }
    return rc == 0 || rc == BLE_HS_EALREADY;
} // startSecurity

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL || CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
/**
 * @brief Inject the provided passkey into the Security Manager.
 * @param [in] peerInfo Connection information for the peer.
 * @param [in] passkey The 6-digit passkey to inject.
 * @return true if the passkey was injected successfully.
 */
bool NimBLEDevice::injectPassKey(const NimBLEConnInfo& peerInfo, uint32_t passkey) {
    ble_sm_io pkey{.action = BLE_SM_IOACT_INPUT, .passkey = passkey};
    int       rc = ble_sm_inject_io(peerInfo.getConnHandle(), &pkey);
    NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_INPUT; ble_sm_inject_io result: %d", rc);
    return rc == 0;
}

/**
 * @brief Inject the provided numeric comparison response into the Security Manager.
 * @param [in] peerInfo Connection information for the peer.
 * @param [in] accept Whether the user confirmed or declined the comparison.
 */
bool NimBLEDevice::injectConfirmPasskey(const NimBLEConnInfo& peerInfo, bool accept) {
    ble_sm_io pkey{.action = BLE_SM_IOACT_NUMCMP, .numcmp_accept = accept};
    int       rc = ble_sm_inject_io(peerInfo.getConnHandle(), &pkey);
    NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_NUMCMP; ble_sm_inject_io result: %d", rc);
    return rc == 0;
}
# endif // CONFIG_BT_NIMBLE_ROLE_CENTRAL || CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

/* -------------------------------------------------------------------------- */
/*                                  UTILITIES                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Set the BLEDevice name.
 * @param [in] deviceName The name to set.
 */
bool NimBLEDevice::setDeviceName(const std::string& deviceName) {
    int rc = ble_svc_gap_device_name_set(deviceName.c_str());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Device name not set - too long");
        return false;
    }

    return true;
} // setDeviceName

/**
 * @brief Set a custom callback for gap events.
 * @param [in] handler The function to call when gap events occur.
 * @returns
 */
bool NimBLEDevice::setCustomGapHandler(gap_event_handler handler) {
    int rc = ble_gap_event_listener_register(&m_listener, handler, NULL);
    if (rc == BLE_HS_EALREADY) {
        NIMBLE_LOGI(LOG_TAG, "Already listening to GAP events.");
        return true;
    } else if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_event_listener_register: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    return rc == 0;
} // setCustomGapHandler

/**
 * @brief Return a string representation of the address of this device.
 * @return A string representation of this device address.
 */
std::string NimBLEDevice::toString() {
    return getAddress().toString();
} // toString

# if CONFIG_NIMBLE_CPP_DEBUG_ASSERT_ENABLED || __DOXYGEN__
/**
 * @brief Debug assert - weak function.
 * @param [in] file The file where the assert occurred.
 * @param [in] line The line number where the assert occurred.
 */
void nimble_cpp_assert(const char* file, unsigned line) {
    console_printf("Assertion failed at %s:%u\n", file, line);
    ble_npl_time_delay(10);
    abort();
}
# endif // CONFIG_NIMBLE_CPP_DEBUG_ASSERT_ENABLED

void NimBLEDevice::setDeviceCallbacks(NimBLEDeviceCallbacks* cb) {
    m_pDeviceCallbacks = cb ? cb : &defaultDeviceCallbacks;
}

int NimBLEDeviceCallbacks::onStoreStatus(struct ble_store_status_event* event, void* arg) {
    NIMBLE_LOGD("NimBLEDeviceCallbacks", "onStoreStatus: default");
    return ble_store_util_status_rr(event, arg);
}

#endif // CONFIG_BT_ENABLED
