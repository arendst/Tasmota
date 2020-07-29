// Copyright 2020 David Conran

/// @file
/// @brief Airwell "Manchester code" based protocol.
/// Some other Airwell products use the COOLIX protocol.

// Supports:
//   Brand: Airwell,  Model: RC08W remote
//   Brand: Airwell,  Model: RC04 remote
//   Brand: Airwell,  Model: DLS 21 DCI R410 AW A/C

#ifndef IR_AIRWELL_H_
#define IR_AIRWELL_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif


// Constants
const uint64_t kAirwellKnownGoodState = 0x140500002;  // Mode Fan, Speed 1, 25C
// Temperature
const uint8_t kAirwellMinTemp = 16;  // Celsius
const uint8_t kAirwellMaxTemp = 30;  // Celsius
const uint8_t kAirwellTempSize = 4;  // Bits
const uint8_t kAirwellTempOffset = 19;  // 0b1111 << 19
// Fan
const uint8_t kAirwellFanSize = 2;  // Bits
const uint8_t kAirwellFanOffset = 28;  // 0b11 << 28
const uint8_t kAirwellFanLow = 0;     // 0b00
const uint8_t kAirwellFanMedium = 1;  // 0b01
const uint8_t kAirwellFanHigh = 2;    // 0b10
const uint8_t kAirwellFanAuto = 3;    // 0b11
// Modes
const uint8_t kAirwellModeSize = 3;  // Bits
const uint8_t kAirwellModeOffset = 30;  // 0b111 << 30
const uint8_t kAirwellCool = 1;  // 0b001
const uint8_t kAirwellHeat = 2;  // 0b010
const uint8_t kAirwellAuto = 3;  // 0b011
const uint8_t kAirwellDry = 4;   // 0b100
const uint8_t kAirwellFan = 5;   // 0b101
// Power
const uint8_t kAirwellPowerToggleBit = 33;  // 0b1 << 33


// Classes
/// Class for handling detailed Airwell A/C messages.
class IRAirwellAc {
 public:
  explicit IRAirwellAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset();
#if SEND_AIRWELL
  void send(const uint16_t repeat = kAirwellMinRepeats);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_AIRWELL
  void begin();
  void setPowerToggle(const bool on);
  bool getPowerToggle();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  uint64_t getRaw();
  void setRaw(const uint64_t state);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  uint64_t remote_state;  // The state of the IR remote in native IR code form.
  void checksum(void);
};
#endif  // IR_AIRWELL_H_
