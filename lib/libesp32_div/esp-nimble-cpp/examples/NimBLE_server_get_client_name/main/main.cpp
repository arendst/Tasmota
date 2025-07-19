/** NimBLE_server_get_client_name
 *
 *  Demonstrates 2 ways for the server to read the device name from the connected client.
 *
 *  Created: on June 24 2024
 *      Author: H2zero
 *
 */

#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID            "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define ENC_CHARACTERISTIC_UUID "9551f35b-8d91-42e4-8f7e-1358dfe272dc"

NimBLEServer* pServer;

class ServerCallbacks : public NimBLEServerCallbacks {
    // Same as before but now includes the name parameter
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, std::string& name) override {
        printf("Client address: %s Name: %s\n", connInfo.getAddress().toString().c_str(), name.c_str());
    }

    // Same as before but now includes the name parameter
    void onAuthenticationComplete(const NimBLEConnInfo& connInfo, const std::string& name) override {
        if (!connInfo.isEncrypted()) {
            NimBLEDevice::getServer()->disconnect(connInfo.getConnHandle());
            printf("Encrypt connection failed - disconnecting client\n");
            return;
        }

        printf("Encrypted Client address: %s Name: %s\n", connInfo.getAddress().toString().c_str(), name.c_str());
    }
};

extern "C" void app_main(void) {
    printf("Starting BLE Server!\n");

    NimBLEDevice::init("Connect to me!");
    NimBLEDevice::setSecurityAuth(true, false, true); // Enable bonding to see full name on phones.

    pServer                               = NimBLEDevice::createServer();
    NimBLEService*        pService        = pServer->createService(SERVICE_UUID);
    NimBLECharacteristic* pCharacteristic =
        pService->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    pCharacteristic->setValue("Hello World says NimBLE!");

    NimBLECharacteristic* pEncCharacteristic = pService->createCharacteristic(
        ENC_CHARACTERISTIC_UUID,
        (NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::WRITE_ENC));
    pEncCharacteristic->setValue("Hello World says NimBLE Encrypted");

    pService->start();

    pServer->setCallbacks(new ServerCallbacks());
    pServer->getPeerNameOnConnect(true); // Setting this will enable the onConnect callback that provides the name.

    BLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);

    pAdvertising->start();
    printf("Advertising started, connect with your phone.\n");

    while (true) {
        auto clientCount = pServer->getConnectedCount();
        if (clientCount) {
            printf("Connected clients:\n");
            for (auto i = 0; i < clientCount; ++i) {
                NimBLEConnInfo peerInfo = pServer->getPeerInfo(i);
                printf("Client address: %s Name: %s\n", peerInfo.getAddress().toString().c_str(),
                // This function blocks until the name is retrieved, so cannot be used in callback functions.
                       pServer->getPeerName(peerInfo).c_str());

            }
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
