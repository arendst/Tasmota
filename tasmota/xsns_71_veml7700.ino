/*
  xsns_71_VEML7700.ino - VEML7700 Franklin Lightning Sensor support for Tasmota

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

const char HTTP_SNS_LUX[] PROGMEM = "{s}%s " D_ILLUMINANCE "{m}%d " D_UNIT_LUX " {e}";
const char HTTP_SNS_WHITE[] PROGMEM = "{s}%s " D_WHITE_CONTENT "{m}%d {e}";
const char JSON_SNS_VEML7700[] PROGMEM = ",\"%s\":{\"" D_JSON_ILLUMINANCE "\":%d,\"" D_JSON_WHITE_CONTENT "\":%d}";

struct VEML7700STRUCT
{
  char types[9]   = D_NAME_VEML7700;
  uint8_t address = VEML7700_I2CADDR_DEFAULT;  
  uint16_t lux = 0;
  uint16_t white = 0;
} veml7700_sensor;

uint8_t veml7700_active = 0;

/********************************************************************************************/

void VEML7700Detect(void) {
  if (I2cActive(veml7700_sensor.address)) return;
  if (veml7700.begin()) {
    I2cSetActiveFound(veml7700_sensor.address, veml7700_sensor.types);
    veml7700_active = 1;
  }
}

void VEML7700EverySecond(void) {
    veml7700_sensor.lux = (uint16_t) veml7700.readLux();
    veml7700_sensor.white = (uint16_t) veml7700.readWhite();
}

void VEML7700Show(bool json)
{
  if (json) {
    ResponseAppend_P(JSON_SNS_VEML7700, D_NAME_VEML7700, veml7700_sensor.lux, veml7700_sensor.white);
    
#ifdef USE_DOMOTICZ
      if (0 == tele_period) DomoticzSensor(DZ_ILLUMINANCE, veml7700_sensor.lux);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {   
    WSContentSend_PD(HTTP_SNS_LUX, D_NAME_VEML7700, veml7700_sensor.lux);
    WSContentSend_PD(HTTP_SNS_WHITE, D_NAME_VEML7700, veml7700_sensor.white);
#endif // USE_WEBSERVER
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
  else if (veml7700_active) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        VEML7700EverySecond();
        break;
      case FUNC_COMMAND:
        //result = VEML7700Cmd();
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
