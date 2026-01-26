/**
 *  NimBLE Multi Advertiser Demo:
 *
 *  Demonstrates the Bluetooth 5.x extended advertising capabilities.
 *
 *  This demo will advertise 2 advertisements, and extended scannable instance
 *  and a connectable legacy instance. They will advertise for 5 seconds then
 *  sleep for 20 seconds. The extended scannable instance will use the scan
 *  request callback to update it's data when a scan response is requested.
 *
 *  Created: on April 9 2022
 *      Author: H2zero
 */

#include <NimBLEDevice.h>
#include <esp_sleep.h>

#define SERVICE_UUID        "ABCD"
#define CHARACTERISTIC_UUID "1234"

/** Time in milliseconds to advertise */
static uint32_t advTime = 5000;

/** Time to sleep between advertisements */
static uint32_t sleepTime = 20;

/** Primary PHY used for advertising, can be one of BLE_HCI_LE_PHY_1M or BLE_HCI_LE_PHY_CODED */
static uint8_t primaryPhy = BLE_HCI_LE_PHY_CODED;

/**
 *  Secondary PHY used for advertising and connecting,
 *  can be one of BLE_HCI_LE_PHY_1M, BLE_HCI_LE_PHY_2M or BLE_HCI_LE_PHY_CODED
 */
static uint8_t secondaryPhy = BLE_HCI_LE_PHY_1M;

/** Handler class for server events */
class ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        printf("Client connected: %s\n", connInfo.getAddress().toString().c_str());
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        printf("Client disconnected\n");
        // if still advertising we won't sleep yet.
        if (!pServer->getAdvertising()->isAdvertising()) {
            printf("Sleeping for %" PRIu32 " seconds\n", sleepTime);
            esp_deep_sleep_start();
        }
    }
} serverCallbacks;

/** Callback class to handle advertising events */
class AdvCallbacks : public NimBLEExtAdvertisingCallbacks {
    void onStopped(NimBLEExtAdvertising* pAdv, int reason, uint8_t instId) override {
        /* Check the reason advertising stopped, don't sleep if client is connecting */
        printf("Advertising instance %u stopped\n", instId);
        switch (reason) {
            case 0:
                printf(" client connecting\n");
                return;
            case BLE_HS_ETIMEOUT:
                printf("Time expired - sleeping for %" PRIu32 " seconds\n", sleepTime);
                break;
            default:
                break;
        }

        esp_deep_sleep_start();
    }

    bool m_updatedSR = false;

    void onScanRequest(NimBLEExtAdvertising* pAdv, uint8_t instId, NimBLEAddress addr) override {
        printf("Scan request for instance %u\n", instId);
        // if the data has already been updated we don't need to change it again.
        if (!m_updatedSR) {
            printf("Updating scan data\n");
            NimBLEExtAdvertisement sr;
            sr.setServiceData(NimBLEUUID(SERVICE_UUID), std::string("Hello from scan response!"));
            pAdv->setScanResponseData(instId, sr);
            m_updatedSR = true;
        }
    }
} advCallbacks;

extern "C" void app_main(void) {
    /** Initialize NimBLE and set the device name */
    NimBLEDevice::init("Multi advertiser");

    /** Create a server for our legacy advertiser */
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);

    NimBLEService*        pService = pServer->createService(SERVICE_UUID);
    NimBLECharacteristic* pCharacteristic =
        pService->createCharacteristic(CHARACTERISTIC_UUID,
                                       NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    pCharacteristic->setValue("Hello World");

    /** Start the service */
    pService->start();

    /** Create our multi advertising instances */

    /** extended scannable instance advertising on coded and 1m PHY's. */
    NimBLEExtAdvertisement extScannable(primaryPhy, secondaryPhy);

    /** Legacy advertising as a connectable device. */
    NimBLEExtAdvertisement legacyConnectable;

    /** Optional scan response data. */
    NimBLEExtAdvertisement legacyScanResponse;

    /** As per Bluetooth specification, extended advertising cannot be both scannable and connectable */
    extScannable.setScannable(true);
    extScannable.setConnectable(false);

    /** Set the initial data */
    extScannable.setServiceData(NimBLEUUID(SERVICE_UUID), std::string("Scan me!"));

    /** Enable the scan response callback, we will use this to update the data. */
    extScannable.enableScanRequestCallback(true);

    /** Optional custom address for this advertisment. */
    legacyConnectable.setAddress(NimBLEAddress("DE:AD:BE:EF:BA:AD"));

    /** Set the advertising data. */
    legacyConnectable.setName("Legacy");
    legacyConnectable.setCompleteServices16({NimBLEUUID(SERVICE_UUID)});

    /** Set the legacy and connectable flags. */
    legacyConnectable.setLegacyAdvertising(true);
    legacyConnectable.setConnectable(true);

    /** Put some data in the scan response if desired. */
    legacyScanResponse.setServiceData(NimBLEUUID(SERVICE_UUID), "Legacy SR");

    /** Get the advertising ready */
    NimBLEExtAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

    /** Set the callbacks to handle advertising events */
    pAdvertising->setCallbacks(&advCallbacks);

    /**
     *  Set instance data.
     *  Up to 5 instances can be used if configured in menuconfig, instance 0 is always available.
     *
     *  We will set the extended scannable data on instance 0 and the legacy data on instance 1.
     *  Note that the legacy scan response data needs to be set to the same instance (1).
     */
    if (pAdvertising->setInstanceData(0, extScannable) && pAdvertising->setInstanceData(1, legacyConnectable) &&
        pAdvertising->setScanResponseData(1, legacyScanResponse)) {
        /**
         *  NimBLEExtAdvertising::start takes the advertisement instance ID to start
         *  and a duration in milliseconds or a max number of advertisements to send (or both).
         */
        if (pAdvertising->start(0, advTime) && pAdvertising->start(1, advTime)) {
            printf("Started advertising\n");
        } else {
            printf("Failed to start advertising\n");
        }
    } else {
        printf("Failed to register advertisement data\n");
    }

    esp_sleep_enable_timer_wakeup(sleepTime * 1000000);
}
