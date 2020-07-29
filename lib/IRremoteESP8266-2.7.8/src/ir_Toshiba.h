// Copyright 2017 David Conran

/// @file
/// @brief Support for Toshiba protocols.
/// @see https://github.com/r45635/HVAC-IR-Control
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L77
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1205
/// @see https://docs.google.com/spreadsheets/d/1yidE2fvaO9kpCHfKafIdH31q4uaskYR1OwwrkyOxbp0/edit?usp=drivesdk
/// @see https://www.toshiba-carrier.co.jp/global/about/index.htm
/// @see http://www.toshiba-carrier.co.th/AboutUs/Pages/CompanyProfile.aspx

// Supports:
//   Brand: Toshiba,  Model: RAS-B13N3KV2
//   Brand: Toshiba,  Model: Akita EVO II
//   Brand: Toshiba,  Model: RAS-B13N3KVP-E
//   Brand: Toshiba,  Model: RAS 18SKP-ES
//   Brand: Toshiba,  Model: WH-TA04NE
//   Brand: Toshiba,  Model: WC-L03SE
//   Brand: Carrier,  Model: 42NQV060M2 / 38NYV060M2 A/C
//   Brand: Carrier,  Model: 42NQV050M2 / 38NYV050M2 A/C
//   Brand: Carrier,  Model: 42NQV035M2 / 38NYV035M2 A/C
//   Brand: Carrier,  Model: 42NQV025M2 / 38NYV025M2 A/C

#ifndef IR_TOSHIBA_H_
#define IR_TOSHIBA_H_

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

// Constants
// Byte[0] - 0xF2
// Byte[1] - 0x0D (inverted previous byte's value)
// Byte[2] - The expected payload length (in bytes) past the Byte[4].
const uint8_t kToshibaAcLengthByte = 2;  ///< Byte pos of the "length" attribute
const uint8_t kToshibaAcMinLength = 6;  ///< Min Nr. of bytes in a message.
///< Known lengths are:
///<   1 (56 bit message)
///<   3 (72 bit message)
///<   4 (80 bit message)
// Byte[3] - The bit-inverted value of the "length" byte.
const uint16_t kToshibaAcInvertedLength = 4;  ///< Nr. of leading bytes in
                                              ///< inverted pairs.
// Byte[4]
const uint8_t kToshibaAcShortMsgBit = 5;  ///< Mask 0b00x00000
const uint8_t kToshibaAcLongMsgBit = 3;   ///< Mask 0b00001000
// Byte[5]
const uint8_t kToshibaAcSwingOffset = 0;     ///< Bit offset.
const uint8_t kToshibaAcSwingSize = 2;       ///< Mask 0b000000xx
const uint8_t kToshibaAcSwingStep = 0;       ///<            0b00
const uint8_t kToshibaAcSwingOn = 1;         ///<            0b01
const uint8_t kToshibaAcSwingOff = 2;        ///<            0b10

const uint8_t kToshibaAcTempOffset = 4;  ///< Bit offset.
const uint8_t kToshibaAcTempSize = 4;  ///< Mask 0bxxxx0000
const uint8_t kToshibaAcMinTemp = 17;  ///< 17C
const uint8_t kToshibaAcMaxTemp = 30;  ///< 30C
// Byte[6]
const uint8_t kToshibaAcModeOffset = 0;
const uint8_t kToshibaAcModeSize = 3;  // Mask 0b00000xxx
const uint8_t kToshibaAcAuto = 0;                // 0b000
const uint8_t kToshibaAcCool = 1;                // 0b001
const uint8_t kToshibaAcDry =  2;                // 0b010
const uint8_t kToshibaAcHeat = 3;                // 0b011
const uint8_t kToshibaAcFan =  4;                // 0b100
const uint8_t kToshibaAcOff =  7;                // 0b111
const uint8_t kToshibaAcFanOffset = 5;
const uint8_t kToshibaAcFanSize = 3;   // Mask 0bxxx00000
const uint8_t kToshibaAcFanAuto = 0;   //      0b000
const uint8_t kToshibaAcFanMin =  1;   //      0b001
const uint8_t kToshibaAcFanMed =  3;   //      0b011
const uint8_t kToshibaAcFanMax =  5;   //      0b101
// Byte[8] (Checksum for 72 bit messages, Eco/Turbo for long 80 bit messages)
const uint8_t kToshibaAcEcoTurboOffset = 0;
const uint8_t kToshibaAcEcoTurboSize = 2;  // Mask 0b000000xx
const uint8_t kToshibaAcTurboOn = 1;       //            0b01
const uint8_t kToshibaAcEconoOn = 3;       //            0b11
// Byte[last] - Checksum (xor)

// Legacy defines. (Deperecated)
#define TOSHIBA_AC_AUTO kToshibaAcAuto
#define TOSHIBA_AC_COOL kToshibaAcCool
#define TOSHIBA_AC_DRY kToshibaAcDry
#define TOSHIBA_AC_HEAT kToshibaAcHeat
#define TOSHIBA_AC_POWER kToshibaAcPower
#define TOSHIBA_AC_FAN_AUTO kToshibaAcFanAuto
#define TOSHIBA_AC_FAN_MAX kToshibaAcFanMax
#define TOSHIBA_AC_MIN_TEMP kToshibaAcMinTemp
#define TOSHIBA_AC_MAX_TEMP kToshibaAcMaxTemp

// Classes
/// Class for handling detailed Toshiba A/C messages.
class IRToshibaAC {
 public:
  explicit IRToshibaAC(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset(void);
#if SEND_TOSHIBA_AC
  void send(const uint16_t repeat = kToshibaACMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TOSHIBA_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setTurbo(const bool on);
  bool getTurbo(void);
  void setEcono(const bool on);
  bool getEcono(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(const bool raw = false);
  void setRaw(const uint8_t newState[]);
  uint8_t* getRaw(void);
  static uint16_t getInternalStateLength(const uint8_t state[],
                                         const uint16_t size);
  uint16_t getStateLength(void);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kToshibaACStateLength);
  uint8_t getSwing(const bool raw = true);
  void setSwing(const uint8_t setting);
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
  uint8_t remote_state[kToshibaACStateLengthLong];  ///< The state in code form.
  uint8_t backup[kToshibaACStateLengthLong];  ///< A backup copy of the state.
  uint8_t prev_mode;  ///< Store of the previously set mode.
  bool _send_swing;  ///< Flag indicating if we need to send a swing message.
  uint8_t _swing_mode;  ///< The saved swing state/mode/command.
  void checksum(const uint16_t length = kToshibaACStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kToshibaACStateLength);
  void setStateLength(const uint16_t size);
  void _backupState(void);
  void _restoreState(void);
};

#endif  // IR_TOSHIBA_H_
