/** NimBLE Extended Advertiser Demo:
 *
 *  Demonstrates the Bluetooth 5.x extended advertising capabilities.
 *
 *  This demo will advertise a long data string on the CODED and 1M Phy's and
 *  starts a server allowing connection over either PHY's. It will advertise for
 *  5 seconds then sleep for 20 seconds, if a client connects it will sleep once
 *  it has disconnected then repeats.
 *
 *  Created: on April 2 2022
 *      Author: H2zero
 *
*/

/****************************************************
 * For use with ESP32C3, ESP32S3, ESP32H2 ONLY!     *
 /**************************************************/

#include "NimBLEDevice.h"
#if !CONFIG_BT_NIMBLE_EXT_ADV
#  error Must enable extended advertising, see nimconfig.h file.
#endif

#include "esp_sleep.h"

#define SERVICE_UUID        "ABCD"
#define CHARACTERISTIC_UUID "1234"

/* Time in milliseconds to advertise */
static uint32_t advTime = 5000;

/* Time to sleep between advertisements */
static uint32_t sleepSeconds = 20;

/* Primary PHY used for advertising, can be one of BLE_HCI_LE_PHY_1M or BLE_HCI_LE_PHY_CODED */
static uint8_t primaryPhy = BLE_HCI_LE_PHY_CODED;

/* Secondary PHY used for advertising and connecting,
 * can be one of BLE_HCI_LE_PHY_1M, BLE_HCI_LE_PHY_2M or BLE_HCI_LE_PHY_CODED
 */
static uint8_t secondaryPhy = BLE_HCI_LE_PHY_1M;


/* Handler class for server events */
class ServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, ble_gap_conn_desc* desc) {
        Serial.printf("Client connected:: %s\n", NimBLEAddress(desc->peer_ota_addr).toString().c_str());
    };

    void onDisconnect(NimBLEServer* pServer) {
        Serial.printf("Client disconnected - sleeping for %u seconds\n", sleepSeconds);
        esp_deep_sleep_start();
    };
};

/* Callback class to handle advertising events */
class advertisingCallbacks: public NimBLEExtAdvertisingCallbacks {
    void onStopped(NimBLEExtAdvertising* pAdv, int reason, uint8_t inst_id) {
        /* Check the reason advertising stopped, don't sleep if client is connecting */
        printf("Advertising instance %u stopped\n", inst_id);
        switch (reason) {
            case 0:
                printf("Client connecting\n");
                return;
            case BLE_HS_ETIMEOUT:
                printf("Time expired - sleeping for %u seconds\n", sleepSeconds);
                break;
            default:
                break;
        }

        esp_deep_sleep_start();
    }
};

void setup () {
    Serial.begin(115200);

    NimBLEDevice::init("Extended advertiser");

    /* Create the server and add the services/characteristics/descriptors */
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks);

    NimBLEService *pService = pServer->createService(SERVICE_UUID);
    NimBLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID,
                                                                           NIMBLE_PROPERTY::READ |
                                                                           NIMBLE_PROPERTY::WRITE |
                                                                           NIMBLE_PROPERTY::NOTIFY);

    pCharacteristic->setValue("Hello World");

    /* Start the services */
    pService->start();

    /*
    * Create an extended advertisement with the instance ID 0 and set the PHY's.
    * Multiple instances can be added as long as the instance ID is incremented.
    */
    NimBLEExtAdvertisement extAdv(primaryPhy, secondaryPhy);

    /* Set the advertisement as connectable */
    extAdv.setConnectable(true);

    /* As per Bluetooth specification, extended advertising cannot be both scannable and connectable */
    extAdv.setScannable(false); // The default is false, set here for demonstration.

    /* Extended advertising allows for 251 bytes (minus header bytes ~20) in a single advertisement or up to 1650 if chained */
    extAdv.setServiceData(NimBLEUUID(SERVICE_UUID), std::string("Extended Advertising Demo.\r\n"
                                                                "Extended advertising allows for "
                                                                "251 bytes of data in a single advertisement,\r\n"
                                                                "or up to 1650 bytes with chaining.\r\n"
                                                                "This example message is 226 bytes long "
                                                                "and is using CODED_PHY for long range."));

    extAdv.setCompleteServices16({NimBLEUUID(SERVICE_UUID)});

    /* When extended advertising is enabled `NimBLEDevice::getAdvertising` returns a pointer to `NimBLEExtAdvertising */
    NimBLEExtAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

    /* Set the callbacks for advertising events */
    pAdvertising->setCallbacks(new advertisingCallbacks);

    /*
     * NimBLEExtAdvertising::setInstanceData takes the instance ID and
     * a reference to a `NimBLEExtAdvertisement` object. This sets the data
     * that will be advertised for this instance ID, returns true if successful.
     *
     * Note: It is safe to create the advertisement as a local variable if setInstanceData
     * is called before exiting the code block as the data will be copied.
     */
    if (pAdvertising->setInstanceData(0, extAdv)) {
        /*
         * `NimBLEExtAdvertising::start` takes the advertisement instance ID to start
         * and a duration in milliseconds or a max number of advertisements to send (or both).
         */
        if (pAdvertising->start(0, advTime)) {
            Serial.printf("Started advertising\n");
        } else {
            Serial.printf("Failed to start advertising\n");
        }
    } else {
        Serial.printf("Failed to register advertisment data\n");
    }

    esp_sleep_enable_timer_wakeup(sleepSeconds * 1000000);
}

void loop () {
}