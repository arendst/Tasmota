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

#include "NimBLEAdvertisedDevice.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER

# include "NimBLEDevice.h"
# include "NimBLEUtils.h"
# include "NimBLELog.h"

# include <climits>

static const char* LOG_TAG = "NimBLEAdvertisedDevice";

/**
 * @brief Constructor
 * @param [in] event The advertisement event data.
 */
NimBLEAdvertisedDevice::NimBLEAdvertisedDevice(const ble_gap_event* event, uint8_t eventType)
# if CONFIG_BT_NIMBLE_EXT_ADV
    : m_address{event->ext_disc.addr},
      m_advType{eventType},
      m_rssi{event->ext_disc.rssi},
      m_callbackSent{0},
      m_advLength{event->ext_disc.length_data},
      m_isLegacyAdv{!!(event->ext_disc.props & BLE_HCI_ADV_LEGACY_MASK)},
      m_sid{event->ext_disc.sid},
      m_primPhy{event->ext_disc.prim_phy},
      m_secPhy{event->ext_disc.sec_phy},
      m_periodicItvl{event->ext_disc.periodic_adv_itvl},
      m_payload(event->ext_disc.data, event->ext_disc.data + event->ext_disc.length_data) {
# else
    : m_address{event->disc.addr},
      m_advType{eventType},
      m_rssi{event->disc.rssi},
      m_callbackSent{0},
      m_advLength{event->disc.length_data},
      m_payload(event->disc.data, event->disc.data + event->disc.length_data) {
# endif
} // NimBLEAdvertisedDevice

/**
 * @brief Update the advertisement data.
 * @param [in] event The advertisement event data.
 */
void NimBLEAdvertisedDevice::update(const ble_gap_event* event, uint8_t eventType) {
# if CONFIG_BT_NIMBLE_EXT_ADV
    const auto& disc = event->ext_disc;
    m_isLegacyAdv    = disc.props & BLE_HCI_ADV_LEGACY_MASK;
# else
    const auto& disc = event->disc;
# endif

    m_rssi = disc.rssi;
    if (eventType == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP && isLegacyAdvertisement()) {
        m_payload.insert(m_payload.end(), disc.data, disc.data + disc.length_data);
        return;
    }
    m_advLength = disc.length_data;
    m_payload.assign(disc.data, disc.data + disc.length_data);
    m_callbackSent = 0; // new data, reset callback sent flag
} // update

/**
 * @brief Get the address of the advertising device.
 * @return The address of the advertised device.
 */
const NimBLEAddress& NimBLEAdvertisedDevice::getAddress() const {
    return m_address;
} // getAddress

/**
 * @brief Get the advertisement type.
 * @return The advertising type the device is reporting:
 * * BLE_HCI_ADV_TYPE_ADV_IND            (0) - indirect advertising
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD  (1) - direct advertising - high duty cycle
 * * BLE_HCI_ADV_TYPE_ADV_SCAN_IND       (2) - indirect scan response
 * * BLE_HCI_ADV_TYPE_ADV_NONCONN_IND    (3) - indirect advertising - not connectable
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD  (4) - direct advertising - low duty cycle
 */
uint8_t NimBLEAdvertisedDevice::getAdvType() const {
    return m_advType;
} // getAdvType

/**
 * @brief Get the advertisement flags.
 * @return The advertisement flags, a bitmask of:
 * BLE_HS_ADV_F_DISC_LTD (0x01) - limited discoverability
 * BLE_HS_ADV_F_DISC_GEN (0x02) - general discoverability
 * BLE_HS_ADV_F_BREDR_UNSUP - BR/EDR not supported
 */
uint8_t NimBLEAdvertisedDevice::getAdvFlags() const {
    size_t data_loc;
    if (findAdvField(BLE_HS_ADV_TYPE_FLAGS, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_FLAGS_LEN + 1) {
            return *field->value;
        }
    }

    return 0;
} // getAdvFlags

/**
 * @brief Get the appearance.
 *
 * A %BLE device can declare its own appearance.  The appearance is how it would like to be shown to an end user
 * typically in the form of an icon.
 *
 * @return The appearance of the advertised device.
 */
uint16_t NimBLEAdvertisedDevice::getAppearance() const {
    size_t data_loc;
    if (findAdvField(BLE_HS_ADV_TYPE_APPEARANCE, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_APPEARANCE_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getAppearance

/**
 * @brief Get the advertisement interval.
 * @return The advertisement interval in 0.625ms units.
 */
uint16_t NimBLEAdvertisedDevice::getAdvInterval() const {
    size_t data_loc;
    if (findAdvField(BLE_HS_ADV_TYPE_ADV_ITVL, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_ADV_ITVL_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getAdvInterval

/**
 * @brief Get the preferred min connection interval.
 * @return The preferred min connection interval in 1.25ms units.
 */
uint16_t NimBLEAdvertisedDevice::getMinInterval() const {
    size_t data_loc;
    if (findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1) {
            return *field->value | *(field->value + 1) << 8;
        }
    }

    return 0;
} // getMinInterval

/**
 * @brief Get the preferred max connection interval.
 * @return The preferred max connection interval in 1.25ms units.
 */
uint16_t NimBLEAdvertisedDevice::getMaxInterval() const {
    size_t data_loc;
    if (findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1) {
            return *(field->value + 2) | *(field->value + 3) << 8;
        }
    }

    return 0;
} // getMaxInterval

/**
 * @brief Get the manufacturer data.
 * @param [in] index The index of the of the manufacturer data set to get.
 * @return The manufacturer data.
 */
std::string NimBLEAdvertisedDevice::getManufacturerData(uint8_t index) const {
    return getPayloadByType(BLE_HS_ADV_TYPE_MFG_DATA, index);
} // getManufacturerData

/**
 * @brief Get the count of manufacturer data sets.
 * @return The number of manufacturer data sets.
 */
uint8_t NimBLEAdvertisedDevice::getManufacturerDataCount() const {
    return findAdvField(BLE_HS_ADV_TYPE_MFG_DATA);
} // getManufacturerDataCount

/**
 * @brief Get the URI from the advertisement.
 * @return The URI data.
 */
std::string NimBLEAdvertisedDevice::getURI() const {
    return getPayloadByType(BLE_HS_ADV_TYPE_URI);
} // getURI

/**
 * @brief Get the data from any type available in the advertisement.
 * @param [in] type The advertised data type BLE_HS_ADV_TYPE.
 * @param [in] index The index of the data type.
 * @return The data available under the type `type`.
 */
std::string NimBLEAdvertisedDevice::getPayloadByType(uint16_t type, uint8_t index) const {
    size_t data_loc;
    if (findAdvField(type, index, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length > 1) {
            return std::string((char*)field->value, field->length - 1);
        }
    }

    return "";
} // getPayloadByType

/**
 * @brief Get the advertised name.
 * @return The name of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getName() const {
    return getPayloadByType(BLE_HS_ADV_TYPE_COMP_NAME);
} // getName

/**
 * @brief Get the RSSI.
 * @return The RSSI of the advertised device.
 */
int8_t NimBLEAdvertisedDevice::getRSSI() const {
    return m_rssi;
} // getRSSI

/**
 * @brief Get the scan object that created this advertised device.
 * @return The scan object.
 */
NimBLEScan* NimBLEAdvertisedDevice::getScan() const {
    return NimBLEDevice::getScan();
} // getScan

/**
 * @brief Get the number of target addresses.
 * @return The number of addresses.
 */
uint8_t NimBLEAdvertisedDevice::getTargetAddressCount() const {
    uint8_t count  = findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR);
    count         += findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR);

    return count;
}

/**
 * @brief Get the target address at the index.
 * @param [in] index The index of the target address.
 * @return The target address.
 */
NimBLEAddress NimBLEAdvertisedDevice::getTargetAddress(uint8_t index) const {
    size_t  data_loc = ULONG_MAX;
    uint8_t count    = findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR, index, &data_loc);
    if (count < index + 1) {
        index -= count;
        count  = findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR, index, &data_loc);
    }

    if (count > 0 && data_loc != ULONG_MAX) {
        index++;
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length < index * BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN) {
            // In the case of more than one field of target addresses we need to adjust the index
            index -= count - field->length / BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN;
        }
        if (field->length > index * BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN) {
            return NimBLEAddress{field->value + (index - 1) * BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN, field->type};
        }
    }

    return NimBLEAddress{};
}

/**
 * @brief Get the service data.
 * @param [in] index The index of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(uint8_t index) const {
    uint8_t bytes;
    size_t  data_loc = findServiceData(index, &bytes);
    if (data_loc != ULONG_MAX) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length > bytes) {
            const char* field_data = reinterpret_cast<const char*>(field->value + bytes);
            return std::string(field_data, field->length - bytes - 1);
        }
    }

    return "";
} // getServiceData

/**
 * @brief Get the service data.
 * @param [in] uuid The uuid of the service data requested.
 * @return The advertised service data or empty string if no data.
 */
std::string NimBLEAdvertisedDevice::getServiceData(const NimBLEUUID& uuid) const {
    uint8_t bytes;
    uint8_t index      = 0;
    size_t  data_loc   = findServiceData(index, &bytes);
    size_t  pl_size    = m_payload.size() - 2;
    uint8_t uuid_bytes = uuid.bitSize() / 8;

    while (data_loc < pl_size) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (bytes == uuid_bytes && NimBLEUUID(field->value, bytes) == uuid) {
            const char* field_data = reinterpret_cast<const char*>(field->value + bytes);
            return std::string(field_data, field->length - bytes - 1);
        }

        index++;
        data_loc = findServiceData(index, &bytes);
    }

    NIMBLE_LOGI(LOG_TAG, "No service data found");
    return "";
} // getServiceData

/**
 * @brief Get the UUID of the service data at the index.
 * @param [in] index The index of the service data UUID requested.
 * @return The advertised service data UUID or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceDataUUID(uint8_t index) const {
    uint8_t bytes;
    size_t  data_loc = findServiceData(index, &bytes);
    if (data_loc != ULONG_MAX) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length >= bytes) {
            return NimBLEUUID(field->value, bytes);
        }
    }

    return NimBLEUUID("");
} // getServiceDataUUID

/**
 * @brief Find the service data at the index.
 * @param [in] index The index of the service data to find.
 * @param [in] bytes A pointer to storage for the number of the bytes in the UUID.
 * @return The index in the vector where the data is located, ULONG_MAX if not found.
 */
size_t NimBLEAdvertisedDevice::findServiceData(uint8_t index, uint8_t* bytes) const {
    *bytes = 0;

    size_t  data_loc = 0;
    uint8_t found    = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID16, index, &data_loc);
    if (found > index) {
        *bytes = 2;
        return data_loc;
    }

    index -= found;
    found  = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID32, index, &data_loc);
    if (found > index) {
        *bytes = 4;
        return data_loc;
    }

    index -= found;
    found  = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID128, index, &data_loc);
    if (found > index) {
        *bytes = 16;
        return data_loc;
    }

    return ULONG_MAX;
}

/**
 * @brief Get the count of advertised service data UUIDS
 * @return The number of service data UUIDS in the vector.
 */
uint8_t NimBLEAdvertisedDevice::getServiceDataCount() const {
    uint8_t count  = findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID16);
    count         += findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID32);
    count         += findAdvField(BLE_HS_ADV_TYPE_SVC_DATA_UUID128);

    return count;
} // getServiceDataCount

/**
 * @brief Get the Service UUID.
 * @param [in] index The index of the service UUID requested.
 * @return The Service UUID of the advertised service, or an empty UUID if not found.
 */
NimBLEUUID NimBLEAdvertisedDevice::getServiceUUID(uint8_t index) const {
    uint8_t type       = BLE_HS_ADV_TYPE_INCOMP_UUIDS16;
    size_t  data_loc   = 0;
    uint8_t uuid_bytes = 0;
    uint8_t count      = 0;

    do {
        count = findAdvField(type, index, &data_loc);
        if (count > index) {
            if (type < BLE_HS_ADV_TYPE_INCOMP_UUIDS32) {
                uuid_bytes = 2;
            } else if (type < BLE_HS_ADV_TYPE_INCOMP_UUIDS128) {
                uuid_bytes = 4;
            } else {
                uuid_bytes = 16;
            }
            break;

        } else {
            type++;
            index -= count;
        }

    } while (type <= BLE_HS_ADV_TYPE_COMP_UUIDS128);

    if (uuid_bytes > 0) {
        index++;
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        // In the case of more than one field of service uuid's we need to adjust
        // the index to account for the uuids of the previous fields.
        if (field->length < index * uuid_bytes) {
            index -= count - field->length / uuid_bytes;
        }

        if (field->length > uuid_bytes * index) {
            return NimBLEUUID(field->value + uuid_bytes * (index - 1), uuid_bytes);
        }
    }

    return NimBLEUUID("");
} // getServiceUUID

/**
 * @brief Get the number of services advertised
 * @return The count of services in the advertising packet.
 */
uint8_t NimBLEAdvertisedDevice::getServiceUUIDCount() const {
    uint8_t count  = findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS16);
    count         += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS16);
    count         += findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS32);
    count         += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS32);
    count         += findAdvField(BLE_HS_ADV_TYPE_INCOMP_UUIDS128);
    count         += findAdvField(BLE_HS_ADV_TYPE_COMP_UUIDS128);

    return count;
} // getServiceUUIDCount

/**
 * @brief Check advertised services for existence of the required UUID
 * @param [in] uuid The service uuid to look for in the advertisement.
 * @return Return true if service is advertised
 */
bool NimBLEAdvertisedDevice::isAdvertisingService(const NimBLEUUID& uuid) const {
    size_t count = getServiceUUIDCount();
    for (size_t i = 0; i < count; i++) {
        if (uuid == getServiceUUID(i)) {
            return true;
        }
    }

    return false;
} // isAdvertisingService

/**
 * @brief Get the TX Power.
 * @return The TX Power of the advertised device.
 */
int8_t NimBLEAdvertisedDevice::getTXPower() const {
    size_t data_loc = 0;
    if (findAdvField(BLE_HS_ADV_TYPE_TX_PWR_LVL, 0, &data_loc) > 0) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data_loc]);
        if (field->length == BLE_HS_ADV_TX_PWR_LVL_LEN + 1) {
            return *(int8_t*)field->value;
        }
    }

    return -99;
} // getTXPower

/**
 * @brief Does this advertisement have preferred connection parameters?
 * @return True if connection parameters are present.
 */
bool NimBLEAdvertisedDevice::haveConnParams() const {
    return findAdvField(BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE) > 0;
} // haveConnParams

/**
 * @brief Does this advertisement have have the advertising interval?
 * @return True if the advertisement interval is present.
 */
bool NimBLEAdvertisedDevice::haveAdvInterval() const {
    return findAdvField(BLE_HS_ADV_TYPE_ADV_ITVL) > 0;
} // haveAdvInterval

/**
 * @brief Does this advertisement have an appearance value?
 * @return True if there is an appearance value present.
 */
bool NimBLEAdvertisedDevice::haveAppearance() const {
    return findAdvField(BLE_HS_ADV_TYPE_APPEARANCE) > 0;
} // haveAppearance

/**
 * @brief Does this advertisement have manufacturer data?
 * @return True if there is manufacturer data present.
 */
bool NimBLEAdvertisedDevice::haveManufacturerData() const {
    return findAdvField(BLE_HS_ADV_TYPE_MFG_DATA) > 0;
} // haveManufacturerData

/**
 * @brief Does this advertisement have a URI?
 * @return True if there is a URI present.
 */
bool NimBLEAdvertisedDevice::haveURI() const {
    return findAdvField(BLE_HS_ADV_TYPE_URI) > 0;
} // haveURI

/**
 * @brief Does this advertisement have a adv type `type`?
 * @return True if there is a `type` present.
 */
bool NimBLEAdvertisedDevice::haveType(uint16_t type) const {
    return findAdvField(type) > 0;
}

/**
 * @brief Does the advertisement contain a target address?
 * @return True if an address is present.
 */
bool NimBLEAdvertisedDevice::haveTargetAddress() const {
    return findAdvField(BLE_HS_ADV_TYPE_PUBLIC_TGT_ADDR) > 0 || findAdvField(BLE_HS_ADV_TYPE_RANDOM_TGT_ADDR) > 0;
}

/**
 * @brief Does this advertisement have a name value?
 * @return True if there is a name value present.
 */
bool NimBLEAdvertisedDevice::haveName() const {
    return findAdvField(BLE_HS_ADV_TYPE_COMP_NAME) > 0;
} // haveName

/**
 * @brief Does this advertisement have a service data value?
 * @return True if there is a service data value present.
 */
bool NimBLEAdvertisedDevice::haveServiceData() const {
    return getServiceDataCount() > 0;
} // haveServiceData

/**
 * @brief Does this advertisement have a service UUID value?
 * @return True if there is a service UUID value present.
 */
bool NimBLEAdvertisedDevice::haveServiceUUID() const {
    return getServiceUUIDCount() > 0;
} // haveServiceUUID

/**
 * @brief Does this advertisement have a transmission power value?
 * @return True if there is a transmission power value present.
 */
bool NimBLEAdvertisedDevice::haveTXPower() const {
    return findAdvField(BLE_HS_ADV_TYPE_TX_PWR_LVL) > 0;
} // haveTXPower

# if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Get the set ID of the extended advertisement.
 * @return The set ID.
 */
uint8_t NimBLEAdvertisedDevice::getSetId() const {
    return m_sid;
} // getSetId

/**
 * @brief Get the primary PHY used by this advertisement.
 * @return The PHY type, one of:
 *  * BLE_HCI_LE_PHY_1M
 *  * BLE_HCI_LE_PHY_CODED
 */
uint8_t NimBLEAdvertisedDevice::getPrimaryPhy() const {
    return m_primPhy;
} // getPrimaryPhy

/**
 * @brief Get the primary PHY used by this advertisement.
 * @return The PHY type, one of:
 *  * BLE_HCI_LE_PHY_1M
 *  * BLE_HCI_LE_PHY_2M
 *  * BLE_HCI_LE_PHY_CODED
 */
uint8_t NimBLEAdvertisedDevice::getSecondaryPhy() const {
    return m_secPhy;
} // getSecondaryPhy

/**
 * @brief Get the periodic interval of the advertisement.
 * @return The periodic advertising interval, 0 if not periodic advertising.
 */
uint16_t NimBLEAdvertisedDevice::getPeriodicInterval() const {
    return m_periodicItvl;
} // getPeriodicInterval
# endif

uint8_t NimBLEAdvertisedDevice::findAdvField(uint8_t type, uint8_t index, size_t* data_loc) const {
    size_t  length = m_payload.size();
    size_t  data   = 0;
    uint8_t count  = 0;

    while (length > 2) {
        const ble_hs_adv_field* field = reinterpret_cast<const ble_hs_adv_field*>(&m_payload[data]);
        if (field->length >= length) {
            return count;
        }

        if (field->type == type || (type == BLE_HS_ADV_TYPE_COMP_NAME && field->type == BLE_HS_ADV_TYPE_INCOMP_NAME)) {
            switch (type) {
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

                case BLE_HS_ADV_TYPE_COMP_NAME:
                    // keep looking for complete name, else use this
                    if (data_loc != nullptr && field->type == BLE_HS_ADV_TYPE_INCOMP_NAME) {
                        *data_loc = data;
                        index++;
                    }
                    // fall through
                default:
                    count++;
                    break;
            }

            if (data_loc != nullptr) {
                if (count > index) { // assumes index values default to 0
                    break;
                }
            }
        }

        length -= 1 + field->length;
        data   += 1 + field->length;
    }

    if (data_loc != nullptr && count > index) {
        *data_loc = data;
    }

    return count;
} // findAdvField

/**
 * @brief Create a string representation of this device.
 * @return A string representation of this device.
 */
std::string NimBLEAdvertisedDevice::toString() const {
    std::string res = "Name: " + getName() + ", Address: " + getAddress().toString();

    if (haveAppearance()) {
        char val[6];
        snprintf(val, sizeof(val), "%d", getAppearance());
        res += ", appearance: ";
        res += val;
    }

    if (haveManufacturerData()) {
        auto mfgData  = getManufacturerData();
        res          += ", manufacturer data: ";
        res += NimBLEUtils::dataToHexString(reinterpret_cast<const uint8_t*>(mfgData.data()), mfgData.length());
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

    if (haveServiceData()) {
        uint8_t count  = getServiceDataCount();
        res           += "\nService Data:";
        for (uint8_t i = 0; i < count; i++) {
            res += "\nUUID: " + std::string(getServiceDataUUID(i));
            res += ", Data: " + getServiceData(i);
        }
    }

    return res;

} // toString

/**
 * @brief Get the length of the advertisement data in the payload.
 * @return The number of bytes in the payload that is from the advertisement.
 */
uint8_t NimBLEAdvertisedDevice::getAdvLength() const {
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
uint8_t NimBLEAdvertisedDevice::getAddressType() const {
    return m_address.getType();
} // getAddressType

/**
 * @brief Check if this device is advertising as connectable.
 * @return True if the device is connectable.
 */
bool NimBLEAdvertisedDevice::isConnectable() const {
# if CONFIG_BT_NIMBLE_EXT_ADV
    if (m_isLegacyAdv) {
        return m_advType == BLE_HCI_ADV_RPT_EVTYPE_ADV_IND || m_advType == BLE_HCI_ADV_RPT_EVTYPE_DIR_IND;
    }
# endif
    return (m_advType & BLE_HCI_ADV_CONN_MASK) || (m_advType & BLE_HCI_ADV_DIRECT_MASK);
} // isConnectable

/**
 * @brief Check if this device is advertising as scannable.
 * @return True if the device is scannable.
 */
bool NimBLEAdvertisedDevice::isScannable() const {
    return isLegacyAdvertisement() && (m_advType == BLE_HCI_ADV_TYPE_ADV_IND || m_advType == BLE_HCI_ADV_TYPE_ADV_SCAN_IND);
} // isScannable

/**
 * @brief Check if this advertisement is a legacy or extended type
 * @return True if legacy (Bluetooth 4.x), false if extended (bluetooth 5.x).
 */
bool NimBLEAdvertisedDevice::isLegacyAdvertisement() const {
# if CONFIG_BT_NIMBLE_EXT_ADV
    return m_isLegacyAdv;
# else
    return true;
# endif
} // isLegacyAdvertisement

/**
 * @brief Convenience operator to convert this NimBLEAdvertisedDevice to NimBLEAddress representation.
 * @details This allows passing NimBLEAdvertisedDevice to functions
 * that accept NimBLEAddress and/or or it's methods as a parameter.
 */
NimBLEAdvertisedDevice::operator NimBLEAddress() const {
    NimBLEAddress address(getAddress());
    return address;
} // operator NimBLEAddress

/**
 * @brief Get the payload advertised by the device.
 * @return The advertisement payload.
 */
const std::vector<uint8_t>& NimBLEAdvertisedDevice::getPayload() const {
    return m_payload;
}

/**
 * @brief Get the begin iterator for the payload.
 * @return A read only iterator pointing to the first byte in the payload.
 */
const std::vector<uint8_t>::const_iterator NimBLEAdvertisedDevice::begin() const {
    return m_payload.cbegin();
}

/**
 * @brief Get the end iterator for the payload.
 * @return A read only iterator pointing to one past the last byte of the payload.
 */
const std::vector<uint8_t>::const_iterator NimBLEAdvertisedDevice::end() const {
    return m_payload.cend();
}

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER
