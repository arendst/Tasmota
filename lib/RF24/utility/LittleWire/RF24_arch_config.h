#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

#include <stdint.h>
  #include <stdio.h>
  #include <time.h>
  #include <string.h>
  #include <sys/time.h>
  #include <stddef.h>

  // Additional fixes for LittleWire
  #include <LittleWireSPI/littlewirespi.h>
  #include <LittleWireSPI/avr_fixes.h>
  extern LittleWireSPI _SPI;

  // GCC a Arduino Missing
  #define _BV(x) (1<<(x))
  #define pgm_read_word(p) (*(p))
  #define pgm_read_byte(p) (*(p))
  #define pgm_read_ptr(p) (*(p))
  
  //typedef uint16_t prog_uint16_t;
  #define PSTR(x) (x)
  #define printf_P printf
  #define strlen_P strlen
  #define PROGMEM
  #define PRIPSTR "%s"

  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
	#if defined(RF24_TINY)
	  #define printf_P(...)
    #endif
  #endif
  
  
#endif
