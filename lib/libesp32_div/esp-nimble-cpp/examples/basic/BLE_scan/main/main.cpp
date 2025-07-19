/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
   Refactored back to IDF by H2zero
*/

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
***********************/

#include <NimBLEDevice.h>

extern "C"{void app_main(void);}

int scanTime = 5 * 1000; // In milliseconds, 0 = scan forever
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice* advertisedDevice) {
      printf("Advertised Device: %s \n", advertisedDevice->toString().c_str());
    }
};

void scanTask (void * parameter){
    for(;;) {
        // put your main code here, to run repeatedly:
        BLEScanResults foundDevices = pBLEScan->getResults(scanTime, false);
        printf("Devices found: %d\n", foundDevices.getCount());
        printf("Scan done!\n");
        pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
        vTaskDelay(2000/portTICK_PERIOD_MS); // Delay a second between loops.
    }
    
    vTaskDelete(NULL);
}

void app_main(void) {
  printf("Scanning...\n");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setScanCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  xTaskCreate(scanTask, "scanTask", 5000, NULL, 1, NULL);
}
