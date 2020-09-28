 /*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irsnd.h
 *
 * Copyright (c) 2010-2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _IRSND_H_
#define _IRSND_H_

#include "irmpsystem.h"
#ifndef IRSND_USE_AS_LIB
#  include "irsndconfig.h"
#endif

#ifdef ARDUINO
#  include "irsndArduinoExt.h"

#elif defined (ARM_STM32)                         // STM32
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRSND_PORT                            CONCAT(GPIO, IRSND_PORT_LETTER)
#  if defined (ARM_STM32L1XX)
#    define IRSND_PORT_RCC                      CONCAT(RCC_AHBPeriph_GPIO, IRSND_PORT_LETTER)
#    define IRSND_GPIO_AF                       CONCAT(GPIO_AF_TIM, IRSND_TIMER_NUMBER)
#  elif defined (ARM_STM32F10X)
#    define IRSND_PORT_RCC                      CONCAT(RCC_APB2Periph_GPIO, IRSND_PORT_LETTER)
#  elif  defined (ARM_STM32F30X)
#    define IRSND_PORT_RCC                      CONCAT(RCC_AHBPeriph_GPIO, IRSND_PORT_LETTER)
#  elif defined (ARM_STM32F4XX)
#    define IRSND_PORT_RCC                      CONCAT(RCC_AHB1Periph_GPIO, IRSND_PORT_LETTER)
#    define IRSND_GPIO_AF                       CONCAT(GPIO_AF_TIM, IRSND_TIMER_NUMBER)
#  endif
#  define IRSND_BIT                             CONCAT(GPIO_Pin_, IRSND_BIT_NUMBER)
#  define IRSND_TIMER                           CONCAT(TIM, IRSND_TIMER_NUMBER)
#  define IRSND_TIMER_CHANNEL                   CONCAT(TIM_Channel_, IRSND_TIMER_CHANNEL_NUMBER)
#  if ((IRSND_TIMER_NUMBER >= 2) && (IRSND_TIMER_NUMBER <= 5)) || ((IRSND_TIMER_NUMBER >= 12) && (IRSND_TIMER_NUMBER <= 14))
#    define IRSND_TIMER_RCC                     CONCAT(RCC_APB1Periph_TIM, IRSND_TIMER_NUMBER)
#  elif (IRSND_TIMER_NUMBER == 1) || ((IRSND_TIMER_NUMBER >= 8) && (IRSND_TIMER_NUMBER <= 11))
#    define IRSND_TIMER_RCC                     CONCAT(RCC_APB2Periph_TIM, IRSND_TIMER_NUMBER)
#  else
#    error IRSND_TIMER_NUMBER not valid.
#  endif
#  ifndef USE_STDPERIPH_DRIVER
#    warning The STM32 port of IRSND uses the ST standard peripheral drivers which are not enabled in your build configuration.
#  endif

#elif defined(PIC_C18)

# if defined(__12F1840)
    // Do not change lines below unless you have a different HW. This example is for 12F1840
    // setup macro for PWM used PWM module

    //~ #    define PWMon()                         TMR2=0,IRSND_PIN=1
    //~ #    define PWMoff()                        CCP1CON &=(~0b1100)
    //~ #    define PWMon()                         TMR2ON=1
    //~ #    define PWMoff()                        TMR2ON=0
    #if defined(IRSND_DEBUG)
        #define PWMon()                             LATA0=1
        #define PWMoff()                            LATA0=0
        #define IRSND_PIN                           LATA0
    #else
        #    define PWMon()                         TMR2=0,CCP1CON |=0b1100
        #    define PWMoff()                        CCP1CON &=(~0b1100)
        #    define IRSND_PIN                       RA2
    #endif

#else
    // Do not change lines below until you have a different HW. Example is for 18F2550/18F4550
    // setup macro for PWM used PWM module
    #  if IRSND_OCx == IRSND_PIC_CCP2
    #    define PWMon()                             TMR2=0,CCP2CON |=0b1100
    #    define PWMoff()                            CCP2CON &=(~0b1100)
    #    define IRSND_PIN                           TRISCbits.TRISC1        // RC1 = PWM2
    #    define SetDCPWM(x)                         SetDCPWM2(x)
    #    define ClosePWM                            ClosePWM2
    #    define OpenPWM(x)                          OpenPWM2(x)
    #  endif
    #  if IRSND_OCx == IRSND_PIC_CCP1
    #    define PWMon()                             TMR2=0,CCP1CON |=0b1100
    #    define PWMoff()                            CCP1CON &=(~0b1100)
    #    define IRSND_PIN                           TRISCbits.TRISC2        // RC2 = PWM1
    #    define SetDCPWM(x)                         SetDCPWM1(x)
    #    define ClosePWM                            ClosePWM1
    #    define OpenPWM(x)                          OpenPWM1(x)
    # endif
# endif
#  endif // PIC_C18

#if IRSND_SUPPORT_SIEMENS_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, SIEMENS protocol disabled (should be at least 15000)
#  undef IRSND_SUPPORT_SIEMENS_PROTOCOL
#  define IRSND_SUPPORT_SIEMENS_PROTOCOL        0
#endif

#if IRSND_SUPPORT_A1TVBOX_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, A1TVBOX protocol disabled (should be at least 15000)
#  undef IRSND_SUPPORT_A1TVBOX_PROTOCOL
#  define IRSND_SUPPORT_A1TVBOX_PROTOCOL        0
#endif

#if IRSND_SUPPORT_RECS80_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, RECS80 protocol disabled (should be at least 15000)
#  undef IRSND_SUPPORT_RECS80_PROTOCOL
#  define IRSND_SUPPORT_RECS80_PROTOCOL         0
#endif

#if IRSND_SUPPORT_RECS80EXT_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, RECS80EXT protocol disabled (should be at least 15000)
#  undef IRSND_SUPPORT_RECS80EXT_PROTOCOL
#  define IRSND_SUPPORT_RECS80EXT_PROTOCOL      0
#endif

#if IRSND_SUPPORT_LEGO_PROTOCOL == 1 && F_INTERRUPTS < 20000
#  warning F_INTERRUPTS too low, LEGO protocol disabled (should be at least 20000)
#  undef IRSND_SUPPORT_LEGO_PROTOCOL
#  define IRSND_SUPPORT_LEGO_PROTOCOL           0
#endif

#include "irmpprotocols.h"

#define IRSND_NO_REPETITIONS                     0      // no repetitions
#define IRSND_MAX_REPETITIONS                   14      // max # of repetitions
#define IRSND_ENDLESS_REPETITION                15      // endless repetions
#define IRSND_REPETITION_MASK                   0x0F    // lower nibble of flags
#define IRSND_RAW_REPETITION_FRAME              0x10    // send one or more raw repetition frames, yet only used for NEC

#ifdef __cplusplus
extern "C"
{
#endif

extern void                                     irsnd_init (void);
#  ifdef __cplusplus
extern bool                                     irsnd_is_busy (void);
extern bool                                     irsnd_send_data (IRMP_DATA *, uint8_t);
extern bool                                     irsnd_ISR (void);
#else
extern uint8_t                                  irsnd_is_busy (void);
extern uint8_t                                  irsnd_send_data (IRMP_DATA *, uint8_t);
extern uint8_t                                  irsnd_ISR (void);
#endif
extern void                                     irsnd_stop (void);

#if IRSND_USE_CALLBACK == 1
extern void                                     irsnd_set_callback_ptr (void (*cb)(uint8_t));
#endif // IRSND_USE_CALLBACK == 1

#ifdef __cplusplus
}
#endif

#endif /* _IRSND_H_ */
