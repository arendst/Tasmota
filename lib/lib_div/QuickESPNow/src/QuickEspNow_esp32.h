#ifndef _QUICK_ESPNOW_ESP32_h
#define _QUICK_ESPNOW_ESP32_h
#ifdef ESP32

#include "Arduino.h"
#include "Comms_hal.h"

#include <esp_now.h>
#include <esp_wifi.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/task.h>

// Disable debug dependency if debug level is 0
#if CORE_DEBUG_LEVEL > 0
#include <QuickDebug.h>
constexpr auto QESPNOW_TAG = "QESPNOW";
#else // CORE_DEBUG_LEVEL
#define DEBUG_ERROR(...)
#define DEBUG_INFO(...)
#define DEBUG_VERBOSE(...)
#define DEBUG_WARN(...)
#define DEBUG_DBG(...)
#endif

//#define MEAS_TPUT

static uint8_t ESPNOW_BROADCAST_ADDRESS[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static const uint8_t MIN_WIFI_CHANNEL = 0;
static const uint8_t MAX_WIFI_CHANNEL = 14;
static const uint8_t CURRENT_WIFI_CHANNEL = 255;
static const size_t ESPNOW_MAX_MESSAGE_LENGTH = 250; ///< @brief Maximum message length
static const uint8_t ESPNOW_ADDR_LEN = 6; ///< @brief Address length
static const uint8_t ESPNOW_QUEUE_SIZE = 3; ///< @brief Queue size

#ifdef MEAS_TPUT
static const time_t MEAS_TP_EVERY_MS = 10000; ///< @brief Measurement time period
#endif // MEAS_TPUT

typedef struct {
    uint16_t frame_head;
    uint16_t duration;
    uint8_t destination_address[6];
    uint8_t source_address[6];
    uint8_t broadcast_address[6];
    uint16_t sequence_control;

    uint8_t category_code;
    uint8_t organization_identifier[3]; // 0x18fe34
    uint8_t random_values[4];
    struct {
        uint8_t element_id;                 // 0xdd
        uint8_t lenght;                     //
        uint8_t organization_identifier[3]; // 0x18fe34
        uint8_t type;                       // 4
        uint8_t version;
        uint8_t body[0];
    } vendor_specific_content;
} __attribute__ ((packed)) espnow_frame_format_t;

typedef struct {
    uint8_t dstAddress[ESPNOW_ADDR_LEN]; /**< Message topic*/
    uint8_t payload[ESPNOW_MAX_MESSAGE_LENGTH]; /**< Message payload*/
    size_t payload_len; /**< Payload length*/
} comms_tx_queue_item_t;

typedef struct {
    uint8_t srcAddress[ESPNOW_ADDR_LEN]; /**< Source Address */
    uint8_t dstAddress[ESPNOW_ADDR_LEN]; /**< Destination Address */
    uint8_t payload[ESPNOW_MAX_MESSAGE_LENGTH]; /**< Message payload */
    size_t payload_len; /**< Payload length */
    int8_t rssi; /**< RSSI */
} comms_rx_queue_item_t;

typedef struct {
    uint8_t mac[ESP_NOW_ETH_ALEN];
    time_t last_msg;
    bool active;
} peer_t;
typedef struct {
    uint8_t peer_number;
    peer_t peer[ESP_NOW_MAX_TOTAL_PEER_NUM];
} peer_list_t;

class PeerListClass {
protected:
    peer_list_t peer_list;

public:
    bool peer_exists (const uint8_t* mac);
    peer_t* get_peer (const uint8_t* mac);
    bool update_peer_use (const uint8_t* mac);
    bool delete_peer (const uint8_t* mac);
    uint8_t* delete_peer ();
    bool add_peer (const uint8_t* mac);
    uint8_t get_peer_number ();
#ifdef UNIT_TEST
    void dump_peer_list ();
#endif
};

class QuickEspNow : public Comms_halClass {
public:
    bool begin (uint8_t channel = CURRENT_WIFI_CHANNEL, uint32_t interface = 0, bool synchronousSend = true) override;
    void stop () override;
    comms_send_error_t send (const uint8_t* dstAddress, const uint8_t* payload, size_t payload_len) override;
    comms_send_error_t sendBcast (const uint8_t* payload, size_t payload_len) {
        return send (ESPNOW_BROADCAST_ADDRESS, payload, payload_len);
    }
    void onDataRcvd (comms_hal_rcvd_data dataRcvd) override;
    void onDataSent (comms_hal_sent_data sentResult) override;
    uint8_t getAddressLength ()  override { return ESPNOW_ADDR_LEN; }
    uint8_t getMaxMessageLength ()  override { return ESPNOW_MAX_MESSAGE_LENGTH; }
    void enableTransmit (bool enable) override;
    bool setChannel (uint8_t channel, wifi_second_chan_t ch2 = WIFI_SECOND_CHAN_NONE);
    bool setWiFiBandwidth (wifi_interface_t iface = WIFI_IF_AP, wifi_bandwidth_t bw = WIFI_BW_HT20);
    bool readyToSendData ();

protected:
    wifi_interface_t wifi_if;
    PeerListClass peer_list;
    TaskHandle_t espnowTxTask;
    TaskHandle_t espnowRxTask;

#ifdef MEAS_TPUT
    unsigned long txDataSent = 0;
    unsigned long rxDataReceived = 0;
    unsigned long txDataDropped = 0;
    time_t lastDataTPMeas = 0;
    TimerHandle_t dataTPTimer;
    float txDataTP = 0;
    float rxDataTP = 0;
    float txDroppedDataRatio = 0;

    static void tp_timer_cb (void* param);
    void calculateDataTP ();
#endif // MEAS_TPUT

    bool readyToSend = true;
    bool waitingForConfirmation = false;
    bool synchronousSend = false;
    uint8_t sentStatus;
    int queueSize = ESPNOW_QUEUE_SIZE;

    QueueHandle_t tx_queue;
    QueueHandle_t rx_queue;
    //SemaphoreHandle_t espnow_send_mutex;
    //uint8_t channel;
    bool followWiFiChannel = false;

    bool initComms ();
    bool addPeer (const uint8_t* peer_addr);
    static void espnowTxTask_cb (void* param);
    int32_t sendEspNowMessage (comms_tx_queue_item_t* message);
    void espnowTxHandle ();

    static void espnowRxTask_cb (void* param);
    void espnowRxHandle ();

    static void ICACHE_FLASH_ATTR rx_cb(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int len);
    static void ICACHE_FLASH_ATTR tx_cb (uint8_t* mac_addr, uint8_t status);
};

extern QuickEspNow quickEspNow;

#endif // ESP32
#endif // _QUICK_ESPNOW_ESP32_h
