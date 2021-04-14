/** Example of continuous scanning for BLE advertisements.
 * This example will scan forever while consuming as few resources as possible
 * and report all advertisments on the serial monitor.
 *
 * Created: on January 31 2021
 *      Author: H2zero
 *
 */

#include "NimBLEDevice.h"

NimBLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {
    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice->toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

/** *Optional* Sets the filtering mode used by the scanner in the BLE controller.
 *
 *  Can be one of:
 *  CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE (0) (default)
 *  Filter by device address only, advertisements from the same address will be reported only once.
 *
 *  CONFIG_BTDM_SCAN_DUPL_TYPE_DATA (1)
 *  Filter by data only, advertisements with the same data will only be reported once,
 *  even from different addresses.
 *
 *  CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE (2)
 *  Filter by address and data, advertisements from the same address will be reported only once,
 *  except if the data in the advertisement has changed, then it will be reported again.
 *
 *  Can only be used BEFORE calling NimBLEDevice::init.
*/
  NimBLEDevice::setScanFilterMode(CONFIG_BTDM_SCAN_DUPL_TYPE_DEVICE);

/** *Optional* Sets the scan filter cache size in the BLE controller.
 *  When the number of duplicate advertisements seen by the controller
 *  reaches this value it will clear the cache and start reporting previously
 *  seen devices. The larger this number, the longer time between repeated
 *  device reports. Range 10 - 1000. (default 20)
 *
 *  Can only be used BEFORE calling NimBLEDevice::init.
 */
  NimBLEDevice::setScanDuplicateCacheSize(200);

  NimBLEDevice::init("");

  pBLEScan = NimBLEDevice::getScan(); //create new scan
  // Set the callback for when devices are discovered, no duplicates.
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), false);
  pBLEScan->setActiveScan(true); // Set active scanning, this will get more data from the advertiser.
  pBLEScan->setInterval(97); // How often the scan occurs / switches channels; in milliseconds,
  pBLEScan->setWindow(37);  // How long to scan during the interval; in milliseconds.
  pBLEScan->setMaxResults(0); // do not store the scan results, use callback only.
}

void loop() {
  // If an error occurs that stops the scan, it will be restarted here.
  if(pBLEScan->isScanning() == false) {
      // Start scan with: duration = 0 seconds(forever), no scan end callback, not a continuation of a previous scan.
      pBLEScan->start(0, nullptr, false);
  }

  delay(2000);
}