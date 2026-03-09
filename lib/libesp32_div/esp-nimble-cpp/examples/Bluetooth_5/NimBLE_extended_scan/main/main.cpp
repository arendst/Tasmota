/**
 *  NimBLE Extended Scanner Demo:
 *
 *  Demonstrates the Bluetooth 5.x scanning capabilities of the NimBLE library.
 *
 *  Created: on November 28, 2024
 *      Author: H2zero
 */

#include <NimBLEDevice.h>

static uint32_t        scanTimeMs = 10 * 1000; // In milliseconds, 0 = scan forever
static NimBLEScan::Phy scanPhy    = NimBLEScan::Phy::SCAN_ALL;

/** Define a class to handle the callbacks when advertisements are received */
class ScanCallbacks : public NimBLEScanCallbacks {
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) {
        printf("Advertised Device found: %s\n PHY1: %d\n PHY2: %d\n",
               advertisedDevice->toString().c_str(),
               advertisedDevice->getPrimaryPhy(),
               advertisedDevice->getSecondaryPhy());
    }

    /** Callback to process the results of the completed scan or restart it */
    void onScanEnd(const NimBLEScanResults& scanResults, int reason) {
        printf("Scan Ended, reason: %d; found %d devices\n", reason, scanResults.getCount());

        /** Try Different PHY's */
        switch (scanPhy) {
            case NimBLEScan::Phy::SCAN_ALL:
                printf("Scanning only 1M PHY\n");
                scanPhy = NimBLEScan::Phy::SCAN_1M;
                break;
            case NimBLEScan::Phy::SCAN_1M:
                printf("Scanning only CODED PHY\n");
                scanPhy = NimBLEScan::Phy::SCAN_CODED;
                break;
            case NimBLEScan::Phy::SCAN_CODED:
                printf("Scanning all PHY's\n");
                scanPhy = NimBLEScan::Phy::SCAN_ALL;
                break;
        }

        NimBLEScan* pScan = NimBLEDevice::getScan();
        pScan->setPhy(scanPhy);
        pScan->start(scanTimeMs);
    }
} scanCallbacks;

extern "C" void app_main(void) {
    printf("Starting Extended Scanner\n");

    /** Initialize NimBLE and set the device name */
    NimBLEDevice::init("NimBLE Extended Scanner");
    NimBLEScan* pScan = NimBLEDevice::getScan();

    /** Set the callbacks that the scanner will call on events. */
    pScan->setScanCallbacks(&scanCallbacks);

    /** Use active scanning to obtain scan response data from advertisers */
    pScan->setActiveScan(true);

    /** Set the initial PHY's to scan on, default is SCAN_ALL */
    pScan->setPhy(scanPhy);

    /** Start scanning for scanTimeMs */
    pScan->start(scanTimeMs);
    printf("Scanning for peripherals\n");
}
