/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara
    Refactored back to IDF by H2zero

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
***********************/
#include <NimBLEDevice.h>

extern "C" {void app_main(void);}

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer, BLEConnInfo& connInfo) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer, BLEConnInfo& connInfo, int reason) {
      deviceConnected = false;
    }
/***************** New - Security handled here ********************
****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest(){
    printf("Server PassKeyRequest\n");
    return 123456;
  }

  bool onConfirmPIN(uint32_t pass_key){
    printf("The passkey YES/NO number: %" PRIu32"\n", pass_key);
    return true;
  }

  void onAuthenticationComplete(BLEConnInfo& connInfo){
    printf("Starting BLE work!\n");
  }
/*******************************************************************/
};

void connectedTask (void * parameter){
    for(;;) {
        // notify changed value
        if (deviceConnected) {
            pCharacteristic->setValue((uint8_t*)&value, 4);
            pCharacteristic->notify();
            value++;
            vTaskDelay(100/portTICK_PERIOD_MS);  // bluetooth stack will go into congestion, if too many packets are sent
        }
        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
            vTaskDelay(500/portTICK_PERIOD_MS); // give the bluetooth stack the chance to get things ready
            pServer->startAdvertising(); // restart advertising
            printf("start advertising\n");
            oldDeviceConnected = deviceConnected;
        }
        // connecting
        if (deviceConnected && !oldDeviceConnected) {
            // do stuff here on connecting
            oldDeviceConnected = deviceConnected;
        }

        vTaskDelay(10/portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer
    }

    vTaskDelete(NULL);
}

void app_main(void) {
  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                /******* Enum Type NIMBLE_PROPERTY now *******
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
                **********************************************/
                      NIMBLE_PROPERTY::READ   |
                      NIMBLE_PROPERTY::WRITE  |
                      NIMBLE_PROPERTY::NOTIFY |
                      NIMBLE_PROPERTY::INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  /***************************************************
   NOTE: DO NOT create a 2902 descriptor.
   it will be created automatically if notifications
   or indications are enabled on a characteristic.

   pCharacteristic->addDescriptor(new BLE2902());
  ****************************************************/
  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  /** This method had been removed **
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  **/

  xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);

  BLEDevice::startAdvertising();
  printf("Waiting a client connection to notify...\n");
}
