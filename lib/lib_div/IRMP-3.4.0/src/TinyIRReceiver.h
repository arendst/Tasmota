/*
 *  TinyIRReceiver.h
 *
 *
 *  Copyright (C) 2021  Armin Joachimsmeyer
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

#ifndef TINY_IR_RECEIVER_H
#define TINY_IR_RECEIVER_H

#include <Arduino.h>

#include "LongUnion.h"

/*
 * Set input pin and output pin definitions etc.
 */
#if !defined(IR_INPUT_PIN)
#if defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__)
#warning "IR_INPUT_PIN is not defined, we set it to 10"
#define IR_INPUT_PIN    10
#else
#warning "IR_INPUT_PIN is not defined, we set it to 2"
#define IR_INPUT_PIN    2
#endif
#endif

#if !defined(IR_FEEDBACK_LED_PIN) && defined(LED_BUILTIN)
#define IR_FEEDBACK_LED_PIN    LED_BUILTIN
#endif

//#define DO_NOT_USE_FEEDBACK_LED // Activate it if you do not want the feedback LED function, saving only 2 bytes code and 2 clock cycles per interrupt.

#if ! (defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)) /* ATtinyX5 */ \
&& ! ( (defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)) && ( (defined(ARDUINO_AVR_DIGISPARKPRO) && ((IR_INPUT_PIN == 3) || (IR_INPUT_PIN == 9))) /*ATtinyX7(digisparkpro) and pin 3 or 9 */\
        || (! defined(ARDUINO_AVR_DIGISPARKPRO) && ((IR_INPUT_PIN == 3) || (IR_INPUT_PIN == 14)))) ) /*ATtinyX7(ATTinyCore) and pin 3 or 14 */ \
&& ! ( ( defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) \
        || defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) \
        || defined(__AVR_ATmega8__) || defined(__AVR_ATmega48__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PB__) \
        || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)) \
&& ((IR_INPUT_PIN == 2) || (IR_INPUT_PIN == 3)) ) /* ATmegas and pin 2 or 3 */
#define TINY_RECEICER_USE_ARDUINO_ATTACH_INTERRUPT // cannot use any static ISR vector here
#endif

/*
 * This function is called if a complete command was received and must be implemented by the including file (user code)
 */
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat);

// LSB first, 1 start bit + 16 bit address + 8 bit data + 8 bit inverted data + 1 stop bit.
// see: https://www.sbprojects.net/knowledge/ir/nec.php

#define NEC_ADDRESS_BITS        16 // 16 bit address or 8 bit address and 8 bit inverted address
#define NEC_COMMAND_BITS        16 // Command and inverted command

#define NEC_BITS                (NEC_ADDRESS_BITS + NEC_COMMAND_BITS)
#define NEC_UNIT                560

#define NEC_HEADER_MARK         (16 * NEC_UNIT) // 9000
#define NEC_HEADER_SPACE        (8 * NEC_UNIT)  // 4500

#define NEC_BIT_MARK            NEC_UNIT
#define NEC_ONE_SPACE           (3 * NEC_UNIT)  // 1690
#define NEC_ZERO_SPACE          NEC_UNIT

#define NEC_REPEAT_HEADER_SPACE (4 * NEC_UNIT)  // 2250
#define NEC_REPEAT_PERIOD       110000 // Not used yet - Commands are repeated every 110 ms (measured from start to start) for as long as the key on the remote control is held down.

/*
 * Macros for comparing timing values
 */
#define lowerValue25Percent(aDuration)   (aDuration - (aDuration / 4))
#define upperValue25Percent(aDuration)   (aDuration + (aDuration / 4))
#define lowerValue(aDuration)   (aDuration - (aDuration / 2))
#define upperValue(aDuration)   (aDuration + (aDuration / 2))

/*
 * The states for the state machine
 */
#define IR_RECEIVER_STATE_WAITING_FOR_START_MARK        0
#define IR_RECEIVER_STATE_WAITING_FOR_START_SPACE       1
#define IR_RECEIVER_STATE_WAITING_FOR_FIRST_DATA_MARK   2
#define IR_RECEIVER_STATE_WAITING_FOR_DATA_SPACE        3
#define IR_RECEIVER_STATE_WAITING_FOR_DATA_MARK         4
#define IR_RECEIVER_STATE_WAITING_FOR_STOP_MARK         5
/*
 * The control and data structure of the state machine
 */
struct TinyIRReceiverStruct {
    /*
     * State machine
     */
    uint32_t LastChangeMicros; // microseconds of last Pin Change Interrupt.
    uint8_t IRReceiverState;
    uint8_t IRRawDataBitCounter;
    /*
     * Data
     */
    uint32_t IRRawDataMask;
    LongUnion IRRawData;
    bool IRRepeatDetected;
};

void initPCIInterruptForTinyReceiver();

#endif // TINY_IR_RECEIVER_H

#pragma once
