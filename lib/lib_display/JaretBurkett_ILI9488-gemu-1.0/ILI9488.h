/***************************************************
  STM32 Support added by Jaret Burkett at OSHlab.com

  This is our library for the Adafruit  ILI9488 Breakout and Shield
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

#ifndef _ILI9488H_
#define _ILI9488H_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include <renderer.h>
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

#ifdef ARDUINO_STM32_FEATHER
typedef volatile uint32 RwReg;
#endif
#if defined (__AVR__) || defined(TEENSYDUINO) || defined (__arm__) || defined (__STM32F1__)
#define USE_FAST_PINIO
#endif

#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN   0x10
#define ILI9488_SLPOUT  0x11
#define ILI9488_PTLON   0x12
#define ILI9488_NORON   0x13

#define ILI9488_RDMODE  0x0A
#define ILI9488_RDMADCTL  0x0B
#define ILI9488_RDPIXFMT  0x0C
#define ILI9488_RDIMGFMT  0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF 0x28
#define ILI9488_DISPON  0x29

#define ILI9488_CASET   0x2A
#define ILI9488_PASET   0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_RAMRD   0x2E

#define ILI9488_PTLAR   0x30
#define ILI9488_MADCTL  0x36
#define ILI9488_PIXFMT  0x3A

#define ILI9488_WRCTRLD 0x53
#define ILI9488_CAPC9   0xCF

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1  0xC0
#define ILI9488_PWCTR2  0xC1
#define ILI9488_PWCTR3  0xC2
#define ILI9488_PWCTR4  0xC3
#define ILI9488_PWCTR5  0xC4
#define ILI9488_VMCTR1  0xC5
#define ILI9488_VMCTR2  0xC7

#define ILI9488_RDID1   0xDA
#define ILI9488_RDID2   0xDB
#define ILI9488_RDID3   0xDC
#define ILI9488_RDID4   0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1
/*
#define ILI9488_PWCTR6  0xFC

*/

#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308

// Color definitions
#define ILI9488_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9488_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9488_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9488_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9488_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9488_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9488_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9488_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9488_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9488_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9488_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9488_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9488_RED         0xF800      /* 255,   0,   0 */
#define ILI9488_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9488_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9488_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9488_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9488_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9488_PINK        0xF81F




class ILI9488 : public Renderer {

 public:

  ILI9488(int8_t cs,int8_t mosi,int8_t sclk,int8_t bp);

  void begin(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void setAddrWindow_int(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea);
  void scroll(uint16_t pixels);
  void pushColor(uint16_t color);
  void pushColors(uint16_t *data, uint16_t len, boolean first);
  //void drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void fillScreen(uint16_t color);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
  void setRotation(uint8_t r);
  void invertDisplay(boolean i);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
  uint16_t GetColorFromIndex(uint8_t index);
  void DisplayOnff(int8_t on);
  void writecommand(uint8_t c);
  void writedata(uint8_t d);
  void write16BitColor(uint16_t color);
  void commandList(uint8_t *addr);
  void hw_spi_init();
  void dim(uint8_t contrast);

 private:
  uint8_t  tabcolor;
  uint8_t dimmer;
  void fastSPIwrite(uint8_t d,uint8_t dc);
  void spi_lcd_mode_init(void);
  void start(void);
  void stop(void);
  int8_t  _cs, _mosi, _sclk, _bp, _hwspi;

};

#endif
