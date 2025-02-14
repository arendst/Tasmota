#include "QuickEspNow.h"

#ifdef ESP8266

typedef struct {
    signed rssi : 8;
    unsigned rate : 4;
    unsigned is_group : 1;
    unsigned : 1;
    unsigned sig_mode : 2;
    unsigned legacy_length : 12;
    unsigned damatch0 : 1;
    unsigned damatch1 : 1;
    unsigned bssidmatch0 : 1;
    unsigned bssidmatch1 : 1;
    unsigned MCS : 7;
    unsigned CWB : 1;
    unsigned HT_length : 16;
    unsigned Smoothing : 1;
    unsigned Not_Sounding : 1;
    unsigned : 1;
    unsigned Aggregation : 1;
    unsigned STBC : 2;
    unsigned FEC_CODING : 1;
    unsigned SGI : 1;
    unsigned rxend_state : 8;
    unsigned ampdu_cnt : 8;
    unsigned channel : 4;
    unsigned : 12;
} wifi_pkt_rx_ctrl_t;

typedef struct {
    wifi_pkt_rx_ctrl_t rx_ctrl;
    uint8_t payload[0]; /* ieee80211 packet buff */
} wifi_promiscuous_pkt_t;

QuickEspNow quickEspNow;

bool QuickEspNow::begin (uint8_t channel, uint32_t wifi_interface, bool synchronousSend) {

    this->synchronousSend = synchronousSend;

    DEBUG_DBG (QESPNOW_TAG, "Channel: %d, Interface: %d", channel, wifi_interface);
    // Set the wifi interface
    switch (wifi_interface) {
    case WIFI_IF_STA:
        wifi_if = WIFI_IF_STA;
        break;
    case WIFI_IF_AP:
        wifi_if = WIFI_IF_AP;
        break;
    default:
        DEBUG_ERROR (QESPNOW_TAG, "Unknown wifi interface");
        return false;
        break;
    }

    // check channel
    if (channel != CURRENT_WIFI_CHANNEL && (channel < MIN_WIFI_CHANNEL || channel > MAX_WIFI_CHANNEL)) {
        DEBUG_ERROR (QESPNOW_TAG, "Invalid wifi channel %d", channel);
        return false;
    }

    // use current channel
    if (channel == CURRENT_WIFI_CHANNEL) {
        uint8_t ch;
        ch = WiFi.channel ();
        DEBUG_DBG (QESPNOW_TAG, "Current channel: %d", ch);
        channel = ch;
        followWiFiChannel = true;
    } else {
        setChannel (channel);
    }
    DEBUG_INFO (QESPNOW_TAG, "Starting ESP-NOW in in channel %u interface %s", channel, wifi_if == WIFI_IF_STA ? "STA" : "AP");

    this->channel = channel;

    return initComms ();
}

void QuickEspNow::stop () {
    DEBUG_INFO (QESPNOW_TAG, "-------------> ESP-NOW STOP");
    os_timer_disarm (&espnowTxTask);
    os_timer_disarm (&espnowRxTask);
    esp_now_unregister_recv_cb ();
    esp_now_unregister_send_cb ();
    esp_now_deinit ();
    followWiFiChannel = false;
}

bool QuickEspNow::readyToSendData () {
    return tx_queue.size () < queueSize;
}

bool QuickEspNow::setChannel (uint8_t channel) {
    
    if (followWiFiChannel) {
        DEBUG_WARN (QESPNOW_TAG, "Cannot set channel while following WiFi channel");
        return false;
    }
    
    if (!wifi_set_channel (channel)) {
        DEBUG_ERROR (QESPNOW_TAG, "Error setting wifi channel: %u", channel);
        return false;
    }

    this->channel = channel;
    
    return true;
}

comms_send_error_t QuickEspNow::send (const uint8_t* dstAddress, const uint8_t* payload, size_t payload_len) {
    comms_tx_queue_item_t message;

    if (!dstAddress || !payload || !payload_len) {
        DEBUG_WARN (QESPNOW_TAG, "Parameters error");
        return COMMS_SEND_PARAM_ERROR;
    }

    if (payload_len > ESP_NOW_MAX_DATA_LEN) {
        DEBUG_WARN (QESPNOW_TAG, "Length error. %d", payload_len);
        return COMMS_SEND_PAYLOAD_LENGTH_ERROR;
    }

    if (tx_queue.size () >= ESPNOW_QUEUE_SIZE) {
#ifdef MEAS_TPUT
        //comms_tx_queue_item_t* tempBuffer;
        //tempBuffer = tx_queue.front ();
        //txDataDropped += tempBuffer->payload_len;
#endif // MEAS_TPUT
        // tx_queue.pop ();
        // DEBUG_DBG (QESPNOW_TAG, "Message dropped");
        return COMMS_SEND_QUEUE_FULL_ERROR;
    }

    memcpy (message.dstAddress, dstAddress, ESP_NOW_ETH_ALEN);
    message.payload_len = payload_len;
    memcpy (message.payload, payload, payload_len);

    if (tx_queue.push (&message)) {
#ifdef MEAS_TPUT
        txDataSent += message.payload_len;
#endif // MEAS_TPUT
        DEBUG_DBG (QESPNOW_TAG, "--------- %d Comms messages queued. Len: %d", tx_queue.size (), payload_len);
        DEBUG_VERBOSE (QESPNOW_TAG, "--------- Ready to send is %s", readyToSend ? "true" : "false");
        if (synchronousSend) {
            waitingForConfirmation = true;
            DEBUG_INFO (QESPNOW_TAG, "--------- Waiting for send confirmation");
            while (waitingForConfirmation) {
//                esp_yield ();
               delay(0);
            }
            DEBUG_INFO (QESPNOW_TAG, "--------- Confirmation is %s", sentStatus == ESP_NOW_SEND_SUCCESS ? "true" : "false");
            return (sentStatus == ESP_NOW_SEND_SUCCESS) ? COMMS_SEND_OK : COMMS_SEND_CONFIRM_ERROR;
        }
        return COMMS_SEND_OK;
    } else {
        DEBUG_WARN (QESPNOW_TAG, "Error queuing Comms message to " MACSTR, MAC2STR (dstAddress));
        return COMMS_SEND_MSG_ENQUEUE_ERROR;
    }
}

void QuickEspNow::onDataRcvd (comms_hal_rcvd_data dataRcvd) {
    this->dataRcvd = dataRcvd;
}

#ifdef MEAS_TPUT
void QuickEspNow::calculateDataTP () {
    time_t measTime = (millis () - lastDataTPMeas);
    lastDataTPMeas = millis ();

    if (txDataSent > 0) {
        txDataTP = txDataSent * 1000 / measTime;
        //DEBUG_WARN(QESPNOW_TAG, "Meas time: %d, Data sent: %d, Data TP: %f", measTime, txDataSent, txDataTP);
        txDroppedDataRatio = (float)txDataDropped / (float)txDataSent;
        //DEBUG_WARN(QESPNOW_TAG, "Data dropped: %d, Drop ratio: %f", txDataDropped, txDroppedDataRatio);
        txDataSent = 0;
    } else {
        txDataTP = 0;
        txDroppedDataRatio = 0;
    }
    if (rxDataReceived > 0) {
        rxDataTP = rxDataReceived * 1000 / measTime;
        //DEBUG_WARN(QESPNOW_TAG, "Meas time: %d, Data received: %d, Data TP: %f", measTime, rxDataReceived, rxDataTP);
        rxDataReceived = 0;
    } else {
        rxDataTP = 0;
    }
    txDataDropped = 0;
}

void QuickEspNow::tp_timer_cb (void* param) {
    quickEspNow.calculateDataTP ();
    DEBUG_WARN (QESPNOW_TAG, "TxData TP: %.3f kbps, Drop Ratio: %.2f %%, RxDataTP: %.3f kbps",
                quickEspNow.txDataTP * 8 / 1000,
                quickEspNow.txDroppedDataRatio * 100,
                quickEspNow.rxDataTP * 8 / 1000);
}
#endif // MEAS_TPUT

void QuickEspNow::onDataSent (comms_hal_sent_data sentResult) {
    this->sentResult = sentResult;
}

int32_t QuickEspNow::sendEspNowMessage (comms_tx_queue_item_t* message) {
    int32_t error;

    if (!message) {
        DEBUG_WARN (QESPNOW_TAG, "Message is null");
        return -1;
    }
    if (!(message->payload_len) || (message->payload_len > ESP_NOW_MAX_DATA_LEN)) {
        DEBUG_WARN (QESPNOW_TAG, "Message length error");
        return -1;
    }

    DEBUG_VERBOSE (QESPNOW_TAG, "ESP-NOW message to " MACSTR, MAC2STR (message->dstAddress));

    readyToSend = false;
    DEBUG_VERBOSE (QESPNOW_TAG, "-------------- Ready to send: false");

    error = esp_now_send (message->dstAddress, message->payload, message->payload_len);
    DEBUG_DBG (QESPNOW_TAG, "esp now send result = %d", error);

    return error;
}

void QuickEspNow::espnowTxHandle () {
    if (readyToSend) {
        //DEBUG_WARN ("Process queue: Elements: %d", tx_queue.size ());
        comms_tx_queue_item_t* message;
        while (!tx_queue.empty ()) {
            if (!readyToSend) return;
            message = tx_queue.front ();
            DEBUG_DBG (QESPNOW_TAG, "Comms message got from queue. %d left", tx_queue.size ());
            DEBUG_VERBOSE (QESPNOW_TAG, "Ready to send is %s", readyToSend ? "true" : "false");
            DEBUG_VERBOSE (QESPNOW_TAG, "synchrnousSend is %s", synchronousSend ? "true" : "false");
            if (!sendEspNowMessage (message)) {
                DEBUG_DBG (QESPNOW_TAG, "Message to " MACSTR " sent. Len: %u", MAC2STR (message->dstAddress), message->payload_len);
            } else {
                DEBUG_WARN (QESPNOW_TAG, "Error sending message to " MACSTR ". Len: %u", MAC2STR (message->dstAddress), message->payload_len);
            }
            message->payload_len = 0;
            tx_queue.pop ();
            DEBUG_DBG (QESPNOW_TAG, "Comms message pop. Queue size %d", tx_queue.size ());
        }

    } else {
        DEBUG_DBG (QESPNOW_TAG, "Not ready to send");
    }
}

void QuickEspNow::enableTransmit (bool enable) {
    DEBUG_DBG (QESPNOW_TAG, "Send esp-now task %s", enable ? "enabled" : "disabled");
    if (enable) {
        os_timer_arm (&espnowTxTask, TASK_PERIOD, true);
        os_timer_arm (&espnowRxTask, TASK_PERIOD, true);
    } else {
        os_timer_disarm (&espnowTxTask);
        os_timer_disarm (&espnowRxTask);
    }
}

bool QuickEspNow::initComms () {
    if (esp_now_init ()) {
        DEBUG_ERROR (QESPNOW_TAG, "Failed to init ESP-NOW");
//        ESP.restart ();
//        delay (1);
        return false;
    }

    if (wifi_if == WIFI_IF_STA) {
        esp_now_set_self_role (ESP_NOW_ROLE_SLAVE);
    } else {
        esp_now_set_self_role (ESP_NOW_ROLE_CONTROLLER);
    }

    esp_now_register_recv_cb (reinterpret_cast<esp_now_recv_cb_t>(rx_cb));
    esp_now_register_send_cb (reinterpret_cast<esp_now_send_cb_t>(tx_cb));

    os_timer_setfn (&espnowTxTask, espnowTxTask_cb, NULL);
    os_timer_arm (&espnowTxTask, TASK_PERIOD, true);

    os_timer_setfn (&espnowRxTask, espnowRxTask_cb, NULL);
    os_timer_arm (&espnowRxTask, TASK_PERIOD, true);

#ifdef MEAS_TPUT
    os_timer_setfn (&dataTPTimer, tp_timer_cb, NULL);
    os_timer_arm (&dataTPTimer, MEAS_TP_EVERY_MS, true);
#endif // MEAS_TPUT

    return true;
}

void QuickEspNow::espnowTxTask_cb (void* param) {
    quickEspNow.espnowTxHandle ();
}

void QuickEspNow::rx_cb (uint8_t* mac_addr, uint8_t* data, uint8_t len) {
    espnow_frame_format_t* espnow_data = (espnow_frame_format_t*)(data - sizeof (espnow_frame_format_t));
    wifi_promiscuous_pkt_t* promiscuous_pkt = (wifi_promiscuous_pkt_t*)(data - sizeof (wifi_pkt_rx_ctrl_t) - sizeof (espnow_frame_format_t));
    wifi_pkt_rx_ctrl_t* rx_ctrl = &promiscuous_pkt->rx_ctrl;

    comms_rx_queue_item_t message;

    DEBUG_DBG (QESPNOW_TAG, "Received message with RSSI %d from " MACSTR " Len: %u", rx_ctrl->rssi, MAC2STR (mac_addr), len);

    memcpy (message.srcAddress, mac_addr, ESP_NOW_ETH_ALEN);
    memcpy (message.payload, data, len);
    message.payload_len = len;
    message.rssi = rx_ctrl->rssi - 100;
    memcpy (message.dstAddress, espnow_data->destination_address, ESP_NOW_ETH_ALEN);
    
    if (quickEspNow.rx_queue.size () >= ESPNOW_QUEUE_SIZE) {
        quickEspNow.tx_queue.pop ();
        DEBUG_DBG (QESPNOW_TAG, "Rx Message dropped");
    }
    
#ifdef MEAS_TPUT
    quickEspNow.rxDataReceived += len;
#endif // MEAS_TPUT

    if (quickEspNow.rx_queue.push (&message)) {
        DEBUG_DBG (QESPNOW_TAG, "Message pushed to queue");
    } else {
        DEBUG_WARN (QESPNOW_TAG, "Error queuing message");
    }
}

void QuickEspNow::espnowRxTask_cb (void* param) {
    quickEspNow.espnowRxHandle ();
}

void QuickEspNow::espnowRxHandle () {
    comms_rx_queue_item_t *rxMessage;

    if (!rx_queue.empty ()) {
        rxMessage = rx_queue.front ();
        DEBUG_DBG (QESPNOW_TAG, "Comms message got from queue. %d left", rx_queue.size ());
        DEBUG_VERBOSE (QESPNOW_TAG, "Received message from " MACSTR " Len: %u", MAC2STR (rxMessage->srcAddress), rxMessage->payload_len);
        DEBUG_VERBOSE (QESPNOW_TAG, "Message: %.*s", rxMessage->payload_len, rxMessage->payload);


        if (quickEspNow.dataRcvd) {
            bool broadcast = ! memcmp (rxMessage->dstAddress, ESPNOW_BROADCAST_ADDRESS, ESP_NOW_ETH_ALEN);
        // quickEspNow.dataRcvd (mac_addr, data, len, rx_ctrl->rssi - 98); // rssi should be in dBm but it has added almost 100 dB. Do not know why
            quickEspNow.dataRcvd (rxMessage->srcAddress, rxMessage->payload, rxMessage->payload_len, rxMessage->rssi, broadcast); // rssi should be in dBm but it has added almost 100 dB. Do not know why
        }

        rxMessage->payload_len = 0;
        rx_queue.pop ();
        DEBUG_DBG (QESPNOW_TAG, "RX Comms message pop. Queue size %d", rx_queue.size ());
    }

}

void QuickEspNow::tx_cb (uint8_t* mac_addr, uint8_t status) {
    quickEspNow.readyToSend = true;
    quickEspNow.sentStatus = status;
    DEBUG_DBG (QESPNOW_TAG, "-------------- Tx Confirmed %s", status == ESP_NOW_SEND_SUCCESS ? "true" : "false");
    quickEspNow.waitingForConfirmation = false;
    DEBUG_DBG (QESPNOW_TAG, "-------------- Ready to send: true");
    if (quickEspNow.sentResult) {
        quickEspNow.sentResult (mac_addr, status);
    }
}

#endif // ESP8266