/*
  xsns_62_esp32_mi.ino - MI-BLE-sensors via ESP32 support for Tasmota
  enabled by ESP32 && !USE_BLE_ESP32
  if (ESP32 && USE_BLE_ESP32) then xsns_62_esp32_mi_ble.ino is used

  Copyright (C) 2021  Christian Baars and Theo Arends

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
  0.9.5.6 20221006  changed - remove old HASS code, allow adding unknown sensors, prepare YLAI003
  -------
  0.9.5.5 20220326  changed - refactored connection task for asynchronous op, add response option,
                              fixed MI32Key command
  -------
  0.9.5.4 20220325  changed - add Berry adv_watch and adv_block to BLE class
  -------
  0.9.5.3 20220315  changed - reworked Berry part, active scanning and holding active connections possible, new format of advertisement buffer
  -------
  0.9.5.1 20220209  changed - rename YEERC to YLYK01, add dimmer YLKG08 (incl. YLKG07), change button report scheme
  -------
  0.9.5.0 20211016  changed - major rewrite, added mi32cfg (file and command), Homekit-Bridge,
                              extended GUI,
                              removed BLOCK, PERIOD, TIME, UNIT, BATTERY and PAGE -> replaced via Berry-Support
  -------
  0.9.1.7 20201116  changed - small bugfixes, add BLOCK and OPTION command, send BLE scan via MQTT
  -------
  0.9.1.0 20200712  changed - add lights and YLYK01, add pure passive mode with decryption,
                              lots of refactoring
  -------
  0.9.0.1 20200706  changed - adapt to new NimBLE-API, tweak scan process
  -------
  0.9.0.0 20200413  started - initial development by Christian Baars
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/
#ifndef USE_BLE_ESP32
#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support
#if defined CONFIG_IDF_TARGET_ESP32 || defined CONFIG_IDF_TARGET_ESP32C3 || defined CONFIG_IDF_TARGET_ESP32C2 || defined CONFIG_IDF_TARGET_ESP32C6 || defined CONFIG_IDF_TARGET_ESP32S3

#ifdef USE_MI_ESP32

#ifdef USE_ENERGY_SENSOR
// #define USE_MI_ESP32_ENERGY //prepare for some GUI extensions
#endif

#define XSNS_62                    62

#include <vector>
#include "freertos/ringbuf.h"

#include <t_bearssl.h>

#include "include/xsns_62_esp32_mi.h"

void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
void MI32AddKey(mi_bindKey_t keyMAC);
void MI32HandleEveryDevice(NimBLEAdvertisedDevice* advertisedDevice, uint8_t addr[6], int RSSI);

std::vector<mi_sensor_t> MIBLEsensors;
RingbufHandle_t BLERingBufferQueue = nullptr;

static BLEScan* MI32Scan;
static NimBLEClient* MI32Client;

/*********************************************************************************************\
 * BLE Callback Classes
\*********************************************************************************************/

class MI32SensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pclient) {
    // AddLog(LOG_LEVEL_DEBUG,PSTR("connected %s"), MI32getDeviceName(MI32.conCtx->slot));
    MI32.infoMsg = MI32_DID_CONNECT;
    MI32.mode.willConnect = 0;
    MI32.mode.connected = 1;
    pclient->updateConnParams(8,11,0,1000);
  }
  void onDisconnect(NimBLEClient* pclient, int reason) {
    MI32.mode.connected = 0;
    MI32.infoMsg = MI32_DID_DISCONNECT;
    MI32.conCtx->error = reason;
    MI32.conCtx->operation = 5; //set for all disconnects that come from the remote device or connection loss
    MI32.mode.triggerBerryConnCB = 1;
    //AddLog(LOG_LEVEL_DEBUG,PSTR("disconnected"));
  }
};

class MI32AdvCallbacks: public NimBLEScanCallbacks {
  void onScanEnd(NimBLEScanResults results) {
    MI32.infoMsg = MI32_SCAN_ENDED;
    MI32.mode.runningScan = 0;
    MI32.mode.deleteScanTask = 1; // if scan ended dew to a BLE controller error, make sure we stop the task
  }

  void IRAM_ATTR onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    static bool _mutex = false;
    if(_mutex) return;
    _mutex = true;

    int RSSI = advertisedDevice->getRSSI();
    uint8_t addr[6];
    memcpy(addr,advertisedDevice->getAddress().getNative(),6);
    MI32_ReverseMAC(addr);
    size_t ServiceDataLength = 0;

    if(MI32.beAdvCB != nullptr && MI32.mode.triggerBerryAdvCB == 0){
      berryAdvPacket_t *_packet = (berryAdvPacket_t *)MI32.beAdvBuf;
      memcpy(_packet->MAC,addr,6);
      _packet->addressType = advertisedDevice->getAddressType();
      _packet->RSSI = (uint8_t)RSSI;
      uint8_t *_payload = advertisedDevice->getPayload();
      _packet->length = advertisedDevice->getPayloadLength();
      memcpy(_packet->payload,_payload, _packet->length);
      MI32.mode.triggerBerryAdvCB = 1;
    }

    if (advertisedDevice->getServiceDataCount() == 0) {
      if(MI32.option.handleEveryDevice == 1) {
        MI32HandleEveryDevice(advertisedDevice, addr, RSSI);
      }
      _mutex = false;
      return;
    }

    uint16_t UUID = advertisedDevice->getServiceDataUUID(0).getNative()->u16.value;
    ServiceDataLength = advertisedDevice->getServiceData(0).length();

    if(UUID==0xfe95) {
      MI32ParseResponse((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    else if(UUID==0xfcd2) {
      std::string optionalName = advertisedDevice->getName();
      MI32parseBTHomePacket((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI, optionalName.c_str());
    }
    else if(UUID==0xfdcd) { // deprecated
      MI32parseCGD1Packet((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    else if(UUID==0x181a) { //ATC and PVVX - deprecated, change FW setting of these devices to BTHome V2
      MI32ParseATCPacket((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    else if(MI32.option.handleEveryDevice == 1) {
        MI32HandleEveryDevice(advertisedDevice, addr, RSSI);
    }
  _mutex = false;
  };
};

class MI32ServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
        struct{
          BLERingBufferItem_t header;
          uint8_t buffer[6];
        } item;
        item.header.length = 6;
        item.header.type = BLE_OP_ON_CONNECT;
        memcpy(item.buffer,connInfo.getAddress().getNative(),6);
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t) + 6 , pdMS_TO_TICKS(1));
        MI32.infoMsg = MI32_SERV_CLIENT_CONNECTED;
    };
    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
        struct{
          BLERingBufferItem_t header;
        } item;
        item.header.length = 0;
        item.header.type = BLE_OP_ON_DISCONNECT;
        memset(MI32.conCtx->MAC,0,6);
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t), pdMS_TO_TICKS(1));
        MI32.infoMsg = MI32_SERV_CLIENT_DISCONNECTED;
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
        NimBLEDevice::startAdvertising(0);
#else
        NimBLEDevice::startAdvertising();
#endif
    };
};

class MI32CharacteristicCallbacks: public NimBLECharacteristicCallbacks {
    void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo){
        struct{
          BLERingBufferItem_t header;
        } item;
        item.header.length = 0;
        item.header.type = BLE_OP_ON_READ;
        item.header.returnCharUUID = pCharacteristic->getUUID().getNative()->u16.value;
        item.header.handle = pCharacteristic->getHandle();
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t), pdMS_TO_TICKS(1));
    };

    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
        struct{
          BLERingBufferItem_t header;
          uint8_t buffer[255];
        } item;
        item.header.length = pCharacteristic->getDataLength();;
        item.header.type = BLE_OP_ON_WRITE;
        item.header.returnCharUUID = pCharacteristic->getUUID().getNative()->u16.value;
        item.header.handle = pCharacteristic->getHandle();
        memcpy(item.buffer,pCharacteristic->getValue(),pCharacteristic->getDataLength());
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t) + item.header.length , pdMS_TO_TICKS(1));
    };

    /** The status returned in status is defined in NimBLECharacteristic.h.
     *  The value returned in code is the NimBLE host return code.
     */
    void onStatus(NimBLECharacteristic* pCharacteristic, int code) {
        struct{
          BLERingBufferItem_t header;
          uint8_t buffer[4];
        } item;
        item.header.length = 4;
        item.header.type = BLE_OP_ON_STATUS;
        item.header.returnCharUUID = pCharacteristic->getUUID().getNative()->u16.value;
        item.header.handle = pCharacteristic->getHandle();
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t) + 4, pdMS_TO_TICKS(1));
    };

    void onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) {
        struct{
          BLERingBufferItem_t header;
        } item;
        item.header.length = 0;
        item.header.type = BLE_OP_ON_UNSUBSCRIBE + subValue;;
        item.header.returnCharUUID = pCharacteristic->getUUID().getNative()->u16.value;
        item.header.handle = pCharacteristic->getHandle();
        xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t), pdMS_TO_TICKS(1));
    };
};


void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  if(isNotify){
    struct{
      BLERingBufferItem_t header;
      uint8_t buffer[255];
    } item;
    item.header.length = length;
    // item.header.type = 103;  does not matter for now
    memcpy(item.buffer,pData,length);
    item.header.returnCharUUID = pRemoteCharacteristic->getUUID().getNative()->u16.value;
    item.header.handle = pRemoteCharacteristic->getHandle();
    xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t) + length , pdMS_TO_TICKS(5));
    MI32.mode.readingDone = 1;
    MI32.infoMsg = MI32_GOT_NOTIFICATION;
    return;
  }
}

static MI32AdvCallbacks MI32ScanCallbacks;
static MI32SensorCallback MI32SensorCB;
static MI32CharacteristicCallbacks MI32ChrCallback;

/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

/**
 * @brief Remove all colons from null terminated char array
 *
 * @param _string Typically representing a MAC-address like AA:BB:CC:DD:EE:FF
 */
void MI32stripColon(char* _string){
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

/**
 * @brief Convert string that represents a hexadecimal number to a byte array
 *
 * @param _string input string in format: AABBCCDDEEFF or AA:BB:CC:DD:EE:FF, case insensitive
 * @param _mac  target byte array must match the correct size (i.e. AA:BB -> uint8_t bytes[2])
 */

void MI32HexStringToBytes(char* _string, uint8_t* _byteArray) {
  MI32stripColon(_string);
  UpperCase(_string,_string);
  uint32_t index = 0;
  uint32_t _end = strlen(_string);
  memset(_byteArray,0,_end/2);
  while (index < _end) {
      char c = _string[index];
      uint8_t value = 0;
      if(c >= '0' && c <= '9')
        value = (c - '0');
      else if (c >= 'A' && c <= 'F')
        value = (10 + (c - 'A'));
      _byteArray[(index/2)] += value << (((index + 1) % 2) * 4);
      index++;
  }
}

/**
 * @brief Reverse an array of 6 bytes
 *
 * @param _mac a byte array of size 6 (typically representing a MAC address)
 */
void MI32_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

void MI32AddKey(mi_bindKey_t keyMAC){
  bool unknownMAC = true;
  uint32_t _slot = 0;
  for(auto &_sensor : MIBLEsensors){
    if(memcmp(keyMAC.MAC,_sensor.MAC,sizeof(keyMAC.MAC))==0){
      _sensor.key = new uint8_t[16];
      memcpy(_sensor.key,keyMAC.key,16);
      unknownMAC=false;
      _sensor.status.hasWrongKey = 0;
      AddLog(LOG_LEVEL_INFO,PSTR("add key to %s"),MI32getDeviceName(_slot));
    }
    _slot++;
  }
  if(unknownMAC){
    AddLog(LOG_LEVEL_ERROR,PSTR("M32: unknown MAC"));
  }
}


/**
 * @brief Decrypts payload in place
 *
 * @param _buf - pointer to the buffer at position of PID
 * @param _bufSize - buffersize (last position is two bytes behind last byte of TAG)
 * @param _payload - target buffer
 * @param _slot - sensor slot in the global vector
 * @return int - error code, 0 for success
 */
int MI32_decryptPacket(char * _buf, uint16_t _bufSize, uint8_t * _payload, uint32 _slot){
  // int32_t start = _getCycleCount();
  mi_beacon_t *_beacon = (mi_beacon_t *)_buf;

  uint8_t nonce[13]; //v3:13, v5:12
  uint32_t nonceLen = 12; // most devices are v5
  uint8_t tag[4] = {0};
  const unsigned char authData[1] = {0x11};
  size_t dataLen = _bufSize - 11 ; // _bufsize - frame - type - frame.counter - MAC

  if(MIBLEsensors[_slot].key == nullptr){
    // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: No Key found !!"));
    return -2;
  }

  uint32_t _version = (uint32_t)_beacon->frame.version;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: encrypted msg from %s with version:%u"),MI32getDeviceName(_slot),_version);

  if(_version == 5){
    if(_beacon->frame.includesMAC){
      for (uint32_t i = 0; i<6; i++){
        nonce[i] = _beacon->MAC[i];
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: has MAC"));
      memcpy(_payload,(uint8_t*)&_beacon->capability, dataLen); //special packet
      dataLen -= 7;
    }
    else{
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: has no MAC"));
      for (uint32_t i = 0; i<6; i++){
        nonce[i] = MIBLEsensors[_slot].MAC[5-i];
      }
      dataLen = _bufSize -5 ;
      memcpy(_payload,_beacon->MAC, dataLen); //special packet
      dataLen -= 7;
      // AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*) _payload, dataLen);
    }
    // nonce: device MAC, device type, frame cnt, ext. cnt
    memcpy((uint8_t*)&nonce+6,(uint8_t*)&_beacon->productID,2);
    nonce[8] = _beacon->counter;
    memcpy((uint8_t*)&nonce+9,(uint8_t*)&_payload[dataLen],3);
    // memcpy((uint8_t*)&tag,(uint8_t*)&_payload[dataLen-4],4);
    memcpy((uint8_t*)&tag,(uint8_t*)&_buf[_bufSize-4],4);
  }
  else if(_version == 3){
    // nonce:  frame_ctrl, device type, ext. cnt, frame cnt, device MAC(only first 5 bytes)
    memcpy(_payload,(uint8_t*)&_beacon->capability, dataLen); //special packet
    nonceLen = 13;
    memcpy((uint8_t*)&nonce,(uint8_t*)&_beacon->frame,2);
    memcpy((uint8_t*)&nonce+2,(uint8_t*)&_beacon->productID,2);
    nonce[4] = _beacon->counter;
    memcpy((uint8_t*)&nonce+5,(uint8_t*)&_buf[_bufSize-4],3);
    for (uint32_t i = 0; i<5; i++){
      nonce[i+8] = _beacon->MAC[i];
    }
    // tag[0] = _buf[_bufSize-1]; // it is unclear, if this value is a checksum
    dataLen -= 4;
  }
  else{
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: unexpected decryption version:%u"),_version); // should never happen
  }

  br_aes_small_ctrcbc_keys keyCtx;
  br_aes_small_ctrcbc_init(&keyCtx, MIBLEsensors[_slot].key, 16);

  br_ccm_context ctx;
  br_ccm_init(&ctx, &keyCtx.vtable);
  br_ccm_reset(&ctx, nonce, nonceLen, sizeof(authData), dataLen, sizeof(tag));
  br_ccm_aad_inject(&ctx, authData, sizeof(authData));
  br_ccm_flip(&ctx);
  br_ccm_run(&ctx, 0, _payload, dataLen);

  if(br_ccm_check_tag(&ctx, &tag)) return 0;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: decrypted in %2_f mSec"), &enctime);
  // AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*) _payload, dataLen);
  if(_version == 3 && _payload[1] == 0x10) return 0; // no known way to really verify decryption, but 0x10 is expected here for button events
  return -1; // wrong key ... maybe corrupt data packet too
}

/*********************************************************************************************\
 * common functions
\*********************************************************************************************/

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _MAC        BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t * _MAC, uint16_t _type, uint8_t counter){
  DEBUG_SENSOR_LOG(PSTR("%s: will test ID-type: %x"),D_CMND_MI32, _type);
  uint16_t _pid = _type; // save for unknown types
  bool _success = false;
  for (uint32_t i=0;i<MI32_TYPES;i++){ // i < sizeof(kMI32DeviceID) gives compiler warning
    if(_type == kMI32DeviceID[i]){
      DEBUG_SENSOR_LOG(PSTR("M32: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
  }
  if(!_success) _type = UNKNOWN_MI;

  DEBUG_SENSOR_LOG(PSTR("%s: vector size %u"),D_CMND_MI32, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_MAC,MIBLEsensors[i].MAC,6)==0){
      DEBUG_SENSOR_LOG(PSTR("%s: known sensor at slot: %u"),D_CMND_MI32, i);
      MIBLEsensors[i].lastTime = Rtc.local_time;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Counters: %x %x"),MIBLEsensors[i].lastCnt, counter);
      if(MIBLEsensors[i].lastCnt==counter && counter!=0) {
        // AddLog(LOG_LEVEL_DEBUG,PSTR("Old packet"));
        return 0xff; // packet received before, stop here
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: i: %x %x %x %x %x %x"),D_CMND_MI32, MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[0]);
    DEBUG_SENSOR_LOG(PSTR("%s: n: %x %x %x %x %x %x"),D_CMND_MI32, _MAC[5], _MAC[4], _MAC[3],_MAC[2],_MAC[1],_MAC[0]);
  }
  if(MI32.mode.didGetConfig || MIBLEsensors.size() > 31){ // web UI is currently limited to 32
    DEBUG_SENSOR_LOG(PSTR("M32: ignore new sensor, because of loaded config"));
    return 0xff; //discard the data
  }
  DEBUG_SENSOR_LOG(PSTR("%s: found new sensor"),D_CMND_MI32);
  mi_sensor_t _newSensor{};
  memcpy(_newSensor.MAC,_MAC, 6);
  _newSensor.PID = _pid;
  _newSensor.type = _type;
  _newSensor.eventType.raw = 0;
  _newSensor.feature.raw = 0;
  _newSensor.status.raw = 0;
  _newSensor.temp = NAN;
  _newSensor.bat=0x00;
  _newSensor.RSSI=0;
  _newSensor.lux = 0x00ffffff;
  _newSensor.key = nullptr;
  _newSensor.lastTime = Rtc.local_time;
  switch (_type)
    {
    case UNKNOWN_MI: case BTHOME:
      _newSensor.feature.raw = 0;
      break;
    case FLORA:
      _newSensor.moisture =0xff;
      _newSensor.fertility =0xffff;
      _newSensor.firmware[0]='\0';
      _newSensor.feature.temp=1;
      _newSensor.feature.moist=1;
      _newSensor.feature.fert=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      break;
    case NLIGHT:
      _newSensor.events=0x00;
      _newSensor.feature.motion=1;
      _newSensor.feature.NMT=1;
      _newSensor.NMT=0;
      break;
    case MJYD2S:
      _newSensor.NMT=0;
      _newSensor.events=0x00;
      _newSensor.feature.motion=1;
      _newSensor.feature.NMT=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
      _newSensor.NMT=0;
      break;
    case YLYK01: case YLKG08: case YLAI003:
      _newSensor.feature.Btn = 1;
      _newSensor.Btn = 99;
      if(_type == YLKG08){
        _newSensor.feature.knob = 1;
        _newSensor.dimmer = 0;
      }
      break;
    case MCCGQ02:
      _newSensor.events=0x00;
      _newSensor.feature.bat=1;
      _newSensor.feature.door=1;
      _newSensor.door = 255;
      break;
    case SJWS01L:
      _newSensor.feature.leak=1;
      _newSensor.feature.bat=1;
      _newSensor.feature.Btn=1;
      _newSensor.Btn=99;
      break;
    default:
      _newSensor.hum=NAN;
      _newSensor.feature.temp=1;
      _newSensor.feature.hum=1;
      _newSensor.feature.tempHum=1;
      _newSensor.feature.bat=1;
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog(LOG_LEVEL_DEBUG,PSTR("M32: new %s at slot: %u"),MI32getDeviceName(MIBLEsensors.size()-1),MIBLEsensors.size()-1);
  MI32.mode.shallShowStatusInfo = 1;
  return MIBLEsensors.size()-1;
};

/**
 * @brief trigger real-time message for motion or RC
 *
 */
void MI32triggerTele(void){
  MI32.mode.triggeredTele = 1;
  MqttPublishSensor();
  XdrvRulesProcess(0);
}

#ifdef USE_MI_EXT_GUI
/**
 * @brief Saves a sensor value mapped to the graph range of 0-20 pixel, this function automatically reads the actual hour from system time
 *
 * @param history - pointer to uint8_t[23]
 * @param value - value as float, this
 * @param type  - internal type. for BLE: 0 - temperature, 1 - humidity, 2 - illuminance, for internal sensors: 100 - wattage
 */
void MI32addHistory(uint8_t history[24], float value, const uint32_t type){
  const uint32_t _hour = (LocalTime()%SECS_PER_DAY)/SECS_PER_HOUR;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history hour: %u"),_hour);
  switch(type){
    case 0:  //temperature
      history[_hour] = ((((value + 5.0f)/4) + 1) + 0b10000000); //temp
      break;
    case 1: //humidity
      history[_hour] = (((value/5.0f) + 1) + 0b10000000) ; //hum
      break;
    case 2: //light
      if(value>100.0f) value=100.0f; //clamp it for now
      history[_hour] = (((value/5.0f) + 1) + 0b10000000); //lux
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history lux: %u in hour:%u"),history[_hour], _hour);
      break;
    case 3: //BLE device sighting
      uint16_t sightings = history[_hour] & 0b01111111;
      if(sightings<20){
        history[_hour] = (sightings | 0b10000000) + 1;
        // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history sighting: %u in hour:%u"),history[_hour], _hour);
      }
      break;
#ifdef USE_MI_ESP32_ENERGY
    case 100: // energy
      if(value == 0.0f) value = 1.0f;
      const uint8_t _watt = ((MI32ln(value)*2) + 0b10000000); //watt
      history[_hour] = _watt;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history energy: %u for value:%u"),history[_hour], value); //still playing with the mapping
      break;
#endif //USE_MI_ESP32_ENERGY
  }
}

/**
 * @brief Returns a value between 0-21 for use as a data point in the history graph of the extended web UI
 *
 * @param history - pointer to uint8_t[23]
 * @param hour  - hour of datapoint
 * @return uint8_t  - value for the y-axis, should be between 0-21
 */
uint8_t MI32fetchHistory(uint8_t history[24], uint32_t hour){
    if((hour>23 || bitRead(history[hour],7)) == 0) {
      return 0; //invalidated data
    }
    return (history[hour]) - 0b10000000;
}

/**
 * @brief Invalidates the history data of the following hour by setting MSB to 0, should be called at FUNC_JSON_APPEND
 *
 */
void Mi32invalidateOldHistory(){
  uint32_t _hour = (LocalTime()%SECS_PER_DAY)/SECS_PER_HOUR;
  static uint32_t _lastInvalidatedHour = 99;
  if (_lastInvalidatedHour == _hour){
    return;
  }
  uint32_t _nextHour = (_hour>22)?0:_hour+1;
  for(auto &_sensor:MIBLEsensors){
    if(_sensor.feature.temp == 1){
      bitClear(_sensor.temp_history[_nextHour],7);
    }
    if(_sensor.feature.hum == 1){
      bitClear(_sensor.hum_history[_nextHour],7);
    }
    if(_sensor.feature.lux == 1){
      bitClear(_sensor.lux_history[_nextHour],7);
    }
    if(_sensor.feature.payload == 1){
      bitClear(_sensor.temp_history[_nextHour],7);
    }
  }
  _lastInvalidatedHour = _hour;
}

#endif //USE_MI_EXT_GUI
/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/

void MI32PreInit(void) {

  MI32.mode.init = false;

  //test section for options
  MI32.option.allwaysAggregate = 1;
  MI32.option.noSummary = 0;
  MI32.option.directBridgeMode = 0;
  MI32.option.ignoreBogusBattery = 1; // from advertisements
  MI32.option.handleEveryDevice = 0; // scan for every BLE device with a public address

  MI32loadCfg();
  if(MIBLEsensors.size()>0){
    MI32.mode.didGetConfig = 1;
  }

  MI32.beAdvCB = nullptr;
  AddLog(LOG_LEVEL_INFO,PSTR("M32: pre-init"));
}

void MI32Init(void) {
  if (MI32.mode.init) { return; }

  if (TasmotaGlobal.global_state.wifi_down && TasmotaGlobal.global_state.eth_down) {
    if (!(WIFI_MANAGER == Wifi.config_type || WIFI_MANAGER_RESET_ONLY == Wifi.config_type)) return; 
  }

  if (!TasmotaGlobal.global_state.wifi_down) {
    TasmotaGlobal.wifi_stay_asleep = true;
    if (WiFi.getSleep() == false) {
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Put WiFi modem in sleep mode"));
      WiFi.setSleep(true); // Sleep
    }
  }

  if (!MI32.mode.init) {
  #ifdef CONFIG_BTDM_BLE_SCAN_DUPL
    // NimBLEDevice::setScanFilterMode(2); //CONFIG_BTDM_SCAN_DUPL_TYPE_DATA_DEVICE
    // NimBLEDevice::setScanDuplicateCacheSize(10); // will not be perfect for every situation (few vs many BLE devices nearby)
  #endif
    const std::string name(TasmotaGlobal.hostname);
    NimBLEDevice::init(name);
    AddLog(LOG_LEVEL_INFO,PSTR("M32: Init BLE device: %s"),TasmotaGlobal.hostname);
    MI32.mode.init = 1;
    MI32.mode.readyForNextConnJob = 1;
    MI32StartTask(MI32_TASK_SCAN); // Let's get started !!
  }

  return;
}

/*********************************************************************************************\
 * Berry section - partly used by HomeKit too
\*********************************************************************************************/
extern "C" {

  bool MI32checkBLEinitialization(){
    return (MI32.mode.init && Settings->flag5.mi32_enable);
  }

  void MI32BerryLoop(){
    MI32BLELoop();
  }

  bool MI32runBerryServer(uint16_t operation){
    MI32.conCtx->operation = operation;
    AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Berry server op: %d, response: %u"),MI32.conCtx->operation, MI32.conCtx->response);
    if(MI32.mode.readyForNextServerJob == 0){
      MI32.mode.triggerNextServerJob = 0;
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: old server job not finished yet!!"));
      return false;
    }
    MI32.mode.triggerNextServerJob = 1;
    return true;
  }

  bool MI32runBerryConnection(uint8_t operation, bool response, int32_t* arg1){
    if(MI32.conCtx != nullptr){
      if(arg1 != nullptr){
        MI32.conCtx->arg1 = *arg1;
        MI32.conCtx->hasArg1 = true;
        AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: arg1: %u"),MI32.conCtx->arg1);
      }
      else{
        MI32.conCtx->hasArg1 = false;
      }
      MI32.conCtx->response = response;
      if(operation > 200){
        return MI32runBerryServer(operation);
      }
      MI32.conCtx->oneOp = (operation > 9);
      MI32.conCtx->operation = operation%10;
      AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Berry connection op: %d, addrType: %d, oneOp: %u, response: %u"),MI32.conCtx->operation, MI32.conCtx->addrType, MI32.conCtx->oneOp, MI32.conCtx->response);
      if(MI32.conCtx->oneOp){
        MI32StartConnectionTask();
      }
      else{
        if(MI32.mode.connected){
          AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: continue connection job"));
          MI32.mode.triggerNextConnJob = 1;
          if(!MI32.mode.readyForNextConnJob){
            AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: old connection job not finished yet!!"));
          }
        }
        else{
          MI32StartConnectionTask(); //first job of many or unexpected disconnect
        }
      }
      return true;
    }
    return false;
  }

  void MI32setBerryConnCB(void* function, uint8_t *buffer){
    if(MI32.conCtx == nullptr){
      MI32.conCtx = new MI32connectionContextBerry_t;
    }
    MI32.conCtx->buffer = buffer;
    MI32.beConnCB = function;
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: Connection Ctx created"));
  }

  void MI32setBerryServerCB(void* function, uint8_t *buffer){
    if(function == nullptr || buffer == nullptr)
    {
      MI32.mode.deleteServerTask = 1;
      MI32.beServerCB = nullptr;
      AddLog(LOG_LEVEL_INFO,PSTR("BLE: Server session stopping"));
      return;
    }
    if(MI32.conCtx == nullptr){
      MI32.conCtx = new MI32connectionContextBerry_t;
    }
    MI32.conCtx->buffer = buffer;
    MI32.beServerCB = function;
    MI32StartTask(MI32_TASK_SERV);
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: Server Ctx created"));
  }

  bool MI32setBerryCtxSvc(const char *Svc, bool discoverAttributes){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->serviceUUID = NimBLEUUID(Svc);
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: SVC: %s"),MI32.conCtx->serviceUUID.toString().c_str());
      MI32.mode.discoverAttributes = discoverAttributes;
      return true;
    }
    return false;
  }

  bool MI32setBerryCtxChr(const char *Chr){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->charUUID = NimBLEUUID(Chr);
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: CHR: %s"),MI32.conCtx->charUUID.toString().c_str());
      uint16_t _uuid = MI32.conCtx->charUUID.getNative()->u16.value; //if not "notify op" -> present requested characteristic as return UUID
      MI32.conCtx->returnCharUUID = _uuid;
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: return 16-bit UUID: %04x"),MI32.conCtx->returnCharUUID);
      return true;
    }
    return false;
  }

  bool MI32setBerryCtxMAC(uint8_t *MAC, uint8_t type){
    if(MI32.conCtx != nullptr){
      memcpy(MI32.conCtx->MAC,MAC,6);
      if(type<4) MI32.conCtx->addrType = type;
      else MI32.conCtx->addrType = 0;
      return true;
    }
    return false;
  }

  void MI32setBerryAdvCB(void* function, uint8_t *buffer){
    MI32.beAdvCB = function;
    MI32.beAdvBuf = buffer;
  }

  bool MI32addMACtoBlockList(uint8_t *MAC, uint8_t type){
    NimBLEDevice::addIgnored(NimBLEAddress(MAC,type));
    return NimBLEDevice::isIgnored(NimBLEAddress(MAC,type));
  }

  bool MI32addMACtoWatchList(uint8_t *MAC, uint8_t type){
    NimBLEAddress _newAddress = NimBLEAddress(MAC,type);
    if(MI32Scan==nullptr){
      if(!NimBLEDevice::whiteListAdd(_newAddress)){
        return false;
      }
    }
    else{
      bool _runningScan = MI32Scan->stop();
      if(NimBLEDevice::whiteListAdd(_newAddress)){
        MI32Scan->setFilterPolicy(BLE_HCI_SCAN_FILT_USE_WL);
        if(_runningScan) MI32Scan->start(0, false);
      }
      else {
        if(_runningScan) MI32Scan->start(0, false);
        return false;
      }
    }
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: add %s to watchlist of size: %u"),_newAddress.toString().c_str(),NimBLEDevice::getWhiteListCount());
    return true;
  }

  void MI32setBatteryForSlot(uint32_t slot, uint8_t value){
    if(slot>MIBLEsensors.size()-1) return;
    if(MIBLEsensors[slot].feature.bat){
      MIBLEsensors[slot].bat = value;
    }
  }

  void MI32setHumidityForSlot(uint32_t slot, float value){
    if(slot>MIBLEsensors.size()-1) return;
    if(MIBLEsensors[slot].feature.hum){
      MIBLEsensors[slot].hum = value;
    }
  }

  void MI32setTemperatureForSlot(uint32_t slot, float value){
    if(slot>MIBLEsensors.size()-1) return;
    if(MIBLEsensors[slot].feature.temp){
      MIBLEsensors[slot].temp = value;
    }
  }

  uint32_t MI32numberOfDevices(){
    return MIBLEsensors.size();
  }

  uint8_t * MI32getDeviceMAC(uint32_t slot){
    if(slot>MIBLEsensors.size()-1) return NULL;
    return MIBLEsensors[slot].MAC;
  }

  char * MI32getDeviceName(uint32_t slot){
    if(MIBLEsensors[slot].name != nullptr){
      return MIBLEsensors[slot].name;
    }
    static char _name[12];
    if( MIBLEsensors[slot].type == UNKNOWN_MI){
      if(MIBLEsensors[slot].PID == 0){
        snprintf_P(_name,8,PSTR("BLE_%02u"),slot);
      } else {
        snprintf_P(_name,8,PSTR("MI_%04X"),MIBLEsensors[slot].PID);
      }
    }
    else{
      GetTextIndexed(_name, sizeof(_name), MIBLEsensors[slot].type-1, kMI32DeviceType);
    }
    return _name;
  }

} //extern "C"

/*********************************************************************************************\
 * Config section
\*********************************************************************************************/

void MI32loadCfg(){
  if (TfsFileExists("/mi32cfg")){
  MIBLEsensors.reserve(10);
  const size_t _buf_size = 2048;
  char * _filebuf = (char*)calloc(_buf_size,1);
    AddLog(LOG_LEVEL_INFO,PSTR("M32: found config file"));
    if(TfsLoadFile("/mi32cfg",(uint8_t*)_filebuf,_buf_size)){
      AddLog(LOG_LEVEL_INFO,PSTR("M32: %s"),_filebuf);
      JsonParser parser(_filebuf);
      JsonParserToken root = parser.getRoot();
      if (!root) {AddLog(LOG_LEVEL_INFO,PSTR("M32: invalid root "));}
      JsonParserArray arr = root.getArray();
      if (!arr) {AddLog(LOG_LEVEL_INFO,PSTR("M32: invalid array object"));; }
      bool _error;
      int32_t _numberOfDevices;
      for (auto _dev  : arr) {
          AddLog(LOG_LEVEL_INFO,PSTR("M32: found device in config file"));
          JsonParserObject _device = _dev.getObject();
          uint8_t _mac[6];
          JsonParserToken _val = _device[PSTR("MAC")];
          _error = true;
          if (_val) {
              char *_macStr = (char *)_val.getStr();
              AddLog(LOG_LEVEL_INFO,PSTR("M32: found MAC: %s"), _macStr);
              if(strlen(_macStr)!=12){
                AddLog(LOG_LEVEL_INFO,PSTR("M32: wrong MAC length: %u"), strlen(_macStr));
                break;
              }
              MI32HexStringToBytes(_macStr,_mac);
              _val = _device[PSTR("PID")];
              if(_val){
                uint8_t _pid[2];
                char *_pidStr = (char *)_val.getStr();
                AddLog(LOG_LEVEL_INFO,PSTR("M32: found PID: %s"), _pidStr);
                if(strlen(_pidStr)!=4){
                  AddLog(LOG_LEVEL_INFO,PSTR("M32: wrong PID length: %u"), strlen(_pidStr));
                  break;
                }
                MI32HexStringToBytes(_pidStr,_pid);
                uint16_t _pid16 = _pid[0]*256 + _pid[1];
                _numberOfDevices = MIBLEgetSensorSlot(_mac,_pid16,0);
                _error = false;
              }
          }
          _val = _device[PSTR("key")];
          if (_val) {
            char *_keyStr = (char *)_val.getStr();
            if(strlen(_keyStr)>0){
              if(strlen(_keyStr)==32){
                uint8_t *_key = (uint8_t*) malloc(16);
                MI32HexStringToBytes(_keyStr,_key);
                MIBLEsensors[_numberOfDevices].key = _key;
              }
              else{
                _error = true;
                break;
              }
            }
          }
          _val = _device[PSTR("name")];
          if (_val) {
            char *_name = (char *)_val.getStr();
            MIBLEsensors[_numberOfDevices].name = new char[strlen(_name) + 1];
            strcpy(MIBLEsensors[_numberOfDevices].name, _name);
            AddLog(LOG_LEVEL_INFO,PSTR("M32: found name: %s"), _name);
          }
          _val = _device[PSTR("feat")];
          if (_val) {
            MIBLEsensors[_numberOfDevices].feature.raw = _val.getUInt();
          }
      }
      if(!_error){
        AddLog(LOG_LEVEL_INFO,PSTR("M32: added %u devices from config file"), _numberOfDevices + 1);
      }
    }
  free(_filebuf);
  }
}

void MI32saveConfig(){
  const size_t _buf_size = 2048;
  char * _filebuf = (char*) malloc(_buf_size);
  _filebuf[0] = '[';
  uint32_t _pos = 1;
  for(auto _sensor: MIBLEsensors){
    char _MAC[13];
    ToHex_P(_sensor.MAC,6,_MAC,13);
    char _key[33];
    _key[0] = 0;
    if(_sensor.key != nullptr){
      ToHex_P(_sensor.key,16,_key,33);
    }
    char _name_feat[64];
    if(_sensor.name != nullptr){
      snprintf_P(_name_feat,64,PSTR(",\"name\":\"%s\",\"feat\":%u"),_sensor.name,_sensor.feature.raw);
    }
    else if(_sensor.type == BTHOME && _sensor.name == nullptr){
      snprintf_P(_name_feat,64,PSTR(",\"feat\":%u"),_sensor.feature.raw);
    }
    else{
      _name_feat[0] = 0;
    }
    uint32_t _inc = snprintf_P(_filebuf+_pos,200,PSTR("{\"MAC\":\"%s\",\"PID\":\"%04x\",\"key\":\"%s\"%s},"),_MAC,_sensor.PID,_key,_name_feat);
    _pos += _inc;
  }
  _filebuf[_pos-1] = ']';
  _filebuf[_pos] = '\0';
  if (_pos>2){
    AddLog(LOG_LEVEL_INFO,PSTR("M32: %s"), _filebuf);
    if (TfsSaveFile("/mi32cfg",(uint8_t*)_filebuf,_pos+1)) {
      AddLog(LOG_LEVEL_INFO,PSTR("M32: %u bytes written to config"), _pos+1);
    }
  }
  else{
    AddLog(LOG_LEVEL_ERROR,PSTR("M32: nothing written to config"));
  }
  free(_filebuf);
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

void MI32suspendScanTask(void){
  if (MI32.ScanTask != nullptr && MI32.mode.runningScan == 1) vTaskSuspend(MI32.ScanTask);
}

void MI32resumeScanTask(void){
  if (MI32.ScanTask != nullptr && MI32.mode.runningScan == 1) vTaskResume(MI32.ScanTask);
}

void MI32StartTask(uint32_t task){
  if (MI32.mode.willConnect == 1) return; // we are in the middle of connecting to something ... do not interrupt this.
  MI32.role = 0;
  switch(task){
    case MI32_TASK_SCAN:
      if (MI32.mode.connected == 1) return;
      MI32StartScanTask();
      break;
    case MI32_TASK_CONN:
      if (MI32.mode.canConnect == 0) return;
      MI32.mode.deleteScanTask = 1;
      MI32StartConnectionTask();
      break;
    case MI32_TASK_SERV:
      MI32.mode.deleteScanTask = 1;
      MI32StartServerTask();
      break;
    default:
      break;
  }
}

// Scan task section

void MI32StartScanTask(){
    if (MI32.mode.connected == 1) return;
    if(MI32.ScanTask!=nullptr) vTaskDelete(MI32.ScanTask);
    MI32.mode.runningScan = 1;
    MI32.mode.deleteScanTask = 0;
    MI32.role = 1;
    xTaskCreatePinnedToCore(
    MI32ScanTask,    /* Function to implement the task */
    "MI32ScanTask",  /* Name of the task */
    2048,             /* Stack size in words */
    NULL,             /* Task input parameter */
    0,                /* Priority of the task */
    &MI32.ScanTask,  /* Task handle. */
    0);               /* Core where the task should run */
}

void MI32ScanTask(void *pvParameters){
  if(MI32.mode.didGetConfig){
    vTaskDelay(5000/ portTICK_PERIOD_MS);
  }

  MI32Scan = NimBLEDevice::getScan();

  MI32Scan->setScanCallbacks(&MI32ScanCallbacks,true);
  if(NimBLEDevice::getWhiteListCount()>0){
    MI32Scan->setFilterPolicy(BLE_HCI_SCAN_FILT_USE_WL);
  }
  else {
    MI32Scan->setFilterPolicy(BLE_HCI_SCAN_FILT_NO_WL);
  }

  MI32Scan->setActiveScan(MI32.option.activeScan == 1);
  MI32Scan->setMaxResults(0);
  // MI32Scan->setInterval(30);
  // MI32Scan->setWindow(25);
  MI32Scan->start(0, false); // never stop scanning, will pause automatically while connecting
  MI32.infoMsg = MI32.option.activeScan?MI32_START_SCANNING_ACTIVE:MI32_START_SCANNING_PASSIVE;

  uint32_t timer = 0;
  for(;;){
    vTaskDelay(100/ portTICK_PERIOD_MS);
    if(MI32.mode.deleteScanTask == 1){
      MI32Scan->stop();
      MI32.mode.runningScan = 0;
      MI32.ScanTask = nullptr;
      break;
    }
    if(MI32.mode.updateScan == 1){
      MI32Scan->stop();
      MI32Scan->setActiveScan(MI32.option.activeScan == 1);
      MI32Scan->start(0,true);
      MI32.mode.updateScan = 0;
      MI32.infoMsg = MI32.option.activeScan?MI32_START_SCANNING_ACTIVE:MI32_START_SCANNING_PASSIVE;
    }
  }
  MI32.mode.deleteScanTask = 0;
  vTaskDelete( NULL );
}

// connection task section

bool MI32ConnectActiveSensor(){ // only use inside a task !!
  if(MI32.conCtx->operation == 5) {
    return false;
  }
  NimBLEAddress _address = NimBLEAddress(MI32.conCtx->MAC, MI32.conCtx->addrType);
  if(MI32Client != nullptr){
    if(MI32Client->isConnected() && MI32.mode.connected == 1){ //we only accept a "clean" state without obvious packet losses
      if(MI32.conCtx->operation == 5){ //5 is the disconnect operation
        NimBLEDevice::deleteClient(MI32Client); // disconnect the old
        return false; // request disconnect
      }
      if(MI32Client->getPeerAddress() == _address){
        MI32.infoMsg = MI32_STILL_CONNECTED;
        return true; // still connected -> keep it
      }
      else{
        // AddLog(LOG_LEVEL_ERROR,PSTR("M32: disconnect %s"),MI32Client->getPeerAddress().toString().c_str());
        NimBLEDevice::deleteClient(MI32Client); // disconnect the old and connect the new
      }
    }
  }

  MI32Client = nullptr;
  if(NimBLEDevice::getClientListSize()) {
    MI32Client = NimBLEDevice::getClientByPeerAddress(_address);
  }
  if (!MI32Client){
    MI32Client = NimBLEDevice::createClient(_address);
    MI32Client->setClientCallbacks(&MI32SensorCB , false);
  }
  if (!MI32Client->connect(false)) {
      NimBLEDevice::deleteClient(MI32Client);
      // AddLog(LOG_LEVEL_ERROR,PSTR("M32: did not connect client"));
      return false;
  }
  return true;
}

/**
 * @brief Retrieves all services of the connected BLE device and stores the result into the transfer buffer of Berry's BLE module
*         buffer format: 
 *        first byte: number of services
 *        next byte: format of the UUID in bits, next N bytes: the UUID as 16-bit-uint or uint8_t buffer of 16 bytes
 *        ... next service
 */
void MI32ConnectionGetServices(){
  std::vector<NimBLERemoteService*> *srvvector = MI32Client->getServices(true); // refresh
  MI32.conCtx->buffer[1] = srvvector->size(); // number of services
  uint32_t i = 2;
  for (auto &srv: *srvvector) {
    MI32.conCtx->buffer[i] = srv->getUUID().bitSize(); // 16/128 bit
    if(MI32.conCtx->buffer[i] == 16){
      MI32.conCtx->buffer[i+1] = srv->getUUID().getNative()->u16.value & 0xff;
      MI32.conCtx->buffer[i+2] = srv->getUUID().getNative()->u16.value >> 8;
    }
    else{
      memcpy((MI32.conCtx->buffer)+i+1,srv->getUUID().getNative()->u128.value,MI32.conCtx->buffer[i]); // the UUID
    }
    i += 1 + (MI32.conCtx->buffer[i]/8);
  }
  MI32.conCtx->buffer[0] = i;
}

/**
 * @brief Retrieves all characteristics of the given service and stores the result into the transfer buffer of Berry's BLE module
 *         buffer format: 
 *         first byte: number of characteristics
 *         next byte: format of the UUID in bits, next N bytes: the UUID as 16-bit-uint or uint8_t buffer of 16 bytes
 *         next byte: properties in a bitfield
 *         ... next characteristic
 * 
 * @param pSvc 
 */
void MI32ConnectionGetCharacteristics(NimBLERemoteService* pSvc);
void MI32ConnectionGetCharacteristics(NimBLERemoteService* pSvc){
  std::vector<NimBLERemoteCharacteristic*> *charvector = pSvc->getCharacteristics(true); // refresh
  MI32.conCtx->buffer[1] = charvector->size(); // number of characteristics
  uint32_t i = 2;
  for (auto &chr: *charvector) {
    MI32.conCtx->buffer[i] = chr->getUUID().bitSize(); // 16/128 bit
    if(MI32.conCtx->buffer[i] == 16){
      MI32.conCtx->buffer[i+1] = chr->getUUID().getNative()->u16.value & 0xff;
      MI32.conCtx->buffer[i+2] = chr->getUUID().getNative()->u16.value >> 8;
    }
    else{
      memcpy((MI32.conCtx->buffer)+i+1,chr->getUUID().getNative()->u128.value,MI32.conCtx->buffer[i]); // the UUID
    }
    i += 1 + (MI32.conCtx->buffer[i]/8);
    MI32.conCtx->buffer[i] = chr->getProperties(); // flags as bitfield
    MI32.conCtx->buffer[i+1] = chr->getHandle() & 0xff;
    MI32.conCtx->buffer[i+2] = chr->getHandle() >> 8;
    i += 3;
  }
  MI32.conCtx->buffer[0] = i;
}

bool MI32StartConnectionTask(){
    if(MI32.conCtx == nullptr) return false;
    if(MI32.conCtx->buffer == nullptr) return false;
    MI32.mode.willConnect = 1;
    MI32Scan->stop();
    MI32suspendScanTask();
    MI32.role = 2;
    xTaskCreatePinnedToCore(
      MI32ConnectionTask,    /* Function to implement the task */
      "MI32ConnectionTask",  /* Name of the task */
      4096,             /* Stack size in words */
      NULL,             /* Task input parameter */
      2,                /* Priority of the task */
      &MI32.ConnTask,   /* Task handle. */
      0);               /* Core where the task should run */
    return true;
}

void MI32ConnectionTask(void *pvParameters){
#if !defined(CONFIG_IDF_TARGET_ESP32C3) || !defined(CONFIG_IDF_TARGET_ESP32C6) //needs more testing ...
    // NimBLEDevice::setOwnAddrType(BLE_OWN_ADDR_RANDOM,false); //seems to be important for i.e. xbox controller, hopefully not breaking other things
#ifdef CONFIG_BT_NIMBLE_NVS_PERSIST
    NimBLEDevice::setSecurityAuth(true, true, true);
#else
    NimBLEDevice::setSecurityAuth(false, true, true);
#endif

#endif //CONFIG_IDF_TARGET_ESP32C3
    MI32.conCtx->error = MI32_CONN_NO_ERROR;
    if (MI32ConnectActiveSensor()){
      MI32.mode.readingDone = 0;
      uint32_t timer = 0;
      while (MI32.mode.connected == 0){
        if (timer>1000){
          MI32Client->disconnect();
          NimBLEDevice::deleteClient(MI32Client);
          MI32.mode.willConnect = 0;
          MI32.mode.triggerBerryConnCB = 1;
          MI32.conCtx->error = MI32_CONN_NO_CONNECT; // not connected
          MI32StartTask(MI32_TASK_SCAN);
          vTaskDelay(100/ portTICK_PERIOD_MS);
          vTaskDelete( NULL );
        }
        timer++;
        vTaskDelay(10/ portTICK_PERIOD_MS);
      }
      if(MI32.mode.discoverAttributes || MI32.conCtx->hasArg1){ // explicit or in the first run with selection by handle
        MI32Client->discoverAttributes(); // solves connection problems on i.e. yeelight dimmer
      }
      NimBLERemoteService* pSvc = nullptr;
      NimBLERemoteCharacteristic* pChr = nullptr;
      std::vector<NimBLERemoteCharacteristic*>*charvector = nullptr;

      // AddLog(LOG_LEVEL_INFO,PSTR("M32: start connection loop"));
      bool keepConnectionAlive = true;
      MI32.mode.triggerNextConnJob = 1;
      while(keepConnectionAlive){
        while(MI32.mode.triggerNextConnJob == 0){
          vTaskDelay(50/ portTICK_PERIOD_MS);
          if(MI32.mode.connected == 0){
              MI32StartTask(MI32_TASK_SCAN);
              vTaskDelete( NULL );
          }
          // AddLog(LOG_LEVEL_INFO,PSTR("M32: wait ..."));
        }

        MI32.mode.triggerNextConnJob = 0;
        MI32.mode.readyForNextConnJob = 0;
        if(MI32.conCtx->operation == 5){
          MI32Client->disconnect();
          break;
        }

        if(MI32.conCtx->operation == 6){ // get remote services to Berry
          MI32ConnectionGetServices();
        }
        else{
          if(MI32.conCtx->hasArg1){
            pSvc = nullptr; // invalidate possible dangling service from last operation
          }
          else{
            pSvc = MI32Client->getService(MI32.conCtx->serviceUUID);
          }
        }

        if(pSvc) {
          if(MI32.conCtx->operation == 7){ // get remote characteristics to Berry
            MI32ConnectionGetCharacteristics(pSvc);
          }
          else{
          pChr = pSvc->getCharacteristic(MI32.conCtx->charUUID);
          }
        }
        else if(MI32.conCtx->hasArg1){
          pChr = MI32Client->getCharacteristic(MI32.conCtx->arg1); // get by handle, overriding svc and chr values
        }
        else{
          if(MI32.conCtx->operation != 6){
            MI32.conCtx->error = MI32_CONN_NO_SERVICE;
          }
        }
        if (pChr){
          switch(MI32.conCtx->operation){
            case 1:
              if(pChr->canRead()) {
                NimBLEAttValue _val = pChr->readValue();
                MI32.conCtx->buffer[0] = _val.size();
                memcpy( MI32.conCtx->buffer + 1,_val.data(),MI32.conCtx->buffer[0]);
                MI32.conCtx->handle = pChr->getHandle();
              }
              else{
                MI32.conCtx->error = MI32_CONN_CAN_NOT_READ;
              }
              break;
            case 2:
              if(pChr->canWrite() || pChr->canWriteNoResponse()) {
                uint8_t len = MI32.conCtx->buffer[0];
                if(pChr->writeValue(MI32.conCtx->buffer + 1,len,MI32.conCtx->response & !pChr->canWriteNoResponse())) { // falls always back to "no response" if server provides both options
                  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: write op done"));
                  MI32.conCtx->handle = pChr->getHandle();
                }
                else{
                MI32.conCtx->error = MI32_CONN_DID_NOT_WRITE;
                }
              }
              else{
                MI32.conCtx->error = MI32_CONN_CAN_NOT_WRITE;
              }
              MI32.mode.readingDone = 1;
              break;
            case 3:
              if (BLERingBufferQueue == nullptr){
                BLERingBufferQueue = xRingbufferCreate(2048, RINGBUF_TYPE_NOSPLIT);
                if(!BLERingBufferQueue) {
                  MI32.conCtx->error = MI32_CONN_CAN_NOT_NOTIFY;
                  break;
                }
              }
              if(MI32.conCtx->hasArg1){ // characteristic selected by handle
                if (pChr->canNotify()) {
                  if(!pChr->subscribe(true, MI32notifyCB, MI32.conCtx->response)) {
                    MI32.conCtx->error = MI32_CONN_CAN_NOT_NOTIFY; // will return the last result only ATM, maybe check differently
                  }
                }
              }
              else { //  characteristic selected by UUID
                charvector = pSvc->getCharacteristics(true); // always try to subscribe to all characteristics with the same UUID
                uint32_t position = 1;
                for (auto &it: *charvector) {
                  if (it->getUUID() == MI32.conCtx->charUUID) {
                    if (it->canNotify()) {
                      if(!it->subscribe(true, MI32notifyCB, MI32.conCtx->response)) {
                        MI32.conCtx->error = MI32_CONN_CAN_NOT_NOTIFY; // will return the last result only ATM, maybe check differently
                      }
                      else{
                        MI32.conCtx->buffer[position++] = it->getHandle() >> 8;
                        MI32.conCtx->buffer[position++] = it->getHandle() & 0xff;
                        MI32.conCtx->handle = it->getHandle();
                      }
                    }
                  }
                }
                MI32.conCtx->buffer[0] = position - 1;
              }
              break;
          default:
              break;
          }
        }
        else{
            if(MI32.conCtx->operation < 5){ // no characteristics are fine for ops, that are not read, write or subscribe
              MI32.conCtx->error = MI32_CONN_NO_CHARACTERISTIC;
            }
        }
        timer = 0;

        if(MI32.conCtx->error == MI32_CONN_NO_ERROR){
          while (timer<150){
            if (MI32.mode.readingDone || !MI32.conCtx->oneOp){
              break;
            }
            else if (timer>148){
              if (MI32.conCtx->operation==3 && MI32.conCtx->oneOp) {
                MI32.conCtx->error = MI32_CONN_NOTIFY_TIMEOUT; //did not read on notify - timeout only for one-shot op
              }
            }
            timer++;
            vTaskDelay(100/ portTICK_PERIOD_MS);
          }
        }
      MI32.mode.readingDone = 0;
      if(MI32.conCtx->oneOp){
        MI32Client->disconnect();
        keepConnectionAlive = false;
      }
      else{
        MI32.mode.readyForNextConnJob = 1;
        MI32.mode.triggerBerryConnCB = 1;
      }
    }
  }
  else{
    MI32.mode.willConnect = 0;
    MI32.conCtx->error = MI32_CONN_NO_CONNECT; // could not connect (including op:5 in not connected state)
  }
  MI32.mode.connected = 0;
  MI32.mode.triggerBerryConnCB = 1;
  if (BLERingBufferQueue != nullptr){
    vRingbufferDelete(BLERingBufferQueue);
    BLERingBufferQueue = nullptr;
  }
  MI32StartTask(MI32_TASK_SCAN);
  vTaskDelete( NULL );
}

// server task section

bool MI32StartServerTask(){
  AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: Server task ... start"));
  if (BLERingBufferQueue == nullptr){
    BLERingBufferQueue = xRingbufferCreate(2048, RINGBUF_TYPE_NOSPLIT);
    if(!BLERingBufferQueue) {
      AddLog(LOG_LEVEL_ERROR,PSTR("BLE: failed to create ringbuffer queue"));
      return false;
    }
  }
  MI32.role = 3;
  xTaskCreatePinnedToCore(
    MI32ServerTask,    /* Function to implement the task */
    "MI32ServerTask",  /* Name of the task */
    8192,             /* Stack size in words */
    NULL,             /* Task input parameter */
    2,                /* Priority of the task */
    &MI32.ServerTask,   /* Task handle. */
    0);               /* Core where the task should run */
  return true;
}

void MI32ServerSetAdv(NimBLEServer *pServer, std::vector<NimBLEService*>& servicesToStart, bool &shallStartServices);
/**
 * @brief Sets the advertisement message from the data of the context, could be regular advertisement or scan response
 * 
 * @param pServer - our server instance
 * @param servicesToStart - for the first run, this vector holds all our services, would not be used for later modifications of the advertisement message
 * @param shallStartServices - true only for the first call, which will finish the construction of the server by starting all services
 */
void MI32ServerSetAdv(NimBLEServer *pServer, std::vector<NimBLEService*>& servicesToStart, bool &shallStartServices){
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
  NimBLEExtAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
#else
  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
#endif
  /**  optional argument arg1
    BLE_GAP_CONN_MODE_NON (0) - not connectable advertising
    BLE_GAP_CONN_MODE_DIR (1) - directed connectable advertising
    BLE_GAP_CONN_MODE_UND (2) - undirected connectable advertising
   */
  if(MI32.conCtx->hasArg1){
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
    //TODO
#else
    pAdvertising->setAdvertisementType(MI32.conCtx->arg1);
    // AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: AdvertisementType: %u"),MI32.conCtx->arg1);
#endif
    // AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: AdvertisementType: %u"),MI32.conCtx->arg1);
  }
  struct{
    BLERingBufferItem_t header;
    uint8_t buffer[255];
  } item;
  item.header.length = 0;
  if(shallStartServices && MI32.conCtx->operation == BLE_OP_SET_ADV){
    for (auto & pService : servicesToStart) {
        pService->start();
    }
    shallStartServices = false; // only do this at the first run
    if(servicesToStart.size() != 0){
      pServer->start();         // only start server when svc and chr do exist
      uint32_t idx = 0;
      for (auto & pService : servicesToStart) {
        std::vector<NimBLECharacteristic *> characteristics = pService->getCharacteristics();
        for (auto & pCharacteristic : characteristics) {
          uint16_t handle = pCharacteristic->getHandle(); // now we have handles, so pass them to Berry
          item.buffer[idx] = (uint8_t)handle>>8;
          item.buffer[idx+1] = (uint8_t)handle&0xff;
          if (idx > 254) break; // limit to 127 characteristics
          idx += 2;
        }
      }
      item.header.length = idx;
    }
    servicesToStart.clear(); // release vector
  }
#ifdef CONFIG_BT_NIMBLE_EXT_ADV
  NimBLEExtAdvertisement adv;
  adv.setLegacyAdvertising(true); // use legacy for the start
  adv.setData((const uint8_t*)&MI32.conCtx->buffer[1], MI32.conCtx->buffer[0]);
  pAdvertising->setInstanceData(0,adv); // instance id 0
  if(MI32.conCtx->operation == BLE_OP_SET_ADV){
    if(pAdvertising->isAdvertising() == false && !shallStartServices){ // first advertisement
      vTaskDelay(1000/ portTICK_PERIOD_MS);   // work around to prevent crash on start
      pAdvertising->start(0);
    }
  } else
  {
    pAdvertising->setScanResponseData(0, adv); // instance id 0
  }
#else
  NimBLEAdvertisementData adv;
  adv.addData((char *)&MI32.conCtx->buffer[1], MI32.conCtx->buffer[0]);
  if(MI32.conCtx->operation == BLE_OP_SET_ADV){
    pAdvertising->setAdvertisementData(adv); // replace whole advertisement with our custom data from the Berry side
    if(pAdvertising->isAdvertising() == false && !shallStartServices){ // first advertisement
      vTaskDelay(1000/ portTICK_PERIOD_MS);   // work around to prevent crash on start
      pAdvertising->start();
    }
  } else
  {
    pAdvertising->setScanResponseData(adv);
    pAdvertising->setScanResponse(true);
  }
#endif //CONFIG_BT_NIMBLE_EXT_ADV

  MI32.infoMsg = MI32_SERV_SCANRESPONSE_ADDED + (MI32.conCtx->operation - BLE_OP_SET_SCAN_RESP); // .. ADV or SCAN RESPONSE
  item.header.type = MI32.conCtx->operation;
  item.header.returnCharUUID = 0;
  item.header.handle = 0;
  xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t) + item.header.length, pdMS_TO_TICKS(20));
}

void MI32ServerSetCharacteristic(NimBLEServer *pServer, std::vector<NimBLEService*>& servicesToStart, bool &shallStartServices);
/**
 * @brief Create a characteristic or modify its value with data of the context
 * 
 * @param pServer - our server instance
 * @param servicesToStart - before the finish of the server construction, a characteristic and maybe the holding service will be created and added to this vector
 * @param shallStartServices - true, if the server construction is not finished by first setting of advertisement data
 */
void MI32ServerSetCharacteristic(NimBLEServer *pServer, std::vector<NimBLEService*>& servicesToStart, bool &shallStartServices){
  MI32.conCtx->error = MI32_CONN_NO_ERROR;
  NimBLEService *pService = pServer->getServiceByUUID(MI32.conCtx->serviceUUID); // retrieve ...
  if(pService == nullptr){
    pService = pServer->createService(MI32.conCtx->serviceUUID);  //... or create service.
    if(pService == nullptr){
      MI32.conCtx->error = MI32_CONN_NO_SERVICE;
      return;
    }

    if(shallStartServices){
      servicesToStart.push_back(pService);
    }
  }
  NimBLECharacteristic *pCharacteristic = pService->getCharacteristic(MI32.conCtx->charUUID); // again retrieve ...
  if(pCharacteristic == nullptr){
    uint32_t _writeRSP = MI32.conCtx->response ?  NIMBLE_PROPERTY::WRITE :  NIMBLE_PROPERTY::WRITE_NR;
    uint32_t _property = NIMBLE_PROPERTY::READ |
                         _writeRSP |
                         NIMBLE_PROPERTY::NOTIFY |
                         NIMBLE_PROPERTY::INDICATE; // default to "all"
    if(MI32.conCtx->hasArg1){
      _property = MI32.conCtx->arg1;    // override with optional argument
      // AddLog(LOG_LEVEL_DEBUG,PSTR("BLE: _property: %u"),_property);
    }
    pCharacteristic = pService->createCharacteristic(MI32.conCtx->charUUID,
                                                    _property);  //... or create characteristic.
    if(pCharacteristic == nullptr){
      MI32.conCtx->error = MI32_CONN_NO_CHARACTERISTIC;
      return;
    }
    pCharacteristic->setCallbacks(&MI32ChrCallback);
    MI32.infoMsg = MI32_SERV_CHARACTERISTIC_ADDED;
  }
  pCharacteristic->setValue(MI32.conCtx->buffer + 1, MI32.conCtx->buffer[0]); // set value
  pCharacteristic->notify(true); // TODO: fallback to indication
  struct{
    BLERingBufferItem_t header;
  } item;
  item.header.length = 0;
  item.header.type = BLE_OP_SET_CHARACTERISTIC;
  item.header.returnCharUUID = pCharacteristic->getUUID().getNative()->u16.value;
  item.header.handle = pCharacteristic->getHandle();
  xRingbufferSend(BLERingBufferQueue, (const void*)&item, sizeof(BLERingBufferItem_t), pdMS_TO_TICKS(1));
}

void MI32ServerTask(void *pvParameters){
  MI32.conCtx->error = MI32_CONN_NO_ERROR;
  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MI32ServerCallbacks());
  MI32.mode.readyForNextServerJob = 1;
  MI32.mode.deleteServerTask = 0;
  std::vector<NimBLEService*> servicesToStart;
  bool shallStartServices = true; //will start service at the first call MI32ServerSetAdv()

  for(;;){
    while(MI32.mode.triggerNextServerJob == 0){
      if(MI32.mode.deleteServerTask == 1){
        delete MI32.conCtx;
        pServer->stopAdvertising();
        MI32StartTask(MI32_TASK_SCAN);
        vRingbufferDelete(BLERingBufferQueue);
        BLERingBufferQueue = nullptr;
        MI32.conCtx = nullptr;
        vTaskDelete( NULL );
      }
      vTaskDelay(50/ portTICK_PERIOD_MS);
    }
    MI32.mode.readyForNextServerJob = 0;
    switch(MI32.conCtx->operation){
      case BLE_OP_SET_ADV: case BLE_OP_SET_SCAN_RESP:
        MI32ServerSetAdv(pServer, servicesToStart, shallStartServices);
        break;
      case BLE_OP_SET_CHARACTERISTIC:
        MI32ServerSetCharacteristic(pServer, servicesToStart, shallStartServices);
        break;
    }

    MI32.mode.triggerNextServerJob = 0;
    MI32.mode.readyForNextServerJob = 1;
    MI32.mode.triggerBerryServerCB = 1;
  }
}

/*********************************************************************************************\
 * parse the response from advertisements
\*********************************************************************************************/

void MI32parseMiBeacon(char * _buf, uint32_t _slot, uint16_t _bufSize){

  float _tempFloat;
  mi_beacon_t* _beacon = (mi_beacon_t*)_buf;
  mi_payload_t _payload;

  MIBLEsensors[_slot].lastCnt = _beacon->counter;

#ifdef USE_MI_EXT_GUI
  bitSet(MI32.widgetSlot,_slot);
#endif //USE_MI_EXT_GUI
if(_beacon->frame.includesObj == 0){
  if(_beacon->capability == 0x28) MIBLEsensors[_slot].status.isUnbounded = 1;
  return; //nothing to parse
}

int decryptRet = 0;
if(_beacon->frame.isEncrypted){
    MIBLEsensors[_slot].feature.needsKey = 1;
    decryptRet = MI32_decryptPacket(_buf,_bufSize, (uint8_t*)&_payload,_slot);
    // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: decryptRet: %d"),decryptRet);
  }
else{
  uint32_t _offset = (_beacon->frame.includesCapability)?0:1;
  size_t _payloadSize = *(uint8_t*)(&_beacon->payload.size - _offset);
  if(_beacon->frame.includesMAC && _beacon->frame.includesObj) {
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: offset %u, size: %u"),_offset,_payloadSize);
      memcpy((uint8_t*)&_payload,(uint8_t*)(&_beacon->payload)-_offset, _payloadSize + 3);
      // AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)&_payload,_payloadSize + 3);
      }
  }
if(decryptRet!=0){
  AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Decryption failed with error: %d for %u"),decryptRet, _slot);
  if (decryptRet == -1) MIBLEsensors[_slot].status.hasWrongKey = 1;
  return;
}

  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u with payload type: %02x"), MI32getDeviceName(_slot),_slot,_payload.type);
  switch(_payload.type){
    case 0x0002:
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("M32: obj id to pair: %02x"),_payload.objID); // pretty pointless for Tasmota
      break;
    case 0x1001:
      MIBLEsensors[_slot].feature.Btn = 1;
      if(_payload.Btn.type == 4){     //dimmer knob rotation
        MIBLEsensors[_slot].eventType.knob = 1;
        if(_payload.Btn.num == 0){
          MIBLEsensors[_slot].pressed = 0;
          MIBLEsensors[_slot].dimmer = _payload.Btn.value;
        }
        else {
          MIBLEsensors[_slot].pressed = 1;
          MIBLEsensors[_slot].dimmer = _payload.Btn.num;
        }
      MI32.mode.shallTriggerTele = 1;
      break; //To-Do: Map to HomeKit somehow or wait for real support of this device class  in HomeKit
      }
      if(_payload.Btn.num == 1 && MIBLEsensors[_slot].feature.knob){ //dimmer knob long press
        MIBLEsensors[_slot].longpress = _payload.Btn.value;
        MI32.mode.shallTriggerTele = 1;
        MIBLEsensors[_slot].eventType.longpress = 1;
        break;
      }
      // single, double, long
      MIBLEsensors[_slot].Btn = _payload.Btn.num;
      if(MIBLEsensors[_slot].feature.knob){
        MIBLEsensors[_slot].BtnType = _payload.Btn.value - 1;
      }
      else{
        MIBLEsensors[_slot].BtnType = _payload.Btn.type;
      }
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 1: U16:  %u Button"), MIBLEsensors[_slot].Btn );
    break;
    case 0x1004:
       MIBLEsensors[_slot].feature.temp = 1;
      _tempFloat=(float)(_payload.temp)/10.0f;
      if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        MIBLEsensors[_slot].eventType.temp = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode 4: temp updated"));
      }
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].temp_history, _tempFloat, 0);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 4: U16:  %u Temp"), _payload.temp );
    break;
    case 0x1006:
      MIBLEsensors[_slot].feature.hum = 1;
      _tempFloat=(float)(_payload.hum)/10.0f;
      if(_tempFloat<101){
        MIBLEsensors[_slot].hum=_tempFloat;
        MIBLEsensors[_slot].eventType.hum = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode 6: hum updated"));
      }
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].hum_history, _tempFloat, 1);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 6: U16:  %u Hum"), _payload.hum);
    break;
    case 0x1007:
      MIBLEsensors[_slot].feature.lux = 1;
      MIBLEsensors[_slot].lux=_payload.lux & 0x00ffffff;
      if(MIBLEsensors[_slot].type==MJYD2S){
        MIBLEsensors[_slot].eventType.noMotion  = 1;
      }
      MIBLEsensors[_slot].eventType.lux  = 1;
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].lux_history, (float)MIBLEsensors[_slot].lux, 2);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 7: U24: %u Lux"), _payload.lux & 0x00ffffff);
    break;
    case 0x1008:
      MIBLEsensors[_slot].feature.moist = 1;
      MIBLEsensors[_slot].moisture=_payload.moist;
      MIBLEsensors[_slot].eventType.moist  = 1;
      DEBUG_SENSOR_LOG(PSTR("Mode 8: moisture updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 8: U8: %u Moisture"), _payload.moist);
    break;
    case 0x1009:
      MIBLEsensors[_slot].feature.fert = 1;
      MIBLEsensors[_slot].fertility=_payload.fert;
      MIBLEsensors[_slot].eventType.fert  = 1;
      DEBUG_SENSOR_LOG(PSTR("Mode 9: fertility updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 9: U16: %u Fertility"), _payload.fert);
    break;
    case 0x100a: case 0x4803:
      MIBLEsensors[_slot].feature.bat = 1;
      if(MI32.option.ignoreBogusBattery){
        if(MIBLEsensors[_slot].type==LYWSD03MMC || MIBLEsensors[_slot].type==MHOC401){
          break;
        }
      }
      if(_payload.bat<101){
        MIBLEsensors[_slot].bat = _payload.bat;
        MIBLEsensors[_slot].eventType.bat  = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode a: U8: %u %%"), _payload.bat);
    break;
    case 0x100d:
      _tempFloat=(float)(_payload.HT.temp)/10.0f;
      if(_tempFloat<60){
          MIBLEsensors[_slot].temp = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("Mode d: temp updated"));
      }
      _tempFloat=(float)(_payload.HT.hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors[_slot].hum = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("Mode d: hum updated"));
      }
      MIBLEsensors[_slot].eventType.tempHum  = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode d: U16:  %x Temp U16: %x Hum"), _payload.HT.temp,  _payload.HT.hum);
    break;

    case 0x000f:
    // if (_payload.ten!=0) break;
      MIBLEsensors[_slot].feature.motion = 1;
      MIBLEsensors[_slot].feature.NMT = 1; //only driver based
      MIBLEsensors[_slot].eventType.motion = 1;
      MIBLEsensors[_slot].events++;
      MIBLEsensors[_slot].lux = _payload.lux & 0x00ffffff;
      MIBLEsensors[_slot].eventType.lux = 1;
      MIBLEsensors[_slot].NMT = 0;
      MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].lux_history, (float)MIBLEsensors[_slot].lux, 2);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("motion: primary"),MIBLEsensors[_slot].lux );
    break;
    case 0x1014:
      MIBLEsensors[_slot].feature.leak = 1;
      MIBLEsensors[_slot].leak = _payload.leak;
      MIBLEsensors[_slot].eventType.leak = 1;
      if(_payload.leak>0) MI32.mode.shallTriggerTele = 1;
      break;
    case 0x1017:
      MIBLEsensors[_slot].feature.NMT = 1;
      MIBLEsensors[_slot].NMT = _payload.NMT;
      MIBLEsensors[_slot].eventType.NMT = 1;
      MI32.mode.shallTriggerTele = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 17: NMT: %u seconds"), _payload.NMT);
    break;
    case 0x1019:
      MIBLEsensors[_slot].feature.door = 1;
      MIBLEsensors[_slot].door = _payload.door;
      MIBLEsensors[_slot].eventType.door = 1;
      MIBLEsensors[_slot].events++;
      MI32.mode.shallTriggerTele = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 19: %u"), _payload.door);
    break;
    case 0x4e0c:
      MIBLEsensors[_slot].feature.Btn = 1;
      MIBLEsensors[_slot].Btn = _payload.size; // a guess, makes sense for the XMWXKG01LM
      MIBLEsensors[_slot].BtnType = 0;
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      break;
    case 0x4e0d:
      MIBLEsensors[_slot].feature.Btn = 1;
      MIBLEsensors[_slot].Btn = _payload.size;
      MIBLEsensors[_slot].BtnType = 1;
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      break;
    case 0x4e0e:
      MIBLEsensors[_slot].feature.Btn = 1;
      MIBLEsensors[_slot].Btn = _payload.size;
      MIBLEsensors[_slot].BtnType = 2;
      MIBLEsensors[_slot].eventType.Btn = 1;
      MI32.mode.shallTriggerTele = 1;
      break;
    default:
      if (MIBLEsensors[_slot].type==NLIGHT){
        MIBLEsensors[_slot].eventType.motion = 1; //motion
        MIBLEsensors[_slot].events++;
        MIBLEsensors[_slot].NMT = 0;
        MI32.mode.shallTriggerTele = 1;
      }
      else{
        //unknown payload
        AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)_buf,_bufSize);
        AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)&_payload,_payload.size + 2);
      }
    break;
  }
  if(MIBLEsensors[_slot].eventType.raw == 0) return;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(MI32.option.directBridgeMode == 1) MI32.mode.shallTriggerTele = 1;
}

void MI32parseBTHomePacket(char * _buf, uint32_t length, uint8_t addr[6], int RSSI, const char* optionalName){
  const uint32_t _slot = MIBLEgetSensorSlot(addr, 0xb770, 0); // fake ID, constant fake counter
  if(_slot==0xff) return;

  auto &_sensor =  MIBLEsensors[_slot];
  if (optionalName[0] != '\0'){
    if(_sensor.name == nullptr){
      _sensor.name = new char[strlen(optionalName) + 1];
      strcpy(_sensor.name, optionalName);
    }
  }
  _sensor.RSSI = RSSI;

  BTHome_info_t info;
  info.byte_value = _buf[0];
  _sensor.feature.needsKey = info.encrypted;

  uint32_t idx = 1;
  while(idx < length - 1){
    switch(_buf[idx]){
      case 0x00:
        if(_buf[idx+1] == _sensor.lastCnt){
          return; // known packet
        }
        _sensor.lastCnt = _buf[idx+1];
        idx += 2;
        break;
      case 0x01:
        _sensor.bat = _buf[idx+1];
        _sensor.eventType.bat = 1;
        _sensor.feature.bat = 1;
        idx += 2;
      break;
      case 0x02:
        _sensor.temp = (int16_t)(_buf[idx+1]|_buf[idx+2] << 8)/100.0f;
        _sensor.eventType.temp = 1;
        _sensor.feature.temp = 1;
        MI32addHistory(_sensor.temp_history, _sensor.temp, 0);
        idx += 3;
        break;
      case 0x03:
       _sensor.hum = (uint16_t)(_buf[idx+1]|_buf[idx+2] << 8)/100.0f;
       _sensor.eventType.hum = 1;
       _sensor.feature.hum = 1;
       MI32addHistory(_sensor.hum_history, _sensor.hum, 1);
       idx += 3;
       break;
      case 0x0b:
        // power ??
        idx += 4;
        break;
      case 0x0c:
        //voltage
        idx += 3;
        break;
      case 0x10:
        // binary power on/off??
        idx += 2;
        break;
      default:
        AddLog(LOG_LEVEL_INFO,PSTR("M32: unknown BTHome data type: %u, discard rest of data buffer!"),_buf[idx]);
        AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)_buf,length);
        idx = length; // "break"
        break;
    }
  }
#ifdef USE_MI_EXT_GUI
  bitSet(MI32.widgetSlot,_slot);
#endif //USE_MI_EXT_GUI
 _sensor.shallSendMQTT = 1;
  if(MI32.option.directBridgeMode == 1) MI32.mode.shallTriggerTele = 1;
}

void MI32ParseATCPacket(char * _buf, uint32_t length, uint8_t addr[6], int RSSI){
  ATCPacket_t *_packet = (ATCPacket_t*)_buf;
  bool isATC = (length == 0x0d);
  uint32_t _slot;
  if (isATC)  _slot = MIBLEgetSensorSlot(_packet->MAC, 0x0a1c, _packet->A.frameCnt); // This must be a hard-coded fake ID
  else {
    MI32_ReverseMAC(_packet->MAC);
    _slot = MIBLEgetSensorSlot(_packet->MAC, 0x944a, _packet->P.frameCnt); // ... and again
  }
  if(_slot==0xff) return;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u"), MI32getDeviceName(_slot),_slot);

  MIBLEsensors[_slot].RSSI=RSSI;
  // MIBLEsensors[_slot].lastTime = Rtc.local_time;
  if(isATC){
    MIBLEsensors[_slot].temp = (float)(int16_t(__builtin_bswap16(_packet->A.temp)))/10.0f;
    MIBLEsensors[_slot].hum = (float)_packet->A.hum;
    MIBLEsensors[_slot].bat = _packet->A.batPer;
  }
  else{
    MIBLEsensors[_slot].temp = (float)(_packet->P.temp)/100.0f;
    MIBLEsensors[_slot].hum = (float)_packet->P.hum/100.0f;
    MIBLEsensors[_slot].bat = _packet->P.batPer;
  }

  MIBLEsensors[_slot].eventType.tempHum  = 1;
  MIBLEsensors[_slot].eventType.bat  = 1;
#ifdef USE_MI_EXT_GUI
  bitSet(MI32.widgetSlot,_slot);
  MI32addHistory(MIBLEsensors[_slot].temp_history, (float)MIBLEsensors[_slot].temp, 0);
  MI32addHistory(MIBLEsensors[_slot].hum_history, (float)MIBLEsensors[_slot].hum, 1);
#endif //USE_MI_EXT_GUI
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(MI32.option.directBridgeMode == 1) MI32.mode.shallTriggerTele = 1;
}

void MI32parseCGD1Packet(char * _buf, uint32_t length, uint8_t addr[6], int RSSI){ // no MiBeacon
  uint8_t _addr[6];
  memcpy(_addr,addr,6);
  uint32_t _slot = MIBLEgetSensorSlot(_addr, 0x0576, 0); // This must be hard-coded, no object-id in Cleargrass-packet, we have no packet counter too
  if(_slot==0xff) return;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u"), MI32getDeviceName(_slot),_slot);
  MIBLEsensors[_slot].RSSI=RSSI;
  // MIBLEsensors[_slot].lastTime = Rtc.local_time;
  cg_packet_t _packet;
  memcpy((char*)&_packet,_buf,sizeof(_packet));
  switch (_packet.mode){
    case 0x0401:
      float _tempFloat;
      _tempFloat=(float)(_packet.temp)/10.0f;
      if(_tempFloat<60){
          MIBLEsensors[_slot].temp = _tempFloat;
          MIBLEsensors[_slot].eventType.temp  = 1;
          DEBUG_SENSOR_LOG(PSTR("CGD1: temp updated"));
#ifdef USE_MI_EXT_GUI
          MI32addHistory(MIBLEsensors[_slot].temp_history, (float)MIBLEsensors[_slot].temp, 0);
#endif //USE_MI_EXT_GUI
      }
      _tempFloat=(float)(_packet.hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors[_slot].hum = _tempFloat;
          MIBLEsensors[_slot].eventType.hum  = 1;
          DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
#ifdef USE_MI_EXT_GUI
          MI32addHistory(MIBLEsensors[_slot].hum_history, (float)MIBLEsensors[_slot].hum, 1);
#endif //USE_MI_EXT_GUI
      }
      DEBUG_SENSOR_LOG(PSTR("CGD1: U16:  %x Temp U16: %x Hum"), _packet.temp,  _packet.hum);
      break;
    case 0x0102:
      if(_packet.bat<101){
      MIBLEsensors[_slot].bat = _packet.bat;
      MIBLEsensors[_slot].eventType.bat  = 1;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
      break;
    default:
      DEBUG_SENSOR_LOG(PSTR("M32: Unexpected CGD1-packet"));
  }
  if(MIBLEsensors[_slot].eventType.raw == 0) return;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(MI32.option.directBridgeMode == 1) MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_EXT_GUI
  bitSet(MI32.widgetSlot,_slot);
#endif //USE_MI_EXT_GUI
}

void MI32ParseResponse(char *buf, uint16_t bufsize, uint8_t addr[6], int RSSI) {
    if(bufsize<9) {  //9 is from the NLIGHT
      return;
    }
    uint16_t _type= buf[3]*256 + buf[2];
    // AddLog(LOG_LEVEL_INFO, PSTR("%02x %02x %02x %02x"),(uint8_t)buf[0], (uint8_t)buf[1],(uint8_t)buf[2],(uint8_t)buf[3]);
    uint16_t _slot = MIBLEgetSensorSlot(addr, _type, buf[4]);
    if(_slot!=0xff) {
      MIBLEsensors[_slot].RSSI=RSSI;
      MI32parseMiBeacon(buf,_slot,bufsize);
    }
}

void MI32HandleEveryDevice(NimBLEAdvertisedDevice* advertisedDevice, uint8_t addr[6], int RSSI) {
    if(advertisedDevice->getAddressType() != BLE_ADDR_PUBLIC) {
      return;
    }
    uint16_t _slot = MIBLEgetSensorSlot(addr, 0, 0);
    if(_slot==0xff) {
      return;
    }
    auto &_sensor = MIBLEsensors[_slot];
    if (advertisedDevice->haveName()){
      if(_sensor.name == nullptr){
        std::string name = advertisedDevice->getName();
        _sensor.name = new char[name.length() + 1];
        strcpy(_sensor.name, name.c_str());
      }
    }
    if(_sensor.payload == nullptr) {
      _sensor.payload = new uint8_t[64]();
    }
    if(_sensor.payload != nullptr) {
      memcpy(_sensor.payload, advertisedDevice->getPayload(), advertisedDevice->getPayloadLength());
      _sensor.payload_len = advertisedDevice->getPayloadLength();
      bitSet(MI32.widgetSlot,_slot);
      MI32addHistory(_sensor.temp_history, 0.0f, 3); // reuse temp_history as sighting history
      _sensor.RSSI=RSSI;
      _sensor.feature.payload = 1;
      _sensor.eventType.payload = 1;
      if(MI32.option.directBridgeMode == 1){
        MI32.mode.shallTriggerTele = 1;
        _sensor.shallSendMQTT = 1;
      } 
    }
}

/**
 * Called automatically every 50 milliseconds or can be triggered from Berry with BLE.loop() - useful from fast_loop
*/
void MI32BLELoop()
{
  if(MI32.mode.triggerBerryAdvCB == 1){
    if(MI32.beAdvCB != nullptr){
        // AddLogBuffer(LOG_LEVEL_DEBUG,MI32.beAdvBuf,40);
        uint8_t _index = 9; // is the first byte of payload in the advertisement buffer
        int _svc = 0;
        int _manu = 0;
        while(_index < 9 + MI32.beAdvBuf[8]){ //index of payload + _packet->length
          if(MI32.beAdvBuf[_index+1] == 0x16){
            _svc = _index + 2;
          }
          else if(MI32.beAdvBuf[_index+1] == 0xff){
            _manu = _index + 2;
          }
          _index += MI32.beAdvBuf[_index] + 1;
        }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: svc:%u , manu:%u"),_svc,_manu);
      void (*func_ptr)(int,int) = (void (*)(int,int))MI32.beAdvCB;
      func_ptr(_svc,_manu);
    }
    MI32.mode.triggerBerryAdvCB = 0;
  }

  // client callback
  // handle notification queue only if there is no message from read/write or subscribe, which is prioritized
  if(MI32.mode.connected == 1 && BLERingBufferQueue != nullptr && MI32.mode.triggerBerryConnCB == 0) {
    size_t size;
    BLERingBufferItem_t *q = (BLERingBufferItem_t *)xRingbufferReceive(BLERingBufferQueue, &size, pdMS_TO_TICKS(1));
    
    if(q != nullptr){
      if(q->length != 0){
        memcpy(MI32.conCtx->buffer,&q->length,q->length + 1);
      }
      MI32.conCtx->returnCharUUID = q->returnCharUUID;
      MI32.conCtx->handle = q->handle;
      MI32.conCtx->operation = 103;
      MI32.conCtx->error = 0;
      vRingbufferReturnItem(BLERingBufferQueue, (void *)q);
      MI32.mode.triggerBerryConnCB = 1;
    }
  }
  if(MI32.mode.triggerBerryConnCB == 1){
    if(MI32.beConnCB != nullptr){
      void (*func_ptr)(int, int, int, int) = (void (*)(int, int, int, int))MI32.beConnCB;
      char _message[32];
      GetTextIndexed(_message, sizeof(_message), MI32.conCtx->error, kMI32_ConnErrorMsg);
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: BryCbMsg: %s"),_message);
      func_ptr(MI32.conCtx->error, MI32.conCtx->operation , MI32.conCtx->returnCharUUID, MI32.conCtx->handle);
    }
    MI32.mode.triggerBerryConnCB = 0;
  }

  // server callback
  if(MI32.mode.connected == 0 && BLERingBufferQueue != nullptr){
    size_t size;
    BLERingBufferItem_t *q = (BLERingBufferItem_t *)xRingbufferReceive(BLERingBufferQueue, &size, pdMS_TO_TICKS(1));
    
    if(q != nullptr){
      if(q->length != 0){
        memcpy(MI32.conCtx->buffer,&q->length,q->length + 1);
      }
      MI32.conCtx->buffer[0] = q->length;
      MI32.conCtx->returnCharUUID = q->returnCharUUID;
      MI32.conCtx->handle = q->handle;
      MI32.conCtx->operation = q->type;
      MI32.conCtx->error = 0;
      vRingbufferReturnItem(BLERingBufferQueue, (void *)q);
      if(MI32.beServerCB != nullptr){
        void (*func_ptr)(int, int, int, int) = (void (*)(int, int, int, int))MI32.beServerCB;
        char _message[32];
        GetTextIndexed(_message, sizeof(_message), MI32.conCtx->error, kMI32_ConnErrorMsg);
        AddLog(LOG_LEVEL_DEBUG,PSTR("M32: BryCbMsg: %s"),_message);
        func_ptr(MI32.conCtx->error, MI32.conCtx->operation , MI32.conCtx->returnCharUUID, MI32.conCtx->handle);
      }
    }
  }
  if(MI32.infoMsg > 0){
    char _message[32];
    GetTextIndexed(_message, sizeof(_message), MI32.infoMsg-1, kMI32_BLEInfoMsg);
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s"),_message);
    MI32.infoMsg = 0;
  }
}

/**
 * @brief Launch functions from Core 1 to make race conditions less likely
 *
 */

void MI32Every50mSecond(){
  if(MI32.mode.shallTriggerTele == 1){
      MI32.mode.shallTriggerTele = 0;
      MI32triggerTele();
  }
  MI32BLELoop();
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void MI32EverySecond(bool restart){

  for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
    if(MIBLEsensors[i].type==NLIGHT || MIBLEsensors[i].type==MJYD2S){
      MIBLEsensors[i].NMT++;
    }
  }

  // should not be needed with a stable BLE stack
  if(MI32.role == 1 && MI32.mode.runningScan == 0){
    AddLog(LOG_LEVEL_INFO,PSTR("BLE: restart scan"));
    MI32StartTask(MI32_TASK_SCAN);
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void CmndMi32Key(void) {
  if (44 == XdrvMailbox.data_len || 36 == XdrvMailbox.data_len) {  // a KEY-MAC-string
    mi_bindKey_t keyMAC;
    MI32HexStringToBytes(XdrvMailbox.data,keyMAC.buf);
    if(36 == XdrvMailbox.data_len){
      memmove(keyMAC.buf + 10, keyMAC.buf + 6, 12);
      const uint8_t _fillbytes[4] = {0x8d,0x3d,0x3c,0x97}; // only valid for YLKG08 and YLKG07 ??
      memcpy(keyMAC.buf + 6,_fillbytes,4);
      AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*) keyMAC.buf, 16);
    }
    MI32AddKey(keyMAC);
    ResponseCmndDone();
  }
}

void CmndMi32Name(void) {
  if(XdrvMailbox.index > MIBLEsensors.size() - 1){
    ResponseCmndDone();
    return;
  }
  if(MIBLEsensors[XdrvMailbox.index].name != nullptr){
    delete []MIBLEsensors[XdrvMailbox.index].name;
  }
  if(XdrvMailbox.data_len==0){
    MIBLEsensors[XdrvMailbox.index].name = nullptr;
  }
  else{
    MIBLEsensors[XdrvMailbox.index].name = new char[XdrvMailbox.data_len + 1];
    strcpy(MIBLEsensors[XdrvMailbox.index].name,XdrvMailbox.data);
  }
  ResponseCmndChar((const char*)MI32getDeviceName(XdrvMailbox.index));
}

void CmndMi32Cfg(void) {
  MI32saveConfig();
  ResponseCmndDone();
}

void CmndMi32Option(void){
  bool onOff = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      if(XdrvMailbox.data_len>0){
        MI32.option.allwaysAggregate = onOff;
      }
      else{
        onOff = MI32.option.allwaysAggregate;
      }
      break;
    case 1:
      if(XdrvMailbox.data_len>0){
        MI32.option.noSummary = onOff;
      }
      else{
        onOff = MI32.option.noSummary;
      }
      break;
    case 2:
      if(XdrvMailbox.data_len>0){
        MI32.option.directBridgeMode = onOff;
      }
      else{
        onOff = MI32.option.directBridgeMode;
      }
      break;
    case 3:
     if(XdrvMailbox.data_len>0){
        MI32.mode.didGetConfig = onOff;
      }
      else{
        onOff = MI32.mode.didGetConfig;
      }
      break;
    case 4:
      if(XdrvMailbox.data_len>0){
        if(MI32.option.activeScan != onOff){
          MI32.option.activeScan = onOff;
          if(MI32.mode.runningScan == 1){
            MI32.mode.updateScan = 1;
          }
          else{
            MI32StartTask(MI32_TASK_SCAN);
          }
        }
      }
      else{
        onOff = MI32.option.activeScan;
      }
      break;
    case 5:
     if(XdrvMailbox.data_len>0){
        MI32.option.handleEveryDevice = onOff;
      }
      else{
        onOff = MI32.option.handleEveryDevice;
      }
      break;
#ifdef CONFIG_BT_NIMBLE_NVS_PERSIST
    case 99: // TODO: should be moved to some reset command, i.e. "reset 6"
       NimBLEDevice::deleteAllBonds();
       AddLog(LOG_LEVEL_DEBUG,PSTR("NVS: deleteAllBonds"));
      break;
#endif //CONFIG_BT_NIMBLE_NVS_PERSIST
  }
  ResponseCmndIdxNumber(onOff?1:0);
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/
#ifdef USE_MI_EXT_GUI
bool MI32HandleWebGUIResponse(void){
  char tmp[16];
  WebGetArg(PSTR("wi"), tmp, sizeof(tmp));
  if (strlen(tmp)) {
    WSContentBegin(200, CT_PLAIN);                         
    if(MI32.widgetSlot!=0){
      for(uint32_t i=0;i<32;i++){
        if(bitRead(MI32.widgetSlot,i)){
          MI32sendWidget(i);
          bitClear(MI32.widgetSlot,i);
          break;
        }
      }
    }
    WSContentEnd();
    return true;
  }
  return false;
}

#ifdef USE_MI_ESP32_ENERGY
//https://gist.github.com/LingDong-/7e4c4cae5cbbc44400a05fba65f06f23
// used for logarithmic mapping of 0 - 3600 watts to 0-20 pixel - TaylorLog did not work as expected
float MI32ln(float x) {
  unsigned int bx = * (unsigned int *) (&x);
  unsigned int ex = bx >> 23;
  signed int t = (signed int)ex-(signed int)127;
  unsigned int s = (t < 0) ? (-t) : t;
  bx = 1065353216 | (bx & 8388607);
  x = * (float *) (&bx);
  return -1.49278+(2.11263+(-0.729104+0.10969*x)*x)*x+0.6931471806*t;
}
#endif //USE_MI_ESP32_ENERGY

void MI32createPolyline(char *polyline, uint8_t *history){
  uint32_t _pos = 0;
  uint32_t _inc = 0;
  for (uint32_t i = 0; i<24;i++){
    uint32_t y = 21-MI32fetchHistory(history,i);
    if (y>20){
      y = 150; //create a big gap in the graph to represent invalidated data
    }
    _inc = snprintf_P(polyline+_pos,10,PSTR("%u,%u "),i*6,y);
    _pos+=_inc;
  }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: polyline: %s"),polyline);
}

#ifdef USE_MI_ESP32_ENERGY
void MI32sendEnergyWidget(){
  if (Energy->current_available && Energy->voltage_available) {
    WSContentSend_P(HTTP_MI32_POWER_WIDGET,MIBLEsensors.size()+1, Energy->voltage,Energy->current[1]);
    char _polyline[176];
    MI32createPolyline(_polyline,MI32.energy_history);
    WSContentSend_P(PSTR("<p>" D_POWERUSAGE ": %.1f " D_UNIT_WATT ""),Energy->active_power);
    WSContentSend_P(HTTP_MI32_GRAPH,_polyline,185,124,124,_polyline,1);
    WSContentSend_P(PSTR("</p></div>"));
  }
}
#endif //USE_MI_ESP32_ENERGY
#ifdef USE_WEBCAM
void MI32sendCamWidget(){
#ifndef USE_BERRY_CAM
    if (Wc.CamServer && Wc.up) {
      WSContentSend_P(PSTR("<div class='box"));
      if(Settings->webcam_config.resolution>7){
        WSContentSend_P(PSTR(" big"));
      }
      WSContentSend_P(PSTR("' id='cam' style='background-image:url(http://%_I:81/stream);background-repeat:no-repeat;background-size:cover;'></div>"),
        (uint32_t)WiFi.localIP());
    }
#endif //USE_BERRY_CAM
}
#endif //USE_WEBCAM

void MI32sendWidget(uint32_t slot){
  auto _sensor = MIBLEsensors[slot];
  char _MAC[13];
  ToHex_P(_sensor.MAC,6,_MAC,13);
  uint32_t _opacity = 1;
  if(_sensor.RSSI == 0){
    _opacity=0;
  }
  char _key[33] ={0};
  if(_sensor.key!=nullptr){
    ToHex_P(_sensor.key,16,_key,33);
  }
  else if(_sensor.feature.needsKey == 1){
    snprintf_P(_key,32,PSTR("!! needs key !!"));
    _opacity=0;
  }
  if (_sensor.status.hasWrongKey == 1){
    snprintf_P(_key,32,PSTR("!! wrong key !!"));
    _opacity=0;
  }
  if (_sensor.status.isUnbounded == 1){
    if(_sensor.type != CGD1){ //only exception atm
      snprintf_P(_key,32,PSTR("!! not paired !!"));
      _opacity=0;
    }
  }
  if(_sensor.feature.raw == 0){ //no known data types to show
    _opacity=0;
  }

  char _bat[24];
  snprintf_P(_bat,24,PSTR("&#128267;%u%%"), _sensor.bat);
  if(!_sensor.feature.bat) _bat[0] = 0;
  if (_sensor.bat == 0) _bat[9] = 0;
  WSContentSend_P(HTTP_MI32_WIDGET,slot+1,_opacity,_MAC,_sensor.RSSI,_bat,_key,MI32getDeviceName(slot));

  if(_sensor.feature.temp == 1 && _sensor.feature.hum == 1){
    if(!isnan(_sensor.temp)){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.temp_history);
      WSContentSend_P(PSTR("<p>" D_JSON_TEMPERATURE ": %.1f °C"),_sensor.temp);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,185,124,124,_polyline,1);
      WSContentSend_P(PSTR("</p>"));
    }
    if(!isnan(_sensor.hum)){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.hum_history);
      WSContentSend_P(PSTR("<p>" D_JSON_HUMIDITY ": %.1f %%"),_sensor.hum);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,151,190,216,_polyline,2);
      WSContentSend_P(PSTR("</p>"));
    }
    if(!isnan(_sensor.temp) && !isnan(_sensor.hum)){
      WSContentSend_P(PSTR("" D_JSON_DEWPOINT ": %.1f °C"),CalcTempHumToDew(_sensor.temp,_sensor.hum));
    }
  }
  else if(_sensor.feature.temp == 1){
    if(!isnan(_sensor.temp)){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.temp_history);
      WSContentSend_P(PSTR("<p>" D_JSON_TEMPERATURE ": %.1f °C"),_sensor.temp);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,185,124,124,_polyline,1);
      WSContentSend_P(PSTR("</p>"));
    }
  }
  if(_sensor.feature.lux == 1){
    if(_sensor.lux!=0x00ffffff){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.lux_history);
      WSContentSend_P(PSTR("<p>" D_JSON_ILLUMINANCE ": %d Lux"),_sensor.lux);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,242,240,176,_polyline,3);
      WSContentSend_P(PSTR("</p>"));
    }
  }
  if(_sensor.feature.knob == 1){
      if(_sensor.pressed == 0) {
        WSContentSend_P(PSTR("<p>Dimmer Steps: %d</p>"),_sensor.dimmer);
      }
      else {
        WSContentSend_P(PSTR("<p>Dimmer Steps pressed: %d</p>"),_sensor.dimmer);
      }
      WSContentSend_P(PSTR("<p>Hold: %u</p>"),_sensor.longpress);
  }
  if(_sensor.feature.Btn == 1){
      char _message[16];
      GetTextIndexed(_message, sizeof(_message), _sensor.BtnType, kMI32_ButtonMsg);
      if(_sensor.Btn<12) WSContentSend_P(PSTR("<p>Button%u: %s</p>"),_sensor.Btn,_message);
  }
  if(_sensor.feature.motion == 1){
      WSContentSend_P(PSTR("<p>Events: %u</p>"),_sensor.events);
      WSContentSend_P(PSTR("<p>No motion for > <span class='Ti'>%u</span> seconds</p>"),_sensor.NMT);
  }
  if(_sensor.feature.door == 1){
    if(_sensor.door!=255){
      if(_sensor.door==1){
        WSContentSend_P(PSTR("<p>Contact open</p>"));
      }
      else{
        WSContentSend_P(PSTR("<p>Contact closed</p>"));
      }
      WSContentSend_P(PSTR("<p>Events: %u</p>"),_sensor.events);
    }
  }
  if(_sensor.feature.leak == 1){
    if(_sensor.leak==1){
      WSContentSend_P(PSTR("<p>Leak !!!</p>"));
    }
    else{
      WSContentSend_P(PSTR("<p>No leak</p>"));
    }
  }
  if(_sensor.feature.payload == 1){
    if(_sensor.payload != nullptr){
      char _payload[128];
      char _polyline[176];
      ToHex_P((const unsigned char*)_sensor.payload,_sensor.payload_len,_payload, (_sensor.payload_len * 2) + 1);
      MI32createPolyline(_polyline,_sensor.temp_history);
      WSContentSend_P(PSTR("<p>Payload:"));
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,60,240,176,_polyline,4);
      WSContentSend_P(PSTR("</p><code style='word-break: break-all;'>%s</code>"),_payload);
    }
  }
  WSContentSend_P(PSTR("<p>Timestamp: %s</p>"),GetDT(_sensor.lastTime).c_str());
  WSContentSend_P(PSTR("</div>"));
}

void MI32InitGUI(void){
  MI32.widgetSlot=0;
  WSContentStart_P("m32");
  WSContentSend_P(HTTP_MI32_SCRIPT_1);
  WSContentSendStyle();
  WSContentSend_P(HTTP_MI32_STYLE);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,1,185,124,124,185,124,124);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,2,151,190,216,151,190,216);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,3,242,240,176,242,240,176);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,4,60,240,176,60,240,176);

  char _role[16];
  GetTextIndexed(_role, sizeof(_role), MI32.role, HTTP_MI32_PARENT_BLE_ROLE);
  WSContentSend_P((HTTP_MI32_PARENT_START),MIBLEsensors.size(),UpTime(),ESP.getFreeHeap()/1024,_role);

  uint32_t _slot;
  for(_slot = 0;_slot<MIBLEsensors.size();_slot++){
    MI32sendWidget(_slot);
  }

#ifdef USE_MI_ESP32_ENERGY
  MI32sendEnergyWidget();
#endif //USE_MI_ESP32_ENERGY
#ifdef USE_WEBCAM
  MI32sendCamWidget();
#endif //USE_WEBCAM 
  WSContentSend_P(PSTR("</div>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
}

void MI32HandleWebGUI(void){
  if (!HttpCheckPriviledgedAccess()) { return; }
  if (MI32HandleWebGUIResponse()) { return; }
  MI32InitGUI();
}
#endif //USE_MI_EXT_GUI

const char HTTP_MI32[] PROGMEM = "{s}Mi ESP32 {m} %u devices{e}";

#ifndef USE_MI_EXT_GUI
const char HTTP_BATTERY[] PROGMEM = "{s}%s " D_BATTERY "{m}%u %%{e}";
const char HTTP_LASTBUTTON[] PROGMEM = "{s}%s Last Button{m}%u {e}";
const char HTTP_EVENTS[] PROGMEM = "{s}%s Events{m}%u {e}";
const char HTTP_NMT[] PROGMEM = "{s}%s No motion{m}> %u seconds{e}";
const char HTTP_DOOR[] PROGMEM = "{s}%s Door{m}> %u open/closed{e}";
const char HTTP_MI32_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%u us/cm{e}";
#endif //USE_MI_EXT_GUI
const char HTTP_MI32_MAC[] PROGMEM = "{s}%s %s{m}%s{e}";
const char HTTP_MI32_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";
const char HTTP_RSSI[] PROGMEM = "{s}%s " D_RSSI "{m}%d dBm{e}";

void MI32ShowContinuation(bool *commaflg) {
  if (*commaflg) {
    ResponseAppend_P(PSTR(","));
  } else {
    *commaflg = true;
  }
}

void MI32Show(bool json)
{
  if (json) {
    if(MI32.mode.triggeredTele == 0){
      if(MI32.option.noSummary == 1) return; // no message at TELEPERIOD
      }
    if(TasmotaGlobal.masterlog_level == LOG_LEVEL_DEBUG_MORE) return; // we want to announce sensors unlinked to the ESP, check for LOG_LEVEL_DEBUG_MORE is medium-safe
    MI32suspendScanTask();
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      if(MI32.mode.triggeredTele == 1 && MIBLEsensors[i].eventType.raw == 0) continue;
      if(MI32.mode.triggeredTele == 1 && MIBLEsensors[i].shallSendMQTT==0) continue;

      bool commaflg = false;
      if( MIBLEsensors[i].name == nullptr){
        ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),
          MI32getDeviceName(i),
          MIBLEsensors[i].MAC[3], MIBLEsensors[i].MAC[4], MIBLEsensors[i].MAC[5]);
      }
      else{
        ResponseAppend_P(PSTR(",\"%s\":{"),
          MI32getDeviceName(i));
      }

      if(MI32.mode.triggeredTele == 1){
        bool tempHumSended = false;
        if(MIBLEsensors[i].feature.tempHum == 1){
          if(MIBLEsensors[i].eventType.tempHum == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
            if (!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppendTHD(MIBLEsensors[i].temp, MIBLEsensors[i].hum);
              tempHumSended = true;
            }
          }
        }
        if(MIBLEsensors[i].feature.temp == 1 && !tempHumSended){
          if(MIBLEsensors[i].eventType.temp == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1) {
            if (!isnan(MIBLEsensors[i].temp)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%*_f"),
                Settings->flag2.temperature_resolution, &MIBLEsensors[i].temp);
            }
          }
        }
        if(MIBLEsensors[i].feature.hum == 1 && !tempHumSended){
          if(MIBLEsensors[i].eventType.hum == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1) {
            if (!isnan(MIBLEsensors[i].hum)) {
              char hum[FLOATSZ];
              dtostrfd(MIBLEsensors[i].hum, Settings->flag2.humidity_resolution, hum);
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_HUMIDITY "\":%s"), hum);
            }
          }
        }
        if (MIBLEsensors[i].feature.lux == 1){
          if(MIBLEsensors[i].eventType.lux == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
            if ((MIBLEsensors[i].lux != 0x0ffffff)) { // this is the error code -> no lux
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "\":%u"), MIBLEsensors[i].lux);
            }
          }
        }
        if (MIBLEsensors[i].feature.moist == 1){
          if(MIBLEsensors[i].eventType.moist == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
            if ((MIBLEsensors[i].moisture != 0xff)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_MOISTURE "\":%u"), MIBLEsensors[i].moisture);
            }
          }
        }
        if (MIBLEsensors[i].feature.fert){
          if(MIBLEsensors[i].eventType.fert || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
            if ((MIBLEsensors[i].fertility != 0xffff)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"Fertility\":%u"), MIBLEsensors[i].fertility);
            }
          }
        }
        if (MIBLEsensors[i].feature.Btn == 1){
          if(MIBLEsensors[i].eventType.Btn == 1){
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Button%u\":%u"),MIBLEsensors[i].Btn,MIBLEsensors[i].BtnType + 1); //internal type is Xiaomi/Homekit 0,1,2 -> Tasmota 1,2,3
          }
        }
        if (MIBLEsensors[i].feature.knob == 1){
          if(MIBLEsensors[i].eventType.knob == 1){
            MI32ShowContinuation(&commaflg);
            char _pressed[3] = {'_','P',0};
            if (MIBLEsensors[i].pressed == 0){
              _pressed[0] = 0;
            }
            ResponseAppend_P(PSTR("\"Dimmer%s\":%d"),_pressed, MIBLEsensors[i].dimmer);
          }
          if(MIBLEsensors[i].eventType.longpress == 1){
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Hold\":%d"), MIBLEsensors[i].longpress);
          }
        }
      } // minimal summary
      if (MIBLEsensors[i].feature.motion == 1){
        if(MIBLEsensors[i].eventType.motion == 1 || MI32.mode.triggeredTele == 0){
          if(MI32.mode.triggeredTele == 1) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Motion\":1")); // only real-time
          }
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Events\":%u"),MIBLEsensors[i].events);
        }
        else if(MIBLEsensors[i].eventType.noMotion && MI32.mode.triggeredTele){
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Motion\":0"));
        }
      }

      if (MIBLEsensors[i].feature.door == 1){
        if(MIBLEsensors[i].eventType.door == 1 || MI32.mode.triggeredTele == 0){
          if(MI32.mode.triggeredTele == 1) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Door\":%u"),MIBLEsensors[i].door);
          }
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Events\":%u"),MIBLEsensors[i].events);
        }
      }

      if (MIBLEsensors[i].type == FLORA && MI32.mode.triggeredTele == 0) {
        if (MIBLEsensors[i].firmware[0] != '\0') { // this is the error code -> no firmware
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Firmware\":\"%s\""), MIBLEsensors[i].firmware);
        }
      }

      if (MIBLEsensors[i].feature.NMT  == 1|| MI32.mode.triggeredTele == 0){
        if(MIBLEsensors[i].eventType.NMT == 1){
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"NMT\":%u"), MIBLEsensors[i].NMT);
        }
      }
      if (MIBLEsensors[i].feature.bat == 1){
        if(MIBLEsensors[i].eventType.bat == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
          if ((MIBLEsensors[i].bat != 0x00)) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Battery\":%u"), MIBLEsensors[i].bat);
          }
        }
      }
      if (MIBLEsensors[i].feature.payload == 1){
        if(MIBLEsensors[i].eventType.payload == 1 || MI32.mode.triggeredTele == 0 || MI32.option.allwaysAggregate == 1){
          if ((MIBLEsensors[i].payload != nullptr)) {
            MI32ShowContinuation(&commaflg);
            char _payload[128];
            ToHex_P((const unsigned char*)MIBLEsensors[i].payload,MIBLEsensors[i].payload_len,_payload, (MIBLEsensors[i].payload_len * 2) + 1);
            ResponseAppend_P(PSTR("\"Payload\":\"%s\""),_payload);
          }
        }
      }
      MI32ShowContinuation(&commaflg);
      ResponseAppend_P(PSTR("\"RSSI\":%d,"), MIBLEsensors[i].RSSI);
      if(MI32.mode.triggeredTele == 0){
        ResponseAppend_P(PSTR("\"Time\":%d,"), MIBLEsensors[i].lastTime);
      }
      ResponseAppend_P(PSTR("\"MAC\":\"%02X%02X%02X%02X%02X%02X\""),MIBLEsensors[i].MAC[0],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[5]);
      ResponseJsonEnd();

      MIBLEsensors[i].eventType.raw = 0;
      if(MIBLEsensors[i].shallSendMQTT==1){
        MIBLEsensors[i].shallSendMQTT = 0;
        continue;
      }
    }
    MI32.mode.triggeredTele = 0;
#ifdef USE_MI_EXT_GUI
    Mi32invalidateOldHistory();
#ifdef USE_MI_ESP32_ENERGY
    MI32addHistory(MI32.energy_history,Energy->active_power[0],100); //TODO: which value??
#endif //USE_MI_ESP32_ENERGY
#endif //USE_MI_EXT_GUI
    MI32resumeScanTask();
#ifdef USE_WEBSERVER
    } else {
      MI32suspendScanTask();

      WSContentSend_P(HTTP_MI32, MIBLEsensors.size());

#ifndef USE_MI_EXT_GUI
      for (uint32_t i = 0; i<MIBLEsensors.size(); i++) {
        WSContentSend_PD(HTTP_MI32_HL);
        char _MAC[18];
        ToHex_P(MIBLEsensors[i].MAC,6,_MAC,18,':');
        const char * _sensorName = MI32getDeviceName(i);
        WSContentSend_P(HTTP_MI32_MAC, _sensorName, D_MAC_ADDRESS, _MAC);
        WSContentSend_P(HTTP_RSSI, _sensorName, MIBLEsensors[i].RSSI);
        if (MIBLEsensors[i].type==FLORA) {
          if (!isnan(MIBLEsensors[i].temp)) {
            WSContentSend_Temp(_sensorName, MIBLEsensors[i].temp);
          }
          if (MIBLEsensors[i].moisture!=0xff) {
            WSContentSend_PD(HTTP_SNS_MOISTURE, _sensorName, MIBLEsensors[i].moisture);
          }
          if (MIBLEsensors[i].fertility!=0xffff) {
            WSContentSend_PD(HTTP_MI32_FLORA_DATA, _sensorName, MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA) { // everything "above" Flora
          if (!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)) {
            WSContentSend_THD(_sensorName, MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        }
        if (MIBLEsensors[i].feature.needsKey == 1) {
          if(MIBLEsensors[i].key == nullptr){
            WSContentSend_PD(PSTR("{s}No known Key!!{m} can not decrypt messages{e}"));
          }
          else if(MIBLEsensors[i].status.hasWrongKey == 1){
            WSContentSend_PD(PSTR("{s}Wrong Key!!{m} can not decrypt messages{e}"));
          }
        }
        if (MIBLEsensors[i].type==NLIGHT || MIBLEsensors[i].type==MJYD2S) {
          WSContentSend_PD(HTTP_EVENTS, _sensorName, MIBLEsensors[i].events);
          if(MIBLEsensors[i].NMT>0) WSContentSend_PD(HTTP_NMT, _sensorName, MIBLEsensors[i].NMT);
        }
        if(MIBLEsensors[i].door != 255 && MIBLEsensors[i].type==MCCGQ02){
          WSContentSend_PD(HTTP_DOOR, _sensorName, MIBLEsensors[i].door);
        }
        if (MIBLEsensors[i].lux!=0x00ffffff) { // this is the error code -> no valid value
          WSContentSend_PD(HTTP_SNS_ILLUMINANCE, _sensorName, MIBLEsensors[i].lux);
        }
        if(MIBLEsensors[i].bat!=0x00){
            WSContentSend_PD(HTTP_BATTERY, _sensorName, MIBLEsensors[i].bat);
        }
        if (MIBLEsensors[i].type==YLYK01){
          WSContentSend_PD(HTTP_LASTBUTTON, _sensorName, MIBLEsensors[i].Btn);
        }
      }
#endif //USE_MI_EXT_GUI
#endif  // USE_WEBSERVER
    }
    MI32resumeScanTask();
}

int ExtStopBLE(){
      if(Settings->flag5.mi32_enable == 0) return 0;
      if (MI32.ScanTask != nullptr){
        MI32.mode.deleteScanTask = 1;
        MI32.role = 0;
        AddLog(LOG_LEVEL_INFO,PSTR("M32: stop BLE"));
        while (MI32.mode.runningScan == 1) delay(5);
      }
      return 0;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns62(uint32_t function)
{
  if (!Settings->flag5.mi32_enable) { return false; }  // SetOption115 - Enable ESP32 MI32 BLE

  bool result = false;

  if (FUNC_INIT == function){
    MI32PreInit();
  }

  if (!MI32.mode.init) {
    if (function == FUNC_EVERY_250_MSECOND) {
      MI32Init();
    }
    return result;
  }
  switch (function) {
    case FUNC_EVERY_50_MSECOND:
      MI32Every50mSecond();
      break;
    case FUNC_EVERY_SECOND:
      MI32EverySecond(false);
      break;
    case FUNC_SAVE_BEFORE_RESTART:
    case FUNC_INTERRUPT_STOP:
      ExtStopBLE();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kMI32_Commands, MI32_Commands);
      break;
/*
    case FUNC_INTERRUPT_START:
      break;
*/
    case FUNC_JSON_APPEND:
      MI32Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      MI32Show(0);
      break;
#ifdef USE_MI_EXT_GUI
      case FUNC_WEB_ADD_MAIN_BUTTON:
        if (Settings->flag5.mi32_enable) WSContentSend_P(HTTP_BTN_MENU_MI32);
        break;
      case FUNC_WEB_ADD_HANDLER:
        WebServer_on(PSTR("/m32"), MI32HandleWebGUI);
        break;
#endif  //USE_MI_EXT_GUI
#endif  // USE_WEBSERVER
    }
  return result;
}
#endif  // USE_MI_ESP32
#endif  // CONFIG_IDF_TARGET_ESP32 or CONFIG_IDF_TARGET_ESP32C3
#endif  // ESP32
#endif  // USE_BLE_ESP32
