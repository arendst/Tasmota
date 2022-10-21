/*
 * NimBLEExtAdvertising.cpp
 *
 *  Created: on February 6, 2022
 *      Author H2zero
 */

#include "nimconfig.h"
#if defined(CONFIG_BT_ENABLED) && \
    defined(CONFIG_BT_NIMBLE_ROLE_BROADCASTER) && \
    CONFIG_BT_NIMBLE_EXT_ADV

#if defined(CONFIG_NIMBLE_CPP_IDF)
#include "services/gap/ble_svc_gap.h"
#else
#include "nimble/nimble/host/services/gap/include/services/gap/ble_svc_gap.h"
#endif
#include "NimBLEExtAdvertising.h"
#include "NimBLEDevice.h"
#include "NimBLEServer.h"
#include "NimBLEUtils.h"
#include "NimBLELog.h"

static NimBLEExtAdvertisingCallbacks defaultCallbacks;
static const char* LOG_TAG = "NimBLEExtAdvertising";


/**
 * @brief Destructor: deletes callback instances if requested.
 */
NimBLEExtAdvertising::~NimBLEExtAdvertising() {
    if(m_deleteCallbacks && m_pCallbacks != &defaultCallbacks) {
        delete m_pCallbacks;
    }
}


/**
 * @brief Register the extended advertisement data.
 * @param [in] inst_id The extended advertisement instance ID to assign to this data.
 * @param [in] adv The extended advertisement instance with the data to set.
 * @return True if advertising started successfully.
 */
bool NimBLEExtAdvertising::setInstanceData(uint8_t inst_id, NimBLEExtAdvertisement& adv) {
    adv.m_params.sid = inst_id;

    // Legacy advertising as connectable requires the scannable flag also.
    if (adv.m_params.legacy_pdu && adv.m_params.connectable) {
        adv.m_params.scannable = true;
    }

    // If connectable or not scannable disable the callback for scan response requests
    if (adv.m_params.connectable || !adv.m_params.scannable) {
        adv.m_params.scan_req_notif = false;
    }

#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    NimBLEServer* pServer = NimBLEDevice::getServer();
    if (pServer != nullptr) {
        if (!pServer->m_gattsStarted) {
            pServer->start();
        }
    }

    int rc = ble_gap_ext_adv_configure(inst_id,
                                       &adv.m_params,
                                       NULL,
                                       (pServer != nullptr) ? NimBLEServer::handleGapEvent :
                                                              NimBLEExtAdvertising::handleGapEvent,
                                       NULL);
#else
    int rc = ble_gap_ext_adv_configure(inst_id,
                                       &data.m_params,
                                       NULL,
                                       NimBLEExtAdvertising::handleGapEvent,
                                       NULL);
#endif

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Advertising config error: rc = %d", rc);
    } else {
        os_mbuf *buf;
        buf = os_msys_get_pkthdr(adv.m_payload.size(), 0);
        if (!buf) {
            NIMBLE_LOGE(LOG_TAG, "Data buffer allocation failed");
            return false;
        }

        rc = os_mbuf_append(buf, &adv.m_payload[0], adv.m_payload.size());
        if (rc != 0) {
            NIMBLE_LOGE(LOG_TAG, "Unable to copy data: rc = %d", rc);
            return false;
        } else {
            if (adv.m_params.scannable && !adv.m_params.legacy_pdu) {
                rc = ble_gap_ext_adv_rsp_set_data(inst_id, buf);
            } else {
                rc = ble_gap_ext_adv_set_data(inst_id, buf);
            }

            if (rc != 0) {
                NIMBLE_LOGE(LOG_TAG, "Invalid advertisement data: rc = %d", rc);
            } else {
                if (adv.m_advAddress != NimBLEAddress("")) {
                    ble_addr_t addr;
                    memcpy(&addr.val, adv.m_advAddress.getNative(), 6);
                    // Custom advertising address must be random.
                    addr.type = BLE_OWN_ADDR_RANDOM;
                    rc = ble_gap_ext_adv_set_addr(inst_id, &addr);
                }

                if (rc != 0) {
                    NIMBLE_LOGE(LOG_TAG, "Error setting advertisement address: rc = %d", rc);
                    return false;
                }
            }
        }
    }

    return (rc == 0);
}


/**
 * @brief Set the scan response data for a legacy advertisement.
 * @param [in] inst_id The extended advertisement instance ID to assign to this data.
 * @param [in] lsr A reference to a NimBLEExtAdvertisement that contains the data.
 */
bool NimBLEExtAdvertising::setScanResponseData(uint8_t inst_id, NimBLEExtAdvertisement & lsr) {
    os_mbuf *buf = os_msys_get_pkthdr(lsr.m_payload.size(), 0);
    if (!buf) {
        NIMBLE_LOGE(LOG_TAG, "Data buffer allocation failed");
        return false;
    }

    int rc = os_mbuf_append(buf, &lsr.m_payload[0], lsr.m_payload.size());

    if (rc != 0) {
        NIMBLE_LOGE(LOG_TAG, "Unable to copy scan data: rc = %d", rc);
        return false;
    } else {
        rc = ble_gap_ext_adv_rsp_set_data(inst_id, buf);
    }
    return (rc == 0);
}


/**
 * @brief Start extended advertising.
 * @param [in] inst_id The extended advertisement instance ID to start.
 * @param [in] duration How long to advertise for in milliseconds, 0 = forever (default).
 * @param [in] max_events Maximum number of advertisement events to send, 0 = no limit (default).
 * @return True if advertising started successfully.
 */
bool NimBLEExtAdvertising::start(uint8_t inst_id, int duration, int max_events) {
    NIMBLE_LOGD(LOG_TAG, ">> Extended Advertising start");

    // If Host is not synced we cannot start advertising.
    if(!NimBLEDevice::m_synced) {
        NIMBLE_LOGE(LOG_TAG, "Host reset, wait for sync.");
        return false;
    }

    int rc = ble_gap_ext_adv_start(inst_id, duration / 10, max_events);

    switch (rc) {
        case 0:
             m_advStatus[inst_id] = true;
             break;

        case BLE_HS_EINVAL:
             NIMBLE_LOGE(LOG_TAG, "Unable to advertise - Value Error");
             break;

        case BLE_HS_EALREADY:
             NIMBLE_LOGI(LOG_TAG, "Advertisement Already active");
             break;

        case BLE_HS_ETIMEOUT_HCI:
        case BLE_HS_EOS:
        case BLE_HS_ECONTROLLER:
        case BLE_HS_ENOTSYNCED:
             NIMBLE_LOGE(LOG_TAG, "Unable to advertise - Host Reset");
             break;

        default:
            NIMBLE_LOGE(LOG_TAG, "Error enabling advertising; rc=%d, %s",
                        rc, NimBLEUtils::returnCodeToString(rc));
            break;
    }

    NIMBLE_LOGD(LOG_TAG, "<< Extended Advertising start");
    return (rc == 0 || rc == BLE_HS_EALREADY);
} // start


/**
 * @brief Stop and remove this instance data from the advertisement set.
 * @param [in] inst_id The extended advertisement instance to stop advertising.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::removeInstance(uint8_t inst_id) {
    if (stop(inst_id)) {
        int rc =  ble_gap_ext_adv_remove(inst_id);
        if (rc != 0 && rc != BLE_HS_EALREADY) {
            NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_remove rc = %d %s",
                        rc, NimBLEUtils::returnCodeToString(rc));
            return false;
        }
        return true;
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
        } else {
            NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_clear rc = %d %s",
                        rc, NimBLEUtils::returnCodeToString(rc));
        }
    }

    return false;
} // removeAll


/**
 * @brief Stop advertising this instance data.
 * @param [in] inst_id The extended advertisement instance to stop advertising.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::stop(uint8_t inst_id) {
    int rc = ble_gap_ext_adv_stop(inst_id);
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_stop rc = %d %s",
                    rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    m_advStatus[inst_id] = false;
    return true;
} // stop


/**
 * @brief Stop all advertisements.
 * @return True if successful.
 */
bool NimBLEExtAdvertising::stop() {
    int rc = ble_gap_ext_adv_clear();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "ble_gap_ext_adv_stop rc = %d %s",
                    rc, NimBLEUtils::returnCodeToString(rc));
        return false;
    }

    for(auto it : m_advStatus) {
        it = false;
    }

    return true;
} // stop


/**
 * @brief Set a callback to call when the advertisement stops.
 * @param [in] pCallbacks A pointer to a callback to be invoked when an advertisement stops.
 * @param [in] deleteCallbacks if true callback class will be deleted when advertising is destructed.
 */
void NimBLEExtAdvertising::setCallbacks(NimBLEExtAdvertisingCallbacks* pCallbacks,
                                        bool deleteCallbacks) {
    if (pCallbacks != nullptr){
        m_pCallbacks = pCallbacks;
        m_deleteCallbacks = deleteCallbacks;
    } else {
        m_pCallbacks = &defaultCallbacks;
    }
} // setCallbacks


/**
 * @brief Check if currently advertising.
 * @param [in] inst_id The instance ID of the advertised data to get the status of.
 * @return True if advertising is active.
 */
bool NimBLEExtAdvertising::isActive(uint8_t inst_id) {
    return m_advStatus[inst_id];
} // isAdvertising


/**
 * @brief Check if any instances are currently advertising.
 * @return True if any instance is active.
 */
bool NimBLEExtAdvertising::isAdvertising() {
    for (auto it : m_advStatus) {
        if (it) {
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
    for(auto it : m_advStatus) {
        it = false;
    }
} // onHostSync


/**
 * @brief Handler for gap events when not using peripheral role.
 * @param [in] event the event data.
 * @param [in] arg pointer to the advertising instance.
 */
/*STATIC*/
int NimBLEExtAdvertising::handleGapEvent(struct ble_gap_event *event, void *arg) {
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
                    NIMBLE_LOGC(LOG_TAG, "host reset, rc = %d", event->adv_complete.reason);
                    NimBLEDevice::onReset(event->adv_complete.reason);
                    return 0;
                default:
                    break;
            }
            pAdv->m_advStatus[event->adv_complete.instance] = false;
            pAdv->m_pCallbacks->onStopped(pAdv, event->adv_complete.reason,
                                          event->adv_complete.instance);
            break;
        }

        case BLE_GAP_EVENT_SCAN_REQ_RCVD: {
            pAdv->m_pCallbacks->onScanRequest(pAdv, event->scan_req_rcvd.instance,
                                              NimBLEAddress(event->scan_req_rcvd.scan_addr));
            break;
        }
    }

    return 0;
} // handleGapEvent


/** Default callback handlers */
void NimBLEExtAdvertisingCallbacks::onStopped(NimBLEExtAdvertising *pAdv,
                                              int reason, uint8_t inst_id) {
    NIMBLE_LOGD("NimBLEExtAdvertisingCallbacks", "onStopped: Default");
} // onStopped


void NimBLEExtAdvertisingCallbacks::onScanRequest(NimBLEExtAdvertising *pAdv,
                                                  uint8_t inst_id, NimBLEAddress addr) {
    NIMBLE_LOGD("NimBLEExtAdvertisingCallbacks", "onScanRequest: Default");
} // onScanRequest


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
NimBLEExtAdvertisement::NimBLEExtAdvertisement(uint8_t priPhy, uint8_t secPhy)
:   m_advAddress("")
{
    memset (&m_params, 0, sizeof(m_params));
    m_params.own_addr_type = NimBLEDevice::m_own_addr_type;
    m_params.primary_phy   = priPhy;
    m_params.secondary_phy = secPhy;
    m_params.tx_power      = 127;
} // NimBLEExtAdvertisement


/**
 * @brief Sets wether the advertisement should use legacy (BLE 4.0, 31 bytes max) advertising.
 * @param [in] val true = using legacy advertising.
 */
void NimBLEExtAdvertisement::setLegacyAdvertising(bool val) {
    m_params.legacy_pdu = val;
} // setLegacyAdvertising


/**
 * @brief Sets wether the advertisement has scan response data available.
 * @param [in] val true = scan response is available.
 */
void NimBLEExtAdvertisement::setScannable(bool val) {
    m_params.scannable = val;
} // setScannable



/**
 * @brief Sets the transmission power level for this advertisement.
 * @param [in] dbm the transmission power to use in dbm.
 * @details The allowable value range depends on device hardware. \n
 * The ESP32C3 and ESP32S3 have a range of -27 to +18.
 */
void NimBLEExtAdvertisement::setTxPower(int8_t dbm) {
    m_params.tx_power = dbm;
}


/**
 * @brief Sets wether this advertisement should advertise as a connectable device.
 * @param [in] val True = connectable.
 */
void NimBLEExtAdvertisement::setConnectable(bool val) {
#if defined(CONFIG_BT_NIMBLE_ROLE_PERIPHERAL)
    m_params.connectable = val;
#endif
} // setConnectable


/**
 * @brief Set the address to use for this advertisement.
 * @param [in] addr The address to use.
 */
void NimBLEExtAdvertisement::setAddress(const NimBLEAddress & addr) {
    m_advAddress = addr;
    // Must use random address type.
    m_params.own_addr_type = BLE_OWN_ADDR_RANDOM;
}


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
void NimBLEExtAdvertisement::setDirectedPeer(const NimBLEAddress & addr) {
    ble_addr_t peerAddr;
    memcpy(&peerAddr.val, addr.getNative(), 6);
    peerAddr.type = addr.getType();
    m_params.peer = peerAddr;
} // setDirectedPeer


/**
 * @brief Enable or disable direct advertisements to the peer set with `NimBLEExtAdvertisement::setDirectedPeer`
 * @param [in] val true = send directed advertisements to peer.
 * @param [in] high_duty true = use fast advertising rate, default - true.
 */
void NimBLEExtAdvertisement::setDirected(bool val, bool high_duty) {
    m_params.directed = val;
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
 * @param [in] val Set to true to enable anonymous advertising.
 *
 * @details Anonymous advertising omits the device's address from the advertisement.
 */
void NimBLEExtAdvertisement::setAnonymous(bool val) {
    m_params.anonymous = val;
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
    std::vector<uint8_t> swap;
    std::swap(m_payload, swap);
}


/**
 * @brief Get the size of the current data.
 */
size_t NimBLEExtAdvertisement::getDataSize() {
    return m_payload.size();
} // getDataSize


/**
 * @brief Set the advertisement data.
 * @param [in] data The data to be set as the payload.
 * @param [in] length The size of data.
 * @details This will completely replace any data that was previously set.
 */
void NimBLEExtAdvertisement::setData(const uint8_t * data, size_t length) {
    m_payload.assign(data, data + length);
} // setData


/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 */
void NimBLEExtAdvertisement::addData(const std::string &data) {
    addData((uint8_t*)data.data(), data.length());
} // addData


/**
 * @brief Add data to the payload to be advertised.
 * @param [in] data The data to be added to the payload.
 * @param [in] length The size of data to be added to the payload.
 */
void NimBLEExtAdvertisement::addData(const uint8_t * data, size_t length) {
    m_payload.insert(m_payload.end(), data, data + length);
} // addData


/**
 * @brief Set the appearance.
 * @param [in] appearance The appearance code value.
 *
 * See also:
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.gap.appearance.xml
 */
void NimBLEExtAdvertisement::setAppearance(uint16_t appearance) {
    char cdata[2];
    cdata[0] = 3;
    cdata[1] = BLE_HS_ADV_TYPE_APPEARANCE; // 0x19
    addData(std::string(cdata, 2) + std::string((char*) &appearance, 2));
} // setAppearance


/**
 * @brief Set the advertisement flags.
 * @param [in] flag The flags to be set in the advertisement.
 * * BLE_HS_ADV_F_DISC_LTD
 * * BLE_HS_ADV_F_DISC_GEN
 * * BLE_HS_ADV_F_BREDR_UNSUP - must always use with NimBLE
 */
void NimBLEExtAdvertisement::setFlags(uint8_t flag) {
    char cdata[3];
    cdata[0] = 2;
    cdata[1] = BLE_HS_ADV_TYPE_FLAGS;  // 0x01
    cdata[2] = flag | BLE_HS_ADV_F_BREDR_UNSUP;
    addData(std::string(cdata, 3));
} // setFlags


/**
 * @brief Set manufacturer specific data.
 * @param [in] data The manufacturer data to advertise.
 */
void NimBLEExtAdvertisement::setManufacturerData(const std::string &data) {
    char cdata[2];
    cdata[0] = data.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_MFG_DATA ;  // 0xff
    addData(std::string(cdata, 2) + data);
} // setManufacturerData


/**
 * @brief Set the URI to advertise.
 * @param [in] uri The uri to advertise.
 */
void NimBLEExtAdvertisement::setURI(const std::string &uri) {
    char cdata[2];
    cdata[0] = uri.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_URI;
    addData(std::string(cdata, 2) + uri);
} // setURI


/**
 * @brief Set the complete name of this device.
 * @param [in] name The name to advertise.
 */
void NimBLEExtAdvertisement::setName(const std::string &name) {
    char cdata[2];
    cdata[0] = name.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_COMP_NAME;  // 0x09
    addData(std::string(cdata, 2) + name);
} // setName


/**
 * @brief Set a single service to advertise as a complete list of services.
 * @param [in] uuid The service to advertise.
 */
void NimBLEExtAdvertisement::setCompleteServices(const NimBLEUUID &uuid) {
    setServices(true, uuid.bitSize(), {uuid});
} // setCompleteServices


/**
 * @brief Set the complete list of 16 bit services to advertise.
 * @param [in] v_uuid A vector of 16 bit UUID's to advertise.
 */
void NimBLEExtAdvertisement::setCompleteServices16(const std::vector<NimBLEUUID>& v_uuid) {
    setServices(true, 16, v_uuid);
} // setCompleteServices16


/**
 * @brief Set the complete list of 32 bit services to advertise.
 * @param [in] v_uuid A vector of 32 bit UUID's to advertise.
 */
void NimBLEExtAdvertisement::setCompleteServices32(const std::vector<NimBLEUUID>& v_uuid) {
    setServices(true, 32, v_uuid);
} // setCompleteServices32


/**
 * @brief Set a single service to advertise as a partial list of services.
 * @param [in] uuid The service to advertise.
 */
void NimBLEExtAdvertisement::setPartialServices(const NimBLEUUID &uuid) {
    setServices(false, uuid.bitSize(), {uuid});
} // setPartialServices


/**
 * @brief Set the partial list of services to advertise.
 * @param [in] v_uuid A vector of 16 bit UUID's to advertise.
 */
void NimBLEExtAdvertisement::setPartialServices16(const std::vector<NimBLEUUID>& v_uuid) {
    setServices(false, 16, v_uuid);
} // setPartialServices16


/**
 * @brief Set the partial list of services to advertise.
 * @param [in] v_uuid A vector of 32 bit UUID's to advertise.
 */
void NimBLEExtAdvertisement::setPartialServices32(const std::vector<NimBLEUUID>& v_uuid) {
    setServices(false, 32, v_uuid);
} // setPartialServices32


/**
 * @brief Utility function to create the list of service UUID's from a vector.
 * @param [in] complete If true the vector is the complete set of services.
 * @param [in] size The bit size of the UUID's in the vector. (16, 32, or 128).
 * @param [in] v_uuid The vector of service UUID's to advertise.
 */
void NimBLEExtAdvertisement::setServices(const bool complete, const uint8_t size,
                                             const std::vector<NimBLEUUID> &v_uuid)
{
    char cdata[2];
    cdata[0] = (size / 8) * v_uuid.size() + 1;
    switch(size) {
        case 16:
            cdata[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS16 : BLE_HS_ADV_TYPE_INCOMP_UUIDS16;
            break;
        case 32:
            cdata[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS32 : BLE_HS_ADV_TYPE_INCOMP_UUIDS32;
            break;
        case 128:
            cdata[1] = complete ? BLE_HS_ADV_TYPE_COMP_UUIDS128 : BLE_HS_ADV_TYPE_INCOMP_UUIDS128;
            break;
        default:
            return;
    }

    std::string uuids;

    for(auto &it : v_uuid){
        if(it.bitSize() != size) {
            NIMBLE_LOGE(LOG_TAG, "Service UUID(%d) invalid", size);
            return;
        } else {
            switch(size) {
                case 16:
                    uuids += std::string((char*)&it.getNative()->u16.value, 2);
                    break;
                case 32:
                    uuids += std::string((char*)&it.getNative()->u32.value, 4);
                    break;
                case 128:
                    uuids += std::string((char*)&it.getNative()->u128.value, 16);
                    break;
                default:
                    return;
            }
        }
    }

    addData(std::string(cdata, 2) + uuids);
} // setServices


/**
 * @brief Set the service data (UUID + data)
 * @param [in] uuid The UUID to set with the service data.
 * @param [in] data The data to be associated with the service data advertised.
 */
void NimBLEExtAdvertisement::setServiceData(const NimBLEUUID &uuid, const std::string &data) {
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
void NimBLEExtAdvertisement::setShortName(const std::string &name) {
    char cdata[2];
    cdata[0] = name.length() + 1;
    cdata[1] = BLE_HS_ADV_TYPE_INCOMP_NAME;  // 0x08
    addData(std::string(cdata, 2) + name);
} // setShortName


/**
 * @brief Adds Tx power level to the advertisement data.
 */
void NimBLEExtAdvertisement::addTxPower() {
    m_params.include_tx_power = 1;
} // addTxPower


/**
 * @brief Set the preferred connection interval parameters.
 * @param [in] min The minimum interval desired.
 * @param [in] max The maximum interval desired.
 */
void NimBLEExtAdvertisement::setPreferredParams(uint16_t min, uint16_t max) {
    uint8_t data[6];
    data[0] = BLE_HS_ADV_SLAVE_ITVL_RANGE_LEN + 1;
    data[1] = BLE_HS_ADV_TYPE_SLAVE_ITVL_RANGE;
    data[2] = min;
    data[3] = min >> 8;
    data[4] = max;
    data[5] = max >> 8;
    addData(data, 6);
} // setPreferredParams

#endif /* CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_BROADCASTER && CONFIG_BT_NIMBLE_EXT_ADV */
