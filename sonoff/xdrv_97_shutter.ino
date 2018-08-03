/*
  xdrv_19_Shutter.ino - Shutter/Blind support for Sonoff-TasmotaS

  Copyright (C) 2018  Stefan Bode

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

#ifdef USE_SHUTTER   // +3.8k code

Ticker TickerShutter;

enum ShutterCommands {
  CMND_OPEN, CMND_CLOSE, CMND_STOP, CMND_POSITION, CMND_OPENTIME, CMND_CLOSETIME, CMND_SHUTTERRELAY, CMND_SET50PERCENT, CMND_SHUTTERSETCLOSE, CMND_SHUTTERINVERT };
const char kShutterCommands[] PROGMEM =
  D_CMND_OPEN "|" D_CMND_CLOSE "|" D_CMND_STOP "|" D_CMND_POSITION  "|" D_CMND_OPENTIME "|" D_CMND_CLOSETIME "|" D_CMND_SHUTTERRELAY "|" D_CMND_SET50PERCENT "|" D_CMND_SHUTTERSETCLOSE "|" D_CMND_SHUTTERINVERT;


const char JSON_SHUTTER_POS[] PROGMEM = "%s,\"%s-%d\":%d";                                  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

uint8_t Shutter_Open_Time[MAX_SHUTTERS] ;               // duration to open the shutter
uint8_t Shutter_Close_Time[MAX_SHUTTERS];              // duratin to close the shutter
int32_t Shutter_Open_Max[MAX_SHUTTERS];               // max value on maximum open calculated
int32_t Shutter_Target_Position[MAX_SHUTTERS] ;        // position to go to
int32_t Shutter_Start_Position[MAX_SHUTTERS] ;
uint16_t Shutter_Close_Velocity[MAX_SHUTTERS];          // in relation to open velocity. higher value = faster
int8_t  Shutter_Direction[MAX_SHUTTERS];               // 1 == UP , 0 == stop; -1 == down
int32_t Shutter_Real_Position[MAX_SHUTTERS];          // value between 0 and Shutter_Open_Max
power_t shutter_mask = 0;     // bit mask with 11 at the position of relays that belong to at least ONE shutter
power_t old_power = power;    // preserve old bitmask for power to extract the relay that changes.
power_t SwitchedRelay = 0;    // bitmatrix that contain the relays that was lastly changed.
uint16_t m1[MAX_SHUTTERS],m2[MAX_SHUTTERS],b1[MAX_SHUTTERS] ;   // slope and constant definition to trim shutter movement behavior.
uint32_t shutter_time[MAX_SHUTTERS] ;

void Rtc_ms50_Second()
{
  shutter_time[0]++;
  shutter_time[1]++;
  shutter_time[2]++;
  shutter_time[3]++;
}

void ShutterInit()
{
  shutters_present = 0;

  for (byte i=0;i < MAX_SHUTTERS; i++) {
    // set startrelay to 1 on first init, but only to shutter 1. 90% usecase
    Settings.shutter_startrelay[i] = (Settings.shutter_startrelay[i] == 0 && i ==  0? 1 : Settings.shutter_startrelay[i]);
    if (Settings.shutter_startrelay[i]) {
      shutters_present++;
      TickerShutter.attach_ms(50, Rtc_ms50_Second );
      // default the 50 percent should not have any impact without changing it. set to 60
      Settings.shutter_set50percent[i] = (Settings.shutter_set50percent[i] == 0 ? 50 : Settings.shutter_set50percent[i]);
      // use 10 sek as default to allow everybody to play without deep initialize
      Shutter_Open_Time[i] = (Settings.shutter_opentime[i] > 0 ? Settings.shutter_opentime[i] : 10);
      Shutter_Close_Time[i] = (Settings.shutter_closetime[i] > 0 ? Settings.shutter_closetime[i] : 10);
      // Update Calculation 20 because timeintervall is 0.05 sec
      Shutter_Open_Max[i] = 2000 * Shutter_Open_Time[i];
      Shutter_Close_Velocity[i] =  Shutter_Open_Max[i] /  Shutter_Close_Time[i] / 20 ;

      // calculate a ramp slope at the first 5 percent to compensate that shutters move with down part later than the upper part
      m1[i] = Shutter_Open_Max[i] * (100 - Settings.shutter_set50percent[i] ) / 5000;
      b1[i] = Shutter_Open_Max[i] - (m1[i] * 100);
      m2[i] = (b1[i] + 5 * m1[i]) / 5;
      shutter_mask |= 3 << (Settings.shutter_startrelay[i] -1)  ;

      Shutter_Real_Position[i] =   Settings.shutter_position[i] <= 5 ?  m2[i] * Settings.shutter_position[i] : m1[i] * Settings.shutter_position[i] + b1[i];
      Shutter_Start_Position[i] = Shutter_Real_Position[i];

      snprintf_P(log_data, sizeof(log_data), PSTR("Shutter %d (Relay:%d): Init. Pos: %d [%d %%], Open Vel.: 100 Close Vel.: %d , Max Way: %d, Opentime %d [s], Closetime %d [s], Calc: m1: %d, b1 %d, m2: %d, binmask %d, is inverted %d"), i, Settings.shutter_startrelay[i],Shutter_Real_Position[i],Settings.shutter_position[i],  Shutter_Close_Velocity[i] , Shutter_Open_Max[i], Shutter_Open_Time[i], Shutter_Close_Time[i],m1[i],b1[i],m2[i],shutter_mask,Settings.shutter_invert[i]);
      AddLog(LOG_LEVEL_INFO);
    } else {
      // teminate loop at first INVALUD shutter.
      break;
    }
  }
}

void Schutter_Update_Position()
{
  for (byte i=0; i < shutters_present; i++) {
    power_t powerstate = (3 << (Settings.shutter_startrelay[i] -1) ) & power;

    if (Shutter_Direction[i] != 0) {
      //char stemp1[20];
      Shutter_Real_Position[i] = Shutter_Start_Position[i] + ( shutter_time[i] * (Shutter_Direction[i] > 0 ? 100 : -Shutter_Close_Velocity[i]));
      // avoid real position leavin the boundaries.
      Shutter_Real_Position[i] = Shutter_Real_Position[i] < 0 ? 0 : (Shutter_Real_Position[i] > Shutter_Open_Max[i] ? Shutter_Open_Max[i] : Shutter_Real_Position[i]) ;
      // check if corresponding relay if OFF. Then stop movement.
      //snprintf_P(log_data, sizeof(log_data), PSTR("Debug:Shutter %d Powerstate: %d, Source: %s, power %ld"),i,powerstate, GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), power );
      //AddLog(LOG_LEVEL_DEBUG);
      // check IF OFF and not caused by pulsetimer                    check if ON and not by SRC_SHUTTER            now merge everything with the TWO relevant relays in this loop
      if ( ((!powerstate  && SRC_PULSETIMER != last_source ) || ( powerstate  && SRC_SHUTTER != last_source  ) ) && (SwitchedRelay & (3 << (Settings.shutter_startrelay[i] -1)))) {
        Shutter_Target_Position[i] = Shutter_Real_Position[i];
        snprintf_P(log_data, sizeof(log_data), PSTR("Shutter %d: Switch OFF motor. Target: %ld, source: %d, powerstate %ld"), i, Shutter_Target_Position[i], last_source, powerstate);
        AddLog(LOG_LEVEL_DEBUG);
      }
      //if (Shutter_Real_Position[i] * Shutter_Direction[i] >= Shutter_Target_Position[i] * Shutter_Direction[i] || Shutter_Real_Position[i] < Shutter_Close_Velocity[i] * -Shutter_Direction[i]) {
      if (Shutter_Real_Position[i] * Shutter_Direction[i] >= Shutter_Target_Position[i] * Shutter_Direction[i] ) {
        // calculate relay number responsibe for current movement.
        uint8_t cur_relay = Settings.shutter_startrelay[i] + (Shutter_Direction[i] == 1 ? 0 : 1) ;
        snprintf_P(log_data, sizeof(log_data), PSTR("Shutter %d: Stopp Pos. %d, relay: %d, pulsetimer: %d, rtcshutter: %ld"), i, Shutter_Real_Position[i], cur_relay -1, Settings.pulse_timer[cur_relay -1], shutter_time[i]);
        AddLog(LOG_LEVEL_DEBUG);
        Settings.shutter_position[i] = m2[i] * 5 > Shutter_Real_Position[i] ? Shutter_Real_Position[i] / m2[i] : (Shutter_Real_Position[i]-b1[i]) / m1[i];
        Shutter_Start_Position[i] = Shutter_Real_Position[i];

        if (Settings.pulse_timer[cur_relay-1] > 0) {
          // we have a momentary switch here. Needs additional pulse on same relay after the end
          if (SRC_PULSETIMER == last_source || SRC_SHUTTER == last_source) {
            ExecuteCommandPower(cur_relay, 1, SRC_SHUTTER);
          } else {
            last_source = SRC_SHUTTER;
          }
        } else {
          if (!Settings.flag3.paired_interlock) {
            ExecuteCommandPower(Settings.shutter_startrelay[i], 0, SRC_PULSETIMER);
            ExecuteCommandPower(Settings.shutter_startrelay[i]+1 , 0, SRC_PULSETIMER);
          } else {
            ExecuteCommandPower(cur_relay, 0, SRC_SHUTTER);
          }
        }
        Shutter_Direction[i] = 0;
      }
    } else {
      // no movement by software ; eventuall someone hits a relay and we must to start move.
      //    is YES one of the relays is ON but not we did it            combine only with the relays relevant to this shutter. do not care on ON relays on other shutters
      if ((shutter_mask & powerstate) > 0 && SRC_SHUTTER != last_source && (SwitchedRelay & (3 << (Settings.shutter_startrelay[i] -1)))) { // ONE of the two relays from the shutter report ON and direction == 0 ==> manual movement request
        snprintf_P(log_data, sizeof(log_data), PSTR("Shutter: Start..."));

        AddLog(LOG_LEVEL_DEBUG);
        last_source = SRC_SHUTTER; // avoid switch off in the next loop
        if (((1 << Settings.shutter_startrelay[i] ) & powerstate) > 0) { // tetsing on CLOSE relay, if ON
          // close with relay two
          Shutter_StartInit(i, -1, 0);
        } else {
          // opens with relay one
          Shutter_StartInit(i, 1, Shutter_Open_Max[i]);
        }
        snprintf_P(log_data, sizeof(log_data), PSTR("Shutter %d: is moving %ld, power: %ld, join: %ld, direction: %d"),i, shutter_mask ,power, shutter_mask & powerstate, Shutter_Direction[i]);
        AddLog(LOG_LEVEL_DEBUG);
      }
    }
  }
}

void Shutter_StartInit (uint8_t index, uint8_t direction, int32_t target_pos)
{
  Shutter_Direction[index] = direction;
  Shutter_Target_Position[index] = target_pos;
  Shutter_Start_Position[index] = Shutter_Real_Position[index];
  shutter_time[index] = 0;
}

boolean ShutterCommand()
{
  char command [CMDSZ];
  boolean serviced = true;
  boolean valid_entry = false;
  byte index;

  index = XdrvMailbox.index;

  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kShutterCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if ( (CMND_OPEN == command_code)  && (index > 0) && (index <= shutters_present)) {
    if ((Shutter_Open_Max[index-1] - Shutter_Real_Position[index-1]) / 100 > 1 ) {
      Shutter_StartInit(index-1, 1, Shutter_Open_Max[index-1]);

      ExecuteCommandPower(Settings.shutter_startrelay[index-1], 1, SRC_SHUTTER);
      if (!Settings.flag3.paired_interlock) {
        ExecuteCommandPower(Settings.shutter_startrelay[index-1]+1, 0, SRC_SHUTTER);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, index);
  }
  else if ( CMND_CLOSE == command_code  && (index > 0) && (index <= shutters_present)) {
    if (Shutter_Real_Position[index-1] / Shutter_Close_Velocity[index-1] > 1 ) {
      Shutter_StartInit(index-1, -1, 0);

      ExecuteCommandPower(Settings.shutter_startrelay[index-1]+1, 1, SRC_SHUTTER);
      if (!Settings.flag3.paired_interlock) {
        ExecuteCommandPower(Settings.shutter_startrelay[index-1], 1, SRC_SHUTTER);
      }
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, index);
  }
  else if (CMND_STOP == command_code && (index > 0) && (index <= shutters_present)) {
    Shutter_Target_Position[index-1] = Shutter_Real_Position[index-1];
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, index);
  }
  else if (CMND_SHUTTERINVERT == command_code && (index > 0) && (index <= shutters_present)) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
      Settings.shutter_invert[index-1] = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, XdrvMailbox.payload);
  }
  else if (CMND_POSITION == command_code && (index > 0) && (index <= shutters_present)) {
    // webgui still send also on inverted shutter the native position.
    XdrvMailbox.payload = Settings.shutter_invert[index-1] &&  SRC_WEBGUI != last_source ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
    Shutter_Target_Position[index-1] = XdrvMailbox.payload < 5 ?  m2[index-1] * XdrvMailbox.payload : m1[index-1] * XdrvMailbox.payload + b1[index-1];
    snprintf_P(log_data, sizeof(log_data), PSTR("lastsource %d: webgui:%d"), last_source,  SRC_WEBGUI);
    AddLog(LOG_LEVEL_DEBUG);

    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100) && abs(Shutter_Target_Position[index-1] - Shutter_Real_Position[index-1] ) / Shutter_Close_Velocity[index-1] > 1) {
      int8_t new_shutterdirection = Shutter_Real_Position[index-1] < Shutter_Target_Position[index-1] ? 1 : -1;

      if (Shutter_Direction[index-1] ==  -new_shutterdirection ) {
        // direction need to be changed. on momentary switches first stop the Shutter
        if (!Settings.flag3.paired_interlock) {
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] +1, new_shutterdirection == 1 ? 0 : 1,SRC_SHUTTER );
        } else {
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] + (Shutter_Direction[index-1] == 1 ? 0 : 1), 1, SRC_SHUTTER);
          delay(100);
        }
      }
      if (Shutter_Direction[index-1] !=  new_shutterdirection ) {
        Shutter_StartInit(index-1, new_shutterdirection, Shutter_Target_Position[index-1]);

        if (!Settings.flag3.paired_interlock) {
          // set direction
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] +1, new_shutterdirection == 1 ? 0 : 1, SRC_SHUTTER);
          // power on
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] , 1, SRC_SHUTTER);
        } else {
          // now start the motor for the right direction
          ExecuteCommandPower(Settings.shutter_startrelay[index-1] + (Shutter_Direction[index-1] == 1 ? 0 : 1), 1, SRC_SHUTTER);
        }
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index,  Settings.shutter_invert[index-1] ? 100 - XdrvMailbox.payload : XdrvMailbox.payload);
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.shutter_invert[index-1]  ? 100 - Settings.shutter_position[index-1] : Settings.shutter_position[index-1]);
    }
  }
  else if (((CMND_OPENTIME == command_code) || (CMND_CLOSETIME == command_code) ) && (index > 0) && (index <= shutters_present) ) {
    if (  (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      if (CMND_OPENTIME == command_code) {
        Settings.shutter_opentime[index-1] = XdrvMailbox.payload;
      } else {
        Settings.shutter_closetime[index-1] = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, XdrvMailbox.payload);
      ShutterInit();
    } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, (CMND_OPENTIME == command_code ? Settings.shutter_opentime[index-1] : Settings.shutter_closetime[index-1]));
    }
  }
  else if ((CMND_SHUTTERRELAY == command_code) && (index > 0) && (index <= MAX_SHUTTERS)) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 64)) {
      Settings.shutter_startrelay[index-1] = XdrvMailbox.payload;
      if (XdrvMailbox.payload > 0) {
        shutter_mask |= 3 << (XdrvMailbox.payload - 1);
      } else {
        shutter_mask ^= 3 << (Settings.shutter_startrelay[index-1] - 1);
      }
      snprintf_P(log_data, sizeof(log_data), PSTR("Shutterrelay %d: is:%d"), index,  XdrvMailbox.payload);
      AddLog(LOG_LEVEL_INFO);
      Settings.shutter_startrelay[index-1] = XdrvMailbox.payload;
      ShutterInit();
      // if payload is 0 to disable the relay there must be a reboot. Otherwhise does not work
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index,Settings.shutter_startrelay[index-1] );
  }
  else if (CMND_SET50PERCENT == command_code && (index > 0) && (index <= shutters_present)) {
      if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
        Settings.shutter_set50percent[index-1] = Settings.shutter_invert[index-1] ? 100 - XdrvMailbox.payload : XdrvMailbox.payload;
        ShutterInit();
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, XdrvMailbox.payload);
      } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.shutter_set50percent[index-1]);
      }
  }
  else if (CMND_SHUTTERSETCLOSE == command_code && (index > 0) && (index <= shutters_present)) {
      Shutter_Real_Position[index-1] = 0;
      Shutter_StartInit(index-1, 0, 0);
      Settings.shutter_position[index-1] = 0;
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_INDEX_NVALUE, command, index, D_CONFIGURATION_RESET);
  } else {
    serviced = false;  // Unknown command
  }
  return serviced;
}

void Schutter_Report_Position()
{
  for (byte i=0; i < shutters_present; i++) {
    if (Shutter_Direction[i] != 0) {
      char stemp1[20];
      //Settings.shutter_position[i] = m2[i] * 5 > Shutter_Real_Position[i] ? Shutter_Real_Position[i] / m2[i] : (Shutter_Real_Position[i]-b1[i]) / m1[i];
      snprintf_P(log_data, sizeof(log_data), PSTR("Shutter %d: Real Pos: %d, Target %d, source: %s, pos %%: %d, direction: %d, rtcshutter: %ld"), i,Shutter_Real_Position[i], Shutter_Target_Position[i], GetTextIndexed(stemp1, sizeof(stemp1), last_source, kCommandSource), Settings.shutter_position[i], Shutter_Direction[i],  shutter_time[i]);
      AddLog(LOG_LEVEL_INFO);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_97

boolean Xdrv97(byte function)
{
  boolean result = false;

  if (Settings.flag3.shutter_mode) {
    switch (function) {
      case FUNC_PRE_INIT:
        ShutterInit();
        break;
      case FUNC_EVERY_50_MSECOND:
        Schutter_Update_Position();
        break;
      case FUNC_EVERY_SECOND:
        Schutter_Report_Position();
        break;
      case FUNC_COMMAND:
        result = ShutterCommand();
        break;
      case FUNC_JSON_APPEND:
        for (byte i=0; i < shutters_present; i++) {
          snprintf_P(mqtt_data, sizeof(mqtt_data), JSON_SHUTTER_POS, mqtt_data, D_SHUTTER, i+1, Settings.shutter_invert[i] ? 100 - Settings.shutter_position[i]: Settings.shutter_position[i]);
        }
        break;
      case FUNC_SET_POWER:
        // extraxt the number of the relay that was switched and save for later in Update Position.
        SwitchedRelay = power ^ old_power;
        old_power = power;
      break;
    }
  }
  return result;
}

#endif
