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
#include "Wire.h"


// TODO: Update description

/*********************************************************************************************\
 * MAX17043 fuel-gauge systems 3,7 volt Lipo batteries
 *
 * For background information see https://github.com/DFRobot/DFRobot_MAX17043/tree/master
 *
 * Commands available:
 * 
 * 
 \*********************************************************************************************/

#define XSNS_109        109

DFRobot_MAX17043        gauge;

/********************************************************************************************/

void Max17043Init(void) {
  Serial.begin(115200);
  while(!Serial);
  Serial.println();
  Serial.println();
  
  while(gauge.begin() != 0) {
    Serial.println("gauge begin faild!");
    delay(2000);
  }
  delay(2);
  Serial.println("gauge begin successful!");
}

void Max17043SerialInput(void) {
    Serial.print("voltage: ");
    Serial.print(gauge.readVoltage());
    Serial.println(" mV");

    Serial.print("precentage: ");
    Serial.print(gauge.readPercentage());
    Serial.println(" %");
}


/*********************************************************************************************\
   Interface
\*********************************************************************************************/

bool Xsns109(uint32_t function) {

  // Set return value to `false`
  boolean result = false;

  // Check which callback ID is called by Tasmota
  switch (function) {
    case FUNC_INIT:
        Max17043Init(); 
      break;
    case FUNC_LOOP:
    case FUNC_SLEEP_LOOP:
      Max17043SerialInput();
      break;
    case FUNC_JSON_APPEND:
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
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