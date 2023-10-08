// Copyright 2018 Erdem U. Altinyurt
// Copyright 2019 David Conran

/// @file
/// @brief Support for Vestel protocols.
/// Vestel added by Erdem U. Altinyurt

// Supports:
//   Brand: Vestel,  Model: BIOX CXP-9 A/C (9K BTU)

#ifndef IR_VESTEL_H_
#define IR_VESTEL_H_

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

/// Native representation of a Vestel A/C message.
union VestelProtocol{
  struct {
    uint64_t cmdState;
    uint64_t timeState;
  };
  struct {
    // Command
    uint64_t Signature  :12;  // 0x201
    uint64_t CmdSum     :8;
    uint64_t Swing      :4;  // auto 0xA, stop 0xF
    uint64_t TurboSleep :4;  // normal 0x1, sleep 0x3, turbo 0x7
    uint64_t            :8;
    uint64_t Temp       :4;
    uint64_t Fan        :4;
    uint64_t Mode       :3;
    uint64_t            :3;
    uint64_t Ion        :1;
    uint64_t            :1;
    uint64_t Power      :2;
    uint64_t UseCmd     :1;
    uint64_t            :0;
    // Time
    uint64_t            :12;
    uint64_t TimeSum    :8;
    uint64_t OffTenMins :3;
    uint64_t OffHours   :5;
    uint64_t OnTenMins  :3;
    uint64_t OnHours    :5;
    uint64_t Hours      :5;
    uint64_t OnTimer    :1;
    uint64_t OffTimer   :1;
    uint64_t Timer      :1;
    uint64_t Minutes    :8;
    uint64_t            :0;
  };
};

// Constants
const uint16_t kVestelAcHdrMark = 3110;
const uint16_t kVestelAcHdrSpace = 9066;
const uint16_t kVestelAcBitMark = 520;
const uint16_t kVestelAcOneSpace = 1535;
const uint16_t kVestelAcZeroSpace = 480;
const uint16_t kVestelAcTolerance = 30;

const uint8_t kVestelAcMinTempH = 16;
const uint8_t kVestelAcMinTempC = 18;
const uint8_t kVestelAcMaxTemp = 30;

const uint8_t kVestelAcAuto = 0;
const uint8_t kVestelAcCool = 1;
const uint8_t kVestelAcDry = 2;
const uint8_t kVestelAcFan = 3;
const uint8_t kVestelAcHeat = 4;

const uint8_t kVestelAcFanAuto = 1;
const uint8_t kVestelAcFanLow = 5;
const uint8_t kVestelAcFanMed = 9;
const uint8_t kVestelAcFanHigh = 0xB;
const uint8_t kVestelAcFanAutoCool = 0xC;
const uint8_t kVestelAcFanAutoHot = 0xD;

const uint8_t kVestelAcNormal = 1;
const uint8_t kVestelAcSleep = 3;
const uint8_t kVestelAcTurbo = 7;
const uint8_t kVestelAcIon = 4;
const uint8_t kVestelAcSwing = 0xA;

// Default states
const uint64_t kVestelAcStateDefault = 0x0F00D9001FEF201ULL;
const uint64_t kVestelAcTimeStateDefault = 0x201ULL;

// Classes
/// Class for handling detailed Vestel A/C messages.
class IRVestelAc {
 public:
  explicit IRVestelAc(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset(void);
#if SEND_VESTEL_AC
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_VESTEL_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setAuto(const int8_t autoLevel);
  void setTimer(const uint16_t minutes);
  uint16_t getTimer(void) const;
  void setTime(const uint16_t minutes);
  uint16_t getTime(void) const;
  void setOnTimer(const uint16_t minutes);
  uint16_t getOnTimer(void) const;
  void setOffTimer(const uint16_t minutes);
  uint16_t getOffTimer(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setRaw(const uint8_t* newState);
  void setRaw(const uint64_t newState);
  uint64_t getRaw(void);
  static bool validChecksum(const uint64_t state);
  void setSwing(const bool on);
  bool getSwing(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setIon(const bool on);
  bool getIon(void) const;
  bool isTimeCommand(void) const;
  bool isOnTimerActive(void) const;
  void setOnTimerActive(const bool on);
  bool isOffTimerActive(void) const;
  void setOffTimerActive(const bool on);
  bool isTimerActive(void) const;
  void setTimerActive(const bool on);
  static uint8_t calcChecksum(const uint64_t state);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
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
  VestelProtocol _;
  void checksum(void);
};

#endif  // IR_VESTEL_H_
