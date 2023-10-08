
/*
 TMRh20 2015
 RF24 Configuration file for Arduino Due
 
 */

#ifndef __RF24_ARCH_CONFIG_H__
#define __RF24_ARCH_CONFIG_H__

  /*** USER DEFINES:  ***/  
  //#define FAILURE_HANDLING
  //#define SERIAL_DEBUG
  //#define MINIMAL
  /**********************/
  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)

  #include <Arduino.h>
  #include <SPI.h>

  #define _BV(x) (1<<(x))
  #define _SPI SPI
  
  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif

  #define printf_P printf
  #define strlen_P strlen
  #define PRIPSTR "%s"


#endif // __RF24_CONFIG_H__

