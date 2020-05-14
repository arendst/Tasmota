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
    m_deviceType       = 0;
    m_manufacturerData = "";
    m_name             = "";
    m_rssi             = -9999;
    m_serviceData      = "";
    m_txPower          = 0;
    m_pScan            = nullptr;

    m_haveAppearance       = false;
    m_haveManufacturerData = false;
    m_haveName             = false;
    m_haveRSSI             = false;
    m_haveServiceData      = false;
    m_haveServiceUUID      = false;
    m_haveTXPower          = false;

} // NimBLEAdvertisedDevice


/**
 * @brief Get the address.
 *
 * Every %BLE device exposes an address that is used to identify it and subsequently connect to it.
 * Call this function to obtain the address of the advertised device.
 *
 * @return The address of the advertised device.
 */
NimBLEAddress NimBLEAdvertisedDevice::getAddress() {
    return m_address;
} // getAddress


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
 * @brief Get the name.
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
 * @brief Get the scan object that created this advertisement.
 * @return The scan object.
 */
NimBLEScan* NimBLEAdvertisedDevice::getScan() {
    return m_pScan;
} // getScan


/**
 * @brief Get the service data.
 * @return The ServiceData of the advertised device.
 */
std::string NimBLEAdvertisedDevice::getServiceData() {
    return m_serviceData;
} //getServiceData


/**
 * @brief Get the service data UUID.
 * @return The service data UUID.
 */
 
NimBLEUUID NimBLEAdvertisedDevice::getServiceDataUUID() {
    return m_serviceDataUUID;
} // getServiceDataUUID


/**
 * @brief Get the Service UUID.
 * @return The Service UUID of the advertised device.
 */
 
NimBLEUUID NimBLEAdvertisedDevice::getServiceUUID() {  //TODO Remove it eventually, is no longer useful
    return m_serviceUUIDs[0];
} // getServiceUUID


/**
 * @brief Check advertised serviced for existence required UUID
 * @return Return true if service is advertised
 */
bool NimBLEAdvertisedDevice::isAdvertisingService(NimBLEUUID uuid){
    for (int i = 0; i < m_serviceUUIDs.size(); i++) {
        NIMBLE_LOGI(LOG_TAG, "Comparing UUIDS: %s %s", m_serviceUUIDs[i].toString().c_str(), uuid.toString().c_str());
        if (m_serviceUUIDs[i].equals(uuid)) return true;
    }
    return false;
}


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
 void NimBLEAdvertisedDevice::parseAdvertisement(ble_hs_adv_fields *fields) {
    //char s[BLE_HS_ADV_MAX_SZ];
    uint8_t *u8p;
    uint8_t length;
    int i;

    if (fields->uuids16 != NULL) {
        for (i = 0; i < fields->num_uuids16; i++) {
            setServiceUUID(NimBLEUUID(fields->uuids16[i].value));
        }
    }

    if (fields->uuids32 != NULL) {
        for (i = 0; i < fields->num_uuids32; i++) {
            setServiceUUID(NimBLEUUID(fields->uuids32[i].value));
        }
    }

    if (fields->uuids128 != NULL) {
        for (i = 0; i < fields->num_uuids128; i++) {
            setServiceUUID(NimBLEUUID(&fields->uuids128[i]));
        }
    }

    if (fields->name != NULL) {
        setName(std::string(reinterpret_cast<char*>(fields->name), fields->name_len));
    }

    if (fields->tx_pwr_lvl_is_present) {
        setTXPower(fields->tx_pwr_lvl);
    }

    if (fields->svc_data_uuid16 != NULL) {

        u8p = fields->svc_data_uuid16;
        length = fields->svc_data_uuid16_len;
        
        if (length < 2) {
            NIMBLE_LOGE(LOG_TAG,"Length too small for ESP_BLE_AD_TYPE_SERVICE_DATA");
        }
        else{
            uint16_t uuid = *(uint16_t*)u8p;
            setServiceDataUUID(NimBLEUUID(uuid));
            if (length > 2) {
                setServiceData(std::string(reinterpret_cast<char*>(u8p + 2), length - 2));
            }
        }
    }

    if (fields->svc_data_uuid32 != NULL) {

        u8p = fields->svc_data_uuid16;
        length = fields->svc_data_uuid16_len;
        
        if (length < 4) {
            NIMBLE_LOGE(LOG_TAG,"Length too small for ESP_BLE_AD_TYPE_32SERVICE_DATA");
        }
        
        uint32_t uuid = *(uint32_t*) u8p;
        setServiceDataUUID(NimBLEUUID(uuid));
        if (length > 4) {
            setServiceData(std::string(reinterpret_cast<char*>(u8p + 4), length - 4));
        }
    }
    
    if (fields->svc_data_uuid128 != NULL) {

        u8p = fields->svc_data_uuid16;
        length = fields->svc_data_uuid16_len;
        
        if (length < 16) {
            NIMBLE_LOGE(LOG_TAG,"Length too small for ESP_BLE_AD_TYPE_128SERVICE_DATA");
        }

        setServiceDataUUID(NimBLEUUID(u8p, (size_t)16, false));
        if (length > 16) {
            setServiceData(std::string(reinterpret_cast<char*>(u8p + 16), length - 16));
        }
    }

    if (fields->appearance_is_present) {
        NIMBLE_LOGD(LOG_TAG, "    appearance=0x%04x", fields->appearance);
        setAppearance(fields->appearance);
    }
    
/**** TODO: create storage and fucntions for these parameters
    if (fields->public_tgt_addr != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    public_tgt_addr=");
        u8p = fields->public_tgt_addr;
        for (i = 0; i < fields->num_public_tgt_addrs; i++) {
            NIMBLE_LOGD(LOG_TAG, "public_tgt_addr=%s ", addr_str(u8p));
            u8p += BLE_HS_ADV_PUBLIC_TGT_ADDR_ENTRY_LEN;
        }
        NIMBLE_LOGD(LOG_TAG, "\n");
    }
    
    if (fields->slave_itvl_range != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    slave_itvl_range=");
        print_bytes(fields->slave_itvl_range, BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN);
        NIMBLE_LOGD(LOG_TAG, "\n");
    }

    if (fields->adv_itvl_is_present) {
        NIMBLE_LOGD(LOG_TAG, "    adv_itvl=0x%04x\n", fields->adv_itvl);
    }

    if (fields->uri != NULL) {
        NIMBLE_LOGD(LOG_TAG, "    uri=");
        print_bytes(fields->uri, fields->uri_len);
        NIMBLE_LOGD(LOG_TAG, "\n");
    }
*/
    if (fields->mfg_data != NULL) {
        setManufacturerData(std::string(reinterpret_cast<char*>(fields->mfg_data), fields->mfg_data_len));
    }
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
    NIMBLE_LOGD(LOG_TAG,"- appearance: %d", m_appearance);
} // setAppearance


/**
 * @brief Set the manufacturer data for this device.
 * @param [in] The discovered manufacturer data.
 */
void NimBLEAdvertisedDevice::setManufacturerData(std::string manufacturerData) {
    m_manufacturerData     = manufacturerData;
    m_haveManufacturerData = true;

    char* pHex = NimBLEUtils::buildHexData(nullptr, (uint8_t*) m_manufacturerData.data(), (uint8_t) m_manufacturerData.length());
    NIMBLE_LOGD(LOG_TAG,"- manufacturer data: %s", pHex);
    free(pHex);
} // setManufacturerData


/**
 * @brief Set the name for this device.
 * @param [in] name The discovered name.
 */
void NimBLEAdvertisedDevice::setName(std::string name) {
    m_name     = name;
    m_haveName = true;
    NIMBLE_LOGD(LOG_TAG,"- setName(): name: %s", m_name.c_str());
} // setName


/**
 * @brief Set the RSSI for this device.
 * @param [in] rssi The discovered RSSI.
 */
void NimBLEAdvertisedDevice::setRSSI(int rssi) {
    m_rssi     = rssi;
    m_haveRSSI = true;
    NIMBLE_LOGD(LOG_TAG,"- setRSSI(): rssi: %d", m_rssi);
} // setRSSI


/**
 * @brief Set the Scan that created this advertised device.
 * @param pScan The Scan that created this advertised device.
 */
void NimBLEAdvertisedDevice::setScan(NimBLEScan* pScan) {
    m_pScan = pScan;
} // setScan


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
    m_serviceUUIDs.push_back(serviceUUID);
    m_haveServiceUUID = true;
    NIMBLE_LOGD(LOG_TAG,"- addServiceUUID(): serviceUUID: %s", serviceUUID.toString().c_str());
} // setServiceUUID


/**
 * @brief Set the ServiceData value.
 * @param [in] data ServiceData value.
 */
void NimBLEAdvertisedDevice::setServiceData(std::string serviceData) {
    m_haveServiceData = true;         // Set the flag that indicates we have service data.
    m_serviceData     = serviceData;  // Save the service data that we received.
} //setServiceData


/**
 * @brief Set the ServiceDataUUID value.
 * @param [in] data ServiceDataUUID value.
 */
void NimBLEAdvertisedDevice::setServiceDataUUID(NimBLEUUID uuid) {
    m_haveServiceData = true;         // Set the flag that indicates we have service data.
    m_serviceDataUUID = uuid;
} // setServiceDataUUID


/**
 * @brief Set the power level for this device.
 * @param [in] txPower The discovered power level.
 */
void NimBLEAdvertisedDevice::setTXPower(int8_t txPower) {
    m_txPower     = txPower;
    m_haveTXPower = true;
    NIMBLE_LOGD(LOG_TAG,"- txPower: %d", m_txPower);
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
    
    res += ", advType: " + std::string(NimBLEUtils::advTypeToString(m_advType));
    
    return res;

} // toString


uint8_t* NimBLEAdvertisedDevice::getPayload() {
    return m_payload;
}


uint8_t NimBLEAdvertisedDevice::getAddressType() {
    return m_addressType;
}


void NimBLEAdvertisedDevice::setAddressType(uint8_t type) {
    m_addressType = type;
}


size_t NimBLEAdvertisedDevice::getPayloadLength() {
    return m_payloadLength;
}


void NimBLEAdvertisedDevice::setAdvertisementResult(uint8_t* payload, uint8_t length){
    m_payload = payload;
    m_payloadLength = length;
}

#endif /* CONFIG_BT_ENABLED */

