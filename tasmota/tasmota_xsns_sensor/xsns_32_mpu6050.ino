/*
  xsns_32_mpu6050.ino - MPU6050 gyroscope and temperature sensor support for Tasmota

  Copyright (C) 2021  Oliver Welter

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
#ifdef USE_MPU6050
/*********************************************************************************************\
 * MPU6050 3 axis gyroscope and temperature sensor
 *
 * Source: Oliver Welter, with special thanks to Jeff Rowberg
 *
 * I2C Address: 0x68 or 0x69 with AD0 HIGH
\*********************************************************************************************/

#define XSNS_32                          32
#define XI2C_25                          25  // See I2CDEVICES.md

#define D_SENSOR_MPU6050                 "MPU6050"

#define MPU_6050_ADDR_AD0_LOW            0x68
#define MPU_6050_ADDR_AD0_HIGH           0x69

uint8_t MPU_6050_address;
uint8_t MPU_6050_addresses[] = { MPU_6050_ADDR_AD0_LOW, MPU_6050_ADDR_AD0_HIGH };
uint8_t MPU_6050_found;

int16_t MPU_6050_ax = 0, MPU_6050_ay = 0, MPU_6050_az = 0;
int16_t MPU_6050_gx = 0, MPU_6050_gy = 0, MPU_6050_gz = 0;
int16_t MPU_6050_temperature = 0;

#ifdef USE_MPU6050_DMP
  #include "MPU6050_6Axis_MotionApps20.h"
  #include "I2Cdev.h"
  #include <helper_3dmath.h>
  typedef struct MPU6050_DMP{
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float euler[3];         // [psi, theta, phi]    Euler angle container
  float yawPitchRoll[3];  // [yaw, pitch roll]    Yaw-pitch-roll container
  } MPU6050_DMP;

  MPU6050_DMP MPU6050_dmp;
#else
  #include <MPU6050.h>
#endif //USE_MPU6050_DMP
MPU6050 mpu6050;

void MPU_6050PerformReading(void)
{
#ifdef USE_MPU6050_DMP
    mpu6050.resetFIFO(); // with a default sampling rate of 200Hz, we create a delay of approx. 5ms with a complete read cycle
    MPU6050_dmp.fifoCount = mpu6050.getFIFOCount();
    while (MPU6050_dmp.fifoCount < MPU6050_dmp.packetSize) MPU6050_dmp.fifoCount = mpu6050.getFIFOCount();
    mpu6050.getFIFOBytes(MPU6050_dmp.fifoBuffer, MPU6050_dmp.packetSize);
    MPU6050_dmp.fifoCount -= MPU6050_dmp.packetSize;
    // calculate euler and acceleration with the DMP
    mpu6050.dmpGetQuaternion(&MPU6050_dmp.q, MPU6050_dmp.fifoBuffer);
    mpu6050.dmpGetEuler(MPU6050_dmp.euler, &MPU6050_dmp.q);
    mpu6050.dmpGetAccel(&MPU6050_dmp.aa, MPU6050_dmp.fifoBuffer);
    mpu6050.dmpGetGravity(&MPU6050_dmp.gravity, &MPU6050_dmp.q);
    mpu6050.dmpGetLinearAccel(&MPU6050_dmp.aaReal, &MPU6050_dmp.aa, &MPU6050_dmp.gravity);
    mpu6050.dmpGetYawPitchRoll(MPU6050_dmp.yawPitchRoll, &MPU6050_dmp.q, &MPU6050_dmp.gravity);
    MPU_6050_gx = MPU6050_dmp.euler[0] * 180/M_PI;
    MPU_6050_gy = MPU6050_dmp.euler[1] * 180/M_PI;
    MPU_6050_gz = MPU6050_dmp.euler[2] * 180/M_PI;
    MPU_6050_ax = MPU6050_dmp.aaReal.x;
    MPU_6050_ay = MPU6050_dmp.aaReal.y;
    MPU_6050_az = MPU6050_dmp.aaReal.z;
#else
  mpu6050.getMotion6(
    &MPU_6050_ax,
    &MPU_6050_ay,
    &MPU_6050_az,
    &MPU_6050_gx,
    &MPU_6050_gy,
    &MPU_6050_gz
  );
#endif //USE_MPU6050_DMP
  MPU_6050_temperature = mpu6050.getTemperature();
}

/* Work in progress - not yet fully functional
void MPU_6050SetGyroOffsets(int x, int y, int z)
{
  mpu050.setXGyroOffset(x);
  mpu6050.setYGyroOffset(y);
  mpu6050.setZGyroOffset(z);
}

void MPU_6050SetAccelOffsets(int x, int y, int z)
{
  mpu6050.setXAccelOffset(x);
  mpu6050.setYAccelOffset(y);
  mpu6050.setZAccelOffset(z);
}
*/

void MPU_6050Detect(void)
{
  for (uint32_t i = 0; i < sizeof(MPU_6050_addresses); i++)
  {
    MPU_6050_address = MPU_6050_addresses[i];
    if (!I2cSetDevice(MPU_6050_address)) { break; }
    mpu6050.setAddr(MPU_6050_addresses[i]);

#ifdef USE_MPU6050_DMP
    MPU6050_dmp.devStatus = mpu6050.dmpInitialize();
    mpu6050.setXGyroOffset(220);
    mpu6050.setYGyroOffset(76);
    mpu6050.setZGyroOffset(-85);
    mpu6050.setZAccelOffset(1788);
    if (MPU6050_dmp.devStatus == 0) {
      mpu6050.setDMPEnabled(true);
      MPU6050_dmp.packetSize = mpu6050.dmpGetFIFOPacketSize();
      MPU_6050_found = true;
    }
#else
    mpu6050.initialize();
    MPU_6050_found = mpu6050.testConnection();
#endif //USE_MPU6050_DMP
    Settings->flag2.axis_resolution = 2;  // Need to be services by command Sensor32
  }

  if (MPU_6050_found) {
    I2cSetActiveFound(MPU_6050_address, D_SENSOR_MPU6050);
  }
}

#define D_YAW "Yaw"
#define D_PITCH "Pitch"
#define D_ROLL "Roll"

#ifdef USE_WEBSERVER
const char HTTP_SNS_AXIS[] PROGMEM =
  "{s}" D_SENSOR_MPU6050 " " D_AX_AXIS "{m}%s{e}"                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_AY_AXIS "{m}%s{e}"                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_AZ_AXIS "{m}%s{e}"                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_GX_AXIS "{m}%s{e}"                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_GY_AXIS "{m}%s{e}"                              // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_GZ_AXIS "{m}%s{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#ifdef USE_MPU6050_DMP
const char HTTP_SNS_YPR[] PROGMEM =
  "{s}" D_SENSOR_MPU6050 " " D_YAW "{m}%s{e}"                                  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_PITCH "{m}%s{e}"                                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
  "{s}" D_SENSOR_MPU6050 " " D_ROLL "{m}%s{e}";                                // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif // USE_MPU6050_DMP
#endif // USE_WEBSERVER

#define D_JSON_AXIS_AX "AccelXAxis"
#define D_JSON_AXIS_AY "AccelYAxis"
#define D_JSON_AXIS_AZ "AccelZAxis"
#define D_JSON_AXIS_GX "GyroXAxis"
#define D_JSON_AXIS_GY "GyroYAxis"
#define D_JSON_AXIS_GZ "GyroZAxis"
#define D_JSON_YAW "Yaw"
#define D_JSON_PITCH "Pitch"
#define D_JSON_ROLL "Roll"

void MPU_6050Show(bool json)
{
  MPU_6050PerformReading();

  float tempConv = ConvertTemp(MPU_6050_temperature / 340.0 + 35.53);
  char axis_ax[33];
  dtostrfd(MPU_6050_ax, Settings->flag2.axis_resolution, axis_ax);
  char axis_ay[33];
  dtostrfd(MPU_6050_ay, Settings->flag2.axis_resolution, axis_ay);
  char axis_az[33];
  dtostrfd(MPU_6050_az, Settings->flag2.axis_resolution, axis_az);
  char axis_gx[33];
  dtostrfd(MPU_6050_gx, Settings->flag2.axis_resolution, axis_gx);
  char axis_gy[33];
  dtostrfd(MPU_6050_gy, Settings->flag2.axis_resolution, axis_gy);
  char axis_gz[33];
  dtostrfd(MPU_6050_gz, Settings->flag2.axis_resolution, axis_gz);
#ifdef USE_MPU6050_DMP
  char axis_yaw[33];
  dtostrfd(MPU6050_dmp.yawPitchRoll[0] / PI * 180.0, Settings->flag2.axis_resolution, axis_yaw);
  char axis_pitch[33];
  dtostrfd(MPU6050_dmp.yawPitchRoll[1] / PI * 180.0, Settings->flag2.axis_resolution, axis_pitch);
  char axis_roll[33];
  dtostrfd(MPU6050_dmp.yawPitchRoll[2] / PI * 180.0, Settings->flag2.axis_resolution, axis_roll);
#endif // USE_MPU6050_DMP

  if (json) {
    char json_axis_ax[25];
    snprintf_P(json_axis_ax, sizeof(json_axis_ax), PSTR(",\"" D_JSON_AXIS_AX "\":%s"), axis_ax);
    char json_axis_ay[25];
    snprintf_P(json_axis_ay, sizeof(json_axis_ay), PSTR(",\"" D_JSON_AXIS_AY "\":%s"), axis_ay);
    char json_axis_az[25];
    snprintf_P(json_axis_az, sizeof(json_axis_az), PSTR(",\"" D_JSON_AXIS_AZ "\":%s"), axis_az);
    char json_axis_gx[25];
    snprintf_P(json_axis_gx, sizeof(json_axis_gx), PSTR(",\"" D_JSON_AXIS_GX "\":%s"), axis_gx);
    char json_axis_gy[25];
    snprintf_P(json_axis_gy, sizeof(json_axis_gy), PSTR(",\"" D_JSON_AXIS_GY "\":%s"), axis_gy);
    char json_axis_gz[25];
    snprintf_P(json_axis_gz, sizeof(json_axis_gz), PSTR(",\"" D_JSON_AXIS_GZ "\":%s"), axis_gz);
#ifdef USE_MPU6050_DMP
    char json_ypr_y[25];
    snprintf_P(json_ypr_y, sizeof(json_ypr_y), PSTR(",\"" D_JSON_YAW "\":%s"), axis_yaw);
    char json_ypr_p[25];
    snprintf_P(json_ypr_p, sizeof(json_ypr_p), PSTR(",\"" D_JSON_PITCH "\":%s"), axis_pitch);
    char json_ypr_r[25];
    snprintf_P(json_ypr_r, sizeof(json_ypr_r), PSTR(",\"" D_JSON_ROLL "\":%s"), axis_roll);
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s%s%s%s%s%s%s%s%s}"),
      D_SENSOR_MPU6050, Settings->flag2.temperature_resolution, &tempConv, json_axis_ax, json_axis_ay, json_axis_az, json_axis_gx, json_axis_gy, json_axis_gz,
      json_ypr_y, json_ypr_p, json_ypr_r);
#else
    ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_TEMPERATURE "\":%*_f%s%s%s%s%s%s}"),
      D_SENSOR_MPU6050, Settings->flag2.temperature_resolution, &tempConv, json_axis_ax, json_axis_ay, json_axis_az, json_axis_gx, json_axis_gy, json_axis_gz);
#endif // USE_MPU6050_DMP
#ifdef USE_DOMOTICZ
    DomoticzFloatSensor(DZ_TEMP, tempConv);
#endif // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_Temp(D_SENSOR_MPU6050, tempConv);
    WSContentSend_PD(HTTP_SNS_AXIS, axis_ax, axis_ay, axis_az, axis_gx, axis_gy, axis_gz);
#ifdef USE_MPU6050_DMP
    WSContentSend_PD(HTTP_SNS_YPR, axis_yaw, axis_pitch, axis_roll);
#endif // USE_MPU6050_DMP
#endif // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns32(uint32_t function)
{
  if (!I2cEnabled(XI2C_25)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    MPU_6050Detect();
  }
  else if (MPU_6050_found) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        if (TasmotaGlobal.tele_period == Settings->tele_period -3) {
          MPU_6050PerformReading();
        }
        break;
      case FUNC_JSON_APPEND:
        MPU_6050Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MPU_6050Show(0);
        MPU_6050PerformReading();
        break;
#endif // USE_WEBSERVER
    }
  }
  return result;
}

#endif // USE_MPU6050
#endif // USE_I2C
