/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmp.h
 *
 * Copyright (c) 2009-2020 Frank Meyer - frank(at)fli4l.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#ifndef _IRMP_H_
#define _IRMP_H_

#ifndef IRMP_USE_AS_LIB
#  define IRMPCONFIG_STAGE1_H
#  include "irmpconfig.h"
#  undef IRMPCONFIG_STAGE1_H
#endif

#include "irmpsystem.h"

#ifndef IRMP_USE_AS_LIB
#  define IRMPCONFIG_STAGE2_H
#  include "irmpconfig.h"
#  undef IRMPCONFIG_STAGE2_H
#endif

#ifdef ARDUINO
#  include "irmpArduinoExt.h"

#elif defined (__AVR_XMEGA__)
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRMP_PORT_PRE                         CONCAT(PORT, IRMP_PORT_LETTER)
#  define IRMP_DDR_PRE                          CONCAT(PORT, IRMP_PORT_LETTER)
#  define IRMP_PIN_PRE                          CONCAT(PORT, IRMP_PORT_LETTER)
#  define IRMP_PORT                             IRMP_PORT_PRE.OUT
#  define IRMP_DDR                              IRMP_DDR_PRE.DIR
#  define IRMP_PIN                              IRMP_PIN_PRE.IN
#  define IRMP_BIT                              IRMP_BIT_NUMBER
#  define input(x)                              ((x) & (1 << IRMP_BIT))

#elif defined (ATMEL_AVR)
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRMP_PORT                             CONCAT(PORT, IRMP_PORT_LETTER)
#  define IRMP_DDR                              CONCAT(DDR, IRMP_PORT_LETTER)
#  define IRMP_PIN                              CONCAT(PIN, IRMP_PORT_LETTER)
#  define IRMP_BIT                              IRMP_BIT_NUMBER
#  define input(x)                              ((x) & (1 << IRMP_BIT))

#elif defined (PIC_C18) || defined (PIC_CCS) || defined(PIC_XC32)
#  define input(x)                              (x)

#elif defined (ARM_STM32)
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRMP_PORT                             CONCAT(GPIO, IRMP_PORT_LETTER)
#  if defined (ARM_STM32L1XX)
#    define IRMP_PORT_RCC                       CONCAT(RCC_AHBPeriph_GPIO, IRMP_PORT_LETTER)
#  elif defined (ARM_STM32F10X)
#    define IRMP_PORT_RCC                       CONCAT(RCC_APB2Periph_GPIO, IRMP_PORT_LETTER)
#  elif defined (ARM_STM32F30X)
#    define IRMP_PORT_RCC                       CONCAT(RCC_AHBPeriph_GPIO, IRMP_PORT_LETTER)
#  elif defined (ARM_STM32F4XX)
#    define IRMP_PORT_RCC                       CONCAT(RCC_AHB1Periph_GPIO, IRMP_PORT_LETTER)
#  endif
#  define IRMP_BIT                              CONCAT(GPIO_Pin_, IRMP_BIT_NUMBER)
#  define IRMP_PIN                              IRMP_PORT   // for use with input(x) below
#  define input(x)                              (GPIO_ReadInputDataBit(x, IRMP_BIT))
#  ifndef USE_STDPERIPH_DRIVER
#    warning The STM32 port of IRMP uses the ST standard peripheral drivers which are not enabled in your build configuration.
#  endif

#elif defined (ARM_STM32_HAL)
#  define IRMP_BIT                              IRMP_BIT_NUMBER
#  define IRMP_PIN                              IRMP_BIT_NUMBER   // for use with input(x) below
#  define input(x)                              HAL_GPIO_ReadPin(IRMP_PORT_LETTER, x)

#elif defined (STELLARIS_ARM_CORTEX_M4)
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRMP_PORT_PERIPH                      CONCAT(SYSCTL_PERIPH_GPIO, IRMP_PORT_LETTER)
#  define IRMP_PORT_BASE                        CONCAT(GPIO_PORT, CONCAT(IRMP_PORT_LETTER, _BASE))
#  define IRMP_PORT_PIN                         CONCAT(GPIO_PIN_, IRMP_BIT_NUMBER)
#  define IRMP_PIN                              IRMP_PORT_PIN
#  define input(x)                              ((uint8_t)(ROM_GPIOPinRead(IRMP_PORT_BASE, IRMP_PORT_PIN)))
#  define sei()                                 IntMasterEnable()

#elif defined(__SDCC_stm8)
#  define _CONCAT(a,b)                          a##b
#  define CONCAT(a,b)                           _CONCAT(a,b)
#  define IRMP_GPIO_STRUCT                      CONCAT(GPIO, IRMP_PORT_LETTER)
#  define IRMP_BIT                              IRMP_BIT_NUMBER
#  define IRMP_PIN                              IRMP_GPIO_STRUCT->IDR
#  define input(x)                              ((x) & (1 << IRMP_BIT))

#elif defined (TEENSY_ARM_CORTEX_M4)
#  define input(x)                              ((uint8_t)(digitalReadFast(x)))

#elif defined (__MBED__)
#  define IRMP_BIT                              gpioIRin
#  define input(x)                              (gpio_read (&x))

#elif defined(__xtensa__)
#  define IRMP_BIT                              IRMP_BIT_NUMBER
#  define input(x)                              GPIO_INPUT_GET(IRMP_BIT_NUMBER)

#elif defined(_CHIBIOS_HAL_)
#  define input(x)                              palReadLine(x)

#endif

#if IRMP_USE_IDLE_CALL == 1
void irmp_idle(void);                   // the user has to provide an implementation of the irmp_idle() function and link it
#endif

#if IRMP_USE_COMPLETE_CALLBACK == 1
void irmp_register_complete_callback_function(void (*aCompleteCallbackFunction)(void));
#endif

#if IRMP_SUPPORT_TECHNICS_PROTOCOL == 1
#  undef IRMP_SUPPORT_MATSUSHITA_PROTOCOL
#  define IRMP_SUPPORT_MATSUSHITA_PROTOCOL      1
#endif

#if IRMP_32_BIT == 0 && IRMP_SUPPORT_MERLIN_PROTOCOL == 1
#  undef IRMP_SUPPORT_MERLIN_PROTOCOL
#  warning MERLIN protocol disabled, IRMP_32_BIT=1 needed
#endif

#if IRMP_SUPPORT_DENON_PROTOCOL == 1 && IRMP_SUPPORT_RUWIDO_PROTOCOL == 1
#  warning DENON protocol conflicts wih RUWIDO, please enable only one of both protocols
#  warning RUWIDO protocol disabled
#  undef IRMP_SUPPORT_RUWIDO_PROTOCOL
#  define IRMP_SUPPORT_RUWIDO_PROTOCOL          0
#endif

#if IRMP_SUPPORT_KASEIKYO_PROTOCOL == 1 && IRMP_SUPPORT_PANASONIC_PROTOCOL == 1
#  warning KASEIKYO protocol conflicts wih PANASONIC, please enable only one of both protocols
#  warning PANASONIC protocol disabled
#  undef IRMP_SUPPORT_PANASONIC_PROTOCOL
#  define IRMP_SUPPORT_PANASONIC_PROTOCOL       0
#endif

#if IRMP_SUPPORT_DENON_PROTOCOL == 1 && IRMP_SUPPORT_ACP24_PROTOCOL == 1
#  warning DENON protocol conflicts wih ACP24, please enable only one of both protocols
#  warning ACP24 protocol disabled
#  undef IRMP_SUPPORT_ACP24_PROTOCOL
#  define IRMP_SUPPORT_ACP24_PROTOCOL           0
#endif

#if IRMP_SUPPORT_RC6_PROTOCOL == 1 && IRMP_SUPPORT_ROOMBA_PROTOCOL == 1
#  warning RC6 protocol conflicts wih ROOMBA, please enable only one of both protocols
#  warning ROOMBA protocol disabled
#  undef IRMP_SUPPORT_ROOMBA_PROTOCOL
#  define IRMP_SUPPORT_ROOMBA_PROTOCOL          0
#endif

#if IRMP_SUPPORT_PANASONIC_PROTOCOL == 1 && IRMP_SUPPORT_MITSU_HEAVY_PROTOCOL == 1
#  warning PANASONIC protocol conflicts wih MITSU_HEAVY, please enable only one of both protocols
#  warning MITSU_HEAVY protocol disabled
#  undef IRMP_SUPPORT_MITSU_HEAVY_PROTOCOL
#  define IRMP_SUPPORT_MITSU_HEAVY_PROTOCOL      0
#endif

#if IRMP_SUPPORT_RC5_PROTOCOL == 1 && IRMP_SUPPORT_ORTEK_PROTOCOL == 1
#  warning RC5 protocol conflicts wih ORTEK, please enable only one of both protocols
#  warning ORTEK protocol disabled
#  undef IRMP_SUPPORT_ORTEK_PROTOCOL
#  define IRMP_SUPPORT_ORTEK_PROTOCOL           0
#endif

#if IRMP_SUPPORT_RC5_PROTOCOL == 1 && IRMP_SUPPORT_S100_PROTOCOL == 1
#  warning RC5 protocol conflicts wih S100, please enable only one of both protocols
#  warning S100 protocol disabled
#  undef IRMP_SUPPORT_S100_PROTOCOL
#  define IRMP_SUPPORT_S100_PROTOCOL            0
#endif

#if IRMP_SUPPORT_NUBERT_PROTOCOL == 1 && IRMP_SUPPORT_FAN_PROTOCOL == 1
#  warning NUBERT protocol conflicts wih FAN, please enable only one of both protocols
#  warning FAN protocol disabled
#  undef IRMP_SUPPORT_FAN_PROTOCOL
#  define IRMP_SUPPORT_FAN_PROTOCOL             0
#endif

#if IRMP_SUPPORT_FDC_PROTOCOL == 1 && IRMP_SUPPORT_ORTEK_PROTOCOL == 1
#  warning FDC protocol conflicts wih ORTEK, please enable only one of both protocols
#  warning ORTEK protocol disabled
#  undef IRMP_SUPPORT_ORTEK_PROTOCOL
#  define IRMP_SUPPORT_ORTEK_PROTOCOL           0
#endif

#if IRMP_SUPPORT_ORTEK_PROTOCOL == 1 && IRMP_SUPPORT_NETBOX_PROTOCOL == 1
#  warning ORTEK protocol conflicts wih NETBOX, please enable only one of both protocols
#  warning NETBOX protocol disabled
#  undef IRMP_SUPPORT_NETBOX_PROTOCOL
#  define IRMP_SUPPORT_NETBOX_PROTOCOL          0
#endif

#if IRMP_SUPPORT_GRUNDIG_PROTOCOL == 1 && IRMP_SUPPORT_RCII_PROTOCOL == 1
#  warning GRUNDIG protocol conflicts wih RCII, please enable only one of both protocols
#  warning RCII protocol disabled
#  undef IRMP_SUPPORT_RCII_PROTOCOL
#  define IRMP_SUPPORT_RCII_PROTOCOL          0
#endif

#if IRMP_SUPPORT_NOKIA_PROTOCOL == 1 && IRMP_SUPPORT_RCII_PROTOCOL == 1
#  warning NOKIA protocol conflicts wih RCII, please enable only one of both protocols
#  warning RCII protocol disabled
#  undef IRMP_SUPPORT_RCII_PROTOCOL
#  define IRMP_SUPPORT_RCII_PROTOCOL          0
#endif

#if IRMP_SUPPORT_SIEMENS_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, SIEMENS protocol disabled (should be at least 15000)
#  undef IRMP_SUPPORT_SIEMENS_PROTOCOL
#  define IRMP_SUPPORT_SIEMENS_PROTOCOL         0
#endif

#if IRMP_SUPPORT_RUWIDO_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, RUWIDO protocol disabled (should be at least 15000)
#  undef IRMP_SUPPORT_RUWIDO_PROTOCOL
#  define IRMP_SUPPORT_RUWIDO_PROTOCOL          0
#endif

#if IRMP_SUPPORT_RECS80_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, RECS80 protocol disabled (should be at least 15000)
#  undef IRMP_SUPPORT_RECS80_PROTOCOL
#  define IRMP_SUPPORT_RECS80_PROTOCOL          0
#endif

#if IRMP_SUPPORT_RECS80EXT_PROTOCOL == 1 && F_INTERRUPTS < 15000
#  warning F_INTERRUPTS too low, RECS80EXT protocol disabled (should be at least 15000)
#  undef IRMP_SUPPORT_RECS80EXT_PROTOCOL
#  define IRMP_SUPPORT_RECS80EXT_PROTOCOL       0
#endif

#if IRMP_SUPPORT_LEGO_PROTOCOL == 1 && F_INTERRUPTS < 20000
#  warning F_INTERRUPTS too low, LEGO protocol disabled (should be at least 20000)
#  undef IRMP_SUPPORT_LEGO_PROTOCOL
#  define IRMP_SUPPORT_LEGO_PROTOCOL            0
#endif

#if IRMP_SUPPORT_SAMSUNG48_PROTOCOL == 1 && IRMP_SUPPORT_SAMSUNG_PROTOCOL == 0
#  warning SAMSUNG48 protocol needs also SAMSUNG protocol, SAMSUNG protocol enabled
#  undef IRMP_SUPPORT_SAMSUNG_PROTOCOL
#  define IRMP_SUPPORT_SAMSUNG_PROTOCOL         1
#endif

#if IRMP_SUPPORT_JVC_PROTOCOL == 1 && IRMP_SUPPORT_NEC_PROTOCOL == 0
#  warning JVC protocol needs also NEC protocol, NEC protocol enabled
#  undef IRMP_SUPPORT_NEC_PROTOCOL
#  define IRMP_SUPPORT_NEC_PROTOCOL             1
#endif

#if IRMP_SUPPORT_NEC16_PROTOCOL == 1 && IRMP_SUPPORT_NEC_PROTOCOL == 0
#  warning NEC16 protocol needs also NEC protocol, NEC protocol enabled
#  undef IRMP_SUPPORT_NEC_PROTOCOL
#  define IRMP_SUPPORT_NEC_PROTOCOL             1
#endif

#if IRMP_SUPPORT_NEC42_PROTOCOL == 1 && IRMP_SUPPORT_NEC_PROTOCOL == 0
#  warning NEC42 protocol needs also NEC protocol, NEC protocol enabled
#  undef IRMP_SUPPORT_NEC_PROTOCOL
#  define IRMP_SUPPORT_NEC_PROTOCOL             1
#endif

#if IRMP_SUPPORT_LGAIR_PROTOCOL == 1 && IRMP_SUPPORT_NEC_PROTOCOL == 0
#  warning LGAIR protocol needs also NEC protocol, NEC protocol enabled
#  undef IRMP_SUPPORT_NEC_PROTOCOL
#  define IRMP_SUPPORT_NEC_PROTOCOL             1
#endif

#if IRMP_SUPPORT_MELINERA_PROTOCOL == 1 && IRMP_SUPPORT_NEC_PROTOCOL == 0
#  warning MELINERA protocol needs also NEC protocol, NEC protocol enabled
#  undef IRMP_SUPPORT_NEC_PROTOCOL
#  define IRMP_SUPPORT_NEC_PROTOCOL             1
#endif

#if IRMP_SUPPORT_RCMM_PROTOCOL == 1 && F_INTERRUPTS < 20000
#  warning F_INTERRUPTS too low, RCMM protocol disabled (should be at least 20000)
#  undef IRMP_SUPPORT_RCMM_PROTOCOL
#  define IRMP_SUPPORT_RCMM_PROTOCOL            0
#endif

#if IRMP_SUPPORT_PENTAX_PROTOCOL == 1 && F_INTERRUPTS > 17000 && __SIZEOF_INT__ < 4
#  warning F_INTERRUPTS too high, PENTAX protocol disabled (should be max 17000)
#  undef IRMP_SUPPORT_PENTAX_PROTOCOL
#  define IRMP_SUPPORT_PENTAX_PROTOCOL          0
#endif

#if IRMP_SUPPORT_GREE_PROTOCOL == 1 && F_INTERRUPTS > 17000 && __SIZEOF_INT__ < 4
#  warning F_INTERRUPTS too high, GREE protocol disabled (should be max 17000)
#  undef IRMP_SUPPORT_GREE_PROTOCOL
#  define IRMP_SUPPORT_GREE_PROTOCOL            0
#endif

#if F_INTERRUPTS > 20000
#error F_INTERRUPTS too high (should be not greater than 20000)
#endif

#include "irmpprotocols.h"

#define IRMP_FLAG_REPETITION            0x01
#define IRMP_FLAG_RELEASE               0x02                                    // see IRMP_ENABLE_RELEASE_DETECTION in irmpconfig.h

#ifdef __cplusplus
extern "C"
{
#endif

extern void                             irmp_init (void);
#ifdef __cplusplus
extern bool                             irmp_get_data (IRMP_DATA *);
extern bool                             irmp_ISR (void);
#else
extern uint_fast8_t                     irmp_get_data (IRMP_DATA *);
extern uint_fast8_t                     irmp_ISR (void);
#endif

#if IRMP_PROTOCOL_NAMES == 1
extern const char * const               irmp_protocol_names[IRMP_N_PROTOCOLS + 1] PROGMEM;
#endif

#if IRMP_USE_CALLBACK == 1
extern void                             irmp_set_callback_ptr (void (*cb)(uint_fast8_t));
#endif // IRMP_USE_CALLBACK == 1

#ifdef __cplusplus
}
#endif

#endif /* _IRMP_H_ */
