
/*
 * Control Pins
 * */

#ifdef USE_FAST_PINIO
#define SPI_DC_HIGH()           *dcport |=  dcpinmask
#define SPI_DC_LOW()            *dcport &= ~dcpinmask
#define SPI_CS_HIGH()           *csport |= cspinmask
#define SPI_CS_LOW()            *csport &= ~cspinmask
#else
#define SPI_DC_HIGH()           digitalWrite(_dc, HIGH)
#define SPI_DC_LOW()            digitalWrite(_dc, LOW)
#define SPI_CS_HIGH()           digitalWrite(_cs, HIGH)
#define SPI_CS_LOW()            digitalWrite(_cs, LOW)
#endif

/*
 * Software SPI Macros
 * */

#ifdef USE_FAST_PINIO
#define SSPI_MOSI_HIGH()        *mosiport |=  mosipinmask
#define SSPI_MOSI_LOW()         *mosiport &= ~mosipinmask
#define SSPI_SCK_HIGH()         *clkport |=  clkpinmask
#define SSPI_SCK_LOW()          *clkport &= ~clkpinmask
#define SSPI_MISO_READ()        ((*misoport & misopinmask) != 0)
#else
#define SSPI_MOSI_HIGH()        digitalWrite(_mosi, HIGH)
#define SSPI_MOSI_LOW()         digitalWrite(_mosi, LOW)
#define SSPI_SCK_HIGH()         digitalWrite(_sclk, HIGH)
#define SSPI_SCK_LOW()          digitalWrite(_sclk, LOW)
#define SSPI_MISO_READ()        digitalRead(_miso)
#endif

#define SSPI_BEGIN_TRANSACTION()
#define SSPI_END_TRANSACTION()
#define SSPI_WRITE(v)           spiWrite(v)
#define SSPI_WRITE16(s)         SSPI_WRITE((s) >> 8); SSPI_WRITE(s)
#define SSPI_WRITE32(l)         SSPI_WRITE((l) >> 24); SSPI_WRITE((l) >> 16); SSPI_WRITE((l) >> 8); SSPI_WRITE(l)
#define SSPI_WRITE_PIXELS(c,l)  for(uint32_t i=0; i<(l); i+=2){ SSPI_WRITE(((uint8_t*)(c))[i+1]); SSPI_WRITE(((uint8_t*)(c))[i]); }

/*
 * Hardware SPI Macros
 * */

#define SPI_OBJECT  SPI

#if defined (__AVR__) ||  defined(TEENSYDUINO) ||  defined(ARDUINO_ARCH_STM32F1)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClockDivider(SPI_CLOCK_DIV2);
#elif defined (__arm__)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClockDivider(11);
#elif defined(ESP8266) || defined(ESP32)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setFrequency(_freq);
#elif defined(RASPI)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClock(_freq);
#elif defined(ARDUINO_ARCH_STM32F1)
    #define HSPI_SET_CLOCK() SPI_OBJECT.setClock(_freq);
#else
    #define HSPI_SET_CLOCK()
#endif

#ifdef SPI_HAS_TRANSACTION
    #define HSPI_BEGIN_TRANSACTION() SPI_OBJECT.beginTransaction(SPISettings(_freq, MSBFIRST, SPI_MODE0))
    #define HSPI_END_TRANSACTION()   SPI_OBJECT.endTransaction()
#else
    #define HSPI_BEGIN_TRANSACTION() HSPI_SET_CLOCK(); SPI_OBJECT.setBitOrder(MSBFIRST); SPI_OBJECT.setDataMode(SPI_MODE0)
    #define HSPI_END_TRANSACTION()
#endif

#ifdef ESP32
    #define SPI_HAS_WRITE_PIXELS
#endif
#if defined(ESP8266) || defined(ESP32)
    // Optimized SPI (ESP8266 and ESP32)
    #define HSPI_READ()              SPI_OBJECT.transfer(0)
    #define HSPI_WRITE(b)            SPI_OBJECT.write(b)
    #define HSPI_WRITE16(s)          SPI_OBJECT.write16(s)
    #define HSPI_WRITE32(l)          SPI_OBJECT.write32(l)
    #ifdef SPI_HAS_WRITE_PIXELS
        #define SPI_MAX_PIXELS_AT_ONCE  32
        #define HSPI_WRITE_PIXELS(c,l)   SPI_OBJECT.writePixels(c,l)
    #else
        #define HSPI_WRITE_PIXELS(c,l)   for(uint32_t i=0; i<((l)/2); i++){ SPI_WRITE16(((uint16_t*)(c))[i]); }
    #endif
#else
    // Standard Byte-by-Byte SPI

    #if defined (__AVR__) || defined(TEENSYDUINO)
static inline uint8_t _avr_spi_read(void) __attribute__((always_inline));
static inline uint8_t _avr_spi_read(void) {
    uint8_t r = 0;
    SPDR = r;
    while(!(SPSR & _BV(SPIF)));
    r = SPDR;
    return r;
}
        #define HSPI_WRITE(b)            {SPDR = (b); while(!(SPSR & _BV(SPIF)));}
        #define HSPI_READ()              _avr_spi_read()
    #else
        #define HSPI_WRITE(b)            SPI_OBJECT.transfer((uint8_t)(b))
        #define HSPI_READ()              HSPI_WRITE(0)
    #endif
    #define HSPI_WRITE16(s)          HSPI_WRITE((s) >> 8); HSPI_WRITE(s)
    #define HSPI_WRITE32(l)          HSPI_WRITE((l) >> 24); HSPI_WRITE((l) >> 16); HSPI_WRITE((l) >> 8); HSPI_WRITE(l)
    #define HSPI_WRITE_PIXELS(c,l)   for(uint32_t i=0; i<(l); i+=2){ HSPI_WRITE(((uint8_t*)(c))[i+1]); HSPI_WRITE(((uint8_t*)(c))[i]); }
#endif

#define SPI_BEGIN()             if(_sclk < 0){SPI_OBJECT.begin();}
#define SPI_BEGIN_TRANSACTION() if(_sclk < 0){HSPI_BEGIN_TRANSACTION();}
#define SPI_END_TRANSACTION()   if(_sclk < 0){HSPI_END_TRANSACTION();}
#define SPI_WRITE16(s)          if(_sclk < 0){HSPI_WRITE16(s);}else{SSPI_WRITE16(s);}
#define SPI_WRITE32(l)          if(_sclk < 0){HSPI_WRITE32(l);}else{SSPI_WRITE32(l);}
#define SPI_WRITE_PIXELS(c,l)   if(_sclk < 0){HSPI_WRITE_PIXELS(c,l);}else{SSPI_WRITE_PIXELS(c,l);}
