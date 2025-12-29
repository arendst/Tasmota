/*!
 * @file Adafruit_seesaw.cpp
 *
 * @mainpage Adafruit seesaw arduino driver
 *
 * @section intro_sec Introduction
 *
 * This is part of Adafruit's seesaw driver for the Arduino platform.  It is
 * designed specifically to work with the Adafruit products that use seesaw
 * technology.
 *
 * These chips use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the board.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Dean Miller for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#include "Adafruit_seesaw.h"
#include <Arduino.h>

//#define SEESAW_I2C_DEBUG

/*!
 *****************************************************************************************
 *  @brief      Create a seesaw object on a given I2C bus
 *
 *  @param      i2c_bus the I2C bus connected to the seesaw, defaults to "Wire"
 ****************************************************************************************/
Adafruit_seesaw::Adafruit_seesaw(TwoWire *i2c_bus) {
  if (i2c_bus == NULL) {
    _i2cbus = &Wire;
  } else {
    _i2cbus = i2c_bus;
  }
}

/*!
 *****************************************************************************************
 *  @brief      Start the seesaw
 *
 *				This should be called when your sketch is
 *connecting to the seesaw
 *
 *  @param      addr the I2C address of the seesaw
 *  @param      flow the flow control pin to use
 *  @param		reset pass true to reset the seesaw on startup. Defaults
 *to true.
 *
 *  @return     true if we could connect to the seesaw, false otherwise
 ****************************************************************************************/
bool Adafruit_seesaw::begin(uint8_t addr, int8_t flow, bool reset) {
  _flow = flow;

  if (_flow != -1)
    ::pinMode(_flow, INPUT);

  if (_i2c_dev) {
    delete _i2c_dev;
  }

  _i2c_dev = new Adafruit_I2CDevice(addr, _i2cbus);

  bool found = false;
  for (int retries = 0; retries < 10; retries++) {
    if (_i2c_dev->begin()) {
      found = true;
      break;
    }
    delay(10);
  }

  if (!found) {
    return false;
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Begun");
#endif

  if (reset) {
    found = false;
    SWReset();
    for (int retries = 0; retries < 10; retries++) {
      if (_i2c_dev->detected()) {
        found = true;
        break;
      }
      delay(10);
    }
  }

  if (!found) {
    return false;
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Reset");
#endif

  found = false;
  for (int retries = 0; !found && retries < 10; retries++) {
    uint8_t c = 0;

    this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID, &c, 1);
    if ((c == SEESAW_HW_ID_CODE_SAMD09) || (c == SEESAW_HW_ID_CODE_TINY817) ||
        (c == SEESAW_HW_ID_CODE_TINY807) || (c == SEESAW_HW_ID_CODE_TINY816) ||
        (c == SEESAW_HW_ID_CODE_TINY806) || (c == SEESAW_HW_ID_CODE_TINY1616) ||
        (c == SEESAW_HW_ID_CODE_TINY1617)) {
      found = true;
      _hardwaretype = c;
    }

    delay(10);
  }

#ifdef SEESAW_I2C_DEBUG
  Serial.println("Done!");
#endif

  return found;
}

/*!
 *******************************************************************
 *  @brief      perform a software reset. This resets all seesaw registers to
 *their default values.
 *  			This is called automatically from
 *Adafruit_seesaw.begin()
 * @returns  True on I2C write success, false otherwise

 ********************************************************************/
bool Adafruit_seesaw::SWReset() {
  return this->write8(SEESAW_STATUS_BASE, SEESAW_STATUS_SWRST, 0xFF);
}

/*!
 **************************************************************************
 *  @brief      Returns the available options compiled into the seesaw firmware.
 *  @return     the available options compiled into the seesaw firmware. If the
 *option is included, the corresponding bit is set. For example, if the ADC
 *module is compiled in then (ss.getOptions() & (1UL << SEESAW_ADC_BASE)) > 0
 ***********************************************************************/
uint32_t Adafruit_seesaw::getOptions() {
  uint8_t buf[4];
  this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_OPTIONS, buf, 4);
  uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return ret;
}

/*!
 *********************************************************************
 *  @brief      Returns the version of the seesaw
 *  @return     The version code. Bits [31:16] will be a date code, [15:0] will
 *be the product id.
 ********************************************************************/
uint32_t Adafruit_seesaw::getVersion() {
  uint8_t buf[4];
  this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_VERSION, buf, 4);
  uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return ret;
}

/*!
 *********************************************************************
 *  @brief        Returns the version of the seesaw
 *  @param  pid   Pointer to uint16_t for product code result.
 *  @param  year  Pointer to uint8_t for date code year result.
 *  @param  mon   Pointer to uint8_t for date code month result.
 *  @param  day   Pointer to uint8_t for date code day result.
 *  @return       Always returns true.
 ********************************************************************/
bool Adafruit_seesaw::getProdDatecode(uint16_t *pid, uint8_t *year,
                                      uint8_t *mon, uint8_t *day) {
  uint32_t vers = getVersion();
  *pid = vers >> 16;

  *year = vers & 0x3F;
  *mon = (vers >> 7) & 0xF;
  *day = (vers >> 11) & 0x1F;
  return true;
}

/*!
 **************************************************************************
 *  @brief      Set the mode of a GPIO pin.
 *
 *  @param      pin the pin number. On the SAMD09 breakout, this corresponds to
 *the number on the silkscreen.
 *  @param		mode the mode to set the pin. One of INPUT, OUTPUT, or
 *INPUT_PULLUP.

 ************************************************************************/
void Adafruit_seesaw::pinMode(uint8_t pin, uint8_t mode) {
  if (pin >= 32)
    pinModeBulk(0, 1ul << (pin - 32), mode);
  else
    pinModeBulk(1ul << pin, mode);
}

/*!
 ***************************************************************************
 *  @brief      Set the output of a GPIO pin
 *
 *  @param      pin the pin number. On the SAMD09 breakout, this corresponds to
 *the number on the silkscreen.
 *	@param		value the value to write to the GPIO pin. This should be
 *HIGH or LOW.
 ***************************************************************************/
void Adafruit_seesaw::digitalWrite(uint8_t pin, uint8_t value) {
  if (pin >= 32)
    digitalWriteBulk(0, 1ul << (pin - 32), value);
  else
    digitalWriteBulk(1ul << pin, value);
}

/*!
 ****************************************************************************
 *  @brief      Read the current status of a GPIO pin
 *
 *  @param      pin the pin number. On the SAMD09 breakout, this corresponds to
 *the number on the silkscreen.
 *
 *  @return     the status of the pin. HIGH or LOW (1 or 0).
 ***********************************************************************/
bool Adafruit_seesaw::digitalRead(uint8_t pin) {
  if (pin >= 32)
    return digitalReadBulkB((1ul << (pin - 32))) != 0;
  else
    return digitalReadBulk((1ul << pin)) != 0;
}

/*!
 ****************************************************************************
 *  @brief      read the status of multiple pins on port A.
 *
 *  @param      pins a bitmask of the pins to write. On the SAMD09 breakout,
 *this corresponds to the number on the silkscreen. For example, passing 0b0110
 *will return the values of pins 2 and 3.
 *
 *  @return     the status of the passed pins. If 0b0110 was passed and pin 2 is
 *high and pin 3 is low, 0b0010 (decimal number 2) will be returned.
 *******************************************************************/
uint32_t Adafruit_seesaw::digitalReadBulk(uint32_t pins) {
  uint8_t buf[4];
  this->read(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, buf, 4);
  uint32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                 ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return ret & pins;
}

/*!
 **************************************************************************
 *  @brief      read the status of multiple pins on port B.
 *
 *  @param      pins a bitmask of the pins to write.
 *
 *  @return     the status of the passed pins. If 0b0110 was passed and pin 2 is
 *high and pin 3 is low, 0b0010 (decimal number 2) will be returned.
 ************************************************************************/
uint32_t Adafruit_seesaw::digitalReadBulkB(uint32_t pins) {
  uint8_t buf[8];
  this->read(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, buf, 8);
  uint32_t ret = ((uint32_t)buf[4] << 24) | ((uint32_t)buf[5] << 16) |
                 ((uint32_t)buf[6] << 8) | (uint32_t)buf[7];
  return ret & pins;
}

/*!
 **********************************************************************
 *  @brief      Enable or disable GPIO interrupts on the passed pins
 *
 *  @param      pins a bitmask of the pins to write. On the SAMD09 breakout,
 *this corresponds to the number on the silkscreen. For example, passing 0b0110
 *will enable or disable interrups on pins 2 and 3.
 *	@param		enabled pass true to enable the interrupts on the passed
 *pins, false to disable the interrupts on the passed pins.
 ***********************************************************************/
void Adafruit_seesaw::setGPIOInterrupts(uint32_t pins, bool enabled) {
  uint8_t cmd[] = {(uint8_t)(pins >> 24), (uint8_t)(pins >> 16),
                   (uint8_t)(pins >> 8), (uint8_t)pins};
  if (enabled)
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_INTENSET, cmd, 4);
  else
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_INTENCLR, cmd, 4);
}

/*!
 ****************************************************************
 *  @brief      read the analog value on an ADC-enabled pin.
 *
 *  @param      pin the number of the pin to read. On the SAMD09 breakout, this
 *corresponds to the number on the silkscreen. On the default seesaw firmware on
 *the SAMD09 breakout, pins 2, 3, and 4 are ADC-enabled.
 *
 *  @return     the analog value. This is an integer between 0 and 1023
 ***********************************************************************/
uint16_t Adafruit_seesaw::analogRead(uint8_t pin) {
  uint8_t buf[2];
  uint8_t p = 0;

  if (_hardwaretype == SEESAW_HW_ID_CODE_SAMD09) {
    switch (pin) {
    case ADC_INPUT_0_PIN:
      p = 0;
      break;
    case ADC_INPUT_1_PIN:
      p = 1;
      break;
    case ADC_INPUT_2_PIN:
      p = 2;
      break;
    case ADC_INPUT_3_PIN:
      p = 3;
      break;
    default:
      return 0;
    }
  } else if ((_hardwaretype == SEESAW_HW_ID_CODE_TINY807) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY817) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY816) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY806) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1616) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1617)) {
    p = pin;
  } else {
    return 0;
  }

  this->read(SEESAW_ADC_BASE, SEESAW_ADC_CHANNEL_OFFSET + p, buf, 2, 500);
  uint16_t ret = ((uint16_t)buf[0] << 8) | buf[1];
  delay(1);
  return ret;
}

/*!
 ******************************************************************************
 *  @brief      read the analog value on an capacitive touch-enabled pin.
 *
 *  @param      pin the number of the pin to read.
 *
 *  @return     the analog value. This is an integer between 0 and 1023
 ****************************************************************************/
uint16_t Adafruit_seesaw::touchRead(uint8_t pin) {
  uint8_t buf[2];
  uint8_t p = pin;
  uint16_t ret = 65535;

  for (uint8_t retry = 0; retry < 5; retry++) {
    if (this->read(SEESAW_TOUCH_BASE, SEESAW_TOUCH_CHANNEL_OFFSET + p, buf, 2,
                   3000 + retry * 1000)) {
      ret = ((uint16_t)buf[0] << 8) | buf[1];
      break;
    }
  }
  return ret;
}

/*!
 ***************************************************************************
 *  @brief      set the mode of multiple GPIO pins at once.
 *
 *  @param      pins a bitmask of the pins to write. On the SAMD09 breakout,
 *this corresponds to the number on the silkscreen. For example, passing 0b0110
 *will set the mode of pins 2 and 3.
 *	@param		mode the mode to set the pins to. One of INPUT, OUTPUT,
 *or INPUT_PULLUP.
 ************************************************************************/
void Adafruit_seesaw::pinModeBulk(uint32_t pins, uint8_t mode) {
  uint8_t cmd[] = {(uint8_t)(pins >> 24), (uint8_t)(pins >> 16),
                   (uint8_t)(pins >> 8), (uint8_t)pins};
  switch (mode) {
  case OUTPUT:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRSET_BULK, cmd, 4);
    break;
  case INPUT:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 4);
    break;
  case INPUT_PULLUP:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 4);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_PULLENSET, cmd, 4);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_SET, cmd, 4);
    break;
  case INPUT_PULLDOWN:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 4);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_PULLENSET, cmd, 4);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_CLR, cmd, 4);
    break;
  }
}

/*!
 *****************************************************************************************
 *  @brief      set the mode of multiple GPIO pins at once. This supports both
 *ports A and B.
 *
 *  @param      pinsa a bitmask of the pins to write on port A. On the SAMD09
 *breakout, this corresponds to the number on the silkscreen. For example,
 *passing 0b0110 will set the mode of pins 2 and 3.
 *  @param      pinsb a bitmask of the pins to write on port B.
 *	@param		mode the mode to set the pins to. One of INPUT, OUTPUT,
 *or INPUT_PULLUP.
 ****************************************************************************************/
void Adafruit_seesaw::pinModeBulk(uint32_t pinsa, uint32_t pinsb,
                                  uint8_t mode) {
  uint8_t cmd[] = {(uint8_t)(pinsa >> 24), (uint8_t)(pinsa >> 16),
                   (uint8_t)(pinsa >> 8),  (uint8_t)pinsa,
                   (uint8_t)(pinsb >> 24), (uint8_t)(pinsb >> 16),
                   (uint8_t)(pinsb >> 8),  (uint8_t)pinsb};
  switch (mode) {
  case OUTPUT:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRSET_BULK, cmd, 8);
    break;
  case INPUT:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 8);
    break;
  case INPUT_PULLUP:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 8);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_PULLENSET, cmd, 8);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_SET, cmd, 8);
    break;
  case INPUT_PULLDOWN:
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_DIRCLR_BULK, cmd, 8);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_PULLENSET, cmd, 8);
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_CLR, cmd, 8);
    break;
  }
}

/*!
 *****************************************************************************************
 *  @brief      write a value to multiple GPIO pins at once.
 *
 *  @param      pins a bitmask of the pins to write. On the SAMD09 breakout,
 *this corresponds to the number on the silkscreen. For example, passing 0b0110
 *will write the passed value to pins 2 and 3.
 *	@param		value pass HIGH to set the output on the passed pins to
 *HIGH, low to set the output on the passed pins to LOW.
 ****************************************************************************************/
void Adafruit_seesaw::digitalWriteBulk(uint32_t pins, uint8_t value) {
  uint8_t cmd[] = {(uint8_t)(pins >> 24), (uint8_t)(pins >> 16),
                   (uint8_t)(pins >> 8), (uint8_t)pins};
  if (value)
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_SET, cmd, 4);
  else
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_CLR, cmd, 4);
}

/*!
 *****************************************************************************************
 *  @brief      write a value to multiple GPIO pins at once. This supports both
 *ports A and B
 *
 *  @param      pinsa a bitmask of the pins to write on port A. On the SAMD09
 *breakout, this corresponds to the number on the silkscreen. For example,
 *passing 0b0110 will write the passed value to pins 2 and 3.
 *  @param      pinsb a bitmask of the pins to write on port B.
 *	@param		value pass HIGH to set the output on the passed pins to
 *HIGH, low to set the output on the passed pins to LOW.
 ****************************************************************************************/
void Adafruit_seesaw::digitalWriteBulk(uint32_t pinsa, uint32_t pinsb,
                                       uint8_t value) {
  uint8_t cmd[] = {(uint8_t)(pinsa >> 24), (uint8_t)(pinsa >> 16),
                   (uint8_t)(pinsa >> 8),  (uint8_t)pinsa,
                   (uint8_t)(pinsb >> 24), (uint8_t)(pinsb >> 16),
                   (uint8_t)(pinsb >> 8),  (uint8_t)pinsb};
  if (value)
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_SET, cmd, 8);
  else
    this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_CLR, cmd, 8);
}

/*!
 *****************************************************************************************
 *  @brief      write the entire GPIO port at once.
 *
 *  @param      port_values The up-to-32 values to write to the pins, doesn't
 *set direction used for bulk writing quickly all valid pins
 ****************************************************************************************/
void Adafruit_seesaw::digitalWriteBulk(uint32_t port_values) {
  uint8_t cmd[] = {(uint8_t)(port_values >> 24), (uint8_t)(port_values >> 16),
                   (uint8_t)(port_values >> 8), (uint8_t)port_values};
  this->write(SEESAW_GPIO_BASE, SEESAW_GPIO_BULK, cmd, 4);
}

/*!
 *****************************************************************************************
 *  @brief      write a PWM value to a PWM-enabled pin
 *
 *  @param      pin the number of the pin to write. On the SAMD09 breakout, this
 *corresponds to the number on the silkscreen. on the default seesaw firmware on
 *the SAMD09 breakout, pins 5, 6, and 7 are PWM enabled.
 *	@param		value the value to write to the pin
 *	@param		width the width of the value to write. Defaults to 8. If
 *16 is passed a 16 bit value will be written.
 ****************************************************************************************/
void Adafruit_seesaw::analogWrite(uint8_t pin, uint16_t value, uint8_t width) {
  int8_t p = -1;

  if (_hardwaretype == SEESAW_HW_ID_CODE_SAMD09) {
    switch (pin) {
    case PWM_0_PIN:
      p = 0;
      break;
    case PWM_1_PIN:
      p = 1;
      break;
    case PWM_2_PIN:
      p = 2;
      break;
    case PWM_3_PIN:
      p = 3;
      break;
    default:
      return;
    }
  } else if ((_hardwaretype == SEESAW_HW_ID_CODE_SAMD09) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY817) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY807) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY816) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY806) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1616) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1617)) {
    p = pin;
  } else {
    return;
  }

  if (width == 16) {
    uint8_t cmd[] = {(uint8_t)p, (uint8_t)(value >> 8), (uint8_t)value};
    this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_PWM, cmd, 3);
  } else {
    uint16_t mappedVal = map(value, 0, 255, 0, 65535);
    uint8_t cmd[] = {(uint8_t)p, (uint8_t)(mappedVal >> 8), (uint8_t)mappedVal};
    this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_PWM, cmd, 3);
  }
}

/*!
 *  @brief      set the PWM frequency of a PWM-enabled pin. Note that on SAMD09,
 *              SAMD11 boards the frequency will be mapped to closest match
 *		fixed frequencies. Also note that PWM pins 4 and 5 share a
 *timer, and PWM pins 6 and 7 share a timer. Changing the frequency for one pin
 *will change the frequency for the other pin that is on the timer.
 *
 *  @param      pin the number of the pin to change frequency of. On the SAMD09
 *              breakout, this corresponds to the number on the silkscreen.
 *              on the default seesaw firmware on the SAMD09 breakout, pins 5,
 *6, and 7 are PWM enabled.
 *  @param      freq the frequency to set.
 ******************************************************************************/
void Adafruit_seesaw::setPWMFreq(uint8_t pin, uint16_t freq) {
  int8_t p = -1;

  if (_hardwaretype == SEESAW_HW_ID_CODE_SAMD09) {
    switch (pin) {
    case PWM_0_PIN:
      p = 0;
      break;
    case PWM_1_PIN:
      p = 1;
      break;
    case PWM_2_PIN:
      p = 2;
      break;
    case PWM_3_PIN:
      p = 3;
      break;
    default:
      break;
    }
  } else if ((_hardwaretype == SEESAW_HW_ID_CODE_TINY817) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY807) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY816) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY806) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1616) ||
             (_hardwaretype == SEESAW_HW_ID_CODE_TINY1617)) {
    p = pin;
  } else {
    return;
  }

  if (p > -1) {
    uint8_t cmd[] = {(uint8_t)p, (uint8_t)(freq >> 8), (uint8_t)freq};
    this->write(SEESAW_TIMER_BASE, SEESAW_TIMER_FREQ, cmd, 3);
  }
}

/*!
 *  @brief      Enable the data ready interrupt on the passed sercom. Note that
 *both the interrupt module and the passed sercom must be compiled into the
 *seesaw firmware for this to function. If both of these things are true, the
 *interrupt pin on the seesaw will fire when there is data to be read from the
 *passed sercom. On the default seesaw firmeare on the SAMD09 breakout, no
 *sercoms are enabled.
 *
 *  @param      sercom the sercom to enable the interrupt on.
 ****************************************************************************************/
void Adafruit_seesaw::enableSercomDataRdyInterrupt(uint8_t sercom) {
  _sercom_inten.bit.DATA_RDY = 1;
  this->write8(SEESAW_SERCOM0_BASE + sercom, SEESAW_SERCOM_INTEN,
               _sercom_inten.reg);
}

/*!
 ***************************************************************************************
 *  @brief      Disable the data ready interrupt on the passed sercom.
 *
 *  @param      sercom the sercom to disable the interrupt on.
 ****************************************************************************************/
void Adafruit_seesaw::disableSercomDataRdyInterrupt(uint8_t sercom) {
  _sercom_inten.bit.DATA_RDY = 0;
  this->write8(SEESAW_SERCOM0_BASE + sercom, SEESAW_SERCOM_INTEN,
               _sercom_inten.reg);
}

/*!
 *****************************************************************************************
 *  @brief      Reads a character from the passed sercom if one is available.
 *Note that on the default seesaw firmware on the SAMD09 breakout no sercoms are
 *enabled.
 *
 *  @param      sercom the sercom to read data from.
 *  @returns    One byte of data
 ****************************************************************************************/
char Adafruit_seesaw::readSercomData(uint8_t sercom) {
  return this->read8(SEESAW_SERCOM0_BASE + sercom, SEESAW_SERCOM_DATA);
}

/*!
 *****************************************************************************************
 *  @brief      Return the EEPROM address used to store I2C address
 *
 *  @return     the EEPROM address location
 ****************************************************************************************/
uint8_t Adafruit_seesaw::getI2CaddrEEPROMloc() {
  // All SAMDs use fixed location -> 0x3F
  // ATtinys place at end of EEPROM, so can vary:
  //   8xx have 128B of EEPROM -> 0x7F
  //   16xx have 256B of EERPOM -> 0xFF
  switch (_hardwaretype) {
  case SEESAW_HW_ID_CODE_SAMD09:
    return 0x3F;
  case SEESAW_HW_ID_CODE_TINY817:
  case SEESAW_HW_ID_CODE_TINY807:
  case SEESAW_HW_ID_CODE_TINY816:
  case SEESAW_HW_ID_CODE_TINY806:
    return 0x7F;
  case SEESAW_HW_ID_CODE_TINY1616:
  case SEESAW_HW_ID_CODE_TINY1617:
    return 0xFF;
  default:
    return 0x00;
  }
}

/*!
 *****************************************************************************************
 *  @brief      Set the seesaw I2C address. This will automatically call
 *Adafruit_seesaw.begin() with the new address.
 *
 *  @param      addr the new address for the seesaw. This must be a valid 7 bit
 *I2C address.
 ****************************************************************************************/
void Adafruit_seesaw::setI2CAddr(uint8_t addr) {
  this->EEPROMWrite8(getI2CaddrEEPROMloc(), addr);
  delay(250);
  this->begin(addr); // restart w/ the new addr
}

/*!
 *****************************************************************************************
 *  @brief      Read the I2C address of the seesaw
 *
 *  @return     the 7 bit I2C address of the seesaw... which you probably
 *already know because you just read data from it.
 ****************************************************************************************/
uint8_t Adafruit_seesaw::getI2CAddr() {
  return this->EEPROMRead8(getI2CaddrEEPROMloc());
}

/*!
 *****************************************************************************************
 *  @brief      Write a 1 byte to an EEPROM address
 *
 *  @param      addr the address to write to. On the default seesaw firmware on
 *the SAMD09 breakout this is between 0 and 63.
 *	@param		val to write between 0 and 255
 ****************************************************************************************/
void Adafruit_seesaw::EEPROMWrite8(uint8_t addr, uint8_t val) {
  this->EEPROMWrite(addr, &val, 1);
}

/*!
 *****************************************************************************************
 *  @brief      write a string of bytes to EEPROM starting at the passed address
 *
 *  @param      addr the starting address to write the first byte. This will be
 *automatically incremented with each byte written.
 *	@param		buf the buffer of bytes to be written.
 *	@param		size the number of bytes to write. Writing past the end
 *of available EEPROM may result in undefined behavior.
 ****************************************************************************************/
void Adafruit_seesaw::EEPROMWrite(uint8_t addr, uint8_t *buf, uint8_t size) {
  this->write(SEESAW_EEPROM_BASE, addr, buf, size);
}

/*!
 *****************************************************************************************
 *  @brief      Read 1 byte from the specified EEPROM address.
 *
 *  @param      addr the address to read from. One the default seesaw firmware
 *on the SAMD09 breakout this is between 0 and 63.
 *
 *  @return     the value between 0 and 255 that was read from the passed
 *address.
 ****************************************************************************************/
uint8_t Adafruit_seesaw::EEPROMRead8(uint8_t addr) {
  return this->read8(SEESAW_EEPROM_BASE, addr);
}

/*!
 *****************************************************************************************
 *  @brief      Set the baud rate on SERCOM0.
 *
 *  @param      baud the baud rate to set. This is an integer value. Baud rates
 *up to 115200 are supported.
 ****************************************************************************************/
void Adafruit_seesaw::UARTSetBaud(uint32_t baud) {
  uint8_t cmd[] = {(uint8_t)(baud >> 24), (uint8_t)(baud >> 16),
                   (uint8_t)(baud >> 8), (uint8_t)baud};
  this->write(SEESAW_SERCOM0_BASE, SEESAW_SERCOM_BAUD, cmd, 4);
}

/*!
 *****************************************************************************************
 *  @brief      activate or deactivate a key and edge on the keypad module
 *
 *  @param      key the key number to activate
 *  @param		edge the edge to trigger on
 *  @param		enable passing true will enable the passed event,
 *passing false will disable it.
 ****************************************************************************************/
void Adafruit_seesaw::setKeypadEvent(uint8_t key, uint8_t edge, bool enable) {
  keyState ks;
  ks.bit.STATE = enable;
  ks.bit.ACTIVE = (1 << edge);
  uint8_t cmd[] = {key, ks.reg};
  this->write(SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_EVENT, cmd, 2);
}

/**
 *****************************************************************************************
 *  @brief      enable the keypad interrupt that fires when events are in the
 *fifo.
 ****************************************************************************************/
void Adafruit_seesaw::enableKeypadInterrupt() {
  this->write8(SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_INTENSET, 0x01);
}

/**
 *****************************************************************************************
 *  @brief      disable the keypad interrupt that fires when events are in the
 *fifo.
 ****************************************************************************************/
void Adafruit_seesaw::disableKeypadInterrupt() {
  this->write8(SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_INTENCLR, 0x01);
}

/**
 *****************************************************************************************
 *  @brief      Get the number of events currently in the fifo
 *  @return     the number of events in the fifo
 ****************************************************************************************/
uint8_t Adafruit_seesaw::getKeypadCount() {
  return this->read8(SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_COUNT, 500);
}

/**
 *****************************************************************************************
 *  @brief      Read all keyEvents into the passed buffer
 *
 *  @param      buf pointer to where the keyEvents should be stored
 *  @param		count the number of events to read
 *  @returns    True on I2C read success
 ****************************************************************************************/
bool Adafruit_seesaw::readKeypad(keyEventRaw *buf, uint8_t count) {
  return this->read(SEESAW_KEYPAD_BASE, SEESAW_KEYPAD_FIFO, (uint8_t *)buf,
                    count, 1000);
}

/**
 *****************************************************************************************
 *  @brief      Read the temperature of the seesaw board in degrees Celsius.
 *NOTE: not all seesaw firmwares have the temperature sensor enabled.
 *  @return     Temperature in degrees Celsius as a floating point value.
 ****************************************************************************************/
float Adafruit_seesaw::getTemp() {
  uint8_t buf[4];
  this->read(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP, buf, 4, 1000);
  int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
  return (1.0 / (1UL << 16)) * ret;
}

/**
 *****************************************************************************************
 *  @brief      Read the current position of the encoder
 *  @param encoder Which encoder to use, defaults to 0
 *  @return     The encoder position as a 32 bit signed integer.
 ****************************************************************************************/
int32_t Adafruit_seesaw::getEncoderPosition(uint8_t encoder) {
  uint8_t buf[4];
  this->read(SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION + encoder, buf, 4);
  int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

  return ret;
}

/**
 *****************************************************************************************
 *  @brief      Set the current position of the encoder
 *  @param encoder Which encoder to use, defaults to 0
 *  @param     pos the position to set the encoder to.
 ****************************************************************************************/
void Adafruit_seesaw::setEncoderPosition(int32_t pos, uint8_t encoder) {
  uint8_t buf[] = {(uint8_t)(pos >> 24), (uint8_t)(pos >> 16),
                   (uint8_t)(pos >> 8), (uint8_t)(pos & 0xFF)};
  this->write(SEESAW_ENCODER_BASE, SEESAW_ENCODER_POSITION + encoder, buf, 4);
}

/**
 *****************************************************************************************
 *  @brief      Read the change in encoder position since it was last read.
 *  @param encoder Which encoder to use, defaults to 0
 *  @return     The encoder change as a 32 bit signed integer.
 ****************************************************************************************/
int32_t Adafruit_seesaw::getEncoderDelta(uint8_t encoder) {
  uint8_t buf[4];
  this->read(SEESAW_ENCODER_BASE, SEESAW_ENCODER_DELTA + encoder, buf, 4);
  int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];

  return ret;
}

/**
 *****************************************************************************************
 *  @brief      Enable the interrupt to fire when the encoder changes position.
 *  @param encoder Which encoder to use, defaults to 0
 *  @returns    True on I2C write success
 ****************************************************************************************/
bool Adafruit_seesaw::enableEncoderInterrupt(uint8_t encoder) {
  return this->write8(SEESAW_ENCODER_BASE, SEESAW_ENCODER_INTENSET + encoder,
                      0x01);
}

/**
 *****************************************************************************************
 *  @brief      Disable the interrupt from firing when the encoder changes
 *position.
 *  @param encoder Which encoder to use, defaults to 0
 *  @returns    True on I2C write success
 ****************************************************************************************/
bool Adafruit_seesaw::disableEncoderInterrupt(uint8_t encoder) {
  return this->write8(SEESAW_ENCODER_BASE, SEESAW_ENCODER_INTENCLR + encoder,
                      0x01);
}

/**
 *****************************************************************************************
 *  @brief      Write 1 byte to the specified seesaw register.
 *
 *  @param      regHigh the module address register (ex. SEESAW_NEOPIXEL_BASE)
 *	@param		regLow the function address register (ex.
 *SEESAW_NEOPIXEL_PIN)
 *	@param		value the value between 0 and 255 to write
 *  @returns    True on I2C write success
 ****************************************************************************************/
bool Adafruit_seesaw::write8(byte regHigh, byte regLow, byte value) {
  return this->write(regHigh, regLow, &value, 1);
}

/**
 *****************************************************************************************
 *  @brief      read 1 byte from the specified seesaw register.
 *
 *  @param      regHigh the module address register (ex. SEESAW_STATUS_BASE)
 *	@param		regLow the function address register (ex.
 *SEESAW_STATUS_VERSION)
 *	@param		delay a number of microseconds to delay before reading
 *out the data. Different delay values may be necessary to ensure the seesaw
 *chip has time to process the requested data. Defaults to 125.
 *
 *  @return     the value between 0 and 255 read from the passed register
 ****************************************************************************************/
uint8_t Adafruit_seesaw::read8(byte regHigh, byte regLow, uint16_t delay) {
  uint8_t ret;
  this->read(regHigh, regLow, &ret, 1, delay);

  return ret;
}

/**
 *****************************************************************************************
 *  @brief      Read a specified number of bytes into a buffer from the seesaw.
 *
 *  @param      regHigh the module address register (ex. SEESAW_STATUS_BASE)
 *	@param		regLow the function address register (ex.
 *SEESAW_STATUS_VERSION)
 *	@param		buf the buffer to read the bytes into
 *	@param		num the number of bytes to read.
 *	@param		delay an optional delay in between setting the read
 *register and reading out the data. This is required for some seesaw functions
 *(ex. reading ADC data)
 *  @returns    True on I2C read success
 ****************************************************************************************/
bool Adafruit_seesaw::read(uint8_t regHigh, uint8_t regLow, uint8_t *buf,
                           uint8_t num, uint16_t delay) {
  uint8_t pos = 0;
  uint8_t prefix[2];
  prefix[0] = (uint8_t)regHigh;
  prefix[1] = (uint8_t)regLow;

  // on arduino we need to read in 32 byte chunks
  while (pos < num) {
    uint8_t read_now = min(32, num - pos);

    if (_flow != -1) {
      while (!::digitalRead(_flow))
        yield();
    }

    if (!_i2c_dev->write(prefix, 2)) {
      return false;
    }

    // TODO: tune this
    delayMicroseconds(delay);

    if (_flow != -1) {
      while (!::digitalRead(_flow))
        yield();
    }

#ifdef SEESAW_I2C_DEBUG
    Serial.print("Reading ");
    Serial.print(read_now);
    Serial.println(" bytes");
#endif

    if (!_i2c_dev->read(buf + pos, read_now)) {
      return false;
    }
    pos += read_now;
#ifdef SEESAW_I2C_DEBUG
    Serial.print("pos: ");
    Serial.print(pos);
    Serial.print(" num:");
    Serial.println(num);
#endif
  }
  return true;
}

/*!
 *****************************************************************************************
 *  @brief      Write a specified number of bytes to the seesaw from the passed
 *buffer.
 *
 *  @param      regHigh the module address register (ex. SEESAW_GPIO_BASE)
 *  @param	regLow the function address register (ex. SEESAW_GPIO_BULK_SET)
 *  @param	buf the buffer the the bytes from
 *  @param	num the number of bytes to write.
 *  @returns    True on I2C write success
 ****************************************************************************************/
bool Adafruit_seesaw::write(uint8_t regHigh, uint8_t regLow,
                            uint8_t *buf = NULL, uint8_t num = 0) {
  uint8_t prefix[2];
  prefix[0] = (uint8_t)regHigh;
  prefix[1] = (uint8_t)regLow;

  if (_flow != -1)
    while (!::digitalRead(_flow))
      yield();

  if (!_i2c_dev->write(buf, num, true, prefix, 2)) {
    return false;
  }

  return true;
}

/*!
 *****************************************************************************************
 *  @brief      The print wrapper for the seesaw class. Calling this allows you
 *to use ss.print() or ss.println() and write to the UART on SERCOM0 of the
 *seesaw. Note that this functionality is only available when the UART (sercom)
 *module is compiled into the seesaw firmware. On the default seesaw firmware on
 *the SAMD09 breakout this functionality is not available.
 *
 *  @param      character the character to write.
 *  @returns    The number of bytes written (1)
 ****************************************************************************************/
size_t Adafruit_seesaw::write(uint8_t character) {
  // TODO: add support for multiple sercoms
  this->write8(SEESAW_SERCOM0_BASE, SEESAW_SERCOM_DATA, character);
  delay(1); // TODO: this can be optimized... it's only needed for longer writes
  return 1;
}

/*!
 ************************************************************************
 *  @brief      The print wrapper for the seesaw class allowing the user to
 *print a string. Calling this allows you to use ss.print() or ss.println() and
 *write to the UART on SERCOM0 of the seesaw. Note that this functionality is
 *only available when the UART (sercom) module is compiled into the seesaw
 *firmware. On the default seesaw firmware on the SAMD09 breakout this
 *functionality is not available.
 *
 *  @param      str the string to write
 *  @return     number of bytes written (not including trailing 0)
 *********************************************************************/
size_t Adafruit_seesaw::write(const char *str) {
  uint8_t buf[32];
  uint8_t len = 0;
  while (*str) {
    buf[len] = *str;
    str++;
    len++;
  }
  this->write(SEESAW_SERCOM0_BASE, SEESAW_SERCOM_DATA, buf, len);
  return len;
}
