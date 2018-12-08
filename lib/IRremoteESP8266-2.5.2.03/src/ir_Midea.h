// Copyright 2017 David Conran
#ifndef IR_MIDEA_H_
#define IR_MIDEA_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//                  MM    MM IIIII DDDDD   EEEEEEE   AAA
//                  MMM  MMM  III  DD  DD  EE       AAAAA
//                  MM MM MM  III  DD   DD EEEEE   AA   AA
//                  MM    MM  III  DD   DD EE      AAAAAAA
//                  MM    MM IIIII DDDDDD  EEEEEEE AA   AA

// Midea added by crankyoldgit & bwze
// Ref:
//   https://docs.google.com/spreadsheets/d/1TZh4jWrx4h9zzpYUI9aYXMl1fYOiqu-xVuOOMqagxrs/edit?usp=sharing

// Constants
const uint8_t kMideaACCool = 0;     // 0b000
const uint8_t kMideaACDry = 1;      // 0b001
const uint8_t kMideaACAuto = 2;     // 0b010
const uint8_t kMideaACHeat = 3;     // 0b011
const uint8_t kMideaACFan = 4;      // 0b100
const uint8_t kMideaACFanAuto = 0;  // 0b000
const uint8_t kMideaACFanLow = 1;   // 0b001
const uint8_t kMideaACFanMed = 2;   // 0b010
const uint8_t kMideaACFanHigh = 3;  // 0b011
const uint64_t kMideaACPower = 1ULL << 39;
const uint64_t kMideaACSleep = 1ULL << 38;
const uint8_t kMideaACMinTempF = 62;  // Fahrenheit
const uint8_t kMideaACMaxTempF = 86;  // Fahrenheit
const uint8_t kMideaACMinTempC = 16;  // Celsius
const uint8_t kMideaACMaxTempC = 30;  // Celsius
const uint64_t kMideaACStateMask = 0x0000FFFFFFFFFFFF;
const uint64_t kMideaACTempMask = 0x0000FFFFE0FFFFFF;
const uint64_t kMideaACFanMask = 0x0000FFC7FFFFFFFF;
const uint64_t kMideaACModeMask = 0x0000FFF8FFFFFFFF;
const uint64_t kMideaACChecksumMask = 0x0000FFFFFFFFFF00;

// Legacy defines. (Deprecated)
#define MIDEA_AC_COOL kMideaACCool
#define MIDEA_AC_DRY kMideaACDry
#define MIDEA_AC_AUTO kMideaACAuto
#define MIDEA_AC_HEAT kMideaACHeat
#define MIDEA_AC_FAN kMideaACFan
#define MIDEA_AC_FAN_AUTO kMideaACFanAuto
#define MIDEA_AC_FAN_LOW kMideaACFanLow
#define MIDEA_AC_FAN_MED kMideaACFanMed
#define MIDEA_AC_FAN_HI kMideaACFanHigh
#define MIDEA_AC_POWER kMideaACPower
#define MIDEA_AC_SLEEP kMideaACSleep
#define MIDEA_AC_MIN_TEMP_F kMideaACMinTempF
#define MIDEA_AC_MAX_TEMP_F kMideaACMaxTempF
#define MIDEA_AC_MIN_TEMP_C kMideaACMinTempC
#define MIDEA_AC_MAX_TEMP_C kMideaACMaxTempC

class IRMideaAC {
 public:
  explicit IRMideaAC(uint16_t pin);

  void stateReset();
#if SEND_MIDEA
  void send();
#endif  // SEND_MIDEA
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp, const bool useCelsius = false);
  uint8_t getTemp(const bool useCelsius = false);
  void setFan(const uint8_t fan);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setRaw(uint64_t newState);
  uint64_t getRaw();
  static bool validChecksum(const uint64_t state);
  void setSleep(const bool state);
  bool getSleep();
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif
#ifndef UNIT_TEST

 private:
#endif
  uint64_t remote_state;
  void checksum();
  static uint8_t calcChecksum(const uint64_t state);
  IRsend _irsend;
};

#endif  // IR_MIDEA_H_
