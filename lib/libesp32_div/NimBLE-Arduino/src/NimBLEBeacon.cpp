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
#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include <string.h>
#include <algorithm>
#include "NimBLEBeacon.h"
#include "NimBLELog.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))

static const char* LOG_TAG = "NimBLEBeacon";


/**
 * @brief Construct a default beacon object.
 */
NimBLEBeacon::NimBLEBeacon() {
    m_beaconData.manufacturerId = 0x4c00;
    m_beaconData.subType        = 0x02;
    m_beaconData.subTypeLength  = 0x15;
    m_beaconData.major          = 0;
    m_beaconData.minor          = 0;
    m_beaconData.signalPower    = 0;
    memset(m_beaconData.proximityUUID, 0, sizeof(m_beaconData.proximityUUID));
} // NimBLEBeacon


/**
 * @brief Retrieve the data that is being advertised.
 * @return The advertised data.
 */
std::string NimBLEBeacon::getData() {
    return std::string((char*) &m_beaconData, sizeof(m_beaconData));
} // getData


/**
 * @brief Get the major value being advertised.
 * @return The major value advertised.
 */
uint16_t NimBLEBeacon::getMajor() {
    return m_beaconData.major;
}


/**
 * @brief Get the manufacturer ID being advertised.
 * @return The manufacturer ID value advertised.
 */
uint16_t NimBLEBeacon::getManufacturerId() {
    return m_beaconData.manufacturerId;
}


/**
 * @brief Get the minor value being advertised.
 * @return minor value advertised.
 */
uint16_t NimBLEBeacon::getMinor() {
    return m_beaconData.minor;
}


/**
 * @brief Get the proximity UUID being advertised.
 * @return The UUID advertised.
 */
NimBLEUUID NimBLEBeacon::getProximityUUID() {
    return NimBLEUUID(m_beaconData.proximityUUID, 16, true);
}


/**
 * @brief Get the signal power being advertised.
 * @return signal power level advertised.
 */
int8_t NimBLEBeacon::getSignalPower() {
    return m_beaconData.signalPower;
}


/**
 * @brief Set the raw data for the beacon record.
 * @param [in] data The raw beacon data.
 */
void NimBLEBeacon::setData(const std::string &data) {
    if (data.length() != sizeof(m_beaconData)) {
        NIMBLE_LOGE(LOG_TAG, "Unable to set the data ... length passed in was %d and expected %d",
                                                        data.length(), sizeof(m_beaconData));
        return;
    }
    memcpy(&m_beaconData, data.data(), sizeof(m_beaconData));
} // setData


/**
 * @brief Set the major value.
 * @param [in] major The major value.
 */
void NimBLEBeacon::setMajor(uint16_t major) {
    m_beaconData.major = ENDIAN_CHANGE_U16(major);
} // setMajor


/**
 * @brief Set the manufacturer ID.
 * @param [in] manufacturerId The manufacturer ID value.
 */
void NimBLEBeacon::setManufacturerId(uint16_t manufacturerId) {
    m_beaconData.manufacturerId = ENDIAN_CHANGE_U16(manufacturerId);
} // setManufacturerId


/**
 * @brief Set the minor value.
 * @param [in] minor The minor value.
 */
void NimBLEBeacon::setMinor(uint16_t minor) {
    m_beaconData.minor = ENDIAN_CHANGE_U16(minor);
} // setMinor


/**
 * @brief Set the proximity UUID.
 * @param [in] uuid The proximity UUID.
 */
void NimBLEBeacon::setProximityUUID(const NimBLEUUID &uuid) {
    NimBLEUUID temp_uuid = uuid;
    temp_uuid.to128();
    std::reverse_copy(temp_uuid.getNative()->u128.value,
                      temp_uuid.getNative()->u128.value + 16,
                      m_beaconData.proximityUUID);
} // setProximityUUID


/**
 * @brief Set the signal power.
 * @param [in] signalPower The signal power value.
 */
void NimBLEBeacon::setSignalPower(int8_t signalPower) {
    m_beaconData.signalPower = signalPower;
} // setSignalPower

#endif
