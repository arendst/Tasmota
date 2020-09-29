/*
 * NimBLEAdvertising.cpp
 *
 *  Created: on March 3, 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEAdvertising.cpp
 *
 * This class encapsulates advertising a BLE Server.
 *  Created on: Jun 21, 2017
 *      Author: kolban
 *
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER)

#include "services/gap/ble_svc_gap.h"
#include "NimBLEAdvertising.h"
#include "NimBLEDevice.h"
#include "NimBLEServer.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEAdvertising";


/**
 * @brief Construct a default advertising object.
 */
NimBLEAdvertising::NimBLEAdvertising() {
    memset(&m_advData, 0, sizeof m_advData);
    memset(&m_scanData, 0, sizeof m_scanData);
    memset(&m_advParams, 0, sizeof m_advParams);
    const char *name = ble_svc_gap_device_name();

    m_advData.name                   = (uint8_t *)name;
    m_advData.name_len               = strlen(name);
    m_advData.name_is_complete       = 1;
    m_scanData.tx_pwr_lvl_is_present = 1;
    m_scanData.tx_pwr_lvl            = NimBLEDevice::getPower();
    m_advData.flags                  = (BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP);
    m_advData.appearance             = 0;
    m_advData.appearance_is_present  = 0;
    m_advData.mfg_data_len           = 0;
    m_advData.mfg_data               = nullptr;

    m_advParams.conn_mode            = BLE_GAP_CONN_MODE_UND;
    m_advParams.disc_mode            = BLE_GAP_DISC_MODE_GEN;
    m_advParams.itvl_min             = 0;
    m_advParams.itvl_max             = 0;

    m_customAdvData                  = false;
    m_customScanResponseData         = false;
    m_scanResp                       = true;
    m_advDataSet                     = false;

} // NimBLEAdvertising


/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
void NimBLEAdvertising::addServiceUUID(const NimBLEUUID &serviceUUID) {
    m_serviceUUIDs.push_back(serviceUUID);
    m_advDataSet = false;
} // addServiceUUID


/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The string representation of the service to expose.
 */
void NimBLEAdvertising::addServiceUUID(const char* serviceUUID) {
    addServiceUUID(NimBLEUUID(serviceUUID));
} // addServiceUUID


/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
void NimBLEAdvertising::removeServiceUUID(const NimBLEUUID &serviceUUID) {
    //m_serviceUUIDs.erase(std::remove_if(m_serviceUUIDs.begin(), m_serviceUUIDs.end(),[serviceUUID](const NimBLEUUID &s) {return serviceUUID == s;}), m_serviceUUIDs.end());
    for(auto it = m_serviceUUIDs.begin(); it != m_serviceUUIDs.end(); ++it) {
        if((*it) == serviceUUID) {
            m_serviceUUIDs.erase(it);
            break;
        }
    }
    m_advDataSet = false;
} // addServiceUUID


/**
 * @brief Set the device appearance in the advertising data.
 * The codes for distinct appearances can be found here:\n
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml.
 * @param [in] appearance The appearance of the device in the advertising data.
 */
void NimBLEAdvertising::setAppearance(uint16_t appearance) {
    m_advData.appearance = appearance;
    m_advData.appearance_is_present = 1;
} // setAppearance


/**
 * @brief Set the type of advertisment to use.
 * @param [in] adv_type:
 * * BLE_HCI_ADV_TYPE_ADV_IND            (0) - indirect advertising
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_HD  (1) - direct advertisng - high duty cycle
 * * BLE_HCI_ADV_TYPE_ADV_SCAN_IND       (2) - indirect scan response
 * * BLE_HCI_ADV_TYPE_ADV_NONCONN_IND    (3) - indirect advertisng - not connectable
 * * BLE_HCI_ADV_TYPE_ADV_DIRECT_IND_LD  (4) - direct advertising - low duty cycle
 */
void NimBLEAdvertising::setAdvertisementType(uint8_t adv_type){
    m_advParams.conn_mode = adv_type;
} // setAdvertisementType


/**
 * @brief Set the minimum advertising interval.
 * @param [in] mininterval Minimum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEAdvertising::setMinInterval(uint16_t mininterval) {
    m_advParams.itvl_min = mininterval;
} // setMinInterval


/**
 * @brief Set the maximum advertising interval.
 * @param [in] maxinterval Maximum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEAdvertising::setMaxInterval(uint16_t maxinterval) {
    m_advParams.itvl_max = maxinterval;
} // setMaxInterval


/**
 * @brief Set if scan response is available.
 * @param [in] set true = scan response available.
 */
void NimBLEAdvertising::setScanResponse(bool set) {
    m_scanResp = set;
} // setScanResponse


/**
 * @brief Set the filtering for the scan filter.
 * @param [in] scanRequestWhitelistOnly If true, only allow scan requests from those on the white list.
 * @param [in] connectWhitelistOnly If true, only allow connections from those on the white list.
 */
void NimBLEAdvertising::setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly) {
    NIMBLE_LOGD(LOG_TAG, ">> setScanFilter: scanRequestWhitelistOnly: %d, connectWhitelistOnly: %d", scanRequestWhitelistOnly, connectWhitelistOnly);
    if (!scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_NONE;
        NIMBLE_LOGD(LOG_TAG, "<< setScanFilter");
        return;
    }
    if (scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_SCAN;
        NIMBLE_LOGD(LOG_TAG, "<< setScanFilter");
        return;
    }
    if (!scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_CONN;
        NIMBLE_LOGD(LOG_TAG, "<< setScanFilter");
        return;
    }
    if (scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_BOTH;
        NIMBLE_LOGD(LOG_TAG, "<< setScanFilter");
        return;
    }
} // setScanFilter


/**
 * @brief Set the advertisement data that is to be published in a regular advertisement.
 * @param [in] advertisementData The data to be advertised.
 * @details The use of this function will replace any data set with addServiceUUID\n
 * or setAppearance. If you wish for these to be advertised you must include them\n
 * in the advertisementData parameter sent.
 */

void NimBLEAdvertising::setAdvertisementData(NimBLEAdvertisementData& advertisementData) {
    NIMBLE_LOGD(LOG_TAG, ">> setAdvertisementData");
    int rc = ble_gap_adv_set_data(
        (uint8_t*)advertisementData.getPayload().data(),
        advertisementData.getPayload().length());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_set_data: %d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }
    m_customAdvData = true;   // Set the flag that indicates we are using custom advertising data.
    NIMBLE_LOGD(LOG_TAG, "<< setAdvertisementData");
} // setAdvertisementData


/**
 * @brief Set the advertisement data that is to be published in a scan response.
 * @param [in] advertisementData The data to be advertised.
 * @details Calling this without also using setAdvertisementData will have no effect.\n
 * When using custom scan response data you must also use custom advertisement data.
 */
void NimBLEAdvertising::setScanResponseData(NimBLEAdvertisementData& advertisementData) {
    NIMBLE_LOGD(LOG_TAG, ">> setScanResponseData");
    int rc = ble_gap_adv_rsp_set_data(
        (uint8_t*)advertisementData.getPayload().data(),
        advertisementData.getPayload().length());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_rsp_set_data: %d %s", rc,  NimBLEUtils::returnCodeToString(rc));
    }
    m_customScanResponseData = true;   // Set the flag that indicates we are using custom scan response data.
    NIMBLE_LOGD(LOG_TAG, "<< setScanResponseData");
} // setScanResponseData


/**
 * @brief Start advertising.
 * @param [in] duration The duration, in seconds, to advertise, 0 == advertise forever.
 * @param [in] advCompleteCB A pointer to a callback to be invoked when advertising ends.
 */
void NimBLEAdvertising::start(uint32_t duration, void (*advCompleteCB)(NimBLEAdvertising *pAdv)) {
    NIMBLE_LOGD(LOG_TAG, ">> Advertising start: customAdvData: %d, customScanResponseData: %d", m_customAdvData, m_customScanResponseData);

    // If Host is not synced we cannot start advertising.
    if(!NimBLEDevice::m_synced) {
        NIMBLE_LOGE(LOG_TAG, "Host reset, wait for sync.");
        return;
    }

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    NimBLEServer* pServer = NimBLEDevice::getServer();
    if(pServer != nullptr) {
        if(!pServer->m_gattsStarted){
            pServer->start();
        } else if(pServer->getConnectedCount() >= NIMBLE_MAX_CONNECTIONS) {
            NIMBLE_LOGW(LOG_TAG, "Max connections reached - not advertising");
            return;
        }
    }
#endif

    // If already advertising just return
    if(ble_gap_adv_active()) {
        return;
    }

    if(duration == 0){
        duration = BLE_HS_FOREVER;
    }
    else{
        duration = duration*1000; // convert duration to milliseconds
    }

    m_advCompCB = advCompleteCB;

    int rc = 0;

    if (!m_customAdvData && !m_advDataSet) {
        //start with 3 bytes for the flags data
        uint8_t payloadLen = 3;

        for(auto &it : m_serviceUUIDs) {
            if(it.getNative()->u.type == BLE_UUID_TYPE_16) {
                int add = (m_advData.num_uuids16 > 0) ? 2 : 4;
                if((payloadLen + add) > 31){
                    m_advData.uuids16_is_complete = 0;
                    continue;
                }
                payloadLen += add;

                if(nullptr == (m_advData.uuids16 = (ble_uuid16_t*)realloc(m_advData.uuids16,
                                                   (m_advData.num_uuids16 + 1) * sizeof(ble_uuid16_t))))
                {
                    NIMBLE_LOGE(LOG_TAG, "Error, no mem");
                    abort();
                }
                memcpy(&m_advData.uuids16[m_advData.num_uuids16].value,
                       &it.getNative()->u16.value, 2);

                m_advData.uuids16[m_advData.num_uuids16].u.type = BLE_UUID_TYPE_16;
                m_advData.uuids16_is_complete = 1;
                m_advData.num_uuids16++;
            }
            if(it.getNative()->u.type == BLE_UUID_TYPE_32) {
                int add = (m_advData.num_uuids32 > 0) ? 4 : 6;
                if((payloadLen + add) > 31){
                    m_advData.uuids32_is_complete = 0;
                    continue;
                }
                payloadLen += add;

                if(nullptr == (m_advData.uuids32 = (ble_uuid32_t*)realloc(m_advData.uuids32,
                                                   (m_advData.num_uuids32 + 1) * sizeof(ble_uuid32_t))))
                {
                    NIMBLE_LOGE(LOG_TAG, "Error, no mem");
                    abort();
                }
                memcpy(&m_advData.uuids32[m_advData.num_uuids32].value,
                       &it.getNative()->u32.value, 4);

                m_advData.uuids32[m_advData.num_uuids32].u.type = BLE_UUID_TYPE_32;
                m_advData.uuids32_is_complete = 1;
                m_advData.num_uuids32++;
            }
            if(it.getNative()->u.type == BLE_UUID_TYPE_128){
                int add = (m_advData.num_uuids128 > 0) ? 16 : 18;
                if((payloadLen + add) > 31){
                    m_advData.uuids128_is_complete = 0;
                    continue;
                }
                payloadLen += add;

                if(nullptr == (m_advData.uuids128 = (ble_uuid128_t*)realloc(m_advData.uuids128,
                              (m_advData.num_uuids128 + 1) * sizeof(ble_uuid128_t))))
                {
                    NIMBLE_LOGE(LOG_TAG, "Error, no mem");
                    abort();
                }
                memcpy(&m_advData.uuids128[m_advData.num_uuids128].value,
                       &it.getNative()->u128.value, 16);

                m_advData.uuids128[m_advData.num_uuids128].u.type = BLE_UUID_TYPE_128;
                m_advData.uuids128_is_complete = 1;
                m_advData.num_uuids128++;
            }
        }

        // check if there is room for the name, if not put it in scan data
        if((payloadLen + m_advData.name_len) > 29) {
            if(m_scanResp){
                m_scanData.name = m_advData.name;
                m_scanData.name_len = m_advData.name_len;
                m_scanData.name_is_complete = m_advData.name_is_complete;
                m_advData.name = nullptr;
                m_advData.name_len = 0;
            } else {
                // if not using scan response just cut the name down
                // leaving 2 bytes for the data specifier.
                m_advData.name_len = (29 - payloadLen);
            }
            m_advData.name_is_complete = 0;
        }

        if(m_advData.name_len > 0) {
            payloadLen += (m_advData.name_len + 2);
        }

        if(m_scanResp) {
            // name length + type byte + length byte + tx power type + length + data
            if((m_scanData.name_len + 5) > 31) {
                // prioritize name data over tx power
                m_scanData.tx_pwr_lvl_is_present = 0;
                m_scanData.tx_pwr_lvl = 0;
                // limit name to 29 to leave room for the data specifiers
                if(m_scanData.name_len > 29) {
                    m_scanData.name_len = 29;
                    m_scanData.name_is_complete = false;
                }
            }

            rc = ble_gap_adv_rsp_set_fields(&m_scanData);
            if (rc != 0) {
                NIMBLE_LOGC(LOG_TAG, "error setting scan response data; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
                abort();
            }
        // if not using scan response and there is room,
        // put the tx power data into the advertisment
        } else if (payloadLen < 29) {
            m_advData.tx_pwr_lvl_is_present = 1;
            m_advData.tx_pwr_lvl = NimBLEDevice::getPower();
        }

        rc = ble_gap_adv_set_fields(&m_advData);
        if (rc != 0) {
            NIMBLE_LOGC(LOG_TAG, "error setting advertisement data; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
            abort();
        }

        if(m_advData.num_uuids128 > 0) {
            free(m_advData.uuids128);
            m_advData.uuids128 = nullptr;
            m_advData.num_uuids128 = 0;
        }

        if(m_advData.num_uuids32 > 0) {
            free(m_advData.uuids32);
            m_advData.uuids32 = nullptr;
            m_advData.num_uuids32 = 0;
        }

        if(m_advData.num_uuids16 > 0) {
            free(m_advData.uuids16);
            m_advData.uuids16 = nullptr;
            m_advData.num_uuids16 = 0;
        }

        m_advDataSet = true;
    }

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    rc = ble_gap_adv_start(0, NULL, duration,
                           &m_advParams,
                           (pServer != nullptr) ? NimBLEServer::handleGapEvent : NimBLEAdvertising::handleGapEvent,
                           (pServer != nullptr) ? (void*)pServer : (void*)this);
#else
    rc = ble_gap_adv_start(0, NULL, duration,
                           &m_advParams, NimBLEAdvertising::handleGapEvent, this);
#endif
    if (rc != 0) {
        NIMBLE_LOGC(LOG_TAG, "Error enabling advertising; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        abort();
    }

    NIMBLE_LOGD(LOG_TAG, "<< Advertising start");
} // start


/**
 * @brief Stop advertising.
 */
void NimBLEAdvertising::stop() {
    NIMBLE_LOGD(LOG_TAG, ">> stop");
    int rc = ble_gap_adv_stop();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_stop rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return;
    }

    NIMBLE_LOGD(LOG_TAG, "<< stop");
} // stop


/**
 * @brief Handles the callback when advertising stops.
 */
void NimBLEAdvertising::advCompleteCB() {
    if(m_advCompCB != nullptr) {
        m_advCompCB(this);
    }
}


/**
 * @brief Check if currently advertising.
 * @return true if advertising is active.
 */
bool NimBLEAdvertising::isAdvertising() {
    return ble_gap_adv_active();
}


/*
 * Host reset seems to clear advertising data,
 * we need clear the flag so it reloads it.
 */
void NimBLEAdvertising::onHostReset() {
    m_advDataSet = false;
}


/**
 * @brief Handler for gap events when not using peripheral role.
 * @param [in] event the event data.
 * @param [in] arg pointer to the advertising instance.
 */
/*STATIC*/
int NimBLEAdvertising::handleGapEvent(struct ble_gap_event *event, void *arg) {
    NimBLEAdvertising *pAdv = (NimBLEAdvertising*)arg;

    if(event->type == BLE_GAP_EVENT_ADV_COMPLETE) {
        pAdv->advCompleteCB();
    }
    return 0;
}


/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 */
void NimBLEAdvertisementData::addData(const std::string &data) {
    if ((m_payload.length() + data.length()) > BLE_HS_ADV_MAX_SZ) {
        return;
    }
    m_payload.append(data);
} // addData


/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 * @param [in] length The size of data to be added to the payload.
 */
void NimBLEAdvertisementData::addData(char * data, size_t length){
    if ((m_payload.length() + length) > BLE_HS_ADV_MAX_SZ) {
        return;
    }
    m_payload.append(data,length);
} // addData


/**
 * @brief Set the appearance.
 * @param [in] appearance The appearance code value.
 *
 * See also:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
 */
void NimBLEAdvertisementData::setAppearance(uint16_t appearance) {
    char cdata[2];
    cdata[0] = 3;
    cdata[1] = BLE_HS_ADV_TYPE_APPEARANCE; // 0x19
    addData(std::string(cdata, 2) + std::string((char*) &appearance, 2));
} // setAppearance


/**
 * @brief Set the complete services to advertise.
 * @param [in] uuid The UUID of the service.
 */
void NimBLEAdvertisementData::setCompleteServices(const NimBLEUUID &uuid) {
    char cdata[2];
    switch (uuid.bitSize()) {
        case 16: {
            // [Len] [0x02] [LL] [HH]
            cdata[0] = 3;
            cdata[1] = BLE_HS_ADV_TYPE_COMP_UUIDS16;  // 0x03
            addData(std::string(cdata, 2) + std::string((char*) &uuid.getNative()->u16.value, 2));
            break;
        }

        case 32: {
            // [Len] [0x04] [LL] [LL] [HH] [HH]
            cdata[0] = 5;
            cdata[1] = BLE_HS_ADV_TYPE_COMP_UUIDS32;  // 0x05
            addData(std::string(cdata, 2) + std::string((char*) &uuid.getNative()->u32.value, 4));
            break;
        }

        case 128: {
            // [Len] [0x04] [0] [1] ... [15]
            cdata[0] = 17;
            cdata[1] = BLE_HS_ADV_TYPE_COMP_UUIDS128;  // 0x07
            addData(std::string(cdata, 2) + std::string((char*) uuid.getNative()->u128.value, 16));
            break;
        }

        default:
            return;
    }
} // setCompleteServices


/**
 * @brief Set the advertisement flags.
 * @param [in] flag The flags to be set in the advertisement.
 * * BLE_HS_ADV_F_DISC_LTD
 * * BLE_HS_ADV_F_DISC_GEN
 * * BLE_HS_ADV_F_BREDR_UNSUP - must always use with NimBLE
 */
void NimBLEAdvertisementData::setFlags(uint8_t flag) {
    char cdata[3];
    cdata[0] = 2;
    cdata[1] = BLE_HS_ADV_TYPE_FLAGS;  // 0x01
    cdata[2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
    addData(std::string(cdata, 3));
} // setFlag


/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 */
void NimBLEAdvertisementData::setManufacturerData(const std::string &data) {
    NIMBLE_LOGD("NimBLEAdvertisementData", ">> setManufacturerData");
    char cdata[2];
    cdata[0] = data.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_MFG_DATA ;  // 0xff
    addData(std::string(cdata, 2) + data);
    NIMBLE_LOGD("NimBLEAdvertisementData", "<< setManufacturerData");
} // setManufacturerData


/**
 * @brief Set the complete name of this device.
 * @param [in] name The name to advertise.
 */
void NimBLEAdvertisementData::setName(const std::string &name) {
    NIMBLE_LOGD("NimBLEAdvertisementData", ">> setName: %s", name.c_str());
    char cdata[2];
    cdata[0] = name.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_COMP_NAME;  // 0x09
    addData(std::string(cdata, 2) + name);
    NIMBLE_LOGD("NimBLEAdvertisementData", "<< setName");
} // setName


/**
 * @brief Set the partial services to advertise.
 * @param [in] uuid The single service to advertise.
 */
void NimBLEAdvertisementData::setPartialServices(const NimBLEUUID &uuid) {
    char cdata[2];
    switch (uuid.bitSize()) {
        case 16: {
            // [Len] [0x02] [LL] [HH]
            cdata[0] = 3;
            cdata[1] =  BLE_HS_ADV_TYPE_INCOMP_UUIDS16;  // 0x02
            addData(std::string(cdata, 2) + std::string((char *) &uuid.getNative()->u16.value, 2));
            break;
        }

        case 32: {
            // [Len] [0x04] [LL] [LL] [HH] [HH]
            cdata[0] = 5;
            cdata[1] =  BLE_HS_ADV_TYPE_INCOMP_UUIDS32; // 0x04
            addData(std::string(cdata, 2) + std::string((char *) &uuid.getNative()->u32.value, 4));
            break;
        }

        case 128: {
            // [Len] [0x04] [0] [1] ... [15]
            cdata[0] = 17;
            cdata[1] = BLE_HS_ADV_TYPE_INCOMP_UUIDS128;  // 0x06
            addData(std::string(cdata, 2) + std::string((char *) &uuid.getNative()->u128.value, 16));
            break;
        }

        default:
            return;
    }
} // setPartialServices


/**
 * @brief Set the service data (UUID + data)
 * @param [in] uuid The UUID to set with the service data.
 * @param [in] data The data to be associated with the service data advertised.
 */
void NimBLEAdvertisementData::setServiceData(const NimBLEUUID &uuid, const std::string &data) {
    char cdata[2];
    switch (uuid.bitSize()) {
        case 16: {
            // [Len] [0x16] [UUID16] data
            cdata[0] = data.length() + 3;
            cdata[1] = BLE_HS_ADV_TYPE_SVC_DATA_UUID16;  // 0x16
            addData(std::string(cdata, 2) + std::string((char*) &uuid.getNative()->u16.value, 2) + data);
            break;
        }

        case 32: {
            // [Len] [0x20] [UUID32] data
            cdata[0] = data.length() + 5;
            cdata[1] = BLE_HS_ADV_TYPE_SVC_DATA_UUID32; // 0x20
            addData(std::string(cdata, 2) + std::string((char*) &uuid.getNative()->u32.value, 4) + data);
            break;
        }

        case 128: {
            // [Len] [0x21] [UUID128] data
            cdata[0] = data.length() + 17;
            cdata[1] = BLE_HS_ADV_TYPE_SVC_DATA_UUID128;  // 0x21
            addData(std::string(cdata, 2) + std::string((char*) &uuid.getNative()->u128.value, 16) + data);
            break;
        }

        default:
            return;
    }
} // setServiceData


/**
 * @brief Set the short name.
 * @param [in] name The short name of the device.
 */
void NimBLEAdvertisementData::setShortName(const std::string &name) {
    NIMBLE_LOGD("NimBLEAdvertisementData", ">> setShortName: %s", name.c_str());
    char cdata[2];
    cdata[0] = name.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_INCOMP_NAME;  // 0x08
    addData(std::string(cdata, 2) + name);
    NIMBLE_LOGD("NimBLEAdvertisementData", "<< setShortName");
} // setShortName


/**
 * @brief Retrieve the payload that is to be advertised.
 * @return The payload that is to be advertised.
 */
std::string NimBLEAdvertisementData::getPayload() {
    return m_payload;
} // getPayload

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
#endif /* CONFIG_BT_ENABLED */
