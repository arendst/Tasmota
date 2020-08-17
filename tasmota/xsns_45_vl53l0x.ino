/*
  xsns_45_vl53l0x.ino - VL53L0X time of flight sensor support for Tasmota

  Copyright (C) 2020  Theo Arends and Gerhard Mutz

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
#ifdef USE_VL53L0X
/*********************************************************************************************\
 * VL53L0x time of flight sensor
 *
 * I2C Addres: 0x29
\*********************************************************************************************/

#define XSNS_45     45
#define XI2C_31     31  // See I2CDEVICES.md

#include <Wire.h>
#include "VL53L0X.h"
VL53L0X sensor;

struct {
  uint16_t distance;
  uint16_t distance_prev;
  uint16_t buffer[5];
  uint8_t ready = 0;
  uint8_t index;
} Vl53l0x;

/********************************************************************************************/

void Vl53l0Detect(void) {
  if (!I2cSetDevice(0x29)) { return; }

  if (!sensor.init()) { return; }

  I2cSetActiveFound(sensor.getAddress(), "VL53L0X");

  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  Vl53l0x.ready = 1;

  Vl53l0x.index = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_VL53L0X[] PROGMEM =
 "{s}VL53L0X " D_DISTANCE "{m}%d" D_UNIT_MILLIMETER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#define USE_VL_MEDIAN

void Vl53l0Every_250MSecond(void) {
  // every 200 ms
  uint16_t dist = sensor.readRangeContinuousMillimeters();
  if ((0 == dist) || (dist > 2000)) {
    dist = 9999;
  }

#ifdef USE_VL_MEDIAN
  // store in ring buffer
  Vl53l0x.buffer[Vl53l0x.index] = dist;
  Vl53l0x.index++;
  if (Vl53l0x.index >= 5) {
    Vl53l0x.index = 0;
  }

  // sort list and take median
  uint16_t tbuff[5];
  memmove(tbuff, Vl53l0x.buffer, sizeof(tbuff));
  uint16_t tmp;
  uint8_t flag;
  for (uint32_t ocnt = 0; ocnt < 5; ocnt++) {
    flag = 0;
    for (uint32_t count = 0; count < 4; count++) {
      if (tbuff[count] > tbuff[count +1]) {
        tmp = tbuff[count];
        tbuff[count] = tbuff[count +1];
        tbuff[count +1] = tmp;
        flag = 1;
      }
    }
    if (!flag) { break; }
  }
  Vl53l0x.distance = tbuff[2];
#else
  Vl53l0x.distance = dist;
#endif
}

#ifdef USE_DOMOTICZ
void Vl53l0Every_Second(void) {
  if (abs(Vl53l0x.distance - Vl53l0x.distance_prev) > 8) {
    Vl53l0x.distance_prev = Vl53l0x.distance;
    DomoticzSensor(DZ_ILLUMINANCE, Vl53l0x.distance);
  }
}
#endif  // USE_DOMOTICZ

void Vl53l0Show(boolean json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"VL53L0X\":{\"" D_JSON_DISTANCE "\":%d}"), Vl53l0x.distance);
#ifdef USE_DOMOTICZ
    if (0 == tele_period) {
      DomoticzSensor(DZ_ILLUMINANCE, Vl53l0x.distance);
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_VL53L0X, Vl53l0x.distance);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns45(byte function)
{
  if (!I2cEnabled(XI2C_31)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Vl53l0Detect();
  }
  else if (Vl53l0x.ready) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Vl53l0Every_250MSecond();
        break;
#ifdef USE_DOMOTICZ
     case FUNC_EVERY_SECOND:
        Vl53l0Every_Second();
        break;
#endif  // USE_DOMOTICZ
      case FUNC_JSON_APPEND:
        Vl53l0Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Vl53l0Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_VL53L0X
#endif  // USE_I2C
