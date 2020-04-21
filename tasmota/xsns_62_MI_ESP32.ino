/*
  xsns_62_MI_ESP32.ino - MI-BLE-sensors via ESP32 support for Tasmota

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
  0.9.0.0 20200413  started - initial development by Christian Baars
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/
#ifdef ESP32                       // ESP32 only. Use define USE_HM10 for ESP8266 support

#ifdef USE_MI_ESP32

#define XSNS_62                    62

#include <NimBLEDevice.h>
#include <vector>

void MI32scanEndedCB(NimBLEScanResults results);
void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);


struct {
  uint16_t perPage = 4;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  struct {
    uint32_t init:1;
    uint32_t connected:1;
    uint32_t autoScan:1;
    uint32_t canScan:1;
    uint32_t runningScan:1;
    uint32_t canConnect:1;
    uint32_t willConnect:1;
    uint32_t readingDone:1;
    uint32_t shallSetTime:1;
    uint32_t willSetTime:1;
    uint32_t shallReadBatt:1;
    uint32_t willReadBatt:1;
  } mode;
  struct {
    uint8_t sensor;           // points to to the number 0...255
  } state;
} MI32;

#pragma pack(1)  // byte-aligned structures to read the sensor data

  struct {
    uint16_t temp;
    uint8_t hum;
    uint16_t volt; // LYWSD03 only
  } LYWSD0x_HT;
  struct {
    uint8_t spare;
    uint16_t temp;
    uint16_t hum;
  } CGD1_HT;
  struct {
    uint16_t temp;
    uint8_t spare;
    uint32_t lux;
    uint8_t moist;
    uint16_t fert;
  } Flora_TLMF; // temperature, lux, moisture, fertility


struct mi_beacon_t{
  uint16_t frame;
  uint16_t productID;
  uint8_t counter;
  uint8_t Mac[6];
  uint8_t spare;
  uint8_t type;
  uint8_t ten;
  uint8_t size;
  union {
    struct{ //0d
      uint16_t temp;
      uint16_t hum;
    }HT;
    uint8_t bat; //0a
    uint16_t temp; //04
    uint16_t hum; //06
    uint32_t lux; //07
    uint8_t moist; //08
    uint16_t fert; //09
  };
};

struct cg_packet_t {
  uint16_t frameID;
  uint8_t serial[6];
  uint16_t mode;
  union {
    struct {
    int16_t temp;  // -9 - 59 °C
    uint16_t hum;
    };
    uint8_t bat;
  };
};

#pragma pack(0)

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MI-HT_V1=2; LYWSD02=3; LYWSD03=4; CGG1=5; CGD1=6
  uint8_t serial[6];
  uint8_t showedUp;
  float temp; //Flora, MJ_HT_V1, LYWSD0x, CGx
  union {
    struct {
      float moisture;
      float fertility;
      uint32_t lux;
    }; // Flora
    struct {
      float hum;
    }; // MJ_HT_V1, LYWSD0x
  };
  union
  {
    uint8_t bat; // many values seem to be hard-coded garbage (LYWSD0x, GCD1)
    uint16_t volt; // LYWSD03MMC
  };
};

std::vector<mi_sensor_t> MIBLEsensors;
BLEScan* MI32Scan;
BLEScanResults MI32foundDevices;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_MI32 "MI32"

const char S_JSON_MI32_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_MI32 "%s\":%d}";
const char S_JSON_MI32_COMMAND[] PROGMEM        = "{\"" D_CMND_MI32 "%s%s\"}";
const char kMI32_Commands[] PROGMEM             = "Period|Time|Page|Battery";

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4
#define CGG1        5
#define CGD1        6

const uint16_t kMI32SlaveID[6]={ 0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347, // CGG1
                                  0x0576  // CGD1
                                  };

const char kMI32SlaveType1[] PROGMEM = "Flora";
const char kMI32SlaveType2[] PROGMEM = "MJ_HT_V1";
const char kMI32SlaveType3[] PROGMEM = "LYWSD02";
const char kMI32SlaveType4[] PROGMEM = "LYWSD03";
const char kMI32SlaveType5[] PROGMEM = "CGG1";
const char kMI32SlaveType6[] PROGMEM = "CGD1";
const char * kMI32SlaveType[] PROGMEM = {kMI32SlaveType1,kMI32SlaveType2,kMI32SlaveType3,kMI32SlaveType4,kMI32SlaveType5,kMI32SlaveType6};

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum MI32_Commands {          // commands useable in console or rules
  CMND_MI32_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_MI32_TIME,             // set LYWSD02-Time from ESP8266-time
  CMND_MI32_PAGE,             // sensor entries per web page, which will be shown alternated
  CMND_MI32_BATTERY           // read all battery levels
  };

enum MI32_TASK {
       MI32_TASK_SCAN = 0,
       MI32_TASK_CONN = 1,
       MI32_TASK_TIME = 2,
       MI32_TASK_BATT = 3,
};

/*********************************************************************************************\
 * Classes
\*********************************************************************************************/

class MI32SensorCallback : public NimBLEClientCallbacks {
  void onConnect(NimBLEClient* pclient) {
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("connected %s"), kMI32SlaveType[(MIBLEsensors[MI32.state.sensor].type)-1]);
    MI32.mode.willConnect = 0;
    MI32.mode.connected = 1;
  }
  void onDisconnect(NimBLEClient* pclient) {
    MI32.mode.connected = 0;
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("disconnected %s"), kMI32SlaveType[(MIBLEsensors[MI32.state.sensor].type)-1]);
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
    // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("Advertised Device: %s Buffer: %u"),advertisedDevice.getAddress().toString().c_str(),advertisedDevice.getServiceData().length());
    if (advertisedDevice->getServiceData().length() == 0) return;
    uint16_t uuid = advertisedDevice->getServiceDataUUID().getNative()->u16.value;
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%x"),uuid);
    uint8_t addr[6];
    memcpy(addr,advertisedDevice->getAddress().getNative(),6);
    MI32_ReverseMAC(addr);
    if(uuid==0xfe95) {
      MI32ParseResponse((char*)advertisedDevice->getServiceData().c_str(),advertisedDevice->getServiceData().length(), addr);
    }
    else if(uuid==0xfdcd) {
      MI32parseCGD1Packet((char*)advertisedDevice->getServiceData().c_str(),advertisedDevice->getServiceData().length(), addr);
    }
  };
};


static MI32AdvCallbacks MI32ScanCallbacks;
static MI32SensorCallback MI32SensorCB;
static NimBLEClient* MI32Client;

/*********************************************************************************************\
 * BLE callback functions
\*********************************************************************************************/

void MI32scanEndedCB(NimBLEScanResults results){
  AddLog_P2(LOG_LEVEL_DEBUG,PSTR("Scan ended"));
  MI32.mode.runningScan = 0;
}

void MI32notifyCB(NimBLERemoteCharacteristic* pRemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("Notified length: %u"),length);
    switch(MIBLEsensors[MI32.state.sensor].type){
      case LYWSD03MMC: case LYWSD02:
        MI32readHT_LY((char*)pData);
        MI32.mode.readingDone = 1;
        break;
      default:
        MI32.mode.readingDone = 1;
        break;
    }
}
/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

void MI32_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

/*********************************************************************************************\
 * common functions
\*********************************************************************************************/


/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_serial)[6], uint16_t _type){

  DEBUG_SENSOR_LOG(PSTR("%s: will test ID-type: %x"),D_CMND_MI32, _type);
  bool _success = false;
  for (uint32_t i=0;i<6;i++){ // i < sizeof(kMI32SlaveID) gives compiler warning
    if(_type == kMI32SlaveID[i]){
      DEBUG_SENSOR_LOG(PSTR("MI32: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("%s: ID-type is not: %x"),D_CMND_MI32,kMI32SlaveID[i]);
    }
  }
  if(!_success) return 0xff;

  DEBUG_SENSOR_LOG(PSTR("%s: vector size %u"),D_CMND_MI32, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors[i].serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("%s: known sensor at slot: %u"),D_CMND_MI32, i);
      if(MIBLEsensors[i].showedUp < 3){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors[i].showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: i: %x %x %x %x %x %x"),D_CMND_MI32, MIBLEsensors[i].serial[5], MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[0]);
    DEBUG_SENSOR_LOG(PSTR("%s: n: %x %x %x %x %x %x"),D_CMND_MI32, _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("%s: found new sensor"),D_CMND_MI32);
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 255;
  _newSensor.temp =NAN;
  _newSensor.bat=0x00;
  switch (_type)
    {
    case 1:
      _newSensor.moisture =NAN;
      _newSensor.fertility =NAN;
      _newSensor.lux = 0x00ffffff;
      break;
    case 2: case 3: case 4: case 5: case 6:
      _newSensor.hum=NAN;
      break;
    default:
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: new %s at slot: %u"),D_CMND_MI32, kMI32SlaveType[_type-1],MIBLEsensors.size()-1);
  return MIBLEsensors.size()-1;
};

/*********************************************************************************************\
 * init NimBLE
\*********************************************************************************************/

void MI32Init(void) {
  MI32.mode.init = false;
  if (!MI32.mode.init) {
    NimBLEDevice::init("");

    MI32.mode.canScan = 1;
    MI32.mode.init = 1;
    MI32.period = Settings.tele_period;

    MI32StartScanTask(); // Let's get started !!
  }
  return;
}

/*********************************************************************************************\
 * Task section
\*********************************************************************************************/

void MI32StartTask(uint32_t task){
  switch(task){
    case MI32_TASK_SCAN:
      if (MI32.mode.canScan == 0 || MI32.mode.willConnect == 1) return;
      if (MI32.mode.runningScan == 1 || MI32.mode.connected == 1) return;
      MI32StartScanTask();
      break;
    case MI32_TASK_CONN:
      if (MI32.mode.canConnect == 0 || MI32.mode.willConnect == 1 ) return;
      if (MI32.mode.runningScan == 1 || MI32.mode.connected == 1) return;
      MI32StartSensorTask();
      break;
    case MI32_TASK_TIME:
      if (MI32.mode.shallSetTime == 0) return;
      MI32StartTimeTask();
      break;
    case MI32_TASK_BATT:
      if (MI32.mode.willReadBatt == 1) return;
      MI32StartBatteryTask();
      break;
    default:
      break;
  }
}

void MI32ConnectActiveSensor(){ // only use inside a task !!
    MI32Client = nullptr;
    esp_bd_addr_t address;
    memcpy(address,MIBLEsensors[MI32.state.sensor].serial,sizeof(address));
    if(NimBLEDevice::getClientListSize()) {
      MI32Client = NimBLEDevice::getClientByPeerAddress(NimBLEAddress(address));
      if(MI32Client){
        if(!MI32Client->connect(NimBLEAddress(address), 0,false)) {
            MI32.mode.willConnect = 0;
            vTaskDelete( NULL );
        }
      }
      else {
        MI32Client = NimBLEDevice::getDisconnectedClient();
      }
    }
    if(!MI32Client) {
      if(NimBLEDevice::getClientListSize() >= NIMBLE_MAX_CONNECTIONS) {
          MI32.mode.willConnect = 0;
          vTaskDelete( NULL );
      }
      MI32Client = NimBLEDevice::createClient();
      MI32Client->setClientCallbacks(&MI32SensorCB , false);
      MI32Client->setConnectionParams(12,12,0,51);
      MI32Client->setConnectTimeout(10);
      if (!MI32Client->connect(NimBLEAddress(address),0,false)) {
          MI32.mode.willConnect = 0;
          NimBLEDevice::deleteClient(MI32Client);
          vTaskDelete( NULL );
      }
    }
}


void MI32StartScanTask(){
    if (MI32.mode.connected) return;
    MI32.mode.runningScan = 1;
    xTaskCreatePinnedToCore(
    MI32ScanTask,    /* Function to implement the task */
    "MI32ScanTask",  /* Name of the task */
    4096,             /* Stack size in words */
    NULL,             /* Task input parameter */
    0,                /* Priority of the task */
    NULL,             /* Task handle. */
    0);               /* Core where the task should run */
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: Start scanning"),D_CMND_MI32);
}

void MI32ScanTask(void *pvParameters){
  NimBLEScan* pScan = NimBLEDevice::getScan();
  pScan->setAdvertisedDeviceCallbacks(&MI32ScanCallbacks);
  pScan->setActiveScan(false);
  pScan->start(5, MI32scanEndedCB); // hard coded duration
  uint32_t timer = 0;
  while (MI32.mode.runningScan){
    if (timer>15){
      vTaskDelete( NULL );
    }
    timer++;
    vTaskDelay(1000);
  }
  vTaskDelete( NULL );
}

void MI32StartSensorTask(){
    MI32.mode.willConnect = 1;
    xTaskCreatePinnedToCore(
      MI32SensorTask,    /* Function to implement the task */
      "MI32SensorTask",  /* Name of the task */
      8192,             /* Stack size in words */
      NULL,             /* Task input parameter */
      15,                /* Priority of the task */
      NULL,             /* Task handle. */
      0);               /* Core where the task should run */
      AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: Start sensor connections"),D_CMND_MI32);
      AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: with sensor: %u"),D_CMND_MI32, MI32.state.sensor);
}

void MI32SensorTask(void *pvParameters){
    if (MIBLEsensors[MI32.state.sensor].type != LYWSD03MMC) {
      MI32.mode.willConnect = 0;
      vTaskDelete( NULL );
    }
    MI32ConnectActiveSensor();
    MI32.mode.readingDone = 1;
    switch(MIBLEsensors[MI32.state.sensor].type){
      case LYWSD03MMC:
        MI32.mode.readingDone = 0;
        MI32connectLYWSD03();
        break;
      default:
      break;
    }
    uint32_t timer = 0;
    while (!MI32.mode.readingDone){
      if (timer>150){
        break;
      }
      timer++;
      vTaskDelay(100);
    }
    MI32Client->disconnect();
    NimBLEDevice::deleteClient(MI32Client);
    vTaskDelay(500);
    MI32.mode.connected = 0;
    vTaskDelete( NULL );
}

void MI32connectLYWSD03(){
  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
  static BLEUUID charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");
  pSvc = MI32Client->getService(serviceUUID);
  if(pSvc) {
      pChr = pSvc->getCharacteristic(charUUID);
  }
  if(pChr->canNotify()) {
    if(!pChr->registerForNotify(MI32notifyCB)) {
      MI32.mode.willConnect = 0;
      MI32Client->disconnect();
      return;
    }
  }
}

void MI32StartTimeTask(){
    MI32.mode.willConnect = 1;
    xTaskCreatePinnedToCore(
      MI32TimeTask,    /* Function to implement the task */
      "MI32TimeTask",  /* Name of the task */
      8912,             /* Stack size in words */
      NULL,             /* Task input parameter */
      15,                /* Priority of the task */
      NULL,             /* Task handle. */
      0);               /* Core where the task should run */
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: Start time set"),D_CMND_MI32);
      // AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: with sensor: %u"),D_CMND_MI32, MI32.state.sensor);
}

void MI32TimeTask(void *pvParameters){
  if (MIBLEsensors[MI32.state.sensor].type != LYWSD02) {
      MI32.mode.shallSetTime = 0;
      vTaskDelete( NULL );
    }
  MI32ConnectActiveSensor();

  uint32_t timer = 0;
  while (MI32.mode.connected == 0){
      if (timer>1000){
        break;
      }
      timer++;
      vTaskDelay(10);
    }

  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  static BLEUUID serviceUUID("EBE0CCB0-7A0A-4B0C-8A1A-6FF2997DA3A6");
  static BLEUUID charUUID("EBE0CCB7-7A0A-4B0C-8A1A-6FF2997DA3A6");
  pSvc = MI32Client->getService(serviceUUID);
  if(pSvc) {
      pChr = pSvc->getCharacteristic(charUUID);
  }
  if(pChr->canWrite()) {
    union {
      uint8_t buf[5];
      uint32_t time;
    } _utc;
    _utc.time = Rtc.utc_time;
    _utc.buf[4] = Rtc.time_timezone / 60;

    if(!pChr->writeValue(_utc.buf,sizeof(_utc.buf),true)) { // true is important !
      MI32.mode.willConnect = 0;
      MI32Client->disconnect();
    }
    else {
      MI32.mode.shallSetTime = 0;
      MI32.mode.willSetTime = 0;
    }
  }
  MI32Client->disconnect();
  NimBLEDevice::deleteClient(MI32Client);
  vTaskDelay(500);
  MI32.mode.connected = 0;
  vTaskDelete( NULL );
}

void MI32StartBatteryTask(){
    if (MI32.mode.connected) return;
    MI32.mode.willReadBatt = 1;
    xTaskCreatePinnedToCore(
    MI32BatteryTask, /* Function to implement the task */
    "MI32BatteryTask",  /* Name of the task */
    8192,             /* Stack size in words */
    NULL,             /* Task input parameter */
    15,                /* Priority of the task */
    NULL,             /* Task handle. */
    0);               /* Core where the task should run */
}

void MI32BatteryTask(void *pvParameters){
    // all reported battery values are probably crap, but we allow the reading on demand
    switch (MIBLEsensors[MI32.state.sensor].type){
      case LYWSD03MMC: case MJ_HT_V1: case CGG1:
        MI32.mode.willConnect = 0;
        MI32.mode.willReadBatt = 0;
        vTaskDelete( NULL );
        break;
      default:
        break;
      }

    MI32.mode.connected = 0;
    MI32ConnectActiveSensor();
    uint32_t timer = 0;
      while (MI32.mode.connected == 0){
      if (timer>1000){
        break;
      }
      timer++;
      vTaskDelay(10);
    }

    switch(MIBLEsensors[MI32.state.sensor].type){
      case FLORA:
        MI32batteryFLORA();
        break;
      case LYWSD02:
        MI32batteryLYWSD02();
        break;
      case CGD1:
        MI32batteryCGD1();
       break;
    }
    MI32Client->disconnect();
    MI32.mode.willReadBatt = 0;
    NimBLEDevice::deleteClient(MI32Client);
    vTaskDelay(500);
    MI32.mode.connected = 0;
    vTaskDelete( NULL );
}

void MI32batteryFLORA(){
  uint32_t timer = 0;
  while (!MI32.mode.connected){
      if (timer>1000){
        break;
      }
      timer++;
      vTaskDelay(10);
    }
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s connected for battery"),kMI32SlaveType[MIBLEsensors[MI32.state.sensor].type-1] );
  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  static BLEUUID FLserviceUUID("00001204-0000-1000-8000-00805f9b34fb");
  static BLEUUID FLcharUUID("00001a02-0000-1000-8000-00805f9b34fb");

  pSvc = MI32Client->getService(FLserviceUUID);
  if(pSvc) {     /** make sure it's not null */
      pChr = pSvc->getCharacteristic(FLcharUUID);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: got Flora char %s"),D_CMND_MI32, pChr->getUUID().toString().c_str());
  }
  else {
    MI32.mode.readingDone = 1;
    return;
  }
  if(pChr->canRead()) {
    const char *buf = pChr->readValue().c_str();
    MI32readBat((char*)buf);
  }
}

void MI32batteryLYWSD02(){
  uint32_t timer = 0;
  while (!MI32.mode.connected){
      if (timer>1000){
        break;
      }
      timer++;
      vTaskDelay(10);
    }

  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  static BLEUUID LY2serviceUUID("EBE0CCB0-7A0A-4B0C-8A1A-6FF2997DA3A6");
  static BLEUUID LY2charUUID("EBE0CCC4-7A0A-4B0C-8A1A-6FF2997DA3A6");

  pSvc = MI32Client->getService(LY2serviceUUID);
  if(pSvc) {
      pChr = pSvc->getCharacteristic(LY2charUUID);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: got LYWSD02 char %s"),D_CMND_MI32, pChr->getUUID().toString().c_str());
  }
  else {
    return;
  }
  if(pChr->canRead()) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("LYWSD02 char"));
    const char *buf = pChr->readValue().c_str();
    MI32readBat((char*)buf);
  }
}

void MI32batteryCGD1(){
  uint32_t timer = 0;
  while (!MI32.mode.connected){
      if (timer>1000){
        break;
      }
      timer++;
      vTaskDelay(10);
    }

  NimBLERemoteService* pSvc = nullptr;
  NimBLERemoteCharacteristic* pChr = nullptr;
  static BLEUUID CGD1serviceUUID("180F");
  static BLEUUID CGD1charUUID("2A19");

  pSvc = MI32Client->getService(CGD1serviceUUID);
  if(pSvc) {
      pChr = pSvc->getCharacteristic(CGD1charUUID);
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: got CGD1 char %s"),D_CMND_MI32, pChr->getUUID().toString().c_str());
  }
  else {
    return;
  }
  if(pChr->canRead()) {
    const char *buf = pChr->readValue().c_str();
    MI32readBat((char*)buf);
  }
}


/*********************************************************************************************\
 * parse the response from advertisements
\*********************************************************************************************/

void MI32parseMiBeacon(char * _buf, uint32_t _slot){
  float _tempFloat;
  mi_beacon_t _beacon;
  if (MIBLEsensors[_slot].type==MJ_HT_V1 || MIBLEsensors[_slot].type==CGG1){
    memcpy((uint8_t*)&_beacon+1,(uint8_t*)_buf, sizeof(_beacon)); // shift by one byte for the MJ_HT_V1
    memcpy((uint8_t*)&_beacon.Mac,(uint8_t*)&_beacon.Mac+1,6);    // but shift back the MAC
  }
  else{
    memcpy((void*)&_beacon,(void*)_buf, sizeof(_beacon));
  }
  MI32_ReverseMAC(_beacon.Mac);

  DEBUG_SENSOR_LOG(PSTR("MiBeacon type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[0],(uint8_t)_buf[1],(uint8_t)_buf[2],(uint8_t)_buf[3],(uint8_t)_buf[4],(uint8_t)_buf[5],(uint8_t)_buf[6],(uint8_t)_buf[7]);
  DEBUG_SENSOR_LOG(PSTR("         type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[8],(uint8_t)_buf[9],(uint8_t)_buf[10],(uint8_t)_buf[11],(uint8_t)_buf[12],(uint8_t)_buf[13],(uint8_t)_buf[14],(uint8_t)_buf[15]);

  if(MIBLEsensors[_slot].type==4 || MIBLEsensors[_slot].type==6){
    DEBUG_SENSOR_LOG(PSTR("LYWSD03 and CGD1 no support for MiBeacon, type %u"),MIBLEsensors[_slot].type);
    return;
  }
  DEBUG_SENSOR_LOG(PSTR("%s at slot %u"), kMI32SlaveType[MIBLEsensors[_slot].type-1],_slot);
  switch(_beacon.type){
    case 0x04:
    _tempFloat=(float)(_beacon.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 4: temp updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 4: U16:  %u Temp"), _beacon.temp );
    break;
    case 0x06:
    _tempFloat=(float)(_beacon.hum)/10.0f;
    if(_tempFloat<101){
        MIBLEsensors[_slot].hum=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 6: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 6: U16:  %u Hum"), _beacon.hum);
    break;
    case 0x07:
    MIBLEsensors[_slot].lux=_beacon.lux & 0x00ffffff;
    DEBUG_SENSOR_LOG(PSTR("Mode 7: U24: %u Lux"), _beacon.lux & 0x00ffffff);
    break;
    case 0x08:
    _tempFloat =(float)_beacon.moist;
    if(_tempFloat<100){
        MIBLEsensors[_slot].moisture=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 8: moisture updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 8: U8: %u Moisture"), _beacon.moist);
    break;
    case 0x09:
    _tempFloat=(float)(_beacon.fert);
    if(_tempFloat<65535){ // ???
        MIBLEsensors[_slot].fertility=_tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode 9: fertility updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode 9: U16: %u Fertility"), _beacon.fert);
    break;
    case 0x0a:
    if(_beacon.bat<101){
      MIBLEsensors[_slot].bat = _beacon.bat;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
    DEBUG_SENSOR_LOG(PSTR("Mode a: U8: %u %%"), _beacon.bat);
    break;
    case 0x0d:
    _tempFloat=(float)(_beacon.HT.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: temp updated"));
     }
    _tempFloat=(float)(_beacon.HT.hum)/10.0f;
    if(_tempFloat<100){
        MIBLEsensors[_slot].hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("Mode d: hum updated"));
    }
    DEBUG_SENSOR_LOG(PSTR("Mode d: U16:  %x Temp U16: %x Hum"), _beacon.HT.temp,  _beacon.HT.hum);
    break;
  }
}

void MI32parseCGD1Packet(char * _buf, uint32_t length, uint8_t addr[6]){ // no MiBeacon
  uint8_t _addr[6];
  memcpy(_addr,addr,6);
  uint32_t _slot = MIBLEgetSensorSlot(_addr, 0x0576); // This must be hard-coded, no object-id in Cleargrass-packet
  DEBUG_SENSOR_LOG(PSTR("MI32: Sensor slot: %u"), _slot);
  if(_slot==0xff) return;
  cg_packet_t _packet;
  memcpy((char*)&_packet,_buf,sizeof(_packet));
  switch (_packet.mode){
    case 0x0401:
      float _tempFloat;
      _tempFloat=(float)(_packet.temp)/10.0f;
      if(_tempFloat<60){
          MIBLEsensors.at(_slot).temp = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("CGD1: temp updated"));
      }
      _tempFloat=(float)(_packet.hum)/10.0f;
      if(_tempFloat<100){
          MIBLEsensors.at(_slot).hum = _tempFloat;
          DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
      }
      DEBUG_SENSOR_LOG(PSTR("CGD1: U16:  %x Temp U16: %x Hum"), _packet.temp,  _packet.hum);
      break;
    case 0x0102:
      if(_packet.bat<101){
      MIBLEsensors.at(_slot).bat = _packet.bat;
      DEBUG_SENSOR_LOG(PSTR("Mode a: bat updated"));
      }
      break;
    default:
      DEBUG_SENSOR_LOG(PSTR("MI32: unexpected CGD1-packet"));
  }
}

void MI32ParseResponse(char *buf, uint16_t bufsize, uint8_t addr[6]) {
    if(bufsize<10) {
      return;
    }
    char * _pos = buf;
    uint16_t _type= _pos[3]*256 + _pos[2];
    // AddLog_P2(LOG_LEVEL_INFO, PSTR("%02x %02x %02x %02x"),(uint8_t)buf[0], (uint8_t)buf[1],(uint8_t)buf[2],(uint8_t)buf[3]);
    uint8_t _addr[6];
    memcpy(_addr,addr,6);
    uint16_t _slot = MIBLEgetSensorSlot(_addr, _type);
    if(_slot!=0xff) MI32parseMiBeacon(_pos,_slot);
}

/***********************************************************************\
 * Read data from connections
\***********************************************************************/

void MI32readHT_LY(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_MI32,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0] != 0 && _buf[1] != 0){
    memcpy(&LYWSD0x_HT,(void *)_buf,sizeof(LYWSD0x_HT));
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: T * 100: %u, H: %u, V: %u"),D_CMND_MI32,LYWSD0x_HT.temp,LYWSD0x_HT.hum, LYWSD0x_HT.volt);
    uint32_t _slot = MI32.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(LYWSD0x_HT.temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)LYWSD0x_HT.hum;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("LYWSD0x: hum updated"));
    }
    if (MIBLEsensors[_slot].type == LYWSD03MMC){
      MIBLEsensors[_slot].volt = LYWSD0x_HT.volt;
    }
  }
}

bool MI32readBat(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_MI32,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0] != 0){
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: Battery: %u"),D_CMND_MI32,_buf[0]);
    uint32_t _slot = MI32.state.sensor;
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    if(_buf[0]<101){
        MIBLEsensors[_slot].bat=_buf[0];
        return true;
    }
  }
  return false;
}

/**
 * @brief Main loop of the driver, "high level"-loop
 *
 */

void MI32EverySecond(bool restart){
  static uint32_t _counter = MI32.period - 15;
  static uint32_t _nextSensorSlot = 0;

  if(restart){
    _counter = 0;
    MI32.mode.canScan = 0;
    MI32.mode.canConnect = 1;
    return;
  }

  if (MI32.mode.shallSetTime) {
    MI32.mode.canScan = 0;
    MI32.mode.canConnect = 0;
    if (MI32.mode.willSetTime == 0){
      MI32.mode.willSetTime = 1;
      MI32StartTask(MI32_TASK_TIME);
    }
  }

  if (MI32.mode.willReadBatt) return;

  if (_counter>MI32.period) {
    _counter = 0;
    MI32.mode.canScan = 0;
    MI32.mode.canConnect = 1;
  }

  if(MI32.mode.connected == 1 || MI32.mode.willConnect == 1) return;

  if(MIBLEsensors.size()==0) {
    if (MI32.mode.runningScan == 0 && MI32.mode.canScan == 1) MI32StartTask(MI32_TASK_SCAN);
    return;
  }

  if(_counter==0) {
    MI32.state.sensor = _nextSensorSlot;
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: active sensor now: %u"),D_CMND_MI32, MI32.state.sensor);
    MI32.mode.canScan = 0;
    if (MI32.mode.runningScan == 1 || MI32.mode.connected == 1) return;
    _nextSensorSlot++;
    MI32.mode.canConnect = 1;
    if(MI32.mode.connected == 0) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("will connect to %s"),kMI32SlaveType[MIBLEsensors[MI32.state.sensor].type-1] );

    if (MI32.mode.shallReadBatt) {
      MI32StartTask(MI32_TASK_BATT);
    }
    else{
      MI32StartTask(MI32_TASK_CONN);
    }

    }
    if (MI32.state.sensor==MIBLEsensors.size()-1) {
      _nextSensorSlot= 0;
      _counter++;
      if (MI32.mode.shallReadBatt){
        MI32.mode.shallReadBatt = 0;
      }
      MI32.mode.canConnect = 0;
      MI32.mode.canScan = 1;
    }
  }
  else _counter++;
  if (MI32.state.sensor>MIBLEsensors.size()-1) {
    _nextSensorSlot = 0;
    MI32.mode.canScan = 1;
  }
  MI32StartTask(MI32_TASK_SCAN);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool MI32Cmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_MI32);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_MI32), disp_len)) {  // prefix
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kMI32_Commands);
    switch (command_code) {
      case CMND_MI32_PERIOD:
        if (XdrvMailbox.data_len > 0) {
          if (XdrvMailbox.payload==1) {
            MI32EverySecond(true);
            XdrvMailbox.payload = MI32.period;
            }
          else {
            MI32.period = XdrvMailbox.payload;
          }
        }
        else {
          XdrvMailbox.payload = MI32.period;
        }
        Response_P(S_JSON_MI32_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_MI32_TIME:
        if (XdrvMailbox.data_len > 0) {
          if(MIBLEsensors.size()>XdrvMailbox.payload){
            if(MIBLEsensors[XdrvMailbox.payload].type == LYWSD02){
              AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: will set Time"),D_CMND_MI32);
              MI32.state.sensor = XdrvMailbox.payload;
              MI32.mode.canScan = 0;
              MI32.mode.canConnect = 0;
              MI32.mode.shallSetTime = 1;
              MI32.mode.willSetTime = 0;
              }
            }
          }
        Response_P(S_JSON_MI32_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_MI32_PAGE:
        if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.payload == 0) XdrvMailbox.payload = MI32.perPage; // ignore 0
            MI32.perPage = XdrvMailbox.payload;
          }
        else XdrvMailbox.payload = MI32.perPage;
        Response_P(S_JSON_MI32_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_MI32_BATTERY:
        MI32EverySecond(true);
        MI32.mode.shallReadBatt = 1;
        MI32.mode.canConnect = 1;
        XdrvMailbox.payload = MI32.period;
        Response_P(S_JSON_MI32_COMMAND, command, "");
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
 * Presentation
\*********************************************************************************************/

const char HTTP_MI32[] PROGMEM = "{s}MI ESP32 {m}%u%s / %u{e}";
const char HTTP_MI32_SERIAL[] PROGMEM = "{s}%s %s{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_VOLTAGE[] PROGMEM = "{s}%s " D_VOLTAGE "{m}%s V{e}";
const char HTTP_MI32_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%uus/cm{e}";
const char HTTP_MI32_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";

void MI32Show(bool json)
{
  if (json) {
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      char slave[33];
      sprintf_P(slave,"%s-%02x%02x%02x",kMI32SlaveType[MIBLEsensors[i].type-1],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]);
      ResponseAppend_P(PSTR(",\"%s\":{"),slave);
        if (MIBLEsensors[i].type==FLORA){
          if(!isnan(MIBLEsensors[i].temp)){ // this is the error code -> no temperature
              char temperature[FLOATSZ]; // all sensors have temperature
              dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, temperature);
              ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), temperature);
            }
          else {
            ResponseAppend_P(PSTR("}"));
            continue;
          }
          if(MIBLEsensors[i].lux!=0x0ffffff){ // this is the error code -> no lux
            ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%u"), MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){
            ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%d"), MIBLEsensors[i].moisture);
          }
          if(!isnan(MIBLEsensors[i].fertility)){
            ResponseAppend_P(PSTR(",\"Fertility\":%d"), MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA){
          if(!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)){
            ResponseAppendTHD(MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        }
        if(MIBLEsensors[i].bat!=0x00){ // this is the error code -> no battery
        if (MIBLEsensors[i].type != LYWSD03MMC) ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors[i].bat);
        else {
          dtostrfd((MIBLEsensors[i].volt)/100.0f, Settings.flag2.temperature_resolution, slave); // reuse slave, borrow temperature resolution
          ResponseAppend_P(PSTR(",\"" D_VOLTAGE "\":%s"), slave);
        }
        }
        ResponseAppend_P(PSTR("}"));
    }
#ifdef USE_WEBSERVER
    } else {
      static  uint16_t _page = 0;
      static  uint16_t _counter = 0;
      int32_t i = _page * MI32.perPage;
      uint32_t j = i + MI32.perPage;
      if (j+1>MIBLEsensors.size()){
        j = MIBLEsensors.size();
      }
      char stemp[5] ={0};
      if (MIBLEsensors.size()-(_page*MI32.perPage)>1 && MI32.perPage!=1) {
        sprintf_P(stemp,"-%u",j);
      }
      if (MIBLEsensors.size()==0) i=-1; // only for the GUI

      WSContentSend_PD(HTTP_MI32, i+1,stemp,MIBLEsensors.size());
      for (i; i<j; i++) {
        WSContentSend_PD(HTTP_MI32_HL);
        WSContentSend_PD(HTTP_MI32_SERIAL, kMI32SlaveType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, MIBLEsensors[i].serial[0], MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]);
        if (MIBLEsensors[i].type==FLORA){
          if(!isnan(MIBLEsensors[i].temp)){
            char temperature[FLOATSZ];
            dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, temperature);
            WSContentSend_PD(HTTP_SNS_TEMP, kMI32SlaveType[MIBLEsensors[i].type-1], temperature, TempUnit());
          }
          if(MIBLEsensors[i].lux!=0x00ffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kMI32SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){
            WSContentSend_PD(HTTP_SNS_MOISTURE, kMI32SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if(!isnan(MIBLEsensors[i].fertility)){
            WSContentSend_PD(HTTP_MI32_FLORA_DATA, kMI32SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA){ // everything "above" Flora
          if(!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)){
            WSContentSend_THD(kMI32SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        }
        if(MIBLEsensors[i].bat!=0x00){
          if (MIBLEsensors[i].type != LYWSD03MMC) WSContentSend_PD(HTTP_BATTERY, kMI32SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
          else {
            char voltage[FLOATSZ];
            dtostrfd((MIBLEsensors[i].volt)/1000.0f, 2, voltage); // borrow temperature resolution
            WSContentSend_PD(HTTP_VOLTAGE, kMI32SlaveType[MIBLEsensors[i].type-1], voltage);
          }
        }
      }
      _counter++;
      if(_counter>3) {
        _page++;
        _counter=0;
      }
      if(MIBLEsensors.size()%MI32.perPage==0 && _page==MIBLEsensors.size()/MI32.perPage) _page=0;
      if(_page>MIBLEsensors.size()/MI32.perPage) _page=0;
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns62(uint8_t function)
{
  bool result = false;
  if (FUNC_INIT == function){
    MI32Init();
  }

  if (MI32.mode.init) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MI32EverySecond(false);
        break;
      case FUNC_COMMAND:
        result = MI32Cmd();
        break;
      case FUNC_JSON_APPEND:
        MI32Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MI32Show(0);
        break;
#endif  // USE_WEBSERVER
      }
  }
  return result;
}
#endif  // USE_MI_ESP32
#endif  // ESP32
