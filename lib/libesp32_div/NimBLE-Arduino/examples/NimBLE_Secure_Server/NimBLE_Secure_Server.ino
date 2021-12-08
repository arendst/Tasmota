/** NimBLE_Secure_Server Demo:
 *  
 * This example demonstrates the secure passkey protected conenction and communication between an esp32 server and an esp32 client.
 * Please note that esp32 stores auth info in nvs memory. After a successful connection it is possible that a passkey change will be ineffective.
 * To avoid this clear the memory of the esp32's between security testings. esptool.py is capable of this, example: esptool.py --port /dev/ttyUSB0 erase_flash.
 *
 *  Created: on Jan 08 2021
 *      Author: mblasee
 */

#include <NimBLEDevice.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting NimBLE Server");
  NimBLEDevice::init("NimBLE");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setSecurityPasskey(123456);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);
  NimBLEServer *pServer = NimBLEDevice::createServer();
  NimBLEService *pService = pServer->createService("ABCD");
  NimBLECharacteristic *pNonSecureCharacteristic = pService->createCharacteristic("1234", NIMBLE_PROPERTY::READ );
  NimBLECharacteristic *pSecureCharacteristic = pService->createCharacteristic("1235", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN);

  pService->start();
  pNonSecureCharacteristic->setValue("Hello Non Secure BLE");
  pSecureCharacteristic->setValue("Hello Secure BLE");

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("ABCD");
  pAdvertising->start();
}

void loop() {
}
