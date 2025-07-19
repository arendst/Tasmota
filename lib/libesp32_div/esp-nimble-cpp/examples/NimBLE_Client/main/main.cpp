
/** NimBLE_Client Demo:
 *
 *  Demonstrates many of the available features of the NimBLE client library.
 *
 *  Created: on March 24 2020
 *      Author: H2zero
 */

#include <NimBLEDevice.h>

static const NimBLEAdvertisedDevice* advDevice;
static bool                          doConnect  = false;
static uint32_t                      scanTimeMs = 5000; /** scan time in milliseconds, 0 = scan forever */

/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
class ClientCallbacks : public NimBLEClientCallbacks {
    void onConnect(NimBLEClient* pClient) override { printf("Connected\n"); }

    void onDisconnect(NimBLEClient* pClient, int reason) override {
        printf("%s Disconnected, reason = %d - Starting scan\n", pClient->getPeerAddress().toString().c_str(), reason);
        NimBLEDevice::getScan()->start(scanTimeMs, false, true);
    }

    /********************* Security handled here *********************/
    void onPassKeyEntry(NimBLEConnInfo& connInfo) override {
        printf("Server Passkey Entry\n");
        /**
         * This should prompt the user to enter the passkey displayed
         * on the peer device.
         */
        NimBLEDevice::injectPassKey(connInfo, 123456);
    }

    void onConfirmPasskey(NimBLEConnInfo& connInfo, uint32_t pass_key) override {
        printf("The passkey YES/NO number: %" PRIu32 "\n", pass_key);
        /** Inject false if passkeys don't match. */
        NimBLEDevice::injectConfirmPasskey(connInfo, true);
    }

    /** Pairing process complete, we can check the results in connInfo */
    void onAuthenticationComplete(NimBLEConnInfo& connInfo) override {
        if (!connInfo.isEncrypted()) {
            printf("Encrypt connection failed - disconnecting\n");
            /** Find the client with the connection handle provided in connInfo */
            NimBLEDevice::getClientByHandle(connInfo.getConnHandle())->disconnect();
            return;
        }
    }
} clientCallbacks;

/** Define a class to handle the callbacks when scan events are received */
class ScanCallbacks : public NimBLEScanCallbacks {
    void onResult(const NimBLEAdvertisedDevice* advertisedDevice) override {
        printf("Advertised Device found: %s\n", advertisedDevice->toString().c_str());
        if (advertisedDevice->isAdvertisingService(NimBLEUUID("DEAD"))) {
            printf("Found Our Service\n");
            /** stop scan before connecting */
            NimBLEDevice::getScan()->stop();
            /** Save the device reference in a global for the client to use*/
            advDevice = advertisedDevice;
            /** Ready to connect now */
            doConnect = true;
        }
    }

    /** Callback to process the results of the completed scan or restart it */
    void onScanEnd(const NimBLEScanResults& results, int reason) override {
        printf("Scan Ended, reason: %d, device count: %d; Restarting scan\n", reason, results.getCount());
        NimBLEDevice::getScan()->start(scanTimeMs, false, true);
    }
} scanCallbacks;

/** Notification / Indication receiving handler callback */
void notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
    std::string str  = (isNotify == true) ? "Notification" : "Indication";
    str             += " from ";
    str             += pRemoteCharacteristic->getClient()->getPeerAddress().toString();
    str             += ": Service = " + pRemoteCharacteristic->getRemoteService()->getUUID().toString();
    str             += ", Characteristic = " + pRemoteCharacteristic->getUUID().toString();
    str             += ", Value = " + std::string((char*)pData, length);
    printf("%s\n", str.c_str());
}

/** Handles the provisioning of clients and connects / interfaces with the server */
bool connectToServer() {
    NimBLEClient* pClient = nullptr;

    /** Check if we have a client we should reuse first **/
    if (NimBLEDevice::getCreatedClientCount()) {
        /**
         *  Special case when we already know this device, we send false as the
         *  second argument in connect() to prevent refreshing the service database.
         *  This saves considerable time and power.
         */
        pClient = NimBLEDevice::getClientByPeerAddress(advDevice->getAddress());
        if (pClient) {
            if (!pClient->connect(advDevice, false)) {
                printf("Reconnect failed\n");
                return false;
            }
            printf("Reconnected client\n");
        } else {
            /**
             *  We don't already have a client that knows this device,
             *  check for a client that is disconnected that we can use.
             */
            pClient = NimBLEDevice::getDisconnectedClient();
        }
    }

    /** No client to reuse? Create a new one. */
    if (!pClient) {
        if (NimBLEDevice::getCreatedClientCount() >= NIMBLE_MAX_CONNECTIONS) {
            printf("Max clients reached - no more connections available\n");
            return false;
        }

        pClient = NimBLEDevice::createClient();

        printf("New client created\n");

        pClient->setClientCallbacks(&clientCallbacks, false);
        /**
         *  Set initial connection parameters:
         *  These settings are safe for 3 clients to connect reliably, can go faster if you have less
         *  connections. Timeout should be a multiple of the interval, minimum is 100ms.
         *  Min interval: 12 * 1.25ms = 15, Max interval: 12 * 1.25ms = 15, 0 latency, 150 * 10ms = 1500ms timeout
         */
        pClient->setConnectionParams(12, 12, 0, 150);

        /** Set how long we are willing to wait for the connection to complete (milliseconds), default is 30000. */
        pClient->setConnectTimeout(5 * 1000);

        if (!pClient->connect(advDevice)) {
            /** Created a client but failed to connect, don't need to keep it as it has no data */
            NimBLEDevice::deleteClient(pClient);
            printf("Failed to connect, deleted client\n");
            return false;
        }
    }

    if (!pClient->isConnected()) {
        if (!pClient->connect(advDevice)) {
            printf("Failed to connect\n");
            return false;
        }
    }

    printf("Connected to: %s RSSI: %d\n", pClient->getPeerAddress().toString().c_str(), pClient->getRssi());

    /** Now we can read/write/subscribe the characteristics of the services we are interested in */
    NimBLERemoteService*        pSvc = nullptr;
    NimBLERemoteCharacteristic* pChr = nullptr;
    NimBLERemoteDescriptor*     pDsc = nullptr;

    pSvc = pClient->getService("DEAD");
    if (pSvc) {
        pChr = pSvc->getCharacteristic("BEEF");
    }

    if (pChr) {
        if (pChr->canRead()) {
            printf("%s Value: %s\n", pChr->getUUID().toString().c_str(), pChr->readValue().c_str());
        }

        if (pChr->canWrite()) {
            if (pChr->writeValue("Tasty")) {
                printf("Wrote new value to: %s\n", pChr->getUUID().toString().c_str());
            } else {
                pClient->disconnect();
                return false;
            }

            if (pChr->canRead()) {
                printf("The value of: %s is now: %s\n", pChr->getUUID().toString().c_str(), pChr->readValue().c_str());
            }
        }

        if (pChr->canNotify()) {
            if (!pChr->subscribe(true, notifyCB)) {
                pClient->disconnect();
                return false;
            }
        } else if (pChr->canIndicate()) {
            /** Send false as first argument to subscribe to indications instead of notifications */
            if (!pChr->subscribe(false, notifyCB)) {
                pClient->disconnect();
                return false;
            }
        }
    } else {
        printf("DEAD service not found.\n");
    }

    pSvc = pClient->getService("BAAD");
    if (pSvc) {
        pChr = pSvc->getCharacteristic("F00D");
        if (pChr) {
            if (pChr->canRead()) {
                printf("%s Value: %s\n", pChr->getUUID().toString().c_str(), pChr->readValue().c_str());
            }

            pDsc = pChr->getDescriptor(NimBLEUUID("C01D"));
            if (pDsc) {
                printf("Descriptor: %s  Value: %s\n", pDsc->getUUID().toString().c_str(), pDsc->readValue().c_str());
            }

            if (pChr->canWrite()) {
                if (pChr->writeValue("No tip!")) {
                    printf("Wrote new value to: %s\n", pChr->getUUID().toString().c_str());
                } else {
                    pClient->disconnect();
                    return false;
                }

                if (pChr->canRead()) {
                    printf("The value of: %s is now: %s\n", pChr->getUUID().toString().c_str(), pChr->readValue().c_str());
                }
            }

            if (pChr->canNotify()) {
                if (!pChr->subscribe(true, notifyCB)) {
                    pClient->disconnect();
                    return false;
                }
            } else if (pChr->canIndicate()) {
                /** Send false as first argument to subscribe to indications instead of notifications */
                if (!pChr->subscribe(false, notifyCB)) {
                    pClient->disconnect();
                    return false;
                }
            }
        }
    } else {
        printf("BAAD service not found.\n");
    }

    printf("Done with this device!\n");
    return true;
}

extern "C" void app_main(void) {
    printf("Starting NimBLE Client\n");
    /** Initialize NimBLE and set the device name */
    NimBLEDevice::init("NimBLE-Client");

    /**
     * Set the IO capabilities of the device, each option will trigger a different pairing method.
     *  BLE_HS_IO_KEYBOARD_ONLY   - Passkey pairing
     *  BLE_HS_IO_DISPLAY_YESNO   - Numeric comparison pairing
     *  BLE_HS_IO_NO_INPUT_OUTPUT - DEFAULT setting - just works pairing
     */
    // NimBLEDevice::setSecurityIOCap(BLE_HS_IO_KEYBOARD_ONLY); // use passkey
    // NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_YESNO); //use numeric comparison

    /**
     * 2 different ways to set security - both calls achieve the same result.
     *  no bonding, no man in the middle protection, BLE secure connections.
     *  These are the default values, only shown here for demonstration.
     */
    // NimBLEDevice::setSecurityAuth(false, false, true);

    NimBLEDevice::setSecurityAuth(/*BLE_SM_PAIR_AUTHREQ_BOND | BLE_SM_PAIR_AUTHREQ_MITM |*/ BLE_SM_PAIR_AUTHREQ_SC);

    /** Optional: set the transmit power */
    NimBLEDevice::setPower(3); /** 3dbm */
    NimBLEScan* pScan = NimBLEDevice::getScan();

    /** Set the callbacks to call when scan events occur, no duplicates */
    pScan->setScanCallbacks(&scanCallbacks, false);

    /** Set scan interval (how often) and window (how long) in milliseconds */
    pScan->setInterval(100);
    pScan->setWindow(100);

    /**
     * Active scan will gather scan response data from advertisers
     *  but will use more energy from both devices
     */
    pScan->setActiveScan(true);

    /** Start scanning for advertisers */
    pScan->start(scanTimeMs);
    printf("Scanning for peripherals\n");

    /** Loop here until we find a device we want to connect to */
    for (;;) {
        vTaskDelay(10 / portTICK_PERIOD_MS);

        if (doConnect) {
            doConnect = false;
            /** Found a device we want to connect to, do it now */
            if (connectToServer()) {
                printf("Success! we should now be getting notifications, scanning for more!\n");
            } else {
                printf("Failed to connect, starting scan\n");
            }

            NimBLEDevice::getScan()->start(scanTimeMs, false, true);
        }
    }
}
