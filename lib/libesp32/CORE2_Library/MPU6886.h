/*
 Note: The MPU6886 is an I2C sensor and uses the Arduino Wire library.
 Because the sensor is not 5V tolerant, we are using a 3.3 V 8 MHz Pro Mini or
 a 3.3 V Teensy 3.1. We have disabled the internal pull-ups used by the Wire
 library in the Wire.h/twi.c utility file. We are also using the 400 kHz fast
 I2C mode by setting the TWI_FREQ  to 400000L /twi.h utility file.
 */
#ifndef _MPU6886_H_
#define _MPU6886_H_

#include <Wire.h>
#include <Arduino.h>
#include "MahonyAHRS.h"

#define MPU6886_ADDRESS           0x68 
#define MPU6886_WHOAMI            0x75
#define MPU6886_ACCEL_INTEL_CTRL  0x69
#define MPU6886_SMPLRT_DIV        0x19
#define MPU6886_INT_PIN_CFG       0x37
#define MPU6886_INT_ENABLE        0x38
#define MPU6886_ACCEL_XOUT_H      0x3B
#define MPU6886_ACCEL_XOUT_L      0x3C
#define MPU6886_ACCEL_YOUT_H      0x3D
#define MPU6886_ACCEL_YOUT_L      0x3E
#define MPU6886_ACCEL_ZOUT_H      0x3F
#define MPU6886_ACCEL_ZOUT_L      0x40

#define MPU6886_TEMP_OUT_H        0x41
#define MPU6886_TEMP_OUT_L        0x42

#define MPU6886_GYRO_XOUT_H       0x43
#define MPU6886_GYRO_XOUT_L       0x44
#define MPU6886_GYRO_YOUT_H       0x45
#define MPU6886_GYRO_YOUT_L       0x46
#define MPU6886_GYRO_ZOUT_H       0x47
#define MPU6886_GYRO_ZOUT_L       0x48

#define MPU6886_USER_CTRL         0x6A
#define MPU6886_PWR_MGMT_1        0x6B
#define MPU6886_PWR_MGMT_2        0x6C
#define MPU6886_CONFIG            0x1A
#define MPU6886_GYRO_CONFIG       0x1B
#define MPU6886_ACCEL_CONFIG      0x1C
#define MPU6886_ACCEL_CONFIG2     0x1D
#define MPU6886_FIFO_EN           0x23

//#define G (9.8)
#define RtA     57.324841
#define AtR    	0.0174533	
#define Gyro_Gr	0.0010653

class MPU6886 {
    public:
      enum Ascale {
        AFS_2G = 0,
        AFS_4G,
        AFS_8G,
        AFS_16G
      };

      enum Gscale {
        GFS_250DPS = 0,
        GFS_500DPS,
        GFS_1000DPS,
        GFS_2000DPS
      };

      Gscale Gyscale = GFS_2000DPS;
      Ascale Acscale = AFS_8G;
    public:
      MPU6886();
      int Init(void);
      void getAccelAdc(int16_t* ax, int16_t* ay, int16_t* az);
      void getGyroAdc(int16_t* gx, int16_t* gy, int16_t* gz);
      void getTempAdc(int16_t *t);

      void getAccelData(float* ax, float* ay, float* az);
      void getGyroData(float* gx, float* gy, float* gz);
      void getTempData(float *t);

      void SetGyroFsr(Gscale scale);
      void SetAccelFsr(Ascale scale);

      void getAhrsData(float *pitch,float *roll,float *yaw);

    public:
      float aRes, gRes;

    private:

    private:
      void I2C_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer);
      void I2C_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer);
      void getGres();
      void getAres();

};
#endif