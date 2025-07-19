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

#include "NimBLEEddystoneTLM.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER

# include "NimBLEUUID.h"
# include "NimBLELog.h"

# define ENDIAN_CHANGE_U16(x) ((((x) & 0xFF00) >> 8) + (((x) & 0xFF) << 8))
# define ENDIAN_CHANGE_U32(x) \
     ((((x) & 0xFF000000) >> 24) + (((x) & 0x00FF0000) >> 8)) + ((((x) & 0xFF00) << 8) + (((x) & 0xFF) << 24))

static const char* LOG_TAG = "NimBLEEddystoneTLM";

/**
 * @brief Retrieve the data that is being advertised.
 * @return The advertised data.
 */
const NimBLEEddystoneTLM::BeaconData NimBLEEddystoneTLM::getData() {
    return m_eddystoneData;
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
int16_t NimBLEEddystoneTLM::getTemp() {
    return ENDIAN_CHANGE_U16(m_eddystoneData.temp);
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
    std::string out    = "";
    uint32_t    rawsec = ENDIAN_CHANGE_U32(m_eddystoneData.tmil);
    char        val[12];

    out += "Version ";
    snprintf(val, sizeof(val), "%d", m_eddystoneData.version);
    out += val;
    out += "\n";
    out += "Battery Voltage ";
    snprintf(val, sizeof(val), "%d", ENDIAN_CHANGE_U16(m_eddystoneData.volt));
    out += val;
    out += " mV\n";

    out             += "Temperature ";
    uint8_t intTemp  = m_eddystoneData.temp / 256;
    uint8_t frac     = m_eddystoneData.temp % 256 * 100 / 256;
    snprintf(val, sizeof(val), "%d.%d", intTemp, frac);
    out += val;
    out += " C\n";

    out += "Adv. Count ";
    snprintf(val, sizeof(val), "%" PRIu32, ENDIAN_CHANGE_U32(m_eddystoneData.advCount));
    out += val;
    out += "\n";

    out += "Time in seconds ";
    snprintf(val, sizeof(val), "%" PRIu32, rawsec / 10);
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
 * @param [in] data A pointer to the data to advertise.
 * @param [in] length The length of the data.
 */
void NimBLEEddystoneTLM::setData(const uint8_t* data, uint8_t length) {
    if (length != sizeof(m_eddystoneData)) {
        NIMBLE_LOGE(LOG_TAG,
                    "Unable to set the data ... length passed in was %d and expected %d",
                    length,
                    sizeof(m_eddystoneData));
        return;
    }
    memcpy(&m_eddystoneData, data, length);
} // setData

/**
 * @brief Set the raw data for the beacon advertisement.
 * @param [in] data The raw data to advertise.
 */
void NimBLEEddystoneTLM::setData(const NimBLEEddystoneTLM::BeaconData& data) {
    m_eddystoneData = data;
} // setData

/**
 * @brief Set the UUID to advertise.
 * @param [in] uuid The UUID.
 */
void NimBLEEddystoneTLM::setUUID(const NimBLEUUID& uuid) {
    if (uuid.bitSize() != 16) {
        NIMBLE_LOGE(LOG_TAG, "UUID must be 16 bits");
        return;
    }
    beaconUUID = *reinterpret_cast<const uint16_t*>(uuid.getValue());
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
 * @param [in] temp The temperature value in 8.8 fixed point format.
 */
void NimBLEEddystoneTLM::setTemp(int16_t temp) {
    m_eddystoneData.temp = temp;
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

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER
