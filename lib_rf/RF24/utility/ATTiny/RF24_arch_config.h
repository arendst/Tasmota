
/*
 TMRh20 2015
 ATTiny Configuration File
*/

#ifndef __RF24_ARCH_CONFIG_H__
#define __RF24_ARCH_CONFIG_H__

  /*** USER DEFINES:  ***/  
  //#define FAILURE_HANDLING
  //#define MINIMAL
  /**********************/

  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)



  #if ARDUINO < 100
	#include <WProgram.h>
  #else
	#include <Arduino.h>
  #endif
  #include <stddef.h>  
  
  // Include the header file for SPI functions ( Main SPI code is contained in RF24.cpp for simplicity )
  #include "spi.h"

  #define _SPI SPI
  
  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
	#if defined(RF24_TINY)
	#define printf_P(...)
    #endif
  #endif

  #include <avr/pgmspace.h>
  #define PRIPSTR "%S"
  
  

#endif // __RF24_ARCH_CONFIG_H__

