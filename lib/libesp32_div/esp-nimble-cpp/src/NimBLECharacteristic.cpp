/*
 * NimBLECharacteristic.cpp
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * BLECharacteristic.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLECharacteristic.h"
#include "NimBLE2904.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#define NULL_HANDLE (0xffff)
#define NIMBLE_SUB_NOTIFY   0x0001
#define NIMBLE_SUB_INDICATE 0x0002

static NimBLECharacteristicCallbacks defaultCallback;
static const char* LOG_TAG = "NimBLECharacteristic";


/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID (const char*) for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 * @param [in] max_len - The maximum length in bytes that the characteristic value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pService - pointer to the service instance this characteristic belongs to.
 */
NimBLECharacteristic::NimBLECharacteristic(const char* uuid, uint16_t properties,
                                           uint16_t max_len, NimBLEService* pService)
: NimBLECharacteristic(NimBLEUUID(uuid), properties, max_len, pService) {
}

/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 * @param [in] max_len - The maximum length in bytes that the characteristic value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pService - pointer to the service instance this characteristic belongs to.
 */
NimBLECharacteristic::NimBLECharacteristic(const NimBLEUUID &uuid, uint16_t properties,
                                           uint16_t max_len, NimBLEService* pService)
:   m_value(std::min(CONFIG_NIMBLE_CPP_ATT_VALUE_INIT_LENGTH , (int)max_len), max_len) {
    m_uuid        = uuid;
    m_handle      = NULL_HANDLE;
    m_properties  = properties;
    m_pCallbacks  = &defaultCallback;
    m_pService    = pService;
    m_removed     = 0;
} // NimBLECharacteristic

/**
 * @brief Destructor.
 */
NimBLECharacteristic::~NimBLECharacteristic() {
    for(auto &it : m_dscVec) {
        delete it;
    }
} // ~NimBLECharacteristic


/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @param [in] max_len - The max length in bytes of the descriptor value.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(const char* uuid, uint32_t properties, uint16_t max_len) {
    return createDescriptor(NimBLEUUID(uuid), properties, max_len);
}


/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @param [in] max_len - The max length in bytes of the descriptor value.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(const NimBLEUUID &uuid, uint32_t properties, uint16_t max_len) {
    NimBLEDescriptor* pDescriptor = nullptr;
    if (uuid == NimBLEUUID(uint16_t(0x2904))) {
        pDescriptor = new NimBLE2904(this);
    } else {
        pDescriptor = new NimBLEDescriptor(uuid, properties, max_len, this);
    }

    addDescriptor(pDescriptor);
    return pDescriptor;
} // createDescriptor


/**
 * @brief Add a descriptor to the characteristic.
 * @param [in] pDescriptor A pointer to the descriptor to add.
 */
void NimBLECharacteristic::addDescriptor(NimBLEDescriptor *pDescriptor) {
    bool foundRemoved = false;

    if(pDescriptor->m_removed > 0) {
        for(auto& it : m_dscVec) {
            if(it == pDescriptor) {
                foundRemoved = true;
                pDescriptor->m_removed = 0;
            }
        }
    }

    if(!foundRemoved) {
        m_dscVec.push_back(pDescriptor);
    }

    pDescriptor->setCharacteristic(this);
    NimBLEDevice::getServer()->serviceChanged();
}


/**
 * @brief Remove a descriptor from the characteristic.
 * @param[in] pDescriptor A pointer to the descriptor instance to remove from the characteristic.
 * @param[in] deleteDsc If true it will delete the descriptor instance and free it's resources.
 */
void NimBLECharacteristic::removeDescriptor(NimBLEDescriptor *pDescriptor, bool deleteDsc) {
    // Check if the descriptor was already removed and if so, check if this
    // is being called to delete the object and do so if requested.
    // Otherwise, ignore the call and return.
    if(pDescriptor->m_removed > 0) {
        if(deleteDsc) {
            for(auto it = m_dscVec.begin(); it != m_dscVec.end(); ++it) {
                if ((*it) == pDescriptor) {
                    delete *it;
                    m_dscVec.erase(it);
                    break;
                }
            }
        }

        return;
    }

    pDescriptor->m_removed = deleteDsc ? NIMBLE_ATT_REMOVE_DELETE : NIMBLE_ATT_REMOVE_HIDE;
    NimBLEDevice::getServer()->serviceChanged();
} // removeDescriptor


/**
 * @brief Return the BLE Descriptor for the given UUID.
 * @param [in] uuid The UUID of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(const char* uuid) {
    return getDescriptorByUUID(NimBLEUUID(uuid));
} // getDescriptorByUUID


/**
 * @brief Return the BLE Descriptor for the given UUID.
 * @param [in] uuid The UUID of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(const NimBLEUUID &uuid) {
    for (auto &it : m_dscVec) {
        if (it->getUUID() == uuid) {
            return it;
        }
    }
    return nullptr;
} // getDescriptorByUUID

/**
 * @brief Return the BLE Descriptor for the given handle.
 * @param [in] handle The handle of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor *NimBLECharacteristic::getDescriptorByHandle(uint16_t handle) {
    for (auto &it : m_dscVec) {
        if (it->getHandle() == handle) {
            return it;
        }
    }
    return nullptr;
}


/**
 * @brief Get the handle of the characteristic.
 * @return The handle of the characteristic.
 */
uint16_t NimBLECharacteristic::getHandle() {
    return m_handle;
} // getHandle


/**
 * @brief Get the properties of the characteristic.
 * @return The properties of the characteristic.
 */
uint16_t NimBLECharacteristic::getProperties() {
    return m_properties;
} // getProperties


/**
 * @brief Get the service associated with this characteristic.
 */
NimBLEService* NimBLECharacteristic::getService() {
    return m_pService;
} // getService


void NimBLECharacteristic::setService(NimBLEService *pService) {
    m_pService = pService;
}


/**
 * @brief Get the UUID of the characteristic.
 * @return The UUID of the characteristic.
 */
NimBLEUUID NimBLECharacteristic::getUUID() {
    return m_uuid;
} // getUUID


/**
 * @brief Retrieve the current value of the characteristic.
 * @return The NimBLEAttValue containing the current characteristic value.
 */
NimBLEAttValue NimBLECharacteristic::getValue(time_t *timestamp) {
    if(timestamp != nullptr) {
        m_value.getValue(timestamp);
    }

    return m_value;
} // getValue


/**
 * @brief Retrieve the the current data length of the characteristic.
 * @return The length of the current characteristic data.
 */
size_t NimBLECharacteristic::getDataLength() {
    return m_value.size();
}


/**
 * @brief STATIC callback to handle events from the NimBLE stack.
 */
int NimBLECharacteristic::handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt,
                             void *arg)
{
    if (conn_handle > BLE_HCI_LE_CONN_HANDLE_MAX)
    {
        NIMBLE_LOGW(LOG_TAG, "Conn_handle (%d) is above the maximum value (%d)", conn_handle, BLE_HCI_LE_CONN_HANDLE_MAX);
        return BLE_ATT_ERR_INVALID_HANDLE;
    }

    const ble_uuid_t *uuid;
    int rc;
    NimBLEConnInfo peerInfo;
    NimBLECharacteristic* pCharacteristic = (NimBLECharacteristic*)arg;

    NIMBLE_LOGD(LOG_TAG, "Characteristic %s %s event", pCharacteristic->getUUID().toString().c_str(),
                                    ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR ? "Read" : "Write");

    uuid = ctxt->chr->uuid;
    if(ble_uuid_cmp(uuid, &pCharacteristic->getUUID().getNative()->u) == 0){
        switch(ctxt->op) {
            case BLE_GATT_ACCESS_OP_READ_CHR: {
                ble_gap_conn_find(conn_handle, &peerInfo.m_desc);

                 // If the packet header is only 8 bytes this is a follow up of a long read
                 // so we don't want to call the onRead() callback again.
                if(ctxt->om->om_pkthdr_len > 8 ||
                   conn_handle == BLE_HS_CONN_HANDLE_NONE ||
                   pCharacteristic->m_value.size() <= (ble_att_mtu(peerInfo.m_desc.conn_handle) - 3)) {
                    pCharacteristic->m_pCallbacks->onRead(pCharacteristic, peerInfo);
                }

                ble_npl_hw_enter_critical();
                rc = os_mbuf_append(ctxt->om, pCharacteristic->m_value.data(), pCharacteristic->m_value.size());
                ble_npl_hw_exit_critical(0);
                return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }

            case BLE_GATT_ACCESS_OP_WRITE_CHR: {
                uint16_t att_max_len = pCharacteristic->m_value.max_size();

                if (ctxt->om->om_len > att_max_len) {
                    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                }

                uint8_t buf[att_max_len];
                size_t len = ctxt->om->om_len;
                memcpy(buf, ctxt->om->om_data,len);

                os_mbuf *next;
                next = SLIST_NEXT(ctxt->om, om_next);
                while(next != NULL){
                    if((len + next->om_len) > att_max_len) {
                        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                    }
                    memcpy(&buf[len], next->om_data, next->om_len);
                    len += next->om_len;
                    next = SLIST_NEXT(next, om_next);
                }

                ble_gap_conn_find(conn_handle, &peerInfo.m_desc);
                pCharacteristic->setValue(buf, len);
                pCharacteristic->m_pCallbacks->onWrite(pCharacteristic, peerInfo);
                return 0;
            }
            default:
                break;
        }
    }

    return BLE_ATT_ERR_UNLIKELY;
}


/**
 * @brief Get the number of clients subscribed to the characteristic.
 * @returns Number of clients subscribed to notifications / indications.
 */
size_t NimBLECharacteristic::getSubscribedCount() {
    return m_subscribedVec.size();
}


/**
 * @brief Set the subscribe status for this characteristic.\n
 * This will maintain a vector of subscribed clients and their indicate/notify status.
 */
void NimBLECharacteristic::setSubscribe(struct ble_gap_event *event) {
    NimBLEConnInfo peerInfo;
    if(ble_gap_conn_find(event->subscribe.conn_handle, &peerInfo.m_desc) != 0) {
        return;
    }

    uint16_t subVal = 0;
    if(event->subscribe.cur_notify > 0 && (m_properties & NIMBLE_PROPERTY::NOTIFY)) {
        subVal |= NIMBLE_SUB_NOTIFY;
    }
    if(event->subscribe.cur_indicate && (m_properties & NIMBLE_PROPERTY::INDICATE)) {
        subVal |= NIMBLE_SUB_INDICATE;
    }

    NIMBLE_LOGI(LOG_TAG, "New subscribe value for conn: %d val: %d",
                          event->subscribe.conn_handle, subVal);

    if(!event->subscribe.cur_indicate && event->subscribe.prev_indicate) {
       NimBLEDevice::getServer()->clearIndicateWait(event->subscribe.conn_handle);
    }


    auto it = m_subscribedVec.begin();
    for(;it != m_subscribedVec.end(); ++it) {
        if((*it).first == event->subscribe.conn_handle) {
            break;
        }
    }

    if(subVal > 0) {
        if(it == m_subscribedVec.end()) {
            m_subscribedVec.push_back({event->subscribe.conn_handle, subVal});
        } else {
            (*it).second = subVal;
        }
    } else if(it != m_subscribedVec.end()) {
        m_subscribedVec.erase(it);
    }

    m_pCallbacks->onSubscribe(this, peerInfo, subVal);
}


/**
 * @brief Send an indication.
 */
void NimBLECharacteristic::indicate() {
    notify(false);
} // indicate


/**
 * @brief Send an indication.
 * @param[in] value A pointer to the data to send.
 * @param[in] length The length of the data to send.
 */
void NimBLECharacteristic::indicate(const uint8_t* value, size_t length) {
    notify(value, length, false);
} // indicate


/**
 * @brief Send an indication.
 * @param[in] value A std::vector<uint8_t> containing the value to send as the notification value.
 */
void NimBLECharacteristic::indicate(const std::vector<uint8_t>& value) {
    notify(value.data(), value.size(), false);
} // indicate


/**
 * @brief Send a notification or indication.
 * @param[in] is_notification if true sends a notification, false sends an indication.
 * @param[in] conn_handle Connection handle to send individual notification, or BLE_HCI_LE_CONN_HANDLE_MAX + 1 to send notification to all subscribed clients.
 */
void NimBLECharacteristic::notify(bool is_notification, uint16_t conn_handle) {
    notify(m_value.data(), m_value.length(), is_notification, conn_handle);
} // notify


/**
 * @brief Send a notification or indication.
 * @param[in] value A std::vector<uint8_t> containing the value to send as the notification value.
 * @param[in] is_notification if true sends a notification, false sends an indication.
 * @param[in] conn_handle Connection handle to send individual notification, or BLE_HCI_LE_CONN_HANDLE_MAX + 1 to send notification to all subscribed clients.
 */
void NimBLECharacteristic::notify(const std::vector<uint8_t>& value, bool is_notification, uint16_t conn_handle) {
    notify(value.data(), value.size(), is_notification, conn_handle);
} // notify


/**
 * @brief Send a notification or indication.
 * @param[in] value A pointer to the data to send.
 * @param[in] length The length of the data to send.
 * @param[in] is_notification if true sends a notification, false sends an indication.
 * @param[in] conn_handle Connection handle to send individual notification, or BLE_HCI_LE_CONN_HANDLE_MAX + 1 to send notification to all subscribed clients.
 */
void NimBLECharacteristic::notify(const uint8_t* value, size_t length, bool is_notification, uint16_t conn_handle) {
    NIMBLE_LOGD(LOG_TAG, ">> notify: length: %d", length);

    if(!(m_properties & NIMBLE_PROPERTY::NOTIFY) &&
       !(m_properties & NIMBLE_PROPERTY::INDICATE))
    {
        NIMBLE_LOGE(LOG_TAG,
                    "<< notify-Error; Notify/indicate not enabled for characteristic: %s",
                    std::string(getUUID()).c_str());
    }

    if (m_subscribedVec.size() == 0) {
        NIMBLE_LOGD(LOG_TAG, "<< notify: No clients subscribed.");
        return;
    }

    m_pCallbacks->onNotify(this);

    bool reqSec = (m_properties & BLE_GATT_CHR_F_READ_AUTHEN) ||
                  (m_properties & BLE_GATT_CHR_F_READ_AUTHOR) ||
                  (m_properties & BLE_GATT_CHR_F_READ_ENC);
    int rc = 0;

    for (auto &it : m_subscribedVec) {
        // check if need a specific client
        if ((conn_handle <= BLE_HCI_LE_CONN_HANDLE_MAX) && (it.first != conn_handle)) {
            continue;
        }

        uint16_t _mtu = getService()->getServer()->getPeerMTU(it.first) - 3;

        // check if connected and subscribed
        if(_mtu == 0 || it.second == 0) {
            continue;
        }

        // check if security requirements are satisfied
        if(reqSec) {
            struct ble_gap_conn_desc desc;
            rc = ble_gap_conn_find(it.first, &desc);
            if(rc != 0 || !desc.sec_state.encrypted) {
                continue;
            }
        }

        if (length > _mtu) {
            NIMBLE_LOGW(LOG_TAG, "- Truncating to %d bytes (maximum notify size)", _mtu);
        }

        if(is_notification && (!(it.second & NIMBLE_SUB_NOTIFY))) {
            NIMBLE_LOGW(LOG_TAG,
            "Sending notification to client subscribed to indications, sending indication instead");
            is_notification = false;
        }

        if(!is_notification && (!(it.second & NIMBLE_SUB_INDICATE))) {
            NIMBLE_LOGW(LOG_TAG,
            "Sending indication to client subscribed to notification, sending notification instead");
            is_notification = true;
        }

        // don't create the m_buf until we are sure to send the data or else
        // we could be allocating a buffer that doesn't get released.
        // We also must create it in each loop iteration because it is consumed with each host call.
        os_mbuf *om = ble_hs_mbuf_from_flat(value, length);

        if(!is_notification && (m_properties & NIMBLE_PROPERTY::INDICATE)) {
            if(!NimBLEDevice::getServer()->setIndicateWait(it.first)) {
               NIMBLE_LOGE(LOG_TAG, "prior Indication in progress");
               os_mbuf_free_chain(om);
               return;
            }

            rc = ble_gattc_indicate_custom(it.first, m_handle, om);
            if(rc != 0){
                NimBLEDevice::getServer()->clearIndicateWait(it.first);
            }
        } else {
            ble_gattc_notify_custom(it.first, m_handle, om);
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< notify");
} // Notify


/**
 * @brief Set the callback handlers for this characteristic.
 * @param [in] pCallbacks An instance of a NimBLECharacteristicCallbacks class\n
 * used to define any callbacks for the characteristic.
 */
void NimBLECharacteristic::setCallbacks(NimBLECharacteristicCallbacks* pCallbacks) {
    if (pCallbacks != nullptr){
        m_pCallbacks = pCallbacks;
    } else {
        m_pCallbacks = &defaultCallback;
    }
} // setCallbacks


/**
 * @brief Get the callback handlers for this characteristic.
 */
NimBLECharacteristicCallbacks* NimBLECharacteristic::getCallbacks() {
    return m_pCallbacks;
} //getCallbacks


/**
 * @brief Set the value of the characteristic from a data buffer .
 * @param [in] data The data buffer to set for the characteristic.
 * @param [in] length The number of bytes in the data buffer.
 */
void NimBLECharacteristic::setValue(const uint8_t* data, size_t length) {
#if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 4
    char* pHex = NimBLEUtils::buildHexData(nullptr, data, length);
    NIMBLE_LOGD(LOG_TAG, ">> setValue: length=%d, data=%s, characteristic UUID=%s",
                length, pHex, getUUID().toString().c_str());
    free(pHex);
#endif

    m_value.setValue(data, length);
    NIMBLE_LOGD(LOG_TAG, "<< setValue");
} // setValue


/**
 * @brief Set the value of the characteristic from a `std::vector<uint8_t>`.\n
 * @param [in] vec The std::vector<uint8_t> reference to set the characteristic value from.
 */
void NimBLECharacteristic::setValue(const std::vector<uint8_t>& vec) {
    return setValue((uint8_t*)&vec[0], vec.size());
}// setValue


/**
 * @brief Return a string representation of the characteristic.
 * @return A string representation of the characteristic.
 */
std::string NimBLECharacteristic::toString() {
    std::string res = "UUID: " + m_uuid.toString() + ", handle : 0x";
    char hex[5];
    snprintf(hex, sizeof(hex), "%04x", m_handle);
    res += hex;
    res += " ";
    if (m_properties & BLE_GATT_CHR_PROP_READ ) res += "Read ";
    if (m_properties & BLE_GATT_CHR_PROP_WRITE) res += "Write ";
    if (m_properties & BLE_GATT_CHR_PROP_WRITE_NO_RSP) res += "WriteNoResponse ";
    if (m_properties & BLE_GATT_CHR_PROP_BROADCAST) res += "Broadcast ";
    if (m_properties & BLE_GATT_CHR_PROP_NOTIFY) res += "Notify ";
    if (m_properties & BLE_GATT_CHR_PROP_INDICATE) res += "Indicate ";
    return res;
} // toString


/**
 * @brief Callback function to support a read request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLECharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onRead: default");
} // onRead


/**
 * @brief Callback function to support a write request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLECharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onWrite: default");
} // onWrite


/**
 * @brief Callback function to support a Notify request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void NimBLECharacteristicCallbacks::onNotify(NimBLECharacteristic* pCharacteristic) {
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onNotify: default");
} // onNotify


/**
 * @brief Callback function to support a Notify/Indicate Status report.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] code Status return code from the NimBLE stack.
 * @details The status code for success is 0 for notifications and BLE_HS_EDONE for indications,
 * any other value is an error.
 */
void NimBLECharacteristicCallbacks::onStatus(NimBLECharacteristic* pCharacteristic, int code) {
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onStatus: default");
} // onStatus


/**
 * @brief Callback function called when a client changes subscription status.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 * @param [in] subValue The subscription status:
 * * 0 = Un-Subscribed
 * * 1 = Notifications
 * * 2 = Indications
 * * 3 = Notifications and Indications
 */
void NimBLECharacteristicCallbacks::onSubscribe(NimBLECharacteristic* pCharacteristic,
                                                NimBLEConnInfo& connInfo,
                                                uint16_t subValue)
{
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onSubscribe: default");
}

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
