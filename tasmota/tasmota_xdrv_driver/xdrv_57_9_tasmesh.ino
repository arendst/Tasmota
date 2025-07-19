/*
  xdrv_57_tasmesh.ino - Mesh support for Tasmota using ESP-Now

  Copyright (C) 2021  Christian Baars, Federico Leoni and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action     Description
  --------------------------------------------------------------------------------------------
  0.9.5.1 20210622  integrate  Expand number of chunks to satisfy larger MQTT messages
                               Refactor to latest Tasmota standards
  ---
  0.9.4.1 20210503  integrate  Add some minor tweak for channel management by Federico Leoni
  ---
  0.9.0.0 20200927  started    From scratch by Christian Baars
*/

#ifdef USE_TASMESH

/*********************************************************************************************\
* Build a mesh of nodes using ESP-Now
* Connect it through an ESP32-broker to WLAN
\*********************************************************************************************/

#define XDRV_57             57

/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/

#ifdef ESP32

void CB_MESHDataSent(const uint8_t *MAC, esp_now_send_status_t sendStatus);
void CB_MESHDataSent(const uint8_t *MAC, esp_now_send_status_t sendStatus) {
  char _destMAC[18];
  ToHex_P(MAC, 6, _destMAC, 18, ':');
  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Sent to %s status %d"), _destMAC, sendStatus);
}

//void CB_MESHDataReceived(const uint8_t *MAC, const uint8_t *packet, int len) {
void CB_MESHDataReceived(const esp_now_recv_info_t *esp_now_info, const uint8_t *packet, int len);
void CB_MESHDataReceived(const esp_now_recv_info_t *esp_now_info, const uint8_t *packet, int len) {
  static bool _locked = false;
  if (_locked) { return; }

  _locked = true;

  uint8_t *MAC = esp_now_info->src_addr;

  char _srcMAC[18];
  ToHex_P(MAC, 6, _srcMAC, 18, ':');
  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Rcvd from %s"), _srcMAC);
  mesh_packet_t *_recvPacket = (mesh_packet_t*)packet;
  if ((_recvPacket->type == PACKET_TYPE_REGISTER_NODE) || (_recvPacket->type == PACKET_TYPE_REFRESH_NODE)) {
    if (MESHcheckPeerList((const uint8_t *)MAC) == false) {
      MESHencryptPayload(_recvPacket, 0); //decrypt it and check
      if (memcmp(_recvPacket->payload, MESH.broker, 6) == 0) {
        MESHaddPeer((uint8_t*)MAC);
//        AddLog(LOG_LEVEL_INFO, PSTR("MSH: Rcvd topic %s, payload %*_H"), (char*)_recvPacket->payload + 6, MESH.packetToConsume.front().chunkSize+5, (uint8_t *)&MESH.packetToConsume.front().payload);
        for (auto &_peer : MESH.peers) {
          if (memcmp(_peer.MAC, _recvPacket->sender, 6) == 0) {
            strcpy(_peer.topic, (char*)_recvPacket->payload + 6);
            MESHsubscribe((char*)&_peer.topic);
            _locked = false;
            return;
          }
        }
      } else {
        char _cryptMAC[18];
        ToHex_P(_recvPacket->payload, 6, _cryptMAC, 18, ':');
        AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Peer %s denied, wrong MAC %s"), _srcMAC, _cryptMAC);
        _locked = false;
        return;
      }
    } else {
      if (_recvPacket->type == PACKET_TYPE_REGISTER_NODE) {
        MESH.flags.nodeWantsTimeASAP = 1; //this could happen after wake from deepsleep on battery powered device
      } else {
        MESH.flags.nodeWantsTime = 1;
      }
    }
  }
  MESH.lmfap = millis();
  if (MESHcheckPeerList(MAC) == true){
    MESH.packetToConsume.push(*_recvPacket);
    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Packet %d from %s to queue"), MESH.packetToConsume.size(), _srcMAC);
  }
  _locked = false;
}

#else  // ESP8266

void CB_MESHDataSent(uint8_t *MAC, uint8_t sendStatus) {
  char _destMAC[18];
  ToHex_P(MAC, 6, _destMAC, 18, ':');
  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Sent to %s status %d"), _destMAC, sendStatus);
}

void CB_MESHDataReceived(uint8_t *MAC, uint8_t *packet, uint8_t len) {
  MESH.lmfap = millis(); //any peer
  if (memcmp(MAC, MESH.broker, 6) == 0) {
    MESH.lastMessageFromBroker = millis(); //directly from the broker
  }
  mesh_packet_t *_recvPacket = (mesh_packet_t*)packet;
  switch (_recvPacket->type) {
    case PACKET_TYPE_TIME:
      Rtc.utc_time = _recvPacket->senderTime;
      Rtc.user_time_entry = true;
      MESH.lastMessageFromBroker = millis();
      if (MESH.flags.nodeGotTime == 0) {
        RtcSync("Mesh");
        TasmotaGlobal.rules_flag.system_boot  = 1; // for now we consider the node booted and let trigger system#boot on RULES
      }
      MESH.flags.nodeGotTime = 1;
      //Wifi.retry = 0;
      // Response_P(PSTR("{\"%s\":{\"Time\":1}}"), D_CMND_MESH); //got the time, now we can publish some sensor data
      // XdrvRulesProcess();
      break;
    case PACKET_TYPE_PEERLIST:
      MESH.packetToConsume.push(*_recvPacket);
      return;
      break;
    default:
      // nothing for now;
      break;
  }
  if (memcmp(_recvPacket->receiver, MESH.sendPacket.sender, 6) != 0) { //MESH.sendPacket.sender simply stores the MAC of the node
    if (ROLE_NODE_SMALL == MESH.role) {
      return; // a 'small node' does not perform mesh functions
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Packet to resend ..."));
    MESH.packetToResend.push(*_recvPacket);
    return;
  } else {
    if (_recvPacket->type == PACKET_TYPE_WANTTOPIC) {
      MESH.flags.brokerNeedsTopic = 1;
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Broker needs topic ..."));
      return; //nothing left to be done
    }
    // for(auto &_message : MESH.packetsAlreadyReceived){
    //   if(memcmp(_recvPacket,_message,15==0)){
    //     AddLog(LOG_LEVEL_INFO, PSTR("MSH: Packet already received"));
    //     return;
    //   }
    // }
    // MESH.packetsAlreadyReceived.push_back((mesh_packet_header_t*) _recvPacket);
    // AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Packet to consume ..."));
    MESH.packetToConsume.push(*_recvPacket);
  }
}

#endif  // ESP32

/*********************************************************************************************\
 * init driver
\*********************************************************************************************/

void MESHInit(void) {
  MESH.interval = MESH_REFRESH;
  MESH.role = ROLE_NONE;
  MESH.packetsAlreadyReceived.reserve(5);
  MESH.peers.reserve(10);
  MESH.multiPackets.reserve(2);

  MESH.sendPacket.counter = 0;
  MESH.sendPacket.chunks = 1;
  MESH.sendPacket.chunk = 0;
  MESH.sendPacket.type = PACKET_TYPE_TIME;
  MESH.sendPacket.TTL = 2;

  MESHsetWifi(1);           // (Re-)enable wifi as long as Mesh is not enabled

  AddLog(LOG_LEVEL_INFO, PSTR("MSH: Initialized"));
}

void MESHdeInit(void) {
#ifdef ESP8266  // only ESP8266, ESP32 as a broker should not use deepsleep
  AddLog(LOG_LEVEL_INFO, PSTR("MSH: Stopping"));
  // TODO: degister from the broker, so he can stop MQTT-proxy
  esp_now_deinit();
#endif  // ESP8266
}

/*********************************************************************************************\
 * MQTT proxy functions
\*********************************************************************************************/

#ifdef ESP32

/**
 * @brief Subscribes as a proxy
 *
 * @param topic - received from the referring node
 */
void MESHsubscribe(char *topic) {
  char stopic[TOPSZ];
  GetTopic_P(stopic, CMND, topic, PSTR("#"));
  MqttSubscribe(stopic);
}

void MESHunsubscribe(char *topic) {
  char stopic[TOPSZ];
  GetTopic_P(stopic, CMND, topic, PSTR("#"));
  MqttUnsubscribe(stopic);
}

void MESHconnectMQTT(void){
  for (auto &_peer : MESH.peers) {
    AddLog(LOG_LEVEL_INFO, PSTR("MSH: Reconnect topic %s"), _peer.topic);
    if (_peer.topic[0] != 0) {
      MESHsubscribe(_peer.topic);
    }
  }
}

/**
 * @brief Intercepts mqtt message, that the broker (ESP32) subscribes to as a proxy for a node.
 *        Is called from xdrv_02_mqtt.ino. Will send the message in the payload via ESP-NOW.
 *
 * @param _topic
 * @param _data
 * @param data_len
 * @return true
 * @return false
 */
bool MESHinterceptMQTTonBroker(char* _topic, uint8_t* _data, unsigned int data_len) {
  if (MESH.role != ROLE_BROKER) { return false; }

  char stopic[TOPSZ];
//  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Intercept topic %s"), _topic);
  for (auto &_peer : MESH.peers) {
    GetTopic_P(stopic, CMND, _peer.topic, PSTR("")); //cmnd/topic/
    if (strlen(_topic) != strlen(_topic)) {
      return false; // prevent false result when _topic is the leading substring of stopic
    }
    if (memcmp(_topic, stopic, strlen(stopic)) == 0) {
      MESH.sendPacket.chunkSize = strlen(_topic) +1;

      if (MESH.sendPacket.chunkSize + data_len > MESH_PAYLOAD_SIZE) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Intercept payload oversized %d"), data_len);
        return false;
      }

      memcpy(MESH.sendPacket.receiver, _peer.MAC, 6);
      memcpy(MESH.sendPacket.payload, _topic, MESH.sendPacket.chunkSize);
      memcpy(MESH.sendPacket.payload + MESH.sendPacket.chunkSize, _data, data_len);
      MESH.sendPacket.chunkSize += data_len;
      MESH.sendPacket.chunks = 1;
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Intercept payload '%s'"), MESH.sendPacket.payload);
      MESH.sendPacket.type = PACKET_TYPE_MQTT;
      MESH.sendPacket.senderTime = Rtc.utc_time;
      MESHsendPacket(&MESH.sendPacket);
      // int result = esp_now_send(MESH.sendPacket.receiver, (uint8_t *)&MESH.sendPacket, (sizeof(MESH.sendPacket))-(MESH_PAYLOAD_SIZE-MESH.sendPacket.chunkSize));
      //send to Node
      return true;
    }
  }
  return false;
}

#else  // ESP8266

void MESHreceiveMQTT(mesh_packet_t *_packet);
void MESHreceiveMQTT(mesh_packet_t *_packet){
  uint32_t _slength = strlen((char*)_packet->payload);
  if (_packet->chunks == 1) { //single chunk message
    MqttDataHandler((char*)_packet->payload, (uint8_t*)(_packet->payload)+_slength+1, (_packet->chunkSize)-_slength);
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("MSH: Multiple chunks %u not supported yet"), _packet->chunks);
    // TODO: reconstruct message in buffer or only handle short messages
  }
}

#endif  // ESP32

bool MESHroleNode(void) {
  return (MESH.role > ROLE_BROKER);
}

/**
 * @brief Redirects the outgoing mqtt message on the node just before it would have been sent to
 *        the broker via ESP-NOW
 *
 * @param _topic
 * @param _data
 * @param _retained - currently unused
 * @return true
 * @return false
 */
bool MESHrouteMQTTtoMESH(const char* _topic, char* _data, bool _retained) {
  if (!MESHroleNode()) { return false; }

  size_t _bytesLeft = strlen(_topic) + strlen(_data) +2;
  MESH.sendPacket.counter++;
  MESH.sendPacket.chunk = 0;
  MESH.sendPacket.chunks = (_bytesLeft / MESH_PAYLOAD_SIZE) +1;
  memcpy(MESH.sendPacket.receiver, MESH.broker, 6);
  MESH.sendPacket.type = PACKET_TYPE_MQTT;
  MESH.sendPacket.chunkSize = MESH_PAYLOAD_SIZE;
  MESH.sendPacket.peerIndex = 0;
//  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Chunks %u, Counter %u"), MESH.sendPacket.chunks, MESH.sendPacket.counter);
  size_t _topicSize = strlen(_topic) +1;
  size_t _offsetData = 0;
  while (_bytesLeft > 0) {
    size_t _byteLeftInChunk = MESH_PAYLOAD_SIZE;
    // MESH.sendPacket.chunkSize = MESH_PAYLOAD_SIZE;
    if (MESH.sendPacket.chunk == 0) {
      memcpy(MESH.sendPacket.payload, _topic, _topicSize);
      MESH.sendPacket.chunkSize = _topicSize;

      MESH.currentTopicSize = MESH.sendPacket.chunkSize;

      _bytesLeft -= _topicSize;
      _byteLeftInChunk -= _topicSize;
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Topic in payload '%s'"), (char*)MESH.sendPacket.payload);
//      AddLog(LOG_LEVEL_INFO, PSTR("MSH: After topic -> chunk:%u, pre-size: %u"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize);
    }
    if (_byteLeftInChunk > 0) {
      if (_byteLeftInChunk > _bytesLeft) {
//        AddLog(LOG_LEVEL_INFO, PSTR("MSH: only last chunk bL:%u bLiC:%u oSD:%u"),_bytesLeft,_byteLeftInChunk,_offsetData);
        _byteLeftInChunk = _bytesLeft;
//        AddLog(LOG_LEVEL_INFO, PSTR("MSH: only last chunk after correction -> chunk:%u, pre-size: %u"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize);
      }
      if (MESH.sendPacket.chunk > 0) { _topicSize = 0; }
//      AddLog(LOG_LEVEL_INFO, PSTR("MSH: %u"),_topicSize);
      memcpy(MESH.sendPacket.payload + _topicSize, _data + _offsetData, _byteLeftInChunk);
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH:  Data in payload '%s'"), (char*)MESH.sendPacket.payload + _topicSize);
      _offsetData += _byteLeftInChunk;
      _bytesLeft -= _byteLeftInChunk;
    }
    MESH.sendPacket.chunkSize += _byteLeftInChunk;
    MESH.packetToResend.push(MESH.sendPacket);
//    AddLog(LOG_LEVEL_INFO, PSTR("MSH: chunk %u, size %u, payload %*_H"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize,MESH.sendPacket.chunkSize,(uint8_t*)MESH.sendPacket.payload);
    if (MESH.sendPacket.chunk == MESH.sendPacket.chunks) {
//      AddLog(LOG_LEVEL_INFO, PSTR("MSH: Too many chunks %u"), MESH.sendPacket.chunk +1);
    }

    SHOW_FREE_MEM(PSTR("MESHrouteMQTTtoMESH"));

    MESH.sendPacket.chunk++;
    MESH.sendPacket.chunkSize = 0;
  }
  return true;
}

/**
 * @brief The node sends its mqtt topic to the broker
 *
 */
void MESHregisterNode(uint8_t mode){
  memcpy(MESH.sendPacket.receiver, MESH.broker, 6);  // First 6 bytes -> MAC of broker
  strcpy((char*)MESH.sendPacket.payload +6, TasmotaGlobal.mqtt_topic);  // Remaining bytes -> topic of node
  AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Register node with topic '%s'"), (char*)MESH.sendPacket.payload +6);
  MESH.sendPacket.TTL = 2;
  MESH.sendPacket.chunks = 1;
  MESH.sendPacket.chunk = 0;
  MESH.sendPacket.chunkSize = strlen(TasmotaGlobal.mqtt_topic) + 1 + 6;
  memcpy(MESH.sendPacket.payload, MESH.broker, 6);
  MESH.sendPacket.type = (mode == 0) ? PACKET_TYPE_REGISTER_NODE : PACKET_TYPE_REFRESH_NODE;
  MESHsendPacket(&MESH.sendPacket);
}

/*********************************************************************************************\
 * Generic functions
\*********************************************************************************************/

void MESHstartNode(int32_t _channel, uint8_t _role){ //we need a running broker with a known channel at that moment
#ifdef ESP8266 // for now only ESP8266, might be added for the ESP32 later
  MESH.channel = _channel;
  WiFi.mode(WIFI_STA);
  WiFiHelper::begin("", "", MESH.channel, nullptr, false); //fake connection attempt to set channel
  wifi_promiscuous_enable(1);
  wifi_set_channel(MESH.channel);
  wifi_promiscuous_enable(0);
  WiFi.disconnect();
  MESHsetWifi(0);
  esp_now_deinit();  // in case it was already initialized but disconnected
  int init_result = esp_now_init();
  if (init_result != 0) {
    AddLog(LOG_LEVEL_INFO, PSTR("MSH: Node init failed with error: %d"), init_result);
    // try to re-launch wifi
    MESH.role = ROLE_NONE;
    MESHsetWifi(1);
    WifiBegin(3, MESH.channel);
    return;
  }

//  AddLog(LOG_LEVEL_INFO, PSTR("MSH: Node initialized, channel: %u"),wifi_get_channel()); //check if we succesfully set the
  Response_P(PSTR("{\"%s\":{\"Node\":1,\"Channel\":%u,\"Role\":%u}}"), D_CMND_MESH, wifi_get_channel(), _role);
  XdrvRulesProcess(0);

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(CB_MESHDataSent);
  esp_now_register_recv_cb(CB_MESHDataReceived);

  MESHsetKey(MESH.key);
  memcpy(MESH.sendPacket.receiver, MESH.broker, 6);
  WiFi.macAddress(MESH.sendPacket.sender);
  MESHaddPeer(MESH.broker); //must always be peer 0!! -return code -7 for peer list full
  MESHcountPeers();
  MESH.lastMessageFromBroker = millis();  // Init
  MESH.role = (0 == _role) ? ROLE_NODE_SMALL : ROLE_NODE_FULL;
  MESHsetSleep();
  MESHregisterNode(0);
#endif  // ESP8266
}

void MESHstartBroker(void) {       // Must be called after WiFi is initialized!! Rule - on system#boot do meshbroker endon
#ifdef ESP32
  WiFi.mode(WIFI_AP_STA);
  WiFi.AP.begin();
  AddLog(LOG_LEVEL_INFO, PSTR("MSH: Broker MAC %s"), WiFi.softAPmacAddress().c_str());
  WiFi.softAPmacAddress(MESH.broker); //set MESH.broker to the needed MAC
  uint32_t _channel = WiFi.channel();

  esp_now_deinit();  // in case it was already initialized by disconnected
  esp_err_t init_result = esp_now_init();
  if (esp_err_t() != ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR("MSH: Broker init failed with error: %s"), init_result);
    return;
  }

  Response_P(PSTR("{\"%s\":{\"Broker\":1,\"Channel\":%u}}"), D_CMND_MESH, _channel);
  XdrvRulesProcess(0);
//  AddLog(LOG_LEVEL_INFO, PSTR("MSH: Broker initialized on channel %u"), _channel);
  esp_now_register_send_cb(CB_MESHDataSent);
  esp_now_register_recv_cb(CB_MESHDataReceived);
  MESHsetKey(MESH.key);
  MESHcountPeers();
  memcpy(MESH.sendPacket.sender, MESH.broker, 6);
  MESH.role = ROLE_BROKER;
  MESHsetSleep();
#endif  // ESP32
}

/*********************************************************************************************\
 * Main loops
\*********************************************************************************************/

#ifdef ESP32

void MESHevery50MSecond(void) {
  // if (MESH.packetToResend.size() > 0) {
  //   // pass the packets
  // }
  if (MESH.packetToConsume.size() > 0) {
//    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: _ %15_H"), (uint8_t *)&MESH.packetToConsume.front());
    for (auto &_headerBytes : MESH.packetsAlreadyReceived) {
//      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: . %15_H"), (uint8_t *)_headerBytes.raw);
      if (memcmp(MESH.packetToConsume.front().sender, _headerBytes.raw, 15) == 0) {
        MESH.packetToConsume.pop();
        return;
      }
    }
    mesh_first_header_bytes _bytes;
    memcpy(_bytes.raw, &MESH.packetToConsume.front(), 15);
    MESH.packetsAlreadyReceived.push_back(_bytes);
//    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: ... %15_H"), (uint8_t *)_bytes.raw);
    if (MESH.packetsAlreadyReceived.size() > MESH_MAX_PACKETS) {
      MESH.packetsAlreadyReceived.erase(MESH.packetsAlreadyReceived.begin());
//      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Erase received data"));
    }

    // do something on the node
    // AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: %30_H), (uint8_t *)&MESH.packetToConsume.front());

#ifdef USE_TASMESH_HEARTBEAT
    for (auto &_peer : MESH.peers){
      if (memcmp(_peer.MAC, MESH.packetToConsume.front().sender, 6) == 0) {
        _peer.lastHeartbeatFromPeer = millis();

        if (!_peer.isAlive) {
          _peer.isAlive = true;
          char stopic[TOPSZ];
          GetTopic_P(stopic, TELE, _peer.topic, S_LWT);
          MqttPublishPayload(stopic, PSTR(MQTT_LWT_ONLINE));
        }
        break;
      }
    }
#endif // USE_TASMESH_HEARTBEAT

    MESHencryptPayload(&MESH.packetToConsume.front(), 0);
    switch (MESH.packetToConsume.front().type) {
      // case PACKET_TYPE_REGISTER_NODE:
      //   AddLog(LOG_LEVEL_INFO, PSTR("MSH: received topic: %s"), (char*)MESH.packetToConsume.front().payload + 6);
      //   // AddLog(LOG_LEVEL_INFO, PSTR("MSH: %*_H), MESH.packetToConsume.front().chunkSize+5, (uint8_t *)&MESH.packetToConsume.front().payload);
      //   for(auto &_peer : MESH.peers){
      //     if(memcmp(_peer.MAC,MESH.packetToConsume.front().sender,6)==0){
      //       strcpy(_peer.topic,(char*)MESH.packetToConsume.front().payload+6);
      //       MESHsubscribe((char*)&_peer.topic);
      //     }
      //   }
      //   break;
      case PACKET_TYPE_PEERLIST:
        for (uint32_t i = 0; i < MESH.packetToConsume.front().chunkSize; i += 6) {
          if (memcmp(MESH.packetToConsume.front().payload +i, MESH.sendPacket.sender, 6) == 0) {
            continue;              // Do not add myself
          }
          if (MESHcheckPeerList(MESH.packetToConsume.front().payload +i) == false) {
            MESHaddPeer(MESH.packetToConsume.front().payload +i);
          }
        }
        break;
      case  PACKET_TYPE_MQTT:      // Redirected MQTT from node in packet [char* _space_ char*]
//        AddLog(LOG_LEVEL_INFO, PSTR("MSH: Received node output '%s'"), (char*)MESH.packetToConsume.front().payload);
        if (MESH.packetToConsume.front().chunks > 1) {
          bool _foundMultiPacket = false;
          for (auto &_packet_combined : MESH.multiPackets) {
//            AddLog(LOG_LEVEL_INFO, PSTR("MSH: Append to multipacket"));
            if (memcmp(_packet_combined.header.sender, MESH.packetToConsume.front().sender, 12) == 0) {
              if (_packet_combined.header.counter == MESH.packetToConsume.front().counter) {
                memcpy(_packet_combined.raw + (MESH.packetToConsume.front().chunk * MESH_PAYLOAD_SIZE), MESH.packetToConsume.front().payload, MESH.packetToConsume.front().chunkSize);
                bitSet(_packet_combined.receivedChunks, MESH.packetToConsume.front().chunk);
                _foundMultiPacket = true;
//                AddLog(LOG_LEVEL_INFO, PSTR("MSH: Multipacket rcvd chunk mask 0x%08X"), _packet_combined.receivedChunks);
              }
            }
            uint32_t _temp = (1 << (uint8_t)MESH.packetToConsume.front().chunks) -1; //example: 1+2+4 == (2^3)-1
//            AddLog(LOG_LEVEL_INFO, PSTR("MSH: _temp: %u = %u"),_temp,_packet_combined.receivedChunks);
            if (_packet_combined.receivedChunks == _temp) {
              char * _data = (char*)_packet_combined.raw + strlen((char*)_packet_combined.raw) + 1;
//              AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Publish multipacket"));
              MqttPublishPayload((char*)_packet_combined.raw, _data);
              MESH.multiPackets.erase(MESH.multiPackets.begin());
              break;
            }
          }
          if (!_foundMultiPacket) {
            mesh_packet_combined_t _packet;
            memcpy(_packet.header.sender, MESH.packetToConsume.front().sender, sizeof(_packet.header));
            memcpy(_packet.raw + (MESH.packetToConsume.front().chunk * MESH_PAYLOAD_SIZE), MESH.packetToConsume.front().payload, MESH.packetToConsume.front().chunkSize);
            _packet.receivedChunks = 0;
            bitSet(_packet.receivedChunks, MESH.packetToConsume.front().chunk);
            MESH.multiPackets.push_back(_packet);
//            AddLog(LOG_LEVEL_INFO, PSTR("MSH: New multipacket with chunks %u"), _packet.header.chunks);
          }
        } else {
//          AddLog(LOG_LEVEL_INFO, PSTR("MSH: chunk: %u size: %u"), MESH.packetToConsume.front().chunk, MESH.packetToConsume.front().chunkSize);
//          if (MESH.packetToConsume.front().chunk==0) AddLog(LOG_LEVEL_INFO, PSTR("MSH: %*_H), MESH.packetToConsume.front().chunkSize, (uint8_t *)&MESH.packetToConsume.front().payload);
          char * _data = (char*)MESH.packetToConsume.front().payload + strlen((char*)MESH.packetToConsume.front().payload) +1;
//          AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Publish packet"));
          MqttPublishPayload((char*)MESH.packetToConsume.front().payload, _data);

          uint32_t idx = 0;
          for (auto &_peer : MESH.peers){
            if (memcmp(_peer.MAC, MESH.packetToConsume.front().sender, 6) == 0) {
              _peer.lastMessageFromPeer = millis();
              MESH.lastTeleMsgs[idx] = std::string(_data);
              break;
            }
            idx++;
          }
//          AddLog(LOG_LEVEL_INFO, PSTR("MSH: %*_H), MESH.packetToConsume.front().chunkSize, (uint8_t *)&MESH.packetToConsume.front().payload);
        }
        break;
#ifdef USE_TASMESH_HEARTBEAT
      case PACKET_TYPE_HEARTBEAT:
        break;
#endif  // USE_TASMESH_HEARTBEAT

      default:
        AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)&MESH.packetToConsume.front(), MESH.packetToConsume.front().chunkSize +5);
      break;
    }
    MESH.packetToConsume.pop();
//    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Consumed one packet %u"), (char*)MESH.packetToConsume.size());
  }
}

void MESHEverySecond(void) {
  static uint32_t _second = 0;
  _second++;
  // send a time packet every x seconds
  if (MESH.flags.nodeWantsTimeASAP) {
    MESHsendTime();
    MESH.flags.nodeWantsTimeASAP = 0;
    return;
  }
  if (_second % 5 == 0) {
    if ((MESH.flags.nodeWantsTime == 1) || (_second % 30 == 0)) { //every 5 seconds on demand or every 30 seconds anyway
      MESHsendTime();
      MESH.flags.nodeWantsTime = 0;
      return;
    }
  }
  uint32_t _peerNumber = _second%45;
  if (_peerNumber < MESH.peers.size()) {
    if (MESH.peers[_peerNumber].topic[0] == 0) {
      AddLog(LOG_LEVEL_INFO, PSTR("MSH: Broker wants topic from peer %u"), _peerNumber);
      MESHdemandTopic(_peerNumber);
    }
  }
  if (MESH.multiPackets.size() > 3) {
    AddLog(LOG_LEVEL_INFO, PSTR("MSH: Multi packets in buffer %u"), MESH.multiPackets.size());
    MESH.multiPackets.erase(MESH.multiPackets.begin());
  }

#ifdef USE_TASMESH_HEARTBEAT
  for (auto &_peer : MESH.peers){
    if (_peer.isAlive && TimePassedSince(_peer.lastHeartbeatFromPeer) > TASMESH_OFFLINE_DELAY * 1000) {
      _peer.isAlive = false;
      char stopic[TOPSZ];
      GetTopic_P(stopic, TELE, _peer.topic, S_LWT);
      MqttPublishPayload(stopic, PSTR(MQTT_LWT_OFFLINE));
    }
  }
#endif // USE_TASMESH_HEARTBEAT
}

#else  // ESP8266

void MESHevery50MSecond(void) {
  if (ROLE_NONE == MESH.role) { return; }

  if (MESH.packetToResend.size() > 0) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Next packet %d to resend of type %u, TTL %u"),
      MESH.packetToResend.size(), MESH.packetToResend.front().type, MESH.packetToResend.front().TTL);
    if (MESH.packetToResend.front().TTL > 0) {
      MESH.packetToResend.front().TTL--;
      if (memcmp(MESH.packetToResend.front().sender, MESH.broker, 6) != 0) { //do not send back the packet to the broker
        MESHsendPacket(&MESH.packetToResend.front());
      }
    } else {
      MESH.packetToResend.pop();
    }
    // pass the packets
  }

  if (MESH.packetToConsume.size() > 0) {
    MESHencryptPayload(&MESH.packetToConsume.front(), 0);
    switch (MESH.packetToConsume.front().type) {
      case PACKET_TYPE_MQTT:
        if (memcmp(MESH.packetToConsume.front().sender, MESH.sendPacket.sender, 6) == 0) {
          //discard echo
          break;
        }
        // AddLog(LOG_LEVEL_INFO, PSTR("MSH: node received topic: %s"), (char*)MESH.packetToConsume.front().payload);
        MESHreceiveMQTT(&MESH.packetToConsume.front());
        break;
      case PACKET_TYPE_PEERLIST:
        for (uint32_t i = 0; i < MESH.packetToConsume.front().chunkSize; i += 6) {
          if (memcmp(MESH.packetToConsume.front().payload +i, MESH.sendPacket.sender, 6) == 0) {
            continue; //do not add myself
          }
          if (MESHcheckPeerList(MESH.packetToConsume.front().payload +i) == false) {
            MESHaddPeer(MESH.packetToConsume.front().payload +i);
          }
        }
        break;
      default:
        break;
    }
    MESH.packetToConsume.pop();
  }
}

void MESHEverySecond(void) {
  if (MESH.role > ROLE_BROKER) {
    if (MESH.flags.brokerNeedsTopic == 1) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Broker wants topic"));
      MESHregisterNode(1); //refresh info
      MESH.flags.brokerNeedsTopic = 0;
    }
    if (millis() - MESH.lastMessageFromBroker > 31000) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Broker not seen for >30 secs"));
      MESHregisterNode(1); //refresh info
    }
    if (millis() - MESH.lastMessageFromBroker > 70000) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("MSH: Broker not seen for 70 secs, try to re-launch wifi"));
      MESH.role = ROLE_NONE;
      MESHdeInit();  // if we don't deinit after losing connection, we will get an error trying to reinit later
      MESHsetWifi(1);
      WifiBegin(3, MESH.channel);
    }

#ifdef USE_TASMESH_HEARTBEAT
    MESH.sendPacket.counter++;
    MESH.sendPacket.TTL = 2;
    MESH.sendPacket.chunks = 0;
    MESH.sendPacket.chunk = 0;
    MESH.sendPacket.chunkSize = 0;
    MESH.sendPacket.type = PACKET_TYPE_HEARTBEAT;
    MESHsendPacket(&MESH.sendPacket);
#endif // USE_TASMESH_HEARTBEAT
  }
}

#endif  // ESP8266

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

void MESHshow(bool json) {
  if (json) {
    if (ROLE_BROKER == MESH.role) {
      ResponseAppend_P(PSTR(",\"MESH\":{\"channel\":%u"), MESH.channel);
      ResponseAppend_P(PSTR(",\"nodes\":%u"),MESH.peers.size());
      if (MESH.peers.size() > 0) {
        ResponseAppend_P(PSTR(",\"MAC\":["));
        bool comma = false;
        for (auto &_peer : MESH.peers) {
          char _MAC[18];
          ToHex_P(_peer.MAC, 6, _MAC,18, ':');
          ResponseAppend_P(PSTR("%s\"%s\""), (comma)?",":"", _MAC);
          comma = true;
        }
        ResponseAppend_P(PSTR("]"));
      }
      ResponseJsonEnd();
    }
  } else {
#ifdef ESP32 //web UI only on the the broker = ESP32
#ifdef USE_WEBSERVER
    if (ROLE_BROKER == MESH.role) {
//      WSContentSend_PD(PSTR("TAS-MESH:<br>"));
      WSContentSend_PD(PSTR("<b>Broker MAC</b> %s<br>"), WiFi.softAPmacAddress().c_str());
      WSContentSend_PD(PSTR("<b>Broker Channel</b> %u<hr>"), WiFi.channel());
      uint32_t idx = 0;
      for (auto &_peer : MESH.peers) {
        char _MAC[18];
        ToHex_P(_peer.MAC, 6, _MAC, 18, ':');
        WSContentSend_PD(PSTR("<b>Node MAC</b> %s<br>"), _MAC);
        WSContentSend_PD(PSTR("<b>Node last message</b> %u ms<br>"), millis() - _peer.lastMessageFromPeer);
        WSContentSend_PD(PSTR("<b>Node MQTT topic</b> %s"), _peer.topic);
/*
        WSContentSend_PD(PSTR("Node MQTT topic: %s <br>"), _peer.topic);
        if (MESH.lastTeleMsgs.size() > idx) {
          char json_buffer[MESH.lastTeleMsgs[idx].length() +1];
          strcpy(json_buffer, (char*)MESH.lastTeleMsgs[idx].c_str());
          JsonParser parser(json_buffer);
          JsonParserObject root = parser.getRootObject();
          for (auto key : root) {
            JsonParserObject subObj = key.getValue().getObject();
            if (subObj) {
              WSContentSend_PD(PSTR("<ul>%s:"), key.getStr());
              for (auto subkey : subObj) {
                WSContentSend_PD(PSTR("<ul>%s: %s</ul>"), subkey.getStr(), subkey.getValue().getStr());
              }
              WSContentSend_PD(PSTR("</ul>"));
            } else {
              WSContentSend_PD(PSTR("<ul>%s: %s</ul>"), key.getStr(), key.getValue().getStr());
            }
          }
//          AddLog(LOG_LEVEL_INFO,PSTR("MSH: teleJSON %s"), (char*)MESH.lastTeleMsgs[idx].c_str());
//          AddLog(LOG_LEVEL_INFO,PSTR("MSH: stringsize: %u"),MESH.lastTeleMsgs[idx].length());
        } else {
//          AddLog(LOG_LEVEL_INFO,PSTR("MSH: telemsgSize: %u"),MESH.lastTeleMsgs.size());
        }
*/
        WSContentSend_PD(PSTR("<hr>"));
        idx++;
      }
    }
#endif  // USE_WEBSERVER
#endif  // ESP32
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kMeshCommands[] PROGMEM = "Mesh|"  // Prefix
  "Broker|Node|Peer|Channel|Interval";

void (* const MeshCommand[])(void) PROGMEM = {
  &CmndMeshBroker, &CmndMeshNode, &CmndMeshPeer, &CmndMeshChannel, &CmndMeshInterval };

void CmndMeshBroker(void) {
#ifdef ESP32  // only ESP32 currently supported as broker
  MESH.channel = WiFi.channel(); // The Broker gets the channel from the router, no need to declare it with MESHCHANNEL (will be mandatory set it when ETH will be implemented)
  MESHstartBroker();
  ResponseCmndNumber(MESH.channel);
#endif  // ESP32
}

void CmndMeshNode(void) {
#ifndef ESP32  // only ESP8266 current supported as node
  if (XdrvMailbox.data_len > 0) {
    MESHHexStringToBytes(XdrvMailbox.data, MESH.broker);
    if (XdrvMailbox.index != 0) { XdrvMailbox.index = 1; }    // Everything not 0 is a full node
    // meshnode FA:KE:AD:DR:ES:S1
    bool broker = false;
    char EspSsid[11];
    String mac_address = XdrvMailbox.data;
    snprintf_P(EspSsid, sizeof(EspSsid), PSTR("ESP_%s"), mac_address.substring(6).c_str());
    int32_t getWiFiChannel(const char *EspSsid);
    if (int32_t ch = WiFi.scanNetworks()) {
      for (uint8_t i = 0; i < ch; i++) {
        if (!strcmp(EspSsid, WiFi.SSID(i).c_str())) {
          MESH.channel = WiFi.channel(i);
          broker = true;
          AddLog(LOG_LEVEL_INFO, PSTR("MSH: Successfully connected to Mesh Broker using MAC %s as %s on channel %d"),
            XdrvMailbox.data, EspSsid, MESH.channel);
          MESHstartNode(MESH.channel, XdrvMailbox.index);
          ResponseCmndNumber(MESH.channel);
        }
      }
    }
    if (!broker) {
      AddLog(LOG_LEVEL_INFO, PSTR("MSH: No Mesh Broker found using MAC %s"), XdrvMailbox.data);
    }
  }
#endif  // ESP32
}

void CmndMeshPeer(void) {
  if (XdrvMailbox.data_len > 0) {
    uint8_t _MAC[6];
    MESHHexStringToBytes(XdrvMailbox.data, _MAC);
    char _peerMAC[18];
    ToHex_P(_MAC, 6, _peerMAC, 18, ':');
    AddLog(LOG_LEVEL_DEBUG,PSTR("MSH: MAC-string %s (%s)"), XdrvMailbox.data, _peerMAC);
    if (MESHcheckPeerList((const uint8_t *)_MAC) == false) {
      MESHaddPeer(_MAC);
      MESHcountPeers();
      ResponseCmndChar(_peerMAC);
    } else if (WiFiHelper::macAddress() == String(_peerMAC) || WiFi.softAPmacAddress() == String(_peerMAC)){
      // a device can be added as its own peer, but every send will result in a ESP_NOW_SEND_FAIL
      AddLog(LOG_LEVEL_DEBUG,PSTR("MSH: device %s cannot be a peer of itself"), XdrvMailbox.data, _peerMAC);
    } else {
      AddLog(LOG_LEVEL_DEBUG,PSTR("MSH: %s is already on peer list, will not add"), XdrvMailbox.data, _peerMAC);
    }
  }
}

void CmndMeshChannel(void) {
  if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 14)) {
    MESH.channel = XdrvMailbox.payload;
  }
  ResponseCmndNumber(MESH.channel);
}

void CmndMeshInterval(void) {
  if ((XdrvMailbox.payload > 1) && (XdrvMailbox.payload < 201)) {
    MESH.interval = XdrvMailbox.payload;   // 2 to 200 ms
    MESHsetSleep();
  }
  ResponseCmndNumber(MESH.interval);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv57(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kMeshCommands, MeshCommand);
      break;
    case FUNC_PRE_INIT:
      MESHInit();                  // TODO: save state
      break;
  }
  if (MESH.role) {
    switch (function) {
      case FUNC_LOOP:
        static uint32_t mesh_transceive_msecond = 0;             // State 50msecond timer
        if (TimeReached(mesh_transceive_msecond)) {
          SetNextTimeInterval(mesh_transceive_msecond, MESH.interval);
          MESHevery50MSecond();
        }
        break;
      case FUNC_EVERY_SECOND:
        MESHEverySecond();
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MESHshow(0);
        break;
#endif
      case FUNC_JSON_APPEND:
        MESHshow(1);
        break;
#ifdef ESP32
      case FUNC_MQTT_SUBSCRIBE:
        MESHconnectMQTT();
        break;
#endif  // ESP32
      case FUNC_SHOW_SENSOR:
        MESHsendPeerList();          // Sync this to the Teleperiod with a delay
        break;
#ifdef USE_DEEPSLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        MESHdeInit();
        break;
#endif  // USE_DEEPSLEEP
      case FUNC_ACTIVE:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_TASMESH
