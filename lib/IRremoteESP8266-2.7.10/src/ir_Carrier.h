// Copyright 2020 David Conran
/// @file
/// @brief Carrier A/C
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1127
/// @see https://docs.google.com/spreadsheets/d/1EZy78L0cn1KDIX1aKq2biptejFqCjD5HO3tLiRvXf48/edit#gid=0

// Supports:
//   Brand: Carrier/Surrey,  Model: 42QG5A55970 remote
//   Brand: Carrier/Surrey,  Model: 619EGX0090E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0120E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0180E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0220E0 A/C
//   Brand: Carrier/Surrey,  Model: 53NGK009/012 Inverter

#ifndef IR_CARRIER_H_
#define IR_CARRIER_H_

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

// CARRIER_AC64
const uint8_t kCarrierAc64ChecksumOffset = 16;
const uint8_t kCarrierAc64ChecksumSize = 4;
const uint8_t kCarrierAc64ModeOffset = kCarrierAc64ChecksumOffset +
                                       kCarrierAc64ChecksumSize;  // 20
const uint8_t kCarrierAc64ModeSize = 2;
const uint8_t kCarrierAc64Heat = 0b01;  // 1
const uint8_t kCarrierAc64Cool = 0b10;  // 2
const uint8_t kCarrierAc64Fan =  0b11;  // 3
const uint8_t kCarrierAc64FanOffset = kCarrierAc64ModeOffset +
                                      kCarrierAc64ModeSize;  // 22
const uint8_t kCarrierAc64FanSize = 2;
const uint8_t kCarrierAc64FanAuto =    0b00;  // 0
const uint8_t kCarrierAc64FanLow =     0b01;  // 1
const uint8_t kCarrierAc64FanMedium =  0b10;  // 2
const uint8_t kCarrierAc64FanHigh =    0b11;  // 3
const uint8_t kCarrierAc64TempOffset = kCarrierAc64FanOffset +
                                       kCarrierAc64FanSize;  // 24
const uint8_t kCarrierAc64TempSize = 4;
const uint8_t kCarrierAc64MinTemp = 16;  // Celsius
const uint8_t kCarrierAc64MaxTemp = 30;  // Celsius
const uint8_t kCarrierAc64SwingVOffset = kCarrierAc64TempOffset +
                                         kCarrierAc64TempSize + 1;  // 29
const uint8_t kCarrierAc64PowerOffset = kCarrierAc64SwingVOffset + 6 + 1;  // 36
const uint8_t kCarrierAc64OffTimerEnableOffset =
    kCarrierAc64PowerOffset + 1;  // 37
const uint8_t kCarrierAc64OnTimerEnableOffset =
    kCarrierAc64OffTimerEnableOffset + 1;  // 38
const uint8_t kCarrierAc64SleepOffset =
    kCarrierAc64OnTimerEnableOffset + 1;  // 39
const uint8_t kCarrierAc64TimerSize = 4;
const uint8_t kCarrierAc64TimerMax = 9;  // Hours.
const uint8_t kCarrierAc64TimerMin = 1;  // Hours.
const uint8_t kCarrierAc64OnTimerOffset =
    kCarrierAc64SleepOffset + 12 + 1;  // 52
const uint8_t kCarrierAc64OffTimerOffset = kCarrierAc64OnTimerOffset +
                                           kCarrierAc64TimerSize + 4;  // 60


// Classes

/// Class for handling detailed Carrier 64 bit A/C messages.
class IRCarrierAc64 {
 public:
  explicit IRCarrierAc64(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);

  void stateReset();
#if SEND_CARRIER_AC64
  void send(const uint16_t repeat = kCarrierAc64MinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_CARRIER_AC64
  void begin();
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPower(const bool on);
  bool getPower();
  void on();
  void off();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setSwingV(const bool on);
  bool getSwingV(void);
  void setSleep(const bool on);
  bool getSleep(void);
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOnTimer(void);
  void setOffTimer(const uint16_t nr_of_mins);
  uint16_t getOffTimer(void);
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
  uint64_t remote_state;  ///< The state of the IR remote.
  void checksum(void);
  void _cancelOnTimer(void);
  void _cancelOffTimer(void);
};
#endif  // IR_CARRIER_H_
