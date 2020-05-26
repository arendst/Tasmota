/*
 * NimBLEService.cpp
 *
 *  Created: on March 7, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEServiceMap.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLEService.h"


/**
 * @brief Return the service by UUID.
 * @param [in] UUID The UUID to look up the service.
 * @return The characteristic.
 */
NimBLEService* NimBLEServiceMap::getByUUID(const char* uuid) {
    return getByUUID(NimBLEUUID(uuid));
}

/**
 * @brief Return the service by UUID.
 * @param [in] UUID The UUID to look up the service.
 * @return The characteristic.
 */
NimBLEService* NimBLEServiceMap::getByUUID(const NimBLEUUID &uuid, uint8_t inst_id) {
    for (auto &myPair : m_uuidMap) {
        if (myPair.first->getUUID().equals(uuid)) {
            return myPair.first;
        }
    }
    //return m_uuidMap.at(uuid.toString());
    return nullptr;
} // getByUUID


/**
 * @brief Return the service by handle.
 * @param [in] handle The handle to look up the service.
 * @return The service.
 */
/*
NimBLEService* NimBLEServiceMap::getByHandle(uint16_t handle) {
    return m_handleMap.at(handle);
} // getByHandle
*/

/**
 * @brief Set the service by UUID.
 * @param [in] uuid The uuid of the service.
 * @param [in] characteristic The service to cache.
 * @return N/A.
 */
void NimBLEServiceMap::setByUUID(const NimBLEUUID &uuid, NimBLEService* service) {
    m_uuidMap.insert(std::pair<NimBLEService*, std::string>(service, uuid.toString()));
} // setByUUID


/**
 * @brief Set the service by handle.
 * @param [in] handle The handle of the service.
 * @param [in] service The service to cache.
 * @return N/A.
 */
 /*
void NimBLEServiceMap::setByHandle(uint16_t handle, NimBLEService* service) {
    m_handleMap.insert(std::pair<uint16_t, NimBLEService*>(handle, service));
} // setByHandle
*/

/**
 * @brief Return a string representation of the service map.
 * @return A string representation of the service map.
 */
std::string NimBLEServiceMap::toString() {
    std::string res;
    //char hex[5];
    for (auto &myPair: m_uuidMap) {
    //  res += "handle: 0x";
    //  snprintf(hex, sizeof(hex), "%04x", myPair.first);
    //  res += hex;
        res += ", uuid: " + myPair.second + "\n";
    }
    return res;
} // toString


/**
 * @brief Get the first service in the map.
 * @return The first service in the map.
 */
NimBLEService* NimBLEServiceMap::getFirst() {
    m_iterator = m_uuidMap.begin();
    if (m_iterator == m_uuidMap.end()) return nullptr;
    NimBLEService* pRet = m_iterator->first;
    m_iterator++;
    return pRet;
} // getFirst


/**
 * @brief Get the next service in the map.
 * @return The next service in the map.
 */
NimBLEService* NimBLEServiceMap::getNext() {
    if (m_iterator == m_uuidMap.end()) return nullptr;
    NimBLEService* pRet = m_iterator->first;
    m_iterator++;
    return pRet;
} // getNext


/**
 * @brief Removes service from maps.
 * @return N/A.
 */
void NimBLEServiceMap::removeService(NimBLEService* service) {
    //m_handleMap.erase(service->getHandle());
    m_uuidMap.erase(service);
} // removeService


/**
 * @brief Returns the amount of registered services
 * @return amount of registered services
 */
int NimBLEServiceMap::getRegisteredServiceCount(){
    //return m_handleMap.size();
    return m_uuidMap.size();
}


#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
