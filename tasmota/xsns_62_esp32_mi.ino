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
#if defined CONFIG_IDF_TARGET_ESP32 || defined CONFIG_IDF_TARGET_ESP32C3

#ifdef USE_MI_ESP32

#ifdef USE_ENERGY_SENSOR
// #define USE_MI_ESP32_ENERGY //prepare for some GUI extensions
#endif

#define XSNS_62                    62

#include <NimBLEDevice.h>
#include <vector>

#include <t_bearssl.h>

#include "xsns_62_esp32_mi.h"

#ifdef USE_MI_HOMEKIT
extern "C" void mi_homekit_main(void);
extern "C" void mi_homekit_update_value(void* handle, float value, uint32_t type);
extern "C" void mi_homekit_stop();
void MI32getSetupCodeFromMAC(char* code);
#endif //USE_MI_HOMEKIT


void MI32scanEndedCB(NimBLEScanResults results);
void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
void MI32AddKey(mi_bindKey_t keyMAC);

std::vector<mi_sensor_t> MIBLEsensors;

static BLEScan* MI32Scan;


/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

class MI32SensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pclient) {
    // AddLog(LOG_LEVEL_DEBUG,PSTR("connected %s"), kMI32DeviceType[(MIBLEsensors[MI32.conCtx->slot].type)-1]);
    MI32.infoMsg = MI32_DID_CONNECT;
    MI32.mode.willConnect = 0;
    MI32.mode.connected = 1;
  }
  void onDisconnect(NimBLEClient* pclient) {
    MI32.mode.connected = 0;
     MI32.infoMsg = MI32_DID_DISCONNECT;
    //AddLog(LOG_LEVEL_DEBUG,PSTR("disconnected"));
  }
  bool onConnParamsUpdateRequest(NimBLEClient* MI32Client, const ble_gap_upd_params* params) {
    if(params->itvl_min < 24) { /** 1.25ms units */
      return false;
    } else if(params->itvl_max > 40) { /** 1.25ms units */
      return false;
    } else if(params->latency > 2) { /** Number of intervals allowed to skip */
      return false;
    } else if(params->supervision_timeout > 100) { /** 10ms units */
      return false;
    }
    return true;
  }
};

class MI32AdvCallbacks: public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    static bool _mutex = false;
    if(_mutex) return;
    _mutex = true;

    int RSSI = advertisedDevice->getRSSI();
    uint8_t addr[6];
    memcpy(addr,advertisedDevice->getAddress().getNative(),6);
    MI32_ReverseMAC(addr);
    size_t ServiceDataLength = 0;


    if (advertisedDevice->getServiceDataCount() == 0) {
      if(MI32.beAdvCB != nullptr && MI32.mode.triggerBerryAdvCB == 0){
        berryAdvPacket_t *_packet = (berryAdvPacket_t *)MI32.beAdvBuf;
        memcpy(_packet->MAC,addr,6);
        _packet->addressType = advertisedDevice->getAddressType();
        _packet->svcUUID = 0;
        _packet->RSSI = (uint8_t)RSSI;
        _packet->length = ServiceDataLength;
        _packet->svcData[0] = 0; //guarantee it is zero!!
        if(advertisedDevice->haveManufacturerData()){
          std::string _md = advertisedDevice->getManufacturerData();
          _packet->svcData[0] = _md.size();
          memcpy((_packet->svcData)+ServiceDataLength+1,_md.data(), _md.size());
        }
        MI32.mode.triggerBerryAdvCB = 1;
      }
      _mutex = false;
      return;
    }
    uint16_t UUID = advertisedDevice->getServiceDataUUID(0).getNative()->u16.value;

    ServiceDataLength = advertisedDevice->getServiceData(0).length();
    if(MI32.beAdvCB != nullptr && MI32.mode.triggerBerryAdvCB == 0){
      berryAdvPacket_t *_packet = (berryAdvPacket_t *)MI32.beAdvBuf;
      memcpy(_packet->MAC,addr,6);
      _packet->addressType = advertisedDevice->getAddressType();
      _packet->svcUUID = UUID;
      _packet->RSSI = (uint8_t)RSSI;
      _packet->length = ServiceDataLength;
      memcpy(_packet->svcData,advertisedDevice->getServiceData(0).data(),ServiceDataLength);
      MI32.mode.triggerBerryAdvCB = 1;
    }

    if(UUID==0xfe95) {
      MI32ParseResponse((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    else if(UUID==0xfdcd) {
      MI32parseCGD1Packet((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    else if(UUID==0x181a) { //ATC and PVVX
      MI32ParseATCPacket((char*)advertisedDevice->getServiceData(0).data(),ServiceDataLength, addr, RSSI);
    }
    // else {
    //   MI32Scan->erase(advertisedDevice->getAddress());
    // }
  _mutex = false;
  };
};


static MI32AdvCallbacks MI32ScanCallbacks;
static MI32SensorCallback MI32SensorCB;
static NimBLEClient* MI32Client;

/*********************************************************************************************\
 * BLE callback functions
\*********************************************************************************************/

void MI32scanEndedCB(NimBLEScanResults results){
  MI32.infoMsg = MI32_SCAN_ENDED;
  MI32.mode.runningScan = 0;
}

void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    MI32.infoMsg = MI32_GOT_NOTIFICATION;
    MI32.conCtx->buffer[0] = (uint8_t)length;
    memcpy(MI32.conCtx->buffer + 1, pData, length);
    MI32.mode.triggerBerryConnCB = 1;
    MI32.mode.readingDone = 1;
}
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
 * @brief Convert string that repesents a hexadecimal number to a byte array
 *
 * @param _string input string in format: AABBCCDDEEFF or AA:BB:CC:DD:EE:FF, caseinsensitive
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
 * @param _mac a byte array of size 6 (typicalliy representing a MAC address)
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
  for(auto _sensor : MIBLEsensors){
    if(memcmp(keyMAC.MAC,_sensor.MAC,sizeof(keyMAC.MAC))==0){
      AddLog(LOG_LEVEL_DEBUG,PSTR("new key"));
      uint8_t* _key = (uint8_t*) malloc(16);
      memcpy(_key,keyMAC.key,16);
      _sensor.key = _key;
      unknownMAC=false;
      _sensor.status.hasWrongKey = 0;
      AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*) _sensor.key, 16);
    }
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
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: encrypted msg from %s with version:%u"),kMI32DeviceType[MIBLEsensors[_slot].type-1],_version);

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
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: decrypted in %.2f mSec"),enctime);
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
 * @param _MAC     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_MAC)[6], uint16_t _type, uint8_t counter){
  DEBUG_SENSOR_LOG(PSTR("%s: will test ID-type: %x"),D_CMND_MI32, _type);
  bool _success = false;
  for (uint32_t i=0;i<MI32_TYPES;i++){ // i < sizeof(kMI32DeviceID) gives compiler warning
    if(_type == kMI32DeviceID[i]){
      DEBUG_SENSOR_LOG(PSTR("M32: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("%s: ID-type is not: %x"),D_CMND_MI32,kMI32DeviceID[i]);
    }
  }
  if(!_success) return 0xff;

  DEBUG_SENSOR_LOG(PSTR("%s: vector size %u"),D_CMND_MI32, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_MAC,MIBLEsensors[i].MAC,sizeof(_MAC))==0){
      DEBUG_SENSOR_LOG(PSTR("%s: known sensor at slot: %u"),D_CMND_MI32, i);
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Counters: %x %x"),MIBLEsensors[i].lastCnt, counter);
      if(MIBLEsensors[i].lastCnt==counter) {
        // AddLog(LOG_LEVEL_DEBUG,PSTR("Old packet"));
        return 0xff; // packet received before, stop here
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: i: %x %x %x %x %x %x"),D_CMND_MI32, MIBLEsensors[i].MAC[5], MIBLEsensors[i].MAC[4],MIBLEsensors[i].MAC[3],MIBLEsensors[i].MAC[2],MIBLEsensors[i].MAC[1],MIBLEsensors[i].MAC[0]);
    DEBUG_SENSOR_LOG(PSTR("%s: n: %x %x %x %x %x %x"),D_CMND_MI32, _MAC[5], _MAC[4], _MAC[3],_MAC[2],_MAC[1],_MAC[0]);
  }
  if(MI32.mode.didGetConfig){
    DEBUG_SENSOR_LOG(PSTR("M32: ignore new sensor, because of loaded config"));
    return 0xff; //discard the data
  }
  DEBUG_SENSOR_LOG(PSTR("%s: found new sensor"),D_CMND_MI32);
  mi_sensor_t _newSensor;
  memcpy(_newSensor.MAC,_MAC, sizeof(_MAC));
  _newSensor.type = _type;
  _newSensor.eventType.raw = 0;
  _newSensor.feature.raw = 0;
  _newSensor.status.raw = 0;
  _newSensor.temp = NAN;
  _newSensor.temp_history = (uint8_t*) calloc(24,1);
  _newSensor.bat=0x00;
  _newSensor.RSSI=0;
  _newSensor.lux = 0x00ffffff;
  _newSensor.lux_history = (uint8_t*) calloc(24,1);
  _newSensor.key = nullptr;
  switch (_type)
    {
    case FLORA:
      _newSensor.moisture =0xff;
      _newSensor.fertility =0xffff;
      _newSensor.firmware[0]='\0';
      _newSensor.feature.temp=1;
      _newSensor.feature.moist=1;
      _newSensor.feature.fert=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
#ifdef USE_MI_HOMEKIT
      _newSensor.light_hap_service = nullptr;
#endif
      break;
    case NLIGHT:
      _newSensor.events=0x00;
      _newSensor.feature.motion=1;
      _newSensor.feature.NMT=1;
      _newSensor.NMT=0;
#ifdef USE_MI_HOMEKIT
      _newSensor.motion_hap_service = nullptr;
#endif //USE_MI_HOMEKIT
      break;
    case MJYD2S:
      _newSensor.NMT=0;
      _newSensor.events=0x00;
      _newSensor.feature.motion=1;
      _newSensor.feature.NMT=1;
      _newSensor.feature.lux=1;
      _newSensor.feature.bat=1;
#ifdef USE_MI_HOMEKIT
      _newSensor.light_hap_service = nullptr;
      _newSensor.motion_hap_service = nullptr;
#endif //USE_MI_HOMEKIT
      _newSensor.feature.bat=1;
      _newSensor.NMT=0;
      break;
    case YLYK01: case YLKG08:
      _newSensor.feature.Btn = 1;
      _newSensor.Btn = 99;
      if(_type == YLKG08){
        _newSensor.feature.knob = 1;
        _newSensor.dimmer = 0;
      }
#ifdef USE_MI_HOMEKIT
      _newSensor.button_hap_service[0] = nullptr;
#endif //USE_MI_HOMEKIT
      break;
    case MCCGQ02:
      _newSensor.events=0x00;
      _newSensor.feature.bat=1;
      _newSensor.feature.door=1;
#ifdef USE_MI_HOMEKIT
      _newSensor.door_sensor_hap_service = nullptr;
#endif //USE_MI_HOMEKIT
      _newSensor.door = 255;
      break;
    case SJWS01L:
      _newSensor.feature.leak=1;
      _newSensor.feature.bat=1;
      _newSensor.feature.Btn=1;
      _newSensor.Btn=99;
#ifdef USE_MI_HOMEKIT
      _newSensor.leak_hap_service = nullptr;
      _newSensor.bat_hap_service = nullptr;
      _newSensor.button_hap_service[0] = nullptr;
#endif //USE_MI_HOMEKIT
      break;
    default:
      _newSensor.hum=NAN;
      _newSensor.hum_history = (uint8_t*) calloc(24,1);
      _newSensor.feature.temp=1;
      _newSensor.feature.hum=1;
      _newSensor.feature.tempHum=1;
      _newSensor.feature.bat=1;
#ifdef USE_MI_HOMEKIT
      _newSensor.temp_hap_service = nullptr;
      _newSensor.hum_hap_service = nullptr;
      _newSensor.bat_hap_service = nullptr;
#endif //USE_MI_HOMEKIT
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog(LOG_LEVEL_DEBUG,PSTR("M32: new %s at slot: %u"),kMI32DeviceType[_type-1],MIBLEsensors.size()-1);
  MI32.mode.shallShowStatusInfo = 1;
  return MIBLEsensors.size()-1;
};

/**
 * @brief trigger real-time message for motion or RC
 *
 */
void MI32triggerTele(void){
  MI32.mode.triggeredTele = 1;
  MqttPublishTeleperiodSensor();
}

/**
 * @brief Is called after every finding of new BLE sensor
 *
 */
void MI32StatusInfo() {
  MI32.mode.shallShowStatusInfo = 0;
  Response_P(PSTR("{\"M32\":{\"found\":%u}}"), MIBLEsensors.size());
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
void MI32addHistory(uint8_t *history, float value, uint32_t type){
  uint32_t _hour = (LocalTime()%SECS_PER_DAY)/SECS_PER_HOUR;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history hour: %u"),_hour);
  switch(type){
    case 0:  //temperature
      history[_hour] = (((uint8_t)(value + 5.0f)/4)+1) + 0b10000000; //temp
      break;
    case 1: //humidity
      history[_hour] = (((uint8_t)(value/5 ))+1) + 0b10000000; //hum
      break;
    case 2: //light
      if(value>100.0f) value=100.0f; //clamp it for now
      history[_hour] = (((uint8_t)(value/5.0f))+1) + 0b10000000; //lux
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history lux: %u in hour:%u"),history[_hour], _hour);
      break;
#ifdef USE_MI_ESP32_ENERGY
    case 100: // energy
      if(value == 0.0f) value = 1.0f;
      uint8_t _watt = ((uint8_t)(MI32ln(value))*2) + 0b10000000; //watt
      history[_hour] = _watt;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: history energy: %u for value:%u"),history[_hour], value); //still playing with the mapping
      break;
#endif //USE_MI_ESP32_ENERGY
  }
}

/**
 * @brief Returns a value betwenn 0-21 for use as a data point in the history graph of the extended web UI
 * 
 * @param history - pointer to uint8_t[23]
 * @param hour  - hour of datapoint
 * @return uint8_t  - value for the y-axis, should be between 0-21
 */
uint8_t MI32fetchHistory(uint8_t *history, uint32_t hour){
    if(hour>23) {
      return 0;} //should never happen
    if(bitRead(history[hour],7) == 0) {
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
  for(auto _sensor:MIBLEsensors){
    if(_sensor.feature.temp == 1){
      bitClear(_sensor.temp_history[_nextHour],7);
    }
    if(_sensor.feature.hum == 1){
      bitClear(_sensor.hum_history[_nextHour],7);
    }
    if(_sensor.feature.lux == 1){
      bitClear(_sensor.lux_history[_nextHour],7);
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
  MI32.option.minimalSummary = 0;
  MI32.option.directBridgeMode = 0;
  MI32.option.showRSSI = 1;
  MI32.option.ignoreBogusBattery = 1; // from advertisements

  MI32loadCfg();
  if(MIBLEsensors.size()>0){
    MI32.mode.didGetConfig = 1;
  }

  MI32.beAdvCB = nullptr;
  AddLog(LOG_LEVEL_INFO,PSTR("M32: pre-init"));
}

void MI32Init(void) {
  if (MI32.mode.init) { return; }

  if (TasmotaGlobal.global_state.wifi_down && TasmotaGlobal.global_state.eth_down) { return; }

  if (!TasmotaGlobal.global_state.wifi_down) {
    TasmotaGlobal.wifi_stay_asleep = true;
    if (WiFi.getSleep() == false) {
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Put WiFi modem in sleep mode"));
      WiFi.setSleep(true); // Sleep
    }
  }

  if(MI32.mode.didGetConfig){
    MI32.mode.didStartHAP = 0;
  #ifdef USE_MI_HOMEKIT
    MI32getSetupCodeFromMAC(MI32.hk_setup_code);
    AddLog(LOG_LEVEL_INFO,PSTR("M32: Init HAP core"));
    mi_homekit_main();
  #else 
    MI32.mode.didStartHAP = 1;
  #endif //USE_MI_HOMEKIT
  }
  
  if (!MI32.mode.init) {
    NimBLEDevice::init("MI32");
    AddLog(LOG_LEVEL_INFO,PSTR("M32: Init BLE device"));
    MI32.mode.init = 1;

    MI32StartScanTask(); // Let's get started !!
  }
#ifdef USE_MI_EXT_GUI
#ifdef USE_MI_ESP32_ENERGY
  MI32.energy_history = (uint8_t*) calloc(24,1);
#endif //USE_MI_ESP32_ENERGY
#endif //USE_MI_EXT_GUI
  return;
}

/*********************************************************************************************\
 * Berry section - partly used by HomeKit too
\*********************************************************************************************/
extern "C" {

  bool MI32runBerryConnection(uint8_t operation){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->operation = operation%100;
      AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Berry connection op: %d, addrType: %d"),MI32.conCtx->operation, MI32.conCtx->addrType);
      MI32StartConnectionTask();
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
    AddLog(LOG_LEVEL_INFO,PSTR("M32: Connection Ctx created"));
  }

  bool MI32setBerryCtxSvc(const char *Svc){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->serviceUUID = NimBLEUUID(Svc);
      AddLog(LOG_LEVEL_INFO,PSTR("M32: SVC: %s"),MI32.conCtx->serviceUUID.toString().c_str());
      // AddLog(LOG_LEVEL_INFO,PSTR("M32: SVC: %s"),Svc);
      return true;
    }
    return false;
  }

  bool MI32setBerryCtxChr(const char *Chr){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->charUUID = NimBLEUUID(Chr);
      AddLog(LOG_LEVEL_INFO,PSTR("M32: CHR: %s"),MI32.conCtx->charUUID.toString().c_str());
      // AddLog(LOG_LEVEL_INFO,PSTR("M32: CHR: %s"),Chr);
      return true;
    }
    return false;
  }

  bool MI32setBerryCtxMAC(uint8_t *MAC, uint8_t type){
    if(MI32.conCtx != nullptr){
      MI32.conCtx->MAC = MAC;
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

  const char * MI32getDeviceName(uint32_t slot){
    if(slot>MIBLEsensors.size()-1) return "";
    return kMI32DeviceType[MIBLEsensors[slot].type-1];
  }

} //extern "C"
/*********************************************************************************************\
 * Homekit section
\*********************************************************************************************/
#ifdef USE_MI_HOMEKIT
extern "C" {

  const char * MI32getSetupCode(){
    return (const char*)MI32.hk_setup_code;
  }

  uint32_t MI32numOfRelays(){
    if(TasmotaGlobal.devices_present>0) MI32.HKinfoMsg = MI32_HAP_OUTLET_ADDED;
    return TasmotaGlobal.devices_present;
  }

  void MI32setRelayFromHK(uint32_t relay, bool onOff){
      ExecuteCommandPower(relay, onOff, SRC_IGNORE);
  }

  uint32_t MI32getDeviceType(uint32_t slot){
    return MIBLEsensors[slot].type;
  }

/**
 * @brief Get at least a bit of the status of the HAP core, i.e. to reduce the activy of the driver while doing the pairing
 * 
 * @param event 
 */
  void MI32passHapEvent(uint32_t event){
    switch(event){
      case 5: //HAP_EVENT_PAIRING_STARTED
        MI32suspendScanTask();
      default:
        vTaskResume(MI32.ScanTask);
    }
    if(event==4){
      MI32.HKinfoMsg = MI32_HAP_CONTROLLER_DISCONNECTED;
      MI32.HKconnectedControllers--;
    }
    if(event==3){
      MI32.HKinfoMsg = MI32_HAP_CONTROLLER_CONNECTED;
      MI32.HKconnectedControllers++;
    }
  }

  void MI32didStartHAP(bool HAPdidStart){
    if(HAPdidStart) {
      MI32.mode.didStartHAP = 1;
      MI32.HKinfoMsg = MI32_HAP_DID_START;
      }
    else{
      MI32.HKinfoMsg = MI32_HAP_DID_NOT_START;
    }
  }

/**
 * @brief Simply store the writeable HAP characteristics as void pointers in the "main" driver for updates of the values
 * 
 * @param slot - sensor slot in MIBLEsensors
 * @param type - sensors type, except for the buttons this is equal to the mibeacon types
 * @param handle - a void ponter to a characteristic
 */
  void MI32saveHAPhandles(uint32_t slot, uint32_t type, void* handle){
    // AddLog(LOG_LEVEL_INFO,PSTR("M32: pass ptr to hap service, type:%u"), type);
    switch(type){
      case 1000: case 1001: case 1002: case 1003: case 1004: case 1005:
        MIBLEsensors[slot].button_hap_service[type-1000] = handle;
        break;
      case 0x04:
        MIBLEsensors[slot].temp_hap_service = handle;
        break;
      case 0x06:
        MIBLEsensors[slot].hum_hap_service = handle;
        break;
      case 0x0a:
        MIBLEsensors[slot].bat_hap_service = handle;
        break;
      case 0x07:
        MIBLEsensors[slot].light_hap_service = handle;
        break;
      case 0x0f:
        MIBLEsensors[slot].motion_hap_service = handle;
        break;
      case 0x14:
        MIBLEsensors[slot].leak_hap_service = handle;
        break;
      case 0x19:
        MIBLEsensors[slot].door_sensor_hap_service = handle;
        break;
      case 0xf0:
        if(slot>3) break; //support only 4 for now
        MI32.outlet_hap_service[slot] = handle;
        break;
    }
  }
}

/**
 * @brief Creates a simplified setup code from the Wifi MAC for HomeKit by converting every ascii-converted byte to 1, if it not 2-9
 *        Example: AABBCC1234f2
 *              -> 111-11-234
 *        This is no security feature, only for convenience
 *  * @param setupcode 
 */
  void MI32getSetupCodeFromMAC(char *setupcode){
    uint8_t _mac[6];
    char _macStr[13] = { 0 };
    WiFi.macAddress(_mac);
    ToHex_P(_mac,6,_macStr,13);
    AddLog(LOG_LEVEL_INFO,PSTR("M32: Wifi MAC: %s"), _macStr);
    for(int i = 0; i<10; i++){
      if(_macStr[i]>'9' || _macStr[i]<'1') setupcode[i]='1';
      else setupcode[i] = _macStr[i];
    }
    setupcode[3] = '-';
    setupcode[6] = '-';
    setupcode[10] = 0;
    AddLog(LOG_LEVEL_INFO,PSTR("M32: HK setup code: %s"), setupcode);
    return;
  }

#endif //USE_MI_HOMEKIT
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
            mi_bindKey_t _keyMAC;
            uint8_t *_key = (uint8_t*) malloc(16);
            char *_keyStr = (char *)_val.getStr();
            if(strlen(_keyStr)==0){
              continue;
            }
            if(strlen(_keyStr)!=32){
              _error = true;
              break;
            }
            MI32HexStringToBytes(_keyStr,_key);
            MIBLEsensors[_numberOfDevices].key = _key;
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
    uint32_t _inc = snprintf_P(_filebuf+_pos,200,PSTR("{\"MAC\":\"%s\",\"PID\":\"%04x\",\"key\":\"%s\"},"),_MAC,kMI32DeviceID[_sensor.type - 1],_key);
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
  if (MI32.ScanTask != nullptr) vTaskSuspend(MI32.ScanTask);
}

void MI32StartTask(uint32_t task){
  switch(task){
    case MI32_TASK_SCAN:
      if (MI32.mode.willConnect == 1) return;
      if (MI32.mode.runningScan == 1 || MI32.mode.connected == 1) return;
      MI32StartScanTask();
      break;
    case MI32_TASK_CONN:
      if (MI32.mode.canConnect == 0 || MI32.mode.willConnect == 1 ) return;
      if (MI32.mode.connected == 1) return;
      MI32StartConnectionTask();
      break;
    default:
      break;
  }
}

bool MI32ConnectActiveSensor(){ // only use inside a task !!
    NimBLEAddress _address = NimBLEAddress(MI32.conCtx->MAC, MI32.conCtx->addrType);
    MI32Client = nullptr;
    if(NimBLEDevice::getClientListSize()) {
      MI32Client = NimBLEDevice::getClientByPeerAddress(_address);
    }
    if (!MI32Client){
      MI32Client = NimBLEDevice::createClient(_address);
      MI32Client->setClientCallbacks(&MI32SensorCB , false);
    }
    if (!MI32Client->connect(false)) {
        MI32.mode.willConnect = 0;
        NimBLEDevice::deleteClient(MI32Client);
        // AddLog(LOG_LEVEL_ERROR,PSTR("M32: did not connect client"));
        return false;
    }
    return true;
}

void MI32StartScanTask(){
    if (MI32.mode.connected) return;
    if(MI32.ScanTask!=nullptr) vTaskDelete(MI32.ScanTask);
    MI32.mode.runningScan = 1;
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
  if (MI32Scan == nullptr) MI32Scan = NimBLEDevice::getScan();

  MI32Scan->setInterval(70);
  MI32Scan->setWindow(50);
  MI32Scan->setAdvertisedDeviceCallbacks(&MI32ScanCallbacks,true);
  MI32Scan->setActiveScan(false);
  MI32Scan->setMaxResults(0);
  MI32Scan->start(0, MI32scanEndedCB, true); // never stop scanning, will pause automatically while connecting
  MI32.infoMsg = MI32_START_SCANNING;
  
  uint32_t timer = 0;
  for(;;){
    vTaskDelay(10000/ portTICK_PERIOD_MS);
  }
  vTaskDelete( NULL );
}


bool MI32StartConnectionTask(){
    if(MI32.conCtx == nullptr) return false;
    if(MI32.conCtx->buffer == nullptr) return false;
    MI32.mode.willConnect = 1;
    MI32Scan->stop();
    MI32suspendScanTask();
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
    MI32.mode.connected = 0;
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
      NimBLERemoteService* pSvc = nullptr;
      NimBLERemoteCharacteristic* pChr = nullptr;

      pSvc = MI32Client->getService(MI32.conCtx->serviceUUID);
      if(pSvc) {
          pChr = pSvc->getCharacteristic(MI32.conCtx->charUUID);
      }
      else{
        MI32.conCtx->error = MI32_CONN_NO_SERVICE;
      }
      if (pChr){
        switch(MI32.conCtx->operation){
          case 11:
            if(pChr->canRead()) {
            std::string _val = pChr->readValue();
            MI32.conCtx->buffer[0] = (uint8_t)_val.size();
            const char *_c_val = _val.c_str();
            memcpy( MI32.conCtx->buffer + 1,_c_val,MI32.conCtx->buffer[0]);
            }
            else{
            MI32.conCtx->error = MI32_CONN_CAN_NOT_READ;
            }
            break;
          case 13:
            if(pChr->canNotify()) {
              if(pChr->subscribe(true,MI32notifyCB,false)) AddLog(LOG_LEVEL_DEBUG,PSTR("M32: subscribe"));
            }
            else{
              MI32.conCtx->error = MI32_CONN_CAN_NOT_NOTIFY;
            }
            break;
          case 12:
            if(pChr->canWrite()) {
              uint8_t len = MI32.conCtx->buffer[0];
              if(pChr->writeValue(MI32.conCtx->buffer + 1,len,true)) { // true is important !
                // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: write op done"));
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
        default:
            break;
        }
      }
      else{
              MI32.conCtx->error = MI32_CONN_NO_CHARACTERISTIC;
      }
      timer = 0;

      while (timer<150){
        if (MI32.mode.readingDone){
          break;
        }
        else{
          MI32.conCtx->error = MI32_CONN_NOTIFY_TIMEOUT; //did not read on notify - timeout
        }
        timer++;
        vTaskDelay(100/ portTICK_PERIOD_MS);

    }
  MI32Client->disconnect();
  DEBUG_SENSOR_LOG(PSTR("M32: requested disconnect"));
  }
  else{
    MI32.conCtx->error = MI32_CONN_NO_CONNECT; // not connected
  }
  MI32.mode.connected = 0;
  MI32.mode.triggerBerryConnCB = 1;
  MI32StartTask(MI32_TASK_SCAN);
  vTaskDelete( NULL );
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
  uint32_t _payloadSize = (_beacon->frame.includesCapability)?_beacon->payload.size:_beacon->payload.ten;
  if(_beacon->frame.includesMAC && _beacon->frame.includesObj) {
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: offset %u, size: %u"),_offset,_payloadSize);
      memcpy((uint8_t*)&_payload,(uint8_t*)(&_beacon->payload)-_offset, _payloadSize + 3);
      // AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)&_payload,_payloadSize + 3);
      }
  }
if(decryptRet!=0){
  AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Decryption failed with error: %d"),decryptRet);
  if (decryptRet == -1) MIBLEsensors[_slot].status.hasWrongKey = 1;
  return;
}

  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u with payload type: %02x"), kMI32DeviceType[MIBLEsensors[_slot].type-1],_slot,_payload.type);
  MIBLEsensors[_slot].lastTime = millis();
  switch(_payload.type){
    case 0x01:
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
#ifdef USE_MI_HOMEKIT
      if((void**)MIBLEsensors[_slot].button_hap_service[0] != nullptr){
        mi_homekit_update_value(MIBLEsensors[_slot].button_hap_service[0], (float)2.0f, 0x01); // only one button, long press = 2
        }
#endif //USE_MI_HOMEKIT 
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
#ifdef USE_MI_HOMEKIT
      if(MIBLEsensors[_slot].Btn>5) break; // hard coded limit for now
      if((void**)MIBLEsensors[_slot].button_hap_service[MIBLEsensors[_slot].Btn] != nullptr){
        // AddLog(LOG_LEVEL_DEBUG,PSTR("Send Button %u:  SingleLong:%u, pointer: %x"), MIBLEsensors[_slot].Btn,_singleLong,MIBLEsensors[_slot].button_hap_service[MIBLEsensors[_slot].Btn] );
        mi_homekit_update_value(MIBLEsensors[_slot].button_hap_service[MIBLEsensors[_slot].Btn], (float)MIBLEsensors[_slot].BtnType, 0x01);
        }
#endif //USE_MI_HOMEKIT
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 1: U16:  %u Button"), MIBLEsensors[_slot].Btn );
    break;
    case 0x04:
      _tempFloat=(float)(_payload.temp)/10.0f;
      if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        MIBLEsensors[_slot].eventType.temp = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode 4: temp updated"));
      }
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].temp_hap_service, _tempFloat, 0x04);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].temp_history, _tempFloat, 0);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 4: U16:  %u Temp"), _payload.temp );
    break;
    case 0x06:
      _tempFloat=(float)(_payload.hum)/10.0f;
      if(_tempFloat<101){
        MIBLEsensors[_slot].hum=_tempFloat;
        MIBLEsensors[_slot].eventType.hum = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode 6: hum updated"));
      }
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].hum_hap_service, _tempFloat,0x06);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].hum_history, _tempFloat, 1);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 6: U16:  %u Hum"), _payload.hum);
    break;
    case 0x07:
      MIBLEsensors[_slot].lux=_payload.lux & 0x00ffffff;
      if(MIBLEsensors[_slot].type==MJYD2S){
        MIBLEsensors[_slot].eventType.noMotion  = 1;
      }
      MIBLEsensors[_slot].eventType.lux  = 1;
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].light_hap_service, (float)MIBLEsensors[_slot].lux,0x07);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].lux_history, (float)MIBLEsensors[_slot].lux, 2);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 7: U24: %u Lux"), _payload.lux & 0x00ffffff);
    break;
    case 0x08:
      MIBLEsensors[_slot].moisture=_payload.moist;
      MIBLEsensors[_slot].eventType.moist  = 1;
      DEBUG_SENSOR_LOG(PSTR("Mode 8: moisture updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 8: U8: %u Moisture"), _payload.moist);
    break;
    case 0x09:
           MIBLEsensors[_slot].fertility=_payload.fert;
           MIBLEsensors[_slot].eventType.fert  = 1;
          DEBUG_SENSOR_LOG(PSTR("Mode 9: fertility updated"));
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 9: U16: %u Fertility"), _payload.fert);
    break;
    case 0x0a:
      if(MI32.option.ignoreBogusBattery){
        if(MIBLEsensors[_slot].type==LYWSD03MMC || MIBLEsensors[_slot].type==MHOC401){
          break;
        }
      }
      if(_payload.bat<101){
        MIBLEsensors[_slot].bat = _payload.bat;
        MIBLEsensors[_slot].eventType.bat  = 1;
        DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
#ifdef USE_MI_HOMEKIT
          mi_homekit_update_value(MIBLEsensors[_slot].bat_hap_service, (float)_payload.bat,0xa);
#endif //USE_MI_HOMEKIT
      }
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode a: U8: %u %%"), _payload.bat);
    break;
    case 0x0d:
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

    case 0x0f:
    if (_payload.ten!=0) break;
      MIBLEsensors[_slot].eventType.motion = 1;
      MIBLEsensors[_slot].events++;
      MIBLEsensors[_slot].lux = _payload.lux & 0x00ffffff;
      MIBLEsensors[_slot].eventType.lux = 1;
      MIBLEsensors[_slot].NMT = 0;
      MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].motion_hap_service, (float)1,0x0f);
        mi_homekit_update_value(MIBLEsensors[_slot].light_hap_service, (float)_payload.lux,0x07);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
      MI32addHistory(MIBLEsensors[_slot].lux_history, (float)MIBLEsensors[_slot].lux, 2);
#endif //USE_MI_EXT_GUI
      // AddLog(LOG_LEVEL_DEBUG,PSTR("motion: primary"),MIBLEsensors[_slot].lux );
    break;
    case 0x14:
      MIBLEsensors[_slot].leak = _payload.leak;
      MIBLEsensors[_slot].eventType.leak = 1;
      if(_payload.leak>0) MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].leak_hap_service, (float)_payload.leak,0x14);
#endif //USE_MI_HOMEKIT
      break;
    case 0x17:
      MIBLEsensors[_slot].NMT = _payload.NMT;
      MIBLEsensors[_slot].eventType.NMT = 1;
      MI32.mode.shallTriggerTele = 1;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 17: NMT: %u seconds"), _payload.NMT);
    break;
    case 0x19:
      MIBLEsensors[_slot].door = _payload.door;
      MIBLEsensors[_slot].eventType.door = 1;
      MIBLEsensors[_slot].events++;
      MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_HOMEKIT
        mi_homekit_update_value(MIBLEsensors[_slot].door_sensor_hap_service, (float)_payload.door,0x19);
#endif //USE_MI_HOMEKIT
      // AddLog(LOG_LEVEL_DEBUG,PSTR("Mode 19: %u"), _payload.door);
    break;

    default:
      if (MIBLEsensors[_slot].type==NLIGHT){
        MIBLEsensors[_slot].eventType.motion = 1; //motion
        MIBLEsensors[_slot].events++;
        MIBLEsensors[_slot].NMT = 0;
        MI32.mode.shallTriggerTele = 1;
#ifdef USE_MI_HOMEKIT
          mi_homekit_update_value(MIBLEsensors[_slot].motion_hap_service, (float)1,0x0f);
#endif //USE_MI_HOMEKIT
      }
      else{
        //unknown payload
        AddLogBuffer(LOG_LEVEL_DEBUG,(uint8_t*)_buf,_bufSize);
      }
    break;
  }
  if(MIBLEsensors[_slot].eventType.raw == 0) return;
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(MI32.option.directBridgeMode) MI32.mode.shallTriggerTele = 1;
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
  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1],_slot);

  MIBLEsensors[_slot].RSSI=RSSI;
  MIBLEsensors[_slot].lastTime = millis();
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
#ifdef USE_MI_HOMEKIT
    mi_homekit_update_value(MIBLEsensors[_slot].temp_hap_service, MIBLEsensors.at(_slot).temp,0x04);
    mi_homekit_update_value(MIBLEsensors[_slot].hum_hap_service, MIBLEsensors.at(_slot).hum,0x06);
    mi_homekit_update_value(MIBLEsensors[_slot].bat_hap_service, (float)MIBLEsensors.at(_slot).bat,0x0a);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
  bitSet(MI32.widgetSlot,_slot);
  MI32addHistory(MIBLEsensors[_slot].temp_history, (float)MIBLEsensors[_slot].temp, 0);
  MI32addHistory(MIBLEsensors[_slot].hum_history, (float)MIBLEsensors[_slot].hum, 1);
#endif //USE_MI_EXT_GUI
  MIBLEsensors[_slot].shallSendMQTT = 1;
  if(MI32.option.directBridgeMode) MI32.mode.shallTriggerTele = 1;

}

void MI32parseCGD1Packet(char * _buf, uint32_t length, uint8_t addr[6], int RSSI){ // no MiBeacon
  uint8_t _addr[6];
  memcpy(_addr,addr,6);
  uint32_t _slot = MIBLEgetSensorSlot(_addr, 0x0576, 0); // This must be hard-coded, no object-id in Cleargrass-packet, we have no packet counter too
  if(_slot==0xff) return;
  // AddLog(LOG_LEVEL_DEBUG,PSTR("%s at slot %u"), kMI32DeviceType[MIBLEsensors[_slot].type-1],_slot);
  MIBLEsensors[_slot].RSSI=RSSI;
  MIBLEsensors[_slot].lastTime = millis();
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
#ifdef USE_MI_HOMEKIT
            mi_homekit_update_value(MIBLEsensors[_slot].temp_hap_service, _tempFloat,0x04);
#endif //USE_MI_HOMEKIT
#ifdef USE_MI_EXT_GUI
          MI32addHistory(MIBLEsensors[_slot].temp_history, (float)MIBLEsensors[_slot].temp, 0);
#endif //USE_MI_EXT_GUI
      }
      _tempFloat=(float)(_packet.hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors[_slot].hum = _tempFloat;
          MIBLEsensors[_slot].eventType.hum  = 1;
          DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
#ifdef USE_MI_HOMEKIT
            mi_homekit_update_value(MIBLEsensors[_slot].hum_hap_service, _tempFloat,0x06);
#endif //USE_MI_HOMEKIT
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
  if(MI32.option.directBridgeMode) MI32.mode.shallTriggerTele = 1;
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
    uint8_t _addr[6];
    memcpy(_addr,addr,6);
    uint16_t _slot = MIBLEgetSensorSlot(_addr, _type, buf[4]);
    if(_slot!=0xff) {
      MIBLEsensors[_slot].RSSI=RSSI;
      MI32parseMiBeacon(buf,_slot,bufsize);
    }
}

/**
 * @brief Launch functions from Core 1 to make race conditions less likely
 *
 */

void MI32Every50mSecond(){
  if(MI32.mode.shallTriggerTele){
      MI32.mode.shallTriggerTele = 0;
      MI32triggerTele();
  }
  if(MI32.mode.triggerBerryAdvCB == 1){
    if(MI32.beAdvCB != nullptr){
    void (*func_ptr)(void) = (void (*)(void))MI32.beAdvCB;   
    func_ptr();
    } 
    MI32.mode.triggerBerryAdvCB = 0;
  }
  if(MI32.mode.triggerBerryConnCB == 1){
    if(MI32.beConnCB != nullptr){
    void (*func_ptr)(int) = (void (*)(int))MI32.beConnCB;
    char _message[32];
    GetTextIndexed(_message, sizeof(_message), MI32.conCtx->error, kMI32_ConnErrorMsg);
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s"),_message);
    func_ptr(MI32.conCtx->error);
    } 
    MI32.mode.triggerBerryConnCB = 0;
  }
  if(MI32.infoMsg > 0){
    char _message[32];
    GetTextIndexed(_message, sizeof(_message), MI32.infoMsg-1, kMI32_BLEInfoMsg);
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s"),_message);
    MI32.infoMsg = 0;
  }
#ifdef USE_MI_HOMEKIT
  if(MI32.HKinfoMsg > 0){
    char _message[32];
    GetTextIndexed(_message, sizeof(_message), MI32.HKinfoMsg-1, kMI32_HKInfoMsg);
    AddLog(LOG_LEVEL_DEBUG,PSTR("M32: %s"),_message);
    MI32.HKinfoMsg = 0;
  }
#endif //USE_MI_HOMEKIT
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void MI32EverySecond(bool restart){

#ifdef USE_MI_HOMEKIT
  if(TasmotaGlobal.devices_present>0){
    for(uint32_t i=0;i<TasmotaGlobal.devices_present;i++){
      power_t mask = 1 << i;
      // AddLog(LOG_LEVEL_DEBUG,PSTR("M32: Power masl: %u"), (TasmotaGlobal.power & mask));
      mi_homekit_update_value(MI32.outlet_hap_service[i],(TasmotaGlobal.power & mask)!=0,0xf0);
    }
  }
#endif //USE_MI_HOMEKIT


  for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
    if(MIBLEsensors[i].type==NLIGHT || MIBLEsensors[i].type==MJYD2S){
      MIBLEsensors[i].NMT++;
#ifdef USE_MI_HOMEKIT
      if(MIBLEsensors[i].NMT > 20){ //TODO: Make a choosable timeout later
        mi_homekit_update_value(MIBLEsensors[i].motion_hap_service,0.0f,0x0f);
      }
#endif //USE_MI_HOMEKIT
    }
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

void CmndMi32Cfg(void) {
  MI32saveConfig();
  ResponseCmndDone();
}

void CmndMi32Option(void){
  bool onOff = atoi(XdrvMailbox.data);
  switch(XdrvMailbox.index) {
    case 0:
      MI32.option.allwaysAggregate = onOff;
      break;
    case 1:
      MI32.option.noSummary = onOff;
      break;
    case 2:
      MI32.option.directBridgeMode = onOff;
      break;
    case 3:
      MI32.mode.didGetConfig = onOff;
      break;
  }
  ResponseCmndDone();
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
    if(MI32.widgetSlot==0) {WSContentEnd();return true;}
    for(uint32_t i=0;i<32;i++){
      if(bitRead(MI32.widgetSlot,i)){
        MI32sendWidget(i);
        WSContentEnd();
        bitClear(MI32.widgetSlot,i);
        return true;
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
  if (Energy.current_available && Energy.voltage_available) {
    WSContentSend_P(HTTP_MI32_POWER_WIDGET,MIBLEsensors.size()+1, Energy.voltage,Energy.current[1]);
    char _polyline[176];
    MI32createPolyline(_polyline,MI32.energy_history);
    WSContentSend_P(PSTR("<p>" D_POWERUSAGE ": %.1f " D_UNIT_WATT ""),Energy.active_power);
    WSContentSend_P(HTTP_MI32_GRAPH,_polyline,185,124,124,_polyline,1);
    WSContentSend_P(PSTR("</p></div>"));
  }
}
#endif //USE_MI_ESP32_ENERGY

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

  char _bat[24];
  snprintf_P(_bat,24,PSTR("&#128267;%u%%"), _sensor.bat);
  if(!_sensor.feature.bat) _bat[0] = 0;
  if (_sensor.bat == 0) _bat[9] = 0;
  WSContentSend_P(HTTP_MI32_WIDGET,slot+1,_opacity,_MAC,_sensor.RSSI,_bat,_key,kMI32DeviceType[_sensor.type-1]);

  if(_sensor.feature.tempHum){
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
  else if(_sensor.feature.temp){
    if(!isnan(_sensor.temp)){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.temp_history);
      WSContentSend_P(PSTR("<p>" D_JSON_TEMPERATURE ": %.1f °C"),_sensor.temp);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,185,124,124,_polyline,1);
      WSContentSend_P(PSTR("</p>"));
    }
  }
  if(_sensor.feature.lux){
    if(_sensor.lux!=0x00ffffff){
      char _polyline[176];
      MI32createPolyline(_polyline,_sensor.lux_history);
      WSContentSend_P(PSTR("<p>" D_JSON_ILLUMINANCE ": %d Lux"),_sensor.lux);
      WSContentSend_P(HTTP_MI32_GRAPH,_polyline,242,240,176,_polyline,3);
      WSContentSend_P(PSTR("</p>"));
    }
  }
  if(_sensor.feature.knob){
      if(_sensor.pressed == 0) {
        WSContentSend_P(PSTR("<p>Dimmer Steps: %d</p>"),_sensor.dimmer);
      }
      else {
        WSContentSend_P(PSTR("<p>Dimmer Steps pressed: %d</p>"),_sensor.dimmer);
      }
      WSContentSend_P(PSTR("<p>Long: %u</p>"),_sensor.longpress);
  }
  if(_sensor.feature.Btn){
      char _message[16];
      GetTextIndexed(_message, sizeof(_message), _sensor.BtnType, kMI32_ButtonMsg);
      if(_sensor.Btn<12) WSContentSend_P(PSTR("<p>Button%u: %s</p>"),_sensor.Btn,_message);
  }
  if(_sensor.feature.motion){
      WSContentSend_P(PSTR("<p>Events: %u</p>"),_sensor.events);
      WSContentSend_P(PSTR("<p>No motion for > <span class='Ti'>%u</span> seconds</p>"),_sensor.NMT);
  }
  if(_sensor.feature.door){
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
  if(_sensor.feature.leak){
    if(_sensor.leak==1){
      WSContentSend_P(PSTR("<p>Leak !!!</p>"));
    }
    else{
      WSContentSend_P(PSTR("<p>No leak</p>"));
    }
  }
  WSContentSend_P(PSTR("</div>"));
}

void MI32InitGUI(void){
  MI32suspendScanTask();
  MI32.widgetSlot=0;
  WSContentStart_P("m32");
  WSContentSend_P(HTTP_MI32_SCRIPT_1);
  // WSContentSend_P(HTTP_MI32_SCRIPT_1);
  WSContentSendStyle();
  WSContentSend_P(HTTP_MI32_STYLE);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,1,185,124,124,185,124,124);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,2,151,190,216,151,190,216);
  WSContentSend_P(HTTP_MI32_STYLE_SVG,3,242,240,176,242,240,176);
#ifdef USE_MI_HOMEKIT
  WSContentSend_P((HTTP_MI32_PARENT_START),MIBLEsensors.size(),UpTime(),MI32.hk_setup_code,MI32.HKconnectedControllers,ESP.getFreeHeap()/1024);
#else
  const char _setupCode[1] = {0};
  WSContentSend_P((HTTP_MI32_PARENT_START),MIBLEsensors.size(),UpTime(),_setupCode,ESP.getFreeHeap()/1024);
#endif //USE_MI_HOMEKIT
  for(uint32_t _slot = 0;_slot<MIBLEsensors.size();_slot++){
    MI32sendWidget(_slot);
  }
#ifdef USE_MI_ESP32_ENERGY
  MI32sendEnergyWidget();
#endif //USE_MI_ESP32_ENERGY
  WSContentSend_P(PSTR("</div>"));
  WSContentSpaceButton(BUTTON_MAIN);
  WSContentStop();
  vTaskResume(MI32.ScanTask);
}

void MI32HandleWebGUI(void){
  if (!HttpCheckPriviledgedAccess()) { return; }
  if (MI32HandleWebGUIResponse()) { return; }
  MI32InitGUI();
}
#endif //USE_MI_EXT_GUI

const char HTTP_MI32[] PROGMEM = "{s}Mi ESP32 {m} %u devices{e}";

#ifndef USE_MI_EXT_GUI
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u %%{e}";
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
#ifdef USE_HOME_ASSISTANT
    bool _noSummarySave = MI32.option.noSummary;
    bool _minimalSummarySave = MI32.option.minimalSummary;
    if(hass_mode==2){
      MI32.option.noSummary = false;
      MI32.option.minimalSummary = false;
    }
#endif //USE_HOME_ASSISTANT

    if(!MI32.mode.triggeredTele){
      // MI32.mode.shallClearResults=1;
      if(MI32.option.noSummary) return; // no message at TELEPERIOD
      }
    MI32suspendScanTask();
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      if(MI32.mode.triggeredTele && MIBLEsensors[i].eventType.raw == 0) continue;
      if(MI32.mode.triggeredTele && MIBLEsensors[i].shallSendMQTT==0) continue;

      bool commaflg = false;
      ResponseAppend_P(PSTR(",\"%s-%02x%02x%02x\":{"),
        kMI32DeviceType[MIBLEsensors[i].type-1],
        MIBLEsensors[i].MAC[3], MIBLEsensors[i].MAC[4], MIBLEsensors[i].MAC[5]);

      if((!MI32.mode.triggeredTele && !MI32.option.minimalSummary)||MI32.mode.triggeredTele){
        bool tempHumSended = false;
        if(MIBLEsensors[i].feature.tempHum){
          if(MIBLEsensors[i].eventType.tempHum || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
            if (!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              MI32ShowContinuation(&commaflg);
              ResponseAppendTHD(MIBLEsensors[i].temp, MIBLEsensors[i].hum);
              tempHumSended = true;
            }
          }
        }
        if(MIBLEsensors[i].feature.temp && !tempHumSended){
          if(MIBLEsensors[i].eventType.temp || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate) {
            if (!isnan(MIBLEsensors[i].temp)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%*_f"),
                Settings->flag2.temperature_resolution, &MIBLEsensors[i].temp);
            }
          }
        }
        if(MIBLEsensors[i].feature.hum && !tempHumSended){
          if(MIBLEsensors[i].eventType.hum || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate) {
            if (!isnan(MIBLEsensors[i].hum)
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode!=-1)
#endif //USE_HOME_ASSISTANT
            ) {
              char hum[FLOATSZ];
              dtostrfd(MIBLEsensors[i].hum, Settings->flag2.humidity_resolution, hum);
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_HUMIDITY "\":%s"), hum);
            }
          }
        }
        if (MIBLEsensors[i].feature.lux){
          if(MIBLEsensors[i].eventType.lux || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].lux == 0x0ffffff)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].lux != 0x0ffffff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) { // this is the error code -> no lux
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_ILLUMINANCE "\":%u"), MIBLEsensors[i].lux);
            }
          }
        }
        if (MIBLEsensors[i].feature.moist){
          if(MIBLEsensors[i].eventType.moist || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].moisture == 0xff)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_MOISTURE "\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].moisture != 0xff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"" D_JSON_MOISTURE "\":%u"), MIBLEsensors[i].moisture);
            }
          }
        }
        if (MIBLEsensors[i].feature.fert){
          if(MIBLEsensors[i].eventType.fert || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
            if ((hass_mode != -1) && (MIBLEsensors[i].fertility == 0xffff)) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"Fertility\":null"));
            } else
#endif //USE_HOME_ASSISTANT
            if ((MIBLEsensors[i].fertility != 0xffff)
#ifdef USE_HOME_ASSISTANT
              || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
            ) {
              MI32ShowContinuation(&commaflg);
              ResponseAppend_P(PSTR("\"Fertility\":%u"), MIBLEsensors[i].fertility);
            }
          }
        }
        if (MIBLEsensors[i].feature.Btn){
          if(MIBLEsensors[i].eventType.Btn
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
          ){
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Button%u\":%u"),MIBLEsensors[i].Btn,MIBLEsensors[i].BtnType + 1); //internal type is Xiaomi/Homekit 0,1,2 -> Tasmota 1,2,3
          }
        }
        if (MIBLEsensors[i].feature.knob){
          if(MIBLEsensors[i].eventType.knob
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
          ){
            MI32ShowContinuation(&commaflg);
            char _pressed[3] = {'_','P',0};
            if (MIBLEsensors[i].pressed == 0){
              _pressed[0] = 0;
            }
            ResponseAppend_P(PSTR("\"Dimmer%s\":%d"),_pressed, MIBLEsensors[i].dimmer);
          }
          if(MIBLEsensors[i].eventType.longpress
#ifdef USE_HOME_ASSISTANT
              ||(hass_mode==2)
#endif //USE_HOME_ASSISTANT
          ){
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Hold\":%d"), MIBLEsensors[i].longpress);
          }
        }
      } // minimal summary
      if (MIBLEsensors[i].feature.motion){
        if(MIBLEsensors[i].eventType.motion || !MI32.mode.triggeredTele){
          if(MI32.mode.triggeredTele) {
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

      if (MIBLEsensors[i].feature.door){
        if(MIBLEsensors[i].eventType.door || !MI32.mode.triggeredTele){
          if(MI32.mode.triggeredTele) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Door\":%u"),MIBLEsensors[i].door);
          }
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Events\":%u"),MIBLEsensors[i].events);
        }
      }

      if (MIBLEsensors[i].type == FLORA && !MI32.mode.triggeredTele) {
        if (MIBLEsensors[i].firmware[0] != '\0') { // this is the error code -> no firmware
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"Firmware\":\"%s\""), MIBLEsensors[i].firmware);
        }
      }

      if (MIBLEsensors[i].feature.NMT || !MI32.mode.triggeredTele){
        if(MIBLEsensors[i].eventType.NMT){
          MI32ShowContinuation(&commaflg);
          ResponseAppend_P(PSTR("\"NMT\":%u"), MIBLEsensors[i].NMT);
        }
      }
      if (MIBLEsensors[i].feature.bat){
        if(MIBLEsensors[i].eventType.bat || !MI32.mode.triggeredTele || MI32.option.allwaysAggregate){
#ifdef USE_HOME_ASSISTANT
          if ((hass_mode != -1) && (MIBLEsensors[i].bat == 0x00)) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Battery\":null"));
          } else
#endif //USE_HOME_ASSISTANT
          if ((MIBLEsensors[i].bat != 0x00)
#ifdef USE_HOME_ASSISTANT
            || (hass_mode != -1)
#endif //USE_HOME_ASSISTANT
          ) {
            MI32ShowContinuation(&commaflg);
            ResponseAppend_P(PSTR("\"Battery\":%u"), MIBLEsensors[i].bat);
          }
        }
      }
      if (MI32.option.showRSSI) {
        MI32ShowContinuation(&commaflg);
        ResponseAppend_P(PSTR("\"RSSI\":%d"), MIBLEsensors[i].RSSI);
      }
      ResponseJsonEnd();

      MIBLEsensors[i].eventType.raw = 0;
      if(MIBLEsensors[i].shallSendMQTT==1){
        MIBLEsensors[i].shallSendMQTT = 0;
        continue;
      }
    }
    MI32.mode.triggeredTele = 0;
#ifdef USE_HOME_ASSISTANT
    if(hass_mode==2){
      MI32.option.noSummary = _noSummarySave;
      MI32.option.minimalSummary = _minimalSummarySave;
    }
#endif //USE_HOME_ASSISTANT
#ifdef USE_MI_EXT_GUI
    Mi32invalidateOldHistory();
#ifdef USE_MI_ESP32_ENERGY
    MI32addHistory(MI32.energy_history,Energy.active_power[0],100); //TODO: which value??
#endif //USE_MI_ESP32_ENERGY
#endif //USE_MI_EXT_GUI
    vTaskResume(MI32.ScanTask);
#ifdef USE_WEBSERVER
    } else {
      MI32suspendScanTask();

      WSContentSend_P(HTTP_MI32, MIBLEsensors.size());
#ifdef USE_MI_HOMEKIT
      if(MI32.mode.didStartHAP){
        WSContentSend_PD(PSTR("{s}HomeKit Code{m} %s{e}"),MI32.hk_setup_code);
      }
#endif //USE_MI_HOMEKIT
#ifndef USE_MI_EXT_GUI
      for (uint32_t i = 0; i<MIBLEsensors.size(); i++) {
        WSContentSend_PD(HTTP_MI32_HL);
        char _MAC[18];
        ToHex_P(MIBLEsensors[i].MAC,6,_MAC,18,':');
        WSContentSend_PD(HTTP_MI32_MAC, kMI32DeviceType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, _MAC);
        WSContentSend_PD(HTTP_RSSI, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].RSSI);
        if (MIBLEsensors[i].type==FLORA) {
          if (!isnan(MIBLEsensors[i].temp)) {
            WSContentSend_Temp(kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp);
          }
          if (MIBLEsensors[i].moisture!=0xff) {
            WSContentSend_PD(HTTP_SNS_MOISTURE, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if (MIBLEsensors[i].fertility!=0xffff) {
            WSContentSend_PD(HTTP_MI32_FLORA_DATA, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA) { // everything "above" Flora
          if (!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)) {
            WSContentSend_THD(kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        }
        if (MIBLEsensors[i].feature.needsKey) {
          if(MIBLEsensors[i].key == nullptr){
            WSContentSend_PD(PSTR("{s}No known Key!!{m} can not decrypt messages{e}"));
          }
          else if(MIBLEsensors[i].status.hasWrongKey){
            WSContentSend_PD(PSTR("{s}Wrong Key!!{m} can not decrypt messages{e}"));
          }
        }
        if (MIBLEsensors[i].type==NLIGHT || MIBLEsensors[i].type==MJYD2S) {
          WSContentSend_PD(HTTP_EVENTS, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].events);
          if(MIBLEsensors[i].NMT>0) WSContentSend_PD(HTTP_NMT, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].NMT);
        }
        if(MIBLEsensors[i].door != 255 && MIBLEsensors[i].type==MCCGQ02){
          WSContentSend_PD(HTTP_DOOR, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].door);
        }
        if (MIBLEsensors[i].lux!=0x00ffffff) { // this is the error code -> no valid value
          WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
        }
        if(MIBLEsensors[i].bat!=0x00){
            WSContentSend_PD(HTTP_BATTERY, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
        }
        if (MIBLEsensors[i].type==YLYK01){
          WSContentSend_PD(HTTP_LASTBUTTON, kMI32DeviceType[MIBLEsensors[i].type-1], MIBLEsensors[i].Btn);
        }
      }
#endif //USE_MI_EXT_GUI
#endif  // USE_WEBSERVER
    }
    vTaskResume(MI32.ScanTask);
}

int ExtStopBLE(){
      if(Settings->flag5.mi32_enable == 0) return 0;
      if (MI32.ScanTask != nullptr){
        MI32Scan->stop();
        vTaskDelete(MI32.ScanTask);
        AddLog(LOG_LEVEL_INFO,PSTR("M32: stop BLE"));
      } 
#ifdef USE_MI_HOMEKIT
      if(MI32.mode.didStartHAP) {
        AddLog(LOG_LEVEL_INFO,PSTR("M32: stop Homebridge"));
        mi_homekit_stop();
      }
#endif //USE_MI_HOMEKIT
      return 0;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns62(uint8_t function)
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
      ExtStopBLE();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kMI32_Commands, MI32_Commands);
      break;
    case FUNC_JSON_APPEND:
      MI32Show(1);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      MI32Show(0);
      break;
#ifdef USE_MI_EXT_GUI
      case FUNC_WEB_ADD_MAIN_BUTTON:
        if (MI32.mode.didGetConfig) WSContentSend_P(HTTP_BTN_MENU_MI32);
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
