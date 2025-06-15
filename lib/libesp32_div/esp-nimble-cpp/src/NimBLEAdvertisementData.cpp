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

#include "NimBLEAdvertisementData.h"
#if (CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && !CONFIG_BT_NIMBLE_EXT_ADV) || defined(_DOXYGEN_)

# include "NimBLEDevice.h"
# include "NimBLEUtils.h"
# include "NimBLEUUID.h"
# include "NimBLELog.h"

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "host/ble_hs_adv.h"
# else
#  include "nimble/nimble/host/include/host/ble_hs_adv.h"
# endif

static const char* LOG_TAG = "NimBLEAdvertisementData";

/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 * @param [in] length The size of data to be added to the payload.
 */
bool NimBLEAdvertisementData::addData(const uint8_t* data, size_t length) {
    if (m_payload.size() + length > BLE_HS_ADV_MAX_SZ) {
        NIMBLE_LOGE(LOG_TAG, "Data length exceeded");
        return false;
    }

    m_payload.insert(m_payload.end(), data, data + length);
    return true;
} // addData

/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 */
bool NimBLEAdvertisementData::addData(const std::vector<uint8_t>& data) {
    return addData(&data[0], data.size());
} // addData

/**
 * @brief Set the appearance.
 * @param [in] appearance The appearance code value.
 * @return True if successful.
 * @details If the appearance value is 0 then it will be removed from the advertisement if set previously.
 */
bool NimBLEAdvertisementData::setAppearance(uint16_t appearance) {
    if (appearance == 0) {
        return removeData(BLE_HS_ADV_TYPE_APPEARANCE);
    }

    uint8_t data[4];
    data[0] = 3;
    data[1] = BLE_HS_ADV_TYPE_APPEARANCE;
    data[2] = appearance;
    data[3] = (appearance >> 8) & 0xFF;
    return addData(data, 4);
} // setAppearance

/**
 * @brief Set the advertisement flags.
 * @param [in] flag The flags to be set in the advertisement.
 * * BLE_HS_ADV_F_DISC_LTD
 * * BLE_HS_ADV_F_DISC_GEN
 * * BLE_HS_ADV_F_BREDR_UNSUP - must always use with NimBLE
 * A flag value of 0 will remove the flags from the advertisement.
 */
bool NimBLEAdvertisementData::setFlags(uint8_t flag) {
    int dataLoc = getDataLocation(BLE_HS_ADV_TYPE_FLAGS);
    if (dataLoc != -1) {
        if (flag) {
            m_payload[dataLoc + 2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
            return true;
        } else {
            return removeData(BLE_HS_ADV_TYPE_FLAGS);
        }
    }

    uint8_t data[3];
    data[0] = 2;
    data[1] = BLE_HS_ADV_TYPE_FLAGS;
    data[2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
    return addData(data, 3);
} // setFlags

/**
 * @brief Adds Tx power level to the advertisement data.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::addTxPower() {
    uint8_t data[3];
    data[0] = BLE_HS_ADV_TX_PWR_LVL_LEN + 1;
    data[1] = BLE_HS_ADV_TYPE_TX_PWR_LVL;
# ifndef CONFIG_IDF_TARGET_ESP32P4
    data[2] = NimBLEDevice::getPower(NimBLETxPowerType::Advertise);
# else
    data[2] = 0;
# endif
    return addData(data, 3);
} // addTxPower

/**
 * @brief Set the preferred min and max connection intervals to advertise.
 * @param [in] minInterval The minimum preferred connection interval.
 * @param [in] maxInterval The Maximum preferred connection interval.
 * @details  Range = 0x0006(7.5ms) to 0x0C80(4000ms), values not within the range will be limited to this range.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setPreferredParams(uint16_t minInterval, uint16_t maxInterval) {
    minInterval = std::max<uint16_t>(minInterval, 0x6);
    minInterval = std::min<uint16_t>(minInterval, 0xC80);
    maxInterval = std::max<uint16_t>(maxInterval, 0x6);
    maxInterval = std::min<uint16_t>(maxInterval, 0xC80);
    maxInterval = std::max<uint16_t>(maxInterval, minInterval); // Max must be greater than or equal to min.

    uint8_t data[6];
    data[0] = BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1;
    data[1] = BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE;
    data[2] = minInterval;
    data[3] = minInterval >> 8;
    data[4] = maxInterval;
    data[5] = maxInterval >> 8;
    return addData(data, 6);
} // setPreferredParams

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
bool NimBLEAdvertisementData::addServiceUUID(const NimBLEUUID& serviceUUID) {
    uint8_t bytes = serviceUUID.bitSize() / 8;
    int     type;
    switch (bytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS128;
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Cannot add UUID, invalid size!");
            return false;
    }

    int     dataLoc = getDataLocation(type);
    uint8_t length  = bytes;
    if (dataLoc == -1) {
        length += 2;
    }

    if (length + getPayload().size() > BLE_HS_ADV_MAX_SZ) {
        NIMBLE_LOGE(LOG_TAG, "Cannot add UUID, data length exceeded!");
        return false;
    }

    uint8_t        data[BLE_HS_ADV_MAX_SZ];
    const uint8_t* uuid = serviceUUID.getValue();
    if (dataLoc == -1) {
        data[0] = 1 + bytes;
        data[1] = type;
        memcpy(&data[2], uuid, bytes);
        return addData(data, length);
    }

    m_payload.insert(m_payload.begin() + dataLoc + m_payload[dataLoc] + 1, uuid, uuid + bytes);
    m_payload[dataLoc] += bytes;
    return true;
} // addServiceUUID

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The string representation of the service to expose.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::addServiceUUID(const char* serviceUUID) {
    return addServiceUUID(NimBLEUUID(serviceUUID));
} // addServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if successful or uuid not found, false if uuid error or data could not be reset.
 */
bool NimBLEAdvertisementData::removeServiceUUID(const NimBLEUUID& serviceUUID) {
    uint8_t bytes = serviceUUID.bitSize() / 8;
    int     type;
    switch (bytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_COMP_UUIDS128;
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Cannot remove UUID, invalid size!");
            return false;
    }

    int dataLoc = getDataLocation(type);
    if (dataLoc == -1) {
        return true;
    }

    int uuidLoc = -1;
    for (size_t i = dataLoc + 2; i < m_payload.size(); i += bytes) {
        if (memcmp(&m_payload[i], serviceUUID.getValue(), bytes) == 0) {
            uuidLoc = i;
            break;
        }
    }

    if (uuidLoc == -1) {
        return true;
    }

    if (m_payload[dataLoc] - bytes == 1) {
        return removeData(type);
    }

    m_payload.erase(m_payload.begin() + uuidLoc, m_payload.begin() + uuidLoc + bytes);
    m_payload[dataLoc] -= bytes;
    return true;
} // removeServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if successful or uuid not found, false if uuid error or data could not be reset.
 */
bool NimBLEAdvertisementData::removeServiceUUID(const char* serviceUUID) {
    return removeServiceUUID(NimBLEUUID(serviceUUID));
} // removeServiceUUID

/**
 * @brief Remove all service UUIDs from the advertisement.
 */
bool NimBLEAdvertisementData::removeServices() {
    return true;
} // removeServices

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @param [in] length The length of the data.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setManufacturerData(const uint8_t* data, size_t length) {
    if (length > BLE_HS_ADV_MAX_FIELD_SZ) {
        NIMBLE_LOGE(LOG_TAG, "MFG data too long");
        return false;
    }

    uint8_t mdata[BLE_HS_ADV_MAX_SZ];
    mdata[0] = length + 1;
    mdata[1] = BLE_HS_ADV_TYPE_MFG_DATA;
    memcpy(&mdata[2], data, length);
    return addData(mdata, length + 2);
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setManufacturerData(const std::string& data) {
    return setManufacturerData(reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setManufacturerData(const std::vector<uint8_t>& data) {
    return setManufacturerData(&data[0], data.size());
} // setManufacturerData

/**
 * @brief Set the URI to advertise.
 * @param [in] uri The uri to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setURI(const std::string& uri) {
    if (uri.length() > BLE_HS_ADV_MAX_FIELD_SZ) {
        NIMBLE_LOGE(LOG_TAG, "URI too long");
        return false;
    }

    uint8_t data[BLE_HS_ADV_MAX_SZ];
    uint8_t length = 2 + uri.length();
    data[0]        = length - 1;
    data[1]        = BLE_HS_ADV_TYPE_URI;
    memcpy(&data[2], uri.c_str(), uri.length());
    return addData(data, length);
} // setURI

/**
 * @brief Set the complete name of this device.
 * @param [in] name The name to advertise.
 * @param [in] isComplete If true the name is complete, which will set the data type accordingly.
 * @details If the name is longer than 29 characters it will be truncated.
 * and the data type will be set to incomplete name.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setName(const std::string& name, bool isComplete) {
    if (name.length() > BLE_HS_ADV_MAX_FIELD_SZ) {
        NIMBLE_LOGE(LOG_TAG, "Name too long - truncating");
        isComplete = false;
    }

    uint8_t data[BLE_HS_ADV_MAX_SZ];
    uint8_t length = 2 + std::min<uint8_t>(name.length(), BLE_HS_ADV_MAX_FIELD_SZ);
    data[0]        = length - 1;
    data[1]        = isComplete ? BLE_HS_ADV_TYPE_COMP_NAME : BLE_HS_ADV_TYPE_INCOMP_NAME;
    memcpy(&data[2], name.c_str(), std::min<uint8_t>(name.length(), BLE_HS_ADV_MAX_FIELD_SZ));
    return addData(data, length);
} // setName

/**
 * @brief Set the short name.
 * @param [in] name The short name of the device.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setShortName(const std::string& name) {
    return setName(name, false);
} // setShortName

/**
 * @brief Set a single service to advertise as a complete list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setCompleteServices(const NimBLEUUID& uuid) {
    return setServices(true, uuid.bitSize(), {uuid});
} // setCompleteServices

/**
 * @brief Set the complete list of 16 bit services to advertise.
 * @param [in] uuids A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setCompleteServices16(const std::vector<NimBLEUUID>& uuids) {
    return setServices(true, 16, uuids);
} // setCompleteServices16

/**
 * @brief Set the complete list of 32 bit services to advertise.
 * @param [in] uuids A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setCompleteServices32(const std::vector<NimBLEUUID>& uuids) {
    return setServices(true, 32, uuids);
} // setCompleteServices32

/**
 * @brief Set a single service to advertise as a partial list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setPartialServices(const NimBLEUUID& uuid) {
    return setServices(false, uuid.bitSize(), {uuid});
} // setPartialServices

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] uuids A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setPartialServices16(const std::vector<NimBLEUUID>& uuids) {
    return setServices(false, 16, uuids);
} // setPartialServices16

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] uuids A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEAdvertisementData::setPartialServices32(const std::vector<NimBLEUUID>& uuids) {
    return setServices(false, 32, uuids);
} // setPartialServices32

/**
 * @brief Utility function to create the list of service UUID's from a vector.
 * @param [in] complete If true the vector is the complete set of services.
 * @param [in] size The bit size of the UUID's in the vector. (16, 32, or 128).
 * @param [in] uuids The vector of service UUID's to advertise.
 * @return True if successful.
 * @details The number of services will be truncated if the total length exceeds 31 bytes.
 */
bool NimBLEAdvertisementData::setServices(bool complete, uint8_t size, const std::vector<NimBLEUUID>& uuids) {
    uint8_t bytes  = size / 8;
    uint8_t length = 2; // start with 2 for length + type bytes
    uint8_t data[BLE_HS_ADV_MAX_SZ];

    for (const auto& uuid : uuids) {
        if (uuid.bitSize() != size) {
            NIMBLE_LOGE(LOG_TAG, "Service UUID(%d) invalid", size);
            continue;
        } else {
            if (length + bytes >= BLE_HS_ADV_MAX_SZ) {
                NIMBLE_LOGW(LOG_TAG, "Too many services - truncating");
                complete = false;
                break;
            }
            memcpy(&data[length], uuid.getValue(), bytes);
            length += bytes;
        }
    }

    data[0] = length - 1; // don't count the length byte as part of the AD length

    switch (size) {
        case 16:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS16 : BLE_HS_ADV_TYPE_INCOMP_UUIDS16);
            break;
        case 32:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS32 : BLE_HS_ADV_TYPE_INCOMP_UUIDS32);
            break;
        case 128:
            data[1] = (complete ? BLE_HS_ADV_TYPE_COMP_UUIDS128 : BLE_HS_ADV_TYPE_INCOMP_UUIDS128);
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Cannot set services, invalid size!");
            return false;
    }

    return addData(data, length);
} // setServices

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @param [in] length The length of the data.
 * @note If data length is 0 the service data will not be advertised.
 * @return True if successful, false if data length is too long or could not be set.
 */
bool NimBLEAdvertisementData::setServiceData(const NimBLEUUID& uuid, const uint8_t* data, size_t length) {
    uint8_t uuidBytes = uuid.bitSize() / 8;
    uint8_t sDataLen  = 2 + uuidBytes + length;
    if (sDataLen > BLE_HS_ADV_MAX_SZ) {
        NIMBLE_LOGE(LOG_TAG, "Service Data too long");
        return false;
    }

    uint8_t type;
    switch (uuidBytes) {
        case 2:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID16;
            break;
        case 4:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID32;
            break;
        case 16:
            type = BLE_HS_ADV_TYPE_SVC_DATA_UUID128;
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Cannot set service data, invalid size!");
            return false;
    }

    if (length == 0) {
        removeData(type);
        return true;
    }

    uint8_t sData[BLE_HS_ADV_MAX_SZ];
    sData[0] = uuidBytes + length + 1;
    sData[1] = type;
    memcpy(&sData[2], uuid.getValue(), uuidBytes);
    memcpy(&sData[2 + uuidBytes], data, length);
    return addData(sData, sDataLen);
} // setServiceData

/**
 * @brief Set the service data (UUID + data)
 * @param [in] uuid The UUID to set with the service data.
 * @param [in] data The data to be associated with the service data advertised.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEAdvertisementData::setServiceData(const NimBLEUUID& uuid, const std::string& data) {
    return setServiceData(uuid, reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setServiceData

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEAdvertisementData::setServiceData(const NimBLEUUID& uuid, const std::vector<uint8_t>& data) {
    return setServiceData(uuid, &data[0], data.size());
} // setServiceData

/**
 * @brief Get the location of the data in the payload.
 * @param [in] type The type of data to search for.
 * @return -1 if the data is not found, otherwise the index of the data in the payload.
 */
int NimBLEAdvertisementData::getDataLocation(uint8_t type) const {
    size_t index = 0;
    while (index < m_payload.size()) {
        if (m_payload[index + 1] == type) {
            return index;
        }
        index += m_payload[index] + 1;
    }
    return -1;
} // getDataLocation

/**
 * @brief Remove data from the advertisement data.
 * @param [in] type The type of data to remove.
 * @return True if successful, false if the data was not found.
 */
bool NimBLEAdvertisementData::removeData(uint8_t type) {
    int dataLoc = getDataLocation(type);
    if (dataLoc != -1) {
        std::vector<uint8_t> swap(m_payload.begin(), m_payload.begin() + dataLoc);
        int                  nextData = dataLoc + m_payload[dataLoc] + 1;
        swap.insert(swap.end(), m_payload.begin() + nextData, m_payload.end());
        swap.swap(m_payload);
        return true;
    }

    return false;
} // removeData

/**
 * @brief Retrieve the payload that is to be advertised.
 * @return The payload of the advertisement data.
 */
std::vector<uint8_t> NimBLEAdvertisementData::getPayload() const {
    return m_payload;
} // getPayload

/**
 * @brief Clear the advertisement data for reuse.
 */
void NimBLEAdvertisementData::clearData() {
    std::vector<uint8_t>().swap(m_payload);
} // clearData

/**
 * @brief Get the string representation of the advertisement data.
 * @return The string representation of the advertisement data.
 */
std::string NimBLEAdvertisementData::toString() const {
    std::string hexStr = NimBLEUtils::dataToHexString(&m_payload[0], m_payload.size());
    std::string str;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        str += hexStr[i];
        str += hexStr[i + 1];
        if (i + 2 < hexStr.length()) {
            str += " ";
        }
    }

    return str;
} // toString

#endif // (CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && !CONFIG_BT_NIMBLE_EXT_ADV) || defined(_DOXYGEN_)
