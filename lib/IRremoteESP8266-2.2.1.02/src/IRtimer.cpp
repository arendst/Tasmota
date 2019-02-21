// Copyright 2017 David Conran

#include "IRtimer.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif

// This class performs a simple time in useconds since instantiated.
// Handles when the system timer wraps around (once).

IRtimer::IRtimer() {
  reset();
}

void IRtimer::reset() {
#ifndef UNIT_TEST
  start = micros();
#else
  start = 0;
#endif
}

uint32_t IRtimer::elapsed() {
#ifndef UNIT_TEST
  uint32_t now = micros();
#else
  uint32_t now = 0;
#endif
  if (start <= now)  // Check if the system timer has wrapped.
    return now - start;  // No wrap.
  else
    return UINT32_MAX - start + now;  // Has wrapped.
}
