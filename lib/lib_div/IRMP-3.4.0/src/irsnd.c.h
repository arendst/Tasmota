/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * @file irsnd.c
 *
 * Copyright (c) 2010-2020 Frank Meyer - frank(at)fli4l.de
 *
 * Supported AVR mikrocontrollers:
 *
 * ATtiny87,  ATtiny167
 * ATtiny45,  ATtiny85
 * ATtiny44   ATtiny84
 * ATtiny2313 ATtiny4313
 * ATmega8,   ATmega16,  ATmega32
 * ATmega162
 * ATmega164, ATmega324, ATmega644,  ATmega644P, ATmega1284, ATmega1284P
 * ATmega88,  ATmega88P, ATmega168,  ATmega168P, ATmega328P
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#include "irsnd.h"

#ifndef F_CPU
#  error F_CPU unkown
#endif

#define IRMP_NEC_REPETITION_PROTOCOL                0xFF            // pseudo protocol: NEC repetition frame

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  ATtiny pin definition of OC0A / OC0B
 *  ATmega pin definition of OC2 / OC2A / OC2B / OC0 / OC0A / OC0B
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)        // ATtiny44/84 uses OC0A = PB2 or OC0B = PA7
#  if IRSND_OCx == IRSND_OC0A                                       // OC0A
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        2
#  elif IRSND_OCx == IRSND_OC0B                                     // OC0B
#    define IRSND_PORT_LETTER                       A
#    define IRSND_BIT_NUMBER                        7
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC0A or IRSND_OC0B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)      // ATtiny45/85 uses OC0A = PB0 or OC0B = PB1
#  if IRSND_OCx == IRSND_OC0A                                       // OC0A
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        0
#  elif IRSND_OCx == IRSND_OC0B                                     // OC0B
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        1
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC0A or IRSND_OC0B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATtiny2313__) || defined (__AVR_ATtiny4313__)  // ATtiny2313/4313 uses OC0A = PB2 or OC0B = PD5
#  if IRSND_OCx == IRSND_OC0A                                       // OC0A
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        2
#  elif IRSND_OCx == IRSND_OC0B                                     // OC0B
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        5
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC0A or IRSND_OC0B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATtiny87__) || defined (__AVR_ATtiny167__)     // ATtiny87/167 uses OC0A = PA2
#  if IRSND_OCx == IRSND_OC0A                                       // OC0A
#    define IRSND_PORT_LETTER                       A
#    define IRSND_BIT_NUMBER                        2
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC0A in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATmega8__)                                     // ATmega8 uses only OC2 = PB3
#  if IRSND_OCx == IRSND_OC2                                        // OC2
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        3
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC2 in irsndconfig.h
#  endif // IRSND_OCx
#elif defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__)      // ATmega16|32 uses OC0 = PB3 or OC2 = PD7
#  if IRSND_OCx == IRSND_OC2                                        // OC2
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        7
#  elif IRSND_OCx == IRSND_OC0                                      // OC0
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        3
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC2 or IRSND_OC0 in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATmega162__)                                   // ATmega162 uses OC2 = PB1 or OC0 = PB0
#  if IRSND_OCx == IRSND_OC2                                        // OC2
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        1
#  elif IRSND_OCx == IRSND_OC0                                      // OC0
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        0
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC2 or IRSND_OC0 in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATmega164__)   \
   || defined (__AVR_ATmega324__)   \
   || defined (__AVR_ATmega644__)   \
   || defined (__AVR_ATmega644P__)  \
   || defined (__AVR_ATmega1284__)  \
   || defined (__AVR_ATmega1284P__)                                 // ATmega164|324|644|644P|1284 uses OC2A = PD7 or OC2B = PD6 or OC0A = PB3 or OC0B = PB4
#  if IRSND_OCx == IRSND_OC2A                                       // OC2A
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        7
#  elif IRSND_OCx == IRSND_OC2B                                     // OC2B
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        6
#  elif IRSND_OCx == IRSND_OC0A                                     // OC0A
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        3
#  elif IRSND_OCx == IRSND_OC0B                                     // OC0B
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        4
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC2A, IRSND_OC2B, IRSND_OC0A, or IRSND_OC0B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATmega48__)    \
   || defined (__AVR_ATmega88__)    \
   || defined (__AVR_ATmega88P__)   \
   || defined (__AVR_ATmega168__)   \
   || defined (__AVR_ATmega168P__)  \
   || defined (__AVR_ATmega328P__)                                  // ATmega48|88|168|168|328 uses OC2A = PB3 or OC2B = PD3 or OC0A = PD6 or OC0B = PD5
#  if IRSND_OCx == IRSND_OC2A                                       // OC2A
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        3
#  elif IRSND_OCx == IRSND_OC2B                                     // OC2B
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        3
#  elif IRSND_OCx == IRSND_OC0A                                     // OC0A
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        6
#  elif IRSND_OCx == IRSND_OC0B                                     // OC0B
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        5
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_OC2A, IRSND_OC2B, IRSND_OC0A, or IRSND_OC0B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (__AVR_ATmega8515__)                                  // ATmega8515 uses OC0 = PB0 or OC1A = PD5 or OC1B = PE2
#  if IRSND_OCx == IRSND_OC0
#    define IRSND_PORT_LETTER                       B
#    define IRSND_BIT_NUMBER                        0
#  elif IRSND_OCx == IRSND_OC1A
#    define IRSND_PORT_LETTER                       D
#    define IRSND_BIT_NUMBER                        5
#  elif IRSND_OCx == IRSND_OC1B
#    define IRSND_PORT_LETTER                       E
#    define IRSND_BIT_NUMBER                        2
#  endif // IRSND_OCx

#elif defined (__AVR_XMEGA__)                                       // ATxmega
#  if IRSND_OCx == IRSND_XMEGA_OC0A
#    define IRSND_BIT_NUMBER                        0
#  elif IRSND_OCx == IRSND_XMEGA_OC0B
#    define IRSND_BIT_NUMBER                        1
#  elif IRSND_OCx == IRSND_XMEGA_OC0C
#    define IRSND_BIT_NUMBER                        2
#  elif IRSND_OCx == IRSND_XMEGA_OC0D
#    define IRSND_BIT_NUMBER                        3
#  elif IRSND_OCx == IRSND_XMEGA_OC1A
#    define IRSND_BIT_NUMBER                        4
#  elif IRSND_OCx == IRSND_XMEGA_OC1B
#    define IRSND_BIT_NUMBER                        5
#  else
#    error Wrong value for IRSND_OCx, choose IRSND_XMEGA_OC0A, IRSND_XMEGA_OC0B, IRSND_XMEGA_OC0C, IRSND_XMEGA_OC0D, IRSND_XMEGA_OC1A, or IRSND_XMEGA_OC1B in irsndconfig.h
#  endif // IRSND_OCx

#elif defined (PIC_C18)                                                 // Microchip C18 compiler
    //Nothing here to do here -> See irsndconfig.h
#elif defined (ARM_STM32)                                               // STM32
    //Nothing here to do here -> See irsndconfig.h
#elif defined (ARM_STM32_HAL)                                           // STM32 with Hal Library
    //Nothing here to do here -> See irsndconfig.h
#elif defined (__xtensa__)                                              // ESP8266
    //Nothing here to do here -> See irsndconfig.h

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Macro digitalPinHasPWM bothers PIC_C18 compiler, but why?
 *
 * #elif defined (TEENSY_ARM_CORTEX_M4)                                // Teensy3
 * #  if !digitalPinHasPWM(IRSND_PIN)
 * #    error need pin with PWM output.
 * #  endif
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

 #elif defined(ARDUINO)
// specified here to avoid else case

#else
#  if !defined (unix) && !defined (WIN32)
#    error mikrocontroller not defined, please fill in definitions here.
#  endif // unix, WIN32
#endif // __AVR...

#if defined(__AVR_XMEGA__)
#  define _CONCAT(a,b)                              a##b
#  define CONCAT(a,b)                               _CONCAT(a,b)
#  define IRSND_PORT                                IRSND_PORT_PRE.OUT
#  define IRSND_DDR                                 IRSND_PORT_PRE.DIR
#  define IRSND_PIN                                 IRSND_PORT_PRE.IN
#  define IRSND_BIT                                 IRSND_BIT_NUMBER
#elif defined(ATMEL_AVR)
#  define _CONCAT(a,b)                              a##b
#  define CONCAT(a,b)                               _CONCAT(a,b)
#  define IRSND_PORT                                CONCAT(PORT, IRSND_PORT_LETTER)
#  define IRSND_DDR                                 CONCAT(DDR, IRSND_PORT_LETTER)
#  define IRSND_BIT                                 IRSND_BIT_NUMBER
#endif

#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
    typedef uint16_t    IRSND_PAUSE_LEN;
#else
    typedef uint8_t     IRSND_PAUSE_LEN;
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  IR timings
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#define IRSND_SIRCS_START_BIT_PULSE_LEN             (uint8_t)(F_INTERRUPTS * SIRCS_START_BIT_PULSE_TIME + 0.5)
#define IRSND_SIRCS_START_BIT_PAUSE_LEN             (uint8_t)(F_INTERRUPTS * SIRCS_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_SIRCS_1_PULSE_LEN                     (uint8_t)(F_INTERRUPTS * SIRCS_1_PULSE_TIME + 0.5)
#define IRSND_SIRCS_0_PULSE_LEN                     (uint8_t)(F_INTERRUPTS * SIRCS_0_PULSE_TIME + 0.5)
#define IRSND_SIRCS_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * SIRCS_PAUSE_TIME + 0.5)
#define IRSND_SIRCS_AUTO_REPETITION_PAUSE_LEN       (uint16_t)(F_INTERRUPTS * SIRCS_AUTO_REPETITION_PAUSE_TIME + 0.5)               // use uint16_t!
#define IRSND_SIRCS_FRAME_REPEAT_PAUSE_LEN          (uint16_t)(F_INTERRUPTS * SIRCS_FRAME_REPEAT_PAUSE_TIME + 0.5)                  // use uint16_t!

#define IRSND_NEC_START_BIT_PULSE_LEN               (uint8_t)(F_INTERRUPTS * NEC_START_BIT_PULSE_TIME + 0.5)
#define IRSND_NEC_START_BIT_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * NEC_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_NEC_REPEAT_START_BIT_PAUSE_LEN        (uint8_t)(F_INTERRUPTS * NEC_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_NEC_PULSE_LEN                         (uint8_t)(F_INTERRUPTS * NEC_PULSE_TIME + 0.5)
#define IRSND_NEC_1_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * NEC_1_PAUSE_TIME + 0.5)
#define IRSND_NEC_0_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * NEC_0_PAUSE_TIME + 0.5)
#define IRSND_NEC_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * NEC_FRAME_REPEAT_PAUSE_TIME + 0.5)                    // use uint16_t!

#define IRSND_MELINERA_START_BIT_PULSE_LEN          (uint8_t)(F_INTERRUPTS * MELINERA_START_BIT_PULSE_TIME + 0.5)
#define IRSND_MELINERA_START_BIT_PAUSE_LEN          (uint8_t)(F_INTERRUPTS * MELINERA_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_MELINERA_REPEAT_START_BIT_PAUSE_LEN   (uint8_t)(F_INTERRUPTS * MELINERA_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_MELINERA_1_PULSE_LEN                  (uint8_t)(F_INTERRUPTS * MELINERA_1_PULSE_TIME + 0.5)
#define IRSND_MELINERA_0_PULSE_LEN                  (uint8_t)(F_INTERRUPTS * MELINERA_0_PULSE_TIME + 0.5)
#define IRSND_MELINERA_1_PAUSE_LEN                  (uint8_t)(F_INTERRUPTS * MELINERA_1_PAUSE_TIME + 0.5)
#define IRSND_MELINERA_0_PAUSE_LEN                  (uint8_t)(F_INTERRUPTS * MELINERA_0_PAUSE_TIME + 0.5)
#define IRSND_MELINERA_FRAME_REPEAT_PAUSE_LEN       (uint16_t)(F_INTERRUPTS * MELINERA_FRAME_REPEAT_PAUSE_TIME + 0.5)               // use uint16_t!

#define IRSND_SAMSUNG_START_BIT_PULSE_LEN           (uint8_t)(F_INTERRUPTS * SAMSUNG_START_BIT_PULSE_TIME + 0.5)
#define IRSND_SAMSUNG_START_BIT_PAUSE_LEN           (uint8_t)(F_INTERRUPTS * SAMSUNG_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_SAMSUNG_PULSE_LEN                     (uint8_t)(F_INTERRUPTS * SAMSUNG_PULSE_TIME + 0.5)
#define IRSND_SAMSUNG_1_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * SAMSUNG_1_PAUSE_TIME + 0.5)
#define IRSND_SAMSUNG_0_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * SAMSUNG_0_PAUSE_TIME + 0.5)
#define IRSND_SAMSUNG_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * SAMSUNG_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_SAMSUNG32_AUTO_REPETITION_PAUSE_LEN   (uint16_t)(F_INTERRUPTS * SAMSUNG32_AUTO_REPETITION_PAUSE_TIME + 0.5)           // use uint16_t!
#define IRSND_SAMSUNG32_FRAME_REPEAT_PAUSE_LEN      (uint16_t)(F_INTERRUPTS * SAMSUNG32_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_SAMSUNG48_AUTO_REPETITION_PAUSE_LEN   (uint16_t)(F_INTERRUPTS * SAMSUNG48_AUTO_REPETITION_PAUSE_TIME + 0.5)           // use uint16_t!
#define IRSND_SAMSUNG48_FRAME_REPEAT_PAUSE_LEN      (uint16_t)(F_INTERRUPTS * SAMSUNG48_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_MATSUSHITA_START_BIT_PULSE_LEN        (uint8_t)(F_INTERRUPTS * MATSUSHITA_START_BIT_PULSE_TIME + 0.5)
#define IRSND_MATSUSHITA_START_BIT_PAUSE_LEN        (uint8_t)(F_INTERRUPTS * MATSUSHITA_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_MATSUSHITA_PULSE_LEN                  (uint8_t)(F_INTERRUPTS * MATSUSHITA_PULSE_TIME + 0.5)
#define IRSND_MATSUSHITA_1_PAUSE_LEN                (uint8_t)(F_INTERRUPTS * MATSUSHITA_1_PAUSE_TIME + 0.5)
#define IRSND_MATSUSHITA_0_PAUSE_LEN                (uint8_t)(F_INTERRUPTS * MATSUSHITA_0_PAUSE_TIME + 0.5)
#define IRSND_MATSUSHITA_FRAME_REPEAT_PAUSE_LEN     (uint16_t)(F_INTERRUPTS * MATSUSHITA_FRAME_REPEAT_PAUSE_TIME + 0.5)             // use uint16_t!

#define IRSND_KASEIKYO_START_BIT_PULSE_LEN          (uint8_t)(F_INTERRUPTS * KASEIKYO_START_BIT_PULSE_TIME + 0.5)
#define IRSND_KASEIKYO_START_BIT_PAUSE_LEN          (uint8_t)(F_INTERRUPTS * KASEIKYO_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_KASEIKYO_PULSE_LEN                    (uint8_t)(F_INTERRUPTS * KASEIKYO_PULSE_TIME + 0.5)
#define IRSND_KASEIKYO_1_PAUSE_LEN                  (uint8_t)(F_INTERRUPTS * KASEIKYO_1_PAUSE_TIME + 0.5)
#define IRSND_KASEIKYO_0_PAUSE_LEN                  (uint8_t)(F_INTERRUPTS * KASEIKYO_0_PAUSE_TIME + 0.5)
#define IRSND_KASEIKYO_AUTO_REPETITION_PAUSE_LEN    (uint16_t)(F_INTERRUPTS * KASEIKYO_AUTO_REPETITION_PAUSE_TIME + 0.5)            // use uint16_t!
#define IRSND_KASEIKYO_FRAME_REPEAT_PAUSE_LEN       (uint16_t)(F_INTERRUPTS * KASEIKYO_FRAME_REPEAT_PAUSE_TIME + 0.5)               // use uint16_t!

#define IRSND_PANASONIC_START_BIT_PULSE_LEN         (uint8_t)(F_INTERRUPTS * PANASONIC_START_BIT_PULSE_TIME + 0.5)
#define IRSND_PANASONIC_START_BIT_PAUSE_LEN         (uint8_t)(F_INTERRUPTS * PANASONIC_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_PANASONIC_PULSE_LEN                   (uint8_t)(F_INTERRUPTS * PANASONIC_PULSE_TIME + 0.5)
#define IRSND_PANASONIC_1_PAUSE_LEN                 (uint8_t)(F_INTERRUPTS * PANASONIC_1_PAUSE_TIME + 0.5)
#define IRSND_PANASONIC_0_PAUSE_LEN                 (uint8_t)(F_INTERRUPTS * PANASONIC_0_PAUSE_TIME + 0.5)
#define IRSND_PANASONIC_AUTO_REPETITION_PAUSE_LEN   (uint16_t)(F_INTERRUPTS * PANASONIC_AUTO_REPETITION_PAUSE_TIME + 0.5)           // use uint16_t!
#define IRSND_PANASONIC_FRAME_REPEAT_PAUSE_LEN      (uint16_t)(F_INTERRUPTS * PANASONIC_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_MITSU_HEAVY_START_BIT_PULSE_LEN       (uint8_t)(F_INTERRUPTS * MITSU_HEAVY_START_BIT_PULSE_TIME + 0.5)
#define IRSND_MITSU_HEAVY_START_BIT_PAUSE_LEN       (uint8_t)(F_INTERRUPTS * MITSU_HEAVY_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_MITSU_HEAVY_PULSE_LEN                 (uint8_t)(F_INTERRUPTS * MITSU_HEAVY_PULSE_TIME + 0.5)
#define IRSND_MITSU_HEAVY_1_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * MITSU_HEAVY_1_PAUSE_TIME + 0.5)
#define IRSND_MITSU_HEAVY_0_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * MITSU_HEAVY_0_PAUSE_TIME + 0.5)
#define IRSND_MITSU_HEAVY_FRAME_REPEAT_PAUSE_LEN    (uint16_t)(F_INTERRUPTS * MITSU_HEAVY_FRAME_REPEAT_PAUSE_TIME + 0.5)             // use uint16_t!

#define IRSND_RECS80_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * RECS80_START_BIT_PULSE_TIME + 0.5)
#define IRSND_RECS80_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * RECS80_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_RECS80_PULSE_LEN                      (uint8_t)(F_INTERRUPTS * RECS80_PULSE_TIME + 0.5)
#define IRSND_RECS80_1_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * RECS80_1_PAUSE_TIME + 0.5)
#define IRSND_RECS80_0_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * RECS80_0_PAUSE_TIME + 0.5)
#define IRSND_RECS80_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * RECS80_FRAME_REPEAT_PAUSE_TIME + 0.5)                 // use uint16_t!

#define IRSND_RC5_START_BIT_LEN                     (uint8_t)(F_INTERRUPTS * RC5_BIT_TIME + 0.5)
#define IRSND_RC5_BIT_LEN                           (uint8_t)(F_INTERRUPTS * RC5_BIT_TIME + 0.5)
#define IRSND_RC5_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * RC5_FRAME_REPEAT_PAUSE_TIME + 0.5)                    // use uint16_t!

#define IRSND_RC6_START_BIT_PULSE_LEN               (uint8_t)(F_INTERRUPTS * RC6_START_BIT_PULSE_TIME + 0.5)
#define IRSND_RC6_START_BIT_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * RC6_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_RC6_BIT_LEN                           (uint8_t)(F_INTERRUPTS * RC6_BIT_TIME + 0.5)
#define IRSND_RC6_BIT_2_LEN                         (uint8_t)(F_INTERRUPTS * RC6_BIT_2_TIME + 0.5)
#define IRSND_RC6_BIT_3_LEN                         (uint8_t)(F_INTERRUPTS * RC6_BIT_3_TIME + 0.5)
#define IRSND_RC6_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * RC6_FRAME_REPEAT_PAUSE_TIME + 0.5)                    // use uint16_t!

#define IRSND_DENON_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * DENON_PULSE_TIME + 0.5)
#define IRSND_DENON_1_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * DENON_1_PAUSE_TIME + 0.5)
#define IRSND_DENON_0_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * DENON_0_PAUSE_TIME + 0.5)
#define IRSND_DENON_AUTO_REPETITION_PAUSE_LEN       (uint16_t)(F_INTERRUPTS * DENON_AUTO_REPETITION_PAUSE_TIME + 0.5)               // use uint16_t!
#define IRSND_DENON_FRAME_REPEAT_PAUSE_LEN          (uint16_t)(F_INTERRUPTS * DENON_FRAME_REPEAT_PAUSE_TIME + 0.5)                  // use uint16_t!

#define IRSND_THOMSON_PULSE_LEN                     (uint8_t)(F_INTERRUPTS * THOMSON_PULSE_TIME + 0.5)
#define IRSND_THOMSON_1_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * THOMSON_1_PAUSE_TIME + 0.5)
#define IRSND_THOMSON_0_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * THOMSON_0_PAUSE_TIME + 0.5)
#define IRSND_THOMSON_AUTO_REPETITION_PAUSE_LEN     (uint16_t)(F_INTERRUPTS * THOMSON_AUTO_REPETITION_PAUSE_TIME + 0.5)             // use uint16_t!
#define IRSND_THOMSON_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * THOMSON_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_RECS80EXT_START_BIT_PULSE_LEN         (uint8_t)(F_INTERRUPTS * RECS80EXT_START_BIT_PULSE_TIME + 0.5)
#define IRSND_RECS80EXT_START_BIT_PAUSE_LEN         (uint8_t)(F_INTERRUPTS * RECS80EXT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_RECS80EXT_PULSE_LEN                   (uint8_t)(F_INTERRUPTS * RECS80EXT_PULSE_TIME + 0.5)
#define IRSND_RECS80EXT_1_PAUSE_LEN                 (uint8_t)(F_INTERRUPTS * RECS80EXT_1_PAUSE_TIME + 0.5)
#define IRSND_RECS80EXT_0_PAUSE_LEN                 (uint8_t)(F_INTERRUPTS * RECS80EXT_0_PAUSE_TIME + 0.5)
#define IRSND_RECS80EXT_FRAME_REPEAT_PAUSE_LEN      (uint16_t)(F_INTERRUPTS * RECS80EXT_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_TELEFUNKEN_START_BIT_PULSE_LEN        (uint8_t)(F_INTERRUPTS * TELEFUNKEN_START_BIT_PULSE_TIME + 0.5)
#define IRSND_TELEFUNKEN_START_BIT_PAUSE_LEN        (uint8_t)(F_INTERRUPTS * TELEFUNKEN_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_TELEFUNKEN_PULSE_LEN                  (uint8_t)(F_INTERRUPTS * TELEFUNKEN_PULSE_TIME + 0.5)
#define IRSND_TELEFUNKEN_1_PAUSE_LEN                (uint8_t)(F_INTERRUPTS * TELEFUNKEN_1_PAUSE_TIME + 0.5)
#define IRSND_TELEFUNKEN_0_PAUSE_LEN                (uint8_t)(F_INTERRUPTS * TELEFUNKEN_0_PAUSE_TIME + 0.5)
#define IRSND_TELEFUNKEN_AUTO_REPETITION_PAUSE_LEN  (uint16_t)(F_INTERRUPTS * TELEFUNKEN_AUTO_REPETITION_PAUSE_TIME + 0.5)          // use uint16_t!
#define IRSND_TELEFUNKEN_FRAME_REPEAT_PAUSE_LEN     (uint16_t)(F_INTERRUPTS * TELEFUNKEN_FRAME_REPEAT_PAUSE_TIME + 0.5)             // use uint16_t!

#define IRSND_BOSE_START_BIT_PULSE_LEN              (uint8_t)(F_INTERRUPTS * BOSE_START_BIT_PULSE_TIME + 0.5)
#define IRSND_BOSE_START_BIT_PAUSE_LEN              (uint8_t)(F_INTERRUPTS * BOSE_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_BOSE_PULSE_LEN                        (uint8_t)(F_INTERRUPTS * BOSE_PULSE_TIME + 0.5)
#define IRSND_BOSE_1_PAUSE_LEN                      (uint8_t)(F_INTERRUPTS * BOSE_1_PAUSE_TIME + 0.5)
#define IRSND_BOSE_0_PAUSE_LEN                      (uint8_t)(F_INTERRUPTS * BOSE_0_PAUSE_TIME + 0.5)
#define IRSND_BOSE_AUTO_REPETITION_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * BOSE_AUTO_REPETITION_PAUSE_TIME + 0.5)              // use uint16_t!
#define IRSND_BOSE_FRAME_REPEAT_PAUSE_LEN           (uint16_t)(F_INTERRUPTS * BOSE_FRAME_REPEAT_PAUSE_TIME + 0.5)                 // use uint16_t!

#define IRSND_NUBERT_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * NUBERT_START_BIT_PULSE_TIME + 0.5)
#define IRSND_NUBERT_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * NUBERT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_NUBERT_1_PULSE_LEN                    (uint8_t)(F_INTERRUPTS * NUBERT_1_PULSE_TIME + 0.5)
#define IRSND_NUBERT_1_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * NUBERT_1_PAUSE_TIME + 0.5)
#define IRSND_NUBERT_0_PULSE_LEN                    (uint8_t)(F_INTERRUPTS * NUBERT_0_PULSE_TIME + 0.5)
#define IRSND_NUBERT_0_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * NUBERT_0_PAUSE_TIME + 0.5)
#define IRSND_NUBERT_AUTO_REPETITION_PAUSE_LEN      (uint16_t)(F_INTERRUPTS * NUBERT_AUTO_REPETITION_PAUSE_TIME + 0.5)              // use uint16_t!
#define IRSND_NUBERT_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * NUBERT_FRAME_REPEAT_PAUSE_TIME + 0.5)                 // use uint16_t!

#define IRSND_FAN_START_BIT_PULSE_LEN               (uint8_t)(F_INTERRUPTS * FAN_START_BIT_PULSE_TIME + 0.5)
#define IRSND_FAN_START_BIT_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * FAN_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_FAN_1_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * FAN_1_PULSE_TIME + 0.5)
#define IRSND_FAN_1_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * FAN_1_PAUSE_TIME + 0.5)
#define IRSND_FAN_0_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * FAN_0_PULSE_TIME + 0.5)
#define IRSND_FAN_0_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * FAN_0_PAUSE_TIME + 0.5)
#define IRSND_FAN_AUTO_REPETITION_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * FAN_AUTO_REPETITION_PAUSE_TIME + 0.5)              // use uint16_t!
#define IRSND_FAN_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * FAN_FRAME_REPEAT_PAUSE_TIME + 0.5)                 // use uint16_t!

#define IRSND_SPEAKER_START_BIT_PULSE_LEN           (uint8_t)(F_INTERRUPTS * SPEAKER_START_BIT_PULSE_TIME + 0.5)
#define IRSND_SPEAKER_START_BIT_PAUSE_LEN           (uint8_t)(F_INTERRUPTS * SPEAKER_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_SPEAKER_1_PULSE_LEN                   (uint8_t)(F_INTERRUPTS * SPEAKER_1_PULSE_TIME + 0.5)
#define IRSND_SPEAKER_1_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * SPEAKER_1_PAUSE_TIME + 0.5)
#define IRSND_SPEAKER_0_PULSE_LEN                   (uint8_t)(F_INTERRUPTS * SPEAKER_0_PULSE_TIME + 0.5)
#define IRSND_SPEAKER_0_PAUSE_LEN                   (uint8_t)(F_INTERRUPTS * SPEAKER_0_PAUSE_TIME + 0.5)
#define IRSND_SPEAKER_AUTO_REPETITION_PAUSE_LEN     (uint16_t)(F_INTERRUPTS * SPEAKER_AUTO_REPETITION_PAUSE_TIME + 0.5)             // use uint16_t!
#define IRSND_SPEAKER_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * SPEAKER_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_BANG_OLUFSEN_START_BIT1_PULSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT1_PULSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_START_BIT1_PAUSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT1_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_START_BIT2_PULSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT2_PULSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_START_BIT2_PAUSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT2_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_START_BIT3_PULSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT3_PULSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_START_BIT3_PAUSE_LEN     (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_START_BIT3_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_PULSE_LEN                (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_PULSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_1_PAUSE_LEN              (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_1_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_0_PAUSE_LEN              (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_0_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_R_PAUSE_LEN              (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_R_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_TRAILER_BIT_PAUSE_LEN    (uint8_t)(F_INTERRUPTS * BANG_OLUFSEN_TRAILER_BIT_PAUSE_TIME + 0.5)
#define IRSND_BANG_OLUFSEN_FRAME_REPEAT_PAUSE_LEN   (uint16_t)(F_INTERRUPTS * BANG_OLUFSEN_FRAME_REPEAT_PAUSE_TIME + 0.5)           // use uint16_t!

#define IRSND_GRUNDIG_NOKIA_IR60_PRE_PAUSE_LEN      (uint8_t)(F_INTERRUPTS * GRUNDIG_NOKIA_IR60_PRE_PAUSE_TIME + 0.5)
#define IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN            (uint8_t)(F_INTERRUPTS * GRUNDIG_NOKIA_IR60_BIT_TIME + 0.5)
#define IRSND_GRUNDIG_AUTO_REPETITION_PAUSE_LEN     (uint16_t)(F_INTERRUPTS * GRUNDIG_AUTO_REPETITION_PAUSE_TIME + 0.5)             // use uint16_t!
#define IRSND_NOKIA_AUTO_REPETITION_PAUSE_LEN       (uint16_t)(F_INTERRUPTS * NOKIA_AUTO_REPETITION_PAUSE_TIME + 0.5)               // use uint16_t!
#define IRSND_GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_LEN (uint16_t)(F_INTERRUPTS * GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_TIME + 0.5)   // use uint16_t!

#define IRSND_IR60_AUTO_REPETITION_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * IR60_AUTO_REPETITION_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_SIEMENS_START_BIT_LEN                 (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_START_BIT_PULSE_TIME + 0.5)
#define IRSND_SIEMENS_BIT_LEN                       (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_BIT_PULSE_TIME + 0.5)
#define IRSND_SIEMENS_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_FRAME_REPEAT_PAUSE_TIME + 0.5)      // use uint16_t!

#define IRSND_RUWIDO_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_START_BIT_PULSE_TIME + 0.5)
#define IRSND_RUWIDO_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_RUWIDO_BIT_PULSE_LEN                  (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_BIT_PULSE_TIME + 0.5)
#define IRSND_RUWIDO_BIT_PAUSE_LEN                  (uint8_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_BIT_PAUSE_TIME + 0.5)
#define IRSND_RUWIDO_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * SIEMENS_OR_RUWIDO_FRAME_REPEAT_PAUSE_TIME + 0.5)      // use uint16_t!

#define IRSND_FDC_START_BIT_PULSE_LEN               (uint8_t)(F_INTERRUPTS * FDC_START_BIT_PULSE_TIME + 0.5)
#define IRSND_FDC_START_BIT_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * FDC_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_FDC_PULSE_LEN                         (uint8_t)(F_INTERRUPTS * FDC_PULSE_TIME + 0.5)
#define IRSND_FDC_1_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * FDC_1_PAUSE_TIME + 0.5)
#define IRSND_FDC_0_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * FDC_0_PAUSE_TIME + 0.5)
#define IRSND_FDC_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * FDC_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_RCCAR_START_BIT_PULSE_LEN             (uint8_t)(F_INTERRUPTS * RCCAR_START_BIT_PULSE_TIME + 0.5)
#define IRSND_RCCAR_START_BIT_PAUSE_LEN             (uint8_t)(F_INTERRUPTS * RCCAR_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_RCCAR_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * RCCAR_PULSE_TIME + 0.5)
#define IRSND_RCCAR_1_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * RCCAR_1_PAUSE_TIME + 0.5)
#define IRSND_RCCAR_0_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * RCCAR_0_PAUSE_TIME + 0.5)
#define IRSND_RCCAR_FRAME_REPEAT_PAUSE_LEN          (uint16_t)(F_INTERRUPTS * RCCAR_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_JVC_START_BIT_PULSE_LEN               (uint8_t)(F_INTERRUPTS * JVC_START_BIT_PULSE_TIME + 0.5)
#define IRSND_JVC_START_BIT_PAUSE_LEN               (uint8_t)(F_INTERRUPTS * JVC_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_JVC_REPEAT_START_BIT_PAUSE_LEN        (uint8_t)(F_INTERRUPTS * JVC_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_JVC_PULSE_LEN                         (uint8_t)(F_INTERRUPTS * JVC_PULSE_TIME + 0.5)
#define IRSND_JVC_1_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * JVC_1_PAUSE_TIME + 0.5)
#define IRSND_JVC_0_PAUSE_LEN                       (uint8_t)(F_INTERRUPTS * JVC_0_PAUSE_TIME + 0.5)
#define IRSND_JVC_FRAME_REPEAT_PAUSE_LEN            (uint16_t)(F_INTERRUPTS * JVC_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#define IRSND_NIKON_START_BIT_PULSE_LEN             (uint8_t)(F_INTERRUPTS * NIKON_START_BIT_PULSE_TIME + 0.5)
#define IRSND_NIKON_START_BIT_PAUSE_LEN             (uint16_t)(F_INTERRUPTS * NIKON_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_NIKON_REPEAT_START_BIT_PAUSE_LEN      (uint8_t)(F_INTERRUPTS * NIKON_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_NIKON_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * NIKON_PULSE_TIME + 0.5)
#define IRSND_NIKON_1_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * NIKON_1_PAUSE_TIME + 0.5)
#define IRSND_NIKON_0_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * NIKON_0_PAUSE_TIME + 0.5)
#define IRSND_NIKON_FRAME_REPEAT_PAUSE_LEN          (uint16_t)(F_INTERRUPTS * NIKON_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_LEGO_START_BIT_PULSE_LEN              (uint8_t)(F_INTERRUPTS * LEGO_START_BIT_PULSE_TIME + 0.5)
#define IRSND_LEGO_START_BIT_PAUSE_LEN              (uint8_t)(F_INTERRUPTS * LEGO_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_LEGO_REPEAT_START_BIT_PAUSE_LEN       (uint8_t)(F_INTERRUPTS * LEGO_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_LEGO_PULSE_LEN                        (uint8_t)(F_INTERRUPTS * LEGO_PULSE_TIME + 0.5)
#define IRSND_LEGO_1_PAUSE_LEN                      (uint8_t)(F_INTERRUPTS * LEGO_1_PAUSE_TIME + 0.5)
#define IRSND_LEGO_0_PAUSE_LEN                      (uint8_t)(F_INTERRUPTS * LEGO_0_PAUSE_TIME + 0.5)
#define IRSND_LEGO_FRAME_REPEAT_PAUSE_LEN           (uint16_t)(F_INTERRUPTS * LEGO_FRAME_REPEAT_PAUSE_TIME + 0.5)               // use uint16_t!

#define IRSND_IRMP16_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * IRMP16_START_BIT_PULSE_TIME + 0.5)
#define IRSND_IRMP16_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * IRMP16_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_IRMP16_REPEAT_START_BIT_PAUSE_LEN     (uint8_t)(F_INTERRUPTS * IRMP16_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_IRMP16_PULSE_LEN                      (uint8_t)(F_INTERRUPTS * IRMP16_PULSE_TIME + 0.5)
#define IRSND_IRMP16_1_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * IRMP16_1_PAUSE_TIME + 0.5)
#define IRSND_IRMP16_0_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * IRMP16_0_PAUSE_TIME + 0.5)
#define IRSND_IRMP16_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * IRMP16_FRAME_REPEAT_PAUSE_TIME + 0.5)               // use uint16_t!

#define IRSND_A1TVBOX_START_BIT_PULSE_LEN           (uint8_t)(F_INTERRUPTS * A1TVBOX_START_BIT_PULSE_TIME + 0.5)
#define IRSND_A1TVBOX_START_BIT_PAUSE_LEN           (uint8_t)(F_INTERRUPTS * A1TVBOX_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_A1TVBOX_BIT_PULSE_LEN                 (uint8_t)(F_INTERRUPTS * A1TVBOX_BIT_PULSE_TIME + 0.5)
#define IRSND_A1TVBOX_BIT_PAUSE_LEN                 (uint8_t)(F_INTERRUPTS * A1TVBOX_BIT_PAUSE_TIME + 0.5)
#define IRSND_A1TVBOX_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * A1TVBOX_FRAME_REPEAT_PAUSE_TIME + 0.5)            // use uint16_t!
#define IRSND_A1TVBOX_FRAME_REPEAT_PAUSE_LEN        (uint16_t)(F_INTERRUPTS * A1TVBOX_FRAME_REPEAT_PAUSE_TIME + 0.5)            // use uint16_t!

#define IRSND_ROOMBA_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * ROOMBA_START_BIT_PULSE_TIME + 0.5)
#define IRSND_ROOMBA_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * ROOMBA_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_ROOMBA_1_PULSE_LEN                    (uint8_t)(F_INTERRUPTS * ROOMBA_1_PULSE_TIME + 0.5)
#define IRSND_ROOMBA_0_PULSE_LEN                    (uint8_t)(F_INTERRUPTS * ROOMBA_0_PULSE_TIME + 0.5)
#define IRSND_ROOMBA_1_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * ROOMBA_1_PAUSE_TIME + 0.5)
#define IRSND_ROOMBA_0_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * ROOMBA_0_PAUSE_TIME + 0.5)
#define IRSND_ROOMBA_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * ROOMBA_FRAME_REPEAT_PAUSE_TIME + 0.5)               // use uint16_t!

#define IRSND_PENTAX_START_BIT_PULSE_LEN            (uint8_t)(F_INTERRUPTS * PENTAX_START_BIT_PULSE_TIME + 0.5)
#define IRSND_PENTAX_START_BIT_PAUSE_LEN            (uint8_t)(F_INTERRUPTS * PENTAX_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_PENTAX_REPEAT_START_BIT_PAUSE_LEN     (uint8_t)(F_INTERRUPTS * PENTAX_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_PENTAX_PULSE_LEN                      (uint8_t)(F_INTERRUPTS * PENTAX_PULSE_TIME + 0.5)
#define IRSND_PENTAX_1_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * PENTAX_1_PAUSE_TIME + 0.5)
#define IRSND_PENTAX_0_PAUSE_LEN                    (uint8_t)(F_INTERRUPTS * PENTAX_0_PAUSE_TIME + 0.5)
#define IRSND_PENTAX_FRAME_REPEAT_PAUSE_LEN         (uint16_t)(F_INTERRUPTS * PENTAX_FRAME_REPEAT_PAUSE_TIME + 0.5)              // use uint16_t!

#define IRSND_ACP24_START_BIT_PULSE_LEN             (uint8_t)(F_INTERRUPTS * ACP24_START_BIT_PULSE_TIME + 0.5)
#define IRSND_ACP24_START_BIT_PAUSE_LEN             (uint8_t)(F_INTERRUPTS * ACP24_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_ACP24_REPEAT_START_BIT_PAUSE_LEN      (uint8_t)(F_INTERRUPTS * ACP24_REPEAT_START_BIT_PAUSE_TIME + 0.5)
#define IRSND_ACP24_PULSE_LEN                       (uint8_t)(F_INTERRUPTS * ACP24_PULSE_TIME + 0.5)
#define IRSND_ACP24_1_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * ACP24_1_PAUSE_TIME + 0.5)
#define IRSND_ACP24_0_PAUSE_LEN                     (uint8_t)(F_INTERRUPTS * ACP24_0_PAUSE_TIME + 0.5)
#define IRSND_ACP24_FRAME_REPEAT_PAUSE_LEN          (uint16_t)(F_INTERRUPTS * ACP24_FRAME_REPEAT_PAUSE_TIME + 0.5)                // use uint16_t!

#ifdef PIC_C18                                  // PIC C18
#  define IRSND_FREQ_TYPE                       uint8_t
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 30000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 32000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 36000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 38000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 40000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 56000  / 2 / Pre_Scaler / PIC_Scaler) - 1)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) ((F_CPU / 455000 / 2 / Pre_Scaler / PIC_Scaler) - 1)
#elif defined (ARM_STM32)                       // STM32
#  define IRSND_FREQ_TYPE                       uint32_t
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) (30000)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) (32000)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) (36000)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) (38000)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) (40000)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) (56000)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) (455000)
#elif defined (ARM_STM32_HAL)                   // STM32 with Hal Library
#  define IRSND_FREQ_TYPE                       uint32_t
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) (30000)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) (32000)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) (36000)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) (38000)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) (40000)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) (56000)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) (455000)
#elif defined (TEENSY_ARM_CORTEX_M4)            // TEENSY
#  define IRSND_FREQ_TYPE                       float
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) (30000)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) (32000)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) (36000)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) (38000)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) (40000)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) (56000)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) (455000)
#elif defined (__xtensa__)                      // ESP8266
#  define IRSND_FREQ_TYPE                       float
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) (30000)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) (32000)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) (36000)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) (38000)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) (40000)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) (56000)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) (455000)
#else                                           // AVR
#  if F_CPU >= 16000000L
#    define AVR_PRESCALER                       8
#  else
#    define AVR_PRESCALER                       1
#  endif
#  define IRSND_FREQ_TYPE                       uint8_t
#  define IRSND_FREQ_30_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 30000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_32_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 32000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_36_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 36000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_38_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 38000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_40_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 40000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_56_KHZ                     (IRSND_FREQ_TYPE) ((F_CPU / 56000 / AVR_PRESCALER / 2) - 1)
#  define IRSND_FREQ_455_KHZ                    (IRSND_FREQ_TYPE) ((F_CPU / 455000 / AVR_PRESCALER / 2) - 1)
#endif

// Used for Arduino by IRTimer.cpp.h
volatile uint8_t                                irsnd_busy = 0;
volatile uint8_t                                irsnd_is_on = FALSE;

static volatile uint8_t                         irsnd_protocol = 0;
static volatile uint8_t                         irsnd_buffer[11] = {0};
static volatile uint8_t                         irsnd_repeat = 0;

#if defined(ARDUINO)
#include "irsndArduinoExt.cpp.h" // must be after the declarations of irsnd_busy etc.
#else

#if IRSND_USE_CALLBACK == 1
static void                                     (*irsnd_callback_ptr) (uint8_t);
#endif // IRSND_USE_CALLBACK == 1

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  Switch PWM on
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
irsnd_on (void)
{
    if (! irsnd_is_on)
    {
#ifndef ANALYZE
#  if defined(PIC_C18)                                  // PIC C18
        PWMon();
        // IRSND_PIN = 0; // output mode -> enable PWM outout pin (0=PWM on, 1=PWM off)

#  elif defined (ARM_STM32)                             // STM32
        TIM_SelectOCxM(IRSND_TIMER, IRSND_TIMER_CHANNEL, TIM_OCMode_PWM1); // enable PWM as OC-mode
        TIM_CCxCmd(IRSND_TIMER, IRSND_TIMER_CHANNEL, TIM_CCx_Enable);      // enable OC-output (is being disabled in TIM_SelectOCxM())
        TIM_Cmd(IRSND_TIMER, ENABLE);                   // enable counter

#  elif defined (ARM_STM32_HAL)                         // STM32 with Hal Library
        HAL_TIM_PWM_Start(&IRSND_TIMER_HANDLER, IRSND_TIMER_CHANNEL_NUMBER);

#  elif defined (TEENSY_ARM_CORTEX_M4)                  // TEENSY
        analogWrite(IRSND_PIN, 33 * 255 / 100);         // pwm 33%

#  elif defined (__xtensa__)                            // ESP8266 (Arduino)
        analogWrite(IRSND_PIN, 33 * 1023 / 100);        // pwm 33%

#  elif defined (__AVR_XMEGA__)
#    if (IRSND_OCx == IRSND_XMEGA_OC0A)                                 // use OC0A
                XMEGA_Timer.CTRLB |= (1<<TC0_CCAEN_bp);                 // Compare A
#    elif (IRSND_OCx == IRSND_XMEGA_OC0B)                               // use OC0B
                XMEGA_Timer.CTRLB |= (1<<TC0_CCBEN_bp);                 // Compare B
#    elif IRSND_OCx == IRSND_XMEGA_OC0C                                 // use OC0C
                XMEGA_Timer.CTRLB |= (1<<TC0_CCCEN_bp);                 // Compare C
#    elif IRSND_OCx == IRSND_XMEGA_OC0D                                 // use OC0D
                XMEGA_Timer.CTRLB |= (1<<TC0_CCDEN_bp);                 // Compare D
#    elif IRSND_OCx == IRSND_XMEGA_OC1A                                 // use OC1A
                XMEGA_Timer.CTRLB |= (1<<TC1_CCAEN_bp);                 // Compare A
#    elif IRSND_OCx == IRSND_XMEGA_OC1B                                 // use OC1B
                XMEGA_Timer.CTRLB |= (1<<TC1_CCBEN_bp);                 // Compare B
#    else
#       error wrong value of IRSND_OCx
#    endif // IRSND_OCx

#  else                                                 // AVR
#    if   IRSND_OCx == IRSND_OC2                        // use OC2
        TCCR2 |= (1<<COM20)|(1<<WGM21);                 // toggle OC2 on compare match,  clear Timer 2 at compare match OCR2
#    elif IRSND_OCx == IRSND_OC2A                       // use OC2A
        TCCR2A |= (1<<COM2A0)|(1<<WGM21);               // toggle OC2A on compare match, clear Timer 2 at compare match OCR2A
#    elif IRSND_OCx == IRSND_OC2B                       // use OC2B
        TCCR2A |= (1<<COM2B0)|(1<<WGM21);               // toggle OC2B on compare match, clear Timer 2 at compare match OCR2A (yes: A, not B!)
#    elif IRSND_OCx == IRSND_OC0                        // use OC0
        TCCR0 |= (1<<COM00)|(1<<WGM01);                 // toggle OC0 on compare match,  clear Timer 0 at compare match OCR0
#    elif IRSND_OCx == IRSND_OC0A                       // use OC0A
        TCCR0A |= (1<<COM0A0)|(1<<WGM01);               // toggle OC0A on compare match, clear Timer 0 at compare match OCR0A
#    elif IRSND_OCx == IRSND_OC0B                       // use OC0B
        TCCR0A |= (1<<COM0B0)|(1<<WGM01);               // toggle OC0B on compare match, clear Timer 0 at compare match OCR0A (yes: A, not B!)
#    else
#      error wrong value of IRSND_OCx
#    endif // IRSND_OCx
#  endif // C18
#endif // ANALYZE

#if IRSND_USE_CALLBACK == 1
        if (irsnd_callback_ptr)
        {
            (*irsnd_callback_ptr) (TRUE);
        }
#endif // IRSND_USE_CALLBACK == 1

        irsnd_is_on = TRUE;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  Switch PWM off
 *  @details  Switches PWM off
 *  Only called by irsnd_ISR
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
static void
irsnd_off (void)
{
    if (irsnd_is_on)
    {
#ifndef ANALYZE

#  if defined(PIC_C18)                                                                  // PIC C18
        PWMoff();
        // IRSND_PIN = 1; //input mode -> disbale PWM output pin (0=PWM on, 1=PWM off)

#  elif defined (ARM_STM32)                                                             // STM32
        TIM_Cmd(IRSND_TIMER, DISABLE);                                                  // disable counter
        TIM_SelectOCxM(IRSND_TIMER, IRSND_TIMER_CHANNEL, TIM_ForcedAction_InActive);    // force output inactive
        TIM_CCxCmd(IRSND_TIMER, IRSND_TIMER_CHANNEL, TIM_CCx_Enable);                   // enable OC-output (is being disabled in TIM_SelectOCxM())
        TIM_SetCounter(IRSND_TIMER, 0);                                                 // reset counter value

#  elif defined (ARM_STM32_HAL)                                                         // STM32
        HAL_TIM_PWM_Stop(&IRSND_TIMER_HANDLER, IRSND_TIMER_CHANNEL_NUMBER);

#  elif defined (TEENSY_ARM_CORTEX_M4)                                                  // TEENSY
        analogWrite(IRSND_PIN, 0);                                                      // pwm off, LOW level

#  elif defined (__xtensa__)                                                            // ESP8266
        analogWrite(IRSND_PIN, 0);                                                      // pwm off, LOW level

#  elif defined (__AVR_XMEGA__)
#    if (IRSND_OCx == IRSND_XMEGA_OC0A)                                                 // use OC0A
        XMEGA_Timer.CTRLB &= ~(1<<TC0_CCAEN_bp);                                        // Compare A disconnected
#    elif (IRSND_OCx == IRSND_XMEGA_OC0B)                                               // use OC0B
        XMEGA_Timer.CTRLB &= ~(1<<TC0_CCBEN_bp);                                        // Compare B disconnected
#    elif IRSND_OCx == IRSND_XMEGA_OC0C                                                 // use OC0C
        XMEGA_Timer.CTRLB &= ~(1<<TC0_CCCEN_bp);                                        // Compare C disconnected
#    elif IRSND_OCx == IRSND_XMEGA_OC0D                                                 // use OC0D
        XMEGA_Timer.CTRLB &= ~(1<<TC0_CCDEN_bp);                                        // Compare D disconnected
#    elif IRSND_OCx == IRSND_XMEGA_OC1A                                                 // use OC1A
                XMEGA_Timer.CTRLB &= ~(1<<TC1_CCAEN_bp);                                // Compare A disconnected
#    elif IRSND_OCx == IRSND_XMEGA_OC1B                                                 // use OC1B
                XMEGA_Timer.CTRLB &= ~(1<<TC1_CCBEN_bp);                                // Compare B disconnected
#    else
#       error wrong value of IRSND_OCx
#    endif // IRSND_OCx

#  else //AVR

#    if   IRSND_OCx == IRSND_OC2                        // use OC2
        TCCR2 &= ~(1<<COM20);                           // normal port operation, OC2 disconnected.
#    elif IRSND_OCx == IRSND_OC2A                       // use OC2A
        TCCR2A &= ~(1<<COM2A0);                         // normal port operation, OC2A disconnected.
#    elif IRSND_OCx == IRSND_OC2B                       // use OC2B
        TCCR2A &= ~(1<<COM2B0);                         // normal port operation, OC2B disconnected.
#    elif IRSND_OCx == IRSND_OC0                        // use OC0
        TCCR0 &= ~(1<<COM00);                           // normal port operation, OC0 disconnected.
#    elif IRSND_OCx == IRSND_OC0A                       // use OC0A
        TCCR0A &= ~(1<<COM0A0);                         // normal port operation, OC0A disconnected.
#    elif IRSND_OCx == IRSND_OC0B                       // use OC0B
        TCCR0A &= ~(1<<COM0B0);                         // normal port operation, OC0B disconnected.
#    else
#      error wrong value of IRSND_OCx
#    endif // IRSND_OCx
        IRSND_PORT  &= ~(1<<IRSND_BIT);                 // set IRSND_BIT to low
#  endif //C18
#endif // ANALYZE

#if IRSND_USE_CALLBACK == 1
        if (irsnd_callback_ptr)
        {
           (*irsnd_callback_ptr) (FALSE);
        }
#endif // IRSND_USE_CALLBACK == 1

        irsnd_is_on = FALSE;
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  Set PWM frequency
 *  @details  sets pwm frequency
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined(__12F1840)
extern void pwm_init(uint16_t freq);
#include <stdio.h>
#endif

static void
irsnd_set_freq (IRSND_FREQ_TYPE freq)
{
#ifndef ANALYZE
#  if defined(PIC_C18)                                                                      // PIC C18 or XC8
#    if defined(__12F1840)                                                                  // XC8
        TRISA2=0;
        PR2=freq;
        CCP1M0=1;
        CCP1M1=1;
        CCP1M2=1;
        CCP1M3=1;
        DC1B0=1;
        DC1B1=0;
        CCPR1L = 0b01101001;
        TMR2IF = 0;
        TMR2ON=1;
        CCP1CON &=(~0b0011); // p 197 "active high"
#    else                                                                                   // PIC C18
        OpenPWM(freq);
        SetDCPWM( (uint16_t) (freq * 2) + 1); // freq*2 = Duty cycles 50%
#    endif
        PWMoff();
#  elif defined (ARM_STM32)                                                                 // STM32
        static uint32_t      TimeBaseFreq = 0;

        if (TimeBaseFreq == 0)
        {
            RCC_ClocksTypeDef        RCC_ClocksStructure;
            /* Get system clocks and store timer clock in variable */
            RCC_GetClocksFreq(&RCC_ClocksStructure);
#    if ((IRSND_TIMER_NUMBER >= 2) && (IRSND_TIMER_NUMBER <= 5)) || ((IRSND_TIMER_NUMBER >= 12) && (IRSND_TIMER_NUMBER <= 14))
            if (RCC_ClocksStructure.PCLK1_Frequency == RCC_ClocksStructure.HCLK_Frequency)
            {
               TimeBaseFreq = RCC_ClocksStructure.PCLK1_Frequency;
            }
            else
            {
               TimeBaseFreq = RCC_ClocksStructure.PCLK1_Frequency * 2;
            }
#    else
            if (RCC_ClocksStructure.PCLK2_Frequency == RCC_ClocksStructure.HCLK_Frequency)
            {
               TimeBaseFreq = RCC_ClocksStructure.PCLK2_Frequency;
            }
            else
            {
               TimeBaseFreq = RCC_ClocksStructure.PCLK2_Frequency * 2;
            }
#    endif
        }

        freq = TimeBaseFreq/freq;

        /* Set frequency */
        TIM_SetAutoreload(IRSND_TIMER, freq - 1);
        /* Set duty cycle */
        TIM_SetCompare1(IRSND_TIMER, (freq + 1) / 2);

#  elif defined (ARM_STM32_HAL)                                                            // STM32 with Hal Library

        TIM_MasterConfigTypeDef sMasterConfig;
        TIM_OC_InitTypeDef sConfigOC;

        uint32_t uwPrescalerValue = (uint32_t) ((IRSND_TIMER_SPEED_APBX) / (freq*10)) - 1; //1kHz

        //IRSND_TIMER_HANDLER.Instance = TIM2;
        IRSND_TIMER_HANDLER.Init.Prescaler = uwPrescalerValue;
        IRSND_TIMER_HANDLER.Init.CounterMode = TIM_COUNTERMODE_UP;
        IRSND_TIMER_HANDLER.Init.Period = 10 -1;
        IRSND_TIMER_HANDLER.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        IRSND_TIMER_HANDLER.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

        if (HAL_TIM_PWM_Init(&IRSND_TIMER_HANDLER) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

        if (HAL_TIMEx_MasterConfigSynchronization(&IRSND_TIMER_HANDLER, &sMasterConfig) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = IRSND_TIMER_HANDLER.Init.Period / 2;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

        if (HAL_TIM_PWM_ConfigChannel(&IRSND_TIMER_HANDLER, &sConfigOC, IRSND_TIMER_CHANNEL_NUMBER ) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

#  elif defined (TEENSY_ARM_CORTEX_M4)
        analogWriteResolution(8);                                                           // 8 bit
        analogWriteFrequency(IRSND_PIN, freq);
        analogWrite(IRSND_PIN, 0);                                                          // pwm off, LOW level

#elif defined (__xtensa__)
        // analogWriteRange(255);
        analogWriteFreq(freq);
        analogWrite(IRSND_PIN, 0);                                                          // pwm off, LOW level

#  elif defined (__AVR_XMEGA__)
        XMEGA_Timer.CCA = freq;

#  else                                                                                     // AVR

#    if IRSND_OCx == IRSND_OC2
        OCR2 = freq;                                                                        // use register OCR2 for OC2
#    elif IRSND_OCx == IRSND_OC2A                                                           // use OC2A
        OCR2A = freq;                                                                       // use register OCR2A for OC2A and OC2B!
#    elif IRSND_OCx == IRSND_OC2B                                                           // use OC2B
        OCR2A = freq;                                                                       // use register OCR2A for OC2A and OC2B!
#    elif IRSND_OCx == IRSND_OC0                                                            // use OC0
        OCR0 = freq;                                                                        // use register OCR2 for OC2
#    elif IRSND_OCx == IRSND_OC0A                                                           // use OC0A
        OCR0A = freq;                                                                       // use register OCR0A for OC0A and OC0B!
#    elif IRSND_OCx == IRSND_OC0B                                                           // use OC0B
        OCR0A = freq;                                                                       // use register OCR0A for OC0A and OC0B!
#    else
#      error wrong value of IRSND_OCx
#    endif
#  endif //PIC_C18
#endif // ANALYZE
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  Initialize the PWM
 *  @details  Configures 0CR0A, 0CR0B and 0CR2B as PWM channels
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
void
irsnd_init (void)
{
#ifndef ANALYZE
#  if defined(PIC_C18)                                                      // PIC C18 or XC8 compiler
#    if ! defined(__12F1840)                                                // only C18:
        OpenTimer;
#    endif
        irsnd_set_freq (IRSND_FREQ_36_KHZ);                                 // default frequency
        IRSND_PIN = 0;                                                      // set IO to outout
        PWMoff();
#  elif defined (ARM_STM32)                                                 // STM32
        GPIO_InitTypeDef            GPIO_InitStructure;
        TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
        TIM_OCInitTypeDef           TIM_OCInitStructure;

       /* GPIOx clock enable */
#    if defined (ARM_STM32L1XX)
        RCC_AHBPeriphClockCmd(IRSND_PORT_RCC, ENABLE);
#    elif defined (ARM_STM32F10X)
        RCC_APB2PeriphClockCmd(IRSND_PORT_RCC, ENABLE);
        // RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // only in case of remapping, not necessary for default port-timer mapping
#    elif defined (ARM_STM32F30X)
        RCC_AHBPeriphClockCmd(IRSND_PORT_RCC, ENABLE);
#    elif defined (ARM_STM32F4XX)
        RCC_AHB1PeriphClockCmd(IRSND_PORT_RCC, ENABLE);
#    endif

        /* GPIO Configuration */
        GPIO_InitStructure.GPIO_Pin = IRSND_BIT;
#    if defined (ARM_STM32L1XX) || defined (ARM_STM32F4XX)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(IRSND_PORT, &GPIO_InitStructure);
        GPIO_PinAFConfig(IRSND_PORT, (uint8_t)IRSND_BIT_NUMBER, IRSND_GPIO_AF);
#    elif defined (ARM_STM32F10X)
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(IRSND_PORT, &GPIO_InitStructure);
        // GPIO_PinRemapConfig(GPIO_*Remap*_TIM[IRSND_TIMER_NUMBER], ENABLE); // only in case of remapping, not necessary for default port-timer mapping
#    elif defined (ARM_STM32F30X)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(IRSND_PORT, &GPIO_InitStructure);
        // GPIO_PinRemapConfig(GPIO_*Remap*_TIM[IRSND_TIMER_NUMBER], ENABLE); // only in case of remapping, not necessary for default port-timer mapping

#    endif

        /* TIMx clock enable */
#    if ((IRSND_TIMER_NUMBER >= 2) && (IRSND_TIMER_NUMBER <= 5)) || ((IRSND_TIMER_NUMBER >= 12) && (IRSND_TIMER_NUMBER <= 14))
        RCC_APB1PeriphClockCmd(IRSND_TIMER_RCC, ENABLE);
#    else
        RCC_APB2PeriphClockCmd(IRSND_TIMER_RCC, ENABLE);
#    endif

        /* Time base configuration */
        TIM_TimeBaseStructure.TIM_Period = -1;     // set dummy value (don't set to 0), will be initialized later
        TIM_TimeBaseStructure.TIM_Prescaler = 0;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInit(IRSND_TIMER, &TIM_TimeBaseStructure);

        /* PWM1 Mode configuration */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;         // will be initialized later
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
        TIM_OC1Init(IRSND_TIMER, &TIM_OCInitStructure);

        /* Preload configuration */
        TIM_ARRPreloadConfig(IRSND_TIMER, ENABLE);
        TIM_OC1PreloadConfig(IRSND_TIMER, TIM_OCPreload_Enable);

        irsnd_set_freq (IRSND_FREQ_36_KHZ);                                         // set default frequency

#  elif defined (ARM_STM32_HAL)
        irsnd_set_freq (IRSND_FREQ_36_KHZ);                                         // default frequency

#  elif defined (TEENSY_ARM_CORTEX_M4)
        if (!digitalPinHasPWM(IRSND_PIN))
        {
            return;
        }

#  elif defined (__xtensa__)
        pinMode(IRSND_PIN, OUTPUT);
        irsnd_set_freq (IRSND_FREQ_36_KHZ);

#  elif defined (__AVR_XMEGA__)
        IRSND_PORT &= ~(1<<IRSND_BIT);                                              // set IRSND_BIT to low
        IRSND_DDR |= (1<<IRSND_BIT);                                                // set IRSND_BIT to output

        XMEGA_Timer.PER = 0xFFFF; //Topwert
        XMEGA_Timer.CTRLB |= TC_WGMODE_FRQ_gc; //Modus: Frequenz entspricht CTC

#    if AVR_PRESCALER == 8
        XMEGA_Timer.CTRLA |= TC_CLKSEL_DIV8_gc;                                     // start Timer  prescaler = 8
#    else
        XMEGA_Timer.CTRLA |= TC_CLKSEL_DIV1_gc;                                     // start Timer  prescaler = 1
#    endif

# else                                                                              // AVR
        IRSND_PORT &= ~(1<<IRSND_BIT);                                              // set IRSND_BIT to low
        IRSND_DDR |= (1<<IRSND_BIT);                                                // set IRSND_BIT to output

#    if   IRSND_OCx == IRSND_OC2                                                    // use OC2
        TCCR2 = (1<<WGM21);                                                         // CTC mode
#       if AVR_PRESCALER == 8
          TCCR2 |= (1<<CS21);                                                       // start Timer 2, prescaler = 8
#       else
          TCCR2 |= (1<<CS20);                                                       // start Timer 2, prescaler = 1
#       endif
#    elif IRSND_OCx == IRSND_OC2A || IRSND_OCx == IRSND_OC2B                        // use OC2A or OC2B
        TCCR2A = (1<<WGM21);                                                        // CTC mode
#       if AVR_PRESCALER == 8
          TCCR2B = (1<<CS21);                                                       // start Timer 2, prescaler = 8
#       else
          TCCR2B = (1<<CS20);                                                       // start Timer 2, prescaler = 1
#       endif
#    elif IRSND_OCx == IRSND_OC0                                                    // use OC0
        TCCR0 = (1<<WGM01);                                                         // CTC mode
#       if AVR_PRESCALER == 8
          TCCR0 |= (1<<CS01);                                                       // start Timer 0, prescaler = 8
#       else
          TCCR0 |= (1<<CS00);                                                       // start Timer 0, prescaler = 1
#       endif
#    elif IRSND_OCx == IRSND_OC0A || IRSND_OCx == IRSND_OC0B                        // use OC0A or OC0B
        TCCR0A = (1<<WGM01);                                                        // CTC mode
#       if AVR_PRESCALER == 8
          TCCR0B = (1<<CS01);                                                       // start Timer 0, prescaler = 8
#       else
          TCCR0B = (1<<CS00);                                                       // start Timer 0, prescaler = 1
#       endif
#    else
#      error wrong value of IRSND_OCx
#    endif
        irsnd_set_freq (IRSND_FREQ_36_KHZ);                                         // default frequency
#  endif //PIC_C18
#endif // ANALYZE
}

#if IRSND_USE_CALLBACK == 1
void
irsnd_set_callback_ptr (void (*cb)(uint8_t))
{
    irsnd_callback_ptr = cb;
}
#endif // IRSND_USE_CALLBACK == 1

#endif // ARDUINO

#  ifdef __cplusplus
bool
#else
uint8_t
#endif
irsnd_is_busy (void)
{
    return irsnd_busy;
}

static uint16_t
bitsrevervse (uint16_t x, uint8_t len)
{
    uint16_t    xx = 0;

    while(len)
    {
        xx <<= 1;
        if (x & 1)
        {
            xx |= 1;
        }
        x >>= 1;
        len--;
    }
    return xx;
}


#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
static uint8_t  sircs_additional_bitlen;
#endif // IRSND_SUPPORT_SIRCS_PROTOCOL == 1

/*
 * @param  do_wait - wait for last command to have ended before sending. For Arduino: Additionally wait for sent command to have ended.
 */
#  ifdef __cplusplus
bool
#else
uint8_t
#endif
irsnd_send_data (IRMP_DATA * irmp_data_p, uint8_t do_wait)
{
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
    static uint8_t  toggle_bit_recs80;
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
    static uint8_t  toggle_bit_recs80ext;
#endif
#if IRSND_SUPPORT_RC5_PROTOCOL == 1
    static uint8_t  toggle_bit_rc5;
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1 || IRSND_SUPPORT_RC6A_PROTOCOL == 1
    static uint8_t  toggle_bit_rc6;
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
    static uint8_t  toggle_bit_thomson;
#endif
    uint16_t        address;
    uint16_t        command;

    if (do_wait)
    {
        while (irsnd_busy)
        {
            // wait for last command to have ended
        }
    }
    else if (irsnd_busy)
    {
        return (FALSE);
    }

    irsnd_protocol  = irmp_data_p->protocol;
    irsnd_repeat    = irmp_data_p->flags & IRSND_REPETITION_MASK;

    switch (irsnd_protocol)
    {
#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
        case IRMP_SIRCS_PROTOCOL:
        {
            // uint8_t  sircs_additional_command_len;
            uint8_t  sircs_additional_address_len;

            sircs_additional_bitlen = (irmp_data_p->address & 0xFF00) >> 8;                             // additional bitlen

            if (sircs_additional_bitlen > 15 - SIRCS_MINIMUM_DATA_LEN)
            {
                // sircs_additional_command_len = 15 - SIRCS_MINIMUM_DATA_LEN;
                sircs_additional_address_len = sircs_additional_bitlen - (15 - SIRCS_MINIMUM_DATA_LEN);
            }
            else
            {
                // sircs_additional_command_len = sircs_additional_bitlen;
                sircs_additional_address_len = 0;
            }

            command = bitsrevervse (irmp_data_p->command, 15);

            irsnd_buffer[0] = (command & 0x7F80) >> 7;                                                  // CCCCCCCC
            irsnd_buffer[1] = (command & 0x007F) << 1;                                                  // CCCC****

            if (sircs_additional_address_len > 0)
            {
                address = bitsrevervse (irmp_data_p->address, 5);
                irsnd_buffer[1] |= (address & 0x0010) >> 4;
                irsnd_buffer[2] =  (address & 0x000F) << 4;
            }
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
        case IRMP_APPLE_PROTOCOL:
        {
            command = irmp_data_p->command | (irmp_data_p->address << 8);                               // store address as ID in upper byte of command
            address = 0x87EE;                                                                           // set fixed NEC-lookalike address (customer ID of apple)

            address = bitsrevervse (address, NEC_ADDRESS_LEN);
            command = bitsrevervse (command, NEC_COMMAND_LEN);

            irsnd_protocol = IRMP_NEC_PROTOCOL;                                                         // APPLE protocol is NEC with id instead of inverted command

            irsnd_buffer[0] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[1] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[2] = (command & 0xFF00) >> 8;                                                          // CCCCCCCC
            irsnd_buffer[3] = (command & 0x00FF);                                                               // cccccccc (ID)
            irsnd_busy      = TRUE;
            break;
        }
        case IRMP_NEC_PROTOCOL:
        {
            if (irmp_data_p->flags & IRSND_RAW_REPETITION_FRAME)
            {
                irsnd_protocol = IRMP_NEC_REPETITION_PROTOCOL;                                          // send a raw repetition frame
                irsnd_buffer[0] = 0x00;                                                                 // no address, no command
            }
            else
            {
                address = bitsrevervse (irmp_data_p->address, NEC_ADDRESS_LEN);
                command = bitsrevervse (irmp_data_p->command, NEC_COMMAND_LEN);

                irsnd_buffer[0] = (address & 0xFF00) >> 8;                                              // AAAAAAAA
                irsnd_buffer[1] = (address & 0x00FF);                                                   // AAAAAAAA
                irsnd_buffer[2] = (command & 0xFF00) >> 8;                                              // CCCCCCCC
                irsnd_buffer[3] = ~((command & 0xFF00) >> 8);                                           // cccccccc
            }
            irsnd_busy = TRUE;
            break;
        }
        case IRMP_ONKYO_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, NEC_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, NEC_COMMAND_LEN);

            irsnd_buffer[0] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[1] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[2] = (command & 0xFF00) >> 8;                                                          // CCCCCCCC
            irsnd_buffer[3] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
        case IRMP_NEC16_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, NEC16_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, NEC16_COMMAND_LEN);

            irsnd_buffer[0] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[1] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NEC42_PROTOCOL == 1
        case IRMP_NEC42_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, NEC42_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, NEC42_COMMAND_LEN);

            irsnd_buffer[0] = ( (address & 0x1FE0) >> 5);                                                       // AAAAAAAA
            irsnd_buffer[1] = ( (address & 0x001F) << 3) | ((~address & 0x1C00) >> 10);                         // AAAAAaaa
            irsnd_buffer[2] =                              ((~address & 0x03FC) >> 2);                          // aaaaaaaa
            irsnd_buffer[3] = ((~address & 0x0003) << 6) | ( (command & 0x00FC) >> 2);                          // aaCCCCCC
            irsnd_buffer[4] = ( (command & 0x0003) << 6) | ((~command & 0x00FC) >> 2);                          // CCcccccc
            irsnd_buffer[5] = ((~command & 0x0003) << 6);                                                       // cc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_MELINERA_PROTOCOL == 1
        case IRMP_MELINERA_PROTOCOL:
        {
            command = irmp_data_p->command;

            irsnd_buffer[0] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_LGAIR_PROTOCOL == 1
        case IRMP_LGAIR_PROTOCOL:
        {
            address = irmp_data_p->address;
            command = irmp_data_p->command;

            irsnd_buffer[0] = ( (address & 0x00FF));                                                            // AAAAAAAA
            irsnd_buffer[1] = ( (command & 0xFF00) >> 8);                                                       // CCCCCCCC
            irsnd_buffer[2] = ( (command & 0x00FF));                                                            // CCCCCCCC
            irsnd_buffer[3] = (( ((command & 0xF000) >> 12) +                                                   // checksum
                                 ((command & 0x0F00) >> 8) +
                                 ((command & 0x00F0) >>4 ) +
                                 ((command & 0x000F))) & 0x000F) << 4;
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
        case IRMP_SAMSUNG_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, SAMSUNG_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, SAMSUNG_COMMAND_LEN);

            irsnd_buffer[0] =  (address & 0xFF00) >> 8;                                                         // AAAAAAAA
            irsnd_buffer[1] =  (address & 0x00FF);                                                              // AAAAAAAA
            irsnd_buffer[2] =  (command & 0x00F0) | ((command & 0xF000) >> 12);                                 // IIIICCCC
            irsnd_buffer[3] = ((command & 0x0F00) >> 4) | ((~(command & 0xF000) >> 12) & 0x0F);                 // CCCCcccc
            irsnd_buffer[4] = (~(command & 0x0F00) >> 4) & 0xF0;                                                // cccc0000
            irsnd_busy      = TRUE;
            break;
        }
        case IRMP_SAMSUNG32_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, SAMSUNG_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, SAMSUNG32_COMMAND_LEN);

            irsnd_buffer[0] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[1] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[2] = (command & 0xFF00) >> 8;                                                          // CCCCCCCC
            irsnd_buffer[3] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_SAMSUNG48_PROTOCOL == 1
        case IRMP_SAMSUNG48_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, SAMSUNG_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, 16);

            irsnd_buffer[0] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[1] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[2] = ((command & 0xFF00) >> 8);                                                        // CCCCCCCC
            irsnd_buffer[3] = ~((command & 0xFF00) >> 8);                                                       // cccccccc
            irsnd_buffer[4] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_buffer[5] = ~(command & 0x00FF);                                                              // cccccccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
        case IRMP_MATSUSHITA_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, MATSUSHITA_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, MATSUSHITA_COMMAND_LEN);

            irsnd_buffer[0] = (command & 0x0FF0) >> 4;                                                          // CCCCCCCC
            irsnd_buffer[1] = ((command & 0x000F) << 4) | ((address & 0x0F00) >> 8);                            // CCCCAAAA
            irsnd_buffer[2] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_TECHNICS_PROTOCOL == 1
        case IRMP_TECHNICS_PROTOCOL:
        {
            command = bitsrevervse (irmp_data_p->command, TECHNICS_COMMAND_LEN);

            irsnd_buffer[0] = (command & 0x07FC) >> 3;                                                          // CCCCCCCC
            irsnd_buffer[1] = ((command & 0x0007) << 5) | ((~command & 0x07C0) >> 6);                           // CCCccccc
            irsnd_buffer[2] = (~command & 0x003F) << 2;                                                         // cccccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1
        case IRMP_KASEIKYO_PROTOCOL:
        {
            uint8_t xor_value;
            uint16_t genre2;

            address = bitsrevervse (irmp_data_p->address, KASEIKYO_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, KASEIKYO_COMMAND_LEN + 4);
            genre2 = bitsrevervse ((irmp_data_p->flags & ~IRSND_REPETITION_MASK) >> 4, 4);

            xor_value = ((address & 0x000F) ^ ((address & 0x00F0) >> 4) ^ ((address & 0x0F00) >> 8) ^ ((address & 0xF000) >> 12)) & 0x0F;

            irsnd_buffer[0] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[1] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[2] = xor_value << 4 | (command & 0x000F);                                              // XXXXCCCC
            irsnd_buffer[3] = (genre2 << 4) | (command & 0xF000) >> 12;                                         // ggggCCCC
            irsnd_buffer[4] = (command & 0x0FF0) >> 4;                                                          // CCCCCCCC

            xor_value = irsnd_buffer[2] ^ irsnd_buffer[3] ^ irsnd_buffer[4];

            irsnd_buffer[5] = xor_value;
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_PANASONIC_PROTOCOL == 1
        case IRMP_PANASONIC_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, PANASONIC_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, PANASONIC_COMMAND_LEN);

            irsnd_buffer[0] = 0x40;                                                                             // 01000000
            irsnd_buffer[1] = 0x04;                                                                             // 00000100
            irsnd_buffer[2] = 0x01;                                                                             // 00000001
            irsnd_buffer[3] = (address & 0xFF00) >> 8;                                                          // AAAAAAAA
            irsnd_buffer[4] = (address & 0x00FF);                                                               // AAAAAAAA
            irsnd_buffer[5] = (command & 0xFF00) >> 8;                                                          // CCCCCCCC
            irsnd_buffer[6] = (command & 0x00FF);                                                               // CCCCCCCC

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
        case IRMP_MITSU_HEAVY_PROTOCOL:
        {
            address = irmp_data_p->address;
            command = irmp_data_p->command;

            irsnd_buffer[0] = 0x4A;
            irsnd_buffer[1] = 0x75;
            irsnd_buffer[2] = 0xC3;
            irsnd_buffer[3] = 0x64;
            irsnd_buffer[4] = 0x9B;
            irsnd_buffer[5] = ~(address & 0xFF00) >> 8;
            irsnd_buffer[6] = (address & 0xFF00) >> 8;
            irsnd_buffer[7] = ~(address & 0x00FF);
            irsnd_buffer[8] = (address & 0x00FF);
            irsnd_buffer[9] = ~(command & 0x00FF);
            irsnd_buffer[10] = (command & 0x00FF);

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
        case IRMP_RECS80_PROTOCOL:
        {
            toggle_bit_recs80 = toggle_bit_recs80 ? 0x00 : 0x80;

            irsnd_buffer[0] = toggle_bit_recs80 | ((irmp_data_p->address & 0x000F) << 4) |
                              ((irmp_data_p->command & 0x003C) >> 2);                                           // TAAACCCC
            irsnd_buffer[1] = (irmp_data_p->command & 0x03) << 6;                                               // CC000000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
        case IRMP_RECS80EXT_PROTOCOL:
        {
            toggle_bit_recs80ext = toggle_bit_recs80ext ? 0x00 : 0x40;

            irsnd_buffer[0] = 0x80 | toggle_bit_recs80ext | ((irmp_data_p->address & 0x000F) << 2) |
                                ((irmp_data_p->command & 0x0030) >> 4);                                         // STAAAACC
            irsnd_buffer[1] = (irmp_data_p->command & 0x0F) << 4;                                               // CCCC0000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RC5_PROTOCOL == 1
        case IRMP_RC5_PROTOCOL:
        {
            toggle_bit_rc5 = toggle_bit_rc5 ? 0x00 : 0x40;

            irsnd_buffer[0] = ((irmp_data_p->command & 0x40) ? 0x00 : 0x80) | toggle_bit_rc5 |
                                ((irmp_data_p->address & 0x001F) << 1) | ((irmp_data_p->command & 0x20) >> 5);  // CTAAAAAC
            irsnd_buffer[1] = (irmp_data_p->command & 0x1F) << 3;                                               // CCCCC000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
        case IRMP_RC6_PROTOCOL:
        {
            toggle_bit_rc6 = toggle_bit_rc6 ? 0x00 : 0x08;

            irsnd_buffer[0] = 0x80 | toggle_bit_rc6 | ((irmp_data_p->address & 0x00E0) >> 5);                   // 1MMMTAAA, MMM = 000
            irsnd_buffer[1] = ((irmp_data_p->address & 0x001F) << 3) | ((irmp_data_p->command & 0xE0) >> 5);    // AAAAACCC
            irsnd_buffer[2] = (irmp_data_p->command & 0x1F) << 3;                                               // CCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RC6A_PROTOCOL == 1
        case IRMP_RC6A_PROTOCOL:
        {
            toggle_bit_rc6 = toggle_bit_rc6 ? 0x00 : 0x08;

            irsnd_buffer[0] = 0x80 | 0x60 | ((irmp_data_p->address & 0x3000) >> 12);                                                // 1MMMT0AA, MMM = 110
            irsnd_buffer[1] = ((irmp_data_p->address & 0x0FFF) >> 4) ;                                                              // AAAAAAAA
            irsnd_buffer[2] = ((irmp_data_p->address & 0x000F) << 4) | ((irmp_data_p->command & 0xF000) >> 12) | toggle_bit_rc6;    // AAAACCCC
            irsnd_buffer[3] = (irmp_data_p->command & 0x0FF0) >> 4;                                                                 // CCCCCCCC
            irsnd_buffer[4] = (irmp_data_p->command & 0x000F) << 4;                                                                 // CCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_DENON_PROTOCOL == 1
        case IRMP_DENON_PROTOCOL:
        {
            irsnd_buffer[0] = ((irmp_data_p->address & 0x1F) << 3) | ((irmp_data_p->command & 0x0380) >> 7);    // AAAAACCC (1st frame)
            irsnd_buffer[1] = (irmp_data_p->command & 0x7F) << 1;                                               // CCCCCCC
            irsnd_buffer[2] = ((irmp_data_p->address & 0x1F) << 3) | (((~irmp_data_p->command) & 0x0380) >> 7); // AAAAAccc (2nd frame)
            irsnd_buffer[3] = (~(irmp_data_p->command) & 0x7F) << 1;                                            // ccccccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
        case IRMP_THOMSON_PROTOCOL:
        {
            toggle_bit_thomson = toggle_bit_thomson ? 0x00 : 0x08;

            irsnd_buffer[0] = ((irmp_data_p->address & 0x0F) << 4) | toggle_bit_thomson | ((irmp_data_p->command & 0x0070) >> 4);   // AAAATCCC (1st frame)
            irsnd_buffer[1] = (irmp_data_p->command & 0x0F) << 4;                                                                   // CCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_BOSE_PROTOCOL == 1
        case IRMP_BOSE_PROTOCOL:
        {
            command = bitsrevervse (irmp_data_p->command, BOSE_COMMAND_LEN);

            irsnd_buffer[0] = (command & 0xFF00) >> 8;                                                                      // CCCCCCCC
            irsnd_buffer[1] = ~((command & 0xFF00) >> 8);                                                                   // cccccccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NUBERT_PROTOCOL == 1
        case IRMP_NUBERT_PROTOCOL:
        {
            irsnd_buffer[0] = irmp_data_p->command >> 2;                                                        // CCCCCCCC
            irsnd_buffer[1] = (irmp_data_p->command & 0x0003) << 6;                                             // CC000000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_FAN_PROTOCOL == 1
        case IRMP_FAN_PROTOCOL:
        {
            irsnd_buffer[0] = irmp_data_p->command >> 3;                                                        // CCCCCCCC
            irsnd_buffer[1] = (irmp_data_p->command & 0x0007) << 5;                                             // CCC00000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_SPEAKER_PROTOCOL == 1
        case IRMP_SPEAKER_PROTOCOL:
        {
            irsnd_buffer[0] = irmp_data_p->command >> 2;                                                        // CCCCCCCC
            irsnd_buffer[1] = (irmp_data_p->command & 0x0003) << 6;                                             // CC000000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
        case IRMP_BANG_OLUFSEN_PROTOCOL:
        {
            irsnd_buffer[0] = irmp_data_p->command >> 11;                                                       // SXSCCCCC
            irsnd_buffer[1] = irmp_data_p->command >> 3;                                                        // CCCCCCCC
            irsnd_buffer[2] = (irmp_data_p->command & 0x0007) << 5;                                             // CCC00000
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
        case IRMP_GRUNDIG_PROTOCOL:
        {
            command = bitsrevervse (irmp_data_p->command, GRUNDIG_COMMAND_LEN);

            irsnd_buffer[0] = 0xFF;                                                                             // S1111111 (1st frame)
            irsnd_buffer[1] = 0xC0;                                                                             // 11
            irsnd_buffer[2] = 0x80 | (command >> 2);                                                            // SCCCCCCC (2nd frame)
            irsnd_buffer[3] = (command << 6) & 0xC0;                                                            // CC

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_TELEFUNKEN_PROTOCOL == 1
        case IRMP_TELEFUNKEN_PROTOCOL:
        {
            irsnd_buffer[0] = irmp_data_p->command >> 7;                                                        // CCCCCCCC
            irsnd_buffer[1] = (irmp_data_p->command << 1) & 0xff;                                               // CCCCCCC

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
        case IRMP_IR60_PROTOCOL:
        {
            command = (bitsrevervse (0x7d, IR60_COMMAND_LEN) << 7) | bitsrevervse (irmp_data_p->command, IR60_COMMAND_LEN);
#if 0
            irsnd_buffer[0] = command >> 6 | 0x01;                                                              // 1011111S (start instruction frame)
            irsnd_buffer[1] = (command & 0x7F) << 1;                                                            // CCCCCCC_ (2nd frame)
#else
            irsnd_buffer[0] = ((command & 0x7F) << 1) | 0x01;                                                   // CCCCCCCS (1st frame)
            irsnd_buffer[1] = command >> 6;                                                                     // 1011111_ (start instruction frame)
#endif

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
        case IRMP_NOKIA_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, NOKIA_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, NOKIA_COMMAND_LEN);

            irsnd_buffer[0] = 0xBF;                                                                             // S0111111 (1st + 3rd frame)
            irsnd_buffer[1] = 0xFF;                                                                             // 11111111
            irsnd_buffer[2] = 0x80;                                                                             // 1
            irsnd_buffer[3] = 0x80 | command >> 1;                                                              // SCCCCCCC (2nd frame)
            irsnd_buffer[4] = (command << 7) | (address >> 1);                                                  // CAAAAAAA
            irsnd_buffer[5] = (address << 7);                                                                   // A

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_SIEMENS_PROTOCOL == 1
        case IRMP_SIEMENS_PROTOCOL:
        {
            irsnd_buffer[0] = ((irmp_data_p->address & 0x07FF) >> 3);                                           // AAAAAAAA
            irsnd_buffer[1] = ((irmp_data_p->address & 0x0007) << 5) | ((irmp_data_p->command >> 5) & 0x1F);    // AAACCCCC
            irsnd_buffer[2] = ((irmp_data_p->command & 0x001F) << 3) | ((~irmp_data_p->command & 0x01) << 2);   // CCCCCc

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RUWIDO_PROTOCOL == 1
        case IRMP_RUWIDO_PROTOCOL:
        {
            irsnd_buffer[0] = ((irmp_data_p->address & 0x01FF) >> 1);                                           // AAAAAAAA
            irsnd_buffer[1] = ((irmp_data_p->address & 0x0001) << 7) | ((irmp_data_p->command & 0x7F));         // ACCCCCCC
            irsnd_buffer[2] = ((~irmp_data_p->command & 0x01) << 7);                                            // c
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
        case IRMP_FDC_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, FDC_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, FDC_COMMAND_LEN);

            irsnd_buffer[0] = (address & 0xFF);                                                                 // AAAAAAAA
            irsnd_buffer[1] = 0;                                                                                // 00000000
            irsnd_buffer[2] = 0;                                                                                // 0000RRRR
            irsnd_buffer[3] = (command & 0xFF);                                                                 // CCCCCCCC
            irsnd_buffer[4] = ~(command & 0xFF);                                                                // cccccccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_RCCAR_PROTOCOL == 1
        case IRMP_RCCAR_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, 2);                                                   //                            A0 A1
            command = bitsrevervse (irmp_data_p->command, RCCAR_COMMAND_LEN - 2);                               // D0 D1 D2 D3 D4 D5 D6 D7 C0 C1 V

            irsnd_buffer[0] = ((command & 0x06) << 5) | ((address & 0x0003) << 4) | ((command & 0x0780) >> 7);  //          C0 C1 A0 A1 D0 D1 D2 D3
            irsnd_buffer[1] = ((command & 0x78) << 1) | ((command & 0x0001) << 3);                              //          D4 D5 D6 D7 V  0  0  0

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_JVC_PROTOCOL == 1
        case IRMP_JVC_PROTOCOL:
        {
            address = bitsrevervse (irmp_data_p->address, JVC_ADDRESS_LEN);
            command = bitsrevervse (irmp_data_p->command, JVC_COMMAND_LEN);

            irsnd_buffer[0] = ((address & 0x000F) << 4) | (command & 0x0F00) >> 8;                              // AAAACCCC
            irsnd_buffer[1] = (command & 0x00FF);                                                               // CCCCCCCC

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
        case IRMP_NIKON_PROTOCOL:
        {
            irsnd_buffer[0] = (irmp_data_p->command & 0x0003) << 6;                                             // CC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_LEGO_PROTOCOL == 1
        case IRMP_LEGO_PROTOCOL:
        {
            uint8_t crc = 0x0F ^ ((irmp_data_p->command & 0x0F00) >> 8) ^ ((irmp_data_p->command & 0x00F0) >> 4) ^ (irmp_data_p->command & 0x000F);

            irsnd_buffer[0] = (irmp_data_p->command & 0x0FF0) >> 4;                                             // CCCCCCCC
            irsnd_buffer[1] = ((irmp_data_p->command & 0x000F) << 4) | crc;                                     // CCCCcccc
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_IRMP16_PROTOCOL == 1
        case IRMP_IRMP16_PROTOCOL:
        {
            command = bitsrevervse (irmp_data_p->command, IRMP16_COMMAND_LEN);

            irsnd_buffer[0] = (command & 0xFF00) >> 8;                                                          // CCCCCCCC
            irsnd_buffer[1] = (command & 0x00FF);                                                               // CCCCCCCC
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
        case IRMP_A1TVBOX_PROTOCOL:
        {
            irsnd_buffer[0] = 0x80 | (irmp_data_p->address >> 2);                                               // 10AAAAAA
            irsnd_buffer[1] = (irmp_data_p->address << 6) | (irmp_data_p->command >> 2);                        // AACCCCCC
            irsnd_buffer[2] = (irmp_data_p->command << 6);                                                      // CC

            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_ROOMBA_PROTOCOL == 1
        case IRMP_ROOMBA_PROTOCOL:
        {
            irsnd_buffer[0] = (irmp_data_p->command & 0x7F) << 1;                                               // CCCCCCC.
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_PENTAX_PROTOCOL == 1
        case IRMP_PENTAX_PROTOCOL:
        {
            irsnd_buffer[0] = (irmp_data_p->command & 0x3F) << 2;                                               // CCCCCC..
            irsnd_busy      = TRUE;
            break;
        }
#endif
#if IRSND_SUPPORT_ACP24_PROTOCOL == 1
#       define ACP_SET_BIT(acp24_bitno, c, irmp_bitno)                                          \
        do                                                                                      \
        {                                                                                       \
            if ((c) & (1<<(irmp_bitno)))                                                        \
            {                                                                                   \
                irsnd_buffer[((acp24_bitno)>>3)] |= 1 << (((7 - (acp24_bitno)) & 0x07));        \
            }                                                                                   \
        } while (0)

        case IRMP_ACP24_PROTOCOL:
        {
            uint16_t    cmd = irmp_data_p->command;
            uint8_t     i;

            address = bitsrevervse (irmp_data_p->address, ACP24_ADDRESS_LEN);

            for (i = 0; i < 8; i++)
            {
                irsnd_buffer[i] = 0x00;                                                                         // CCCCCCCC
            }

            // ACP24-Frame:
            //           1         2         3         4         5         6
            // 0123456789012345678901234567890123456789012345678901234567890123456789
            // N VVMMM    ? ???    t vmA x                 y                     TTTT
            //
            // irmp_data_p->command:
            //
            //         5432109876543210
            //         NAVVvMMMmtxyTTTT

            ACP_SET_BIT( 0, cmd, 15);
            ACP_SET_BIT(24, cmd, 14);
            ACP_SET_BIT( 2, cmd, 13);
            ACP_SET_BIT( 3, cmd, 12);
            ACP_SET_BIT(22, cmd, 11);
            ACP_SET_BIT( 4, cmd, 10);
            ACP_SET_BIT( 5, cmd,  9);
            ACP_SET_BIT( 6, cmd,  8);
            ACP_SET_BIT(23, cmd,  7);
            ACP_SET_BIT(20, cmd,  6);
            ACP_SET_BIT(26, cmd,  5);
            ACP_SET_BIT(44, cmd,  4);
            ACP_SET_BIT(66, cmd,  3);
            ACP_SET_BIT(67, cmd,  2);
            ACP_SET_BIT(68, cmd,  1);
            ACP_SET_BIT(69, cmd,  0);

            irsnd_busy      = TRUE;
            break;
        }
#endif

        default:
        {
            break;
        }
    }

#if defined(ARDUINO)
    storeIRTimer(); // store current timer state to enable alternately send and receive with the same timer
    initIRTimerForSend(); // Setup timer and interrupts for sending
    if (do_wait) {
        while (irsnd_busy) {
            // do nothing;
        }
    }
#endif

    return irsnd_busy;
}

void
irsnd_stop (void)
{
    irsnd_repeat = 0;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 *  ISR routine
 *  @details  ISR routine, called from 10000 to 20000, typically 15000 times per second
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#  ifdef __cplusplus
bool
#else
uint8_t
#endif
irsnd_ISR (void)
{
    static uint8_t              send_trailer                    = FALSE;
    static uint8_t              current_bit                     = 0xFF;
    static uint8_t              pulse_counter                   = 0;
    static IRSND_PAUSE_LEN      pause_counter                   = 0;
    static uint8_t              startbit_pulse_len              = 0;
    static IRSND_PAUSE_LEN      startbit_pause_len              = 0;
    static uint8_t              pulse_1_len                     = 0;
    static uint8_t              pause_1_len                     = 0;
    static uint8_t              pulse_0_len                     = 0;
    static uint8_t              pause_0_len                     = 0;
    static uint8_t              has_stop_bit                    = 0;
    static uint8_t              new_frame                       = TRUE;
    static uint8_t              complete_data_len               = 0;
    static uint8_t              n_repeat_frames                 = 0;                                // number of repetition frames
    static uint8_t              n_auto_repetitions              = 0;                                // number of auto_repetitions
    static uint8_t              auto_repetition_counter         = 0;                                // auto_repetition counter
    static uint16_t             auto_repetition_pause_len       = 0;                                // pause before auto_repetition, uint16_t!
    static uint16_t             auto_repetition_pause_counter   = 0;                                // pause before auto_repetition, uint16_t!
    static uint8_t              repeat_counter                  = 0;                                // repeat counter
    static uint16_t             repeat_frame_pause_len          = 0;                                // pause before repeat, uint16_t!
    static uint16_t             packet_repeat_pause_counter     = 0;                                // pause before repeat, uint16_t!
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
    static uint8_t              last_bit_value;
#endif
    static uint8_t              pulse_len = 0xFF;
    static IRSND_PAUSE_LEN      pause_len = 0xFF;

    if (irsnd_busy)
    {
        if (current_bit == 0xFF && new_frame)                                       // start of transmission...
        {
            if (auto_repetition_counter > 0)
            {
                auto_repetition_pause_counter++;

                if (auto_repetition_pause_counter >= auto_repetition_pause_len)
                {
                    auto_repetition_pause_counter = 0;

#if IRSND_SUPPORT_DENON_PROTOCOL == 1
                    if (irsnd_protocol == IRMP_DENON_PROTOCOL)                              // n'th denon frame
                    {
                        current_bit = 16;
                        complete_data_len   = 2 * DENON_COMPLETE_DATA_LEN + 1;
                    }
                    else
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
                    if (irsnd_protocol == IRMP_GRUNDIG_PROTOCOL)                            // n'th grundig frame
                    {
                        current_bit = 15;
                        complete_data_len   = 16 + GRUNDIG_COMPLETE_DATA_LEN;
                    }
                    else
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
                    if (irsnd_protocol == IRMP_IR60_PROTOCOL)                               // n'th IR60 frame
                    {
                        current_bit = 7;
                        complete_data_len   = 2 * IR60_COMPLETE_DATA_LEN + 1;
                    }
                    else
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
                    if (irsnd_protocol == IRMP_NOKIA_PROTOCOL)                              // n'th nokia frame
                    {
                        if (auto_repetition_counter + 1 < n_auto_repetitions)
                        {
                            current_bit = 23;
                            complete_data_len   = 24 + NOKIA_COMPLETE_DATA_LEN;
                        }
                        else                                                                // nokia stop frame
                        {
                            current_bit = 0xFF;
                            complete_data_len   = NOKIA_COMPLETE_DATA_LEN;
                        }
                    }
                    else
#endif
                    {
                        ;
                    }
                }
                else
                {
#ifdef ANALYZE
                    if (irsnd_is_on)
                    {
                        putchar ('0');
                    }
                    else
                    {
                        putchar ('1');
                    }
#endif
                    return irsnd_busy;
                }
            }
            else if (packet_repeat_pause_counter < repeat_frame_pause_len)
            {
                packet_repeat_pause_counter++;
#ifdef ANALYZE
                if (irsnd_is_on)
                {
                    putchar ('0');
                }
                else
                {
                    putchar ('1');
                }
#endif
                return irsnd_busy;
            }
            else
            {
                if (send_trailer)
                {
                    irsnd_busy = FALSE;
                    send_trailer = FALSE;
                    return irsnd_busy;
                }

                n_repeat_frames             = irsnd_repeat;

                if (n_repeat_frames == IRSND_ENDLESS_REPETITION)
                {
                    n_repeat_frames = 255;
                }

                packet_repeat_pause_counter = 0;
                pulse_counter               = 0;
                pause_counter               = 0;

                switch (irsnd_protocol)
                {
#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
                    case IRMP_SIRCS_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SIRCS_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_SIRCS_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_SIRCS_1_PULSE_LEN;
                        pause_1_len                 = IRSND_SIRCS_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_SIRCS_0_PULSE_LEN;
                        pause_0_len                 = IRSND_SIRCS_PAUSE_LEN - 1;
                        has_stop_bit                = SIRCS_STOP_BIT;
                        complete_data_len           = SIRCS_MINIMUM_DATA_LEN + sircs_additional_bitlen;
                        n_auto_repetitions          = (repeat_counter == 0) ? SIRCS_FRAMES : 1;             // 3 frames auto repetition if first frame
                        auto_repetition_pause_len   = IRSND_SIRCS_AUTO_REPETITION_PAUSE_LEN;                // 25ms pause
                        repeat_frame_pause_len      = IRSND_SIRCS_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_40_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
                    case IRMP_NEC_PROTOCOL:
                    case IRMP_NEC_REPETITION_PROTOCOL:
                    case IRMP_ONKYO_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NEC_START_BIT_PULSE_LEN;

                        if (repeat_counter > 0 || irsnd_protocol == IRMP_NEC_REPETITION_PROTOCOL)
                        {
                            startbit_pause_len      = IRSND_NEC_REPEAT_START_BIT_PAUSE_LEN - 1;
                            complete_data_len       = 0;
                        }
                        else
                        {
                            startbit_pause_len      = IRSND_NEC_START_BIT_PAUSE_LEN - 1;
                            complete_data_len       = NEC_COMPLETE_DATA_LEN;
                        }

                        pulse_1_len                 = IRSND_NEC_PULSE_LEN;
                        pause_1_len                 = IRSND_NEC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NEC_PULSE_LEN;
                        pause_0_len                 = IRSND_NEC_0_PAUSE_LEN - 1;
                        has_stop_bit                = NEC_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_NEC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
                    case IRMP_NEC16_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NEC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_NEC_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_NEC_PULSE_LEN;
                        pause_1_len                 = IRSND_NEC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NEC_PULSE_LEN;
                        pause_0_len                 = IRSND_NEC_0_PAUSE_LEN - 1;
                        has_stop_bit                = NEC_STOP_BIT;
                        complete_data_len           = NEC16_COMPLETE_DATA_LEN + 1;                          // 1 more: sync bit
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_NEC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NEC42_PROTOCOL == 1
                    case IRMP_NEC42_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NEC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_NEC_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_NEC_PULSE_LEN;
                        pause_1_len                 = IRSND_NEC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NEC_PULSE_LEN;
                        pause_0_len                 = IRSND_NEC_0_PAUSE_LEN - 1;
                        has_stop_bit                = NEC_STOP_BIT;
                        complete_data_len           = NEC42_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_NEC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_MELINERA_PROTOCOL == 1
                    case IRMP_MELINERA_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_MELINERA_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_MELINERA_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_MELINERA_1_PULSE_LEN;
                        pause_1_len                 = IRSND_MELINERA_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_MELINERA_0_PULSE_LEN;
                        pause_0_len                 = IRSND_MELINERA_0_PAUSE_LEN - 1;
                        has_stop_bit                = MELINERA_STOP_BIT;
                        complete_data_len           = MELINERA_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_MELINERA_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_LGAIR_PROTOCOL == 1
                    case IRMP_LGAIR_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NEC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_NEC_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_NEC_PULSE_LEN;
                        pause_1_len                 = IRSND_NEC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NEC_PULSE_LEN;
                        pause_0_len                 = IRSND_NEC_0_PAUSE_LEN - 1;
                        has_stop_bit                = NEC_STOP_BIT;
                        complete_data_len           = LGAIR_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_NEC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
                    case IRMP_SAMSUNG_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SAMSUNG_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_SAMSUNG_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_1_len                 = IRSND_SAMSUNG_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_0_len                 = IRSND_SAMSUNG_0_PAUSE_LEN - 1;
                        has_stop_bit                = SAMSUNG_STOP_BIT;
                        complete_data_len           = SAMSUNG_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_SAMSUNG_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }

                    case IRMP_SAMSUNG32_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SAMSUNG_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_SAMSUNG_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_1_len                 = IRSND_SAMSUNG_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_0_len                 = IRSND_SAMSUNG_0_PAUSE_LEN - 1;
                        has_stop_bit                = SAMSUNG_STOP_BIT;
                        complete_data_len           = SAMSUNG32_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = SAMSUNG32_FRAMES;                                     // 1 frame
                        auto_repetition_pause_len   = IRSND_SAMSUNG32_AUTO_REPETITION_PAUSE_LEN;            // 47 ms pause
                        repeat_frame_pause_len      = IRSND_SAMSUNG32_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_SAMSUNG48_PROTOCOL == 1
                    case IRMP_SAMSUNG48_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SAMSUNG_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_SAMSUNG_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_1_len                 = IRSND_SAMSUNG_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_SAMSUNG_PULSE_LEN;
                        pause_0_len                 = IRSND_SAMSUNG_0_PAUSE_LEN - 1;
                        has_stop_bit                = SAMSUNG_STOP_BIT;
                        complete_data_len           = SAMSUNG48_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = SAMSUNG48_FRAMES;                                     // 1 frame
                        auto_repetition_pause_len   = IRSND_SAMSUNG48_AUTO_REPETITION_PAUSE_LEN;            // 47 ms pause
                        repeat_frame_pause_len      = IRSND_SAMSUNG48_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
                    case IRMP_MATSUSHITA_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_MATSUSHITA_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_MATSUSHITA_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_MATSUSHITA_PULSE_LEN;
                        pause_1_len                 = IRSND_MATSUSHITA_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_MATSUSHITA_PULSE_LEN;
                        pause_0_len                 = IRSND_MATSUSHITA_0_PAUSE_LEN - 1;
                        has_stop_bit                = MATSUSHITA_STOP_BIT;
                        complete_data_len           = MATSUSHITA_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_MATSUSHITA_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_TECHNICS_PROTOCOL == 1
                    case IRMP_TECHNICS_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_MATSUSHITA_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_MATSUSHITA_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_MATSUSHITA_PULSE_LEN;
                        pause_1_len                 = IRSND_MATSUSHITA_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_MATSUSHITA_PULSE_LEN;
                        pause_0_len                 = IRSND_MATSUSHITA_0_PAUSE_LEN - 1;
                        has_stop_bit                = MATSUSHITA_STOP_BIT;
                        complete_data_len           = TECHNICS_COMPLETE_DATA_LEN;                           // here TECHNICS
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_MATSUSHITA_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1
                    case IRMP_KASEIKYO_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_KASEIKYO_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_KASEIKYO_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_KASEIKYO_PULSE_LEN;
                        pause_1_len                 = IRSND_KASEIKYO_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_KASEIKYO_PULSE_LEN;
                        pause_0_len                 = IRSND_KASEIKYO_0_PAUSE_LEN - 1;
                        has_stop_bit                = KASEIKYO_STOP_BIT;
                        complete_data_len           = KASEIKYO_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = (repeat_counter == 0) ? KASEIKYO_FRAMES : 1;          // 2 frames auto repetition if first frame
                        auto_repetition_pause_len   = IRSND_KASEIKYO_AUTO_REPETITION_PAUSE_LEN;             // 75 ms pause
                        repeat_frame_pause_len      = IRSND_KASEIKYO_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_PANASONIC_PROTOCOL == 1
                    case IRMP_PANASONIC_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_PANASONIC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_PANASONIC_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_PANASONIC_PULSE_LEN;
                        pause_1_len                 = IRSND_PANASONIC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_PANASONIC_PULSE_LEN;
                        pause_0_len                 = IRSND_PANASONIC_0_PAUSE_LEN - 1;
                        has_stop_bit                = PANASONIC_STOP_BIT;
                        complete_data_len           = PANASONIC_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = PANASONIC_FRAMES;                                     // 1 frame
                        auto_repetition_pause_len   = IRSND_PANASONIC_AUTO_REPETITION_PAUSE_LEN;            // 40 ms pause
                        repeat_frame_pause_len      = IRSND_PANASONIC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
                    case IRMP_MITSU_HEAVY_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_MITSU_HEAVY_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_MITSU_HEAVY_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_MITSU_HEAVY_PULSE_LEN;
                        pause_1_len                 = IRSND_MITSU_HEAVY_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_MITSU_HEAVY_PULSE_LEN;
                        pause_0_len                 = IRSND_MITSU_HEAVY_0_PAUSE_LEN - 1;
                        has_stop_bit                = MITSU_HEAVY_STOP_BIT;
                        complete_data_len           = MITSU_HEAVY_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = MITSU_HEAVY_FRAMES;                                   // 1 frame
                        auto_repetition_pause_len   = 0;;
                        repeat_frame_pause_len      = IRSND_MITSU_HEAVY_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_40_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
                    case IRMP_RECS80_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RECS80_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RECS80_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_RECS80_PULSE_LEN;
                        pause_1_len                 = IRSND_RECS80_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_RECS80_PULSE_LEN;
                        pause_0_len                 = IRSND_RECS80_0_PAUSE_LEN - 1;
                        has_stop_bit                = RECS80_STOP_BIT;
                        complete_data_len           = RECS80_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RECS80_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
                    case IRMP_RECS80EXT_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RECS80EXT_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RECS80EXT_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_RECS80EXT_PULSE_LEN;
                        pause_1_len                 = IRSND_RECS80EXT_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_RECS80EXT_PULSE_LEN;
                        pause_0_len                 = IRSND_RECS80EXT_0_PAUSE_LEN - 1;
                        has_stop_bit                = RECS80EXT_STOP_BIT;
                        complete_data_len           = RECS80EXT_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RECS80EXT_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_TELEFUNKEN_PROTOCOL == 1
                    case IRMP_TELEFUNKEN_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_TELEFUNKEN_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_TELEFUNKEN_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_TELEFUNKEN_PULSE_LEN;
                        pause_1_len                 = IRSND_TELEFUNKEN_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_TELEFUNKEN_PULSE_LEN;
                        pause_0_len                 = IRSND_TELEFUNKEN_0_PAUSE_LEN - 1;
                        has_stop_bit                = TELEFUNKEN_STOP_BIT;
                        complete_data_len           = TELEFUNKEN_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frames
                        auto_repetition_pause_len   = 0;                                                    // IRSND_TELEFUNKEN_AUTO_REPETITION_PAUSE_LEN; xx ms pause
                        repeat_frame_pause_len      = IRSND_TELEFUNKEN_FRAME_REPEAT_PAUSE_LEN;              // 117 msec pause
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RC5_PROTOCOL == 1
                    case IRMP_RC5_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RC5_BIT_LEN;
                        startbit_pause_len          = IRSND_RC5_BIT_LEN;
                        pulse_len                   = IRSND_RC5_BIT_LEN;
                        pause_len                   = IRSND_RC5_BIT_LEN;
                        has_stop_bit                = RC5_STOP_BIT;
                        complete_data_len           = RC5_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RC5_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
                    case IRMP_RC6_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RC6_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RC6_START_BIT_PAUSE_LEN - 1;
                        pulse_len                   = IRSND_RC6_BIT_LEN;
                        pause_len                   = IRSND_RC6_BIT_LEN;
                        has_stop_bit                = RC6_STOP_BIT;
                        complete_data_len           = RC6_COMPLETE_DATA_LEN_SHORT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RC6_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RC6A_PROTOCOL == 1
                    case IRMP_RC6A_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RC6_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RC6_START_BIT_PAUSE_LEN - 1;
                        pulse_len                   = IRSND_RC6_BIT_LEN;
                        pause_len                   = IRSND_RC6_BIT_LEN;
                        has_stop_bit                = RC6_STOP_BIT;
                        complete_data_len           = RC6_COMPLETE_DATA_LEN_LONG;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RC6_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_DENON_PROTOCOL == 1
                    case IRMP_DENON_PROTOCOL:
                    {
                        startbit_pulse_len          = 0x00;
                        startbit_pause_len          = 0x00;
                        pulse_1_len                 = IRSND_DENON_PULSE_LEN;
                        pause_1_len                 = IRSND_DENON_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_DENON_PULSE_LEN;
                        pause_0_len                 = IRSND_DENON_0_PAUSE_LEN - 1;
                        has_stop_bit                = DENON_STOP_BIT;
                        complete_data_len           = DENON_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = DENON_FRAMES;                                         // 2 frames, 2nd with inverted command
                        auto_repetition_pause_len   = IRSND_DENON_AUTO_REPETITION_PAUSE_LEN;                // 65 ms pause after 1st frame
                        repeat_frame_pause_len      = IRSND_DENON_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);                                                 // in theory 32kHz, in practice 36kHz is better
                        break;
                    }
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
                    case IRMP_THOMSON_PROTOCOL:
                    {
                        startbit_pulse_len          = 0x00;
                        startbit_pause_len          = 0x00;
                        pulse_1_len                 = IRSND_THOMSON_PULSE_LEN;
                        pause_1_len                 = IRSND_THOMSON_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_THOMSON_PULSE_LEN;
                        pause_0_len                 = IRSND_THOMSON_0_PAUSE_LEN - 1;
                        has_stop_bit                = THOMSON_STOP_BIT;
                        complete_data_len           = THOMSON_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = THOMSON_FRAMES;                                       // only 1 frame
                        auto_repetition_pause_len   = IRSND_THOMSON_AUTO_REPETITION_PAUSE_LEN;
                        repeat_frame_pause_len      = IRSND_THOMSON_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_BOSE_PROTOCOL == 1
                    case IRMP_BOSE_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_BOSE_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_BOSE_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_BOSE_PULSE_LEN;
                        pause_1_len                 = IRSND_BOSE_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_BOSE_PULSE_LEN;
                        pause_0_len                 = IRSND_BOSE_0_PAUSE_LEN - 1;
                        has_stop_bit                = BOSE_STOP_BIT;
                        complete_data_len           = BOSE_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = BOSE_FRAMES;                                          // 1 frame
                        auto_repetition_pause_len   = IRSND_BOSE_AUTO_REPETITION_PAUSE_LEN;                 // 40 ms pause
                        repeat_frame_pause_len      = IRSND_BOSE_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NUBERT_PROTOCOL == 1
                    case IRMP_NUBERT_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NUBERT_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_NUBERT_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_NUBERT_1_PULSE_LEN;
                        pause_1_len                 = IRSND_NUBERT_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NUBERT_0_PULSE_LEN;
                        pause_0_len                 = IRSND_NUBERT_0_PAUSE_LEN - 1;
                        has_stop_bit                = NUBERT_STOP_BIT;
                        complete_data_len           = NUBERT_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = NUBERT_FRAMES;                                        // 2 frames
                        auto_repetition_pause_len   = IRSND_NUBERT_AUTO_REPETITION_PAUSE_LEN;               // 35 ms pause
                        repeat_frame_pause_len      = IRSND_NUBERT_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_FAN_PROTOCOL == 1
                    case IRMP_FAN_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_FAN_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_FAN_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_FAN_1_PULSE_LEN;
                        pause_1_len                 = IRSND_FAN_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_FAN_0_PULSE_LEN;
                        pause_0_len                 = IRSND_FAN_0_PAUSE_LEN - 1;
                        has_stop_bit                = FAN_STOP_BIT;
                        complete_data_len           = FAN_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = FAN_FRAMES;                                           // only 1 frame
                        auto_repetition_pause_len   = IRSND_FAN_AUTO_REPETITION_PAUSE_LEN;                  // 35 ms pause
                        repeat_frame_pause_len      = IRSND_FAN_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_SPEAKER_PROTOCOL == 1
                    case IRMP_SPEAKER_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SPEAKER_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_SPEAKER_START_BIT_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_SPEAKER_1_PULSE_LEN;
                        pause_1_len                 = IRSND_SPEAKER_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_SPEAKER_0_PULSE_LEN;
                        pause_0_len                 = IRSND_SPEAKER_0_PAUSE_LEN - 1;
                        has_stop_bit                = SPEAKER_STOP_BIT;
                        complete_data_len           = SPEAKER_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = SPEAKER_FRAMES;                                       // 2 frames
                        auto_repetition_pause_len   = IRSND_SPEAKER_AUTO_REPETITION_PAUSE_LEN;              // 35 ms pause
                        repeat_frame_pause_len      = IRSND_SPEAKER_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
                    case IRMP_BANG_OLUFSEN_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_BANG_OLUFSEN_START_BIT1_PULSE_LEN;
                        startbit_pause_len          = IRSND_BANG_OLUFSEN_START_BIT1_PAUSE_LEN - 1;
                        pulse_1_len                 = IRSND_BANG_OLUFSEN_PULSE_LEN;
                        pause_1_len                 = IRSND_BANG_OLUFSEN_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_BANG_OLUFSEN_PULSE_LEN;
                        pause_0_len                 = IRSND_BANG_OLUFSEN_0_PAUSE_LEN - 1;
                        has_stop_bit                = BANG_OLUFSEN_STOP_BIT;
                        complete_data_len           = BANG_OLUFSEN_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_BANG_OLUFSEN_FRAME_REPEAT_PAUSE_LEN;
                        last_bit_value              = 0;
                        irsnd_set_freq (IRSND_FREQ_455_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
                    case IRMP_GRUNDIG_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        startbit_pause_len          = IRSND_GRUNDIG_NOKIA_IR60_PRE_PAUSE_LEN - 1;
                        pulse_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        pause_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        has_stop_bit                = GRUNDIG_NOKIA_IR60_STOP_BIT;
                        complete_data_len           = GRUNDIG_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = GRUNDIG_FRAMES;                                       // 2 frames
                        auto_repetition_pause_len   = IRSND_GRUNDIG_AUTO_REPETITION_PAUSE_LEN;              // 20m sec pause
                        repeat_frame_pause_len      = IRSND_GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_LEN;      // 117 msec pause
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
                    case IRMP_IR60_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        startbit_pause_len          = IRSND_GRUNDIG_NOKIA_IR60_PRE_PAUSE_LEN - 1;
                        pulse_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        pause_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        has_stop_bit                = GRUNDIG_NOKIA_IR60_STOP_BIT;
                        complete_data_len           = IR60_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = IR60_FRAMES;                                          // 2 frames
                        auto_repetition_pause_len   = IRSND_IR60_AUTO_REPETITION_PAUSE_LEN;                 // 20m sec pause
                        repeat_frame_pause_len      = IRSND_GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_LEN;      // 117 msec pause
                        irsnd_set_freq (IRSND_FREQ_30_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
                    case IRMP_NOKIA_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        startbit_pause_len          = IRSND_GRUNDIG_NOKIA_IR60_PRE_PAUSE_LEN - 1;
                        pulse_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        pause_len                   = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                        has_stop_bit                = GRUNDIG_NOKIA_IR60_STOP_BIT;
                        complete_data_len           = NOKIA_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = NOKIA_FRAMES;                                         // 2 frames
                        auto_repetition_pause_len   = IRSND_NOKIA_AUTO_REPETITION_PAUSE_LEN;                // 20 msec pause
                        repeat_frame_pause_len      = IRSND_GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_LEN;      // 117 msec pause
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_SIEMENS_PROTOCOL == 1
                    case IRMP_SIEMENS_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_SIEMENS_BIT_LEN;
                        startbit_pause_len          = IRSND_SIEMENS_BIT_LEN;
                        pulse_len                   = IRSND_SIEMENS_BIT_LEN;
                        pause_len                   = IRSND_SIEMENS_BIT_LEN;
                        has_stop_bit                = SIEMENS_OR_RUWIDO_STOP_BIT;
                        complete_data_len           = SIEMENS_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_SIEMENS_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RUWIDO_PROTOCOL == 1
                    case IRMP_RUWIDO_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RUWIDO_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RUWIDO_START_BIT_PAUSE_LEN;
                        pulse_len                   = IRSND_RUWIDO_BIT_PULSE_LEN;
                        pause_len                   = IRSND_RUWIDO_BIT_PAUSE_LEN;
                        has_stop_bit                = SIEMENS_OR_RUWIDO_STOP_BIT;
                        complete_data_len           = RUWIDO_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RUWIDO_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_36_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
                    case IRMP_FDC_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_FDC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_FDC_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = FDC_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_FDC_PULSE_LEN;
                        pause_1_len                 = IRSND_FDC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_FDC_PULSE_LEN;
                        pause_0_len                 = IRSND_FDC_0_PAUSE_LEN - 1;
                        has_stop_bit                = FDC_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_FDC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_RCCAR_PROTOCOL == 1
                    case IRMP_RCCAR_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_RCCAR_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_RCCAR_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = RCCAR_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_RCCAR_PULSE_LEN;
                        pause_1_len                 = IRSND_RCCAR_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_RCCAR_PULSE_LEN;
                        pause_0_len                 = IRSND_RCCAR_0_PAUSE_LEN - 1;
                        has_stop_bit                = RCCAR_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_RCCAR_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_JVC_PROTOCOL == 1
                    case IRMP_JVC_PROTOCOL:
                    {
                        if (repeat_counter != 0)                                                            // skip start bit if repetition frame
                        {
                            current_bit = 0;
                        }

                        startbit_pulse_len          = IRSND_JVC_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_JVC_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = JVC_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_JVC_PULSE_LEN;
                        pause_1_len                 = IRSND_JVC_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_JVC_PULSE_LEN;
                        pause_0_len                 = IRSND_JVC_0_PAUSE_LEN - 1;
                        has_stop_bit                = JVC_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_JVC_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
                    case IRMP_NIKON_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_NIKON_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_NIKON_START_BIT_PAUSE_LEN;
                        complete_data_len           = NIKON_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_NIKON_PULSE_LEN;
                        pause_1_len                 = IRSND_NIKON_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_NIKON_PULSE_LEN;
                        pause_0_len                 = IRSND_NIKON_0_PAUSE_LEN - 1;
                        has_stop_bit                = NIKON_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_NIKON_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_LEGO_PROTOCOL == 1
                    case IRMP_LEGO_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_LEGO_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_LEGO_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = LEGO_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_LEGO_PULSE_LEN;
                        pause_1_len                 = IRSND_LEGO_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_LEGO_PULSE_LEN;
                        pause_0_len                 = IRSND_LEGO_0_PAUSE_LEN - 1;
                        has_stop_bit                = LEGO_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_LEGO_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_IRMP16_PROTOCOL == 1
                    case IRMP_IRMP16_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_IRMP16_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_IRMP16_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = IRMP16_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_IRMP16_PULSE_LEN;
                        pause_1_len                 = IRSND_IRMP16_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_IRMP16_PULSE_LEN;
                        pause_0_len                 = IRSND_IRMP16_0_PAUSE_LEN - 1;
                        has_stop_bit                = IRMP16_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_IRMP16_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
                    case IRMP_A1TVBOX_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_A1TVBOX_BIT_PULSE_LEN;                          // don't use A1TVBOX_START_BIT_PULSE_LEN
                        startbit_pause_len          = IRSND_A1TVBOX_BIT_PAUSE_LEN;                          // don't use A1TVBOX_START_BIT_PAUSE_LEN
                        pulse_len                   = IRSND_A1TVBOX_BIT_PULSE_LEN;
                        pause_len                   = IRSND_A1TVBOX_BIT_PAUSE_LEN;
                        has_stop_bit                = A1TVBOX_STOP_BIT;
                        complete_data_len           = A1TVBOX_COMPLETE_DATA_LEN + 1;                        // we send stop bit as data
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_A1TVBOX_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_ROOMBA_PROTOCOL == 1
                    case IRMP_ROOMBA_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_ROOMBA_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_ROOMBA_START_BIT_PAUSE_LEN;
                        pulse_1_len                 = IRSND_ROOMBA_1_PULSE_LEN;
                        pause_1_len                 = IRSND_ROOMBA_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_ROOMBA_0_PULSE_LEN;
                        pause_0_len                 = IRSND_ROOMBA_0_PAUSE_LEN - 1;
                        has_stop_bit                = ROOMBA_STOP_BIT;
                        complete_data_len           = ROOMBA_COMPLETE_DATA_LEN;
                        n_auto_repetitions          = ROOMBA_FRAMES;                                        // 8 frames
                        auto_repetition_pause_len   = IRSND_ROOMBA_FRAME_REPEAT_PAUSE_LEN;
                        repeat_frame_pause_len      = IRSND_ROOMBA_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_PENTAX_PROTOCOL == 1
                    case IRMP_PENTAX_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_PENTAX_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_PENTAX_START_BIT_PAUSE_LEN;
                        complete_data_len           = PENTAX_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_PENTAX_PULSE_LEN;
                        pause_1_len                 = IRSND_PENTAX_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_PENTAX_PULSE_LEN;
                        pause_0_len                 = IRSND_PENTAX_0_PAUSE_LEN - 1;
                        has_stop_bit                = PENTAX_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_PENTAX_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
#if IRSND_SUPPORT_ACP24_PROTOCOL == 1
                    case IRMP_ACP24_PROTOCOL:
                    {
                        startbit_pulse_len          = IRSND_ACP24_START_BIT_PULSE_LEN;
                        startbit_pause_len          = IRSND_ACP24_START_BIT_PAUSE_LEN - 1;
                        complete_data_len           = ACP24_COMPLETE_DATA_LEN;
                        pulse_1_len                 = IRSND_ACP24_PULSE_LEN;
                        pause_1_len                 = IRSND_ACP24_1_PAUSE_LEN - 1;
                        pulse_0_len                 = IRSND_ACP24_PULSE_LEN;
                        pause_0_len                 = IRSND_ACP24_0_PAUSE_LEN - 1;
                        has_stop_bit                = ACP24_STOP_BIT;
                        n_auto_repetitions          = 1;                                                    // 1 frame
                        auto_repetition_pause_len   = 0;
                        repeat_frame_pause_len      = IRSND_ACP24_FRAME_REPEAT_PAUSE_LEN;
                        irsnd_set_freq (IRSND_FREQ_38_KHZ);
                        break;
                    }
#endif
                    default:
                    {
                        irsnd_busy = FALSE;
                        break;
                    }
                }
            }
        }

        if (irsnd_busy)
        {
            new_frame = FALSE;

            switch (irsnd_protocol)
            {
#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1
                case IRMP_SIRCS_PROTOCOL:
#endif
#if IRSND_SUPPORT_NEC_PROTOCOL == 1
                case IRMP_NEC_PROTOCOL:
                case IRMP_NEC_REPETITION_PROTOCOL:
                case IRMP_ONKYO_PROTOCOL:
#endif
#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
                case IRMP_NEC16_PROTOCOL:
#endif
#if IRSND_SUPPORT_NEC42_PROTOCOL == 1
                case IRMP_NEC42_PROTOCOL:
#endif
#if IRSND_SUPPORT_MELINERA_PROTOCOL == 1
                case IRMP_MELINERA_PROTOCOL:
#endif
#if IRSND_SUPPORT_LGAIR_PROTOCOL == 1
                case IRMP_LGAIR_PROTOCOL:
#endif
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
                case IRMP_SAMSUNG_PROTOCOL:
                case IRMP_SAMSUNG32_PROTOCOL:
#endif
#if IRSND_SUPPORT_SAMSUNG48_PROTOCOL == 1
                case IRMP_SAMSUNG48_PROTOCOL:
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
                case IRMP_MATSUSHITA_PROTOCOL:
#endif
#if IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1
                case IRMP_TECHNICS_PROTOCOL:
#endif
#if IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1
                case IRMP_KASEIKYO_PROTOCOL:
#endif
#if IRSND_SUPPORT_PANASONIC_PROTOCOL == 1
                case IRMP_PANASONIC_PROTOCOL:
#endif
#if IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
                case IRMP_MITSU_HEAVY_PROTOCOL:
#endif
#if IRSND_SUPPORT_RECS80_PROTOCOL == 1
                case IRMP_RECS80_PROTOCOL:
#endif
#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1
                case IRMP_RECS80EXT_PROTOCOL:
#endif
#if IRSND_SUPPORT_TELEFUNKEN_PROTOCOL == 1
                case IRMP_TELEFUNKEN_PROTOCOL:
#endif
#if IRSND_SUPPORT_DENON_PROTOCOL == 1
                case IRMP_DENON_PROTOCOL:
#endif
#if IRSND_SUPPORT_BOSE_PROTOCOL == 1
                case IRMP_BOSE_PROTOCOL:
#endif
#if IRSND_SUPPORT_NUBERT_PROTOCOL == 1
                case IRMP_NUBERT_PROTOCOL:
#endif
#if IRSND_SUPPORT_FAN_PROTOCOL == 1
                case IRMP_FAN_PROTOCOL:
#endif
#if IRSND_SUPPORT_SPEAKER_PROTOCOL == 1
                case IRMP_SPEAKER_PROTOCOL:
#endif
#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
                case IRMP_BANG_OLUFSEN_PROTOCOL:
#endif
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
                case IRMP_FDC_PROTOCOL:
#endif
#if IRSND_SUPPORT_RCCAR_PROTOCOL == 1
                case IRMP_RCCAR_PROTOCOL:
#endif
#if IRSND_SUPPORT_JVC_PROTOCOL == 1
                case IRMP_JVC_PROTOCOL:
#endif
#if IRSND_SUPPORT_NIKON_PROTOCOL == 1
                case IRMP_NIKON_PROTOCOL:
#endif
#if IRSND_SUPPORT_LEGO_PROTOCOL == 1
                case IRMP_LEGO_PROTOCOL:
#endif
#if IRSND_SUPPORT_IRMP16_PROTOCOL == 1
                case IRMP_IRMP16_PROTOCOL:
#endif
#if IRSND_SUPPORT_THOMSON_PROTOCOL == 1
                case IRMP_THOMSON_PROTOCOL:
#endif
#if IRSND_SUPPORT_ROOMBA_PROTOCOL == 1
                case IRMP_ROOMBA_PROTOCOL:
#endif
#if IRSND_SUPPORT_PENTAX_PROTOCOL == 1
                case IRMP_PENTAX_PROTOCOL:
#endif
#if IRSND_SUPPORT_ACP24_PROTOCOL == 1
                case IRMP_ACP24_PROTOCOL:
#endif

#if IRSND_SUPPORT_SIRCS_PROTOCOL == 1  || IRSND_SUPPORT_NEC_PROTOCOL == 1 || IRSND_SUPPORT_NEC16_PROTOCOL == 1 || IRSND_SUPPORT_NEC42_PROTOCOL == 1 || \
    IRSND_SUPPORT_LGAIR_PROTOCOL == 1 || IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1 || IRSND_SUPPORT_MATSUSHITA_PROTOCOL == 1 || IRSND_SUPPORT_TECHNICS_PROTOCOL == 1 || \
    IRSND_SUPPORT_KASEIKYO_PROTOCOL == 1 || IRSND_SUPPORT_RECS80_PROTOCOL == 1 || IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1 || IRSND_SUPPORT_DENON_PROTOCOL == 1 || \
    IRSND_SUPPORT_NUBERT_PROTOCOL == 1 || IRSND_SUPPORT_FAN_PROTOCOL == 1 || IRSND_SUPPORT_SPEAKER_PROTOCOL == 1 || IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1 || \
    IRSND_SUPPORT_FDC_PROTOCOL == 1 || IRSND_SUPPORT_RCCAR_PROTOCOL == 1 || IRSND_SUPPORT_JVC_PROTOCOL == 1 || IRSND_SUPPORT_NIKON_PROTOCOL == 1 || \
    IRSND_SUPPORT_LEGO_PROTOCOL == 1 || IRSND_SUPPORT_THOMSON_PROTOCOL == 1 || IRSND_SUPPORT_ROOMBA_PROTOCOL == 1 || IRSND_SUPPORT_TELEFUNKEN_PROTOCOL == 1 || \
    IRSND_SUPPORT_PENTAX_PROTOCOL == 1 || IRSND_SUPPORT_ACP24_PROTOCOL == 1 || IRSND_SUPPORT_PANASONIC_PROTOCOL == 1 || IRSND_SUPPORT_BOSE_PROTOCOL == 1 || \
    IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL == 1 || IRSND_SUPPORT_IRMP16_PROTOCOL == 1 || IRSND_SUPPORT_MELINERA_PROTOCOL
                {
                    if (pulse_counter == 0)
                    {
                        if (current_bit == 0xFF)                                                        // send start bit
                        {
                            pulse_len = startbit_pulse_len;
                            pause_len = startbit_pause_len;
                        }
                        else if (current_bit < complete_data_len)                                       // send n'th bit
                        {
#if IRSND_SUPPORT_SAMSUNG_PROTOCOL == 1
                            if (irsnd_protocol == IRMP_SAMSUNG_PROTOCOL)
                            {
                                if (current_bit < SAMSUNG_ADDRESS_LEN)                                  // send address bits
                                {
                                    pulse_len = IRSND_SAMSUNG_PULSE_LEN;
                                    pause_len = (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7)))) ?
                                                    (IRSND_SAMSUNG_1_PAUSE_LEN - 1) : (IRSND_SAMSUNG_0_PAUSE_LEN - 1);
                                }
                                else if (current_bit == SAMSUNG_ADDRESS_LEN)                            // send SYNC bit (16th bit)
                                {
                                    pulse_len = IRSND_SAMSUNG_PULSE_LEN;
                                    pause_len = IRSND_SAMSUNG_START_BIT_PAUSE_LEN - 1;
                                }
                                else if (current_bit < SAMSUNG_COMPLETE_DATA_LEN)                       // send n'th bit
                                {
                                    uint8_t cur_bit = current_bit - 1;                                  // sync skipped, offset = -1 !

                                    pulse_len = IRSND_SAMSUNG_PULSE_LEN;
                                    pause_len = (irsnd_buffer[cur_bit >> 3] & (1<<(7-(cur_bit & 7)))) ?
                                                    (IRSND_SAMSUNG_1_PAUSE_LEN - 1) : (IRSND_SAMSUNG_0_PAUSE_LEN - 1);
                                }
                            }
                            else
#endif

#if IRSND_SUPPORT_NEC16_PROTOCOL == 1
                            if (irsnd_protocol == IRMP_NEC16_PROTOCOL)
                            {
                                if (current_bit < NEC16_ADDRESS_LEN)                                    // send address bits
                                {
                                    pulse_len = IRSND_NEC_PULSE_LEN;
                                    pause_len = (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7)))) ?
                                                    (IRSND_NEC_1_PAUSE_LEN - 1) : (IRSND_NEC_0_PAUSE_LEN - 1);
                                }
                                else if (current_bit == NEC16_ADDRESS_LEN)                              // send SYNC bit (8th bit)
                                {
                                    pulse_len = IRSND_NEC_PULSE_LEN;
                                    pause_len = IRSND_NEC_START_BIT_PAUSE_LEN - 1;
                                }
                                else if (current_bit < NEC16_COMPLETE_DATA_LEN + 1)                     // send n'th bit
                                {
                                    uint8_t cur_bit = current_bit - 1;                                  // sync skipped, offset = -1 !

                                    pulse_len = IRSND_NEC_PULSE_LEN;
                                    pause_len = (irsnd_buffer[cur_bit >> 3] & (1<<(7-(cur_bit & 7)))) ?
                                                    (IRSND_NEC_1_PAUSE_LEN - 1) : (IRSND_NEC_0_PAUSE_LEN - 1);
                                }
                            }
                            else
#endif

#if IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL == 1
                            if (irsnd_protocol == IRMP_BANG_OLUFSEN_PROTOCOL)
                            {
                                if (current_bit == 0)                                                   // send 2nd start bit
                                {
                                    pulse_len = IRSND_BANG_OLUFSEN_START_BIT2_PULSE_LEN;
                                    pause_len = IRSND_BANG_OLUFSEN_START_BIT2_PAUSE_LEN - 1;
                                }
                                else if (current_bit == 1)                                              // send 3rd start bit
                                {
                                    pulse_len = IRSND_BANG_OLUFSEN_START_BIT3_PULSE_LEN;
                                    pause_len = IRSND_BANG_OLUFSEN_START_BIT3_PAUSE_LEN - 1;
                                }
                                else if (current_bit == 2)                                              // send 4th start bit
                                {
                                    pulse_len = IRSND_BANG_OLUFSEN_START_BIT2_PULSE_LEN;
                                    pause_len = IRSND_BANG_OLUFSEN_START_BIT2_PAUSE_LEN - 1;
                                }
                                else if (current_bit == 19)                                             // send trailer bit
                                {
                                    pulse_len = IRSND_BANG_OLUFSEN_PULSE_LEN;
                                    pause_len = IRSND_BANG_OLUFSEN_TRAILER_BIT_PAUSE_LEN - 1;
                                }
                                else if (current_bit < BANG_OLUFSEN_COMPLETE_DATA_LEN)                  // send n'th bit
                                {
                                    uint8_t cur_bit_value = (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7)))) ? 1 : 0;
                                    pulse_len = IRSND_BANG_OLUFSEN_PULSE_LEN;

                                    if (cur_bit_value == last_bit_value)
                                    {
                                        pause_len = IRSND_BANG_OLUFSEN_R_PAUSE_LEN - 1;
                                    }
                                    else
                                    {
                                        pause_len = cur_bit_value ? (IRSND_BANG_OLUFSEN_1_PAUSE_LEN - 1) : (IRSND_BANG_OLUFSEN_0_PAUSE_LEN - 1);
                                        last_bit_value = cur_bit_value;
                                    }
                                }
                            }
                            else
#endif
                            if (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7))))
                            {
                                pulse_len = pulse_1_len;
                                pause_len = pause_1_len;
                            }
                            else
                            {
                                pulse_len = pulse_0_len;
                                pause_len = pause_0_len;
                            }
                        }
                        else if (has_stop_bit)                                                          // send stop bit
                        {
                            pulse_len = pulse_0_len;

                            if (auto_repetition_counter < n_auto_repetitions)
                            {
                                pause_len = pause_0_len;
                            }
                            else
                            {
                                pause_len = 255;                                                        // last frame: pause of 255
                            }
                        }
                    }

                    if (pulse_counter < pulse_len)
                    {
                        if (pulse_counter == 0)
                        {
                            irsnd_on ();
                        }
                        pulse_counter++;
                    }
                    else if (pause_counter < pause_len)
                    {
                        if (pause_counter == 0)
                        {
                            irsnd_off ();
                        }
                        pause_counter++;
                    }
                    else
                    {
                        current_bit++;

                        if (current_bit >= complete_data_len + has_stop_bit)
                        {
                            current_bit = 0xFF;
                            auto_repetition_counter++;

                            if (auto_repetition_counter == n_auto_repetitions)
                            {
                                irsnd_busy = FALSE;
                                auto_repetition_counter = 0;
                            }
                            new_frame = TRUE;
                        }

                        pulse_counter = 0;
                        pause_counter = 0;
                    }
                    break;
                }
#endif

#if IRSND_SUPPORT_RC5_PROTOCOL == 1
                case IRMP_RC5_PROTOCOL:
#endif
#if IRSND_SUPPORT_RC6_PROTOCOL == 1
                case IRMP_RC6_PROTOCOL:
#endif
#if IRSND_SUPPORT_RC6A_PROTOCOL == 1
                case IRMP_RC6A_PROTOCOL:
#endif
#if IRSND_SUPPORT_SIEMENS_PROTOCOL == 1
                case IRMP_SIEMENS_PROTOCOL:
#endif
#if IRSND_SUPPORT_RUWIDO_PROTOCOL == 1
                case IRMP_RUWIDO_PROTOCOL:
#endif
#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1
                case IRMP_GRUNDIG_PROTOCOL:
#endif
#if IRSND_SUPPORT_IR60_PROTOCOL == 1
                case IRMP_IR60_PROTOCOL:
#endif
#if IRSND_SUPPORT_NOKIA_PROTOCOL == 1
                case IRMP_NOKIA_PROTOCOL:
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
                case IRMP_A1TVBOX_PROTOCOL:
#endif

#if IRSND_SUPPORT_RC5_PROTOCOL      == 1 || \
    IRSND_SUPPORT_RC6_PROTOCOL      == 1 || \
    IRSND_SUPPORT_RC6A_PROTOCOL     == 1 || \
    IRSND_SUPPORT_RUWIDO_PROTOCOL   == 1 || \
    IRSND_SUPPORT_SIEMENS_PROTOCOL  == 1 || \
    IRSND_SUPPORT_GRUNDIG_PROTOCOL  == 1 || \
    IRSND_SUPPORT_IR60_PROTOCOL     == 1 || \
    IRSND_SUPPORT_NOKIA_PROTOCOL    == 1 || \
    IRSND_SUPPORT_A1TVBOX_PROTOCOL  == 1
                {
                    if (pulse_counter == pulse_len && pause_counter == pause_len)
                    {
                        current_bit++;

                        if (current_bit >= complete_data_len)
                        {
                            current_bit = 0xFF;

#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1 || IRSND_SUPPORT_IR60_PROTOCOL == 1 || IRSND_SUPPORT_NOKIA_PROTOCOL == 1
                            if (irsnd_protocol == IRMP_GRUNDIG_PROTOCOL || irsnd_protocol == IRMP_IR60_PROTOCOL || irsnd_protocol == IRMP_NOKIA_PROTOCOL)
                            {
                                auto_repetition_counter++;

                                if (repeat_counter > 0)
                                {                                           // set 117 msec pause time
                                    auto_repetition_pause_len = IRSND_GRUNDIG_NOKIA_IR60_FRAME_REPEAT_PAUSE_LEN;
                                }

                                if (repeat_counter < n_repeat_frames)       // tricky: repeat n info frames per auto repetition before sending last stop frame
                                {
                                    n_auto_repetitions++;                   // increment number of auto repetitions
                                    repeat_counter++;
                                }
                                else if (auto_repetition_counter == n_auto_repetitions)
                                {
                                    irsnd_busy = FALSE;
                                    auto_repetition_counter = 0;
                                }
                            }
                            else
#endif
                            {
                                irsnd_busy  = FALSE;
                            }

                            new_frame = TRUE;
                            irsnd_off ();
                        }

                        pulse_counter = 0;
                        pause_counter = 0;
                    }

                    if (! new_frame)
                    {
                        uint8_t first_pulse;

#if IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1 || IRSND_SUPPORT_IR60_PROTOCOL == 1 || IRSND_SUPPORT_NOKIA_PROTOCOL == 1
                        if (irsnd_protocol == IRMP_GRUNDIG_PROTOCOL || irsnd_protocol == IRMP_IR60_PROTOCOL || irsnd_protocol == IRMP_NOKIA_PROTOCOL)
                        {
                            if (current_bit == 0xFF ||                                                                  // start bit of start-frame
                                (irsnd_protocol == IRMP_GRUNDIG_PROTOCOL && current_bit == 15) ||                       // start bit of info-frame (Grundig)
                                (irsnd_protocol == IRMP_IR60_PROTOCOL && current_bit == 7) ||                           // start bit of data frame (IR60)
                                (irsnd_protocol == IRMP_NOKIA_PROTOCOL && (current_bit == 23 || current_bit == 47)))    // start bit of info- or stop-frame (Nokia)
                            {
                                pulse_len = startbit_pulse_len;
                                pause_len = startbit_pause_len;
                                first_pulse = TRUE;
                            }
                            else                                                                                        // send n'th bit
                            {
                                pulse_len = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                                pause_len = IRSND_GRUNDIG_NOKIA_IR60_BIT_LEN;
                                first_pulse = (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7)))) ? TRUE : FALSE;
                            }
                        }
                        else // if (irsnd_protocol == IRMP_RC5_PROTOCOL || irsnd_protocol == IRMP_RC6_PROTOCOL || irsnd_protocol == IRMP_RC6A_PROTOCOL ||
                             //     irsnd_protocol == IRMP_SIEMENS_PROTOCOL || irsnd_protocol == IRMP_RUWIDO_PROTOCOL)
#endif
                        {
                            if (current_bit == 0xFF)                                                                    // 1 start bit
                            {
#if IRSND_SUPPORT_RC6_PROTOCOL == 1 || IRSND_SUPPORT_RC6A_PROTOCOL == 1
                                if (irsnd_protocol == IRMP_RC6_PROTOCOL || irsnd_protocol == IRMP_RC6A_PROTOCOL)
                                {
                                    pulse_len = startbit_pulse_len;
                                    pause_len = startbit_pause_len;
                                }
                                else
#endif
#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1
                                if (irsnd_protocol == IRMP_A1TVBOX_PROTOCOL)
                                {
                                    current_bit = 0;
                                }
                                else
#endif
                                {
                                    ;
                                }

                                first_pulse = TRUE;
                            }
                            else                                                                                        // send n'th bit
                            {
#if IRSND_SUPPORT_RC6_PROTOCOL == 1 || IRSND_SUPPORT_RC6A_PROTOCOL == 1
                                if (irsnd_protocol == IRMP_RC6_PROTOCOL || irsnd_protocol == IRMP_RC6A_PROTOCOL)
                                {
                                    pulse_len = IRSND_RC6_BIT_LEN;
                                    pause_len = IRSND_RC6_BIT_LEN;

                                    if (irsnd_protocol == IRMP_RC6_PROTOCOL)
                                    {
                                        if (current_bit == 4)                                                           // toggle bit (double len)
                                        {
                                            pulse_len = IRSND_RC6_BIT_2_LEN;                                            // = 2 * RC_BIT_LEN
                                            pause_len = IRSND_RC6_BIT_2_LEN;                                            // = 2 * RC_BIT_LEN
                                        }
                                    }
                                    else // if (irsnd_protocol == IRMP_RC6A_PROTOCOL)
                                    {
                                        if (current_bit == 4)                                                           // toggle bit (double len)
                                        {
                                            pulse_len = IRSND_RC6_BIT_3_LEN;                                            // = 3 * RC6_BIT_LEN
                                            pause_len = IRSND_RC6_BIT_2_LEN;                                            // = 2 * RC6_BIT_LEN
                                        }
                                        else if (current_bit == 5)                                                      // toggle bit (double len)
                                        {
                                            pause_len = IRSND_RC6_BIT_2_LEN;                                            // = 2 * RC6_BIT_LEN
                                        }
                                    }
                                }
#endif
                                first_pulse = (irsnd_buffer[current_bit >> 3] & (1<<(7-(current_bit & 7)))) ? TRUE : FALSE;
                            }

                            if (irsnd_protocol == IRMP_RC5_PROTOCOL)
                            {
                                first_pulse = first_pulse ? FALSE : TRUE;
                            }
                        }

                        if (first_pulse)
                        {
                            // printf ("first_pulse: current_bit: %d  %d < %d  %d < %d\n", current_bit, pause_counter, pause_len, pulse_counter, pulse_len);

                            if (pulse_counter < pulse_len)
                            {
                                if (pulse_counter == 0)
                                {
                                    irsnd_on ();
                                }
                                pulse_counter++;
                            }
                            else // if (pause_counter < pause_len)
                            {
                                if (pause_counter == 0)
                                {
                                    irsnd_off ();
                                }
                                pause_counter++;
                            }
                        }
                        else
                        {
                            // printf ("first_pause: current_bit: %d  %d < %d  %d < %d\n", current_bit, pause_counter, pause_len, pulse_counter, pulse_len);

                            if (pause_counter < pause_len)
                            {
                                if (pause_counter == 0)
                                {
                                    irsnd_off ();
                                }
                                pause_counter++;
                            }
                            else // if (pulse_counter < pulse_len)
                            {
                                if (pulse_counter == 0)
                                {
                                    irsnd_on ();
                                }
                                pulse_counter++;
                            }
                        }
                    }
                    break;
                }
#endif // IRSND_SUPPORT_RC5_PROTOCOL == 1 || IRSND_SUPPORT_RC6_PROTOCOL == 1 || || IRSND_SUPPORT_RC6A_PROTOCOL == 1 || IRSND_SUPPORT_SIEMENS_PROTOCOL == 1 ||
       // IRSND_SUPPORT_RUWIDO_PROTOCOL == 1 || IRSND_SUPPORT_GRUNDIG_PROTOCOL == 1 || IRSND_SUPPORT_IR60_PROTOCOL == 1 || IRSND_SUPPORT_NOKIA_PROTOCOL == 1

                default:
                {
                    irsnd_busy = FALSE;
                    break;
                }
            }
        }

        if (! irsnd_busy)
        {
            if (repeat_counter < n_repeat_frames)
            {
#if IRSND_SUPPORT_FDC_PROTOCOL == 1
                if (irsnd_protocol == IRMP_FDC_PROTOCOL)
                {
                    irsnd_buffer[2] |= 0x0F;
                }
#endif
                repeat_counter++;
                irsnd_busy = TRUE;
            }
            else
            {
                irsnd_busy = TRUE; //Rainer
                send_trailer = TRUE;
                n_repeat_frames = 0;
                repeat_counter = 0;
            }
        }
    }

#ifdef ANALYZE
    if (irsnd_is_on)
    {
        putchar ('0');
    }
    else
    {
        putchar ('1');
    }
#endif

    return irsnd_busy;
}

#ifdef ANALYZE

// main function - for unix/linux + windows only!
// AVR: see main.c!
// Compile it under linux with:
// cc irsnd.c -o irsnd
//
// usage: ./irsnd protocol hex-address hex-command >filename

int
main (int argc, char ** argv)
{
    int         protocol;
    int         address;
    int         command;
    IRMP_DATA   irmp_data;

    if (argc != 4 && argc != 5)
    {
        fprintf (stderr, "usage: %s protocol hex-address hex-command [repeat] > filename\n", argv[0]);
        return 1;
    }

    if (sscanf (argv[1], "%d", &protocol) == 1 &&
        sscanf (argv[2], "%x", &address) == 1 &&
        sscanf (argv[3], "%x", &command) == 1)
    {
        irmp_data.protocol = protocol;
        irmp_data.address = address;
        irmp_data.command = command;

        if (argc == 5)
        {
            irmp_data.flags = atoi (argv[4]);
        }
        else
        {
            irmp_data.flags = 0;
        }

        irsnd_init ();

        (void) irsnd_send_data (&irmp_data, TRUE);

        while (irsnd_busy)
        {
            irsnd_ISR ();
        }

        putchar ('\n');

#if 0 // enable here to send twice
        (void) irsnd_send_data (&irmp_data, TRUE);

        while (irsnd_busy)
        {
            irsnd_ISR ();
        }

        putchar ('\n');
#endif
    }
    else
    {
        fprintf (stderr, "%s: wrong arguments\n", argv[0]);
        return 1;
    }
    return 0;
}

#endif // ANALYZE
