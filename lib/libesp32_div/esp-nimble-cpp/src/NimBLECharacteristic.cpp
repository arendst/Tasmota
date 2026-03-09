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

#include "NimBLECharacteristic.h"
#if CONFIG_BT_NIMBLE_ENABLED && MYNEWT_VAL(BLE_ROLE_PERIPHERAL)

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  if !defined(ESP_IDF_VERSION_MAJOR) || ESP_IDF_VERSION_MAJOR < 5
#   define ble_gatts_notify_custom   ble_gattc_notify_custom
#   define ble_gatts_indicate_custom ble_gattc_indicate_custom
#  endif
# endif

# include "NimBLE2904.h"
# include "NimBLEDevice.h"
# include "NimBLELog.h"

static NimBLECharacteristicCallbacks defaultCallback;
static const char*                   LOG_TAG = "NimBLECharacteristic";

/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID (const char*) for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 * @param [in] maxLen - The maximum length in bytes that the characteristic value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pService - pointer to the service instance this characteristic belongs to.
 */
NimBLECharacteristic::NimBLECharacteristic(const char* uuid, uint16_t properties, uint16_t maxLen, NimBLEService* pService)
    : NimBLECharacteristic(NimBLEUUID(uuid), properties, maxLen, pService) {}

/**
 * @brief Construct a characteristic
 * @param [in] uuid - UUID for the characteristic.
 * @param [in] properties - Properties for the characteristic.
 * @param [in] maxLen - The maximum length in bytes that the characteristic value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pService - pointer to the service instance this characteristic belongs to.
 */
NimBLECharacteristic::NimBLECharacteristic(const NimBLEUUID& uuid, uint16_t properties, uint16_t maxLen, NimBLEService* pService)
    : NimBLELocalValueAttribute{uuid, 0, maxLen}, m_pCallbacks{&defaultCallback}, m_pService{pService} {
    setProperties(properties);
} // NimBLECharacteristic

/**
 * @brief Destructor.
 */
NimBLECharacteristic::~NimBLECharacteristic() {
    for (const auto& dsc : m_vDescriptors) {
        delete dsc;
    }
} // ~NimBLECharacteristic

/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @param [in] maxLen - The max length in bytes of the descriptor value.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(const char* uuid, uint32_t properties, uint16_t maxLen) {
    return createDescriptor(NimBLEUUID(uuid), properties, maxLen);
}

/**
 * @brief Create a new BLE Descriptor associated with this characteristic.
 * @param [in] uuid - The UUID of the descriptor.
 * @param [in] properties - The properties of the descriptor.
 * @param [in] maxLen - The max length in bytes of the descriptor value.
 * @return The new BLE descriptor.
 */
NimBLEDescriptor* NimBLECharacteristic::createDescriptor(const NimBLEUUID& uuid, uint32_t properties, uint16_t maxLen) {
    NimBLEDescriptor* pDescriptor = nullptr;
    if (uuid == NimBLEUUID(static_cast<uint16_t>(0x2904))) {
        NIMBLE_LOGW(LOG_TAG, "0x2904 descriptor should be created with create2904()");
        pDescriptor = create2904();
    } else {
        pDescriptor = new NimBLEDescriptor(uuid, properties, maxLen, this);
    }

    addDescriptor(pDescriptor);
    return pDescriptor;
} // createDescriptor

/**
 * @brief Create a Characteristic Presentation Format Descriptor for this characteristic.
 * @return A pointer to a NimBLE2904 descriptor.
 */
NimBLE2904* NimBLECharacteristic::create2904() {
    NimBLE2904* pDescriptor = new NimBLE2904(this);
    addDescriptor(pDescriptor);
    return pDescriptor;
} // create2904

/**
 * @brief Add a descriptor to the characteristic.
 * @param [in] pDescriptor A pointer to the descriptor to add.
 */
void NimBLECharacteristic::addDescriptor(NimBLEDescriptor* pDescriptor) {
    bool foundRemoved = false;
    if (pDescriptor->getRemoved() > 0) {
        for (const auto& dsc : m_vDescriptors) {
            if (dsc == pDescriptor) {
                foundRemoved = true;
                pDescriptor->setRemoved(0);
            }
        }
    }

    // Check if the descriptor is already in the vector and if so, return.
    for (const auto& dsc : m_vDescriptors) {
        if (dsc == pDescriptor) {
            pDescriptor->setCharacteristic(this); // Update the characteristic pointer in the descriptor.
            return;
        }
    }

    if (!foundRemoved) {
        m_vDescriptors.push_back(pDescriptor);
    }

    pDescriptor->setCharacteristic(this);
    NimBLEDevice::getServer()->serviceChanged();
}

/**
 * @brief Remove a descriptor from the characteristic.
 * @param[in] pDescriptor A pointer to the descriptor instance to remove from the characteristic.
 * @param[in] deleteDsc If true it will delete the descriptor instance and free it's resources.
 */
void NimBLECharacteristic::removeDescriptor(NimBLEDescriptor* pDescriptor, bool deleteDsc) {
    // Check if the descriptor was already removed and if so, check if this
    // is being called to delete the object and do so if requested.
    // Otherwise, ignore the call and return.
    if (pDescriptor->getRemoved() > 0) {
        if (deleteDsc) {
            for (auto it = m_vDescriptors.begin(); it != m_vDescriptors.end(); ++it) {
                if ((*it) == pDescriptor) {
                    delete (*it);
                    m_vDescriptors.erase(it);
                    break;
                }
            }
        }

        return;
    }

    pDescriptor->setRemoved(deleteDsc ? NIMBLE_ATT_REMOVE_DELETE : NIMBLE_ATT_REMOVE_HIDE);
    NimBLEDevice::getServer()->serviceChanged();
} // removeDescriptor

/**
 * @brief Return the BLE Descriptor for the given UUID.
 * @param [in] uuid The UUID of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(const char* uuid) const {
    return getDescriptorByUUID(NimBLEUUID(uuid));
} // getDescriptorByUUID

/**
 * @brief Return the BLE Descriptor for the given UUID.
 * @param [in] uuid The UUID of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByUUID(const NimBLEUUID& uuid) const {
    for (const auto& dsc : m_vDescriptors) {
        if (dsc->getUUID() == uuid) {
            return dsc;
        }
    }
    return nullptr;
} // getDescriptorByUUID

/**
 * @brief Return the BLE Descriptor for the given handle.
 * @param [in] handle The handle of the descriptor.
 * @return A pointer to the descriptor object or nullptr if not found.
 */
NimBLEDescriptor* NimBLECharacteristic::getDescriptorByHandle(uint16_t handle) const {
    for (const auto& dsc : m_vDescriptors) {
        if (dsc->getHandle() == handle) {
            return dsc;
        }
    }
    return nullptr;
} // getDescriptorByHandle

/**
 * @brief Get the properties of the characteristic.
 * @return The properties of the characteristic.
 */
uint16_t NimBLECharacteristic::getProperties() const {
    return m_properties;
} // getProperties

/**
 * @brief Get the service that owns this characteristic.
 */
NimBLEService* NimBLECharacteristic::getService() const {
    return m_pService;
} // getService

void NimBLECharacteristic::setService(NimBLEService* pService) {
    m_pService = pService;
} // setService

/**
 * @brief Send an indication.
 * @param[in] connHandle Connection handle to send an individual indication, or BLE_HS_CONN_HANDLE_NONE to send
 * the indication to all subscribed clients.
 * @return True if the indication was sent successfully, false otherwise.
 */
bool NimBLECharacteristic::indicate(uint16_t connHandle) const {
    auto value{m_value}; // make a copy to avoid issues if the value is changed while indicating
    return sendValue(value.data(), value.size(), false, connHandle);
} // indicate

/**
 * @brief Send an indication.
 * @param[in] value A pointer to the data to send.
 * @param[in] length The length of the data to send.
 * @param[in] connHandle Connection handle to send an individual indication, or BLE_HS_CONN_HANDLE_NONE to send
 * the indication to all subscribed clients.
 * @return True if the indication was sent successfully, false otherwise.
 */
bool NimBLECharacteristic::indicate(const uint8_t* value, size_t length, uint16_t connHandle) const {
    return sendValue(value, length, false, connHandle);
} // indicate

/**
 * @brief Send a notification.
 * @param[in] connHandle Connection handle to send an individual notification, or BLE_HS_CONN_HANDLE_NONE to send
 * the notification to all subscribed clients.
 * @return True if the notification was sent successfully, false otherwise.
 */
bool NimBLECharacteristic::notify(uint16_t connHandle) const {
    auto value{m_value}; // make a copy to avoid issues if the value is changed while notifying
    return sendValue(value.data(), value.size(), true, connHandle);
} // notify

/**
 * @brief Send a notification.
 * @param[in] value A pointer to the data to send.
 * @param[in] length The length of the data to send.
 * @param[in] connHandle Connection handle to send an individual notification, or BLE_HS_CONN_HANDLE_NONE to send
 * the notification to all subscribed clients.
 * @return True if the notification was sent successfully, false otherwise.
 */
bool NimBLECharacteristic::notify(const uint8_t* value, size_t length, uint16_t connHandle) const {
    return sendValue(value, length, true, connHandle);
} // indicate

/**
 * @brief Sends a notification or indication.
 * @param[in] value A pointer to the data to send.
 * @param[in] length The length of the data to send.
 * @param[in] isNotification if true sends a notification, false sends an indication.
 * @param[in] connHandle Connection handle to send to a specific peer.
 * @return True if the value was sent successfully, false otherwise.
 */
bool NimBLECharacteristic::sendValue(const uint8_t* value, size_t length, bool isNotification, uint16_t connHandle) const {
    ble_npl_hw_enter_critical();
    const auto subs = getSubscribers(); // make a copy to avoid issues if subscribers change while sending
    ble_npl_hw_exit_critical(0);

    bool chSpecified = connHandle != BLE_HS_CONN_HANDLE_NONE;
    bool requireSecure = m_properties & (BLE_GATT_CHR_F_READ_ENC | BLE_GATT_CHR_F_READ_AUTHEN | BLE_GATT_CHR_F_READ_AUTHOR);
    int rc = chSpecified ? BLE_HS_ENOENT : 0; // if handle specified, assume not found until sent

    // Notify all connected peers unless a specific handle is provided
    for (const auto& entry : subs) {
        uint16_t ch = entry.getConnHandle();
        if (ch == BLE_HS_CONN_HANDLE_NONE || (chSpecified && ch != connHandle)) {
            continue;
        }

        if (requireSecure && !entry.isSecured()) {
            NIMBLE_LOGW(LOG_TAG, "skipping notify/indicate to connHandle=%d, link not secured", entry.getConnHandle());
            continue;
        }

        // Must re-create the data buffer on each iteration because it is freed by the calls below.
        uint8_t  retries = 10; // wait up to 10ms for a free buffer
        os_mbuf* om      = ble_hs_mbuf_from_flat(value, length);
        while (!om && --retries) {
            ble_npl_time_delay(ble_npl_time_ms_to_ticks32(1));
            om = ble_hs_mbuf_from_flat(value, length);
        }

        if (!om) {
            rc = BLE_HS_ENOMEM;
            break;
        }

        if (isNotification) {
            rc = ble_gatts_notify_custom(ch, m_handle, om);
        } else {
            rc = ble_gatts_indicate_custom(ch, m_handle, om);
        }

        if (rc != 0 || chSpecified) {
            break;
        }
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "failed to send value, rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    return true;
} // sendValue

/**
 * @brief Process a subscription or unsubscription request from a peer.
 * @param[in] connInfo A reference to the connection info of the peer.
 * @param[in] subVal The subscription value (bitmask).
 */
void NimBLECharacteristic::processSubRequest(NimBLEConnInfo& connInfo, uint8_t subVal) const {
    // Only allocate subscribers for characteristics that support notify or indicate.
    const uint16_t props = getProperties();
    if (!(props & (BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_INDICATE))) {
        return;
    }

    auto found     = false;
    auto firstFree = -1;
    for (auto& entry : m_subPeers) {
        if (entry.getConnHandle() == connInfo.getConnHandle()) {
            found = true;
            if (!subVal) {
                ble_npl_hw_enter_critical();
                entry = SubPeerEntry{}; // unsubscribed, reset entry
                ble_npl_hw_exit_critical(0);
            }
            break;
        }

        if (firstFree == -1 && entry.getConnHandle() == BLE_HS_CONN_HANDLE_NONE) {
            firstFree = &entry - &m_subPeers[0];
        }
    }

    if (!found && subVal) {
        if (firstFree >= 0) {
            ble_npl_hw_enter_critical();
            m_subPeers[firstFree].setConnHandle(connInfo.getConnHandle());
            m_subPeers[firstFree].setSubNotify(subVal & 0x1);
            m_subPeers[firstFree].setSubIndicate(subVal & 0x2);
            m_subPeers[firstFree].setSecured(connInfo.isEncrypted() || connInfo.isAuthenticated() || connInfo.isBonded());
            if (m_properties & (BLE_GATT_CHR_F_READ_AUTHEN | BLE_GATT_CHR_F_READ_AUTHOR | BLE_GATT_CHR_F_READ_ENC)) {
                // characteristic requires security/authorization
                if (!m_subPeers[firstFree].isSecured()) {
                    m_subPeers[firstFree].setAwaitingSecure(true);
                    ble_npl_hw_exit_critical(0);
                    NimBLEDevice::startSecurity(connInfo.getConnHandle());
                    NIMBLE_LOGD(LOG_TAG,
                                "Subscription deferred until link is secured for connHandle=%d",
                                connInfo.getConnHandle());
                    return;
                }
            }
            ble_npl_hw_exit_critical(0);
        } else {
            // should never happen, but log just in case
            NIMBLE_LOGE(LOG_TAG, "No free subscription slots");
            return;
        }
    }

    m_pCallbacks->onSubscribe(const_cast<NimBLECharacteristic*>(this), const_cast<NimBLEConnInfo&>(connInfo), subVal);
}

/**
 * @brief Update the security status of a subscribed peer.
 * @param[in] peerInfo A reference to the connection info of the peer.
 */
void NimBLECharacteristic::updatePeerStatus(const NimBLEConnInfo& peerInfo) const {
    if (!(getProperties() & (NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::INDICATE))) {
        return;
    }

    ble_npl_hw_enter_critical();
    for (auto& entry : m_subPeers) {
        if (entry.getConnHandle() == peerInfo.getConnHandle()) {
            entry.setSecured(peerInfo.isEncrypted() || peerInfo.isAuthenticated() || peerInfo.isBonded());
            if (entry.isAwaitingSecure()) {
                entry.setAwaitingSecure(false);
                ble_npl_hw_exit_critical(0);
                m_pCallbacks->onSubscribe(const_cast<NimBLECharacteristic*>(this),
                                          const_cast<NimBLEConnInfo&>(peerInfo),
                                          entry.isSubNotify() | (entry.isSubIndicate() << 1));
                return;
            }
            break;
        }
    }
    ble_npl_hw_exit_critical(0);
}

/**
 * @brief Handle a read event from a client.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLECharacteristic::readEvent(NimBLEConnInfo& connInfo) {
    m_pCallbacks->onRead(this, connInfo);
} // readEvent

/**
 * @brief Handle a write event from a client.
 * @param [in] val A pointer to the data written by the client.
 * @param [in] len The length of the data written by the client.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLECharacteristic::writeEvent(const uint8_t* val, uint16_t len, NimBLEConnInfo& connInfo) {
    setValue(val, len);
    m_pCallbacks->onWrite(this, connInfo);
} // writeEvent

/**
 * @brief Set the callback handlers for this characteristic.
 * @param [in] pCallbacks An instance of a NimBLECharacteristicCallbacks class\n
 * used to define any callbacks for the characteristic.
 */
void NimBLECharacteristic::setCallbacks(NimBLECharacteristicCallbacks* pCallbacks) {
    if (pCallbacks != nullptr) {
        m_pCallbacks = pCallbacks;
    } else {
        m_pCallbacks = &defaultCallback;
    }
} // setCallbacks

/**
 * @brief Get the callback handlers for this characteristic.
 */
NimBLECharacteristicCallbacks* NimBLECharacteristic::getCallbacks() const {
    return m_pCallbacks;
} // getCallbacks

/**
 * @brief Return a string representation of the characteristic.
 * @return A string representation of the characteristic.
 */
std::string NimBLECharacteristic::toString() const {
    std::string res = "UUID: " + m_uuid.toString() + ", handle : 0x";
    char        hex[5];
    snprintf(hex, sizeof(hex), "%04x", getHandle());
    res += hex;
    res += " ";
    if (m_properties & BLE_GATT_CHR_PROP_READ) res += "Read ";
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
 * @brief Callback function to support a Notify/Indicate Status report.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 * @param [in] code Status return code from the NimBLE stack.
 * @details The status code for success is 0 for notifications and BLE_HS_EDONE for indications,
 * any other value is an error.
 */
void NimBLECharacteristicCallbacks::onStatus(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, int code) {
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
                                                NimBLEConnInfo&       connInfo,
                                                uint16_t              subValue) {
    NIMBLE_LOGD("NimBLECharacteristicCallbacks", "onSubscribe: default");
}

#endif // CONFIG_BT_NIMBLE_ENABLED && MYNEWT_VAL(BLE_ROLE_PERIPHERAL)
