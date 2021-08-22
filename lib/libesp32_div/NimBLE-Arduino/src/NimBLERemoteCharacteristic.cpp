/*
 * NimBLERemoteCharacteristic.cpp
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLERemoteCharacteristic.cpp
 *
 *  Created on: Mar 16, 2017
 *      Author: kolban
 */

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLERemoteCharacteristic.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLERemoteCharacteristic";

/**
 * @brief Constructor.
 * @param [in] reference to the service this characteristic belongs to.
 * @param [in] ble_gatt_chr struct defined as:
 *  struct ble_gatt_chr {
 *      uint16_t def_handle;
 *      uint16_t val_handle;
 *      uint8_t properties;
 *      ble_uuid_any_t uuid;
 *  };
 */
 NimBLERemoteCharacteristic::NimBLERemoteCharacteristic(NimBLERemoteService *pRemoteService,
                                                        const struct ble_gatt_chr *chr)
{
    NIMBLE_LOGD(LOG_TAG, ">> NimBLERemoteCharacteristic()");
     switch (chr->uuid.u.type) {
        case BLE_UUID_TYPE_16:
            m_uuid = NimBLEUUID(chr->uuid.u16.value);
            break;
        case BLE_UUID_TYPE_32:
            m_uuid = NimBLEUUID(chr->uuid.u32.value);
            break;
        case BLE_UUID_TYPE_128:
            m_uuid = NimBLEUUID(const_cast<ble_uuid128_t*>(&chr->uuid.u128));
            break;
        default:
            break;
    }

    m_handle             = chr->val_handle;
    m_defHandle          = chr->def_handle;
    m_endHandle          = 0;
    m_charProp           = chr->properties;
    m_pRemoteService     = pRemoteService;
    m_notifyCallback     = nullptr;
    m_timestamp          = 0;
    m_valMux             = portMUX_INITIALIZER_UNLOCKED;

    NIMBLE_LOGD(LOG_TAG, "<< NimBLERemoteCharacteristic(): %s", m_uuid.toString().c_str());
 } // NimBLERemoteCharacteristic


/**
 *@brief Destructor.
 */
NimBLERemoteCharacteristic::~NimBLERemoteCharacteristic() {
    deleteDescriptors();
} // ~NimBLERemoteCharacteristic

/*
#define BLE_GATT_CHR_PROP_BROADCAST                     0x01
#define BLE_GATT_CHR_PROP_READ                          0x02
#define BLE_GATT_CHR_PROP_WRITE_NO_RSP                  0x04
#define BLE_GATT_CHR_PROP_WRITE                         0x08
#define BLE_GATT_CHR_PROP_NOTIFY                        0x10
#define BLE_GATT_CHR_PROP_INDICATE                      0x20
#define BLE_GATT_CHR_PROP_AUTH_SIGN_WRITE               0x40
#define BLE_GATT_CHR_PROP_EXTENDED                      0x80
*/

/**
 * @brief Does the characteristic support broadcasting?
 * @return True if the characteristic supports broadcasting.
 */
bool NimBLERemoteCharacteristic::canBroadcast() {
    return (m_charProp & BLE_GATT_CHR_PROP_BROADCAST) != 0;
} // canBroadcast


/**
 * @brief Does the characteristic support indications?
 * @return True if the characteristic supports indications.
 */
bool NimBLERemoteCharacteristic::canIndicate() {
    return (m_charProp & BLE_GATT_CHR_PROP_INDICATE) != 0;
} // canIndicate


/**
 * @brief Does the characteristic support notifications?
 * @return True if the characteristic supports notifications.
 */
bool NimBLERemoteCharacteristic::canNotify() {
    return (m_charProp & BLE_GATT_CHR_PROP_NOTIFY) != 0;
} // canNotify


/**
 * @brief Does the characteristic support reading?
 * @return True if the characteristic supports reading.
 */
bool NimBLERemoteCharacteristic::canRead() {
    return (m_charProp & BLE_GATT_CHR_PROP_READ) != 0;
} // canRead


/**
 * @brief Does the characteristic support writing?
 * @return True if the characteristic supports writing.
 */
bool NimBLERemoteCharacteristic::canWrite() {
    return (m_charProp & BLE_GATT_CHR_PROP_WRITE) != 0;
} // canWrite


/**
 * @brief Does the characteristic support writing with no response?
 * @return True if the characteristic supports writing with no response.
 */
bool NimBLERemoteCharacteristic::canWriteNoResponse() {
    return (m_charProp & BLE_GATT_CHR_PROP_WRITE_NO_RSP) != 0;
} // canWriteNoResponse


/**
 * @brief Callback used by the API when a descriptor is discovered or search complete.
 */
int NimBLERemoteCharacteristic::descriptorDiscCB(uint16_t conn_handle,
                                    const struct ble_gatt_error *error,
                                    uint16_t chr_val_handle,
                                    const struct ble_gatt_dsc *dsc,
                                    void *arg)
{
    int rc = error->status;
    NIMBLE_LOGD(LOG_TAG, "Descriptor Discovered >> status: %d handle: %d",
                         rc, (rc == 0) ? dsc->handle : -1);

    desc_filter_t *filter = (desc_filter_t*)arg;
    const NimBLEUUID *uuid_filter = filter->uuid;
    ble_task_data_t *pTaskData = (ble_task_data_t*)filter->task_data;
    NimBLERemoteCharacteristic *characteristic = (NimBLERemoteCharacteristic*)pTaskData->pATT;

    if (characteristic->getRemoteService()->getClient()->getConnId() != conn_handle){
        return 0;
    }

    switch (rc) {
        case 0: {
            if (uuid_filter != nullptr) {
                if (ble_uuid_cmp(&uuid_filter->getNative()->u, &dsc->uuid.u) != 0) {
                    return 0;
                } else {
                    rc = BLE_HS_EDONE;
                }
            }

            NimBLERemoteDescriptor* pNewRemoteDescriptor = new NimBLERemoteDescriptor(characteristic, dsc);
            characteristic->m_descriptorVector.push_back(pNewRemoteDescriptor);
            break;
        }
        default:
            break;
    }

    /*  If rc == BLE_HS_EDONE, resume the task with a success error code and stop the discovery process.
     *  Else if rc == 0, just return 0 to continue the discovery until we get BLE_HS_EDONE.
     *  If we get any other error code tell the application to abort by returning non-zero in the rc.
     */
    if (rc == BLE_HS_EDONE) {
        pTaskData->rc = 0;
        xTaskNotifyGive(pTaskData->task);
    } else if(rc != 0) {
        // Error; abort discovery.
        pTaskData->rc = rc;
        xTaskNotifyGive(pTaskData->task);
    }

    NIMBLE_LOGD(LOG_TAG,"<< Descriptor Discovered. status: %d", pTaskData->rc);
    return rc;
}


/**
 * @brief callback from NimBLE when the next characteristic of the service is discovered.
 */
int NimBLERemoteCharacteristic::nextCharCB(uint16_t conn_handle,
                                           const struct ble_gatt_error *error,
                                           const struct ble_gatt_chr *chr, void *arg)
{
    int rc = error->status;
    NIMBLE_LOGD(LOG_TAG, "Next Characteristic >> status: %d handle: %d",
                         rc, (rc == 0) ? chr->val_handle : -1);

    ble_task_data_t *pTaskData = (ble_task_data_t*)arg;
    NimBLERemoteCharacteristic *pChar = (NimBLERemoteCharacteristic*)pTaskData->pATT;

    if (pChar->getRemoteService()->getClient()->getConnId() != conn_handle) {
        return 0;
    }

    if (rc == 0) {
        pChar->m_endHandle = chr->def_handle - 1;
        rc = BLE_HS_EDONE;
    } else if (rc == BLE_HS_EDONE) {
        pChar->m_endHandle = pChar->getRemoteService()->getEndHandle();
    } else {
        pTaskData->rc = rc;
    }

    xTaskNotifyGive(pTaskData->task);
    return rc;
}


/**
 * @brief Populate the descriptors (if any) for this characteristic.
 * @param [in] the end handle of the characteristic, or the service, whichever comes first.
 */
bool NimBLERemoteCharacteristic::retrieveDescriptors(const NimBLEUUID *uuid_filter) {
    NIMBLE_LOGD(LOG_TAG, ">> retrieveDescriptors() for characteristic: %s", getUUID().toString().c_str());

    // If this is the last handle then there are no descriptors
    if (m_handle == getRemoteService()->getEndHandle()) {
        return true;
    }

    int rc = 0;
    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(), 0, nullptr};

    // If we don't know the end handle of this characteristic retrieve the next one in the service
    // The end handle is the next characteristic definition handle -1.
    if (m_endHandle == 0) {
        rc = ble_gattc_disc_all_chrs(getRemoteService()->getClient()->getConnId(),
                                     m_handle,
                                     getRemoteService()->getEndHandle(),
                                     NimBLERemoteCharacteristic::nextCharCB,
                                     &taskData);
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Error getting end handle rc=%d", rc);
            return false;
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (taskData.rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Could not retrieve end handle rc=%d", taskData.rc);
            return false;
        }
    }

    desc_filter_t filter = {uuid_filter, &taskData};

    rc = ble_gattc_disc_all_dscs(getRemoteService()->getClient()->getConnId(),
                                 m_handle,
                                 m_endHandle,
                                 NimBLERemoteCharacteristic::descriptorDiscCB,
                                 &filter);

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_dscs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    if (taskData.rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Failed to retrieve descriptors; startHandle:%d endHandle:%d taskData.rc=%d",
                             m_handle, m_endHandle, taskData.rc);
    }

    NIMBLE_LOGD(LOG_TAG, "<< retrieveDescriptors(): Found %d descriptors.", m_descriptorVector.size());
    return (taskData.rc == 0);
} // retrieveDescriptors


/**
 * @brief Get the descriptor instance with the given UUID that belongs to this characteristic.
 * @param [in] uuid The UUID of the descriptor to find.
 * @return The Remote descriptor (if present) or null if not present.
 */
NimBLERemoteDescriptor* NimBLERemoteCharacteristic::getDescriptor(const NimBLEUUID &uuid) {
    NIMBLE_LOGD(LOG_TAG, ">> getDescriptor: uuid: %s", uuid.toString().c_str());

    for(auto &it: m_descriptorVector) {
        if(it->getUUID() == uuid) {
            NIMBLE_LOGD(LOG_TAG, "<< getDescriptor: found the descriptor with uuid: %s", uuid.toString().c_str());
            return it;
        }
    }

    size_t prev_size = m_descriptorVector.size();
    if(retrieveDescriptors(&uuid)) {
        if(m_descriptorVector.size() > prev_size) {
            return m_descriptorVector.back();
        }

        // If the request was successful but 16/32 bit descriptor not found
        // try again with the 128 bit uuid.
        if(uuid.bitSize() == BLE_UUID_TYPE_16 ||
           uuid.bitSize() == BLE_UUID_TYPE_32)
        {
            NimBLEUUID uuid128(uuid);
            uuid128.to128();
            return getDescriptor(uuid128);
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< getDescriptor: Not found");
    return nullptr;
} // getDescriptor


/**
 * @brief Get a pointer to the vector of found descriptors.
 * @param [in] refresh If true the current descriptor vector will be cleared and\n
 * all descriptors for this characteristic retrieved from the peripheral.\n
 * If false the vector will be returned with the currently stored descriptors
 * of this characteristic.
 * @return A pointer to the vector of descriptors for this characteristic.
 */
std::vector<NimBLERemoteDescriptor*>* NimBLERemoteCharacteristic::getDescriptors(bool refresh) {
    if(refresh) {
        deleteDescriptors();

        if (!retrieveDescriptors()) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to get descriptors");
        }
        else{
            NIMBLE_LOGI(LOG_TAG, "Found %d descriptor(s)", m_descriptorVector.size());
        }
    }
    return &m_descriptorVector;
} // getDescriptors


/**
 * @brief Get iterator to the beginning of the vector of remote descriptor pointers.
 * @return An iterator to the beginning of the vector of remote descriptor pointers.
 */
std::vector<NimBLERemoteDescriptor*>::iterator NimBLERemoteCharacteristic::begin() {
    return m_descriptorVector.begin();
}


/**
 * @brief Get iterator to the end of the vector of remote descriptor pointers.
 * @return An iterator to the end of the vector of remote descriptor pointers.
 */
std::vector<NimBLERemoteDescriptor*>::iterator NimBLERemoteCharacteristic::end() {
    return m_descriptorVector.end();
}


/**
 * @brief Get the handle for this characteristic.
 * @return The handle for this characteristic.
 */
uint16_t NimBLERemoteCharacteristic::getHandle() {
    return m_handle;
} // getHandle

/**
 * @brief Get the handle for this characteristics definition.
 * @return The handle for this characteristic definition.
 */
uint16_t NimBLERemoteCharacteristic::getDefHandle() {
    return m_defHandle;
} // getDefHandle


/**
 * @brief Get the remote service associated with this characteristic.
 * @return The remote service associated with this characteristic.
 */
NimBLERemoteService* NimBLERemoteCharacteristic::getRemoteService() {
    return m_pRemoteService;
} // getRemoteService


/**
 * @brief Get the UUID for this characteristic.
 * @return The UUID for this characteristic.
 */
NimBLEUUID NimBLERemoteCharacteristic::getUUID() {
    return m_uuid;
} // getUUID


/**
 * @brief Get the value of the remote characteristic.
 * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
 * @return The value of the remote characteristic.
 */
std::string NimBLERemoteCharacteristic::getValue(time_t *timestamp) {
    portENTER_CRITICAL(&m_valMux);
    std::string value = m_value;
    if(timestamp != nullptr) {
        *timestamp = m_timestamp;
    }
    portEXIT_CRITICAL(&m_valMux);

    return value;
}


/**
 * @brief Read an unsigned 16 bit value
 * @return The unsigned 16 bit value.
 * @deprecated Use readValue<uint16_t>().
 */
uint16_t NimBLERemoteCharacteristic::readUInt16() {
    return readValue<uint16_t>();
} // readUInt16


/**
 * @brief Read an unsigned 32 bit value.
 * @return the unsigned 32 bit value.
 * @deprecated Use readValue<uint32_t>().
 */
uint32_t NimBLERemoteCharacteristic::readUInt32() {
    return readValue<uint32_t>();
} // readUInt32


/**
 * @brief Read a byte value
 * @return The value as a byte
 * @deprecated Use readValue<uint8_t>().
 */
uint8_t NimBLERemoteCharacteristic::readUInt8() {
    return readValue<uint8_t>();
} // readUInt8


/**
 * @brief Read a float value.
 * @return the float value.
 */
float NimBLERemoteCharacteristic::readFloat() {
	return readValue<float>();
} // readFloat


/**
 * @brief Read the value of the remote characteristic.
 * @param [in] timestamp A pointer to a time_t struct to store the time the value was read.
 * @return The value of the remote characteristic.
 */
std::string NimBLERemoteCharacteristic::readValue(time_t *timestamp) {
    NIMBLE_LOGD(LOG_TAG, ">> readValue(): uuid: %s, handle: %d 0x%.2x",
                         getUUID().toString().c_str(), getHandle(), getHandle());

    NimBLEClient* pClient = getRemoteService()->getClient();
    std::string value;

    if (!pClient->isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Disconnected");
        return value;
    }

    int rc = 0;
    int retryCount = 1;
    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(),0, &value};

    do {
        rc = ble_gattc_read_long(pClient->getConnId(), m_handle, 0,
                                 NimBLERemoteCharacteristic::onReadCB,
                                 &taskData);
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to read characteristic; rc=%d, %s",
                                  rc, NimBLEUtils::returnCodeToString(rc));
            return value;
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        rc = taskData.rc;

        switch(rc){
            case 0:
            case BLE_HS_EDONE:
                rc = 0;
                break;
            // Characteristic is not long-readable, return with what we have.
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_ATTR_NOT_LONG):
                NIMBLE_LOGI(LOG_TAG, "Attribute not long");
                rc = 0;
                break;
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHEN):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHOR):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_ENC):
                if (retryCount && pClient->secureConnection())
                    break;
            /* Else falls through. */
            default:
                NIMBLE_LOGE(LOG_TAG, "<< readValue rc=%d", rc);
                return value;
        }
    } while(rc != 0 && retryCount--);

    time_t t = time(nullptr);
    portENTER_CRITICAL(&m_valMux);
    m_value = value;
    m_timestamp = t;
    if(timestamp != nullptr) {
        *timestamp = m_timestamp;
    }
    portEXIT_CRITICAL(&m_valMux);

    NIMBLE_LOGD(LOG_TAG, "<< readValue length: %d rc=%d", value.length(), rc);
    return value;
} // readValue


/**
 * @brief Callback for characteristic read operation.
 * @return success == 0 or error code.
 */
int NimBLERemoteCharacteristic::onReadCB(uint16_t conn_handle,
                const struct ble_gatt_error *error,
                struct ble_gatt_attr *attr, void *arg)
{
    ble_task_data_t *pTaskData = (ble_task_data_t*)arg;
    NimBLERemoteCharacteristic *characteristic = (NimBLERemoteCharacteristic*)pTaskData->pATT;
    uint16_t conn_id = characteristic->getRemoteService()->getClient()->getConnId();

    if(conn_id != conn_handle) {
        return 0;
    }

    NIMBLE_LOGI(LOG_TAG, "Read complete; status=%d conn_handle=%d", error->status, conn_handle);

    std::string *strBuf = (std::string*)pTaskData->buf;
    int rc = error->status;

    if(rc == 0) {
        if(attr) {
            if(((*strBuf).length() + attr->om->om_len) > BLE_ATT_ATTR_MAX_LEN) {
                rc = BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
            } else {
                NIMBLE_LOGD(LOG_TAG, "Got %d bytes", attr->om->om_len);
                (*strBuf) += std::string((char*) attr->om->om_data, attr->om->om_len);
                return 0;
            }
        }
    }

    pTaskData->rc = rc;
    xTaskNotifyGive(pTaskData->task);

    return rc;
}


/**
 * @brief Subscribe or unsubscribe for notifications or indications.
 * @param [in] val 0x00 to unsubscribe, 0x01 for notifications, 0x02 for indications.
 * @param [in] notifyCallback A callback to be invoked for a notification.
 * @param [in] response If write response required set this to true.
 * If NULL is provided then no callback is performed.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::setNotify(uint16_t val, notify_callback notifyCallback, bool response) {
    NIMBLE_LOGD(LOG_TAG, ">> setNotify(): %s, %02x", toString().c_str(), val);

    m_notifyCallback = notifyCallback;

    NimBLERemoteDescriptor* desc = getDescriptor(NimBLEUUID((uint16_t)0x2902));
    if(desc == nullptr) {
        NIMBLE_LOGW(LOG_TAG, "<< setNotify(): Callback set, CCCD not found");
        return true;
    }

    NIMBLE_LOGD(LOG_TAG, "<< setNotify()");

    return desc->writeValue((uint8_t *)&val, 2, response);
} // setNotify


/**
 * @brief Subscribe for notifications or indications.
 * @param [in] notifications If true, subscribe for notifications, false subscribe for indications.
 * @param [in] notifyCallback A callback to be invoked for a notification.
 * @param [in] response If true, require a write response from the descriptor write operation.
 * If NULL is provided then no callback is performed.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::subscribe(bool notifications, notify_callback notifyCallback, bool response) {
    if(notifications) {
        return setNotify(0x01, notifyCallback, response);
    } else {
        return setNotify(0x02, notifyCallback, response);
    }
} // subscribe


/**
 * @brief Unsubscribe for notifications or indications.
 * @param [in] response bool if true, require a write response from the descriptor write operation.
 * @return false if writing to the descriptor failed.
 */
bool NimBLERemoteCharacteristic::unsubscribe(bool response) {
    return setNotify(0x00, nullptr, response);
} // unsubscribe


 /**
 * @brief backward-compatibility method for subscribe/unsubscribe notifications/indications
 * @param [in] notifyCallback A callback to be invoked for a notification. If NULL is provided then we
 * will unregister for notifications.
 * @param [in] notifications If true, register for notifications, false register for indications.
 * @param [in] response If true, require a write response from the descriptor write operation.
 * @return true if successful.
 * @deprecated Use subscribe() / unsubscribe() instead.
 */
bool NimBLERemoteCharacteristic::registerForNotify(notify_callback notifyCallback, bool notifications, bool response) {
    bool success;
    if(notifyCallback != nullptr) {
        success = subscribe(notifications, notifyCallback, response);
    } else {
        success = unsubscribe(response);
    }
    return success;
} // registerForNotify


/**
 * @brief Delete the descriptors in the descriptor vector.
 * @details We maintain a vector called m_descriptorVector that contains pointers to NimBLERemoteDescriptors
 * object references. Since we allocated these in this class, we are also responsible for deleting
 * them. This method does just that.
 */
void NimBLERemoteCharacteristic::deleteDescriptors() {
    NIMBLE_LOGD(LOG_TAG, ">> deleteDescriptors");

    for(auto &it: m_descriptorVector) {
        delete it;
    }
    m_descriptorVector.clear();
    NIMBLE_LOGD(LOG_TAG, "<< deleteDescriptors");
} // deleteDescriptors


/**
 * @brief Delete descriptor by UUID
 * @param [in] uuid The UUID of the descriptor to be deleted.
 * @return Number of descriptors left in the vector.
 */
size_t NimBLERemoteCharacteristic::deleteDescriptor(const NimBLEUUID &uuid) {
    NIMBLE_LOGD(LOG_TAG, ">> deleteDescriptor");

    for(auto it = m_descriptorVector.begin(); it != m_descriptorVector.end(); ++it) {
        if((*it)->getUUID() == uuid) {
            delete *it;
            m_descriptorVector.erase(it);
            break;
        }
    }

    NIMBLE_LOGD(LOG_TAG, "<< deleteDescriptor");

    return m_descriptorVector.size();
} // deleteDescriptor


/**
 * @brief Convert a NimBLERemoteCharacteristic to a string representation;
 * @return a String representation.
 */
std::string NimBLERemoteCharacteristic::toString() {
    std::string res = "Characteristic: uuid: " + m_uuid.toString();
    char val[6];
    res += ", handle: ";
    snprintf(val, sizeof(val), "%d", getHandle());
    res += val;
    res += " 0x";
    snprintf(val, sizeof(val), "%04x", getHandle());
    res += val;
    res += ", props: ";
    res += " 0x";
    snprintf(val, sizeof(val), "%02x", m_charProp);
    res += val;

    for(auto &it: m_descriptorVector) {
        res += "\n" + it->toString();
    }

    return res;
} // toString


/**
 * @brief Write the new value for the characteristic.
 * @param [in] newValue The new value to write.
 * @param [in] response Do we expect a response?
 * @return false if not connected or cant perform write for some reason.
 */
bool NimBLERemoteCharacteristic::writeValue(const std::string &newValue, bool response) {
    return writeValue((uint8_t*)newValue.c_str(), newValue.length(), response);
} // writeValue


/**
 * @brief Write the new value for the characteristic from a data buffer.
 * @param [in] data A pointer to a data buffer.
 * @param [in] length The length of the data in the data buffer.
 * @param [in] response Whether we require a response from the write.
 * @return false if not connected or cant perform write for some reason.
 */
bool NimBLERemoteCharacteristic::writeValue(const uint8_t* data, size_t length, bool response) {

    NIMBLE_LOGD(LOG_TAG, ">> writeValue(), length: %d", length);

    NimBLEClient* pClient = getRemoteService()->getClient();

    if (!pClient->isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Disconnected");
        return false;
    }

    int rc = 0;
    int retryCount = 1;
    uint16_t mtu = ble_att_mtu(pClient->getConnId()) - 3;

    // Check if the data length is longer than we can write in one connection event.
    // If so we must do a long write which requires a response.
    if(length <= mtu && !response) {
        rc =  ble_gattc_write_no_rsp_flat(pClient->getConnId(), m_handle, data, length);
        return (rc==0);
    }

    ble_task_data_t taskData = {this, xTaskGetCurrentTaskHandle(), 0, nullptr};

    do {
        if(length > mtu) {
            NIMBLE_LOGI(LOG_TAG,"long write %d bytes", length);
            os_mbuf *om = ble_hs_mbuf_from_flat(data, length);
            rc = ble_gattc_write_long(pClient->getConnId(), m_handle, 0, om,
                                      NimBLERemoteCharacteristic::onWriteCB,
                                      &taskData);
        } else {
            rc = ble_gattc_write_flat(pClient->getConnId(), m_handle,
                                      data, length,
                                      NimBLERemoteCharacteristic::onWriteCB,
                                      &taskData);
        }
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to write characteristic; rc=%d", rc);
            return false;
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        rc = taskData.rc;

        switch(rc){
            case 0:
            case BLE_HS_EDONE:
                rc = 0;
                break;
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_ATTR_NOT_LONG):
                NIMBLE_LOGE(LOG_TAG, "Long write not supported by peer; Truncating length to %d", mtu);
                retryCount++;
                length = mtu;
                break;

            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHEN):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHOR):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_ENC):
                if (retryCount && pClient->secureConnection())
                    break;
            /* Else falls through. */
            default:
                NIMBLE_LOGE(LOG_TAG, "<< writeValue, rc: %d", rc);
                return false;
        }
    } while(rc != 0 && retryCount--);

    NIMBLE_LOGD(LOG_TAG, "<< writeValue, rc: %d", rc);
    return (rc == 0);
} // writeValue


/**
 * @brief Callback for characteristic write operation.
 * @return success == 0 or error code.
 */
int NimBLERemoteCharacteristic::onWriteCB(uint16_t conn_handle,
                const struct ble_gatt_error *error,
                struct ble_gatt_attr *attr, void *arg)
{
    ble_task_data_t *pTaskData = (ble_task_data_t*)arg;
    NimBLERemoteCharacteristic *characteristic = (NimBLERemoteCharacteristic*)pTaskData->pATT;

    if(characteristic->getRemoteService()->getClient()->getConnId() != conn_handle){
        return 0;
    }

    NIMBLE_LOGI(LOG_TAG, "Write complete; status=%d conn_handle=%d", error->status, conn_handle);

    pTaskData->rc = error->status;
    xTaskNotifyGive(pTaskData->task);

    return 0;
}


#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
