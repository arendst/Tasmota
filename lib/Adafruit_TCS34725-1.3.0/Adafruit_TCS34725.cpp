/*!
 *  @file Adafruit_TCS34725.cpp
 *
 *  @mainpage Driver for the TCS34725 digital color sensors.
 *
 *  @section intro_sec Introduction
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  @section author Author
 *
 *  KTOWN (Adafruit Industries)
 *
 *  @section license License
 *
 *  BSD (see license.txt)
 *
 *  @section HISTORY
 *
 *  v1.0 - First release
 */
#ifdef __AVR
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif
#include <math.h>
#include <stdlib.h>

#include "Adafruit_TCS34725.h"

/*!
 *  @brief  Implements missing powf function
 *  @param  x
 *          Base number
 *  @param  y
 *          Exponent
 *  @return x raised to the power of y
 */
float powf(const float x, const float y) {
  return (float)(pow((double)x, (double)y));
}

/*!
 *  @brief  Writes a register and an 8 bit value over I2C
 *  @param  reg
 *  @param  value
 */
void Adafruit_TCS34725::write8(uint8_t reg, uint32_t value) {
  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
  _wire->write(value & 0xFF);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
  _wire->send(value & 0xFF);
#endif
  _wire->endTransmission();
}

/*!
 *  @brief  Reads an 8 bit value over I2C
 *  @param  reg
 *  @return value
 */
uint8_t Adafruit_TCS34725::read8(uint8_t reg) {
  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
#endif
  _wire->endTransmission();

  _wire->requestFrom(_i2caddr, 1);
#if ARDUINO >= 100
  return _wire->read();
#else
  return _wire->receive();
#endif
}

/*!
 *  @brief  Reads a 16 bit values over I2C
 *  @param  reg
 *  @return value
 */
uint16_t Adafruit_TCS34725::read16(uint8_t reg) {
  uint16_t x;
  uint16_t t;

  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | reg);
#else
  _wire->send(TCS34725_COMMAND_BIT | reg);
#endif
  _wire->endTransmission();

  _wire->requestFrom(_i2caddr, 2);
#if ARDUINO >= 100
  t = _wire->read();
  x = _wire->read();
#else
  t = _wire->receive();
  x = _wire->receive();
#endif
  x <<= 8;
  x |= t;
  return x;
}

/*!
 *  @brief  Enables the device
 */
void Adafruit_TCS34725::enable() {
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(3);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
  /* Set a delay for the integration time.
    This is only necessary in the case where enabling and then
    immediately trying to read values back. This is because setting
    AEN triggers an automatic integration, so if a read RGBC is
    performed too quickly, the data is not yet valid and all 0's are
    returned */
  switch (_tcs34725IntegrationTime) {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    delay(700);
    break;
  }
}

/*!
 *  @brief  Disables the device (putting it in lower power sleep mode)
 */
void Adafruit_TCS34725::disable() {
  /* Turn the device off to save power */
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}

/*!
 *  @brief  Constructor
 *  @param  it
 *          Integration Time
 *  @param  gain
 *          Gain
 */
Adafruit_TCS34725::Adafruit_TCS34725(tcs34725IntegrationTime_t it,
                                     tcs34725Gain_t gain) {
  _tcs34725Initialised = false;
  _tcs34725IntegrationTime = it;
  _tcs34725Gain = gain;
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @param  addr
 *          i2c address
 *  @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_TCS34725::begin(uint8_t addr) {
  _i2caddr = addr;
  _wire = &Wire;

  return init();
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @param  addr
 *          i2c address
 *  @param  *theWire
 *          The Wire object
 *  @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_TCS34725::begin(uint8_t addr, TwoWire *theWire) {
  _i2caddr = addr;
  _wire = theWire;

  return init();
}

/*!
 *  @brief  Initializes I2C and configures the sensor
 *  @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_TCS34725::begin() {
  _i2caddr = TCS34725_ADDRESS;
  _wire = &Wire;

  return init();
}

/*!
 *  @brief  Part of begin
 *  @return True if initialization was successful, otherwise false.
 */
boolean Adafruit_TCS34725::init() {
  _wire->begin();

  /* Make sure we're actually connected */
  uint8_t x = read8(TCS34725_ID);
  if ((x != 0x44) && (x != 0x10)) {
    return false;
  }
  _tcs34725Initialised = true;

  /* Set default integration time and gain */
  setIntegrationTime(_tcs34725IntegrationTime);
  setGain(_tcs34725Gain);

  /* Note: by default, the device is in power down mode on bootup */
  enable();

  return true;
}

/*!
 *  @brief  Sets the integration time for the TC34725
 *  @param  it
 *          Integration Time
 */
void Adafruit_TCS34725::setIntegrationTime(tcs34725IntegrationTime_t it) {
  if (!_tcs34725Initialised)
    begin();

  /* Update the timing register */
  write8(TCS34725_ATIME, it);

  /* Update value placeholders */
  _tcs34725IntegrationTime = it;
}

/*!
 *  @brief  Adjusts the gain on the TCS34725
 *  @param  gain
 *          Gain (sensitivity to light)
 */
void Adafruit_TCS34725::setGain(tcs34725Gain_t gain) {
  if (!_tcs34725Initialised)
    begin();

  /* Update the timing register */
  write8(TCS34725_CONTROL, gain);

  /* Update value placeholders */
  _tcs34725Gain = gain;
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void Adafruit_TCS34725::getRawData(uint16_t *r, uint16_t *g, uint16_t *b,
                                   uint16_t *c) {
  if (!_tcs34725Initialised)
    begin();

  *c = read16(TCS34725_CDATAL);
  *r = read16(TCS34725_RDATAL);
  *g = read16(TCS34725_GDATAL);
  *b = read16(TCS34725_BDATAL);

  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime) {
  case TCS34725_INTEGRATIONTIME_2_4MS:
    delay(3);
    break;
  case TCS34725_INTEGRATIONTIME_24MS:
    delay(24);
    break;
  case TCS34725_INTEGRATIONTIME_50MS:
    delay(50);
    break;
  case TCS34725_INTEGRATIONTIME_101MS:
    delay(101);
    break;
  case TCS34725_INTEGRATIONTIME_154MS:
    delay(154);
    break;
  case TCS34725_INTEGRATIONTIME_700MS:
    delay(700);
    break;
  }
}

/*!
 *  @brief  Reads the raw red, green, blue and clear channel values in
 *          one-shot mode (e.g., wakes from sleep, takes measurement, enters
 *          sleep)
 *  @param  *r
 *          Red value
 *  @param  *g
 *          Green value
 *  @param  *b
 *          Blue value
 *  @param  *c
 *          Clear channel value
 */
void Adafruit_TCS34725::getRawDataOneShot(uint16_t *r, uint16_t *g, uint16_t *b,
                                          uint16_t *c) {
  if (!_tcs34725Initialised)
    begin();

  enable();
  getRawData(r, g, b, c);
  disable();
}

/*!
 *  @brief  Read the RGB color detected by the sensor.
 *  @param  *r
 *          Red value normalized to 0-255
 *  @param  *g
 *          Green value normalized to 0-255
 *  @param  *b
 *          Blue value normalized to 0-255
 */
void Adafruit_TCS34725::getRGB(float *r, float *g, float *b) {
  uint16_t red, green, blue, clear;
  getRawData(&red, &green, &blue, &clear);
  uint32_t sum = clear;

  // Avoid divide by zero errors ... if clear = 0 return black
  if (clear == 0) {
    *r = *g = *b = 0;
    return;
  }

  *r = (float)red / sum * 255.0;
  *g = (float)green / sum * 255.0;
  *b = (float)blue / sum * 255.0;
}

/*!
 *  @brief  Converts the raw R/G/B values to color temperature in degrees Kelvin
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return Color temperature in degrees Kelvin
 */
uint16_t Adafruit_TCS34725::calculateColorTemperature(uint16_t r, uint16_t g,
                                                      uint16_t b) {
  float X, Y, Z; /* RGB to XYZ correlation      */
  float xc, yc;  /* Chromaticity co-ordinates   */
  float n;       /* McCamy's formula            */
  float cct;

  /* 1. Map RGB values to their XYZ counterparts.    */
  /* Based on 6500K fluorescent, 3000K fluorescent   */
  /* and 60W incandescent values for a wide range.   */
  /* Note: Y = Illuminance or lux                    */
  X = (-0.14282F * r) + (1.54924F * g) + (-0.95641F * b);
  Y = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);
  Z = (-0.68202F * r) + (0.77073F * g) + (0.56332F * b);

  /* 2. Calculate the chromaticity co-ordinates      */
  xc = (X) / (X + Y + Z);
  yc = (Y) / (X + Y + Z);

  /* 3. Use McCamy's formula to determine the CCT    */
  n = (xc - 0.3320F) / (0.1858F - yc);

  /* Calculate the final CCT */
  cct =
      (449.0F * powf(n, 3)) + (3525.0F * powf(n, 2)) + (6823.3F * n) + 5520.33F;

  /* Return the results in degrees Kelvin */
  return (uint16_t)cct;
}

/*!
 *  @brief  Converts the raw R/G/B values to color temperature in degrees
 *          Kelvin using the algorithm described in DN40 from Taos (now AMS).
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @param  c
 *          Clear channel value
 *  @return Color temperature in degrees Kelvin
 */
uint16_t Adafruit_TCS34725::calculateColorTemperature_dn40(uint16_t r,
                                                           uint16_t g,
                                                           uint16_t b,
                                                           uint16_t c) {
  int rc;              /* Error return code */
  uint16_t r2, g2, b2; /* RGB values minus IR component */
  int gl;              /* Results of the initial lux conversion */
  uint8_t gain_int;    /* Gain multiplier as a normal integer */
  uint16_t sat;        /* Digital saturation level */
  uint16_t ir;         /* Inferred IR content */

  /* Analog/Digital saturation:
   *
   * (a) As light becomes brighter, the clear channel will tend to
   *     saturate first since R+G+B is approximately equal to C.
   * (b) The TCS34725 accumulates 1024 counts per 2.4ms of integration
   *     time, up to a maximum values of 65535. This means analog
   *     saturation can occur up to an integration time of 153.6ms
   *     (64*2.4ms=153.6ms).
   * (c) If the integration time is > 153.6ms, digital saturation will
   *     occur before analog saturation. Digital saturation occurs when
   *     the count reaches 65535.
   */
  if ((256 - _tcs34725IntegrationTime) > 63) {
    /* Track digital saturation */
    sat = 65535;
  } else {
    /* Track analog saturation */
    sat = 1024 * (256 - _tcs34725IntegrationTime);
  }

  /* Ripple rejection:
   *
   * (a) An integration time of 50ms or multiples of 50ms are required to
   *     reject both 50Hz and 60Hz ripple.
   * (b) If an integration time faster than 50ms is required, you may need
   *     to average a number of samples over a 50ms period to reject ripple
   *     from fluorescent and incandescent light sources.
   *
   * Ripple saturation notes:
   *
   * (a) If there is ripple in the received signal, the value read from C
   *     will be less than the max, but still have some effects of being
   *     saturated. This means that you can be below the 'sat' value, but
   *     still be saturating. At integration times >150ms this can be
   *     ignored, but <= 150ms you should calculate the 75% saturation
   *     level to avoid this problem.
   */
  if ((256 - _tcs34725IntegrationTime) <= 63) {
    /* Adjust sat to 75% to avoid analog saturation if atime < 153.6ms */
    sat -= sat / 4;
  }

  /* Check for saturation and mark the sample as invalid if true */
  if (c >= sat) {
    return 0;
  }

  /* AMS RGB sensors have no IR channel, so the IR content must be */
  /* calculated indirectly. */
  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;

  /* Remove the IR component from the raw RGB values */
  r2 = r - ir;
  g2 = g - ir;
  b2 = b - ir;

  /* Convert gain to a usable integer value */
  switch (_tcs34725Gain) {
  case TCS34725_GAIN_4X: /* GAIN 4X */
    gain_int = 4;
    break;
  case TCS34725_GAIN_16X: /* GAIN 16X */
    gain_int = 16;
    break;
  case TCS34725_GAIN_60X: /* GAIN 60X */
    gain_int = 60;
    break;
  case TCS34725_GAIN_1X: /* GAIN 1X */
  default:
    gain_int = 1;
    break;
  }

  /* Calculate the counts per lux (CPL), taking into account the optional
   * arguments for Glass Attenuation (GA) and Device Factor (DF).
   *
   * GA = 1/T where T is glass transmissivity, meaning if glass is 50%
   * transmissive, the GA is 2 (1/0.5=2), and if the glass attenuates light
   * 95% the GA is 20 (1/0.05). A GA of 1.0 assumes perfect transmission.
   *
   * NOTE: It is recommended to have a CPL > 5 to have a lux accuracy
   *       < +/- 0.5 lux, where the digitization error can be calculated via:
   *       'DER = (+/-2) / CPL'.
   */
  float cpl =
      (((256 - _tcs34725IntegrationTime) * 2.4f) * gain_int) / (1.0f * 310.0f);

  /* Determine lux accuracy (+/- lux) */
  float der = 2.0f / cpl;

  /* Determine the maximum lux value */
  float max_lux = 65535.0 / (cpl * 3);

  /* Lux is a function of the IR-compensated RGB channels and the associated
   * color coefficients, with G having a particularly heavy influence to
   * match the nature of the human eye.
   *
   * NOTE: The green value should be > 10 to ensure the accuracy of the lux
   *       conversions. If it is below 10, the gain should be increased, but
   *       the clear<100 check earlier should cover this edge case.
   */
  gl = 0.136f * (float)r2 + /** Red coefficient. */
       1.000f * (float)g2 + /** Green coefficient. */
       -0.444f * (float)b2; /** Blue coefficient. */

  float lux = gl / cpl;

  /* A simple method of measuring color temp is to use the ratio of blue */
  /* to red light, taking IR cancellation into account. */
  uint16_t cct = (3810 * (uint32_t)b2) / /** Color temp coefficient. */
                     (uint32_t)r2 +
                 1391; /** Color temp offset. */

  return cct;
}

/*!
 *  @brief  Converts the raw R/G/B values to lux
 *  @param  r
 *          Red value
 *  @param  g
 *          Green value
 *  @param  b
 *          Blue value
 *  @return Lux value
 */
uint16_t Adafruit_TCS34725::calculateLux(uint16_t r, uint16_t g, uint16_t b) {
  float illuminance;

  /* This only uses RGB ... how can we integrate clear or calculate lux */
  /* based exclusively on clear since this might be more reliable?      */
  illuminance = (-0.32466F * r) + (1.57837F * g) + (-0.73191F * b);

  return (uint16_t)illuminance;
}

/*!
 *  @brief  Sets inerrupt for TCS34725
 *  @param  i
 *          Interrupt (True/False)
 */
void Adafruit_TCS34725::setInterrupt(boolean i) {
  uint8_t r = read8(TCS34725_ENABLE);
  if (i) {
    r |= TCS34725_ENABLE_AIEN;
  } else {
    r &= ~TCS34725_ENABLE_AIEN;
  }
  write8(TCS34725_ENABLE, r);
}

/*!
 *  @brief  Clears inerrupt for TCS34725
 */
void Adafruit_TCS34725::clearInterrupt() {
  _wire->beginTransmission(_i2caddr);
#if ARDUINO >= 100
  _wire->write(TCS34725_COMMAND_BIT | 0x66);
#else
  _wire->send(TCS34725_COMMAND_BIT | 0x66);
#endif
  _wire->endTransmission();
}

/*!
 *  @brief  Sets inerrupt limits
 *  @param  low
 *          Low limit
 *  @param  high
 *          High limit
 */
void Adafruit_TCS34725::setIntLimits(uint16_t low, uint16_t high) {
  write8(0x04, low & 0xFF);
  write8(0x05, low >> 8);
  write8(0x06, high & 0xFF);
  write8(0x07, high >> 8);
}
