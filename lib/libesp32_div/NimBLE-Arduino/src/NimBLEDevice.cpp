/*
 * NimBLEDevice.cpp
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEDevice.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: kolban
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEDevice.h"
#include "NimBLEUtils.h"

#ifdef ESP_PLATFORM
#  include "esp_err.h"
#  include "esp_bt.h"
#  include "nvs_flash.h"
#  if defined(CONFIG_NIMBLE_CPP_IDF)
#    include "esp_nimble_hci.h"
#    include "nimble/nimble_port.h"
#    include "nimble/nimble_port_freertos.h"
#    include "host/ble_hs.h"
#    include "host/ble_hs_pvcy.h"
#    include "host/util/util.h"
#    include "services/gap/ble_svc_gap.h"
#    include "services/gatt/ble_svc_gatt.h"
#  else
#    include "nimble/esp_port/esp-hci/include/esp_nimble_hci.h"
#  endif
#else
#  include "nimble/nimble/controller/include/controller/ble_phy.h"
#endif

#ifndef CONFIG_NIMBLE_CPP_IDF
#  include "nimble/porting/nimble/include/nimble/nimble_port.h"
#  include "nimble/porting/npl/freertos/include/nimble/nimble_port_freertos.h"
#  include "nimble/nimble/host/include/host/ble_hs.h"
#  include "nimble/nimble/host/include/host/ble_hs_pvcy.h"
#  include "nimble/nimble/host/util/include/host/util/util.h"
#  include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
#  include "nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h"
#endif

#if defined(ESP_PLATFORM) && defined(CONFIG_ENABLE_ARDUINO_DEPENDS)
#  include "esp32-hal-bt.h"
#endif

#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEDevice";

/**
 * Singletons for the NimBLEDevice.
 */
static bool            initialized = false;
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
NimBLEScan*     NimBLEDevice::m_pScan = nullptr;
#endif
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
NimBLEServer*   NimBLEDevice::m_pServer = nullptr;
#endif
uint32_t        NimBLEDevice::m_passkey = 123456;
bool            NimBLEDevice::m_synced = false;
#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#  if CONFIG_BT_NIMBLE_EXT_ADV
NimBLEExtAdvertising* NimBLEDevice::m_bleAdvertising = nullptr;
#  else
NimBLEAdvertising* NimBLEDevice::m_bleAdvertising = nullptr;
#  endif
#endif

gap_event_handler           NimBLEDevice::m_customGapHandler = nullptr;
ble_gap_event_listener      NimBLEDevice::m_listener;
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
std::list <NimBLEClient*>   NimBLEDevice::m_cList;
#endif
std::list <NimBLEAddress>   NimBLEDevice::m_ignoreList;
std::vector<NimBLEAddress>  NimBLEDevice::m_whiteList;
NimBLESecurityCallbacks*    NimBLEDevice::m_securityCallbacks = nullptr;
uint8_t                     NimBLEDevice::m_own_addr_type = BLE_OWN_ADDR_PUBLIC;
#ifdef ESP_PLATFORM
uint16_t                    NimBLEDevice::m_scanDuplicateSize = CONFIG_BTDM_SCAN_DUPL_CACHE_SIZE;
uint8_t                     NimBLEDevice::m_scanFilterMode = CONFIG_BTDM_SCAN_DUPL_TYPE;
#endif

/**
 * @brief Create a new instance of a server.
 * @return A new instance of the server.
 */
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
/* STATIC */ NimBLEServer* NimBLEDevice::createServer() {
    if(NimBLEDevice::m_pServer == nullptr) {
        NimBLEDevice::m_pServer = new NimBLEServer();
        ble_gatts_reset();
        ble_svc_gap_init();
        ble_svc_gatt_init();
    }

    return m_pServer;
} // createServer


/**
 * @brief Get the instance of the server.
 * @return A pointer to the server instance.
 */
/* STATIC */ NimBLEServer* NimBLEDevice::getServer() {
    return m_pServer;
} // getServer
#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)


#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
#  if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Get the instance of the advertising object.
 * @return A pointer to the advertising object.
 */
NimBLEExtAdvertising* NimBLEDevice::getAdvertising() {
    if(m_bleAdvertising == nullptr) {
        m_bleAdvertising = new NimBLEExtAdvertising();
    }
    return m_bleAdvertising;
}


/**
 * @brief Convenience function to begin advertising.
 * @param [in] inst_id The extended advertisement instance ID to start.
 * @param [in] duration How long to advertise for in milliseconds, 0 = forever (default).
 * @param [in] max_events Maximum number of advertisement events to send, 0 = no limit (default).
 * @return True if advertising started successfully.
 */
bool NimBLEDevice::startAdvertising(uint8_t inst_id,
                                    int duration,
                                    int max_events) {
    return getAdvertising()->start(inst_id, duration, max_events);
} // startAdvertising


/**
 * @brief Convenience function to stop advertising a data set.
 * @param [in] inst_id The extended advertisement instance ID to stop advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEDevice::stopAdvertising(uint8_t inst_id) {
    return getAdvertising()->stop(inst_id);
} // stopAdvertising

#  endif

#  if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
/**
 * @brief Get the instance of the advertising object.
 * @return A pointer to the advertising object.
 */
NimBLEAdvertising* NimBLEDevice::getAdvertising() {
    if(m_bleAdvertising == nullptr) {
        m_bleAdvertising = new NimBLEAdvertising();
    }
    return m_bleAdvertising;
}


/**
 * @brief Convenience function to begin advertising.
 * @return True if advertising started successfully.
 */
bool NimBLEDevice::startAdvertising() {
    return getAdvertising()->start();
} // startAdvertising
#  endif

/**
 * @brief Convenience function to stop all advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEDevice::stopAdvertising() {
    return getAdvertising()->stop();
} // stopAdvertising
#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)


/**
 * @brief Retrieve the Scan object that we use for scanning.
 * @return The scanning object reference.  This is a singleton object.  The caller should not
 * try and release/delete it.
 */
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
/* STATIC */
NimBLEScan* NimBLEDevice::getScan() {
    if (m_pScan == nullptr) {
        m_pScan = new NimBLEScan();
    }
    return m_pScan;
} // getScan
#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)


/**
 * @brief Creates a new client object and maintains a list of all client objects
 * each client can connect to 1 peripheral device.
 * @param [in] peerAddress An optional peer address that is copied to the new client
 * object, allows for calling NimBLEClient::connect(bool) without a device or address parameter.
 * @return A reference to the new client object.
 */
#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)
/* STATIC */
NimBLEClient* NimBLEDevice::createClient(NimBLEAddress peerAddress) {
    if(m_cList.size() >= NIMBLE_MAX_CONNECTIONS) {
        NIMBLE_LOGW(LOG_TAG,"Number of clients exceeds Max connections. Cur=%d Max=%d",
                    m_cList.size(), NIMBLE_MAX_CONNECTIONS);
    }

    NimBLEClient* pClient = new NimBLEClient(peerAddress);
    m_cList.push_back(pClient);

    return pClient;
} // createClient


/**
 * @brief Delete the client object and remove it from the list.\n
 * Checks if it is connected or trying to connect and disconnects/stops it first.
 * @param [in] pClient A pointer to the client object.
 */
/* STATIC */
bool NimBLEDevice::deleteClient(NimBLEClient* pClient) {
    if(pClient == nullptr) {
        return false;
    }

    // Set the connection established flag to false to stop notifications
    // from accessing the attribute vectors while they are being deleted.
    pClient->m_connEstablished = false;
    int rc =0;

    if(pClient->isConnected()) {
        rc = pClient->disconnect();
        if (rc != 0 && rc != BLE_HS_EALREADY && rc != BLE_HS_ENOTCONN) {
            return false;
        }

        while(pClient->isConnected()) {
            taskYIELD();
        }
        // Since we set the flag to false the app will not get a callback
        // in the disconnect event so we call it here for good measure.
        pClient->m_pClientCallbacks->onDisconnect(pClient);

    } else if(pClient->m_pTaskData != nullptr) {
        rc = ble_gap_conn_cancel();
        if (rc != 0 && rc != BLE_HS_EALREADY) {
            return false;
        }
        while(pClient->m_pTaskData != nullptr) {
            taskYIELD();
        }
    }

    m_cList.remove(pClient);
    delete pClient;

    return true;
} // deleteClient


/**
 * @brief Get the list of created client objects.
 * @return A pointer to the list of clients.
 */
/* STATIC */
std::list<NimBLEClient*>* NimBLEDevice::getClientList() {
    return &m_cList;
} // getClientList


/**
 * @brief Get the number of created client objects.
 * @return Number of client objects created.
 */
/* STATIC */
size_t NimBLEDevice::getClientListSize() {
    return m_cList.size();
} // getClientList


/**
 * @brief Get a reference to a client by connection ID.
 * @param [in] conn_id The client connection ID to search for.
 * @return A pointer to the client object with the spcified connection ID.
 */
/* STATIC */
NimBLEClient* NimBLEDevice::getClientByID(uint16_t conn_id) {
    for(auto it = m_cList.cbegin(); it != m_cList.cend(); ++it) {
        if((*it)->getConnId() == conn_id) {
            return (*it);
        }
    }
    assert(0);
    return nullptr;
} // getClientByID


/**
 * @brief Get a reference to a client by peer address.
 * @param [in] peer_addr The address of the peer to search for.
 * @return A pointer to the client object with the peer address.
 */
/* STATIC */
NimBLEClient* NimBLEDevice::getClientByPeerAddress(const NimBLEAddress &peer_addr) {
    for(auto it = m_cList.cbegin(); it != m_cList.cend(); ++it) {
        if((*it)->getPeerAddress().equals(peer_addr)) {
            return (*it);
        }
    }
    return nullptr;
} // getClientPeerAddress


/**
 * @brief Finds the first disconnected client in the list.
 * @return A pointer to the first client object that is not connected to a peer.
 */
/* STATIC */
NimBLEClient* NimBLEDevice::getDisconnectedClient() {
    for(auto it = m_cList.cbegin(); it != m_cList.cend(); ++it) {
        if(!(*it)->isConnected()) {
            return (*it);
        }
    }
    return nullptr;
} // getDisconnectedClient

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#ifdef ESP_PLATFORM
/**
 * @brief Set the transmission power.
 * @param [in] powerLevel The power level to set, can be one of:
 * *   ESP_PWR_LVL_N12 = 0, Corresponding to -12dbm
 * *   ESP_PWR_LVL_N9  = 1, Corresponding to  -9dbm
 * *   ESP_PWR_LVL_N6  = 2, Corresponding to  -6dbm
 * *   ESP_PWR_LVL_N3  = 3, Corresponding to  -3dbm
 * *   ESP_PWR_LVL_N0  = 4, Corresponding to   0dbm
 * *   ESP_PWR_LVL_P3  = 5, Corresponding to  +3dbm
 * *   ESP_PWR_LVL_P6  = 6, Corresponding to  +6dbm
 * *   ESP_PWR_LVL_P9  = 7, Corresponding to  +9dbm
 * @param [in] powerType The BLE function to set the power level for, can be one of:
 * *   ESP_BLE_PWR_TYPE_CONN_HDL0  = 0,  For connection handle 0
 * *   ESP_BLE_PWR_TYPE_CONN_HDL1  = 1,  For connection handle 1
 * *   ESP_BLE_PWR_TYPE_CONN_HDL2  = 2,  For connection handle 2
 * *   ESP_BLE_PWR_TYPE_CONN_HDL3  = 3,  For connection handle 3
 * *   ESP_BLE_PWR_TYPE_CONN_HDL4  = 4,  For connection handle 4
 * *   ESP_BLE_PWR_TYPE_CONN_HDL5  = 5,  For connection handle 5
 * *   ESP_BLE_PWR_TYPE_CONN_HDL6  = 6,  For connection handle 6
 * *   ESP_BLE_PWR_TYPE_CONN_HDL7  = 7,  For connection handle 7
 * *   ESP_BLE_PWR_TYPE_CONN_HDL8  = 8,  For connection handle 8
 * *   ESP_BLE_PWR_TYPE_ADV        = 9,  For advertising
 * *   ESP_BLE_PWR_TYPE_SCAN       = 10, For scan
 * *   ESP_BLE_PWR_TYPE_DEFAULT    = 11, For default, if not set other, it will use default value
 */
/* STATIC */
void NimBLEDevice::setPower(esp_power_level_t powerLevel, esp_ble_power_type_t powerType) {
    NIMBLE_LOGD(LOG_TAG, ">> setPower: %d (type: %d)", powerLevel, powerType);

    esp_err_t errRc = esp_ble_tx_power_set(powerType, powerLevel);
    if (errRc != ESP_OK) {
        NIMBLE_LOGE(LOG_TAG, "esp_ble_tx_power_set: rc=%d", errRc);
    }

    NIMBLE_LOGD(LOG_TAG, "<< setPower");
} // setPower


/**
 * @brief Get the transmission power.
 * @param [in] powerType The power level to set, can be one of:
 * *   ESP_BLE_PWR_TYPE_CONN_HDL0  = 0,  For connection handle 0
 * *   ESP_BLE_PWR_TYPE_CONN_HDL1  = 1,  For connection handle 1
 * *   ESP_BLE_PWR_TYPE_CONN_HDL2  = 2,  For connection handle 2
 * *   ESP_BLE_PWR_TYPE_CONN_HDL3  = 3,  For connection handle 3
 * *   ESP_BLE_PWR_TYPE_CONN_HDL4  = 4,  For connection handle 4
 * *   ESP_BLE_PWR_TYPE_CONN_HDL5  = 5,  For connection handle 5
 * *   ESP_BLE_PWR_TYPE_CONN_HDL6  = 6,  For connection handle 6
 * *   ESP_BLE_PWR_TYPE_CONN_HDL7  = 7,  For connection handle 7
 * *   ESP_BLE_PWR_TYPE_CONN_HDL8  = 8,  For connection handle 8
 * *   ESP_BLE_PWR_TYPE_ADV        = 9,  For advertising
 * *   ESP_BLE_PWR_TYPE_SCAN       = 10, For scan
 * *   ESP_BLE_PWR_TYPE_DEFAULT    = 11, For default, if not set other, it will use default value
 * @return the power level currently used by the type specified.
 */
/* STATIC */
int NimBLEDevice::getPower(esp_ble_power_type_t powerType) {
    switch(esp_ble_tx_power_get(powerType)) {
        case ESP_PWR_LVL_N12:
            return -12;
        case ESP_PWR_LVL_N9:
            return -9;
        case ESP_PWR_LVL_N6:
            return -6;
        case ESP_PWR_LVL_N3:
            return -3;
        case ESP_PWR_LVL_N0:
            return 0;
        case ESP_PWR_LVL_P3:
            return 3;
        case ESP_PWR_LVL_P6:
            return 6;
        case ESP_PWR_LVL_P9:
            return 9;
        default:
            return BLE_HS_ADV_TX_PWR_LVL_AUTO;
    }
} // getPower

#else

void NimBLEDevice::setPower(int dbm) {
    ble_phy_txpwr_set(dbm);
}


int NimBLEDevice::getPower() {
    return ble_phy_txpwr_get();
}
#endif

/**
 * @brief Get our device address.
 * @return A NimBLEAddress object of our public address if we have one,
 * if not then our current random address.
 */
/* STATIC*/
NimBLEAddress NimBLEDevice::getAddress() {
    ble_addr_t addr = {BLE_ADDR_PUBLIC, 0};

    if(BLE_HS_ENOADDR == ble_hs_id_copy_addr(BLE_ADDR_PUBLIC, addr.val, NULL)) {
        NIMBLE_LOGD(LOG_TAG, "Public address not found, checking random");
        addr.type = BLE_ADDR_RANDOM;
        ble_hs_id_copy_addr(BLE_ADDR_RANDOM, addr.val, NULL);
    }

    return NimBLEAddress(addr);
} // getAddress


/**
 * @brief Return a string representation of the address of this device.
 * @return A string representation of this device address.
 */
/* STATIC */
std::string NimBLEDevice::toString() {
    return getAddress().toString();
} // toString


/**
 * @brief Setup local mtu that will be used to negotiate mtu during request from client peer.
 * @param [in] mtu Value to set local mtu:
 * * This should be larger than 23 and lower or equal to BLE_ATT_MTU_MAX = 527.
 */
/* STATIC */
int NimBLEDevice::setMTU(uint16_t mtu) {
    NIMBLE_LOGD(LOG_TAG, ">> setLocalMTU: %d", mtu);

    int rc =  ble_att_set_preferred_mtu(mtu);

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Could not set local mtu value to: %d", mtu);
    }

    NIMBLE_LOGD(LOG_TAG, "<< setLocalMTU");
    return rc;
} // setMTU


/**
 * @brief Get local MTU value set.
 * @return The current preferred MTU setting.
 */
/* STATIC */
uint16_t NimBLEDevice::getMTU() {
    return ble_att_preferred_mtu();
}


#ifdef ESP_PLATFORM
/**
 * @brief Set the duplicate filter cache size for filtering scanned devices.
 * @param [in] cacheSize The number of advertisements filtered before the cache is reset.\n
 * Range is 10-1000, a larger value will reduce how often the same devices are reported.
 * @details Must only be called before calling NimBLEDevice::init.
 */
/*STATIC*/
void NimBLEDevice::setScanDuplicateCacheSize(uint16_t cacheSize) {
    if(initialized) {
        NIMBLE_LOGE(LOG_TAG, "Cannot change scan cache size while initialized");
        return;
    } else if(cacheSize > 1000 || cacheSize <10) {
        NIMBLE_LOGE(LOG_TAG, "Invalid scan cache size; min=10 max=1000");
        return;
    }

    m_scanDuplicateSize = cacheSize;
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
/*STATIC*/
void NimBLEDevice::setScanFilterMode(uint8_t mode) {
    if(initialized) {
        NIMBLE_LOGE(LOG_TAG, "Cannot change scan duplicate type while initialized");
        return;
    } else if(mode > 2) {
        NIMBLE_LOGE(LOG_TAG, "Invalid scan duplicate type");
        return;
    }

    m_scanFilterMode = mode;
}
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL) || defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
/**
 * @brief Gets the number of bonded peers stored
 */
/*STATIC*/
int NimBLEDevice::getNumBonds() {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_peers, rc;

    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc !=0) {
        return 0;
    }

    return num_peers;
}


/**
 * @brief Deletes all bonding information.
 */
/*STATIC*/
void NimBLEDevice::deleteAllBonds() {
    ble_store_clear();
}


/**
 * @brief Deletes a peer bond.
 * @param [in] address The address of the peer with which to delete bond info.
 * @returns true on success.
 */
/*STATIC*/
bool NimBLEDevice::deleteBond(const NimBLEAddress &address) {
    ble_addr_t delAddr;
    memcpy(&delAddr.val, address.getNative(),6);
    delAddr.type = address.getType();

    int rc = ble_gap_unpair(&delAddr);
    if (rc != 0) {
        return false;
    }

    return true;
}


/**
 * @brief Checks if a peer device is bonded.
 * @param [in] address The address to check for bonding.
 * @returns true if bonded.
 */
/*STATIC*/
bool NimBLEDevice::isBonded(const NimBLEAddress &address) {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_peers, rc;

    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return false;
    }

    for (int i = 0; i < num_peers; i++) {
        NimBLEAddress storedAddr(peer_id_addrs[i]);
        if(storedAddr == address) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Get the address of a bonded peer device by index.
 * @param [in] index The index to retrieve the peer address of.
 * @returns NimBLEAddress of the found bonded peer or nullptr if not found.
 */
/*STATIC*/
NimBLEAddress NimBLEDevice::getBondedAddress(int index) {
    ble_addr_t peer_id_addrs[MYNEWT_VAL(BLE_STORE_MAX_BONDS)];
    int num_peers, rc;

    rc = ble_store_util_bonded_peers(&peer_id_addrs[0], &num_peers, MYNEWT_VAL(BLE_STORE_MAX_BONDS));
    if (rc != 0) {
        return nullptr;
    }

    if (index > num_peers || index < 0) {
        return nullptr;
    }

    return NimBLEAddress(peer_id_addrs[index]);
}
#endif

/**
 * @brief Checks if a peer device is whitelisted.
 * @param [in] address The address to check for in the whitelist.
 * @returns true if the address is in the whitelist.
 */
/*STATIC*/
bool NimBLEDevice::onWhiteList(const NimBLEAddress & address) {
    for (auto &it : m_whiteList) {
        if (it == address) {
            return true;
        }
    }

    return false;
}


/**
 * @brief Add a peer address to the whitelist.
 * @param [in] address The address to add to the whitelist.
 * @returns true if successful.
 */
/*STATIC*/
bool NimBLEDevice::whiteListAdd(const NimBLEAddress & address) {
    if (NimBLEDevice::onWhiteList(address)) {
        return true;
    }

    m_whiteList.push_back(address);
    std::vector<ble_addr_t> wlVec;
    wlVec.reserve(m_whiteList.size());

    for (auto &it : m_whiteList) {
        ble_addr_t wlAddr;
        memcpy(&wlAddr.val, it.getNative(), 6);
        wlAddr.type = it.getType();
        wlVec.push_back(wlAddr);
    }

    int rc = ble_gap_wl_set(&wlVec[0], wlVec.size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed adding to whitelist rc=%d", rc);
        return false;
    }

    return true;
}


/**
 * @brief Remove a peer address from the whitelist.
 * @param [in] address The address to remove from the whitelist.
 * @returns true if successful.
 */
/*STATIC*/
bool NimBLEDevice::whiteListRemove(const NimBLEAddress & address) {
    if (!NimBLEDevice::onWhiteList(address)) {
        return true;
    }

    std::vector<ble_addr_t> wlVec;
    wlVec.reserve(m_whiteList.size());

    for (auto &it : m_whiteList) {
        if (it != address) {
            ble_addr_t wlAddr;
            memcpy(&wlAddr.val, it.getNative(), 6);
            wlAddr.type = it.getType();
            wlVec.push_back(wlAddr);
        }
    }

    int rc = ble_gap_wl_set(&wlVec[0], wlVec.size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed removing from whitelist rc=%d", rc);
        return false;
    }

    // Don't remove from the list unless NimBLE returned success
    for (auto it = m_whiteList.begin(); it < m_whiteList.end(); ++it) {
        if ((*it) == address) {
            m_whiteList.erase(it);
            break;
        }
    }

    return true;
}


/**
 * @brief Gets the count of addresses in the whitelist.
 * @returns The number of addresses in the whitelist.
 */
/*STATIC*/
size_t NimBLEDevice::getWhiteListCount() {
    return m_whiteList.size();
}


/**
 * @brief Gets the address at the vector index.
 * @param [in] index The vector index to retrieve the address from.
 * @returns the NimBLEAddress at the whitelist index or nullptr if not found.
 */
/*STATIC*/
NimBLEAddress NimBLEDevice::getWhiteListAddress(size_t index) {
    if (index > m_whiteList.size()) {
        NIMBLE_LOGE(LOG_TAG, "Invalid index; %u", index);
        return nullptr;
    }
    return m_whiteList[index];
}


/**
 * @brief Host reset, we pass the message so we don't make calls until resynced.
 * @param [in] reason The reason code for the reset.
 */
/* STATIC */
void NimBLEDevice::onReset(int reason)
{
    if(!m_synced) {
        return;
    }

    m_synced = false;

    NIMBLE_LOGC(LOG_TAG, "Resetting state; reason=%d, %s", reason,
                        NimBLEUtils::returnCodeToString(reason));

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
    if(initialized) {
        if(m_pScan != nullptr) {
            m_pScan->onHostReset();
        }
    }
#endif
} // onReset


/**
 * @brief Host resynced with controller, all clear to make calls to the stack.
 */
/* STATIC */
void NimBLEDevice::onSync(void)
{
    NIMBLE_LOGI(LOG_TAG, "NimBle host synced.");
    // This check is needed due to potentially being called multiple times in succession
    // If this happens, the call to scan start may get stuck or cause an advertising fault.
    if(m_synced) {
        return;
    }

    /* Make sure we have proper identity address set (public preferred) */
    int rc = ble_hs_util_ensure_addr(0);
    assert(rc == 0);

#ifndef ESP_PLATFORM
    rc = ble_hs_id_infer_auto(m_own_addr_type, &m_own_addr_type);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "error determining address type; rc=%d", rc);
        return;
    }
#endif

    // Yield for housekeeping before returning to operations.
    // Occasionally triggers exception without.
    taskYIELD();

    m_synced = true;

    if(initialized) {
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
        if(m_pScan != nullptr) {
            m_pScan->onHostSync();
        }
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
        if(m_bleAdvertising != nullptr) {
            m_bleAdvertising->onHostSync();
        }
#endif
    }
} // onSync


/**
 * @brief The main host task.
 */
/* STATIC */
void NimBLEDevice::host_task(void *param)
{
    NIMBLE_LOGI(LOG_TAG, "BLE Host Task Started");

    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
} // host_task


/**
 * @brief Initialize the %BLE environment.
 * @param [in] deviceName The device name of the device.
 */
/* STATIC */
void NimBLEDevice::init(const std::string &deviceName) {
    if(!initialized){
        int rc=0;
#ifdef ESP_PLATFORM
        esp_err_t errRc = ESP_OK;

#ifdef CONFIG_ENABLE_ARDUINO_DEPENDS
        // make sure the linker includes esp32-hal-bt.c so Arduino init doesn't release BLE memory.
        btStarted();
#endif

        errRc = nvs_flash_init();

        if (errRc == ESP_ERR_NVS_NO_FREE_PAGES || errRc == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            errRc = nvs_flash_init();
        }

        ESP_ERROR_CHECK(errRc);

        esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);

        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
#if  defined (CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
        bt_cfg.bluetooth_mode = ESP_BT_MODE_BLE;
#else
        bt_cfg.mode = ESP_BT_MODE_BLE;
        bt_cfg.ble_max_conn = CONFIG_BT_NIMBLE_MAX_CONNECTIONS;
#endif
        bt_cfg.normal_adv_size = m_scanDuplicateSize;
        bt_cfg.scan_duplicate_type = m_scanFilterMode;

        ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
        ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
        ESP_ERROR_CHECK(esp_nimble_hci_init());
#endif
        nimble_port_init();

        // Setup callbacks for host events
        ble_hs_cfg.reset_cb = NimBLEDevice::onReset;
        ble_hs_cfg.sync_cb = NimBLEDevice::onSync;

        // Set initial security capabilities
        ble_hs_cfg.sm_io_cap = BLE_HS_IO_NO_INPUT_OUTPUT;
        ble_hs_cfg.sm_bonding = 0;
        ble_hs_cfg.sm_mitm = 0;
        ble_hs_cfg.sm_sc = 1;
        ble_hs_cfg.sm_our_key_dist = 1;
        ble_hs_cfg.sm_their_key_dist = 3;

        ble_hs_cfg.store_status_cb = ble_store_util_status_rr; /*TODO: Implement handler for this*/

        // Set the device name.
        rc = ble_svc_gap_device_name_set(deviceName.c_str());
        assert(rc == 0);

        ble_store_config_init();

        nimble_port_freertos_init(NimBLEDevice::host_task);
    }

    // Wait for host and controller to sync before returning and accepting new tasks
    while(!m_synced){
        taskYIELD();
    }

    initialized = true; // Set the initialization flag to ensure we are only initialized once.
} // init


/**
 * @brief Shutdown the NimBLE stack/controller.
 * @param [in] clearAll If true, deletes all server/advertising/scan/client objects after deinitializing.
 * @note If clearAll is true when called, any references to the created objects become invalid.
 */
/* STATIC */
void NimBLEDevice::deinit(bool clearAll) {
    int ret = nimble_port_stop();
    if (ret == 0) {
        nimble_port_deinit();
#ifdef ESP_PLATFORM
        ret = esp_nimble_hci_and_controller_deinit();
        if (ret != ESP_OK) {
            NIMBLE_LOGE(LOG_TAG, "esp_nimble_hci_and_controller_deinit() failed with error: %d", ret);
        }
#endif
        initialized = false;
        m_synced = false;

        if(clearAll) {
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
            if(NimBLEDevice::m_pServer != nullptr) {
                delete NimBLEDevice::m_pServer;
                NimBLEDevice::m_pServer = nullptr;
            }
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)
            if(NimBLEDevice::m_bleAdvertising != nullptr) {
                delete NimBLEDevice::m_bleAdvertising;
                NimBLEDevice::m_bleAdvertising = nullptr;
            }
#endif

#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
            if(NimBLEDevice::m_pScan != nullptr) {
                delete NimBLEDevice::m_pScan;
                NimBLEDevice::m_pScan= nullptr;
            }
#endif

#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
            for(auto &it : m_cList) {
                deleteClient(it);
                m_cList.clear();
            }
#endif

            m_ignoreList.clear();

            if(m_securityCallbacks != nullptr) {
                delete m_securityCallbacks;
            }
        }
    }
} // deinit

/**
 * @brief Set the BLEDevice's name
 * @param [in] deviceName The device name of the device.
 */
/* STATIC */
void NimBLEDevice::setDeviceName(const std::string &deviceName) {
    ble_svc_gap_device_name_set(deviceName.c_str());
} // setDeviceName


/**
 * @brief Check if the initialization is complete.
 * @return true if initialized.
 */
/*STATIC*/
bool NimBLEDevice::getInitialized() {
    return initialized;
} // getInitialized


/**
 * @brief Set the authorization mode for this device.
 * @param bonding If true we allow bonding, false no bonding will be performed.
 * @param mitm If true we are capable of man in the middle protection, false if not.
 * @param sc If true we will perform secure connection pairing, false we will use legacy pairing.
 */
/*STATIC*/
void NimBLEDevice::setSecurityAuth(bool bonding, bool mitm, bool sc) {
    NIMBLE_LOGD(LOG_TAG, "Setting bonding: %d, mitm: %d, sc: %d",bonding,mitm,sc);
    ble_hs_cfg.sm_bonding = bonding;
    ble_hs_cfg.sm_mitm = mitm;
    ble_hs_cfg.sm_sc = sc;
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
/*STATIC*/
void NimBLEDevice::setSecurityAuth(uint8_t auth_req) {
    NimBLEDevice::setSecurityAuth((auth_req & BLE_SM_PAIR_AUTHREQ_BOND)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_MITM)>0,
                                (auth_req & BLE_SM_PAIR_AUTHREQ_SC)>0);
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
/*STATIC*/
void NimBLEDevice::setSecurityIOCap(uint8_t iocap) {
    ble_hs_cfg.sm_io_cap = iocap;
} // setSecurityIOCap


/**
 * @brief If we are the initiator of the security procedure this sets the keys we will distribute.
 * @param init_key A bitmap indicating which keys to distribute during pairing.\n
 * The available bits are defined as:
 * * 0x01: BLE_SM_PAIR_KEY_DIST_ENC  - Distribute the encryption key.
 * * 0x02: BLE_SM_PAIR_KEY_DIST_ID   - Distribute the ID key (IRK).
 * * 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 * * 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
/*STATIC*/
void NimBLEDevice::setSecurityInitKey(uint8_t init_key) {
    ble_hs_cfg.sm_our_key_dist = init_key;
} // setsSecurityInitKey


/**
 * @brief Set the keys we are willing to accept during pairing.
 * @param resp_key A bitmap indicating which keys to accept during pairing.
 * The available bits are defined as:
 * * 0x01: BLE_SM_PAIR_KEY_DIST_ENC  -  Accept the encryption key.
 * * 0x02: BLE_SM_PAIR_KEY_DIST_ID   -  Accept the ID key (IRK).
 * * 0x04: BLE_SM_PAIR_KEY_DIST_SIGN
 * * 0x08: BLE_SM_PAIR_KEY_DIST_LINK
 */
/*STATIC*/
void NimBLEDevice::setSecurityRespKey(uint8_t resp_key) {
    ble_hs_cfg.sm_their_key_dist = resp_key;
} // setsSecurityRespKey


/**
 * @brief Set the passkey the server will ask for when pairing.
 * @param [in] pin The passkey to use.
 */
/*STATIC*/
void NimBLEDevice::setSecurityPasskey(uint32_t pin) {
    m_passkey = pin;
} // setSecurityPasskey


/**
 * @brief Get the current passkey used for pairing.
 * @return The current passkey.
 */
/*STATIC*/
uint32_t NimBLEDevice::getSecurityPasskey() {
    return m_passkey;
} // getSecurityPasskey


/**
 * @brief Set callbacks that will be used to handle encryption negotiation events and authentication events
 * @param [in] callbacks Pointer to NimBLESecurityCallbacks class
 * @deprecated For backward compatibility, New code should use client/server callback methods.
 */
/*STATIC*/
void NimBLEDevice::setSecurityCallbacks(NimBLESecurityCallbacks* callbacks) {
    NimBLEDevice::m_securityCallbacks = callbacks;
} // setSecurityCallbacks


#ifdef ESP_PLATFORM
/**
 * @brief Set the own address type.
 * @param [in] own_addr_type Own Bluetooth Device address type.\n
 * The available bits are defined as:
 * * 0x00: BLE_OWN_ADDR_PUBLIC
 * * 0x01: BLE_OWN_ADDR_RANDOM
 * * 0x02: BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
 * * 0x03: BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
 * @param [in] useNRPA If true, and address type is random, uses a non-resolvable random address.
 */
/*STATIC*/
void NimBLEDevice::setOwnAddrType(uint8_t own_addr_type, bool useNRPA) {
    m_own_addr_type = own_addr_type;
    switch (own_addr_type) {
#ifdef CONFIG_IDF_TARGET_ESP32
        case BLE_OWN_ADDR_PUBLIC:
            ble_hs_pvcy_rpa_config(NIMBLE_HOST_DISABLE_PRIVACY);
            break;
#endif
        case BLE_OWN_ADDR_RANDOM:
            setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
#ifdef CONFIG_IDF_TARGET_ESP32
            ble_hs_pvcy_rpa_config(useNRPA ? NIMBLE_HOST_ENABLE_NRPA : NIMBLE_HOST_ENABLE_RPA);
#endif
            break;
        case BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT:
        case BLE_OWN_ADDR_RPA_RANDOM_DEFAULT:
            setSecurityInitKey(BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID);
#ifdef CONFIG_IDF_TARGET_ESP32
            ble_hs_pvcy_rpa_config(NIMBLE_HOST_ENABLE_RPA);
#endif
            break;
    }
} // setOwnAddrType
#endif

/**
 * @brief Start the connection securing and authorization for this connection.
 * @param conn_id The connection id of the peer device.
 * @returns NimBLE stack return code, 0 = success.
 */
/* STATIC */
int NimBLEDevice::startSecurity(uint16_t conn_id) {
    int rc = ble_gap_security_initiate(conn_id);
    if(rc != 0){
        NIMBLE_LOGE(LOG_TAG, "ble_gap_security_initiate: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    return rc;
} // startSecurity


/**
 * @brief Check if the device address is on our ignore list.
 * @param [in] address The address to look for.
 * @return True if ignoring.
 */
/*STATIC*/
bool NimBLEDevice::isIgnored(const NimBLEAddress &address) {
    for(auto &it : m_ignoreList) {
        if(it.equals(address)){
            return true;
        }
    }

    return false;
}


/**
 * @brief Add a device to the ignore list.
 * @param [in] address The address of the device we want to ignore.
 */
/*STATIC*/
void NimBLEDevice::addIgnored(const NimBLEAddress &address) {
    m_ignoreList.push_back(address);
}


/**
 * @brief Remove a device from the ignore list.
 * @param [in] address The address of the device we want to remove from the list.
 */
/*STATIC*/
void  NimBLEDevice::removeIgnored(const NimBLEAddress &address) {
    for(auto it = m_ignoreList.begin(); it != m_ignoreList.end(); ++it) {
        if((*it).equals(address)){
            m_ignoreList.erase(it);
            return;
        }
    }
}


/**
 * @brief Set a custom callback for gap events.
 * @param [in] handler The function to call when gap events occur.
 */
/*STATIC*/
void NimBLEDevice::setCustomGapHandler(gap_event_handler handler) {
    m_customGapHandler = handler;
    int rc = ble_gap_event_listener_register(&m_listener, m_customGapHandler, NULL);
    if(rc == BLE_HS_EALREADY){
        NIMBLE_LOGI(LOG_TAG, "Already listening to GAP events.");
    }
    else{
        assert(rc == 0);
    }
} // setCustomGapHandler

#endif // CONFIG_BT_ENABLED
