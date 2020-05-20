// Copyright 2017 David Conran

#include "IRtimer.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif

#ifdef UNIT_TEST
// Used to help simulate elapsed time in unit tests.
uint32_t _IRtimer_unittest_now = 0;
uint32_t _TimerMs_unittest_now = 0;
#endif  // UNIT_TEST

// This class performs a simple time in useconds since instantiated.
// Handles when the system timer wraps around (once).
IRtimer::IRtimer() { reset(); }

void IRtimer::reset() {
#ifndef UNIT_TEST
  start = micros();
#else
  start = _IRtimer_unittest_now;
#endif
}

uint32_t IRtimer::elapsed() {
#ifndef UNIT_TEST
  uint32_t now = micros();
#else
  uint32_t now = _IRtimer_unittest_now;
#endif
  if (start <= now)      // Check if the system timer has wrapped.
    return now - start;  // No wrap.
  else
    return UINT32_MAX - start + now;  // Has wrapped.
}

// Only used in unit testing.
#ifdef UNIT_TEST
void IRtimer::add(uint32_t usecs) { _IRtimer_unittest_now += usecs; }
#endif  // UNIT_TEST

// This class performs a simple time in milli-seoncds since instantiated.
// Handles when the system timer wraps around (once).
TimerMs::TimerMs() { reset(); }

void TimerMs::reset() {
#ifndef UNIT_TEST
  start = millis();
#else
  start = _TimerMs_unittest_now;
#endif
}

uint32_t TimerMs::elapsed() {
#ifndef UNIT_TEST
  uint32_t now = millis();
#else
  uint32_t now = _TimerMs_unittest_now;
#endif
  if (start <= now)      // Check if the system timer has wrapped.
    return now - start;  // No wrap.
  else
    return UINT32_MAX - start + now;  // Has wrapped.
}

// Only used in unit testing.
#ifdef UNIT_TEST
void TimerMs::add(uint32_t msecs) { _IRtimer_unittest_now += msecs; }
#endif  // UNIT_TEST
