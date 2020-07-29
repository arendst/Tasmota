/*
 * NimBLEService.h
 *
 *  Created: on March 2, 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLEService.h
 *
 *  Created on: Mar 25, 2017
 *      Author: kolban
 */

#ifndef MAIN_NIMBLESERVICE_H_
#define MAIN_NIMBLESERVICE_H_
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLECharacteristic.h"
#include "NimBLEServer.h"
#include "NimBLEUUID.h"
#include "FreeRTOS.h"


class NimBLEServer;
class NimBLECharacteristic;

/**
 * @brief A data mapping used to manage the set of %BLE characteristics known to the server.
 */
class NimBLECharacteristicMap {
public:
	void setByUUID(NimBLECharacteristic* pCharacteristic, const char* uuid);
	void setByUUID(NimBLECharacteristic* pCharacteristic, NimBLEUUID uuid);
	void setByHandle(uint16_t handle, NimBLECharacteristic* pCharacteristic);
	NimBLECharacteristic* getByUUID(const char* uuid);	
	NimBLECharacteristic* getByUUID(NimBLEUUID uuid);
	NimBLECharacteristic* getByHandle(uint16_t handle);
	NimBLECharacteristic* getFirst();
	NimBLECharacteristic* getNext();
	uint8_t getSize();
	std::string toString();

private:
	std::map<NimBLECharacteristic*, std::string> m_uuidMap;
	std::map<uint16_t, NimBLECharacteristic*> m_handleMap;
	std::map<NimBLECharacteristic*, std::string>::iterator m_iterator;
};


/**
 * @brief The model of a %BLE service.
 *
 */
class NimBLEService {
public:
	NimBLECharacteristic* createCharacteristic(const char* uuid, 
					    uint32_t properties = NIMBLE_PROPERTY::READ | 
											  NIMBLE_PROPERTY::WRITE);
											  
	NimBLECharacteristic* createCharacteristic(NimBLEUUID uuid,
					    uint32_t properties = NIMBLE_PROPERTY::READ | 
											  NIMBLE_PROPERTY::WRITE);
											  
	void               dump();
	NimBLECharacteristic* getCharacteristic(const char* uuid);
	NimBLECharacteristic* getCharacteristic(NimBLEUUID uuid);
	NimBLEUUID            getUUID();
	NimBLEServer*         getServer();
	bool               start();
//	void			   stop();
	std::string        toString();
	uint16_t           getHandle();
	uint8_t			   m_instId = 0;

private:
	NimBLEService(const char* uuid, uint16_t numHandles, NimBLEServer* pServer);
	NimBLEService(NimBLEUUID uuid, uint16_t numHandles, NimBLEServer* pServer);
	friend class NimBLEServer;
	friend class NimBLEDevice;
	
	void               addCharacteristic(NimBLECharacteristic* pCharacteristic);

	NimBLECharacteristicMap m_characteristicMap;
	uint16_t             m_handle;
	NimBLEServer*           m_pServer = nullptr;
	NimBLEUUID              m_uuid;

	uint16_t             m_numHandles;
	void               setHandle(uint16_t handle);
}; // BLEService


#endif // CONFIG_BT_ENABLED
#endif /* MAIN_NIMBLESERVICE_H_ */