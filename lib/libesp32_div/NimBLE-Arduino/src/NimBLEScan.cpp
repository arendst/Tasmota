/*
 * NimBLEScan.cpp
 *
 *  Created: on Jan 24 2020
 *      Author H2zero
 *
 * Originally:
 *
 * BLEScan.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "nimconfig.h"
#if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)

#include "NimBLEScan.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#include <string>

static const char* LOG_TAG = "NimBLEScan";


/**
 * @brief Scan constuctor.
 */
NimBLEScan::NimBLEScan() {
    m_scan_params.filter_policy      = BLE_HCI_SCAN_FILT_NO_WL;
    m_scan_params.passive            = 1; // If set, don’t send scan requests to advertisers (i.e., don’t request additional advertising data).
    m_scan_params.itvl               = 0; // This is defined as the time interval from when the Controller started its last LE scan until it begins the subsequent LE scan. (units=0.625 msec)
    m_scan_params.window             = 0; // The duration of the LE scan. LE_Scan_Window shall be less than or equal to LE_Scan_Interval (units=0.625 msec)
    m_scan_params.limited            = 0; // If set, only discover devices in limited discoverable mode.
    m_scan_params.filter_duplicates  = 0; // If set, the controller ignores all but the first advertisement from each device.
    m_pAdvertisedDeviceCallbacks     = nullptr;
    m_ignoreResults                  = false;
    m_pTaskData                      = nullptr;
    m_duration                       = BLE_HS_FOREVER; // make sure this is non-zero in the event of a host reset
    m_maxResults                     = 0xFF;
}


/**
 * @brief Scan destructor, release any allocated resources.
 */
NimBLEScan::~NimBLEScan() {
     clearResults();
}

/**
 * @brief Handle GAP events related to scans.
 * @param [in] event The event type for this event.
 * @param [in] param Parameter data for this event.
 */
/*STATIC*/int NimBLEScan::handleGapEvent(ble_gap_event* event, void* arg) {

    NimBLEScan* pScan = (NimBLEScan*)arg;

    switch(event->type) {

        case BLE_GAP_EVENT_DISC: {
            if(pScan->m_ignoreResults) {
                NIMBLE_LOGI(LOG_TAG, "Scan op in progress - ignoring results");
                return 0;
            }

            NimBLEAddress advertisedAddress(event->disc.addr);

            // Examine our list of ignored addresses and stop processing if we don't want to see it or are already connected
            if(NimBLEDevice::isIgnored(advertisedAddress)) {
                NIMBLE_LOGI(LOG_TAG, "Ignoring device: address: %s", advertisedAddress.toString().c_str());
                return 0;
            }

            NimBLEAdvertisedDevice* advertisedDevice = nullptr;

            // If we've seen this device before get a pointer to it from the vector
            for(auto &it: pScan->m_scanResults.m_advertisedDevicesVector) {
                if(it->getAddress() == advertisedAddress) {
                    advertisedDevice = it;
                    break;
                }
            }

            // If we haven't seen this device before; create a new instance and insert it in the vector.
            // Otherwise just update the relevant parameters of the already known device.
            if(advertisedDevice == nullptr && event->disc.event_type != BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP){
                // Check if we have reach the scan results limit, ignore this one if so.
                // We still need to store each device when maxResults is 0 to be able to append the scan results
                if(pScan->m_maxResults > 0 && pScan->m_maxResults < 0xFF &&
                  (pScan->m_scanResults.m_advertisedDevicesVector.size() >= pScan->m_maxResults))
                {
                    return 0;
                }
                advertisedDevice = new NimBLEAdvertisedDevice();
                advertisedDevice->setAddress(advertisedAddress);
                advertisedDevice->setAdvType(event->disc.event_type);
                pScan->m_scanResults.m_advertisedDevicesVector.push_back(advertisedDevice);
                NIMBLE_LOGI(LOG_TAG, "New advertiser: %s", advertisedAddress.toString().c_str());
            } else if(advertisedDevice != nullptr) {
                NIMBLE_LOGI(LOG_TAG, "Updated advertiser: %s", advertisedAddress.toString().c_str());
            } else {
                // Scan response from unknown device
                return 0;
            }

            advertisedDevice->m_timestamp = time(nullptr);
            advertisedDevice->setRSSI(event->disc.rssi);
            advertisedDevice->setPayload(event->disc.data, event->disc.length_data,
            event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP);

            if (pScan->m_pAdvertisedDeviceCallbacks) {
                // If not active scanning or scan response is not available
                // report the result to the callback now.
                if(pScan->m_scan_params.passive ||
                  (advertisedDevice->getAdvType() != BLE_HCI_ADV_TYPE_ADV_IND &&
                   advertisedDevice->getAdvType() != BLE_HCI_ADV_TYPE_ADV_SCAN_IND))
                {
                    advertisedDevice->m_callbackSent = true;
                    pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);

                // Otherwise, wait for the scan response so we can report the complete data.
                } else if (event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                    advertisedDevice->m_callbackSent = true;
                    pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);
                }
                // If not storing results and we have invoked the callback, delete the device.
                if(pScan->m_maxResults == 0 && advertisedDevice->m_callbackSent) {
                    pScan->erase(advertisedAddress);
                }
            }

            return 0;
        }
        case BLE_GAP_EVENT_DISC_COMPLETE: {
            NIMBLE_LOGD(LOG_TAG, "discovery complete; reason=%d",
                        event->disc_complete.reason);

            // If a device advertised with scan reponse available and it was not received
            // the callback would not have been invoked, so do it here.
            if(pScan->m_pAdvertisedDeviceCallbacks) {
                for(auto &it : pScan->m_scanResults.m_advertisedDevicesVector) {
                    if(!it->m_callbackSent) {
                        pScan->m_pAdvertisedDeviceCallbacks->onResult(it);
                    }
                }
            }

            if(pScan->m_maxResults == 0) {
                pScan->clearResults();
            }

            if (pScan->m_scanCompleteCB != nullptr) {
                pScan->m_scanCompleteCB(pScan->m_scanResults);
            }

            if(pScan->m_pTaskData != nullptr) {
                pScan->m_pTaskData->rc = event->disc_complete.reason;
                xTaskNotifyGive(pScan->m_pTaskData->task);
            }

            return 0;
        }

        default:
            return 0;
    }
}  // gapEventHandler


/**
 * @brief Should we perform an active or passive scan?
 * The default is a passive scan. An active scan means that we will request a scan response.
 * @param [in] active If true, we perform an active scan otherwise a passive scan.
 */
void NimBLEScan::setActiveScan(bool active) {
    m_scan_params.passive = !active;
} // setActiveScan


/**
 * @brief Set whether or not the BLE controller should only report results
 * from devices it has not already seen.
 * @param [in] enabled If true, scanned devices will only be reported once.
 * @details The controller has a limited buffer and will start reporting
 * dupicate devices once the limit is reached.
 */
void NimBLEScan::setDuplicateFilter(bool enabled) {
    m_scan_params.filter_duplicates = enabled;
} // setDuplicateFilter


/**
 * @brief Set whether or not the BLE controller only report scan results
 * from devices advertising in limited discovery mode, i.e. directed advertising.
 * @param [in] enabled If true, only limited discovery devices will be in scan results.
 */
void NimBLEScan::setLimitedOnly(bool enabled) {
    m_scan_params.limited = enabled;
} // setLimited


/**
 * @brief Sets the scan filter policy.
 * @param [in] filter Can be one of:
 * * BLE_HCI_SCAN_FILT_NO_WL             (0)
 *      Scanner processes all advertising packets (white list not used) except\n
 *      directed, connectable advertising packets not sent to the scanner.
 * * BLE_HCI_SCAN_FILT_USE_WL            (1)
 *      Scanner processes advertisements from white list only. A connectable,\n
 *      directed advertisment is ignored unless it contains scanners address.
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
    m_scan_params.filter_policy = filter;
} // setFilterPolicy


/**
 * @brief Sets the max number of results to store.
 * @param [in] maxResults The number of results to limit storage to\n
 * 0 == none (callbacks only) 0xFF == unlimited, any other value is the limit.
 */
void NimBLEScan::setMaxResults(uint8_t maxResults) {
    m_maxResults = maxResults;
}


/**
 * @brief Set the call backs to be invoked.
 * @param [in] pAdvertisedDeviceCallbacks Call backs to be invoked.
 * @param [in] wantDuplicates  True if we wish to be called back with duplicates.  Default is false.
 */
void NimBLEScan::setAdvertisedDeviceCallbacks(NimBLEAdvertisedDeviceCallbacks* pAdvertisedDeviceCallbacks,
                                              bool wantDuplicates) {
    setDuplicateFilter(!wantDuplicates);
    m_pAdvertisedDeviceCallbacks = pAdvertisedDeviceCallbacks;
} // setAdvertisedDeviceCallbacks


/**
 * @brief Set the interval to scan.
 * @param [in] intervalMSecs The scan interval (how often) in milliseconds.
 */
void NimBLEScan::setInterval(uint16_t intervalMSecs) {
    m_scan_params.itvl = intervalMSecs / 0.625;
} // setInterval


/**
 * @brief Set the window to actively scan.
 * @param [in] windowMSecs How long to actively scan.
 */
void NimBLEScan::setWindow(uint16_t windowMSecs) {
    m_scan_params.window = windowMSecs / 0.625;
} // setWindow


/**
 * @brief Get the status of the scanner.
 * @return true if scanning or scan starting.
 */
bool NimBLEScan::isScanning() {
    return ble_gap_disc_active();
}


/**
 * @brief Start scanning.
 * @param [in] duration The duration in seconds for which to scan.
 * @param [in] scanCompleteCB A function to be called when scanning has completed.
 * @param [in] is_continue Set to true to save previous scan results, false to clear them.
 * @return True if scan started or false if there was an error.
 */
bool NimBLEScan::start(uint32_t duration, void (*scanCompleteCB)(NimBLEScanResults), bool is_continue) {
    NIMBLE_LOGD(LOG_TAG, ">> start(duration=%d)", duration);

    // Save the callback to be invoked when the scan completes.
    m_scanCompleteCB = scanCompleteCB;
    // Save the duration in the case that the host is reset so we can reuse it.
    m_duration = duration;

    // If 0 duration specified then we assume a continuous scan is desired.
    if(duration == 0){
        duration = BLE_HS_FOREVER;
    }
    else{
        // convert duration to milliseconds
        duration = duration * 1000;
    }

    // Set the flag to ignore the results while we are deleting the vector
    if(!is_continue) {
        m_ignoreResults = true;
    }

    int rc = ble_gap_disc(NimBLEDevice::m_own_addr_type, duration, &m_scan_params,
                          NimBLEScan::handleGapEvent, this);

    switch(rc) {
        case 0:
            if(!is_continue) {
                clearResults();
            }
            break;

        case BLE_HS_EALREADY:
            // Clear the cache if already scanning in case an advertiser was missed.
            clearDuplicateCache();
            break;

        case BLE_HS_EBUSY:
            NIMBLE_LOGE(LOG_TAG, "Unable to scan - connection in progress.");
            break;

        case BLE_HS_ETIMEOUT_HCI:
        case BLE_HS_EOS:
        case BLE_HS_ECONTROLLER:
        case BLE_HS_ENOTSYNCED:
            NIMBLE_LOGC(LOG_TAG, "Unable to scan - Host Reset");
            break;

        default:
            NIMBLE_LOGE(LOG_TAG, "Error initiating GAP discovery procedure; rc=%d, %s",
                        rc, NimBLEUtils::returnCodeToString(rc));
            break;
    }

    m_ignoreResults = false;
    NIMBLE_LOGD(LOG_TAG, "<< start()");

    if(rc != 0 && rc != BLE_HS_EALREADY) {
        return false;
    }
    return true;
} // start


/**
 * @brief Start scanning and block until scanning has been completed.
 * @param [in] duration The duration in seconds for which to scan.
 * @param [in] is_continue Set to true to save previous scan results, false to clear them.
 * @return The NimBLEScanResults.
 */
NimBLEScanResults NimBLEScan::start(uint32_t duration, bool is_continue) {
    if(duration == 0) {
        NIMBLE_LOGW(LOG_TAG, "Blocking scan called with duration = forever");
    }

    ble_task_data_t taskData = {nullptr, xTaskGetCurrentTaskHandle(),0, nullptr};
    m_pTaskData = &taskData;

    if(start(duration, nullptr, is_continue)) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    m_pTaskData = nullptr;
    return m_scanResults;
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

    if(m_maxResults == 0) {
        clearResults();
    }

    if (rc != BLE_HS_EALREADY && m_scanCompleteCB != nullptr) {
        m_scanCompleteCB(m_scanResults);
    }

    if(m_pTaskData != nullptr) {
        xTaskNotifyGive(m_pTaskData->task);
    }

    NIMBLE_LOGD(LOG_TAG, "<< stop()");
    return true;
} // stop


/**
 * @brief Clears the duplicate scan filter cache.
 */
void NimBLEScan::clearDuplicateCache() {
#ifdef CONFIG_IDF_TARGET_ESP32 // Not available for ESP32C3
    esp_ble_scan_dupilcate_list_flush();
#endif
}


/**
 * @brief Delete peer device from the scan results vector.
 * @param [in] address The address of the device to delete from the results.
 * @details After disconnecting, it may be required in the case we were connected to a device without a public address.
 */
void NimBLEScan::erase(const NimBLEAddress &address) {
    NIMBLE_LOGD(LOG_TAG, "erase device: %s", address.toString().c_str());

    for(auto it = m_scanResults.m_advertisedDevicesVector.begin(); it != m_scanResults.m_advertisedDevicesVector.end(); ++it) {
        if((*it)->getAddress() == address) {
            delete *it;
            m_scanResults.m_advertisedDevicesVector.erase(it);
            break;
        }
    }
}


/**
 * @brief Called when host reset, we set a flag to stop scanning until synced.
 */
void NimBLEScan::onHostReset() {
    m_ignoreResults = true;
}


/**
 * @brief If the host reset and re-synced this is called.
 * If the application was scanning indefinitely with a callback, restart it.
 */
void NimBLEScan::onHostSync() {
    m_ignoreResults = false;

    if(m_duration == 0 && m_pAdvertisedDeviceCallbacks != nullptr) {
        start(m_duration, m_scanCompleteCB);
    }
}

/**
 * @brief Get the results of the scan.
 * @return NimBLEScanResults object.
 */
NimBLEScanResults NimBLEScan::getResults() {
    return m_scanResults;
}


/**
 * @brief Clear the results of the scan.
 */
void NimBLEScan::clearResults() {
    for(auto &it: m_scanResults.m_advertisedDevicesVector) {
        delete it;
    }
    m_scanResults.m_advertisedDevicesVector.clear();
    clearDuplicateCache();
}


/**
 * @brief Dump the scan results to the log.
 */
void NimBLEScanResults::dump() {
    NIMBLE_LOGD(LOG_TAG, ">> Dump scan results:");
    for (int i=0; i<getCount(); i++) {
        NIMBLE_LOGI(LOG_TAG, "- %s", getDevice(i).toString().c_str());
    }
} // dump


/**
 * @brief Get the count of devices found in the last scan.
 * @return The number of devices found in the last scan.
 */
int NimBLEScanResults::getCount() {
    return m_advertisedDevicesVector.size();
} // getCount


/**
 * @brief Return the specified device at the given index.
 * The index should be between 0 and getCount()-1.
 * @param [in] i The index of the device.
 * @return The device at the specified index.
 */
NimBLEAdvertisedDevice NimBLEScanResults::getDevice(uint32_t i) {
    return *m_advertisedDevicesVector[i];
}


/**
 * @brief Get iterator to the beginning of the vector of advertised device pointers.
 * @return An iterator to the beginning of the vector of advertised device pointers.
 */
std::vector<NimBLEAdvertisedDevice*>::iterator NimBLEScanResults::begin() {
    return m_advertisedDevicesVector.begin();
}


/**
 * @brief Get iterator to the end of the vector of advertised device pointers.
 * @return An iterator to the end of the vector of advertised device pointers.
 */
std::vector<NimBLEAdvertisedDevice*>::iterator NimBLEScanResults::end() {
    return m_advertisedDevicesVector.end();
}


/**
 * @brief Get a pointer to the specified device at the given address.
 * If the address is not found a nullptr is returned.
 * @param [in] address The address of the device.
 * @return A pointer to the device at the specified address.
 */
NimBLEAdvertisedDevice *NimBLEScanResults::getDevice(const NimBLEAddress &address) {
    for(size_t index = 0; index < m_advertisedDevicesVector.size(); index++) {
        if(m_advertisedDevicesVector[index]->getAddress() == address) {
            return m_advertisedDevicesVector[index];
        }
    }

    return nullptr;
}

#endif // #if defined(CONFIG_BT_NIMBLE_ROLE_OBSERVER)
#endif /* CONFIG_BT_ENABLED */
