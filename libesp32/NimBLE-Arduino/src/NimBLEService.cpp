/*
 * NimBLEService.cpp
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEService.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: kolban
 */

// A service is identified by a UUID.  A service is also the container for one or more characteristics.

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEService.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

#include <string>

static const char* LOG_TAG = "NimBLEService"; // Tag for logging.

#define NULL_HANDLE (0xffff)


/**
 * @brief Construct an instance of the NimBLEService
 * @param [in] uuid The UUID of the service.
 * @param [in] numHandles The maximum number of handles associated with the service.
 * @param [in] a pointer to the server instance that this service belongs to.
 */
NimBLEService::NimBLEService(const char* uuid, uint16_t numHandles, NimBLEServer* pServer)
: NimBLEService(NimBLEUUID(uuid), numHandles, pServer) {
}


/**
 * @brief Construct an instance of the BLEService
 * @param [in] uuid The UUID of the service.
 * @param [in] numHandles The maximum number of handles associated with the service.
 * @param [in] a pointer to the server instance that this service belongs to.
 */
NimBLEService::NimBLEService(const NimBLEUUID &uuid, uint16_t numHandles, NimBLEServer* pServer) {
    m_uuid       = uuid;
    m_handle     = NULL_HANDLE;
    m_pServer    = pServer;
    m_numHandles = numHandles;
} // NimBLEService


/**
 * @brief Dump details of this BLE GATT service.
 * @return N/A.
 */
void NimBLEService::dump() {
    NIMBLE_LOGD(LOG_TAG, "Service: uuid:%s, handle: 0x%2x",
        m_uuid.toString().c_str(),
        m_handle);

    std::string res;
    int count = 0;
    char hex[5];
    for (auto &it: m_chrVec) {
        if (count > 0) {res += "\n";}
        snprintf(hex, sizeof(hex), "%04x", it->getHandle());
        count++;
        res += "handle: 0x";
        res += hex;
        res += ", uuid: " + std::string(it->getUUID());
    }
    NIMBLE_LOGD(LOG_TAG, "Characteristics:\n%s", res.c_str());
} // dump


/**
 * @brief Get the UUID of the service.
 * @return the UUID of the service.
 */
NimBLEUUID NimBLEService::getUUID() {
    return m_uuid;
} // getUUID


/**
 * @brief Builds the database of characteristics/descriptors for the service
 * and registers it with the NimBLE stack.
 * @return bool success/failure .
 */

bool NimBLEService::start() {
    NIMBLE_LOGD(LOG_TAG, ">> start(): Starting service: %s", toString().c_str());
    int rc = 0;
    // Nimble requires an array of services to be sent to the api
    // Since we are adding 1 at a time we create an array of 2 and set the type
    // of the second service to 0 to indicate the end of the array.
    ble_gatt_svc_def* svc = new ble_gatt_svc_def[2];
    ble_gatt_chr_def* pChr_a = nullptr;
    ble_gatt_dsc_def* pDsc_a = nullptr;

    svc[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
    svc[0].uuid = &m_uuid.getNative()->u;
    svc[0].includes = NULL;

    size_t numChrs = m_chrVec.size();

    NIMBLE_LOGD(LOG_TAG,"Adding %d characteristics for service %s", numChrs, toString().c_str());

    if(!numChrs){
        svc[0].characteristics = NULL;
    }else{
        // Nimble requires the last characteristic to have it's uuid = 0 to indicate the end
        // of the characteristics for the service. We create 1 extra and set it to null
        // for this purpose.
        pChr_a = new ble_gatt_chr_def[numChrs+1];
        NimBLECharacteristic* pCharacteristic = *m_chrVec.begin();

        for(uint8_t i=0; i < numChrs;) {
            uint8_t numDscs = pCharacteristic->m_dscVec.size();
            if(numDscs) {
                // skip 2902 as it's automatically created by NimBLE
                // if Indicate or Notify flags are set
                if(((pCharacteristic->m_properties & BLE_GATT_CHR_F_INDICATE) ||
                    (pCharacteristic->m_properties & BLE_GATT_CHR_F_NOTIFY))  &&
                     pCharacteristic->getDescriptorByUUID("2902") != nullptr)
                {
                    numDscs--;
                }
            }

            if(!numDscs){
                pChr_a[i].descriptors = NULL;
            } else {
                // Must have last descriptor uuid = 0 so we have to create 1 extra
                //NIMBLE_LOGD(LOG_TAG, "Adding %d descriptors", numDscs);
                pDsc_a = new ble_gatt_dsc_def[numDscs+1];
                NimBLEDescriptor* pDescriptor = *pCharacteristic->m_dscVec.begin();
                for(uint8_t d=0; d < numDscs;) {
                    // skip 2902
                    if(pDescriptor->m_uuid == NimBLEUUID(uint16_t(0x2902))) {
                        //NIMBLE_LOGD(LOG_TAG, "Skipped 0x2902");
                        pDescriptor = *(pCharacteristic->m_dscVec.begin()+d+1);
                        continue;
                    }
                    pDsc_a[d].uuid = &pDescriptor->m_uuid.getNative()->u;
                    pDsc_a[d].att_flags = pDescriptor->m_properties;
                    pDsc_a[d].min_key_size = 0;
                    pDsc_a[d].access_cb = NimBLEDescriptor::handleGapEvent;
                    pDsc_a[d].arg = pDescriptor;
                    d++;
                    pDescriptor = *(pCharacteristic->m_dscVec.begin() + d);
                }

                pDsc_a[numDscs].uuid = NULL;
                pChr_a[i].descriptors = pDsc_a;
            }

            pChr_a[i].uuid = &pCharacteristic->m_uuid.getNative()->u;
            pChr_a[i].access_cb = NimBLECharacteristic::handleGapEvent;
            pChr_a[i].arg = pCharacteristic;
            pChr_a[i].flags = pCharacteristic->m_properties;
            pChr_a[i].min_key_size = 0;
            pChr_a[i].val_handle = &pCharacteristic->m_handle;
            i++;
            pCharacteristic = *(m_chrVec.begin() + i);
        }

        pChr_a[numChrs].uuid = NULL;
        svc[0].characteristics = pChr_a;
    }

    // end of services must indicate to api with type = 0
    svc[1].type = 0;

    rc = ble_gatts_count_cfg((const ble_gatt_svc_def*)svc);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_count_cfg failed, rc= %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    rc = ble_gatts_add_svcs((const ble_gatt_svc_def*)svc);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_add_svcs, rc= %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;

    }

    NIMBLE_LOGD(LOG_TAG, "<< start()");
    return true;
} // start


/**
 * @brief Get the handle associated with this service.
 * @return The handle associated with this service.
 */
uint16_t NimBLEService::getHandle() {
    return m_handle;
} // getHandle


/**
 * @brief Create a new BLE Characteristic associated with this service.
 * @param [in] uuid - The UUID of the characteristic.
 * @param [in] properties - The properties of the characteristic.
 * @return The new BLE characteristic.
 */
NimBLECharacteristic* NimBLEService::createCharacteristic(const char* uuid, uint32_t properties) {
    return createCharacteristic(NimBLEUUID(uuid), properties);
}


/**
 * @brief Create a new BLE Characteristic associated with this service.
 * @param [in] uuid - The UUID of the characteristic.
 * @param [in] properties - The properties of the characteristic.
 * @return The new BLE characteristic.
 */
NimBLECharacteristic* NimBLEService::createCharacteristic(const NimBLEUUID &uuid, uint32_t properties) {
    NimBLECharacteristic* pCharacteristic = new NimBLECharacteristic(uuid, properties, this);
    // Check that we don't add the same characteristic twice.
    if (getCharacteristic(uuid) != nullptr) {
        NIMBLE_LOGW(LOG_TAG, "<< Adding a duplicate characteristic with UUID: %s",
                             std::string(uuid).c_str());
    }

    // Remember this characteristic in our vector of characteristics.
    m_chrVec.push_back(pCharacteristic);

    return pCharacteristic;
} // createCharacteristic


NimBLECharacteristic* NimBLEService::getCharacteristic(const char* uuid) {
    return getCharacteristic(NimBLEUUID(uuid));
}


NimBLECharacteristic* NimBLEService::getCharacteristic(const NimBLEUUID &uuid) {
    for (auto &it : m_chrVec) {
        if (it->getUUID() == uuid) {
            return it;
        }
    }

    return nullptr;
}


/**
 * @brief Return a string representation of this service.
 * A service is defined by:
 * * Its UUID
 * * Its handle
 * @return A string representation of this service.
 */
std::string NimBLEService::toString() {
    std::string res = "UUID: " + getUUID().toString();
    char hex[5];
    snprintf(hex, sizeof(hex), "%04x", getHandle());
    res += ", handle: 0x";
    res += hex;
    return res;
} // toString


/**
 * @brief Get the BLE server associated with this service.
 * @return The BLEServer associated with this service.
 */
NimBLEServer* NimBLEService::getServer() {
    return m_pServer;
} // getServer

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif // CONFIG_BT_ENABLED
