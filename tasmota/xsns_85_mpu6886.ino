/*
  xsns_84_tof10120.ino -MPU6886/MPU9250 accelerometer support for Tasmota

  Copyright (C) 2021  Stephan Hadinger and Theo Arends

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
#if defined(USE_MPU6886) || defined(USE_MPU_ACCEL)    // USE_MPU6886 deprecated, prefer USE_MPU_ACCEL

#include <MPU_accel.h>
/*********************************************************************************************\
 * MPU6886/MPU9250
 * Internal chip found in M5Stack devices, using `Wire1` internal I2C bus
 *
 * I2C Address: 0x68
 *
\*********************************************************************************************/

#define XSNS_85                     85
#define XI2C_58                     58  // See I2CDEVICES.md

#define MPU_ACCEL_ADDRESS            0x68

struct {
  MPU_accel   Mpu;
  uint32_t  mpu_model;
  int16_t   ax=0, ay=0, az=0;  // accelerator data
  int16_t   gyx=0, gyy=0, gyz=0;  // accelerator data
  uint8_t   bus = 0;           // I2C bus
  bool      ready = false;
} mpu_sensor;

/********************************************************************************************/

const char HTTP_MPU6686[] PROGMEM =
 "{s}MPU%d acc_x" "{m}%3_f G" "{e}"
 "{s}MPU%d acc_y" "{m}%3_f G" "{e}"
 "{s}MPU%d acc_z" "{m}%3_f G" "{e}"
 "{s}MPU%d gyr_x" "{m}%i dps" "{e}"
 "{s}MPU%d gyr_y" "{m}%i dps" "{e}"
 "{s}MPU%d gyr_z" "{m}%i dps" "{e}"
 ;

void MPU_Show(uint32_t json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"MPU%d\":{\"AX\":%i,\"AY\":%i,\"AZ\":%i,\"GX\":%i,\"GY\":%i,\"GZ\":%i}"),
                          mpu_sensor.mpu_model,
                          mpu_sensor.ax, mpu_sensor.ay, mpu_sensor.az,
                          mpu_sensor.gyx, mpu_sensor.gyy, mpu_sensor.gyz);
  } else {
    float ax = mpu_sensor.ax / 1000.0f;
    float ay = mpu_sensor.ay / 1000.0f;
    float az = mpu_sensor.az / 1000.0f;
    WSContentSend_PD(HTTP_MPU6686, mpu_sensor.mpu_model, &ax, mpu_sensor.mpu_model, &ay, mpu_sensor.mpu_model, &az,
                                   mpu_sensor.mpu_model, mpu_sensor.gyx, mpu_sensor.mpu_model, mpu_sensor.gyy, mpu_sensor.mpu_model, mpu_sensor.gyz);

  }
}

void MPU_Detect(void) {
#ifdef ESP32
  if (!I2cSetDevice(MPU_ACCEL_ADDRESS, 0)) {
    if (!I2cSetDevice(MPU_ACCEL_ADDRESS, 1)) { return; }        // check on bus 1
    mpu_sensor.bus = 1;
    mpu_sensor.Mpu.setBus(mpu_sensor.bus);                      // switch to bus 1
  }
#else
  if (!I2cSetDevice(MPU_ACCEL_ADDRESS)) { return; }
#endif

  if (mpu_sensor.Mpu.Init() == 0) {
    char model_name[16];
    mpu_sensor.mpu_model = mpu_sensor.Mpu.getModel();
    snprintf_P(model_name, sizeof(model_name), PSTR("MPU%04d"), mpu_sensor.mpu_model);
    I2cSetActiveFound(MPU_ACCEL_ADDRESS, model_name, mpu_sensor.bus);
    mpu_sensor.ready = true;
  }
}

void MPU_Every_Second(void) {
  mpu_sensor.Mpu.getAccelDataInt(&mpu_sensor.ax, &mpu_sensor.ay, &mpu_sensor.az);
  mpu_sensor.Mpu.getGyroDataInt(&mpu_sensor.gyx, &mpu_sensor.gyy, &mpu_sensor.gyz);

  // AddLog(LOG_LEVEL_DEBUG, PSTR(">> Acc x=%i y=%i z=%i gx=%i gy=%i gz=%i"), mpu_sensor.ax, mpu_sensor.ay, mpu_sensor.az,
  //                                                                          mpu_sensor.gyx, mpu_sensor.gyy, mpu_sensor.gyz);

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns85(uint8_t function) {
  if (!I2cEnabled(XI2C_58)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MPU_Detect();
  }
  else if (mpu_sensor.ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MPU_Every_Second();
        break;
      case FUNC_JSON_APPEND:
        MPU_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MPU_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MPU6886 USE_MPU_ACCEL
#endif  // USE_I2C
