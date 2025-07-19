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

#include "NimBLEScan.h"
#if CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER

# include "NimBLEDevice.h"
# include "NimBLELog.h"

# include <string>
# include <climits>

static const char*         LOG_TAG = "NimBLEScan";
static NimBLEScanCallbacks defaultScanCallbacks;

/**
 * @brief Scan constructor.
 */
NimBLEScan::NimBLEScan()
    : m_pScanCallbacks{&defaultScanCallbacks},
      // default interval + window, no whitelist scan filter,not limited scan, no scan response, filter_duplicates
      m_scanParams{0, 0, BLE_HCI_SCAN_FILT_NO_WL, 0, 1, 1},
      m_pTaskData{nullptr},
      m_maxResults{0xFF} {}

/**
 * @brief Scan destructor, release any allocated resources.
 */
NimBLEScan::~NimBLEScan() {
    for (const auto& dev : m_scanResults.m_deviceVec) {
        delete dev;
    }
}

/**
 * @brief Handle GAP events related to scans.
 * @param [in] event The event type for this event.
 * @param [in] param Parameter data for this event.
 */
int NimBLEScan::handleGapEvent(ble_gap_event* event, void* arg) {
    (void)arg;
    NimBLEScan* pScan = NimBLEDevice::getScan();

    switch (event->type) {
        case BLE_GAP_EVENT_EXT_DISC:
        case BLE_GAP_EVENT_DISC: {
            if (!pScan->isScanning()) {
                NIMBLE_LOGI(LOG_TAG, "Scan stopped, ignoring event");
                return 0;
            }

# if CONFIG_BT_NIMBLE_EXT_ADV
            const auto& disc        = event->ext_disc;
            const bool  isLegacyAdv = disc.props & BLE_HCI_ADV_LEGACY_MASK;
            const auto  event_type  = isLegacyAdv ? disc.legacy_event_type : disc.props;
# else
            const auto& disc        = event->disc;
            const bool  isLegacyAdv = true;
            const auto  event_type  = disc.event_type;
# endif
            NimBLEAddress advertisedAddress(disc.addr);

# if CONFIG_BT_NIMBLE_ROLE_CENTRAL
            // stop processing if already connected
            NimBLEClient* pClient = NimBLEDevice::getClientByPeerAddress(advertisedAddress);
            if (pClient != nullptr && pClient->isConnected()) {
                NIMBLE_LOGI(LOG_TAG, "Ignoring device: address: %s, already connected", advertisedAddress.toString().c_str());
                return 0;
            }
# endif
            NimBLEAdvertisedDevice* advertisedDevice = nullptr;

            // If we've seen this device before get a pointer to it from the vector
            for (const auto& dev : pScan->m_scanResults.m_deviceVec) {
# if CONFIG_BT_NIMBLE_EXT_ADV
                // Same address but different set ID should create a new advertised device.
                if (dev->getAddress() == advertisedAddress && dev->getSetId() == disc.sid)
# else
                if (dev->getAddress() == advertisedAddress)
# endif
                {
                    advertisedDevice = dev;
                    break;
                }
            }

            // If we haven't seen this device before; create a new instance and insert it in the vector.
            // Otherwise just update the relevant parameters of the already known device.
            if (advertisedDevice == nullptr) {
                // Check if we have reach the scan results limit, ignore this one if so.
                // We still need to store each device when maxResults is 0 to be able to append the scan results
                if (pScan->m_maxResults > 0 && pScan->m_maxResults < 0xFF &&
                    (pScan->m_scanResults.m_deviceVec.size() >= pScan->m_maxResults)) {
                    return 0;
                }

                if (isLegacyAdv && event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                    NIMBLE_LOGI(LOG_TAG, "Scan response without advertisement: %s", advertisedAddress.toString().c_str());
                }

                advertisedDevice = new NimBLEAdvertisedDevice(event, event_type);
                pScan->m_scanResults.m_deviceVec.push_back(advertisedDevice);
                NIMBLE_LOGI(LOG_TAG, "New advertiser: %s", advertisedAddress.toString().c_str());
            } else {
                advertisedDevice->update(event, event_type);
                if (isLegacyAdv && event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                    NIMBLE_LOGI(LOG_TAG, "Scan response from: %s", advertisedAddress.toString().c_str());
                } else {
                    NIMBLE_LOGI(LOG_TAG, "Duplicate; updated: %s", advertisedAddress.toString().c_str());
                }
            }

            if (!advertisedDevice->m_callbackSent) {
                advertisedDevice->m_callbackSent++;
                pScan->m_pScanCallbacks->onDiscovered(advertisedDevice);
            }

            // If not active scanning or scan response is not available
            // or extended advertisement scanning, report the result to the callback now.
            if (pScan->m_scanParams.passive || !isLegacyAdv || !advertisedDevice->isScannable()) {
                advertisedDevice->m_callbackSent++;
                pScan->m_pScanCallbacks->onResult(advertisedDevice);
            } else if (isLegacyAdv && event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                advertisedDevice->m_callbackSent++;
                // got the scan response report the full data.
                pScan->m_pScanCallbacks->onResult(advertisedDevice);
            }

            // If not storing results and we have invoked the callback, delete the device.
            if (pScan->m_maxResults == 0 && advertisedDevice->m_callbackSent >= 2) {
                pScan->erase(advertisedDevice);
            }

            return 0;
        }

        case BLE_GAP_EVENT_DISC_COMPLETE: {
            NIMBLE_LOGD(LOG_TAG, "discovery complete; reason=%d", event->disc_complete.reason);

            if (pScan->m_maxResults == 0) {
                pScan->clearResults();
            }

            pScan->m_pScanCallbacks->onScanEnd(pScan->m_scanResults, event->disc_complete.reason);

            if (pScan->m_pTaskData != nullptr) {
                NimBLEUtils::taskRelease(*pScan->m_pTaskData, event->disc_complete.reason);
            }

            return 0;
        }

        default:
            return 0;
    }
} // handleGapEvent

/**
 * @brief Should we perform an active or passive scan?
 * The default is a passive scan. An active scan means that we will request a scan response.
 * @param [in] active If true, we perform an active scan otherwise a passive scan.
 */
void NimBLEScan::setActiveScan(bool active) {
    m_scanParams.passive = !active;
} // setActiveScan

/**
 * @brief Set whether or not the BLE controller should only report results
 * from devices it has not already seen.
 * @param [in] enabled If set to 1 (true), scanned devices will only be reported once.
 * If set to 0 duplicates will be reported each time they are seen.
 * If using extended scanning this can be set to 2 which will reset the duplicate filter
 * at the end of each scan period if the scan period is set.
 * @note The controller has a limited buffer and will start reporting
duplicate devices once the limit is reached.
 */
void NimBLEScan::setDuplicateFilter(uint8_t enabled) {
    m_scanParams.filter_duplicates = enabled;
} // setDuplicateFilter

/**
 * @brief Set whether or not the BLE controller only reports scan results
 * from devices advertising in limited discovery mode.
 * @param [in] enabled If true, only limited discovery devices will be in scan results.
 */
void NimBLEScan::setLimitedOnly(bool enabled) {
    m_scanParams.limited = enabled;
} // setLimited

/**
 * @brief Sets the scan filter policy.
 * @param [in] filter Can be one of:
 * * BLE_HCI_SCAN_FILT_NO_WL             (0)
 *      Scanner processes all advertising packets (white list not used) except\n
 *      directed, connectable advertising packets not sent to the scanner.
 * * BLE_HCI_SCAN_FILT_USE_WL            (1)
 *      Scanner processes advertisements from white list only. A connectable,\n
 *      directed advertisement is ignored unless it contains scanners address.
 * * BLE_HCI_SCAN_FILT_NO_WL_INITA       (2)
 *      Scanner process all advertising packets (white list not used). A\n
 *      connectable, directed advertisement shall not be ignored if the InitA
 *      is a resolvable private address.
 * * BLE_HCI_SCAN_FILT_USE_WL_INITA      (3)
 *      Scanner process advertisements from white list only. A connectable,\n
 *      directed advertisement shall not be ignored if the InitA is a
 *      resolvable private address.
 */
void NimBLEScan::setFilterPolicy(uint8_t filter) {
    m_scanParams.filter_policy = filter;
} // setFilterPolicy

/**
 * @brief Sets the max number of results to store.
 * @param [in] maxResults The number of results to limit storage to\n
 * 0 == none (callbacks only) 0xFF == unlimited, any other value is the limit.
 */
void NimBLEScan::setMaxResults(uint8_t maxResults) {
    m_maxResults = maxResults;
} // setMaxResults

/**
 * @brief Set the call backs to be invoked.
 * @param [in] pScanCallbacks Call backs to be invoked.
 * @param [in] wantDuplicates  True if we wish to be called back with duplicates, default: false.
 */
void NimBLEScan::setScanCallbacks(NimBLEScanCallbacks* pScanCallbacks, bool wantDuplicates) {
    setDuplicateFilter(!wantDuplicates);
    if (pScanCallbacks == nullptr) {
        m_pScanCallbacks = &defaultScanCallbacks;
        return;
    }
    m_pScanCallbacks = pScanCallbacks;
} // setScanCallbacks

/**
 * @brief Set the interval to scan.
 * @param [in] intervalMs The scan interval in milliseconds.
 * @details The interval is the time between the start of two consecutive scan windows.
 * When a new interval starts the controller changes the channel it's scanning on.
 */
void NimBLEScan::setInterval(uint16_t intervalMs) {
    m_scanParams.itvl = (intervalMs * 16) / 10;
} // setInterval

/**
 * @brief Set the window to actively scan.
 * @param [in] windowMs How long during the interval to actively scan in milliseconds.
 */
void NimBLEScan::setWindow(uint16_t windowMs) {
    m_scanParams.window = (windowMs * 16) / 10;
} // setWindow

/**
 * @brief Get the status of the scanner.
 * @return true if scanning or scan starting.
 */
bool NimBLEScan::isScanning() {
    return ble_gap_disc_active();
}

# if CONFIG_BT_NIMBLE_EXT_ADV
/**
 * @brief Set the PHYs to scan.
 * @param [in] phyMask The PHYs to scan, a bit mask of:
 * * NIMBLE_CPP_SCAN_1M
 * * NIMBLE_CPP_SCAN_CODED
 */
void NimBLEScan::setPhy(Phy phyMask) {
    m_phy = phyMask;
} // setScanPhy

/**
 * @brief Set the extended scanning period.
 * @param [in] periodMs The scan period in milliseconds
 * @details The period is the time between the start of two consecutive scan periods.
 * This works as a timer to restart scanning at the specified amount of time in periodMs.
 * @note The duration used when this is set must be less than period.
 */
void NimBLEScan::setPeriod(uint32_t periodMs) {
    m_period = (periodMs + 500) / 1280; // round up 1.28 second units
} // setScanPeriod
# endif

/**
 * @brief Start scanning.
 * @param [in] duration The duration in milliseconds for which to scan. 0 == scan forever.
 * @param [in] isContinue Set to true to save previous scan results, false to clear them.
 * @param [in] restart Set to true to restart the scan if already in progress.
 * this is useful to clear the duplicate filter so all devices are reported again.
 * @return True if scan started or false if there was an error.
 */
bool NimBLEScan::start(uint32_t duration, bool isContinue, bool restart) {
    NIMBLE_LOGD(LOG_TAG, ">> start: duration=%" PRIu32, duration);
    if (isScanning()) {
        if (restart) {
            NIMBLE_LOGI(LOG_TAG, "Scan already in progress, restarting it");
            if (!stop()) {
                return false;
            }

            if (!isContinue) {
                clearResults();
            }
        }
    } else { // Don't clear results while scanning is active
        if (!isContinue) {
            clearResults();
        }
    }

    // If scanning is already active, call the functions anyway as the parameters can be changed.

# if CONFIG_BT_NIMBLE_EXT_ADV
    ble_gap_ext_disc_params scan_params;
    scan_params.passive = m_scanParams.passive;
    scan_params.itvl    = m_scanParams.itvl;
    scan_params.window  = m_scanParams.window;
    int rc              = ble_gap_ext_disc(NimBLEDevice::m_ownAddrType,
                              duration / 10, // 10ms units
                              m_period,
                              m_scanParams.filter_duplicates,
                              m_scanParams.filter_policy,
                              m_scanParams.limited,
                              m_phy & SCAN_1M ? &scan_params : NULL,
                              m_phy & SCAN_CODED ? &scan_params : NULL,
                              NimBLEScan::handleGapEvent,
                              NULL);
# else
    int rc = ble_gap_disc(NimBLEDevice::m_ownAddrType,
                          duration ? duration : BLE_HS_FOREVER,
                          &m_scanParams,
                          NimBLEScan::handleGapEvent,
                          NULL);
# endif
    switch (rc) {
        case 0:
        case BLE_HS_EALREADY:
            NIMBLE_LOGD(LOG_TAG, "Scan started");
            break;

        case BLE_HS_EBUSY:
            NIMBLE_LOGE(LOG_TAG, "Unable to scan - connection in progress.");
            break;

        case BLE_HS_ETIMEOUT_HCI:
        case BLE_HS_EOS:
        case BLE_HS_ECONTROLLER:
        case BLE_HS_ENOTSYNCED:
            NIMBLE_LOGE(LOG_TAG, "Unable to scan - Host Reset");
            break;

        default:
            NIMBLE_LOGE(LOG_TAG, "Error starting scan; rc=%d, %s", rc, NimBLEUtils::returnCodeToString(rc));
            break;
    }

    NIMBLE_LOGD(LOG_TAG, "<< start()");
    return rc == 0 || rc == BLE_HS_EALREADY;
} // start

/**
 * @brief Stop an in progress scan.
 * @return True if successful.
 */
bool NimBLEScan::stop() {
    NIMBLE_LOGD(LOG_TAG, ">> stop()");

    int rc = ble_gap_disc_cancel();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "Failed to cancel scan; rc=%d", rc);
        return false;
    }

    if (m_maxResults == 0) {
        clearResults();
    }

    if (m_pTaskData != nullptr) {
        NimBLEUtils::taskRelease(*m_pTaskData);
    }

    NIMBLE_LOGD(LOG_TAG, "<< stop()");
    return true;
} // stop

/**
 * @brief Delete peer device from the scan results vector.
 * @param [in] address The address of the device to delete from the results.
 */
void NimBLEScan::erase(const NimBLEAddress& address) {
    NIMBLE_LOGD(LOG_TAG, "erase device: %s", address.toString().c_str());
    for (auto it = m_scanResults.m_deviceVec.begin(); it != m_scanResults.m_deviceVec.end(); ++it) {
        if ((*it)->getAddress() == address) {
            delete *it;
            m_scanResults.m_deviceVec.erase(it);
            break;
        }
    }
}

/**
 * @brief Delete peer device from the scan results vector.
 * @param [in] device The device to delete from the results.
 */
void NimBLEScan::erase(const NimBLEAdvertisedDevice* device) {
    NIMBLE_LOGD(LOG_TAG, "erase device: %s", device->getAddress().toString().c_str());
    for (auto it = m_scanResults.m_deviceVec.begin(); it != m_scanResults.m_deviceVec.end(); ++it) {
        if ((*it) == device) {
            delete *it;
            m_scanResults.m_deviceVec.erase(it);
            break;
        }
    }
}

/**
 * @brief If the host reset and re-synced this is called.
 * If the application was scanning indefinitely with a callback, restart it.
 */
void NimBLEScan::onHostSync() {
    m_pScanCallbacks->onScanEnd(m_scanResults, BLE_HS_ENOTSYNCED);
}

/**
 * @brief Start scanning and block until scanning has been completed.
 * @param [in] duration The duration in milliseconds for which to scan.
 * @param [in] is_continue Set to true to save previous scan results, false to clear them.
 * @return The scan results.
 */
NimBLEScanResults NimBLEScan::getResults(uint32_t duration, bool is_continue) {
    if (duration == 0) {
        NIMBLE_LOGW(LOG_TAG, "Blocking scan called with duration = forever");
    }

    if (m_pTaskData != nullptr) {
        NIMBLE_LOGE(LOG_TAG, "Scan already in progress");
        return m_scanResults;
    }

    NimBLETaskData taskData;
    m_pTaskData = &taskData;

    if (start(duration, is_continue)) {
        NimBLEUtils::taskWait(taskData, BLE_NPL_TIME_FOREVER);
    }

    m_pTaskData = nullptr;
    return m_scanResults;
} // getResults

/**
 * @brief Get the results of the scan.
 * @return NimBLEScanResults object.
 */
NimBLEScanResults NimBLEScan::getResults() {
    return m_scanResults;
}

/**
 * @brief Clear the stored results of the scan.
 */
void NimBLEScan::clearResults() {
    if (m_scanResults.m_deviceVec.size()) {
        std::vector<NimBLEAdvertisedDevice*> vSwap{};
        ble_npl_hw_enter_critical();
        vSwap.swap(m_scanResults.m_deviceVec);
        ble_npl_hw_exit_critical(0);
        for (const auto& dev : vSwap) {
            delete dev;
        }
    }
} // clearResults

/**
 * @brief Dump the scan results to the log.
 */
void NimBLEScanResults::dump() const {
# if CONFIG_NIMBLE_CPP_LOG_LEVEL >= 3
    for (const auto& dev : m_deviceVec) {
        NIMBLE_LOGI(LOG_TAG, "- %s", dev->toString().c_str());
    }
# endif
} // dump

/**
 * @brief Get the count of devices found in the last scan.
 * @return The number of devices found in the last scan.
 */
int NimBLEScanResults::getCount() const {
    return m_deviceVec.size();
} // getCount

/**
 * @brief Return the specified device at the given index.
 * The index should be between 0 and getCount()-1.
 * @param [in] idx The index of the device.
 * @return The device at the specified index.
 */
const NimBLEAdvertisedDevice* NimBLEScanResults::getDevice(uint32_t idx) const {
    return m_deviceVec[idx];
}

/**
 * @brief Get iterator to the beginning of the vector of advertised device pointers.
 * @return An iterator to the beginning of the vector of advertised device pointers.
 */
std::vector<NimBLEAdvertisedDevice*>::const_iterator NimBLEScanResults::begin() const {
    return m_deviceVec.begin();
}

/**
 * @brief Get iterator to the end of the vector of advertised device pointers.
 * @return An iterator to the end of the vector of advertised device pointers.
 */
std::vector<NimBLEAdvertisedDevice*>::const_iterator NimBLEScanResults::end() const {
    return m_deviceVec.end();
}

/**
 * @brief Get a pointer to the specified device at the given address.
 * If the address is not found a nullptr is returned.
 * @param [in] address The address of the device.
 * @return A pointer to the device at the specified address.
 */
const NimBLEAdvertisedDevice* NimBLEScanResults::getDevice(const NimBLEAddress& address) const {
    for (const auto& dev : m_deviceVec) {
        if (dev->getAddress() == address) {
            return dev;
        }
    }

    return nullptr;
}

static const char* CB_TAG = "NimBLEScanCallbacks";

void NimBLEScanCallbacks::onDiscovered(const NimBLEAdvertisedDevice* pAdvertisedDevice) {
    NIMBLE_LOGD(CB_TAG, "Discovered: %s", pAdvertisedDevice->toString().c_str());
}

void NimBLEScanCallbacks::onResult(const NimBLEAdvertisedDevice* pAdvertisedDevice) {
    NIMBLE_LOGD(CB_TAG, "Result: %s", pAdvertisedDevice->toString().c_str());
}

void NimBLEScanCallbacks::onScanEnd(const NimBLEScanResults& results, int reason) {
    NIMBLE_LOGD(CB_TAG, "Scan ended; reason %d, num results: %d", reason, results.getCount());
}

#endif // CONFIG_BT_ENABLED && CONFIG_BT_NIMBLE_ROLE_OBSERVER
