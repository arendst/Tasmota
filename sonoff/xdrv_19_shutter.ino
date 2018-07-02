/*
  xdrv_19_Shutter.ino - PWM, WS2812 and sonoff led support for Sonoff-TasmotaS

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

enum ShutterCommands {
  CMND_OPEN, CMND_CLOSE, CMND_STOP, CMND_POSITION, CMND_OPENTIME, CMND_CLOSETIME, CMND_SHUTTERRELAY };
const char kShutterCommands[] PROGMEM =
  D_CMND_OPEN "|" D_CMND_CLOSE "|" D_CMND_STOP "|" D_CMND_POSITION  "|" D_CMND_OPENTIME "|" D_CMND_CLOSETIME "|" D_CMND_SHUTTERRELAY;


uint8_t Shutter_Open_Time = 0;               // duration to open the shutter
uint8_t Shutter_Close_Time = 0;              // duratin to close the shutter
uint8_t Shutter_Open_Velocity = 100;
uint32_t Shutter_Open_Max = 0;               // max value on maximum open calculated
uint16_t Shutter_Target_Position = 0;        // position to go to
uint16_t Shutter_Close_Velocity =0;          // in relation to open velocity. higher value = faster
int8_t  Shutter_Direction = 0;               // 1 == UP , 0 == stop; -1 == down
uint32_t Shutter_Real_Position = 0;          // value between 0 and Shutter_Open_Max


void ShutterInit()
{
  // set startrelay to 1 on first init
  Settings.shutter_startrelay = (Settings.shutter_startrelay = 0 ? 1 : Settings.shutter_startrelay);
  Shutter_Open_Time = (Settings.shutter_opentime>0 ? Settings.shutter_opentime : 10);
  Shutter_Close_Time = (Settings.shutter_closetime> 0 ? Settings.shutter_closetime : 10);
  // Update Calculation
  Shutter_Open_Max = 20 * Shutter_Open_Velocity * Shutter_Open_Time;
  Shutter_Close_Velocity = Shutter_Open_Max / ( Shutter_Close_Time * 20 );

  Shutter_Real_Position =   Settings.shutter_position * Shutter_Open_Max / 100;
  snprintf_P(log_data, sizeof(log_data), PSTR("ShutterInit: Position %d [%d %%], Open Velocity: %d Close Velocity: %d , MAx Way: %d, Opentime %d [s], Closetime %d [s]"), Shutter_Real_Position,Settings.shutter_position, Shutter_Open_Velocity, Shutter_Close_Velocity , Shutter_Open_Max, Shutter_Open_Time, Shutter_Close_Time);
  AddLog(LOG_LEVEL_INFO);
}

void Schutter_Update_Position()
{
  if (Shutter_Direction > 0) {
    Shutter_Real_Position = Shutter_Real_Position + Shutter_Direction * Shutter_Open_Velocity;
    if (Shutter_Real_Position > Shutter_Target_Position)  {
      Shutter_Direction=0;
      snprintf_P(log_data, sizeof(log_data), PSTR("Shutter: Opened Position %d"), Shutter_Real_Position);
      AddLog(LOG_LEVEL_DEBUG);
      ExecuteCommandPower(Settings.shutter_startrelay, 0, SRC_SHUTTER);
    }
  } else if (Shutter_Direction < 0){
    Shutter_Real_Position = Shutter_Real_Position + Shutter_Direction * Shutter_Close_Velocity;
    if (Shutter_Real_Position < Shutter_Target_Position || Shutter_Real_Position < Shutter_Close_Velocity) {
      Shutter_Direction=0;
      snprintf_P(log_data, sizeof(log_data), PSTR("Shutter: Closed Position %d"), Shutter_Real_Position);
      AddLog(LOG_LEVEL_DEBUG);
      ExecuteCommandPower(Settings.shutter_startrelay+1, 0, SRC_SHUTTER);
    }
  }
}
/********************************************************************************************/

//boolean ShutterCommand(char *type, uint16_t index, char *dataBuf, uint16_t XdrvMailbox.data_len, int16_t XdrvMailbox.payload)
boolean ShutterCommand()
{
  char command [CMDSZ];
  boolean serviced = true;
  boolean valid_entry = false;

  snprintf_P(log_data, sizeof(log_data), PSTR("Shutter: Command Code: '%s'"), XdrvMailbox.topic);
  AddLog(LOG_LEVEL_DEBUG);
  int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kShutterCommands);
  if (-1 == command_code) {
    serviced = false;  // Unknown command
  }
  else if (CMND_OPEN == command_code) {
    Shutter_Direction = 1;
    Shutter_Target_Position = Shutter_Open_Max;
    Settings.shutter_position = 100;
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, 1);
    ExecuteCommandPower(Settings.shutter_startrelay, 1, SRC_SHUTTER);
  }
  else if (CMND_CLOSE == command_code) {
    Shutter_Direction = -1;
    Shutter_Target_Position = 0;
    Settings.shutter_position = 0;
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, 1);
    ExecuteCommandPower(Settings.shutter_startrelay+1, 1, SRC_SHUTTER);
  }
  else if (CMND_STOP == command_code) {
    Shutter_Direction = 0;
    Settings.shutter_position = Shutter_Real_Position  *100 / Shutter_Open_Max;
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, 1);
    ExecuteCommandPower(Settings.shutter_startrelay, 0, SRC_SHUTTER);
  }
  else if (CMND_POSITION == command_code) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      Shutter_Target_Position = Shutter_Open_Max / 100 * XdrvMailbox.payload;
      Settings.shutter_position = XdrvMailbox.payload;
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, XdrvMailbox.payload);
      Shutter_Direction =  (Shutter_Real_Position < Shutter_Target_Position ? 1 : -1);
      if (Shutter_Direction == 1) {
        ExecuteCommandPower(Settings.shutter_startrelay, 1, SRC_SHUTTER);
      } else {
        ExecuteCommandPower(Settings.shutter_startrelay+1, 1, SRC_SHUTTER);
      }
    } else {
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.shutter_position);
    }
  }
  else if (((CMND_OPENTIME == command_code) || (CMND_CLOSETIME == command_code) ) ) {
    if (  (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
      if (CMND_OPENTIME == command_code) {
        Settings.shutter_opentime = XdrvMailbox.payload;
      } else {
        Settings.shutter_closetime = XdrvMailbox.payload;
      }
      snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command,XdrvMailbox.payload);
      ShutterInit();
    } else {
        snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, (CMND_OPENTIME == command_code ? Settings.shutter_opentime : Settings.shutter_closetime));
    }
  }
  else if (CMND_SHUTTERRELAY == command_code) {
    if ( (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 64)) {
      Settings.shutter_startrelay = XdrvMailbox.payload;
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), S_JSON_COMMAND_NVALUE, command, Settings.shutter_startrelay );
  } else {
    serviced = false;  // Unknown command
  }
  return serviced;
}


void Schutter_Report_Position()
{
  if (Shutter_Direction != 0) {
    snprintf_P(log_data, sizeof(log_data), PSTR("Shutter: Real Position %d, Target %d, Close Velocity %d"), Shutter_Real_Position, Shutter_Target_Position, Shutter_Close_Velocity);
    AddLog(LOG_LEVEL_DEBUG_MORE);
    //Settings.shutter_position = Shutter_Open_Max *100 / Shutter_Real_Position;
  }
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XDRV_19

boolean Xdrv19(byte function)
{
  boolean result = false;

  if (Settings.flag.shutter_mode) {
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
    }
  }
  return result;
}
