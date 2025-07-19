
/** NimBLE Extended Client Demo:
 *
 *  Demonstrates the Bluetooth 5.x client capabilities.
 *
 *  Created: on April 2 2022
 *      Author: H2zero
 *
*/
#include <NimBLEDevice.h>

extern "C" void app_main(void);

#define SERVICE_UUID        "ABCD"
#define CHARACTERISTIC_UUID "1234"

static NimBLEAdvertisedDevice* advDevice;
static bool doConnect = false;
static uint32_t scanTime = 10 * 1000; // In milliseconds, 0 = scan forever

/* Define the PHY's to use when connecting to peer devices, can be 1, 2, or all 3 (default).*/
static uint8_t connectPhys = BLE_GAP_LE_PHY_CODED_MASK | BLE_GAP_LE_PHY_1M_MASK /*| BLE_GAP_LE_PHY_2M_MASK */ ;

/* Define a class to handle the callbacks for client connection events */
class ClientCallbacks : public NimBLEClientCallbacks {
    void onConnect(NimBLEClient* pClient) {
        printf("Connected\n");
    };

    void onDisconnect(NimBLEClient* pClient, int reason) {
        printf("%s Disconnected, reason = %d - Starting scan\n",
                      pClient->getPeerAddress().toString().c_str(), reason);
        NimBLEDevice::getScan()->start(scanTime);
    };
};


/* Define a class to handle the callbacks when advertisements are received */
class scanCallbacks: public NimBLEScanCallbacks {
    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
        printf("Advertised Device found: %s\n", advertisedDevice->toString().c_str());
        if(advertisedDevice->isAdvertisingService(NimBLEUUID("ABCD")))
        {
            printf("Found Our Service\n");
            /* Ready to connect now */
            doConnect = true;
            /* Save the device reference in a global for the client to use*/
            advDevice = advertisedDevice;
            /* stop scan before connecting */
            NimBLEDevice::getScan()->stop();
        }
    }

    /** Callback to process the results of the completed scan or restart it */
    void onScanEnd(NimBLEScanResults results) {
        printf("Scan Ended\n");
    }
};


/* Handles the provisioning of clients and connects / interfaces with the server */
bool connectToServer() {
    NimBLEClient* pClient = nullptr;

    pClient = NimBLEDevice::createClient();
    pClient->setClientCallbacks(new ClientCallbacks, false);

    /* Set the PHY's to use for this connection. This is a bitmask that represents the PHY's:
     * * 0x01 BLE_GAP_LE_PHY_1M_MASK
     * * 0x02 BLE_GAP_LE_PHY_2M_MASK
     * * 0x04 BLE_GAP_LE_PHY_CODED_MASK
     * Combine these with OR ("|"), eg BLE_GAP_LE_PHY_1M_MASK | BLE_GAP_LE_PHY_2M_MASK | BLE_GAP_LE_PHY_CODED_MASK;
     */
    pClient->setConnectPhy(connectPhys);

    /** Set how long we are willing to wait for the connection to complete (milliseconds), default is 30000. */
    pClient->setConnectTimeout(10 * 1000);

    if (!pClient->connect(advDevice)) {
        /* Created a client but failed to connect, don't need to keep it as it has no data */
        NimBLEDevice::deleteClient(pClient);
        printf("Failed to connect, deleted client\n");
        return false;
    }

    printf("Connected to: %s RSSI: %d\n",
           pClient->getPeerAddress().toString().c_str(),
           pClient->getRssi());

    /* Now we can read/write/subscribe the charateristics of the services we are interested in */
    NimBLERemoteService* pSvc = nullptr;
    NimBLERemoteCharacteristic* pChr = nullptr;

    pSvc = pClient->getService(SERVICE_UUID);

    if (pSvc) {
        pChr = pSvc->getCharacteristic(CHARACTERISTIC_UUID);

        if (pChr) {
            // Read the value of the characteristic.
            if (pChr->canRead()) {
                std::string value = pChr->readValue();
                printf("Characteristic value: %s\n", value.c_str());
            }
        }

    } else {
        printf("ABCD service not found.\n");
    }

    NimBLEDevice::deleteClient(pClient);
    printf("Done with this device!\n");
    return true;
}

void connectTask (void * parameter){
    /* Loop here until we find a device we want to connect to */
    for (;;) {
        if (doConnect) {
            /* Found a device we want to connect to, do it now */
            if (connectToServer()) {
                printf("Success!, scanning for more!\n");
            } else {
                printf("Failed to connect, starting scan\n");
            }

            doConnect = false;
            NimBLEDevice::getScan()->start(scanTime);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL);
}

void app_main (void) {
    printf("Starting NimBLE Client\n");
    /* Create a task to handle connecting to peers */
    xTaskCreate(connectTask, "connectTask", 5000, NULL, 1, NULL);

    /* Initialize NimBLE, no device name specified as we are not advertising */
    NimBLEDevice::init("");
    NimBLEScan* pScan = NimBLEDevice::getScan();

    /* create a callback that gets called when advertisers are found */
    pScan->setScanCallbacks(new scanCallbacks());

    /* Set scan interval (how often) and window (how long) in milliseconds */
    pScan->setInterval(97);
    pScan->setWindow(67);

    /*  Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
    pScan->setActiveScan(true);

    /*  Start scanning for advertisers for the scan time specified (in milliseconds) 0 = forever
     *  Optional callback for when scanning stops.
     */
    pScan->start(scanTime);

    printf("Scanning for peripherals\n");
}
