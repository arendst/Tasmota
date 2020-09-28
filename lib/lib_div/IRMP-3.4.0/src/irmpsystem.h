/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpsystem.h - system specific includes and defines
 *
 * Copyright (c) 2009-2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _IRMPSYSTEM_H_
#define _IRMPSYSTEM_H_

#if !defined(_IRMP_H_) && !defined(_IRSND_H_)
#  error please include only irmp.h or irsnd.h, not irmpsystem.h
#endif

#if defined(ARDUINO)                                                                // AVR Arduino. Should be first, since it covers multiple platforms
// to avoid the other includes and defines which are incompatible with ARDUINO

#elif defined(__18CXX)                                                                // Microchip PIC C18 compiler
#  define PIC_C18

#elif defined(__XC8)                                                                // PIC XC8 compiler
#  include <xc.h>
#  define PIC_C18

#elif defined(__XC32)                                                               // XC32 or ChipKit compiler
#  define PIC_XC32

#elif defined(__PCM__) || defined(__PCB__) || defined(__PCH__)                      // CCS PIC compiler
#  define PIC_CCS

#elif defined(STM32L1XX_MD) || defined(STM32L1XX_MDP) || defined(STM32L1XX_HD)      // ARM STM32
#  include <stm32l1xx.h>
#  define ARM_STM32
#  define ARM_STM32L1XX
#  define F_CPU (SysCtlClockGet())

#elif defined(STM32F10X_LD) || defined(STM32F10X_LD_VL) \
   || defined(STM32F10X_MD) || defined(STM32F10X_MD_VL) \
   || defined(STM32F10X_HD) || defined(STM32F10X_HD_VL) \
   || defined(STM32F10X_XL) || defined(STM32F10X_CL)                                // ARM STM32
#  include <stm32f10x.h>
#  define ARM_STM32
#  define ARM_STM32F10X
#  define F_CPU (SysCtlClockGet())

#elif defined(STM32F30X)                                                            // ARM STM32
#  include <stm32f30x.h>
#  define ARM_STM32
#  define ARM_STM32F30X
#  define F_CPU (SysCtlClockGet())

#elif defined(STM32F4XX)                                                            // ARM STM32
#  include <stm32f4xx.h>
#  define ARM_STM32
#  define ARM_STM32F4XX

#elif defined(USE_HAL_DRIVER)                                                       // ARM STM32 with HAL Library
#  include "gpio.h"
#  if defined(_IRSND_H_)
#    include"tim.h"
#  endif
#  define ARM_STM32_HAL
#  define F_CPU SystemCoreClock

#elif defined(__SDCC_stm8)                                                          // STM8
#  define SDCC_STM8

#elif defined(TARGET_IS_BLIZZARD_RA2)                                               // TI Stellaris (tested on Stellaris Launchpad with Code Composer Studio)
#  define STELLARIS_ARM_CORTEX_M4
#  define F_CPU (SysCtlClockGet())

#elif defined(__xtensa__)                                                           // ESP8266 (Arduino)
#  include "Arduino.h"
#  include "ets_sys.h"
#  include "osapi.h"
#  include "gpio.h"
#  include "os_type.h"
#  include "c_types.h"
#  define uint_fast8_t uint8_t
#  define uint_fast16_t uint16_t

#elif defined(TEENSYDUINO) && (defined(__MK20DX256__) || defined(__MK20DX128__))    // Teensy 3.x (tested on Teensy 3.1 in Arduino 1.6.5 / Teensyduino 1.2.5)
#  include <core_pins.h>
#  define TEENSY_ARM_CORTEX_M4

#elif defined(unix) || defined(WIN32) || defined(__APPLE__)                         // Unix/Linux or Windows or Apple
#  define UNIX_OR_WINDOWS

#elif defined(__MBED__)                                                             // mbed platform
// #include "mbed.h"                                                                // if mbed.h is used, source must be compiled as cpp
#include "gpio_api.h"

#elif defined(IRMP_CHIBIOS_HAL)                                                     // ChibiOS HAL
#  include "hal.h"

#else
#  define ATMEL_AVR                                                                 // ATMEL AVR
#endif

#include <string.h>

#ifdef UNIX_OR_WINDOWS                                                              // Analyze on Unix/Linux or Windows
#  include <stdio.h>
#  include <stdlib.h>
#  define F_CPU 8000000L
#  define ANALYZE
#  include <stdint.h>
#  ifdef _MSC_VER
#    define IRMP_PACKED_STRUCT
#  endif
#endif

#if defined(ARDUINO)                                                                // AVR Arduino. Should be first, since it covers multiple platforms
// to avoid the other includes and defines which are incompatible with ARDUINO

#elif defined(ATMEL_AVR)
#  include <stdint.h>
#  include <stdio.h>
#  include <avr/io.h>
#  include <util/delay.h>
#  include <avr/pgmspace.h>
#  include <avr/interrupt.h>
#  define IRSND_OC2                     0       // OC2
#  define IRSND_OC2A                    1       // OC2A
#  define IRSND_OC2B                    2       // OC2B
#  define IRSND_OC0                     3       // OC0
#  define IRSND_OC0A                    4       // OC0A
#  define IRSND_OC0B                    5       // OC0B

#  define IRSND_XMEGA_OC0A              0       // OC0A
#  define IRSND_XMEGA_OC0B              1       // OC0B
#  define IRSND_XMEGA_OC0C              2       // OC0C
#  define IRSND_XMEGA_OC0D              3       // OC0D
#  define IRSND_XMEGA_OC1A              4       // OC1A
#  define IRSND_XMEGA_OC1B              5       // OC1B

#elif defined(STELLARIS_ARM_CORTEX_M4)

#  include "inc/hw_ints.h"
#  include "inc/hw_memmap.h"
#  include "inc/hw_types.h"
#  include "inc/hw_gpio.h"
#  include "driverlib/fpu.h"
#  include "driverlib/sysctl.h"
#  include "driverlib/interrupt.h"
#  include "driverlib/gpio.h"
#  include "driverlib/rom.h"
#  include "driverlib/systick.h"
#  include "driverlib/pin_map.h"
#  include "driverlib/timer.h"
#  define PROGMEM
#  define memcpy_P                      memcpy
#  define APP_SYSTICKS_PER_SEC          32

#elif defined(ARM_STM32F10X)

#  include "stm32f10x_gpio.h"
#  include "stm32f10x_rcc.h"
#  include "stm32f10x_tim.h"
#  include "misc.h"
#  define PROGMEM
#  define memcpy_P                      memcpy

#elif defined(SDCC_STM8)

#  include "stm8s.h"
#  define PROGMEM
#  define memcpy_P                      memcpy
#  define __attribute__(x)
#  define uint_fast8_t                  uint8_t
#  define uint_fast16_t                 uint16_t

#elif defined(TEENSY_ARM_CORTEX_M4)
#  define PROGMEM
#  define memcpy_P                      memcpy

#elif defined(__xtensa__)
#  define PROGMEM
#  define memcpy_P                      memcpy

#elif defined(__MBED__)
#  define PROGMEM
#  define memcpy_P                      memcpy

#else
#  if ! defined(PROGMEM)
#    define PROGMEM
#  endif
#  if ! defined(memcpy_P)
#    define memcpy_P                      memcpy
#  endif

#endif

#if defined(PIC_CCS) || defined(PIC_C18)
typedef unsigned char                   uint8_t;
typedef unsigned short                  uint16_t;
typedef unsigned char                   uint_fast8_t;
typedef unsigned short                  uint_fast16_t;
#endif

#if defined (PIC_C18)                                                               // PIC C18 or XC8 compiler
#  include <p18cxxx.h>                                                              // main PIC18 h file
#ifndef __XC8
#  include <timers.h>                                                               // timer lib
#  include <pwm.h>                                                                  // pwm lib
#endif
#  define IRSND_PIC_CCP1                1                                           // PIC C18 RC2 = PWM1 module
#  define IRSND_PIC_CCP2                2                                           // PIC C18 RC1 = PWM2 module
#endif

#ifndef TRUE
#  define TRUE                          1
#  define FALSE                         0
#endif

#if defined(PIC_XC32)                                                               // XC32 or ChipKit compiler
#  include <xc.h>
#  include <stdint.h>
#endif

#if defined(PIC_C18)
#  define IRMP_PACKED_STRUCT
#else
#  ifndef IRMP_PACKED_STRUCT
#    define IRMP_PACKED_STRUCT          __attribute__ ((__packed__))
#  endif
#endif

typedef struct IRMP_PACKED_STRUCT IRMP_DATA
{
    uint8_t                             protocol;                                   // protocol, e.g. NEC_PROTOCOL
    uint16_t                            address;                                    // address
    uint16_t                            command;                                    // command
    uint8_t                             flags;                                      // flags, e.g. repetition
} IRMP_DATA;

#endif // _IRMPSYSTEM_H_
