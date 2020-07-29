// Copyright 2017 David Conran

#ifndef IRTIMER_H_
#define IRTIMER_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>

// Classes
class IRtimer {
 public:
  IRtimer();
  void reset();
  uint32_t elapsed();
#ifdef UNIT_TEST
  static void add(uint32_t usecs);
#endif  // UNIT_TEST

 private:
  uint32_t start;
};

class TimerMs {
 public:
  TimerMs();
  void reset();
  uint32_t elapsed();
#ifdef UNIT_TEST
  static void add(uint32_t msecs);
#endif  // UNIT_TEST

 private:
  uint32_t start;
};
#endif  // IRTIMER_H_
