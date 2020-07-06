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

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLEClient.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#include <string>
#include <unordered_set>

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

NimBLEClient::NimBLEClient()
{
    m_pClientCallbacks = &defaultCallbacks;
    m_conn_id          = BLE_HS_CONN_HANDLE_NONE;
    m_isConnected      = false;
    m_waitingToConnect = false;
    m_connectTimeout   = 30000;
    m_deleteCallbacks  = false;
    m_pTaskData        = nullptr;

    m_pConnParams.scan_itvl = 16;          // Scan interval in 0.625ms units (NimBLE Default)
    m_pConnParams.scan_window = 16;        // Scan window in 0.625ms units (NimBLE Default)
    m_pConnParams.itvl_min = BLE_GAP_INITIAL_CONN_ITVL_MIN;  // min_int = 0x10*1.25ms = 20ms
    m_pConnParams.itvl_max = BLE_GAP_INITIAL_CONN_ITVL_MAX;  // max_int = 0x20*1.25ms = 40ms
    m_pConnParams.latency  = BLE_GAP_INITIAL_CONN_LATENCY;      // number of packets allowed to skip (extends max interval)
    m_pConnParams.supervision_timeout = BLE_GAP_INITIAL_SUPERVISION_TIMEOUT; // timeout = 400*10ms = 4000ms
    m_pConnParams.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN; // Minimum length of connection event in 0.625ms units
    m_pConnParams.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN; // Maximum length of connection event in 0.625ms units
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

} // ~NimBLEClient


/**
 * @brief Delete any existing services.
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
 * NOT NEEDED
 */
 /*
void NimBLEClient::onHostReset() {

}
 */

/**
 * Add overloaded function to ease connect to peer device with not public address
 */
bool NimBLEClient::connect(NimBLEAdvertisedDevice* device, bool refreshServices) {
    NimBLEAddress address(device->getAddress());
    uint8_t type = device->getAddressType();
    return connect(address, type, refreshServices);
}


/**
 * @brief Connect to the partner (BLE Server).
 * @param [in] address The address of the partner.
 * @return True on success.
 */
bool NimBLEClient::connect(const NimBLEAddress &address, uint8_t type, bool refreshServices) {
    NIMBLE_LOGD(LOG_TAG, ">> connect(%s)", address.toString().c_str());

    if(!NimBLEDevice::m_synced) {
        NIMBLE_LOGC(LOG_TAG, "Host reset, wait for sync.");
        return false;
    }

    if(ble_gap_conn_active()) {
        NIMBLE_LOGE(LOG_TAG, "Connection in progress - must wait.");
        return false;
    }

    if(!NimBLEDevice::getScan()->stop()) {
        return false;
    }

    int rc = 0;
    m_peerAddress = address;

    ble_addr_t peerAddrt;
    memcpy(&peerAddrt.val, address.getNative(),6);
    peerAddrt.type = type;

    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(), 0, nullptr};
    m_pTaskData = &taskData;

    /** Try to connect the the advertiser.  Allow 30 seconds (30000 ms) for
     *  timeout (default value of m_connectTimeout).
     *  Loop on BLE_HS_EBUSY if the scan hasn't stopped yet.
     */
    do{
        rc = ble_gap_connect(BLE_OWN_ADDR_PUBLIC, &peerAddrt, m_connectTimeout, &m_pConnParams,
                            NimBLEClient::handleGapEvent, this);
        if(rc == BLE_HS_EBUSY) {
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
    }while(rc == BLE_HS_EBUSY);

    if (rc != 0 && rc != BLE_HS_EDONE) {
        NIMBLE_LOGE(LOG_TAG, "Error: Failed to connect to device; addr_type=%d "
                    "addr=%s, rc=%d; %s",
                    type,
                    m_peerAddress.toString().c_str(),
                    rc, NimBLEUtils::returnCodeToString(rc));
        m_pTaskData = nullptr;
        m_waitingToConnect = false;
        return false;
    }

    m_waitingToConnect = true;

    // Wait for the connection to complete.
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if(taskData.rc != 0){
        return false;
    }

    if(refreshServices) {
        NIMBLE_LOGD(LOG_TAG, "Refreshing Services for: (%s)", address.toString().c_str());
        deleteServices();
    }

    m_pClientCallbacks->onConnect(this);

    NIMBLE_LOGD(LOG_TAG, "<< connect()");
    return true;
} // connect


/**
 * @brief Called when a characteristic or descriptor requires encryption or authentication to access it.
 * This will pair with the device and bond if enabled.
 * @return True on success.
 */
bool NimBLEClient::secureConnection() {
    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(), 0, nullptr};
    m_pTaskData = &taskData;

    int rc = NimBLEDevice::startSecurity(m_conn_id);
    if(rc != 0){
        m_pTaskData = nullptr;
        return false;
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if(taskData.rc != 0){
        return false;
    }

    return true;
}


/**
 * @brief Disconnect from the peer.
 * @return N/A.
 */
int NimBLEClient::disconnect(uint8_t reason) {
    NIMBLE_LOGD(LOG_TAG, ">> disconnect()");
    int rc = 0;
    if(m_isConnected){
        rc = ble_gap_terminate(m_conn_id, reason);
        if(rc != 0){
            NIMBLE_LOGE(LOG_TAG, "ble_gap_terminate failed: rc=%d %s", rc,
                                    NimBLEUtils::returnCodeToString(rc));
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< disconnect()");
    return rc;
} // disconnect


/**
 * @brief Set the connection paramaters to use when connecting to a server.
 */
void NimBLEClient::setConnectionParams(uint16_t minInterval, uint16_t maxInterval,
                                uint16_t latency, uint16_t timeout,
                                uint16_t scanInterval, uint16_t scanWindow)/*,
                                uint16_t minConnTime, uint16_t maxConnTime)*/
{

    m_pConnParams.scan_itvl = scanInterval;      // Scan interval in 0.625ms units
    m_pConnParams.scan_window = scanWindow;      // Scan window in 0.625ms units
    m_pConnParams.itvl_min = minInterval;        // min_int = 0x10*1.25ms = 20ms
    m_pConnParams.itvl_max = maxInterval;        // max_int = 0x20*1.25ms = 40ms
    m_pConnParams.latency  = latency;            // number of packets allowed to skip (extends max interval)
    m_pConnParams.supervision_timeout = timeout; // timeout = 400*10ms = 4000ms

    // These are not used by NimBLE at this time - Must leave at defaults
    //m_pConnParams->min_ce_len = minConnTime;     // Minimum length of connection event in 0.625ms units
    //m_pConnParams->max_ce_len = maxConnTime;     // Maximum length of connection event in 0.625ms units

    int rc = NimBLEUtils::checkConnParams(&m_pConnParams);
    assert(rc == 0 && "Invalid Connection parameters");
}


/**
 * Update connection parameters can be called only after connection has been established
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
}


/**
 * @brief Set the timeout to wait for connection attempt to complete
 * @params[in] Time to wait in seconds.
 */
void NimBLEClient::setConnectTimeout(uint8_t time) {
    m_connectTimeout = (uint32_t)(time * 1000);
}


/**
 * @brief Get the connection id for this client.
 * @return The connection id.
 */
uint16_t NimBLEClient::getConnId() {
    return m_conn_id;
} // getConnId


/**
 * @brief Retrieve the address of the peer.
 */
NimBLEAddress NimBLEClient::getPeerAddress() {
    return m_peerAddress;
} // getAddress


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
 * @return A reference to the Service or nullptr if don't know about it.
 */
NimBLERemoteService* NimBLEClient::getService(const char* uuid) {
    return getService(NimBLEUUID(uuid));
} // getService


/**
 * @brief Get the service object corresponding to the uuid.
 * @param [in] uuid The UUID of the service being sought.
 * @return A reference to the Service or nullptr if don't know about it.
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
    }

    NIMBLE_LOGD(LOG_TAG, "<< getService: not found");
    return nullptr;
} // getService


/**
 * @Get a pointer to the vector of found services.
 * @param [in] bool value to indicate if the current vector should be cleared and
 * subsequently all services retrieved from the peripheral.
 * If false the vector will be returned with the currently stored services,
 * If true it will retrieve all services from the peripheral and return the vector with all services
 * @return a pointer to the vector of available services.
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
}


/**
 * @ Retrieves the full database of attributes that the peripheral has available.
 */
void NimBLEClient::discoverAttributes() {
    for(auto svc: *getServices(true)) {
        for(auto chr: *svc->getCharacteristics(true)) {
            chr->getDescriptors(true);
        }
    }
}


/**
 * @brief Ask the remote %BLE server for its services.
 * A %BLE Server exposes a set of services for its partners.  Here we ask the server for its set of
 * services and wait until we have received them all.
 * We then ask for the characteristics for each service found and their desciptors.
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

    if(!m_isConnected){
        NIMBLE_LOGE(LOG_TAG, "Disconnected, could not retrieve services -aborting");
        return false;
    }

    int rc = 0;
    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(), 0, nullptr};

    if(uuid_filter == nullptr) {
        rc = ble_gattc_disc_all_svcs(m_conn_id, NimBLEClient::serviceDiscoveredCB, &taskData);
    } else {
        rc = ble_gattc_disc_svc_by_uuid(m_conn_id, &uuid_filter->getNative()->u,
                                        NimBLEClient::serviceDiscoveredCB, &taskData);
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_svcs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    // wait until we have all the services
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

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
 * @brief STATIC Callback for the service discovery API function.
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
        NIMBLE_LOGE(LOG_TAG, "characteristicDiscCB() rc=%d %s",
                             error->status,
                             NimBLEUtils::returnCodeToString(error->status));
        pTaskData->rc = error->status;
    }

    xTaskNotifyGive(pTaskData->task);

    NIMBLE_LOGD(LOG_TAG,"<< << Service Discovered");
    return error->status;
}


/**
 * @brief Get the value of a specific characteristic associated with a specific service.
 * @param [in] serviceUUID The service that owns the characteristic.
 * @param [in] characteristicUUID The characteristic whose value we wish to read.
 * @returns characteristic value or an empty string if not found
 */
std::string NimBLEClient::getValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID) {
    NIMBLE_LOGD(LOG_TAG, ">> getValue: serviceUUID: %s, characteristicUUID: %s",
                         serviceUUID.toString().c_str(), characteristicUUID.toString().c_str());

    std::string ret = "";
    NimBLERemoteService* pService = getService(serviceUUID);

    if(pService != nullptr) {
        NimBLERemoteCharacteristic* pChar = pService->getCharacteristic(characteristicUUID);
        if(pChar != nullptr) {
            ret = pChar->readValue();
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<<getValue");
    return ret;
} // getValue


/**
 * @brief Set the value of a specific characteristic associated with a specific service.
 * @param [in] serviceUUID The service that owns the characteristic.
 * @param [in] characteristicUUID The characteristic whose value we wish to write.
 * @returns true if successful otherwise false
 */
bool NimBLEClient::setValue(const NimBLEUUID &serviceUUID, const NimBLEUUID &characteristicUUID,
                            const std::string &value)
{
    NIMBLE_LOGD(LOG_TAG, ">> setValue: serviceUUID: %s, characteristicUUID: %s",
                         serviceUUID.toString().c_str(), characteristicUUID.toString().c_str());

    bool ret = false;
    NimBLERemoteService* pService = getService(serviceUUID);

    if(pService != nullptr) {
        NimBLERemoteCharacteristic* pChar = pService->getCharacteristic(characteristicUUID);
        if(pChar != nullptr) {
            ret = pChar->writeValue(value);
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< setValue");
    return ret;
} // setValue



/**
 * @brief Get the current mtu of this connection.
 */
uint16_t NimBLEClient::getMTU() {
    return ble_att_mtu(m_conn_id);
}


/**
 * @brief Handle a received GAP event.
 *
 * @param [in] event
 * @param [in] arg = pointer to the client instance
 */
 /*STATIC*/ int NimBLEClient::handleGapEvent(struct ble_gap_event *event, void *arg) {
    NimBLEClient* client = (NimBLEClient*)arg;
    int rc;

    NIMBLE_LOGD(LOG_TAG, "Got Client event %s", NimBLEUtils::gapEventToString(event->type));

    switch(event->type) {

        case BLE_GAP_EVENT_DISCONNECT: {
            if(!client->m_isConnected)
                return 0;

            if(client->m_conn_id != event->disconnect.conn.conn_handle)
                return 0;

            client->m_isConnected = false;
            client->m_waitingToConnect=false;
            // Remove the device from ignore list so we will scan it again
            NimBLEDevice::removeIgnored(client->m_peerAddress);

            NIMBLE_LOGI(LOG_TAG, "disconnect; reason=%d, %s", event->disconnect.reason,
                                    NimBLEUtils::returnCodeToString(event->disconnect.reason));

            // If Host reset tell the device now before returning to prevent
            // any errors caused by calling host functions before resyncing.
            switch(event->disconnect.reason) {
                case BLE_HS_ETIMEOUT_HCI:
                case BLE_HS_EOS:
                case BLE_HS_ECONTROLLER:
                case BLE_HS_ENOTSYNCED:
                    NIMBLE_LOGC(LOG_TAG, "Disconnect - host reset, rc=%d", event->disconnect.reason);
                    NimBLEDevice::onReset(event->disconnect.reason);
                    break;
                default:
                    break;
            }

            //client->m_conn_id = BLE_HS_CONN_HANDLE_NONE;
            client->m_pClientCallbacks->onDisconnect(client);
            rc = event->disconnect.reason;
            break;
        } // BLE_GAP_EVENT_DISCONNECT

        case BLE_GAP_EVENT_CONNECT: {

            if(!client->m_waitingToConnect)
                return 0;

            //if(client->m_conn_id != BLE_HS_CONN_HANDLE_NONE)
            //  return 0;

            client->m_waitingToConnect=false;

            if (event->connect.status == 0) {
                client->m_isConnected = true;

                NIMBLE_LOGD(LOG_TAG, "Connection established");

                client->m_conn_id = event->connect.conn_handle;

                rc = ble_gattc_exchange_mtu(client->m_conn_id, NULL,NULL);
                if(rc != 0) {
                    NIMBLE_LOGE(LOG_TAG, "ble_gattc_exchange_mtu: rc=%d %s",rc,
                                            NimBLEUtils::returnCodeToString(rc));
                    break;
                }

                // In the case of a multiconnecting device we ignore this device when
                // scanning since we are already connected to it
                NimBLEDevice::addIgnored(client->m_peerAddress);
            } else {
                NIMBLE_LOGE(LOG_TAG, "Error: Connection failed; status=%d %s",
                            event->connect.status,
                            NimBLEUtils::returnCodeToString(event->connect.status));

                client->m_isConnected = false;
                rc = event->connect.status;
                break;
            }
            return 0;
        } // BLE_GAP_EVENT_CONNECT

        case BLE_GAP_EVENT_NOTIFY_RX: {
            if(client->m_conn_id != event->notify_rx.conn_handle)
                return 0;

            NIMBLE_LOGD(LOG_TAG, "Notify Recieved for handle: %d",event->notify_rx.attr_handle);

            for(auto &it: client->m_servicesVector) {
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
                    NIMBLE_LOGD(LOG_TAG, "Got Notification for characteristic %s", (*characteristic)->toString().c_str());

                    portENTER_CRITICAL(&(*characteristic)->m_valMux);
                    (*characteristic)->m_value = std::string((char *)event->notify_rx.om->om_data, event->notify_rx.om->om_len);
                    (*characteristic)->m_timestamp = time(nullptr);
                    portEXIT_CRITICAL(&(*characteristic)->m_valMux);

                    if ((*characteristic)->m_notifyCallback != nullptr) {
                        NIMBLE_LOGD(LOG_TAG, "Invoking callback for notification on characteristic %s",
                                             (*characteristic)->toString().c_str());
                        (*characteristic)->m_notifyCallback(*characteristic, event->notify_rx.om->om_data,
                                                            event->notify_rx.om->om_len,
                                                            !event->notify_rx.indication);
                    }
                    break;
                }
            }

            return 0;
        } // BLE_GAP_EVENT_NOTIFY_RX

        case BLE_GAP_EVENT_CONN_UPDATE_REQ:
        case BLE_GAP_EVENT_L2CAP_UPDATE_REQ: {
            if(client->m_conn_id != event->conn_update_req.conn_handle){
                return 0;
            }
            NIMBLE_LOGD(LOG_TAG, "Peer requesting to update connection parameters");
            NIMBLE_LOGD(LOG_TAG, "MinInterval: %d, MaxInterval: %d, Latency: %d, Timeout: %d",
                                    event->conn_update_req.peer_params->itvl_min,
                                    event->conn_update_req.peer_params->itvl_max,
                                    event->conn_update_req.peer_params->latency,
                                    event->conn_update_req.peer_params->supervision_timeout);

            rc = client->m_pClientCallbacks->onConnParamsUpdateRequest(client,
                                    event->conn_update_req.peer_params) ? 0 : BLE_ERR_CONN_PARMS;


            if(!rc && event->type == BLE_GAP_EVENT_CONN_UPDATE_REQ ) {
                event->conn_update_req.self_params->itvl_min = client->m_pConnParams.itvl_min;
                event->conn_update_req.self_params->itvl_max = client->m_pConnParams.itvl_max;
                event->conn_update_req.self_params->latency  = client->m_pConnParams.latency;
                event->conn_update_req.self_params->supervision_timeout = client->m_pConnParams.supervision_timeout;
            }

            NIMBLE_LOGD(LOG_TAG, "%s peer params", (rc == 0) ? "Accepted" : "Rejected");
            return rc;
        } // BLE_GAP_EVENT_CONN_UPDATE_REQ, BLE_GAP_EVENT_L2CAP_UPDATE_REQ

        case BLE_GAP_EVENT_CONN_UPDATE: {
            if(client->m_conn_id != event->conn_update.conn_handle){
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
            if(client->m_conn_id != event->enc_change.conn_handle){
                return 0;
            }

            if(event->enc_change.status == 0) {
                struct ble_gap_conn_desc desc;
                rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
                assert(rc == 0);

                if(NimBLEDevice::m_securityCallbacks != nullptr) {
                    NimBLEDevice::m_securityCallbacks->onAuthenticationComplete(&desc);
                } else {
                    client->m_pClientCallbacks->onAuthenticationComplete(&desc);
                }
            }

            rc = event->enc_change.status;
            break;
        } //BLE_GAP_EVENT_ENC_CHANGE

        case BLE_GAP_EVENT_MTU: {
            if(client->m_conn_id != event->mtu.conn_handle){
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

            if(client->m_conn_id != event->passkey.conn_handle)
                return 0;

            if (event->passkey.params.action == BLE_SM_IOACT_DISP) {
                pkey.action = event->passkey.params.action;
                pkey.passkey = NimBLEDevice::m_passkey; // This is the passkey to be entered on peer
                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "ble_sm_inject_io result: %d", rc);

            } else if (event->passkey.params.action == BLE_SM_IOACT_NUMCMP) {
                NIMBLE_LOGD(LOG_TAG, "Passkey on device's display: %d", event->passkey.params.numcmp);
                pkey.action = event->passkey.params.action;
                // Compatibility only - Do not use, should be removed the in future
                if(NimBLEDevice::m_securityCallbacks != nullptr) {
                    pkey.numcmp_accept = NimBLEDevice::m_securityCallbacks->onConfirmPIN(event->passkey.params.numcmp);
                ////////////////////////////////////////////////////
                } else {
                    pkey.numcmp_accept = client->m_pClientCallbacks->onConfirmPIN(event->passkey.params.numcmp);
                }

                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "ble_sm_inject_io result: %d", rc);

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
                pkey.action = event->passkey.params.action;

                // Compatibility only - Do not use, should be removed the in future
                if(NimBLEDevice::m_securityCallbacks != nullptr) {
                    pkey.passkey = NimBLEDevice::m_securityCallbacks->onPassKeyRequest();
                /////////////////////////////////////////////
                } else {
                    client->m_pClientCallbacks->onPassKeyRequest();
                }

                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "ble_sm_inject_io result: %d", rc);

            } else if (event->passkey.params.action == BLE_SM_IOACT_NONE) {
                NIMBLE_LOGD(LOG_TAG, "No passkey action required");
            }

            return 0;
        } // BLE_GAP_EVENT_PASSKEY_ACTION

        default: {
            return 0;
        }
    } // Switch

    if(client->m_pTaskData != nullptr) {
        client->m_pTaskData->rc = rc;
        xTaskNotifyGive(client->m_pTaskData->task);
        client->m_pTaskData = nullptr;
    }

    return 0;
} // handleGapEvent


/**
 * @brief Are we connected to a server?
 * @return True if we are connected and false if we are not connected.
 */
bool NimBLEClient::isConnected() {
    return m_isConnected;
} // isConnected


/**
 * @brief Set the callbacks that will be invoked.
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


void NimBLEClientCallbacks::onConnect(NimBLEClient* pClient) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConnect: default");
}

void NimBLEClientCallbacks::onDisconnect(NimBLEClient* pClient) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onDisconnect: default");
}

bool NimBLEClientCallbacks::onConnParamsUpdateRequest(NimBLEClient* pClient, const ble_gap_upd_params* params) {
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConnParamsUpdateRequest: default");
    return true;
}

uint32_t NimBLEClientCallbacks::onPassKeyRequest(){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onPassKeyRequest: default: 123456");
    return 123456;
}

void NimBLEClientCallbacks::onPassKeyNotify(uint32_t pass_key){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onPassKeyNotify: default: %d", pass_key);
}

bool NimBLEClientCallbacks::onSecurityRequest(){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onSecurityRequest: default: true");
    return true;
}
void NimBLEClientCallbacks::onAuthenticationComplete(ble_gap_conn_desc* desc){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onAuthenticationComplete: default");
}
bool NimBLEClientCallbacks::onConfirmPIN(uint32_t pin){
    NIMBLE_LOGD("NimBLEClientCallbacks", "onConfirmPIN: default: true");
    return true;
}

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif // CONFIG_BT_ENABLED
