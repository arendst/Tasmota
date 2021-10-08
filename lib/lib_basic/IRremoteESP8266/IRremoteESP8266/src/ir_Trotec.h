// Copyright 2017 stufisher
// Copyright 2019 crankyoldgit

/// @file
/// @brief Support for Trotec protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/279
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1176

// Supports:
//   Brand: Trotec,  Model: PAC 3200 A/C (TROTEC)
//   Brand: Trotec,  Model: PAC 3550 Pro A/C (TROTEC_3550)
//   Brand: Duux,  Model: Blizzard Smart 10K / DXMA04 A/C (TROTEC)
// For Trotec Model PAC 3900 X, use the Midea protocol instead.

#ifndef IR_TROTEC_H_
#define IR_TROTEC_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a Trotec A/C message.
union TrotecProtocol{
  uint8_t raw[kTrotecStateLength];  ///< Remote state in IR code form.
  struct {
    // Byte 0
    uint8_t Intro1:8;  // fixed value
    // Byte 1
    uint8_t Intro2:8;  // fixed value
    // Byte 2
    uint8_t Mode  :2;
    uint8_t       :1;
    uint8_t Power :1;
    uint8_t Fan   :2;
    uint8_t       :2;
    // Byte 3
    uint8_t Temp  :4;
    uint8_t       :3;
    uint8_t Sleep :1;
    // Byte 4
    uint8_t       :8;
    // Byte 5
    uint8_t       :6;
    uint8_t Timer :1;
    uint8_t       :1;
    // Byte 6
    uint8_t Hours :8;
    // Byte 7
    uint8_t       :8;
    // Byte 8
    uint8_t Sum   :8;
  };
};

// Constants
const uint8_t kTrotecIntro1 = 0x12;
const uint8_t kTrotecIntro2 = 0x34;

const uint8_t kTrotecAuto = 0;
const uint8_t kTrotecCool = 1;
const uint8_t kTrotecDry = 2;
const uint8_t kTrotecFan = 3;

const uint8_t kTrotecFanLow = 1;
const uint8_t kTrotecFanMed = 2;
const uint8_t kTrotecFanHigh = 3;

const uint8_t kTrotecMinTemp = 18;
const uint8_t kTrotecDefTemp = 25;
const uint8_t kTrotecMaxTemp = 32;

const uint8_t kTrotecMaxTimer = 23;

/// Native representation of a Trotec 3550 A/C message.
union Trotec3550Protocol{
  uint8_t raw[kTrotecStateLength];  ///< Remote state in IR code form.
  struct {
    // Byte 0
    uint8_t Intro:    8;  // fixed value (0x55)
    // Byte 1
    uint8_t SwingV   :1;
    uint8_t Power    :1;
    uint8_t          :1;  // Unknown
    uint8_t TimerSet :1;
    uint8_t TempC    :4;  // Temp + kTrotec3550MinTempC for degC)
    // Byte 2
    uint8_t TimerHrs :4;
    uint8_t          :4;  // Unknown
    // Byte 3
    uint8_t TempF    :5;  // Temp + kTrotec3550MinTempF for degF)
    uint8_t          :3;  // Unknown
    // Byte 4
    uint8_t          :8;  // Unknown
    // Byte 5
    uint8_t          :8;  // Unknown
    // Byte 6
    uint8_t Mode     :2;
    uint8_t          :2;  // Unknown
    uint8_t Fan      :2;
    uint8_t          :2;  // Unknown
    // Byte 7
    uint8_t          :7;  // Unknown
    uint8_t Celsius  :1;  // DegC or DegF
    // Byte 8
    uint8_t Sum      :8;
  };
};

const uint8_t kTrotec3550MinTempC = 16;
const uint8_t kTrotec3550MaxTempC = 30;
const uint8_t kTrotec3550MinTempF = 59;
const uint8_t kTrotec3550MaxTempF = 86;

// Legacy defines. (Deprecated)
#define TROTEC_AUTO kTrotecAuto
#define TROTEC_COOL kTrotecCool
#define TROTEC_DRY kTrotecDry
#define TROTEC_FAN kTrotecFan
#define TROTEC_FAN_LOW kTrotecFanLow
#define TROTEC_FAN_MED kTrotecFanMed
#define TROTEC_FAN_HIGH kTrotecFanHigh
#define TROTEC_MIN_TEMP kTrotecMinTemp
#define TROTEC_MAX_TEMP kTrotecMaxTemp
#define TROTEC_MAX_TIMER kTrotecMaxTimer

// Class
/// Class for handling detailed Trotec A/C messages.
class IRTrotecESP {
 public:
  explicit IRTrotecESP(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
#if SEND_TROTEC
  void send(const uint16_t repeat = kTrotecDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TROTEC
  void begin(void);
  void stateReset(void);

  void on(void);
  void off(void);
  void setPower(const bool state);
  bool getPower(void) const;

  void setTemp(const uint8_t celsius);
  uint8_t getTemp(void) const;

  void setSpeed(const uint8_t fan);
  uint8_t getSpeed(void) const;

  void setFan(const uint8_t fan) { setSpeed(fan); }
  uint8_t getFan(void) const { return getSpeed(); }

  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);

  bool getSleep(void) const;
  void setSleep(const bool on);

  uint8_t getTimer(void) const;
  void setTimer(const uint8_t timer);

  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kTrotecStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kTrotecStateLength);
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
  TrotecProtocol _;
  void checksum(void);
};

// Class
/// Class for handling detailed Trotec 3550 A/C messages.
class IRTrotec3550 {
 public:
  explicit IRTrotec3550(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);
#if SEND_TROTEC_3550
  void send(const uint16_t repeat = kTrotecDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TROTEC_3550
  void begin(void);
  void stateReset(void);
  void on(void);
  void off(void);
  void setPower(const bool state);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees, const bool celsius = true);
  uint8_t getTemp(void) const;
  void setTempUnit(const bool celsius);
  bool getTempUnit(void) const;
  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;
  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);
  bool getSwingV(void) const;
  void setSwingV(const bool on);
  uint16_t getTimer(void) const;
  void setTimer(const uint16_t mins);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kTrotecStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kTrotecStateLength);
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
  Trotec3550Protocol _;
  void checksum(void);
};
#endif  // IR_TROTEC_H_
