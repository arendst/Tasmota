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

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEDevice.h"
#include "NimBLEService.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

#include <string>

static const char* LOG_TAG = "NimBLEService"; // Tag for logging.

#define NULL_HANDLE (0xffff)


/**
 * @brief Construct an instance of the NimBLEService
 * @param [in] uuid The UUID of the service.
 */
NimBLEService::NimBLEService(const char* uuid)
: NimBLEService(NimBLEUUID(uuid)) {
}


/**
 * @brief Construct an instance of the BLEService
 * @param [in] uuid The UUID of the service.
 */
NimBLEService::NimBLEService(const NimBLEUUID &uuid) {
    m_uuid         = uuid;
    m_handle       = NULL_HANDLE;
    m_pSvcDef      = nullptr;
    m_removed      = 0;

} // NimBLEService


NimBLEService::~NimBLEService() {
    if(m_pSvcDef != nullptr) {
        if(m_pSvcDef->characteristics != nullptr) {
            for(int i=0; m_pSvcDef->characteristics[i].uuid != NULL; ++i) {
                if(m_pSvcDef->characteristics[i].descriptors) {
                    delete(m_pSvcDef->characteristics[i].descriptors);
                }
            }
            delete(m_pSvcDef->characteristics);
        }

        delete(m_pSvcDef);
    }

    for(auto &it : m_chrVec) {
        delete it;
    }
}

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

    // Rebuild the service definition if the server attributes have changed.
    if(getServer()->m_svcChanged && m_pSvcDef != nullptr) {
        delete(m_pSvcDef);
        m_pSvcDef = nullptr;
    }

    if(m_pSvcDef == nullptr) {
        // Nimble requires an array of services to be sent to the api
        // Since we are adding 1 at a time we create an array of 2 and set the type
        // of the second service to 0 to indicate the end of the array.
        ble_gatt_svc_def* svc = new ble_gatt_svc_def[2];
        ble_gatt_chr_def* pChr_a = nullptr;
        ble_gatt_dsc_def* pDsc_a = nullptr;

        svc[0].type = BLE_GATT_SVC_TYPE_PRIMARY;
        svc[0].uuid = &m_uuid.getNative()->u;
        svc[0].includes = NULL;

        int removedCount = 0;
        for(auto it = m_chrVec.begin(); it != m_chrVec.end(); ) {
            if ((*it)->m_removed > 0) {
                if ((*it)->m_removed == NIMBLE_ATT_REMOVE_DELETE) {
                    delete *it;
                    it = m_chrVec.erase(it);
                } else {
                    ++removedCount;
                    ++it;
                }
                continue;
            }

            ++it;
        }

        size_t numChrs = m_chrVec.size() - removedCount;
        NIMBLE_LOGD(LOG_TAG,"Adding %d characteristics for service %s", numChrs, toString().c_str());

        if(!numChrs){
            svc[0].characteristics = NULL;
        }else{
            // Nimble requires the last characteristic to have it's uuid = 0 to indicate the end
            // of the characteristics for the service. We create 1 extra and set it to null
            // for this purpose.
            pChr_a = new ble_gatt_chr_def[numChrs + 1];
            uint8_t i = 0;
            for(auto chr_it = m_chrVec.begin(); chr_it != m_chrVec.end(); ++chr_it) {
                if((*chr_it)->m_removed > 0) {
                    continue;
                }

                removedCount = 0;
                for(auto it = (*chr_it)->m_dscVec.begin(); it != (*chr_it)->m_dscVec.end(); ) {
                    if ((*it)->m_removed > 0) {
                        if ((*it)->m_removed == NIMBLE_ATT_REMOVE_DELETE) {
                            delete *it;
                            it = (*chr_it)->m_dscVec.erase(it);
                        } else {
                            ++removedCount;
                            ++it;
                        }
                        continue;
                    }

                    ++it;
                }

                size_t numDscs = (*chr_it)->m_dscVec.size() - removedCount;

                if(!numDscs){
                    pChr_a[i].descriptors = NULL;
                } else {
                    // Must have last descriptor uuid = 0 so we have to create 1 extra
                    pDsc_a = new ble_gatt_dsc_def[numDscs+1];
                    uint8_t d = 0;
                    for(auto dsc_it = (*chr_it)->m_dscVec.begin(); dsc_it != (*chr_it)->m_dscVec.end(); ++dsc_it ) {
                        if((*dsc_it)->m_removed > 0) {
                            continue;
                        }
                        pDsc_a[d].uuid = &(*dsc_it)->m_uuid.getNative()->u;
                        pDsc_a[d].att_flags = (*dsc_it)->m_properties;
                        pDsc_a[d].min_key_size = 0;
                        pDsc_a[d].access_cb = NimBLEDescriptor::handleGapEvent;
                        pDsc_a[d].arg = (*dsc_it);
                        ++d;
                    }

                    pDsc_a[numDscs].uuid = NULL;
                    pChr_a[i].descriptors = pDsc_a;
                }

                pChr_a[i].uuid = &(*chr_it)->m_uuid.getNative()->u;
                pChr_a[i].access_cb = NimBLECharacteristic::handleGapEvent;
                pChr_a[i].arg = (*chr_it);
                pChr_a[i].flags = (*chr_it)->m_properties;
                pChr_a[i].min_key_size = 0;
                pChr_a[i].val_handle = &(*chr_it)->m_handle;
                ++i;
            }

            pChr_a[numChrs].uuid = NULL;
            svc[0].characteristics = pChr_a;
        }

        // end of services must indicate to api with type = 0
        svc[1].type = 0;
        m_pSvcDef = svc;
    }

    int rc = ble_gatts_count_cfg((const ble_gatt_svc_def*)m_pSvcDef);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gatts_count_cfg failed, rc= %d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    rc = ble_gatts_add_svcs((const ble_gatt_svc_def*)m_pSvcDef);
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

    if (getCharacteristic(uuid) != nullptr) {
        NIMBLE_LOGD(LOG_TAG, "<< Adding a duplicate characteristic with UUID: %s",
                             std::string(uuid).c_str());
    }

    addCharacteristic(pCharacteristic);
    return pCharacteristic;
} // createCharacteristic


/**
 * @brief Add a characteristic to the service.
 * @param[in] pCharacteristic A pointer to the characteristic instance to add to the service.
 */
void NimBLEService::addCharacteristic(NimBLECharacteristic* pCharacteristic) {
    bool foundRemoved = false;

    if(pCharacteristic->m_removed > 0) {
        for(auto& it : m_chrVec) {
            if(it == pCharacteristic) {
                foundRemoved = true;
                pCharacteristic->m_removed = 0;
            }
        }
    }

    if(!foundRemoved) {
        m_chrVec.push_back(pCharacteristic);
    }

    pCharacteristic->setService(this);
    getServer()->serviceChanged();
} // addCharacteristic


/**
 * @brief Remove a characteristic from the service.
 * @param[in] pCharacteristic A pointer to the characteristic instance to remove from the service.
 * @param[in] deleteChr If true it will delete the characteristic instance and free it's resources.
 */
void NimBLEService::removeCharacteristic(NimBLECharacteristic* pCharacteristic, bool deleteChr) {
    // Check if the characteristic was already removed and if so, check if this
    // is being called to delete the object and do so if requested.
    // Otherwise, ignore the call and return.
    if(pCharacteristic->m_removed > 0) {
        if(deleteChr) {
            for(auto it = m_chrVec.begin(); it != m_chrVec.end(); ++it) {
                if ((*it) == pCharacteristic) {
                    m_chrVec.erase(it);
                    delete *it;
                    break;
                }
            }
        }

        return;
    }

    pCharacteristic->m_removed = deleteChr ? NIMBLE_ATT_REMOVE_DELETE : NIMBLE_ATT_REMOVE_HIDE;
    getServer()->serviceChanged();
} // removeCharacteristic


/**
 * @brief Get a pointer to the characteristic object with the specified UUID.
 * @param [in] uuid The UUID of the characteristic.
 * @param instanceId The index of the characteristic to return (used when multiple characteristics have the same UUID).
 * @return A pointer to the characteristic object or nullptr if not found.
 */
NimBLECharacteristic* NimBLEService::getCharacteristic(const char* uuid, uint16_t instanceId) {
    return getCharacteristic(NimBLEUUID(uuid), instanceId);
}

/**
 * @brief Get a pointer to the characteristic object with the specified UUID.
 * @param [in] uuid The UUID of the characteristic.
 * @param instanceId The index of the characteristic to return (used when multiple characteristics have the same UUID).
 * @return A pointer to the characteristic object or nullptr if not found.
 */
NimBLECharacteristic* NimBLEService::getCharacteristic(const NimBLEUUID &uuid, uint16_t instanceId) {
    uint16_t position = 0;
    for (auto &it : m_chrVec) {
        if (it->getUUID() == uuid) {
            if (position == instanceId) {
                return it;
            }
            position++;
        }
    }
    return nullptr;
}

/**
 * @brief Get a pointer to the characteristic object with the specified handle.
 * @param handle The handle of the characteristic.
 * @return A pointer to the characteristic object or nullptr if not found.
 */
NimBLECharacteristic *NimBLEService::getCharacteristicByHandle(uint16_t handle) {
    for (auto &it : m_chrVec) {
        if (it->getHandle() == handle) {
            return it;
        }
    }
    return nullptr;
}

/**
 * @return A vector containing pointers to each characteristic associated with this service.
 */
std::vector<NimBLECharacteristic *> NimBLEService::getCharacteristics() {
    return m_chrVec;
}

/**
 * @return A vector containing pointers to each characteristic with the provided UUID associated with this service.
 */
std::vector<NimBLECharacteristic *> NimBLEService::getCharacteristics(const char *uuid) {
    return getCharacteristics(NimBLEUUID(uuid));
}

/**
 * @return A vector containing pointers to each characteristic with the provided UUID associated with this service.
 */
std::vector<NimBLECharacteristic *> NimBLEService::getCharacteristics(const NimBLEUUID &uuid) {
    std::vector<NimBLECharacteristic*> result;
    for (auto &it : m_chrVec) {
        if (it->getUUID() == uuid) {
            result.push_back(it);
        }
    }
    return result;
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
    return NimBLEDevice::getServer();
}// getServer

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_PERIPHERAL */
