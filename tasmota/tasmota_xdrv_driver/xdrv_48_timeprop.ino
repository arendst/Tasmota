/*
  xdrv_48_timeprop.ino - Timeprop support for Sonoff-Tasmota

  Copyright (C) 2021  Colin Law and Thomas Herrmann

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
*/

#ifdef USE_TIMEPROP
#ifndef FIRMWARE_MINIMAL
/*********************************************************************************************\
 * Code to drive one or more relays in a time proportioned manner give a
 * required power value.
 *
 * Given required power values in the range 0.0 to 1.0 the relays will be
 * driven on/off in such that the average power suppled will represent
 * the required power.
 * The cycle time is configurable.  If, for example, the
 * period is set to 10 minutes and the power input is 0.2 then the output will
 * be on for two minutes in every ten minutes.
 *
 * A value for actuator dead time may be provided. If you have a device that
 * takes a significant time to open/close then set this to the average of the
 * open and close times.  The algorithim will then adjust the output timing
 * accordingly to ensure that the output is not switched more rapidly than
 * the actuator can cope with.
 *
 * A facility to invert the output is provided which can be useful when used in
 * refrigeration processes and similar.
 *
 * In the case where only one relay is being driven the power value is set by
 * writing the value to the mqtt topic cmnd/timeprop_setpower_0.  If more than
 * one relay is being driven (as might be the case for a heat/cool application
 * where one relay drives the heater and the other the cooler) then the power
 * for the second relay is written to topic cmnd/timeprop_setpower_1 and so on.
 *
 * To cope with the problem of temporary wifi failure etc a
 * TIMEPROP_MAX_UPDATE_INTERVALS value is available. This can be set to the max
 * expected time between power updates and if this time is exceeded then the
 * power will fallback to a given safe value until a new value is provided. Set
 * the interval to 0 to disable this feature.
 *
 * Usage:
 * Place this file in the sonoff folder.
 * Clone the library https://github.com/colinl/process-control.git from Github
 * into a subfolder of lib.
 * In user_config.h or user_config_override.h for a single relay, include
 * code as follows:

 #define USE_TIMEPROP    //  include the timeprop feature (+1.2k)
   // for single output
   #define TIMEPROP_NUM_OUTPUTS          1       // how many outputs to control (with separate alogorithm for each)
   #define TIMEPROP_CYCLETIMES           60      // cycle time seconds
   #define TIMEPROP_DEADTIMES            0       // actuator action time seconds
   #define TIMEPROP_OPINVERTS            false   // whether to invert the output
   #define TIMEPROP_FALLBACK_POWERS      0.0       // falls back to this if too long betwen power updates
   #define TIMEPROP_MAX_UPDATE_INTERVALS 120     // max no secs that are allowed between power updates (0 to disable)
   #define TIMEPROP_RELAYS               1       // which relay to control 1:8

 * or for two relays:
 #define USE_TIMEPROP    //  include the timeprop feature (+1.2k)
   // for single output
   #define TIMEPROP_NUM_OUTPUTS          2               // how many outputs to control (with separate alogorithm for each)
   #define TIMEPROP_CYCLETIMES           60,     10      // cycle time seconds
   #define TIMEPROP_DEADTIMES            0,      0       // actuator action time seconds
   #define TIMEPROP_OPINVERTS            false,  false   // whether to invert the output
   #define TIMEPROP_FALLBACK_POWERS      0.0,      0.0       // falls back to this if too long betwen power updates
   #define TIMEPROP_MAX_UPDATE_INTERVALS 120,    120     // max no secs that are allowed between power updates (0 to disable)
   #define TIMEPROP_RELAYS               1,      2       // which relay to control 1:8

 * Publish values between 0 and 1 to the topic(s) described above
 *
**/



#include "Timeprop.h"


#ifndef TIMEPROP_NUM_OUTPUTS
#define TIMEPROP_NUM_OUTPUTS          1       // how many outputs to control (with separate alogorithm for each)
#endif
#ifndef TIMEPROP_CYCLETIMES
#define TIMEPROP_CYCLETIMES           60      // cycle time seconds
#endif
#ifndef TIMEPROP_DEADTIMES
#define TIMEPROP_DEADTIMES            0       // actuator action time seconds
#endif
#ifndef TIMEPROP_OPINVERTS
#define TIMEPROP_OPINVERTS            false   // whether to invert the output
#endif
#ifndef TIMEPROP_FALLBACK_POWERS
#define TIMEPROP_FALLBACK_POWERS      0       // falls back to this if too long between power updates
#endif
#ifndef TIMEPROP_MAX_UPDATE_INTERVALS
#define TIMEPROP_MAX_UPDATE_INTERVALS 120     // max no secs that are allowed between power updates (0 to disable)
#endif
#ifndef TIMEPROP_RELAYS
#define TIMEPROP_RELAYS               1       // which relay to control 1:8
#endif
#ifndef TIMEPROP_REPORT_SETTINGS
#define TIMEPROP_REPORT_SETTINGS      false    // set to true to include timeprop settings in json output
#endif

static Timeprop timeprops[TIMEPROP_NUM_OUTPUTS];
static int relayNos[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_RELAYS};
static long currentRelayStates = 0;  // current actual relay states. Bit 0 first relay

struct {
  Timeprop timeprops[TIMEPROP_NUM_OUTPUTS];
  int relay_nos[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_RELAYS};
  long current_relay_states = 0;  // current actual relay states. Bit 0 first relay
  long current_time_secs = 0;  // a counter that counts seconds since initialisation
} Tprop;

#define D_CMND_TIMEPROP_PREFIX "Timeprop"
#define D_CMND_TIMEPROP_SETPOWER "_SetPower_"   // underscores left in for backwards compatibility
#define D_CMND_TIMEPROP_SETCYCLETIME "SetCycleTime"
#define D_CMND_TIMEPROP_DEADTIME "SetDeadTime"
#define D_CMND_TIMEPROP_OPINVERT "SetOutputInvert"
#define D_CMND_TIMEPROP_FALLBACK_POWER "SetFallbackPower"
#define D_CMND_TIMEPROP_MAX_UPDATE_INTERVAL "SetMaxUpdateInterval"

int cycleTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_CYCLETIMES};
int deadTimes[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_DEADTIMES};
unsigned char opInverts[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_OPINVERTS};
float fallbacks[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_FALLBACK_POWERS};
int maxIntervals[TIMEPROP_NUM_OUTPUTS] = {TIMEPROP_MAX_UPDATE_INTERVALS};

enum TimepropCommands { CMND_TIMEPROP_SETPOWER };

const char kCommands[] PROGMEM = D_CMND_TIMEPROP_PREFIX "|"
  D_CMND_TIMEPROP_SETPOWER "|"
  D_CMND_TIMEPROP_SETCYCLETIME "|"
  D_CMND_TIMEPROP_DEADTIME "|"
  D_CMND_TIMEPROP_OPINVERT "|"
  D_CMND_TIMEPROP_FALLBACK_POWER "|"
  D_CMND_TIMEPROP_MAX_UPDATE_INTERVAL ;

void (* const Command[])(void) PROGMEM = {
  &CmndSetPower,
  &CmndSetCycleTime,
  &CmndSetDeadTime,
  &CmndSetOutputInvert,
  &CmndSetFallbackPower,
  &CmndSetMaxUpdateInterval,
};

/* call this from elsewhere if required to set the power value for one of the timeprop instances */
/* index specifies which one, 0 up */
void TimepropSetPower(int index, float power) {
  if (index >= 0  &&  index < TIMEPROP_NUM_OUTPUTS) {
    Tprop.timeprops[index].setPower( power, Tprop.current_time_secs);
  }
}

void TimepropInit(void) {
  for (int i = 0; i < TIMEPROP_NUM_OUTPUTS; i++) {
    Tprop.timeprops[i].initialise(cycleTimes[i], deadTimes[i], opInverts[i], fallbacks[i],
      maxIntervals[i], Tprop.current_time_secs);
  }
}

void CmndSetPower(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS) {
    if(XdrvMailbox.data_len) {
      float newPower=CharToFloat(XdrvMailbox.data);
      timeprops[XdrvMailbox.index].setPower(newPower, Tprop.current_time_secs );
      ResponseCmndFloat(newPower, 2);
    }
    else {
      ResponseCmndError();
    }
  }
}

// commands for settings all take the same form
// prefix commands with 'timeprop'
// then append the command string eg. 'SetCycleTime'
// then append the output number (0 for a single output)
// then append the value to set set, or
// leave blank to retrieve the current value
// eg.
// 'TimepropSetCycleTime0 120' will set the value of cycle time for output 0 to 120
// 'TimepropSetCycleTime0' will retrieve the value of cycle time for output 0

void CmndSetCycleTime(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS ) {
    if(XdrvMailbox.data_len) {
      int newCycleTime=TextToInt(XdrvMailbox.data);
      if(newCycleTime>0) {
        cycleTimes[XdrvMailbox.index] = newCycleTime;
        Tprop.timeprops[XdrvMailbox.index].initialise(cycleTimes[XdrvMailbox.index], deadTimes[XdrvMailbox.index], opInverts[XdrvMailbox.index], fallbacks[XdrvMailbox.index], maxIntervals[XdrvMailbox.index], Tprop.current_time_secs);
        ResponseCmndNumber(newCycleTime);
      }
      else {
        ResponseCmndError();
      }
    }
    else {
      ResponseCmndNumber(cycleTimes[XdrvMailbox.index]);
    }
  }
}

void CmndSetDeadTime(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS ) {
    if(XdrvMailbox.data_len) {
      int newDeadTime=TextToInt(XdrvMailbox.data);
      if(newDeadTime>0) {
        deadTimes[XdrvMailbox.index] = newDeadTime;
        Tprop.timeprops[XdrvMailbox.index].initialise(cycleTimes[XdrvMailbox.index], deadTimes[XdrvMailbox.index], opInverts[XdrvMailbox.index], fallbacks[XdrvMailbox.index], maxIntervals[XdrvMailbox.index], Tprop.current_time_secs);
        ResponseCmndNumber(newDeadTime);
      }
      else {
        ResponseCmndError();
      }
    } 
    else {
      ResponseCmndNumber(deadTimes[XdrvMailbox.index]);
    }
  }
}

void CmndSetOutputInvert(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS ) {
    if(XdrvMailbox.data_len) {
      unsigned char newInvert=TextToInt(XdrvMailbox.data);
      opInverts[XdrvMailbox.index] = newInvert;
      Tprop.timeprops[XdrvMailbox.index].initialise(cycleTimes[XdrvMailbox.index], deadTimes[XdrvMailbox.index], opInverts[XdrvMailbox.index], fallbacks[XdrvMailbox.index], maxIntervals[XdrvMailbox.index], Tprop.current_time_secs);
      ResponseCmndNumber(newInvert);
    }
    else {
      ResponseCmndNumber(opInverts[XdrvMailbox.index]);
    }
  }
}

void CmndSetFallbackPower(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS ) {
    if(XdrvMailbox.data_len) {
      float newPower=CharToFloat(XdrvMailbox.data);
      if(newPower>=0.0 && newPower<=1.0) {
        fallbacks[XdrvMailbox.index] = newPower;
        Tprop.timeprops[XdrvMailbox.index].initialise(cycleTimes[XdrvMailbox.index], deadTimes[XdrvMailbox.index], opInverts[XdrvMailbox.index], fallbacks[XdrvMailbox.index], maxIntervals[XdrvMailbox.index], Tprop.current_time_secs);
        ResponseCmndFloat(newPower, 2);
      }
      else {
        ResponseCmndError();
      }
    }
    else {
      ResponseCmndFloat(fallbacks[XdrvMailbox.index], 2);
    }
  }
}

void CmndSetMaxUpdateInterval(void) {
  if (XdrvMailbox.index >=0 && XdrvMailbox.index < TIMEPROP_NUM_OUTPUTS ) {
    if(XdrvMailbox.data_len) {
      int newInterval=TextToInt(XdrvMailbox.data);
      if(newInterval>0) {
        maxIntervals[XdrvMailbox.index] = newInterval;
        Tprop.timeprops[XdrvMailbox.index].initialise(cycleTimes[XdrvMailbox.index], deadTimes[XdrvMailbox.index], opInverts[XdrvMailbox.index], fallbacks[XdrvMailbox.index], maxIntervals[XdrvMailbox.index], Tprop.current_time_secs);
        ResponseCmndNumber(newInterval);
      }
      else {
        ResponseCmndError();
      }
    }
    else {
      ResponseCmndNumber(maxIntervals[XdrvMailbox.index]);
    }
  }
}

void TimepropEverySecond(void) {
  Tprop.current_time_secs++;    // increment time
  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    int newState = Tprop.timeprops[i].tick(Tprop.current_time_secs);
    if (newState != bitRead(Tprop.current_relay_states, Tprop.relay_nos[i]-1)){
      // remove the third parameter below if using tasmota prior to v6.0.0
      ExecuteCommandPower(Tprop.relay_nos[i], newState,SRC_IGNORE);
    }
  }
}

// called by the system each time a relay state is changed
void TimepropXdrvPower(void) {
  // for a single relay the state is in the lsb of index, I have think that for
  // multiple outputs then succesive bits will hold the state but have not been
  // able to test that
  Tprop.current_relay_states = XdrvMailbox.index;
}

void ShowValues(void) {
#if TIMEPROP_REPORT_SETTINGS
  ResponseAppend_P(PSTR(",\"Timeprop\":{"));
  for (int i=0; i<TIMEPROP_NUM_OUTPUTS; i++) {
    ResponseAppend_P(PSTR("\"Output%d\":{"),i);
    ResponseAppend_P(PSTR("\"CycleTime\":%d,"),cycleTimes[i]);
    ResponseAppend_P(PSTR("\"DeadTime\":%d,"),deadTimes[i]);
    ResponseAppend_P(PSTR("\"OutputInvert\":%d,"),opInverts[i]);
    ResponseAppend_P(PSTR("\"FallbackPower\":%2_f,"),&fallbacks[i]);
    ResponseAppend_P(PSTR("\"MaxUpdateInterval\":%d"),maxIntervals[i]);
    ResponseAppend_P(i<TIMEPROP_NUM_OUTPUTS-1 ? PSTR("},") : PSTR("}"));
  }
  ResponseAppend_P(PSTR("}"));
#endif // TIMEPROP_REPORT_SETTINGS
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_48       48

bool Xdrv48(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      TimepropInit();
      break;
    case FUNC_EVERY_SECOND:
      TimepropEverySecond();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kCommands, Command);
      break;
    case FUNC_SET_POWER:
      TimepropXdrvPower();
      break;
    case FUNC_JSON_APPEND:
      ShowValues();
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif // FIRMWARE_MINIMAL
#endif // USE_TIMEPROP
