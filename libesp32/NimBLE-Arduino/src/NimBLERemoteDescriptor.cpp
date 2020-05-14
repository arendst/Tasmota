/*
 * NimBLERemoteDescriptor.cpp
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLERemoteDescriptor.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLERemoteDescriptor.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLERemoteDescriptor";

/**
 * @brief Remote descriptor constructor.
 * @param [in] Reference to the Characteristic that this belongs to.
 * @param [in] Reference to the struct that contains the descriptor information.
 */
NimBLERemoteDescriptor::NimBLERemoteDescriptor(NimBLERemoteCharacteristic* pRemoteCharacteristic, 
                                                const struct ble_gatt_dsc *dsc)
{
    switch (dsc->uuid.u.type) {
        case BLE_UUID_TYPE_16: 
            m_uuid = NimBLEUUID(dsc->uuid.u16.value);
            break;
        case BLE_UUID_TYPE_32: 
            m_uuid = NimBLEUUID(dsc->uuid.u32.value);
            break;
        case BLE_UUID_TYPE_128: 
            m_uuid = NimBLEUUID(const_cast<ble_uuid128_t*>(&dsc->uuid.u128));
            break;
        default:
            m_uuid = nullptr;
            break;
    }
    m_handle                = dsc->handle;
    m_pRemoteCharacteristic = pRemoteCharacteristic;
    
}


/**
 * @brief Retrieve the handle associated with this remote descriptor.
 * @return The handle associated with this remote descriptor.
 */
uint16_t NimBLERemoteDescriptor::getHandle() {
    return m_handle;
} // getHandle


/**
 * @brief Get the characteristic that owns this descriptor.
 * @return The characteristic that owns this descriptor.
 */
NimBLERemoteCharacteristic* NimBLERemoteDescriptor::getRemoteCharacteristic() {
    return m_pRemoteCharacteristic;
} // getRemoteCharacteristic


/**
 * @brief Retrieve the UUID associated this remote descriptor.
 * @return The UUID associated this remote descriptor.
 */
NimBLEUUID NimBLERemoteDescriptor::getUUID() {
    return m_uuid;
} // getUUID


/**
 * @brief Callback for Descriptor read operation.
 * @return 0 or error code.
 */
int NimBLERemoteDescriptor::onReadCB(uint16_t conn_handle,
                const struct ble_gatt_error *error,
                struct ble_gatt_attr *attr, void *arg) 
{
    NimBLERemoteDescriptor* desc = (NimBLERemoteDescriptor*)arg;
    
        // Make sure the discovery is for this device
    if(desc->getRemoteCharacteristic()->getRemoteService()->getClient()->getConnId() != conn_handle){
        return 0;
    }
    
    NIMBLE_LOGD(LOG_TAG, "Read complete; status=%d conn_handle=%d", error->status, conn_handle);
    
    if (error->status == 0) {       
        desc->m_value = std::string((char*) attr->om->om_data, attr->om->om_len);
        desc->m_semaphoreReadDescrEvt.give(0);
    } else {
        desc->m_value = "";
        desc->m_semaphoreReadDescrEvt.give(error->status);
    }
    
    return 0;
}


std::string NimBLERemoteDescriptor::readValue() {
    NIMBLE_LOGD(LOG_TAG, ">> Descriptor readValue: %s", toString().c_str());
    
    NimBLEClient* pClient = getRemoteCharacteristic()->getRemoteService()->getClient();
    
    int rc = 0;
    int retryCount = 1;
    
    // Check to see that we are connected.
    if (!pClient->isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Disconnected");
        return "";
    }
    
    do {
        m_semaphoreReadDescrEvt.take("ReadDescriptor");
        
        rc = ble_gattc_read(pClient->getConnId(), m_handle,
                        NimBLERemoteDescriptor::onReadCB, this);
                        
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Descriptor read failed, code: %d", rc);
            m_semaphoreReadDescrEvt.give();
            return "";
        }
        
        rc = m_semaphoreReadDescrEvt.wait("ReadDescriptor");

        switch(rc){
            case 0:
                break;
    
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHEN):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHOR):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_ENC):
                if (retryCount && pClient->secureConnection())
                    break;
            /* Else falls through. */      
            default:
                return "";
        }
    } while(rc != 0 && retryCount--);

    NIMBLE_LOGD(LOG_TAG, "<< Descriptor readValue(): length: %d, rc: %d", m_value.length(), rc);
    
    return (rc == 0) ? m_value : "";
} // readValue


uint8_t NimBLERemoteDescriptor::readUInt8() {
    std::string value = readValue();
    if (value.length() >= 1) {
        return (uint8_t) value[0];
    }
    return 0;
} // readUInt8


uint16_t NimBLERemoteDescriptor::readUInt16() {
    std::string value = readValue();
    if (value.length() >= 2) {
        return *(uint16_t*) value.data();
    }
    return 0;
} // readUInt16


uint32_t NimBLERemoteDescriptor::readUInt32() {
    std::string value = readValue();
    if (value.length() >= 4) {
        return *(uint32_t*) value.data();
    }
    return 0;
} // readUInt32


/**
 * @brief Return a string representation of this BLE Remote Descriptor.
 * @retun A string representation of this BLE Remote Descriptor.
 */
std::string NimBLERemoteDescriptor::toString() {
    std::string res = "Descriptor: uuid: " + getUUID().toString();
    char val[6];
    res += ", handle: ";
    snprintf(val, sizeof(val), "%d", getHandle());
    res += val;
    
    return res;
} // toString


/**
 * @brief Callback for descriptor write operation.
 * @return 0 or error code.
 */
int NimBLERemoteDescriptor::onWriteCB(uint16_t conn_handle,
                const struct ble_gatt_error *error,
                struct ble_gatt_attr *attr, void *arg) 
{
    NimBLERemoteDescriptor* descriptor = (NimBLERemoteDescriptor*)arg;
    
        // Make sure the discovery is for this device
    if(descriptor->getRemoteCharacteristic()->getRemoteService()->getClient()->getConnId() != conn_handle){
        return 0;
    }
    
    NIMBLE_LOGD(LOG_TAG, "Write complete; status=%d conn_handle=%d", error->status, conn_handle);
    
    if (error->status == 0) {       
        descriptor->m_semaphoreDescWrite.give(0);
    } else {
        descriptor->m_semaphoreDescWrite.give(error->status);
    }
    
    return 0;
}


/**
 * @brief Write data to the BLE Remote Descriptor.
 * @param [in] data The data to send to the remote descriptor.
 * @param [in] length The length of the data to send.
 * @param [in] response True if we expect a response.
 */
bool NimBLERemoteDescriptor::writeValue(uint8_t* data, size_t length, bool response) {

    NIMBLE_LOGD(LOG_TAG, ">> Descriptor writeValue: %s", toString().c_str());
    
    NimBLEClient* pClient = getRemoteCharacteristic()->getRemoteService()->getClient();
    
    int rc = 0;
    int retryCount = 1;
    
    // Check to see that we are connected.
    if (!pClient->isConnected()) {
        NIMBLE_LOGE(LOG_TAG, "Disconnected");
        return false;
    }
    
    if(!response) {
        rc =  ble_gattc_write_no_rsp_flat(pClient->getConnId(), m_handle, data, length);
        return (rc==0);
    }
    
    do {
        m_semaphoreDescWrite.take("WriteDescriptor");
        
        rc = ble_gattc_write_flat(pClient->getConnId(), m_handle,
                                  data, length, 
                                  NimBLERemoteDescriptor::onWriteCB, 
                                  this);
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to write descriptor; rc=%d", rc);
            m_semaphoreDescWrite.give();
            return false;
        }
        
        rc = m_semaphoreDescWrite.wait("WriteDescriptor");

        switch(rc){
            case 0:
                break;
     
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHEN):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_AUTHOR):
            case BLE_HS_ATT_ERR(BLE_ATT_ERR_INSUFFICIENT_ENC):
                if (retryCount && pClient->secureConnection())
                    break;
            /* Else falls through. */                 
            default:
                return false;
        }
    } while(rc != 0 && retryCount--);

    NIMBLE_LOGD(LOG_TAG, "<< Descriptor writeValue, rc: %d",rc);
    return (rc == 0); //true;
} // writeValue


/**
 * @brief Write data represented as a string to the BLE Remote Descriptor.
 * @param [in] newValue The data to send to the remote descriptor.
 * @param [in] response True if we expect a response.
 */
bool NimBLERemoteDescriptor::writeValue(std::string newValue, bool response) {
    return writeValue((uint8_t*) newValue.data(), newValue.length(), response);
} // writeValue


/**
 * @brief Write a byte value to the Descriptor.
 * @param [in] The single byte to write.
 * @param [in] True if we expect a response.
 */
bool NimBLERemoteDescriptor::writeValue(uint8_t newValue, bool response) {
    return writeValue(&newValue, 1, response);
} // writeValue


/**
 * @brief In the event of an error this is called to make sure we don't block.
 */
void NimBLERemoteDescriptor::releaseSemaphores() {
    m_semaphoreDescWrite.give(1);
    m_semaphoreReadDescrEvt.give(1);
}
#endif /* CONFIG_BT_ENABLED */
