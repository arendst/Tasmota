/*!
 *  @file Adafruit_MCP9808.cpp
 *
 *  @mainpage Adafruit MCP9808 I2C Temp Sensor
 *
 *  @section intro_sec Introduction
 *
 * 	I2C Driver for Microchip's MCP9808 I2C Temp sensor
 *
 * 	This is a library for the Adafruit MCP9808 breakout:
 * 	http://www.adafruit.com/products/1782
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  @section author Author
 *
 *  K.Townsend (Adafruit Industries)
 *
 * 	@section license License
 *
 * 	BSD (see license.txt)
 *
 * 	@section  HISTORY
 *
 *     v1.0 - First release
 *
 * changes by Martin Wagner for tasmota project:
 *  
 * - the libary supports variabel I2C address
 *
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "Adafruit_MCP9808.h"

/*!
 *    @brief  Instantiates a new MCP9808 class
 */
Adafruit_MCP9808::Adafruit_MCP9808() {}

/*!
 *    @brief  Setups the HW
 *    @param  *theWire
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin(TwoWire *theWire) {
  _wire = theWire;
  _i2caddr = MCP9808_I2CADDR_DEFAULT;
  return init();
}

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin(uint8_t addr) {
  _i2caddr = addr;
  _wire = &Wire;
  return init();
}

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @param  *theWire
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin(uint8_t addr, TwoWire *theWire) {
  _i2caddr = addr;
  _wire = theWire;
  return init();
}

/*!
 *    @brief  Setups the HW with default address
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::begin() {
  _i2caddr = MCP9808_I2CADDR_DEFAULT;
  _wire = &Wire;
  return init();
}

/*!
 *    @brief  init function
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_MCP9808::init() {
  _wire->begin();

  if (read16(MCP9808_REG_MANUF_ID) != 0x0054)
    return false;
  if (read16(MCP9808_REG_DEVICE_ID) != 0x0400)
    return false;

  write16(MCP9808_REG_CONFIG, 0x0);
  return true;
}

/*!
 *   @brief  Reads the 16-bit temperature register and returns the Centigrade
 *           temperature as a float.
 *   @return Temperature in Centigrade.
 */
float Adafruit_MCP9808::readTempC(uint8_t addr) {
   _i2caddr = addr;
  float temp = NAN;
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  if (t != 0xFFFF) {
    temp = t & 0x0FFF;
    temp /= 16.0;
    if (t & 0x1000)
      temp -= 256;
  }

  return temp;
}

/*!
 *   @brief  Reads the 16-bit temperature register and returns the Fahrenheit
 *           temperature as a float.
 *   @return Temperature in Fahrenheit.
 */
float Adafruit_MCP9808::readTempF(uint8_t addr) {
    _i2caddr = addr;
  float temp = NAN;
  uint16_t t = read16(MCP9808_REG_AMBIENT_TEMP);

  if (t != 0xFFFF) {
    temp = t & 0x0FFF;
    temp /= 16.0;
    if (t & 0x1000)
      temp -= 256;

    temp = temp * 9.0 / 5.0 + 32;
  }

  return temp;
}

/*!
 *   @brief  Set Sensor to Shutdown-State or wake up (Conf_Register BIT8)
 *   @param  sw true = shutdown / false = wakeup
 */
void Adafruit_MCP9808::shutdown_wake(uint8_t addr, boolean sw) {
  _i2caddr = addr;
  uint16_t conf_shutdown;
  uint16_t conf_register = read16(MCP9808_REG_CONFIG);
  if (sw == true) {
    conf_shutdown = conf_register | MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
  if (sw == false) {
    conf_shutdown = conf_register & ~MCP9808_REG_CONFIG_SHUTDOWN;
    write16(MCP9808_REG_CONFIG, conf_shutdown);
  }
}

/*!
 *   @brief  Shutdown MCP9808
 */
void Adafruit_MCP9808::shutdown(uint8_t addr) { shutdown_wake(addr, true); }

/*!
 *   @brief  Wake up MCP9808
 */
void Adafruit_MCP9808::wake(uint8_t addr) {
  shutdown_wake(addr, false);
  delay(250);
}

/*!
 *   @brief  Get Resolution Value
 *   @return Resolution value
 */
uint8_t Adafruit_MCP9808::getResolution(uint8_t addr) {
     _i2caddr = addr;
  return read8(MCP9808_REG_RESOLUTION);
}

/*!
 *   @brief  Set Resolution Value
 *   @param  value
 */
void Adafruit_MCP9808::setResolution(uint8_t addr, uint8_t value) {
   _i2caddr = addr;
  write8(MCP9808_REG_RESOLUTION, value & 0x03);
}

/*!
 *    @brief  Low level 16 bit write procedures
 *    @param  reg
 *    @param  value
 */
void Adafruit_MCP9808::write16(uint8_t reg, uint16_t value) {
  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->write(value >> 8);
  _wire->write(value & 0xFF);
  _wire->endTransmission();
}

/*!
 *    @brief  Low level 16 bit read procedure
 *    @param  reg
 *    @return value
 */
uint16_t Adafruit_MCP9808::read16(uint8_t reg) {
  uint16_t val = 0xFFFF;
  uint8_t state;

  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  state = _wire->endTransmission();

  if (state == 0) {
    _wire->requestFrom((uint8_t)_i2caddr, (uint8_t)2);
    val = _wire->read();
    val <<= 8;
    val |= _wire->read();
  }

  return val;
}

/*!
 *    @brief  Low level 8 bit write procedure
 *    @param  reg
 *    @param  value
 */
void Adafruit_MCP9808::write8(uint8_t reg, uint8_t value) {
  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->write(value);
  _wire->endTransmission();
}

/*!
 *    @brief  Low level 8 bit read procedure
 *    @param  reg
 *    @return value
 */
uint8_t Adafruit_MCP9808::read8(uint8_t reg) {
  uint8_t val = 0xFF;
  uint8_t state;

  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  state = _wire->endTransmission();

  if (state == 0) {
    _wire->requestFrom((uint8_t)_i2caddr, (uint8_t)1);
    val = _wire->read();
  }

  return val;
}
