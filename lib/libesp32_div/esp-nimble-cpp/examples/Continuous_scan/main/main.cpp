/**
 *  Continuous Scan Example
 *
 *  This example demonstrates how to continuously scan for BLE devices.
 *  When devices are found the onDiscovered and onResults callbacks will be called with the device data.
 *  The scan will not store the results, only the callbacks will be used
 *  When the scan timeout is reached the onScanEnd callback will be called and the scan will be restarted.
 *  This will clear the duplicate cache in the controller and allow the same devices to be reported again.
 *
 *  Created: on March 24 2020
 *      Author: H2zero
 */

#include <NimBLEDevice.h>

static constexpr uint32_t scanTime = 30 * 1000; // 30 seconds scan time.

class scanCallbacks : public NimBLEScanCallbacks {
    /** Initial discovery, advertisement data only. */
    void onDiscovered(const NimBLEAdvertisedDevice* advertisedDevice) override {
        printf("Discovered Device: %s\n", advertisedDevice->toString().c_str());
    }

    /**
     *  If active scanning the result here will have the scan response data.
     *  If not active scanning then this will be the same as onDiscovered.
     */
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
        printf("Device result: %s\n", advertisedDevice->toString().c_str());
    }

    void onScanEnd(const NimBLEScanResults& results, int reason) override {
        printf("Scan ended reason = %d; restarting scan\n", reason);
        NimBLEDevice::getScan()->start(scanTime, false, true);
    }
} scanCallbacks;

extern "C" void app_main() {
    NimBLEDevice::init("");                         // Initialize the device, you can specify a device name if you want.
    NimBLEScan* pBLEScan = NimBLEDevice::getScan(); // Create the scan object.
    pBLEScan->setScanCallbacks(&scanCallbacks, false); // Set the callback for when devices are discovered, no duplicates.
    pBLEScan->setActiveScan(true);          // Set active scanning, this will get more data from the advertiser.
    pBLEScan->setMaxResults(0);             // Do not store the scan results, use callback only.
    pBLEScan->start(scanTime, false, true); // duration, not a continuation of last scan, restart to get all devices again.
    printf("Scanning...\n");
}