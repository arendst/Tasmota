// Copyright 2017 David Conran

#ifndef TEST_IRSEND_TEST_H_
#define TEST_IRSEND_TEST_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <string>
#include "IRrecv.h"
#include "IRsend.h"
#include "IRtimer.h"

#define OUTPUT_BUF 10000U
#define RAW_BUF 10000U

#ifdef UNIT_TEST
// Used to help simulate elapsed time in unit tests.
extern uint32_t _IRtimer_unittest_now;
#endif  // UNIT_TEST

class IRsendTest : public IRsend {
 public:
  uint32_t output[OUTPUT_BUF];
  uint32_t freq[OUTPUT_BUF];
  uint8_t duty[OUTPUT_BUF];
  uint16_t last;
  uint16_t rawbuf[RAW_BUF];
  decode_results capture;

  explicit IRsendTest(uint16_t x, bool i = false, bool j = true)
      : IRsend(x, i, j) {
    reset();
  }

  void reset() {
    last = 0;
    for (uint16_t i = 0; i < OUTPUT_BUF; i++) output[i] = 0;
    for (uint16_t i = 0; i < RAW_BUF; i++) rawbuf[i] = 0;
  }

  std::string outputStr() {
    std::stringstream result;
    uint8_t lastduty = UINT8_MAX;  // An impossible duty cycle value.
    uint32_t lastfreq = 0;  // An impossible frequency value.
    if (last == 0 && output[0] == 0) return "";
    for (uint16_t i = 0; i <= last; i++) {
      // Display the frequency only if it changes.
      if (freq[i] != lastfreq) {
        result << "f";
        result << freq[i];
        lastfreq = freq[i];
      }
      // Display the duty cycle only if it changes.
      if (duty[i] != lastduty) {
        result << "d";
        result << static_cast<uint16_t>(duty[i]);
        lastduty = duty[i];
      }
      if ((i & 1) != outputOff)  // Odd XOR outputOff
        result << "s";
      else
        result << "m";
      result << output[i];
    }
    reset();
    return result.str();
  }

  void makeDecodeResult(uint16_t offset = 0) {
    capture.decode_type = UNKNOWN;
    capture.bits = 0;
    capture.rawlen = last + 2 - offset;
    capture.overflow = (last - offset >= (int16_t)RAW_BUF);
    capture.repeat = false;
    capture.address = 0;
    capture.command = 0;
    capture.value = 0;
    capture.rawbuf = rawbuf;
    for (uint16_t i = 0; (i < RAW_BUF - 1) && (offset < OUTPUT_BUF);
         i++, offset++)
      if (output[offset] / kRawTick > UINT16_MAX)
        rawbuf[i + 1] = UINT16_MAX;
      else
        rawbuf[i + 1] = output[offset] / kRawTick;
  }

  void dumpRawResult() {
    std::cout << std::dec;
    if (capture.rawlen == 0) return;
    std::cout << "uint16_t rawbuf[" << capture.rawlen - 1 << "] = {";
    for (uint16_t i = 1; i < capture.rawlen; i++) {
      if (i % 8 == 1) std::cout << std::endl << "    ";
      std::cout << (capture.rawbuf[i] * kRawTick);
      // std::cout << "(" << capture.rawbuf[i] << ")";
      if (i < capture.rawlen - 1) std::cout << ", ";
    }
    std::cout << "};" << std::endl;
  }

  void addGap(uint32_t usecs) { space(usecs); }

  uint16_t mark(uint16_t usec) {
    IRtimer::add(usec);
    if (last >= OUTPUT_BUF) return 0;
    if (last & 1)  // Is odd? (i.e. last call was a space())
      output[++last] = usec;
    else
      output[last] += usec;
    duty[last] = _dutycycle;
    freq[last] = _freq_unittest;
    return 0;
  }

  void space(uint32_t time) {
    IRtimer::add(time);
    if (last >= OUTPUT_BUF) return;
    if (last & 1) {  // Is odd? (i.e. last call was a space())
      output[last] += time;
    } else {
      output[++last] = time;
    }
    duty[last] = _dutycycle;
    freq[last] = _freq_unittest;
  }
};

#ifdef UNIT_TEST
class IRsendLowLevelTest : public IRsend {
 public:
  std::string low_level_sequence;

  explicit IRsendLowLevelTest(uint16_t x, bool i = false, bool j = true)
      : IRsend(x, i, j) {
    reset();
  }

  void reset() { low_level_sequence = ""; }

 protected:
  void _delayMicroseconds(uint32_t usec) {
    _IRtimer_unittest_now += usec;
    std::ostringstream Convert;
    Convert << usec;
    low_level_sequence += Convert.str() + "usecs";
  }

  void ledOff() { low_level_sequence += "[Off]"; }

  void ledOn() { low_level_sequence += "[On]"; }
};
#endif  // UNIT_TEST

#endif  // TEST_IRSEND_TEST_H_
