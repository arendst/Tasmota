/** NimBLE_Service_Data_Advertiser Demo:
 *
 *  Simple demo of advertising service data that changes every 5 seconds
 *
 *  Created: on February 7 2021
 *      Author: H2zero
 * 
*/

#include <NimBLEDevice.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

static NimBLEUUID dataUuid(SERVICE_UUID);
static NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
static uint32_t count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  
  NimBLEDevice::init("svc data");
}

void loop() {
  pAdvertising->stop();
  pAdvertising->setServiceData(dataUuid, std::string((char*)&count, sizeof(count)));  
  pAdvertising->start();
  
  Serial.printf("Advertising count = %d\n", count);
  count++;
  delay(5000);
}
