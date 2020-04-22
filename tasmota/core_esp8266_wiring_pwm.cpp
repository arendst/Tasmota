/*
  pwm.c - analogWrite implementation for esp8266

  Use the shared TIMER1 utilities to generate PWM signals

  Original Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifdef ESP8266

#include <Arduino.h>
#include "core_esp8266_waveform.h"

extern "C" {

// Internal-only calls, not for applications
extern void _setPWMPeriodCC(uint32_t cc);
extern bool _stopPWM(int pin);
extern bool _setPWM(int pin, uint32_t cc);

static uint32_t analogMap = 0;
static int32_t analogScale = PWMRANGE;
static uint16_t analogFreq = 1000;

extern void __analogWriteRange(uint32_t range) {
  if (range > 0) {
    analogScale = range;
  }
}


extern void __analogWriteFreq(uint32_t freq) {
  if (freq < 100) {
    analogFreq = 100;
  } else if (freq > 40000) {
    analogFreq = 40000;
  } else {
    analogFreq = freq;
  }
  uint32_t analogPeriod = microsecondsToClockCycles(1000000UL) / analogFreq;
  _setPWMPeriodCC(analogPeriod);
}

extern void __analogWrite(uint8_t pin, int val) {
  if (pin > 16) {
    return;
  }

  uint32_t analogPeriod = microsecondsToClockCycles(1000000UL) / analogFreq;
  _setPWMPeriodCC(analogPeriod);
  if (val < 0) {
    val = 0;
  } else if (val > analogScale) {
    val = analogScale;
  }

  analogMap &= ~(1 << pin);
  uint32_t high = (analogPeriod * val) / analogScale;
  uint32_t low = analogPeriod - high;
  pinMode(pin, OUTPUT);
  if (low == 0) {
    _stopPWM(pin);
    digitalWrite(pin, HIGH);
  } else if (high == 0) {
    _stopPWM(pin);
    digitalWrite(pin, LOW);
  } else {
    _setPWM(pin, high);
    analogMap |= (1 << pin);
  }
}

extern void analogWrite(uint8_t pin, int val) __attribute__((weak, alias("__analogWrite")));
extern void analogWriteFreq(uint32_t freq) __attribute__((weak, alias("__analogWriteFreq")));
extern void analogWriteRange(uint32_t range) __attribute__((weak, alias("__analogWriteRange")));

};

#endif  // ESP8266