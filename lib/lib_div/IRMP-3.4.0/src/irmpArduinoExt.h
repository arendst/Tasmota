/*
 * irmpArduinoExt.h  Arduino extensions to the original irmp.h
 *
 *  Copyright (C) 2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of IRMP https://github.com/ukw100/IRMP.
 *
 *  IRMP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

#if defined(ARDUINO)
#ifndef IRMP_ARDUINO_EXT_H
#define IRMP_ARDUINO_EXT_H

#include <Arduino.h>  // for Print

#include "irmpVersion.h"
#include "IRFeedbackLed.h" // for redefinition of

#include "digitalWriteFast.h" // we use pinModeFast() and digitalReadFast() and digitalWriteFast() in turn

/*
 * For debugging purposes. The timing test pin for some platforms is specified in the PinDefinitionsAndMore.h files included in each example.
 */
//#define IRMP_MEASURE_TIMING
//#define IRMP_TIMING_TEST_PIN <yourPinNumber>
//
/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Enable dynamic pin configuration in contrast to the static one which is known at compile time and saves program memory and CPU cycles.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
//#define IRMP_IRSND_ALLOW_DYNAMIC_PINS
//
//
/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Enable PinChangeInterrupt add on for irmp_ISR(). Tested for NEC, Kaseiko, Denon, RC6 protocols and Arduino Uno and Arduino ATmega.
 * Receives IR protocol data  by using pin change interrupts and no polling by timer.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
//#define IRMP_ENABLE_PIN_CHANGE_INTERRUPT
#if defined(IRMP_ENABLE_PIN_CHANGE_INTERRUPT)
#  if ! (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)) /* ATtinyX5 */ \
&& ! ( (defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)) && ( (defined(ARDUINO_AVR_DIGISPARKPRO) && ((IRMP_INPUT_PIN == 3) || (IRMP_INPUT_PIN == 9))) /*ATtinyX7(digisparkpro) and pin 3 or 9 */\
        || (! defined(ARDUINO_AVR_DIGISPARKPRO) && ((IRMP_INPUT_PIN == 3) || (IRMP_INPUT_PIN == 14)))) ) /*ATtinyX7(ATTinyCore) and pin 3 or 14 */ \
&& ! ( ( defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) \
        || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) \
        || defined(__AVR_ATmega8__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PB__) \
        || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)) \
&& ((IRMP_INPUT_PIN == 2) || (IRMP_INPUT_PIN == 3)) ) /* ATmegas and pin 2 or 3 */
#  define IRMP_USE_ARDUINO_ATTACH_INTERRUPT // cannot use any static ISR vector here
#  endif

#  undef F_INTERRUPTS
#  define F_INTERRUPTS                          15625   // 15625 interrupts per second gives 64 us period
#endif

#if defined(__AVR__)
#define uint_fast8_t uint8_t
#define uint_fast16_t uint16_t

#elif defined(ESP8266)
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "c_types.h"

#elif defined(ARDUINO_ARCH_MBED) // Arduino Nano 33 BLE + Sparkfun Apollo3
#include "mbed.h"

#elif defined(ESP32)
#elif defined(STM32F1xx) // for "Generic STM32F1 series" from "STM32 Boards (selected from submenu)" of Arduino Board manager
#elif defined(ARDUINO_ARCH_STM32) // Untested! use settings from BluePill / STM32F1xx
#elif defined(__STM32F1__) // for "Generic STM32F103C series" from "STM32F1 Boards (STM32duino.com)" of Arduino Board manager
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * Change hardware pin here for Arduino IDE if no IRMP_INPUT_PIN specified
 * Should be first, since it covers multiple platforms
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
extern uint_fast8_t irmp_InputPin; // global variable to hold input pin number. Is referenced by defining IRMP_INPUT_PIN as irmp_InputPin.

#undef IRMP_INPUT_PIN
#define IRMP_INPUT_PIN              irmp_InputPin
#else // defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
#  if !defined(IRMP_INPUT_PIN)                                       // Arduino IDE uses IRMP_INPUT_PIN instead of PORT and BIT
#define IRMP_INPUT_PIN              3
#  endif
#endif

#if defined(IRMP_INPUT_PIN)
#  if defined(__AVR__)
#    define input(x)                digitalReadFast(IRMP_INPUT_PIN)
#  else
#    define input(x)                digitalRead(IRMP_INPUT_PIN)
#  endif
#endif

#if !defined(IRMP_TIMING_TEST_PIN)              // Only for test purposes
#define IRMP_TIMING_TEST_PIN        5
#endif

void irmp_init(uint_fast8_t aIrmpInputPin);
void irmp_init(uint_fast8_t aIrmpInputPin, uint_fast8_t aIrmpFeedbackLedPin);
void irmp_init(uint_fast8_t aIrmpInputPin, uint_fast8_t aIrmpFeedbackLedPin, bool aIrmpLedFeedbackPinIsActiveLow);

bool irmp_IsBusy();

void irmp_tone(uint8_t _pin, unsigned int frequency, unsigned long duration);

extern uint32_t irmp_last_change_micros;

void irmp_result_print(Print *aSerial, IRMP_DATA *aIRMPDataPtr);
void irmp_result_print(IRMP_DATA *aIRMPDataPtr);

void irmp_PCI_ISR(void);
void initPCIInterrupt(void);

void irmp_print_active_protocols(Print *aSerial);
void irmp_print_protocol_name(Print *aSerial, uint8_t aProtocolNumber);

extern const uint8_t irmp_used_protocol_index[] PROGMEM;
extern const char *const irmp_used_protocol_names[] PROGMEM;

#endif // IRMP_ARDUINO_EXT_H
#endif // ARDUINO
