/*
 * NimBLEEddystoneTLM.h
 *
 *  Created: on March 15 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEEddystoneTLM.h
 *
 *  Created on: Mar 12, 2018
 *      Author: pcbreflux
 */

#ifndef _NimBLEEddystoneTLM_H_
#define _NimBLEEddystoneTLM_H_

#include "NimBLEUUID.h"

#include <string>

#define EDDYSTONE_TLM_FRAME_TYPE 0x20

/**
 * @brief Representation of a beacon.
 * See:
 * * https://github.com/google/eddystone
 */
class NimBLEEddystoneTLM {
public:
    NimBLEEddystoneTLM();
    std::string getData();
    NimBLEUUID   getUUID();
    uint8_t  getVersion();
    uint16_t    getVolt();
    float      getTemp();
    uint32_t    getCount();
    uint32_t    getTime();
    std::string toString();
    void        setData(const std::string &data);
    void        setUUID(const NimBLEUUID &l_uuid);
    void        setVersion(uint8_t version);
    void        setVolt(uint16_t volt);
    void        setTemp(float temp);
    void        setCount(uint32_t advCount);
    void        setTime(uint32_t tmil);

private:
    uint16_t beaconUUID;
    struct {
        uint8_t frameType;
        uint8_t version;
        uint16_t volt;
        uint16_t temp;
        uint32_t advCount;
        uint32_t tmil;
    } __attribute__((packed)) m_eddystoneData;

}; // NimBLEEddystoneTLM

#endif /* _NimBLEEddystoneTLM_H_ */
