/*!
 * @file DFRobot_MAX17043.h
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [ouki.wang](ouki.wang@dfrobot.com)
 * @version  V1.0
 * @date  2018-4-14
 * @url https://github.com/DFRobot/DFRobot_MAX17043
 */

#ifndef __DFROBOT_MAX17043_H
#define __DFROBOT_MAX17043_H

#include "Arduino.h"
#include "Wire.h"

#define _DBG        0

#if(_DBG)
  #define _DBG_CODE(x)      x
#else
  #define _DBG_CODE(x)
#endif

#define MAX17043_ADDRESS        0x36

#define MAX17043_VCELL          0x02
#define MAX17043_SOC            0x04
#define MAX17043_MODE           0x06
#define MAX17043_VERSION        0x08
#define MAX17043_CONFIG         0x0c
#define MAX17043_COMMAND        0xfe

class DFRobot_MAX17043 {
public:
  /**
   * @fn DFRobot_MAX17043
   * @brief create MAX17043 object
   * @return MAX17043 object
   */
  DFRobot_MAX17043();
  /**
   * @fn begin
   * @brief MAX17043 begin and test moudle
   *
   * @return initialization result
   * @retval  0     successful
   * @retval -1     faild
   */
  int         begin();
  /**
   * @fn readVoltage
   * @brief read battery voltage in mV
   * @return voltage in mV
   */
  float       readVoltage();
  /**
   * @fn readPercentage
   * @brief read battery remaining capacity in percentage
   *
   * @return battery remaining capacity in percentage
   */
  float       readPercentage();
  /**
   * @fn setInterrupt
   * @brief set MAX17043 interrput threshold
   *
   * @param per       interrupt threshold as %1 - 32% (integer)
   */
  void        setInterrupt(uint8_t per);
  /**
   * @fn clearInterrupt
   * @brief clear MAX17043 interrupt
   */
  void        clearInterrupt();
  /**
   * @fn setSleep
   * @brief set MAX17043 in sleep mode
   *
   */
  void        setSleep();
  /**
   * @fn setWakeUp
   * @brief weak up MAX17043
   *
   */
  void        setWakeUp();

  private:
    void write16(uint8_t reg, uint16_t dat) {
      Wire.begin();
      Wire.beginTransmission(MAX17043_ADDRESS);
      Wire.write(reg);
      Wire.write(dat >> 8);
      Wire.write(dat);
      Wire.endTransmission();
    }

    uint16_t read16(uint8_t reg) {
      uint16_t        temp;
      Wire.begin();
      Wire.beginTransmission(MAX17043_ADDRESS);
      Wire.write(reg);
      Wire.endTransmission();
      Wire.requestFrom(MAX17043_ADDRESS, 2);
      temp = (uint16_t)Wire.read() << 8;
      temp |= (uint16_t)Wire.read();
      Wire.endTransmission();
      return temp;
    }

    void writeRegBits(uint8_t reg, uint16_t dat, uint16_t bits, uint8_t offset) {
      uint16_t        temp;
      temp = read16(reg);
      temp = (temp & (~(bits << offset))) | (dat << offset);
      write16(reg, temp);
    }
};

#endif
