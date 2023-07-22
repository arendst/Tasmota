/*
  xsns_109_max17043.ino - Support for MAX17043 fuel-gauge systems Lipo batteries for Tasmota

  Copyright (C) 2023  Vincent de Groot

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
#ifdef USE_MAX17043

#define XI2C_83       83    // See I2CDEVICES.md

#define SENSOR_NAME    "MAX17043"

#include "DFRobot_MAX17043.h"

/*********************************************************************************************\
 * MAX17043 fuel-gauge for 3.7 Volt Lipo batteries
 *
 * Battery voltage in Volt and State Of Charge (SOC) in percent are published via MQTT
 * 
 * The alert flag and alert threshold are not required for MQTT, the alert pin is not used 
 * by this sensor driver.
 * 
 * Wirering and other information:
 * 
 * \lib\lib_i2c\DFRobot_MAX17043\resources
 * 
 * Tested module(s):
 * 
 * https://www.dfrobot.com/product-1734.html
 * 
 * Not yet tested module(s):
 * 
 * https://www.aliexpress.us/item/2251832479401925.html 
 * 
 \*********************************************************************************************/

#define XSNS_109        109

const char *mqttId = "MAX17043";

DFRobot_MAX17043     gauge; // Class to read from MAX17043

struct MAX17043 
{
  float voltage = 0.0;      // Battery voltage in Volt
  float percentage = 0.0;   // Battery remaining charge in percent
} *max17043 = nullptr;

 /*********************************************************************************************/

void Max17043Init(void) {

  if (I2cSetDevice(MAX17043_ADDRESS)) { 
    I2cSetActiveFound(MAX17043_ADDRESS, "MAX17043");
    if (gauge.begin() == 0) {
      max17043 = (MAX17043 *)calloc(1, sizeof(struct MAX17043));
    }
  }
}

void Max17043Read(void) {

  float percentage = 0.0;
  
  max17043->voltage = gauge.readVoltage()/1000.0;

  // During charging the percentage might be (slightly) above 100%. To avoid stange numbers
  // in the statistics we the percentage provided by this driver will not go above 100%
  percentage = gauge.readPercentage();
  if (percentage > 100.0) {
    max17043->percentage = 100.0;
  }
  else {
    max17043->percentage = percentage;
  }
}

void Max17043Json(void) {
  ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_VOLTAGE "\":%3_f,\"" D_JSON_BATTPERCENT "\":%2_f}"), mqttId, &max17043->voltage, &max17043->percentage );
}

#ifdef USE_WEBSERVER
void Max17043Show(void) {
  WSContentSend_PD(PSTR("{s}%s " D_VOLTAGE "{m}%1_f" D_UNIT_VOLT "{e}"), SENSOR_NAME, &max17043->voltage);
  WSContentSend_PD(PSTR("{s}%s " D_BATTERY_CHARGE "{m}%1_f %% {e}"), SENSOR_NAME, &max17043->percentage);
}  
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 *  Interface
\*********************************************************************************************/

bool Xsns109(uint32_t function) {
if (!I2cEnabled(MAX17043_ADDRESS)) { return false; } 

  if (FUNC_INIT == function) {
    Max17043Init();
  }
  else if (max17043 != nullptr) {
    switch (function) {
    case FUNC_EVERY_SECOND:
      Max17043Read();
      break;
    case FUNC_JSON_APPEND:
      Max17043Json();
      break;
    #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Max17043Show();
        break;
    #endif // USE_WEBSERVER
    }
  }
  return false;
}

#endif  // USE_MAX17043
#endif  // USE_I2C