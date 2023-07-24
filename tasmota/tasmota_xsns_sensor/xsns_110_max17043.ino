/*
  xsns_110_max17043.ino - Support for MAX17043 fuel-gauge systems Lipo batteries for Tasmota

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
/*********************************************************************************************\
 * MAX17043 fuel-gauge for 3.7 Volt Lipo batteries
 *
 * Battery voltage in Volt and State Of Charge (SOC) in percent are published via MQTT
 * 
 * The alert flag and alert threshold are not required for MQTT, the alert pin is not used 
 * by this sensor driver.
 * 
 * Wiring and other information:
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

#define XSNS_110      110
#define XI2C_83       83          // See I2CDEVICES.md

#include "DFRobot_MAX17043.h"

DFRobot_MAX17043 max17043_gauge;  // Class to read from MAX17043
bool max17043 = false;

/*********************************************************************************************/

void Max17043Init(void) {
  if (I2cSetDevice(MAX17043_ADDRESS)) { 
    if (max17043_gauge.begin() == 0) {
      max17043 = true;
      I2cSetActiveFound(MAX17043_ADDRESS, "MAX17043");
    }
  }
}

void Max17043Show(bool json) {
  float voltage = max17043_gauge.readVoltage() / 1000.0;  // Battery voltage in Volt

  // During charging the percentage might be (slightly) above 100%. To avoid strange numbers
  // in the statistics the percentage provided by this driver will not go above 100%
  float percentage = max17043_gauge.readPercentage();     // Battery remaining charge in percent
  if (percentage > 100.0) { percentage = 100.0; }

  if (json) {
    ResponseAppend_P(PSTR(",\"MAX17043\":{\"" D_JSON_VOLTAGE "\":%3_f,\"" D_JSON_BATTPERCENT "\":%2_f}"), &voltage, &percentage );
#ifdef USE_WEBSERVER
  } else {
//    WSContentSend_Voltage("MAX17043", voltage);
    WSContentSend_PD(PSTR("{s}MAX17043 " D_VOLTAGE "{m}%1_f" D_UNIT_VOLT "{e}"), &voltage);
    WSContentSend_PD(PSTR("{s}MAX17043 " D_BATTERY_CHARGE "{m}%1_f %% {e}"), &percentage);
#endif
  }
}

/*********************************************************************************************\
 *  Interface
\*********************************************************************************************/

bool Xsns110(uint32_t function) {
  if (!I2cEnabled(MAX17043_ADDRESS)) { return false; } 

  if (FUNC_INIT == function) {
    Max17043Init();
  }
  else if (max17043) {
    switch (function) {
      case FUNC_JSON_APPEND:
        Max17043Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Max17043Show(0);
        break;
#endif // USE_WEBSERVER
    }
  }
  return false;
}

#endif  // USE_MAX17043
#endif  // USE_I2C
