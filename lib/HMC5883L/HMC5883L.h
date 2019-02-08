/*
HMC5883L magnetometer library

Brice Dubost

GPL v2 or higher
 */

#ifndef HMC5883L_H
#define HMC5883L_H

#include <inttypes.h>
#include <Wire.h>
#include <Arduino.h>


#define MAGaddr 0x1E //0011110b, I2C 7bit address of HMC5883
#define DefaultTimeout 100
#define DefaultGain 1
//Default 8 avg
#define DefaultAverage 3
//Default 15Hz
#define DefaultRate 4

//Mag field
struct MagMes
{
int16_t MX;
int16_t MY;
int16_t MZ;
float f_MX;
float f_MY;
float f_MZ;
};

//Conversion factors for the various gains
const uint16_t Magconv[]={ 1370, 1090, 820, 660, 440, 390, 330, 230 };


#define DefaultRange 2

#define CONF_REG_A   0x00
#define CONF_REG_B   0x01
#define MODE_REG     0x02
#define MAG_X_MSB    0x03
#define STATUS_REG   0x09
#define ID_A_REG     0x0A
#define ID_B_REG     0x0B
#define ID_C_REG     0x0C



class HMC5883L {
 public:
  HMC5883L();

  void setgain(uint8_t usergain);
  void setAddr(uint8_t usergain);
  void setaverage(uint8_t useraverage);
  void setrate(uint8_t userrate);
  uint8_t init();
  uint8_t read_data();
  uint8_t sleep();
  uint8_t unsleep();
  void convert();

  uint8_t isconnected;
  MagMes meas;

 private:
  uint8_t address;
  uint8_t gain;
  uint8_t average;
  uint8_t rate;
  uint8_t timeout;
  uint8_t waitavail(uint8_t numbytes);
  uint8_t check_conn();

};


#endif
