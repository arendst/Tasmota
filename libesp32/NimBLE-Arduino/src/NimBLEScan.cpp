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
    m_own_addr_type                  = 0;
    m_scan_params.filter_policy      = BLE_HCI_SCAN_FILT_NO_WL;
    m_scan_params.passive            = 1; // If set, don’t send scan requests to advertisers (i.e., don’t request additional advertising data).
    m_scan_params.itvl               = 0; // This is defined as the time interval from when the Controller started its last LE scan until it begins the subsequent LE scan. (units=0.625 msec)
    m_scan_params.window             = 0; // The duration of the LE scan. LE_Scan_Window shall be less than or equal to LE_Scan_Interval (units=0.625 msec)
    m_scan_params.limited            = 0; // If set, only discover devices in limited discoverable mode.
    m_scan_params.filter_duplicates  = 0; // If set, the controller ignores all but the first advertisement from each device.
    m_pAdvertisedDeviceCallbacks     = nullptr;
    m_stopped                        = true;
    m_wantDuplicates                 = false;
    m_pTaskData                      = nullptr;
}


/**
 * @brief Handle GAP events related to scans.
 * @param [in] event The event type for this event.
 * @param [in] param Parameter data for this event.
 */
/*STATIC*/int NimBLEScan::handleGapEvent(ble_gap_event* event, void* arg) {

    NimBLEScan* pScan = (NimBLEScan*)arg;
    struct ble_hs_adv_fields fields;
    int rc = 0;

    switch(event->type) {

        case BLE_GAP_EVENT_DISC: {
            if(pScan->m_stopped) {
                NIMBLE_LOGE(LOG_TAG, "Scan stop called, ignoring results.");
                return 0;
            }

            rc = ble_hs_adv_parse_fields(&fields, event->disc.data,
                                     event->disc.length_data);
            if (rc != 0) {
                NIMBLE_LOGE(LOG_TAG, "Gap Event Parse ERROR.");
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
            if(advertisedDevice == nullptr){
                advertisedDevice = new NimBLEAdvertisedDevice();
                advertisedDevice->setAddressType(event->disc.addr.type);
                advertisedDevice->setAddress(advertisedAddress);
                advertisedDevice->setAdvType(event->disc.event_type);
                pScan->m_scanResults.m_advertisedDevicesVector.push_back(advertisedDevice);
                NIMBLE_LOGI(LOG_TAG, "NEW DEVICE FOUND: %s", advertisedAddress.toString().c_str());
            }
            else{
                NIMBLE_LOGI(LOG_TAG, "UPDATING PREVIOUSLY FOUND DEVICE: %s", advertisedAddress.toString().c_str());
            }
            advertisedDevice->setRSSI(event->disc.rssi);
            advertisedDevice->parseAdvertisement(&fields);
            advertisedDevice->setScan(pScan);
            advertisedDevice->setAdvertisementResult(event->disc.data, event->disc.length_data);
            advertisedDevice->m_timestamp = time(nullptr);

            if (pScan->m_pAdvertisedDeviceCallbacks) {
                if(pScan->m_wantDuplicates || !advertisedDevice->m_callbackSent) {
                    // If not active scanning report the result to the listener.
                    if(pScan->m_scan_params.passive || event->disc.event_type == BLE_HCI_ADV_TYPE_ADV_NONCONN_IND) {
                        advertisedDevice->m_callbackSent = true;
                        pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);

                    // Otherwise wait for the scan response so we can report all of the data at once.
                    } else if (event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                        advertisedDevice->m_callbackSent = true;
                        pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);
                    }
                }
            }

            return 0;
        }
        case BLE_GAP_EVENT_DISC_COMPLETE: {
            NIMBLE_LOGD(LOG_TAG, "discovery complete; reason=%d",
                    event->disc_complete.reason);

            if (pScan->m_scanCompleteCB != nullptr) {
                pScan->m_scanCompleteCB(pScan->m_scanResults);
            }

            pScan->m_stopped = true;
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
 * The default is a passive scan.  An active scan means that we will wish a scan response.
 * @param [in] active If true, we perform an active scan otherwise a passive scan.
 * @return N/A.
 */
void NimBLEScan::setActiveScan(bool active) {
    if (active) {
        m_scan_params.passive = 0;
    } else {
        m_scan_params.passive = 1;
    }
} // setActiveScan


/**
 * @brief Set whether or not the BLE controller should only report results
 * from devices it has not already seen.
 * @param [in] active If true, scanned devices will only be reported once.
 * @details The controller has a limited buffer and will start reporting
 * dupicate devices once the limit is reached.
 */
void NimBLEScan::setDuplicateFilter(bool active) {
    m_scan_params.filter_duplicates = active;
} // setDuplicateFilter


/**
 * @brief Set whether or not the BLE controller only report scan results
 * from devices advertising in limited discovery mode, i.e. directed advertising.
 * @param [in] active If true, only limited discovery devices will be in scan results.
 */
void NimBLEScan::setLimitedOnly(bool active) {
    m_scan_params.limited = active;
} // setLimited


/**
 * @brief Sets the scan filter policy.
 * @param [in] filter Can be one of:
 *  BLE_HCI_SCAN_FILT_NO_WL             (0)
 *      Scanner processes all advertising packets (white list not used) except
 *      directed, connectable advertising packets not sent to the scanner.
 *  BLE_HCI_SCAN_FILT_USE_WL            (1)
 *      Scanner processes advertisements from white list only. A connectable,
 *      directed advertisment is ignored unless it contains scanners address.
 *  BLE_HCI_SCAN_FILT_NO_WL_INITA       (2)
 *      Scanner process all advertising packets (white list not used). A
 *      connectable, directed advertisement shall not be ignored if the InitA
 *      is a resolvable private address.
 *  BLE_HCI_SCAN_FILT_USE_WL_INITA      (3)
 *      Scanner process advertisements from white list only. A connectable,
 *      directed advertisement shall not be ignored if the InitA is a
 *      resolvable private address.
 */
void NimBLEScan::setFilterPolicy(uint8_t filter) {
    m_scan_params.filter_policy = filter;
} // setFilterPolicy


/**
 * @brief Set the call backs to be invoked.
 * @param [in] pAdvertisedDeviceCallbacks Call backs to be invoked.
 * @param [in] wantDuplicates  True if we wish to be called back with duplicates.  Default is false.
 */
void NimBLEScan::setAdvertisedDeviceCallbacks(NimBLEAdvertisedDeviceCallbacks* pAdvertisedDeviceCallbacks,
                                              bool wantDuplicates) {
    m_wantDuplicates = wantDuplicates;
    m_pAdvertisedDeviceCallbacks = pAdvertisedDeviceCallbacks;
} // setAdvertisedDeviceCallbacks


/**
 * @brief Set the interval to scan.
 * @param [in] The interval in msecs.
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
 * @brief Start scanning.
 * @param [in] duration The duration in seconds for which to scan.
 * @param [in] scanCompleteCB A function to be called when scanning has completed.
 * @param [in] are we continue scan (true) or we want to clear stored devices (false)
 * @return True if scan started or false if there was an error.
 */
bool NimBLEScan::start(uint32_t duration, void (*scanCompleteCB)(NimBLEScanResults), bool is_continue) {
    NIMBLE_LOGD(LOG_TAG, ">> start(duration=%d)", duration);

    // If Host is not synced we cannot start scanning.
    if(!NimBLEDevice::m_synced) {
        NIMBLE_LOGC(LOG_TAG, "Host reset, wait for sync.");
        return false;
    }

    if(ble_gap_conn_active()) {
        NIMBLE_LOGE(LOG_TAG, "Connection in progress - must wait.");
        return false;
    }

    // If we are already scanning don't start again or we will get stuck on the semaphore.
    if(!m_stopped || ble_gap_disc_active()) { // double check - can cause host reset.
        NIMBLE_LOGE(LOG_TAG, "Scan already in progress");
        return false;
    }

    m_stopped = false;

    // Save the callback to be invoked when the scan completes.
    m_scanCompleteCB = scanCompleteCB;
    // Save the duration in the case that the host is reset so we can reuse it.
    m_duration = duration;

    // If 0 duration specified then we assume a continuous scan is desired.
    if(duration == 0){
        duration = BLE_HS_FOREVER;
    }
    else{
        duration = duration*1000; // convert duration to milliseconds
    }

    //  if we are connecting to devices that are advertising even after being connected, multiconnecting peripherals
    //  then we should not clear vector or we will connect the same device few times
    if(!is_continue) {
        clearResults();
    }

    int rc = 0;
    do{
        rc = ble_gap_disc(m_own_addr_type, duration, &m_scan_params,
                                    NimBLEScan::handleGapEvent, this);
        if(rc == BLE_HS_EBUSY) {
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }
    } while(rc == BLE_HS_EBUSY);

    if (rc != 0 && rc != BLE_HS_EDONE) {
        NIMBLE_LOGE(LOG_TAG, "Error initiating GAP discovery procedure; rc=%d, %s",
                                        rc, NimBLEUtils::returnCodeToString(rc));
        m_stopped = true;
        return false;
    }

    NIMBLE_LOGD(LOG_TAG, "<< start()");
    return true;
} // start


/**
 * @brief Start scanning and block until scanning has been completed.
 * @param [in] duration The duration in seconds for which to scan.
 * @return The BLEScanResults.
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
 * @return N/A.
 */
bool NimBLEScan::stop() {
    NIMBLE_LOGD(LOG_TAG, ">> stop()");

    int rc = ble_gap_disc_cancel();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "Failed to cancel scan; rc=%d\n", rc);
        return false;
    }

    m_stopped = true;

    if (m_scanCompleteCB != nullptr) {
        m_scanCompleteCB(m_scanResults);
    }

    if(m_pTaskData != nullptr) {
        xTaskNotifyGive(m_pTaskData->task);
    }

    NIMBLE_LOGD(LOG_TAG, "<< stop()");
    return true;
} // stop


// delete peer device from cache after disconnecting, it is required in case we are connecting to devices with not public address
void NimBLEScan::erase(const NimBLEAddress &address) {
    NIMBLE_LOGI(LOG_TAG, "erase device: %s", address.toString().c_str());

    for(auto it = m_scanResults.m_advertisedDevicesVector.begin(); it != m_scanResults.m_advertisedDevicesVector.end(); ++it) {
        if((*it)->getAddress() == address) {
            delete *it;
            m_scanResults.m_advertisedDevicesVector.erase(it);
            break;
        }
    }
}


/**
 * @brief If the host reset the scan will have stopped so we should flag it and release the semaphore.
 * @return N/A.
 */
void NimBLEScan::onHostReset() {
    m_stopped = true;
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
 * @brief Return the count of devices found in the last scan.
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
 * @brief Return a pointer to the specified device at the given address.
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
