// Copyright 2020 Quentin Briollant

/// @file
/// @brief Support for Technibel protocol.

#ifndef IR_TECHNIBEL_H_
#define IR_TECHNIBEL_H_

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

// Supports:
//   Brand: Technibel,  Model: IRO PLUS

/// Native representation of a Technibel A/C message.
union TechnibelProtocol{
  uint64_t raw;  // The state of the IR remote.
  struct {
    uint8_t Sum     :8;
    uint8_t Footer  :8;
    uint8_t TimerHours  :5;
    uint8_t         :3;
    uint8_t Temp    :7;
    uint8_t         :1;
    uint8_t Fan     :3;
    uint8_t         :1;
    uint8_t Sleep   :1;
    uint8_t Swing   :1;
    uint8_t UseFah  :1;
    uint8_t TimerEnable :1;
    uint8_t Mode    :4;
    uint8_t FanChange   :1;
    uint8_t TempChange  :1;
    uint8_t TimerChange :1;
    uint8_t Power   :1;
    uint8_t Header  :8;
  };
};

// Constants

const uint8_t kTechnibelAcTimerHoursOffset = 16;
const uint8_t kTechnibelAcTimerMax = 24;

const uint8_t kTechnibelAcTempMinC = 16;  // Deg C
const uint8_t kTechnibelAcTempMaxC = 31;  // Deg C
const uint8_t kTechnibelAcTempMinF = 61;  // Deg F
const uint8_t kTechnibelAcTempMaxF = 88;  // Deg F

const uint8_t kTechnibelAcFanSize = 4;
const uint8_t kTechnibelAcFanLow =    0b0001;
const uint8_t kTechnibelAcFanMedium = 0b0010;
const uint8_t kTechnibelAcFanHigh =   0b0100;

const uint8_t kTechnibelAcCool = 0b0001;
const uint8_t kTechnibelAcDry =  0b0010;
const uint8_t kTechnibelAcFan =  0b0100;
const uint8_t kTechnibelAcHeat = 0b1000;

const uint8_t kTechnibelAcHeaderOffset = 48;
const uint8_t kTechnibelAcHeader = 0b00011000;

const uint64_t kTechnibelAcResetState = 0x180101140000EA;  ///<
///< Mode:Cool, Power:Off, fan:Low, temp:20, swing:Off, sleep:Off


// Classes
/// Class for handling detailed Technibel A/C messages.
class IRTechnibelAc {
 public:
  explicit IRTechnibelAc(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);
  void stateReset(void);
#if SEND_TECHNIBEL_AC
  void send(const uint16_t repeat = kTechnibelAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TECHNIBEL_AC
  void begin(void);
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPower(const bool on);
  bool getPower(void) const;
  void on(void);
  void off(void);
  void setTempUnit(const bool celsius);
  bool getTempUnit(void) const;
  void setTemp(const uint8_t temp, const bool fahrenheit = false);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(const bool on);
  bool getSwing(void) const;
  static bool convertSwing(const stdAc::swingv_t swing);
  static stdAc::swingv_t toCommonSwing(const bool swing);
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setTimerEnabled(const bool on);
  bool getTimerEnabled(void) const;
  void setTimer(const uint16_t nr_of_mins);
  uint16_t getTimer(void) const;
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
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  TechnibelProtocol _;
  uint8_t _saved_temp;  // The previously user requested temp value.
  uint8_t _saved_temp_units;  // The previously user requested temp units.
  void checksum(void);
};
#endif  // IR_TECHNIBEL_H_
