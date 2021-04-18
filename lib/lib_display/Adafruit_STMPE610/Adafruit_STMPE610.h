/*!
 *
 *  @file Adafruit_STMPE610.cpp
 *
 *  This is a library for the Adafruit STMPE610 Resistive
 *  touch screen controller breakout
 *  ----> http://www.adafruit.com/products/1571
 *
 *  Check out the links above for our tutorials and wiring diagrams
 *  These breakouts use SPI or I2C to communicate
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 *  MIT license, all text above must be included in any redistribution
 */

#ifndef _ADAFRUIT_STMPE610H_
#define _ADAFRUIT_STMPE610H_

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>

/** STMPE610 Address **/
#define STMPE_ADDR 0x41

/** Reset Control **/
#define STMPE_SYS_CTRL1 0x03
#define STMPE_SYS_CTRL1_RESET 0x02

/** Clock Contrl **/
#define STMPE_SYS_CTRL2 0x04

/** Touchscreen controller setup **/
#define STMPE_TSC_CTRL 0x40
#define STMPE_TSC_CTRL_EN 0x01
#define STMPE_TSC_CTRL_XYZ 0x00
#define STMPE_TSC_CTRL_XY 0x02

/** Interrupt control **/
#define STMPE_INT_CTRL 0x09
#define STMPE_INT_CTRL_POL_HIGH 0x04
#define STMPE_INT_CTRL_POL_LOW 0x00
#define STMPE_INT_CTRL_EDGE 0x02
#define STMPE_INT_CTRL_LEVEL 0x00
#define STMPE_INT_CTRL_ENABLE 0x01
#define STMPE_INT_CTRL_DISABLE 0x00

/** Interrupt enable **/
#define STMPE_INT_EN 0x0A
#define STMPE_INT_EN_TOUCHDET 0x01
#define STMPE_INT_EN_FIFOTH 0x02
#define STMPE_INT_EN_FIFOOF 0x04
#define STMPE_INT_EN_FIFOFULL 0x08
#define STMPE_INT_EN_FIFOEMPTY 0x10
#define STMPE_INT_EN_ADC 0x40
#define STMPE_INT_EN_GPIO 0x80

/** Interrupt status **/
#define STMPE_INT_STA 0x0B
#define STMPE_INT_STA_TOUCHDET 0x01

/** ADC control **/
#define STMPE_ADC_CTRL1 0x20
#define STMPE_ADC_CTRL1_12BIT 0x08
#define STMPE_ADC_CTRL1_10BIT 0x00

/** ADC control **/
#define STMPE_ADC_CTRL2 0x21
#define STMPE_ADC_CTRL2_1_625MHZ 0x00
#define STMPE_ADC_CTRL2_3_25MHZ 0x01
#define STMPE_ADC_CTRL2_6_5MHZ 0x02

/** Touchscreen controller configuration **/
#define STMPE_TSC_CFG 0x41
#define STMPE_TSC_CFG_1SAMPLE 0x00
#define STMPE_TSC_CFG_2SAMPLE 0x40
#define STMPE_TSC_CFG_4SAMPLE 0x80
#define STMPE_TSC_CFG_8SAMPLE 0xC0
#define STMPE_TSC_CFG_DELAY_10US 0x00
#define STMPE_TSC_CFG_DELAY_50US 0x08
#define STMPE_TSC_CFG_DELAY_100US 0x10
#define STMPE_TSC_CFG_DELAY_500US 0x18
#define STMPE_TSC_CFG_DELAY_1MS 0x20
#define STMPE_TSC_CFG_DELAY_5MS 0x28
#define STMPE_TSC_CFG_DELAY_10MS 0x30
#define STMPE_TSC_CFG_DELAY_50MS 0x38
#define STMPE_TSC_CFG_SETTLE_10US 0x00
#define STMPE_TSC_CFG_SETTLE_100US 0x01
#define STMPE_TSC_CFG_SETTLE_500US 0x02
#define STMPE_TSC_CFG_SETTLE_1MS 0x03
#define STMPE_TSC_CFG_SETTLE_5MS 0x04
#define STMPE_TSC_CFG_SETTLE_10MS 0x05
#define STMPE_TSC_CFG_SETTLE_50MS 0x06
#define STMPE_TSC_CFG_SETTLE_100MS 0x07

/** FIFO level to generate interrupt **/
#define STMPE_FIFO_TH 0x4A

/** Current filled level of FIFO **/
#define STMPE_FIFO_SIZE 0x4C

/** Current status of FIFO **/
#define STMPE_FIFO_STA 0x4B
#define STMPE_FIFO_STA_RESET 0x01
#define STMPE_FIFO_STA_OFLOW 0x80
#define STMPE_FIFO_STA_FULL 0x40
#define STMPE_FIFO_STA_EMPTY 0x20
#define STMPE_FIFO_STA_THTRIG 0x10

/** Touchscreen controller drive I **/
#define STMPE_TSC_I_DRIVE 0x58
#define STMPE_TSC_I_DRIVE_20MA 0x00
#define STMPE_TSC_I_DRIVE_50MA 0x01

/** Data port for TSC data address **/
#define STMPE_TSC_DATA_X 0x4D
#define STMPE_TSC_DATA_Y 0x4F
#define STMPE_TSC_FRACTION_Z 0x56

/** GPIO **/
#define STMPE_GPIO_SET_PIN 0x10
#define STMPE_GPIO_CLR_PIN 0x11
#define STMPE_GPIO_DIR 0x13
#define STMPE_GPIO_ALT_FUNCT 0x17

/*!
 *  @brief  Class for working with points
 */
class TS_Point {
public:
  TS_Point();
  TS_Point(int16_t x, int16_t y, int16_t z);

  bool operator==(TS_Point);
  bool operator!=(TS_Point);

  int16_t x; /**< x coordinate **/
  int16_t y; /**< y coordinate **/
  int16_t z; /**< z coordinate **/
};

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          STMPE610
 */
class Adafruit_STMPE610 {
public:
  Adafruit_STMPE610(uint8_t cspin, uint8_t mosipin, uint8_t misopin,
                    uint8_t clkpin);
  Adafruit_STMPE610(uint8_t cspin, SPIClass *theSPI = &SPI);
  Adafruit_STMPE610(TwoWire *theWire = &Wire);

  boolean begin(uint8_t i2caddr = STMPE_ADDR);

  void writeRegister8(uint8_t reg, uint8_t val);
  uint16_t readRegister16(uint8_t reg);
  uint8_t readRegister8(uint8_t reg);
  void readData(uint16_t *x, uint16_t *y, uint8_t *z);
  uint16_t getVersion();
  boolean touched();
  boolean bufferEmpty();
  uint8_t bufferSize();
  TS_Point getPoint();

private:
  uint8_t spiIn();
  void spiOut(uint8_t x);

  TwoWire *_wire;
  SPIClass *_spi;
  int8_t _CS, _MOSI, _MISO, _CLK;
  uint8_t _i2caddr;

  int m_spiMode;
};

#endif
