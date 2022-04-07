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

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)

#include "NimBLEDevice.h"
#include "NimBLEAdvertisedDevice.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEAdvertisedDevice";


/**
 * @brief Constructor
 */
NimBLEAdvertisedDevice::NimBLEAdvertisedDevice() :
    m_payload(62,0)
{
    m_advType          = 0;
    m_rssi             = -9999;
    m_callbackSent     = false;
    m_timestamp        = 0;
    m_advLength        = 0;
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
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_APPEARANCE, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length == BLE_HS_ADV_APPEARANCE_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getAppearance


/**
 * @brief Get the advertisement interval.
 * @return The advertisement interval in 0.625ms units.
 */
uint16_t NimBLEAdvertisedDevice::getAdvInterval() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_ADV_ITVL, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length == BLE_HS_ADV_ADV_ITVL_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getAdvInterval


/**
 * @brief Get the preferred min connection interval.
 * @return The preferred min connection interval in 1.25ms units.
 */
uint16_t NimBLEAdvertisedDevice::getMinInterval() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length == BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getMinInterval


/**
 * @brief Get the preferred max connection interval.
 * @return The preferred max connection interval in 1.25ms units.
 */
uint16_t NimBLEAdvertisedDevice::getMaxInterval() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length == BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1) {
            return *(field->value + 2) | *(field->value + 3) << 8;
        }
    }

    return 0;
} // getMaxInterval


/**
 * @brief Get the manufacturer data.
 * @return The manufacturer data of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getManufacturerData() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_MFG_DATA, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length > 1) {
            return std::string((char*)field->value, field->length - 1);
        }
    }

    return "";
} // getManufacturerData


/**
 * @brief Get the URI from the advertisement.
 * @return The URI data.
 */
std::string NimBLEAdvertisedDevice::getURI() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_URI, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length > 1) {
            return std::string((char*)field->value, field->length - 1);
        }
    }

    return "";
} // getURI


/**
 * @brief Get the advertised name.
 * @return The name of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getName() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_COMP_NAME, 0, &data_loc) > 0 ||
       findAdvField(BLE_HS_ADV_TYPE_INCOMP_NAME, 0, &data_loc) > 0)
    {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length > 1) {
            return std::string((char*)field->value, field->length - 1);
        }
    }

    return "";
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
 * @brief Get the number of target addresses.
 * @return The number of addresses.
 */
size_t NimBLEAdvertisedDevice::getTargetAddressCount() {
    uint8_t count = 0;

    count = findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR);
    count += findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR);

    return count;
}


/**
 * @brief Get the target address at the index.
 * @param [in] index The index of the target address.
 * @return The target address.
 */
NimBLEAddress NimBLEAdvertisedDevice::getTargetAddress(uint8_t index) {
    ble_hs_adv_field *field = nullptr;
    uint8_t count = 0;
    uint8_t data_loc = 0xFF;

    index++;
    count = findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR, index, &data_loc);

    if (count < index) {
        index -= count;
        count = findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR, index, &data_loc);
    }

    if(count > 0 && data_loc != 0xFF) {
        field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length < index *  BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN) {
            index -= count - field->length / BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN;
        }
        if(field->length > index * BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN) {
            return NimBLEAddress(field->value + (index - 1) * BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN);
        }
    }

    return NimBLEAddress("");
}


/**
 * @brief Get the service data.
 * @param [in] index The index of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(uint8_t index) {
    ble_hs_adv_field *field = nullptr;
    uint8_t bytes;
    uint8_t data_loc = findServiceData(index, &bytes);

    if(data_loc != 0xFF) {
        field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length > bytes) {
            return std::string((char*)(field->value + bytes), field->length - bytes - 1);
        }
    }

    return "";
} //getServiceData


/**
 * @brief Get the service data.
 * @param [in] uuid The uuid of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(const NimBLEUUID &uuid) {
    ble_hs_adv_field *field = nullptr;
    uint8_t bytes;
    uint8_t index = 0;
    uint8_t data_loc = findServiceData(index, &bytes);
    uint8_t uuidBytes = uuid.bitSize() / 8;
    uint8_t plSize = m_payload.size() - 2;

    while(data_loc < plSize) {
        field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(bytes == uuidBytes && NimBLEUUID(field->value, bytes, false) == uuid) {
           return std::string((char*)(field->value + bytes), field->length - bytes - 1);
        }

        index++;
        data_loc = findServiceData(index, &bytes);
    }

    NIMBLE_LOGI(LOG_TAG, "No service data found");
    return "";
} //getServiceData


/**
 * @brief Get the UUID of the serice data at the index.
 * @param [in] index The index of the service data UUID requested.
 * @return The advertised service data UUID or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceDataUUID(uint8_t index) {
    ble_hs_adv_field *field = nullptr;
    uint8_t bytes;
    uint8_t data_loc = findServiceData(index, &bytes);

    if(data_loc != 0xFF) {
        field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length >= bytes) {
            return NimBLEUUID(field->value, bytes, false);
        }
    }

    return NimBLEUUID("");
} // getServiceDataUUID


/**
 * @brief Find the service data at the index.
 * @param [in] index The index of the service data to find.
 * @param [in] bytes A pointer to storage for the number of the bytes in the UUID.
 * @return The index in the vector where the data is located, 0xFF if not found.
 */
uint8_t NimBLEAdvertisedDevice::findServiceData(uint8_t index, uint8_t *bytes) {
    uint8_t data_loc = 0;
    uint8_t found = 0;

    *bytes = 0;
    index++;
    found = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID16, index, &data_loc);
    if(found == index) {
        *bytes = 2;
        return data_loc;
    }

    index -= found;
    found = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID32, index, &data_loc);
    if(found == index) {
        *bytes = 4;
        return data_loc;
    }

    index -= found;
    found = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID128, index, &data_loc);
    if(found == index) {
        *bytes = 16;
        return data_loc;
    }

    return 0xFF;
}


/**
 * @brief Get the count of advertised service data UUIDS
 * @return The number of service data UUIDS in the vector.
 */
size_t NimBLEAdvertisedDevice::getServiceDataCount() {
    uint8_t count = 0;

    count += findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID16);
    count += findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID32);
    count += findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID128);

    return count;
} // getServiceDataCount


/**
 * @brief Get the Service UUID.
 * @param [in] index The index of the service UUID requested.
 * @return The Service UUID of the advertised service, or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceUUID(uint8_t index) {
    uint8_t count = 0;
    uint8_t data_loc = 0;
    uint8_t uuidBytes = 0;
    uint8_t type = BLE_HS_ADV_TYPE_INCOMP_UUIDS16;
    ble_hs_adv_field *field = nullptr;

    index++;

    do {
        count = findAdvField(type, index, &data_loc);
        if(count >= index) {
            if(type < BLE_HS_ADV_TYPE_INCOMP_UUIDS32) {
                uuidBytes = 2;
            } else if(type < BLE_HS_ADV_TYPE_INCOMP_UUIDS128) {
                uuidBytes = 4;
            } else {
                uuidBytes = 16;
            }
            break;

        } else {
            type++;
            index -= count;
        }

    } while(type <= BLE_HS_ADV_TYPE_COMP_UUIDS128);

    if(uuidBytes > 0) {
        field = (ble_hs_adv_field *)&m_payload[data_loc];
        // In the case of more than one field of service uuid's we need to adjust
        // the index to account for the uuids of the previous fields.
        if(field->length < index * uuidBytes) {
            index -= count - field->length / uuidBytes;
        }

        if(field->length > uuidBytes * index) {
            return NimBLEUUID(field->value + uuidBytes * (index - 1), uuidBytes, false);
        }
    }

    return NimBLEUUID("");
} // getServiceUUID


/**
 * @brief Get the number of services advertised
 * @return The count of services in the advertising packet.
 */
size_t NimBLEAdvertisedDevice::getServiceUUIDCount() {
    uint8_t count = 0;

    count += findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS16);
    count += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS16);
    count += findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS32);
    count += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS32);
    count += findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS128);
    count += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS128);

    return count;
} // getServiceUUIDCount


/**
 * @brief Check advertised services for existance of the required UUID
 * @param [in] uuid The service uuid to look for in the advertisement.
 * @return Return true if service is advertised
 */
bool NimBLEAdvertisedDevice::isAdvertisingService(const NimBLEUUID &uuid) {
    size_t count = getServiceUUIDCount();
    for(size_t i = 0; i < count; i++) {
        if(uuid == getServiceUUID(i)) {
            return true;
        }
    }

    return false;
} // isAdvertisingService


/**
 * @brief Get the TX Power.
 * @return The TX Power of the advertised device.
 */
int8_t NimBLEAdvertisedDevice::getTXPower() {
    uint8_t data_loc = 0;

    if(findAdvField(BLE_HS_ADV_TYPE_TX_PWR_LVL, 0, &data_loc) > 0) {
        ble_hs_adv_field *field = (ble_hs_adv_field *)&m_payload[data_loc];
        if(field->length == BLE_HS_ADV_TX_PWR_LVL_LEN + 1) {
            return *(int8_t*)field->value;
        }
    }

    return -99;
} // getTXPower


/**
 * @brief Does this advertisement have preferred connection parameters?
 * @return True if connection parameters are present.
 */
bool NimBLEAdvertisedDevice::haveConnParams() {
    return findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE) > 0;
} // haveConnParams


/**
 * @brief Does this advertisement have have the advertising interval?
 * @return True if the advertisement interval is present.
 */
bool NimBLEAdvertisedDevice::haveAdvInterval() {
    return findAdvField(BLE_HS_ADV_TYPE_ADV_ITVL) > 0;
} // haveAdvInterval


/**
 * @brief Does this advertisement have an appearance value?
 * @return True if there is an appearance value present.
 */
bool NimBLEAdvertisedDevice::haveAppearance() {
    return findAdvField(BLE_HS_ADV_TYPE_APPEARANCE) > 0;
} // haveAppearance


/**
 * @brief Does this advertisement have manufacturer data?
 * @return True if there is manufacturer data present.
 */
bool NimBLEAdvertisedDevice::haveManufacturerData() {
    return findAdvField(BLE_HS_ADV_TYPE_MFG_DATA) > 0;
} // haveManufacturerData


/**
 * @brief Does this advertisement have a URI?
 * @return True if there is a URI present.
 */
bool NimBLEAdvertisedDevice::haveURI() {
    return findAdvField(BLE_HS_ADV_TYPE_URI) > 0;
} // haveURI


/**
 * @brief Does the advertisement contain a target address?
 * @return True if an address is present.
 */
bool NimBLEAdvertisedDevice::haveTargetAddress() {
    return findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR) > 0 ||
           findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR) > 0;
}


/**
 * @brief Does this advertisement have a name value?
 * @return True if there is a name value present.
 */
bool NimBLEAdvertisedDevice::haveName() {
    return findAdvField(BLE_HS_ADV_TYPE_COMP_NAME) > 0 ||
           findAdvField(BLE_HS_ADV_TYPE_INCOMP_NAME) > 0;
} // haveName


/**
 * @brief Does this advertisement have a signal strength value?
 * @return True if there is a signal strength value present.
 */
bool NimBLEAdvertisedDevice::haveRSSI() {
    return m_rssi != -9999;
} // haveRSSI


/**
 * @brief Does this advertisement have a service data value?
 * @return True if there is a service data value present.
 */
bool NimBLEAdvertisedDevice::haveServiceData() {
    return getServiceDataCount() > 0;
} // haveServiceData


/**
 * @brief Does this advertisement have a service UUID value?
 * @return True if there is a service UUID value present.
 */
bool NimBLEAdvertisedDevice::haveServiceUUID() {
    return getServiceUUIDCount() > 0;
} // haveServiceUUID


/**
 * @brief Does this advertisement have a transmission power value?
 * @return True if there is a transmission power value present.
 */
bool NimBLEAdvertisedDevice::haveTXPower() {
    return findAdvField(BLE_HS_ADV_TYPE_TX_PWR_LVL) > 0;
} // haveTXPower


uint8_t NimBLEAdvertisedDevice::findAdvField(uint8_t type, uint8_t index, uint8_t *data_loc) {
    ble_hs_adv_field *field = nullptr;
    uint8_t data = 0;
    uint8_t length = m_payload.size();
    uint8_t count = 0;

    if(length < 2) {
        return count;
    }

    while (length > 1) {
        field = (ble_hs_adv_field*)&m_payload[data];

        if (field->length >= length) {
            return count;
        }

        if (field->type == type) {
            switch(type) {
                case BLE_HS_ADV_TYPE_INCOMP_UUIDS16:
                case BLE_HS_ADV_TYPE_COMP_UUIDS16:
                    count += field->length / 2;
                    break;

                case BLE_HS_ADV_TYPE_INCOMP_UUIDS32:
                case BLE_HS_ADV_TYPE_COMP_UUIDS32:
                    count += field->length / 4;
                    break;

                case BLE_HS_ADV_TYPE_INCOMP_UUIDS128:
                case BLE_HS_ADV_TYPE_COMP_UUIDS128:
                    count += field->length / 16;
                    break;

                case BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR:
                case BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR:
                    count += field->length / 6;
                    break;

                default:
                    count++;
                    break;
            }

            if(data_loc != nullptr) {
                if(index == 0 || count >= index) {
                    break;
                }
            }
        }

        length -= 1 + field->length;
        data += 1 + field->length;
    }

    if(data_loc != nullptr && field != nullptr) {
        *data_loc = data;
    }

    return count;
}


/**
 * @brief Set the address of the advertised device.
 * @param [in] address The address of the advertised device.
 */
void NimBLEAdvertisedDevice::setAddress(NimBLEAddress address) {
    m_address = address;
} // setAddress


/**
 * @brief Set the adFlag for this device.
 * @param [in] advType The advertisement flag data from the advertisement.
 */
void NimBLEAdvertisedDevice::setAdvType(uint8_t advType) {
    m_advType = advType;
} // setAdvType


/**
 * @brief Set the RSSI for this device.
 * @param [in] rssi The RSSI of the discovered device.
 */
void NimBLEAdvertisedDevice::setRSSI(int rssi) {
    m_rssi = rssi;
} // setRSSI


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
    return &m_payload[0];
} // getPayload


/**
 * @brief Stores the payload of the advertised device in a vector.
 * @param [in] payload The advertisement payload.
 * @param [in] length The length of the payload in bytes.
 * @param [in] append Indicates if the the data should be appended (scan response).
 */
void NimBLEAdvertisedDevice::setPayload(const uint8_t *payload, uint8_t length, bool append) {
    if(!append) {
        m_advLength = length;
        m_payload.assign(payload, payload + length);
    } else {
        m_payload.insert(m_payload.end(), payload, payload + length);
    }
}


/**
 * @brief Get the length of the advertisement data in the payload.
 * @return The number of bytes in the payload that is from the advertisment.
 */
uint8_t NimBLEAdvertisedDevice::getAdvLength() {
    return m_advLength;
}


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
    return m_payload.size();
} // getPayloadLength

#endif /* CONFIG_BT_ENABLED  && CONFIG_BT_NIMBLE_ROLE_CENTRAL */

