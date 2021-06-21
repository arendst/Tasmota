// Copyright 2021 David Conran (crankyoldgit)

/// @file
/// @brief Support for Truma protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1440
/// @see https://docs.google.com/spreadsheets/d/1k-RHu0vSIB6IweiTZSa3Rxy3Z_qPUtqwcqot8uXVO6I/edit?usp=sharing

// Supports:
//   Brand: Truma,  Model: Aventa A/C
//   Brand: Truma,  Model: 40091-86700 remote

#ifndef IR_TRUMA_H_
#define IR_TRUMA_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a Truma A/C message.
union TrumaProtocol{
  uint64_t raw;  ///< Remote state in IR code form.
  struct {
    // Byte 0 (least significant byte)
    uint8_t :8;  // fixed value (0x81)
    // Byte 1
    uint8_t Mode     :2;
    uint8_t PowerOff :1;
    uint8_t Fan      :3;
    uint8_t          :2;  // fixed value (0b11)
    // Byte 2
    uint8_t Temp:5;  ///< Temp in DegC minus 10(DEC).
    uint8_t :3;
    // Byte 3
    uint8_t :8;  // fixed value (0xFF)
    // Byte 4
    uint8_t :8;  // fixed value (0xFF)
    // Byte 5
    uint8_t :8;  // fixed value (0xFF)
    // Byte 6
    uint8_t Sum:8;  ///< Checksum value
  };
};

// Constants
const uint64_t kTrumaDefaultState = 0x50FFFFFFE6E781;  ///< Off, Auto, 16C, High
const uint8_t kTrumaChecksumInit = 5;

const uint8_t kTrumaAuto = 0;  // 0b00
const uint8_t kTrumaCool = 2;  // 0b10
const uint8_t kTrumaFan = 3;   // 0b11

const uint8_t kTrumaFanQuiet = 3;  // 0b011
const uint8_t kTrumaFanHigh = 4;   // 0b100
const uint8_t kTrumaFanMed = 5;    // 0b101
const uint8_t kTrumaFanLow = 6;    // 0b110

const uint8_t kTrumaTempOffset = 10;
const uint8_t kTrumaMinTemp = 16;
const uint8_t kTrumaMaxTemp = 31;


// Class
/// Class for handling detailed Truma A/C messages.
class IRTrumaAc {
 public:
  explicit IRTrumaAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
#if SEND_TRUMA
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TRUMA
  void begin(void);
  void stateReset(void);

  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;

  void setTemp(const uint8_t celsius);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;

  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);

  void setQuiet(const bool on);
  bool getQuiet(void) const;

  uint64_t getRaw(void);
  void setRaw(const uint64_t state);
  static bool validChecksum(const uint64_t state);
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
  TrumaProtocol _;
  uint8_t _lastfan;  // Last user chosen/valid fan speed.
  uint8_t _lastmode;  // Last user chosen operation mode.
  static uint8_t calcChecksum(const uint64_t state);
  void checksum(void);
};

#endif  // IR_TRUMA_H_
