/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver similar to SSD1306 so, just change the display() method.

However, SH1106 driver don't provide several functions such as scroll commands.


*********************************************************************/

//#include <avr/pgmspace.h>
#ifndef __SAM3X8E__
// #include <util/delay.h>
#endif
#include <stdlib.h>

#include <Wire.h>

#include "Adafruit_GFX.h"
#include "Adafruit_SH1106.h"

// the memory buffer for the LCD

static uint8_t buffer[SH1106_LCDHEIGHT * SH1106_LCDWIDTH / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0xFF,
#if (SH1106_LCDHEIGHT * SH1106_LCDWIDTH > 96*16)
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00,
0x80, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x8C, 0x8E, 0x84, 0x00, 0x00, 0x80, 0xF8,
0xF8, 0xF8, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0x80,
0x00, 0xE0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xC7, 0x01, 0x01,
0x01, 0x01, 0x83, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xFE, 0xC7, 0x01, 0x01, 0x01, 0x01, 0x83, 0xFF,
0xFF, 0xFF, 0x00, 0x38, 0xFE, 0xC7, 0x83, 0x01, 0x01, 0x01, 0x83, 0xC7, 0xFF, 0xFF, 0x00, 0x00,
0x01, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0xFF, 0xFF, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0xFF,
0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFF,
0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xC7, 0xC7, 0x8F,
0x8F, 0x9F, 0xBF, 0xFF, 0xFF, 0xC3, 0xC0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x00, 0x01, 0x03, 0x03, 0x03,
0x03, 0x03, 0x01, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01,
0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x00, 0x00,
0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x03,
0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#if (SH1106_LCDHEIGHT == 64)
0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F,
0x87, 0xC7, 0xF7, 0xFF, 0xFF, 0x1F, 0x1F, 0x3D, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0x7C, 0x7D, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x0F, 0x07, 0x00, 0x30, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xC0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F,
0x0F, 0x07, 0x1F, 0x7F, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0,
0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00,
0x00, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x0E, 0xFC, 0xF8, 0x00, 0x00, 0xF0, 0xF8, 0x1C, 0x0E,
0x06, 0x06, 0x06, 0x0C, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFC,
0xFE, 0xFC, 0x00, 0x18, 0x3C, 0x7E, 0x66, 0xE6, 0xCE, 0x84, 0x00, 0x00, 0x06, 0xFF, 0xFF, 0x06,
0x06, 0xFC, 0xFE, 0xFC, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0xC0, 0xF8,
0xFC, 0x4E, 0x46, 0x46, 0x46, 0x4E, 0x7C, 0x78, 0x40, 0x18, 0x3C, 0x76, 0xE6, 0xCE, 0xCC, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x1F, 0x0F, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x03, 0x07, 0x0E, 0x0C,
0x18, 0x18, 0x0C, 0x06, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x01, 0x0F, 0x0E, 0x0C, 0x18, 0x0C, 0x0F,
0x07, 0x01, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00,
0x00, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x07,
0x07, 0x0C, 0x0C, 0x18, 0x1C, 0x0C, 0x06, 0x06, 0x00, 0x04, 0x0E, 0x0C, 0x18, 0x0C, 0x0F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#endif
#endif
};

#define sh1106_swap(a, b) { int16_t t = a; a = b; b = t; }

// the most basic function, set a single pixel
void Adafruit_SH1106::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    sh1106_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    sh1106_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  // x is which column
    switch (color)
    {
      case WHITE:   buffer[x+ (y/8)*SH1106_LCDWIDTH] |=  (1 << (y&7)); break;
      case BLACK:   buffer[x+ (y/8)*SH1106_LCDWIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*SH1106_LCDWIDTH] ^=  (1 << (y&7)); break;
    }

}

Adafruit_SH1106::Adafruit_SH1106(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT) {
  cs = CS;
  rst = RST;
  dc = DC;
  sclk = SCLK;
  sid = SID;
  hwSPI = false;
}

// constructor for hardware SPI - we indicate DataCommand, ChipSelect, Reset
Adafruit_SH1106::Adafruit_SH1106(int8_t DC, int8_t RST, int8_t CS) : Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT) {
  dc = DC;
  rst = RST;
  cs = CS;
  hwSPI = true;
}

// initializer for I2C - we only indicate the reset pin!
Adafruit_SH1106::Adafruit_SH1106(int8_t reset) :
Adafruit_GFX(SH1106_LCDWIDTH, SH1106_LCDHEIGHT) {
  sclk = dc = cs = sid = -1;
  rst = reset;
}


void Adafruit_SH1106::begin(uint8_t vccstate, uint8_t i2caddr, bool reset) {
  _vccstate = vccstate;
  _i2caddr = i2caddr;
    // I2C Init
    Wire.begin();

   #if defined SH1106_128_32
    // Init sequence for 128x32 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x1F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x0);                                   // no offset
    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x02);
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    SH1106_command(0x8F);
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined SH1106_128_64
    // Init sequence for 128x64 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x3F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x00);                                   // no offset

    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x12);
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x9F); }
    else
      { SH1106_command(0xCF); }
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined SH1106_96_16
    // Init sequence for 96x16 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x0F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x00);                                   // no offset
    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x2);	//ada x12
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0xAF); }
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  SH1106_command(SH1106_DISPLAYON);//--turn on oled panel
}


void Adafruit_SH1106::invertDisplay(uint8_t i) {
  if (i) {
    SH1106_command(SH1106_INVERTDISPLAY);
  } else {
    SH1106_command(SH1106_NORMALDISPLAY);
  }
}

void Adafruit_SH1106::SH1106_command(uint8_t c) {

    // I2C
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(control);
    WIRE_WRITE(c);
    Wire.endTransmission();

}

// startscrollright
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
/*void Adafruit_SH1106::startscrollright(uint8_t start, uint8_t stop){
  SH1106_command(SH1106_RIGHT_HORIZONTAL_SCROLL);
  SH1106_command(0X00);
  SH1106_command(start);
  SH1106_command(0X00);
  SH1106_command(stop);
  SH1106_command(0X00);
  SH1106_command(0XFF);
  SH1106_command(SH1106_ACTIVATE_SCROLL);
}

// startscrollleft
// Activate a right handed scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void Adafruit_SH1106::startscrollleft(uint8_t start, uint8_t stop){
  SH1106_command(SH1106_LEFT_HORIZONTAL_SCROLL);
  SH1106_command(0X00);
  SH1106_command(start);
  SH1106_command(0X00);
  SH1106_command(stop);
  SH1106_command(0X00);
  SH1106_command(0XFF);
  SH1106_command(SH1106_ACTIVATE_SCROLL);
}

// startscrolldiagright
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void Adafruit_SH1106::startscrolldiagright(uint8_t start, uint8_t stop){
  SH1106_command(SH1106_SET_VERTICAL_SCROLL_AREA);
  SH1106_command(0X00);
  SH1106_command(SH1106_LCDHEIGHT);
  SH1106_command(SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  SH1106_command(0X00);
  SH1106_command(start);
  SH1106_command(0X00);
  SH1106_command(stop);
  SH1106_command(0X01);
  SH1106_command(SH1106_ACTIVATE_SCROLL);
}

// startscrolldiagleft
// Activate a diagonal scroll for rows start through stop
// Hint, the display is 16 rows tall. To scroll the whole display, run:
// display.scrollright(0x00, 0x0F)
void Adafruit_SH1106::startscrolldiagleft(uint8_t start, uint8_t stop){
  SH1106_command(SH1106_SET_VERTICAL_SCROLL_AREA);
  SH1106_command(0X00);
  SH1106_command(SH1106_LCDHEIGHT);
  SH1106_command(SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  SH1106_command(0X00);
  SH1106_command(start);
  SH1106_command(0X00);
  SH1106_command(stop);
  SH1106_command(0X01);
  SH1106_command(SH1106_ACTIVATE_SCROLL);
}

void Adafruit_SH1106::stopscroll(void){
  SH1106_command(SH1106_DEACTIVATE_SCROLL);
}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void Adafruit_SH1106::dim(boolean dim) {
  uint8_t contrast;

  if (dim) {
    contrast = 0; // Dimmed display
  } else {
    if (_vccstate == SH1106_EXTERNALVCC) {
      contrast = 0x9F;
    } else {
      contrast = 0xCF;
    }
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  SH1106_command(SH1106_SETCONTRAST);
  SH1106_command(contrast);
}*/

void Adafruit_SH1106::SH1106_data(uint8_t c) {

  if (sid != -1)
  {
    // SPI
    //digitalWrite(cs, HIGH);
    *csport |= cspinmask;
    //digitalWrite(dc, HIGH);
    *dcport |= dcpinmask;
    //digitalWrite(cs, LOW);
    *csport &= ~cspinmask;
    fastSPIwrite(c);
    //digitalWrite(cs, HIGH);
    *csport |= cspinmask;
  }
  else
  {
    // I2C
    uint8_t control = 0x40;   // Co = 0, D/C = 1
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(control);
    WIRE_WRITE(c);
    Wire.endTransmission();
  }

}
/*#define SH1106_SETLOWCOLUMN 0x00
#define SH1106_SETHIGHCOLUMN 0x10

#define SH1106_SETSTARTLINE 0x40*/

void Adafruit_SH1106::display(void) {

    SH1106_command(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
    SH1106_command(SH1106_SETHIGHCOLUMN | 0x0);  // hi col = 0
    SH1106_command(SH1106_SETSTARTLINE | 0x0); // line #0


    // I2C
    //height >>= 3;
    //width >>= 3;
	byte height=64;
	byte width=132;
	byte m_row = 0;
	byte m_col = 2;


	height >>= 3;
	width >>= 3;
	//Serial.println(width);

	int p = 0;

	byte i, j, k =0;

	for ( i = 0; i < height; i++) {

		// send a bunch of data in one xmission
        SH1106_command(0xB0 + i + m_row);//set page address
        SH1106_command(m_col & 0xf);//set lower column address
        SH1106_command(0x10 | (m_col >> 4));//set higher column address

        for( j = 0; j < 8; j++){
			Wire.beginTransmission(_i2caddr);
            Wire.write(0x40);
            for ( k = 0; k < width; k++, p++) {
		Wire.write(buffer[p]);
            }
            Wire.endTransmission();
        	}
	}

}

/*void Adafruit_SH1106::display(void) {
  SH1106_command(SH1106_COLUMNADDR);
  SH1106_command(0);   // Column start address (0 = reset)
  SH1106_command(SH1106_LCDWIDTH-1); // Column end address (127 = reset)

  SH1106_command(SH1106_PAGEADDR);
  SH1106_command(0); // Page start address (0 = reset)
  #if SH1106_LCDHEIGHT == 64
    SH1106_command(7); // Page end address
  #endif
  #if SH1106_LCDHEIGHT == 32
    SH1106_command(3); // Page end address
  #endif
  #if SH1106_LCDHEIGHT == 16
    SH1106_command(1); // Page end address
  #endif

  if (sid != -1)
  {
    // SPI
    *csport |= cspinmask;
    *dcport |= dcpinmask;
    *csport &= ~cspinmask;

    for (uint16_t i=0; i<(SH1106_LCDWIDTH*SH1106_LCDHEIGHT/8); i++) {
      fastSPIwrite(buffer[i]);
      //SH1106_data(buffer[i]);
    }
    *csport |= cspinmask;
  }
  else
  {
    // save I2C bitrate
#ifndef __SAM3X8E__
    uint8_t twbrbackup = TWBR;
    TWBR = 12; // upgrade to 400KHz!
#endif

    //Serial.println(TWBR, DEC);
    //Serial.println(TWSR & 0x3, DEC);

    // I2C
	int k = 0;
    for (uint16_t i=0; i<(SH1106_LCDWIDTH*SH1106_LCDHEIGHT/8); i++) {
      // send a bunch of data in one xmission
	  if(k < 2)
	  {
		k++;
		continue;
	  }
      Wire.beginTransmission(_i2caddr);
      WIRE_WRITE(0x40);
      for (uint8_t x=0; x<16; x++) {
  WIRE_WRITE(buffer[i]);
  i++;
      }
      i--;
      Wire.endTransmission();
    }
#ifndef __SAM3X8E__
    TWBR = twbrbackup;
#endif
  }
}
*/
// clear everything
void Adafruit_SH1106::clearDisplay(void) {
  memset(buffer, 0, (SH1106_LCDWIDTH*SH1106_LCDHEIGHT/8));
}


inline void Adafruit_SH1106::fastSPIwrite(uint8_t d) {

  if(hwSPI) {
    (void)SPI.transfer(d);
  } else {
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      *clkport &= ~clkpinmask;
      if(d & bit) *mosiport |=  mosipinmask;
      else        *mosiport &= ~mosipinmask;
      *clkport |=  clkpinmask;
    }
  }
  //*csport |= cspinmask;
}

void Adafruit_SH1106::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  boolean bSwap = false;
  switch(rotation) {
    case 0:
      // 0 degree rotation, do nothing
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x
      bSwap = true;
      sh1106_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      x -= (w-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
      bSwap = true;
      sh1106_swap(x, y);
      y = HEIGHT - y - 1;
      y -= (w-1);
      break;
  }

  if(bSwap) {
    drawFastVLineInternal(x, y, w, color);
  } else {
    drawFastHLineInternal(x, y, w, color);
  }
}

void Adafruit_SH1106::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Do bounds/limit checks
  if(y < 0 || y >= HEIGHT) { return; }

  // make sure we don't try to draw below 0
  if(x < 0) {
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if( (x + w) > WIDTH) {
    w = (WIDTH - x);
  }

  // if our width is now negative, punt
  if(w <= 0) { return; }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SH1106_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y&7);

  switch (color)
  {
  case WHITE:         while(w--) { *pBuf++ |= mask; }; break;
    case BLACK: mask = ~mask;   while(w--) { *pBuf++ &= mask; }; break;
  case INVERSE:         while(w--) { *pBuf++ ^= mask; }; break;
  }
}

void Adafruit_SH1106::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  bool bSwap = false;
  switch(rotation) {
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      sh1106_swap(x, y);
      x = WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y
      bSwap = true;
      sh1106_swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  if(bSwap) {
    drawFastHLineInternal(x, y, h, color);
  } else {
    drawFastVLineInternal(x, y, h, color);
  }
}


void Adafruit_SH1106::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  }

  // make sure we don't go past the height of the display
  if( (__y + __h) > HEIGHT) {
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt
  if(__h <= 0) {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * SH1106_LCDWIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) {
      mask &= (0XFF >> (mod-h));
    }

  switch (color)
    {
    case WHITE:   *pBuf |=  mask;  break;
    case BLACK:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += SH1106_LCDWIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) {
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += SH1106_LCDWIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color)
    {
      case WHITE:   *pBuf |=  mask;  break;
      case BLACK:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
}
