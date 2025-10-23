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

#include "NimBLEExtAdvertising.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_EXT_ADV

# if defined(CONFIG_NIMBLE_CPP_IDF)
#  include "services/gap/ble_svc_gap.h"
# else
#  include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
# endif

# include "NimBLEDevice.h"
# include "NimBLEServer.h"
# include "NimBLEUtils.h"
# include "NimBLELog.h"

static NimBLEExtAdvertisingCallbacks defaultCallbacks;
static const char*                   LOG_TAG = "NimBLEExtAdvertising";

/**
 * @brief Constructor.
 */
NimBLEExtAdvertising::NimBLEExtAdvertising()
    : m_deleteCallbacks{false},
      m_pCallbacks{&defaultCallbacks},
      m_advStatus(CONFIG_BT_NIMBLE_MAX_EXT_ADV_INSTANCES + 1, false) {}

/**
 * @brief Destructor: deletes callback instances if requested.
 */
NimBLEExtAdvertising::~NimBLEExtAdvertising() {
    if (m_deleteCallbacks) {
        delete m_pCallbacks;
    }
}

/**
 * @brief Register the extended advertisement data.
 * @param [in] instId The extended advertisement instance ID to assign to this data.
 * @param [in] adv The extended advertisement instance with the data to set.
 * @return True if advertising started successfully.
 */
bool NimBLEExtAdvertising::setInstanceData(uint8_t instId, NimBLEExtAdvertisement& adv) {
    adv.m_params.sid = instId;

    // Legacy advertising as connectable requires the scannable flag also.
    if (adv.m_params.legacy_pdu && adv.m_params.connectable) {
        adv.m_params.scannable = true;
    }

    // If connectable or not scannable disable the callback for scan response requests
    if (adv.m_params.connectable || !adv.m_params.scannable) {
        adv.m_params.scan_req_notif = false;
    }

# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    NimBLEServer* pServer = NimBLEDevice::getServer();
    if (pServer != nullptr) {
        pServer->start(); // make sure the GATT server is ready before advertising
    }

    int rc = ble_gap_ext_adv_configure(
        instId,
        &adv.m_params,
        NULL,
        (pServer != nullptr) ? NimBLEServer::handleGapEvent : NimBLEExtAdvertising::handleGapEvent,
        NULL);
# else
    int rc = ble_gap_ext_adv_configure(instId, &adv.m_params, NULL, NimBLEExtAdvertising::handleGapEvent, NULL);
# endif

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Advertising config error: rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    os_mbuf* buf;
    buf = os_msys_get_pkthdr(adv.m_payload.size(), 0);
    if (!buf) {
        NIMBLE_LOGE(LOG_TAG, "Data buffer allocation failed");
        return false;
    }

    rc = os_mbuf_append(buf, &adv.m_payload[0], adv.m_payload.size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Unable to copy data: rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    if (adv.m_params.scannable && !adv.m_params.legacy_pdu) {
        rc = ble_gap_ext_adv_rsp_set_data(instId, buf);
    } else {
        rc = ble_gap_ext_adv_set_data(instId, buf);
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Invalid advertisement data: rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    if (!adv.m_advAddress.isNull()) {
        rc = ble_gap_ext_adv_set_addr(instId, adv.m_advAddress.getBase());
    }

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Error setting advertisement address: rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    return true;
} // setInstanceData

/**
 * @brief Set the scan response data for a legacy advertisement.
 * @param [in] instId The extended advertisement instance ID to assign to this data.
 * @param [in] data A reference to a NimBLEExtAdvertisement that contains the data.
 */
bool NimBLEExtAdvertising::setScanResponseData(uint8_t instId, NimBLEExtAdvertisement& data) {
    os_mbuf* buf = os_msys_get_pkthdr(data.m_payload.size(), 0);
    if (!buf) {
        NIMBLE_LOGE(LOG_TAG, "Data buffer allocation failed");
        return false;
    }

    int rc = os_mbuf_append(buf, &data.m_payload[0], data.m_payload.size());
    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Unable to copy scan data: rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    rc = ble_gap_ext_adv_rsp_set_data(instId, buf);
    return (rc == 0);
} // setScanResponseData

/**
 * @brief Start extended advertising.
 * @param [in] instId The extended advertisement instance ID to start.
 * @param [in] duration How long to advertise for in milliseconds, 0 = forever (default).
 * @param [in] maxEvents Maximum number of advertisement events to send, 0 = no limit (default).
 * @return True if advertising started successfully.
 */
bool NimBLEExtAdvertising::start(uint8_t instId, int duration, int maxEvents) {
    // If Host is not synced we cannot start advertising.
    if (!NimBLEDevice::m_synced) {
        NIMBLE_LOGE(LOG_TAG, "Host reset, wait for sync.");
        return false;
    }

    int rc = ble_gap_ext_adv_start(instId, duration / 10, maxEvents);
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "Error enabling advertising; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    m_advStatus[instId] = true;
    return true;
} // start

/**
 * @brief Stop and remove this instance data from the advertisement set.
 * @param [in] instId The extended advertisement instance to stop advertising.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::removeInstance(uint8_t instId) {
    if (stop(instId)) {
        int rc = ble_gap_ext_adv_remove(instId);
        if (rc == 0 || rc == BLE_HS_EALREADY) {
            return true;
        }

        NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_remove rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    return false;
} // removeInstance

/**
 * @brief Stop and remove all advertising instance data.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::removeAll() {
    if (stop()) {
        int rc = ble_gap_ext_adv_clear();
        if (rc == 0 || rc == BLE_HS_EALREADY) {
            return true;
        }

        NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_clear rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
    }

    return false;
} // removeAll

/**
 * @brief Stop advertising this instance data.
 * @param [in] instId The extended advertisement instance to stop advertising.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::stop(uint8_t instId) {
    int rc = ble_gap_ext_adv_stop(instId);
    if (rc == 0 || rc == BLE_HS_EALREADY) {
        m_advStatus[instId] = false;
        return true;
    }

    NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_stop rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
    return false;
} // stop

/**
 * @brief Stop all advertisements.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::stop() {
    int rc = ble_gap_ext_adv_clear();
    if (rc == 0 || rc == BLE_HS_EALREADY) {
        for (auto status : m_advStatus) {
            status = false;
        }
        return true;
    }

    NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_stop rc = %d %s", rc, NimBLEUtils::returnCodeToString(rc));
    return false;
} // stop

/**
 * @brief Set a callback to call when the advertisement stops.
 * @param [in] pCallbacks A pointer to a callback to be invoked when an advertisement stops.
 * @param [in] deleteCallbacks if true callback class will be deleted when advertising is destructed.
 */
void NimBLEExtAdvertising::setCallbacks(NimBLEExtAdvertisingCallbacks* pCallbacks, bool deleteCallbacks) {
    if (pCallbacks != nullptr) {
        m_pCallbacks      = pCallbacks;
        m_deleteCallbacks = deleteCallbacks;
    } else {
        m_pCallbacks      = &defaultCallbacks;
        m_deleteCallbacks = false;
    }
} // setCallbacks

/**
 * @brief Check if currently advertising.
 * @param [in] instId The instance ID of the advertised data to get the status of.
 * @return True if advertising is active.
 */
bool NimBLEExtAdvertising::isActive(uint8_t instId) {
    return m_advStatus[instId];
} // isAdvertising

/**
 * @brief Check if any instances are currently advertising.
 * @return True if any instance is active.
 */
bool NimBLEExtAdvertising::isAdvertising() {
    for (const auto status : m_advStatus) {
        if (status) {
            return true;
        }
    }

    return false;
} // isAdvertising

/*
 * Host reset seems to clear advertising data,
 * we need clear the flag so it reloads it.
 */
void NimBLEExtAdvertising::onHostSync() {
    NIMBLE_LOGD(LOG_TAG, "Host re-synced");
    for (auto status : m_advStatus) {
        status = false;
    }
} // onHostSync

/**
 * @brief Handler for gap events when not using peripheral role.
 * @param [in] event the event data.
 * @param [in] arg pointer to the advertising instance.
 */
int NimBLEExtAdvertising::handleGapEvent(ble_gap_event* event, void* arg) {
    (void)arg;
    NimBLEExtAdvertising* pAdv = NimBLEDevice::getAdvertising();

    switch (event->type) {
        case BLE_GAP_EVENT_ADV_COMPLETE: {
            switch (event->adv_complete.reason) {
                // Don't call the callback if host reset, we want to
                // preserve the active flag until re-sync to restart advertising.
                case BLE_HS_ETIMEOUT_HCI:
                case BLE_HS_EOS:
                case BLE_HS_ECONTROLLER:
                case BLE_HS_ENOTSYNCED:
                    NIMBLE_LOGE(LOG_TAG, "host reset, rc = %d", event->adv_complete.reason);
                    NimBLEDevice::onReset(event->adv_complete.reason);
                    return 0;
                default:
                    break;
            }
            pAdv->m_advStatus[event->adv_complete.instance] = false;
            pAdv->m_pCallbacks->onStopped(pAdv, event->adv_complete.reason, event->adv_complete.instance);
            break;
        } // BLE_GAP_EVENT_ADV_COMPLETE

        case BLE_GAP_EVENT_SCAN_REQ_RCVD: {
            pAdv->m_pCallbacks->onScanRequest(pAdv,
                                              event->scan_req_rcvd.instance,
                                              NimBLEAddress(event->scan_req_rcvd.scan_addr));
            break;
        } // BLE_GAP_EVENT_SCAN_REQ_RCVD
    }

    return 0;
} // handleGapEvent

/* -------------------------------------------------------------------------- */
/*                          Default callback handlers                         */
/* -------------------------------------------------------------------------- */

void NimBLEExtAdvertisingCallbacks::onStopped(NimBLEExtAdvertising* pAdv, int reason, uint8_t instId) {
    NIMBLE_LOGD("NimBLEExtAdvertisingCallbacks", "onStopped: Default");
} // onStopped

void NimBLEExtAdvertisingCallbacks::onScanRequest(NimBLEExtAdvertising* pAdv, uint8_t instId, NimBLEAddress addr) {
    NIMBLE_LOGD("NimBLEExtAdvertisingCallbacks", "onScanRequest: Default");
} // onScanRequest

/* -------------------------------------------------------------------------- */
/*                             Advertisement Data                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Construct a BLE extended advertisement.
 * @param [in] priPhy The primary Phy to advertise on, can be one of:
 * * BLE_HCI_LE_PHY_1M
 * * BLE_HCI_LE_PHY_CODED
 * @param [in] secPhy The secondary Phy to advertise on, can be one of:
 * * BLE_HCI_LE_PHY_1M
 * * BLE_HCI_LE_PHY_2M
 * * BLE_HCI_LE_PHY_CODED
 */
NimBLEExtAdvertisement::NimBLEExtAdvertisement(uint8_t priPhy, uint8_t secPhy) {
    m_params.own_addr_type = NimBLEDevice::m_ownAddrType;
    m_params.primary_phy   = priPhy;
    m_params.secondary_phy = secPhy;
    m_params.tx_power      = NimBLEDevice::getPower(NimBLETxPowerType::Advertise);
} // NimBLEExtAdvertisement

/**
 * @brief Sets wether the advertisement should use legacy (BLE 4.0, 31 bytes max) advertising.
 * @param [in] enable true = using legacy advertising.
 */
void NimBLEExtAdvertisement::setLegacyAdvertising(bool enable) {
    m_params.legacy_pdu = enable;
} // setLegacyAdvertising

/**
 * @brief Sets wether the advertisement has scan response data available.
 * @param [in] enable true = scan response is available.
 */
void NimBLEExtAdvertisement::setScannable(bool enable) {
    m_params.scannable = enable;
} // setScannable

/**
 * @brief Sets the transmission power level for this advertisement.
 * @param [in] dbm the transmission power to use in dbm.
 * @details The allowable value range depends on device hardware.
 */
void NimBLEExtAdvertisement::setTxPower(int8_t dbm) {
    m_params.tx_power = dbm;
} // setTxPower

/**
 * @brief Sets wether this advertisement should advertise as a connectable device.
 * @param [in] enable True = connectable.
 */
void NimBLEExtAdvertisement::setConnectable(bool enable) {
# if CONFIG_BT_NIMBLE_ROLE_PERIPHERAL
    m_params.connectable = enable;
# endif
} // setConnectable

/**
 * @brief Set the address to use for this advertisement.
 * @param [in] addr The address to use.
 */
void NimBLEExtAdvertisement::setAddress(const NimBLEAddress& addr) {
    m_advAddress           = addr;
    // Must use random address type.
    m_params.own_addr_type = BLE_OWN_ADDR_RANDOM;
} // setAddress

/**
 * @brief Sets The primary channels to advertise on.
 * @param [in] ch37 Advertise on channel 37.
 * @param [in] ch38 Advertise on channel 38.
 * @param [in] ch39 Advertise on channel 39.
 * @details This will set a bitmask using the input parameters to allow different \n
 * combinations. If all inputs are false then all 3 channels will be used.
 */
void NimBLEExtAdvertisement::setPrimaryChannels(bool ch37, bool ch38, bool ch39) {
    m_params.channel_map = (ch37 | (ch38 << 1) | (ch39 << 2));
} // setPrimaryChannels

/**
 * @brief Set the filtering for the scan filter.
 * @param [in] scanRequestWhitelistOnly If true, only allow scan requests from those on the white list.
 * @param [in] connectWhitelistOnly If true, only allow connections from those on the white list.
 */
void NimBLEExtAdvertisement::setScanFilter(bool scanRequestWhitelistOnly, bool connectWhitelistOnly) {
    if (!scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_params.filter_policy = BLE_HCI_ADV_FILT_NONE;
        return;
    }
    if (scanRequestWhitelistOnly && !connectWhitelistOnly) {
        m_params.filter_policy = BLE_HCI_ADV_FILT_SCAN;
        return;
    }
    if (!scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_params.filter_policy = BLE_HCI_ADV_FILT_CONN;
        return;
    }
    if (scanRequestWhitelistOnly && connectWhitelistOnly) {
        m_params.filter_policy = BLE_HCI_ADV_FILT_BOTH;
        return;
    }
} // setScanFilter

/**
 * @brief Sets the peer to directly advertise to.
 * @param [in] addr The address of the peer to direct the advertisements.
 */
void NimBLEExtAdvertisement::setDirectedPeer(const NimBLEAddress& addr) {
    m_params.peer = *addr.getBase();
} // setDirectedPeer

/**
 * @brief Enable or disable direct advertisements to the peer set with `NimBLEExtAdvertisement::setDirectedPeer`
 * @param [in] enable true = send directed advertisements to peer.
 * @param [in] high_duty true = use fast advertising rate, default - true.
 */
void NimBLEExtAdvertisement::setDirected(bool enable, bool high_duty) {
    m_params.directed           = enable;
    m_params.high_duty_directed = high_duty;
} // setDirected

/**
 * @brief Set the minimum advertising interval.
 * @param [in] mininterval Minimum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEExtAdvertisement::setMinInterval(uint32_t mininterval) {
    m_params.itvl_min = mininterval;
} // setMinInterval

/**
 * @brief Set the maximum advertising interval.
 * @param [in] maxinterval Maximum value for advertising interval in 0.625ms units, 0 = use default.
 */
void NimBLEExtAdvertisement::setMaxInterval(uint32_t maxinterval) {
    m_params.itvl_max = maxinterval;
} // setMaxInterval

/**
 * @brief Set the primary advertising PHY to use
 * @param [in] phy Can be one of following constants:
 * * BLE_HCI_LE_PHY_1M
 * * BLE_HCI_LE_PHY_CODED
 */
void NimBLEExtAdvertisement::setPrimaryPhy(uint8_t phy) {
    m_params.primary_phy = phy;
} // setPrimaryPhy

/**
 * @brief Set the secondary advertising PHY to use
 * @param [in] phy Can be one of following constants:
 * * BLE_HCI_LE_PHY_1M
 * * BLE_HCI_LE_PHY_2M
 * * BLE_HCI_LE_PHY_CODED
 */
void NimBLEExtAdvertisement::setSecondaryPhy(uint8_t phy) {
    m_params.secondary_phy = phy;
} // setSecondaryPhy

/**
 * @brief Sets whether the advertisement should be anonymous.
 * @param [in] enable Set to true to enable anonymous advertising.
 *
 * @details Anonymous advertising omits the device's address from the advertisement.
 */
void NimBLEExtAdvertisement::setAnonymous(bool enable) {
    m_params.anonymous = enable;
} // setAnonymous

/**
 * @brief Sets whether the scan response request callback should be called.
 * @param [in] enable If true the scan response request callback will be called for this advertisement.
 */
void NimBLEExtAdvertisement::enableScanRequestCallback(bool enable) {
    m_params.scan_req_notif = enable;
} // enableScanRequestCallback

/**
 * @brief Clears the data stored in this instance, does not change settings.
 * @details This will clear all data but preserves advertising parameter settings.
 */
void NimBLEExtAdvertisement::clearData() {
    std::vector<uint8_t>().swap(m_payload);
} // clearData

/**
 * @brief Set the advertisement data.
 * @param [in] data The data to be set as the payload.
 * @param [in] length The size of data.
 * @return True if successful, false if the data is too large.
 * @details This will completely replace any data that was previously set.
 */
bool NimBLEExtAdvertisement::setData(const uint8_t* data, size_t length) {
    if (length > CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN) {
        return false;
    }

    std::vector<uint8_t>(data, data + length).swap(m_payload);
    return true;
} // setData

/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 * @param [in] length The size of data to be added to the payload.
 * @return True if successful, false if the data is too large.
 */
bool NimBLEExtAdvertisement::addData(const uint8_t* data, size_t length) {
    if (m_payload.size() + length > CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN) {
        return false;
    }

    m_payload.insert(m_payload.end(), data, data + length);
    return true;
} // addData

/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 * @return True if successful, false if the data is too large.
 */
bool NimBLEExtAdvertisement::addData(const std::string& data) {
    if (m_payload.size() + data.length() > CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN) {
        return false;
    }

    m_payload.insert(m_payload.end(), data.begin(), data.end());
    return true;
} // addData

/**
 * @brief Set the appearance.
 * @param [in] appearance The appearance code value.
 * @return True if successful.
 * @details If the appearance value is 0 then it will be removed from the advertisement if set previously.
 */
bool NimBLEExtAdvertisement::setAppearance(uint16_t appearance) {
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
 * @return True if successful.
 * @details If the flag value is 0 then it will be removed from the advertisement if set previously.
 */
bool NimBLEExtAdvertisement::setFlags(uint8_t flag) {
    if (flag == 0) {
        removeData(BLE_HS_ADV_TYPE_FLAGS);
        return true;
    }

    uint8_t data[3];
    data[0] = 2;
    data[1] = BLE_HS_ADV_TYPE_FLAGS;
    data[2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
    return addData(data, 3);
} // setFlags

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @param [in] length The length of the data.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setManufacturerData(const uint8_t* data, size_t length) {
    uint8_t header[2];
    header[0] = length + 1;
    header[1] = BLE_HS_ADV_TYPE_MFG_DATA;

    if (addData(header, 2)) {
        return addData(data, length);
    }

    m_payload.erase(m_payload.end() - 2, m_payload.end()); // Remove the header if failed.
    return false;
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setManufacturerData(const std::string& data) {
    return setManufacturerData(reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setManufacturerData

/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setManufacturerData(const std::vector<uint8_t>& data) {
    return setManufacturerData(&data[0], data.size());
} // setManufacturerData

/**
 * @brief Set the URI to advertise.
 * @param [in] uri The uri to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setURI(const std::string& uri) {
    uint8_t header[2];
    header[0] = uri.length() + 1;
    header[1] = BLE_HS_ADV_TYPE_URI;
    if (addData(header, 2)) {
        return addData(reinterpret_cast<const uint8_t*>(uri.data()), uri.length());
    }

    m_payload.erase(m_payload.end() - 2, m_payload.end()); // Remove the header if failed.
    return false;
} // setURI

/**
 * @brief Set the complete name of this device.
 * @param [in] name The name to advertise.
 * @param [in] isComplete If true the name is complete, if false it is shortened.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setName(const std::string& name, bool isComplete) {
    uint8_t header[2];
    header[0] = name.length() + 1;
    header[1] = isComplete ? BLE_HS_ADV_TYPE_COMP_NAME : BLE_HS_ADV_TYPE_INCOMP_NAME;

    if (addData(header, 2)) {
        return addData(reinterpret_cast<const uint8_t*>(name.data()), name.length());
    }

    m_payload.erase(m_payload.end() - 2, m_payload.end()); // Remove the header if failed.
    return false;
} // setName

/**
 * @brief Add a service uuid to exposed list of services.
 * @param [in] serviceUUID The UUID of the service to expose.
 */
bool NimBLEExtAdvertisement::addServiceUUID(const NimBLEUUID& serviceUUID) {
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

    if (length + getDataSize() > CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN) {
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
bool NimBLEExtAdvertisement::addServiceUUID(const char* serviceUUID) {
    return addServiceUUID(NimBLEUUID(serviceUUID));
} // addServiceUUID

/**
 * @brief Remove a service UUID from the advertisement.
 * @param [in] serviceUUID The UUID of the service to remove.
 * @return True if successful or uuid not found, false if uuid error or data could not be reset.
 */
bool NimBLEExtAdvertisement::removeServiceUUID(const NimBLEUUID& serviceUUID) {
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
bool NimBLEExtAdvertisement::removeServiceUUID(const char* serviceUUID) {
    return removeServiceUUID(NimBLEUUID(serviceUUID));
} // removeServiceUUID

/**
 * @brief Remove all service UUIDs from the advertisement.
 */
bool NimBLEExtAdvertisement::removeServices() {
    return true;
} // removeServices

/**
 * @brief Set a single service to advertise as a complete list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setCompleteServices(const NimBLEUUID& uuid) {
    return setServices(true, uuid.bitSize(), {uuid});
} // setCompleteServices

/**
 * @brief Set the complete list of 16 bit services to advertise.
 * @param [in] v_uuid A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setCompleteServices16(const std::vector<NimBLEUUID>& v_uuid) {
    return setServices(true, 16, v_uuid);
} // setCompleteServices16

/**
 * @brief Set the complete list of 32 bit services to advertise.
 * @param [in] v_uuid A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setCompleteServices32(const std::vector<NimBLEUUID>& v_uuid) {
    return setServices(true, 32, v_uuid);
} // setCompleteServices32

/**
 * @brief Set a single service to advertise as a partial list of services.
 * @param [in] uuid The service to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setPartialServices(const NimBLEUUID& uuid) {
    return setServices(false, uuid.bitSize(), {uuid});
} // setPartialServices

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] v_uuid A vector of 16 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setPartialServices16(const std::vector<NimBLEUUID>& v_uuid) {
    return setServices(false, 16, v_uuid);
} // setPartialServices16

/**
 * @brief Set the partial list of services to advertise.
 * @param [in] v_uuid A vector of 32 bit UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setPartialServices32(const std::vector<NimBLEUUID>& v_uuid) {
    return setServices(false, 32, v_uuid);
} // setPartialServices32

/**
 * @brief Utility function to create the list of service UUID's from a vector.
 * @param [in] complete If true the vector is the complete set of services.
 * @param [in] size The bit size of the UUID's in the vector. (16, 32, or 128).
 * @param [in] uuids The vector of service UUID's to advertise.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setServices(bool complete, uint8_t size, const std::vector<NimBLEUUID>& uuids) {
    uint8_t header[2];
    uint8_t uuidBytes = size / 8;
    header[0]         = uuidBytes * uuids.size() + 1;

    switch (size) {
        case 16:
            header[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS16 : BLE_HS_ADV_TYPE_INCOMP_UUIDS16;
            break;
        case 32:
            header[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS32 : BLE_HS_ADV_TYPE_INCOMP_UUIDS32;
            break;
        case 128:
            header[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS128 : BLE_HS_ADV_TYPE_INCOMP_UUIDS128;
            break;
        default:
            NIMBLE_LOGE(LOG_TAG, "Cannot set services, invalid size!");
            return false;
    }

    if (addData(header, 2)) {
        int count = 0;
        for (const auto& uuid : uuids) {
            if (uuid.bitSize() != size) {
                NIMBLE_LOGE(LOG_TAG, "Service UUID(%d) invalid", size);
                continue;
            } else {
                if (addData(uuid.getValue(), uuidBytes)) {
                    count++;
                } else {
                    NIMBLE_LOGE(LOG_TAG, "Error setting service UUIDs");
                    m_payload.erase(m_payload.end() - 2 - (count * uuidBytes), m_payload.end());
                    return false;
                }
            }
        }

        return true;
    }

    return false;
} // setServices

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @param [in] length The length of the data.
 * @note If data length is 0 the service data will not be advertised.
 * @return True if successful, false if data length is too long or could not be set.
 */
bool NimBLEExtAdvertisement::setServiceData(const NimBLEUUID& uuid, const uint8_t* data, size_t length) {
    uint8_t uuidBytes = uuid.bitSize() / 8;
    uint8_t sDataLen  = 2 + uuidBytes + length;

    if (m_payload.size() + sDataLen > CONFIG_BT_NIMBLE_MAX_EXT_ADV_DATA_LEN) {
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
        return true; // removed or not found is still successful
    }

    uint8_t header[2];
    header[0] = uuidBytes + length + 1;
    header[1] = type;

    // already checked the length above, no need to check here
    addData(header, 2);
    addData(uuid.getValue(), uuidBytes);
    addData(data, length);
    return true;
} // setServiceData

/**
 * @brief Set the service data (UUID + data)
 * @param [in] uuid The UUID to set with the service data.
 * @param [in] data The data to be associated with the service data advertised.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEExtAdvertisement::setServiceData(const NimBLEUUID& uuid, const std::string& data) {
    return setServiceData(uuid, reinterpret_cast<const uint8_t*>(data.data()), data.length());
} // setServiceData

/**
 * @brief Set the service data advertised for the UUID.
 * @param [in] uuid The UUID the service data belongs to.
 * @param [in] data The data to advertise.
 * @return True if the service data was set successfully.
 * @note If data length is 0 the service data will not be advertised.
 */
bool NimBLEExtAdvertisement::setServiceData(const NimBLEUUID& uuid, const std::vector<uint8_t>& data) {
    return setServiceData(uuid, &data[0], data.size());
} // setServiceData

/**
 * @brief Set the short name.
 * @param [in] name The short name of the device.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setShortName(const std::string& name) {
    return setName(name, false);
} // setShortName

/**
 * @brief Set the preferred min and max connection intervals to advertise.
 * @param [in] minInterval The minimum preferred connection interval.
 * @param [in] maxInterval The Maximum preferred connection interval.
 * @details  Range = 0x0006(7.5ms) to 0x0C80(4000ms), values not within the range will be limited to this range.
 * @return True if successful.
 */
bool NimBLEExtAdvertisement::setPreferredParams(uint16_t minInterval, uint16_t maxInterval) {
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
 * @brief Adds Tx power level to the advertisement data.
 */
bool NimBLEExtAdvertisement::addTxPower() {
    if (m_params.legacy_pdu) {
        m_params.include_tx_power = 0;
        uint8_t data[3];
        data[0] = BLE_HS_ADV_TX_PWR_LVL_LEN + 1;
        data[1] = BLE_HS_ADV_TYPE_TX_PWR_LVL;
# ifndef CONFIG_IDF_TARGET_ESP32P4
        data[2] = NimBLEDevice::getPower(NimBLETxPowerType::Advertise);
# else
        data[2] = 0;
# endif
        return addData(data, 3);
    }

    m_params.include_tx_power = 1;
    return true;
} // addTxPower

/**
 * @brief Get the location of the data in the payload.
 * @param [in] type The type of data to search for.
 * @return -1 if the data is not found, otherwise the index of the data in the payload.
 */
int NimBLEExtAdvertisement::getDataLocation(uint8_t type) const {
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
bool NimBLEExtAdvertisement::removeData(uint8_t type) {
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
 * @brief Get the size of the current data.
 */
size_t NimBLEExtAdvertisement::getDataSize() const {
    return m_payload.size();
} // getDataSize

/**
 * @brief Get the string representation of the advertisement data.
 * @return The string representation of the advertisement data.
 */
std::string NimBLEExtAdvertisement::toString() const {
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

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_EXT_ADV
