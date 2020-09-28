/*
 *  TinIRReceiver.cpp.h
 *
 *  Receives IR protocol data of NEC protocol using pin change interrupts.
 *  NEC is the protocol of most cheap remote controls for Arduino.
 *
 *  No parity check is done!
 *  On a completely received IR command, the user function handleReceivedIRData(uint16_t aAddress, uint8_t aCommand, bool isRepetition)
 *  is called in Interrupt context but with interrupts being enabled to enable use of delay() etc.
 *  !!!!!!!!!!!!!!!!!!!!!!
 *  Functions called in interrupt context should be running as short as possible,
 *  so if you require longer action, save the data (address + command) and handle them in the main loop.
 *  !!!!!!!!!!!!!!!!!!!!!
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

#ifndef TINY_IR_RECEIVER_CPP_H
#define TINY_IR_RECEIVER_CPP_H

#include <Arduino.h>

#include "TinyIRReceiver.h"
#include "digitalWriteFast.h"

//#define TRACE

TinyIRReceiverStruct TinyIRReceiverControl;

void handleReceivedIRData(uint16_t aAddress, uint8_t aCommand, bool isRepetition);

bool MatchDuration(uint16_t aDuration, uint16_t aLowerMatchValue, uint16_t aUpperMatchValue) {
    return (aDuration >= aLowerMatchValue && aDuration <= aUpperMatchValue);
}

#if defined(ESP8266)
void ICACHE_RAM_ATTR IRPinChangeInterruptHandler(void)
#elif defined(ESP32)
void IRAM_ATTR IRPinChangeInterruptHandler(void)
#else
void IRPinChangeInterruptHandler(void)
#endif
        {
    // save IR input level - negative logic, true means inactive / IR pause
    uint_fast8_t tIRLevel = digitalReadFast(IR_INPUT_PIN);

#if !defined(DO_NOT_USE_FEEDBACK_LED) && defined(IR_FEEDBACK_LED_PIN)
#  if defined(__AVR_ATtiny3217__) // TinyCore introduced PinStatus type
    digitalWriteFast(IR_FEEDBACK_LED_PIN, (PinStatus)!tIRLevel);
#  else
    digitalWriteFast(IR_FEEDBACK_LED_PIN, !tIRLevel);
#  endif
#endif

    /*
     * 1. compute microseconds after last change
     */
    uint32_t tCurrentMicros = micros();
    uint16_t tDeltaMicros = tCurrentMicros - TinyIRReceiverControl.LastChangeMicros;
    TinyIRReceiverControl.LastChangeMicros = tCurrentMicros;

    uint8_t tState = TinyIRReceiverControl.IRReceiverState;

#ifdef TRACE
    Serial.print(tState);
    Serial.print(' ');
//    Serial.print(F(" I="));
//    Serial.print(tIRLevel);
//    Serial.print(F(" D="));
//    Serial.print(tDeltaMicros);
//    Serial.println();
#endif

    if (tIRLevel == LOW) {
        /*
         * We receive a signal now
         */
        if (tDeltaMicros > 2 * NEC_HEADER_MARK) {
            // timeout -> must reset state machine
            tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
        }
        if (tState == IR_RECEIVER_STATE_WAITING_FOR_START_MARK) {
            tState = IR_RECEIVER_STATE_WAITING_FOR_START_SPACE;
        }

        else if (tState == IR_RECEIVER_STATE_WAITING_FOR_FIRST_DATA_MARK) {
            // Check start space length
            if (tDeltaMicros >= lowerValue25Percent(NEC_HEADER_SPACE) && tDeltaMicros <= upperValue25Percent(NEC_HEADER_SPACE)) {
                tState = IR_RECEIVER_STATE_WAITING_FOR_DATA_SPACE;
                TinyIRReceiverControl.IRRawDataBitCounter = 0;
                TinyIRReceiverControl.IRRawData.ULong = 0;
                TinyIRReceiverControl.IRRawDataMask = 1;
                TinyIRReceiverControl.IRRepeatDetected = false;
            } else if (tDeltaMicros >= lowerValue25Percent(NEC_REPEAT_HEADER_SPACE)
                    && tDeltaMicros <= upperValue25Percent(NEC_REPEAT_HEADER_SPACE)) {
                TinyIRReceiverControl.IRRepeatDetected = true;
                tState = IR_RECEIVER_STATE_WAITING_FOR_DATA_SPACE;
            } else {
                // This parts are optimized by the compiler into jumps to one code :-)
                // Wrong length -> reset state
                tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
            }
        }

        else if (tState == IR_RECEIVER_STATE_WAITING_FOR_DATA_MARK) {
            // Check data space length
            if (tDeltaMicros >= lowerValue(NEC_ZERO_SPACE) && tDeltaMicros <= upperValue(NEC_ONE_SPACE)) {
                // Here we have a valid bit
                tState = IR_RECEIVER_STATE_WAITING_FOR_DATA_SPACE;
                if (tDeltaMicros >= 2 * NEC_UNIT) {
                    // we received a 1
                    TinyIRReceiverControl.IRRawData.ULong |= TinyIRReceiverControl.IRRawDataMask;
                } else {
                    // we received a 0
                }
                TinyIRReceiverControl.IRRawDataMask = TinyIRReceiverControl.IRRawDataMask << 1;
                TinyIRReceiverControl.IRRawDataBitCounter++;
            } else {
                // Wrong length -> reset state
                tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
            }
        } else {
            // error wrong state for the received level (should not happen!) -> reset state
            tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
        }
    }

    else {
        /*
         * We receive a space now
         */
        if (tState == IR_RECEIVER_STATE_WAITING_FOR_START_SPACE) {
            // Check start bit length
            if (tDeltaMicros >= lowerValue25Percent(NEC_HEADER_MARK) && tDeltaMicros <= upperValue25Percent(NEC_HEADER_MARK)) {
                tState = IR_RECEIVER_STATE_WAITING_FOR_FIRST_DATA_MARK;
            } else {
                // Wrong length -> reset state
                tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
            }
        }

        else if (tState == IR_RECEIVER_STATE_WAITING_FOR_DATA_SPACE) {

            // Check data bit length
            if (tDeltaMicros >= lowerValue(NEC_BIT_MARK) && tDeltaMicros <= upperValue(NEC_BIT_MARK)) {
                if (TinyIRReceiverControl.IRRawDataBitCounter >= NEC_BITS || TinyIRReceiverControl.IRRepeatDetected) {
                    /*
                     * Code complete -> call callback
                     * No parity check
                     */
                    // can not check the length of trailing space
                    tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
#if !defined(ARDUINO_ARCH_MBED)
                    interrupts();
#endif
                    /*
                     * Address reduction to 8 bit
                     */
                    if (TinyIRReceiverControl.IRRawData.UByte.LowByte
                            == (uint8_t) (~TinyIRReceiverControl.IRRawData.UByte.MidLowByte)) {
                        // standard 8 bit address NEC protocol
                        TinyIRReceiverControl.IRRawData.UByte.MidLowByte = 0; // Address is the first 8 bit
                    }

                    handleReceivedTinyIRData(TinyIRReceiverControl.IRRawData.UWord.LowWord,
                            TinyIRReceiverControl.IRRawData.UByte.MidHighByte, TinyIRReceiverControl.IRRepeatDetected);

                } else {
                    tState = IR_RECEIVER_STATE_WAITING_FOR_DATA_MARK;
                }
            } else {
                // Wrong length -> reset state
                tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
            }
        } else {
            // error wrong state for the received level (should not happen!) -> reset state
            tState = IR_RECEIVER_STATE_WAITING_FOR_START_MARK;
        }
    }

    TinyIRReceiverControl.IRReceiverState = tState;
}

void initPCIInterruptForTinyReceiver() {
    pinModeFast(IR_INPUT_PIN, INPUT_PULLUP);

#if !defined(DO_NOT_USE_FEEDBACK_LED) && defined(IR_FEEDBACK_LED_PIN)
    pinModeFast(IR_FEEDBACK_LED_PIN, OUTPUT);
#endif

#if defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__)
    attachInterrupt(IR_INPUT_PIN, IRPinChangeInterruptHandler, CHANGE); // 2.2 us more than version configured with macros and not compatible

#elif ! defined(__AVR__) || defined(TINY_RECEICER_USE_ARDUINO_ATTACH_INTERRUPT)
    // costs 112 bytes FLASH + 4bytes RAM
    attachInterrupt(digitalPinToInterrupt(IR_INPUT_PIN), IRPinChangeInterruptHandler, CHANGE);
#else
#  if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    // use PinChangeInterrupt
    PCMSK |= _BV(IR_INPUT_PIN);
    // clear interrupt bit
    GIFR |= 1 << PCIF;
    // enable interrupt on next change
    GIMSK |= 1 << PCIE;

#  elif defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#    if defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IR_INPUT_PIN == 3)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#      elif (IR_INPUT_PIN == 9)
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#      else
#        error "IR_INPUT_PIN must be 9 or 3."
#      endif // if (IR_INPUT_PIN == 9)

#    else // defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IR_INPUT_PIN == 14)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#      elif (IR_INPUT_PIN == 3)
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#      else
#        error "IR_INPUT_PIN must be 14 or 3."
#      endif // if (IR_INPUT_PIN == 14)
#    endif

#  else // defined(__AVR_ATtiny25__)
    /*
     * ATmegas here
     */
#    if (IR_INPUT_PIN == 2)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#    elif (IR_INPUT_PIN == 3)
    EICRA |= _BV(ISC10);
// clear interrupt bit
    EIFR |= 1 << INTF1;
// enable interrupt on next change
    EIMSK |= 1 << INT1;
#    elif IR_INPUT_PIN == 4 || IR_INPUT_PIN == 5 || IR_INPUT_PIN == 6 || IR_INPUT_PIN == 7
    //ATmega328 (Uno, Nano ) etc. Enable pin change interrupt 20 to 23 for port PD4 to PD7 (Arduino pin 4 to 7)
        PCICR |= _BV(PCIE2);
        PCMSK2 = digitalPinToBitMask(IR_INPUT_PIN);
#    elif IR_INPUT_PIN == 8 || IR_INPUT_PIN == 9 || IR_INPUT_PIN == 10 || IR_INPUT_PIN == 11 || IR_INPUT_PIN == 12 || IR_INPUT_PIN == 13
    //ATmega328 (Uno, Nano ) etc. Enable pin change interrupt 0 to 5 for port PB0 to PB5 (Arduino pin 8 to 13)
        PCICR |= _BV(PCIE0);
        PCMSK0 = digitalPinToBitMask(IR_INPUT_PIN);
#    elif IR_INPUT_PIN == A0 || IR_INPUT_PIN == A1 || IR_INPUT_PIN == A2 || IR_INPUT_PIN == A3 || IR_INPUT_PIN == A4 || IR_INPUT_PIN == A5
    //ATmega328 (Uno, Nano ) etc. Enable pin change interrupt 8 to 13 for port PC0 to PC5 (Arduino pin A0 to A5)
        PCICR |= _BV(PCIE1);
        PCMSK1 = digitalPinToBitMask(IR_INPUT_PIN);
#    else
#      error "IR_INPUT_PIN not allowed."
#    endif // if (IR_INPUT_PIN == 2)
#  endif // defined(__AVR_ATtiny25__)
#endif // ! defined(__AVR__) || defined(IRMP_USE_ARDUINO_ATTACH_INTERRUPT)
}

/*
 * Specify the right INT0, INT1 or PCINT0 interrupt vector according to different pins and cores
 */
#if defined(__AVR__) && !defined(TINY_RECEICER_USE_ARDUINO_ATTACH_INTERRUPT)
#  if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
ISR(PCINT0_vect)
#  elif defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#    if defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IR_INPUT_PIN == 3) //  PB6 / INT0 is connected to USB+ on DigisparkPro boards
ISR(INT0_vect)
#      endif
#     if (IR_INPUT_PIN == 9)
ISR(INT1_vect)
#     endif

#    else // defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IR_INPUT_PIN == 14) // For AVR_ATtiny167 INT0 is on pin 14 / PB6
ISR(INT0_vect)
#      endif
#    endif

#  else // AVR_ATtiny167
#    if (IR_INPUT_PIN == 2)
ISR(INT0_vect)
#    endif
#    if (IR_INPUT_PIN == 3) && !defined(ARDUINO_AVR_DIGISPARKPRO)
ISR(INT1_vect)
#    elif IR_INPUT_PIN == 4 || IR_INPUT_PIN == 5 || IR_INPUT_PIN == 6 || IR_INPUT_PIN == 7
// PCINT for ATmega328 Arduino pins 4 (PD4) to 7 (PD7) - (PCINT 20 to 23)
ISR(PCINT2_vect)
#    elif IR_INPUT_PIN == 8 || IR_INPUT_PIN == 9 || IR_INPUT_PIN == 10 || IR_INPUT_PIN == 11 || IR_INPUT_PIN == 12 || IR_INPUT_PIN == 13
// PCINT for ATmega328 Arduino pins 8 (PB0) to 13 (PB5) - (PCINT 0 to 5)
ISR(PCINT0_vect)
#    elif IR_INPUT_PIN == A0 || IR_INPUT_PIN == A1 || IR_INPUT_PIN == A2 || IR_INPUT_PIN == A3 || IR_INPUT_PIN == A4 || IR_INPUT_PIN == A5
// PCINT for ATmega328 Arduino pins A1 (PC0) to A5 (PC5) - (PCINT 8 to 13)
ISR(PCINT1_vect)
#    endif
#  endif // defined(__AVR_ATtiny25__)
{
    IRPinChangeInterruptHandler();
}
#endif // defined(__AVR__) && ! defined(IRMP_USE_ARDUINO_ATTACH_INTERRUPT)

#endif // TINY_IR_RECEIVER_CPP_H

#pragma once
