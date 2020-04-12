/*
  xsns_62_MI_HM10.ino - MI-BLE-sensors via HM-10 support for Tasmota

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
  0.9.3.1 20200412  added   - clean ups, code shrink, battery bugfix
  ---
  0.9.3.0 20200322  added   - multi page web view, command HM10PAGE, polling for MJ_HT_V1,
                              more stable readings, internal refactoring
  ---
  0.9.2.0 20200317  added   - MiBeacon-support, add Flora, MJ_HT_V1 and CGD1, add dew point,
                              add AUTO(-scan), RULES-message
  ---
  0.9.1.0 20200209  added   - LYWSD02-support, including setting the time
  ---
  0.9.0.0 20200130  started - initial development by Christian Baars (support LYWSD03 only)
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/
#ifdef USE_HM10

#define XSNS_62                    62

#include <TasmotaSerial.h>
#include <vector>

TasmotaSerial *HM10Serial;
#define HM10_BAUDRATE              115200  // default with FW>700 is 115200

#define  HM10_MAX_TASK_NUMBER      12
uint8_t  HM10_TASK_LIST[HM10_MAX_TASK_NUMBER+1][2];   // first value: kind of task - second value: delay in x * 100ms

#define  HM10_MAX_RX_BUF           64

struct {
  uint8_t current_task_delay;  // number of 100ms-cycles
  uint8_t last_command;
  uint16_t perPage = 4;
  uint16_t firmware;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  uint32_t serialSpeed;
  union {
    uint32_t time;
    uint8_t timebuf[4];
  };
  uint16_t autoScanInterval;
  struct {
    uint32_t awaiting:8;
    uint32_t init:1;
    uint32_t pending_task:1;
    uint32_t connected:1;
    uint32_t subscribed:1;
    uint32_t autoScan:1;
    // TODO: more to come
  } mode;
  struct {
    uint8_t sensor;           // points to to the number 0...255
    // TODO: more to come
  } state;
} HM10;

#pragma pack(1)  // byte-aligned structures to read the sensor data

  struct {
    uint16_t temp;
    uint8_t hum;
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
  uint8_t bat; // all sensors
};

std::vector<mi_sensor_t> MIBLEsensors;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_HM10 "HM10"

const char S_JSON_HM10_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_HM10 "%s\":%d}";
const char S_JSON_HM10_COMMAND[] PROGMEM        = "{\"" D_CMND_HM10 "%s%s\"}";
const char kHM10_Commands[] PROGMEM             = "Scan|AT|Period|Baud|Time|Auto|Page";

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4
#define CGG1        5
#define CGD1        6

const uint16_t kHM10SlaveID[6]={ 0x0098, // Flora
                                  0x01aa, // MJ_HT_V1
                                  0x045b, // LYWSD02
                                  0x055b, // LYWSD03
                                  0x0347, // CGG1
                                  0x0576  // CGD1
                                  };

const char kHM10SlaveType1[] PROGMEM = "Flora";
const char kHM10SlaveType2[] PROGMEM = "MJ_HT_V1";
const char kHM10SlaveType3[] PROGMEM = "LYWSD02";
const char kHM10SlaveType4[] PROGMEM = "LYWSD03";
const char kHM10SlaveType5[] PROGMEM = "CGG1";
const char kHM10SlaveType6[] PROGMEM = "CGD1";
const char * kHM10SlaveType[] PROGMEM = {kHM10SlaveType1,kHM10SlaveType2,kHM10SlaveType3,kHM10SlaveType4,kHM10SlaveType5,kHM10SlaveType6};

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum HM10_Commands {          // commands useable in console or rules
  CMND_HM10_DISC_SCAN,        // re-scan for sensors
  CMND_HM10_AT,               // send AT-command for debugging and special configuration
  CMND_HM10_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_HM10_BAUD,             // serial speed of ESP8266 (<-> HM10), does not change baud rate of HM10
  CMND_HM10_TIME,             // set LYWSD02-Time from ESP8266-time
  CMND_HM10_AUTO,             // do discovery scans permanently to receive MiBeacons in seconds between read-cycles
  CMND_HM10_PAGE              // sensor entries per web page, which will be shown alternated
  };

enum HM10_awaitData: uint8_t {
    none = 0,
    tempHumLY = 1,
    TLMF = 2,
    bat = 3,
    tempHumCGD1 = 4,
    discScan = 5,
    tempHumMJ = 6
    };

/*********************************************************************************************\
 * Task codes defines
\*********************************************************************************************/

#define TASK_HM10_NOTASK          0                         // nothing to be done
#define TASK_HM10_ROLE1           1                         // change role to 1
#define TASK_HM10_IMME1           2                         // change imme to 1
#define TASK_HM10_RENEW           3                         // device factory setting
#define TASK_HM10_RESET           4                         // device reset
#define TASK_HM10_DISC            5                         // device discovery scan: AT+DISA?
#define TASK_HM10_CONN            6                         // connect to given MAC
#define TASK_HM10_VERSION         7                         // query FW version
#define TASK_HM10_NAME            8                         // query device name
#define TASK_HM10_FEEDBACK        9                         // get device response
#define TASK_HM10_DISCONN         10                        // disconnect
#define TASK_HM10_SUB_L3          11                        // subscribe to service handle 37

#define TASK_HM10_SCAN9           13                        // longest discovery scan possible
#define TASK_HM10_UN_L3           14                        // unsubscribe  service handle 37

#define TASK_HM10_READ_BT_L3      16                        // read from handle 3A -> Battery
#define TASK_HM10_SUB_L2          17                        // subscribe to service handle 3C
#define TASK_HM10_UN_L2           18                        // unsubscribe  service handle 3C
#define TASK_HM10_READ_BT_L2      19                        // read from handle 43 -> Battery
#define TASK_HM10_TIME_L2         20                        // set time of LYWSD02 to system time
#define TASK_HM10_SHOW0           21                        // set verbositiy to minimum
#define TASK_HM10_READ_BF_FL      22                        // read battery and firmware from flower care
#define TASK_HM10_CALL_TLMF_FL    23                        // write 0xa01f to handle 0x33 to init sensor readings
#define TASK_HM10_READ_TLMF_FL    24                        // read temp,lux,moist and fert from flower care
#define TASK_HM10_SUB_HT_CGD1     25                        // subscribe to service handle 4b
#define TASK_HM10_UN_HT_CGD1      26                        // unsubscribe  service handle 4b
#define TASK_HM10_READ_B_CGD1     27                        // read service handle 11

#define TASK_HM10_READ_B_MJ       29                        // read service handle 18
#define TASK_HM10_SUB_HT_MJ       30                        // subscribe to service handle 0f

#define TASK_HM10_STATUS_EVENT    32                        // process status for RULES

#define TASK_HM10_DONE            99                        // used, if there was a task in the slot or just to wait

/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

void HM10_Launchtask(uint8_t task, uint8_t slot, uint8_t delay){
                          HM10_TASK_LIST[slot][0]   = task;
                          HM10_TASK_LIST[slot][1]   = delay;
                          HM10_TASK_LIST[slot+1][0] = TASK_HM10_NOTASK;           // the tasks must always be launched in ascending order!!
                          HM10.current_task_delay   = HM10_TASK_LIST[0][1];
}

void HM10_TaskReplaceInSlot(uint8_t task, uint8_t slot){
                          HM10.last_command         = HM10_TASK_LIST[slot][0];    // save command
                          HM10_TASK_LIST[slot][0]   = task;  
}

void HM10_ReverseMAC(uint8_t _mac[]){
  uint8_t _reversedMAC[6];
  for (uint8_t i=0; i<6; i++){
    _reversedMAC[5-i] = _mac[i];
  }
  memcpy(_mac,_reversedMAC, sizeof(_reversedMAC));
}

/*********************************************************************************************\
 * chained tasks
\*********************************************************************************************/

void HM10_Reset(void) {   HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_ROLE1,1,1);         // set role to 1
                          HM10_Launchtask(TASK_HM10_IMME1,2,1);         // set imme to 1
                          HM10_Launchtask(TASK_HM10_RESET,3,1);         // reset Device
                          HM10_Launchtask(TASK_HM10_VERSION,4,10);      // read SW Version
                          HM10_Launchtask(TASK_HM10_SCAN9,5,2);         // scan time 9 seconds
                          HM10_Launchtask(TASK_HM10_DISC,6,2);          // discovery
                          HM10_Launchtask(TASK_HM10_STATUS_EVENT,7,2);  // status
                          }

void HM10_Discovery_Scan(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_DISC,1,5);          // discovery
                          HM10_Launchtask(TASK_HM10_STATUS_EVENT,2,2);  // status
                          }

void HM10_Read_LYWSD03(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_L3,2,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_UN_L3,3,80);         // unsubscribe
                          HM10_Launchtask(TASK_HM10_READ_BT_L3,4,5);     // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,5,5);        // disconnect
                          }

void HM10_Read_LYWSD02(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_L2,2,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_UN_L2,3,80);         // unsubscribe
                          HM10_Launchtask(TASK_HM10_READ_BT_L2,4,5);     // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,5,5);        // disconnect
                          }

void HM10_Time_LYWSD02(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,0);        // disconnect
                          HM10_Launchtask(TASK_HM10_CONN,1,5);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,2,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_TIME_L2,3,20);       // subscribe
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

void HM10_Read_Flora(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,0);        // disconnect
                          HM10_Launchtask(TASK_HM10_CONN,1,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,2,5);       // get OK+CONN
                          HM10_Launchtask(TASK_HM10_READ_BF_FL,3,20);    // read battery
                          HM10_Launchtask(TASK_HM10_CALL_TLMF_FL,4,30);  // read TLMF
                          HM10_Launchtask(TASK_HM10_DISCONN,5,10);       // disconnect
                          }

void HM10_Read_CGD1(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUB_HT_CGD1,2,20);   // subscribe
                          HM10_Launchtask(TASK_HM10_UN_HT_CGD1,3,10);    // unsubscribe
                          HM10_Launchtask(TASK_HM10_READ_B_CGD1,4,5);    // read Battery
                          HM10_Launchtask(TASK_HM10_DISCONN,5,5);        // disconnect
                          }

void HM10_Read_MJ_HT_V1(void) {
                          HM10_Launchtask(TASK_HM10_CONN,0,1);           // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,1,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_READ_B_MJ,2,20);     // battery
                          HM10_Launchtask(TASK_HM10_SUB_HT_MJ,3,10);     // temp hum
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_serial)[6], uint16_t _type){

  DEBUG_SENSOR_LOG(PSTR("%s: will test ID-type: %x"),D_CMND_HM10, _type);
  bool _success = false;
  for (uint32_t i=0;i<6;i++){ // i < sizeof(kHM10SlaveID) gives compiler warning
    if(_type == kHM10SlaveID[i]){
      DEBUG_SENSOR_LOG(PSTR("HM10: ID is type %u"), i);
      _type = i+1;
      _success = true;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("%s: ID-type is not: %x"),D_CMND_HM10,kHM10SlaveID[i]);
    }
  }
  if(!_success) return 0xff;
 
  DEBUG_SENSOR_LOG(PSTR("%s: vector size %u"),D_CMND_HM10, MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors[i].serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("%s: known sensor at slot: %u"),D_CMND_HM10, i);
      if(MIBLEsensors[i].showedUp < 4){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors[i].showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: i: %x %x %x %x %x %x"),D_CMND_HM10, MIBLEsensors[i].serial[5], MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[0]);
    DEBUG_SENSOR_LOG(PSTR("%s: n: %x %x %x %x %x %x"),D_CMND_HM10, _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("%s: found new sensor"),D_CMND_HM10);
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 1;
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
  AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: new %s at slot: %u"),D_CMND_HM10, kHM10SlaveType[_type-1],MIBLEsensors.size()-1);
  return MIBLEsensors.size()-1;
};

/*********************************************************************************************\
 * init serial
 * define serial rx/tx port fixed with 115200 baud
\*********************************************************************************************/

void HM10SerialInit(void) {
  HM10.mode.init = false;
  HM10.serialSpeed = HM10_BAUDRATE;
  HM10Serial = new TasmotaSerial(pin[GPIO_HM10_RX], pin[GPIO_HM10_TX], 1, 0, HM10_MAX_RX_BUF);
  if (HM10Serial->begin(HM10.serialSpeed)) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s start serial communication fixed to 115200 baud"),D_CMND_HM10);
    if (HM10Serial->hardwareSerial()) {
      ClaimSerial();
      DEBUG_SENSOR_LOG(PSTR("%s: claim HW"),D_CMND_HM10);
    }
    HM10_Reset();
    HM10.mode.pending_task = 1;
    HM10.mode.init = 1;
    HM10.period = Settings.tele_period;
    DEBUG_SENSOR_LOG(PSTR("%s_TASK_LIST initialized, now return to main loop"),D_CMND_HM10);
  }
  return;
}

/*********************************************************************************************\
 * parse the response
\*********************************************************************************************/

void HM10parseMiBeacon(char * _buf, uint32_t _slot){
  float _tempFloat;
  mi_beacon_t _beacon;
  if (MIBLEsensors[_slot].type==MJ_HT_V1 || MIBLEsensors[_slot].type==CGG1){
    memcpy((uint8_t*)&_beacon+1,(uint8_t*)_buf, sizeof(_beacon)); // shift by one byte for the MJ_HT_V1
    memcpy((uint8_t*)&_beacon.Mac,(uint8_t*)&_beacon.Mac+1,6);    // but shift back the MAC
  }
  else{
    memcpy((void*)&_beacon,(void*)_buf, sizeof(_beacon));
  }
  HM10_ReverseMAC(_beacon.Mac);
  if(memcmp(_beacon.Mac,MIBLEsensors[_slot].serial,sizeof(_beacon.Mac))!=0){
    if (MIBLEsensors[_slot].showedUp>3) return; // probably false alarm from a damaged packet
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: remove garbage sensor"),D_CMND_HM10);
    DEBUG_SENSOR_LOG(PSTR("%s i: %x %x %x %x %x %x"),D_CMND_HM10, MIBLEsensors[_slot].serial[5], MIBLEsensors[_slot].serial[4],MIBLEsensors[_slot].serial[3],MIBLEsensors[_slot].serial[2],MIBLEsensors[_slot].serial[1],MIBLEsensors[_slot].serial[0]);
    DEBUG_SENSOR_LOG(PSTR("%s n: %x %x %x %x %x %x"),D_CMND_HM10, _beacon.Mac[5], _beacon.Mac[4], _beacon.Mac[3],_beacon.Mac[2],_beacon.Mac[1],_beacon.Mac[0]);
    MIBLEsensors.erase(MIBLEsensors.begin()+_slot);
    return;
  }
  if (MIBLEsensors[_slot].showedUp<4) MIBLEsensors[_slot].showedUp++;

  DEBUG_SENSOR_LOG(PSTR("MiBeacon type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[0],(uint8_t)_buf[1],(uint8_t)_buf[2],(uint8_t)_buf[3],(uint8_t)_buf[4],(uint8_t)_buf[5],(uint8_t)_buf[6],(uint8_t)_buf[7]);
  DEBUG_SENSOR_LOG(PSTR("         type:%02x: %02x %02x %02x %02x %02x %02x %02x %02x"),_beacon.type, (uint8_t)_buf[8],(uint8_t)_buf[9],(uint8_t)_buf[10],(uint8_t)_buf[11],(uint8_t)_buf[12],(uint8_t)_buf[13],(uint8_t)_buf[14],(uint8_t)_buf[15]);

  if(MIBLEsensors[_slot].type==4 || MIBLEsensors[_slot].type==6){
    DEBUG_SENSOR_LOG(PSTR("LYWSD03 and CGD1 no support for MiBeacon, type %u"),MIBLEsensors[_slot].type);
    return;
  }
  DEBUG_SENSOR_LOG(PSTR("%s at slot %u"), kHM10SlaveType[MIBLEsensors[_slot].type-1],_slot);
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

void HM10ParseResponse(char *buf, uint16_t bufsize) {
    if (!strncmp(buf,"OK",2)) {
       DEBUG_SENSOR_LOG(PSTR("%s: got OK"),D_CMND_HM10);
     }
    if (!strncmp(buf,"HMSoft",6)) { //8
      const char* _fw = "000";
      memcpy((void *)_fw,(void *)(buf+8),3);
      HM10.firmware = atoi(_fw);
      DEBUG_SENSOR_LOG(PSTR("%s: Firmware: %d"),D_CMND_HM10, HM10.firmware);
      return;
     }
    char * _pos = strstr(buf, "ISA:");
    if(_pos) {
      uint8_t _newMacArray[6] = {0};
      memcpy((void *)_newMacArray,(void *)(_pos+4),6);
      HM10_ReverseMAC(_newMacArray);
      DEBUG_SENSOR_LOG(PSTR("%s:  MAC-array: %02x%02x%02x%02x%02x%02x"),D_CMND_HM10,_newMacArray[0],_newMacArray[1],_newMacArray[2],_newMacArray[3],_newMacArray[4],_newMacArray[5]);
      uint16_t _type=0xffff;

      for (uint32_t idx =10;idx<bufsize;idx++){
        if((uint8_t)buf[idx] == 0xfe){
          if((uint8_t)buf[idx-2] == 0x16 && (uint8_t)buf[idx-1] == 0x95){
            _type = _pos[idx]*256 + _pos[idx-1];
            DEBUG_SENSOR_LOG(PSTR("%s: type %04x _ %02x %02x"),D_CMND_HM10,_type, _pos[idx-1],_pos[idx]);
            _pos = _pos+idx-3;
            break;
          }
        }
      }
      uint16_t _slot = MIBLEgetSensorSlot(_newMacArray, _type);
      if(_slot!=0xff) HM10parseMiBeacon(_pos,_slot);
    }
    else if (strstr(buf, "LOST")){
      HM10.mode.connected = false;
    }
    else if (strstr(buf, "CONN")){
      HM10.current_task_delay = 0;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("%s: empty response"),D_CMND_HM10);
    }
}

void HM10readHT_LY(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return; // "OK"
  if(_buf[0] != 0 && _buf[1] != 0){
    memcpy(&LYWSD0x_HT,(void *)_buf,3);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: T * 100: %u, H: %u"),D_CMND_HM10,LYWSD0x_HT.temp,LYWSD0x_HT.hum);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(LYWSD0x_HT.temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        HM10.mode.awaiting = none;
        HM10.current_task_delay = 0;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)LYWSD0x_HT.hum;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("LYWSD0x: hum updated"));
    }
  }
}

void HM10readHT_CGD1(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return; // "OK"
  if(_buf[0] == 0){
    if(_buf[1]==0 && _buf[2]==0 && _buf[3]==0 && _buf[4]==0) return;
    memcpy(&CGD1_HT,(void *)_buf,5);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: T * 100: %u, H * 100: %u"),D_CMND_HM10,CGD1_HT.temp,CGD1_HT.hum);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(CGD1_HT.temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        HM10.mode.awaiting = none;
        HM10.current_task_delay = 0;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    _tempFloat=(float)CGD1_HT.hum/100.0f;
    if(_tempFloat<100){
      MIBLEsensors[_slot].hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("CGD1: hum updated"));
    }
  }
}

void HM10readHT_MJ_HT_V1(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]!=0x54 && _buf[1]!=0x3d) return; //"T="
  // T=22.7 H=42.2 (response as ASCII)
  // 0123456789012
  uint32_t _temp = (atoi(_buf+2) * 10) + atoi(_buf+5);
  uint32_t _hum = (atoi(_buf+9) * 10) + atoi(_buf+12); 
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: T * 10: %u, H * 10: %u"),D_CMND_HM10,_temp,_hum);
  uint32_t _slot = HM10.state.sensor;

  DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
  static float _tempFloat;
  _tempFloat=(float)_temp/10.0f;
  if(_tempFloat<60){
      MIBLEsensors[_slot].temp=_tempFloat;
      HM10.mode.awaiting = none;
      HM10.current_task_delay = 0;
      MIBLEsensors[_slot].showedUp=255; // this sensor is real
  }
  _tempFloat=(float)_hum/10.0f;
  if(_tempFloat<100){
    MIBLEsensors[_slot].hum = _tempFloat;
    DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: hum updated"));
  }
}

void HM10readTLMF(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return; // "OK"
  if(_buf[0] != 0 || _buf[1] != 0){ // this will lose 0.0 degree, but it is not possible to measure a successful reading
    memcpy(&Flora_TLMF,(void *)_buf,10);
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: T * 10: %u, L: %u, M: %u, F: %u"),D_CMND_HM10,Flora_TLMF.temp,Flora_TLMF.lux,Flora_TLMF.moist,Flora_TLMF.fert);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(Flora_TLMF.temp)/10.0f;
    if(_tempFloat<60){
        MIBLEsensors[_slot].temp=_tempFloat;
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
    }
    MIBLEsensors[_slot].lux = Flora_TLMF.lux;
    _tempFloat=(float)Flora_TLMF.moist;
    if(_tempFloat<100){
      MIBLEsensors[_slot].moisture = _tempFloat;
    }

    MIBLEsensors[_slot].fertility = (float)Flora_TLMF.fert;

    HM10.mode.awaiting = none;
    HM10.current_task_delay = 0;
  }
}

bool HM10readBat(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("%s: raw data: %x%x%x%x%x%x%x"),D_CMND_HM10,_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0]==0x4f && _buf[1]==0x4b) return false; // "OK"
  if(_buf[0] != 0){
    AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s: Battery: %u"),D_CMND_HM10,_buf[0]);
    uint32_t _slot = HM10.state.sensor;
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    if(_buf[0]<101){
        MIBLEsensors[_slot].bat=_buf[0];
        MIBLEsensors[_slot].showedUp=255; // this sensor is real
        return true;
    }
  }
  return false;
}

/*********************************************************************************************\
 * handle the return value from the HM10
\*********************************************************************************************/

bool HM10SerialHandleFeedback(){                  // every 50 milliseconds
  bool success    = false;
  uint32_t i       = 0;
  static char ret[HM10_MAX_RX_BUF] = {0};
  
  while(HM10Serial->available()) {
    // delay(0);
    if(i<HM10_MAX_RX_BUF){
      ret[i] = HM10Serial->read();
    }
    i++;
    success = true;
  }

  if(i==0) return success;

  switch (HM10.mode.awaiting){
    case bat:
      if (HM10.mode.connected) {
        if (HM10readBat(ret)){
          HM10.mode.awaiting = none;
          HM10.current_task_delay = 0;
        }
      }
      break;
    case tempHumLY:
      if (HM10.mode.connected) HM10readHT_LY(ret);
      break;
    case tempHumCGD1:
      if (HM10.mode.connected) HM10readHT_CGD1(ret);
      break;
    case TLMF:
      if (HM10.mode.connected) HM10readTLMF(ret);
      break;
    case discScan:
      if(success) {
        HM10ParseResponse(ret,i);
      }
    break;
    case tempHumMJ:
      if (HM10.mode.connected) HM10readHT_MJ_HT_V1(ret);
      break;    
    case none:
      if(success) {
        AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: response: %s"),D_CMND_HM10, (char *)ret);
        // for(uint32_t j = 0; j<i+1; j+=8){
        //   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%02x %02x %02x %02x %02x %02x %02x %02x"),(uint8_t)ret[j],(uint8_t)ret[j+1],(uint8_t)ret[j+2],(uint8_t)ret[j+3],(uint8_t)ret[j+4],(uint8_t)ret[j+5],(uint8_t)ret[j+6],(uint8_t)ret[j+7]);
        // }
        HM10ParseResponse(ret,i);
      }
    break;
  }
  memset(ret,0,i); // wipe away the recent bytes
  return success;
}

/*********************************************************************************************\
 * execute the next Task
\*********************************************************************************************/

void HM10_TaskEvery100ms(){
  if (HM10.current_task_delay == 0)  {
    uint8_t i = 0;
    bool runningTaskLoop = true;
    while (runningTaskLoop) {                                        // always iterate through the whole task list
      switch(HM10_TASK_LIST[i][0]) {                                 // handle the kind of task
        case TASK_HM10_ROLE1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: set role to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+ROLE1");
          break;
        case TASK_HM10_IMME1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: set imme to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+IMME1");
          break;
        case TASK_HM10_DISC:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: start discovery"),D_CMND_HM10);
          HM10.current_task_delay = 100;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = discScan;
          HM10Serial->write("AT+DISA?");
          break;
        case TASK_HM10_VERSION:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read version"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+VERR?");
          break;
        case TASK_HM10_NAME:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read name"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NAME?");
          break;
        case TASK_HM10_CONN:
          char _con[20];
          sprintf_P(_con,"AT+CON%02x%02x%02x%02x%02x%02x",MIBLEsensors[HM10.state.sensor].serial[0],MIBLEsensors[HM10.state.sensor].serial[1],MIBLEsensors[HM10.state.sensor].serial[2],MIBLEsensors[HM10.state.sensor].serial[3],MIBLEsensors[HM10.state.sensor].serial[4],MIBLEsensors[HM10.state.sensor].serial[5]); 
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: connect %s"),D_CMND_HM10, _con);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write(_con);
          HM10.mode.awaiting = none;
          HM10.mode.connected = true;
          break;
        case TASK_HM10_DISCONN:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: disconnect"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT");
          break;
        case TASK_HM10_RESET:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: Reset Device"),D_CMND_HM10);
          HM10Serial->write("AT+RESET");
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          break;
        case TASK_HM10_SUB_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 25;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          HM10.mode.awaiting = tempHumLY;
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON0037");
          break;
        case TASK_HM10_UN_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          HM10Serial->write("AT+NOTIFYOFF0037");
          break;
        case TASK_HM10_SUB_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 25;                   // set task delay
          HM10.mode.awaiting = tempHumLY;
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON003C");
          break;
        case TASK_HM10_UN_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          HM10Serial->write("AT+NOTIFYOFF003C");
          break;
        case TASK_HM10_TIME_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: set time"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.time = Rtc.utc_time;
          HM10Serial->write("AT+SEND_DATAWR002F");
          HM10Serial->write(HM10.timebuf,4);
          HM10Serial->write(Rtc.time_timezone / 60);
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s Time-string: %x%x%x%x%x"),D_CMND_HM10, HM10.timebuf[0],HM10.timebuf[1],HM10.timebuf[2],HM10.timebuf[3],(Rtc.time_timezone /60));
          break;
        case TASK_HM10_READ_BT_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 003A"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA003A?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_READ_BT_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0043"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0043?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_READ_BF_FL:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0038"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0038?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_CALL_TLMF_FL:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: write to handle 0033"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_READ_TLMF_FL,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+SEND_DATAWR0033");
          HM10Serial->write(0xa0);
          HM10Serial->write(0x1f);
          HM10.mode.awaiting = none;
          break;
        case TASK_HM10_READ_TLMF_FL:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0035"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0035?");
          HM10.mode.awaiting = TLMF;
          break;
        case TASK_HM10_READ_B_CGD1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0011"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0011?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_SUB_HT_CGD1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: subscribe 4b"),D_CMND_HM10);
          HM10.current_task_delay = 5;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = tempHumCGD1;
          HM10Serial->write("AT+NOTIFY_ON004b");
          break;
        case TASK_HM10_UN_HT_CGD1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: un-subscribe 4b"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaiting = none;
          HM10Serial->write("AT+NOTIFYOFF004b");
          break;
        case TASK_HM10_SCAN9:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: scan time to 9"),D_CMND_HM10);
          HM10.current_task_delay = 2;                  // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+SCAN9");
          break;
        case TASK_HM10_READ_B_MJ:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: read handle 0x18"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0018?");
          HM10.mode.awaiting = bat;
          break;
        case TASK_HM10_SUB_HT_MJ:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: subscribe to 0x0f"),D_CMND_HM10);
          HM10.current_task_delay = 10;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON000F");
          HM10.mode.awaiting = tempHumMJ;
          break;
        case TASK_HM10_FEEDBACK:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: get response"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;
        case TASK_HM10_STATUS_EVENT:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s: show status"),D_CMND_HM10);
          HM10StatusInfo();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;
        case TASK_HM10_DONE:                                    // this entry was already handled
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sFound done HM10_TASK"),D_CMND_HM10);
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%snext slot:%u, i: %u"),D_CMND_HM10, HM10_TASK_LIST[i+1][0],i);
          if(HM10_TASK_LIST[i+1][0] == TASK_HM10_NOTASK) {             // check the next entry and if there is none
            DEBUG_SENSOR_LOG(PSTR("%sno Tasks left"),D_CMND_HM10);
            DEBUG_SENSOR_LOG(PSTR("%sHM10_TASK_DONE current slot %u"),D_CMND_HM10, i);
            for (uint8_t j = 0; j < HM10_MAX_TASK_NUMBER+1; j++) {   // do a clean-up:
              DEBUG_SENSOR_LOG(PSTR("%sHM10_TASK cleanup slot %u"),D_CMND_HM10, j);
              HM10_TASK_LIST[j][0] = TASK_HM10_NOTASK;                // reset all task entries
              HM10_TASK_LIST[j][1] = 0;                              // reset all delays
            }
            runningTaskLoop = false;                                  // return to main loop
            HM10.mode.pending_task = 0;                               // back to main loop control
            break; 
          }
      }
      i++; 
    }
  }
  else {
    HM10.current_task_delay--;               // count down every 100 ms
  }
}

void HM10StatusInfo(){
  char stemp[20];
  snprintf_P(stemp, sizeof(stemp),PSTR("{%s:{\"found\": %u}}"),D_CMND_HM10, MIBLEsensors.size());
  AddLog_P2(LOG_LEVEL_INFO, stemp);
  RulesProcessEvent(stemp);
}

/**
 * @brief Main loop of the driver, "high level"-loop
 * 
 */

void HM10EverySecond(bool restart){
  static uint32_t _counter = 0;
  static uint32_t _nextSensorSlot = 0;
  static uint32_t _lastDiscovery = 0;

  if(restart){
    _counter = 0;
    _lastDiscovery = 0;
    return;
  }

  if(HM10.firmware == 0) return;
  if(HM10.mode.pending_task == 1) return;
  if(MIBLEsensors.size()==0 && !HM10.mode.autoScan) return;

  if((HM10.period-_counter)>15 && HM10.mode.autoScan) {
    if(_counter-_lastDiscovery>HM10.autoScanInterval){
      HM10_Discovery_Scan();
      HM10.mode.pending_task = 1;
      _counter+=12;
      _lastDiscovery = _counter;
      return;
    }
  }

  if(_counter==0) {
    HM10.state.sensor = _nextSensorSlot;
    _nextSensorSlot++;
    HM10.mode.pending_task = 1;
    switch(MIBLEsensors[HM10.state.sensor].type){
      case FLORA:
        HM10_Read_Flora();
        break;
      case MJ_HT_V1: case CGG1:
        HM10_Read_MJ_HT_V1();
        break;
      case LYWSD02:
        HM10_Read_LYWSD02();
        break;
      case LYWSD03MMC:
        HM10_Read_LYWSD03();
        break;
      case CGD1:
        HM10_Read_CGD1();
        break;
      default:
        HM10.mode.pending_task = 0;
    }
    if (HM10.state.sensor==MIBLEsensors.size()-1) {
      _nextSensorSlot= 0;
      _counter++;
    }
    DEBUG_SENSOR_LOG(PSTR("%s: active sensor now: %u"),D_CMND_HM10, HM10.state.sensor);
  }
  else _counter++;
  if (_counter>HM10.period) {
    _counter = 0;
    _lastDiscovery = 0;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool HM10Cmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_HM10);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_HM10), disp_len)) {  // prefix
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kHM10_Commands);
    switch (command_code) {
      case CMND_HM10_PERIOD:
        if (XdrvMailbox.data_len > 0) {
          if (XdrvMailbox.payload==1) {
            HM10EverySecond(true);
            XdrvMailbox.payload = HM10.period;
            }
          else {
            HM10.period = XdrvMailbox.payload;
          }
        }
        else {
          XdrvMailbox.payload = HM10.period;
        }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_AUTO:
        if (XdrvMailbox.data_len > 0) {
          if (XdrvMailbox.payload>0) {
            HM10.mode.autoScan = 1;
            HM10.autoScanInterval = XdrvMailbox.payload;
          }
          else {
            HM10.mode.autoScan = 0;
            HM10.autoScanInterval = 0;
          }
        }
        else {
          XdrvMailbox.payload = HM10.autoScanInterval;
        }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_BAUD:
        if (XdrvMailbox.data_len > 0) {
            HM10.serialSpeed = XdrvMailbox.payload;
            HM10Serial->begin(HM10.serialSpeed);
        }
        else {
          XdrvMailbox.payload = HM10.serialSpeed;
        }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_TIME:
        if (XdrvMailbox.data_len > 0) {
          if(MIBLEsensors.size()>XdrvMailbox.payload){
            if(MIBLEsensors[XdrvMailbox.payload].type == LYWSD02){
              HM10.state.sensor = XdrvMailbox.payload;
              HM10_Time_LYWSD02();
              }
            }
          }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_PAGE:
        if (XdrvMailbox.data_len > 0) {
            if (XdrvMailbox.payload == 0) XdrvMailbox.payload = HM10.perPage; // ignore 0
            HM10.perPage = XdrvMailbox.payload;
          }
        else XdrvMailbox.payload = HM10.perPage;
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_AT:
        HM10Serial->write("AT");               // without an argument this will disconnect
        if (strlen(XdrvMailbox.data)!=0) {
          HM10Serial->write("+");
          HM10Serial->write(XdrvMailbox.data); // pass everything without checks
          Response_P(S_JSON_HM10_COMMAND, ":AT+",XdrvMailbox.data);
        }
        else Response_P(S_JSON_HM10_COMMAND, ":AT",XdrvMailbox.data);
        break;
      case CMND_HM10_DISC_SCAN:
        HM10_Discovery_Scan();
        Response_P(S_JSON_HM10_COMMAND, command, "");
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

const char HTTP_HM10[] PROGMEM = "{s}HM10 V%u{m}%u%s / %u{e}";
const char HTTP_HM10_SERIAL[] PROGMEM = "{s}%s %s{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_HM10_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%uus/cm{e}";
const char HTTP_HM10_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";

void HM10Show(bool json)
{
  if (json) {
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      char slave[33];
      sprintf_P(slave,"%s-%02x%02x%02x",kHM10SlaveType[MIBLEsensors[i].type-1],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]);
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
          ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors[i].bat);
        }      
        ResponseAppend_P(PSTR("}"));
    }
#ifdef USE_WEBSERVER
    } else {
      static  uint16_t _page = 0;
      static  uint16_t _counter = 0;
      int32_t i = _page * HM10.perPage;
      uint32_t j = i + HM10.perPage;
      if (j+1>MIBLEsensors.size()){
        j = MIBLEsensors.size();
      }
      char stemp[5] ={0};
      if (MIBLEsensors.size()-(_page*HM10.perPage)>1 && HM10.perPage!=1) {
        sprintf_P(stemp,"-%u",j);
      }
      if (MIBLEsensors.size()==0) i=-1; // only for the GUI

      WSContentSend_PD(HTTP_HM10, HM10.firmware, i+1,stemp,MIBLEsensors.size());
      for (i; i<j; i++) {
        WSContentSend_PD(HTTP_HM10_HL);
        WSContentSend_PD(HTTP_HM10_SERIAL, kHM10SlaveType[MIBLEsensors[i].type-1], D_MAC_ADDRESS, MIBLEsensors[i].serial[0], MIBLEsensors[i].serial[1],MIBLEsensors[i].serial[2],MIBLEsensors[i].serial[3],MIBLEsensors[i].serial[4],MIBLEsensors[i].serial[5]); 
        if (MIBLEsensors[i].type==FLORA){
          if(!isnan(MIBLEsensors[i].temp)){
            char temperature[FLOATSZ];
            dtostrfd(MIBLEsensors[i].temp, Settings.flag2.temperature_resolution, temperature);
            WSContentSend_PD(HTTP_SNS_TEMP, kHM10SlaveType[MIBLEsensors[i].type-1], temperature, TempUnit());
          }
          if(MIBLEsensors[i].lux!=0x00ffffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kHM10SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].lux);
          }
          if(!isnan(MIBLEsensors[i].moisture)){
            WSContentSend_PD(HTTP_SNS_MOISTURE, kHM10SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].moisture);
          }
          if(!isnan(MIBLEsensors[i].fertility)){
            WSContentSend_PD(HTTP_HM10_FLORA_DATA, kHM10SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].fertility);
          }
        }
        if (MIBLEsensors[i].type>FLORA){ // everything "above" Flora
          if(!isnan(MIBLEsensors[i].hum) && !isnan(MIBLEsensors[i].temp)){
            WSContentSend_THD(kHM10SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].temp, MIBLEsensors[i].hum);
          }
        } 
        if(MIBLEsensors[i].bat!=0x00){
          WSContentSend_PD(HTTP_BATTERY, kHM10SlaveType[MIBLEsensors[i].type-1], MIBLEsensors[i].bat);
        }
      }
      _counter++;
      if(_counter>3) {
        _page++;
        _counter=0;
      }
      if(MIBLEsensors.size()%HM10.perPage==0 && _page==MIBLEsensors.size()/HM10.perPage) _page=0;
      if(_page>MIBLEsensors.size()/HM10.perPage) _page=0;
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns62(uint8_t function)
{
  bool result = false;

  if ((pin[GPIO_HM10_RX] < 99) && (pin[GPIO_HM10_TX] < 99)) { 
    switch (function) {
      case FUNC_INIT:
        HM10SerialInit();                                  // init and start communication
        break;
      case FUNC_EVERY_50_MSECOND:
        HM10SerialHandleFeedback();                        // check for device feedback very often
       break;
      case FUNC_EVERY_100_MSECOND:
        if (HM10_TASK_LIST[0][0] != TASK_HM10_NOTASK) {
          HM10_TaskEvery100ms();                           // something has to be done, we'll check in the next step
        }
        break;
      case FUNC_EVERY_SECOND:
        HM10EverySecond(false);
        break;
      case FUNC_COMMAND:
        result = HM10Cmd();
        break;
      case FUNC_JSON_APPEND:
        HM10Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        HM10Show(0);
        break;
#endif  // USE_WEBSERVER
      }
  }
  return result;
}
#endif //USE_HM10
