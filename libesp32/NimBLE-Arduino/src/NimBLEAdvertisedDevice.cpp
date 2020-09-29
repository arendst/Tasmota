/*
 * NimBLEAdvertisedDevice.cpp
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 *  BLEAdvertisedDevice.cpp
 *
 *  Created on: Jul 3, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)

#include "NimBLEDevice.h"
#include "NimBLEAdvertisedDevice.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEAdvertisedDevice";


/**
 * @brief Constructor
 */
NimBLEAdvertisedDevice::NimBLEAdvertisedDevice() {
    m_advType          = 0;
    m_appearance       = 0;
    m_manufacturerData = "";
    m_name             = "";
    m_rssi             = -9999;
    m_txPower          = 0;
    m_payloadLength    = 0;
    m_payload          = nullptr;

    m_haveAppearance       = false;
    m_haveManufacturerData = false;
    m_haveName             = false;
    m_haveRSSI             = false;
    m_haveServiceData      = false;
    m_haveServiceUUID      = false;
    m_haveTXPower          = false;
    m_callbackSent         = false;

} // NimBLEAdvertisedDevice


/**
 * @brief Get the address of the advertising device.
 * @return The address of the advertised device.
 */
NimBLEAddress NimBLEAdvertisedDevice::getAddress() {
    return m_address;
} // getAddress


/**
 * @brief Get the advertisement type.
 * @return The advertising type the device is reporting:
 * * BLE_HCI_ADV_TYPE_ADV_IND            (0) - indirect advertising
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD  (1) - direct advertisng - high duty cycle
 * * BLE_HCI_ADV_TYPE_ADV_SCAN_IND       (2) - indirect scan response
 * * BLE_HCI_ADV_TYPE_ADV_NONCONN_IND    (3) - indirect advertisng - not connectable
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD  (4) - direct advertising - low duty cycle
 */
uint8_t NimBLEAdvertisedDevice::getAdvType() {
    return m_advType;
} // getAdvType


/**
 * @brief Get the appearance.
 *
 * A %BLE device can declare its own appearance.  The appearance is how it would like to be shown to an end user
 * typcially in the form of an icon.
 *
 * @return The appearance of the advertised device.
 */
uint16_t NimBLEAdvertisedDevice::getAppearance() {
    return m_appearance;
} // getAppearance


/**
 * @brief Get the manufacturer data.
 * @return The manufacturer data of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getManufacturerData() {
    return m_manufacturerData;
} // getManufacturerData


/**
 * @brief Get the advertised name.
 * @return The name of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getName() {
    return m_name;
} // getName


/**
 * @brief Get the RSSI.
 * @return The RSSI of the advertised device.
 */
int NimBLEAdvertisedDevice::getRSSI() {
    return m_rssi;
} // getRSSI


/**
 * @brief Get the scan object that created this advertised device.
 * @return The scan object.
 */
NimBLEScan* NimBLEAdvertisedDevice::getScan() {
    return NimBLEDevice::getScan();
} // getScan


/**
 * @brief Get the service data.
 * @param [in] index The vector index of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(uint8_t index) {
    if(index > m_serviceDataVec.size()) {
        NIMBLE_LOGW(LOG_TAG, "getServiceData: index out of range");
        return "";
    }
    return m_serviceDataVec[index].second;
} //getServiceData


/**
 * @brief Get the service data.
 * @param [in] uuid The uuid of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(const NimBLEUUID &uuid) const {
    for(auto &it : m_serviceDataVec) {
        if(it.first == uuid) {
            return it.second;
        }
    }
        NIMBLE_LOGW(LOG_TAG, "getServiceData: uuid not found");
    return "";
} //getServiceData


/**
 * @brief Get the advertised service UUID.
 * @param [in] index The vector index of the service data UUID requested.
 * @return The advertised service UUID or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceDataUUID(uint8_t index) {
    if(!haveServiceData() || index > m_serviceDataVec.size()) {
        NIMBLE_LOGW(LOG_TAG, "getServiceDataUUID: index out of range");
        return NimBLEUUID("");
    }
    return m_serviceDataVec[index].first;
} // getServiceDataUUID


/**
 * @brief Get the count of advertised service data UUIDS
 * @return The number of service data UUIDS in the vector.
 */
size_t NimBLEAdvertisedDevice::getServiceDataCount() {
    return m_serviceDataVec.size();
} // getServiceDataCount


/**
 * @brief Get the Service UUID.
 * @param [in] index The vector index of the service UUID requested.
 * @return The Service UUID of the advertised service, or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceUUID(uint8_t index) {
    if(!haveServiceUUID() || index > m_serviceUUIDs.size()) {
        NIMBLE_LOGW(LOG_TAG, "getServiceUUID: index out of range");
        return NimBLEUUID("");
    }
    return m_serviceUUIDs[index];
} // getServiceUUID


/**
 * @brief Get the number of services advertised
 * @return The count of services in the advertising packet.
 */
size_t NimBLEAdvertisedDevice::getServiceUUIDCount() {
    return m_serviceUUIDs.size();
} // getServiceUUIDCount


/**
 * @brief Check advertised services for existance of the required UUID
 * @return Return true if service is advertised
 */
bool NimBLEAdvertisedDevice::isAdvertisingService(const NimBLEUUID &uuid) const {
    for (int i = 0; i < m_serviceUUIDs.size(); i++) {
        if (m_serviceUUIDs[i].equals(uuid)) return true;
    }
    return false;
} // isAdvertisingService


/**
 * @brief Get the TX Power.
 * @return The TX Power of the advertised device.
 */
int8_t NimBLEAdvertisedDevice::getTXPower() {
    return m_txPower;
} // getTXPower


/**
 * @brief Does this advertisement have an appearance value?
 * @return True if there is an appearance value present.
 */
bool NimBLEAdvertisedDevice::haveAppearance() {
    return m_haveAppearance;
} // haveAppearance


/**
 * @brief Does this advertisement have manufacturer data?
 * @return True if there is manufacturer data present.
 */
bool NimBLEAdvertisedDevice::haveManufacturerData() {
    return m_haveManufacturerData;
} // haveManufacturerData


/**
 * @brief Does this advertisement have a name value?
 * @return True if there is a name value present.
 */
bool NimBLEAdvertisedDevice::haveName() {
    return m_haveName;
} // haveName


/**
 * @brief Does this advertisement have a signal strength value?
 * @return True if there is a signal strength value present.
 */
bool NimBLEAdvertisedDevice::haveRSSI() {
    return m_haveRSSI;
} // haveRSSI


/**
 * @brief Does this advertisement have a service data value?
 * @return True if there is a service data value present.
 */
bool NimBLEAdvertisedDevice::haveServiceData() {
    return m_haveServiceData;
} // haveServiceData


/**
 * @brief Does this advertisement have a service UUID value?
 * @return True if there is a service UUID value present.
 */
bool NimBLEAdvertisedDevice::haveServiceUUID() {
    return m_haveServiceUUID;
} // haveServiceUUID


/**
 * @brief Does this advertisement have a transmission power value?
 * @return True if there is a transmission power value present.
 */
bool NimBLEAdvertisedDevice::haveTXPower() {
    return m_haveTXPower;
} // haveTXPower


/**
 * @brief Parse the advertising pay load.
 *
 * The pay load is a buffer of bytes that is either 31 bytes long or terminated by
 * a 0 length value.  Each entry in the buffer has the format:
 * [length][type][data...]
 *
 * The length does not include itself but does include everything after it until the next record.  A record
 * with a length value of 0 indicates a terminator.
 *
 * https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile
 */
 void NimBLEAdvertisedDevice::parseAdvertisement(uint8_t* payload, uint8_t length) {
    struct ble_hs_adv_fields fields;
    int rc = ble_hs_adv_parse_fields(&fields, payload, length);
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Gap Event Parse ERROR.");
        return;
    }

    m_payload = payload;
    m_payloadLength = length;

#if CONFIG_LOG_DEFAULT_LEVEL > 3 || (ARDUINO_ARCH_ESP32 && CORE_DEBUG_LEVEL >= 4)
    char* pHex = NimBLEUtils::buildHexData(nullptr, m_payload, m_payloadLength);
    NIMBLE_LOGD(LOG_TAG,"payload: %s", pHex);
    free(pHex);
#endif

    if (fields.uuids16 != NULL) {
        for (int i = 0; i < fields.num_uuids16; i++) {
            setServiceUUID(NimBLEUUID(fields.uuids16[i].value));
        }
    }

    if (fields.uuids32 != NULL) {
        for (int i = 0; i < fields.num_uuids32; i++) {
            setServiceUUID(NimBLEUUID(fields.uuids32[i].value));
        }
    }

    if (fields.uuids128 != NULL) {
        for (int i = 0; i < fields.num_uuids128; i++) {
            setServiceUUID(NimBLEUUID(&fields.uuids128[i]));
        }
    }

    if (fields.name != NULL) {
        setName(std::string(reinterpret_cast<char*>(fields.name), fields.name_len));
    }

    if (fields.tx_pwr_lvl_is_present) {
        setTXPower(fields.tx_pwr_lvl);
    }

    if (fields.svc_data_uuid16 != NULL ||
        fields.svc_data_uuid32 != NULL ||
        fields.svc_data_uuid128 != NULL)
    {
        ble_hs_adv_field *field;
        uint8_t *data = payload;
        while(length > 1) {
            field = (ble_hs_adv_field*)data;

            if(field->length > length) {
                break;
            }

            if(field->type == BLE_HS_ADV_TYPE_SVC_DATA_UUID16) {
                if(field->length > 2) {
                    uint16_t uuid;
                    memcpy(&uuid, field->value, 2);
                    setServiceData(NimBLEUUID(uuid), std::string(reinterpret_cast<char*>(field->value + 2), field->length - 3));
                }
            }

            if(field->type == BLE_HS_ADV_TYPE_SVC_DATA_UUID32) {
                if(field->length > 4) {
                    uint32_t uuid;
                    memcpy(&uuid, field->value, 4);
                    setServiceData(NimBLEUUID(uuid), std::string(reinterpret_cast<char*>(field->value + 4), field->length - 5));
                }
            }

            if(field->type == BLE_HS_ADV_TYPE_SVC_DATA_UUID128) {
                if(field->length > 16) {
                    NimBLEUUID uuid(field->value, (size_t)16, false);
                    setServiceData(uuid, std::string(reinterpret_cast<char*>(field->value + 16), field->length - 17));
                }
            }

            length -= 1 + field->length;
            data += 1 + field->length;
        }
    }

    if (fields.appearance_is_present) {
        setAppearance(fields.appearance);
    }

    if (fields.mfg_data != NULL) {
        setManufacturerData(std::string(reinterpret_cast<char*>(fields.mfg_data), fields.mfg_data_len));
    }

/* TODO: create storage and fucntions for these parameters
    if (fields.public_tgt_addr != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    public_tgt_addr=");
        u8p = fields.public_tgt_addr;
        for (i = 0; i < fields.num_public_tgt_addrs; i++) {
            NIMBLE_LOGD(LOG_TAG, "public_tgt_addr=%s ", addr_str(u8p));
            u8p += BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN;
        }
        NIMBLE_LOGD(LOG_TAG, "\n");
    }

    if (fields.slave_itvl_range != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    slave_itvl_range=");
        print_bytes(fields.slave_itvl_range, BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN);
        NIMBLE_LOGD(LOG_TAG, "\n");
    }

    if (fields.adv_itvl_is_present) {
        NIMBLE_LOGD(LOG_TAG, "    adv_itvl=0x%04x\n", fields.adv_itvl);
    }

    if (fields.uri != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    uri=");
        print_bytes(fields.uri, fields.uri_len);
        NIMBLE_LOGD(LOG_TAG, "\n");
    }
*/

 } //parseAdvertisement


/**
 * @brief Set the address of the advertised device.
 * @param [in] address The address of the advertised device.
 */
void NimBLEAdvertisedDevice::setAddress(NimBLEAddress address) {
    m_address = address;
} // setAddress


/**
 * @brief Set the adFlag for this device.
 * @param [in] The discovered adFlag.
 */
void NimBLEAdvertisedDevice::setAdvType(uint8_t advType) {
    m_advType = advType;
} // setAdvType


/**
 * @brief Set the appearance for this device.
 * @param [in] The discovered appearance.
 */
void NimBLEAdvertisedDevice::setAppearance(uint16_t appearance) {
    m_appearance     = appearance;
    m_haveAppearance = true;
} // setAppearance


/**
 * @brief Set the manufacturer data for this device.
 * @param [in] The discovered manufacturer data.
 */
void NimBLEAdvertisedDevice::setManufacturerData(std::string manufacturerData) {
    m_manufacturerData     = manufacturerData;
    m_haveManufacturerData = true;
} // setManufacturerData


/**
 * @brief Set the name for this device.
 * @param [in] name The discovered name.
 */
void NimBLEAdvertisedDevice::setName(std::string name) {
    m_name     = name;
    m_haveName = true;
} // setName


/**
 * @brief Set the RSSI for this device.
 * @param [in] rssi The discovered RSSI.
 */
void NimBLEAdvertisedDevice::setRSSI(int rssi) {
    m_rssi     = rssi;
    m_haveRSSI = true;
} // setRSSI


/**
 * @brief Set the Service UUID for this device.
 * @param [in] serviceUUID The discovered serviceUUID
 */

void NimBLEAdvertisedDevice::setServiceUUID(const char* serviceUUID) {
    return setServiceUUID(NimBLEUUID(serviceUUID));
} // setServiceUUID


/**
 * @brief Set the Service UUID for this device.
 * @param [in] serviceUUID The discovered serviceUUID
 */
void NimBLEAdvertisedDevice::setServiceUUID(NimBLEUUID serviceUUID) {
    // Don't add duplicates
    for (int i = 0; i < m_serviceUUIDs.size(); i++) {
        if (m_serviceUUIDs[i] == serviceUUID) {
            return;
        }
    }
    m_serviceUUIDs.push_back(serviceUUID);
    m_haveServiceUUID = true;
} // setServiceUUID


/**
 * @brief Set the ServiceData value.
 * @param [in] uuid The UUID that the service data belongs to.
 * @param [in] data The service data.
 */
void NimBLEAdvertisedDevice::setServiceData(NimBLEUUID uuid, std::string data) {
    m_haveServiceData = true;
    for(auto &it : m_serviceDataVec) {
        if(it.first == uuid) {
            it.second = data;
            return;
        }
    }
    m_serviceDataVec.push_back({uuid, data});
} //setServiceData


/**
 * @brief Set the power level for this device.
 * @param [in] txPower The discovered power level.
 */
void NimBLEAdvertisedDevice::setTXPower(int8_t txPower) {
    m_txPower     = txPower;
    m_haveTXPower = true;
} // setTXPower


/**
 * @brief Create a string representation of this device.
 * @return A string representation of this device.
 */
std::string NimBLEAdvertisedDevice::toString() {
    std::string res = "Name: " + getName() + ", Address: " + getAddress().toString();

    if (haveAppearance()) {
        char val[6];
        snprintf(val, sizeof(val), "%d", getAppearance());
        res += ", appearance: ";
        res += val;
    }

    if (haveManufacturerData()) {
        char *pHex = NimBLEUtils::buildHexData(nullptr, (uint8_t*)getManufacturerData().data(), getManufacturerData().length());
        res += ", manufacturer data: ";
        res += pHex;
        free(pHex);
    }

    if (haveServiceUUID()) {
        res += ", serviceUUID: " + getServiceUUID().toString();
    }

    if (haveTXPower()) {
        char val[5];
        snprintf(val, sizeof(val), "%d", getTXPower());
        res += ", txPower: ";
        res += val;
    }

    if(haveServiceData()) {
        size_t count = getServiceDataCount();
        res += "\nService Data:";
        for(size_t i = 0; i < count; i++) {
            res += "\nUUID: " + std::string(getServiceDataUUID(i));
            res += ", Data: " + getServiceData(i);
        }
    }

    return res;

} // toString


/**
 * @brief Get the payload advertised by the device.
 * @return The advertisement payload.
 */
uint8_t* NimBLEAdvertisedDevice::getPayload() {
    return m_payload;
} // getPayload


/**
 * @brief Get the advertised device address type.
 * @return The device address type:
 * * BLE_ADDR_PUBLIC      (0x00)
 * * BLE_ADDR_RANDOM      (0x01)
 * * BLE_ADDR_PUBLIC_ID   (0x02)
 * * BLE_ADDR_RANDOM_ID   (0x03)
 */
uint8_t NimBLEAdvertisedDevice::getAddressType() {
    return m_address.getType();
} // getAddressType


/**
 * @brief Get the timeStamp of when the device last advertised.
 * @return The timeStamp of when the device was last seen.
 */
time_t NimBLEAdvertisedDevice::getTimestamp() {
    return m_timestamp;
} // getTimestamp


/**
 * @brief Get the length of the payload advertised by the device.
 * @return The size of the payload in bytes.
 */
size_t NimBLEAdvertisedDevice::getPayloadLength() {
    return m_payloadLength;
} // getPayloadLength


#endif // #if defined( CONFIG_BT_NIMBLE_ROLE_CENTRAL)
#endif /* CONFIG_BT_ENABLED */

