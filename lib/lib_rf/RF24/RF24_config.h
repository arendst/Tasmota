
/*
 Copyright (C) 
  2011      J. Coliz <maniacbug@ymail.com>
  2015-2019 TMRh20
  2015      spaniakos <spaniakos@gmail.com>
  2015      nerdralph
  2015      zador-blood-stained
  2016      akatran
  2017-2019 Avamander <avamander@gmail.com>
  2019      IkpeohaGodson
         
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

/*** USER DEFINES:  ***/  
#define FAILURE_HANDLING
//#define SERIAL_DEBUG
//#define MINIMAL
//#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
//#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO
  
/**********************/
#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)

#if defined (SPI_HAS_TRANSACTION) && !defined (SPI_UART) && !defined (SOFTSPI)
  #define RF24_SPI_TRANSACTIONS
#endif // defined (SPI_HAS_TRANSACTION) && !defined (SPI_UART) && !defined (SOFTSPI)

//ATXMega
#if defined (__AVR_ATxmega64D3__) || defined (__AVR_ATxmega128D3__) || defined (__AVR_ATxmega192D3__) || defined (__AVR_ATxmega256D3__) || defined (__AVR_ATxmega384D3__) // In order to be available both in Windows and Linux this should take presence here.
  #define XMEGA
  #define XMEGA_D3
  #include "utility/ATXMegaD3/RF24_arch_config.h"

#elif ( !defined (ARDUINO) ) // Any non-arduino device is handled via configure/Makefile
  // The configure script detects device and copies the correct includes.h file to /utility/includes.h
  // This behavior can be overridden by calling configure with respective parameters
  // The includes.h file defines either RF24_RPi, MRAA, LITTLEWIRE or RF24_SPIDEV and includes the correct RF24_arch_config.h file
  #include "utility/includes.h"

//ATTiny  
#elif defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) || defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny2313__) || defined (__AVR_ATtiny4313__) || defined (__AVR_ATtiny861__)  
  #define RF24_TINY
  #include "utility/ATTiny/RF24_arch_config.h"

#elif defined (LITTLEWIRE) //LittleWire
  #include "utility/LittleWire/RF24_arch_config.h"

#elif defined (TEENSYDUINO) //Teensy
  #include "utility/Teensy/RF24_arch_config.h"  

#else //Everything else
  #include <Arduino.h>
  
  // RF modules support 10 Mhz SPI bus speed
  const uint32_t RF24_SPI_SPEED = 10000000;  

  #if defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)
    #if defined SPI_UART
      #include <SPI_UART.h>
      #define _SPI uspi
    #elif defined (SOFTSPI)
      // change these pins to your liking
      //
      #ifndef SOFT_SPI_MISO_PIN
        #define SOFT_SPI_MISO_PIN 9
      #endif // SOFT_SPI_MISO_PIN

      #ifndef SOFT_SPI_MOSI_PIN
        #define SOFT_SPI_MOSI_PIN 8
      #endif // SOFT_SPI_MOSI_PIN

      #ifndef SOFT_SPI_SCK_PIN
        #define SOFT_SPI_SCK_PIN 7
      #endif // SOFT_SPI_SCK_PIN

      const uint8_t SPI_MODE = 0;
      #define _SPI spi
  
    #else // !defined (SPI_UART) && !defined (SOFTSPI)
      #include <SPI.h>
      #define _SPI SPI
    #endif // !defined (SPI_UART) && !defined (SOFTSPI)

  #else // defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)
    // Define _BV for non-Arduino platforms and for Arduino DUE
    #include <stdint.h>
    #include <stdio.h>
    #include <string.h>

    #if defined(__arm__) || defined (__ARDUINO_X86__)
      #if defined (__arm__) && defined (SPI_UART)
        #include <SPI_UART.h>
        #define _SPI uspi

      #else // !defined (__arm__) || !defined (SPI_UART)
        #include <SPI.h>
        #define _SPI SPI

      #endif // !defined (__arm__) || !defined (SPI_UART)
    #elif !defined(__arm__) && !defined (__ARDUINO_X86__)
      extern HardwareSPI SPI;

    #endif // !defined(__arm__) && !defined (__ARDUINO_X86__)
 
    #define _BV(x) (1<<(x))
  #endif // defined (ARDUINO) && !defined (__arm__) && !defined (__ARDUINO_X86__)

  #ifdef SERIAL_DEBUG
    #define IF_SERIAL_DEBUG(x) ({x;})
  #else
    #define IF_SERIAL_DEBUG(x)
    #if defined(RF24_TINY)
      #define printf_P(...)

    #endif // defined(RF24_TINY)
  #endif // SERIAL_DEBUG
  
  #if defined (__ARDUINO_X86__)
    #define printf_P printf
    #define _BV(bit) (1<<(bit))

  #endif // defined (__ARDUINO_X86__)
  
  // Progmem is Arduino-specific
  // Arduino DUE is arm and does not include avr/pgmspace
  #if defined (ARDUINO_ARCH_ESP8266) || defined (ESP32)
    #include <pgmspace.h>
    #define PRIPSTR "%s"

  #elif defined (ESP32)
    #include <pgmspace.h>
    #define PRIPSTR "%s"

  #elif defined (ARDUINO) && !defined (ESP_PLATFORM) && ! defined (__arm__) && !defined (__ARDUINO_X86__) || defined (XMEGA)
    #include <avr/pgmspace.h>
    #define PRIPSTR "%S"

  #else // !defined (ARDUINO) || defined (ESP_PLATFORM) || defined (__arm__) || defined (__ARDUINO_X86__) && !defined (XMEGA)
    #if !defined (ARDUINO) // This doesn't work on Arduino DUE
      typedef char const char;

    #else // Fill in pgm_read_byte that is used, but missing from DUE
      #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
    #endif // !defined (ARDUINO)

    typedef uint16_t prog_uint16_t;
    #define PSTR(x) (x)
    #define printf_P printf
    #define strlen_P strlen
    #define PROGMEM
    #define pgm_read_word(p) (*(p))
    #define pgm_read_ptr(p) (*(p))
    #define PRIPSTR "%s"

  #endif // !defined (ARDUINO) || defined (ESP_PLATFORM) || defined (__arm__) || defined (__ARDUINO_X86__) && !defined (XMEGA)
#endif //Everything else

#endif // __RF24_CONFIG_H__
