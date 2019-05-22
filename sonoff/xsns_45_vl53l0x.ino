/*
  xsns_99_vl53l0x.ino - VL53L0X

  Copyright (C) 2018  Theo Arends and Gerhard Mutz

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

#include <Wire.h>
#include "VL53L0X.h"
VL53L0X sensor;

uint8_t vl53l0x_ready = 0;
uint16_t vl53l0x_distance;
uint16_t Vl53l0_buffer[5];
uint8_t Vl53l0_index;


/********************************************************************************************/

void Vl53l0Detect()
{

  if (!I2cDevice(0x29)) {
    return;
  }

  if (vl53l0x_ready) {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("VL53L1X is ready"));
    return;
  }

  if (sensor.init()==true) {
    snprintf_P(log_data, sizeof(log_data), S_LOG_I2C_FOUND_AT, "VL53L0X", sensor.getAddress());
    AddLog(LOG_LEVEL_DEBUG);
  } else {
    return;
  }

  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  vl53l0x_ready = 1;

  Vl53l0_index=0;

}

#define D_UNIT_MILLIMETER "mm"

#ifdef USE_WEBSERVER
const char HTTP_SNS_VL53L0X[] PROGMEM =
 "{s}VL53L0X " D_DISTANCE "{m}%d" D_UNIT_MILLIMETER "{e}"; // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif  // USE_WEBSERVER

#define USE_VL_MEDIAN

void Vl53l0Every_250MSecond() {
  uint16_t tbuff[5],tmp;
  uint8_t flag;

  if (!vl53l0x_ready) return;

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
  if (!vl53l0x_ready) {
    return;
  }

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

#define XSNS_45

bool Xsns45(byte function)
{
  bool result = false;

  if (i2c_flg) {
    switch (function) {
      case FUNC_INIT:
        Vl53l0Detect();
        break;
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
