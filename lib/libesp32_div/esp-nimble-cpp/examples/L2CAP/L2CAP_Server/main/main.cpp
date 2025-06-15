#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "dcbc7255-1e9e-49a0-a360-b0430b6c6905"
#define CHARACTERISTIC_UUID "371a55c8-f251-4ad2-90b3-c7c195b049be"
#define L2CAP_CHANNEL        150
#define L2CAP_MTU            5000

class GATTCallbacks: public BLEServerCallbacks {

public:
    void onConnect(BLEServer* pServer, BLEConnInfo& info) {
        /// Booster #1
        pServer->setDataLen(info.getConnHandle(), 251);
        /// Booster #2 (especially for Apple devices)
        BLEDevice::getServer()->updateConnParams(info.getConnHandle(), 12, 12, 0, 200);
    }
};

class L2CAPChannelCallbacks: public BLEL2CAPChannelCallbacks {

public:
    bool connected = false;
    size_t numberOfReceivedBytes;
    uint8_t nextSequenceNumber;

public:
    void onConnect(NimBLEL2CAPChannel* channel) {
        printf("L2CAP connection established\n");
        connected = true;
        numberOfReceivedBytes = nextSequenceNumber = 0;
    }

    void onRead(NimBLEL2CAPChannel* channel, std::vector<uint8_t>& data) {
        numberOfReceivedBytes += data.size();
        size_t sequenceNumber = data[0];
        printf("L2CAP read %d bytes w/ sequence number %d", data.size(), sequenceNumber);
        if (sequenceNumber != nextSequenceNumber) {
            printf("(wrong sequence number %d, expected %d)\n", sequenceNumber, nextSequenceNumber);
        } else {
            printf("\n");
            nextSequenceNumber++;
        }
    }
    void onDisconnect(NimBLEL2CAPChannel* channel) {
        printf("L2CAP disconnected\n");
        connected = false;
    }
};

extern "C"
void app_main(void) {
    printf("Starting L2CAP server example [%lu free] [%lu min]\n", esp_get_free_heap_size(), esp_get_minimum_free_heap_size());

    BLEDevice::init("L2CAP-Server");
    BLEDevice::setMTU(BLE_ATT_MTU_MAX);

    auto cocServer = BLEDevice::createL2CAPServer();
    auto l2capChannelCallbacks = new L2CAPChannelCallbacks();
    auto channel = cocServer->createService(L2CAP_CHANNEL, L2CAP_MTU, l2capChannelCallbacks);
    
    auto server = BLEDevice::createServer();
    server->setCallbacks(new GATTCallbacks());
    auto service = server->createService(SERVICE_UUID);
    auto characteristic = service->createCharacteristic(CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ);
    characteristic->setValue(L2CAP_CHANNEL);
    service->start();
    auto advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->enableScanResponse(true);

    BLEDevice::startAdvertising();
    printf("Server waiting for connection requests [%lu free] [%lu min]\n", esp_get_free_heap_size(), esp_get_minimum_free_heap_size());

    // Wait until transfer actually starts...
    while (!l2capChannelCallbacks->numberOfReceivedBytes) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    printf("\n\n\n");
    int numberOfSeconds = 0;

    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (!l2capChannelCallbacks->connected) { continue; }
        int bps = l2capChannelCallbacks->numberOfReceivedBytes / ++numberOfSeconds;
        printf("Bandwidth: %d b/sec = %d KB/sec [%lu free] [%lu min]\n", bps, bps / 1024, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    }
}
