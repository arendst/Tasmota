/*
 * NimBLE Scan active/passive switching demo
 *
 * Demonstrates the use of the scan callbacks while alternating between passive and active scanning.
 */

#include "NimBLEDevice.h"
int scanTime = 5 * 1000; // In milliseconds, 0 = scan forever
BLEScan* pBLEScan;

bool active = false;

class scanCallbacks: public NimBLEScanCallbacks {

    void onDiscovered(NimBLEAdvertisedDevice* advertisedDevice) {
      Serial.printf("Discovered Advertised Device: %s \n", advertisedDevice->toString().c_str());
    }

    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
      Serial.printf("Advertised Device Result: %s \n", advertisedDevice->toString().c_str());
    }

    void onScanEnd(NimBLEScanResults results){
        Serial.println("Scan Ended");
        active = !active;
        pBLEScan->setActiveScan(active);
        Serial.printf("scan start, active = %u\n", active);
        pBLEScan->start(scanTime);
    }
};



void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  NimBLEDevice::init("");
  pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setScanCallbacks(new scanCallbacks());
  pBLEScan->setActiveScan(active);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  pBLEScan->start(scanTime);
}

void loop() {
}
