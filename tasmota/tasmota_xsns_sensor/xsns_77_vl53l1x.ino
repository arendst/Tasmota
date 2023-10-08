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
 *********************************************************************************************
 *
 * Note: When using multiple VL53L0X, it is required to also wire the XSHUT pin of all those sensors
 * in order to let Tasmota change by software the I2C address of those and give them an unique address
 * for operation. The sensor don't save its address, so this procedure of changing its address is needed
 * to be performed every restart. The Addresses used for this are 120 (0x78) to 127 (0x7F). In the I2c
 * Standard (https://i2cdevices.org/addresses) those addresses are used by the PCA9685.
 * The base address (0x78) can be changed as a compile option with #define VL53L1X_XSHUT_ADDRESS 0xNN in
 * your user_config_override.h
 *
 * The default value of VL53LXX_MAX_SENSORS is set in the file tasmota.h
 * Changing that is backwards incompatible - Max supported devices by this driver are 8
 *********************************************************************************************
 * The following settings can be overriden
 *
 *
\*********************************************************************************************/

#define XSNS_77     77
#define XI2C_54     54  // See I2CDEVICES.md

#include "VL53L1X.h"

#define VL53L1X_ADDRESS 0x29
#ifndef VL53L1X_XSHUT_ADDRESS
#define VL53L1X_XSHUT_ADDRESS 0x78
#endif

#ifndef VL53L1X_DISTANCE_MODE
#define VL53L1X_DISTANCE_MODE Long
#endif

VL53L1X vl53l1x_device[VL53LXX_MAX_SENSORS];

struct {
  uint16_t distance = 0;
} vl53l1x_data[VL53LXX_MAX_SENSORS];

uint8_t VL53L1X_xshut = 0;
uint8_t VL53L1X_detected = 0;

/********************************************************************************************/

void Vl53l1Detect(void) {

  uint32_t i, xshut;
  for (i = 0, xshut = 1 ; i < VL53LXX_MAX_SENSORS ; i++, xshut <<= 1) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i)) {
      pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), OUTPUT);
      digitalWrite(Pin(GPIO_VL53LXX_XSHUT1, i), 0);
      VL53L1X_xshut |= xshut;
    }
  }

  for (i = 0, xshut = 1 ; i < VL53LXX_MAX_SENSORS ; i++, xshut <<= 1) {
    if (xshut & VL53L1X_xshut) {
      digitalWrite(Pin(GPIO_VL53LXX_XSHUT1, i), 1);
      delay(2);
    }
    if (!I2cSetDevice(VL53L1X_ADDRESS) && !I2cSetDevice((uint8_t)(VL53L1X_XSHUT_ADDRESS+i))) { continue; } // Detection for unconfigured OR configured sensor
    if (vl53l1x_device[i].init()) {
      if (VL53L1X_xshut) {
        vl53l1x_device[i].setAddress((uint8_t)(VL53L1X_XSHUT_ADDRESS+i));
      }
      uint8_t addr = vl53l1x_device[i].getAddress();
      vl53l1x_device[i].setTimeout(500);
      vl53l1x_device[i].setDistanceMode(VL53L1X::VL53L1X_DISTANCE_MODE); // could be Short, Medium, Long
      vl53l1x_device[i].setMeasurementTimingBudget(140000);
      vl53l1x_device[i].startContinuous(50);
      VL53L1X_detected |= xshut;

      if (VL53L1X_xshut) {
          I2cSetActive(addr);
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L1X-%d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);
      } else {
          I2cSetActiveFound(addr, "VL53L1X");
      }
    } // if init
    if (0 == VL53L1X_xshut) break;
  } // for
}

void Vl53l1Every_250MSecond(void) {
  uint32_t i, xshut;
  for (i = 0, xshut = 1; i < VL53LXX_MAX_SENSORS; i++, xshut <<= 1) {
    if (xshut & VL53L1X_detected) {
      uint16_t dist = vl53l1x_device[i].read();
      if (!dist || dist > 4000) {
        dist = 9999;
      }
      vl53l1x_data[i].distance = dist;
    } // if detected
    if (0 == VL53L1X_xshut) break;
  } // for
}

#ifdef USE_DOMOTICZ
void Vl53l1Every_Second(void) {
  float distance = (float)vl53l1x_data[0].distance / 10;  // cm
  DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
}
#endif  // USE_DOMOTICZ

void Vl53l1Show(bool json) {
  uint32_t i, xshut;
  for (i = 0, xshut = 1 ; i < VL53LXX_MAX_SENSORS ; i++, xshut <<= 1) {
    char types[12] = "VL53L1X";
    if (VL53L1X_xshut) {
      snprintf_P(types, sizeof(types), PSTR("VL53L1X%c%d"), IndexSeparator(), i +1);
    }
    float distance = (float)vl53l1x_data[i].distance / 10;  // cm
    if (xshut & VL53L1X_detected) {
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
#ifdef USE_DOMOTICZ
        if (0 == TasmotaGlobal.tele_period) {
          Vl53l1Every_Second();
        }
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
#endif
      }
    } // if detected
    if (0 == VL53L1X_xshut) break;
  } // for
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns77(uint32_t function) {
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
