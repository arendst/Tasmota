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

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "NimBLEEddystoneTLM.h"
#include "NimBLELog.h"

#include <stdio.h>
#include <cstring>

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))
#define ENDIAN_CHANGE_U32(x) ((((x)&0xFF000000)>>24) + (((x)&0x00FF0000)>>8)) + ((((x)&0xFF00)<<8) + (((x)&0xFF)<<24))

static const char LOG_TAG[] = "NimBLEEddystoneTLM";

/**
 * @brief Construct a default EddystoneTLM beacon object.
 */
NimBLEEddystoneTLM::NimBLEEddystoneTLM() {
    beaconUUID = 0xFEAA;
    m_eddystoneData.frameType = EDDYSTONE_TLM_FRAME_TYPE;
    m_eddystoneData.version = 0;
    m_eddystoneData.volt = 3300; // 3300mV = 3.3V
    m_eddystoneData.temp = (uint16_t) ((float) 23.00 * 256); // 8.8 fixed format
    m_eddystoneData.advCount = 0;
    m_eddystoneData.tmil = 0;
} // NimBLEEddystoneTLM


/**
 * @brief Retrieve the data that is being advertised.
 * @return The advertised data.
 */
std::string NimBLEEddystoneTLM::getData() {
    return std::string((char*) &m_eddystoneData, sizeof(m_eddystoneData));
} // getData


/**
 * @brief Get the UUID being advertised.
 * @return The UUID advertised.
 */
NimBLEUUID NimBLEEddystoneTLM::getUUID() {
    return NimBLEUUID(beaconUUID);
} // getUUID


/**
 * @brief Get the version being advertised.
 * @return The version number.
 */
uint8_t NimBLEEddystoneTLM::getVersion() {
    return m_eddystoneData.version;
} // getVersion


/**
 * @brief Get the battery voltage.
 * @return The battery voltage.
 */
uint16_t NimBLEEddystoneTLM::getVolt() {
    return ENDIAN_CHANGE_U16(m_eddystoneData.volt);
} // getVolt


/**
 * @brief Get the temperature being advertised.
 * @return The temperature value.
 */
float NimBLEEddystoneTLM::getTemp() {
    return (int16_t)ENDIAN_CHANGE_U16(m_eddystoneData.temp) / 256.0f;
} // getTemp

/**
 * @brief Get the count of advertisements sent.
 * @return The number of advertisements.
 */
uint32_t NimBLEEddystoneTLM::getCount() {
    return ENDIAN_CHANGE_U32(m_eddystoneData.advCount);
} // getCount


/**
 * @brief Get the advertisement time.
 * @return The advertisement time.
 */
uint32_t NimBLEEddystoneTLM::getTime() {
    return (ENDIAN_CHANGE_U32(m_eddystoneData.tmil)) / 10;
} // getTime


/**
 * @brief Get a string representation of the beacon.
 * @return The string representation.
 */
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
  snprintf(val, sizeof(val), "%" PRIu32, ENDIAN_CHANGE_U32(m_eddystoneData.advCount));
  out += val;
  out += "\n";

  out += "Time in seconds ";
  snprintf(val, sizeof(val), "%" PRIu32, rawsec/10);
  out += val;
  out += "\n";

  out += "Time ";

  snprintf(val, sizeof(val), "%04" PRIu32, rawsec / 864000);
  out += val;
  out += ".";

  snprintf(val, sizeof(val), "%02" PRIu32, (rawsec / 36000) % 24);
  out += val;
  out += ":";

  snprintf(val, sizeof(val), "%02" PRIu32, (rawsec / 600) % 60);
  out += val;
  out += ":";

  snprintf(val, sizeof(val), "%02" PRIu32, (rawsec / 10) % 60);
  out += val;
  out += "\n";

  return out;
} // toString


/**
 * @brief Set the raw data for the beacon advertisement.
 * @param [in] data The raw data to advertise.
 */
void NimBLEEddystoneTLM::setData(const std::string &data) {
    if (data.length() != sizeof(m_eddystoneData)) {
        NIMBLE_LOGE(LOG_TAG, "Unable to set the data ... length passed in was %d and expected %d",
                                                data.length(), sizeof(m_eddystoneData));
        return;
    }
  memcpy(&m_eddystoneData, data.data(), data.length());
} // setData


/**
 * @brief Set the UUID to advertise.
 * @param [in] l_uuid The UUID.
 */
void NimBLEEddystoneTLM::setUUID(const NimBLEUUID &l_uuid) {
    beaconUUID = l_uuid.getNative()->u16.value;
} // setUUID


/**
 * @brief Set the version to advertise.
 * @param [in] version The version number.
 */
void NimBLEEddystoneTLM::setVersion(uint8_t version) {
    m_eddystoneData.version = version;
} // setVersion


/**
 * @brief Set the battery voltage to advertise.
 * @param [in] volt The voltage in millivolts.
 */
void NimBLEEddystoneTLM::setVolt(uint16_t volt) {
    m_eddystoneData.volt = volt;
} // setVolt


/**
 * @brief Set the temperature to advertise.
 * @param [in] temp The temperature value.
 */
void NimBLEEddystoneTLM::setTemp(float temp) {
    m_eddystoneData.temp = ENDIAN_CHANGE_U16((int16_t)(temp * 256.0f));
} // setTemp


/**
 * @brief Set the advertisement count.
 * @param [in] advCount The advertisement number.
 */
void NimBLEEddystoneTLM::setCount(uint32_t advCount) {
    m_eddystoneData.advCount = advCount;
} // setCount


/**
 * @brief Set the advertisement time.
 * @param [in] tmil The advertisement time in milliseconds.
 */
void NimBLEEddystoneTLM::setTime(uint32_t tmil) {
    m_eddystoneData.tmil = tmil;
} // setTime

#endif
