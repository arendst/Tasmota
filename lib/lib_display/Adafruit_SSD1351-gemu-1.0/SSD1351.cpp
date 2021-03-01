/***************************************************
  This is our library for the Adafruit SSD1351 Breakout and Shield

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include "SSD1351.h"
#include <limits.h>
#include <pgmspace.h>


const uint16_t ssd1351_colors[]={SSD1351_BLACK,SSD1351_WHITE,SSD1351_RED,SSD1351_GREEN,SSD1351_BLUE,SSD1351_CYAN,SSD1351_MAGENTA,\
  SSD1351_YELLOW,SSD1351_NAVY,SSD1351_DARKGREEN,SSD1351_DARKCYAN,SSD1351_MAROON,SSD1351_PURPLE,SSD1351_OLIVE,\
SSD1351_LIGHTGREY,SSD1351_DARKGREY,SSD1351_ORANGE,SSD1351_GREENYELLOW,SSD1351_PINK};

// Constructor when using software SPI.  All output pins are configurable.
SSD1351::SSD1351(int8_t cs,int8_t mosi,int8_t sclk, int8_t dc) : Renderer(SSD1351_WIDTH, SSD1351_HEIGHT) {
  _cs   = cs;
  _mosi  = mosi;
  _sclk = sclk;
  _hwspi = 0;
  _dc = dc;
}

#ifndef ESP32

#include "spi_register.h"

/* CPU Clock = 80 Mhz
max clock of display is 4.545 Mhz (220ns sclk cycle)
so cpu/18 => 4.44 Mhz should be ok
HSPI CLK	5	GPIO14
HSPI /CS	8	GPIO15
HSPI MOSI	7	GPIO13
*/

uint8_t ssd131_start;

uint32_t ssd1351_clock;
uint32_t ssd1351_usr;
uint32_t ssd1351_usr1;
uint32_t ssd1351_usr2;
uint32_t ssd1351_spi1c;
uint32_t ssd1351_spi1p;
//uint32_t ssd1351_gpmux;
uint32_t ssd1351_mtdo;


uint32_t ssd1351_clock_prev;
uint32_t ssd1351_usr_prev;
uint32_t ssd1351_usr1_prev;
uint32_t ssd1351_usr2_prev;
uint32_t ssd1351_spi1c_prev;
uint32_t ssd1351_spi1p_prev;
//uint32_t ssd1351_gpmux_prev;
uint32_t ssd1351_mtdo_prev;

// code from espressif SDK
/******************************************************************************
 * FunctionName : spi_lcd_mode_init
 * Description  : SPI master initial function for driving LCD 3 wire spi
*******************************************************************************/
void SSD1351::spi_lcd_mode_init(void) {
	uint32 regvalue;

  if (_dc >= 0) {
    spis = SPISettings(40000000, MSBFIRST, SPI_MODE0);
  } else {
    ssd1351_clock_prev=SPI1CLK;
    ssd1351_usr_prev=SPI1U;
    ssd1351_usr1_prev=SPI1U1;
    ssd1351_usr2_prev=SPI1U2;
    ssd1351_spi1c_prev=SPI1C;
    ssd1351_spi1p_prev=SPI1P;
    //ssd1351_gpmux_prev=GPMUX;
    ssd1351_mtdo_prev=READ_PERI_REG(PERIPHS_IO_MUX_MTDO_U);

    SPI1U = SPIUMOSI | SPIUDUPLEX | SPIUSSE;
    SPI1U1=0;
    SPI1C = 0;

	   //bit9 of PERIPHS_IO_MUX should be cleared when HSPI clock doesn't equal CPU clock
	    //bit8 of PERIPHS_IO_MUX should be cleared when SPI clock doesn't equal CPU clock

	  WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //clear bit9
		//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2);//configure miso to spi mode
	  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2);//configure mosi to spi mode
	  PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2);//configure sclk to spi mode
	   PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2);//configure cs to spi mode

// the current implementation leaves about 1 us between transfers ????
// due to lack of documentation i could not find the reason
// skipping this would double the speed !!!

	//SET_PERI_REG_MASK(SPI_USER(1), SPI_CS_SETUP|SPI_CS_HOLD|SPI_USR_COMMAND);

    SET_PERI_REG_MASK(SPI_USER(1), SPI_USR_COMMAND);

	  CLEAR_PERI_REG_MASK(SPI_USER(1), SPI_FLASH_MODE);
	// SPI clock=CPU clock/8 => 10 Mhz
  /*
	WRITE_PERI_REG(SPI_CLOCK(1),
					((1&SPI_CLKDIV_PRE)<<SPI_CLKDIV_PRE_S)|
					((3&SPI_CLKCNT_N)<<SPI_CLKCNT_N_S)|
					((1&SPI_CLKCNT_H)<<SPI_CLKCNT_H_S)|
					((3&SPI_CLKCNT_L)<<SPI_CLKCNT_L_S)); //clear bit 31,set SPI clock div
          */
// will result in 80/18 = 4,4 Mhz
    SPI.setFrequency(4500000);

    ssd1351_clock=SPI1CLK;
    ssd1351_usr=SPI1U;
    ssd1351_usr1=SPI1U1;
    ssd1351_usr2=SPI1U2;
    ssd1351_spi1c=SPI1C;
    ssd1351_spi1p=SPI1P;
    //ssd1351_gpmux=GPMUX;
    ssd1351_mtdo=READ_PERI_REG(PERIPHS_IO_MUX_MTDO_U);
  }

  ssd131_start = 0;
}

void SSD1351::start(void) {
  if (ssd131_start) return;
  //while(SPI1CMD & SPIBUSY) {}
  if (_dc >= 0) {
    SPI.beginTransaction(spis);
  } else {
    while(READ_PERI_REG(SPI_CMD(1))&SPI_USR);
    SPI1CLK=ssd1351_clock;
    SPI1U=ssd1351_usr;
    SPI1U1=ssd1351_usr1;
    SPI1U2=ssd1351_usr2;
    SPI1C=ssd1351_spi1c;
    SPI1P=ssd1351_spi1p;
    //GPMUX=ssd1351_gpmux;
    WRITE_PERI_REG(PERIPHS_IO_MUX_MTDO_U,ssd1351_mtdo);
  }
  ssd131_start = 1;
}

void SSD1351::stop(void) {
  if (!ssd131_start) return;

  if (_dc >= 0) {
    SPI.endTransaction();
  } else {
    //while(SPI1CMD & SPIBUSY) {}
    while(READ_PERI_REG(SPI_CMD(1))&SPI_USR);
    SPI1CLK=ssd1351_clock_prev;
    SPI1U=ssd1351_usr_prev;
    SPI1U1=ssd1351_usr1_prev;
    SPI1U2=ssd1351_usr2_prev;
    SPI1C=ssd1351_spi1c_prev;
    SPI1P=ssd1351_spi1p_prev;
    //GPMUX=ssd1351_gpmux_prev;
    WRITE_PERI_REG(PERIPHS_IO_MUX_MTDO_U,ssd1351_mtdo_prev);
  }
  ssd131_start = 0;
}

// dc = 0
void SSD1351::writecommand(uint8_t c) {
  if (_hwspi) {
    uint32_t regvalue;
    uint8_t bytetemp;
    start();
//#define SPI_USR_COMMAND_BITLEN 0x0000000F
//#define SPI_USR_COMMAND_BITLEN_S 28
    if (_dc >= 0) {
      digitalWrite(_dc, LOW);
      SPI.transfer(c);
    } else {
      bytetemp = (c >> 1) & 0x7f;
      regvalue= ((8&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S)|((uint32)bytetemp);		//configure transmission variable,9bit transmission length and first 8 command bit
      if (c & 0x01) 	regvalue |= BIT15;        //write the 9th bit
      while (READ_PERI_REG(SPI_CMD(1)) & SPI_USR);		//waiting for spi module available
      WRITE_PERI_REG(SPI_USER2(1), regvalue);				//write  command and command length into spi reg
      SET_PERI_REG_MASK(SPI_CMD(1), SPI_USR);		//transmission start
    }
  } else {
    fastSPIwrite(c, 0);
  }
}

// dc = 1
void SSD1351::writedata(uint8_t d) {
  if (_hwspi) {
    uint32_t regvalue;
    uint8_t bytetemp;
    start();

    if (_dc >= 0) {
      digitalWrite(_dc, HIGH);
      SPI.transfer(d);
    } else {
      bytetemp = (d >> 1) | 0x80;
      regvalue= ((8&SPI_USR_COMMAND_BITLEN)<<SPI_USR_COMMAND_BITLEN_S)|((uint32)bytetemp);		//configure transmission variable,9bit transmission length and first 8 command bit
      if(d&0x01) 	regvalue|=BIT15;        //write the 9th bit
      while(READ_PERI_REG(SPI_CMD(1))&SPI_USR);		//waiting for spi module available
      WRITE_PERI_REG(SPI_USER2(1), regvalue);				//write  command and command length into spi reg
      SET_PERI_REG_MASK(SPI_CMD(1), SPI_USR);		//transmission start
    }
  } else {
    fastSPIwrite(d, 1);
  }
}


void ICACHE_RAM_ATTR SSD1351::fastSPIwrite(uint8_t d, uint8_t dc) {

  WRITE_PERI_REG( PIN_OUT_CLEAR, 1 << _cs);

  if (_dc >= 0) {
    digitalWrite(_dc, dc);
    WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_sclk);
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_sclk);
      if(d&bit) WRITE_PERI_REG( PIN_OUT_SET, 1<<_mosi);
      else   WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_mosi);
      WRITE_PERI_REG( PIN_OUT_SET, 1<<_sclk);
    }
  } else {
    WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_sclk);
    if(dc) WRITE_PERI_REG( PIN_OUT_SET, 1<<_mosi);
    else   WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_mosi);
    WRITE_PERI_REG( PIN_OUT_SET, 1<<_sclk);
    for(uint8_t bit = 0x80; bit; bit >>= 1) {
      WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_sclk);
      if(d&bit) WRITE_PERI_REG( PIN_OUT_SET, 1<<_mosi);
      else   WRITE_PERI_REG( PIN_OUT_CLEAR, 1<<_mosi);
      WRITE_PERI_REG( PIN_OUT_SET, 1<<_sclk);
    }
  }
  WRITE_PERI_REG( PIN_OUT_SET, 1 << _cs);
}

#else
// ESP32 section
uint8_t ssd131_start;

void SSD1351::writedata(uint8_t d) {
  fastSPIwrite(d, 1);
}

void SSD1351::writecommand(uint8_t c) {
  fastSPIwrite(c, 0);
}

#include "soc/spi_reg.h"
#include "soc/spi_struct.h"
#include "esp32-hal-spi.h"
#include "esp32-hal.h"
#include "soc/spi_struct.h"

// diconnect from spi
void SSD1351::start(void) {
  if (ssd131_start) return;
  SPI.beginTransaction(spis);
  ssd131_start = 1;
}

// reconnect to spi
void SSD1351::stop(void) {
  if (!ssd131_start) return;
  SPI.endTransaction();
  ssd131_start = 0;
}

// since ardunio transferBits ia completely disfunctional
// we use our own hardware driver for 9 bit spi
void SSD1351::fastSPIwrite(uint8_t d, uint8_t dc) {

    digitalWrite( _cs, LOW);
    if (_dc >= 0) {
      digitalWrite(_dc, dc);
      SPI.transfer(d);
    } else {
      uint32_t regvalue = d >> 1;
      if (dc) regvalue |= 0x80;
      else regvalue &= 0x7f;
      if (d & 1) regvalue |= 0x8000;

      REG_SET_BIT(SPI_USER_REG(3), SPI_USR_MOSI);
      REG_WRITE(SPI_MOSI_DLEN_REG(3), 9 - 1);
      uint32_t *dp=(uint32_t*)SPI_W0_REG(3);
      *dp = regvalue;
      REG_SET_BIT(SPI_CMD_REG(3), SPI_USR);
      while (REG_GET_FIELD(SPI_CMD_REG(3), SPI_USR));
    }
    digitalWrite( _cs, HIGH);
}

#endif


static const uint8_t PROGMEM initList[] = {
  SSD1351_CMD_COMMANDLOCK, 1, // Set command lock, 1 arg
    0x12,
  SSD1351_CMD_COMMANDLOCK, 1, // Set command lock, 1 arg
    0xB1,
  SSD1351_CMD_DISPLAYOFF, 0,  // Display off, no args
  SSD1351_CMD_CLOCKDIV, 1,
    0xF1, // 7:4 = Oscillator Freq, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
  SSD1351_CMD_MUXRATIO, 1,
    127,
  SSD1351_CMD_DISPLAYOFFSET, 1,
    0x0,
  SSD1351_CMD_SETGPIO, 1,
    0x00,
  SSD1351_CMD_FUNCTIONSELECT, 1,
    0x01,  // internal (diode drop)
  SSD1351_CMD_PRECHARGE, 1,
    0x32,
  SSD1351_CMD_VCOMH, 1,
    0x05,
  SSD1351_CMD_STARTLINE, 1,
    0x00,
  SSD1351_CMD_NORMALDISPLAY, 0,
  SSD1351_CMD_CONTRASTABC, 3,
    0xC8, 0x80, 0xC8,
  SSD1351_CMD_CONTRASTMASTER, 1,
    0x0F,
  SSD1351_CMD_SETVSL, 3,
    0xA0, 0xB5, 0x55,
  SSD1351_CMD_PRECHARGE2, 1,
    0x01,
  SSD1351_CMD_HORIZSCROLL, 1,
    0x00,
  SSD1351_CMD_STOPSCROLL, 0,
  SSD1351_CMD_DISPLAYON, 0, // Main screen turn on
  0 }; // END OF COMMAND LIST


void SSD1351::begin(void) {
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs,HIGH);
  pinMode(_sclk, OUTPUT);
  digitalWrite(_sclk, LOW);
  pinMode(_mosi, OUTPUT);
  digitalWrite(_mosi, LOW);

  if (_dc >= 0) {
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, LOW);
  }


#ifndef ESP32
  if ((_sclk == 14) && (_mosi == 13) && (_cs == 15)) {
    // we use hardware spi
      _hwspi = 1;
      SPI.begin();
      spi_lcd_mode_init();
  } else {
    // we must use software spi
      _hwspi = 0;
  }
#else
  _hwspi = 1;
  SPI.begin(_sclk, -1, _mosi, -1);
  spis = SPISettings(4500000, MSBFIRST, SPI_MODE3);
#endif

  const uint8_t *addr = (const uint8_t *)initList;
  uint8_t cmd, x, numArgs;

  while ((cmd = pgm_read_byte(addr++)) > 0) { // '0' command ends list
    x = pgm_read_byte(addr++);
    numArgs = x & 0x7F;
    if (cmd != 0xFF) { // '255' is ignored
      sendcommand(cmd, addr, numArgs);
    }
    addr += numArgs;
  }
  delay(100);
  setRotation(0);
  stop();
}


void SSD1351::sendcommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes) {
    writecommand(commandByte);
    for (int i=0; i<numDataBytes; i++) {
      writedata(pgm_read_byte(dataBytes++)); // Send the data bytes
    }
}

void SSD1351::sendcommand(uint8_t commandByte,uint8_t *dataBytes, uint8_t numDataBytes) {
    writecommand(commandByte);
    for (int i=0; i<numDataBytes; i++) {
      writedata(*dataBytes++); // Send the data bytes
    }
}

uint16_t SSD1351::GetColorFromIndex(uint8_t index) {
  if (index>=sizeof(ssd1351_colors)/2) index=0;
  return ssd1351_colors[index];
}

void SSD1351::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
  setRotation(rot);
  invertDisplay(false);
  setTextWrap(false);         // Allow text to run off edges
  cp437(true);
  setTextFont(font&3);
  setTextSize(size&7);
  setTextColor(SSD1351_WHITE,SSD1351_BLACK);
  setCursor(0,0);
  fillScreen(SSD1351_BLACK);
  stop();
}

void SSD1351::DisplayOnff(int8_t on) {
  if (on) {
    writecommand(SSD1351_CMD_DISPLAYON);    //Display on
  } else {
    writecommand(SSD1351_CMD_DISPLAYOFF);
  }
  stop();
}

// dimmer 0-100
void SSD1351::dim(uint8_t contrast) {
  writecommand(SSD1351_CMD_CONTRASTMASTER);
  if (contrast>15) contrast=15;
  writedata(contrast);
  stop();
}

#define ssd1351_swap(a, b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation


void SSD1351::setAddrWindow_i(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
   uint16_t x2 = x1 + w - 1,
            y2 = y1 + h - 1;
   if (rotation&1) { // Vertical address increment mode
     ssd1351_swap(x1,y1);
     ssd1351_swap(x2,y2);
   }
   writecommand(SSD1351_CMD_SETCOLUMN); // X range
   writedata(x1);
   writedata(x2);
   writecommand(SSD1351_CMD_SETROW);    // Y range
   writedata(y1);
   writedata(y2);
   writecommand(SSD1351_CMD_WRITERAM);  // Begin write
}

void SSD1351::write16BitColor(uint16_t color){
  writedata(color>>8);
  writedata(color&0xff);
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void SSD1351::setRotation(uint8_t r) {
  // madctl bits:
  // 6,7 Color depth (01 = 64K)
  // 5   Odd/even split COM (0: disable, 1: enable)
  // 4   Scan direction (0: top-down, 1: bottom-up)
  // 3   Reserved
  // 2   Color remap (0: A->B->C, 1: C->B->A)
  // 1   Column remap (0: 0-127, 1: 127-0)
  // 0   Address increment (0: horizontal, 1: vertical)
  uint8_t madctl = 0b01100100; // 64K, enable split, CBA

  rotation = r & 3; // Clip input to valid range

  switch(rotation) {
    case 0:
      madctl |= 0b00010000; // Scan bottom-up
      _width  = SSD1351_WIDTH;
      _height = SSD1351_HEIGHT;
      break;
    case 1:
      madctl |= 0b00010011; // Scan bottom-up, column remap 127-0, vertical
      _width  = SSD1351_HEIGHT;
      _height = SSD1351_WIDTH;
      break;
    case 2:
      madctl |= 0b00000010; // Column remap 127-0
      _width  = SSD1351_WIDTH;
      _height = SSD1351_HEIGHT;
      break;
    case 3:
      madctl |= 0b00000001; // Vertical
      _width  = SSD1351_HEIGHT;
      _height = SSD1351_WIDTH;
      break;
  }

  sendcommand(SSD1351_CMD_SETREMAP, &madctl, 1);
  uint8_t startline = (rotation < 2) ? SSD1351_HEIGHT : 0;
  sendcommand(SSD1351_CMD_STARTLINE, &startline, 1);
  stop();
}

void SSD1351::invertDisplay(boolean i) {
  writecommand(i ? SSD1351_CMD_INVERTDISPLAY : SSD1351_CMD_NORMALDISPLAY);
  stop();
}

void SSD1351::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  setAddrWindow_i(x,y,1,1);
  write16BitColor(color);
  stop();
}

void SSD1351::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  // uint16_t x2 = x1 + w - 1,
  //          y2 = y1 + h - 1;
  uint8_t flag=0;

  if (!x1 && !y1 && !x2 && !y2) {
    x1=0;
    y1=0;
    x2=_width;
    y2=_height;
    flag=1;
  }

  if (x2>_width) x2=_width;
  if (y2>_height) y2=_height;

  x2--;
  y2--;
   if (rotation&1) { // Vertical address increment mode
     ssd1351_swap(x1,y1);
     ssd1351_swap(x2,y2);
   }
   //Serial.printf("x1:%d x2:%d y1:%d y2:%d\n",x1,x2,y1,y2);
   writecommand(SSD1351_CMD_SETCOLUMN); // X range
   writedata(x1);
   writedata(x2);
   writecommand(SSD1351_CMD_SETROW);    // Y range
   writedata(y1);
   writedata(y2);
   writecommand(SSD1351_CMD_WRITERAM);  // Begin write
   if (flag) stop();
}

void SSD1351::pushColors(uint16_t *data, uint16_t len, boolean first) {
  for (uint16_t b=0; b<len; b++){
    write16BitColor(*data++);
  }
  stop();
}

void SSD1351::drawFastVLine(int16_t x,int16_t y,int16_t h,uint16_t color) {
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((y+h-1) >= _height) h = _height-y;

  setAddrWindow_i(x,y,1,h);
  while (h--) {
    write16BitColor(color);
  }
  stop();
}

void SSD1351::drawFastHLine(int16_t x,int16_t y,int16_t w,uint16_t color) {
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x+w-1) >= _width)  w = _width-x;

  setAddrWindow_i(x,y,w,1);
  while (w--) {
    write16BitColor(color);
  }
  stop();
}
