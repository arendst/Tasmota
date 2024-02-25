#if !defined(_RADIOLIB_PRINT_H)
#define _RADIOLIB_PRINT_H

#include <string.h>

#include "ITA2String.h"

// supported encoding schemes
#define RADIOLIB_ASCII                                          0
#define RADIOLIB_ASCII_EXTENDED                                 1
#define RADIOLIB_ITA2                                           2

// based on Arduino Print class
class RadioLibPrint {
  public:
    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str) {
      if (str == NULL) return 0;
      return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char *buffer, size_t size) {
      return write((const uint8_t *)buffer, size);
    }

    size_t print(ITA2String& ita2);
    size_t println(ITA2String& ita2);

    #if defined(RADIOLIB_BUILD_ARDUINO)
    size_t print(const __FlashStringHelper *);
    size_t print(const String &);

    size_t println(const __FlashStringHelper *);
    size_t println(const String &);
    #endif

    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);

    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(void);

#if !RADIOLIB_GODMODE
  protected:
#endif
    uint8_t encoding = RADIOLIB_ASCII_EXTENDED;
    const char* lineFeed;

    size_t printNumber(unsigned long, uint8_t);
    size_t printFloat(double, uint8_t);

};

#endif
