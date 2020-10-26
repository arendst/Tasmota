/***************************************************
  This is a library for the ST7789 IPS SPI display.

  Originally written by Limor Fried/Ladyada for
  Adafruit Industries.

  Modified by Ananev Ilia
 ****************************************************/

#ifndef _ADAFRUIT_ST7789H_
#define _ADAFRUIT_ST7789H_

#include "Arduino.h"
#include "Print.h"
#include <Adafruit_GFX.h>
#include <renderer.h>

#if defined(__AVR__) || defined(CORE_TEENSY)
  #include <avr/pgmspace.h>
  #define USE_FAST_IO
  typedef volatile uint8_t RwReg;
#elif defined(ARDUINO_STM32_FEATHER)
  typedef volatile uint32 RwReg;
  #define USE_FAST_IO
#elif defined(ARDUINO_FEATHER52)
  typedef volatile uint32_t RwReg;
  #define USE_FAST_IO
#elif defined(ESP8266)
  #include <pgmspace.h>
#elif defined(__SAM3X8E__)
  #undef __FlashStringHelper::F(string_literal)
  #define F(string_literal) string_literal
  #include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
#endif

//#define SPI_HAS_TRANSACTION // already defined in SPI.h

#define ST7789_TFTWIDTH 	240
#define ST7789_TFTHEIGHT 	240

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 0

#define ST_CMD_DELAY   0x80    // special signifier for command lists

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD

// Color definitions
#undef BLACK
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#undef WHITE
#define WHITE   0xFFFF


// Color definitions
#define ST7789_BLACK       0x0000      /*   0,   0,   0 */
#define ST7789_NAVY        0x000F      /*   0,   0, 128 */
#define ST7789_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ST7789_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ST7789_MAROON      0x7800      /* 128,   0,   0 */
#define ST7789_PURPLE      0x780F      /* 128,   0, 128 */
#define ST7789_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ST7789_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ST7789_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ST7789_BLUE        0x001F      /*   0,   0, 255 */
#define ST7789_GREEN       0x07E0      /*   0, 255,   0 */
#define ST7789_CYAN        0x07FF      /*   0, 255, 255 */
#define ST7789_RED         0xF800      /* 255,   0,   0 */
#define ST7789_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ST7789_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ST7789_WHITE       0xFFFF      /* 255, 255, 255 */
#define ST7789_ORANGE      0xFD20      /* 255, 165,   0 */
#define ST7789_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ST7789_PINK        0xF81F


class Arduino_ST7789 : public Renderer {

 public:

  Arduino_ST7789(int8_t DC, int8_t RST, int8_t SID, int8_t SCLK, int8_t CS, int8_t bp);
  Arduino_ST7789(int8_t DC, int8_t RST, int8_t CS, int8_t bp);

  void     setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
           pushColor(uint16_t color),
           fillScreen(uint16_t color),
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(boolean i),
           DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font),
           setAddrWindow_int(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),
		       init(uint16_t width, uint16_t height);
  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
  uint16_t GetColorFromIndex(uint8_t index);
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return Color565(r, g, b); }
  void DisplayOnff(int8_t on);
  void dim(uint8_t contrast);
  void pushColors(uint16_t *data, uint8_t len, boolean first);

 protected:
  uint8_t  _colstart, _rowstart, _xstart, _ystart; // some displays need this changed

  void     displayInit(const uint8_t *addr);
  void     spiwrite(uint8_t),
           writecommand(uint8_t c),
           writedata(uint8_t d),
           commonInit(const uint8_t *cmdList);

 private:

  inline void CS_HIGH(void);
  inline void CS_LOW(void);
  inline void DC_HIGH(void);
  inline void DC_LOW(void);

  boolean  _hwSPI;
  boolean  _SPI9bit;
  boolean  _DCbit;
  uint8_t dimmer;
  int8_t  _cs, _dc, _rst, _sid, _sclk, _bp;

#if defined(USE_FAST_IO)
  volatile RwReg  *dataport, *clkport, *csport, *dcport;

  #if defined(__AVR__) || defined(CORE_TEENSY)  // 8 bit!
    uint8_t  datapinmask, clkpinmask, cspinmask, dcpinmask;
  #else    // 32 bit!
    uint32_t  datapinmask, clkpinmask, cspinmask, dcpinmask;
  #endif
#endif

};

#endif
