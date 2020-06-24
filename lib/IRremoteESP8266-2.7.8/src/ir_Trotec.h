// Copyright 2017 stufisher
// Copyright 2019 crankyoldgit

/// @file
/// @brief Support for Trotec protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/pull/279
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1176

// Supports:
//   Brand: Trotec,  Model: PAC 3200 A/C
//   Brand: Duux,  Model: Blizzard Smart 10K / DXMA04 A/C

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

// Constants
// Byte 0
const uint8_t kTrotecIntro1 = 0x12;

// Byte 1
const uint8_t kTrotecIntro2 = 0x34;

// Byte 2
const uint8_t kTrotecModeOffset = 0;
const uint8_t kTrotecModeSize = 2;  // Nr. of bits
const uint8_t kTrotecAuto = 0;
const uint8_t kTrotecCool = 1;
const uint8_t kTrotecDry = 2;
const uint8_t kTrotecFan = 3;

const uint8_t kTrotecPowerBitOffset = 3;

const uint8_t kTrotecFanOffset = 4;
const uint8_t kTrotecFanSize = 2;  // Nr. of bits
const uint8_t kTrotecFanLow = 1;
const uint8_t kTrotecFanMed = 2;
const uint8_t kTrotecFanHigh = 3;

// Byte 3
const uint8_t kTrotecTempOffset = 0;
const uint8_t kTrotecTempSize = 4;  // Nr. of bits
const uint8_t kTrotecMinTemp = 18;
const uint8_t kTrotecDefTemp = 25;
const uint8_t kTrotecMaxTemp = 32;
const uint8_t kTrotecSleepBitOffset = 7;

// Byte 5
const uint8_t kTrotecTimerBitOffset = 6;

// Byte 6
const uint8_t kTrotecMaxTimer = 23;

// Legacy defines. (Deperecated)
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
  bool getPower(void);

  void setTemp(const uint8_t celsius);
  uint8_t getTemp(void);

  void setSpeed(const uint8_t fan);
  uint8_t getSpeed(void);

  uint8_t getMode(void);
  void setMode(const uint8_t mode);

  bool getSleep(void);
  void setSleep(const bool on);

  uint8_t getTimer(void);
  void setTimer(const uint8_t timer);

  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kTrotecStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint8_t remote_state[kTrotecStateLength];  ///< Remote state in IR code form.
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kTrotecStateLength);
  void checksum(void);
};

#endif  // IR_TROTEC_H_
