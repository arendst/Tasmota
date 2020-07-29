/*
 * NimBLEBeacon2.h
 *
 *  Created: on March 15 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLEBeacon2.h
 *
 *  Created on: Jan 4, 2018
 *      Author: kolban
 */

#ifndef MAIN_NIMBLEBEACON_H_
#define MAIN_NIMBLEBEACON_H_
#include "NimBLEUUID.h"
/**
 * @brief Representation of a beacon.
 * See:
 * * https://en.wikipedia.org/wiki/IBeacon
 */
class NimBLEBeacon {
private:
	struct {
		uint16_t manufacturerId;
		uint8_t  subType;
		uint8_t  subTypeLength;
		uint8_t  proximityUUID[16];
		uint16_t major;
		uint16_t minor;
		int8_t   signalPower;
	} __attribute__((packed)) m_beaconData;
public:
	NimBLEBeacon();
	std::string getData();
	uint16_t    getMajor();
	uint16_t    getMinor();
	uint16_t    getManufacturerId();
	NimBLEUUID     getProximityUUID();
	int8_t      getSignalPower();
	void        setData(std::string data);
	void        setMajor(uint16_t major);
	void        setMinor(uint16_t minor);
	void        setManufacturerId(uint16_t manufacturerId);
	void        setProximityUUID(NimBLEUUID uuid);
	void        setSignalPower(int8_t signalPower);
}; // NimBLEBeacon

#endif /* MAIN_NIMBLEBEACON_H_ */
