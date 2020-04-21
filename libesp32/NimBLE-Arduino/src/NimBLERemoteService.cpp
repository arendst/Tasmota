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

    NIMBLE_LOGD(LOG_TAG, ">> BLERemoteService()");
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
    m_haveCharacteristics = false;

    NIMBLE_LOGD(LOG_TAG, "<< BLERemoteService()");
}


/**
 * @brief When deleting the service make sure we delete all characteristics and descriptors.
 * Also release any semaphores they may be holding.
 */ 
NimBLERemoteService::~NimBLERemoteService() {
    removeCharacteristics();
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
NimBLERemoteCharacteristic* NimBLERemoteService::getCharacteristic(NimBLEUUID uuid) {
    if (m_haveCharacteristics) {
        std::string v = uuid.toString();
        for (auto &myPair : m_characteristicMap) {
            if (myPair.first == v) {
                return myPair.second;
            }
        }
    }

    return nullptr;
} // getCharacteristic


/**
 * @brief Callback for Characterisic discovery.
 */
int NimBLERemoteService::characteristicDiscCB(uint16_t conn_handle, 
                                const struct ble_gatt_error *error,
                                const struct ble_gatt_chr *chr, void *arg) 
{
    NIMBLE_LOGD(LOG_TAG,"Characteristic Discovered >> status: %d handle: %d", error->status, conn_handle);
    
    NimBLERemoteService *service = (NimBLERemoteService*)arg;
    int rc=0;

    // Make sure the discovery is for this device
    if(service->getClient()->getConnId() != conn_handle){
        return 0;
    }
    
    switch (error->status) {
        case 0: {
            // Found a service - add it to the map
            NimBLERemoteCharacteristic* pRemoteCharacteristic = new NimBLERemoteCharacteristic(service, chr);
            service->m_characteristicMap.insert(std::pair<std::string, NimBLERemoteCharacteristic*>(pRemoteCharacteristic->getUUID().toString(), pRemoteCharacteristic));
            service->m_characteristicMapByHandle.insert(std::pair<uint16_t, NimBLERemoteCharacteristic*>(chr->val_handle, pRemoteCharacteristic));
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
bool NimBLERemoteService::retrieveCharacteristics() {
    NIMBLE_LOGD(LOG_TAG, ">> retrieveCharacteristics() for service: %s", getUUID().toString().c_str());
    
    int rc = 0;
    //removeCharacteristics(); // Forget any previous characteristics.
    
    m_semaphoreGetCharEvt.take("retrieveCharacteristics");
    
    rc = ble_gattc_disc_all_chrs(m_pClient->getConnId(),
                                 m_startHandle,
                                 m_endHandle,
                                 NimBLERemoteService::characteristicDiscCB,
                                 this);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gattc_disc_all_chrs: rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        m_haveCharacteristics = false;
        m_semaphoreGetCharEvt.give();
        return false;
    }
    
    m_haveCharacteristics = (m_semaphoreGetCharEvt.wait("retrieveCharacteristics") == 0);
    if(m_haveCharacteristics){
        uint16_t endHdl = 0xFFFF;
        NIMBLE_LOGD(LOG_TAG, "Found %d Characteristics", m_characteristicMapByHandle.size());
        for (auto it = m_characteristicMapByHandle.cbegin(); it != m_characteristicMapByHandle.cend(); ++it) {
            NIMBLE_LOGD(LOG_TAG, "Found UUID: %s Handle: %d Def Handle: %d", (*it).second->getUUID().toString().c_str(), (*it).second->getHandle(), (*it).second->getDefHandle());
            // The descriptor handle is between this characteristic val_handle and the next ones def_handle
            // so make the end of the scan at the handle before the next characteristic def_handle
            
            // Make sure we don't go past the service end handle
            if(++it != m_characteristicMapByHandle.cend()){
                NIMBLE_LOGD(LOG_TAG, "Next UUID: %s Handle: %d Def Handle: %d", (*it).second->getUUID().toString().c_str(), (*it).second->getHandle(),(*it).second->getDefHandle());
                
                endHdl = (*it).second->getDefHandle()-1;
            }
            else{ 
                NIMBLE_LOGD(LOG_TAG, "END CHARS");
                endHdl = m_endHandle;
            }
            --it;
            
            //If there is no handles between this characteristic and the next there is no descriptor so skip to the next
            if((*it).second->getHandle() != endHdl){
                if(!m_pClient->m_isConnected || !(*it).second->retrieveDescriptors(endHdl)) {
                    return false;
                }
            }
            //NIMBLE_LOGD(LOG_TAG, "Found %d Characteristics in service UUID: %s", chars->size(), myPair.first.c_str());
        }
        
        NIMBLE_LOGD(LOG_TAG, "<< retrieveCharacteristics()");
        return true;
    }
    
    NIMBLE_LOGE(LOG_TAG, "Could not retrieve characteristics");
    return false;
    
} // retrieveCharacteristics


/**
 * @brief Retrieve a map of all the characteristics of this service.
 * @return A map of all the characteristics of this service.
 */
std::map<std::string, NimBLERemoteCharacteristic*>* NimBLERemoteService::getCharacteristics() {
    return &m_characteristicMap;
} // getCharacteristics


/**
 * @brief Retrieve a map of all the characteristics of this service.
 * @return A map of all the characteristics of this service.
 */
std::map<uint16_t, NimBLERemoteCharacteristic*>* NimBLERemoteService::getCharacteristicsByHandle() {
    return &m_characteristicMapByHandle;
} // getCharacteristicsByHandle


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
std::string NimBLERemoteService::getValue(NimBLEUUID characteristicUuid) {
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
bool NimBLERemoteService::setValue(NimBLEUUID characteristicUuid, std::string value) {
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
 * @brief Delete the characteristics in the characteristics map.
 * We maintain a map called m_characteristicsMap that contains pointers to BLERemoteCharacteristic
 * object references.  Since we allocated these in this class, we are also responsible for deleteing
 * them.  This method does just that.
 * @return N/A.
 */
void NimBLERemoteService::removeCharacteristics() {
    m_characteristicMap.clear();   // Clear the map
    
    for (auto &myPair : m_characteristicMapByHandle) {
       delete myPair.second;
    }
    m_characteristicMapByHandle.clear();   // Clear the map
    
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
    
    for (auto &myPair : m_characteristicMap) {
        res += "\n" + myPair.second->toString();
    }

    return res;
} // toString


/**
 * @brief called when an error occurrs and we need to release the semaphores to resume operations.
 * Will release all characteristic and subsequently all descriptor semaphores for this service. 
 */
void NimBLERemoteService::releaseSemaphores() {
    for (auto &cPair : m_characteristicMapByHandle) {
       cPair.second->releaseSemaphores();
    }
    m_semaphoreGetCharEvt.give(1);
}

#endif /* CONFIG_BT_ENABLED */
