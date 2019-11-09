/**************************************************************************/
/*! 
    @file     Adafruit_TSL2591.h
    @author   KT0WN (adafruit.com)

    This is a library for the Adafruit TSL2591 breakout board
    This library works with the Adafruit TSL2591 breakout 
    ----> https://www.adafruit.com/products/1980
	
    Check out the links above for our tutorials and wiring diagrams 
    These chips use I2C to communicate
 
    Adafruit invests time and resources providing this open source code, 
    please support Adafruit and open-source hardware by purchasing 
    products from Adafruit!
*/
/**************************************************************************/

#ifndef _TSL2591_H_
#define _TSL2591_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

#define TSL2591_VISIBLE           (2)       ///< (channel 0) - (channel 1)
#define TSL2591_INFRARED          (1)       ///< channel 1
#define TSL2591_FULLSPECTRUM      (0)       ///< channel 0

#define TSL2591_ADDR              (0x29)    ///< Default I2C address

#define TSL2591_COMMAND_BIT       (0xA0)    ///< 1010 0000: bits 7 and 5 for 'command normal'

///! Special Function Command for "Clear ALS and no persist ALS interrupt"
#define TSL2591_CLEAR_INT         (0xE7)
///! Special Function Command for "Interrupt set - forces an interrupt"
#define TSL2591_TEST_INT          (0xE4)


#define TSL2591_WORD_BIT          (0x20)    ///< 1 = read/write word (rather than byte)
#define TSL2591_BLOCK_BIT         (0x10)    ///< 1 = using block read/write

#define TSL2591_ENABLE_POWEROFF   (0x00)    ///< Flag for ENABLE register to disable
#define TSL2591_ENABLE_POWERON    (0x01)    ///< Flag for ENABLE register to enable
#define TSL2591_ENABLE_AEN        (0x02)    ///< ALS Enable. This field activates ALS function. Writing a one activates the ALS. Writing a zero disables the ALS.
#define TSL2591_ENABLE_AIEN       (0x10)    ///< ALS Interrupt Enable. When asserted permits ALS interrupts to be generated, subject to the persist filter.
#define TSL2591_ENABLE_NPIEN      (0x80)    ///< No Persist Interrupt Enable. When asserted NP Threshold conditions will generate an interrupt, bypassing the persist filter

#define TSL2591_LUX_DF            (408.0F) ///< Lux cooefficient
#define TSL2591_LUX_COEFB         (1.64F)  ///< CH0 coefficient 
#define TSL2591_LUX_COEFC         (0.59F)  ///< CH1 coefficient A
#define TSL2591_LUX_COEFD         (0.86F)  ///< CH2 coefficient B

/// Enumeration for the sensor integration timing
typedef enum
{
  TSL2591_INTEGRATIONTIME_100MS     = 0x00,  // 100 millis
  TSL2591_INTEGRATIONTIME_200MS     = 0x01,  // 200 millis
  TSL2591_INTEGRATIONTIME_300MS     = 0x02,  // 300 millis
  TSL2591_INTEGRATIONTIME_400MS     = 0x03,  // 400 millis
  TSL2591_INTEGRATIONTIME_500MS     = 0x04,  // 500 millis
  TSL2591_INTEGRATIONTIME_600MS     = 0x05,  // 600 millis
}
tsl2591IntegrationTime_t;

/// Enumeration for the sensor gain
typedef enum
{
  TSL2591_GAIN_LOW                  = 0x00,    /// low gain (1x)
  TSL2591_GAIN_MED                  = 0x10,    /// medium gain (25x)
  TSL2591_GAIN_HIGH                 = 0x20,    /// medium gain (428x)
  TSL2591_GAIN_MAX                  = 0x30,    /// max gain (9876x)
}
tsl2591Gain_t;


/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with TSL2591 Light Sensor
*/
/**************************************************************************/
class Adafruit_TSL2591
{
 public:
  Adafruit_TSL2591();
  
  boolean   begin   ( void );
  void      enable  ( void );
  void      disable ( void );

  float     calculateLux  ( uint16_t ch0, uint16_t ch1 );
  void      setGain       ( tsl2591Gain_t gain );
  void      setTiming     ( tsl2591IntegrationTime_t integration );
  uint16_t  getLuminosity (uint8_t channel );
  uint32_t  getFullLuminosity ( );

  tsl2591IntegrationTime_t getTiming();
  tsl2591Gain_t            getGain();

 private:
  void      write8  ( uint8_t r);
  void      write8  ( uint8_t r, uint8_t v );
  uint16_t  read16  ( uint8_t reg );
  uint8_t   read8   ( uint8_t reg );

  tsl2591IntegrationTime_t _integration;
  tsl2591Gain_t _gain;

  boolean _initialized;
};
#endif
