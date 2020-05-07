/*
 * NimBLEBeacon2.cpp
 *
 *  Created: on March 15 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLEBeacon.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)
#include <string.h>
#include "NimBLEBeacon.h"
#include "NimBLELog.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))

static const char* LOG_TAG = "NimBLEBeacon";

NimBLEBeacon::NimBLEBeacon() {
	m_beaconData.manufacturerId = 0x4c00;
	m_beaconData.subType        = 0x02;
	m_beaconData.subTypeLength  = 0x15;
	m_beaconData.major          = 0;
	m_beaconData.minor          = 0;
	m_beaconData.signalPower    = 0;
	memset(m_beaconData.proximityUUID, 0, sizeof(m_beaconData.proximityUUID));
} // NimBLEBeacon

std::string NimBLEBeacon::getData() {
	return std::string((char*) &m_beaconData, sizeof(m_beaconData));
} // getData

uint16_t NimBLEBeacon::getMajor() {
	return m_beaconData.major;
}

uint16_t NimBLEBeacon::getManufacturerId() {
	return m_beaconData.manufacturerId;
}

uint16_t NimBLEBeacon::getMinor() {
	return m_beaconData.minor;
}

NimBLEUUID NimBLEBeacon::getProximityUUID() {
	return NimBLEUUID(m_beaconData.proximityUUID, 16, false);
}

int8_t NimBLEBeacon::getSignalPower() {
	return m_beaconData.signalPower;
}

/**
 * Set the raw data for the beacon record.
 */
void NimBLEBeacon::setData(std::string data) {
	if (data.length() != sizeof(m_beaconData)) {
		NIMBLE_LOGE(LOG_TAG, "Unable to set the data ... length passed in was %d and expected %d", 
                                                        data.length(), sizeof(m_beaconData));
		return;
	}
	memcpy(&m_beaconData, data.data(), sizeof(m_beaconData));
} // setData

void NimBLEBeacon::setMajor(uint16_t major) {
	m_beaconData.major = ENDIAN_CHANGE_U16(major);
} // setMajor

void NimBLEBeacon::setManufacturerId(uint16_t manufacturerId) {
	m_beaconData.manufacturerId = ENDIAN_CHANGE_U16(manufacturerId);
} // setManufacturerId

void NimBLEBeacon::setMinor(uint16_t minor) {
	m_beaconData.minor = ENDIAN_CHANGE_U16(minor);
} // setMinior

void NimBLEBeacon::setProximityUUID(NimBLEUUID uuid) {
	uuid = uuid.to128();
	memcpy(m_beaconData.proximityUUID, uuid.getNative()->u128.value, 16);
} // setProximityUUID

void NimBLEBeacon::setSignalPower(int8_t signalPower) {
	m_beaconData.signalPower = signalPower;
} // setSignalPower


#endif
