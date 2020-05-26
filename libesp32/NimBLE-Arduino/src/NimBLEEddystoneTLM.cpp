/*
 * NimBLEEddystoneTLM.cpp
 *
 *  Created: on March 15 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEEddystoneTLM.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: pcbreflux
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEEddystoneTLM.h"
#include "NimBLELog.h"

#include <cstring>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))
#define ENDIAN_CHANGE_U32(x) ((((x)&0xFF000000)>>24) + (((x)&0x00FF0000)>>8)) + ((((x)&0xFF00)<<8) + (((x)&0xFF)<<24))

static const char LOG_TAG[] = "NimBLEEddystoneTLM";


NimBLEEddystoneTLM::NimBLEEddystoneTLM() {
    beaconUUID = 0xFEAA;
    m_eddystoneData.frameType = EDDYSTONE_TLM_FRAME_TYPE;
    m_eddystoneData.version = 0;
    m_eddystoneData.volt = 3300; // 3300mV = 3.3V
    m_eddystoneData.temp = (uint16_t) ((float) 23.00 * 256); // 8.8 fixed format
    m_eddystoneData.advCount = 0;
    m_eddystoneData.tmil = 0;
} // NimBLEEddystoneTLM

std::string NimBLEEddystoneTLM::getData() {
    return std::string((char*) &m_eddystoneData, sizeof(m_eddystoneData));
} // getData

NimBLEUUID NimBLEEddystoneTLM::getUUID() {
    return NimBLEUUID(beaconUUID);
} // getUUID

uint8_t NimBLEEddystoneTLM::getVersion() {
    return m_eddystoneData.version;
} // getVersion

uint16_t NimBLEEddystoneTLM::getVolt() {
    return ENDIAN_CHANGE_U16(m_eddystoneData.volt);
} // getVolt

float NimBLEEddystoneTLM::getTemp() {
    return ENDIAN_CHANGE_U16(m_eddystoneData.temp) / 256.0f;
} // getTemp

uint32_t NimBLEEddystoneTLM::getCount() {
    return ENDIAN_CHANGE_U32(m_eddystoneData.advCount);
} // getCount

uint32_t NimBLEEddystoneTLM::getTime() {
    return (ENDIAN_CHANGE_U32(m_eddystoneData.tmil)) / 10;
} // getTime

std::string NimBLEEddystoneTLM::toString() {
  std::string out = "";
  uint32_t rawsec = ENDIAN_CHANGE_U32(m_eddystoneData.tmil);
  char val[12];

  out += "Version "; // + std::string(m_eddystoneData.version);
  snprintf(val, sizeof(val), "%d", m_eddystoneData.version);
  out += val;
  out += "\n";
  out += "Battery Voltage "; // + ENDIAN_CHANGE_U16(m_eddystoneData.volt);
  snprintf(val, sizeof(val), "%d", ENDIAN_CHANGE_U16(m_eddystoneData.volt));
  out += val;
  out += " mV\n";

  out += "Temperature ";
  snprintf(val, sizeof(val), "%.2f", ENDIAN_CHANGE_U16(m_eddystoneData.temp) / 256.0f);
  out += val;
  out += " C\n";

  out += "Adv. Count ";
  snprintf(val, sizeof(val), "%d", ENDIAN_CHANGE_U32(m_eddystoneData.advCount));
  out += val;
  out += "\n";

  out += "Time in seconds ";
  snprintf(val, sizeof(val), "%d", rawsec/10);
  out += val;
  out += "\n";

  out += "Time ";

  snprintf(val, sizeof(val), "%04d", rawsec / 864000);
  out += val;
  out += ".";

  snprintf(val, sizeof(val), "%02d", (rawsec / 36000) % 24);
  out += val;
  out += ":";

  snprintf(val, sizeof(val), "%02d", (rawsec / 600) % 60);
  out += val;
  out += ":";

  snprintf(val, sizeof(val), "%02d", (rawsec / 10) % 60);
  out += val;
  out += "\n";

  return out;
} // toString

/**
 * Set the raw data for the beacon record.
 */
void NimBLEEddystoneTLM::setData(const std::string &data) {
    if (data.length() != sizeof(m_eddystoneData)) {
        NIMBLE_LOGE(LOG_TAG, "Unable to set the data ... length passed in was %d and expected %d",
                                                data.length(), sizeof(m_eddystoneData));
        return;
    }
  memcpy(&m_eddystoneData, data.data(), data.length());
} // setData

void NimBLEEddystoneTLM::setUUID(const NimBLEUUID &l_uuid) {
    beaconUUID = l_uuid.getNative()->u16.value;
} // setUUID

void NimBLEEddystoneTLM::setVersion(uint8_t version) {
    m_eddystoneData.version = version;
} // setVersion

void NimBLEEddystoneTLM::setVolt(uint16_t volt) {
    m_eddystoneData.volt = volt;
} // setVolt

void NimBLEEddystoneTLM::setTemp(float temp) {
    m_eddystoneData.temp = (uint16_t)temp;
} // setTemp

void NimBLEEddystoneTLM::setCount(uint32_t advCount) {
    m_eddystoneData.advCount = advCount;
} // setCount

void NimBLEEddystoneTLM::setTime(uint32_t tmil) {
    m_eddystoneData.tmil = tmil;
} // setTime

#endif
