/*!
 * @file Adafruit_SGP30.cpp
 *
 * @mainpage Adafruit SGP30 gas sensor driver
 *
 * @section intro_sec Introduction
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
 *
 * @section author Author
 * Written by Ladyada for Adafruit Industries.
 *
 * @section license License
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Arduino.h"

#include "Adafruit_SGP30.h"
//#define I2C_DEBUG

/*!
 *  @brief  Instantiates a new SGP30 class
 */
Adafruit_SGP30::Adafruit_SGP30() {}

/*!
 *  @brief  Setups the hardware and detects a valid SGP30. Initializes I2C
 *          then reads the serialnumber and checks that we are talking to an
 * SGP30
 *  @param  theWire
 *          Optional pointer to I2C interface, otherwise use Wire
 *  @param  initSensor
 *          Optional pointer to prevent IAQinit to be called. Used for Deep
 *          Sleep.
 *  @return True if SGP30 found on I2C, False if something went wrong!
 */
boolean Adafruit_SGP30::begin(TwoWire *theWire, boolean initSensor) {
  _i2caddr = SGP30_I2CADDR_DEFAULT;
  _i2c = theWire;

  _i2c->begin();

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
  if ((featureset & 0xF0) != SGP30_FEATURESET)
    return false;
  if (initSensor) {
    if (!IAQinit())
      return false;
  }

  return true;
}

/*!
 *   @brief Commands the sensor to perform a soft reset using the "General
 * Call" mode. Take note that this is not sensor specific and all devices that
 * support the General Call mode on the on the same I2C bus will perform this.
 *
 *   @return True if command completed successfully, false if something went
 *           wrong!
 */
boolean Adafruit_SGP30::softReset(void) {
  uint8_t command[2];
  command[0] = 0x00;
  command[1] = 0x06;
  return readWordFromCommand(command, 2, 10);
}

/*!
 *   @brief  Commands the sensor to begin the IAQ algorithm. Must be called
 * after startup.
 *   @returns True if command completed successfully, false if something went
 *            wrong!
 */
boolean Adafruit_SGP30::IAQinit(void) {
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x03;
  return readWordFromCommand(command, 2, 10);
}

/*!
 *  @brief  Commands the sensor to take a single eCO2/VOC measurement. Places
 *          results in {@link TVOC} and {@link eCO2}
 *  @return True if command completed successfully, false if something went
 *          wrong!
 */
boolean Adafruit_SGP30::IAQmeasure(void) {
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x08;
  uint16_t reply[2];
  if (!readWordFromCommand(command, 2, 12, reply, 2))
    return false;
  TVOC = reply[1];
  eCO2 = reply[0];
  return true;
}

/*!
 *  @brief  Commands the sensor to take a single H2/ethanol raw measurement.
 * Places results in {@link rawH2} and {@link rawEthanol}
 *  @returns True if command completed successfully, false if something went
 * wrong!
 */
boolean Adafruit_SGP30::IAQmeasureRaw(void) {
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x50;
  uint16_t reply[2];
  if (!readWordFromCommand(command, 2, 25, reply, 2))
    return false;
  rawEthanol = reply[1];
  rawH2 = reply[0];
  return true;
}

/*!
 *   @brief  Request baseline calibration values for both CO2 and TVOC IAQ
 *           calculations. Places results in parameter memory locaitons.
 *   @param  eco2_base
 *           A pointer to a uint16_t which we will save the calibration
 *           value to
 *   @param  tvoc_base
 *           A pointer to a uint16_t which we will save the calibration value to
 *   @return True if command completed successfully, false if something went
 *           wrong!
 */
boolean Adafruit_SGP30::getIAQBaseline(uint16_t *eco2_base,
                                       uint16_t *tvoc_base) {
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x15;
  uint16_t reply[2];
  if (!readWordFromCommand(command, 2, 10, reply, 2))
    return false;
  *eco2_base = reply[0];
  *tvoc_base = reply[1];
  return true;
}

/*!
 *  @brief  Assign baseline calibration values for both CO2 and TVOC IAQ
 *          calculations.
 *  @param  eco2_base
 *          A uint16_t which we will save the calibration value from
 *  @param  tvoc_base
 *          A uint16_t which we will save the calibration value from
 *  @return True if command completed successfully, false if something went
 *          wrong!
 */
boolean Adafruit_SGP30::setIAQBaseline(uint16_t eco2_base, uint16_t tvoc_base) {
  uint8_t command[8];
  command[0] = 0x20;
  command[1] = 0x1e;
  command[2] = tvoc_base >> 8;
  command[3] = tvoc_base & 0xFF;
  command[4] = generateCRC(command + 2, 2);
  command[5] = eco2_base >> 8;
  command[6] = eco2_base & 0xFF;
  command[7] = generateCRC(command + 5, 2);

  return readWordFromCommand(command, 8, 10);
}

/*!
 *  @brief  Set the absolute humidity value [mg/m^3] for compensation to
 * increase precision of TVOC and eCO2.
 *  @param  absolute_humidity
 *          A uint32_t [mg/m^3] which we will be used for compensation.
 *          If the absolute humidity is set to zero, humidity compensation
 *          will be disabled.
 *  @return True if command completed successfully, false if something went
 *          wrong!
 */
boolean Adafruit_SGP30::setHumidity(uint32_t absolute_humidity) {
  if (absolute_humidity > 256000) {
    return false;
  }

  uint16_t ah_scaled =
      (uint16_t)(((uint64_t)absolute_humidity * 256 * 16777) >> 24);
  uint8_t command[5];
  command[0] = 0x20;
  command[1] = 0x61;
  command[2] = ah_scaled >> 8;
  command[3] = ah_scaled & 0xFF;
  command[4] = generateCRC(command + 2, 2);

  return readWordFromCommand(command, 5, 10);
}

/*!
 *  @brief  I2C low level interfacing
 */

boolean Adafruit_SGP30::readWordFromCommand(uint8_t command[],
                                            uint8_t commandLength,
                                            uint16_t delayms,
                                            uint16_t *readdata,
                                            uint8_t readlen) {

  _i2c->beginTransmission(_i2caddr);

#ifdef I2C_DEBUG
  Serial.print("\t\t-> ");
#endif

  for (uint8_t i = 0; i < commandLength; i++) {
    _i2c->write(command[i]);
#ifdef I2C_DEBUG
    Serial.print("0x");
    Serial.print(command[i], HEX);
    Serial.print(", ");
#endif
  }
#ifdef I2C_DEBUG
  Serial.println();
#endif
  _i2c->endTransmission();

  delay(delayms);

  if (readlen == 0)
    return true;

  uint8_t replylen = readlen * (SGP30_WORD_LEN + 1);
  if (_i2c->requestFrom(_i2caddr, replylen) != replylen)
    return false;
  uint8_t replybuffer[replylen];
#ifdef I2C_DEBUG
  Serial.print("\t\t<- ");
#endif
  for (uint8_t i = 0; i < replylen; i++) {
    replybuffer[i] = _i2c->read();
#ifdef I2C_DEBUG
    Serial.print("0x");
    Serial.print(replybuffer[i], HEX);
    Serial.print(", ");
#endif
  }

#ifdef I2C_DEBUG
  Serial.println();
#endif

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

uint8_t Adafruit_SGP30::generateCRC(uint8_t *data, uint8_t datalen) {
  // calculates 8-Bit checksum with given polynomial
  uint8_t crc = SGP30_CRC8_INIT;

  for (uint8_t i = 0; i < datalen; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ SGP30_CRC8_POLYNOMIAL;
      else
        crc <<= 1;
    }
  }
  return crc;
}
