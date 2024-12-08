/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    Refactored back to IDF by H2zero

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
***********************/
#include <NimBLEDevice.h>

extern "C"{void app_main(void);}

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


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
    uint32_t onPassKeyDisplay(){
        printf("Server Passkey Display\n");
        /** This should return a random 6 digit number for security
         *  or make your own static passkey as done here.
         */
        return 123456;
    };

    void onConfirmPIN(const NimBLEConnInfo& connInfo, uint32_t pass_key){
        printf("The passkey YES/NO number: %" PRIu32 "\n", pass_key);
        /** Inject false if passkeys don't match. */
        NimBLEDevice::injectConfirmPIN(connInfo, true);
    };

    void onAuthenticationComplete(const NimBLEConnInfo& connInfo){
        /** Check that encryption was successful, if not we disconnect the client */
        if(!connInfo.isEncrypted()) {
            NimBLEDevice::getServer()->disconnect(connInfo.getConnHandle());
            printf("Encrypt connection failed - disconnecting client\n");
            return;
        }
        printf("Starting BLE work!");
    };
  /*******************************************************************/
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic, BLEConnInfo& connInfo) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        printf("*********\n");
        printf("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          printf("%d", rxValue[i]);

        printf("\n*********\n");
      }
    }
};

void connectedTask (void * parameter){
    for(;;) {
        if (deviceConnected) {
            pTxCharacteristic->setValue(&txValue, 1);
            pTxCharacteristic->notify();
            txValue++;
        }

        // disconnecting
        if (!deviceConnected && oldDeviceConnected) {
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
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID_TX,
                                    /******* Enum Type NIMBLE_PROPERTY now *******
                                        BLECharacteristic::PROPERTY_NOTIFY
                                        );
                                    **********************************************/
                                        NIMBLE_PROPERTY::NOTIFY
                                       );

  /***************************************************
   NOTE: DO NOT create a 2902 descriptor
   it will be created automatically if notifications
   or indications are enabled on a characteristic.

   pCharacteristic->addDescriptor(new BLE2902());
  ****************************************************/

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                                            CHARACTERISTIC_UUID_RX,
                                    /******* Enum Type NIMBLE_PROPERTY now *******
                                            BLECharacteristic::PROPERTY_WRITE
                                            );
                                    *********************************************/
                                            NIMBLE_PROPERTY::WRITE
                                            );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);

  // Start advertising
  pServer->getAdvertising()->start();
  printf("Waiting a client connection to notify...\n");
}
