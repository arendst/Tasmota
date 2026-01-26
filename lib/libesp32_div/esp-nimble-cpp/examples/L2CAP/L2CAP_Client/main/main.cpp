#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

// The remote service we wish to connect to.
static BLEUUID serviceUUID("dcbc7255-1e9e-49a0-a360-b0430b6c6905");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("371a55c8-f251-4ad2-90b3-c7c195b049be");

#define L2CAP_CHANNEL        150
#define L2CAP_MTU            5000

const BLEAdvertisedDevice* theDevice = NULL;
BLEClient* theClient = NULL;
BLEL2CAPChannel* theChannel = NULL;

size_t bytesSent = 0;
size_t bytesReceived = 0;

class L2CAPChannelCallbacks: public BLEL2CAPChannelCallbacks {

public:
    void onConnect(NimBLEL2CAPChannel* channel) {
        printf("L2CAP connection established\n");
    }

    void onMTUChange(NimBLEL2CAPChannel* channel, uint16_t mtu) {
        printf("L2CAP MTU changed to %d\n", mtu);
    }

    void onRead(NimBLEL2CAPChannel* channel, std::vector<uint8_t>& data) {
        printf("L2CAP read %d bytes\n", data.size());
    }
    void onDisconnect(NimBLEL2CAPChannel* channel) {
        printf("L2CAP disconnected\n");
    }
};

class MyClientCallbacks: public BLEClientCallbacks {

    void onConnect(BLEClient* pClient) {
        printf("GAP connected\n");
        pClient->setDataLen(251);

        theChannel = BLEL2CAPChannel::connect(pClient, L2CAP_CHANNEL, L2CAP_MTU, new L2CAPChannelCallbacks());
    }

    void onDisconnect(BLEClient* pClient, int reason) {
        printf("GAP disconnected (reason: %d)\n", reason);
        theDevice = NULL;
        theChannel = NULL;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        BLEDevice::getScan()->start(5 * 1000, true);
    }
};

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {

    void onResult(const BLEAdvertisedDevice* advertisedDevice) {
        if (theDevice) { return; }
        printf("BLE Advertised Device found: %s\n", advertisedDevice->toString().c_str());

        if (!advertisedDevice->haveServiceUUID()) { return; }
        if (!advertisedDevice->isAdvertisingService(serviceUUID)) { return; }

        printf("Found the device we're interested in!\n");
        BLEDevice::getScan()->stop();

        // Hand over the device to the other task
        theDevice = advertisedDevice;
    }
};

void connectTask(void *pvParameters) {

    uint8_t sequenceNumber = 0;

    while (true) {
        
        if (!theDevice) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        if (!theClient) {
            theClient = BLEDevice::createClient();
            theClient->setConnectionParams(6, 6, 0, 42);

            auto callbacks = new MyClientCallbacks();
            theClient->setClientCallbacks(callbacks);

            auto success = theClient->connect(theDevice);
            if (!success) {
                printf("Error: Could not connect to device\n");
                break;
            }
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;            
        }

        if (!theChannel) {
            printf("l2cap channel not initialized\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        if (!theChannel->isConnected()) {
            printf("l2cap channel not connected\n");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            continue;
        }

        while (theChannel->isConnected()) {

        /*
            static auto initialDelay = true;
            if (initialDelay) {
                printf("Waiting gracefully 3 seconds before sending data\n");
                vTaskDelay(3000 / portTICK_PERIOD_MS);
                initialDelay = false;
            };
*/
            std::vector<uint8_t> data(5000, sequenceNumber++);
            if (theChannel->write(data)) {
                bytesSent += data.size();
            } else {
                printf("failed to send!\n");
                abort();                
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C"
void app_main(void) {
    printf("Starting L2CAP client example\n");

    xTaskCreate(connectTask, "connectTask", 5000, NULL, 1, NULL);

    BLEDevice::init("L2CAP-Client");
    BLEDevice::setMTU(BLE_ATT_MTU_MAX);

    auto scan = BLEDevice::getScan();
    auto callbacks = new MyAdvertisedDeviceCallbacks();
    scan->setScanCallbacks(callbacks);
    scan->setInterval(1349);
    scan->setWindow(449);
    scan->setActiveScan(true);
    scan->start(25 * 1000, false);

    int numberOfSeconds = 0;

    while (bytesSent == 0) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        int bytesSentPerSeconds = bytesSent / ++numberOfSeconds;
        printf("Bandwidth: %d b/sec = %d KB/sec\n", bytesSentPerSeconds, bytesSentPerSeconds / 1024);
    }
}
