/*!
 * @file Adafruit_SSD1331.cpp
 *
 * @mainpage Adafruit SSD1331 Arduino Library
 *
 * @section intro_sec Introduction
 *
 * This is a library for the 0.96" 16-bit Color OLED with SSD1331 driver chip
 *
 * Pick one up today in the adafruit shop!
 * ------> http://www.adafruit.com/products/684
 *
 * These displays use SPI to communicate, 4 or 5 pins are required to
 * interface
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text above must be included in any redistribution
 */

#include "Adafruit_SSD1331.h"
#include "pins_arduino.h"
#include "wiring_private.h"

/***********************************/

/*!
  @brief   SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner x coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
void Adafruit_SSD1331::setAddrWindow(uint16_t x, uint16_t y, uint16_t w,
                                     uint16_t h) {

  uint8_t x1 = x;
  uint8_t y1 = y;
  if (x1 > 95)
    x1 = 95;
  if (y1 > 63)
    y1 = 63;

  uint8_t x2 = (x + w - 1);
  uint8_t y2 = (y + h - 1);
  if (x2 > 95)
    x2 = 95;
  if (y2 > 63)
    y2 = 63;

  if (x1 > x2) {
    uint8_t t = x2;
    x2 = x1;
    x1 = t;
  }
  if (y1 > y2) {
    uint8_t t = y2;
    y2 = y1;
    y1 = t;
  }

  sendCommand(0x15); // Column addr set
  sendCommand(x1);
  sendCommand(x2);

  sendCommand(0x75); // Column addr set
  sendCommand(y1);
  sendCommand(y2);

  startWrite();
}

/**************************************************************************/
/*!
    @brief   Initialize SSD1331 chip
    Connects to the SSD1331 over SPI and sends initialization procedure commands
    @param    freq  Desired SPI clock frequency
*/
/**************************************************************************/
void Adafruit_SSD1331::begin(uint32_t freq) {
  initSPI(freq);

  // Initialization Sequence
  sendCommand(SSD1331_CMD_DISPLAYOFF); // 0xAE
  sendCommand(SSD1331_CMD_SETREMAP);   // 0xA0
#if defined SSD1331_COLORORDER_RGB
  sendCommand(0x72); // RGB Color
#else
  sendCommand(0x76); // BGR Color
#endif
  sendCommand(SSD1331_CMD_STARTLINE); // 0xA1
  sendCommand(0x0);
  sendCommand(SSD1331_CMD_DISPLAYOFFSET); // 0xA2
  sendCommand(0x0);
  sendCommand(SSD1331_CMD_NORMALDISPLAY); // 0xA4
  sendCommand(SSD1331_CMD_SETMULTIPLEX);  // 0xA8
  sendCommand(0x3F);                      // 0x3F 1/64 duty
  sendCommand(SSD1331_CMD_SETMASTER);     // 0xAD
  sendCommand(0x8E);
  sendCommand(SSD1331_CMD_POWERMODE); // 0xB0
  sendCommand(0x0B);
  sendCommand(SSD1331_CMD_PRECHARGE); // 0xB1
  sendCommand(0x31);
  sendCommand(SSD1331_CMD_CLOCKDIV); // 0xB3
  sendCommand(0xF0); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio
                     // (A[3:0]+1 = 1..16)
  sendCommand(SSD1331_CMD_PRECHARGEA); // 0x8A
  sendCommand(0x64);
  sendCommand(SSD1331_CMD_PRECHARGEB); // 0x8B
  sendCommand(0x78);
  sendCommand(SSD1331_CMD_PRECHARGEC); // 0x8C
  sendCommand(0x64);
  sendCommand(SSD1331_CMD_PRECHARGELEVEL); // 0xBB
  sendCommand(0x3A);
  sendCommand(SSD1331_CMD_VCOMH); // 0xBE
  sendCommand(0x3E);
  sendCommand(SSD1331_CMD_MASTERCURRENT); // 0x87
  sendCommand(0x06);
  sendCommand(SSD1331_CMD_CONTRASTA); // 0x81
  sendCommand(0x91);
  sendCommand(SSD1331_CMD_CONTRASTB); // 0x82
  sendCommand(0x50);
  sendCommand(SSD1331_CMD_CONTRASTC); // 0x83
  sendCommand(0x7D);
  sendCommand(SSD1331_CMD_DISPLAYON); //--turn on oled panel
  _width = TFTWIDTH;
  _height = TFTHEIGHT;
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with software SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    mosi  SPI MOSI pin #
    @param    sclk  SPI Clock pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_SSD1331::Adafruit_SSD1331(int8_t cs, int8_t dc, int8_t mosi,
                                   int8_t sclk, int8_t rst)
    : Adafruit_SPITFT_Renderer(TFTWIDTH, TFTHEIGHT, cs, dc, mosi, sclk, rst, -1) {}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with hardware SPI
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_SSD1331::Adafruit_SSD1331(int8_t cs, int8_t dc, int8_t rst)
    : Adafruit_SPITFT_Renderer(TFTWIDTH, TFTHEIGHT, cs, dc, rst) {}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SSD1331 driver with hardware SPI
    @param    spi   Pointer to an existing SPIClass instance (e.g. &SPI, the
                    microcontroller's primary SPI bus).
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_SSD1331::Adafruit_SSD1331(SPIClass *spi, int8_t cs, int8_t dc,
                                   int8_t rst)
    :
#if defined(ESP8266)
      Adafruit_SPITFT_Renderer(TFTWIDTH, TFTWIDTH, cs, dc, rst) {
#else
      Adafruit_SPITFT_Renderer(TFTWIDTH, TFTWIDTH, spi, cs, dc, rst) {
#endif
}

/**************************************************************************/
/*!
    @brief  Change whether display is on or off
    @param	 enable True if you want the display ON, false OFF
*/
/**************************************************************************/
void Adafruit_SSD1331::enableDisplay(boolean enable) {
  sendCommand(enable ? SSD1331_CMD_DISPLAYON : SSD1331_CMD_DISPLAYOFF);
}
