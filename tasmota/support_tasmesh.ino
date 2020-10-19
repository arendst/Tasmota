/*
  support_tasmesh.ino - mesh via ESP-Now support for Sonoff-Tasmota

  Copyright (C) 2020  Theo Arends & Christian Baars

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
  Version Date      Action    Description
  --------------------------------------------------------------------------------------------
*/

#define USE_TASMESH
#ifdef USE_TASMESH

#include <queue>
#include <t_bearssl_block.h>

#ifdef ESP32
#include <esp_now.h>
#include <esp_wifi.h>
#else
#include <espnow.h> //ESP8266 ... why Espressif, why??
#endif //ESP32

#define MESH_PAYLOAD_SIZE 160 // default 180 - with header of 20 bytes, stays at 200 bytes, which is reported to work with ESP8266
#define MESH_TOPICSZ      64  // max supported topic size
#define MESH_BUFFERS      4   // max buffers number for splitted messages

struct mesh_packet_t{
  uint8_t sender[6];        //MAC
  uint8_t receiver[6];      //MAC
  uint32_t counter:4;       //endless counter to identify a packet
  uint32_t type:6;          //command,mqtt,...
  uint32_t chunks:6;        //number of chunks
  uint32_t chunk:6;         //chunk number
  uint32_t chunkSize:8;     //chunk size
  uint32_t TTL:2;           //time to live, counting down
  union{
    uint32_t senderTime;    //UTC-timestamp from every sender in the MESH
    uint32_t peerIndex;     //only for resending in the MESH
  };
  uint8_t tag[16];          //tag for de/encryption
  uint8_t payload[MESH_PAYLOAD_SIZE];
} __attribute__((packed));

struct mesh_packet_header_t{
  uint8_t sender[6];        //MAC
  uint8_t receiver[6];      //MAC
  uint32_t counter:4;       //endless counter to identify a packet
  uint32_t type:6;          //command,mqtt,...
  uint32_t chunks:6;        //number of chunks
  uint32_t chunk:6;         //chunk number
  uint32_t chunkSize:8;     //chunk size
  uint32_t TTL:2;           //time to live, counting down
  union{
    uint32_t senderTime;    //UTC-timestamp from every sender in the MESH
    uint32_t peerIndex;     //only for resending in the MESH
  };
  uint8_t tag[16];          //tag for de/encryption
} __attribute__((packed));

struct mesh_peer_t{
  uint8_t MAC[6];
  uint32_t lmfp; //time of last message from peer
#ifdef ESP32
  char topic[MESH_TOPICSZ];
#endif //ESP32
};

struct mesh_broker_flags_t{
  uint8_t brokerNeedsTopic:1;
};

struct mesh_packet_combined_t{
  mesh_packet_header_t header;
  uint8_t receivedChunks;
  char raw[MESH_PAYLOAD_SIZE*6];
};

struct{
  uint8_t broker[6];
  uint8_t role;
  uint8_t channel;  //Wifi channel
  uint8_t counter;  //for every message 
  uint32_t lmfb;    //time of last message from broker
  uint32_t lmfap;   //time of last message from any peer
  uint8_t pmk[32];
  mesh_broker_flags_t flags;
  mesh_packet_t sendPacket;
  std::vector<mesh_peer_t> peers;
  std::queue<mesh_packet_t> packetToResend;
  std::queue<mesh_packet_t> packetToConsume;
  std::vector<mesh_packet_header_t> packetsAlreadySended;
  std::vector<mesh_packet_combined_t> multiPackets;
}MESH;

/*********************************************************************************************\
 * declarations for functions with custom types
\*********************************************************************************************/

void MESHsendPacket(mesh_packet_t *_packet);
bool MESHencryptPayload(mesh_packet_t *_packet, int _encrypt); // 1 encryption; 0 decryption

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum MESH_Role {
  ROLE_NONE = 0,                                // not initialized
  ROLE_BROKER,                                  // ESP32 will connect mesh to WLAN
  ROLE_NODE_SMALL,                              // Node will only talk to the broker
  ROLE_NODE_FULL                                // Node will listen and resend every message for MESH functionality
};

enum MESH_Packet_Type {                         // Type of packet
  PACKET_TYPE_TIME = 0,                         // 
  PACKET_TYPE_SENSOR,                           //
  PACKET_TYPE_COMMAND,                          //
  PACKET_TYPE_TOPIC,                            // announce mqtt topic to ESP32-proxy
  PACKET_TYPE_MQTT                              // 
}; 

/*********************************************************************************************\
 * 
\*********************************************************************************************/
#ifdef ESP32
void MESHsendTime(uint32_t _peerNumber){ //only from broker to nodes
  MESH.sendPacket.counter++;
  MESH.sendPacket.type = PACKET_TYPE_TIME;
  MESH.sendPacket.TTL = 3;
  memcpy(MESH.sendPacket.receiver,MESH.peers[_peerNumber].MAC,6);
  MESH.sendPacket.senderTime = Rtc.utc_time;
  MESH.sendPacket.payload[0] = 0;
  mesh_broker_flags_t *_flags = (mesh_broker_flags_t *)MESH.sendPacket.payload;
  if(MESH.peers[_peerNumber].topic[0]==0){
    AddLog_P2(LOG_LEVEL_INFO, PSTR("MESH: broker wants topic from peer: %u"), _peerNumber);
    _flags->brokerNeedsTopic = 1;
  }
  MESH.sendPacket.chunkSize = 0;
  MESHsendPacket(&MESH.sendPacket);
  // esp_now_send(MESH.sendPacket.receiver, (uint8_t *)&MESH.sendPacket, sizeof(MESH.sendPacket)-MESH_PAYLOAD_SIZE+1);
}
#endif //ESP32

void MESHcheckPeerList(const uint8_t *MAC){
  for(auto &_peer : MESH.peers){
    if(memcmp(_peer.MAC,MAC,6)==0){
      _peer.lmfp = millis();
      return;
    }
  }
  MESHaddPeer((uint8_t *)MAC);
}


void MESHcountPeers(void){
  #ifdef ESP32
  esp_now_peer_num_t _num;
  esp_now_get_peer_num(&_num);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TAS-MESH peers: %u"),_num.total_num);
#else
  uint8_t _num;
  uint8_t _numEnc;
  esp_now_get_cnt_info(&_num,&_numEnc);
  AddLog_P2(LOG_LEVEL_INFO, PSTR("TAS-MESH peers: %u"),_num);
#endif
}


void MESHaddPeer(uint8_t *_MAC ){
  mesh_peer_t _newPeer;
  memcpy(_newPeer.MAC,_MAC,6);
  _newPeer.lmfp = millis();
#ifdef ESP32
  _newPeer.topic[0] = 0;
#endif
  MESH.peers.push_back(_newPeer);
  int err;
#ifdef ESP32
  esp_now_peer_info_t _peer;
  _peer.channel = WiFi.channel();
  _peer.encrypt = false;
  _peer.ifidx = ESP_IF_WIFI_AP;
  memcpy(_peer.peer_addr, _MAC, 6);
  err = esp_now_add_peer(&_peer);
#else
  err = esp_now_add_peer(_MAC, ESP_NOW_ROLE_COMBO, MESH.channel, NULL, 0);
#endif
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: peer added, err: %d"), err);
  AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t *)_MAC,6);
#ifdef ESP32
  if(MESH.role == ROLE_BROKER) MESHsendTime(MESH.peers.size()-1);
#endif //ESP32
}

//helper functions
void MESHstripColon(char* _string){
  uint32_t _length = strlen(_string);
  uint32_t _index = 0;
  while (_index < _length) {
    char c = _string[_index];
    if(c==':'){
      memmove(_string+_index,_string+_index+1,_length-_index);
    }
    _index++;
  }
  _string[_index] = 0;
}

void MESHMACStringToBytes(char* _string,uint8_t _MAC[]) { //uppercase
  uint32_t index = 0;
  uint32_t _end = 12;
  while (index < _end) {
      char c = _string[index];
      uint8_t value = 0;
      if(c >= '0' && c <= '9')
        value = (c - '0');
      else if (c >= 'A' && c <= 'F')
        value = (10 + (c - 'A'));
      _MAC[(index/2)] += value << (((index + 1) % 2) * 4);
      index++;
  }
}

void MESHsendPacket(mesh_packet_t *_packet){
  MESHencryptPayload(_packet,1);
  esp_now_send(_packet->receiver, (uint8_t *)_packet, sizeof(MESH.sendPacket) - MESH_PAYLOAD_SIZE + _packet->chunkSize);
}

void MESHsetPMK(uint8_t* _key){ // must be 32 bytes!!!
  char* _pw = SettingsText(SET_STAPWD1 + Settings.sta_active);
  size_t _length = strlen(_pw);
  memset(_key,0,32);
  if(_length>32) _length = 32;
  memcpy(_key,_pw,_length);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: set crypto key to PASSWORD1"));
}


bool MESHencryptPayload(mesh_packet_t *_packet, int _encrypt){

// AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: will encrypt: %u"), _encrypt);

size_t _size = _packet->chunkSize;
char _tag[16];

// AddLog_P2(LOG_LEVEL_DEBUG, PSTR("cc: %u, _size: %u"), _counter,_size);
// AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)_tag,16);

br_chacha20_run bc = br_chacha20_ct_run;

br_poly1305_ctmul32_run((void*)MESH.pmk, (const void *)_packet,
(void *)_packet->payload, _size, (void*)&_packet->senderTime, 4,
_tag, bc, _encrypt);

// AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: encryption done "));

if(_encrypt==1){
  memcpy(_packet->tag,_tag,16);
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: payload encrypted"));
  return true;
}
if(memcmp(_packet->tag,_tag,16)==0){
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: payload decrypted"));
  return true;
}
AddLog_P2(LOG_LEVEL_DEBUG, PSTR("MESH: payload decryption error"));
return false;
}

#endif //USE_TASMESH
