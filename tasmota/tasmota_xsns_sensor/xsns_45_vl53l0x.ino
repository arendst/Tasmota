/*
  xsns_45_vl53l0x.ino - VL53L0X time of flight multiple sensors support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz and Adrian Scillato

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
 *********************************************************************************************
 *
 * Note: When using multiple VL53L0X, it is required to also wire the XSHUT pin of all those sensors
 * in order to let Tasmota change by software the I2C address of those and give them an unique address
 * for operation. The sensor don't save its address, so this procedure of changing its address is needed
 * to be performed every restart. The Addresses used for this are 120 (0x78) to 127 (0x7F). In the I2c
 * Standard (https://i2cdevices.org/addresses) those addresses are used by the PCA9685, so take into
 * account they won't work together.
 *
 * The default value of VL53LXX_MAX_SENSORS is set in the file tasmota.h
 * Changing that is backwards incompatible - Max supported devices by this driver are 8
 *
 **********************************************************************************************
 *
 * How to install this sensor: https://www.st.com/resource/en/datasheet/vl53l0x.pdf
 *
 * If you are going to use long I2C wires read this:
 * https://hackaday.com/2017/02/08/taking-the-leap-off-board-an-introduction-to-i2c-over-long-wires/
 *
\*********************************************************************************************/

#define XSNS_45            45
#define XI2C_31            31  // See I2CDEVICES.md

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define VL53L0X_LONG_RANGE

// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define VL53L0X_HIGH_SPEED
//#define VL53L0X_HIGH_ACCURACY

#define USE_VL_MEDIAN
#define USE_VL_MEDIAN_SIZE 5   // Odd number of samples median detection

#include <Wire.h>
#include "VL53L0X.h"

#define VL53L0X_ADDRESS 0x29
#ifndef VL53L0X_XSHUT_ADDRESS
#define VL53L0X_XSHUT_ADDRESS 0x78
#endif

VL53L0X VL53L0X_device[VL53LXX_MAX_SENSORS];
struct {
  uint16_t distance;
  uint16_t distance_prev;
  uint16_t buffer[5];
  bool ready = false;
  uint8_t index;
} Vl53l0x_data[VL53LXX_MAX_SENSORS];

bool VL53L0X_xshut = false;
bool VL53L0X_detected = false;

#ifdef USE_DEEPSLEEP
bool VL53L0X_standby = false;  // Prevent updating measurments once VL53L0X has been put to standby (just before ESP enters deepsleep)
#endif

/********************************************************************************************/

void Vl53l0Detect(void) {

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i)) {
      pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), OUTPUT);
      digitalWrite(Pin(GPIO_VL53LXX_XSHUT1, i), i==0 ? 1 : 0);
      VL53L0X_xshut = true;
    }
  }

  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        if (VL53L0X_xshut) { pinMode(Pin(GPIO_VL53LXX_XSHUT1, i), INPUT); delay(1); }
        if (!I2cSetDevice(VL53L0X_ADDRESS) && !I2cSetDevice((uint8_t)(VL53L0X_XSHUT_ADDRESS+i))) { return; } // Detection for unconfigured OR configured sensor
        if (VL53L0X_device[i].init()) {
            if (VL53L0X_xshut) { VL53L0X_device[i].setAddress((uint8_t)(VL53L0X_XSHUT_ADDRESS+i)); }
            uint8_t addr = VL53L0X_device[i].getAddress();
            if (VL53L0X_xshut) {
                I2cSetActive(addr);
                AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d " D_SENSOR_DETECTED " - " D_NEW_ADDRESS " 0x%02X"), i+1, addr);
            } else {
                I2cSetActiveFound(addr, "VL53L0X");
            }
            VL53L0X_device[i].setTimeout(500);

#if defined VL53L0X_LONG_RANGE
            // lower the return signal rate limit (default is 0.25 MCPS)
            VL53L0X_device[i].setSignalRateLimit(0.1);
            // increase laser pulse periods (defaults are 14 and 10 PCLKs)
            VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
            VL53L0X_device[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif
#if defined VL53L0X_HIGH_SPEED
            // reduce timing budget to 20 ms (default is about 33 ms)
            VL53L0X_device[i].setMeasurementTimingBudget(20000);
#elif defined VL53L0X_HIGH_ACCURACY
            // increase timing budget to 200 ms
            VL53L0X_device[i].setMeasurementTimingBudget(200000);
#endif
            // Start continuous back-to-back mode (take readings as
            // fast as possible).  To use continuous timed mode
            // instead, provide a desired inter-measurement period in
            // ms (e.g. sensor.startContinuous(100)).
            VL53L0X_device[i].startContinuous();

            Vl53l0x_data[i].ready = true;
            Vl53l0x_data[i].index = 0;
            VL53L0X_detected = true;
            if (!VL53L0X_xshut) { break; }
        } else {
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C D_SENSOR " VL53L0X %d - " D_FAILED_TO_START), i+1);
        }
    }
  }
}

void Vl53l0Every_250MSecond(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
  if (VL53L0X_standby) return;
#endif
  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        uint16_t dist = VL53L0X_device[i].readRangeContinuousMillimeters();
        if ((0 == dist) || (dist > 2200)) {
            dist = 9999;
        }

#ifdef USE_VL_MEDIAN
        // store in ring buffer
        Vl53l0x_data[i].buffer[Vl53l0x_data[i].index] = dist;
        Vl53l0x_data[i].index++;
        if (Vl53l0x_data[i].index >= USE_VL_MEDIAN_SIZE) {
            Vl53l0x_data[i].index = 0;
        }

        // sort list and take median
        uint16_t tbuff[USE_VL_MEDIAN_SIZE];
        memmove(tbuff, Vl53l0x_data[i].buffer, sizeof(tbuff));
        uint16_t tmp;
        uint8_t flag;
        for (uint32_t ocnt = 0; ocnt < USE_VL_MEDIAN_SIZE; ocnt++) {
            flag = 0;
            for (uint32_t count = 0; count < USE_VL_MEDIAN_SIZE -1; count++) {
            if (tbuff[count] > tbuff[count +1]) {
                tmp = tbuff[count];
                tbuff[count] = tbuff[count +1];
                tbuff[count +1] = tmp;
                flag = 1;
            }
            }
            if (!flag) { break; }
        }
        Vl53l0x_data[i].distance = tbuff[(USE_VL_MEDIAN_SIZE -1) / 2];
#else
        Vl53l0x_data[i].distance = dist;
#endif
    }
    if (!VL53L0X_xshut) { break; }
  }
}

#ifdef USE_DOMOTICZ
void Vl53l0Every_Second(void) {
#ifdef USE_DEEPSLEEP
  // Prevent updating measurments once VL53L0X has been put to sleep (just before ESP enters deepsleep)
  if (VL53L0X_standby) return;
#endif
  if (abs(Vl53l0x_data[0].distance - Vl53l0x_data[0].distance_prev) > 8) {
    Vl53l0x_data[0].distance_prev = Vl53l0x_data[0].distance;
    float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
    DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
  }
}
#endif  // USE_DOMOTICZ

void Vl53l0Show(boolean json) {
  for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
    char types[12] = "VL53L0X";
    if (VL53L0X_xshut) {
      snprintf_P(types, sizeof(types), PSTR("VL53L0X%c%d"), IndexSeparator(), i +1);
    }
    if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
      float distance = (Vl53l0x_data[i].distance == 9999) ? NAN : (float)Vl53l0x_data[i].distance / 10;  // cm
      if (json) {
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_DISTANCE "\":%1_f}"), types, &distance);
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_PD(HTTP_SNS_F_DISTANCE_CM, types, &distance);
#endif
      }
    }
    if (VL53L0X_device[i].timeoutOccurred()) {
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_I2C "Timeout waiting for %s"), types);
    }
    if (!VL53L0X_xshut) { break; }
  }
#ifdef USE_DOMOTICZ
  if (json && (0 == TasmotaGlobal.tele_period)){
    float distance = (float)Vl53l0x_data[0].distance / 10;  // cm
    DomoticzFloatSensor(DZ_ILLUMINANCE, distance);
  }
#endif  // USE_DOMOTICZ
}

#ifdef USE_DEEPSLEEP

void VL53L0EnterStandby(void) {
  if (DeepSleepEnabled()) {
    for (uint32_t i = 0; i < VL53LXX_MAX_SENSORS; i++) {
      if (PinUsed(GPIO_VL53LXX_XSHUT1, i) || (!VL53L0X_xshut)) {
        if (Vl53l0x_data[i].ready) {
          // VL53L0X_device[i].stopContinuous();
          // Calling stopContinuous() does not lead to a stable standby state.
          // The current is approx. 300 µA, but should be much lower.
          // Restart is bumpy and sometimes blocks the startup sequence completely.
          VL53L0X_device[i].init();
          Vl53l0x_data[i].ready = false;
        }
      }
    }
    VL53L0X_standby = true;
  }
}

#endif // USE_DEEPSLEEP

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns45(uint32_t function) {
  if (!I2cEnabled(XI2C_31)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Vl53l0Detect();
  }
  else if (VL53L0X_detected) {
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
#ifdef USE_DEEPSLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        VL53L0EnterStandby();
        break;
#endif // USE_DEEPSLEEP
    }
  }
  return result;
}

#endif  // USE_VL53L0X
#endif  // USE_I2C
