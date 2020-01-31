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

TasmotaSerial *HM10Serial;

#define  HM10_MAX_TASK_NUMBER      8
uint8_t  HM10_TASK_LIST[HM10_MAX_TASK_NUMBER+1][2];   // first value: kind of task - second value: delay in x * 100ms
uint8_t  HM10_CURRENT_TASK_DELAY = 0;                // number of 100ms-cycles
uint8_t  HM10_LAST_COMMAND;                          // task command code 

uint16_t HM10_CURRENT_PAYLOAD    = 0;                // payload of a supported command

#define  HM10_MAX_RX_BUF         512
char     HM10_RX_STRING[HM10_MAX_RX_BUF]      = {0};  // make a buffer bigger than the usual 10-byte-message

struct {
  uint8_t current_task_delay;
  struct {
    uint32_t init:1;
    // TODO: more to come
  } mode;
} HM10;


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


#define TASK_HM10_DONE            99                        // used, if there was a task in the slot or just to wait

/*********************************************************************************************\
 * Helper functions
\*********************************************************************************************/

void HM10_Launchtask(uint8_t task, uint8_t slot, uint8_t delay){
                          HM10_TASK_LIST[slot][0]   = task;
                          HM10_TASK_LIST[slot][1]   = delay;
                          HM10_TASK_LIST[slot+1][0] = TASK_HM10_NOTASK;           // the tasks must always be launched in ascending order!!
                          HM10_CURRENT_TASK_DELAY   = HM10_TASK_LIST[0][1];
}

void HM10_TaskReplaceInSlot(uint8_t task, uint8_t slot){
                          HM10_LAST_COMMAND         = HM10_TASK_LIST[slot][0];    // save command
                          HM10_TASK_LIST[slot][0]   = task;  
}

void HM10_Reset(void) {   HM10_Launchtask(TASK_HM10_ROLE1,0,10);         // set role to 1
                          HM10_Launchtask(TASK_HM10_IMME1,1,10);         // set imme to 1
                          HM10_Launchtask(TASK_HM10_RESET,2,10);         // reset Device
                          HM10_Launchtask(TASK_HM10_VERSION,3,10);       // read SW Version
                          HM10_Launchtask(TASK_HM10_NAME,4,10);          // read name
                          }                    




/*********************************************************************************************\
 * init serial
 * define serial rx/tx port fixed with 115200 baud
\*********************************************************************************************/

void HM10SerialInit(void) {
  HM10.mode.init = false;
  HM10Serial = new TasmotaSerial(HM_PIN_RX, HM_PIN_TX, 1, 0, HM10_MAX_RX_BUF); 
  if (HM10Serial->begin(115200)) {
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sstart serial communication fixed to 115200 baud"),D_CMND_HM10);
    if (HM10Serial->hardwareSerial()) {
      ClaimSerial();
      DEBUG_SENSOR_LOG(PSTR("HM10: claim HW"));
    }
    HM10_Reset();
    HM10.mode.init = true;
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK_LIST initialized, now return to main loop"),D_CMND_HM10);
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
  if(success) {
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
  if (HM10_CURRENT_TASK_DELAY == 0)  {
    uint8_t i = 0;
    bool runningTaskLoop = true;
    while (runningTaskLoop) {                                          // always iterate through the whole task list
      switch(HM10_TASK_LIST[i][0]) {                                 // handle the kind of task
        case TASK_HM10_ROLE1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set role to 1"),D_CMND_HM10);
          HM10_CURRENT_TASK_DELAY = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+ROLE1");
          break;
        case TASK_HM10_IMME1:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set imme to 1"),D_CMND_HM10);
          HM10_CURRENT_TASK_DELAY = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+IMME1");
          break;
        case TASK_HM10_DISC:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s set role to 1"),D_CMND_HM10);
          HM10_CURRENT_TASK_DELAY = 35;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+DISC?");
          break;
        case TASK_HM10_VERSION:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read version"),D_CMND_HM10);
          HM10_CURRENT_TASK_DELAY = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+VERR?");
          break;
        case TASK_HM10_NAME:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s read name"),D_CMND_HM10);
          HM10_CURRENT_TASK_DELAY = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          HM10Serial->write("AT+NAME?");
          break;
        
        case TASK_HM10_RESET:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s Reset Device"),D_CMND_HM10);
          HM10Serial->write("AT+RESET");
          HM10_CURRENT_TASK_DELAY = 5;                    // set task delay
          HM10_TaskReplaceInSlot(TASK_HM10_FEEDBACK,i);
          runningTaskLoop = false;
          break;
        
        case TASK_HM10_FEEDBACK:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s get response"),D_CMND_HM10);
          HM10SerialHandleFeedback();
          HM10_CURRENT_TASK_DELAY = HM10_TASK_LIST[i+1][1];;     // set task delay
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
    HM10_CURRENT_TASK_DELAY--;               // count down every 100 ms
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
      }
  }
  return result;
}