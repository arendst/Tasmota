/*
  xsns_77_vl53l1x.ino - VL53L1X

  Copyright (C) 2018  Theo Arends, Rui Marinho and Johann Obermeier

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

#define XSNS_77     77
#define XI2C_53     53  // See I2CDEVICES.md

#include <Wire.h>
#include "VL53L1X.h"
VL53L1X sensor;

uint8_t vl53l1x_ready = 0;
uint16_t vl53l1x_distance = 0;
/********************************************************************************************/

void Vl53l1Detect(void) {
  if (!I2cSetDevice(0x29)) { return; }

  if (!sensor.init()) { return; }

  I2cSetActiveFound(sensor.getAddress(), "VL53L1X");

  sensor.setTimeout(500);

  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(140000);
  sensor.startContinuous(50);
  vl53l1x_ready = 1;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_VL53L1X[] PROGMEM =
 "{s}VL53L1X " D_DISTANCE "{m}%d" D_UNIT_MILLIMETER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

void Vl53l1Every_250MSecond(void) {
  // every 200 ms
  uint16_t dist = sensor.read();
  if ((0 == dist) || (dist > 4000)) {
    dist = 9999;
  }
  vl53l1x_distance = dist;
}

/* No Distance support for Domoticz
#ifdef USE_DOMOTICZ
void Vl53l1Every_Second(void) {
  DomoticzSensor(DZ_ILLUMINANCE, vl53l1x_distance);
}
#endif  // USE_DOMOTICZ
*/

void Vl53l1Show(boolean json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"VL53L1X\":{\"" D_JSON_DISTANCE "\":%d}"), vl53l1x_distance);

/* No Distance support for Domoticz
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_ILLUMINANCE, vl53l1x_distance);
    }
#endif  // USE_DOMOTICZ
*/

#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_VL53L1X, vl53l1x_distance);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns77(byte function)
{
  if (!I2cEnabled(XI2C_53)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Vl53l1Detect();
  }
  else if (vl53l1x_ready) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Vl53l1Every_250MSecond();
        break;
/* #ifdef USE_DOMOTICZ
     case FUNC_EVERY_SECOND:
        Vl53l1Every_Second();
        break;
#endif  // USE_DOMOTICZ */
      case FUNC_JSON_APPEND:
        Vl53l1Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Vl53l1Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VL53L1X
#endif  // USE_I2C
