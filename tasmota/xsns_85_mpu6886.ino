/*
  xsns_84_tof10120.ino - TOF10120 sensor support for Tasmota

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
#ifdef USE_MPU6886

#include <MPU6886.h>
/*********************************************************************************************\
 * MPU6886
 * Internal chip found in M5Stack devices, using `Wire1` internal I2C bus
 *
 * I2C Address: 0x68
 *
\*********************************************************************************************/

#define XSNS_85                     85
#define XI2C_58                     58  // See I2CDEVICES.md

#define MPU6886_ADDRESS            0x68

struct {
  MPU6886   Mpu;
  bool      ready = false;
  int16_t   ax=0, ay=0, az=0;  // accelerator data
  int16_t   gyx=0, gyy=0, gyz=0;  // accelerator data
} mpu6886_sensor;

/********************************************************************************************/

const char HTTP_MPU6686[] PROGMEM =
 "{s}MPU6886 acc_x" "{m}%3_f G" "{e}"
 "{s}MPU6886 acc_y" "{m}%3_f G" "{e}"
 "{s}MPU6886 acc_z" "{m}%3_f G" "{e}"
 "{s}MPU6886 gyr_x" "{m}%i dps" "{e}"
 "{s}MPU6886 gyr_y" "{m}%i dps" "{e}"
 "{s}MPU6886 gyr_z" "{m}%i dps" "{e}"
 ;

void MPU6686_Show(uint32_t json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"MPU6886\":{\"AX\":%i,\"AY\":%i,\"AZ\":%i,\"GX\":%i,\"GY\":%i,\"GZ\":%i}"),
                          mpu6886_sensor.ax, mpu6886_sensor.ay, mpu6886_sensor.az,
                          mpu6886_sensor.gyx, mpu6886_sensor.gyy, mpu6886_sensor.gyz);
  } else {
    float ax = mpu6886_sensor.ax / 1000.0f;
    float ay = mpu6886_sensor.ay / 1000.0f;
    float az = mpu6886_sensor.az / 1000.0f;
    WSContentSend_PD(HTTP_MPU6686, &ax, &ay, &az,
                                   mpu6886_sensor.gyx, mpu6886_sensor.gyy, mpu6886_sensor.gyz);

  }
}

void MPU6686Detect(void) {
#ifdef ESP32
  if (!I2cSetDevice(MPU6886_ADDRESS, 0)) {
    if (!I2cSetDevice(MPU6886_ADDRESS, 1)) { return; }        // check on bus 1
    mpu6886_sensor.Mpu.setBus(1);                             // switch to bus 1
    I2cSetActiveFound(MPU6886_ADDRESS, "MPU6886", 1);
  } else {
    I2cSetActiveFound(MPU6886_ADDRESS, "MPU6886", 0);
  }
#else
  if (!I2cSetDevice(MPU6886_ADDRESS)) { return; }
  I2cSetActiveFound(MPU6886_ADDRESS, "MPU6886");
#endif

  mpu6886_sensor.Mpu.Init();
  mpu6886_sensor.ready = true;
}

void MPU6886Every_Second(void) {
  mpu6886_sensor.Mpu.getAccelDataInt(&mpu6886_sensor.ax, &mpu6886_sensor.ay, &mpu6886_sensor.az);
  mpu6886_sensor.Mpu.getGyroDataInt(&mpu6886_sensor.gyx, &mpu6886_sensor.gyy, &mpu6886_sensor.gyz);

  // AddLog(LOG_LEVEL_DEBUG, PSTR(">> Acc x=%i y=%i z=%i gx=%i gy=%i gz=%i"), mpu6886_sensor.ax, mpu6886_sensor.ay, mpu6886_sensor.az,
  //                                                                          mpu6886_sensor.gyx, mpu6886_sensor.gyy, mpu6886_sensor.gyz);

}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns85(uint8_t function) {
  if (!I2cEnabled(XI2C_58)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MPU6686Detect();
  }
  else if (mpu6886_sensor.ready) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MPU6886Every_Second();
        break;
      case FUNC_JSON_APPEND:
        MPU6686_Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MPU6686_Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_MPU6886
#endif  // USE_I2C