/*
 * NimBLE_Scan_Whitelist demo
 *
 * Created May 16, 2021
 * Author: h2zero
 */

#include <NimBLEDevice.h>

int scanTime = 5; //In seconds
NimBLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice* advertisedDevice) {
    Serial.printf("Advertised Device: %s \n", advertisedDevice->toString().c_str());
    /*
     * Here we add the device scanned to the whitelist based on service data but any
     * advertised data can be used for your preffered data.
     */
    if (advertisedDevice->haveServiceData()) {
      /* If this is a device with data we want to capture, add it to the whitelist */
      if (advertisedDevice->getServiceData(NimBLEUUID("AABB")) != "") {
        Serial.printf("Adding %s to whitelist\n", std::string(advertisedDevice->getAddress()).c_str());
        NimBLEDevice::whiteListAdd(advertisedDevice->getAddress());
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  NimBLEDevice::init("");
  pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setFilterPolicy(BLE_HCI_SCAN_FILT_NO_WL);
  pBLEScan->setWindow(99);
}

void loop() {
  NimBLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");

  Serial.println("Whitelist contains:");
  for (auto i=0; i<NimBLEDevice::getWhiteListCount(); ++i) {
    Serial.println(NimBLEDevice::getWhiteListAddress(i).toString().c_str());
  }

  /*
   * If we have addresses in the whitelist enable the filter unless no devices were found
   * then scan without so we can find any new devices that we want.
   */
  if (NimBLEDevice::getWhiteListCount() > 0) {
    if (foundDevices.getCount() == 0) {
      pBLEScan->setFilterPolicy(BLE_HCI_SCAN_FILT_NO_WL);
    } else {
      pBLEScan->setFilterPolicy(BLE_HCI_SCAN_FILT_USE_WL);
    }
  }
  pBLEScan->clearResults();
  delay(2000);
}
