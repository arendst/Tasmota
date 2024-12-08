/*
 * NimBLEServer.cpp
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEServer.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: kolban
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEServer.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#else
#include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
#include "nimble/nimble/host/services/gatt/include/services/gatt/ble_svc_gatt.h"
#endif

#include <limits.h>

#define NIMBLE_SERVER_GET_PEER_NAME_ON_CONNECT_CB 0
#define NIMBLE_SERVER_GET_PEER_NAME_ON_AUTH_CB 1

static const char* LOG_TAG = "NimBLEServer";
static NimBLEServerCallbacks defaultCallbacks;


/**
 * @brief Construct a %BLE Server
 *
 * This class is not designed to be individually instantiated.  Instead one should create a server by asking
 * the NimBLEDevice class.
 */
NimBLEServer::NimBLEServer() {
    memset(m_indWait, BLE_HS_CONN_HANDLE_NONE, sizeof(m_indWait));
//    m_svcChgChrHdl          = 0xffff; // Future Use
    m_pServerCallbacks      = &defaultCallbacks;
    m_gattsStarted          = false;
#if !CONFIG_BT_NIMBLE_EXT_ADV
    m_advertiseOnDisconnect = true;
#endif
    m_svcChanged            = false;
    m_deleteCallbacks       = true;
    m_getPeerNameOnConnect  = false;
} // NimBLEServer


/**
 * @brief Destructor: frees all resources / attributes created.
 */
NimBLEServer::~NimBLEServer() {
    for(auto &it : m_svcVec) {
        delete it;
    }

    if(m_deleteCallbacks && m_pServerCallbacks != &defaultCallbacks) {
        delete m_pServerCallbacks;
    }
}


/**
 * @brief Create a %BLE Service.
 * @param [in] uuid The UUID of the new service.
 * @return A reference to the new service object.
 */
NimBLEService* NimBLEServer::createService(const char* uuid) {
    return createService(NimBLEUUID(uuid));
} // createService


/**
 * @brief Create a %BLE Service.
 * @param [in] uuid The UUID of the new service.
 * @return A reference to the new service object.
 */
NimBLEService* NimBLEServer::createService(const NimBLEUUID &uuid) {
    NIMBLE_LOGD(LOG_TAG, ">> createService - %s", uuid.toString().c_str());

    // Check that a service with the supplied UUID does not already exist.
    if(getServiceByUUID(uuid) != nullptr) {
        NIMBLE_LOGW(LOG_TAG, "Warning creating a duplicate service UUID: %s",
                             std::string(uuid).c_str());
    }

    NimBLEService* pService = new NimBLEService(uuid);
    m_svcVec.push_back(pService);
    serviceChanged();

    NIMBLE_LOGD(LOG_TAG, "<< createService");
    return pService;
} // createService


/**
 * @brief Get a %BLE Service by its UUID
 * @param [in] uuid The UUID of the service.
 * @param instanceId The index of the service to return (used when multiple services have the same UUID).
 * @return A pointer to the service object or nullptr if not found.
 */
NimBLEService* NimBLEServer::getServiceByUUID(const char* uuid, uint16_t instanceId) {
    return getServiceByUUID(NimBLEUUID(uuid), instanceId);
} // getServiceByUUID


/**
 * @brief Get a %BLE Service by its UUID
 * @param [in] uuid The UUID of the service.
 * @param instanceId The index of the service to return (used when multiple services have the same UUID).
 * @return A pointer to the service object or nullptr if not found.
 */
NimBLEService* NimBLEServer::getServiceByUUID(const NimBLEUUID &uuid, uint16_t instanceId) {
    uint16_t position = 0;
    for (auto &it : m_svcVec) {
        if (it->getUUID() == uuid) {
            if (position == instanceId){
                return it;
            }
            position++;
        }
    }
    return nullptr;
} // getServiceByUUID

/**
 * @brief Get a %BLE Service by its handle
 * @param handle The handle of the service.
 * @return A pointer to the service object or nullptr if not found.
 */
NimBLEService *NimBLEServer::getServiceByHandle(uint16_t handle) {
    for (auto &it : m_svcVec) {
        if (it->getHandle() == handle) {
            return it;
        }
    }
    return nullptr;
}


#if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Retrieve the advertising object that can be used to advertise the existence of the server.
 * @return An advertising object.
 */
NimBLEExtAdvertising* NimBLEServer::getAdvertising() {
    return NimBLEDevice::getAdvertising();
} // getAdvertising
#endif

#if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
/**
 * @brief Retrieve the advertising object that can be used to advertise the existence of the server.
 * @return An advertising object.
 */
NimBLEAdvertising* NimBLEServer::getAdvertising() {
    return NimBLEDevice::getAdvertising();
} // getAdvertising
#endif

/**
 * @brief Sends a service changed notification and resets the GATT server.
 */
void NimBLEServer::serviceChanged() {
    if(m_gattsStarted) {
        m_svcChanged = true;
        ble_svc_gatt_changed(0x0001, 0xffff);
        resetGATT();
    }
}


/**
 * @brief Start the GATT server. Required to be called after setup of all
 * services and characteristics / descriptors for the NimBLE host to register them.
 */
void NimBLEServer::start() {
    if(m_gattsStarted) {
        NIMBLE_LOGW(LOG_TAG, "Gatt server already started");
        return;
    }

    int rc = ble_gatts_start();
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_start; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return;
    }

#if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 4
    ble_gatts_show_local();
#endif
/*** Future use ***
  * TODO: implement service changed handling

    ble_uuid16_t svc = {BLE_UUID_TYPE_16, 0x1801};
    ble_uuid16_t chr = {BLE_UUID_TYPE_16, 0x2a05};

    rc = ble_gatts_find_chr(&svc.u, &chr.u, NULL, &m_svcChgChrHdl);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_find_chr: rc=%d, %s", rc,
                            NimBLEUtils::returnCodeToString(rc));
        abort();
    }

    NIMBLE_LOGI(LOG_TAG, "Service changed characterisic handle: %d", m_svcChgChrHdl);
*/
    // Get the assigned service handles and build a vector of characteristics
    // with Notify / Indicate capabilities for event handling
    for(auto &svc : m_svcVec) {
        if(svc->m_removed == 0) {
            rc = ble_gatts_find_svc(&svc->getUUID().getNative()->u, &svc->m_handle);
            if(rc != 0) {
                NIMBLE_LOGW(LOG_TAG, "GATT Server started without service: %s, Service %s",
                            svc->getUUID().toString().c_str(), svc->isStarted() ? "missing" : "not started");
                continue; // Skip this service as it was not started
            }
        }

        for(auto &chr : svc->m_chrVec) {
            // if Notify / Indicate is enabled but we didn't create the descriptor
            // we do it now.
            if((chr->m_properties & BLE_GATT_CHR_F_INDICATE) ||
                (chr->m_properties & BLE_GATT_CHR_F_NOTIFY)) {
                m_notifyChrVec.push_back(chr);
            }
        }
    }

    m_gattsStarted = true;
} // start


/**
 * @brief Disconnect the specified client with optional reason.
 * @param [in] connId Connection Id of the client to disconnect.
 * @param [in] reason code for disconnecting.
 * @return NimBLE host return code.
 */
int NimBLEServer::disconnect(uint16_t connId, uint8_t reason) {
    NIMBLE_LOGD(LOG_TAG, ">> disconnect()");

    int rc = ble_gap_terminate(connId, reason);
    if(rc != 0){
        NIMBLE_LOGE(LOG_TAG, "ble_gap_terminate failed: rc=%d %s", rc,
                                    NimBLEUtils::returnCodeToString(rc));
    }

    NIMBLE_LOGD(LOG_TAG, "<< disconnect()");
    return rc;
} // disconnect

/**
 * @brief Disconnect the specified client with optional reason.
 * @param [in] connInfo Connection of the client to disconnect.
 * @param [in] reason code for disconnecting.
 * @return NimBLE host return code.
 */
int NimBLEServer::disconnect(const NimBLEConnInfo &connInfo, uint8_t reason) {
    return disconnect(connInfo.getConnHandle(), reason);
} // disconnect

#if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
/**
 * @brief Set the server to automatically start advertising when a client disconnects.
 * @param [in] aod true == advertise, false == don't advertise.
 */
void NimBLEServer::advertiseOnDisconnect(bool aod) {
    m_advertiseOnDisconnect = aod;
} // advertiseOnDisconnect
#endif

/**
 * @brief Set the server to automatically read the name from the connected peer before
 * the onConnect callback is called and enables the override callback with name parameter.
 * @param [in] enable Enable reading the connected peer name upon connection.
 */
void NimBLEServer::getPeerNameOnConnect(bool enable) {
    m_getPeerNameOnConnect = enable;
} // getPeerNameOnConnect

/**
 * @brief Return the number of connected clients.
 * @return The number of connected clients.
 */
size_t NimBLEServer::getConnectedCount() {
    return m_connectedPeersVec.size();
} // getConnectedCount


/**
 * @brief Get the vector of the connected client ID's.
 */
std::vector<uint16_t> NimBLEServer::getPeerDevices() {
    return m_connectedPeersVec;
} // getPeerDevices


/**
 * @brief Get the connection information of a connected peer by vector index.
 * @param [in] index The vector index of the peer.
 */
NimBLEConnInfo NimBLEServer::getPeerInfo(size_t index) {
    if (index >= m_connectedPeersVec.size()) {
        NIMBLE_LOGE(LOG_TAG, "No peer at index %u", index);
        return NimBLEConnInfo();
    }

    return getPeerIDInfo(m_connectedPeersVec[index]);
} // getPeerInfo


/**
 * @brief Get the connection information of a connected peer by address.
 * @param [in] address The address of the peer.
 */
NimBLEConnInfo NimBLEServer::getPeerInfo(const NimBLEAddress& address) {
    ble_addr_t peerAddr;
    memcpy(&peerAddr.val, address.getNative(),6);
    peerAddr.type = address.getType();

    NimBLEConnInfo peerInfo;
    int rc = ble_gap_conn_find_by_addr(&peerAddr, &peerInfo.m_desc);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Peer info not found");
    }

    return peerInfo;
} // getPeerInfo


/**
 * @brief Get the connection information of a connected peer by connection ID.
 * @param [in] id The connection id of the peer.
 */
NimBLEConnInfo NimBLEServer::getPeerIDInfo(uint16_t id) {
    NimBLEConnInfo peerInfo;

    int rc = ble_gap_conn_find(id, &peerInfo.m_desc);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Peer info not found");
    }

    return peerInfo;
} // getPeerIDInfo

/**
 * @brief Callback that is called after reading from the peer name characteristic.
 * @details This will check the task pointer in the task data struct to determine
 * the action to take once the name has been read. If there is a task waiting then
 * it will be woken, if not, the the RC value is checked to determine which callback
 * should be called.
 */
int NimBLEServer::peerNameCB(uint16_t conn_handle,
                             const struct ble_gatt_error *error,
                             struct ble_gatt_attr *attr,
                             void *arg) {
    ble_task_data_t *pTaskData = (ble_task_data_t*)arg;
    std::string *name = (std::string*)pTaskData->buf;
    int rc = error->status;

    if (rc == 0) {
        if (attr) {
            name->append(OS_MBUF_DATA(attr->om, char*), OS_MBUF_PKTLEN(attr->om));
            return rc;
        }
    }

    if (rc == BLE_HS_EDONE) {
        // No ask means this was read for a callback.
        if (pTaskData->task == nullptr) {
            NimBLEServer* pServer = (NimBLEServer*)pTaskData->pATT;
            NimBLEConnInfo peerInfo{};
            ble_gap_conn_find(conn_handle, &peerInfo.m_desc);

            // Use the rc value as a flag to indicate which callback should be called.
            if (pTaskData->rc == NIMBLE_SERVER_GET_PEER_NAME_ON_CONNECT_CB) {
                pServer->m_pServerCallbacks->onConnect(pServer, peerInfo, *name);
            } else if (pTaskData->rc == NIMBLE_SERVER_GET_PEER_NAME_ON_AUTH_CB) {
                pServer->m_pServerCallbacks->onAuthenticationComplete(peerInfo, *name);
            }
        }
    } else {
        NIMBLE_LOGE(LOG_TAG, "NimBLEServerPeerNameCB rc=%d; %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    if (pTaskData->task != nullptr) {
        pTaskData->rc = rc;
        xTaskNotifyGive(pTaskData->task);
    } else {
        // If the read was triggered for callback use then these were allocated.
        delete name;
        delete pTaskData;
    }

    return rc;
}

/**
 * @brief Internal method that sends the read command.
 */
std::string NimBLEServer::getPeerNameInternal(uint16_t conn_handle, TaskHandle_t task, int cb_type) {
    std::string *buf = new std::string{};
    ble_task_data_t *taskData = new ble_task_data_t{this, task, cb_type, buf};
    ble_uuid16_t uuid {{BLE_UUID_TYPE_16}, BLE_SVC_GAP_CHR_UUID16_DEVICE_NAME};
    int rc = ble_gattc_read_by_uuid(conn_handle,
                                    1,
                                    0xffff,
                                    ((ble_uuid_t*)&uuid),
                                    NimBLEServer::peerNameCB,
                                    taskData);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_read_by_uuid rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        NimBLEConnInfo peerInfo{};
        ble_gap_conn_find(conn_handle, &peerInfo.m_desc);
        if (cb_type == NIMBLE_SERVER_GET_PEER_NAME_ON_CONNECT_CB) {
            m_pServerCallbacks->onConnect(this, peerInfo, *buf);
        } else if (cb_type == NIMBLE_SERVER_GET_PEER_NAME_ON_AUTH_CB) {
            m_pServerCallbacks->onAuthenticationComplete(peerInfo, *buf);
        }
        delete buf;
        delete taskData;
    } else if (task != nullptr) {
#ifdef ulTaskNotifyValueClear
        // Clear the task notification value to ensure we block
        ulTaskNotifyValueClear(task, ULONG_MAX);
#endif
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        rc = taskData->rc;
        std::string name{*(std::string*)taskData->buf};
        delete buf;
        delete taskData;

        if (rc != 0 && rc != BLE_HS_EDONE) {
            NIMBLE_LOGE(LOG_TAG, "getPeerName rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        }

        return name;
    }
    // TaskData and name buffer will be deleted in the callback.
    return "";
}

/**
 * @brief Get the name of the connected peer.
 * @param connInfo A reference to a NimBLEConnInfo instance to read the name from.
 * @returns A string containing the name.
 * @note This is a blocking call and should NOT be called from any callbacks!
 */
std::string NimBLEServer::getPeerName(const NimBLEConnInfo& connInfo) {
    std::string name = getPeerNameInternal(connInfo.getConnHandle(), xTaskGetCurrentTaskHandle());
    return name;
}

/**
 * @brief Handle a GATT Server Event.
 *
 * @param [in] event
 * @param [in] gatts_if
 * @param [in] param
 *
 */
/*STATIC*/
int NimBLEServer::handleGapEvent(struct ble_gap_event *event, void *arg) {
    NIMBLE_LOGD(LOG_TAG, ">> handleGapEvent: %s", NimBLEUtils::gapEventToString(event->type));

    int rc = 0;
    NimBLEConnInfo peerInfo;
    NimBLEServer* pServer = NimBLEDevice::getServer();

    switch(event->type) {

        case BLE_GAP_EVENT_CONNECT: {
            if (event->connect.status != 0) {
                /* Connection failed; resume advertising */
                NIMBLE_LOGE(LOG_TAG, "Connection failed");
#if !CONFIG_BT_NIMBLE_EXT_ADV
                NimBLEDevice::startAdvertising();
#endif
            } else {
                rc = ble_gap_conn_find(event->connect.conn_handle, &peerInfo.m_desc);
                if (rc != 0) {
                    return 0;
                }

                pServer->m_connectedPeersVec.push_back(event->connect.conn_handle);

                if (pServer->m_getPeerNameOnConnect) {
                    pServer->getPeerNameInternal(event->connect.conn_handle,
                                                 nullptr,
                                                 NIMBLE_SERVER_GET_PEER_NAME_ON_CONNECT_CB);
                } else {
                    pServer->m_pServerCallbacks->onConnect(pServer, peerInfo);
                }

            }

            return 0;
        } // BLE_GAP_EVENT_CONNECT


        case BLE_GAP_EVENT_DISCONNECT: {
            // If Host reset tell the device now before returning to prevent
            // any errors caused by calling host functions before resync.
            switch(event->disconnect.reason) {
                case BLE_HS_ETIMEOUT_HCI:
                case BLE_HS_EOS:
                case BLE_HS_ECONTROLLER:
                case BLE_HS_ENOTSYNCED:
                    NIMBLE_LOGE(LOG_TAG, "Disconnect - host reset, rc=%d", event->disconnect.reason);
                    NimBLEDevice::onReset(event->disconnect.reason);
                    break;
                default:
                    break;
            }

            pServer->m_connectedPeersVec.erase(std::remove(pServer->m_connectedPeersVec.begin(),
                                                          pServer->m_connectedPeersVec.end(),
                                                          event->disconnect.conn.conn_handle),
                                                          pServer->m_connectedPeersVec.end());

            if(pServer->m_svcChanged) {
                pServer->resetGATT();
            }

            NimBLEConnInfo peerInfo(event->disconnect.conn);
            pServer->m_pServerCallbacks->onDisconnect(pServer, peerInfo, event->disconnect.reason);

#if !CONFIG_BT_NIMBLE_EXT_ADV
            if(pServer->m_advertiseOnDisconnect) {
                pServer->startAdvertising();
            }
#endif
            return 0;
        } // BLE_GAP_EVENT_DISCONNECT

        case BLE_GAP_EVENT_SUBSCRIBE: {
            NIMBLE_LOGI(LOG_TAG, "subscribe event; attr_handle=%d, subscribed: %s",
                                 event->subscribe.attr_handle,
                                 (event->subscribe.cur_notify ? "true":"false"));

            for(auto &it : pServer->m_notifyChrVec) {
                if(it->getHandle() == event->subscribe.attr_handle) {
                    if((it->getProperties() & BLE_GATT_CHR_F_READ_AUTHEN) ||
                       (it->getProperties() & BLE_GATT_CHR_F_READ_AUTHOR) ||
                       (it->getProperties() & BLE_GATT_CHR_F_READ_ENC))
                    {
                        rc = ble_gap_conn_find(event->subscribe.conn_handle, &peerInfo.m_desc);
                        if (rc != 0) {
                            break;
                        }

                        if(!peerInfo.isEncrypted()) {
                            NimBLEDevice::startSecurity(event->subscribe.conn_handle);
                        }
                    }

                    it->setSubscribe(event);
                    break;
                }
            }

            return 0;
        } // BLE_GAP_EVENT_SUBSCRIBE

        case BLE_GAP_EVENT_MTU: {
            NIMBLE_LOGI(LOG_TAG, "mtu update event; conn_handle=%d mtu=%d",
                        event->mtu.conn_handle,
                        event->mtu.value);

            rc = ble_gap_conn_find(event->mtu.conn_handle, &peerInfo.m_desc);
            if (rc != 0) {
                return 0;
            }

            pServer->m_pServerCallbacks->onMTUChange(event->mtu.value, peerInfo);
            return 0;
        } // BLE_GAP_EVENT_MTU

        case BLE_GAP_EVENT_NOTIFY_TX: {
            NimBLECharacteristic *pChar = nullptr;

            for(auto &it : pServer->m_notifyChrVec) {
                if(it->getHandle() == event->notify_tx.attr_handle) {
                    pChar = it;
                }
            }

            if(pChar == nullptr) {
                return 0;
            }

            if(event->notify_tx.indication) {
                if(event->notify_tx.status == 0) {
                    return 0; // Indication sent but not yet acknowledged.
                }
                pServer->clearIndicateWait(event->notify_tx.conn_handle);
            }

            pChar->m_pCallbacks->onStatus(pChar, event->notify_tx.status);

            return 0;
        } // BLE_GAP_EVENT_NOTIFY_TX


        case BLE_GAP_EVENT_ADV_COMPLETE:
#if CONFIG_BT_NIMBLE_EXT_ADV
        case BLE_GAP_EVENT_SCAN_REQ_RCVD:
            return NimBLEExtAdvertising::handleGapEvent(event, arg);
#else
            return NimBLEAdvertising::handleGapEvent(event, arg);
#endif
        // BLE_GAP_EVENT_ADV_COMPLETE | BLE_GAP_EVENT_SCAN_REQ_RCVD

        case BLE_GAP_EVENT_CONN_UPDATE: {
            NIMBLE_LOGD(LOG_TAG, "Connection parameters updated.");
            return 0;
        } // BLE_GAP_EVENT_CONN_UPDATE

        case BLE_GAP_EVENT_REPEAT_PAIRING: {
            /* We already have a bond with the peer, but it is attempting to
             * establish a new secure link.  This app sacrifices security for
             * convenience: just throw away the old bond and accept the new link.
             */

            /* Delete the old bond. */
            rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &peerInfo.m_desc);
            if (rc != 0){
                return BLE_GAP_REPEAT_PAIRING_IGNORE;
            }

            ble_store_util_delete_peer(&peerInfo.m_desc.peer_id_addr);

            /* Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that the host should
             * continue with the pairing operation.
             */
            return BLE_GAP_REPEAT_PAIRING_RETRY;
        } // BLE_GAP_EVENT_REPEAT_PAIRING

        case BLE_GAP_EVENT_ENC_CHANGE: {
            rc = ble_gap_conn_find(event->enc_change.conn_handle, &peerInfo.m_desc);
            if(rc != 0) {
                return BLE_ATT_ERR_INVALID_HANDLE;
            }

            if (pServer->m_getPeerNameOnConnect) {
                pServer->getPeerNameInternal(event->enc_change.conn_handle,
                                             nullptr,
                                             NIMBLE_SERVER_GET_PEER_NAME_ON_AUTH_CB);
            } else {
                pServer->m_pServerCallbacks->onAuthenticationComplete(peerInfo);
            }
            return 0;
        } // BLE_GAP_EVENT_ENC_CHANGE

        case BLE_GAP_EVENT_IDENTITY_RESOLVED: {
            rc = ble_gap_conn_find(event->identity_resolved.conn_handle, &peerInfo.m_desc);
            if(rc != 0) {
                return BLE_ATT_ERR_INVALID_HANDLE;
            }

            pServer->m_pServerCallbacks->onIdentity(peerInfo);
            return 0;
        } // BLE_GAP_EVENT_IDENTITY_RESOLVED

        case BLE_GAP_EVENT_PASSKEY_ACTION: {
            struct ble_sm_io pkey = {0,0};

            if (event->passkey.params.action == BLE_SM_IOACT_DISP) {
                pkey.action = event->passkey.params.action;
                // backward compatibility
                pkey.passkey = NimBLEDevice::getSecurityPasskey(); // This is the passkey to be entered on peer
                // if the (static)passkey is the default, check the callback for custom value
                // both values default to the same.
                if(pkey.passkey == 123456) {
                    pkey.passkey = pServer->m_pServerCallbacks->onPassKeyDisplay();
                }
                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_DISP; ble_sm_inject_io result: %d", rc);

            } else if (event->passkey.params.action == BLE_SM_IOACT_NUMCMP) {
                NIMBLE_LOGD(LOG_TAG, "Passkey on device's display: %" PRIu32, event->passkey.params.numcmp);

                rc = ble_gap_conn_find(event->passkey.conn_handle, &peerInfo.m_desc);
                if(rc != 0) {
                    return BLE_ATT_ERR_INVALID_HANDLE;
                }

                pServer->m_pServerCallbacks->onConfirmPIN(peerInfo, event->passkey.params.numcmp);
            //TODO: Handle out of band pairing
            } else if (event->passkey.params.action == BLE_SM_IOACT_OOB) {
                static uint8_t tem_oob[16] = {0};
                pkey.action = event->passkey.params.action;
                for (int i = 0; i < 16; i++) {
                    pkey.oob[i] = tem_oob[i];
                }
                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_OOB; ble_sm_inject_io result: %d", rc);
            //////////////////////////////////
            } else if (event->passkey.params.action == BLE_SM_IOACT_NONE) {
                NIMBLE_LOGD(LOG_TAG, "No passkey action required");
            }

            NIMBLE_LOGD(LOG_TAG, "<< handleGATTServerEvent");
            return 0;
        } // BLE_GAP_EVENT_PASSKEY_ACTION

        default:
            break;
    }

    NIMBLE_LOGD(LOG_TAG, "<< handleGATTServerEvent");
    return 0;
} // handleGapEvent


/**
 * @brief Set the server callbacks.
 *
 * As a %BLE server operates, it will generate server level events such as a new client connecting or a previous client
 * disconnecting.  This function can be called to register a callback handler that will be invoked when these
 * events are detected.
 *
 * @param [in] pCallbacks The callbacks to be invoked.
 * @param [in] deleteCallbacks if true callback class will be deleted when server is destructed.
 */
void NimBLEServer::setCallbacks(NimBLEServerCallbacks* pCallbacks, bool deleteCallbacks) {
    if (pCallbacks != nullptr){
        m_pServerCallbacks = pCallbacks;
        m_deleteCallbacks = deleteCallbacks;
    } else {
        m_pServerCallbacks = &defaultCallbacks;
    }
} // setCallbacks


/**
 * @brief Remove a service from the server.
 *
 * @details Immediately removes access to the service by clients, sends a service changed indication,
 * and removes the service (if applicable) from the advertisements.
 * The service is not deleted unless the deleteSvc parameter is true, otherwise the service remains
 * available and can be re-added in the future. If desired a removed but not deleted service can
 * be deleted later by calling this method with deleteSvc set to true.
 *
 * @note The service will not be removed from the database until all open connections are closed
 * as it requires resetting the GATT server. In the interim the service will have it's visibility disabled.
 *
 * @note Advertising will need to be restarted by the user after calling this as we must stop
 * advertising in order to remove the service.
 *
 * @param [in] service The service object to remove.
 * @param [in] deleteSvc true if the service should be deleted.
 */
void NimBLEServer::removeService(NimBLEService* service, bool deleteSvc) {
    // Check if the service was already removed and if so check if this
    // is being called to delete the object and do so if requested.
    // Otherwise, ignore the call and return.
    if(service->m_removed > 0) {
        if(deleteSvc) {
            for(auto it = m_svcVec.begin(); it != m_svcVec.end(); ++it) {
                if ((*it) == service) {
                    delete *it;
                    m_svcVec.erase(it);
                    break;
                }
            }
        }

        return;
    }

    int rc = ble_gatts_svc_set_visibility(service->getHandle(), 0);
    if(rc !=0) {
        return;
    }

    service->m_removed = deleteSvc ? NIMBLE_ATT_REMOVE_DELETE : NIMBLE_ATT_REMOVE_HIDE;
    serviceChanged();
#if !CONFIG_BT_NIMBLE_EXT_ADV
    NimBLEDevice::getAdvertising()->removeServiceUUID(service->getUUID());
#endif
}


/**
 * @brief Adds a service which was either already created but removed from availability,\n
 * or created and later added to services list.
 * @param [in] service The service object to add.
 * @note If it is desired to advertise the service it must be added by
 * calling NimBLEAdvertising::addServiceUUID.
 */
void NimBLEServer::addService(NimBLEService* service) {
    // Check that a service with the supplied UUID does not already exist.
    if(getServiceByUUID(service->getUUID()) != nullptr) {
        NIMBLE_LOGW(LOG_TAG, "Warning creating a duplicate service UUID: %s",
                             std::string(service->getUUID()).c_str());
    }

    // If adding a service that was not removed add it and return.
    // Else reset GATT and send service changed notification.
    if(service->m_removed == 0) {
        m_svcVec.push_back(service);
        return;
    }

    service->m_removed = 0;
    serviceChanged();
}


/**
 * @brief Resets the GATT server, used when services are added/removed after initialization.
 */
void NimBLEServer::resetGATT() {
    if(getConnectedCount() > 0) {
        return;
    }

    NimBLEDevice::stopAdvertising();
    ble_gatts_reset();
    ble_svc_gap_init();
    ble_svc_gatt_init();

    for(auto it = m_svcVec.begin(); it != m_svcVec.end(); ) {
        if ((*it)->m_removed > 0) {
            if ((*it)->m_removed == NIMBLE_ATT_REMOVE_DELETE) {
                delete *it;
                it = m_svcVec.erase(it);
            } else {
                ++it;
            }
            continue;
        }

        (*it)->start();
        ++it;
    }

    m_svcChanged = false;
    m_gattsStarted = false;
}


#if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Start advertising.
 * @param [in] inst_id The extended advertisement instance ID to start.
 * @param [in] duration How long to advertise for in milliseconds, 0 = forever (default).
 * @param [in] max_events Maximum number of advertisement events to send, 0 = no limit (default).
 * @return True if advertising started successfully.
 * @details Start the server advertising its existence.  This is a convenience function and is equivalent to
 * retrieving the advertising object and invoking start upon it.
 */
bool NimBLEServer::startAdvertising(uint8_t inst_id,
                                    int duration,
                                    int max_events) {
    return getAdvertising()->start(inst_id, duration, max_events);
} // startAdvertising


/**
 * @brief Convenience function to stop advertising a data set.
 * @param [in] inst_id The extended advertisement instance ID to stop advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEServer::stopAdvertising(uint8_t inst_id) {
    return getAdvertising()->stop(inst_id);
} // stopAdvertising
#endif

#if !CONFIG_BT_NIMBLE_EXT_ADV || defined(_DOXYGEN_)
/**
 * @brief Start advertising.
 * @param [in] duration The duration in milliseconds to advertise for, default = forever.
 * @return True if advertising started successfully.
 * @details Start the server advertising its existence. This is a convenience function and is equivalent to
 * retrieving the advertising object and invoking start upon it.
 */
bool NimBLEServer::startAdvertising(uint32_t duration) {
    return getAdvertising()->start(duration);
} // startAdvertising
#endif


/**
 * @brief Stop advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEServer::stopAdvertising() {
    return getAdvertising()->stop();
} // stopAdvertising


/**
 * @brief Get the MTU of the client.
 * @returns The client MTU or 0 if not found/connected.
 */
uint16_t NimBLEServer::getPeerMTU(uint16_t conn_id) {
    return ble_att_mtu(conn_id);
} //getPeerMTU


/**
 * @brief Request an Update the connection parameters:
 * * Can only be used after a connection has been established.
 * @param [in] conn_handle The connection handle of the peer to send the request to.
 * @param [in] minInterval The minimum connection interval in 1.25ms units.
 * @param [in] maxInterval The maximum connection interval in 1.25ms units.
 * @param [in] latency The number of packets allowed to skip (extends max interval).
 * @param [in] timeout The timeout time in 10ms units before disconnecting.
 */
void NimBLEServer::updateConnParams(uint16_t conn_handle,
                            uint16_t minInterval, uint16_t maxInterval,
                            uint16_t latency, uint16_t timeout)
{
    ble_gap_upd_params params;

    params.latency  = latency;
    params.itvl_max = maxInterval;    // max_int = 0x20*1.25ms = 40ms
    params.itvl_min = minInterval;    // min_int = 0x10*1.25ms = 20ms
    params.supervision_timeout = timeout;    // timeout = 400*10ms = 4000ms
    params.min_ce_len = BLE_GAP_INITIAL_CONN_MIN_CE_LEN;  // Minimum length of connection event in 0.625ms units
    params.max_ce_len = BLE_GAP_INITIAL_CONN_MAX_CE_LEN;  // Maximum length of connection event in 0.625ms units

    int rc = ble_gap_update_params(conn_handle, &params);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Update params error: %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
    }
} // updateConnParams


/**
 * @brief Request an update of the data packet length.
 * * Can only be used after a connection has been established.
 * @details Sends a data length update request to the peer.
 * The Data Length Extension (DLE) allows to increase the Data Channel Payload from 27 bytes to up to 251 bytes.
 * The peer needs to support the Bluetooth 4.2 specifications, to be capable of DLE.
 * @param [in] conn_handle The connection handle of the peer to send the request to.
 * @param [in] tx_octets The preferred number of payload octets to use (Range 0x001B-0x00FB).
 */
void NimBLEServer::setDataLen(uint16_t conn_handle, uint16_t tx_octets) {
#if defined(CONFIG_NIMBLE_CPP_IDF) && !defined(ESP_IDF_VERSION) || \
  (ESP_IDF_VERSION_MAJOR * 100 + ESP_IDF_VERSION_MINOR * 10 + ESP_IDF_VERSION_PATCH) < 432
    return;
#else
    uint16_t tx_time = (tx_octets + 14) * 8;

    int rc = ble_gap_set_data_len(conn_handle, tx_octets, tx_time);
    if(rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Set data length error: %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
    }
#endif
} // setDataLen


bool NimBLEServer::setIndicateWait(uint16_t conn_handle) {
    for(auto i = 0; i < CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++) {
        if(m_indWait[i] == conn_handle) {
            return false;
        }
    }

    return true;
}


void NimBLEServer::clearIndicateWait(uint16_t conn_handle) {
    for(auto i = 0; i < CONFIG_BT_NIMBLE_MAX_CONNECTIONS; i++) {
        if(m_indWait[i] == conn_handle) {
            m_indWait[i] = BLE_HS_CONN_HANDLE_NONE;
            return;
        }
    }
}


/** Default callback handlers */
void NimBLEServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConnect(): Default");
} // onConnect

void NimBLEServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, std::string& name) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConnect(): Default");
} // onConnect

void NimBLEServerCallbacks::onDisconnect(NimBLEServer* pServer,
                                         NimBLEConnInfo& connInfo, int reason) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onDisconnect(): Default");
} // onDisconnect

void NimBLEServerCallbacks::onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onMTUChange(): Default");
} // onMTUChange

uint32_t NimBLEServerCallbacks::onPassKeyDisplay(){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onPassKeyDisplay: default: 123456");
    return 123456;
} //onPassKeyDisplay

void NimBLEServerCallbacks::onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pin){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConfirmPIN: default: true");
    NimBLEDevice::injectConfirmPIN(connInfo, true);
} // onConfirmPIN

void NimBLEServerCallbacks::onIdentity(const NimBLEConnInfo& connInfo){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onIdentity: default");
} // onIdentity

void NimBLEServerCallbacks::onAuthenticationComplete(const NimBLEConnInfo& connInfo){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onAuthenticationComplete: default");
} // onAuthenticationComplete

void NimBLEServerCallbacks::onAuthenticationComplete(const NimBLEConnInfo& connInfo, const std::string& name){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onAuthenticationComplete: default");
} // onAuthenticationComplete

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
