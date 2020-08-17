#ifndef __ARCH_CONFIG_H__
#define __ARCH_CONFIG_H__

  #include "mraa.h"
  #include "spi.h"
  #include "gpio.h"
  #include "compatibility.h"

  #include <stdint.h>
  #include <stdio.h>
  #include <time.h>
  #include <string.h>
  #include <sys/time.h>
  #include <stddef.h>
  #include <iostream>
  #include <unistd.h>
  #include <stdlib.h>
  
  //#include <UtilTime.h> // Precompiled arduino x86 based utiltime for timing functions

  // GCC a Arduino Missing
  #define HIGH	1
  #define LOW	0
  #define _BV(x) (1<<(x))
  #define pgm_read_word(p) (*(p))
  #define pgm_read_byte(p) (*(p))
  #define pgm_read_ptr(p) (*(p))
  #define _SPI spi
  
  #define RF24_LINUX
  //typedef uint16_t prog_uint16_t;
  #define PSTR(x) (x)
  #define printf_P printf
  #define sprintf_P sprintf
  #define strlen_P strlen
  #define PROGMEM
  #define PRIPSTR "%s"

  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif
  
  #define digitalWrite(pin, value) gpio.write(pin, value)
  #define digitalRead(pin) GPIO::read(pin)
  #define pinMode(pin, direction) gpio.open(pin, direction)

  #ifndef __TIME_H__
    // Prophet: Redefine time functions only if precompiled arduino time is not included
	#define delay(milisec) __msleep(milisec)
	#define delayMicroseconds(usec) __usleep(usec)
	#define millis() __millis()
  #endif
  
  #define INPUT mraa::DIR_IN
  #define OUTPUT mraa::DIR_OUT  

  // SPI defines for ARDUINO API
  #define MSBFIRST 1
  #define SPI_MODE0 mraa::SPI_MODE0
  #define SPI_CLOCK_DIV2 8000000

#endif
