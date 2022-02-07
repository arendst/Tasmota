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

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEServer.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"


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
    m_advertiseOnDisconnect = true;
    m_svcChanged            = false;
    m_deleteCallbacks       = true;
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
 * @param [in] numHandles The maximum number of handles associated with this service.
 * @param [in] inst_id if we have multiple services with the same UUID we need
 *             to provide inst_id value different for each service.
 * @return A reference to the new service object.
 */
NimBLEService* NimBLEServer::createService(const NimBLEUUID &uuid, uint32_t numHandles, uint8_t inst_id) {
    NIMBLE_LOGD(LOG_TAG, ">> createService - %s", uuid.toString().c_str());
    // TODO: add functionality to use inst_id for multiple services with same uuid
    (void)inst_id;
    // Check that a service with the supplied UUID does not already exist.
    if(getServiceByUUID(uuid) != nullptr) {
        NIMBLE_LOGW(LOG_TAG, "Warning creating a duplicate service UUID: %s",
                             std::string(uuid).c_str());
    }

    NimBLEService* pService = new NimBLEService(uuid, numHandles, this);
    m_svcVec.push_back(pService); // Save a reference to this service being on this server.
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

/**
 * @brief Retrieve the advertising object that can be used to advertise the existence of the server.
 *
 * @return An advertising object.
 */
NimBLEAdvertising* NimBLEServer::getAdvertising() {
    return NimBLEDevice::getAdvertising();
} // getAdvertising


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
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_start; rc=%d, %s", rc,
                            NimBLEUtils::returnCodeToString(rc));
        abort();
    }

#if CONFIG_NIMBLE_CPP_DEBUG_LEVEL >= 4
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
                abort();
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
 * @brief Set the server to automatically start advertising when a client disconnects.
 * @param [in] aod true == advertise, false == don't advertise.
 */
void NimBLEServer::advertiseOnDisconnect(bool aod) {
    m_advertiseOnDisconnect = aod;
} // advertiseOnDisconnect


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
 * @brief Handle a GATT Server Event.
 *
 * @param [in] event
 * @param [in] gatts_if
 * @param [in] param
 *
 */
/*STATIC*/int NimBLEServer::handleGapEvent(struct ble_gap_event *event, void *arg) {
    NimBLEServer* server = (NimBLEServer*)arg;
    NIMBLE_LOGD(LOG_TAG, ">> handleGapEvent: %s",
                         NimBLEUtils::gapEventToString(event->type));
    int rc = 0;
    struct ble_gap_conn_desc desc;

    switch(event->type) {

        case BLE_GAP_EVENT_CONNECT: {
            if (event->connect.status != 0) {
                /* Connection failed; resume advertising */
                NIMBLE_LOGE(LOG_TAG, "Connection failed");
                NimBLEDevice::startAdvertising();
            }
            else {
                server->m_connectedPeersVec.push_back(event->connect.conn_handle);

                rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
                if (rc != 0) {
                    return 0;
                }

                server->m_pServerCallbacks->onConnect(server);
                server->m_pServerCallbacks->onConnect(server, &desc);
            }

            return 0;
        } // BLE_GAP_EVENT_CONNECT


        case BLE_GAP_EVENT_DISCONNECT: {
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

            server->m_connectedPeersVec.erase(std::remove(server->m_connectedPeersVec.begin(),
                                                          server->m_connectedPeersVec.end(),
                                                          event->disconnect.conn.conn_handle),
                                                          server->m_connectedPeersVec.end());

            if(server->m_svcChanged) {
                server->resetGATT();
            }

            server->m_pServerCallbacks->onDisconnect(server);
            server->m_pServerCallbacks->onDisconnect(server, &event->disconnect.conn);

            if(server->m_advertiseOnDisconnect) {
                server->startAdvertising();
            }
            return 0;
        } // BLE_GAP_EVENT_DISCONNECT

        case BLE_GAP_EVENT_SUBSCRIBE: {
            NIMBLE_LOGI(LOG_TAG, "subscribe event; attr_handle=%d, subscribed: %s",
                                 event->subscribe.attr_handle,
                                 (event->subscribe.cur_notify ? "true":"false"));

            for(auto &it : server->m_notifyChrVec) {
                if(it->getHandle() == event->subscribe.attr_handle) {
                    if((it->getProperties() & BLE_GATT_CHR_F_READ_AUTHEN) ||
                       (it->getProperties() & BLE_GATT_CHR_F_READ_AUTHOR) ||
                       (it->getProperties() & BLE_GATT_CHR_F_READ_ENC))
                    {
                        rc = ble_gap_conn_find(event->subscribe.conn_handle, &desc);
                        if (rc != 0) {
                            break;
                        }

                        if(!desc.sec_state.encrypted) {
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
            rc = ble_gap_conn_find(event->mtu.conn_handle, &desc);
            if (rc != 0) {
                return 0;
            }

            server->m_pServerCallbacks->onMTUChange(event->mtu.value, &desc);
            return 0;
        } // BLE_GAP_EVENT_MTU

        case BLE_GAP_EVENT_NOTIFY_TX: {
            NimBLECharacteristic *pChar = nullptr;

            for(auto &it : server->m_notifyChrVec) {
                if(it->getHandle() == event->notify_tx.attr_handle) {
                    pChar = it;
                }
            }

            if(pChar == nullptr) {
                return 0;
            }

            NimBLECharacteristicCallbacks::Status statusRC;

            if(event->notify_tx.indication) {
                if(event->notify_tx.status != 0) {
                    if(event->notify_tx.status == BLE_HS_EDONE) {
                        statusRC = NimBLECharacteristicCallbacks::Status::SUCCESS_INDICATE;
                    } else if(rc == BLE_HS_ETIMEOUT) {
                        statusRC = NimBLECharacteristicCallbacks::Status::ERROR_INDICATE_TIMEOUT;
                    } else {
                        statusRC = NimBLECharacteristicCallbacks::Status::ERROR_INDICATE_FAILURE;
                    }
                } else {
                    return 0;
                }

                server->clearIndicateWait(event->notify_tx.conn_handle);
            } else {
                if(event->notify_tx.status == 0) {
                    statusRC = NimBLECharacteristicCallbacks::Status::SUCCESS_NOTIFY;
                } else {
                    statusRC = NimBLECharacteristicCallbacks::Status::ERROR_GATT;
                }
            }

            pChar->m_pCallbacks->onStatus(pChar, statusRC, event->notify_tx.status);

            return 0;
        } // BLE_GAP_EVENT_NOTIFY_TX

        case BLE_GAP_EVENT_ADV_COMPLETE: {
            NIMBLE_LOGD(LOG_TAG, "Advertising Complete");
            NimBLEDevice::getAdvertising()->advCompleteCB();
            return 0;
        }

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
            rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
            if (rc != 0){
                return BLE_GAP_REPEAT_PAIRING_IGNORE;
            }

            ble_store_util_delete_peer(&desc.peer_id_addr);

            /* Return BLE_GAP_REPEAT_PAIRING_RETRY to indicate that the host should
             * continue with the pairing operation.
             */
            return BLE_GAP_REPEAT_PAIRING_RETRY;
        } // BLE_GAP_EVENT_REPEAT_PAIRING

        case BLE_GAP_EVENT_ENC_CHANGE: {
            rc = ble_gap_conn_find(event->enc_change.conn_handle, &desc);
            if(rc != 0) {
                return BLE_ATT_ERR_INVALID_HANDLE;
            }
            // Compatibility only - Do not use, should be removed the in future
            if(NimBLEDevice::m_securityCallbacks != nullptr) {
                NimBLEDevice::m_securityCallbacks->onAuthenticationComplete(&desc);
            /////////////////////////////////////////////
            } else {
                server->m_pServerCallbacks->onAuthenticationComplete(&desc);
            }

            return 0;
        } // BLE_GAP_EVENT_ENC_CHANGE

        case BLE_GAP_EVENT_PASSKEY_ACTION: {
            struct ble_sm_io pkey = {0,0};

            if (event->passkey.params.action == BLE_SM_IOACT_DISP) {
                pkey.action = event->passkey.params.action;
                // backward compatibility
                pkey.passkey = NimBLEDevice::getSecurityPasskey(); // This is the passkey to be entered on peer
                // if the (static)passkey is the default, check the callback for custom value
                // both values default to the same.
                if(pkey.passkey == 123456) {
                    pkey.passkey = server->m_pServerCallbacks->onPassKeyRequest();
                }
                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_DISP; ble_sm_inject_io result: %d", rc);

            } else if (event->passkey.params.action == BLE_SM_IOACT_NUMCMP) {
                NIMBLE_LOGD(LOG_TAG, "Passkey on device's display: %d", event->passkey.params.numcmp);
                pkey.action = event->passkey.params.action;
                // Compatibility only - Do not use, should be removed the in future
                if(NimBLEDevice::m_securityCallbacks != nullptr) {
                    pkey.numcmp_accept = NimBLEDevice::m_securityCallbacks->onConfirmPIN(event->passkey.params.numcmp);
                /////////////////////////////////////////////
                } else {
                    pkey.numcmp_accept = server->m_pServerCallbacks->onConfirmPIN(event->passkey.params.numcmp);
                }

                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_NUMCMP; ble_sm_inject_io result: %d", rc);

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
            } else if (event->passkey.params.action == BLE_SM_IOACT_INPUT) {
                NIMBLE_LOGD(LOG_TAG, "Enter the passkey");
                pkey.action = event->passkey.params.action;

                // Compatibility only - Do not use, should be removed the in future
                if(NimBLEDevice::m_securityCallbacks != nullptr) {
                    pkey.passkey = NimBLEDevice::m_securityCallbacks->onPassKeyRequest();
                /////////////////////////////////////////////
                } else {
                    pkey.passkey = server->m_pServerCallbacks->onPassKeyRequest();
                }

                rc = ble_sm_inject_io(event->passkey.conn_handle, &pkey);
                NIMBLE_LOGD(LOG_TAG, "BLE_SM_IOACT_INPUT; ble_sm_inject_io result: %d", rc);

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
 * and removes the service (if applicable) from the advertisments.
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
    NimBLEDevice::getAdvertising()->removeServiceUUID(service->getUUID());
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


/**
 * @brief Start advertising.
 *
 * Start the server advertising its existence.  This is a convenience function and is equivalent to
 * retrieving the advertising object and invoking start upon it.
 */
void NimBLEServer::startAdvertising() {
    NimBLEDevice::startAdvertising();
} // startAdvertising


/**
 * @brief Stop advertising.
 */
void NimBLEServer::stopAdvertising() {
    NimBLEDevice::stopAdvertising();
} // startAdvertising


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
}// updateConnParams


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

void NimBLEServerCallbacks::onConnect(NimBLEServer* pServer) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConnect(): Default");
} // onConnect


void NimBLEServerCallbacks::onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConnect(): Default");
} // onConnect


void NimBLEServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onDisconnect(): Default");
} // onDisconnect

void NimBLEServerCallbacks::onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onDisconnect(): Default");
} // onDisconnect

void NimBLEServerCallbacks::onMTUChange(uint16_t MTU, ble_gap_conn_desc* desc) {
    NIMBLE_LOGD("NimBLEServerCallbacks", "onMTUChange(): Default");
} // onMTUChange

uint32_t NimBLEServerCallbacks::onPassKeyRequest(){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onPassKeyRequest: default: 123456");
    return 123456;
}
/*
void NimBLEServerCallbacks::onPassKeyNotify(uint32_t pass_key){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onPassKeyNotify: default: %d", pass_key);
}

bool NimBLEServerCallbacks::onSecurityRequest(){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onSecurityRequest: default: true");
    return true;
}
*/
void NimBLEServerCallbacks::onAuthenticationComplete(ble_gap_conn_desc*){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onAuthenticationComplete: default");
}
bool NimBLEServerCallbacks::onConfirmPIN(uint32_t pin){
    NIMBLE_LOGD("NimBLEServerCallbacks", "onConfirmPIN: default: true");
    return true;
}


#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif // CONFIG_BT_ENABLED
