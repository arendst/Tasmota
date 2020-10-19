/*
  xdrv_44_tasmesh.ino - Mesh support for Tasmota using ESP-Now

  Copyright (C) 2020  Christian Baars and Theo Arends

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

  --------------------------------------------------------------------------------------------
  Version yyyymmdd  Action    Description
  --------------------------------------------------------------------------------------------
  0.9.0.0 20200927  started - from scratch

*/


#ifdef USE_TASMESH

/*********************************************************************************************\
* Build a mesh of nodes using ESP-Now
* Connect it through an ESP32-broker to WLAN
\*********************************************************************************************/

#define XDRV_44             44

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MESH "MESH"

const char S_JSON_MESH_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_MESH "%s\":%d}";
const char S_JSON_MESH_COMMAND[] PROGMEM        = "{\"" D_CMND_MESH "%s\"}";
const char kMESH_Commands[] PROGMEM             = "Broker|Node|Peer|Channel";

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum MESH_Commands {                            // commands useable in console or rules
  CMND_MESH_BROKER,                             // start broker on ESP32
  CMND_MESH_NODE,                               // start node and connect to broker based on MAC address
  CMND_MESH_PEER,                               // add node to peer list of a broker or node
  CMND_MESH_CHANNEL};                           // set wifi channel on node

/*********************************************************************************************\
 * Callbacks
\*********************************************************************************************/

#ifdef ESP32
void CB_MESHDataSent(const uint8_t *MAC,  esp_now_send_status_t sendStatus);
void CB_MESHDataSent(const uint8_t *MAC,  esp_now_send_status_t sendStatus) {
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("BROKER sended packet"));
}

void CB_MESHDataReceived(const uint8_t *MAC, const uint8_t *packet, int len) {
  MESH.lmfap = millis();
  MESHcheckPeerList((const uint8_t *)MAC);
  mesh_packet_t *_recvPacket = (mesh_packet_t*)packet;
  MESH.packetToConsume.push(*_recvPacket);
}
#else //ESP8266
void CB_MESHDataSent( uint8_t *MAC, uint8_t sendStatus) {
}

void CB_MESHDataReceived(uint8_t *MAC, uint8_t *packet, uint8_t len) {
      MESH.lmfap = millis(); //any peer
      if(memcmp(MAC,MESH.broker,6)==0) MESH.lmfb = millis(); //directly from the broker
      mesh_packet_t *_recvPacket = (mesh_packet_t*)packet;
      if(memcmp(_recvPacket->receiver,MESH.sendPacket.sender,6)!=0){ //MESH.sendPacket.sender simply stores the MAC of the node
        //pass packet back to the MESH
        _recvPacket->peerIndex = 0;
        MESH.packetToResend.push(*_recvPacket);
        return;
      }
      switch(_recvPacket->type){
        case PACKET_TYPE_TIME:
          Rtc.utc_time = _recvPacket->senderTime;
          Rtc.user_time_entry = true;
          memcpy((uint8_t*)&MESH.flags,_recvPacket->payload,1);
          break;
        default:
          MESH.packetToConsume.push(*_recvPacket);
          break;
      }
}
#endif //ESP32

/*********************************************************************************************\
 * init driver
\*********************************************************************************************/

void MESHInit(void) {
  MESH.role == ROLE_NONE;
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TAS-MESH initialized: %u"),Settings.tele_period);

  MESH.sendPacket.counter = 0;
  MESH.sendPacket.chunks = 1;
  MESH.sendPacket.chunk = 0;
  MESH.sendPacket.type = PACKET_TYPE_TIME;
  MESH.sendPacket.TTL = 2;
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
void MESHsubscribe(char *topic){
  char stopic[TOPSZ];
  GetTopic_P(stopic, CMND, topic, PSTR("#"));
  MqttSubscribe(stopic);
}

void MESHunsubscribe(char *topic){
  char stopic[TOPSZ];
  GetTopic_P(stopic, CMND, topic, PSTR("#"));
  MqttUnsubscribe(stopic);
}

void MESHconnectMQTT(void){ 
  for(auto &_peer : MESH.peers){
    AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: reconnect topic: %s"),_peer.topic);
    if(_peer.topic[0]!=0){
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
bool MESHinterceptMQTT(char* _topic, uint8_t* _data, unsigned int data_len){
  char stopic[TOPSZ];
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: Intercept topic: %s"),_topic);
  for(auto &_peer : MESH.peers){
    GetTopic_P(stopic, CMND, _peer.topic, PSTR("")); //cmnd/topic/
    if(strlen(_topic)!= strlen(_topic)) return false; // prevent false result when _topic is the leading substring of stopic
    if(memcmp(_topic, stopic,strlen(stopic)) == 0){
      MESH.sendPacket.chunkSize = strlen(_topic)+1;
      memcpy(MESH.sendPacket.payload,_topic,MESH.sendPacket.chunkSize);
      memcpy(MESH.sendPacket.payload+MESH.sendPacket.chunkSize,_data,data_len);
      MESH.sendPacket.chunkSize += data_len;
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: Intercept payload: %s"),MESH.sendPacket.payload);
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

#else //ESP8266
void MESHreceiveMQTT(mesh_packet_t *_packet);
void MESHreceiveMQTT(mesh_packet_t *_packet){
  uint32_t _slength = strlen((char*)_packet->payload);
  if(_packet->chunks==1){ //single chunk message
    MqttDataHandler((char*)_packet->payload, (uint8_t*)(_packet->payload)+_slength+1, (_packet->chunkSize)-_slength);
  }
  else{
    AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: multiple chunks: %u"),_packet->chunks);
    // TODO: reconstruct message in buffer
  }
}
#endif //ESP32

/**
 * @brief Redirects the mqtt message on the node just before it would have been sended to 
 *        the broker via ESP-NOW
 * 
 * @param _topic 
 * @param _data 
 * @param _retained - currently unused
 * @return true 
 * @return false 
 */
bool MESHredirectMQTT(const char* _topic, char* _data, bool _retained){
  size_t _bytesLeft = strlen(_topic)+strlen(_data)+2;
  MESH.sendPacket.counter++;
  MESH.sendPacket.chunk = 0;
  MESH.sendPacket.chunks = ((_bytesLeft+2)/MESH_PAYLOAD_SIZE)+1;
  memcpy(MESH.sendPacket.receiver,MESH.broker,6);
  MESH.sendPacket.type = PACKET_TYPE_MQTT;
  MESH.sendPacket.chunkSize = MESH_PAYLOAD_SIZE;
  MESH.sendPacket.peerIndex = 0;
  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: chunks: %u, counter: %u"),MESH.sendPacket.chunks,MESH.sendPacket.counter);
  size_t _topicSize = strlen(_topic)+1;
  size_t _offsetData = 0;
  while(_bytesLeft>0){
    size_t _byteLeftInChunk = MESH_PAYLOAD_SIZE;
    // MESH.sendPacket.chunkSize = MESH_PAYLOAD_SIZE;
    if(MESH.sendPacket.chunk == 0){
      memcpy(MESH.sendPacket.payload,_topic,_topicSize);
      MESH.sendPacket.chunkSize = _topicSize;
      _bytesLeft -= _topicSize;
      _byteLeftInChunk -= _topicSize;
      // AddLog_P2(LOG_LEVEL_INFO, PSTR("topic in payload %s"),(char*)MESH.sendPacket.payload);
      // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: after topic -> chunk:%u, pre-size: %u"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize);
    }
    if(_byteLeftInChunk>0){
      if(_byteLeftInChunk>_bytesLeft){
      //  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: only last chunk bL:%u bLiC:%u oSD:%u"),_bytesLeft,_byteLeftInChunk,_offsetData);
        _byteLeftInChunk = _bytesLeft;
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: only last chunk after correction -> chunk:%u, pre-size: %u"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize);
      }
      if(MESH.sendPacket.chunk>0) _topicSize = 0;
      // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: %u"),_offsetPayload);
      memcpy(MESH.sendPacket.payload + _topicSize, _data + _offsetData,_byteLeftInChunk);
      // AddLog_P2(LOG_LEVEL_INFO, PSTR("data in payload %s"),(char*)MESH.sendPacket.payload + _offsetPayload);
      _offsetData += _byteLeftInChunk;
      _bytesLeft -= _byteLeftInChunk;
    }
    MESH.sendPacket.chunkSize += _byteLeftInChunk;
    MESH.packetToResend.push(MESH.sendPacket);
    // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: chunk:%u, size: %u"),MESH.sendPacket.chunk,MESH.sendPacket.chunkSize);
    // AddLogBuffer(LOG_LEVEL_INFO, (uint8_t*)MESH.sendPacket.payload, MESH.sendPacket.chunkSize);

    if(MESH.sendPacket.chunk==MESH.sendPacket.chunks){
      // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: too many chunks: %u"),MESH.sendPacket.chunk+1);
    }
    MESH.sendPacket.chunk++;
    MESH.sendPacket.chunkSize = 0;
  }
  //send to pipeline
  return true;
}

/**
 * @brief The node sends its mqtt topic to the broker
 * 
 */
void MESHanounceTopic(){
  memset(MESH.sendPacket.payload,0,MESH_PAYLOAD_SIZE);
  strcpy((char*)MESH.sendPacket.payload,mqtt_topic);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: topic: %s"),(char*)MESH.sendPacket.payload);
  MESH.sendPacket.chunkSize = strlen((char*)MESH.sendPacket.payload) + 1;
  MESH.sendPacket.type = PACKET_TYPE_TOPIC;
  MESHsendPacket(&MESH.sendPacket);
  // int result = esp_now_send(MESH.sendPacket.receiver, (uint8_t *)&MESH.sendPacket, (sizeof(MESH.sendPacket))-(MESH_PAYLOAD_SIZE-MESH.sendPacket.chunkSize-1));
}

/*********************************************************************************************\
 * generic functions
\*********************************************************************************************/

void MESHstartNode(int32_t _channel){
  MESH.channel = _channel;
  WiFi.mode(WIFI_STA);
  WiFi.begin("","",MESH.channel, nullptr, false); //fake connection attempt to set channel
  WiFi.disconnect();
  if (esp_now_init() != 0) {
    return;
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: Node initialized, channel: %u"),MESH.channel);
#ifdef ESP8266
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
#endif //ESP8266

  esp_now_register_send_cb(CB_MESHDataSent);
  esp_now_register_recv_cb(CB_MESHDataReceived);
  MESHsetPMK(MESH.pmk);
  memcpy(MESH.sendPacket.receiver,MESH.broker,6);
  WiFi.macAddress(MESH.sendPacket.sender);
  MESHaddPeer(MESH.broker); //must always be peer 0!!
  MESHcountPeers();
  MESH.role = ROLE_NODE_SMALL;
  MESHanounceTopic();
}

void MESHstartBroker(){
#ifdef ESP32
  WiFi.mode(WIFI_AP_STA);
  // WiFi.softAP("SSID_NOW","PASSWORD_NOW",9,1);
  // AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)WiFi.softAPmacAddress(),6);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: Broker MAC: %s"),WiFi.softAPmacAddress().c_str());
  WiFi.softAPmacAddress(MESH.broker);

  uint32_t _channel = WiFi.channel();
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  if (esp_now_init() != 0) {
    return;
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: Broker initialized on channel: %u"), _channel);
  esp_now_register_send_cb(CB_MESHDataSent);
  esp_now_register_recv_cb(CB_MESHDataReceived);
  MESHsetPMK(MESH.pmk);
  MESHcountPeers();
  memcpy(MESH.sendPacket.sender,MESH.broker,6);
  MESH.role = ROLE_BROKER;
#endif //ESP32
}

/*********************************************************************************************\
 * main loops
\*********************************************************************************************/
#ifdef ESP32

void MESHevery50MSecond(){
  // if(MESH.packetToResend.size()>0){
  //   // pass the packets
  // }
  if(MESH.packetToConsume.size()>0){
    // do something on the node
    MESHencryptPayload(&MESH.packetToConsume.front(),0);
    switch(MESH.packetToConsume.front().type){
      case PACKET_TYPE_TOPIC:
        AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: received topic: %s"), (char*)MESH.packetToConsume.front().payload);
        AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)&MESH.packetToConsume.front().payload,MESH.packetToConsume.front().chunkSize+5);
        for(auto &_peer : MESH.peers){
          if(memcmp(_peer.MAC,MESH.packetToConsume.front().sender,6)==0){
            strcpy(_peer.topic,(char*)MESH.packetToConsume.front().payload);
            MESHsubscribe((char*)&_peer.topic);
          }
        }
        break;
      // case  PACKET_TYPE_SENSOR:
      //   for(auto &_peer : MESH.peers){
      //     if(memcmp(_peer.MAC,MESH.packetToConsume.front().sender,6)==0){
      //       // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: received sensor output: %s"), (char*)MESH.packetToConsume.front().payload);
      //       char stopic[MESH_TOPICSZ];
      //       GetTopic_P(stopic, TELE, _peer.topic, PSTR("SENSOR"));
      //       MqttClient.publish(stopic, (char*)MESH.packetToConsume.front().payload);
      //       AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: topic: %s output: %s"), stopic, (char*)MESH.packetToConsume.front().payload);
      //       yield();  // #3313
      //       break;
      //     }
      //   }
      case  PACKET_TYPE_MQTT: // redirected MQTT from node in packet [char* _space_ char*]
        {
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: received node output: %s"), (char*)MESH.packetToConsume.front().payload);
        if(MESH.packetToConsume.front().chunks>1){
          bool _foundMultiPacket = false;
          for(auto &_packet_combined : MESH.multiPackets){
            // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: append to multipacket"));
            if(memcmp(_packet_combined.header.sender,MESH.packetToConsume.front().sender,12)==0){
              if(_packet_combined.header.counter == MESH.packetToConsume.front().counter){
                memcpy(_packet_combined.raw+(MESH.packetToConsume.front().chunk * MESH_PAYLOAD_SIZE),MESH.packetToConsume.front().payload,MESH.packetToConsume.front().chunkSize);
                bitSet(_packet_combined.receivedChunks,MESH.packetToConsume.front().chunk);
                _foundMultiPacket = true;
                // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: recChunks= %u"),_packet_combined.receivedChunks);
              }
            }
            uint32_t _temp = (1 << (uint8_t)MESH.packetToConsume.front().chunks)-1 ; //example: 1+2+4 == (2^3)-1
            // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: _temp: %u = %u"),_temp,_packet_combined.receivedChunks);
            if(_packet_combined.receivedChunks==_temp){
              char * _data = (char*)_packet_combined.raw + strlen((char*)_packet_combined.raw) + 1;
              MqttClient.publish((char*)_packet_combined.raw, _data);
              AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: combined done: %s = %s"),(char*)_packet_combined.raw,_data);
              // AddLogBuffer(LOG_LEVEL_INFO,(uint8_t*)_packet_combined.raw,50);
            }
          }
          if(!_foundMultiPacket){
            mesh_packet_combined_t _packet;
            memcpy(_packet.header.sender,MESH.packetToConsume.front().sender,sizeof(_packet.header));
            memcpy(_packet.raw+(MESH.packetToConsume.front().chunk*MESH_PAYLOAD_SIZE),MESH.packetToConsume.front().payload,MESH.packetToConsume.front().chunkSize);
            _packet.receivedChunks = 0;
            bitSet(_packet.receivedChunks,MESH.packetToConsume.front().chunk);
            MESH.multiPackets.push_back(_packet);
            // AddLog_P2(LOG_LEVEL_INFO, PSTR("new multipacket with chunks: %u"),_packet.header.chunks);
          }
          // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: no support yet for multiple chunks: %u"),MESH.packetToConsume.front().chunks);
          break;
        }
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: chunk: %u size: %u"), MESH.packetToConsume.front().chunk, MESH.packetToConsume.front().chunkSize);
        // if (MESH.packetToConsume.front().chunk==0) AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)&MESH.packetToConsume.front().payload,MESH.packetToConsume.front().chunkSize);
        char * _data = (char*)MESH.packetToConsume.front().payload + strlen((char*)MESH.packetToConsume.front().payload)+1;
        MqttClient.publish((char*)MESH.packetToConsume.front().payload, _data);
        // AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: topic: %s output: %s"), (char*)MESH.packetToConsume.front().payload, _data);
        // AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)&MESH.packetToConsume.front().payload,MESH.packetToConsume.front().chunkSize);
        yield();  // #3313
        }
        break;
      default:
        AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)&MESH.packetToConsume.front(),MESH.packetToConsume.front().chunkSize+5);
      break;
    }
    MESH.packetToConsume.pop();
  }
}

void MESHEverySecond(){
  static uint32_t _second = 0;
    _second++;
  // send a time packet every x seconds
  uint32_t _peerNumber = _second%60;
  if(_peerNumber<MESH.peers.size()) {
    MESHsendTime(_peerNumber);
    return;
  }
  if(MESH.multiPackets.size()>3){
    AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: multi packets in buffer: %u"),MESH.multiPackets.size());
    MESH.multiPackets.erase(MESH.multiPackets.begin());
  }
}

#else //ESP8266
void MESHevery50MSecond(){
  if(MESH.packetToResend.size()>0){
  uint32_t _tempIndex = MESH.packetToResend.front().peerIndex;
    if(MESH.peers.size()>_tempIndex){
      MESH.packetToResend.front().senderTime = Rtc.utc_time;
      if (MESH.packetToResend.front().TTL>0){
        MESH.packetToResend.front().TTL--;
        if(memcmp(MESH.packetToResend.front().sender,MESH.broker,6) != 0){ //do not send back the packet to the broker TODO: guarantee that peer[0] is always the broker
          MESHsendPacket(&MESH.packetToResend.front());
          // int result = esp_now_send(MESH.packetToResend.front().receiver, (uint8_t *)&MESH.packetToResend.front(), (sizeof(MESH.sendPacket))-(MESH_PAYLOAD_SIZE-MESH.packetToResend.front().chunkSize));
        }
      }
      MESH.packetToResend.front().peerIndex = _tempIndex + 1;
    }
    else{
      MESH.packetToResend.pop();
    }
    // pass the packets
  }

  if(MESH.packetToConsume.size()>0){
    MESHencryptPayload(&MESH.packetToConsume.front(),0);
    switch(MESH.packetToConsume.front().type){
      case PACKET_TYPE_MQTT:
        if(memcmp(MESH.packetToConsume.front().sender,MESH.sendPacket.sender,6)==0){
          //discard echo
          break;
        }
        AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: node received topic: %s"), (char*)MESH.packetToConsume.front().payload);
        MESHreceiveMQTT(&MESH.packetToConsume.front());
        break;
      default:
        break;
    }
    MESH.packetToConsume.pop();
  }
}


void MESHEverySecond(){
  static uint32_t _second = 0;
  static uint32_t _tele_period = Settings.tele_period;
  if (MESH.role == ROLE_NODE_SMALL){
    _tele_period--;
    if(_tele_period == 0){
      // uint8_t broadcastAddress[6] = {0x30,0xAE,0xA4,0x26,0xE7,0x29};
      // memcpy(MESH.sendPacket.receiver,MESH.broker,6);
      // _tele_period = Settings.tele_period;
      // mqtt_data[0] = 0;
      
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("SENSOR: %s         %u"), mqtt_data, strlen(mqtt_data));
      // MESH.sendPacket.chunkSize = strlen(mqtt_data);
      // memcpy(MESH.sendPacket.payload,mqtt_data,MESH.sendPacket.chunkSize);
      // MESH.sendPacket.type = PACKET_TYPE_SENSOR;
      // MESH.sendPacket.senderTime = Rtc.utc_time;
      // int result = esp_now_send(MESH.sendPacket.receiver, (uint8_t *)&MESH.sendPacket, (sizeof(MESH.sendPacket))-(MESH_PAYLOAD_SIZE-MESH.sendPacket.chunkSize));
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("send error: %d, tele: %u"), result, tele_period);
    }
    if(MESH.flags.brokerNeedsTopic == 1){
      MESHanounceTopic();
      MESH.flags.brokerNeedsTopic = 0;
    }
  }
}
#endif //ESP8266

/*********************************************************************************************\
 * presentation
\*********************************************************************************************/
void MESHshow(bool json){
  if (json) {
  if(MESH.role != ROLE_NONE){
    if(MESH.role != ROLE_BROKER) ResponseAppend_P(PSTR(",\"MESH\":{\"broker\":%u"),MESH.channel);
    else ResponseAppend_P(PSTR(",\"MESH\":{\"node\":%u"),MESH.channel);
    ResponseJsonEnd();
    }
  } else {
#ifdef ESP32
    if(MESH.role == ROLE_BROKER){
      WSContentSend_PD(PSTR("TAS-MESH:<br>"));
      WSContentSend_PD(PSTR("Broker MAC: %s <br>"),WiFi.softAPmacAddress().c_str());
      WSContentSend_PD(PSTR("Broker Channel: %u <hr>"),WiFi.channel());
      for(auto &_peer : MESH.peers){
        char _MAC[18];
        ToHex_P(_peer.MAC,6,_MAC,18,':');
        WSContentSend_PD(PSTR("Node MAC: %s <br>"),_MAC);
        WSContentSend_PD(PSTR("Node last message: %u <br>"),_peer.lmfp);
        WSContentSend_PD(PSTR("Node MQTT topic: %s <hr>"),_peer.topic);
      }
    }
#endif //ESP32
  }
}


/*********************************************************************************************\
 * check the MESH commands
\*********************************************************************************************/

bool MESHCmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_MESH);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_MESH), disp_len)) {  // prefix
    int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kMESH_Commands);

    switch (command_code) {
      case CMND_MESH_BROKER:
        MESHstartBroker();
        Response_P(S_JSON_MESH_COMMAND_NVALUE, command, MESH.channel);
        break;
      case CMND_MESH_NODE:
        if (XdrvMailbox.data_len > 0) {
          MESHstripColon(XdrvMailbox.data);
          MESHMACStringToBytes(XdrvMailbox.data,MESH.broker);
          MESHstartNode(MESH.channel);
          Response_P(S_JSON_MESH_COMMAND_NVALUE, command, MESH.channel);
        }
        break;
      case CMND_MESH_CHANNEL:
        if (XdrvMailbox.data_len > 0) {
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("channel: %u"), XdrvMailbox.payload);
          MESH.channel = XdrvMailbox.payload;
        }
        break;
      case CMND_MESH_PEER:
        if (XdrvMailbox.data_len > 0) {
          uint8_t _MAC[6] = {0};
          MESHstripColon(XdrvMailbox.data);
          MESHMACStringToBytes(XdrvMailbox.data,_MAC);
          // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("MAC-string: %s"), XdrvMailbox.data);
          // AddLogBuffer(LOG_LEVEL_INFO,(uint8_t *)_MAC,6);
          MESHaddPeer(_MAC);
          MESHcountPeers();
        }
        break;
      default:
    	  // else for Unknown command
    	  serviced = false;
    	break;
    }
  } else {
    return false;
  }
  return serviced;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv44(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_PRE_INIT:
      MESHInit();                              // TODO: save state
      break;
    case FUNC_EVERY_50_MSECOND:
      MESHevery50MSecond();
      break;
    case FUNC_EVERY_SECOND:
      MESHEverySecond();
      break;
    case FUNC_COMMAND:
      result = MESHCmd();
      break;
    case FUNC_WEB_SENSOR:
#ifdef USE_WEBSERVER
      MESHshow(0);
#endif
      break;
    case FUNC_JSON_APPEND:
      MESHshow(1);
      break;
#ifdef ESP32
    case FUNC_MQTT_SUBSCRIBE:
      MESHconnectMQTT();
      break;
#endif //ESP32
    // case FUNC_SHOW_SENSOR:
    //   if(MESH.role == ROLE_NODE_SMALL) MESHsendTeleSensor();
    //   break;
  }
return result;
}

#endif  // USE_TASMESH
