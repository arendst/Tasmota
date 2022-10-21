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
