/*
 * NimBLEDescriptor.cpp
 *
 *  Created: on March 10, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEDescriptor.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEService.h"
#include "NimBLEDescriptor.h"
#include "NimBLELog.h"

#include <string>

#define NULL_HANDLE (0xffff)

static const char* LOG_TAG = "NimBLEDescriptor";
static NimBLEDescriptorCallbacks defaultCallbacks;


/**
 * @brief NimBLEDescriptor constructor.
 */
NimBLEDescriptor::NimBLEDescriptor(const char* uuid, uint16_t properties, uint16_t max_len,
                                    NimBLECharacteristic* pCharacteristic)
: NimBLEDescriptor(NimBLEUUID(uuid), max_len, properties, pCharacteristic) {
}

/**
 * @brief NimBLEDescriptor constructor.
 */
NimBLEDescriptor::NimBLEDescriptor(NimBLEUUID uuid, uint16_t properties, uint16_t max_len,
                                    NimBLECharacteristic* pCharacteristic)
{
    m_uuid               = uuid;
    m_value.attr_len     = 0;                           // Initial length is 0.
    m_value.attr_max_len = max_len;                     // Maximum length of the data.
    m_handle             = NULL_HANDLE;                 // Handle is initially unknown.
    m_pCharacteristic    = nullptr;                     // No initial characteristic.
    m_pCallbacks         = &defaultCallbacks;           // No initial callback.
    m_value.attr_value   = (uint8_t*) calloc(max_len,1);  // Allocate storage for the value.
    m_properties         = 0;

    if (properties & BLE_GATT_CHR_F_READ) {             // convert uint16_t properties to uint8_t
        m_properties |= BLE_ATT_F_READ;
    }
    if (properties & (BLE_GATT_CHR_F_WRITE_NO_RSP | BLE_GATT_CHR_F_WRITE)) {
        m_properties |= BLE_ATT_F_WRITE;
    }
    if (properties & BLE_GATT_CHR_F_READ_ENC) {
        m_properties |= BLE_ATT_F_READ_ENC;
    }
    if (properties & BLE_GATT_CHR_F_READ_AUTHEN) {
        m_properties |= BLE_ATT_F_READ_AUTHEN;
    }
    if (properties & BLE_GATT_CHR_F_READ_AUTHOR) {
        m_properties |= BLE_ATT_F_READ_AUTHOR;
    }
    if (properties & BLE_GATT_CHR_F_WRITE_ENC) {
        m_properties |= BLE_ATT_F_WRITE_ENC;
    }
    if (properties & BLE_GATT_CHR_F_WRITE_AUTHEN) {
        m_properties |= BLE_ATT_F_WRITE_AUTHEN;
    }
    if (properties & BLE_GATT_CHR_F_WRITE_AUTHOR) {
        m_properties |= BLE_ATT_F_WRITE_AUTHOR;
    }

} // NimBLEDescriptor


/**
 * @brief NimBLEDescriptor destructor.
 */
NimBLEDescriptor::~NimBLEDescriptor() {
    free(m_value.attr_value);   // Release the storage we created in the constructor.
} // ~NimBLEDescriptor

/**
 * @brief Get the BLE handle for this descriptor.
 * @return The handle for this descriptor.
 */
uint16_t NimBLEDescriptor::getHandle() {
    return m_handle;
} // getHandle


/**
 * @brief Get the length of the value of this descriptor.
 * @return The length (in bytes) of the value of this descriptor.
 */
size_t NimBLEDescriptor::getLength() {
    return m_value.attr_len;
} // getLength


/**
 * @brief Get the UUID of the descriptor.
 */
NimBLEUUID NimBLEDescriptor::getUUID() {
    return m_uuid;
} // getUUID


/**
 * @brief Get the value of this descriptor.
 * @return A pointer to the value of this descriptor.
 */
uint8_t* NimBLEDescriptor::getValue() {
    return m_value.attr_value;
} // getValue


int NimBLEDescriptor::handleGapEvent(uint16_t conn_handle, uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt,
                             void *arg)
{
    const ble_uuid_t *uuid;
    int rc;
    NimBLEDescriptor* pDescriptor = (NimBLEDescriptor*)arg;

    NIMBLE_LOGD(LOG_TAG, "Descriptor %s %s event", pDescriptor->getUUID().toString().c_str(),
                                    ctxt->op == BLE_GATT_ACCESS_OP_READ_DSC ? "Read" : "Write");

    uuid = ctxt->chr->uuid;
    if(ble_uuid_cmp(uuid, &pDescriptor->getUUID().getNative()->u) == 0){
        switch(ctxt->op) {
            case BLE_GATT_ACCESS_OP_READ_DSC: {
                pDescriptor->m_pCallbacks->onRead(pDescriptor);
                rc = os_mbuf_append(ctxt->om, pDescriptor->getValue(), pDescriptor->getLength());
                return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
            }

            case BLE_GATT_ACCESS_OP_WRITE_DSC: {
                if (ctxt->om->om_len > BLE_ATT_ATTR_MAX_LEN) {
                    return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
                }

                pDescriptor->setValue(ctxt->om->om_data, ctxt->om->om_len);
                pDescriptor->m_pCallbacks->onWrite(pDescriptor);
                return 0;
            }
            default:
                break;
        }
    }

    return BLE_ATT_ERR_UNLIKELY;
}

/**
 * @brief Set the callback handlers for this descriptor.
 * @param [in] pCallbacks An instance of a callback structure used to define any callbacks for the descriptor.
 */
void NimBLEDescriptor::setCallbacks(NimBLEDescriptorCallbacks* pCallbacks) {
    if (pCallbacks != nullptr){
        m_pCallbacks = pCallbacks;
    } else {
        m_pCallbacks = &defaultCallbacks;
    }
} // setCallbacks


/**
 * @brief Set the handle of this descriptor.
 * Set the handle of this descriptor to be the supplied value.
 * @param [in] handle The handle to be associated with this descriptor.
 * @return N/A.
 */
void NimBLEDescriptor::setHandle(uint16_t handle) {
    NIMBLE_LOGD(LOG_TAG, ">> setHandle(0x%.2x): Setting descriptor handle to be 0x%.2x", handle, handle);
    m_handle = handle;
    NIMBLE_LOGD(LOG_TAG, "<< setHandle()");
} // setHandle


/**
 * @brief Set the value of the descriptor.
 * @param [in] data The data to set for the descriptor.
 * @param [in] length The length of the data in bytes.
 */
void NimBLEDescriptor::setValue(const uint8_t* data, size_t length) {
    if (length > BLE_ATT_ATTR_MAX_LEN) {
        NIMBLE_LOGE(LOG_TAG, "Size %d too large, must be no bigger than %d", length, BLE_ATT_ATTR_MAX_LEN);
        return;
    }
    m_value.attr_len = length;
    memcpy(m_value.attr_value, data, length);
} // setValue


/**
 * @brief Set the value of the descriptor.
 * @param [in] value The value of the descriptor in string form.
 */
void NimBLEDescriptor::setValue(const std::string &value) {
    setValue((uint8_t*) value.data(), value.length());
} // setValue


/*
void NimBLEDescriptor::setAccessPermissions(uint8_t perm) {
    m_permissions = perm;
}
*/


/**
 * @brief Return a string representation of the descriptor.
 * @return A string representation of the descriptor.
 */
std::string NimBLEDescriptor::toString() {
    char hex[5];
    snprintf(hex, sizeof(hex), "%04x", m_handle);
    std::string res = "UUID: " + m_uuid.toString() + ", handle: 0x" + hex;
    return res;
} // toString


NimBLEDescriptorCallbacks::~NimBLEDescriptorCallbacks() {}

/**
 * @brief Callback function to support a read request.
 * @param [in] pDescriptor The descriptor that is the source of the event.
 */
void NimBLEDescriptorCallbacks::onRead(NimBLEDescriptor* pDescriptor) {
    NIMBLE_LOGD("NimBLEDescriptorCallbacks", "onRead: default");
} // onRead


/**
 * @brief Callback function to support a write request.
 * @param [in] pDescriptor The descriptor that is the source of the event.
 */
void NimBLEDescriptorCallbacks::onWrite(NimBLEDescriptor* pDescriptor) {
    NIMBLE_LOGD("NimBLEDescriptorCallbacks", "onWrite: default");
} // onWrite

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
