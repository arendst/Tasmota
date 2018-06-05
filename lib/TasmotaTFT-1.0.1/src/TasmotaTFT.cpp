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

/*********************************************************************************************\
 * See original Adafruit ILI9341 license text below
 *
 * Based in Adafruit_ILI9341 v1.0.11
 * This is a library for the ILI9341, ILI9481 and ILI9486 TFT LCD using hardware SPI only
 *
 * These displays use hardware SPI to communicate, 4 or 5 pins are required to interface (RST is optional)
\*********************************************************************************************/

#include "TasmotaTFT.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

static inline void spi_begin(void) __attribute__((always_inline));
static inline void spi_begin(void) {
  SPI.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE0));
}
static inline void spi_end(void) __attribute__((always_inline));
static inline void spi_end(void) {
  SPI.endTransaction();
}

struct _sSize
{
  int16_t   width;
  int16_t   height;
};
_sSize _size[TFT_MAX] = {
  {240, 320},    // ILI9340 / ILI9341
  {320, 480},    // ILI9481 / HX8357B
  {320, 480}     // ILI9486
};

TasmotaTFT::TasmotaTFT(int8_t m, int8_t cs, int8_t dc, int8_t rst) : Adafruit_GFX(_size[m].width, _size[m].height) {
  _mod  = m;
  _w    = _size[m].width;
  _h    = _size[m].height;
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  _mosi = _sclk = 0;
}

void TasmotaTFT::writecommand(uint8_t c)
{
  digitalWrite(_dc, LOW);
  digitalWrite(_sclk, LOW);
  digitalWrite(_cs, LOW);
  SPI.transfer(c);
  digitalWrite(_cs, HIGH);
}

void TasmotaTFT::writedata(uint8_t c)
{
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  SPI.transfer(c);
  digitalWrite(_cs, HIGH);
}

void TasmotaTFT::begin(void)
{
  pinMode(_dc, OUTPUT);
  digitalWrite(_dc, LOW);
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  SPI.begin();

  // toggle RST low to reset
  if (_rst > 0) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(100);
    digitalWrite(_rst, HIGH);
    delay(200);
  }

  spi_begin();

  if (_mod == ILI9341) {       // 320 x 240
    writecommand(0xEF);        // Display Supplier dependant
    writedata(0x03);
    writedata(0x80);
    writedata(0x02);

    writecommand(0xCF);        //  Display Supplier dependant
    writedata(0x00);
    writedata(0XC1);
    writedata(0X30);

    writecommand(0xED);        //  Display Supplier dependant
    writedata(0x64);
    writedata(0x03);
    writedata(0X12);
    writedata(0X81);

    writecommand(0xE8);        //  Display Supplier dependant
    writedata(0x85);
    writedata(0x00);
    writedata(0x78);

    writecommand(0xCB);        //  Display Supplier dependant
    writedata(0x39);
    writedata(0x2C);
    writedata(0x00);
    writedata(0x34);
    writedata(0x02);

    writecommand(0xF7);        //  Display Supplier dependant
    writedata(0x20);

    writecommand(0xEA);        //  Display Supplier dependant
    writedata(0x00);
    writedata(0x00);

    writecommand(0xC0);        // Power control 1
    writedata(0x23);           // VRH[5:0]

    writecommand(0xC1);        // Power control 2
    writedata(0x10);           // SAP[2:0];BT[3:0]

    writecommand(0xC5);        // VCM control 1
    writedata(0x3e);           // Contrast
    writedata(0x28);

    writecommand(0xC7);        // VCM control 2
    writedata(0x86);           // --

    writecommand(0x36);        // Memory Access Control
    writedata(0x48);

    writecommand(0x3A);        // Pixel Format
    writedata(0x55);

    writecommand(0xB1);        // Frame Control in Normal Mode
    writedata(0x00);
    writedata(0x18);

    writecommand(0xB6);        // Display Function Control
    writedata(0x08);
    writedata(0x82);
    writedata(0x27);

    writecommand(0xF2);        // 3Gamma Function Disable
    writedata(0x00);

    writecommand(0x26);        // Gamma curve selected
    writedata(0x01);

    writecommand(0xE0);        // Positive Gamma
    writedata(0x0F);
    writedata(0x31);
    writedata(0x2B);
    writedata(0x0C);
    writedata(0x0E);
    writedata(0x08);
    writedata(0x4E);
    writedata(0xF1);
    writedata(0x37);
    writedata(0x07);
    writedata(0x10);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x09);
    writedata(0x00);

    writecommand(0xE1);        // Negative Gamma
    writedata(0x00);
    writedata(0x0E);
    writedata(0x14);
    writedata(0x03);
    writedata(0x11);
    writedata(0x07);
    writedata(0x31);
    writedata(0xC1);
    writedata(0x48);
    writedata(0x08);
    writedata(0x0F);
    writedata(0x0C);
    writedata(0x31);
    writedata(0x36);
    writedata(0x0F);

    writecommand(0x11);        // Exit Sleep
    spi_end();
    delay(120);
    spi_begin();
    writecommand(0x29);        // Display on
  }

  else if (_mod == ILI9481) {  // 480 x 320
    writecommand(0x11);        // Exit Sleep

    spi_end();
    delay(20);
    spi_begin();

    writecommand(0xD0);        // Power Setting
    writedata(0x07);
    writedata(0x42);
    writedata(0x18);

    writecommand(0xD1);        // VCOM Control
    writedata(0x00);
    writedata(0x07);
    writedata(0x10);

    writecommand(0xD2);        // Power Setting for Normal Mode
    writedata(0x01);
    writedata(0x02);

    writecommand(0xC0);        // Panel Driving Setting
    writedata(0x10);
    writedata(0x3B);
    writedata(0x00);
    writedata(0x02);
    writedata(0x11);

    writecommand(0xC5);        // Frame Rate and Inversion Control
    writedata(0x08);           // 0x03

    writecommand(0xC8);        // Gamma Setting
    writedata(0x00);
    writedata(0x32);
    writedata(0x36);
    writedata(0x45);
    writedata(0x06);
    writedata(0x16);
    writedata(0x37);
    writedata(0x75);
    writedata(0x77);
    writedata(0x54);
    writedata(0x0C);
    writedata(0x00);

    writecommand(0x36);        // Address Mode
    writedata(0x0A);

    writecommand(0x3A);        // Pixel Format
    writedata(0x55);

    writecommand(0x2A);        // Column Address
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0x3F);

    writecommand(0x2B);        // Page Address
    writedata(0x00);
    writedata(0x00);
    writedata(0x01);
    writedata(0xDF);           // 0xE0

    spi_end();
    delay(120);
    spi_begin();
    writecommand(0x29);        // Display on
  }

  else if (_mod == ILI9486) {  // 480 x 320
    writecommand(0x01);        // Soft reset
//    writedata(0x00);

    spi_end();
    delay(50);
    spi_begin();

    writecommand(0x28);        // Display off
//    writedata(0x00);

    writecommand(0xC0);        // Power Control 1
    writedata(0x0d);
    writedata(0x0d);

    writecommand(0xC1);        // Power Control 2
    writedata(0x43);
    writedata(0x00);

    writecommand(0xC2);        // Power Control 3
    writedata(0x00);

    writecommand(0xC5);        // VCOM Control
    writedata(0x00);
    writedata(0x48);

    writecommand(0xB6);        // Display Function Control
    writedata(0x00);
    writedata(0x22);           // 0x42 = Rotate display 180 deg.
    writedata(0x3B);

    writecommand(0xE0);        // PGAMCTRL (Positive Gamma Control)
    writedata(0x0f);
    writedata(0x24);
    writedata(0x1c);
    writedata(0x0a);
    writedata(0x0f);
    writedata(0x08);
    writedata(0x43);
    writedata(0x88);
    writedata(0x32);
    writedata(0x0f);
    writedata(0x10);
    writedata(0x06);
    writedata(0x0f);
    writedata(0x07);
    writedata(0x00);

    writecommand(0xE1);        // NGAMCTRL (Negative Gamma Control)
    writedata(0x0F);
    writedata(0x38);
    writedata(0x30);
    writedata(0x09);
    writedata(0x0f);
    writedata(0x0f);
    writedata(0x4e);
    writedata(0x77);
    writedata(0x3c);
    writedata(0x07);
    writedata(0x10);
    writedata(0x05);
    writedata(0x23);
    writedata(0x1b);
    writedata(0x00);

    writecommand(0x20);        // Display Inversion OFF, 0x21 = ON

    writecommand(0x36);        // Memory Access Control
    writedata(0x0A);

    writecommand(0x3A);        // Interface Pixel Format
    writedata(0x55);

    writecommand(0x11);        // Exit Sleep
    spi_end();
    delay(150);
    spi_begin();
    writecommand(0x29);        // Display on
  }

  spi_end();
}

void TasmotaTFT::setScrollStart(uint16_t start)
{
  spi_begin();
  writecommand(0x37);
  writedata(start>>8);
  writedata(start);
  spi_end();
}

void TasmotaTFT::setScrollMargins(uint16_t top, uint16_t bottom)
{
  uint16_t height = _height - (top + bottom);

  spi_begin();
  writecommand(0x33);
  writedata(top>>8);
  writedata(top);
  writedata(height>>8);
  writedata(height);
  writedata(bottom>>8);
  writedata(bottom);
  spi_end();
}

void TasmotaTFT::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  writecommand(TFT_CASET);     // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);        // XSTART
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);        // XEND

  writecommand(TFT_PASET);     // Row addr set
  writedata(y0>>8);
  writedata(y0);               // YSTART
  writedata(y1>>8);
  writedata(y1);               // YEND

  writecommand(TFT_RAMWR);     // write to RAM
}

void TasmotaTFT::pushColor(uint16_t color)
{
  spi_begin();

  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  digitalWrite(_cs, HIGH);

  spi_end();
}

void TasmotaTFT::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  spi_begin();

  setAddrWindow(x,y,x+1,y+1);
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  SPI.transfer(color >> 8);
  SPI.transfer(color);
  digitalWrite(_cs, HIGH);

  spi_end();
}

void TasmotaTFT::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;

  spi_begin();

  setAddrWindow(x, y, x, y+h-1);
  uint8_t hi = color >> 8, lo = color;
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  while (h--) {
    SPI.transfer(hi);
    SPI.transfer(lo);
  }
  digitalWrite(_cs, HIGH);

  spi_end();
}

void TasmotaTFT::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;

  spi_begin();

  setAddrWindow(x, y, x+w-1, y);
  uint8_t hi = color >> 8, lo = color;
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  while (w--) {
    SPI.transfer(hi);
    SPI.transfer(lo);
  }
  digitalWrite(_cs, HIGH);

  spi_end();
}

void TasmotaTFT::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void TasmotaTFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  spi_begin();

  setAddrWindow(x, y, x+w-1, y+h-1);
  uint8_t hi = color >> 8, lo = color;
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      SPI.transfer(hi);
      SPI.transfer(lo);
    }
  }
  digitalWrite(_cs, HIGH);

  spi_end();
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t TasmotaTFT::color565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void TasmotaTFT::setRotation(uint8_t m)
{
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     m = (MADCTL_MX | MADCTL_BGR);
     _width  = _w;
     _height = _h;
     break;
   case 1:
     m = (MADCTL_MV | MADCTL_BGR);
     _width  = _h;
     _height = _w;
     break;
  case 2:
    m = (MADCTL_MY | MADCTL_BGR);
     _width  = _w;
     _height = _h;
    break;
   case 3:
     m = (MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     _width  = _h;
     _height = _w;
     break;
  }

  spi_begin();
  writecommand(TFT_MADCTL);
  writedata(m);
  spi_end();
}

void TasmotaTFT::invertDisplay(boolean i)
{
  spi_begin();
  writecommand(i ? TFT_INVON : TFT_INVOFF);
  spi_end();
}

void TasmotaTFT::showDisplay(boolean i)
{
  spi_begin();
  writecommand(i ? TFT_DISPON : TFT_DISPOFF);
  spi_end();
}

uint8_t TasmotaTFT::readdata(void)
{
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, LOW);
  uint8_t r = SPI.transfer(0x00);
  digitalWrite(_cs, HIGH);

  return r;
}

uint8_t TasmotaTFT::readcommand8(uint8_t c, uint8_t index)
{
  spi_begin();

  digitalWrite(_dc, LOW);      // command
  digitalWrite(_cs, LOW);
  SPI.transfer(0xD9);          // woo sekret command?
  digitalWrite(_dc, HIGH);     // data
  SPI.transfer(0x10 + index);
  digitalWrite(_cs, HIGH);

  digitalWrite(_dc, LOW);      // command
  digitalWrite(_sclk, LOW);
  digitalWrite(_cs, LOW);
  SPI.transfer(c);

  digitalWrite(_dc, HIGH);     // data
  uint8_t r = SPI.transfer(0x00);
  digitalWrite(_cs, HIGH);

  spi_end();
  return r;
}

/***************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
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
