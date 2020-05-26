/*
 * NimBLERemoteService.cpp
 *
 *  Created: on Jan 27 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLERemoteService.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)

#include "NimBLERemoteService.h"
#include "NimBLEUtils.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLERemoteService";

/**
 * @brief Remote Service constructor.
 * @param [in] Reference to the client this belongs to.
 * @param [in] Refernce to the structure with the services' information.
 */
NimBLERemoteService::NimBLERemoteService(NimBLEClient* pClient, const struct ble_gatt_svc* service) {

    NIMBLE_LOGD(LOG_TAG, ">> NimBLERemoteService()");
    m_pClient = pClient;
    switch (service->uuid.u.type) {
        case BLE_UUID_TYPE_16:
            m_uuid = NimBLEUUID(service->uuid.u16.value);
            break;
        case BLE_UUID_TYPE_32:
            m_uuid = NimBLEUUID(service->uuid.u32.value);
            break;
        case BLE_UUID_TYPE_128:
            m_uuid = NimBLEUUID(const_cast<ble_uuid128_t*>(&service->uuid.u128));
            break;
        default:
            m_uuid = nullptr;
            break;
    }
    m_startHandle = service->start_handle;
    m_endHandle = service->end_handle;
    NIMBLE_LOGD(LOG_TAG, "<< NimBLERemoteService()");
}


/**
 * @brief When deleting the service make sure we delete all characteristics and descriptors.
 * Also release any semaphores they may be holding.
 */
NimBLERemoteService::~NimBLERemoteService() {
    removeCharacteristics();
}


/**
 * @brief Get iterator to the beginning of the vector of remote characteristic pointers.
 * @return An iterator to the beginning of the vector of remote characteristic pointers.
 */
std::vector<NimBLERemoteCharacteristic*>::iterator NimBLERemoteService::begin() {
    return m_characteristicVector.begin();
}


/**
 * @brief Get iterator to the end of the vector of remote characteristic pointers.
 * @return An iterator to the end of the vector of remote characteristic pointers.
 */
std::vector<NimBLERemoteCharacteristic*>::iterator NimBLERemoteService::end() {
    return m_characteristicVector.end();
}


/**
 * @brief Get the remote characteristic object for the characteristic UUID.
 * @param [in] uuid Remote characteristic uuid.
 * @return Reference to the remote characteristic object.
 */
NimBLERemoteCharacteristic* NimBLERemoteService::getCharacteristic(const char* uuid) {
    return getCharacteristic(NimBLEUUID(uuid));
} // getCharacteristic


/**
 * @brief Get the characteristic object for the UUID.
 * @param [in] uuid Characteristic uuid.
 * @return Reference to the characteristic object, or nullptr if not found.
 */
NimBLERemoteCharacteristic* NimBLERemoteService::getCharacteristic(const NimBLEUUID &uuid) {
    for(auto &it: m_characteristicVector) {
        if(it->getUUID() == uuid) {
            return it;
        }
    }

    size_t prev_size = m_characteristicVector.size();
    if(retrieveCharacteristics(&uuid)) {
        if(m_characteristicVector.size() > prev_size) {
            return m_characteristicVector.back();
        }
    }

    return nullptr;
} // getCharacteristic


/**
 * @Get a pointer to the vector of found characteristics.
 * @param [in] bool value to indicate if the current vector should be cleared and
 * subsequently all characteristics for this service retrieved from the peripheral.
 * If false the vector will be returned with the currently stored characteristics,
 * if the vector is empty it will retrieve all characteristics of this service
 * from the peripheral.
 * @return a pointer to the vector of descriptors for this characteristic.
 */

std::vector<NimBLERemoteCharacteristic*>* NimBLERemoteService::getCharacteristics(bool refresh) {
    if(refresh) {
        removeCharacteristics();
    }

    if(m_characteristicVector.empty()) {
        if (!retrieveCharacteristics()) {
            NIMBLE_LOGE(LOG_TAG, "Error: Failed to get characteristics");
        }
        else{
            NIMBLE_LOGI(LOG_TAG, "Found %d characteristics", m_characteristicVector.size());
        }
    }
    return &m_characteristicVector;
} // getCharacteristics


/**
 * @brief Callback for Characterisic discovery.
 */
int NimBLERemoteService::characteristicDiscCB(uint16_t conn_handle,
                                const struct ble_gatt_error *error,
                                const struct ble_gatt_chr *chr, void *arg)
{
    NIMBLE_LOGD(LOG_TAG,"Characteristic Discovered >> status: %d handle: %d",
                        error->status, (error->status == 0) ? chr->val_handle : -1);

    NimBLERemoteService *service = (NimBLERemoteService*)arg;
    int rc=0;

    // Make sure the discovery is for this device
    if(service->getClient()->getConnId() != conn_handle){
        return 0;
    }

    switch (error->status) {
        case 0: {
            // Found a service - add it to the vector
            NimBLERemoteCharacteristic* pRemoteCharacteristic = new NimBLERemoteCharacteristic(service, chr);
            service->m_characteristicVector.push_back(pRemoteCharacteristic);
            break;
        }
        case BLE_HS_EDONE:{
            /** All characteristics in this service discovered; start discovering
             *  characteristics in the next service.
             */
            service->m_semaphoreGetCharEvt.give(0);
            rc = 0;
            break;
        }
        default:
            rc = error->status;
            break;
    }
    if (rc != 0) {
        /* Error; abort discovery. */
        // pass non-zero to semaphore on error to indicate an error finding characteristics
        // release memory from any characteristics we created
        //service->removeCharacteristics(); --this will now be done when we clear services on returning with error
        NIMBLE_LOGE(LOG_TAG, "characteristicDiscCB() rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        service->m_semaphoreGetCharEvt.give(1);
    }
    NIMBLE_LOGD(LOG_TAG,"<< Characteristic Discovered. status: %d", rc);
    return rc;
}


/**
 * @brief Retrieve all the characteristics for this service.
 * This function will not return until we have all the characteristics.
 * @return N/A
 */
bool NimBLERemoteService::retrieveCharacteristics(const NimBLEUUID *uuid_filter) {
    NIMBLE_LOGD(LOG_TAG, ">> retrieveCharacteristics() for service: %s", getUUID().toString().c_str());

    int rc = 0;
    //removeCharacteristics(); // Forget any previous characteristics.

    m_semaphoreGetCharEvt.take("retrieveCharacteristics");

    if(uuid_filter == nullptr) {
        rc = ble_gattc_disc_all_chrs(m_pClient->getConnId(),
                             m_startHandle,
                             m_endHandle,
                             NimBLERemoteService::characteristicDiscCB,
                             this);
    } else {
        rc = ble_gattc_disc_chrs_by_uuid(m_pClient->getConnId(),
                             m_startHandle,
                             m_endHandle,
                             &uuid_filter->getNative()->u,
                             NimBLERemoteService::characteristicDiscCB,
                             this);
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_chrs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        m_semaphoreGetCharEvt.give();
        return false;
    }

    if(m_semaphoreGetCharEvt.wait("retrieveCharacteristics") == 0){
        NIMBLE_LOGD(LOG_TAG, "<< retrieveCharacteristics()");
        return true;
    }

    NIMBLE_LOGE(LOG_TAG, "Could not retrieve characteristics");
    return false;

} // retrieveCharacteristics


/**
 * @brief Get the client associated with this service.
 * @return A reference to the client associated with this service.
 */
NimBLEClient* NimBLERemoteService::getClient() {
    return m_pClient;
} // getClient


/**
 * @brief Get the service end handle.
 */
uint16_t NimBLERemoteService::getEndHandle() {
    return m_endHandle;
} // getEndHandle


/**
 * @brief Get the service start handle.
 */
uint16_t NimBLERemoteService::getStartHandle() {
    return m_startHandle;
} // getStartHandle


/**
 * @brief Get the service UUID.
 */
NimBLEUUID NimBLERemoteService::getUUID() {
    return m_uuid;
}


/**
 * @brief Read the value of a characteristic associated with this service.
 * @param [in] characteristicUuid The characteristic to read.
 * @returns a string containing the value or an empty string if not found or error.
 */
std::string NimBLERemoteService::getValue(const NimBLEUUID &characteristicUuid) {
    NIMBLE_LOGD(LOG_TAG, ">> readValue: uuid: %s", characteristicUuid.toString().c_str());

    std::string ret = "";
    NimBLERemoteCharacteristic* pChar = getCharacteristic(characteristicUuid);

    if(pChar != nullptr) {
        ret =  pChar->readValue();
    }

    NIMBLE_LOGD(LOG_TAG, "<< readValue");
    return ret;
} // readValue


/**
 * @brief Set the value of a characteristic.
 * @param [in] characteristicUuid The characteristic to set.
 * @param [in] value The value to set.
 * @returns true on success, false if not found or error
 */
bool NimBLERemoteService::setValue(const NimBLEUUID &characteristicUuid, const std::string &value) {
    NIMBLE_LOGD(LOG_TAG, ">> setValue: uuid: %s", characteristicUuid.toString().c_str());

    bool ret = false;
    NimBLERemoteCharacteristic* pChar = getCharacteristic(characteristicUuid);

    if(pChar != nullptr) {
         ret =  pChar->writeValue(value);
    }

    NIMBLE_LOGD(LOG_TAG, "<< setValue");
    return ret;
} // setValue


/**
 * @brief Delete the characteristics in the characteristics vector.
 * We maintain a vector called m_characteristicsVector that contains pointers to BLERemoteCharacteristic
 * object references. Since we allocated these in this class, we are also responsible for deleting
 * them. This method does just that.
 * @return N/A.
 */
void NimBLERemoteService::removeCharacteristics() {
    for(auto &it: m_characteristicVector) {
        delete it;
    }
    m_characteristicVector.clear();   // Clear the vector
} // removeCharacteristics


/**
 * @brief Create a string representation of this remote service.
 * @return A string representation of this remote service.
 */
std::string NimBLERemoteService::toString() {
    std::string res = "Service: uuid: " + m_uuid.toString();
    char val[6];
    res += ", start_handle: ";
    snprintf(val, sizeof(val), "%d", m_startHandle);
    res += val;
    snprintf(val, sizeof(val), "%04x", m_startHandle);
    res += " 0x";
    res += val;
    res += ", end_handle: ";
    snprintf(val, sizeof(val), "%d", m_endHandle);
    res += val;
    snprintf(val, sizeof(val), "%04x", m_endHandle);
    res += " 0x";
    res += val;

    for (auto &it: m_characteristicVector) {
        res += "\n" + it->toString();
    }

    return res;
} // toString


/**
 * @brief called when an error occurrs and we need to release the semaphores to resume operations.
 * Will release all characteristic and subsequently all descriptor semaphores for this service.
 */
void NimBLERemoteService::releaseSemaphores() {
    for(auto &it: m_characteristicVector) {
       it->releaseSemaphores();
    }
    m_semaphoreGetCharEvt.give(1);
}

#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */
