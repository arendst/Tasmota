'''!
  @file DFRobot_MAX17043.py
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT License (MIT)
  @author [ouki.wang](ouki.wang@dfrobot.com)
  @version  V1.0
  @date  2018-4-14
  @url https://github.com/DFRobot/DFRobot_MAX17043
'''

import time

from machine import I2C, Pin

## Get I2C bus
i2c = I2C(scl = Pin(22), sda = Pin(21), freq=400000)

MAX17043_ADDR = 0x36
MAX17043_VCELL = 0x02
MAX17043_SOC = 0x04
MAX17043_MODE = 0x06
MAX17043_VERSION = 0x08
MAX17043_CONFIG = 0x0c
MAX17043_COMMAND = 0xfe

class DFRobot_MAX17043():
  
  def __init__(self):
    '''!
      @brief create MAX17043 object
      @return MAX17043 object
    '''
    pass
  
  def begin(self):
    '''!
      @brief MAX17043 begin and test moudle
      @return initialization result:
      @retval 0     successful
      @retval -1     faild
    '''
    self._write16(MAX17043_COMMAND, 0x5400)
    time.sleep(0.01)
    if self._read16(MAX17043_CONFIG) == 0x971c:
      self._write16(MAX17043_MODE, 0x4000)
      time.sleep(0.01)
      self._write16(MAX17043_CONFIG, 0x9700)
      return 0
    else:
      return -1
      
  def read_voltage(self):
    '''!
      @brief read battery voltage in mV
      @return voltage in mV
    '''
    return (1.25 * (self._read16(MAX17043_VCELL) >> 4))
  
  def read_percentage(self):
    '''!
      @brief read battery remaining capacity in percentage
      @return battery remaining capacity in percentage
    '''
    tmp = self._read16(MAX17043_SOC)
    return ((tmp >> 8) + 0.003906 * (tmp & 0x00ff))

  def set_Interrupt(self, per):
    '''!
      @brief set MAX17043 interrput threshold
      @param per       interrupt threshold as %1 - 32% (integer)
    '''
    if per > 32:
      per = 32
    elif per < 1:
      per = 1
    per = 32 - int(per)
    self._write_reg_bits(MAX17043_CONFIG, per, 0x01f, 0)

  def clear_interrupt(self):
    '''!
      @brief clear MAX17043 interrupt.
    '''
    self._write_reg_bits(MAX17043_CONFIG, 0, 0x01, 5)
    
  def set_sleep(self):
    '''!
      @brief set MAX17043 in sleep mode.
    '''
    self._write_reg_bits(MAX17043_CONFIG, 1, 0x01, 7)
    
  def set_wakeup(self):
    '''!
      @brief wake up MAX17043.
    '''
    self._write_reg_bits(MAX17043_CONFIG, 0, 0x01, 7)
  
  def _write16(self, reg, dat):
    buf = bytearray(2)
    buf[0] = dat >> 8
    buf[1] = dat & 0x00ff
    i2c.writeto_mem(MAX17043_ADDR, reg, buf)
    
  def _read16(self, reg):
    buf = i2c.readfrom_mem(MAX17043_ADDR, reg, 2)
    return ((buf[0] << 8) | buf[1])
  
  def _write_reg_bits(self, reg, dat, bits, offset):
    tmp = self._read16(reg)
    tmp = (tmp & (~(bits << offset))) | (dat << offset)
    self._write16(reg, tmp)

