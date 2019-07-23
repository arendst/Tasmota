/*!
 * @file Adafruit_SGP30.h
 *
 * This is the documentation for Adafruit's SGP30 driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit SGP30 breakout: http://www.adafruit.com/products/3709
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Ladyada for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
*/

#include "Arduino.h"
#include <Wire.h>

// the i2c address
#define SGP30_I2CADDR_DEFAULT 0x58     ///< SGP30 has only one I2C address

// commands and constants
#define SGP30_FEATURESET       0x0020  ///< The required set for this library
#define SGP30_CRC8_POLYNOMIAL  0x31    ///< Seed for SGP30's CRC polynomial
#define SGP30_CRC8_INIT        0xFF    ///< Init value for CRC
#define SGP30_WORD_LEN         2       ///< 2 bytes per word

/**************************************************************************/
/*!  Class that stores state and functions for interacting with SGP30 Gas Sensor */
/**************************************************************************/
class Adafruit_SGP30 {
 public:
  Adafruit_SGP30();
  boolean begin(TwoWire *theWire = NULL);
  boolean IAQinit(void);
  boolean IAQmeasure(void);

  boolean getIAQBaseline(uint16_t *eco2_base, uint16_t *tvoc_base);
  boolean setIAQBaseline(uint16_t eco2_base, uint16_t tvoc_base);
  boolean setHumidity(uint32_t absolute_humidity);

  /**
   * The last measurement of the IAQ-calculated Total Volatile Organic Compounds in ppb. This value is set when you call {@link IAQmeasure()}
   */
  uint16_t TVOC;

  /**
   * The last measurement of the IAQ-calculated equivalent CO2 in ppm. This value is set when you call {@link IAQmeasure()}
   */
  uint16_t eCO2;

  /**
   * The 48-bit serial number, this value is set when you call {@link begin()}
   */
  uint16_t serialnumber[3];
 private:
  TwoWire *_i2c;
  uint8_t _i2caddr;

  void write(uint8_t address, uint8_t *data, uint8_t n);
  void read(uint8_t address, uint8_t *data, uint8_t n);
  boolean readWordFromCommand(uint8_t command[], uint8_t commandLength, uint16_t delay, uint16_t *readdata = NULL, uint8_t readlen = 0);
  uint8_t generateCRC(uint8_t data[], uint8_t datalen);
};
