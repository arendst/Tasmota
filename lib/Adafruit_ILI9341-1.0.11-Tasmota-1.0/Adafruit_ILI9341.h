/******************************************************************
  This is our library for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required
  to interface (RST is optional)
  Adafruit invests time and resources providing this open source
  code, please support Adafruit and open-source hardware by
  purchasing products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 *******************************************************************/

#ifndef _ADAFRUIT_ILI9341H_
#define _ADAFRUIT_ILI9341H_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include <SPI.h>
#include "Adafruit_GFX.h"

#if defined(ARDUINO_STM32_FEATHER)
typedef volatile uint32 RwReg;
#endif
#if defined(ARDUINO_FEATHER52)
typedef volatile uint32_t RwReg;
#endif

#define ILI9341_TFTWIDTH   240
#define ILI9341_TFTHEIGHT  320

#define ILI9341_NOP        0x00
#define ILI9341_SWRESET    0x01
#define ILI9341_RDDID      0x04
#define ILI9341_RDDST      0x09

#define ILI9341_SLPIN      0x10
#define ILI9341_SLPOUT     0x11
#define ILI9341_PTLON      0x12
#define ILI9341_NORON      0x13

#define ILI9341_RDMODE     0x0A
#define ILI9341_RDMADCTL   0x0B
#define ILI9341_RDPIXFMT   0x0C
#define ILI9341_RDIMGFMT   0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF     0x20
#define ILI9341_INVON      0x21
#define ILI9341_GAMMASET   0x26
#define ILI9341_DISPOFF    0x28
#define ILI9341_DISPON     0x29

#define ILI9341_CASET      0x2A
#define ILI9341_PASET      0x2B
#define ILI9341_RAMWR      0x2C
#define ILI9341_RAMRD      0x2E

#define ILI9341_PTLAR      0x30
#define ILI9341_MADCTL     0x36
#define ILI9341_VSCRSADD   0x37
#define ILI9341_PIXFMT     0x3A

#define ILI9341_FRMCTR1    0xB1
#define ILI9341_FRMCTR2    0xB2
#define ILI9341_FRMCTR3    0xB3
#define ILI9341_INVCTR     0xB4
#define ILI9341_DFUNCTR    0xB6

#define ILI9341_PWCTR1     0xC0
#define ILI9341_PWCTR2     0xC1
#define ILI9341_PWCTR3     0xC2
#define ILI9341_PWCTR4     0xC3
#define ILI9341_PWCTR5     0xC4
#define ILI9341_VMCTR1     0xC5
#define ILI9341_VMCTR2     0xC7

#define ILI9341_RDID1      0xDA
#define ILI9341_RDID2      0xDB
#define ILI9341_RDID3      0xDC
#define ILI9341_RDID4      0xDD

#define ILI9341_GMCTRP1    0xE0
#define ILI9341_GMCTRN1    0xE1
/*
#define ILI9341_PWCTR6     0xFC

 */

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F

#if defined (ARDUINO_STM32_FEATHER)    // doesnt work on wiced feather
  #undef USE_FAST_PINIO
#elif defined (__AVR__) || defined(TEENSYDUINO) || defined(ESP8266) || defined (ESP32) || defined(__arm__)
  #define USE_FAST_PINIO
#endif

class Adafruit_ILI9341 : public Adafruit_GFX {
    protected:

    public:
        Adafruit_ILI9341(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
        Adafruit_ILI9341(int8_t _CS, int8_t _DC, int8_t _RST = -1);

#ifndef ESP32
        void      begin(uint32_t freq = 0);
#else
        void      begin(uint32_t freq = 0, SPIClass &spi=SPI);
#endif

        void      setScrollMargins(uint16_t top, uint16_t bottom);

        void      setRotation(uint8_t r);
        void      invertDisplay(boolean i);
        void      scrollTo(uint16_t y);

        // Required Non-Transaction
        void      drawPixel(int16_t x, int16_t y, uint16_t color);

        // Transaction API
        void      startWrite(void);
        void      endWrite(void);
        void      writePixel(int16_t x, int16_t y, uint16_t color);
        void      writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void      writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void      writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

        // Transaction API not used by GFX
        void      setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        void      writePixel(uint16_t color);
        void      writePixels(uint16_t * colors, uint32_t len);
        void      writeColor(uint16_t color, uint32_t len);
	void      pushColor(uint16_t color);

        // Recommended Non-Transaction
        void      drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void      drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void      fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

        using     Adafruit_GFX::drawRGBBitmap; // Check base class first
        void      drawRGBBitmap(int16_t x, int16_t y,
                    uint16_t *pcolors, int16_t w, int16_t h);

        uint8_t   readcommand8(uint8_t reg, uint8_t index = 0);

        uint16_t  color565(uint8_t r, uint8_t g, uint8_t b);

    private:
#ifdef ESP32
        SPIClass _spi;
#endif
        uint32_t _freq;
#if defined (__AVR__) || defined(TEENSYDUINO)
        int8_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile uint8_t *mosiport, *misoport, *clkport, *dcport, *csport;
        uint8_t  mosipinmask, misopinmask, clkpinmask, cspinmask, dcpinmask;
#endif
#elif defined (__arm__)
        int32_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile RwReg *mosiport, *misoport, *clkport, *dcport, *csport;
        uint32_t  mosipinmask, misopinmask, clkpinmask, cspinmask, dcpinmask;
#endif
#elif defined (ESP8266) || defined (ESP32)
        int8_t   _cs, _dc, _rst, _sclk, _mosi, _miso;
#ifdef USE_FAST_PINIO
        volatile uint32_t *mosiport, *misoport, *clkport, *dcport, *csport;
        uint32_t  mosipinmask, misopinmask, clkpinmask, cspinmask, dcpinmask;
#endif
#else
        int8_t      _cs, _dc, _rst, _sclk, _mosi, _miso;
#endif

        void        writeCommand(uint8_t cmd);
        void        spiWrite(uint8_t v);
        uint8_t     spiRead(void);
};

#endif
