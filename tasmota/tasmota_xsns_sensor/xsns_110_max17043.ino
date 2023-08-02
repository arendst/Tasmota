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

#define XSNS_110           110
#define XI2C_83            83      // See I2CDEVICES.md

#define MAX17043_ADDRESS   0x36

//#define MAX17043_USE_LIB

#ifdef MAX17043_USE_LIB

#include "DFRobot_MAX17043.h"
DFRobot_MAX17043 max17043_gauge;   // Class to read from MAX17043

#else

#define MAX17043_VCELL     0x02
#define MAX17043_SOC       0x04
#define MAX17043_MODE      0x06
#define MAX17043_VERSION   0x08
#define MAX17043_CONFIG    0x0c
#define MAX17043_COMMAND   0xfe

#endif  // MAX17043_USE_LIB

bool max17043 = false;

/*********************************************************************************************/

void Max17043Init(void) {
  if (I2cSetDevice(MAX17043_ADDRESS)) { 

#ifdef MAX17043_USE_LIB

    if (max17043_gauge.begin() == 0) {

#else

    I2cWrite16(MAX17043_ADDRESS, MAX17043_COMMAND, 0x5400);        // Power on reset
    delay(10);
    if (I2cRead16(MAX17043_ADDRESS, MAX17043_CONFIG) == 0x971c) {  // Default 0x971c
      I2cWrite16(MAX17043_ADDRESS, MAX17043_MODE, 0x4000);         // Quick start
      I2cWrite16(MAX17043_ADDRESS, MAX17043_CONFIG, 0x9700);
      delay(10);

#endif  // MAX17043_USE_LIB

      max17043 = true;
      I2cSetActiveFound(MAX17043_ADDRESS, "MAX17043");
    }
  }
}

void Max17043Show(bool json) {

#ifdef MAX17043_USE_LIB

  float voltage = max17043_gauge.readVoltage() / 1000.0;  // Battery voltage in Volt
  float percentage = max17043_gauge.readPercentage();     // Battery remaining charge in percent

#else

  float voltage = (1.25f * (float)(I2cRead16(MAX17043_ADDRESS, MAX17043_VCELL) >> 4)) / 1000.0;  // Battery voltage in Volt
  uint16_t per = I2cRead16(MAX17043_ADDRESS, MAX17043_SOC);
  float percentage = (float)((per >> 8) + 0.003906f * (per & 0x00ff));  // Battery remaining charge in percent

#endif  // MAX17043_USE_LIB

  // During charging the percentage might be (slightly) above 100%. To avoid strange numbers
  // in the statistics the percentage provided by this driver will not go above 100%
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
