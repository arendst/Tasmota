/*
 * Copyright 2020-2025 Ryan Powell <ryan@nable-embedded.io> and
 * esp-nimble-cpp, NimBLE-Arduino contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NimBLEBeacon.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER

# include "NimBLEUUID.h"
# include "NimBLELog.h"

# define ENDIAN_CHANGE_U16(x) ((((x) & 0xFF00) >> 8) + (((x) & 0xFF) << 8))

static const char* LOG_TAG = "NimBLEBeacon";

/**
 * @brief Retrieve the data that is being advertised.
 * @return The advertised data.
 */
const NimBLEBeacon::BeaconData& NimBLEBeacon::getData() {
    return m_beaconData;
} // getData

/**
 * @brief Get the major value being advertised.
 * @return The major value advertised.
 */
uint16_t NimBLEBeacon::getMajor() {
    return m_beaconData.major;
} // getMajor

/**
 * @brief Get the manufacturer ID being advertised.
 * @return The manufacturer ID value advertised.
 */
uint16_t NimBLEBeacon::getManufacturerId() {
    return m_beaconData.manufacturerId;
} // getManufacturerId

/**
 * @brief Get the minor value being advertised.
 * @return minor value advertised.
 */
uint16_t NimBLEBeacon::getMinor() {
    return m_beaconData.minor;
} // getMinor

/**
 * @brief Get the proximity UUID being advertised.
 * @return The UUID advertised.
 */
NimBLEUUID NimBLEBeacon::getProximityUUID() {
    return NimBLEUUID(m_beaconData.proximityUUID, 16).reverseByteOrder();
} // getProximityUUID

/**
 * @brief Get the signal power being advertised.
 * @return signal power level advertised.
 */
int8_t NimBLEBeacon::getSignalPower() {
    return m_beaconData.signalPower;
} // getSignalPower

/**
 * @brief Set the beacon data.
 * @param [in] data A pointer to the raw data that the beacon should advertise.
 * @param [in] length The length of the data.
 */
void NimBLEBeacon::setData(const uint8_t* data, uint8_t length) {
    if (length != sizeof(BeaconData)) {
        NIMBLE_LOGE(LOG_TAG, "Data length must be %d bytes, sent: %d", sizeof(BeaconData), length);
        return;
    }
    memcpy(&m_beaconData, data, length);
} // setData

/**
 * @brief Set the beacon data.
 * @param [in] data The data that the beacon should advertise.
 */
void NimBLEBeacon::setData(const NimBLEBeacon::BeaconData& data) {
    m_beaconData = data;
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
void NimBLEBeacon::setProximityUUID(const NimBLEUUID& uuid) {
    NimBLEUUID temp_uuid = uuid;
    temp_uuid.to128();
    temp_uuid.reverseByteOrder();
    memcpy(m_beaconData.proximityUUID, temp_uuid.getValue(), 16);
} // setProximityUUID

/**
 * @brief Set the signal power.
 * @param [in] signalPower The signal power value.
 */
void NimBLEBeacon::setSignalPower(int8_t signalPower) {
    m_beaconData.signalPower = signalPower;
} // setSignalPower

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER
