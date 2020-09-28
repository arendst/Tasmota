/*---------------------------------------------------------------------------------------------------------------------------------------------------
 * IRFeedbackLed.cpp.h - Arduino extensions for IR feedback LED handling
 *
 * Copyright (c) 2020 Armin Joachimsmeyer
 *
 * This file is part of IRMP https://github.com/ukw100/IRMP.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *---------------------------------------------------------------------------------------------------------------------------------------------------
 */

#if defined(ARDUINO)
#ifndef IR_FEEDBACK_LED_CPP_H
#define IR_FEEDBACK_LED_CPP_H

#include "IRFeedbackLed.h"

static bool irmp_irsnd_LedFeedbackEnabled;

#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
// global variable to hold feedback led pin number. Set to 0 to not activating feedback LED by default.
uint_fast8_t irmp_irsnd_LedFeedbackPin = 0;
#  if defined(FEEDBACK_LED_IS_ACTIVE_LOW)
bool irmp_irsnd_LedFeedbackPinIsActiveLow = true; // global variable to hold feedback led polarity.
#  else
bool irmp_irsnd_LedFeedbackPinIsActiveLow = false; // global variable to hold feedback led polarity.
#  endif
#endif

/*
 * Enable/disable echoing the input signal to the built in (or specified) LED.
 */
void irmp_irsnd_LEDFeedback(bool aEnableBlinkLed)
{
#if defined(IRMP_FEEDBACK_LED_PIN) || defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
    irmp_irsnd_LedFeedbackEnabled = aEnableBlinkLed;
    if (aEnableBlinkLed)
    {
#  if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
        pinMode(irmp_irsnd_LedFeedbackPin, OUTPUT);
        if (irmp_irsnd_LedFeedbackPinIsActiveLow)
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, HIGH);
        }
        else
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, LOW);
        }

#  elif defined(IRMP_FEEDBACK_LED_PIN)
        pinModeFast(IRMP_FEEDBACK_LED_PIN, OUTPUT);
#    if defined(FEEDBACK_LED_IS_ACTIVE_LOW)
        digitalWriteFast(IRMP_FEEDBACK_LED_PIN, HIGH);
#    else
        digitalWriteFast(IRMP_FEEDBACK_LED_PIN, LOW);
#    endif
#  endif //  defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
    }
#  if defined(ALLOW_DISABLE_FEEDBACK_LED_EXPLICIT)
    else
    {
        /*
         * Disable here
         * normally this code is never used, since disabling is done by setting irmp_led_feedback to false.
         */
#    if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
        if(irmp_irsnd_LedFeedbackPin != 0) {
            pinMode(irmp_irsnd_LedFeedbackPin, INPUT);
            digitalWrite(irmp_irsnd_LedFeedbackPin, LOW); // to disable internal pullup
        }
#    else
        pinModeFast(IRMP_FEEDBACK_LED_PIN, INPUT);
        digitalWriteFast(IRMP_FEEDBACK_LED_PIN, LOW); // to disable internal pullup
#    endif
    }
#  endif
#endif // defined(IRMP_FEEDBACK_LED_PIN) || defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
}

/*
 * Internally used from IRMP_ISR() with -oS it is taken as inline function
 */
#if defined(ESP8266)
void ICACHE_RAM_ATTR irmp_irsnd_SetFeedbackLED(bool aSwitchLedOn)
#elif defined(ESP32)
void IRAM_ATTR irmp_irsnd_SetFeedbackLED(bool aSwitchLedOn)
#else
void irmp_irsnd_SetFeedbackLED(bool aSwitchLedOn)
#endif
{
#if defined(IRMP_IRSND_ALLOW_DYNAMIC_PINS)
    if(irmp_irsnd_LedFeedbackPin != 0) {
        if (irmp_irsnd_LedFeedbackPinIsActiveLow)
#  if defined(__AVR_ATtiny3217__) // TinyCore introduced PinStatus type
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, (PinStatus)!aSwitchLedOn);
        }
        else
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, (PinStatus)aSwitchLedOn);
        }
#  else
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, !aSwitchLedOn);
        }
        else
        {
            digitalWrite(irmp_irsnd_LedFeedbackPin, aSwitchLedOn);
        }
#  endif
    }
#elif defined(IRMP_FEEDBACK_LED_PIN)
#  if defined(__AVR__) // As far as I know, there is no active-low built in LED for AVR platform boards
#    if defined(__AVR_ATtiny3217__) // TinyCore introduced PinStatus type
    digitalWriteFast(IRMP_FEEDBACK_LED_PIN, (PinStatus)aSwitchLedOn);
#    else
    digitalWriteFast(IRMP_FEEDBACK_LED_PIN, aSwitchLedOn);
#    endif
#  else
        // hope this is fast enough on other platforms
#    if defined(FEEDBACK_LED_IS_ACTIVE_LOW)
        // If the built in LED on the board is active LOW
    digitalWrite(IRMP_FEEDBACK_LED_PIN, !aSwitchLedOn);
#    else
    digitalWrite(IRMP_FEEDBACK_LED_PIN, aSwitchLedOn);
#    endif
#  endif
#endif
}

#endif // IR_FEEDBACK_LED_CPP_H
#endif // defined(ARDUINO)
