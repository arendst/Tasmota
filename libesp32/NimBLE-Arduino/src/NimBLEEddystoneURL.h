/*
 * NimBLEEddystoneURL.h
 *
 *  Created: on March 15 2020
 *      Author H2zero
 * 
 * Originally:
 *
 * BLEEddystoneURL.h
 *
 *  Created on: Mar 12, 2018
 *      Author: pcbreflux
 */

#ifndef _NIMBLEEddystoneURL_H_
#define _NIMBLEEddystoneURL_H_
#include "NimBLEUUID.h"

#include <string>

#define EDDYSTONE_URL_FRAME_TYPE 0x10

/**
 * @brief Representation of a beacon.
 * See:
 * * https://github.com/google/eddystone
 */
class NimBLEEddystoneURL {
public:
	NimBLEEddystoneURL();
	std::string getData();
	NimBLEUUID	 getUUID();
	int8_t	  getPower();
	std::string getURL();
	std::string getDecodedURL();
	void		setData(std::string data);
	void		setUUID(NimBLEUUID l_uuid);
	void		setPower(int8_t advertisedTxPower);
	void		setURL(std::string url);

private:
	uint16_t beaconUUID;
	uint8_t  lengthURL;
	struct {
		uint8_t frameType;
		int8_t  advertisedTxPower;
		uint8_t url[16];
	} __attribute__((packed)) m_eddystoneData;

}; // NIMBLEEddystoneURL

#endif /* _NIMBLEEddystoneURL_H_ */
