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

#include "DFRobot_MAX17043.h"

/*********************************************************************************************\
 * MAX17043 fuel-gauge for 3.7 volt Lipo batteries
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
  float voltage = 0.0;      // Battery voltage in miliVolt
  float percentage = 0.0;   // Battery remaining capacity in percent
} *max17043 = nullptr;

 /*********************************************************************************************/

void Max17043Init(void) {

  if (gauge.begin() != 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("I2C: MAX17043 not found"));
  } else {
    max17043 = (MAX17043 *)calloc(1, sizeof(struct MAX17043));
    AddLog(LOG_LEVEL_INFO, PSTR("I2C: MAX17043 initialized"));
  }
}

void Max17043Read(void) {

  float percentage = 0.0;
  
  max17043->voltage = gauge.readVoltage();

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

  ResponseAppend_P(JSON_SNS_BGAUGE, mqttId, String(max17043->voltage/1000, 2), String(max17043->percentage, 1));
}

#ifdef USE_WEBSERVER
void Max17043Show(void) {

  WSContentSend_P(PSTR("Battery: %.1f V,  "), max17043->voltage/1000);
  WSContentSend_P(PSTR("%.1f %%"), max17043->percentage);
}
#endif  // USE_WEBSERVER

/*********************************************************************************************\
 *  Interface
\*********************************************************************************************/

bool Xsns109(uint32_t function) {

  // Set return value to `false`
  bool result = false;

  // Check which callback ID is called by Tasmota
  switch (function) {
    case FUNC_INIT:
      Max17043Init(); 
      break;
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
    case FUNC_SAVE_BEFORE_RESTART:
      break;
    case FUNC_COMMAND:
      break;
  }
  // Return boolean result
  return result;
}

#endif  // USE_MAX17043
#endif  // USE_I2C