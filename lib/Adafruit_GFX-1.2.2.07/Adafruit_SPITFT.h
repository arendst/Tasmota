
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


#if defined(ARDUINO_STM32_FEATHER)
typedef volatile uint32 RwReg;
#endif
#if defined(ARDUINO_FEATHER52)
typedef volatile uint32_t RwReg;
#endif

class Adafruit_SPITFT : public Adafruit_GFX {
    protected:

    public:
        Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK, int8_t _RST = -1, int8_t _MISO = -1);
        Adafruit_SPITFT(uint16_t w, uint16_t h, int8_t _CS, int8_t _DC, int8_t _RST = -1);

        virtual void begin(uint32_t freq) = 0;
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
	virtual   void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) = 0;
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

        uint16_t  color565(uint8_t r, uint8_t g, uint8_t b);

    protected:
        uint32_t _freq;
#if defined (__AVR__) || defined(TEENSYDUINO) || defined (ESP8266) || defined (ESP32)
        int8_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#else 
        int32_t  _cs, _dc, _rst, _sclk, _mosi, _miso;
#endif

#ifdef USE_FAST_PINIO
        volatile RwReg *mosiport, *misoport, *clkport, *dcport, *csport;
        RwReg  mosipinmask, misopinmask, clkpinmask, cspinmask, dcpinmask;
#endif

        void        writeCommand(uint8_t cmd);
        void        spiWrite(uint8_t v);
        uint8_t     spiRead(void);
};

#endif
