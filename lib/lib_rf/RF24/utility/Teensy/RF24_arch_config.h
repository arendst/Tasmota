

  #if ARDUINO < 100
	#include <WProgram.h>
  #else
	#include <Arduino.h>
  #endif

  #include <stddef.h>
  
  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>

  #include <SPI.h>
  #define _SPI SPI

  #define printf Serial.printf
  
  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif
  

  #define PRIPSTR "%s"




