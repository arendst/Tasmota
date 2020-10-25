/***************************************************
  This is our library for the Adafruit  SSD1351 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 3 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _SSD1351_
#define _SSD1351_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <renderer.h>
#include <pgmspace.h>



#define SSD1351_WIDTH  128
#define SSD1351_HEIGHT 128


// Color definitions
#define SSD1351_BLACK       0x0000      /*   0,   0,   0 */
#define SSD1351_NAVY        0x000F      /*   0,   0, 128 */
#define SSD1351_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define SSD1351_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define SSD1351_MAROON      0x7800      /* 128,   0,   0 */
#define SSD1351_PURPLE      0x780F      /* 128,   0, 128 */
#define SSD1351_OLIVE       0x7BE0      /* 128, 128,   0 */
#define SSD1351_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define SSD1351_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define SSD1351_BLUE        0x001F      /*   0,   0, 255 */
#define SSD1351_GREEN       0x07E0      /*   0, 255,   0 */
#define SSD1351_CYAN        0x07FF      /*   0, 255, 255 */
#define SSD1351_RED         0xF800      /* 255,   0,   0 */
#define SSD1351_MAGENTA     0xF81F      /* 255,   0, 255 */
#define SSD1351_YELLOW      0xFFE0      /* 255, 255,   0 */
#define SSD1351_WHITE       0xFFFF      /* 255, 255, 255 */
#define SSD1351_ORANGE      0xFD20      /* 255, 165,   0 */
#define SSD1351_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define SSD1351_PINK        0xF81F

#define SSD1351_CMD_SETCOLUMN      0x15
#define SSD1351_CMD_SETROW         0x75
#define SSD1351_CMD_WRITERAM       0x5C
#define SSD1351_CMD_READRAM        0x5D
#define SSD1351_CMD_SETREMAP       0xA0
#define SSD1351_CMD_STARTLINE      0xA1
#define SSD1351_CMD_DISPLAYOFFSET  0xA2
#define SSD1351_CMD_DISPLAYALLOFF  0xA4
#define SSD1351_CMD_DISPLAYALLON   0xA5
#define SSD1351_CMD_NORMALDISPLAY  0xA6
#define SSD1351_CMD_INVERTDISPLAY  0xA7
#define SSD1351_CMD_FUNCTIONSELECT 0xAB
#define SSD1351_CMD_DISPLAYOFF     0xAE
#define SSD1351_CMD_DISPLAYON      0xAF
#define SSD1351_CMD_PRECHARGE      0xB1
#define SSD1351_CMD_DISPLAYENHANCE 0xB2
#define SSD1351_CMD_CLOCKDIV       0xB3
#define SSD1351_CMD_SETVSL         0xB4
#define SSD1351_CMD_SETGPIO        0xB5
#define SSD1351_CMD_PRECHARGE2     0xB6
#define SSD1351_CMD_SETGRAY        0xB8
#define SSD1351_CMD_USELUT         0xB9
#define SSD1351_CMD_PRECHARGELEVEL 0xBB
#define SSD1351_CMD_VCOMH          0xBE
#define SSD1351_CMD_CONTRASTABC    0xC1
#define SSD1351_CMD_CONTRASTMASTER 0xC7
#define SSD1351_CMD_MUXRATIO       0xCA
#define SSD1351_CMD_COMMANDLOCK    0xFD
#define SSD1351_CMD_HORIZSCROLL    0x96
#define SSD1351_CMD_STOPSCROLL     0x9E
#define SSD1351_CMD_STARTSCROLL    0x9F


#define PIN_OUT_SET 0x60000304
#define PIN_OUT_CLEAR 0x60000308

class SSD1351 : public Renderer {

 public:

  SSD1351(int8_t cs,int8_t mosi,int8_t sclk);

  void begin(void);
  void DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font);
  void setAddrWindow_i(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  void pushColors(uint16_t *data, uint8_t len, boolean first);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void write16BitColor(uint16_t color);
  void setRotation(uint8_t r);
  void invertDisplay(boolean i);
  uint16_t GetColorFromIndex(uint8_t index);
  void DisplayOnff(int8_t on);
  void writecommand(uint8_t c);
  void writedata(uint8_t d);
  void commandList(uint8_t *addr);
  void hw_spi_init();
  void sendcommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes);
  void sendcommand(uint8_t commandByte,uint8_t *dataBytes, uint8_t numDataBytes);
  void drawFastVLine(int16_t x,int16_t y,int16_t h,uint16_t color);
  void drawFastHLine(int16_t x,int16_t y,int16_t w,uint16_t color);
  void spi_lcd_mode_init(void);
  void dim(uint8_t contrast);

 private:
  uint8_t  tabcolor;
  void fastSPIwrite(uint8_t d,uint8_t dc);
  void start(void);
  void stop(void);
  int8_t  _cs, _mosi, _sclk, _hwspi;

};

#endif
