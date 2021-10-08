// Copyright 2018 David Conran

/// @file
/// @brief Support for Whirlpool protocols.
/// Decoding help from: \@redmusicxd, \@josh929800, \@raducostea
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/509
/// @note Smart, iFeel, AroundU, PowerSave, & Silent modes are unsupported.
///   Advanced 6thSense, Dehumidify, & Sleep modes are not supported.
/// @note Dim == !Light, Jet == Super == Turbo

// Supports:
//   Brand: Whirlpool,  Model: DG11J1-3A remote
//   Brand: Whirlpool,  Model: DG11J1-04 remote
//   Brand: Whirlpool,  Model: DG11J1-91 remote
//   Brand: Whirlpool,  Model: SPIS409L A/C
//   Brand: Whirlpool,  Model: SPIS412L A/C
//   Brand: Whirlpool,  Model: SPIW409L A/C
//   Brand: Whirlpool,  Model: SPIW412L A/C
//   Brand: Whirlpool,  Model: SPIW418L A/C

#ifndef IR_WHIRLPOOL_H_
#define IR_WHIRLPOOL_H_

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

/// Native representation of a Whirlpool A/C message.
union WhirlpoolProtocol{
  uint8_t raw[kWhirlpoolAcStateLength];  ///< The state in IR code form
  struct {
    // Byte 0~1
    uint8_t pad0[2];
    // Byte 2
    uint8_t Fan     :2;
    uint8_t Power   :1;
    uint8_t Sleep   :1;
    uint8_t         :3;
    uint8_t Swing1  :1;
    // Byte 3
    uint8_t Mode  :3;
    uint8_t       :1;
    uint8_t Temp  :4;
    // Byte 4
    uint8_t       :8;
    // Byte 5
    uint8_t         :4;
    uint8_t Super1  :1;
    uint8_t         :2;
    uint8_t Super2  :1;
    // Byte 6
    uint8_t ClockHours  :5;
    uint8_t LightOff    :1;
    uint8_t             :2;
    // Byte 7
    uint8_t ClockMins       :6;
    uint8_t                 :1;
    uint8_t OffTimerEnabled :1;
    // Byte 8
    uint8_t OffHours  :5;
    uint8_t           :1;
    uint8_t Swing2    :1;
    uint8_t           :1;
    // Byte 9
    uint8_t OffMins         :6;
    uint8_t                 :1;
    uint8_t OnTimerEnabled  :1;
    // Byte 10
    uint8_t OnHours :5;
    uint8_t         :3;
    // Byte 11
    uint8_t OnMins  :6;
    uint8_t         :2;
    // Byte 12
    uint8_t       :8;
    // Byte 13
    uint8_t Sum1  :8;
    // Byte 14
    uint8_t       :8;
    // Byte 15
    uint8_t Cmd   :8;
    // Byte 16~17
    uint8_t pad1[2];
    // Byte 18
    uint8_t       :3;
    uint8_t J191  :1;
    uint8_t       :4;
    // Byte 19
    uint8_t       :8;
    // Byte 20
    uint8_t Sum2  :8;
  };
};

// Constants
const uint8_t kWhirlpoolAcChecksumByte1 = 13;
const uint8_t kWhirlpoolAcChecksumByte2 = kWhirlpoolAcStateLength - 1;
const uint8_t kWhirlpoolAcHeat = 0;
const uint8_t kWhirlpoolAcAuto = 1;
const uint8_t kWhirlpoolAcCool = 2;
const uint8_t kWhirlpoolAcDry = 3;
const uint8_t kWhirlpoolAcFan = 4;
const uint8_t kWhirlpoolAcFanAuto = 0;
const uint8_t kWhirlpoolAcFanHigh = 1;
const uint8_t kWhirlpoolAcFanMedium = 2;
const uint8_t kWhirlpoolAcFanLow = 3;
const uint8_t kWhirlpoolAcMinTemp = 18;     // 18C (DG11J1-3A), 16C (DG11J1-91)
const uint8_t kWhirlpoolAcMaxTemp = 32;     // 32C (DG11J1-3A), 30C (DG11J1-91)
const uint8_t kWhirlpoolAcAutoTemp = 23;    // 23C
const uint8_t kWhirlpoolAcCommandLight = 0x00;
const uint8_t kWhirlpoolAcCommandPower = 0x01;
const uint8_t kWhirlpoolAcCommandTemp = 0x02;
const uint8_t kWhirlpoolAcCommandSleep = 0x03;
const uint8_t kWhirlpoolAcCommandSuper = 0x04;
const uint8_t kWhirlpoolAcCommandOnTimer = 0x05;
const uint8_t kWhirlpoolAcCommandMode = 0x06;
const uint8_t kWhirlpoolAcCommandSwing = 0x07;
const uint8_t kWhirlpoolAcCommandIFeel = 0x0D;
const uint8_t kWhirlpoolAcCommandFanSpeed = 0x11;
const uint8_t kWhirlpoolAcCommand6thSense = 0x17;
const uint8_t kWhirlpoolAcCommandOffTimer = 0x1D;

// Classes
/// Class for handling detailed Whirlpool A/C messages.
class IRWhirlpoolAc {
 public:
  explicit IRWhirlpoolAc(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);
  void stateReset(void);
#if SEND_WHIRLPOOL_AC
  void send(const uint16_t repeat = kWhirlpoolAcDefaultRepeat,
            const bool calcchecksum = true);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_WHIRLPOOL_AC
  void begin(void);
  void setPowerToggle(const bool on);
  bool getPowerToggle(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setSuper(const bool on);
  bool getSuper(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(const bool on);
  bool getSwing(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  uint16_t getClock(void) const;
  void setClock(const uint16_t minspastmidnight);
  uint16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t minspastmidnight);
  void enableOnTimer(const bool on);
  bool isOnTimerEnabled(void) const;
  uint16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t minspastmidnight);
  void enableOffTimer(const bool on);
  bool isOffTimerEnabled(void) const;
  void setCommand(const uint8_t code);
  uint8_t getCommand(void) const;
  whirlpool_ac_remote_model_t getModel(void) const;
  void setModel(const whirlpool_ac_remote_model_t model);
  uint8_t* getRaw(const bool calcchecksum = true);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kWhirlpoolAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kWhirlpoolAcStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  WhirlpoolProtocol _;
  uint8_t _desiredtemp;  ///< The last user explicitly set temperature.
  void checksum(const uint16_t length = kWhirlpoolAcStateLength);
  void _setTemp(const uint8_t temp, const bool remember = true);
  void _setMode(const uint8_t mode);
  int8_t getTempOffset(void) const;
};

#endif  // IR_WHIRLPOOL_H_
