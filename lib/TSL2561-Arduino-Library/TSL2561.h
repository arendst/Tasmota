
/**************************************************************************/
/*! 
    @file     tsl2561.h
    @author   K. Townsend (microBuilder.eu)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/

#ifndef _TSL2561_H_
#define _TSL2561_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

#define TSL2561_VISIBLE 2                   // channel 0 - channel 1
#define TSL2561_INFRARED 1                  // channel 1
#define TSL2561_FULLSPECTRUM 0              // channel 0

// 3 i2c address options!
#define TSL2561_ADDR_LOW  0x29
#define TSL2561_ADDR_FLOAT 0x39
#define TSL2561_ADDR_HIGH 0x49

// Lux calculations differ slightly for CS package
//#define TSL2561_PACKAGE_CS
#define TSL2561_PACKAGE_T_FN_CL

#define TSL2561_READBIT           (0x01)

#define TSL2561_COMMAND_BIT       (0x80)    // Must be 1
#define TSL2561_CLEAR_BIT         (0x40)    // Clears any pending interrupt (write 1 to clear)
#define TSL2561_WORD_BIT          (0x20)    // 1 = read/write word (rather than byte)
#define TSL2561_BLOCK_BIT         (0x10)    // 1 = using block read/write

#define TSL2561_CONTROL_POWERON   (0x03)
#define TSL2561_CONTROL_POWEROFF  (0x00)

#define TSL2561_LUX_LUXSCALE      (14)      // Scale by 2^14
#define TSL2561_LUX_RATIOSCALE    (9)       // Scale ratio by 2^9
#define TSL2561_LUX_CHSCALE       (10)      // Scale channel values by 2^10
#define TSL2561_LUX_CHSCALE_TINT0 (0x7517)  // 322/11 * 2^TSL2561_LUX_CHSCALE
#define TSL2561_LUX_CHSCALE_TINT1 (0x0FE7)  // 322/81 * 2^TSL2561_LUX_CHSCALE

// T, FN and CL package values
#define TSL2561_LUX_K1T           (0x0040)  // 0.125 * 2^RATIO_SCALE
#define TSL2561_LUX_B1T           (0x01f2)  // 0.0304 * 2^LUX_SCALE
#define TSL2561_LUX_M1T           (0x01be)  // 0.0272 * 2^LUX_SCALE
#define TSL2561_LUX_K2T           (0x0080)  // 0.250 * 2^RATIO_SCALE
#define TSL2561_LUX_B2T           (0x0214)  // 0.0325 * 2^LUX_SCALE
#define TSL2561_LUX_M2T           (0x02d1)  // 0.0440 * 2^LUX_SCALE
#define TSL2561_LUX_K3T           (0x00c0)  // 0.375 * 2^RATIO_SCALE
#define TSL2561_LUX_B3T           (0x023f)  // 0.0351 * 2^LUX_SCALE
#define TSL2561_LUX_M3T           (0x037b)  // 0.0544 * 2^LUX_SCALE
#define TSL2561_LUX_K4T           (0x0100)  // 0.50 * 2^RATIO_SCALE
#define TSL2561_LUX_B4T           (0x0270)  // 0.0381 * 2^LUX_SCALE
#define TSL2561_LUX_M4T           (0x03fe)  // 0.0624 * 2^LUX_SCALE
#define TSL2561_LUX_K5T           (0x0138)  // 0.61 * 2^RATIO_SCALE
#define TSL2561_LUX_B5T           (0x016f)  // 0.0224 * 2^LUX_SCALE
#define TSL2561_LUX_M5T           (0x01fc)  // 0.0310 * 2^LUX_SCALE
#define TSL2561_LUX_K6T           (0x019a)  // 0.80 * 2^RATIO_SCALE
#define TSL2561_LUX_B6T           (0x00d2)  // 0.0128 * 2^LUX_SCALE
#define TSL2561_LUX_M6T           (0x00fb)  // 0.0153 * 2^LUX_SCALE
#define TSL2561_LUX_K7T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B7T           (0x0018)  // 0.00146 * 2^LUX_SCALE
#define TSL2561_LUX_M7T           (0x0012)  // 0.00112 * 2^LUX_SCALE
#define TSL2561_LUX_K8T           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B8T           (0x0000)  // 0.000 * 2^LUX_SCALE
#define TSL2561_LUX_M8T           (0x0000)  // 0.000 * 2^LUX_SCALE

// CS package values
#define TSL2561_LUX_K1C           (0x0043)  // 0.130 * 2^RATIO_SCALE
#define TSL2561_LUX_B1C           (0x0204)  // 0.0315 * 2^LUX_SCALE
#define TSL2561_LUX_M1C           (0x01ad)  // 0.0262 * 2^LUX_SCALE
#define TSL2561_LUX_K2C           (0x0085)  // 0.260 * 2^RATIO_SCALE
#define TSL2561_LUX_B2C           (0x0228)  // 0.0337 * 2^LUX_SCALE
#define TSL2561_LUX_M2C           (0x02c1)  // 0.0430 * 2^LUX_SCALE
#define TSL2561_LUX_K3C           (0x00c8)  // 0.390 * 2^RATIO_SCALE
#define TSL2561_LUX_B3C           (0x0253)  // 0.0363 * 2^LUX_SCALE
#define TSL2561_LUX_M3C           (0x0363)  // 0.0529 * 2^LUX_SCALE
#define TSL2561_LUX_K4C           (0x010a)  // 0.520 * 2^RATIO_SCALE
#define TSL2561_LUX_B4C           (0x0282)  // 0.0392 * 2^LUX_SCALE
#define TSL2561_LUX_M4C           (0x03df)  // 0.0605 * 2^LUX_SCALE
#define TSL2561_LUX_K5C           (0x014d)  // 0.65 * 2^RATIO_SCALE
#define TSL2561_LUX_B5C           (0x0177)  // 0.0229 * 2^LUX_SCALE
#define TSL2561_LUX_M5C           (0x01dd)  // 0.0291 * 2^LUX_SCALE
#define TSL2561_LUX_K6C           (0x019a)  // 0.80 * 2^RATIO_SCALE
#define TSL2561_LUX_B6C           (0x0101)  // 0.0157 * 2^LUX_SCALE
#define TSL2561_LUX_M6C           (0x0127)  // 0.0180 * 2^LUX_SCALE
#define TSL2561_LUX_K7C           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B7C           (0x0037)  // 0.00338 * 2^LUX_SCALE
#define TSL2561_LUX_M7C           (0x002b)  // 0.00260 * 2^LUX_SCALE
#define TSL2561_LUX_K8C           (0x029a)  // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B8C           (0x0000)  // 0.000 * 2^LUX_SCALE
#define TSL2561_LUX_M8C           (0x0000)  // 0.000 * 2^LUX_SCALE

enum
{
  TSL2561_REGISTER_CONTROL          = 0x00,
  TSL2561_REGISTER_TIMING           = 0x01,
  TSL2561_REGISTER_THRESHHOLDL_LOW  = 0x02,
  TSL2561_REGISTER_THRESHHOLDL_HIGH = 0x03,
  TSL2561_REGISTER_THRESHHOLDH_LOW  = 0x04,
  TSL2561_REGISTER_THRESHHOLDH_HIGH = 0x05,
  TSL2561_REGISTER_INTERRUPT        = 0x06,
  TSL2561_REGISTER_CRC              = 0x08,
  TSL2561_REGISTER_ID               = 0x0A,
  TSL2561_REGISTER_CHAN0_LOW        = 0x0C,
  TSL2561_REGISTER_CHAN0_HIGH       = 0x0D,
  TSL2561_REGISTER_CHAN1_LOW        = 0x0E,
  TSL2561_REGISTER_CHAN1_HIGH       = 0x0F
};

typedef enum
{
  TSL2561_INTEGRATIONTIME_13MS      = 0x00,    // 13.7ms
  TSL2561_INTEGRATIONTIME_101MS     = 0x01,    // 101ms
  TSL2561_INTEGRATIONTIME_402MS     = 0x02     // 402ms
}
tsl2561IntegrationTime_t;

typedef enum
{
  TSL2561_GAIN_0X                   = 0x00,    // No gain
  TSL2561_GAIN_16X                  = 0x10,    // 16x gain
}
tsl2561Gain_t;


class TSL2561 {
 public:
  TSL2561(uint8_t addr);
  boolean begin(void);
  void enable(void);
  void disable(void);
  void write8(uint8_t r, uint8_t v);
  uint16_t read16(uint8_t reg);

  uint32_t calculateLux(uint16_t ch0, uint16_t ch1);
  void setTiming(tsl2561IntegrationTime_t integration);
  void setGain(tsl2561Gain_t gain);
  uint16_t getLuminosity (uint8_t channel);
  uint32_t getFullLuminosity ();

 private:
  int8_t _addr;
  tsl2561IntegrationTime_t _integration;
  tsl2561Gain_t _gain;

  boolean _initialized;
};
#endif
