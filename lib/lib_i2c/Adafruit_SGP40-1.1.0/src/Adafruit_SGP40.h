/*!
 * @file Adafruit_SGP40.h
 *
 * This is the documentation for Adafruit's SGP40 driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit SGP40 breakout: http://www.adafruit.com/products/4829
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

#ifndef ADAFRUIT_SGP40_H
#define ADAFRUIT_SGP40_H

#include "Arduino.h"
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
extern "C" {
#include "sensirion_arch_config.h"
#include "sensirion_voc_algorithm.h"
};

// the i2c address
#define SGP40_I2CADDR_DEFAULT 0x59 ///< SGP40 has only one I2C address

// commands and constants
#define SGP40_FEATURESET 0x0020    ///< The required set for this library
#define SGP40_CRC8_POLYNOMIAL 0x31 ///< Seed for SGP40's CRC polynomial
#define SGP40_CRC8_INIT 0xFF       ///< Init value for CRC
#define SGP40_WORD_LEN 2           ///< 2 bytes per word

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          SGP40 Gas Sensor
 */
class Adafruit_SGP40 {
public:
  Adafruit_SGP40();
  bool begin(TwoWire *theWire = &Wire);
  bool selfTest(void);

  bool softReset();
  bool heaterOff();
  uint16_t measureRaw(float temperature = 25, float humidity = 50);
  int32_t measureVocIndex(float temperature = 25, float humidity = 50);

  /** The 48-bit serial number, this value is set when you call {@link begin()}
   * **/
  uint16_t serialnumber[3];

private:
  Adafruit_I2CDevice *i2c_dev = NULL; ///< Pointer to I2C bus interface
  void write(uint8_t address, uint8_t *data, uint8_t n);
  void read(uint8_t address, uint8_t *data, uint8_t n);
  bool readWordFromCommand(uint8_t command[], uint8_t commandLength,
                           uint16_t delayms, uint16_t *readdata = NULL,
                           uint8_t readlen = 0);
  uint8_t generateCRC(uint8_t data[], uint8_t datalen);

  VocAlgorithmParams voc_algorithm_params;
};
#endif // ndef ADAFRUIT_SGP40_H
