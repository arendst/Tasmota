/*
 * NimBLEDescriptorMap.cpp
 *
 *  Created: on March 10, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEDescriptorMap.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)

#include "NimBLECharacteristic.h"
#include "NimBLEDescriptor.h"


/**
 * @brief Return the descriptor by UUID.
 * @param [in] UUID The UUID to look up the descriptor.
 * @return The descriptor.  If not present, then nullptr is returned.
 */
NimBLEDescriptor* NimBLEDescriptorMap::getByUUID(const char* uuid) {
    return getByUUID(NimBLEUUID(uuid));
}


/**
 * @brief Return the descriptor by UUID.
 * @param [in] UUID The UUID to look up the descriptor.
 * @return The descriptor.  If not present, then nullptr is returned.
 */
NimBLEDescriptor* NimBLEDescriptorMap::getByUUID(const NimBLEUUID &uuid) {
    for (auto &myPair : m_uuidMap) {
        if (myPair.first->getUUID().equals(uuid)) {
            return myPair.first;
        }
    }
    return nullptr;
} // getByUUID


/**
 * @brief Return the descriptor by handle.
 * @param [in] handle The handle to look up the descriptor.
 * @return The descriptor.
 */
 /*
NimBLEDescriptor* NimBLEDescriptorMap::getByHandle(uint16_t handle) {
    return m_handleMap.at(handle);
} // getByHandle
*/

/**
 * @brief Set the descriptor by UUID.
 * @param [in] uuid The uuid of the descriptor.
 * @param [in] characteristic The descriptor to cache.
 * @return N/A.
 */
void NimBLEDescriptorMap::setByUUID(const char* uuid, NimBLEDescriptor* pDescriptor){
    m_uuidMap.insert(std::pair<NimBLEDescriptor*, std::string>(pDescriptor, uuid));
} // setByUUID



/**
 * @brief Set the descriptor by UUID.
 * @param [in] uuid The uuid of the descriptor.
 * @param [in] characteristic The descriptor to cache.
 * @return N/A.
 */
void NimBLEDescriptorMap::setByUUID(const NimBLEUUID &uuid, NimBLEDescriptor* pDescriptor) {
    m_uuidMap.insert(std::pair<NimBLEDescriptor*, std::string>(pDescriptor, uuid.toString()));
} // setByUUID


/**
 * @brief Set the descriptor by handle.
 * @param [in] handle The handle of the descriptor.
 * @param [in] descriptor The descriptor to cache.
 * @return N/A.
 */
 /*
void NimBLEDescriptorMap::setByHandle(uint16_t handle, NimBLEDescriptor* pDescriptor) {
    m_handleMap.insert(std::pair<uint16_t, NimBLEDescriptor*>(handle, pDescriptor));
} // setByHandle
*/


/**
 * @brief Get the number of descriptors in the map.
 */
uint8_t NimBLEDescriptorMap::getSize() {
    return (uint8_t)m_uuidMap.size();
} // getSize


/**
 * @brief Return a string representation of the descriptor map.
 * @return A string representation of the descriptor map.
 */
std::string NimBLEDescriptorMap::toString() {
    std::string res;
    char hex[5];
    int count = 0;
    for (auto &myPair : m_uuidMap) {
        if (count > 0) {res += "\n";}
        snprintf(hex, sizeof(hex), "%04x", myPair.first->getHandle());
        count++;
        res += "handle: 0x";
        res += hex;
        res += ", uuid: " + myPair.first->getUUID().toString();
    }
    return res;
} // toString


/**
 * @brief Get the first descriptor in the map.
 * @return The first descriptor in the map.
 */
NimBLEDescriptor* NimBLEDescriptorMap::getFirst() {
    m_iterator = m_uuidMap.begin();
    if (m_iterator == m_uuidMap.end()) return nullptr;
    NimBLEDescriptor* pRet = m_iterator->first;
    m_iterator++;
    return pRet;
} // getFirst


/**
 * @brief Get the next descriptor in the map.
 * @return The next descriptor in the map.
 */
NimBLEDescriptor* NimBLEDescriptorMap::getNext() {
    if (m_iterator == m_uuidMap.end()) return nullptr;
    NimBLEDescriptor* pRet = m_iterator->first;
    m_iterator++;
    return pRet;
} // getNext

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
