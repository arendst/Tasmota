/*
 * NimBLE_Server_Whitelist demo
 *
 * Created May 17, 2021
 * Author: h2zero
 */

#include <NimBLEDevice.h>

NimBLECharacteristic* pCharacteristic = nullptr;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
      // Peer disconnected, add them to the whitelist
      // This allows us to use the whitelist to filter connection attempts
      // which will minimize reconnection time.
      NimBLEDevice::whiteListAdd(NimBLEAddress(desc->peer_ota_addr));
      deviceConnected = false;
    }
};

void onAdvComplete(NimBLEAdvertising *pAdvertising) {
    Serial.println("Advertising stopped");
    if (deviceConnected) {
        return;
    }
    // If advertising timed out without connection start advertising without whitelist filter
    pAdvertising->setScanFilter(false,false);
    pAdvertising->start();
}


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  NimBLEDevice::init("ESP32");

  // Create the BLE Server
  NimBLEServer* pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  pServer->advertiseOnDisconnect(false);

  // Create the BLE Service
  NimBLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      NIMBLE_PROPERTY::READ   |
                      NIMBLE_PROPERTY::WRITE  |
                      NIMBLE_PROPERTY::NOTIFY );

  // Start the service
  pService->start();

  // Start advertising
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected) {
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        value++;
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
        if (NimBLEDevice::getWhiteListCount() > 0) {
            // Allow anyone to scan but only whitelisted can connect.
            pAdvertising->setScanFilter(false,true);
        }
        // advertise with whitelist for 30 seconds
        pAdvertising->start(30, onAdvComplete);
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    delay(2000);
}
