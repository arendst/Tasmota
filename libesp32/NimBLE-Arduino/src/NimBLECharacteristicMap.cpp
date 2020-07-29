/*
 * NimBLECharacteristicMap.cpp
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * BLECharacteristicMap.cpp
 *
 *  Created on: Jun 22, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEService.h"
#include "NimBLELog.h"


/**
 * @brief Return the characteristic by handle.
 * @param [in] handle The handle to look up the characteristic.
 * @return The characteristic.
 */ 
NimBLECharacteristic* NimBLECharacteristicMap::getByHandle(uint16_t handle) {
	return m_handleMap.at(handle);
} // getByHandle


/**
 * @brief Return the characteristic by UUID.
 * @param [in] UUID The UUID to look up the characteristic.
 * @return The characteristic.
 */
NimBLECharacteristic* NimBLECharacteristicMap::getByUUID(const char* uuid) {
    return getByUUID(NimBLEUUID(uuid));
}


/**
 * @brief Return the characteristic by UUID.
 * @param [in] UUID The UUID to look up the characteristic.
 * @return The characteristic.
 */
NimBLECharacteristic* NimBLECharacteristicMap::getByUUID(NimBLEUUID uuid) {
	for (auto &myPair : m_uuidMap) {
		if (myPair.first->getUUID().equals(uuid)) {
			return myPair.first;
		}
	}

	return nullptr;
} // getByUUID

/**
 * @brief Get the number of characteristics in the map.
 */
uint8_t NimBLECharacteristicMap::getSize() {
	return (uint8_t)m_uuidMap.size();
} // getSize

/**
 * @brief Get the first characteristic in the map.
 * @return The first characteristic in the map.
 */
NimBLECharacteristic* NimBLECharacteristicMap::getFirst() {
	m_iterator = m_uuidMap.begin();
	if (m_iterator == m_uuidMap.end()) return nullptr;
	NimBLECharacteristic* pRet = m_iterator->first;
	m_iterator++;
	return pRet;
} // getFirst


/**
 * @brief Get the next characteristic in the map.
 * @return The next characteristic in the map.
 */
NimBLECharacteristic* NimBLECharacteristicMap::getNext() {
	if (m_iterator == m_uuidMap.end()) return nullptr;
	NimBLECharacteristic* pRet = m_iterator->first;
	m_iterator++;
	return pRet;
} // getNext


/**
 * @brief Set the characteristic by handle.
 * @param [in] handle The handle of the characteristic.
 * @param [in] characteristic The characteristic to cache.
 * @return N/A.
 */
void NimBLECharacteristicMap::setByHandle(uint16_t handle, NimBLECharacteristic* characteristic) {
	m_handleMap.insert(std::pair<uint16_t, NimBLECharacteristic*>(handle, characteristic));
} // setByHandle


/**
 * @brief Set the characteristic by UUID.
 * @param [in] uuid The uuid of the characteristic.
 * @param [in] characteristic The characteristic to cache.
 * @return N/A.
 */
void NimBLECharacteristicMap::setByUUID(NimBLECharacteristic* pCharacteristic, NimBLEUUID uuid) {
	m_uuidMap.insert(std::pair<NimBLECharacteristic*, std::string>(pCharacteristic, uuid.toString()));
} // setByUUID


/**
 * @brief Return a string representation of the characteristic map.
 * @return A string representation of the characteristic map.
 */
std::string NimBLECharacteristicMap::toString() {
	std::string res;
	int count = 0;
	char hex[5];
	for (auto &myPair: m_uuidMap) {
		if (count > 0) {res += "\n";}
		snprintf(hex, sizeof(hex), "%04x", myPair.first->getHandle());
		count++;
		res += "handle: 0x";
		res += hex;
		res += ", uuid: " + myPair.first->getUUID().toString();
	}
	return res;
} // toString


#endif /* CONFIG_BT_ENABLED */