/*!
 * @file Adafruit_SGP40.cpp
 *
 * @mainpage Adafruit SGP40 gas sensor driver
 *
 * @section intro_sec Introduction
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
 *
 * @section author Author
 * Written by Ladyada for Adafruit Industries.
 *
 * @section license License
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Adafruit_SGP40.h"
#include "Arduino.h"

//#define I2C_DEBUG

/*!
 *  @brief  Instantiates a new SGP40 class
 */
Adafruit_SGP40::Adafruit_SGP40() {}

/*!
 *  @brief  Setups the hardware and detects a valid SGP40. Initializes I2C
 *          then reads the serialnumber and checks that we are talking to an
 * SGP40
 *  @param  theWire
 *          Optional pointer to I2C interface, otherwise use Wire
 *  @return True if SGP40 found on I2C, False if something went wrong!
 */
boolean Adafruit_SGP40::begin(TwoWire *theWire) {
  if (i2c_dev) {
    delete i2c_dev; // remove old interface
  }

  i2c_dev = new Adafruit_I2CDevice(SGP40_I2CADDR_DEFAULT, theWire);

  if (!i2c_dev->begin()) {
    return false;
  }

  uint8_t command[2];
  command[0] = 0x36;
  command[1] = 0x82;
  if (!readWordFromCommand(command, 2, 10, serialnumber, 3))
    return false;

  uint16_t featureset;
  command[0] = 0x20;
  command[1] = 0x2F;
  if (!readWordFromCommand(command, 2, 10, &featureset, 1))
    return false;
  // Serial.print("Featureset 0x"); Serial.println(featureset, HEX);

  VocAlgorithm_init(&voc_algorithm_params);

  return selfTest();
}

/*!
 *   @brief Commands the sensor to perform a soft reset using the "General
 * Call" mode. Take note that this is not sensor specific and all devices that
 * support the General Call mode on the on the same I2C bus will perform this.
 *
 *   @return True if command completed successfully, false if something went
 *           wrong!
 */
boolean Adafruit_SGP40::softReset(void) {
  uint8_t command[2];
  command[0] = 0x00;
  command[1] = 0x06;
  return readWordFromCommand(command, 2, 10);
}

/**
 * @brief Request the sensor to turn off heater to lower curent consumption.
 *        Launching a measurement automatically wakes up the sensor again.
 *
 * @return true: success false: failure
 */
bool Adafruit_SGP40::heaterOff(void) {
  uint8_t command[2];
  command[0] = 0x36;
  command[1] = 0x15;
  return i2c_dev->write(command, 2);
}

/**
 * @brief Request the sensor to perform a self-test, returning the result
 *
 * @return true: success false:failure
 */
bool Adafruit_SGP40::selfTest(void) {
  uint8_t command[2];
  uint16_t reply;

  command[0] = 0x28;
  command[1] = 0x0E;

  if (!readWordFromCommand(command, 2, 250, &reply, 1))
    return false;
  if ((reply == 0xD400)) {
    return true;
  }
  return false;
}

/**
 * @brief Combined the measured gasses, temperature, and humidity
 * to calculate the VOC Index
 *
 * @param temperature The measured temperature in degrees C
 * @param humidity The measured relative humidity in % rH
 * @return int32_t The VOC Index
 */
int32_t Adafruit_SGP40::measureVocIndex(float temperature, float humidity) {
  int32_t voc_index;
  uint16_t sraw = measureRaw(temperature, humidity);

  VocAlgorithm_process(&voc_algorithm_params, sraw, &voc_index);
  return voc_index;
}

/**
 * @brief Return the raw gas measurement
 *
 * @param temperature The measured temperature in degrees C
 * @param humidity The measured relative humidity in % rH
 * @return uint16_t The current raw gas measurement
 */
uint16_t Adafruit_SGP40::measureRaw(float temperature, float humidity) {
  uint8_t command[8];
  uint16_t reply;

  command[0] = 0x26;
  command[1] = 0x0F;

  uint16_t rhticks = (uint16_t)((humidity * 65535) / 100 + 0.5);
  command[2] = rhticks >> 8;
  command[3] = rhticks & 0xFF;
  command[4] = generateCRC(command + 2, 2);
  uint16_t tempticks = (uint16_t)(((temperature + 45) * 65535) / 175);
  command[5] = tempticks >> 8;
  command[6] = tempticks & 0xFF;
  command[7] = generateCRC(command + 5, 2);
  ;

  if (!readWordFromCommand(command, 8, 250, &reply, 1))
    return 0x0;

  return reply;
}

/*!
 *  @brief  I2C low level interfacing
 */

bool Adafruit_SGP40::readWordFromCommand(uint8_t command[],
                                         uint8_t commandLength,
                                         uint16_t delayms, uint16_t *readdata,
                                         uint8_t readlen) {

  if (!i2c_dev->write(command, commandLength)) {
    return false;
  }

  delay(delayms);

  if (readlen == 0)
    return true;

  uint8_t replylen = readlen * (SGP40_WORD_LEN + 1);
  uint8_t replybuffer[replylen];

  if (!i2c_dev->read(replybuffer, replylen)) {
    return false;
  }

  for (uint8_t i = 0; i < readlen; i++) {
    uint8_t crc = generateCRC(replybuffer + i * 3, 2);
#ifdef I2C_DEBUG
    Serial.print("\t\tCRC calced: 0x");
    Serial.print(crc, HEX);
    Serial.print(" vs. 0x");
    Serial.println(replybuffer[i * 3 + 2], HEX);
#endif
    if (crc != replybuffer[i * 3 + 2])
      return false;
    // success! store it
    readdata[i] = replybuffer[i * 3];
    readdata[i] <<= 8;
    readdata[i] |= replybuffer[i * 3 + 1];
#ifdef I2C_DEBUG
    Serial.print("\t\tRead: 0x");
    Serial.println(readdata[i], HEX);
#endif
  }
  return true;
}

uint8_t Adafruit_SGP40::generateCRC(uint8_t *data, uint8_t datalen) {
  // calculates 8-Bit checksum with given polynomial
  uint8_t crc = SGP40_CRC8_INIT;

  for (uint8_t i = 0; i < datalen; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ SGP40_CRC8_POLYNOMIAL;
      else
        crc <<= 1;
    }
  }
  return crc;
}
