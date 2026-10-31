// Copyright 2017 David Conran

#include "IRtimer.h"
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#if defined(ESP32) && !defined(UNIT_TEST)
// IRsend::mark() times the software carrier with IRtimer; keep it in IRAM too.
#include "esp_attr.h"
#include "esp_timer.h"
#define IRTIMER_IRAM IRAM_ATTR
#define IRTIMER_MICROS() ((uint32_t)esp_timer_get_time())
#else
#define IRTIMER_IRAM
#define IRTIMER_MICROS() micros()
#endif

#ifdef UNIT_TEST
// Used to help simulate elapsed time in unit tests.
uint32_t _IRtimer_unittest_now = 0;
uint32_t _TimerMs_unittest_now = 0;
#endif  // UNIT_TEST

/// Class constructor.
IRTIMER_IRAM IRtimer::IRtimer() { reset(); }

/// Resets the IRtimer object. I.e. The counter starts again from now.
void IRTIMER_IRAM IRtimer::reset() {
#ifndef UNIT_TEST
  start = IRTIMER_MICROS();
#else
  start = _IRtimer_unittest_now;
#endif
}

/// Calculate how many microseconds have elapsed since the timer was started.
/// @return Nr. of microseconds.
uint32_t IRTIMER_IRAM IRtimer::elapsed() {
#ifndef UNIT_TEST
  uint32_t now = IRTIMER_MICROS();
#else
  uint32_t now = _IRtimer_unittest_now;
#endif
  if (start <= now)      // Check if the system timer has wrapped.
    return now - start;  // No wrap.
  else
    return UINT32_MAX - start + now;  // Has wrapped.
}

/// Add time to the timer to simulate elapsed time.
/// @param[in] usecs Nr. of uSeconds to be added.
/// @note Only used in unit testing.
#ifdef UNIT_TEST
void IRtimer::add(uint32_t usecs) { _IRtimer_unittest_now += usecs; }
#endif  // UNIT_TEST

/// Class constructor.
TimerMs::TimerMs() { reset(); }

/// Resets the TimerMs object. I.e. The counter starts again from now.
void TimerMs::reset() {
#ifndef UNIT_TEST
  start = millis();
#else
  start = _TimerMs_unittest_now;
#endif
}

/// Calculate how many milliseconds have elapsed since the timer was started.
/// @return Nr. of milliseconds.
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

/// Add time to the timer to simulate elapsed time.
/// @param[in] msecs Nr. of mSeconds to be added.
/// @note Only used in unit testing.
#ifdef UNIT_TEST
void TimerMs::add(uint32_t msecs) { _IRtimer_unittest_now += msecs; }
#endif  // UNIT_TEST
