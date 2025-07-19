// Copyright 2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for Airton protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1670

// Supports:
//   Brand: Airton,  Model: SMVH09B-2A2A3NH ref. 409730 A/C
//   Brand: Airton,  Model: RD1A1 remote

#ifndef IR_AIRTON_H_
#define IR_AIRTON_H_

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

/// Native representation of a Airton 56 A/C message.
/// @see https://docs.google.com/spreadsheets/d/1Kpq7WCkh85heLnTQGlwUfCR6eeu_vfBHvhii8wtP4LU/edit?usp=sharing
union AirtonProtocol{
  uint64_t raw;  ///< The state in code form.
  struct {  // Common
    // Byte 1 & 0 (LSB)
    uint16_t Header   :16;  // Header. (0x11D3)
    // Byte 2
    uint8_t Mode      :3;  // Operating Mode
    uint8_t Power     :1;  // Power Control
    uint8_t Fan       :3;
    uint8_t Turbo     :1;
    // Byte 3
    uint8_t Temp      :4;  // Degrees Celsius (+16 offset)
    uint8_t           :4;  // Unknown / Unused.
    // Byte 4
    uint8_t SwingV    :1;
    uint8_t           :7;  // Unknown / Unused.
    // Byte 5
    uint8_t Econo     :1;
    uint8_t Sleep     :1;
    uint8_t NotAutoOn :1;
    uint8_t           :1;  // Unknown / Unused.
    uint8_t HeatOn    :1;
    uint8_t           :1;  // Unknown / Unused.
    uint8_t Health    :1;
    uint8_t Light     :1;
    // Byte 6
    uint8_t Sum       :8;  // Sepecial checksum value
  };
};

// Constants
const uint8_t kAirtonAuto =    0b000;  // 0
const uint8_t kAirtonCool =    0b001;  // 1
const uint8_t kAirtonDry =     0b010;  // 2
const uint8_t kAirtonFan =     0b011;  // 3
const uint8_t kAirtonHeat =    0b100;  // 4

const uint8_t kAirtonFanAuto = 0b000;  // 0
const uint8_t kAirtonFanMin =  0b001;  // 1
const uint8_t kAirtonFanLow =  0b010;  // 2
const uint8_t kAirtonFanMed =  0b011;  // 3
const uint8_t kAirtonFanHigh = 0b100;  // 4
const uint8_t kAirtonFanMax =  0b101;  // 5

const uint8_t kAirtonMinTemp = 16;  // 16C
const uint8_t kAirtonMaxTemp = 25;  // 25C


/// Class for handling detailed Airton 56-bit A/C messages.
class IRAirtonAc {
 public:
  explicit IRAirtonAc(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset(void);
#if SEND_AIRTON
  void send(const uint16_t repeat = kAirtonDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_AIRTON
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  uint64_t getRaw(void);
  void setRaw(const uint64_t data);
  void setLight(const bool on);
  bool getLight(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setHealth(const bool on);
  bool getHealth(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setSwingV(const bool on);
  bool getSwingV(void) const;
  static bool validChecksum(const uint64_t data);
  static uint8_t calcChecksum(const uint64_t data);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  AirtonProtocol _;
  void checksum(void);
};
#endif  // IR_AIRTON_H_
