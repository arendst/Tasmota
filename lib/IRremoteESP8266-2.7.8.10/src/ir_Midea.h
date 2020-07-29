// Copyright 2017 David Conran

/// @file
/// @brief Support for Midea protocols.
/// Midea added by crankyoldgit & bwze
/// @see https://docs.google.com/spreadsheets/d/1TZh4jWrx4h9zzpYUI9aYXMl1fYOiqu-xVuOOMqagxrs/edit?usp=sharing

// Supports:
//   Brand: Pioneer System,  Model: RYBO12GMFILCAD A/C (12K BTU) (MIDEA)
//   Brand: Pioneer System,  Model: RUBO18GMFILCAD A/C (18K BTU) (MIDEA)
//   Brand: Comfee, Model: MPD1-12CRN7 A/C (MIDEA)
//   Brand: Keystone, Model: RG57H4(B)BGEF remote (MIDEA)
//   Brand: Midea,  Model: FS40-7AR Stand Fan (MIDEA24)
//   Brand: Danby,  Model: DAC080BGUWDB (MIDEA)
//   Brand: Danby,  Model: DAC100BGUWDB (MIDEA)
//   Brand: Danby,  Model: DAC120BGUWDB (MIDEA)
//   Brand: Danby,  Model: R09C/BCGE remote (MIDEA)

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

#if DANBY_DAC
    kSwingVToggleStr = kIonStr;
#endif

// Constants
const uint8_t kMideaACTempOffset = 24;
const uint8_t kMideaACTempSize = 5;  // Bits
const uint8_t kMideaACMinTempF = 62;  // Fahrenheit
const uint8_t kMideaACMaxTempF = 86;  // Fahrenheit
const uint8_t kMideaACMinTempC = 17;  // Celsius
const uint8_t kMideaACMaxTempC = 30;  // Celsius
const uint8_t kMideaACCelsiusOffset = 29;
const uint8_t kMideaACModeOffset = 32;
const uint8_t kMideaACCool = 0;     // 0b000
const uint8_t kMideaACDry = 1;      // 0b001
const uint8_t kMideaACAuto = 2;     // 0b010
const uint8_t kMideaACHeat = 3;     // 0b011
const uint8_t kMideaACFan = 4;      // 0b100
const uint8_t kMideaACFanOffset = 35;
const uint8_t kMideaACFanSize = 2;  // Bits
const uint8_t kMideaACFanAuto = 0;  // 0b00
const uint8_t kMideaACFanLow = 1;   // 0b01
const uint8_t kMideaACFanMed = 2;   // 0b10
const uint8_t kMideaACFanHigh = 3;  // 0b11
const uint8_t kMideaACSleepOffset = 38;
const uint8_t kMideaACPowerOffset = 39;
const uint64_t kMideaACToggleSwingV = 0x0000A201FFFFFF7C;
// For Danby DAC unit, the Ionizer toggle is the same as ToggleSwingV
// const uint64_t kMideaACToggleIonizer = 0x0000A201FFFFFF7C;
const uint64_t kMideaACToggleEcono = 0x0000A202FFFFFF7E;

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

// Classes
/// Class for handling detailed Midea A/C messages.
/// @warning Consider this very alpha code.
class IRMideaAC {
 public:
  explicit IRMideaAC(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
  void stateReset(void);
#if SEND_MIDEA
  void send(const uint16_t repeat = kMideaMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
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
  bool isEconoToggle(void);
  void setEconoToggle(const bool on);
  bool getEconoToggle(void);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint64_t remote_state;  ///< The state of the IR remote in IR code form.
  bool _SwingVToggle;
  bool _EconoToggle;
  void checksum(void);
  static uint8_t calcChecksum(const uint64_t state);
};

#endif  // IR_MIDEA_H_
