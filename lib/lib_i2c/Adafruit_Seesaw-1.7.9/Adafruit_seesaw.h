/*!
 * @file Adafruit_seesaw.h
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
 * Written by Dean Miller for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef LIB_SEESAW_H
#define LIB_SEESAW_H

#include "Adafruit_I2CDevice.h"
#include <Arduino.h>
#include <Wire.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define SEESAW_ADDRESS (0x49) ///< Default Seesaw I2C address
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/

/** Module Base Addreses
 *  The module base addresses for different seesaw modules.
 */
enum {
  SEESAW_STATUS_BASE = 0x00,
  SEESAW_GPIO_BASE = 0x01,
  SEESAW_SERCOM0_BASE = 0x02,

  SEESAW_TIMER_BASE = 0x08,
  SEESAW_ADC_BASE = 0x09,
  SEESAW_DAC_BASE = 0x0A,
  SEESAW_INTERRUPT_BASE = 0x0B,
  SEESAW_DAP_BASE = 0x0C,
  SEESAW_EEPROM_BASE = 0x0D,
  SEESAW_NEOPIXEL_BASE = 0x0E,
  SEESAW_TOUCH_BASE = 0x0F,
  SEESAW_KEYPAD_BASE = 0x10,
  SEESAW_ENCODER_BASE = 0x11,
  SEESAW_SPECTRUM_BASE = 0x12,
};

/** GPIO module function address registers
 */
enum {
  SEESAW_GPIO_DIRSET_BULK = 0x02,
  SEESAW_GPIO_DIRCLR_BULK = 0x03,
  SEESAW_GPIO_BULK = 0x04,
  SEESAW_GPIO_BULK_SET = 0x05,
  SEESAW_GPIO_BULK_CLR = 0x06,
  SEESAW_GPIO_BULK_TOGGLE = 0x07,
  SEESAW_GPIO_INTENSET = 0x08,
  SEESAW_GPIO_INTENCLR = 0x09,
  SEESAW_GPIO_INTFLAG = 0x0A,
  SEESAW_GPIO_PULLENSET = 0x0B,
  SEESAW_GPIO_PULLENCLR = 0x0C,
};

/** status module function address registers
 */
enum {
  SEESAW_STATUS_HW_ID = 0x01,
  SEESAW_STATUS_VERSION = 0x02,
  SEESAW_STATUS_OPTIONS = 0x03,
  SEESAW_STATUS_TEMP = 0x04,
  SEESAW_STATUS_SWRST = 0x7F,
};

/** timer module function address registers
 */
enum {
  SEESAW_TIMER_STATUS = 0x00,
  SEESAW_TIMER_PWM = 0x01,
  SEESAW_TIMER_FREQ = 0x02,
};

/** ADC module function address registers
 */
enum {
  SEESAW_ADC_STATUS = 0x00,
  SEESAW_ADC_INTEN = 0x02,
  SEESAW_ADC_INTENCLR = 0x03,
  SEESAW_ADC_WINMODE = 0x04,
  SEESAW_ADC_WINTHRESH = 0x05,
  SEESAW_ADC_CHANNEL_OFFSET = 0x07,
};

/** Sercom module function address registers
 */
enum {
  SEESAW_SERCOM_STATUS = 0x00,
  SEESAW_SERCOM_INTEN = 0x02,
  SEESAW_SERCOM_INTENCLR = 0x03,
  SEESAW_SERCOM_BAUD = 0x04,
  SEESAW_SERCOM_DATA = 0x05,
};

/** neopixel module function address registers
 */
enum {
  SEESAW_NEOPIXEL_STATUS = 0x00,
  SEESAW_NEOPIXEL_PIN = 0x01,
  SEESAW_NEOPIXEL_SPEED = 0x02,
  SEESAW_NEOPIXEL_BUF_LENGTH = 0x03,
  SEESAW_NEOPIXEL_BUF = 0x04,
  SEESAW_NEOPIXEL_SHOW = 0x05,
};

/** touch module function address registers
 */
enum {
  SEESAW_TOUCH_CHANNEL_OFFSET = 0x10,
};

/** keypad module function address registers
 */
enum {
  SEESAW_KEYPAD_STATUS = 0x00,
  SEESAW_KEYPAD_EVENT = 0x01,
  SEESAW_KEYPAD_INTENSET = 0x02,
  SEESAW_KEYPAD_INTENCLR = 0x03,
  SEESAW_KEYPAD_COUNT = 0x04,
  SEESAW_KEYPAD_FIFO = 0x10,
};

/** keypad module edge definitions
 */
enum {
  SEESAW_KEYPAD_EDGE_HIGH = 0,
  SEESAW_KEYPAD_EDGE_LOW,
  SEESAW_KEYPAD_EDGE_FALLING,
  SEESAW_KEYPAD_EDGE_RISING,
};

/** encoder module edge definitions
 */
enum {
  SEESAW_ENCODER_STATUS = 0x00,
  SEESAW_ENCODER_INTENSET = 0x10,
  SEESAW_ENCODER_INTENCLR = 0x20,
  SEESAW_ENCODER_POSITION = 0x30,
  SEESAW_ENCODER_DELTA = 0x40,
};

/** Audio spectrum module function address registers
 */
enum {
  SEESAW_SPECTRUM_RESULTS_LOWER = 0x00, // Audio spectrum bins 0-31
  SEESAW_SPECTRUM_RESULTS_UPPER = 0x01, // Audio spectrum bins 32-63
  // If some future device supports a larger spectrum, can add additional
  // "bins" working upward from here. Configurable setting registers then
  // work downward from the top to avoid collision between spectrum bins
  // and configurables.
  SEESAW_SPECTRUM_CHANNEL = 0xFD,
  SEESAW_SPECTRUM_RATE = 0xFE,
  SEESAW_SPECTRUM_STATUS = 0xFF,
};

#define ADC_INPUT_0_PIN 2 ///< default ADC input pin
#define ADC_INPUT_1_PIN 3 ///< default ADC input pin
#define ADC_INPUT_2_PIN 4 ///< default ADC input pin
#define ADC_INPUT_3_PIN 5 ///< default ADC input pin

#define PWM_0_PIN 4 ///< default PWM output pin
#define PWM_1_PIN 5 ///< default PWM output pin
#define PWM_2_PIN 6 ///< default PWM output pin
#define PWM_3_PIN 7 ///< default PWM output pin

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN                                                         \
  0x03 ///< for compatibility with platforms that do not already define
       ///< INPUT_PULLDOWN
#endif

/*=========================================================================*/
// clang-format off
#define SEESAW_HW_ID_CODE_SAMD09 0x55 ///< seesaw HW ID code for SAMD09
#define SEESAW_HW_ID_CODE_TINY806 0x84 ///< seesaw HW ID code for ATtiny806
#define SEESAW_HW_ID_CODE_TINY807 0x85 ///< seesaw HW ID code for ATtiny807
#define SEESAW_HW_ID_CODE_TINY816 0x86 ///< seesaw HW ID code for ATtiny816
#define SEESAW_HW_ID_CODE_TINY817 0x87 ///< seesaw HW ID code for ATtiny817
#define SEESAW_HW_ID_CODE_TINY1616 0x88 ///< seesaw HW ID code for ATtiny1616
#define SEESAW_HW_ID_CODE_TINY1617 0x89 ///< seesaw HW ID code for ATtiny1617
// clang-format on

/** raw key event stucture for keypad module */
union keyEventRaw {
  struct {
    uint8_t EDGE : 2; ///< the edge that was triggered
    uint8_t NUM : 6;  ///< the event number
  } bit;              ///< bitfield format
  uint8_t reg;        ///< register format
};

/** extended key event stucture for keypad module */
union keyEvent {
  struct {
    uint8_t EDGE : 2;  ///< the edge that was triggered
    uint16_t NUM : 14; ///< the event number
  } bit;               ///< bitfield format
  uint16_t reg;        ///< register format
};

/** key state struct that will be written to seesaw chip keypad module */
union keyState {
  struct {
    uint8_t STATE : 1;  ///< the current state of the key
    uint8_t ACTIVE : 4; ///< the registered events for that key
  } bit;                ///< bitfield format
  uint8_t reg;          ///< register format
};

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with seesaw
   helper IC
*/
/**************************************************************************/
class Adafruit_seesaw : public Print {
public:
  // constructors
  Adafruit_seesaw(TwoWire *Wi = NULL);
  ~Adafruit_seesaw(void){};

  bool begin(uint8_t addr = SEESAW_ADDRESS, int8_t flow = -1,
             bool reset = true);
  uint32_t getOptions();
  uint32_t getVersion();
  bool getProdDatecode(uint16_t *pid, uint8_t *year, uint8_t *mon,
                       uint8_t *day);

  bool SWReset();

  void pinMode(uint8_t pin, uint8_t mode);
  void pinModeBulk(uint32_t pins, uint8_t mode);
  void pinModeBulk(uint32_t pinsa, uint32_t pinsb, uint8_t mode);
  virtual void analogWrite(uint8_t pin, uint16_t value, uint8_t width = 8);
  void digitalWrite(uint8_t pin, uint8_t value);
  void digitalWriteBulk(uint32_t port_values);
  void digitalWriteBulk(uint32_t pins, uint8_t value);
  void digitalWriteBulk(uint32_t pinsa, uint32_t pinsb, uint8_t value);

  bool digitalRead(uint8_t pin);
  uint32_t digitalReadBulk(uint32_t pins);
  uint32_t digitalReadBulkB(uint32_t pins);

  void setGPIOInterrupts(uint32_t pins, bool enabled);

  virtual uint16_t analogRead(uint8_t pin);

  uint16_t touchRead(uint8_t pin);

  virtual void setPWMFreq(uint8_t pin, uint16_t freq);

  void enableSercomDataRdyInterrupt(uint8_t sercom = 0);
  void disableSercomDataRdyInterrupt(uint8_t sercom = 0);

  char readSercomData(uint8_t sercom = 0);

  void EEPROMWrite8(uint8_t addr, uint8_t val);
  void EEPROMWrite(uint8_t addr, uint8_t *buf, uint8_t size);
  uint8_t EEPROMRead8(uint8_t addr);

  void setI2CAddr(uint8_t addr);
  uint8_t getI2CAddr();

  void UARTSetBaud(uint32_t baud);

  void setKeypadEvent(uint8_t key, uint8_t edge, bool enable = true);
  void enableKeypadInterrupt();
  void disableKeypadInterrupt();
  uint8_t getKeypadCount();
  bool readKeypad(keyEventRaw *buf, uint8_t count);

  float getTemp();

  int32_t getEncoderPosition(uint8_t encoder = 0);
  int32_t getEncoderDelta(uint8_t encoder = 0);
  bool enableEncoderInterrupt(uint8_t encoder = 0);
  bool disableEncoderInterrupt(uint8_t encoder = 0);
  void setEncoderPosition(int32_t pos, uint8_t encoder = 0);

  virtual size_t write(uint8_t);
  virtual size_t write(const char *str);

protected:
  TwoWire *_i2cbus; /*!< The I2C Bus used to communicate with the seesaw */
  Adafruit_I2CDevice *_i2c_dev = NULL; ///< The BusIO device for I2C control

  int8_t _flow; /*!< The flow control pin to use */

  uint8_t _hardwaretype = 0; /*!< what hardware type is attached! */
  uint8_t getI2CaddrEEPROMloc();

  bool write8(byte regHigh, byte regLow, byte value);
  uint8_t read8(byte regHigh, byte regLow, uint16_t delay = 250);

  bool read(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num,
            uint16_t delay = 250);
  bool write(uint8_t regHigh, uint8_t regLow, uint8_t *buf, uint8_t num);

  /*=========================================================================
          REGISTER BITFIELDS
      -----------------------------------------------------------------------*/

  /** Sercom interrupt enable register
   */
  union sercom_inten {
    struct {
      uint8_t DATA_RDY : 1; ///< this bit is set when data becomes available
    } bit;                  ///< bitfields
    uint8_t reg;            ///< full register
  };
  sercom_inten _sercom_inten; ///< sercom interrupt enable register instance

  /*=========================================================================*/
};

#endif
