// Copyright 2019 David Conran

/// @file
/// @brief Amcor A/C protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/834
/// @remark Kudos to ldellus; For the breakdown and mapping of the bit values.
// Supports:
//   Brand: Amcor,  Model: ADR-853H A/C
//   Brand: Amcor,  Model: TAC-495 remote
//   Brand: Amcor,  Model: TAC-444 remote

#ifndef IR_AMCOR_H_
#define IR_AMCOR_H_

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

// state[1]
const uint8_t kAmcorModeFanByte = 1;
// Fan Control
const uint8_t kAmcorFanMin =   0b001;
const uint8_t kAmcorFanMed =   0b010;
const uint8_t kAmcorFanMax =   0b011;
const uint8_t kAmcorFanAuto =  0b100;
const uint8_t kAmcorFanOffset = 4;
const uint8_t kAmcorFanSize = 3;
// Modes
const uint8_t kAmcorCool =          0b001;
const uint8_t kAmcorHeat =          0b010;
const uint8_t kAmcorFan =           0b011;  // Aka "Vent"
const uint8_t kAmcorDry =           0b100;
const uint8_t kAmcorAuto =          0b101;
const uint8_t kAmcorModeOffset = 0;
const uint8_t kAmcorModeSize = 3;

// state[2]
const uint8_t kAmcorTempByte = 2;
// Temperature
const uint8_t kAmcorMinTemp = 12;  // Celsius
const uint8_t kAmcorMaxTemp = 32;  // Celsius
const uint8_t kAmcorTempOffset = 1;
const uint8_t kAmcorTempSize = 6;  // Bits

// state[5]
// Power
const uint8_t kAmcorPowerByte = 5;
const uint8_t kAmcorPowerOffset = 4;
const uint8_t kAmcorPowerSize = 4;
const uint8_t kAmcorPowerOn =   0b0011;  // 0x3
const uint8_t kAmcorPowerOff =  0b1100;  // 0xC

// state[6]
const uint8_t kAmcorSpecialByte = 6;
// Max Mode (aka "Lo" in Cool and "Hi" in Heat)
const uint8_t kAmcorMax = 0b11;
const uint8_t kAmcorMaxOffset = 0;
const uint8_t kAmcorMaxSize = 2;

// "Vent" Mode
const uint8_t kAmcorVentOn =  0b11;
const uint8_t kAmcorVentOffset = 6;
const uint8_t kAmcorVentSize = 2;
// state[7]
// Checksum byte.
const uint8_t kAmcorChecksumByte = kAmcorStateLength - 1;

// Classes

/// Class for handling detailed Amcor A/C messages.
class IRAmcorAc {
 public:
  explicit IRAmcorAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);

  void stateReset();
#if SEND_AMCOR
  void send(const uint16_t repeat = kAmcorDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_AMCOR
  void begin();
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kAmcorStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kAmcorStateLength);
  void setPower(const bool state);
  bool getPower();
  void on();
  void off();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setMax(const bool on);
  bool getMax(void);
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  uint8_t* getRaw();
  void setRaw(const uint8_t state[]);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  /// @cond IGNORE
  IRsendTest _irsend;
  /// @endcond
#endif
  uint8_t remote_state[kAmcorStateLength];  // The state of the IR remote.
  void checksum(void);
};
#endif  // IR_AMCOR_H_
