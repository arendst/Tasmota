// Copyright 2017 David Conran
// Midea

// Supports:
//   Brand: Pioneer System,  Model: RYBO12GMFILCAD A/C (12K BTU)
//   Brand: Pioneer System,  Model: RUBO18GMFILCAD A/C (18K BTU)
//   Brand: Comfee, Model: MPD1-12CRN7 A/C
//   Brand: Keystone, Model: RG57H4(B)BGEF remote

#ifndef IR_MIDEA_H_
#define IR_MIDEA_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

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
const uint8_t kMideaACMinTempC = 17;  // Celsius
const uint8_t kMideaACMaxTempC = 30;  // Celsius
const uint64_t kMideaACStateMask =    0x0000FFFFFFFFFFFF;
const uint64_t kMideaACCelsiusBit =   0x0000000020000000;
const uint64_t kMideaACTempMask =     0x0000FFFFE0FFFFFF;
const uint64_t kMideaACFanMask =      0x0000FFC7FFFFFFFF;
const uint64_t kMideaACModeMask =     0x0000FFF8FFFFFFFF;
const uint64_t kMideaACChecksumMask = 0x0000FFFFFFFFFF00;
const uint64_t kMideaACToggleSwingV = 0x0000A201FFFFFF7C;

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
  explicit IRMideaAC(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);

  void stateReset(void);
#if SEND_MIDEA
  void send(const uint16_t repeat = kMideaMinRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MIDEA
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  bool getUseCelsius(void);
  void setUseCelsius(const bool celsius);
  void setTemp(const uint8_t temp, const bool useCelsius = false);
  uint8_t getTemp(const bool useCelsius = false);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setRaw(const uint64_t newState);
  uint64_t getRaw(void);
  static bool validChecksum(const uint64_t state);
  void setSleep(const bool on);
  bool getSleep(void);
  bool isSwingVToggle(void);
  void setSwingVToggle(const bool on);
  bool getSwingVToggle(void);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  uint64_t remote_state;
  bool _SwingVToggle;
  void checksum(void);
  static uint8_t calcChecksum(const uint64_t state);
};

#endif  // IR_MIDEA_H_
