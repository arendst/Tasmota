/*
  xsns_33_vl53l1x.ino - VL53L1X

  Copyright (C) 2018  Theo Arends and Rui Marinho

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
#ifdef USE_VL53L1X
/*********************************************************************************************\
 * VL53L1X
 *
 * Source:
 *
 * I2C Address: 0x52
\*********************************************************************************************/

#include <Wire.h>
#include "VL53L1X.h"
VL53L1X sensor;

uint8_t vl53l1x_ready = 0;

/********************************************************************************************/

void Vl53l1Detect()
{
  if (vl53l1x_ready) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("VL53L1X is ready"));
    return;
  }

  sensor.setTimeout(500);

  if (!sensor.init()) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("Unable to init sensor"));
    return;
  }

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(140000);
  sensor.startContinuous(50);
  vl53l1x_ready = 1;

  snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "VL53L1X", sensor.getAddress());
  AddLog(LOG_LEVEL_DEBUG);
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_VL53L1X[] PROGMEM =
  "%s{s}VL53L1X " D_DISTANCE "{m}%d" D_UNIT_MILLIMETER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Vl53l1Show(boolean json)
{
  if (!vl53l1x_ready) {
    return;
  }

  uint16_t distance = sensor.read();

  if (json) {
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"VL53L1X\":{\"" D_JSON_DISTANCE "\":%d}"), mqtt_data, distance);
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_VL53L1X, mqtt_data, distance);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_33

boolean Xsns33(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Vl53l1Detect();
      case FUNC_JSON_APPEND:
        Vl53l1Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_APPEND:
        Vl53l1Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VL53L1X
#endif  // USE_I2C
