/*
  xsns_71_VEML7700.ino - VEML7700 Ambient light intensity Sensor support for Tasmota

  Copyright (C) 2020  Martin Wagner

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

#ifdef USE_I2C
#ifdef USE_VEML7700
/*********************************************************************************************\
 * VEML7700 ALS Sensor
 * Using the Adafruit VEML7700 Libary
 * I2C Address: 0x10
\*********************************************************************************************/

#define XSNS_71             71
#define XI2C_50             50  // See I2CDEVICES.md

#include "Adafruit_VEML7700.h"
Adafruit_VEML7700 veml7700 = Adafruit_VEML7700(); //create object copy

#define D_NAME_VEML7700 "VEML7700"
#define D_WHITE_CONTENT "White content"

const char HTTP_SNS_WHITE[] PROGMEM = "{s}%s " D_WHITE_CONTENT "{m}%d {e}";
const char JSON_SNS_VEML7700[] PROGMEM = ",\"%s\":{\"" D_JSON_ILLUMINANCE "\":%d,\"" D_JSON_WHITE_CONTENT "\":%d}";

#define D_CMND_VEML7700_PWR "power"
#define D_CMND_VEML7700_GAIN "gain"
#define D_CMND_VEML7700_INTTIME "inttime"
#define D_CMND_VEML7700_PERSIST "persist"

const char S_JSON_VEML7700_COMMAND_NVALUE[] PROGMEM = "{\"" D_NAME_VEML7700 "\":{\"%s\":%d}}";
const char kVEML7700_Commands[] PROGMEM  = D_CMND_VEML7700_PWR "|" D_CMND_VEML7700_GAIN "|" D_CMND_VEML7700_INTTIME "|" D_CMND_VEML7700_PERSIST;

enum VEML7700_Commands {         // commands for Console
  CMND_VEML7700_PWR,
  CMND_VEML7700_GAIN,
  CMND_VEML7700_SET_IT,
  CMND_VEML7700_PERSIST,
};

struct VEML7700STRUCT
{
  bool active = 0;
  char types[9]   = D_NAME_VEML7700;
  uint8_t address = VEML7700_I2CADDR_DEFAULT;
  uint32_t lux_normalized = 0;
  uint32_t white_normalized = 0;
} veml7700_sensor;


/********************************************************************************************/

void VEML7700Detect(void) {
  if (!I2cSetDevice(veml7700_sensor.address)) return;
  if (veml7700.begin()) {
    I2cSetActiveFound(veml7700_sensor.address, veml7700_sensor.types);
    veml7700_sensor.active = 1;
  }
}

uint16_t VEML7700TranslateItMs (uint8_t ittime){
  switch (ittime) {
    case 0:  return 100;
    case 1:  return 200;
    case 2:  return 400;
    case 3:  return 800;
    case 8:  return 50;
    case 12: return 25;
    default: return 0xFFFF;
  }
}

uint8_t VEML7700TranslateItInt (uint16_t ittimems){
  switch (ittimems) {
    case 100: return 0;
    case 200: return 1;
    case 400: return 2;
    case 800: return 3;
    case 50:  return 8;
    case 25:  return 12;
    default:  return 0xFF;
  }
}

void VEML7700EverySecond(void) {
  veml7700_sensor.lux_normalized = (uint32_t) veml7700.readLuxNormalized();
  veml7700_sensor.white_normalized = (uint32_t) veml7700.readWhiteNormalized();
}

void VEML7700Show(bool json)
{
  if (json) {
    ResponseAppend_P(JSON_SNS_VEML7700, D_NAME_VEML7700, veml7700_sensor.lux_normalized, veml7700_sensor.white_normalized);

#ifdef USE_DOMOTICZ
      if (0 == tele_period) DomoticzSensor(DZ_ILLUMINANCE, veml7700_sensor.lux_normalized);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_ILLUMINANCE, D_NAME_VEML7700, veml7700_sensor.lux_normalized);
    WSContentSend_PD(HTTP_SNS_WHITE, D_NAME_VEML7700, veml7700_sensor.white_normalized);
#endif // USE_WEBSERVER
  }
}

bool VEML7700Cmd(void) {
  char command[CMDSZ];
  uint8_t name_len = strlen(D_NAME_VEML7700);
  if (!strncasecmp_P(XdrvMailbox.topic, PSTR(D_NAME_VEML7700), name_len)) {
    uint32_t command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic + name_len, kVEML7700_Commands);
    switch (command_code) {
      case CMND_VEML7700_PWR:
        if (XdrvMailbox.data_len) {
          if (2 >= XdrvMailbox.payload) {
            veml7700.enable(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_VEML7700_COMMAND_NVALUE, command, veml7700.enabled());
       break;
      case CMND_VEML7700_GAIN:
        if (XdrvMailbox.data_len) {
          if (4 >= XdrvMailbox.payload) {
            veml7700.setGain(XdrvMailbox.payload);
          }
        }
        Response_P(S_JSON_VEML7700_COMMAND_NVALUE, command, veml7700.getGain());
        break;
      case CMND_VEML7700_SET_IT: {
        if (XdrvMailbox.data_len) {
          uint8_t data = VEML7700TranslateItInt(XdrvMailbox.payload);
          if (0xFF != data) {
            veml7700.setIntegrationTime(data);
          }
        }
        uint16_t dataret = VEML7700TranslateItMs(veml7700.getIntegrationTime());
        Response_P(S_JSON_VEML7700_COMMAND_NVALUE, command, dataret);
      }
      break;
    case CMND_VEML7700_PERSIST:
       if (XdrvMailbox.data_len) {
         if (4 >= XdrvMailbox.payload) {
           veml7700.setPersistence(XdrvMailbox.payload);
         }
       }
       Response_P(S_JSON_VEML7700_COMMAND_NVALUE, command, veml7700.getPersistence());
       break;    
    default:
      return false;
    }
    return true;
  }
  else {
    return false;
  }
}
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns71(uint8_t function)
{
  if (!I2cEnabled(XI2C_50)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    VEML7700Detect();
  }
  else if (veml7700_sensor.active) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        VEML7700EverySecond();
        break;
      case FUNC_COMMAND:
        result = VEML7700Cmd();
        break;
      case FUNC_JSON_APPEND:
        VEML7700Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        VEML7700Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VEML7700
#endif  // USE_I2C
