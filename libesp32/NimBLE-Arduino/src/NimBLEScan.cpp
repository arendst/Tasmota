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

#include "NimBLEScan.h"
#include "NimBLEUtils.h"
#include "NimBLEDevice.h"
#include "NimBLELog.h"

#include <string>

static const char* LOG_TAG = "NimBLEScan";

/*
 * Scanning filter policy
 *  NO_WL:
 *      Scanner processes all advertising packets (white list not used) except
 *      directed, connectable advertising packets not sent to the scanner.
 *  USE_WL:
 *      Scanner processes advertisements from white list only. A connectable,
 *      directed advertisment is ignored unless it contains scanners address.
 *  NO_WL_INITA:
 *      Scanner process all advertising packets (white list not used). A
 *      connectable, directed advertisement shall not be ignored if the InitA
 *      is a resolvable private address.
 *  USE_WL_INITA:
 *      Scanner process advertisements from white list only. A connectable,
 *      directed advertisement shall not be ignored if the InitA is a
 *      resolvable private address.
 */
 
//#define BLE_HCI_SCAN_FILT_NO_WL             (0)
//#define BLE_HCI_SCAN_FILT_USE_WL            (1)
//#define BLE_HCI_SCAN_FILT_NO_WL_INITA       (2)
//#define BLE_HCI_SCAN_FILT_USE_WL_INITA      (3)
//#define BLE_HCI_SCAN_FILT_MAX               (3)


/**
 * @brief Scan constuctor.
 */
NimBLEScan::NimBLEScan() {
    uint8_t own_addr_type;
    if(ble_hs_id_infer_auto(0, &own_addr_type) !=0){
        NIMBLE_LOGE(LOG_TAG, "error determining address type\n");
        return;
    }
    m_own_addr_type                  = own_addr_type;
    m_scan_params.filter_policy      = BLE_HCI_SCAN_FILT_NO_WL;
    m_scan_params.passive            = 1; // If set, don’t send scan requests to advertisers (i.e., don’t request additional advertising data).
    m_scan_params.itvl               = 0; // This is defined as the time interval from when the Controller started its last LE scan until it begins the subsequent LE scan. (units=0.625 msec)
    m_scan_params.window             = 0; // The duration of the LE scan. LE_Scan_Window shall be less than or equal to LE_Scan_Interval (units=0.625 msec)
    m_scan_params.limited            = 0; // If set, only discover devices in limited discoverable mode.
    m_scan_params.filter_duplicates  = 1; // If set, the controller ignores all but the first advertisement from each device.
    m_pAdvertisedDeviceCallbacks     = nullptr;
    m_stopped                        = true;
    m_wantDuplicates                 = false;
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

            // Print advertisement data
    //        print_adv_fields(&fields);

            // If we are not scanning, nothing to do with the extra results.
            if (pScan->m_stopped) { 
                return 0;
            }

            // Examine our list of ignored addresses and stop processing if we don't want to see it or are already connected
            if(NimBLEDevice::isIgnored(advertisedAddress)) {
                NIMBLE_LOGI(LOG_TAG, "Ignoring device: address: %s", advertisedAddress.toString().c_str());
                return 0;
            }
            
			NimBLEAdvertisedDevice* advertisedDevice = nullptr;
						
            // If we've seen this device before get a pointer to it from the map
            auto it = pScan->m_scanResults.m_advertisedDevicesMap.find(advertisedAddress.toString());
            if(it != pScan->m_scanResults.m_advertisedDevicesMap.cend()) {
                advertisedDevice = (*it).second;
            }

            // If we haven't seen this device before; create a new instance and insert it in the map.
            // Otherwise just update the relevant parameters of the already known device.
            if(advertisedDevice == nullptr){
                advertisedDevice = new NimBLEAdvertisedDevice();
                advertisedDevice->setAddressType(event->disc.addr.type);
                advertisedDevice->setAddress(advertisedAddress);
                //NIMBLE_LOGE(LOG_TAG, "advertisement type: %d, %s",event->disc.event_type, NimBLEUtils::advTypeToString(event->disc.event_type));
                advertisedDevice->setAdvType(event->disc.event_type);
                pScan->m_scanResults.m_advertisedDevicesMap.insert(std::pair<std::string, NimBLEAdvertisedDevice*>(advertisedAddress.toString(), advertisedDevice));
                NIMBLE_LOGI(LOG_TAG, "NEW DEVICE FOUND: %s", advertisedAddress.toString().c_str());
            }
            else{
                NIMBLE_LOGI(LOG_TAG, "UPDATING PREVIOUSLY FOUND DEVICE: %s", advertisedAddress.toString().c_str());
            }
            advertisedDevice->setRSSI(event->disc.rssi); 
            advertisedDevice->parseAdvertisement(&fields);
            advertisedDevice->setScan(pScan);
            advertisedDevice->setAdvertisementResult(event->disc.data, event->disc.length_data);

            if (pScan->m_pAdvertisedDeviceCallbacks) {
                // If not active scanning report the result to the listener.
                if(pScan->m_scan_params.passive) {
                    pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);
                // Otherwise wait for the scan response so we can report all of the data at once.
                } else if (event->disc.event_type == BLE_HCI_ADV_RPT_EVTYPE_SCAN_RSP) {
                    pScan->m_pAdvertisedDeviceCallbacks->onResult(advertisedDevice);
                }
                //m_pAdvertisedDeviceCallbacks->onResult(*advertisedDevice);
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
            pScan->m_semaphoreScanEnd.give();   
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
 * @brief Set the call backs to be invoked.
 * @param [in] pAdvertisedDeviceCallbacks Call backs to be invoked.
 * @param [in] wantDuplicates  True if we wish to be called back with duplicates.  Default is false.
 */
void NimBLEScan::setAdvertisedDeviceCallbacks(NimBLEAdvertisedDeviceCallbacks* pAdvertisedDeviceCallbacks/*, bool wantDuplicates*/) {
    //m_wantDuplicates = wantDuplicates;
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
    m_semaphoreScanEnd.take("start");
        
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
    //  then we should not clear map or we will connect the same device few times
    if(!is_continue) {
        clearResults();
    }
    
    int rc = 0;
    do{    
        rc = ble_gap_disc(m_own_addr_type, duration, &m_scan_params, 
                                    NimBLEScan::handleGapEvent, this);
        if(rc == BLE_HS_EBUSY) {
            vTaskDelay(2);
        }
    } while(rc == BLE_HS_EBUSY);
    
    if (rc != 0 && rc != BLE_HS_EDONE) {
        NIMBLE_LOGE(LOG_TAG, "Error initiating GAP discovery procedure; rc=%d, %s",
                                        rc, NimBLEUtils::returnCodeToString(rc));
        m_stopped = true;
        m_semaphoreScanEnd.give();
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
    if(start(duration, nullptr, is_continue)) {
        m_semaphoreScanEnd.wait("start");   // Wait for the semaphore to release.
    }
    return m_scanResults;
} // start


/**
 * @brief Stop an in progress scan.
 * @return N/A.
 */
void NimBLEScan::stop() {
    NIMBLE_LOGD(LOG_TAG, ">> stop()");
	
    int rc = ble_gap_disc_cancel();
    if (rc != 0 && rc != BLE_HS_EALREADY) {
        NIMBLE_LOGE(LOG_TAG, "Failed to cancel scan; rc=%d\n", rc);
        return;
    }

    m_stopped = true;
    
    if (m_scanCompleteCB != nullptr) {
        m_scanCompleteCB(m_scanResults);
    }
    
    m_semaphoreScanEnd.give();
    
    NIMBLE_LOGD(LOG_TAG, "<< stop()");
} // stop


// delete peer device from cache after disconnecting, it is required in case we are connecting to devices with not public address
void NimBLEScan::erase(NimBLEAddress address) {
    NIMBLE_LOGI(LOG_TAG, "erase device: %s", address.toString().c_str());
    NimBLEAdvertisedDevice *advertisedDevice = m_scanResults.m_advertisedDevicesMap.find(address.toString())->second;
    m_scanResults.m_advertisedDevicesMap.erase(address.toString());
    delete advertisedDevice;
}


/**
 * @brief If the host reset the scan will have stopped so we should flag it and release the semaphore.
 * @return N/A.
 */
void NimBLEScan::onHostReset() {
    m_stopped = true;
    m_semaphoreScanEnd.give();
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
    for(auto _dev : m_scanResults.m_advertisedDevicesMap){
        delete _dev.second;
    }
    m_scanResults.m_advertisedDevicesMap.clear();
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
    return m_advertisedDevicesMap.size();
} // getCount


/**
 * @brief Return the specified device at the given index.
 * The index should be between 0 and getCount()-1.
 * @param [in] i The index of the device.
 * @return The device at the specified index.
 */
NimBLEAdvertisedDevice NimBLEScanResults::getDevice(uint32_t i) {
    uint32_t x = 0;
    NimBLEAdvertisedDevice dev = *m_advertisedDevicesMap.begin()->second;
    for (auto it = m_advertisedDevicesMap.begin(); it != m_advertisedDevicesMap.end(); it++) {
        dev = *it->second;
        if (x==i)   break;
        x++;
    }
    return dev;
}

#endif /* CONFIG_BT_ENABLED */
