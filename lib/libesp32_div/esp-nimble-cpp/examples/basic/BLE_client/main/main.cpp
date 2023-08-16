/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 * updated for NimBLE by H2zero
 */

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include "BLEDevice.h"
***********************/
#include "NimBLEDevice.h"

extern "C"{void app_main(void);}

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static bool doConnect = false;
static bool connected = false;
static bool doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    printf("Notify callback for characteristic %s of data length %d data: %s\n",
           pBLERemoteCharacteristic->getUUID().toString().c_str(),
           length,
           (char*)pData);
}

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  /** onDisconnect now takes a reason parameter to indicate the reason for disconnection
  void onDisconnect(BLEClient* pclient) { */
  void onDisconnect(BLEClient* pclient, int reason) {
    connected = false;
    printf("onDisconnect");
  }
/***************** New - Security handled here ********************
****** Note: these are the same return values as defaults ********/
  uint32_t onPassKeyRequest(){
    printf("Client PassKeyRequest\n");
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

bool connectToServer() {
    printf("Forming a connection to %s\n", myDevice->getAddress().toString().c_str());

    BLEClient*  pClient  = BLEDevice::createClient();
    printf(" - Created client\n");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    printf(" - Connected to server\n");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      printf("Failed to find our service UUID: %s\n", serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    printf(" - Found our service\n");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      printf("Failed to find our characteristic UUID: %s\n", charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    printf(" - Found our characteristic\n");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      printf("The characteristic value was: %s\n", value.c_str());
    }

    /** registerForNotify() has been removed and replaced with subscribe() / unsubscribe().
     *  Subscribe parameter defaults are: notifications=true, notifyCallback=nullptr, response=true.
     *  Unsubscribe parameter defaults are: response=true.
     */
    if(pRemoteCharacteristic->canNotify()) {
        //pRemoteCharacteristic->registerForNotify(notifyCallback);
        pRemoteCharacteristic->subscribe(true, notifyCallback);
    }

    connected = true;
    return true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */

/*** Only a reference to the advertised device is passed now
  void onResult(BLEAdvertisedDevice advertisedDevice) { **/
  void onResult(BLEAdvertisedDevice* advertisedDevice) {
    printf("BLE Advertised Device found: %s\n", advertisedDevice->toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
/********************************************************************************
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
********************************************************************************/
    if (advertisedDevice->haveServiceUUID() && advertisedDevice->isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
/*******************************************************************
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
*******************************************************************/
      myDevice = advertisedDevice; /** Just save the reference now, no need to copy the object */
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


// This is the Arduino main loop function.
void connectTask (void * parameter){
    for(;;) {
      // If the flag "doConnect" is true then we have scanned for and found the desired
      // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
      // connected we set the connected flag to be true.
      if (doConnect == true) {
        if (connectToServer()) {
          printf("We are now connected to the BLE Server.\n");
        } else {
          printf("We have failed to connect to the server; there is nothin more we will do.\n");
        }
        doConnect = false;
      }

      // If we are connected to a peer BLE Server, update the characteristic each time we are reached
      // with the current time since boot.
      if (connected) {
        char buf[256];
        snprintf(buf, 256, "Time since boot: %lu", (unsigned long)(esp_timer_get_time() / 1000000ULL));
        printf("Setting new characteristic value to %s\n", buf);

        // Set the characteristic's value to be the array of bytes that is actually a string.
        /*** Note: write value now returns true if successful, false otherwise - try again or disconnect ***/
        pRemoteCharacteristic->writeValue((uint8_t*)buf, strlen(buf), false);
      }else if(doScan){
        BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it
      }

      vTaskDelay(1000/portTICK_PERIOD_MS); // Delay a second between loops.
    }

    vTaskDelete(NULL);
} // End of loop


void app_main(void) {
  printf("Starting BLE Client application...\n");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setScanCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);

  xTaskCreate(connectTask, "connectTask", 5000, NULL, 1, NULL);
  pBLEScan->start(5 * 1000, false);
} // End of setup.

