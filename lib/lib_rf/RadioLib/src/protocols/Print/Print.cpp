#include "Print.h"

#include <ctype.h>
#include <math.h>

size_t RadioLibPrint::print(ITA2String& ita2) {
  uint8_t enc = this->encoding;
  this->encoding = RADIOLIB_ITA2;
  uint8_t* arr = ita2.byteArr();
  size_t n = write(arr, ita2.length());
  delete[] arr;
  this->encoding = enc;
  return(n);
}

size_t RadioLibPrint::println(ITA2String& ita2) {
  size_t n = RadioLibPrint::print(ita2);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  while (size--) {
    if (write(*buffer++)) n++;
    else break;
  }
  return n;
}

#if defined(RADIOLIB_BUILD_ARDUINO)
size_t RadioLibPrint::print(const __FlashStringHelper* fstr) {
  // read flash string length
  size_t len = 0;
  RADIOLIB_NONVOLATILE_PTR p = reinterpret_cast<RADIOLIB_NONVOLATILE_PTR>(fstr);
  while(true) {
    char c = RADIOLIB_NONVOLATILE_READ_BYTE(p++);
    len++;
    if(c == '\0') {
      break;
    }
  }

  // dynamically allocate memory
  #if RADIOLIB_STATIC_ONLY
    char str[RADIOLIB_STATIC_ARRAY_SIZE];
  #else
    char* str = new char[len];
  #endif

  // copy string from flash
  p = reinterpret_cast<RADIOLIB_NONVOLATILE_PTR>(fstr);
  for(size_t i = 0; i < len; i++) {
    str[i] = RADIOLIB_NONVOLATILE_READ_BYTE(p + i);
  }

  size_t n = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String ita2 = ITA2String(str);
    n = RadioLibPrint::print(ita2);
  } else {
    n = write((uint8_t*)str, len);
  }
  #if !RADIOLIB_STATIC_ONLY
    delete[] str;
  #endif
  return(n);
}

size_t RadioLibPrint::print(const String& str) {
  size_t n = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String ita2 = ITA2String(str.c_str());
    n = RadioLibPrint::print(ita2);
  } else {
    n = write((uint8_t*)str.c_str(), str.length());
  }
  return(n);
}

size_t RadioLibPrint::println(const __FlashStringHelper* fstr) {
  size_t n = RadioLibPrint::print(fstr);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(const String& str) {
  size_t n = RadioLibPrint::print(str);
  n += RadioLibPrint::println();
  return(n);
}
#endif

size_t RadioLibPrint::print(const char str[]) {
  size_t n = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String ita2 = ITA2String(str);
    n = RadioLibPrint::print(ita2);
  } else {
    n = write((uint8_t*)str, strlen(str));
  }
  return(n);
}

size_t RadioLibPrint::print(char c) {
  size_t n = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String ita2 = ITA2String(c);
    n = RadioLibPrint::print(ita2);
  } else {
    n = write(c);
  }
  return(n);
}

size_t RadioLibPrint::print(unsigned char b, int base) {
  return(RadioLibPrint::print((unsigned long)b, base));
}

size_t RadioLibPrint::print(int n, int base) {
  return(RadioLibPrint::print((long)n, base));
}

size_t RadioLibPrint::print(unsigned int n, int base) {
  return(RadioLibPrint::print((unsigned long)n, base));
}

size_t RadioLibPrint::print(long n, int base) {
  if(base == 0) {
    return(write(n));
  } else if(base == DEC) {
    if (n < 0) {
      int t = RadioLibPrint::print('-');
      n = -n;
      return(RadioLibPrint::printNumber(n, DEC) + t);
    }
    return(RadioLibPrint::printNumber(n, DEC));
  } else {
    return(RadioLibPrint::printNumber(n, base));
  }
}

size_t RadioLibPrint::print(unsigned long n, int base) {
  if(base == 0) {
    return(write(n));
  } else {
    return(RadioLibPrint::printNumber(n, base));
  }
}

size_t RadioLibPrint::print(double n, int digits) {
  return(RadioLibPrint::printFloat(n, digits));
}

size_t RadioLibPrint::println(const char* str) {
  size_t n = RadioLibPrint::print(str);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(char c) {
  size_t n = RadioLibPrint::print(c);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(unsigned char b, int base) {
  size_t n = RadioLibPrint::print(b, base);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(int num, int base) {
  size_t n = RadioLibPrint::print(num, base);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(unsigned int num, int base) {
  size_t n = RadioLibPrint::print(num, base);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(long num, int base) {
  size_t n = RadioLibPrint::print(num, base);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(unsigned long num, int base) {
  size_t n = RadioLibPrint::print(num, base);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(double d, int digits) {
  size_t n = RadioLibPrint::print(d, digits);
  n += RadioLibPrint::println();
  return(n);
}

size_t RadioLibPrint::println(void) {
  size_t n = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String lf = ITA2String("\r\n");
    n = RadioLibPrint::print(lf);
  } else {
    n = write("\r\n");
  }
  return(n);
}

size_t RadioLibPrint::printNumber(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1];
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  if(base < 2) {
    base = 10;
  }

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  size_t l = 0;
  if(this->encoding == RADIOLIB_ITA2) {
    ITA2String ita2 = ITA2String(str);
    uint8_t* arr = ita2.byteArr();
    l = write(arr, ita2.length());
    delete[] arr;
  } else {
    l = write(str);
  }

  return(l);
}

/// \todo improve ITA2 float print speed (characters are sent one at a time)
size_t RadioLibPrint::printFloat(double number, uint8_t digits)  {
  size_t n = 0;

  char code[] = {0x00, 0x00, 0x00, 0x00};
  if (isnan(number)) strcpy(code, "nan");
  if (isinf(number)) strcpy(code, "inf");
  if (number > 4294967040.0) strcpy(code, "ovf");  // constant determined empirically
  if (number <-4294967040.0) strcpy(code, "ovf");  // constant determined empirically

  if(code[0] != 0x00) {
    if(this->encoding == RADIOLIB_ITA2) {
      ITA2String ita2 = ITA2String(code);
      uint8_t* arr = ita2.byteArr();
      n = write(arr, ita2.length());
      delete[] arr;
      return(n);
    } else {
      return(write(code));
    }
  }

  // Handle negative numbers
  if (number < 0.0) {
    if(this->encoding == RADIOLIB_ITA2) {
      ITA2String ita2 = ITA2String("-");
      uint8_t* arr = ita2.byteArr();
      n += write(arr, ita2.length());
      delete[] arr;
    } else {
      n += RadioLibPrint::print('-');
    }
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for(uint8_t i = 0; i < digits; ++i) {
    rounding /= 10.0;
  }
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += RadioLibPrint::print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if(digits > 0) {
    if(encoding == RADIOLIB_ITA2) {
      ITA2String ita2 = ITA2String(".");
      uint8_t* arr = ita2.byteArr();
      n += write(arr, ita2.length());
      delete[] arr;
    } else {
      n += RadioLibPrint::print('.');
    }
  }

  // Extract digits from the remainder one at a time
  while(digits-- > 0) {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += RadioLibPrint::print(toPrint);
    remainder -= toPrint;
  }

  return n;
}
