#ifndef _ADAFRUIT_SPITFT_
#define _ADAFRUIT_SPITFT_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#include <SPI.h>
#include "Adafruit_GFX.h"

#define USE_FAST_PINIO

#if defined(__AVR__)
  typedef volatile uint8_t RwReg;
#elif defined(ARDUINO_STM32_FEATHER)
  typedef volatile uint32 RwReg;
  #undef USE_FAST_PINIO
#elif defined(__OPENCR__) || defined (__OPENCM904__)
  #undef USE_FAST_PINIO
#elif defined(ARDUINO_FEATHER52) || defined(__arm__)
  typedef volatile uint32_t RwReg;
#elif defined(ESP32) || defined(ESP8266)
  typedef volatile uint32_t RwReg;
  #undef USE_FAST_PINIO
#else
  #undef USE_FAST_PINIO
#endif

#include "Adafruit_SPITFT_Macros.h"

/// A heavily optimized SPI display subclass of GFX. Manages SPI bitbanging, transactions, DMA, etc! Despite being called SPITFT, the classic SPI data/command interface is also used by OLEDs.
class Adafruit_SPITFT : public Adafruit_GFX {
    protected:

    public:
        Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
        Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t _CS, int8_t _DC, int8_t _RST = -1);

        virtual void begin(uint32_t freq) = 0;  ///< Virtual begin() function to set SPI frequency, must be overridden in subclass. @param freq Maximum SPI hardware clock speed

        void      initSPI(uint32_t freq);

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

	/*!
	  @brief   SPI displays set an address window rectangle for blitting pixels
	  @param  x  Top left corner x coordinate
	  @param  y  Top left corner x coordinate
	  @param  w  Width of window
	  @param  h  Height of window
	*/
	virtual void      setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;

	/*!
	  @brief   Write a 2-byte color  (must have a transaction in progress)
	  @param    color 16-bit 5-6-5 Color to draw
	*/
	void      inline writePixel(uint16_t color) { SPI_WRITE16(color); }
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
	void      invertDisplay(boolean i);

        uint16_t  color565(uint8_t r, uint8_t g, uint8_t b);

    protected:
        uint32_t _freq;         ///< SPI clock frequency (for hardware SPI)
#if defined (__AVR__) || defined(TEENSYDUINO) || defined (ESP8266) || defined (ESP32)
        int8_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#else 
        int32_t  _cs,            ///< Arduino pin # for chip-select pin 
	  _dc,                   ///< Arduino pin # for data-command pin 
	  _rst,                  ///< Arduino pin # for reset pin 
	  _sclk,                 ///< Arduino pin # for SPI clock pin 
	  _mosi,                 ///< Arduino pin # for SPI MOSI pin 
	  _miso;                 ///< Arduino pin # for SPI MISO pin 
#endif

#ifdef USE_FAST_PINIO
        volatile RwReg *mosiport,   ///< Direct chip register for toggling MOSI with fast bitbang IO
	  *misoport,            ///< Direct chip register for toggling MISO with fast bitbang IO
	  *clkport,             ///< Direct chip register for toggling CLK with fast bitbang IO
	  *dcport,              ///< Direct chip register for toggling DC with fast bitbang IO
	  *csport;              ///< Direct chip register for toggling CS with fast bitbang IO
        RwReg  mosipinmask,     ///< bitmask for turning on/off MOSI with fast register bitbang IO 
	  misopinmask,          ///< bitmask for turning on/off MISO with fast register bitbang IO 
	  clkpinmask,           ///< bitmask for turning on/off CLK with fast register bitbang IO 
	  cspinmask,            ///< bitmask for turning on/off CS with fast register bitbang IO 
	  dcpinmask;            ///< bitmask for turning on/off DC with fast register bitbang IO 
#endif

        void        writeCommand(uint8_t cmd);
        void        spiWrite(uint8_t v);
        uint8_t     spiRead(void);

	uint8_t   invertOnCommand = 0,    ///<  SPI command byte to turn on invert
	  invertOffCommand = 0;           ///<  SPI command byte to turn off invert
	int16_t   _xstart = 0;   ///< Many displays don't have pixels starting at (0,0) of the internal framebuffer, this is the x offset from 0 to align
	int16_t   _ystart = 0;   ///< Many displays don't have pixels starting at (0,0) of the internal framebuffer, this is the y offset from 0 to align
};

#endif
