// Copyright 2020 David Conran

/// @file
/// @brief Delonghi A/C
/// @note Kudos to TheMaxxz For the breakdown and mapping of the bit values.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1096

// Supports:
//   Brand: Delonghi,  Model: PAC A95

#ifndef IR_DELONGHI_H_
#define IR_DELONGHI_H_

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

/// Native representation of a Delonghi A/C message.
union DelonghiProtocol{
  uint64_t raw;  ///< The state of the IR remote.
  struct {
    uint8_t           :8;  // Header
    uint8_t Temp      :5;
    uint8_t Fan       :2;
    uint8_t Fahrenheit:1;
    uint8_t Power     :1;
    uint8_t Mode      :3;
    uint8_t Boost     :1;
    uint8_t Sleep     :1;
    uint8_t           :2;
    uint8_t OnTimer   :1;
    uint8_t OnHours   :5;
    uint8_t           :2;
    uint8_t OnMins    :6;
    uint8_t           :2;
    uint8_t OffTimer  :1;
    uint8_t OffHours  :5;
    uint8_t           :2;
    uint8_t OffMins   :6;
    uint8_t           :2;
    uint8_t Sum       :8;
  };
};

// Constants
const uint8_t kDelonghiAcTempMinC = 18;  // Deg C
const uint8_t kDelonghiAcTempMaxC = 32;  // Deg C
const uint8_t kDelonghiAcTempMinF = 64;  // Deg F
const uint8_t kDelonghiAcTempMaxF = 90;  // Deg F
const uint8_t kDelonghiAcTempAutoDryMode = 0;
const uint8_t kDelonghiAcTempFanMode = 0b00110;
const uint8_t kDelonghiAcFanAuto =   0b00;
const uint8_t kDelonghiAcFanHigh =   0b01;
const uint8_t kDelonghiAcFanMedium = 0b10;
const uint8_t kDelonghiAcFanLow =    0b11;
const uint8_t kDelonghiAcCool =   0b000;
const uint8_t kDelonghiAcDry =    0b001;
const uint8_t kDelonghiAcFan =    0b010;
const uint8_t kDelonghiAcAuto =   0b100;
const uint16_t kDelonghiAcTimerMax = 23 * 60 + 59;
const uint8_t kDelonghiAcChecksumOffset = 56;

// Classes

/// Class for handling detailed Delonghi A/C messages.
class IRDelonghiAc {
 public:
  explicit IRDelonghiAc(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);
  void stateReset(void);
#if SEND_DELONGHI_AC
  void send(const uint16_t repeat = kDelonghiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_DELONGHI_AC
  void begin(void);
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPower(const bool on);
  bool getPower(void) const;
  void on(void);
  void off(void);
  void setTempUnit(const bool celsius);
  bool getTempUnit(void) const;
  void setTemp(const uint8_t temp, const bool fahrenheit = false,
               const bool force = false);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setBoost(const bool on);  // Aka Turbo
  bool getBoost(void) const;  // Aka Turbo
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setOnTimerEnabled(const bool on);
  bool getOnTimerEnabled(void) const;
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOnTimer(void) const;
  void setOffTimerEnabled(const bool on);
  bool getOffTimerEnabled(void) const;
  void setOffTimer(const uint16_t nr_of_mins);
  uint16_t getOffTimer(void) const;
  uint64_t getRaw(void);
  void setRaw(const uint64_t state);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  DelonghiProtocol _;
  uint8_t _saved_temp;  ///< The previously user requested temp value.
  uint8_t _saved_temp_units;  ///< The previously user requested temp units.
  void checksum(void);
};
#endif  // IR_DELONGHI_H_
