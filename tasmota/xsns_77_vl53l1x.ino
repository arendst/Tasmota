/*
  xsns_77_vl53l1x_device[0].ino - VL53L1X sensor support for Tasmota

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

#define VL53LXX_MAX_SENSORS   VL53L0X_MAX_SENSORS
#define GPIO_VL53LXX_XSHUT1   GPIO_VL53L0X_XSHUT1

#include "VL53L1X.h"
VL53L1X vl53l1x_device[VL53LXX_MAX_SENSORS];

#define VL53LXX_ADDRESS 0x29
#ifndef VL53LXX_ADDRESS_MOVED
#define VL53LXX_ADDRESS_MOVED 0x78
#endif

struct {
  uint16_t distance = 0;
} vl53l1x_data[VL53LXX_MAX_SENSORS];

bool VL53L1X_xshut = false;
bool VL53L1X_detected = false;

/********************************************************************************************/

void Vl53l1Detect(void) {

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i)) {
      pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), OUTPUT);
      digitalWrite(Pin(GPIO_VL53LXX_XSHUT1, i), i==0 ? 1 : 0);
      VL53L1X_xshut = true;
    }
    else {
      break; // XSHUT indexes must be continuous. We stop at the 1st not defined one
    }
  }

  if (!I2cSetDevice(VL53LXX_ADDRESS)) { return; }
  if (!vl53l1x_device[0].init()) { return; }

  I2cSetActiveFound(vl53l1x_device[0].getAddress(), "VL53L1X");
  vl53l1x_device[0].setTimeout(500);
  vl53l1x_device[0].setDistanceMode(VL53L1X::Long); // could be Short, Medium, Long
  vl53l1x_device[0].setMeasurementTimingBudget(140000);
  vl53l1x_device[0].startContinuous(50);
  VL53L1X_detected = true;
}

void Vl53l1Every_250MSecond(void) {
  uint16_t dist = vl53l1x_device[0].read();
  if (!dist || dist > 4000) {
    dist = 9999;
  }
  vl53l1x_data[0].distance = dist;
}

#ifdef USE_DOMOTICZ
void Vl53l1Every_Second(void) {
  char distance[FLOATSZ];
  dtostrfd((float)vl53l1x_data[0].distance / 10, 1, distance);
  DomoticzSensor(DZ_ILLUMINANCE, distance);
}
#endif  // USE_DOMOTICZ

void Vl53l1Show(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"VL53L1X\":{\"" D_JSON_DISTANCE "\":%d}"), vl53l1x_data[0].distance);
#ifdef USE_DOMOTICZ
    if (0 == TasmotaGlobal.tele_period) {
      Vl53l1Every_Second();
    }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_DISTANCE, PSTR("VL53L1X"), vl53l1x_data[0].distance);
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
  else if (VL53L1X_detected) {
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
