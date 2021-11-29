/** NimBLE_Secure_Client Demo:
 *  
 * This example demonstrates the secure passkey protected conenction and communication between an esp32 server and an esp32 client.
 * Please note that esp32 stores auth info in nvs memory. After a successful connection it is possible that a passkey change will be ineffective.
 * To avoid this clear the memory of the esp32's between security testings. esptool.py is capable of this, example: esptool.py --port /dev/ttyUSB0 erase_flash.
 *
 *  Created: on Jan 08 2021
 *      Author: mblasee
 */
 
#include <NimBLEDevice.h>

class ClientCallbacks : public NimBLEClientCallbacks
{
  uint32_t onPassKeyRequest()
  {
    Serial.println("Client Passkey Request");
    /** return the passkey to send to the server */
    /** Change this to be different from NimBLE_Secure_Server if you want to test what happens on key mismatch */
    return 123456;
  };
};
static ClientCallbacks clientCB;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting NimBLE Client");

  NimBLEDevice::init("");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_ONLY);
  NimBLEScan *pScan = NimBLEDevice::getScan();
  NimBLEScanResults results = pScan->start(5);

  NimBLEUUID serviceUuid("ABCD");

  for (int i = 0; i < results.getCount(); i++)
  {
    NimBLEAdvertisedDevice device = results.getDevice(i);
    Serial.println(device.getName().c_str());

    if (device.isAdvertisingService(serviceUuid))
    {
      NimBLEClient *pClient = NimBLEDevice::createClient();
      pClient->setClientCallbacks(&clientCB, false);

      if (pClient->connect(&device))
      {
        pClient->secureConnection();
        NimBLERemoteService *pService = pClient->getService(serviceUuid);
        if (pService != nullptr)
        {
          NimBLERemoteCharacteristic *pNonSecureCharacteristic = pService->getCharacteristic("1234");
          
          if (pNonSecureCharacteristic != nullptr)
          {
            // Testing to read a non secured characteristic, you should be able to read this even if you have mismatching passkeys.
            std::string value = pNonSecureCharacteristic->readValue();
            // print or do whatever you need with the value
            Serial.println(value.c_str());
          }

          NimBLERemoteCharacteristic *pSecureCharacteristic = pService->getCharacteristic("1235");

          if (pSecureCharacteristic != nullptr)
          {
            // Testing to read a secured characteristic, you should be able to read this only if you have matching passkeys, otherwise you should
            // get an error like this. E NimBLERemoteCharacteristic: "<< readValue rc=261"
            // This means you are trying to do something without the proper permissions. 
            std::string value = pSecureCharacteristic->readValue();
            // print or do whatever you need with the value
            Serial.println(value.c_str());
          }      
        }
      }
      else
      {
        // failed to connect
        Serial.println("failed to connect");
      }

      NimBLEDevice::deleteClient(pClient);
    }
  }
}

void loop()
{
}
