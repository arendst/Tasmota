/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irsndconfig.h
 *
 * DO NOT INCLUDE THIS FILE, WILL BE INCLUDED BY IRSND.H!
 *
 * Copyright (c) 2010-2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _IRSNDCONFIG_H_
#define _IRSNDCONFIG_H_

#if !defined(_IRSND_H_)
#  error please include only irsnd.h, not irsndconfig.h
#endif

// #define IRSND_DEBUG 1                                // activate debugging

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * F_INTERRUPTS: number of interrupts per second, should be in the range from 10000 to 20000, typically 15000
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef F_INTERRUPTS
#  define F_INTERRUPTS                          15000   // interrupts per second
#endif

#if ! defined(ARDUINO)
/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Change settings from 1 to 0 if you want to disable one or more encoders.
 * This saves program space.
 * 1 enable  encoder
 * 0 disable encoder
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

// typical protocols, disable here!             Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_SIRCS_PROTOCOL            1       // Sony SIRCS           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_NEC_PROTOCOL              1       // NEC + APPLE          >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SAMSUNG_PROTOCOL          1       // Samsung + Samsung32  >= 10000                 ~300 bytes
#define IRSND_SUPPORT_MATSUSHITA_PROTOCOL       1       // Matsushita           >= 10000                 ~200 bytes
#define IRSND_SUPPORT_KASEIKYO_PROTOCOL         1       // Kaseikyo             >= 10000                 ~300 bytes

// more protocols, enable here!                 Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_DENON_PROTOCOL            0       // DENON, Sharp         >= 10000                 ~200 bytes
#define IRSND_SUPPORT_RC5_PROTOCOL              0       // RC5                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RC6_PROTOCOL              0       // RC6                  >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RC6A_PROTOCOL             0       // RC6A                 >= 10000                 ~250 bytes
#define IRSND_SUPPORT_JVC_PROTOCOL              0       // JVC                  >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC16_PROTOCOL            0       // NEC16                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NEC42_PROTOCOL            0       // NEC42                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_IR60_PROTOCOL             0       // IR60 (SDA2008)       >= 10000                 ~250 bytes
#define IRSND_SUPPORT_GRUNDIG_PROTOCOL          0       // Grundig              >= 10000                 ~300 bytes
#define IRSND_SUPPORT_SIEMENS_PROTOCOL          0       // Siemens, Gigaset     >= 15000                 ~150 bytes
#define IRSND_SUPPORT_NOKIA_PROTOCOL            0       // Nokia                >= 10000                 ~400 bytes

// exotic protocols, enable here!               Enable  Remarks                 F_INTERRUPTS            Program Space
#define IRSND_SUPPORT_BOSE_PROTOCOL             0       // BOSE                 >= 10000                 ~100 bytes
#define IRSND_SUPPORT_KATHREIN_PROTOCOL         0       // Kathrein             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_NUBERT_PROTOCOL           0       // NUBERT               >= 10000                 ~100 bytes
#define IRSND_SUPPORT_FAN_PROTOCOL              0       // FAN (ventilator)     >= 10000                 ~100 bytes
#define IRSND_SUPPORT_SPEAKER_PROTOCOL          0       // SPEAKER              >= 10000                 ~100 bytes
#define IRSND_SUPPORT_BANG_OLUFSEN_PROTOCOL     0       // Bang&Olufsen         >= 10000                 ~250 bytes
#define IRSND_SUPPORT_RECS80_PROTOCOL           0       // RECS80               >= 15000                 ~100 bytes
#define IRSND_SUPPORT_RECS80EXT_PROTOCOL        0       // RECS80EXT            >= 15000                 ~100 bytes
#define IRSND_SUPPORT_THOMSON_PROTOCOL          0       // Thomson              >= 10000                 ~250 bytes
#define IRSND_SUPPORT_NIKON_PROTOCOL            0       // NIKON                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_NETBOX_PROTOCOL           0       // Netbox keyboard      >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_ORTEK_PROTOCOL            0       // ORTEK (Hama)         >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_TELEFUNKEN_PROTOCOL       0       // Telefunken 1560      >= 10000                 ~150 bytes
#define IRSND_SUPPORT_FDC_PROTOCOL              0       // FDC IR keyboard      >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_RCCAR_PROTOCOL            0       // RC CAR               >= 10000 (better 15000)  ~150 bytes
#define IRSND_SUPPORT_ROOMBA_PROTOCOL           0       // iRobot Roomba        >= 10000                 ~150 bytes
#define IRSND_SUPPORT_RUWIDO_PROTOCOL           0       // RUWIDO, T-Home       >= 15000                 ~250 bytes
#define IRSND_SUPPORT_A1TVBOX_PROTOCOL          0       // A1 TV BOX            >= 15000 (better 20000)  ~200 bytes
#define IRSND_SUPPORT_LEGO_PROTOCOL             0       // LEGO Power RC        >= 20000                 ~150 bytes
#define IRSND_SUPPORT_RCMM_PROTOCOL             0       // RCMM 12,24, or 32    >= 20000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_LGAIR_PROTOCOL            0       // LG Air Condition     >= 10000                 ~150 bytes.
#define IRSND_SUPPORT_SAMSUNG48_PROTOCOL        0       // Samsung48            >= 10000                 ~100 bytes
#define IRSND_SUPPORT_PENTAX_PROTOCOL           0       // Pentax               >= 10000                 ~150 bytes
#define IRSND_SUPPORT_S100_PROTOCOL             0       // S100                 >= 10000                 ~150 bytes
#define IRSND_SUPPORT_ACP24_PROTOCOL            0       // ACP24                >= 10000                 ~150 bytes
#define IRSND_SUPPORT_TECHNICS_PROTOCOL         0       // TECHNICS             >= 10000                 DON'T CHANGE, NOT SUPPORTED YET!
#define IRSND_SUPPORT_PANASONIC_PROTOCOL        0       // PANASONIC Beamer     >= 10000                 ~150 bytes
#define IRSND_SUPPORT_MITSU_HEAVY_PROTOCOL      0       // Mitsubishi-Heavy Aircondition, similar Timing to Panasonic beamer
#define IRSND_SUPPORT_IRMP16_PROTOCOL           0       // IRMP specific        >= 15000                 ~250 bytes
#endif // ! defined(ARDUINO)

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * AVR XMega section:
 *
 * Change hardware pin here:                    IRSND_XMEGA_OC0A = OC0A on ATxmegas  supporting OC0A, e.g. ATxmega128A1U
 *                                              IRSND_XMEGA_OC0B = OC0B on ATxmegas  supporting OC0B, e.g. ATxmega128A1U
 *                                              IRSND_XMEGA_OC0C = OC0C on ATxmegas  supporting OC0C, e.g. ATxmega128A1U
 *                                              IRSND_XMEGA_OC0D = OC0D on ATxmegas  supporting OC0D, e.g. ATxmega128A1U
 *                                              IRSND_XMEGA_OC1A = OC1A on ATxmegas  supporting OC1A, e.g. ATxmega128A1U
 *                                              IRSND_XMEGA_OC1B = OC1B on ATxmegas  supporting OC1B, e.g. ATxmega128A1U
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined(__AVR_XMEGA__)                                              // XMEGA
#  define IRSND_PORT_PRE                        PORTD
#  define XMEGA_Timer                           TCD0
#  define IRSND_OCx                             IRSND_XMEGA_OC0B        // use OC0B

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * AVR ATmega/ATTiny section:
 *
 * Change hardware pin here:                    IRSND_OC2  = OC2  on ATmegas         supporting OC2,  e.g. ATmega8
 *                                              IRSND_OC2A = OC2A on ATmegas         supporting OC2A, e.g. ATmega88
 *                                              IRSND_OC2B = OC2B on ATmegas         supporting OC2B, e.g. ATmega88
 *                                              IRSND_OC0  = OC0  on ATmegas         supporting OC0,  e.g. ATmega162
 *                                              IRSND_OC0A = OC0A on ATmegas/ATtinys supporting OC0A, e.g. ATtiny84, ATtiny85, ATtiny87/167
 *                                              IRSND_OC0B = OC0B on ATmegas/ATtinys supporting OC0B, e.g. ATtiny84, ATtiny85
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined(ATMEL_AVR)
#  define IRSND_OCx                             IRSND_OC2B              // use OC2B

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * PIC C18 or XC8 section:
 *
 * Change hardware pin here:                    IRSND_PIC_CCP1 = RC2 on PIC 18F2550/18F4550, ...
 *                                              IRSND_PIC_CCP2 = RC1 on PIC 18F2550/18F4550, ...
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined(PIC_C18)                                                  // C18 or XC8 compiler
#  if defined(__12F1840)                                                // XC8 compiler
#    define Pre_Scaler                          1                       // define prescaler for timer2 e.g. 1,4,16
#    define F_CPU                               32000000UL              // PIC frequency: set your freq here
#    define PIC_Scaler                          2                       // PIC needs /2 extra in IRSND_FREQ_32_KHZ calculation for right value

#  else                                                                 // C18 compiler
#    define IRSND_OCx                           IRSND_PIC_CCP2          // Use PWMx for PIC
                                                                        // change other PIC C18 specific settings:
#    define F_CPU                               48000000UL              // PIC frequency: set your freq here
#    define Pre_Scaler                          4                       // define prescaler for timer2 e.g. 1,4,16
#    define PIC_Scaler                          2                       // PIC needs /2 extra in IRSND_FREQ_32_KHZ calculation for right value
#  endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * ARM STM32 section:
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined (ARM_STM32)                                               // use B6 as IR output on STM32
#  define IRSND_PORT_LETTER                     B
#  define IRSND_BIT_NUMBER                      6
#  define IRSND_TIMER_NUMBER                    4
#  define IRSND_TIMER_CHANNEL_NUMBER            1                       // only channel 1 can be used at the moment, others won't work

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * ARM STM32 with HAL section - don't change here, define IRSND_Transmit_GPIO_Port & IRSND_Transmit_Pin in STM32Cube (Main.h)
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined (ARM_STM32_HAL)                                           // IRSND_Transmit_GPIO_Port & IRSND_Transmit_Pin must be defined in STM32Cube
#  define IRSND_PORT_LETTER                     IRSND_Transmit_GPIO_Port//Port of Transmit PWM Pin e.g.
#  define IRSND_BIT_NUMBER                      IRSND_Transmit_Pin      //Pim of Transmit PWM Pin e.g.
#  define IRSND_TIMER_HANDLER                   htim2                   //Handler of Timer e.g. htim (see tim.h)
#  define IRSND_TIMER_CHANNEL_NUMBER            TIM_CHANNEL_2           //Channel of the used Timer PWM Pin e.g. TIM_CHANNEL_2
#  define IRSND_TIMER_SPEED_APBX                64000000                //Speed of the corresponding APBx. (see STM32CubeMX: Clock Configuration)

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Teensy 3.x with teensyduino gcc compiler
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined (TEENSY_ARM_CORTEX_M4)
#  define IRSND_PIN                             5                       // choose an arduino pin with PWM function!

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * ESP8266 (Arduino, see IRSEND.ino)
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif defined (__xtensa__)
#  define IRSND_PIN                             0                       // choose an arduino pin with PWM function!

#elif defined(ARDUINO)
// specified here to avoid else case

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Other target systems
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#elif !defined (UNIX_OR_WINDOWS)
#  error target system not defined.
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Use Callbacks to indicate output signal or something else
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#ifndef IRSND_USE_CALLBACK
#  define IRSND_USE_CALLBACK                    0                       // flag: 0 = don't use callbacks, 1 = use callbacks, default is 0
#endif

#endif // _IRSNDCONFIG_H_
