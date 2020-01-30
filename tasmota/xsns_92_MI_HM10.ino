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

#include <TasmotaSerial.h>

TasmotaSerial *HM10Serial;

#define  HM10_MAX_TASK_NUMBER      12
uint8_t  HM10_TASK_LIST[HM10_MAX_TASK_NUMBER+1][2];   // first value: kind of task - second value: delay in x * 100ms
uint8_t  HM10_CURRENT_TASK_DELAY = 0;                // number of 100ms-cycles
uint8_t  HM10_LAST_COMMAND;                          // task command code 

uint16_t HM10_CURRENT_PAYLOAD    = 0;                // payload of a supported command

#define  HM10_MAX_RX_BUF         512
char     HM10_RX_STRING[HM10_MAX_RX_BUF]      = {0};  // make a buffer bigger than the usual 10-byte-message





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
#define TASK_HM10_FEEDBACK        1                         // check the feedback from the device
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

void HM10_Reset(void) {    HM10_LAST_COMMAND         = TASK_HM10_DONE;             // task command code 
                          HM10State.activeFolder    = 0;                         // see the line above
                          HM10_Launchtask(TASK_HM10_DONE,0,10);                   // just wait for some time , equals delay(1000) -> 10 * 100
                          HM10_Launchtask(TASK_HM10_RESET_DEVICE,   1,0);         // reset Device
                          HM10_Launchtask(TASK_HM10_Q_VERSION,      2,10);        // read SW Version at startup
                          }                    // ASAP


/*********************************************************************************************\
 * init serial
 * define serial rx/tx port fixed with 115200 baud
\*********************************************************************************************/

void HM10SerialInit(void) {
  HM10Serial = new TasmotaSerial(pin[GPIO_RXD], pin[GPIO_TXD], 1, 0, HM10_MAX_RX_BUF); 
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sstart serial communication fixed to 115200 baud"),S_CONTROL_HM10);
  if (HM10Serial->begin(115200)) {
    if (UBXSerial->hardwareSerial()) {
      ClaimSerial();
      DEBUG_SENSOR_LOG(PSTR("HM10: claim HW"));
    }
    HM10_Reset();
    AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK_LIST initialized, now return to main loop"),S_CONTROL_HM10);
  }
  return;
}

/*********************************************************************************************\
 * create the HM10 commands payload, and send it via serial interface to the HM10 player
 \*********************************************************************************************/

void HM10_CMD(uint8_t _cmd,uint16_t _val) {

  HM10Serial->write(cmd, sizeof(cmd));               /
  return;
}

/*********************************************************************************************\
 * handle the return value from the HM10
\*********************************************************************************************/

bool HM10SerialHandleFeedback(){
  bool success    = true;                           // true disables possible repetition of commands, set to false only for debugging
  uint8_t i       = 0;
  uint8_t ret[HM10_MAX_RX_BUF] = {0};                // reset array with zeros
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%swaiting for response"),S_CONTROL_HM10);
  bool receive_data_message = false;                // special response with the format d,a, ...,a,d

  while(HM10Serial->available()) {
    // delay(0);
    if(i<HM10_MAX_RX_BUF){
      ret[i] = HM10Serial->read();
    }
    i++;
  }  
  return success;
}

/*********************************************************************************************\
 * execute the next Task
\*********************************************************************************************/

void HM10_TaskEvery100ms(){
  // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK to be done %u"),S_CONTROL_HM10,HM10_TASK_LIST[0][0]);
  if (HM10_CURRENT_TASK_DELAY == 0)  {
    uint8_t i = 0;
    bool runningTaskLoop = true;
    while (runningTaskLoop) {                                          // always iterate through the whole task list
      switch(HM10_TASK_LIST[i][0]) {                                 // handle the kind of task
        case TASK_HM10_FEEDBACK:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s%sFeedback"),S_CONTROL_HM10,S_TASK_HM10);
          if(HM10_RETRIES>0) {
            if (HM10SerialHandleFeedback()) {
              HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // mark slot as handled if successful
              HM10_CURRENT_TASK_DELAY = HM10_TASK_LIST[i+1][1];      // assign the delay of the next slot to the current global delay
            }
            else {
              HM10_TASK_LIST[i][0] = HM10_LAST_COMMAND;           // reinsert unsuccessful task into the current slot
              HM10_CURRENT_TASK_DELAY++;
              HM10_RETRIES--;
            }
          }
          else {
            HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // mark slot as handled even if not successful
            HM10_CURRENT_TASK_DELAY = HM10_TASK_LIST[i+1][1];      // assign the delay of the next slot to the current global delay
            HM10_RETRIES = 3;
          }
          runningTaskLoop = false;                               // return to main loop
          break;
        case TASK_HM10_PLAY:
          HM10_CMD(HM10_CMD_PLAY, 0);
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s%sPlay"),S_CONTROL_HM10,S_TASK_HM10);
          HM10State.PlayMode = 1;
          HM10_TaskReplaceInSlot(TASK_HM10_Q_TRACK,i);
          runningTaskLoop = false;
          break;
        
        case TASK_HM10_RESET_DEVICE:
          AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s%sReset Device"),S_CONTROL_HM10,S_TASK_HM10);
          HM10_CMD(HM10_CMD_RESET, HM10_CMD_RESET_VALUE);
          HM10_CURRENT_TASK_DELAY = HM10_TASK_LIST[i+1][1];      // set task delay
          HM10_TASK_LIST[i][0] = TASK_HM10_DONE;                 // no feedback for reset
          runningTaskLoop = false;                             // return to main loop
          break;

        case TASK_HM10_DONE:                                    // this entry was already handled
          // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sFound done HM10_TASK"),S_CONTROL_HM10);
          if(HM10_TASK_LIST[i+1][0] == TASK_HM10_NOTASK) {             // check the next entry and if there is none
            // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%s%sno Tasks left"),S_CONTROL_HM10,S_TASK_HM10);
            // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK_DONE current slot %u"),S_CONTROL_HM10, i);
            for (uint8_t j = 0; j < HM10_MAX_TASK_NUMBER+1; j++) {   // do a clean-up:
              // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sHM10_TASK cleanup slot %u"),S_CONTROL_HM10, j);
              HM10_TASK_LIST[j][0] = TASK_HM10_NOTASK;                // reset all task entries
              HM10_TASK_LIST[j][1] = 0;                              // reset all delays
            }
            runningTaskLoop = false;                                  // return to main loop
            // AddLog_P2(LOG_LEVEL_DEBUG, PSTR("%sUpdate GUI via AJAX"),S_CONTROL_HM10);
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

bool Xdrv92(uint8_t function)
{
  bool result = false;

  if ((pin[GPIO_RXD] < 99) && (pin[GPIO_TXD] < 99)) { 
    switch (function) {
      case FUNC_PRE_INIT:
        HM10SerialInit();                                    // init and start communication
        break;
      case FUNC_EVERY_100_MSECOND:
        if (HM10_TASK_LIST[0][0] == TASK_HM10_NOTASK) {       // no task running 
          HM10SerialHandleFeedback();                        // -> sniff for device feedback
          break;
        }
        else {
          HM10_TaskEvery100ms();                             // something has to be done, we'll check in the next step
          break;
        }
      }
  }
  return result;
}

