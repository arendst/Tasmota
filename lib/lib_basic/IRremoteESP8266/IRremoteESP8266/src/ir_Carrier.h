// Copyright 2020-2022 David Conran
/// @file
/// @brief Carrier A/C
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1127
/// @see https://docs.google.com/spreadsheets/d/1EZy78L0cn1KDIX1aKq2biptejFqCjD5HO3tLiRvXf48/edit#gid=0
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1797
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1943

// Supports:
//   Brand: Carrier/Surrey,  Model: 42QG5A55970 remote
//   Brand: Carrier/Surrey,  Model: 619EGX0090E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0120E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0180E0 A/C
//   Brand: Carrier/Surrey,  Model: 619EGX0220E0 A/C
//   Brand: Carrier/Surrey,  Model: 53NGK009/012 Inverter
//   Brand: Carrier,  Model: 40GKX0E2006 remote (CARRIER_AC128)
//   Brand: Carrier,  Model: 3021203 RR03-S-Remote (CARRIER_AC84)
//   Brand: Carrier,  Model: 342WM100CT A/C (CARRIER_AC84)

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

/// Native representation of a Carrier A/C message.
union CarrierProtocol {
  uint64_t raw;  ///< The state of the IR remote.
  struct {
    // Byte 0
    uint8_t :8;
    // Byte 1
    uint8_t :8;
    // Byte 2
    uint8_t Sum:4;
    uint8_t Mode:2;
    uint8_t Fan:2;
    // Byte 3
    uint8_t Temp:4;
    uint8_t :1;
    uint8_t SwingV:1;
    uint8_t :2;
    // Byte 4
    uint8_t :4;
    uint8_t Power:1;
    uint8_t OffTimerEnable:1;
    uint8_t OnTimerEnable:1;
    uint8_t Sleep:1;
    // Byte 5
    uint8_t :8;
    // Byte 6
    uint8_t :4;
    uint8_t OnTimer:4;
    // Byte 7
    uint8_t :4;
    uint8_t OffTimer:4;
  };
};

// Constants

// CARRIER_AC64
const uint8_t kCarrierAc64ChecksumOffset = 16;
const uint8_t kCarrierAc64ChecksumSize = 4;
const uint8_t kCarrierAc64Heat = 0b01;  // 1
const uint8_t kCarrierAc64Cool = 0b10;  // 2
const uint8_t kCarrierAc64Fan =  0b11;  // 3
const uint8_t kCarrierAc64FanAuto =    0b00;  // 0
const uint8_t kCarrierAc64FanLow =     0b01;  // 1
const uint8_t kCarrierAc64FanMedium =  0b10;  // 2
const uint8_t kCarrierAc64FanHigh =    0b11;  // 3
const uint8_t kCarrierAc64MinTemp = 16;  // Celsius
const uint8_t kCarrierAc64MaxTemp = 30;  // Celsius
const uint8_t kCarrierAc64TimerMax = 9;  // Hours.
const uint8_t kCarrierAc64TimerMin = 1;  // Hours.


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
  bool getPower(void) const;
  void on(void);
  void off(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setSwingV(const bool on);
  bool getSwingV(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOnTimer(void) const;
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
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  CarrierProtocol _;
  void checksum(void);
  void _cancelOnTimer(void);
  void _cancelOffTimer(void);
};
#endif  // IR_CARRIER_H_
