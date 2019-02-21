/*!
* @file Adafruit_SPITFT.cpp
*
* @mainpage Adafruit SPI TFT Displays
*
* @section intro_sec Introduction
  This is our library for generic SPI TFT Displays with
  address windows and 16 bit color (e.g. ILI9341, HX8357D, ST7735...)

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
* @section dependencies Dependencies
*
* This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
* Adafruit_GFX</a> being present on your system. Please make sure you have
* installed the latest version before using this library.
*
* @section author Author
*
* Written by Limor "ladyada" Fried for Adafruit Industries.
*
* @section license License
*
* BSD license, all text here must be included in any redistribution.
*
*/

#ifndef __AVR_ATtiny85__ // NOT A CHANCE of this stuff working on ATtiny!

#include "Adafruit_SPITFT.h"
#ifndef ARDUINO_STM32_FEATHER
  #include "pins_arduino.h"
#ifndef RASPI
    #include "wiring_private.h"
#endif
#endif
#include <limits.h>

#include "Adafruit_SPITFT_Macros.h"



/**************************************************************************/
/*!
    @brief  Pass 8-bit (each) R,G,B, get back 16-bit packed color
            This function converts 8-8-8 RGB data to 16-bit 5-6-5
    @param    red   Red 8 bit color
    @param    green Green 8 bit color
    @param    blue  Blue 8 bit color
    @return   Unsigned 16-bit down-sampled color in 5-6-5 format
*/
/**************************************************************************/
uint16_t Adafruit_SPITFT::color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}


/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SPI display driver with software SPI
    @param    w     Display width in pixels
    @param    h     Display height in pixels
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    mosi  SPI MOSI pin #
    @param    sclk  SPI Clock pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
    @param    miso  SPI MISO pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_SPITFT::Adafruit_SPITFT(uint16_t w, uint16_t h,
				 int8_t cs, int8_t dc, int8_t mosi,
				 int8_t sclk, int8_t rst, int8_t miso) 
  : Adafruit_GFX(w, h) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _sclk = sclk;
    _mosi = mosi;
    _miso = miso;
    _freq = 0;
#ifdef USE_FAST_PINIO
    dcport      = (RwReg *)portOutputRegister(digitalPinToPort(dc));
    dcpinmask   = digitalPinToBitMask(dc);
    clkport     = (RwReg *)portOutputRegister(digitalPinToPort(sclk));
    clkpinmask  = digitalPinToBitMask(sclk);
    mosiport    = (RwReg *)portOutputRegister(digitalPinToPort(mosi));
    mosipinmask = digitalPinToBitMask(mosi);
    if(miso >= 0){
        misoport    = (RwReg *)portInputRegister(digitalPinToPort(miso));
        misopinmask = digitalPinToBitMask(miso);
    } else {
        misoport    = 0;
        misopinmask = 0;
    }
    if(cs >= 0) {
        csport    = (RwReg *)portOutputRegister(digitalPinToPort(cs));
        cspinmask = digitalPinToBitMask(cs);
    } else {
        // No chip-select line defined; might be permanently tied to GND.
        // Assign a valid GPIO register (though not used for CS), and an
        // empty pin bitmask...the nonsense bit-twiddling might be faster
        // than checking _cs and possibly branching.
        csport    = dcport;
        cspinmask = 0;
    }
#endif
}

/**************************************************************************/
/*!
    @brief  Instantiate Adafruit SPI display driver with hardware SPI
    @param    w     Display width in pixels
    @param    h     Display height in pixels
    @param    cs    Chip select pin #
    @param    dc    Data/Command pin #
    @param    rst   Reset pin # (optional, pass -1 if unused)
*/
/**************************************************************************/
Adafruit_SPITFT::Adafruit_SPITFT(uint16_t w, uint16_t h,
				 int8_t cs, int8_t dc, int8_t rst) 
  : Adafruit_GFX(w, h) {
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _sclk = -1;
    _mosi = -1;
    _miso = -1;
    _freq = 0;
#ifdef USE_FAST_PINIO
    clkport     = 0;
    clkpinmask  = 0;
    mosiport    = 0;
    mosipinmask = 0;
    misoport    = 0;
    misopinmask = 0;
    dcport      = (RwReg *)portOutputRegister(digitalPinToPort(dc));
    dcpinmask   = digitalPinToBitMask(dc);
    if(cs >= 0) {
        csport    = (RwReg *)portOutputRegister(digitalPinToPort(cs));
        cspinmask = digitalPinToBitMask(cs);
    } else {
        // See notes in prior constructor.
        csport    = dcport;
        cspinmask = 0;
    }
#endif
}

/**************************************************************************/
/*!
    @brief   Initialiaze the SPI interface (hardware or software)
    @param    freq  The desired maximum SPI hardware clock frequency
*/
/**************************************************************************/
void Adafruit_SPITFT::initSPI(uint32_t freq) {
    _freq = freq;

    // Control Pins
    if(_cs >= 0) {
        pinMode(_cs, OUTPUT);
        digitalWrite(_cs, HIGH); // Deselect
    }
    pinMode(_dc, OUTPUT);
    digitalWrite(_dc, LOW);

    // Software SPI
    if(_sclk >= 0){
        pinMode(_mosi, OUTPUT);
        digitalWrite(_mosi, LOW);
        pinMode(_sclk, OUTPUT);
        digitalWrite(_sclk, HIGH);
        if(_miso >= 0){
            pinMode(_miso, INPUT);
        }
    }

    // Hardware SPI
    SPI_BEGIN();

    // toggle RST low to reset
    if (_rst >= 0) {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(100);
        digitalWrite(_rst, LOW);
        delay(100);
        digitalWrite(_rst, HIGH);
        delay(200);
    }
}

/**************************************************************************/
/*!
    @brief   Read one byte from SPI interface (hardware or software
    @returns One byte, MSB order
*/
/**************************************************************************/
uint8_t Adafruit_SPITFT::spiRead() {
    if(_sclk < 0){
        return HSPI_READ();
    }
    if(_miso < 0){
        return 0;
    }
    uint8_t r = 0;
    for (uint8_t i=0; i<8; i++) {
        SSPI_SCK_LOW();
        SSPI_SCK_HIGH();
        r <<= 1;
        if (SSPI_MISO_READ()){
            r |= 0x1;
        }
    }
    return r;
}

/**************************************************************************/
/*!
    @brief   Write one byte to SPI interface (hardware or software
    @param  b  One byte to send, MSB order
*/
/**************************************************************************/
void Adafruit_SPITFT::spiWrite(uint8_t b) {
    if(_sclk < 0){
        HSPI_WRITE(b);
        return;
    }
    for(uint8_t bit = 0x80; bit; bit >>= 1){
        if((b) & bit){
            SSPI_MOSI_HIGH();
        } else {
            SSPI_MOSI_LOW();
        }
        SSPI_SCK_LOW();
        SSPI_SCK_HIGH();
    }
}


/*
 * Transaction API
 * */

/**************************************************************************/
/*!
    @brief   Begin an SPI transaction & set CS low.
*/
/**************************************************************************/
void inline Adafruit_SPITFT::startWrite(void){
    SPI_BEGIN_TRANSACTION();
    SPI_CS_LOW();
}

/**************************************************************************/
/*!
    @brief   Begin an SPI transaction & set CS high.
*/
/**************************************************************************/
void inline Adafruit_SPITFT::endWrite(void){
    SPI_CS_HIGH();
    SPI_END_TRANSACTION();
}

/**************************************************************************/
/*!
    @brief   Write a command byte (must have a transaction in progress)
    @param   cmd  The 8-bit command to send
*/
/**************************************************************************/
void Adafruit_SPITFT::writeCommand(uint8_t cmd){
    SPI_DC_LOW();
    spiWrite(cmd);
    SPI_DC_HIGH();
}

/**************************************************************************/
/*!
    @brief   Push a 2-byte color to the framebuffer RAM, will start transaction
    @param    color 16-bit 5-6-5 Color to draw
*/
/**************************************************************************/
void Adafruit_SPITFT::pushColor(uint16_t color) {
  startWrite();
  SPI_WRITE16(color);
  endWrite();
}



/**************************************************************************/
/*!
    @brief   Blit multiple 2-byte colors  (must have a transaction in progress)
    @param    colors Array of 16-bit 5-6-5 Colors to draw
    @param    len  How many pixels to draw - 2 bytes per pixel!
*/
/**************************************************************************/
void Adafruit_SPITFT::writePixels(uint16_t * colors, uint32_t len){
    SPI_WRITE_PIXELS((uint8_t*)colors , len * 2);
}

/**************************************************************************/
/*!
    @brief   Blit a 2-byte color many times  (must have a transaction in progress)
    @param    color  The 16-bit 5-6-5 Color to draw
    @param    len    How many pixels to draw
*/
/**************************************************************************/
void Adafruit_SPITFT::writeColor(uint16_t color, uint32_t len){
#ifdef SPI_HAS_WRITE_PIXELS
    if(_sclk >= 0){
        for (uint32_t t=0; t<len; t++){
            writePixel(color);
        }
        return;
    }
    static uint16_t temp[SPI_MAX_PIXELS_AT_ONCE];
    size_t blen = (len > SPI_MAX_PIXELS_AT_ONCE)?SPI_MAX_PIXELS_AT_ONCE:len;
    uint16_t tlen = 0;

    for (uint32_t t=0; t<blen; t++){
        temp[t] = color;
    }

    while(len){
        tlen = (len>blen)?blen:len;
        writePixels(temp, tlen);
        len -= tlen;
    }
#else
    uint8_t hi = color >> 8, lo = color;
    if(_sclk < 0){ //AVR Optimization
        for (uint32_t t=len; t; t--){
            HSPI_WRITE(hi);
            HSPI_WRITE(lo);
        }
        return;
    }
    for (uint32_t t=len; t; t--){
        spiWrite(hi);
        spiWrite(lo);
    }
#endif
}

/**************************************************************************/
/*!
   @brief    Write a pixel (must have a transaction in progress)
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_SPITFT::writePixel(int16_t x, int16_t y, uint16_t color) {
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
    setAddrWindow(x,y,1,1);
    writePixel(color);
}

/**************************************************************************/
/*!
   @brief    Write a filled rectangle (must have a transaction in progress)
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_SPITFT::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    if((x >= _width) || (y >= _height)) return;
    int16_t x2 = x + w - 1, y2 = y + h - 1;
    if((x2 < 0) || (y2 < 0)) return;

    // Clip left/top
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(y < 0) {
        y = 0;
        h = y2 + 1;
    }

    // Clip right/bottom
    if(x2 >= _width)  w = _width  - x;
    if(y2 >= _height) h = _height - y;

    int32_t len = (int32_t)w * h;
    setAddrWindow(x, y, w, h);
    writeColor(color, len);
}

/**************************************************************************/
/*!
   @brief    Write a perfectly vertical line (must have a transaction in progress)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void inline Adafruit_SPITFT::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
    writeFillRect(x, y, 1, h, color);
}

/**************************************************************************/
/*!
   @brief    Write a perfectly horizontal line (must have a transaction in progress)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void inline Adafruit_SPITFT::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
    writeFillRect(x, y, w, 1, color);
}

/**************************************************************************/
/*!
   @brief    Draw a pixel - sets up transaction
    @param   x   x coordinate
    @param   y   y coordinate
   @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void Adafruit_SPITFT::drawPixel(int16_t x, int16_t y, uint16_t color){
    startWrite();
    writePixel(x, y, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief    Write a perfectly vertical line - sets up transaction
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_SPITFT::drawFastVLine(int16_t x, int16_t y,
        int16_t h, uint16_t color) {
    startWrite();
    writeFastVLine(x, y, h, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief    Write a perfectly horizontal line - sets up transaction
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_SPITFT::drawFastHLine(int16_t x, int16_t y,
        int16_t w, uint16_t color) {
    startWrite();
    writeFastHLine(x, y, w, color);
    endWrite();
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void Adafruit_SPITFT::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color) {
    startWrite();
    writeFillRect(x,y,w,h,color);
    endWrite();
}


/**************************************************************************/
/*!
    @brief      Invert the display using built-in hardware command
    @param   i  True if you want to invert, false to make 'normal'
*/
/**************************************************************************/
void Adafruit_SPITFT::invertDisplay(boolean i) {
  startWrite();
  writeCommand(i ? invertOnCommand : invertOffCommand);
  endWrite();
}


/**************************************************************************/
/*!
   @brief   Draw a 16-bit image (RGB 5/6/5) at the specified (x,y) position.  
   For 16-bit display devices; no color reduction performed.
   Adapted from https://github.com/PaulStoffregen/ILI9341_t3 
   by Marc MERLIN. See examples/pictureEmbed to use this.
   5/6/2017: function name and arguments have changed for compatibility
   with current GFX library and to avoid naming problems in prior
   implementation.  Formerly drawBitmap() with arguments in different order.

    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    pcolors  16-bit array with 16-bit color bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Height of bitmap in pixels
*/
/**************************************************************************/
void Adafruit_SPITFT::drawRGBBitmap(int16_t x, int16_t y,
  uint16_t *pcolors, int16_t w, int16_t h) {

    int16_t x2, y2; // Lower-right coord
    if(( x             >= _width ) ||      // Off-edge right
       ( y             >= _height) ||      // " top
       ((x2 = (x+w-1)) <  0      ) ||      // " left
       ((y2 = (y+h-1)) <  0)     ) return; // " bottom

    int16_t bx1=0, by1=0, // Clipped top-left within bitmap
            saveW=w;      // Save original bitmap width value
    if(x < 0) { // Clip left
        w  +=  x;
        bx1 = -x;
        x   =  0;
    }
    if(y < 0) { // Clip top
        h  +=  y;
        by1 = -y;
        y   =  0;
    }
    if(x2 >= _width ) w = _width  - x; // Clip right
    if(y2 >= _height) h = _height - y; // Clip bottom

    pcolors += by1 * saveW + bx1; // Offset bitmap ptr to clipped top-left
    startWrite();
    setAddrWindow(x, y, w, h); // Clipped area
    while(h--) { // For each (clipped) scanline...
      writePixels(pcolors, w); // Push one (clipped) row
      pcolors += saveW; // Advance pointer by one full (unclipped) line
    }
    endWrite();
}

#endif // !__AVR_ATtiny85__
