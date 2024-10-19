/*
 * NimBLEClient.cpp
 *
 *  Created: on Jan 26 2020
 *      Author H2zero
 *
 * Originally:
 * BLEClient.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: kolban
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLEClient.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#include <string>
#include <unordered_set>
#include <climits>

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "nimble/nimble_port.h"
#else
#include "nimble/porting/nimble/include/nimble/nimble_port.h"
#endif

static const char* LOG_TAG = "NimBLEClient";
static NimBLEClientCallbacks defaultCallbacks;

/*
 * Design
 * ------
 * When we perform a getService() request, we are asking the BLE server to return each of the services
 * that it exposes.  For each service, we receive a callback which contains details
 * of the exposed service including its UUID.
 *
 * The objects we will invent for a NimBLEClient will be as follows:
 * * NimBLERemoteService - A model of a remote service.
 * * NimBLERemoteCharacteristic - A model of a remote characteristic
 * * NimBLERemoteDescriptor - A model of a remote descriptor.
 *
 * Since there is a hierarchical relationship here, we will have the idea that from a NimBLERemoteService will own
 * zero or more remote characteristics and a NimBLERemoteCharacteristic will own zero or more remote NimBLEDescriptors.
 *
 * We will assume that a NimBLERemoteService contains a vector of owned characteristics
 * and that a NimBLECharacteristic contains a vector of owned descriptors.
 *
 *
 */


/**
 * @brief Constructor, private - only callable by NimBLEDevice::createClient
 * to ensure proper handling of the list of client objects.
 */
NimBLEClient::NimBLEClient(const NimBLEAddress &peerAddress) : m_peerAddress(peerAddress) {
    m_pClientCallbacks = &defaultCallbacks;
    m_conn_id          = BLE_HS_CONN_HANDLE_NONE;
    m_connectTimeout   = 30000;
    m_deleteCallbacks  = false;
    m_pTaskData        = nullptr;
    m_connEstablished  = false;
    m_lastErr          = 0;
#if CONFIG_BT_NIMBLE_EXT_ADV
    m_phyMask          = BLE_GAP_LE_PHY_1M_MASK |
                         BLE_GAP_LE_PHY_2M_MASK |
                         BLE_GAP_LE_PHY_CODED_MASK;
#endif

    m_pConnParams.scan_itvl = 16;          // Scan interval in 0.625ms units (NimBLE Default)
    m_pConnParams.scan_window = 16;        // Scan window in 0.625ms units (NimBLE Default)
    m_pConnParams.itvl_min = BLE_GAP_INITIAL_CONN_ITVL_MIN;  // min_int = 0x10*1.25ms = 20ms
    m_pConnParams.itvl_max = BLE_GAP_INITIAL_CONN_ITVL_MAX;  // max_int = 0x20*1.25ms = 40ms
    m_pConnParams.latency  = BLE_GAP_INITIAL_CONN_LATENCY;      // number of packets allowed to skip (extends max interval)
    m_pConnParams.supervision_timeout = BLE_GAP_INITIAL_SUPERVISION_TIMEOUT; // timeout = 400*10ms = 4000ms
    m_pConnParams.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN; // Minimum length of connection event in 0.625ms units
    m_pConnParams.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN; // Maximum length of connection event in 0.625ms units

    memset(&m_dcTimer, 0, sizeof(m_dcTimer));
    ble_npl_callout_init(&m_dcTimer, nimble_port_get_dflt_eventq(),
                         NimBLEClient::dcTimerCb, this);
} // NimBLEClient


/**
 * @brief Destructor, private - only callable by NimBLEDevice::deleteClient
 * to ensure proper disconnect and removal from device list.
 */
NimBLEClient::~NimBLEClient() {
    // We may have allocated service references associated with this client.
    // Before we are finished with the client, we must release resources.
    deleteServices();

    if(m_deleteCallbacks && m_pClientCallbacks != &defaultCallbacks) {
        delete m_pClientCallbacks;
    }

    ble_npl_callout_deinit(&m_dcTimer);

} // ~NimBLEClient


/**
 * @brief If we have asked to disconnect and the event does not
 * occur within the supervision timeout + added delay, this will
 * be called to reset the host in the case of a stalled controller.
 */
void NimBLEClient::dcTimerCb(ble_npl_event *event) {
 /*   NimBLEClient *pClient = (NimBLEClient*)event->arg;
    NIMBLE_LOGE(LOG_TAG, "Timed out disconnecting from %s - resetting host",
                std::string(pClient->getPeerAddress()).c_str());
 */
    ble_hs_sched_reset(BLE_HS_ECONTROLLER);
}


/**
 * @brief Delete all service objects created by this client and clear the vector.
 */
void NimBLEClient::deleteServices() {
    NIMBLE_LOGD(LOG_TAG, ">> deleteServices");
    // Delete all the services.
    for(auto &it: m_servicesVector) {
        delete it;
    }
    m_servicesVector.clear();

    NIMBLE_LOGD(LOG_TAG, "<< deleteServices");
} // deleteServices


/**
 * @brief Delete service by UUID
 * @param [in] uuid The UUID of the service to be deleted from the local database.
 * @return Number of services left.
 */
size_t NimBLEClient::deleteService(const NimBLEUUID &uuid) {
    NIMBLE_LOGD(LOG_TAG, ">> deleteService");
    // Delete the requested service.
    for(auto it = m_servicesVector.begin(); it != m_servicesVector.end(); ++it) {
        if((*it)->getUUID() == uuid) {
            delete *it;
            m_servicesVector.erase(it);
            break;
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< deleteService");

    return m_servicesVector.size();
} // deleteServices


/**
 * @brief Connect to the BLE Server.
 * @param [in] deleteAttributes If true this will delete any attribute objects this client may already\n
 * have created and clears the vectors after successful connection.
 * @return True on success.
 */
bool NimBLEClient::connect(bool deleteAttributes) {
    return connect(m_peerAddress, deleteAttributes);
}

/**
 * @brief Connect to an advertising device.
 * @param [in] device The device to connect to.
 * @param [in] deleteAttributes If true this will delete any attribute objects this client may already\n
 * have created and clears the vectors after successful connection.
 * @return True on success.
 */
bool NimBLEClient::connect(NimBLEAdvertisedDevice* device, bool deleteAttributes) {
    NimBLEAddress address(device->getAddress());
    return connect(address, deleteAttributes);
}


/**
 * @brief Connect to the BLE Server.
 * @param [in] address The address of the server.
 * @param [in] deleteAttributes If true this will delete any attribute objects this client may already\n
 * have created and clears the vectors after successful connection.
 * @return True on success.
 */
bool NimBLEClient::connect(const NimBLEAddress &address, bool deleteAttributes) {
    NIMBLE_LOGD(LOG_TAG, ">> connect(%s)", address.toString().c_str());

    if(!NimBLEDevice::m_synced) {
        NIMBLE_LOGE(LOG_TAG, "Host reset, wait for sync.");
        return false;
    }

    if(isConnected() || m_connEstablished || m_pTaskData != nullptr) {
        NIMBLE_LOGE(LOG_TAG, "Client busy, connected to %s, id=%d",
                    std::string(m_peerAddress).c_str(), getConnId());
        return false;
    }

    ble_addr_t peerAddr_t;
    memcpy(&peerAddr_t.val, address.getNative(),6);
    peerAddr_t.type = address.getType();
    if(ble_gap_conn_find_by_addr(&peerAddr_t, NULL) == 0) {
        NIMBLE_LOGE(LOG_TAG, "A connection to %s already exists",
                    address.toString().c_str());
        return false;
    }

    if(address == NimBLEAddress("")) {
        NIMBLE_LOGE(LOG_TAG, "Invalid peer address;(NULL)");
        return false;
    } else {
        m_peerAddress = address;
    }

    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    ble_task_data_t taskData = {this, cur_task, 0, nullptr};
    m_pTaskData = &taskData;
    int rc = 0;

    /* Try to connect the the advertiser.  Allow 30 seconds (30000 ms) for
     *  timeout (default value of m_connectTimeout).
     *  Loop on BLE_HS_EBUSY if the scan hasn't stopped yet.
     */
    do {
#if CONFIG_BT_NIMBLE_EXT_ADV
        rc = ble_gap_ext_connect(NimBLEDevice::m_own_addr_type,
                                 &peerAddr_t,
                                 m_connectTimeout,
                                 m_phyMask,
                                 &m_pConnParams,
                                 &m_pConnParams,
                                 &m_pConnParams,
                                 NimBLEClient::handleGapEvent,
                                 this);

#else
        rc = ble_gap_connect(NimBLEDevice::m_own_addr_type, &peerAddr_t,
                             m_connectTimeout, &m_pConnParams,
                             NimBLEClient::handleGapEvent, this);
#endif
        switch (rc) {
            case 0:
                break;

            case BLE_HS_EBUSY:
                // Scan was still running, stop it and try again
                if (!NimBLEDevice::getScan()->stop()) {
                    rc = BLE_HS_EUNKNOWN;
                }
                break;

            case BLE_HS_EDONE:
                // A connection to this device already exists, do not connect twice.
                NIMBLE_LOGE(LOG_TAG, "Already connected to device; addr=%s",
                            std::string(m_peerAddress).c_str());
                break;

            case BLE_HS_EALREADY:
                // Already attempting to connect to this device, cancel the previous
                // attempt and report failure here so we don't get 2 connections.
                NIMBLE_LOGE(LOG_TAG, "Already attempting to connect to %s - cancelling",
                            std::string(m_peerAddress).c_str());
                ble_gap_conn_cancel();
                break;

            default:
                NIMBLE_LOGE(LOG_TAG, "Failed to connect to %s, rc=%d; %s",
                            std::string(m_peerAddress).c_str(),
                            rc, NimBLEUtils::returnCodeToString(rc));
                break;
        }

    } while (rc == BLE_HS_EBUSY);

    m_lastErr = rc;

    if(rc != 0) {
        m_pTaskData = nullptr;
        return false;
    }

#ifdef ulTaskNotifyValueClear
    // Clear the task notification value to ensure we block
    ulTaskNotifyValueClear(cur_task, ULONG_MAX);
#endif
    // Wait for the connect timeout time +1 second for the connection to complete
    if(ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(m_connectTimeout + 1000)) == pdFALSE) {
        m_pTaskData = nullptr;
        // If a connection was made but no response from MTU exchange; disconnect
        if(isConnected()) {
            NIMBLE_LOGE(LOG_TAG, "Connect timeout - no response");
            disconnect();
        } else {
        // workaround; if the controller doesn't cancel the connection
        // at the timeout, cancel it here.
            NIMBLE_LOGE(LOG_TAG, "Connect timeout - cancelling");
            ble_gap_conn_cancel();
        }

        return false;

    } else if(taskData.rc != 0){
        m_lastErr = taskData.rc;
        NIMBLE_LOGE(LOG_TAG, "Connection failed; status=%d %s",
                    taskData.rc,
                    NimBLEUtils::returnCodeToString(taskData.rc));
        // If the failure was not a result of a disconnection
        // make sure we disconnect now to avoid dangling connections
        if(isConnected()) {
            disconnect();
        }
        return false;
    } else {
        NIMBLE_LOGI(LOG_TAG, "Connection established");
    }

    if(deleteAttributes) {
        deleteServices();
    }

    m_connEstablished = true;
    m_pClientCallbacks->onConnect(this);

    NIMBLE_LOGD(LOG_TAG, "<< connect()");
    // Check if still connected before returning
    return isConnected();
} // connect


/**
 * @brief Initiate a secure connection (pair/bond) with the server.\n
 * Called automatically when a characteristic or descriptor requires encryption or authentication to access it.
 * @return True on success.
 */
bool NimBLEClient::secureConnection() {
    NIMBLE_LOGD(LOG_TAG, ">> secureConnection()");
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    ble_task_data_t taskData = {this, cur_task, 0, nullptr};

    int retryCount = 1;

    do {
        m_pTaskData = &taskData;

        int rc = NimBLEDevice::startSecurity(m_conn_id);
        if(rc != 0 && rc != BLE_HS_EALREADY){
            m_lastErr = rc;
            m_pTaskData = nullptr;
            return false;
        }

#ifdef ulTaskNotifyValueClear
        // Clear the task notification value to ensure we block
        ulTaskNotifyValueClear(cur_task, ULONG_MAX);
#endif
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    } while (taskData.rc == (BLE_HS_ERR_HCI_BASE + BLE_ERR_PINKEY_MISSING) && retryCount--);

    if(taskData.rc != 0){
        m_lastErr = taskData.rc;
        NIMBLE_LOGE(LOG_TAG, "secureConnection: failed rc=%d", taskData.rc);
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "<< secureConnection: success");
    return true;
} // secureConnection


/**
 * @brief Disconnect from the peer.
 * @return Error code from NimBLE stack, 0 = success.
 */
int NimBLEClient::disconnect(uint8_t reason) {
    NIMBLE_LOGD(LOG_TAG, ">> disconnect()");
    int rc = 0;
    if(isConnected()) {
        // If the timer was already started, ignore this call.
        if(ble_npl_callout_is_active(&m_dcTimer)) {
            NIMBLE_LOGI(LOG_TAG, "Already disconnecting, timer started");
            return BLE_HS_EALREADY;
        }

        ble_gap_conn_desc desc;
        if(ble_gap_conn_find(m_conn_id, &desc) != 0){
            NIMBLE_LOGI(LOG_TAG, "Connection ID not found");
            return BLE_HS_EALREADY;
        }

        // We use a timer to detect a controller error in the event that it does
        // not inform the stack when disconnection is complete.
        // This is a common error in certain esp-idf versions.
        // The disconnect timeout time is the supervision timeout time + 1 second.
        // In the case that the event happens shortly after the supervision timeout
        // we don't want to prematurely reset the host.
        ble_npl_time_t ticks;
        ble_npl_time_ms_to_ticks((desc.supervision_timeout + 100) * 10, &ticks);
        ble_npl_callout_reset(&m_dcTimer, ticks);

        rc = ble_gap_terminate(m_conn_id, reason);
        if (rc != 0) {
            if(rc != BLE_HS_EALREADY) {
                ble_npl_callout_stop(&m_dcTimer);
            }
            NIMBLE_LOGE(LOG_TAG, "ble_gap_terminate failed: rc=%d %s",
                        rc, NimBLEUtils::returnCodeToString(rc));
        }
    } else {
        NIMBLE_LOGD(LOG_TAG, "Not connected to any peers");
    }

    NIMBLE_LOGD(LOG_TAG, "<< disconnect()");
    m_lastErr = rc;
    return rc;
} // disconnect


#if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Set the PHY types to use when connecting to a server.
 * @param [in] mask A bitmask indicating what PHYS to connect with.\n
 * The available bits are:
 * * 0x01 BLE_GAP_LE_PHY_1M_MASK
 * * 0x02 BLE_GAP_LE_PHY_2M_MASK
 * * 0x04 BLE_GAP_LE_PHY_CODED_MASK
 */
void NimBLEClient::setConnectPhy(uint8_t mask) {
    m_phyMask = mask;
}
#endif


/**
 * @brief Set the connection parameters to use when connecting to a server.
 * @param [in] minInterval The minimum connection interval in 1.25ms units.
 * @param [in] maxInterval The maximum connection interval in 1.25ms units.
 * @param [in] latency The number of packets allowed to skip (extends max interval).
 * @param [in] timeout The timeout time in 10ms units before disconnecting.
 * @param [in] scanInterval The scan interval to use when attempting to connect in 0.625ms units.
 * @param [in] scanWindow The scan window to use when attempting to connect in 0.625ms units.
 */
void NimBLEClient::setConnectionParams(uint16_t minInterval, uint16_t maxInterval,
                                uint16_t latency, uint16_t timeout,
                                uint16_t scanInterval, uint16_t scanWindow)/*,
                                uint16_t minConnTime, uint16_t maxConnTime)*/
{

    m_pConnParams.scan_itvl = scanInterval;
    m_pConnParams.scan_window = scanWindow;
    m_pConnParams.itvl_min = minInterval;
    m_pConnParams.itvl_max = maxInterval;
    m_pConnParams.latency  = latency;
    m_pConnParams.supervision_timeout = timeout;

    // These are not used by NimBLE at this time - Must leave at defaults
    //m_pConnParams->min_ce_len = minConnTime;     // Minimum length of connection event in 0.625ms units
    //m_pConnParams->max_ce_len = maxConnTime;     // Maximum length of connection event in 0.625ms units
} // setConnectionParams


/**
 * @brief Update the connection parameters:
 * * Can only be used after a connection has been established.
 * @param [in] minInterval The minimum connection interval in 1.25ms units.
 * @param [in] maxInterval The maximum connection interval in 1.25ms units.
 * @param [in] latency The number of packets allowed to skip (extends max interval).
 * @param [in] timeout The timeout time in 10ms units before disconnecting.
 */
void NimBLEClient::updateConnParams(uint16_t minInterval, uint16_t maxInterval,
                            uint16_t latency, uint16_t timeout)
{
    ble_gap_upd_params params;

    params.latency  = latency;
    params.itvl_max = maxInterval;
    params.itvl_min = minInterval;
    params.supervision_timeout = timeout;
    // These are not used by NimBLE at this time - Must leave at defaults
    params.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN;
    params.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN;

    int rc = ble_gap_update_params(m_conn_id, &params);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Update params error: %d, %s",
                    rc, NimBLEUtils::returnCodeToString(rc));
    }
} // updateConnParams


/**
 * @brief Request an update of the data packet length.
 * * Can only be used after a connection has been established.
 * @details Sends a data length update request to the server the client is connected to.
 * The Data Length Extension (DLE) allows to increase the Data Channel Payload from 27 bytes to up to 251 bytes.
 * The server needs to support the Bluetooth 4.2 specifications, to be capable of DLE.
 * @param [in] tx_octets The preferred number of payload octets to use (Range 0x001B-0x00FB).
 */
void NimBLEClient::setDataLen(uint16_t tx_octets) {
#if defined(CONFIG_NIMBLE_CPP_IDF) && !defined(ESP_IDF_VERSION) || \
  (ESP_IDF_VERSION_MAJOR * 100 + ESP_IDF_VERSION_MINOR * 10 + ESP_IDF_VERSION_PATCH) < 432
    return;
#else
    uint16_t tx_time = (tx_octets + 14) * 8;

    int rc = ble_gap_set_data_len(m_conn_id, tx_octets, tx_time);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Set data length error: %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
    }
#endif
} // setDataLen


/**
 * @brief Get detailed information about the current peer connection.
 */
NimBLEConnInfo NimBLEClient::getConnInfo() {
    NimBLEConnInfo connInfo;
    if (!isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Not connected");
    } else {
        int rc = ble_gap_conn_find(m_conn_id, &connInfo.m_desc);
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Connection info not found");
        }
    }

    return connInfo;
} // getConnInfo


/**
 * @brief Set the timeout to wait for connection attempt to complete.
 * @param [in] time The number of milliseconds before timeout.
 */
void NimBLEClient::setConnectTimeout(uint32_t time) {
    m_connectTimeout = time;
} // setConnectTimeout


/**
 * @brief Get the connection id for this client.
 * @return The connection id.
 */
uint16_t NimBLEClient::getConnId() {
    return m_conn_id;
} // getConnId

/**
 * @brief Clear the connection information for this client.
 * @note This is designed to be used to reset the connection information after
 *       calling setConnection(), and should not be used to disconnect from a
 *       peer. To disconnect from a peer, use disconnect().
 */
void NimBLEClient::clearConnection() {
    m_conn_id = BLE_HS_CONN_HANDLE_NONE;
    m_connEstablished = false;
    m_peerAddress = NimBLEAddress();
} // clearConnection

/**
 * @brief Set the connection information for this client.
 * @param [in] connInfo The connection information.
 * @return True on success.
 * @note Sets the connection established flag to true.
 * @note If the client is already connected to a peer, this will return false.
 * @note This is designed to be used when a connection is made outside of the
 *       NimBLEClient class, such as when a connection is made by the
 *       NimBLEServer class and the client is passed the connection id. This use
 *       enables the GATT Server to read the name of the device that has
 *       connected to it.
 */
bool NimBLEClient::setConnection(NimBLEConnInfo &connInfo) {
    if (isConnected() || m_connEstablished) {
        NIMBLE_LOGE(LOG_TAG, "Already connected");
        return false;
    }

    m_peerAddress = connInfo.getAddress();
    m_conn_id = connInfo.getConnHandle();
    m_connEstablished = true;

    return true;
} // setConnection

/**
 * @brief Set the connection information for this client.
 * @param [in] conn_id The connection id.
 * @note Sets the connection established flag to true.
 * @note This is designed to be used when a connection is made outside of the
 *       NimBLEClient class, such as when a connection is made by the
 *       NimBLEServer class and the client is passed the connection id. This use
 *       enables the GATT Server to read the name of the device that has
 *       connected to it.
 * @note If the client is already connected to a peer, this will return false.
 * @note This will look up the peer address using the connection id.
 */
bool NimBLEClient::setConnection(uint16_t conn_id) {
    // we weren't provided the peer address, look it up using ble_gap_conn_find
    NimBLEConnInfo connInfo;
    int rc = ble_gap_conn_find(m_conn_id, &connInfo.m_desc);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Connection info not found");
        return false;
    }

    return setConnection(connInfo);
} // setConnection

/**
 * @brief Retrieve the address of the peer.
 */
NimBLEAddress NimBLEClient::getPeerAddress() {
    return m_peerAddress;
} // getPeerAddress


/**
 * @brief Set the peer address.
 * @param [in] address The address of the peer that this client is
 * connected or should connect to.
 */
void NimBLEClient::setPeerAddress(const NimBLEAddress &address) {
    if(isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Cannot set peer address while connected");
        return;
    }

    m_peerAddress = address;
    NIMBLE_LOGD(LOG_TAG, "Peer address set: %s", std::string(m_peerAddress).c_str());
} // setPeerAddress


/**
 * @brief Ask the BLE server for the RSSI value.
 * @return The RSSI value.
 */
int NimBLEClient::getRssi() {
    NIMBLE_LOGD(LOG_TAG, ">> getRssi()");
    if (!isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "<< getRssi(): Not connected");
        return 0;
    }

    int8_t rssiValue = 0;
    int rc = ble_gap_conn_rssi(m_conn_id, &rssiValue);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed to read RSSI error code: %d, %s",
                                rc, NimBLEUtils::returnCodeToString(rc));
        m_lastErr = rc;
        return 0;
    }

    return rssiValue;
} // getRssi


/**
 * @brief Get iterator to the beginning of the vector of remote service pointers.
 * @return An iterator to the beginning of the vector of remote service pointers.
 */
std::vector<NimBLERemoteService*>::iterator NimBLEClient::begin() {
    return m_servicesVector.begin();
}


/**
 * @brief Get iterator to the end of the vector of remote service pointers.
 * @return An iterator to the end of the vector of remote service pointers.
 */
std::vector<NimBLERemoteService*>::iterator NimBLEClient::end() {
    return m_servicesVector.end();
}


/**
 * @brief Get the service BLE Remote Service instance corresponding to the uuid.
 * @param [in] uuid The UUID of the service being sought.
 * @return A pointer to the service or nullptr if not found.
 */
NimBLERemoteService* NimBLEClient::getService(const char* uuid) {
    return getService(NimBLEUUID(uuid));
} // getService


/**
 * @brief Get the service object corresponding to the uuid.
 * @param [in] uuid The UUID of the service being sought.
 * @return A pointer to the service or nullptr if not found.
 */
NimBLERemoteService* NimBLEClient::getService(const NimBLEUUID &uuid) {
    NIMBLE_LOGD(LOG_TAG, ">> getService: uuid: %s", uuid.toString().c_str());

    for(auto &it: m_servicesVector) {
        if(it->getUUID() == uuid) {
            NIMBLE_LOGD(LOG_TAG, "<< getService: found the service with uuid: %s", uuid.toString().c_str());
            return it;
        }
    }

    size_t prev_size = m_servicesVector.size();
    if(retrieveServices(&uuid)) {
        if(m_servicesVector.size() > prev_size) {
            return m_servicesVector.back();
        }

        // If the request was successful but 16/32 bit uuid not found
        // try again with the 128 bit uuid.
        if(uuid.bitSize() == BLE_UUID_TYPE_16 ||
           uuid.bitSize() == BLE_UUID_TYPE_32)
        {
            NimBLEUUID uuid128(uuid);
            uuid128.to128();
            if(retrieveServices(&uuid128)) {
                if(m_servicesVector.size() > prev_size) {
                    return m_servicesVector.back();
                }
            }
        } else {
            // If the request was successful but the 128 bit uuid not found
            // try again with the 16 bit uuid.
            NimBLEUUID uuid16(uuid);
            uuid16.to16();
            // if the uuid was 128 bit but not of the BLE base type this check will fail
            if (uuid16.bitSize() == BLE_UUID_TYPE_16) {
                if(retrieveServices(&uuid16)) {
                    if(m_servicesVector.size() > prev_size) {
                        return m_servicesVector.back();
                    }
                }
            }
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< getService: not found");
    return nullptr;
} // getService


/**
 * @brief Get a pointer to the vector of found services.
 * @param [in] refresh If true the current services vector will be cleared and\n
 * all services will be retrieved from the peripheral.\n
 * If false the vector will be returned with the currently stored services.
 * @return A pointer to the vector of available services.
 */
std::vector<NimBLERemoteService*>* NimBLEClient::getServices(bool refresh) {
    if(refresh) {
        deleteServices();

        if (!retrieveServices()) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to get services");
        }
        else{
            NIMBLE_LOGI(LOG_TAG, "Found %d services", m_servicesVector.size());
        }
    }
    return &m_servicesVector;
} // getServices


/**
 * @brief Retrieves the full database of attributes that the peripheral has available.
 * @return True if successful.
 */
bool NimBLEClient::discoverAttributes() {
    deleteServices();

    if (!retrieveServices()){
        return false;
    }


    for(auto svc: m_servicesVector) {
        if (!svc->retrieveCharacteristics()) {
            return false;
        }

        for(auto chr: svc->m_characteristicVector) {
            if (!chr->retrieveDescriptors()) {
                return false;
            }
        }
    }

    return true;
} // discoverAttributes


/**
 * @brief Ask the remote %BLE server for its services.\n
 * Here we ask the server for its set of services and wait until we have received them all.
 * @return true on success otherwise false if an error occurred
 */
bool NimBLEClient::retrieveServices(const NimBLEUUID *uuid_filter) {
/**
 * Design
 * ------
 * We invoke ble_gattc_disc_all_svcs.  This will request a list of the services exposed by the
 * peer BLE partner to be returned in the callback function provided.
 */

    NIMBLE_LOGD(LOG_TAG, ">> retrieveServices");

    if(!isConnected()){
        NIMBLE_LOGE(LOG_TAG, "Disconnected, could not retrieve services -aborting");
        return false;
    }

    int rc = 0;
    TaskHandle_t cur_task = xTaskGetCurrentTaskHandle();
    ble_task_data_t taskData = {this, cur_task, 0, nullptr};

    if(uuid_filter == nullptr) {
        rc = ble_gattc_disc_all_svcs(m_conn_id, NimBLEClient::serviceDiscoveredCB, &taskData);
    } else {
        rc = ble_gattc_disc_svc_by_uuid(m_conn_id, &uuid_filter->getNative()->u,
                                        NimBLEClient::serviceDiscoveredCB, &taskData);
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_svcs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        m_lastErr = rc;
        return false;
    }

#ifdef ulTaskNotifyValueClear
    // Clear the task notification value to ensure we block
    ulTaskNotifyValueClear(cur_task, ULONG_MAX);
#endif

    // wait until we have all the services
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    m_lastErr = taskData.rc;

    if(taskData.rc == 0){
        NIMBLE_LOGD(LOG_TAG, "<< retrieveServices");
        return true;
    }
    else {
        NIMBLE_LOGE(LOG_TAG, "Could not retrieve services");
        return false;
    }
} // getServices


/**
 * @brief STATIC Callback for the service discovery API function.\n
 * When a service is found or there is none left or there was an error
 * the API will call this and report findings.
 */
int NimBLEClient::serviceDiscoveredCB(
                uint16_t conn_handle,
                const struct ble_gatt_error *error,
                const struct ble_gatt_svc *service, void *arg)
{
    NIMBLE_LOGD(LOG_TAG,"Service Discovered >> status: %d handle: %d",
                        error->status, (error->status == 0) ? service->start_handle : -1);

    ble_task_data_t *pTaskData = (ble_task_data_t*)arg;
    NimBLEClient *client = (NimBLEClient*)pTaskData->pATT;

    // Make sure the service discovery is for this device
    if(client->getConnId() != conn_handle){
        return 0;
    }

    if(error->status == 0) {
        // Found a service - add it to the vector
        NimBLERemoteService* pRemoteService = new NimBLERemoteService(client, service);
        client->m_servicesVector.push_back(pRemoteService);
        return 0;
    }

    if(error->status == BLE_HS_EDONE) {
        pTaskData->rc = 0;
    } else {
        NIMBLE_LOGE(LOG_TAG, "serviceDiscoveredCB() rc=%d %s",
                             error->status,
                             NimBLEUtils::returnCodeToString(error->status));
        pTaskData->rc = error->status;
    }

    xTaskNotifyGive(pTaskData->task);

    NIMBLE_LOGD(LOG_TAG,"<< Service Discovered");
    return error->status;
}


/**
 * @brief Get the value of a specific characteristic associated with a specific service.
 * @param [in] serviceUUID The service that owns the characteristic.
 * @param [in] characteristicUUID The characteristic whose value we wish to read.
 * @returns characteristic value or an empty string if not found
 */
NimBLEAttValue NimBLEClient::getValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID) {
    NIMBLE_LOGD(LOG_TAG, ">> getValue: serviceUUID: %s, characteristicUUID: %s",
                         serviceUUID.toString().c_str(), characteristicUUID.toString().c_str());

    NimBLEAttValue ret;
    NimBLERemoteService* pService = getService(serviceUUID);

    if(pService != nullptr) {
        NimBLERemoteCharacteristic* pChar = pService->getCharacteristic(characteristicUUID);
        if(pChar != nullptr) {
            ret = pChar->readValue();
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< getValue");
    return ret;
} // getValue


/**
 * @brief Set the value of a specific characteristic associated with a specific service.
 * @param [in] serviceUUID The service that owns the characteristic.
 * @param [in] characteristicUUID The characteristic whose value we wish to write.
 * @param [in] value The value to write to the characteristic.
 * @param [in] response If true, uses write with response operation.
 * @returns true if successful otherwise false
 */
bool NimBLEClient::setValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID,
                            const NimBLEAttValue &value, bool response)
{
    NIMBLE_LOGD(LOG_TAG, ">> setValue: serviceUUID: %s, characteristicUUID: %s",
                         serviceUUID.toString().c_str(), characteristicUUID.toString().c_str());

    bool ret = false;
    NimBLERemoteService* pService = getService(serviceUUID);

    if(pService != nullptr) {
        NimBLERemoteCharacteristic* pChar = pService->getCharacteristic(characteristicUUID);
        if(pChar != nullptr) {
            ret = pChar->writeValue(value, response);
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< setValue");
    return ret;
} // setValue


/**
 * @brief Get the remote characteristic with the specified handle.
 * @param [in] handle The handle of the desired characteristic.
 * @returns The matching remote characteristic, nullptr otherwise.
 */
NimBLERemoteCharacteristic* NimBLEClient::getCharacteristic(const uint16_t handle)
{
    NimBLERemoteService *pService = nullptr;
    for(auto it = m_servicesVector.begin(); it != m_servicesVector.end(); ++it) {
      if ((*it)->getStartHandle() <= handle && handle <= (*it)->getEndHandle()) {
          pService = *it;
          break;
      }
    }

    if (pService != nullptr) {
        for (auto it = pService->begin(); it != pService->end(); ++it) {
            if ((*it)->getHandle() == handle) {
                return *it;
            }
        }
    }

    return nullptr;
}

/**
 * @brief Get the current mtu of this connection.
 * @returns The MTU value.
 */
uint16_t NimBLEClient::getMTU() {
    return ble_att_mtu(m_conn_id);
} // getMTU


/**
 * @brief Handle a received GAP event.
 * @param [in] event The event structure sent by the NimBLE stack.
 * @param [in] arg A pointer to the client instance that registered for this callback.
 */
 /*STATIC*/
int NimBLEClient::handleGapEvent(struct ble_gap_event *event, void *arg) {
    NimBLEClient* pClient = (NimBLEClient*)arg;
    int rc;

    NIMBLE_LOGD(LOG_TAG, "Got Client event %s", NimBLEUtils::gapEventToString(event->type));

    switch(event->type) {

        case BLE_GAP_EVENT_DISCONNECT: {
            rc = event->disconnect.reason;
            // If Host reset tell the device now before returning to prevent
            // any errors caused by calling host functions before resyncing.
            switch(rc) {
                case BLE_HS_ECONTROLLER:
                case BLE_HS_ETIMEOUT_HCI:
                case BLE_HS_ENOTSYNCED:
                case BLE_HS_EOS:
                    NIMBLE_LOGE(LOG_TAG, "Disconnect - host reset, rc=%d", rc);
                    NimBLEDevice::onReset(rc);
                    break;
                default:
                    // Check that the event is for this client.
                    if(pClient->m_conn_id != event->disconnect.conn.conn_handle) {
                        return 0;
                    }
                    break;
            }

            // Stop the disconnect timer since we are now disconnected.
            ble_npl_callout_stop(&pClient->m_dcTimer);

            // Remove the device from ignore list so we will scan it again
            NimBLEDevice::removeIgnored(pClient->m_peerAddress);

            // No longer connected, clear the connection ID.
            pClient->m_conn_id = BLE_HS_CONN_HANDLE_NONE;

            // If we received a connected event but did not get established (no PDU)
            // then a disconnect event will be sent but we should not send it to the
            // app for processing. Instead we will ensure the task is released
            // and report the error.
            if(!pClient->m_connEstablished)
                break;

            NIMBLE_LOGI(LOG_TAG, "disconnect; reason=%d, %s",
                        rc, NimBLEUtils::returnCodeToString(rc));

            pClient->m_connEstablished = false;
            pClient->m_pClientCallbacks->onDisconnect(pClient, rc);
            break;
        } // BLE_GAP_EVENT_DISCONNECT

        case BLE_GAP_EVENT_CONNECT: {
            // If we aren't waiting for this connection response
            // we should drop the connection immediately.
            if(pClient->isConnected() || pClient->m_pTaskData == nullptr) {
                ble_gap_terminate(event->connect.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
                return 0;
            }

            rc = event->connect.status;
            if (rc == 0) {
                NIMBLE_LOGI(LOG_TAG, "Connected event");

                pClient->m_conn_id = event->connect.conn_handle;

                rc = ble_gattc_exchange_mtu(pClient->m_conn_id, NULL,NULL);
                if(rc != 0) {
                    NIMBLE_LOGE(LOG_TAG, "MTU exchange error; rc=%d %s",
                                rc, NimBLEUtils::returnCodeToString(rc));
                    break;
                }

                // In the case of a multi-connecting device we ignore this device when
                // scanning since we are already connected to it
                NimBLEDevice::addIgnored(pClient->m_peerAddress);
            } else {
                pClient->m_conn_id = BLE_HS_CONN_HANDLE_NONE;
                break;
            }

            return 0;
        } // BLE_GAP_EVENT_CONNECT

        case BLE_GAP_EVENT_NOTIFY_RX: {
            if(pClient->m_conn_id != event->notify_rx.conn_handle)
                return 0;

            // If a notification comes before this flag is set we might
            // access a vector while it is being cleared in connect()
            if(!pClient->m_connEstablished) {
                return 0;
            }

            NIMBLE_LOGD(LOG_TAG, "Notify Received for handle: %d",
                        event->notify_rx.attr_handle);

            for(auto &it: pClient->m_servicesVector) {
                // Dont waste cycles searching services without this handle in its range
                if(it->getEndHandle() < event->notify_rx.attr_handle) {
                    continue;
                }

                auto cVector = &it->m_characteristicVector;
                NIMBLE_LOGD(LOG_TAG, "checking service %s for handle: %d",
                            it->getUUID().toString().c_str(),
                            event->notify_rx.attr_handle);

                auto characteristic = cVector->cbegin();
                for(; characteristic != cVector->cend(); ++characteristic) {
                    if((*characteristic)->m_handle == event->notify_rx.attr_handle)
                        break;
                }

                if(characteristic != cVector->cend()) {
                    NIMBLE_LOGD(LOG_TAG, "Got Notification for characteristic %s",
                                (*characteristic)->toString().c_str());

                    uint32_t data_len = OS_MBUF_PKTLEN(event->notify_rx.om);
                    (*characteristic)->m_value.setValue(event->notify_rx.om->om_data, data_len);

                    if ((*characteristic)->m_notifyCallback != nullptr) {
                        NIMBLE_LOGD(LOG_TAG, "Invoking callback for notification on characteristic %s",
                                    (*characteristic)->toString().c_str());
                        (*characteristic)->m_notifyCallback(*characteristic, event->notify_rx.om->om_data,
                                                            data_len, !event->notify_rx.indication);
                    }
                    break;
                }
            }

            return 0;
        } // BLE_GAP_EVENT_NOTIFY_RX

        case BLE_GAP_EVENT_CONN_UPDATE_REQ:
        case BLE_GAP_EVENT_L2CAP_UPDATE_REQ: {
            if(pClient->m_conn_id != event->conn_update_req.conn_handle){
                return 0;
            }
            NIMBLE_LOGD(LOG_TAG, "Peer requesting to update connection parameters");
            NIMBLE_LOGD(LOG_TAG, "MinInterval: %d, MaxInterval: %d, Latency: %d, Timeout: %d",
                        event->conn_update_req.peer_params->itvl_min,
                        event->conn_update_req.peer_params->itvl_max,
                        event->conn_update_req.peer_params->latency,
                        event->conn_update_req.peer_params->supervision_timeout);

            rc = pClient->m_pClientCallbacks->onConnParamsUpdateRequest(pClient,
                                    event->conn_update_req.peer_params) ? 0 : BLE_ERR_CONN_PARMS;


            if(!rc && event->type == BLE_GAP_EVENT_CONN_UPDATE_REQ ) {
                event->conn_update_req.self_params->itvl_min = pClient->m_pConnParams.itvl_min;
                event->conn_update_req.self_params->itvl_max = pClient->m_pConnParams.itvl_max;
                event->conn_update_req.self_params->latency  = pClient->m_pConnParams.latency;
                event->conn_update_req.self_params->supervision_timeout = pClient->m_pConnParams.supervision_timeout;
            }

            NIMBLE_LOGD(LOG_TAG, "%s peer params", (rc == 0) ? "Accepted" : "Rejected");
            return rc;
        } // BLE_GAP_EVENT_CONN_UPDATE_REQ, BLE_GAP_EVENT_L2CAP_UPDATE_REQ

        case BLE_GAP_EVENT_CONN_UPDATE: {
            if(pClient->m_conn_id != event->conn_update.conn_handle){
                return 0;
            }
            if(event->conn_update.status == 0) {
                NIMBLE_LOGI(LOG_TAG, "Connection parameters updated.");
            } else {
                NIMBLE_LOGE(LOG_TAG, "Update connection parameters failed.");
            }
            return 0;
        } // BLE_GAP_EVENT_CONN_UPDATE

        case BLE_GAP_EVENT_ENC_CHANGE: {
            if(pClient->m_conn_id != event->enc_change.conn_handle){
                return 0;
            }

            if(event->enc_change.status == 0 ||
               event->enc_change.status == (BLE_HS_ERR_HCI_BASE + BLE_ERR_PINKEY_MISSING))
            {
                NimBLEConnInfo peerInfo;
                rc = ble_gap_conn_find(event->enc_change.conn_handle, &peerInfo.m_desc);
                if (rc != 0) {
                    NIMBLE_LOGE(LOG_TAG, "Connection info not found");
                    rc = 0;
                    break;
                }

                if (event->enc_change.status == (BLE_HS_ERR_HCI_BASE + BLE_ERR_PINKEY_MISSING)) {
                    // Key is missing, try deleting.
                    ble_store_util_delete_peer(&peerInfo.m_desc.peer_id_addr);
                } else {
                    pClient->m_pClientCallbacks->onAuthenticationComplete(peerInfo);
                }
            }

            rc = event->enc_change.status;
            break;
        } //BLE_GAP_EVENT_ENC_CHANGE

        case BLE_GAP_EVENT_IDENTITY_RESOLVED: {
            NimBLEConnInfo peerInfo;
            rc = ble_gap_conn_find(event->identity_resolved.conn_handle, &peerInfo.m_desc);
            if (rc != 0) {
                NIMBLE_LOGE(LOG_TAG, "Connection info not found");
                rc = 0;
                break;
            }

            pClient->m_pClientCallbacks->onIdentity(peerInfo);
            break;
        } // BLE_GAP_EVENT_IDENTITY_RESOLVED

        case BLE_GAP_EVENT_MTU: {
            if(pClient->m_conn_id != event->mtu.conn_handle){
                return 0;
            }
            NIMBLE_LOGI(LOG_TAG, "mtu update event; conn_handle=%d mtu=%d",
                        event->mtu.conn_handle,
                        event->mtu.value);
            rc = 0;
            break;
        } // BLE_GAP_EVENT_MTU

        case BLE_GAP_EVENT_PASSKEY_ACTION: {
            struct ble_sm_io pkey = {0,0};
            (void)pkey; //warning: variable 'pkey' set but not used [-Wunused-but-set-variable]

            if(pClient->m_conn_id != event->passkey.conn_handle)
                return 0;

            NimBLEConnInfo peerInfo;
            rc = ble_gap_conn_find(event->passkey.conn_handle, &peerInfo.m_desc);
            if (rc != 0) {
                NIMBLE_LOGE(LOG_TAG, "Connection info not found");
                rc = 0;
                break;
            }

            if (event->passkey.params.action == BLE_SM_IOACT_NUMCMP) {
                NIMBLE_LOGD(LOG_TAG, "Passkey on device's display: %" PRIu32, event->passkey.params.numcmp);
                pClient->m_pClientCallbacks->onConfirmPIN(peerInfo, event->passkey.params.numcmp);
            //TODO: Handle out of band pairing
            } else if (event->passkey.params.action == BLE_SM_IOACT_OOB) {
                static uint8_t tem_oob[16] = {0};
                pkey.action = event->passkey.params.action;
                for (int i = 0; i < 16; i++) {
                    pkey.oob[i] = tem_oob[i];
                }
                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "ble_sm_inject_io result: %d", rc);
            ////////
            } else if (event->passkey.params.action == BLE_SM_IOACT_INPUT) {
                NIMBLE_LOGD(LOG_TAG, "Enter the passkey");
                pClient->m_pClientCallbacks->onPassKeyEntry(peerInfo);
            } else if (event->passkey.params.action == BLE_SM_IOACT_NONE) {
                NIMBLE_LOGD(LOG_TAG, "No passkey action required");
            }

            return 0;
        } // BLE_GAP_EVENT_PASSKEY_ACTION

        default: {
            return 0;
        }
    } // Switch

    if(pClient->m_pTaskData != nullptr) {
        pClient->m_pTaskData->rc = rc;
        if(pClient->m_pTaskData->task) {
            xTaskNotifyGive(pClient->m_pTaskData->task);
        }
        pClient->m_pTaskData = nullptr;
    }

    return 0;
} // handleGapEvent


/**
 * @brief Are we connected to a server?
 * @return True if we are connected and false if we are not connected.
 */
bool NimBLEClient::isConnected() {
    return m_conn_id != BLE_HS_CONN_HANDLE_NONE;
} // isConnected


/**
 * @brief Set the callbacks that will be invoked when events are received.
 * @param [in] pClientCallbacks A pointer to a class to receive the event callbacks.
 * @param [in] deleteCallbacks If true this will delete the callback class sent when the client is destructed.
 */
void NimBLEClient::setClientCallbacks(NimBLEClientCallbacks* pClientCallbacks, bool deleteCallbacks) {
    if (pClientCallbacks != nullptr){
        m_pClientCallbacks = pClientCallbacks;
    } else {
        m_pClientCallbacks = &defaultCallbacks;
    }
    m_deleteCallbacks = deleteCallbacks;
} // setClientCallbacks


/**
 * @brief Return a string representation of this client.
 * @return A string representation of this client.
 */
std::string NimBLEClient::toString() {
    std::string res = "peer address: " + m_peerAddress.toString();
    res += "\nServices:\n";

    for(auto &it: m_servicesVector) {
        res += it->toString() + "\n";
    }

    return res;
} // toString


/**
 * @brief Get the last error code reported by the NimBLE host
 * @return int, the NimBLE error code.
 */
int NimBLEClient::getLastError() {
    return m_lastErr;
} // getLastError


void NimBLEClientCallbacks::onConnect(NimBLEClient* pClient) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConnect: default");
}

void NimBLEClientCallbacks::onDisconnect(NimBLEClient* pClient, int reason) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onDisconnect: default");
}

bool NimBLEClientCallbacks::onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConnParamsUpdateRequest: default");
    return true;
}

void NimBLEClientCallbacks::onPassKeyEntry(const NimBLEConnInfo& connInfo){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onPassKeyEntry: default: 123456");
    NimBLEDevice::injectPassKey(connInfo, 123456);
} //onPassKeyEntry

void NimBLEClientCallbacks::onAuthenticationComplete(const NimBLEConnInfo& connInfo){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onAuthenticationComplete: default");
}

void NimBLEClientCallbacks::onIdentity(const NimBLEConnInfo& connInfo){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onIdentity: default");
} // onIdentity

void NimBLEClientCallbacks::onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pin){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConfirmPIN: default: true");
    NimBLEDevice::injectConfirmPIN(connInfo, true);
}

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_CENTRAL */
