/***************************************************
  This is a library for the ST7789 IPS SPI display.

  Originally written by Limor Fried/Ladyada for
  Adafruit Industries.

  Modified by Ananev Ilia
 ****************************************************/

#include "Arduino_ST7789.h"
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

const uint16_t ST7789_colors[]={ST7789_BLACK,ST7789_WHITE,ST7789_RED,ST7789_GREEN,ST7789_BLUE,ST7789_CYAN,ST7789_MAGENTA,\
  ST7789_YELLOW,ST7789_NAVY,ST7789_DARKGREEN,ST7789_DARKCYAN,ST7789_MAROON,ST7789_PURPLE,ST7789_OLIVE,\
ST7789_LIGHTGREY,ST7789_DARKGREY,ST7789_ORANGE,ST7789_GREENYELLOW,ST7789_PINK};

#ifdef ESP32
#include "esp8266toEsp32.h"
#define ST7789_DIMMER
#endif



uint16_t Arduino_ST7789::GetColorFromIndex(uint8_t index) {
  if (index>=sizeof(ST7789_colors)/2) index=0;
  return ST7789_colors[index];
}

static const uint8_t PROGMEM
  init_cmd[] = {                 		    // Initialization commands for 7789 screens
    10,                       				// 9 commands in list:
    ST7789_SWRESET,   ST_CMD_DELAY,  		// 1: Software reset, no args, w/delay
      150,                     				// 150 ms delay
    ST7789_SLPOUT ,   ST_CMD_DELAY,  		// 2: Out of sleep mode, no args, w/delay
      255,                    				// 255 = 500 ms delay
    ST7789_COLMOD , 1+ST_CMD_DELAY,  		// 3: Set color mode, 1 arg + delay:
      0x55,                   				// 16-bit color
      10,                     				// 10 ms delay
    ST7789_MADCTL , 1,  					// 4: Memory access ctrl (directions), 1 arg:
      0x00,                   				// Row addr/col addr, bottom to top refresh
    ST7789_INVON ,   ST_CMD_DELAY,  		// 7: Inversion ON
      10,
    ST7789_NORON  ,   ST_CMD_DELAY,  		// 8: Normal display on, no args, w/delay
      10,                     				// 10 ms delay
    ST7789_DISPON ,   ST_CMD_DELAY,  		// 9: Main screen turn on, no args, w/delay
    255 };                  				// 255 = 500 ms delay

inline uint16_t swapcolor(uint16_t x) {
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}

#if defined (SPI_HAS_TRANSACTION)
  static SPISettings ST7789_SPISettings;
#elif defined (__AVR__) || defined(CORE_TEENSY)
  static uint8_t SPCRbackup;
  static uint8_t mySPCR;
#endif


#if defined (SPI_HAS_TRANSACTION)
#define SPI_BEGIN_TRANSACTION()    if (_hwSPI)    SPI.beginTransaction(ST7789_SPISettings)
#define SPI_END_TRANSACTION()      if (_hwSPI)    SPI.endTransaction()
#else
#define SPI_BEGIN_TRANSACTION()    (void)
#define SPI_END_TRANSACTION()      (void)
#endif

// Constructor when using software SPI.  All output pins are configurable.
Arduino_ST7789::Arduino_ST7789(int8_t dc, int8_t rst, int8_t sid, int8_t sclk, int8_t cs, int8_t bp)
  : Renderer(_width, _height)
{
  _cs   = cs;
  _dc   = dc;
  _sid  = sid;
  _sclk = sclk;
  _rst  = rst;
  _hwSPI = false;
  _bp = bp;
  if(dc == -1) _SPI9bit = true;
  else _SPI9bit = false;
}

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
Arduino_ST7789::Arduino_ST7789(int8_t dc, int8_t rst, int8_t cs, int8_t bp)
  : Renderer(_width, _height) {
  _cs   = cs;
  _dc   = dc;
  _rst  = rst;
  _hwSPI = true;
  _SPI9bit = false;
  _sid  = _sclk = -1;
  _bp = bp;
}


void Arduino_ST7789::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
  setRotation(rot);
  if (_width==320 || _height==320) {
    invertDisplay(false);
  } else {
    invertDisplay(true);
  }
  //setTextWrap(false);         // Allow text to run off edges
  //cp437(true);
  setTextFont(font&3);
  setTextSize(size&7);
  setTextColor(ST7789_WHITE,ST7789_BLACK);
  setCursor(0,0);
  fillScreen(ST7789_BLACK);
}

inline void Arduino_ST7789::spiwrite(uint8_t c)
{

  //Serial.println(c, HEX);

  if (_hwSPI)
  {
#if defined (SPI_HAS_TRANSACTION)
      SPI.transfer(c);
#elif defined (__AVR__) || defined(CORE_TEENSY)
      SPCRbackup = SPCR;
      SPCR = mySPCR;
      SPI.transfer(c);
      SPCR = SPCRbackup;
#elif defined (__arm__)
      SPI.setClockDivider(21); //4MHz
      SPI.setDataMode(SPI_MODE2);
      SPI.transfer(c);
#endif
  }
  else
  {
	  if(_SPI9bit)
	  {
		//9s bit send first
#if defined(USE_FAST_IO)
		*clkport &= ~clkpinmask;
		if(_DCbit) *dataport |=  datapinmask;
		else        *dataport &= ~datapinmask;
		*clkport |=  clkpinmask;
#else
		digitalWrite(_sclk, LOW);
		if(_DCbit) digitalWrite(_sid, HIGH);
		else        digitalWrite(_sid, LOW);
		digitalWrite(_sclk, HIGH);
#endif


		// Fast SPI bitbang swiped from LPD8806 library
		for(uint8_t bit = 0x80; bit; bit >>= 1) {
#if defined(USE_FAST_IO)
		*clkport &= ~clkpinmask;
		if(c & bit) *dataport |=  datapinmask;
		else        *dataport &= ~datapinmask;
		*clkport |=  clkpinmask;
#else
		digitalWrite(_sclk, LOW);
		if(c & bit) digitalWrite(_sid, HIGH);
		else        digitalWrite(_sid, LOW);
		digitalWrite(_sclk, HIGH);
#endif

	  }
	  }
	  else
	  {
		  // Fast SPI bitbang swiped from LPD8806 library
		for(uint8_t bit = 0x80; bit; bit >>= 1) {
#if defined(USE_FAST_IO)
		*clkport &= ~clkpinmask;
		if(c & bit) *dataport |=  datapinmask;
		else        *dataport &= ~datapinmask;
		*clkport |=  clkpinmask;
#else
		digitalWrite(_sclk, LOW);
		if(c & bit) digitalWrite(_sid, HIGH);
		else        digitalWrite(_sid, LOW);
		digitalWrite(_sclk, HIGH);
#endif
	  }
	}
  }
}

void Arduino_ST7789::writecommand(uint8_t c) {

  DC_LOW();
  CS_LOW();
  SPI_BEGIN_TRANSACTION();

  spiwrite(c);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_ST7789::writedata(uint8_t c) {
  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  spiwrite(c);

  CS_HIGH();
  SPI_END_TRANSACTION();
}



// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_ST7789::displayInit(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;
  //<-----------------------------------------------------------------------------------------
  DC_HIGH();
  if (!_hwSPI) {
#if defined(USE_FAST_IO)
      *clkport |=  clkpinmask;
#else
      digitalWrite(_sclk, HIGH);
#endif
  }
  //<-----------------------------------------------------------------------------------------

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while (numCommands--) {                 // For each command...
    writecommand(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & ST_CMD_DELAY;   //   If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;            //   Mask out delay bit
    while (numArgs--) {                   //   For each argument...
      writedata(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if (ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if (ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}


// Initialization code common to all ST7789 displays
void Arduino_ST7789::commonInit(const uint8_t *cmdList) {
  _ystart = _xstart = 0;
  _colstart  = _rowstart = 0; // May be overridden in init func

  pinMode(_dc, OUTPUT);
  if (_cs>=0) {
	  pinMode(_cs, OUTPUT);
  }

  if (_bp>=0) {
// #define ESP32_PWM_CHANNEL 1
#ifdef ST7789_DIMMER
    analogWrite(_bp, 128);
#else
    pinMode(_bp, OUTPUT);
#endif

  }


#if defined(USE_FAST_IO)
  dcport    = portOutputRegister(digitalPinToPort(_dc));
  dcpinmask = digitalPinToBitMask(_dc);
  if (_cs>=0) {
	   csport    = portOutputRegister(digitalPinToPort(_cs));
	   cspinmask = digitalPinToBitMask(_cs);
  }
#endif

  if(_hwSPI) { // Using hardware SPI
#if defined (SPI_HAS_TRANSACTION)
    SPI.begin();
  //  ST7789_SPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE2);
    ST7789_SPISettings = SPISettings(1000000, MSBFIRST, SPI_MODE2);
#elif defined (__AVR__) || defined(CORE_TEENSY)
    SPCRbackup = SPCR;
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.setDataMode(SPI_MODE2);
    mySPCR = SPCR; // save our preferred state
    SPCR = SPCRbackup;  // then restore
#elif defined (__SAM3X8E__)
    SPI.begin();
    SPI.setClockDivider(21); //4MHz
    SPI.setDataMode(SPI_MODE2);
#endif
  } else {
    pinMode(_sclk, OUTPUT);
    pinMode(_sid , OUTPUT);
    digitalWrite(_sclk, LOW);
    digitalWrite(_sid, LOW);

#if defined(USE_FAST_IO)
    clkport     = portOutputRegister(digitalPinToPort(_sclk));
    dataport    = portOutputRegister(digitalPinToPort(_sid));
    clkpinmask  = digitalPinToBitMask(_sclk);
    datapinmask = digitalPinToBitMask(_sid);
#endif
  }

  // toggle RST low to reset; CS low so it'll listen to us
  CS_LOW();
  if (_rst != -1) {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(50);
    digitalWrite(_rst, LOW);
    delay(50);
    digitalWrite(_rst, HIGH);
    delay(50);
  }

  if (cmdList)
    displayInit(cmdList);
}

void Arduino_ST7789::setRotation(uint8_t m) {

  writecommand(ST7789_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     writedata(ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);

     _xstart = 0;
     _ystart = 0;
     if (_width==240 && _height==240) {
       _xstart = ST7789_240x240_XSTART_R0;
       _ystart = ST7789_240x240_YSTART_R0;
     }
     if (_width==135 && _height==240) {
       _xstart = ST7789_135x240_XSTART_R0;
       _ystart = ST7789_135x240_YSTART_R0;
     }
     break;
   case 1:
     writedata(ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

     _ystart = 0;
     _xstart = 0;
     if (_width==240 && _height==240) {
       _xstart = ST7789_240x240_XSTART_R1;
       _ystart = ST7789_240x240_YSTART_R1;
     }
     if (_width==240 && _height==135) {
       _xstart = ST7789_135x240_XSTART_R1;
       _ystart = ST7789_135x240_YSTART_R1;
     }
     break;
  case 2:
     writedata(ST7789_MADCTL_RGB);

     _xstart = 0;
     _ystart = 0;
     if (_width==240 && _height==240) {
       _xstart = ST7789_240x240_XSTART_R2;
       _ystart = ST7789_240x240_YSTART_R2;
     }
     if (_width==135 && _height==240) {
       _xstart = ST7789_135x240_XSTART_R2;
       _ystart = ST7789_135x240_YSTART_R2;
     }
     break;

   case 3:
     writedata(ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);

     _xstart = 0;
     _ystart = 0;
     if (_width==240 && _height==240) {
       _xstart = ST7789_240x240_XSTART_R3;
       _ystart = ST7789_240x240_YSTART_R3;
     }
     if (_width==240 && _height==135) {
       _xstart = ST7789_135x240_XSTART_R3;
       _ystart = ST7789_135x240_YSTART_R3;
     }
     break;
  }
}

void Arduino_ST7789::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  setAddrWindow_int(x0,y0,x1-1,y1-1);
}

void Arduino_ST7789::setAddrWindow_int(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  uint16_t x_start = x0 + _xstart, x_end = x1 + _xstart;
  uint16_t y_start = y0 + _ystart, y_end = y1 + _ystart;

  writecommand(ST7789_CASET); // Column addr set
  writedata(x_start >> 8);
  writedata(x_start & 0xFF);     // XSTART
  writedata(x_end >> 8);
  writedata(x_end & 0xFF);     // XEND

  writecommand(ST7789_RASET); // Row addr set
  writedata(y_start >> 8);
  writedata(y_start & 0xFF);     // YSTART
  writedata(y_end >> 8);
  writedata(y_end & 0xFF);     // YEND

  writecommand(ST7789_RAMWR); // write to RAM
}

void Arduino_ST7789::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  setAddrWindow_int(x,y,x+1,y+1);

  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  spiwrite(color >> 8);
  spiwrite(color);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_ST7789::drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  setAddrWindow_int(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  while (h--) {
    spiwrite(hi);
    spiwrite(lo);
  }

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_ST7789::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow_int(x, y, x+w-1, y);

  uint8_t hi = color >> 8, lo = color;

  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  while (w--) {
    spiwrite(hi);
    spiwrite(lo);
  }

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_ST7789::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void Arduino_ST7789::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  setAddrWindow_int(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  SPI_BEGIN_TRANSACTION();

  DC_HIGH();
  CS_LOW();
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      spiwrite(hi);
      spiwrite(lo);
    }
    delay(0);
  }
  CS_HIGH();
  SPI_END_TRANSACTION();
}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Arduino_ST7789::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void Arduino_ST7789::invertDisplay(boolean i) {
  writecommand(i ? ST7789_INVON : ST7789_INVOFF);
}

/******** low level bit twiddling **********/

inline void Arduino_ST7789::CS_HIGH(void) {
	if(_cs>=0) {
		#if defined(USE_FAST_IO)
			*csport |= cspinmask;
		#else
		digitalWrite(_cs, HIGH);
		#endif
	}
}

inline void Arduino_ST7789::CS_LOW(void) {
	if(_cs>=0) {
		#if defined(USE_FAST_IO)
		*csport &= ~cspinmask;
		#else
		digitalWrite(_cs, LOW);
		#endif
	}
}

inline void Arduino_ST7789::DC_HIGH(void) {
	_DCbit = true;
#if defined(USE_FAST_IO)
  *dcport |= dcpinmask;
#else
  digitalWrite(_dc, HIGH);
#endif
}

inline void Arduino_ST7789::DC_LOW(void) {
	_DCbit = false;
#if defined(USE_FAST_IO)
  *dcport &= ~dcpinmask;
#else
  digitalWrite(_dc, LOW);
#endif
}

void Arduino_ST7789::init(uint16_t width, uint16_t height) {
  commonInit(NULL);

  _height = height;
  _width = width;

  displayInit(init_cmd);

  setRotation(2);

}

void Arduino_ST7789::DisplayOnff(int8_t on) {
  if (on) {
    writecommand(ST7789_DISPON);    //Display on
    if (_bp>=0) {
#ifdef ST7789_DIMMER
      analogWrite(_bp, dimmer);
      // ledcWrite(ESP32_PWM_CHANNEL,dimmer);
#else
      digitalWrite(_bp,HIGH);
#endif
    }
  } else {
    writecommand(ST7789_DISPOFF);
    if (_bp>=0) {
#ifdef ST7789_DIMMER
      analogWrite(_bp, 0);
      // ledcWrite(ESP32_PWM_CHANNEL,0);
#else
      digitalWrite(_bp,LOW);
#endif
    }
  }
}

// dimmer 0-100
void Arduino_ST7789::dim(uint8_t dim) {
  dimmer = dim;
  if (dimmer>15) dimmer=15;
  dimmer=((float)dimmer/15.0)*255.0;
#ifdef ESP32
  analogWrite(_bp, dimmer);
  // ledcWrite(ESP32_PWM_CHANNEL,dimmer);
#endif
}

void Arduino_ST7789::pushColor(uint16_t color) {
  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  spiwrite(color >> 8);
  spiwrite(color);

  CS_HIGH();
  SPI_END_TRANSACTION();
}

void Arduino_ST7789::pushColors(uint16_t *data, uint16_t len, boolean first) {
  uint16_t color;

  SPI_BEGIN_TRANSACTION();
  DC_HIGH();
  CS_LOW();

  while (len--) {
    color = *data++;
    spiwrite(color >> 8);
    spiwrite(color);
  }

  CS_HIGH();
  SPI_END_TRANSACTION();
}
