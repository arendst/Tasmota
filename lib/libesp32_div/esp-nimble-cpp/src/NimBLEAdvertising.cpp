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

#include "NimBLEAdvertising.h"
#if (CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && !CONFIG_BT_NIMBLE_EXT_ADV) || defined(_DOXYGEN_)

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "services/gap/ble_svc_gap.h"
# else
#  include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
# endif
# include "NimBLEDevice.h"
# include "NimBLEServer.h"
# include "NimBLEUtils.h"
# include "NimBLELog.h"

static const char* LOG_TAG = "NimBLEAdvertising";

/**
 * @brief Construct a default advertising object.
 */
NimBLEAdvertising::NimBLEAdvertising()
    : m_advData{},
      m_scanData{},
      m_advParams{},
      m_advCompCb{nullptr},
      m_slaveItvl{0},
      m_duration{BLE_HS_FOREVER},
      m_scanResp{false},
      m_advDataSet{false} {
# if !CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    m_advParams.conn_mode = BLE_GAP_CONN_MODE_NON;
# else
    m_advParams.conn_mode = BLE_GAP_CONN_MODE_UND;
    m_advData.setFlags(BLE_HS_ADV_F_DISC_GEN);
# endif
    m_advParams.disc_mode = BLE_GAP_DISC_MODE_GEN;
} // NimBLEAdvertising

/**
 * @brief Stops the current advertising and resets the advertising data to the default values.
 * @return True if successful.
 */
bool NimBLEAdvertising::reset() {
    if (!stop()) {
        return false;
    }

    *this = NimBLEAdvertising();
    return true;
} // reset

/**
 * @brief Set the type of connectable mode to advertise.
 * @param [in] mode The connectable mode:
 * * BLE_GAP_CONN_MODE_NON    (0) - not connectable advertising
 * * BLE_GAP_CONN_MODE_DIR    (1) - directed connectable advertising
 * * BLE_GAP_CONN_MODE_UND    (2) - undirected connectable advertising
 * @return True if the connectable mode was set, false if the mode is invalid.
 */
bool NimBLEAdvertising::setConnectableMode(uint8_t mode) {
    if (mode > BLE_GAP_CONN_MODE_UND) {
        NIMBLE_LOGE(LOG_TAG, "Invalid connectable mode: %u", mode);
        return false;
    }

    if (mode == BLE_GAP_CONN_MODE_NON) { // Non-connectable advertising doesn't need flags.
        m_advData.setFlags(0);
    }

    m_advParams.conn_mode = mode;
    return true;
} // setAdvertisementType

/**
 * @brief Set the discoverable mode to use.
 * @param [in] mode The discoverable mode:
 * * BLE_GAP_DISC_MODE_NON    (0) - non-discoverable
 * * BLE_GAP_DISC_MODE_LTD    (1) - limited discoverable
 * * BLE_GAP_DISC_MODE_GEN    (2) - general discoverable
 * @return True if the discoverable mode was set, false if the mode is invalid.
 */
bool NimBLEAdvertising::setDiscoverableMode(uint8_t mode) {
    switch (mode) {
        case BLE_GAP_DISC_MODE_NON:
            m_advData.setFlags(BLE_HS_ADV_F_BREDR_UNSUP);
            break;
        case BLE_GAP_DISC_MODE_LTD:
            m_advData.setFlags(BLE_HS_ADV_F_DISC_LTD);
            break;
        case BLE_GAP_DISC_MODE_GEN:
            m_advData.setFlags(BLE_HS_ADV_F_DISC_GEN);
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Invalid discoverable mode: %u", mode);
            return false;
    }

    m_advParams.disc_mode = mode;
    return true;
} // setDiscoverableMode

/**
 * @brief Set the advertising interval.
 * @param [in] interval The advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEAdvertising::setAdvertisingInterval(uint16_t interval) {
    m_advParams.itvl_min = interval;
    m_advParams.itvl_max = interval;
} // setAdvertisingInterval

/**
 * @brief Set the minimum advertising interval.
 * @param [in] minInterval Minimum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEAdvertising::setMinInterval(uint16_t minInterval) {
    m_advParams.itvl_min = minInterval;
} // setMinInterval

/**
 * @brief Set the maximum advertising interval.
 * @param [in] maxInterval Maximum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEAdvertising::setMaxInterval(uint16_t maxInterval) {
    m_advParams.itvl_max = maxInterval;
} // setMaxInterval

/**
 * @brief Enable scan response data.
 * @param [in] enable If true, scan response data will be available, false disabled, default = disabled.
 * @details The scan response data is sent in response to a scan request from a peer device.
 */
void NimBLEAdvertising::enableScanResponse(bool enable) {
    m_scanResp   = enable;
    m_advDataSet = false;
} // enableScanResponse

/**
 * @brief Set the filtering for the scan filter.
 * @param [in] scanRequestWhitelistOnly If true, only allow scan requests from those on the white list.
 * @param [in] connectWhitelistOnly If true, only allow connections from those on the white list.
 */
void NimBLEAdvertising::setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly) {
    if (!scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_NONE;
        return;
    }
    if (scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_SCAN;
        return;
    }
    if (!scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_CONN;
        return;
    }
    if (scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_advParams.filter_policy = BLE_HCI_ADV_FILT_BOTH;
        return;
    }
} // setScanFilter

/**
 * @brief Start advertising.
 * @param [in] duration The duration, in milliseconds, to advertise, 0 == advertise forever.
 * @param [in] dirAddr The address of a peer to directly advertise to.
 * @return True if advertising started successfully.
 */
bool NimBLEAdvertising::start(uint32_t duration, const NimBLEAddress* dirAddr) {
    NIMBLE_LOGD(LOG_TAG,
                ">> Advertising start: duration=%" PRIu32 ", dirAddr=%s",
                duration,
                dirAddr ? dirAddr->toString().c_str() : "NULL");

    if (!NimBLEDevice::m_synced) {
        NIMBLE_LOGE(LOG_TAG, "Host not synced!");
        return false;
    }

    if (ble_gap_adv_active()) {
        NIMBLE_LOGW(LOG_TAG, "Advertising already active");
        return true;
    }

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    NimBLEServer* pServer = NimBLEDevice::getServer();
    if (pServer != nullptr) {
        pServer->start(); // make sure the GATT server is ready before advertising
    }
# endif

    if (!m_advDataSet) {
        if (!setAdvertisementData(m_advData)) {
            return false;
        }

        if (m_scanResp && m_scanData.getPayload().size() > 0) {
            if (!setScanResponseData(m_scanData)) {
                return false;
            }
        }
    }

    // Save the duration incase of host reset so we can restart with the same params
    m_duration = duration;
    if (duration == 0) {
        duration = BLE_HS_FOREVER;
    }

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    int rc = ble_gap_adv_start(NimBLEDevice::m_ownAddrType,
                               (dirAddr != nullptr) ? dirAddr->getBase() : NULL,
                               duration,
                               &m_advParams,
                               (pServer != nullptr) ? NimBLEServer::handleGapEvent : NimBLEAdvertising::handleGapEvent,
                               this);
# else
    int rc =
        ble_gap_adv_start(NimBLEDevice::m_ownAddrType, NULL, duration, &m_advParams, NimBLEAdvertising::handleGapEvent, this);
# endif
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "Error enabling advertising; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "<< Advertising start");
    return true;
} // start

/**
 * @brief Stop advertising.
 * @return True if advertising stopped successfully.
 */
bool NimBLEAdvertising::stop() {
    int rc = ble_gap_adv_stop();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_stop rc=%d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    return true;
} // stop

/**
 * @brief Set the callback to be invoked when advertising stops.
 * @param [in] callback The callback to be invoked when advertising stops.
 */
void NimBLEAdvertising::setAdvertisingCompleteCallback(advCompleteCB_t callback) {
    m_advCompCb = callback;
} // setAdvertisingCompleteCallback

/**
 * @brief Check if currently advertising.
 * @return True if advertising is active.
 */
bool NimBLEAdvertising::isAdvertising() {
    return ble_gap_adv_active();
} // isAdvertising

/*
 * Host reset seems to clear advertising data,
 * we need clear the flag so it reloads it.
 */
void NimBLEAdvertising::onHostSync() {
    m_advDataSet = false;
    // If we were advertising forever, restart it now
    if (m_duration == 0) {
        start(m_duration);
    } else {
        // Otherwise we should tell the app that advertising stopped.
        if (m_advCompCb != nullptr) {
            m_advCompCb(this);
        }
    }
} // onHostSync

/**
 * @brief Handler for gap events when not using peripheral role.
 * @param [in] event the event data.
 * @param [in] arg pointer to the advertising instance.
 */
int NimBLEAdvertising::handleGapEvent(struct ble_gap_event* event, void* arg) {
    NimBLEAdvertising* pAdv = (NimBLEAdvertising*)arg;

    if (event->type == BLE_GAP_EVENT_ADV_COMPLETE) {
        switch (event->adv_complete.reason) {
            // Don't call the callback if host reset, we want to
            // preserve the active flag until re-sync to restart advertising.
            case BLE_HS_ETIMEOUT_HCI:
            case BLE_HS_EOS:
            case BLE_HS_ECONTROLLER:
            case BLE_HS_ENOTSYNCED:
                NIMBLE_LOGE(LOG_TAG, "host reset, rc=%d", event->adv_complete.reason);
                NimBLEDevice::onReset(event->adv_complete.reason);
                return 0;
            default:
                break;
        }

        if (pAdv->m_advCompCb != nullptr) {
            pAdv->m_advCompCb(pAdv);
        }
    }
    return 0;
} // handleGapEvent

/* -------------------------------------------------------------------------- */
/*                             Advertisement Data                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Set the advertisement data that is to be broadcast in a regular advertisement.
 * @param [in] data The data to be broadcast.
 * @return True if the data was set successfully.
 */
bool NimBLEAdvertising::setAdvertisementData(const NimBLEAdvertisementData& data) {
    int rc = ble_gap_adv_set_data(&data.getPayload()[0], data.getPayload().size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_set_data: %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "setAdvertisementData: %s", data.toString().c_str());
    m_advData    = data; // make a copy in the member object in case this is custom.
    m_advDataSet = true; // Set the flag that indicates the data was set already so we don't set it again.
    return true;
} // setAdvertisementData

/**
 * @brief Get the current advertisement data.
 * @returns a reference to the current advertisement data.
 */
const NimBLEAdvertisementData& NimBLEAdvertising::getAdvertisementData() {
    return m_advData;
} // getAdvertisementData

/**
 * @brief Set the data that is to be provided in a scan response.
 * @param [in] data The data to be provided in the scan response
 * @return True if the data was set successfully.
 * @details The scan response data is sent in response to a scan request from a peer device.
 * If this is set without setting the advertisement data when advertising starts this may be overwritten.
 */
bool NimBLEAdvertising::setScanResponseData(const NimBLEAdvertisementData& data) {
    int rc = ble_gap_adv_rsp_set_data(&data.getPayload()[0], data.getPayload().size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_adv_rsp_set_data: %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "setScanResponseData: %s", data.toString().c_str());
    m_scanData = data; // copy the data into the member object in case this is custom.
    return true;
} // setScanResponseData

/**
 * @brief Get the current scan response data.
 * @returns a reference to the current scan response data.
 */
const NimBLEAdvertisementData& NimBLEAdvertising::getScanData() {
    return m_advData;
} // getScanData

/**
 * @brief Clear the advertisement and scan response data and set the flags to BLE_HS_ADV_F_DISC_GEN.
 */
void NimBLEAdvertising::clearData() {
    m_advData.clearData();
    m_advData.setFlags(BLE_HS_ADV_F_DISC_GEN);
    m_scanData.clearData();
    m_advDataSet = false;
} // clearData

/**
 * @brief Refresh advertsing data dynamically without stop/start cycle.
 * For instance allows refreshing manufacturer data dynamically.
 *
 * @return True if the data was set successfully.
 * @details If scan response is enabled it will be refreshed as well.
 */
bool NimBLEAdvertising::refreshAdvertisingData() {
    bool success = setAdvertisementData(m_advData);
    if (m_scanResp) {
        success = setScanResponseData(m_scanData);
    }

    return success;
} // refreshAdvertisingData

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 * @return True if the service was added successfully.
 */
bool NimBLEAdvertising::addServiceUUID(const NimBLEUUID& serviceUUID) {
    if (!m_advData.addServiceUUID(serviceUUID)) {
        if (!m_scanData.addServiceUUID(serviceUUID)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // addServiceUUID

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The string representation of the service to expose.
 * @return True if the service was added successfully.
 */
bool NimBLEAdvertising::addServiceUUID(const char* serviceUUID) {
    return addServiceUUID(NimBLEUUID(serviceUUID));
} // addServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if the service was removed successfully.
 */
bool NimBLEAdvertising::removeServiceUUID(const NimBLEUUID& serviceUUID) {
    bool success = m_advData.removeServiceUUID(serviceUUID);
    success      = m_scanData.removeServiceUUID(serviceUUID);
    m_advDataSet = false;
    return success;
} // removeServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if the service was removed successfully.
 */
bool NimBLEAdvertising::removeServiceUUID(const char* serviceUUID) {
    return removeServiceUUID(NimBLEUUID(serviceUUID));
} // removeServiceUUID

/**
 * @brief Remove all service UUIDs from the advertisement.
 * @return True if the services were removed successfully.
 */
bool NimBLEAdvertising::removeServices() {
    bool success = m_advData.removeServices();
    success = m_advDataSet = m_scanData.removeServices();
    m_advDataSet           = false;
    return success;
} // removeServices

/**
 * @brief Set the device appearance in the advertising data.
 * @param [in] appearance The appearance of the device in the advertising data.
 * If the appearance value is 0 then the appearance will not be in the advertisement.
 * @return True if the appearance was set successfully.
 */
bool NimBLEAdvertising::setAppearance(uint16_t appearance) {
    if (!m_advData.setAppearance(appearance)) {
        if (!m_scanData.setAppearance(appearance)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // setAppearance

/**
 * @brief Set the preferred min and max connection intervals to advertise.
 * @param [in] minInterval The minimum preferred connection interval.
 * @param [in] maxInterval The Maximum preferred connection interval.
 * @return True if the preferred connection interval was set successfully.
 * @details  Range = 0x0006(7.5ms) to 0x0C80(4000ms), values not within the range will be limited to this range.
 */
bool NimBLEAdvertising::setPreferredParams(uint16_t minInterval, uint16_t maxInterval) {
    if (!m_advData.setPreferredParams(minInterval, maxInterval)) {
        if (!m_scanData.setPreferredParams(minInterval, maxInterval)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // setPreferredParams

/**
 * @brief Add the transmission power level to the advertisement packet.
 * @return True if the transmission power level was added successfully.
 */
bool NimBLEAdvertising::addTxPower() {
    if (!m_advData.addTxPower()) {
        if (!m_scanData.addTxPower()) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // addTxPower

/**
 * @brief Set the advertised name of the device.
 * @param [in] name The name to advertise.
 * @return True if the name was set successfully.
 * @note If the name is too long it will be truncated.
 * @details If scan response is enabled the name will be set in the scan response data.
 */
bool NimBLEAdvertising::setName(const std::string& name) {
    if (m_scanResp && m_scanData.setName(name)) {
        m_advDataSet = false;
        return true;
    }

    if (!m_advData.setName(name)) {
        return false;
    }

    m_advDataSet = false;
    return true;
} // setName

/**
 * @brief Set the advertised manufacturer data.
 * @param [in] data The data to advertise.
 * @param [in] length The length of the data.
 * @return True if the manufacturer data was set successfully.
 */
bool NimBLEAdvertising::setManufacturerData(const uint8_t* data, size_t length) {
    if (!m_advData.setManufacturerData(data, length)) {
        if (!m_scanData.setManufacturerData(data, length)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // setManufacturerData

/**
 * @brief Set the advertised manufacturer data.
 * @param [in] data The data to advertise.
 * @return True if the manufacturer data was set successfully.
 */
bool NimBLEAdvertising::setManufacturerData(const std::string& data) {
    return setManufacturerData(reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setManufacturerData

/**
 * @brief Set the advertised manufacturer data.
 * @param [in] data The data to advertise.
 * @return True if the manufacturer data was set successfully.
 */
bool NimBLEAdvertising::setManufacturerData(const std::vector<uint8_t>& data) {
    return setManufacturerData(&data[0], data.size());
} // setManufacturerData

/**
 * @brief Set the advertised URI.
 * @param [in] uri The URI to advertise.
 * @return True if the URI was set successfully.
 */
bool NimBLEAdvertising::setURI(const std::string& uri) {
    if (!m_advData.setURI(uri)) {
        if (!m_scanData.setURI(uri)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // setURI

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @param [in] length The length of the data.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEAdvertising::setServiceData(const NimBLEUUID& uuid, const uint8_t* data, size_t length) {
    if (!m_advData.setServiceData(uuid, data, length)) {
        if (!m_scanData.setServiceData(uuid, data, length)) {
            return false;
        }
    }

    m_advDataSet = false;
    return true;
} // setServiceData

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEAdvertising::setServiceData(const NimBLEUUID& uuid, const std::vector<uint8_t>& data) {
    return setServiceData(uuid, data.data(), data.size());
} // setServiceData

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEAdvertising::setServiceData(const NimBLEUUID& uuid, const std::string& data) {
    return setServiceData(uuid, reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setServiceData

#endif // (CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && !CONFIG_BT_NIMBLE_EXT_ADV) || defined(_DOXYGEN_)
