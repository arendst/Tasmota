/*
  xsns_77_vl53l1x.ino - VL53L1X sensor support for Tasmota

  Copyright (C) 2021  Theo Arends, Rui Marinho and Johann Obermeier

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
 * I2C Address: 0x29
\*********************************************************************************************/

#define XSNS_77     77
#define XI2C_54     54  // See I2CDEVICES.md

#include "VL53L1X.h"
VL53L1X vl53l1x = VL53L1X(); // create object copy

#define VL53L1X_ADDRESS 0x29

struct {
  uint16_t distance = 0;
  bool ready = false;
} vl53l1x_sensors;

/********************************************************************************************/

void Vl53l1Detect(void) {
  if (!I2cSetDevice(VL53L1X_ADDRESS)) { return; }
  if (!vl53l1x.init()) { return; }

  I2cSetActiveFound(vl53l1x.getAddress(), "VL53L1X");
  vl53l1x.setTimeout(500);
  vl53l1x.setDistanceMode(VL53L1X::Long); // could be Short, Medium, Long
  vl53l1x.setMeasurementTimingBudget(140000);
  vl53l1x.startContinuous(50);
  vl53l1x_sensors.ready = true;
}

void Vl53l1Every_250MSecond(void) {
  uint16_t dist = vl53l1x.read();
  if (!dist || dist > 4000) {
    dist = 9999;
  }
  vl53l1x_sensors.distance = dist;
}

#ifdef USE_DOMOTICZ
void Vl53l1Every_Second(void) {
  char distance[FLOATSZ];
  dtostrfd((float)vl53l1x_sensors.distance / 10, 1, distance);
  DomoticzSensor(DZ_ILLUMINANCE, distance);
}
#endif  // USE_DOMOTICZ

void Vl53l1Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"VL53L1X\":{\"" D_JSON_DISTANCE "\":%d}"), vl53l1x_sensors.distance);
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      Vl53l1Every_Second();
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_DISTANCE, PSTR("VL53L1X"), vl53l1x_sensors.distance);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns77(uint8_t function) {
  if (!I2cEnabled(XI2C_54)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Vl53l1Detect();
  }
  else if (vl53l1x_sensors.ready) {
    switch (function) {
      case FUNC_EVERY_250_MSECOND:
        Vl53l1Every_250MSecond();
        break;
#ifdef USE_DOMOTICZ
     case FUNC_EVERY_SECOND:
        Vl53l1Every_Second();
        break;
#endif  // USE_DOMOTICZ
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
