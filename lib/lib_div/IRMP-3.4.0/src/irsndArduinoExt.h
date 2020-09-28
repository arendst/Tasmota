/*
 * irsndArduinoExt.h  Arduino extensions to the original irsnd.h
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
#ifndef IRSND_ARDUINO_EXT_H
#define IRSND_ARDUINO_EXT_H

#include "irmpVersion.h"

#include "digitalWriteFast.h" // we use pinModeFast() and digitalReadFast() and digitalWriteFast() in turn

#if defined(IR_OUTPUT_IS_ACTIVE_LOW) || defined(IRSND_GENERATE_NO_SEND_RF)
#define IR_OUTPUT_ACTIVE_LEVEL      LOW
#define IR_OUTPUT_INACTIVE_LEVEL    HIGH
#else
#define IR_OUTPUT_ACTIVE_LEVEL      HIGH
#define IR_OUTPUT_INACTIVE_LEVEL    LOW
#endif
//
/*
 * Set hardware pin defaults for Arduino IDE if no IRSND_OUTPUT_PIN specified
 */
#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
extern uint_fast8_t irsnd_output_pin;

#undef IRSND_OUTPUT_PIN
#define IRSND_OUTPUT_PIN        irsnd_output_pin

void irsnd_init(uint_fast8_t aIrsndOutputPin);
void irsnd_init(uint_fast8_t aIrsndOutputPin, uint_fast8_t aFeedbackLedPin);
void irsnd_init(uint_fast8_t aIrsndOutputPin, uint_fast8_t aFeedbackLedPin, bool aIrmpLedFeedbackPinIsActiveLow);

#else // defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
#  if !defined(IRSND_OUTPUT_PIN)                // Arduino IDE uses IRSND_OUTPUT_PIN instead of PORT and BIT
#define IRSND_OUTPUT_PIN            4
#  endif
#endif // defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)

void irsnd_data_print(Print *aSerial, IRMP_DATA *aIRMPDataPtr);

#if defined(ARDUINO_ARCH_MBED) // Arduino Nano 33 BLE + Sparkfun Apollo3
#include "mbed.h"
#define F_CPU 0 // dummy definition to avoid warning at irsnd.c.h:27 #error F_CPU unkown
#endif

#if !defined(IRMP_TIMING_TEST_PIN)              // Only for test purposes
#define IRMP_TIMING_TEST_PIN        5
#endif

#if ! defined(IRSND_IR_FREQUENCY)
#define IRSND_IR_FREQUENCY          38000
#endif
#define IRSND_INTERRUPT_FREQUENCY   (IRSND_IR_FREQUENCY * 2)  // *2 to toggle output pin at each interrupt

/*
 * For Arduino AVR use timer 2 with FIXED 38000 * 2 = 76000 interrupts per second to toggle output pin.
 * ISR is called each 4. interrupt at a rate of 19000 interrupts per second.
 * (Re)define F_INTERRUPTS to the resulting call value in order to generate correct values for all the protocols.
 */
#  undef  F_INTERRUPTS
#  define F_INTERRUPTS              (IRSND_IR_FREQUENCY / 2)   // 19000 interrupts per second

extern volatile uint8_t irsnd_is_on;                // Used by IRTimer.cpp.h
extern volatile uint8_t irsnd_busy;                 // Used by IRTimer.cpp.h

#endif /* IRSND_ARDUINO_EXT_H */
#endif // ARDUINO
