// Copyright 2022 Daniele Gobbetti

/// @file
/// @brief Support for the York AC protocol (remote GRYLH2A)

// Supports:
//   Brand: York,  Model: MHH07P17 A/C
//   Brand: York,  Model: GRYLH2A remote

#ifndef IR_YORK_H_
#define IR_YORK_H_

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


/// Native representation of a York A/C message.
union YorkProtocol{
  uint8_t raw[kYorkStateLength];  ///< The state of the IR remote.
  struct {
    // byte 0-5
    uint8_t preamble[6];  // unknown, fixed 0x08, 0x10, 0x07, 0x02, 0x40, 0x08
    // byte 6
    uint8_t Key1     :4;  // key pressed on the remote: 1 power,  2 temp up, 3
                          // temp down...
    uint8_t Key2     :4;  // only set when setting ontime/offtime:
                          // Key1 value is 0x6 (enter key) and Key2 is 0x3 for
                          // "start" and 0x2 for "stop"
    // byte 7
    uint8_t Fan      :4;  // Fan speed: 1 low, 2 mid, 3 max, 8 auto
    uint8_t Power    :1;  // main unit power: 1 on, 0 off
    uint8_t          :3;
    // byte 8
    uint8_t Mode     :4;  // 1 heat, 2 cool, 3 dry, 4 fan, 8 auto
    uint8_t          :4;
    // byte 9
    uint8_t          :2;
    uint8_t Temp     :6;  // Degrees Celsius
    // byte 10
    uint8_t OffTimer :8;  // Power off time: 10s of minutes from now
    // byte 11
    uint8_t OnTimer  :8;  // Power on time: 10s of minutes from now
    // byte 12
    uint8_t          :8;  // unknown, normally 0x00, could be 0x08 when ontime
                          // set, 0x88 if both on and offtime set, 0x60 if
                          // sleep mode set
    // byte 13
    uint8_t SwingV   :1;  // 0 off, 1 on
    uint8_t          :7;
    // byte 14
    uint8_t          :8;  // checksum preamble, fixed 0xEC
    // byte 15-16
    uint8_t Chk1     :8;  // checksum, algorithm CRC-16/ARC, first byte
    uint8_t Chk2     :8;  // checksum, algorithm CRC-16/ARC, second byte
  };
};

// Constants
const uint8_t kYorkKnownGoodState[kYorkStateLength] = {
    0x08, 0x10, 0x07, 0x02, 0x40, 0x08,
    0x03, 0x18, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00,
    0xEC,
    0xF5, 0xF2};  // Mode "Heat", Fan Speed "auto", Temp: 24, Power: on

// Temperature
const uint8_t kYorkMinTemp = 18;  // Celsius
const uint8_t kYorkMaxTemp = 32;  // Celsius
// Fan
const uint8_t kYorkFanLow = 1;
const uint8_t kYorkFanMedium = 2;
const uint8_t kYorkFanHigh = 3;
const uint8_t kYorkFanAuto = 8;
// Modes
const uint8_t kYorkHeat = 1;
const uint8_t kYorkCool = 2;
const uint8_t kYorkDry = 3;
const uint8_t kYorkFan = 4;
const uint8_t kYorkAuto = 8;

// Classes
/// Class for handling detailed York A/C messages.
class IRYorkAc {
 public:
  explicit IRYorkAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset();
#if SEND_YORK
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_YORK
  void begin();
  void setPowerToggle(const bool on);
  bool getPowerToggle() const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp() const;
  void setFan(const uint8_t speed);
  uint8_t getFan() const;
  void setMode(const uint8_t mode);
  uint8_t getMode() const;
  uint16_t getOnTimer(void) const;
  uint16_t getOffTimer(void) const;
  void setOnTimer(const uint16_t mins);
  void setOffTimer(const uint16_t mins);
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kYorkStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  void calcChecksum();
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString() const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  YorkProtocol _;
};
#endif  // IR_YORK_H_
