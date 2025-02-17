#include "QuickEspNow.h"

#ifdef ESP32

QuickEspNow quickEspNow;

constexpr auto PEERLIST_TAG = "PEERLIST";


bool QuickEspNow::begin (uint8_t channel, uint32_t wifi_interface, bool synchronousSend) {

    wifi_second_chan_t ch2 = WIFI_SECOND_CHAN_NONE;
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
        esp_wifi_get_channel (&ch, &ch2);
        channel = ch;
        DEBUG_DBG (QESPNOW_TAG, "Current channel: %d : %d", channel, ch2);
        followWiFiChannel = true;
    }
    setChannel (channel, ch2);

    DEBUG_INFO (QESPNOW_TAG, ARDUHAL_LOG_COLOR (ARDUHAL_LOG_COLOR_RED) "Starting ESP-NOW in in channel %u interface %s", channel, wifi_if == WIFI_IF_STA ? "STA" : "AP");

    this->channel = channel;

    return initComms ();
}

void QuickEspNow::stop () {
    DEBUG_INFO (QESPNOW_TAG, "-------------> ESP-NOW STOP");
    if (espnowTxTask) { 
      vTaskDelete (espnowTxTask);
      espnowTxTask = nullptr;
    }
    if (espnowRxTask) { 
      vTaskDelete (espnowRxTask);
      espnowRxTask = nullptr;
    }
    esp_now_unregister_recv_cb ();
    esp_now_unregister_send_cb ();
    esp_now_deinit ();
    followWiFiChannel = false;
}

bool QuickEspNow::readyToSendData () {
    return uxQueueMessagesWaiting (tx_queue) < queueSize;
}

bool QuickEspNow::setChannel (uint8_t channel, wifi_second_chan_t ch2) {

    if (followWiFiChannel) {
        DEBUG_WARN(QESPNOW_TAG, "Cannot set channel while following WiFi channel");
        return false;
    }
    
    esp_err_t err_ok;
    if ((err_ok = esp_wifi_set_promiscuous (true))) {
        DEBUG_ERROR (QESPNOW_TAG, "Error setting promiscuous mode: %s", esp_err_to_name (err_ok));
        return false;
    }
    if ((err_ok = esp_wifi_set_channel (channel, ch2))) { // This is needed even in STA mode. If not done and using IDF > 4.0, the ESP-NOW will not work.
        DEBUG_DBG (QESPNOW_TAG, "Error setting wifi channel: %d - %s", err_ok, esp_err_to_name (err_ok));
        return false;
    }
    if ((err_ok = esp_wifi_set_promiscuous (false))) {
        DEBUG_ERROR (QESPNOW_TAG, "Error setting promiscuous mode off: %s", esp_err_to_name (err_ok));
        return false;
    }

    this->channel = channel;

    return true;
}

comms_send_error_t QuickEspNow::send (const uint8_t* dstAddress, const uint8_t* payload, size_t payload_len) {
    comms_tx_queue_item_t message;

    if (!dstAddress || !payload || !payload_len) {
        DEBUG_WARN (QESPNOW_TAG, "Parameters error");
        return COMMS_SEND_PAYLOAD_LENGTH_ERROR;
    }

    if (payload_len > ESP_NOW_MAX_DATA_LEN) {
        DEBUG_WARN (QESPNOW_TAG, "Length error. %d", payload_len);
        return COMMS_SEND_PAYLOAD_LENGTH_ERROR;
    }

    if (uxQueueMessagesWaiting (tx_queue) >= queueSize) {
        // comms_tx_queue_item_t tempBuffer;
        // xQueueReceive (tx_queue, &tempBuffer, 0);
#ifdef MEAS_TPUT
        //txDataDropped += tempBuffer.payload_len;
#endif // MEAS_TPUT
        //DEBUG_DBG (QESPNOW_TAG, "Message dropped");
        return COMMS_SEND_QUEUE_FULL_ERROR;
    }
    memcpy (message.dstAddress, dstAddress, ESP_NOW_ETH_ALEN);
    message.payload_len = payload_len;
    memcpy (message.payload, payload, payload_len);

    if (xQueueSend (tx_queue, &message, pdMS_TO_TICKS (10))) {
#ifdef MEAS_TPUT
        txDataSent += message.payload_len;
#endif // MEAS_TPUT
        DEBUG_DBG (QESPNOW_TAG, "--------- %d Comms messages queued. Len: %d", uxQueueMessagesWaiting (tx_queue), payload_len);
        DEBUG_VERBOSE (QESPNOW_TAG, "--------- Ready to send is %s", readyToSend ? "true" : "false");
        DEBUG_VERBOSE (QESPNOW_TAG, "--------- SyncronousSend is %s", synchronousSend ? "true" : "false");
        if (synchronousSend) {
            waitingForConfirmation = true;
            DEBUG_INFO (QESPNOW_TAG, "--------- Waiting for send confirmation");
            while (waitingForConfirmation) {
                taskYIELD ();
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
        //DEBUG_WARN("Meas time: %d, Data sent: %d, Data TP: %f", measTime, txDataSent, txDataTP);
        txDroppedDataRatio = (float)txDataDropped / (float)txDataSent;
        //DEBUG_WARN("Data dropped: %d, Drop ratio: %f", txDataDropped, txDroppedDataRatio);
        txDataSent = 0;
    } else {
        txDataTP = 0;
        txDroppedDataRatio = 0;
    }
    if (rxDataReceived > 0) {
        rxDataTP = rxDataReceived * 1000 / measTime;
        //DEBUG_WARN("Meas time: %d, Data received: %d, Data TP: %f", measTime, rxDataReceived, rxDataTP);
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


    addPeer (message->dstAddress);
    DEBUG_DBG (QESPNOW_TAG, "Peer added " MACSTR, MAC2STR (message->dstAddress));
    readyToSend = false;
    DEBUG_VERBOSE (QESPNOW_TAG, "-------------- Ready to send: false");

    error = esp_now_send (message->dstAddress, message->payload, message->payload_len);
    DEBUG_DBG (QESPNOW_TAG, "esp now send result = %s", esp_err_to_name (error));
    if (error != ESP_OK) {
        DEBUG_WARN (QESPNOW_TAG, "Error sending message: %s", esp_err_to_name (error));
    }
    // if (error == ESP_OK) {
    //     txDataSent += message->payload_len;
    // }
    if (error == ESP_ERR_ESPNOW_NO_MEM) {
        delay (2);
    }

    return error;
}

void QuickEspNow::espnowTxHandle () {
    if (readyToSend) {
    //DEBUG_WARN ("Process queue: Elements: %d", tx_queue.size ());
        comms_tx_queue_item_t message;
        while (xQueueReceive (tx_queue, &message, pdMS_TO_TICKS (1000))) {
            DEBUG_DBG (QESPNOW_TAG, "Comms message got from queue. %d left", uxQueueMessagesWaiting (tx_queue));
            while (!readyToSend && !synchronousSend) {
                delay (0);
            }
            if (!sendEspNowMessage (&message)) {
                DEBUG_DBG (QESPNOW_TAG, "Message to " MACSTR " sent. Len: %u", MAC2STR (message.dstAddress), message.payload_len);
            } else {
                DEBUG_WARN (QESPNOW_TAG, "Error sending message to " MACSTR ". Len: %u", MAC2STR (message.dstAddress), message.payload_len);
            }
        //message.payload_len = 0;
            DEBUG_DBG (QESPNOW_TAG, "Comms message pop. Queue size %d", uxQueueMessagesWaiting (tx_queue));
        }

    } else {
        DEBUG_DBG (QESPNOW_TAG, "Not ready to send");
    }
}

void QuickEspNow::enableTransmit (bool enable) {
    DEBUG_DBG (QESPNOW_TAG, "Send esp-now task %s", enable ? "enabled" : "disabled");
    if (enable) {
        if (espnowTxTask_cb) {
            vTaskResume (espnowTxTask);
            vTaskResume (espnowRxTask);
        }
    } else {
        if (espnowTxTask_cb) {
            vTaskSuspend (espnowTxTask);
            vTaskSuspend (espnowRxTask);
        }
    }
}

bool QuickEspNow::addPeer (const uint8_t* peer_addr) {
    esp_now_peer_info_t peer;
    esp_err_t error = ESP_OK;

    if (peer_list.get_peer_number () >= ESP_NOW_MAX_TOTAL_PEER_NUM) {
        DEBUG_VERBOSE (QESPNOW_TAG, "Peer list full. Deleting older");
        if (uint8_t* deleted_mac = peer_list.delete_peer ()) {
            esp_now_del_peer (deleted_mac);
        } else {
            DEBUG_ERROR (QESPNOW_TAG, "Error deleting peer");
            return false;
        }
    }

    if (peer_list.peer_exists (peer_addr)) {
        DEBUG_VERBOSE (QESPNOW_TAG, "Peer already exists");
        ESP_ERROR_CHECK (esp_now_get_peer (peer_addr, &peer));

        uint8_t currentChannel = peer.channel;
        DEBUG_DBG (QESPNOW_TAG, "Peer " MACSTR " is using channel %d", MAC2STR (peer_addr), currentChannel);
        if (currentChannel != this->channel) {
            DEBUG_DBG (QESPNOW_TAG, "Peer channel has to change from %d to %d", currentChannel, this->channel);
            ESP_ERROR_CHECK_WITHOUT_ABORT (esp_now_get_peer (peer_addr, &peer));
            peer.channel = this->channel;
            ESP_ERROR_CHECK_WITHOUT_ABORT (esp_now_mod_peer (&peer));
            DEBUG_ERROR (QESPNOW_TAG, "Peer channel changed to %d", this->channel);
        }
        return true;
    }

    memcpy (peer.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);
    uint8_t ch;
    wifi_second_chan_t secondCh;
    esp_wifi_get_channel (&ch, &secondCh);
    peer.channel = ch;
    peer.ifidx = wifi_if;
    peer.encrypt = false;
    error = esp_now_add_peer (&peer);
    if (!error) {
        DEBUG_DBG (QESPNOW_TAG, "Peer added");
        peer_list.add_peer (peer_addr);
    } else {
        DEBUG_ERROR (QESPNOW_TAG, "Error adding peer: %s", esp_err_to_name (error));
        return false;
    }
    DEBUG_DBG (QESPNOW_TAG, "Peer " MACSTR " added on channel %u. Result 0x%X %s", MAC2STR (peer_addr), ch, error, esp_err_to_name (error));
    return error == ESP_OK;
}

bool QuickEspNow::initComms () {
    if (esp_now_init ()) {
        DEBUG_ERROR (QESPNOW_TAG, "Failed to init ESP-NOW");
//        ESP.restart ();
//        delay (1);
        return false;
    }

    esp_now_register_recv_cb (rx_cb);
    esp_now_register_send_cb (reinterpret_cast<esp_now_send_cb_t>(tx_cb));

    int txQueueSize = queueSize;
    if (synchronousSend) {
        txQueueSize = 1;
    }
    
    tx_queue = xQueueCreate (txQueueSize, sizeof (comms_tx_queue_item_t));
    xTaskCreateUniversal (espnowTxTask_cb, "espnow_loop", 8 * 1024, NULL, 1, &espnowTxTask, CONFIG_ARDUINO_RUNNING_CORE);

    rx_queue = xQueueCreate (queueSize, sizeof (comms_rx_queue_item_t));
    xTaskCreateUniversal (espnowRxTask_cb, "receive_handle", 4 * 1024, NULL, 1, &espnowRxTask, CONFIG_ARDUINO_RUNNING_CORE);

#ifdef MEAS_TPUT
    dataTPTimer = xTimerCreate ("espnow_tp_timer", pdMS_TO_TICKS (MEAS_TP_EVERY_MS), pdTRUE, NULL, tp_timer_cb);
    xTimerStart (dataTPTimer, 0);
#endif // MEAS_TPUT

    return true;
}

void QuickEspNow::espnowTxTask_cb (void* param) {
    for (;;) {
        quickEspNow.espnowTxHandle ();
    }

}

void QuickEspNow::espnowRxHandle () {
    comms_rx_queue_item_t rxMessage;

    if (xQueueReceive (rx_queue, &rxMessage, portMAX_DELAY)) {
        DEBUG_DBG (QESPNOW_TAG, "Comms message got from queue. %d left", uxQueueMessagesWaiting (rx_queue));
        DEBUG_VERBOSE (QESPNOW_TAG, "Received message from " MACSTR " Len: %u", MAC2STR (rxMessage.srcAddress), rxMessage.payload_len);
        DEBUG_VERBOSE (QESPNOW_TAG, "Message: %.*s", rxMessage.payload_len, rxMessage.payload);

        if (quickEspNow.dataRcvd) {
            bool broadcast = !memcmp (rxMessage.dstAddress, ESPNOW_BROADCAST_ADDRESS, ESP_NOW_ETH_ALEN);
            quickEspNow.dataRcvd (rxMessage.srcAddress, rxMessage.payload, rxMessage.payload_len, rxMessage.rssi, broadcast); // rssi should be in dBm but it has added almost 100 dB. Do not know why
        }
    } else {
        DEBUG_DBG (QESPNOW_TAG, "No message in queue");
    }

}

void QuickEspNow::espnowRxTask_cb (void* param) {
    for (;;) {
        quickEspNow.espnowRxHandle ();
    }
}

void QuickEspNow::rx_cb(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int len) {
    espnow_frame_format_t* espnow_data = (espnow_frame_format_t*)(data - sizeof(espnow_frame_format_t));
    wifi_promiscuous_pkt_t* promiscuous_pkt = (wifi_promiscuous_pkt_t*)(data - sizeof(wifi_pkt_rx_ctrl_t) - sizeof(espnow_frame_format_t));
    wifi_pkt_rx_ctrl_t* rx_ctrl = &promiscuous_pkt->rx_ctrl;
    const uint8_t* mac_addr = esp_now_info->src_addr;
    comms_rx_queue_item_t message;

    DEBUG_DBG(QESPNOW_TAG, "Received message with RSSI %d from " MACSTR " Len: %u", rx_ctrl->rssi, MAC2STR(esp_now_info->src_addr), len);

    memcpy(message.srcAddress, mac_addr, ESP_NOW_ETH_ALEN);
    memcpy(message.payload, data, len);
    message.payload_len = len;
    message.rssi = rx_ctrl->rssi;
    memcpy(message.dstAddress, espnow_data->destination_address, ESP_NOW_ETH_ALEN);

    if (uxQueueMessagesWaiting(quickEspNow.rx_queue) >= quickEspNow.queueSize) {
        comms_rx_queue_item_t tempBuffer;
        xQueueReceive(quickEspNow.rx_queue, &tempBuffer, 0);
        DEBUG_DBG(QESPNOW_TAG, "Rx Message dropped");
    }
#ifdef MEAS_TPUT
    quickEspNow.rxDataReceived += len;
#endif // MEAS_TPUT

    if (!xQueueSend(quickEspNow.rx_queue, &message, pdMS_TO_TICKS(100))) {
        DEBUG_WARN(QESPNOW_TAG, "Error sending message to queue");
    }
}

void QuickEspNow::tx_cb (uint8_t* mac_addr, uint8_t status) {
    quickEspNow.readyToSend = true;
    quickEspNow.sentStatus = status;
    quickEspNow.waitingForConfirmation = false;
    DEBUG_DBG (QESPNOW_TAG, "-------------- Ready to send: true. Status: %d", status);
    if (quickEspNow.sentResult) {
        quickEspNow.sentResult (mac_addr, status);
    }
}

uint8_t PeerListClass::get_peer_number () {
    return peer_list.peer_number;
}

bool PeerListClass::peer_exists (const uint8_t* mac) {
    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        if (memcmp (peer_list.peer[i].mac, mac, ESP_NOW_ETH_ALEN) == 0) {
            if (peer_list.peer[i].active) {
                peer_list.peer[i].last_msg = millis ();
                DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " found. Updated last_msg", MAC2STR (mac));
                return true;
            }
        }
    }
    return false;
}

peer_t* PeerListClass::get_peer (const uint8_t* mac) {
    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        if (memcmp (peer_list.peer[i].mac, mac, ESP_NOW_ETH_ALEN) == 0) {
            if (peer_list.peer[i].active) {
                DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " found", MAC2STR (mac));
                return &(peer_list.peer[i]);
            }
        }
    }
    return NULL;
}

bool PeerListClass::update_peer_use (const uint8_t* mac) {
    peer_t* peer = get_peer (mac);
    if (peer) {
        peer->last_msg = millis ();
        return true;
    }
    return false;
}

bool PeerListClass::add_peer (const uint8_t* mac) {
    if (int i = peer_exists (mac)) {
        DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " already exists", MAC2STR (mac));
        return false;
    }
    if (peer_list.peer_number >= ESP_NOW_MAX_TOTAL_PEER_NUM) {
        //DEBUG_VERBOSE (PEERLIST_TAG, "Peer list full. Deleting older");
#ifndef UNIT_TEST
        DEBUG_ERROR (PEERLIST_TAG, "Should never happen");
#endif
        return false;
        // delete_peer (); // Delete should happen in higher level
    }

    for (uint8_t i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        if (!peer_list.peer[i].active) {
            memcpy (peer_list.peer[i].mac, mac, ESP_NOW_ETH_ALEN);
            peer_list.peer[i].active = true;
            peer_list.peer[i].last_msg = millis ();
            peer_list.peer_number++;
            DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " added. Total peers = %d", MAC2STR (mac), peer_list.peer_number);
            return true;
        }
    }

    return false;
}

bool PeerListClass::delete_peer (const uint8_t* mac) {
    peer_t* peer = get_peer (mac);
    if (peer) {
        peer->active = false;
        peer_list.peer_number--;
        DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " deleted. Total peers = %d", MAC2STR (mac), peer_list.peer_number);
        return true;
    }
    return false;
}

// Delete peer with older message
uint8_t* PeerListClass::delete_peer () {
    uint32_t oldest_msg = 0;
    int oldest_index = -1;
    uint8_t* mac = NULL;
    for (int i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        if (peer_list.peer[i].active) {
            if (peer_list.peer[i].last_msg < oldest_msg || oldest_msg == 0) {
                oldest_msg = peer_list.peer[i].last_msg;
                oldest_index = i;
                DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " is %d ms old. Deleting", MAC2STR (peer_list.peer[i].mac), oldest_msg);
            }
        }
    }
    if (oldest_index != -1) {
        peer_list.peer[oldest_index].active = false;
        peer_list.peer_number--;
        mac = peer_list.peer[oldest_index].mac;
        DEBUG_VERBOSE (PEERLIST_TAG, "Peer " MACSTR " deleted. Last message %d ms ago. Total peers = %d", MAC2STR (mac), millis () - peer_list.peer[oldest_index].last_msg, peer_list.peer_number);
    }
    return mac;
}

bool QuickEspNow::setWiFiBandwidth (wifi_interface_t iface, wifi_bandwidth_t bw) {
    esp_err_t err_ok;
    if ((err_ok = esp_wifi_set_bandwidth (iface, bw))) {
        DEBUG_ERROR (QESPNOW_TAG, "Error setting wifi bandwidth: %s", esp_err_to_name (err_ok));
    }
    return !err_ok;
}

#ifdef UNIT_TEST
void PeerListClass::dump_peer_list () {
    Serial.printf ("Number of peers %d\n", peer_list.peer_number);
    for (int i = 0; i < ESP_NOW_MAX_TOTAL_PEER_NUM; i++) {
        if (peer_list.peer[i].active) {
            Serial.printf ("Peer " MACSTR " is %d ms old\n", MAC2STR (peer_list.peer[i].mac), millis () - peer_list.peer[i].last_msg);
        }
    }
}
#endif // UNIT_TEST
#endif // ESP32
