/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * irmpPinChangeInterrupt.cpp.h
 *
 * Functions and ISR for the pin change interrupt functionality for IRMP - For Arduino platform
 * Must be included after irmp_ISR to have all the internal variables of irmp_ISR declared
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

#ifndef IRMP_PIN_CHANGE_INTERRUPT_CPP_H
#define IRMP_PIN_CHANGE_INTERRUPT_CPP_H

//#define PCI_DEBUG

#  if defined(__AVR__)
void irmp_debug_print(const __FlashStringHelper *aMessage, bool aDoShortOutput = true);
#  else
void irmp_debug_print(const char *aMessage, bool aDoShortOutput);
#  endif

uint32_t irmp_last_change_micros; // microseconds of last Pin Change Interrupt. Used for irmp_IsBusy().
/*
 * Wrapper for irmp_ISR() in order to run it with Pin Change Interrupts.
 * Needs additional 8-9us per call and 13us for signal going inactive and 19us for going active.
 * Tested for NEC, Kaseiko, Denon, RC6, Samsung and others.
 * Requires micros() for timing.
 */
//#define PCI_DEBUG
#if defined(ESP8266)
void ICACHE_RAM_ATTR irmp_PCI_ISR(void)
#elif defined(ESP32)
void IRAM_ATTR irmp_PCI_ISR(void)
#else
void irmp_PCI_ISR(void)
#endif
{

    // save IR input level - negative logic, true means inactive / IR pause
    uint_fast8_t irmp_input = input(IRMP_PIN);

    /*
     * 1. compute ticks after last change
     */
    uint32_t tMicros = micros();
    uint32_t tTicks = tMicros - irmp_last_change_micros; // values up to 10000
    irmp_last_change_micros = tMicros;
#if (F_INTERRUPTS == 15625)
    // F_INTERRUPTS value of 31250 does not work (maybe 8 bit overflow?)
    tTicks = (tTicks << 2) >> 8;// saves 1.3 us
#else
#error F_INTERRUPTS must be 15625 (to avoid a time consuming division)
#endif

    if (tTicks != 0) {
        tTicks -= 1; // adjust for irmp_pulse_time / irmp_pause_time incremented in irmp_ISR()
    }

    /*
     * 2. check for input level and tweak timings
     */
    if (irmp_input) // true means inactive / IR pause
    {
        // start of pause -> just set pulse width
        irmp_pulse_time += tTicks;
    } else {
        if (irmp_start_bit_detected) {
            irmp_pause_time += tTicks;
        } else { // start pulse here -> set pause or time between repetitions
            if (tTicks > 0xFFFF) {
                // avoid overflow for 16 bit key_repetition_len
                tTicks = 0xFFFF;
            }
            key_repetition_len = tTicks;
        }
    }

    /*
     * 3. call the protocol detection routine
     */
    irmp_ISR();

    if (!irmp_ir_detected && irmp_input) {
        /*
         * No valid protocol detected and IR input is inactive now -> simulate end for protocols.
         * IRMP may be waiting for stop bit, but detects it only at the next call, so do one additional call.
         * !!! ATTENTION !!! This will NOT work if we try to receive simultaneously two protocols which are only different in length like NEC16 and NEC42
         */
#ifdef PCI_DEBUG
        Serial.write('x');
        if (irmp_bit > 0 && irmp_bit == irmp_param.complete_len)
        {
            Serial.println(irmp_start_bit_detected); // print start bit if complete_len is reached
        }
#endif
        if (irmp_start_bit_detected && irmp_bit == irmp_param.complete_len && irmp_param.stop_bit == TRUE) {
            // Try to detect a nec repeat irmp_bit is 0
#ifdef PCI_DEBUG
            irmp_debug_print(F("R"));
#endif
            PAUSE_LEN irmp_pause_time_store = irmp_pause_time;
            irmp_pause_time = STOP_BIT_PAUSE_LEN_MIN + 1; // set pause time to minimal pause required to detect a stop bit
            irmp_ISR(); // Call to detect a NEC repeat
#ifdef PCI_DEBUG
            irmp_debug_print(F("E")); // print info after call
            Serial.println();
#endif
            if (irmp_ir_detected) {
                // no protocol detected -> restore irmp_pause_time. Not sure if this is really required.
                irmp_pause_time = irmp_pause_time_store;
            }
        }

        // For condition see also line 4203 and 5098 in irmp.c.h
        if (irmp_start_bit_detected && irmp_bit > 0 && irmp_bit == irmp_param.complete_len) {
            // Complete length of bit now received -> try to detect end of protocol
#ifdef PCI_DEBUG
            irmp_debug_print(F("S")); // print info before call
#endif
            PAUSE_LEN irmp_pause_time_store = irmp_pause_time;
            irmp_pause_time = STOP_BIT_PAUSE_LEN_MIN + 1; // set pause time to minimal pause required to detect a stop bit
            irmp_ISR(); // Call to detect end of protocol, irmp_param.stop_bit (printed as Sb) should be set to 0 if stop bit was successfully detected.
#ifdef PCI_DEBUG
            irmp_debug_print(F("E")); // print info after call
            Serial.println();
#endif
            if (irmp_ir_detected) {
                // no protocol detected -> restore irmp_pause_time. Not sure if this is really required.
                irmp_pause_time = irmp_pause_time_store;
            }
        }

#if (IRMP_SUPPORT_MANCHESTER == 1)
    /*
     * Simulate end for Manchester/biphase protocols - 130 bytes
     */
    if (((irmp_bit == irmp_param.complete_len - 1 && tTicks < irmp_param.pause_1_len_max)
                    || (irmp_bit == irmp_param.complete_len - 2 && tTicks > irmp_param.pause_1_len_max))
            && (irmp_param.flags & IRMP_PARAM_FLAG_IS_MANCHESTER) && irmp_start_bit_detected)
    {
#  ifdef PCI_DEBUG
    Serial.println('M'); // Try to detect a Manchester end of protocol
#  endif
    irmp_pause_time = 2 * irmp_param.pause_1_len_max;
        irmp_ISR();        // Write last one (with value 0) or 2 (with last value 1) data bits and set wait for dummy stop bit
        irmp_ISR();// process dummy stop bit
        irmp_ISR();// reset stop bit and call callback
    }
#endif
    }
}

void initPCIInterrupt() {
#if defined(__AVR_ATtiny1616__)  || defined(__AVR_ATtiny3216__) || defined(__AVR_ATtiny3217__)
    attachInterrupt(IRMP_INPUT_PIN, irmp_PCI_ISR, CHANGE); // 14.2 us before LED Feedback compared to 12 if configured with macros and not compatible

#elif ! defined(__AVR__) || defined(IRMP_USE_ARDUINO_ATTACH_INTERRUPT)
    attachInterrupt(digitalPinToInterrupt(IRMP_INPUT_PIN), irmp_PCI_ISR, CHANGE);

#else
#  if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    // use PinChangeInterrupt
    PCMSK |= _BV(IRMP_INPUT_PIN);
    // clear interrupt bit
    GIFR |= 1 << PCIF;
    // enable interrupt on next change
    GIMSK |= 1 << PCIE;

#  elif defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#    if defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IRMP_INPUT_PIN == 3)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#      elif (IRMP_INPUT_PIN == 9)
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#      else
#        error "For interrupt mode (IRMP_ENABLE_PIN_CHANGE_INTERRUPT is defined) IRMP_INPUT_PIN must be 9 or 3."
#      endif // if (IRMP_INPUT_PIN == 9)

#    else // defined(ARDUINO_AVR_DIGISPARKPRO)
#      if (IRMP_INPUT_PIN == 14)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#      elif (IRMP_INPUT_PIN == 3)
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#      else
#        error "For interrupt mode (IRMP_ENABLE_PIN_CHANGE_INTERRUPT is defined) IRMP_INPUT_PIN must be 14 or 3."
#      endif // if (IRMP_INPUT_PIN == 14)
#    endif

#  else // defined(__AVR_ATtiny25__)
    /*
     * ATmegas here
     */
#    if (IRMP_INPUT_PIN == 2)
    // interrupt on any logical change
    EICRA |= _BV(ISC00);
    // clear interrupt bit
    EIFR |= 1 << INTF0;
    // enable interrupt on next change
    EIMSK |= 1 << INT0;
#    elif (IRMP_INPUT_PIN == 3)
    EICRA |= _BV(ISC10);
    // clear interrupt bit
    EIFR |= 1 << INTF1;
    // enable interrupt on next change
    EIMSK |= 1 << INT1;
#    else
#      error "For interrupt mode (IRMP_ENABLE_PIN_CHANGE_INTERRUPT is defined) IRMP_INPUT_PIN must be 2 or 3."
#    endif // if (IRMP_INPUT_PIN == 2)
#  endif // defined(__AVR_ATtiny25__)
#endif // ! defined(__AVR__) || defined(IRMP_USE_ARDUINO_ATTACH_INTERRUPT)
}

/*
 * Specify the right INT0, INT1 or PCINT0 interrupt vector according to different pins and cores
 */
#if defined(__AVR__) && ! defined(IRMP_USE_ARDUINO_ATTACH_INTERRUPT)
# if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
ISR(PCINT0_vect)

#  else
#    if defined(__AVR_ATtiny87__) || defined(__AVR_ATtiny167__)
#      if defined(ARDUINO_AVR_DIGISPARKPRO)
#        if (IRMP_INPUT_PIN == 3) //  PB6 / INT0 is connected to USB+ on DigisparkPro boards
ISR(INT0_vect)
#        endif
#       if (IRMP_INPUT_PIN == 9)
ISR(INT1_vect)
#       endif

#      else // defined(ARDUINO_AVR_DIGISPARKPRO)
#        if (IRMP_INPUT_PIN == 14) // For AVR_ATtiny167 INT0 is on pin 14 / PB6
ISR(INT0_vect)
#        endif
#      endif

#    else // AVR_ATtiny167
#    if (IRMP_INPUT_PIN == 2)
ISR(INT0_vect)
#      endif
#    endif // AVR_ATtiny167

#    if (IRMP_INPUT_PIN == 3) && !defined(ARDUINO_AVR_DIGISPARKPRO)
ISR(INT1_vect)
#    endif
#  endif // defined(__AVR_ATtiny25__)
{
    irmp_PCI_ISR();
}
#endif // defined(__AVR__)

#if defined(__AVR__)
void irmp_debug_print(const __FlashStringHelper *aMessage, bool aDoShortOutput)
#else
void irmp_debug_print(const char *aMessage, bool aDoShortOutput)
#endif
{
    Serial.print(aMessage);
    Serial.print(' ');
    Serial.print(irmp_ir_detected); // valid IR command detected
    Serial.print(F(" St"));
    Serial.print(irmp_start_bit_detected);

    Serial.print(F(" Ws"));
    Serial.print(wait_for_space); // true if in data/address section and no signal. Now increment pause time.
    Serial.print(F(" Wss"));
    Serial.print(wait_for_start_space); // true if we have received start bit

    Serial.print(F(" L"));
    Serial.print(irmp_param.complete_len); // maximum bit position
    Serial.print(F(" B"));
    Serial.print((int8_t) irmp_bit); // current bit position - FF(-1) is start value
    Serial.print(F(" Pu"));
    Serial.print(irmp_pulse_time); // bit time for pulse
    Serial.print(F(" Pa"));
    Serial.print(irmp_pause_time);

    Serial.print(F(" Sb"));
    Serial.print(irmp_param.stop_bit); // boolean. 1 = stop bit required

    if (!aDoShortOutput)
    {
        Serial.print(F(" F"));
        Serial.print(irmp_flags); // currently only repetition flag
        Serial.print(F(" K"));
        Serial.print(key_repetition_len); // the pause after a command to distinguish repetitions from new commands
        Serial.print(F(" R"));
        Serial.print(repetition_frame_number); // Number of repetitions
    }

    Serial.println();
}

#endif // IRMP_PIN_CHANGE_INTERRUPT_CPP_H
