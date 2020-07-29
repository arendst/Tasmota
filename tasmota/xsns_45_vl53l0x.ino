/*
  xsns_45_vl53l0x.ino - VL53L0X support for Tasmota

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

#define XSNS_45 45
#define XI2C_31 31  // See I2CDEVICES.md

#include <Wire.h>
#include "VL53L0X.h"
VL53L0X sensor;

uint8_t vl53l0x_ready = 0;
uint16_t vl53l0x_distance;
uint16_t Vl53l0_buffer[5];
uint8_t Vl53l0_index;

/********************************************************************************************/

void Vl53l0Detect(void)
{
  if (!I2cSetDevice(0x29)) { return; }

  if (!sensor.init()) { return; }

  I2cSetActiveFound(sensor.getAddress(), "VL53L0X");

  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  vl53l0x_ready = 1;

  Vl53l0_index=0;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_VL53L0X[] PROGMEM =
 "{s}VL53L0X " D_DISTANCE "{m}%d" D_UNIT_MILLIMETER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#define USE_VL_MEDIAN

void Vl53l0Every_250MSecond(void)
{
  uint16_t tbuff[5],tmp;
  uint8_t flag;

  // every 200 ms
  uint16_t dist = sensor.readRangeContinuousMillimeters();
  if (dist==0 || dist>2000) {
    dist=9999;
  }

#ifdef USE_VL_MEDIAN
  // store in ring buffer
  Vl53l0_buffer[Vl53l0_index]=dist;
  Vl53l0_index++;
  if (Vl53l0_index>=5) Vl53l0_index=0;

  // sort list and take median
  memmove(tbuff,Vl53l0_buffer,sizeof(tbuff));
  for (byte ocnt=0; ocnt<5; ocnt++) {
    flag=0;
    for (byte count=0; count<4; count++) {
      if (tbuff[count]>tbuff[count+1]) {
        tmp=tbuff[count];
        tbuff[count]=tbuff[count+1];
        tbuff[count+1]=tmp;
        flag=1;
      }
    }
    if (!flag) break;
  }
  vl53l0x_distance=tbuff[2];
#else
  vl53l0x_distance=dist;
#endif
}

void Vl53l0Show(boolean json)
{
  if (json) {
    ResponseAppend_P(PSTR(",\"VL53L0X\":{\"" D_JSON_DISTANCE "\":%d}"), vl53l0x_distance);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_VL53L0X, vl53l0x_distance);
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
  else if (vl53l0x_ready) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Vl53l0Every_250MSecond();
        break;
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
