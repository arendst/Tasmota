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

  0.9.1.0 20200130  added   - LYWSD02-support, including setting the time
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

#define  HM10_MAX_RX_BUF         512
char     HM10_RX_STRING[HM10_MAX_RX_BUF]      = {0};

struct {
  uint8_t current_task_delay;  // number of 100ms-cycles
  uint8_t last_command;
  uint16_t firmware;
  uint32_t period;             // set manually in addition to TELE-period, is set to TELE-period after start
  uint32_t serialSpeed;
  union {
    uint32_t time;
    uint8_t timebuf[4];
  };
  struct {
    uint32_t init:1;
    uint32_t pending_task:1;
    uint32_t connected:1;
    uint32_t subscribed:1;
    uint32_t awaitingHT:1;
    uint32_t awaitingB:1;
    // TODO: more to come
  } mode;
  struct {
    uint8_t sensor;           // points to to the number 0...255
    // TODO: more to come
  } state;
} HM10;

#pragma pack(1)
struct {
  uint16_t temp;
  uint8_t hum;
} LYWSD0x_HT;
#pragma pack(0)

struct mi_sensor_t{
  uint8_t type; //Flora = 1; MI-HT_V1=2; LYWSD02=3; LYWSD03=4
  uint8_t serial[6];
  uint8_t showedUp;
  float temp; //Flora, MJ_HT_V1, LYWSD0x
  union {
    struct {
      float moisture;
      float fertility;
      uint16_t lux;
    }; // Flora
    struct {
      float hum;
      uint8_t bat;
    }; // MJ_HT_V1, LYWSD0x
  };
};

std::vector<mi_sensor_t> MIBLEsensors;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_HM10 "HM10"

const char S_JSON_HM10_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_HM10 "%s\":%d}";
const char S_JSON_HM10_COMMAND[] PROGMEM        = "{\"" D_CMND_HM10 "%s%s\"}";
const char kHM10_Commands[] PROGMEM             = "Scan|AT|Period|Baud|Time";

#define FLORA       1
#define MJ_HT_V1    2
#define LYWSD02     3
#define LYWSD03MMC  4

uint8_t kHM10SlaveID[4][3]  = { 0xC4,0x7C,0x8D, // Flora
                                0x58,0x2D,0x34, // MJ_HT_V1
                                0xE7,0x2E,0x00, // LYWSD02
                                0xA4,0xC1,0x38, // LYWSD03
                                };

const char kHM10SlaveType1[] PROGMEM = "Flora";
const char kHM10SlaveType2[] PROGMEM = "MJ_HT_V1";
const char kHM10SlaveType3[] PROGMEM = "LYWSD02";
const char kHM10SlaveType4[] PROGMEM = "LYWSD03";
const char * kHM10SlaveType[] PROGMEM = {kHM10SlaveType1,kHM10SlaveType2,kHM10SlaveType3,kHM10SlaveType4};

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum HM10_Commands {          // commands useable in console or rules
  CMND_HM10_DISC_SCAN,        // re-scan for sensors
  CMND_HM10_AT,               // send AT-command for debugging and special configuration
  CMND_HM10_PERIOD,           // set period like TELE-period in seconds between read-cycles
  CMND_HM10_BAUD,             // serial speed of ESP8266 (<-> HM10), does not change baud rate of HM10
  CMND_HM10_TIME              // set LYWSD02-Time from ESP8266-time 
  };

/*********************************************************************************************\
 * Task codes defines
\*********************************************************************************************/

#define TASK_HM10_NOTASK          0                         // nothing to be done
#define TASK_HM10_ROLE1           1                         // change role to 1
#define TASK_HM10_IMME1           2                         // change imme to 1
#define TASK_HM10_RENEW           3                         // device factory setting
#define TASK_HM10_RESET           4                         // device reset
#define TASK_HM10_DISC            5                         // device discovery scan
#define TASK_HM10_CONN            6                         // connect to given MAC
#define TASK_HM10_VERSION         7                         // query FW version
#define TASK_HM10_NAME            8                         // query device name
#define TASK_HM10_FEEDBACK        9                         // get device response
#define TASK_HM10_DISCONN         10                        // disconnect
#define TASK_HM10_SUB_L3          11                        // subscribe to service handle 37
#define TASK_HM10_READ_HT         12                        // read from handle 36 -> Hum & Temp
#define TASK_HM10_FINDALLCHARS    13                        // read all available characteristics
#define TASK_HM10_UN_L3           14                        // subscribe  service handle 37
#define TASK_HM10_DELAY_SUB       15                        // start reading from subscription delayed
#define TASK_HM10_READ_BT_L3      16                        // read from handle 3A -> Battery
#define TASK_HM10_SUB_L2          17                        // subscribe to service handle 3C
#define TASK_HM10_UN_L2           18                        // subscribe  service handle 3C
#define TASK_HM10_READ_BT_L2      19                        // read from handle 43 -> Battery
#define TASK_HM10_TIME_L2         20                        // set time of LYWSD02 to system time

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

/*********************************************************************************************\
 * chained tasks
\*********************************************************************************************/

void HM10_Reset(void) {   HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_ROLE1,1,1);         // set role to 1
                          HM10_Launchtask(TASK_HM10_IMME1,2,1);         // set imme to 1
                          HM10_Launchtask(TASK_HM10_RESET,3,1);         // reset Device
                          HM10_Launchtask(TASK_HM10_VERSION,4,10);      // read SW Version
                          HM10_Launchtask(TASK_HM10_DISC,5,50);         // discovery
                          }

void HM10_Discovery_Scan(void) {
                          HM10_Launchtask(TASK_HM10_DISCONN,0,1);       // disconnect
                          HM10_Launchtask(TASK_HM10_DISC,1,1);          // discovery
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
                          HM10_Launchtask(TASK_HM10_TIME_L2,3,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_DISCONN,4,5);        // disconnect
                          }

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor, 0xff for Auto-type
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_serial)[6], uint8_t _type){
  if(_type==0xff){
    DEBUG_SENSOR_LOG(PSTR("MIBLE: will test MAC-type"));
    for (uint32_t i=0;i<4;i++){
      if(memcmp(_serial,kHM10SlaveID+i,3)==0){
        DEBUG_SENSOR_LOG(PSTR("MIBLE: MAC is type %u"), i);
        _type = i+1;
      }
      else {
        DEBUG_SENSOR_LOG(PSTR("MIBLE: MAC-type is unknown"));
      }
    }
  }
  if(_type==0xff) return _type; // error

  DEBUG_SENSOR_LOG(PSTR("MIBLE: vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors.at(i).serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("MIBLE: known sensor at slot: %u"), i);
      if(MIBLEsensors.at(i).showedUp < 3){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors.at(i).showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MIBLE i: %x %x %x %x %x %x"), MIBLEsensors.at(i).serial[0], MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[5]);
    DEBUG_SENSOR_LOG(PSTR("MIBLE n: %x %x %x %x %x %x"), _serial[0], _serial[1], _serial[2],_serial[3],_serial[4],_serial[5]);
  }
  DEBUG_SENSOR_LOG(PSTR("MIBLE: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 1; // does not matter for HM-10
  _newSensor.temp =-1000.0f;
  switch (_type)
    {
    case 1:
      _newSensor.moisture =-1000.0f;
      _newSensor.fertility =-1000.0f;
      _newSensor.lux = 0xffff;
      break;
    case 2: case 3: case 4:
      _newSensor.hum=-1.0f;
      _newSensor.bat=0xff;
      break;
    default:
      break;
    }
  MIBLEsensors.push_back(_newSensor);
  DEBUG_SENSOR_LOG(PSTR("MIBLE: new sensor at slot: %u"), MIBLEsensors.size()-1);
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
      DEBUG_SENSOR_LOG(PSTR("HM10: claim HW"));
    }
    HM10_Reset();
    HM10.mode.pending_task = 1;
    HM10.mode.init = 1;
    HM10.period = Settings.tele_period;
    DEBUG_SENSOR_LOG(PSTR("%s_TASK_LIST initialized, now return to main loop"),D_CMND_HM10);
  }
  return;
}

/**
 * @brief convert Mac-String to byte array
 * 
 * @param string Hex-string, must contain 12 chars (no error checking)
 * @param _mac Must be a uint8_t[6], filled with zeros
 */

void HM10MACStringToBytes(const char* string, uint8_t _mac[]) {
    uint32_t index = 0;
    while (index < 12) {
        char c = string[index];
        uint32_t value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        _mac[(index/2)] += value << (((index + 1) % 2) * 4);
        // DEBUG_SENSOR_LOG(PSTR("HM10:  Char: %c, Value: %x, Index/2: %u, valueadded: %x, MAC-index: %x"), c, value,(index/2),value << (((index + 1) % 2) * 4), _mac[index/2]);
        index++;
    }
    DEBUG_SENSOR_LOG(PSTR("HM10:  MAC-array: %x%x%x%x%x%x"),_mac[0],_mac[1],_mac[2],_mac[3],_mac[4],_mac[5]);
}


/*********************************************************************************************\
 * parse the response
\*********************************************************************************************/

void HM10ParseResponse(char *buf) {
    if (!strncmp(buf,"OK",2)) {
       DEBUG_SENSOR_LOG(PSTR("HM10: got OK"));
     }
    if (!strncmp(buf,"HMSoft",6)) { //8
      const char* _fw = "000";
      memcpy((void *)_fw,(void *)(buf+8),3);
      HM10.firmware = atoi(_fw);
      DEBUG_SENSOR_LOG(PSTR("HM10: Firmware: %d"), HM10.firmware);
      return;
     }
    char * _pos = strstr(buf, "IS0:");
    if(_pos) {
      const char* _mac = "000000000000";
      memcpy((void *)_mac,(void *)(_pos+4),12);
      DEBUG_SENSOR_LOG(PSTR("HM10: found Mac: %s"), _mac);
      uint8_t _newMacArray[6] = {0};
      HM10MACStringToBytes(_mac, _newMacArray);
      DEBUG_SENSOR_LOG(PSTR("HM10:  MAC-array: %x%x%x%x%x%x"),_newMacArray[0],_newMacArray[1],_newMacArray[2],_newMacArray[3],_newMacArray[4],_newMacArray[5]);
      MIBLEgetSensorSlot(_newMacArray, 0xff);
    }
    if (strstr(buf, "LOST")){
      HM10.mode.connected = false;
    }
    else {
      DEBUG_SENSOR_LOG(PSTR("HM10: empty response"));
    }
}

void HM10readTempHum(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("HM10: raw data: %x%x%x%x%x%x%x"),_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0] != 0 && _buf[1] != 0){
    memcpy(&LYWSD0x_HT,(void *)_buf,3);
    DEBUG_SENSOR_LOG(PSTR("HM10: Temperature * 100: %u, Humidity: %u"),LYWSD0x_HT.temp,LYWSD0x_HT.hum);
    uint32_t _slot = HM10.state.sensor;

    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    static float _tempFloat;
    _tempFloat=(float)(LYWSD0x_HT.temp)/100.0f;
    if(_tempFloat<60){
        MIBLEsensors.at(_slot).temp=_tempFloat;
        HM10.mode.awaitingHT = false;
        HM10.current_task_delay = 0;
    }
    _tempFloat=(float)LYWSD0x_HT.hum;
    if(_tempFloat<100){
      MIBLEsensors.at(_slot).hum = _tempFloat;
      DEBUG_SENSOR_LOG(PSTR("LYWSD03: hum updated"));
    }
  }
}

bool HM10readBat(char *_buf){
  DEBUG_SENSOR_LOG(PSTR("HM10: raw data: %x%x%x%x%x%x%x"),_buf[0],_buf[1],_buf[2],_buf[3],_buf[4],_buf[5],_buf[6]);
  if(_buf[0] != 0){
    DEBUG_SENSOR_LOG(PSTR("HM10: Battery: %u"),_buf[0]);
    uint32_t _slot = HM10.state.sensor;
    DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
    if(_buf[0]<101){
        MIBLEsensors.at(_slot).bat=_buf[0];
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
  char ret[HM10_MAX_RX_BUF] = {0};                // reset array with zeros
  
 
  while(HM10Serial->available()) {
    // delay(0);
    if(i<HM10_MAX_RX_BUF){
      ret[i] = HM10Serial->read();
    }
    i++;
    success = true;
  }
  if(HM10.mode.awaitingHT) {
    if (HM10.mode.connected) HM10readTempHum(ret);
  }
  else if(HM10.mode.awaitingB) {
    if (HM10.mode.connected) {
      if (HM10readBat(ret)){
        HM10.mode.awaitingB = false;
        HM10.current_task_delay = 0;
      }
    }
  }
  else if(success) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s response: %s"),D_CMND_HM10, (char *)ret);
    HM10ParseResponse(ret);
  }
  else {
    // DEBUG_SENSOR_LOG(PSTR("%s got no response"),D_CMND_HM10);
  }
  return success;
}

/*********************************************************************************************\
 * execute the next Task
\*********************************************************************************************/

void HM10_TaskEvery100ms(){
  if (HM10.current_task_delay == 0)  {
    uint8_t i = 0;
    bool runningTaskLoop = true;
    while (runningTaskLoop) {                                          // always iterate through the whole task list
      switch(HM10_TASK_LIST[i][0]) {                                 // handle the kind of task
        case TASK_HM10_ROLE1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set role to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+ROLE1");
          break;
        case TASK_HM10_IMME1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set imme to 1"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+IMME1");
          break;
        case TASK_HM10_DISC:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s start discovery"),D_CMND_HM10);
          HM10.current_task_delay = 35;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+DISC?");
          break;
        case TASK_HM10_VERSION:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read version"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+VERR?");
          break;
        case TASK_HM10_NAME:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read name"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NAME?");
          break;
        case TASK_HM10_CONN:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s connect"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          char _con[20];
          sprintf_P(_con,"AT+CON%02x%02x%02x%02x%02x%02x",MIBLEsensors.at(HM10.state.sensor).serial[0],MIBLEsensors.at(HM10.state.sensor).serial[1],MIBLEsensors.at(HM10.state.sensor).serial[2],MIBLEsensors.at(HM10.state.sensor).serial[3],MIBLEsensors.at(HM10.state.sensor).serial[4],MIBLEsensors.at(HM10.state.sensor).serial[5]);
          HM10Serial->write(_con);
          HM10.mode.awaitingB = false;
          HM10.mode.connected = true;
          break;
        case TASK_HM10_DISCONN:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s disconnect"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT");
          break;
        case TASK_HM10_RESET:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s Reset Device"),D_CMND_HM10);
          HM10Serial->write("AT+RESET");
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          break;
        case TASK_HM10_SUB_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 25;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_DELAY_SUB,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON0037");
          break;
        case TASK_HM10_UN_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaitingHT = false;
          HM10Serial->write("AT+NOTIFYOFF0037");
          break;
        case TASK_HM10_SUB_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 25;                   // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_DELAY_SUB,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON003C");
          break;
        case TASK_HM10_UN_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s un-subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.mode.awaitingHT = false;
          HM10Serial->write("AT+NOTIFYOFF003C");
          break;
        case TASK_HM10_TIME_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set time"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10.time = Rtc.utc_time;
          HM10Serial->write("AT+SEND_DATAWR002F");
          HM10Serial->write(HM10.timebuf,4);
          HM10Serial->write(Rtc.time_timezone / 60);
          AddLog_P2(LOG_LEVEL_DEBUG,PSTR("%s Time-string: %x%x%x%x%x"),D_CMND_HM10, HM10.timebuf[0],HM10.timebuf[1],HM10.timebuf[2],HM10.timebuf[3],(Rtc.time_timezone /60));
          break;
        case TASK_HM10_READ_HT:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read handle 0036"),D_CMND_HM10);
          HM10.current_task_delay = 0;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0036?");
          HM10.mode.awaitingHT = true;
          break;
        case TASK_HM10_READ_BT_L3:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read handle 003A"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA003A?");
          HM10.mode.awaitingB = true;
          break;
        case TASK_HM10_READ_BT_L2:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read handle 0043"),D_CMND_HM10);
          HM10.current_task_delay = 2;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0043?");
          HM10.mode.awaitingB = true;
          break;
        // case TASK_HM10_FINDALLCHARS:
        //   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s find all chars"),D_CMND_HM10);
        //   HM10.current_task_delay = 5;                    // set task delay
        //   HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
        //   runningTaskLoop = false;
        //   HM10Serial->write("AT+FINDALLCHARS?");
        //   break;
        case TASK_HM10_FEEDBACK:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s get response"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;
        case TASK_HM10_DELAY_SUB:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s start reading"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          HM10.mode.awaitingHT = true;
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

/**
 * @brief Main loop of the driver, "high level"-loop
 * 
 */

void HM10EverySecond(){
  if(HM10.firmware == 0) return;
  if(HM10.mode.pending_task == 1) return;
  if (MIBLEsensors.size()==0) return;

  static uint32_t _counter = 0;
  static uint32_t _nextSensorSlot = 0;
  if(_counter==0) {
    HM10.state.sensor = _nextSensorSlot;
    _nextSensorSlot++;
    if(MIBLEsensors.at(HM10.state.sensor).type==LYWSD03MMC) { // only this sensor for now
      HM10.mode.pending_task = 1;
      HM10_Read_LYWSD03();
    }
    if(MIBLEsensors.at(HM10.state.sensor).type==LYWSD02) {
      HM10.mode.pending_task = 1;
      HM10_Read_LYWSD02();
    }
    if (HM10.state.sensor==MIBLEsensors.size()-1) {
      _nextSensorSlot= 0;
      _counter++;
    }
    DEBUG_SENSOR_LOG(PSTR("%s active sensor now: %u"),D_CMND_HM10, HM10.state.sensor);
  }
  else _counter++;
  if (_counter>HM10.period) _counter = 0;
}

bool HM10Cmd(void) {
  char command[CMDSZ];
  bool serviced = true;
  uint8_t disp_len = strlen(D_CMND_HM10);

  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_CMND_HM10), disp_len)) {  // prefix
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + disp_len, kHM10_Commands);
    switch (command_code) {
      case CMND_HM10_PERIOD:
        if (XdrvMailbox.data_len > 0) {
          if (command_code == CMND_HM10_PERIOD)    { HM10.period = XdrvMailbox.payload; }
        }
        else {
          if (command_code == CMND_HM10_PERIOD) XdrvMailbox.payload = HM10.period;
        }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_BAUD:
        if (XdrvMailbox.data_len > 0) {
          if (command_code == CMND_HM10_BAUD) {
            HM10.serialSpeed = XdrvMailbox.payload;
            HM10Serial->begin(HM10.serialSpeed);
            }
        }
        else {
          if (command_code == CMND_HM10_BAUD) XdrvMailbox.payload = HM10.serialSpeed;
        }
        Response_P(S_JSON_HM10_COMMAND_NVALUE, command, XdrvMailbox.payload);
        break;
      case CMND_HM10_TIME:
        if (XdrvMailbox.data_len > 0) {
          if(MIBLEsensors.size()>XdrvMailbox.payload){
            if(MIBLEsensors.at(XdrvMailbox.payload).type == LYWSD02){
              HM10.state.sensor = XdrvMailbox.payload;
              HM10_Time_LYWSD02();
              }
            }
          }
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
        if (command_code == CMND_HM10_DISC_SCAN) { HM10_Discovery_Scan(); }
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

const char HTTP_HM10[] PROGMEM = "{s}HM10" " Firmware " "{m}%u{e}";
const char HTTP_HM10_SERIAL[] PROGMEM = "{s}%s %s{m}%02x:%02x:%02x:%02x:%02x:%02x%{e}";
const char HTTP_BATTERY[] PROGMEM = "{s}%s" " Battery" "{m}%u%%{e}";
const char HTTP_HM10_FLORA_DATA[] PROGMEM = "{s}%s" " Fertility" "{m}%sus/cm{e}";
const char HTTP_HM10_HL[] PROGMEM = "{s}<hr>{m}<hr>{e}";

void HM10Show(bool json)
{
  if (json) {
    for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
      char slave[33];
      sprintf_P(slave,"%s-%02x%02x%02x",kHM10SlaveType[MIBLEsensors.at(i).type-1],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[5]);
      char temperature[33]; // all sensors have temperature
      dtostrfd(MIBLEsensors.at(i).temp, Settings.flag2.temperature_resolution, temperature);

      ResponseAppend_P(PSTR(",\"%s\":{"),slave);
        if(MIBLEsensors.at(i).temp!=-1000.0f){ // this is the error code -> no temperature
          ResponseAppend_P(PSTR("\"" D_JSON_TEMPERATURE "\":%s"), temperature);
        }
        if (MIBLEsensors.at(i).type==FLORA){
          char lux[33];
          char moisture[33];
          char fertility[33];
          dtostrfd((float)MIBLEsensors.at(i).lux, 0, lux);
          dtostrfd(MIBLEsensors.at(i).moisture, 0, moisture);
          dtostrfd(MIBLEsensors.at(i).fertility, 0, fertility);
          if(MIBLEsensors.at(i).lux!=0xffff){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"" D_JSON_ILLUMINANCE "\":%s"), lux);
          }
          if(MIBLEsensors.at(i).moisture!=-1000.0f){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"" D_JSON_MOISTURE "\":%s"), moisture);
          }
          if(MIBLEsensors.at(i).fertility!=-1000.0f){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"Fertility\":%s"), fertility);
          }
        }
        if (MIBLEsensors.at(i).type>FLORA){ 
          char humidity[33];
          dtostrfd(MIBLEsensors.at(i).hum, Settings.flag2.humidity_resolution, humidity);
          if(MIBLEsensors.at(i).hum!=-1.0f){ // this is the error code -> no temperature
              ResponseAppend_P(PSTR(",\"" D_JSON_HUMIDITY "\":%s"), humidity);
          }
          if(MIBLEsensors.at(i).bat!=0xff){ // this is the error code -> no temperature
            ResponseAppend_P(PSTR(",\"Battery\":%u"), MIBLEsensors.at(i).bat);
          }
        }
        ResponseAppend_P(PSTR("}"));
    }
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_HM10, HM10.firmware);
      for (uint32_t i = 0; i < MIBLEsensors.size(); i++) {
        WSContentSend_PD(HTTP_HM10_HL);
        WSContentSend_PD(HTTP_HM10_SERIAL, kHM10SlaveType[MIBLEsensors.at(i).type-1], D_MAC_ADDRESS, MIBLEsensors.at(i).serial[0], MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[5]); 
        if(MIBLEsensors.at(i).temp!=-1000.0f){
          char temperature[33];
          dtostrfd(MIBLEsensors.at(i).temp, Settings.flag2.temperature_resolution, temperature);
          WSContentSend_PD(HTTP_SNS_TEMP, kHM10SlaveType[MIBLEsensors.at(i).type-1], temperature, TempUnit());
        }
        if (MIBLEsensors.at(i).type==FLORA){
          if(MIBLEsensors.at(i).lux!=0xffff){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_ILLUMINANCE, kHM10SlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).lux);
          }
          if(MIBLEsensors.at(i).moisture!=-1000.0f){ // this is the error code -> no valid value
            WSContentSend_PD(HTTP_SNS_MOISTURE, kHM10SlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).moisture);
          }
          if(MIBLEsensors.at(i).fertility!=-1000.0f){ // this is the error code -> no valid value
            char fertility[33];
            dtostrfd(MIBLEsensors.at(i).fertility, 0, fertility);
            WSContentSend_PD(HTTP_HM10_FLORA_DATA, kHM10SlaveType[MIBLEsensors.at(i).type-1], fertility);
          }
        }
        if (MIBLEsensors.at(i).type>FLORA){ // everything "above" Flora
          if(MIBLEsensors.at(i).hum!=-1.0f){ // this is the error code -> no humidity
            char humidity[33];
            dtostrfd(MIBLEsensors.at(i).hum, Settings.flag2.humidity_resolution, humidity);
            WSContentSend_PD(HTTP_SNS_HUM, kHM10SlaveType[MIBLEsensors.at(i).type-1], humidity);
          }
          if(MIBLEsensors.at(i).bat!=0xff){
            WSContentSend_PD(HTTP_BATTERY, kHM10SlaveType[MIBLEsensors.at(i).type-1], MIBLEsensors.at(i).bat);
          }
        }
      }
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
        HM10EverySecond();
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