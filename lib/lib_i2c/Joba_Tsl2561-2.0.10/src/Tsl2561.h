/*
Copyright: Joachim Banzhaf, 2018

This file is part of the Joba_Tsl2561 Library.

    Joba_Tsl2561 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Joba_Tsl2561 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Joba_Tsl2561.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Tsl2561_H
#define Tsl2561_H

#include <Arduino.h>
#include <Wire.h>

// To be able to override pin definitions in build flags (used in examples)
#ifndef TSL2561_SDA
  #define TSL2561_SDA SDA
#endif
#ifndef TSL2561_SCL
  #define TSL2561_SCL SCL
#endif

class Tsl2561 {

public:

  typedef enum {
    ADDR_NONE  = 0b0000000,
    ADDR_GND   = 0b0101001,
    ADDR_FLOAT = 0b0111001,
    ADDR_VDD   = 0b1001001
  } address_t;

  typedef enum : uint8_t {
    REG_CONTROL,        // Control of basic functions
    REG_TIMING,         // Integration time/gain control
    REG_THRESHLOWLOW,   // Low byte of low interrupt threshold
    REG_THRESHLOWHIGH,  // High byte of low interrupt threshold
    REG_THRESHHIGHLOW,  // Low byte of high interrupt threshold
    REG_THRESHHIGHHIGH, // High byte of high interrupt threshold
    REG_INTERRUPT,      // Interrupt control
    REG_RESERVED1,
    REG_CRC,            // Factory test - not a user register
    REG_RESERVED2,
    REG_ID,             // Part number/Rev ID
    REG_RESERVED3,
    REG_DATA0LOW,       // Low byte of ADC channel 0
    REG_DATA0HIGH,      // High byte of ADC channel 0
    REG_DATA1LOW,       // Low byte of ADC channel 1
    REG_DATA1HIGH       // High byte of ADC channel 1
  } register_t;

  enum : uint8_t{
    CONTROL_CMD     = 0b10000000,
    CONTROL_CLEAR   = 0b01000000,
    CONTROL_WORD    = 0b00100000, // SPI only?
    CONTROL_BLOCK   = 0b00010000, // SPI only?
    CONTROL_ADDRESS = 0b00001111
  };

  enum {
    POWER_OFF,
    POWER_ON = 0b11,
  };

  enum {
    GAIN_OFF,
    GAIN_ON = 0b00010000
  };

  typedef enum {
    EXP_ON  = 0b00001011,
    EXP_OFF = 0b00000011,
    EXP_14  = 0b00000000,
    EXP_101 = 0b00000001,
    EXP_402 = 0b00000010
  } exposure_t;

  enum {
    PKG_ID      = 0b11110000,
    PKG_REV     = 0b00001111,
    PKG_CS      = 0b00010000,
    PKG_T_FN_CL = 0b01010000
  };

  typedef enum {
    ERR_OK      = 0,
    ERR_GENERAL = 1,
    ERR_GONE    = 2,
    ERR_RW      = 3,
    ERR_BUSY    = 4
  } status_t;

  Tsl2561( TwoWire &wire );

  bool begin( address_t addr );
  bool begin();

  bool available();
  status_t status() const;
  address_t address() const;

  bool id( uint8_t &id );
  static uint8_t type( uint8_t id );
  static uint8_t revision( uint8_t id );
  static bool packageCS( uint8_t id );
  static bool packageT_FN_CL( uint8_t id );

  bool on();
  bool off();
  bool setSensitivity( bool gain, exposure_t exposure );
  bool getSensitivity( bool &gain, exposure_t &exposure );
  bool fullLuminosity( uint16_t &luminosity );
  bool irLuminosity( uint16_t &luminosity );

protected:
  bool readByte( register_t reg, uint8_t &val );
  bool readWord( register_t reg, uint16_t &val );
  bool writeByte( register_t reg, uint8_t val );

private:
  Tsl2561();
  Tsl2561( Tsl2561 & );

  address_t _addr;
  TwoWire &_wire;
  status_t _status;
};

#endif
