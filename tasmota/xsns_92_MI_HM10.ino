/*
  xsns_92_Ml_BLE.ino - MI-BLE-sensors via HM-10 support for Tasmota

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

  ---
  0.9.0.0 20200130  started - further development by Christian Baars
                    base    - no real base project
                    forked  - from arendst/tasmota            - https://github.com/arendst/Tasmota

*/

#define XSNS_92                    92

#define HM_PIN_RX  5   // D1 Hardcoded while developing
#define HM_PIN_TX  4   // D2

#include <TasmotaSerial.h>
#include <vector>

TasmotaSerial *HM10Serial;

#define  HM10_MAX_TASK_NUMBER      12
uint8_t  HM10_TASK_LIST[HM10_MAX_TASK_NUMBER+1][2];   // first value: kind of task - second value: delay in x * 100ms



#define  HM10_MAX_RX_BUF         512
char     HM10_RX_STRING[HM10_MAX_RX_BUF]      = {0};

struct {
  uint8_t current_task_delay;                // number of 100ms-cycles
  uint8_t last_command;
  uint16_t firmware;
  struct {
    uint32_t init:1;
    uint32_t subscribed:1;
    // TODO: more to come
  } mode;
} HM10;

#pragma pack(1)
struct {
  uint16_t temp;
  uint8_t hum;
} LYWSD03;
#pragma pack(0)

struct mi_sensor_t{
  uint8_t type; //flora = 1; MI-HT_V1=2; LYWSD02=3; LYWSD03=4
  uint8_t serial[6];
  uint8_t showedUp;
  union {
    struct {
      float temp;
      float moisture;
      float fertility;
      uint16_t lux;
    } Flora;
    struct {
      float temp;
      float hum;
      uint8_t bat;
    } MJ_HT_V1;
    struct {
      float temp;
      float hum;
      uint8_t bat;
    } LYWSD0x; // LYWSD02 and LYWSD03
  };
};

std::vector<mi_sensor_t> MIBLEsensors;

/*********************************************************************************************\
 * constants
\*********************************************************************************************/

#define D_CMND_HM10 "HM10"

const char S_JSON_HM10_COMMAND_NVALUE[] PROGMEM = "{\"" D_CMND_HM10 "%s\":%d}";
const char S_JSON_HM10_COMMAND[] PROGMEM        = "{\"" D_CMND_HM10 "%s\"}";
const char kHM10_Commands[] PROGMEM             = "Track|Play";

/*********************************************************************************************\
 * enumerations
\*********************************************************************************************/

enum HM10_Commands {
  CMND_HM10_TRACK
};
                        
/*********************************************************************************************\
 * command defines
\*********************************************************************************************/




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
#define TASK_HM10_SUBSCR          11                        // subscribe to service handle
#define TASK_HM10_READ            12                        // read from handle
#define TASK_HM10_FINDALLCHARS    13                        // read all available characteristics

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

void HM10_Reset(void) {   HM10_Launchtask(TASK_HM10_ROLE1,0,1);         // set role to 1
                          HM10_Launchtask(TASK_HM10_IMME1,1,1);         // set imme to 1
                          HM10_Launchtask(TASK_HM10_RESET,2,1);         // reset Device
                          HM10_Launchtask(TASK_HM10_VERSION,3,10);       // read SW Version
                          HM10_Launchtask(TASK_HM10_DISC,4,1);           // disscovery
                          HM10_Launchtask(TASK_HM10_CONN,5,5);          // connect
                          HM10_Launchtask(TASK_HM10_FEEDBACK,6,35);      // get OK+CONN
                          HM10_Launchtask(TASK_HM10_SUBSCR,7,20);        // subscribe
                          HM10_Launchtask(TASK_HM10_READ,8,35);          // read
                          HM10_Launchtask(TASK_HM10_READ,9,35);          // read
                          HM10_Launchtask(TASK_HM10_READ,10,35);          // read
                          HM10_Launchtask(TASK_HM10_DISCONN,11,250);          // disconnect
                          }                    

/**
 * @brief Return the slot number of a known sensor or return create new sensor slot
 *
 * @param _serial     BLE address of the sensor
 * @param _type       Type number of the sensor
 * @return uint32_t   Known or new slot in the sensors-vector
 */
uint32_t MIBLEgetSensorSlot(uint8_t (&_serial)[6], uint8_t _type){
  DEBUG_SENSOR_LOG(PSTR("MIBLE: vector size %u"), MIBLEsensors.size());
  for(uint32_t i=0; i<MIBLEsensors.size(); i++){
    if(memcmp(_serial,MIBLEsensors.at(i).serial,sizeof(_serial))==0){
      DEBUG_SENSOR_LOG(PSTR("MIBLE: known sensor at slot: %u"), i);
      if(MIBLEsensors.at(i).showedUp < 3){ // if we got an intact packet, the sensor should show up several times
        MIBLEsensors.at(i).showedUp++;     // count up to the above number ... now we are pretty sure
      }
      return i;
    }
    DEBUG_SENSOR_LOG(PSTR("MIBLE i: %x %x %x %x %x %x"), MIBLEsensors.at(i).serial[5], MIBLEsensors.at(i).serial[4],MIBLEsensors.at(i).serial[3],MIBLEsensors.at(i).serial[2],MIBLEsensors.at(i).serial[1],MIBLEsensors.at(i).serial[0]);
    DEBUG_SENSOR_LOG(PSTR("MIBLE n: %x %x %x %x %x %x"), _serial[5], _serial[4], _serial[3],_serial[2],_serial[1],_serial[0]);
  }
  DEBUG_SENSOR_LOG(PSTR("MIBLE: found new sensor"));
  mi_sensor_t _newSensor;
  memcpy(_newSensor.serial,_serial, sizeof(_serial));
  _newSensor.type = _type;
  _newSensor.showedUp = 1;
  switch (_type)
    {
    case 1:
      _newSensor.Flora.temp =-1000.0f;
      _newSensor.Flora.moisture =-1000.0f;
      _newSensor.Flora.fertility =-1000.0f;
      _newSensor.Flora.lux = 0xffff;
      break;
    case 2:
      _newSensor.MJ_HT_V1.temp=-1000.0f;
      _newSensor.MJ_HT_V1.hum=-1.0f;
      _newSensor.MJ_HT_V1.bat=0xff;
      break;
    case 3: case 4:
      _newSensor.LYWSD0x.temp=-1000.0f;
      _newSensor.LYWSD0x.hum=-1.0f;
      _newSensor.LYWSD0x.bat=0xff;
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
  HM10Serial = new TasmotaSerial(HM_PIN_RX, HM_PIN_TX, 1, 0, HM10_MAX_RX_BUF); 
  if (HM10Serial->begin(115200)) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s start serial communication fixed to 115200 baud"),D_CMND_HM10);
    if (HM10Serial->hardwareSerial()) {
      ClaimSerial();
      DEBUG_SENSOR_LOG(PSTR("HM10: claim HW"));
    }
    HM10_Reset();
    HM10.mode.init = true;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s_TASK_LIST initialized, now return to main loop"),D_CMND_HM10);
  }
  return;
}

/*********************************************************************************************\
 * create the HM10 commands payload, and send it via serial interface to the HM10 player
 \*********************************************************************************************/

// void HM10_CMD(uint8_t _cmd,uint16_t _val) {

//   HM10Serial->write(cmd, sizeof(cmd));               /
//   return;
// }

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
     }

    else {
      DEBUG_SENSOR_LOG(PSTR("HM10: empty response"));
    }
}

/*********************************************************************************************\
 * handle the return value from the HM10
\*********************************************************************************************/

bool HM10SerialHandleFeedback(){
  bool success    = false;                           // true disables possible repetition of commands, set to false only for debugging
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
  if(HM10.mode.subscribed) {
    DEBUG_SENSOR_LOG(PSTR("HM10: raw data: %x%x%x%x%x%x%x"),ret[0],ret[1],ret[2],ret[3],ret[4],ret[5],ret[6]);
    if(ret[0] != 0 && ret[1] != 0){
      memcpy(&LYWSD03,(void *)ret,3);
      DEBUG_SENSOR_LOG(PSTR("HM10: Temperature * 100: %u, Humidity: %u"),LYWSD03.temp,LYWSD03.hum);
      uint8_t _serial[6] = {0};
      uint32_t _slot = MIBLEgetSensorSlot(_serial, 4);
      DEBUG_SENSOR_LOG(PSTR("MIBLE: Sensor slot: %u"), _slot);
      static float _tempFloat;
      _tempFloat=(float)(LYWSD03.temp)/100.0f;
      if(_tempFloat<60){
          MIBLEsensors.at(_slot).LYWSD0x.temp=_tempFloat;
      }
      _tempFloat=(float)LYWSD03.hum;
      if(_tempFloat<100){
        MIBLEsensors.at(_slot).LYWSD0x.hum = _tempFloat;
        DEBUG_SENSOR_LOG(PSTR("MJ_HT_V1: hum updated"));
      }
    }
  }
  else if(success) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s response: %s"),D_CMND_HM10, (char *)ret);
    HM10ParseResponse(ret);
  }
  else {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s got no response"),D_CMND_HM10);
  }
  return success;
}

/*********************************************************************************************\
 * execute the next Task
\*********************************************************************************************/

void HM10_TaskEvery100ms(){
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK to be done %u"),D_CMND_HM10,HM10_TASK_LIST[0][0]);
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
          HM10Serial->write("AT+CONA4C138ED815A");
          break;
        case TASK_HM10_DISCONN:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s disconnect"),D_CMND_HM10);
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT");
          HM10.mode.subscribed = false;
          break;
        case TASK_HM10_RESET:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s Reset Device"),D_CMND_HM10);
          HM10Serial->write("AT+RESET");
          HM10.current_task_delay = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          break;
        case TASK_HM10_SUBSCR:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s subscribe"),D_CMND_HM10);
          HM10.current_task_delay = 15;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NOTIFY_ON0037");
          
        case TASK_HM10_READ:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read handle 0036"),D_CMND_HM10);
          HM10.current_task_delay = 0;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+READDATA0036?");
          HM10.mode.subscribed = true;
        case TASK_HM10_FINDALLCHARS:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s find all chars"),D_CMND_HM10);
          HM10.current_task_delay = 35;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+FINDALLCHARS?");
        case TASK_HM10_FEEDBACK:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s get response"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10.current_task_delay = HM10_TASK_LIST[i+1][1];;     // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;
          break;

        case TASK_HM10_DONE:                                    // this entry was already handled
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sFound done HM10_TASK"),D_CMND_HM10);
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%snext slot:%u, i: %u"),D_CMND_HM10, HM10_TASK_LIST[i+1][0],i);
          if(HM10_TASK_LIST[i+1][0] == TASK_HM10_NOTASK) {             // check the next entry and if there is none
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sno Tasks left"),D_CMND_HM10);
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK_DONE current slot %u"),D_CMND_HM10, i);
            for (uint8_t j = 0; j < HM10_MAX_TASK_NUMBER+1; j++) {   // do a clean-up:
              AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK cleanup slot %u"),D_CMND_HM10, j);
              HM10_TASK_LIST[j][0] = TASK_HM10_NOTASK;                // reset all task entries
              HM10_TASK_LIST[j][1] = 0;                              // reset all delays
            }
            runningTaskLoop = false;                                  // return to main loop
            // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sUpdate GUI via AJAX"),D_CMND_HM10);
            // HM10_GUI_NEEDS_UPDATE = true;
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


const char HTTP_HM10[] PROGMEM =
 "{s}HM10" " Firmware " "{m}%u{e}";

void HM10Show(bool json)
{
    if (json) {
      if (MIBLEsensors.size()==0) return;

      char temperature[33];
      dtostrfd(MIBLEsensors.at(0).LYWSD0x.temp, Settings.flag2.temperature_resolution, temperature);
      char humidity[33];
      dtostrfd(MIBLEsensors.at(0).LYWSD0x.hum, Settings.flag2.humidity_resolution, humidity);

      ResponseAppend_P(JSON_SNS_TEMPHUM, F("LYWSD03"), temperature, humidity);
#ifdef USE_WEBSERVER
    } else {
      WSContentSend_PD(HTTP_HM10, HM10.firmware);
      if (MIBLEsensors.size()==0) return;

      char temperature[33];
      dtostrfd(MIBLEsensors.at(0).LYWSD0x.temp, Settings.flag2.temperature_resolution, temperature);
      char humidity[33];
      dtostrfd(MIBLEsensors.at(0).LYWSD0x.hum, Settings.flag2.humidity_resolution, humidity);

      WSContentSend_PD(HTTP_SNS_TEMP, F("LYWSD03"), temperature, TempUnit());
      WSContentSend_PD(HTTP_SNS_HUM, F("LYWSD03"), humidity);
#endif  // USE_WEBSERVER
    }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns92(uint8_t function)
{
  bool result = false;

  // if ((pin[HM_PIN_RX] < 99) && (pin[HM_PIN_TX] < 99)) { 
  if (true) { 
    switch (function) {
      case FUNC_INIT:
        HM10SerialInit();                                    // init and start communication
        break;
      case FUNC_EVERY_100_MSECOND:
        if (HM10_TASK_LIST[0][0] == TASK_HM10_NOTASK) {       // no task running 
          // DEBUG_SENSOR_LOG(PSTR("HM10: no TASK in array"));
          HM10SerialHandleFeedback();                        // -> sniff for device feedback
          break;
        }
        else {
          // DEBUG_SENSOR_LOG(PSTR("HM10: every 100msec"));
          HM10_TaskEvery100ms();                             // something has to be done, we'll check in the next step
          break;
        }
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