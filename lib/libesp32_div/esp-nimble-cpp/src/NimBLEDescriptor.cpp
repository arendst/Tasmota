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

#include "NimBLEDescriptor.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL

# include "NimBLEService.h"
# include "NimBLELog.h"

# include <string>

static const char*               LOG_TAG = "NimBLEDescriptor";
static NimBLEDescriptorCallbacks defaultCallbacks;

/**
 * @brief Construct a descriptor
 * @param [in] uuid - UUID (const char*) for the descriptor.
 * @param [in] properties - Properties for the descriptor.
 * @param [in] max_len - The maximum length in bytes that the descriptor value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pCharacteristic - pointer to the characteristic instance this descriptor belongs to.
 */
NimBLEDescriptor::NimBLEDescriptor(const char* uuid, uint16_t properties, uint16_t max_len, NimBLECharacteristic* pCharacteristic)
    : NimBLEDescriptor(NimBLEUUID(uuid), properties, max_len, pCharacteristic) {}

/**
 * @brief Construct a descriptor
 * @param [in] uuid - UUID (const char*) for the descriptor.
 * @param [in] properties - Properties for the descriptor.
 * @param [in] max_len - The maximum length in bytes that the descriptor value can hold. (Default: 512 bytes for esp32, 20 for all others).
 * @param [in] pCharacteristic - pointer to the characteristic instance this descriptor belongs to.
 */
NimBLEDescriptor::NimBLEDescriptor(const NimBLEUUID& uuid, uint16_t properties, uint16_t max_len, NimBLECharacteristic* pCharacteristic)
    : NimBLELocalValueAttribute{uuid, 0, max_len}, m_pCallbacks{&defaultCallbacks}, m_pCharacteristic{pCharacteristic} {
    // Check if this is the client configuration descriptor and set to removed if true.
    if (uuid == NimBLEUUID((uint16_t)0x2902)) {
        NIMBLE_LOGW(LOG_TAG, "Manually created 2902 descriptor has no functionality; please remove.");
        setRemoved(NIMBLE_ATT_REMOVE_HIDE);
    }

    // convert uint16_t properties to uint8_t for descriptor properties
    uint8_t descProperties = 0;
    if (properties & NIMBLE_PROPERTY::READ) {
        descProperties |= BLE_ATT_F_READ;
    }
    if (properties & (NIMBLE_PROPERTY::WRITE_NR | NIMBLE_PROPERTY::WRITE)) {
        descProperties |= BLE_ATT_F_WRITE;
    }
    if (properties & NIMBLE_PROPERTY::READ_ENC) {
        descProperties |= BLE_ATT_F_READ_ENC;
    }
    if (properties & NIMBLE_PROPERTY::READ_AUTHEN) {
        descProperties |= BLE_ATT_F_READ_AUTHEN;
    }
    if (properties & NIMBLE_PROPERTY::READ_AUTHOR) {
        descProperties |= BLE_ATT_F_READ_AUTHOR;
    }
    if (properties & NIMBLE_PROPERTY::WRITE_ENC) {
        descProperties |= BLE_ATT_F_WRITE_ENC;
    }
    if (properties & NIMBLE_PROPERTY::WRITE_AUTHEN) {
        descProperties |= BLE_ATT_F_WRITE_AUTHEN;
    }
    if (properties & NIMBLE_PROPERTY::WRITE_AUTHOR) {
        descProperties |= BLE_ATT_F_WRITE_AUTHOR;
    }

    setProperties(descProperties);
} // NimBLEDescriptor

/**
 * @brief Get the characteristic this descriptor belongs to.
 * @return A pointer to the characteristic this descriptor belongs to.
 */
NimBLECharacteristic* NimBLEDescriptor::getCharacteristic() const {
    return m_pCharacteristic;
} // getCharacteristic

/**
 * @brief Set the callback handlers for this descriptor.
 * @param [in] pCallbacks An instance of a callback structure used to define any callbacks for the descriptor.
 */
void NimBLEDescriptor::setCallbacks(NimBLEDescriptorCallbacks* pCallbacks) {
    if (pCallbacks != nullptr) {
        m_pCallbacks = pCallbacks;
    } else {
        m_pCallbacks = &defaultCallbacks;
    }
} // setCallbacks

/**
 * @brief Set the characteristic this descriptor belongs to.
 * @param [in] pChar A pointer to the characteristic this descriptor belongs to.
 */
void NimBLEDescriptor::setCharacteristic(NimBLECharacteristic* pChar) {
    m_pCharacteristic = pChar;
} // setCharacteristic

/**
 * @brief Return a string representation of the descriptor.
 * @return A string representation of the descriptor.
 */
std::string NimBLEDescriptor::toString() const {
    char hex[5];
    snprintf(hex, sizeof(hex), "%04x", getHandle());
    std::string res = "UUID: " + m_uuid.toString() + ", handle: 0x" + hex;
    return res;
} // toString

void NimBLEDescriptor::readEvent(NimBLEConnInfo& connInfo) {
    m_pCallbacks->onRead(this, connInfo);
} // readEvent

void NimBLEDescriptor::writeEvent(const uint8_t* val, uint16_t len, NimBLEConnInfo& connInfo) {
    setValue(val, len);
    m_pCallbacks->onWrite(this, connInfo);
} // writeEvent

/**
 * @brief Callback function to support a read request.
 * @param [in] pDescriptor The descriptor that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLEDescriptorCallbacks::onRead(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLEDescriptorCallbacks", "onRead: default");
} // onRead

/**
 * @brief Callback function to support a write request.
 * @param [in] pDescriptor The descriptor that is the source of the event.
 * @param [in] connInfo A reference to a NimBLEConnInfo instance containing the peer info.
 */
void NimBLEDescriptorCallbacks::onWrite(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo) {
    NIMBLE_LOGD("NimBLEDescriptorCallbacks", "onWrite: default");
} // onWrite

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
