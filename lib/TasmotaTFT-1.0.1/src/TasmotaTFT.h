/*
  TasmotaTFT.cpp - Adapted implementation of Adafruit ILI9341  for Tasmota

  Copyright (C) 2018 Adafruit and Theo Arends

  This library is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _TasmotaTFT_h
#define _TasmotaTFT_h
/*********************************************************************************************\
 * See original Adafruit ILI9341 license text below
 *
 * This is a library for the ILI9341, ILI9481 and ILI9486 TFT LCD
 *
 * These displays use SPI to communicate, 4 or 5 pins are required to interface (RST is optional)
\*********************************************************************************************/

#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>

// Supported TFT LCD drivers
#define ILI9340         0     // 320 x 240
#define ILI9341         0     // 320 x 240
#define ILI9481         1     // 480 x 320
#define HX8357B         1     // 480 x 320
#define ILI9486         2     // 480 x 320
#define TFT_MAX         3

// Common driver registers
#define TFT_RDMODE      0x0A   // Get Power Mode
#define TFT_RDMADCTL    0x0B   // Get Address Mode
#define TFT_RDPIXFMT    0x0C   // Get Pixel Format
#define TFT_RDIMGFMT    0x0D   // Get Display Mode
#define TFT_RDSELFDIAG  0x0F   // Get Diagnostic Result

#define TFT_INVOFF      0x20   // Exit Invert Mode
#define TFT_INVON       0x21   // Enter Invert Mode
#define TFT_DISPOFF     0x28   // Set Display Off
#define TFT_DISPON      0x29   // Set Display on

#define TFT_CASET       0x2A   // Set Column Address
#define TFT_PASET       0x2B   // Set Page Address
#define TFT_RAMWR       0x2C   // Write Memory Start

#define TFT_MADCTL      0x36   // Set Address Mode

// Color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F

class TasmotaTFT : public Adafruit_GFX {
  public:
    TasmotaTFT(int8_t _MODEL, int8_t _CS, int8_t _DC, int8_t _RST = -1);
    void begin(void);
    void setScrollStart(uint16_t start);
    void setScrollMargins(uint16_t top, uint16_t bottom);
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void pushColor(uint16_t color);
    void fillScreen(uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setRotation(uint8_t r);
    void invertDisplay(boolean i);
    void showDisplay(boolean i);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */
    uint8_t readdata(void);
    uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

    void spiwrite(uint8_t);
    void writecommand(uint8_t c);
    void writedata(uint8_t d);
    void commandList(uint8_t *addr);
    uint8_t spiread(void);

  private:
    uint8_t tabcolor;
    uint8_t _mod;
    int16_t _w;
    int16_t _h;
    int32_t _cs;
    int32_t _dc;
    int32_t _rst;
    int32_t _mosi;
    int32_t _miso;
    int32_t _sclk;
};

#endif  // _TasmotaTFT_h

/***************************************************
  This is our library for the Adafruit  ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
