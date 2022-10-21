
/** NimBLE Extended Client Demo:
 *
 *  Demonstrates the Bluetooth 5.x client capabilities.
 *
 *  Created: on April 2 2022
 *      Author: H2zero
 *
*/

/****************************************************
 * For use with ESP32C3, ESP32S3, ESP32H2 ONLY!     *
 /**************************************************/

#include <NimBLEDevice.h>
#if !CONFIG_BT_NIMBLE_EXT_ADV
#  error Must enable extended advertising, see nimconfig.h file.
#endif

void scanEndedCB(NimBLEScanResults results);

#define SERVICE_UUID        "ABCD"
#define CHARACTERISTIC_UUID "1234"

static NimBLEAdvertisedDevice* advDevice;
static bool doConnect = false;
static uint32_t scanTime = 10; /* 0 = scan forever */

/* Define the PHY's to use when connecting to peer devices, can be 1, 2, or all 3 (default).*/
static uint8_t connectPhys = BLE_GAP_LE_PHY_CODED_MASK | BLE_GAP_LE_PHY_1M_MASK /*| BLE_GAP_LE_PHY_2M_MASK */ ;

/* Define a class to handle the callbacks for client connection events */
class ClientCallbacks : public NimBLEClientCallbacks {
    void onConnect(NimBLEClient* pClient) {
        Serial.printf("Connected\n");
    };

    void onDisconnect(NimBLEClient* pClient) {
        Serial.printf("%s Disconnected - Starting scan\n", pClient->getPeerAddress().toString().c_str());
        NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
    };
};


/* Define a class to handle the callbacks when advertisements are received */
class AdvertisedDeviceCallbacks: public NimBLEAdvertisedDeviceCallbacks {

    void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
        Serial.printf("Advertised Device found: %s\n", advertisedDevice->toString().c_str());
        if(advertisedDevice->isAdvertisingService(NimBLEUUID("ABCD")))
        {
            Serial.printf("Found Our Service\n");
            /* Ready to connect now */
            doConnect = true;
            /* Save the device reference in a global for the client to use*/
            advDevice = advertisedDevice;
            /* stop scan before connecting */
            NimBLEDevice::getScan()->stop();
        }
    };
};


/* Callback to process the results of the last scan or restart it */
void scanEndedCB(NimBLEScanResults results){
    Serial.printf("Scan Ended\n");
    if (!doConnect) { /* Don't start the scan while connecting */
        NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
    }
}


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

    /** Set how long we are willing to wait for the connection to complete (seconds), default is 30. */
    pClient->setConnectTimeout(10);

    if (!pClient->connect(advDevice)) {
        /* Created a client but failed to connect, don't need to keep it as it has no data */
        NimBLEDevice::deleteClient(pClient);
        Serial.printf("Failed to connect, deleted client\n");
        return false;
    }

    Serial.printf("Connected to: %s RSSI: %d\n",
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
                Serial.printf("Characteristic value: %s\n", value.c_str());
            }
        }

    } else {
        Serial.printf("ABCD service not found.\n");
    }

    NimBLEDevice::deleteClient(pClient);
    Serial.printf("Done with this device!\n");
    return true;
}

void setup () {
    Serial.begin(115200);
    Serial.printf("Starting NimBLE Client\n");

    /* Initialize NimBLE, no device name specified as we are not advertising */
    NimBLEDevice::init("");
    NimBLEScan* pScan = NimBLEDevice::getScan();

    /* create a callback that gets called when advertisers are found */
    pScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());

    /* Set scan interval (how often) and window (how long) in milliseconds */
    pScan->setInterval(97);
    pScan->setWindow(67);

    /*  Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
    pScan->setActiveScan(true);

    /*  Start scanning for advertisers for the scan time specified (in seconds) 0 = forever
     *  Optional callback for when scanning stops.
     */
    pScan->start(scanTime, scanEndedCB);

    Serial.printf("Scanning for peripherals\n");
}

void loop () {
    /* Loop here until we find a device we want to connect to */
    if (doConnect) {
        /* Found a device we want to connect to, do it now */
        if (connectToServer()) {
            Serial.printf("Success!, scanning for more!\n");
        } else {
            Serial.printf("Failed to connect, starting scan\n");
        }

        doConnect = false;
        NimBLEDevice::getScan()->start(scanTime, scanEndedCB);
    }

    delay(10);
}
