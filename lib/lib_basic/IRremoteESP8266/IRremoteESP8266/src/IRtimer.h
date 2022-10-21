// Copyright 2017 David Conran

#ifndef IRTIMER_H_
#define IRTIMER_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>

// Classes

/// This class offers a simple counter in micro-seconds since instantiated.
/// @note Handles when the system timer wraps around (once).
class IRtimer {
 public:
  IRtimer();
  void reset();
  uint32_t elapsed();
#ifdef UNIT_TEST
  static void add(uint32_t usecs);
#endif  // UNIT_TEST

 private:
  uint32_t start;  ///< Time in uSeconds when the class was instantiated/reset.
};

/// This class offers a simple counter in milli-seconds since instantiated.
/// @note Handles when the system timer wraps around (once).
class TimerMs {
 public:
  TimerMs();
  void reset();
  uint32_t elapsed();
#ifdef UNIT_TEST
  static void add(uint32_t msecs);
#endif  // UNIT_TEST

 private:
  uint32_t start;  ///< Time in mSeconds when the class was instantiated/reset.
};
#endif  // IRTIMER_H_
