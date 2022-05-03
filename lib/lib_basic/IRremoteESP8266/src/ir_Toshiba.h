// Copyright 2017 David Conran

/// @file
/// @brief Support for Toshiba protocols.
/// @see https://github.com/r45635/HVAC-IR-Control
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266T.ino#L77
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1205
/// @see https://docs.google.com/spreadsheets/d/1yidE2fvaO9kpCHfKafIdH31q4uaskYR1OwwrkyOxbp0/edit?usp=drivesdk
/// @see https://www.toshiba-carrier.co.jp/global/about/index.htm
/// @see http://www.toshiba-carrier.co.th/AboutUs/Pages/CompanyProfile.aspx
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1692

// Supports:
//   Brand: Toshiba,  Model: RAS-B13N3KV2
//   Brand: Toshiba,  Model: Akita EVO II
//   Brand: Toshiba,  Model: RAS-B13N3KVP-E
//   Brand: Toshiba,  Model: RAS 18SKP-ES
//   Brand: Toshiba,  Model: WH-TA04NE
//   Brand: Toshiba,  Model: WC-L03SE
//   Brand: Toshiba,  Model: WH-UB03NJ remote
//   Brand: Toshiba,  Model: RAS-2558V A/C
//   Brand: Toshiba,  Model: WH-TA01JE remote
//   Brand: Toshiba,  Model: RAS-25SKVP2-ND A/C
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

/// Native representation of a Toshiba A/C message.
union ToshibaProtocol{
  uint8_t raw[kToshibaACStateLengthLong];  ///< The state in code form.
  struct {
    // Byte[0] - 0xF2
    uint8_t :8;
    // Byte[1] - 0x0D (inverted previous byte's value)
    uint8_t :8;
    // Byte[2] - The expected payload length (in bytes) past the Byte[4].
    ///< Known lengths are:
    ///<   1 (56 bit message)
    ///<   3 (72 bit message)
    ///<   4 (80 bit message)
    uint8_t Length   :8;
    // Byte[3] - The bit-inverted value of the "length" byte.
    uint8_t          :8;
    // Byte[4]
    uint8_t          :3;
    uint8_t LongMsg  :1;
    uint8_t          :1;
    uint8_t ShortMsg :1;
    uint8_t          :2;
    // Byte[5]
    uint8_t Swing    :3;
    uint8_t          :1;
    uint8_t Temp     :4;
    // Byte[6]
    uint8_t Mode     :3;
    uint8_t          :2;
    uint8_t Fan      :3;
    // Byte[7]
    uint8_t          :4;
    uint8_t Filter   :1;
    uint8_t          :3;

    // Byte[8]
    // (Checksum for 72 bit messages, Eco/Turbo for long 80 bit messages)
    uint8_t EcoTurbo :8;
  };
};

// Constants

const uint8_t kToshibaAcLengthByte = 2;  ///< Byte pos of the "length" attribute
const uint8_t kToshibaAcMinLength = 6;  ///< Min Nr. of bytes in a message.

const uint16_t kToshibaAcInvertedLength = 4;  ///< Nr. of leading bytes in
                                              ///< inverted pairs.

const uint8_t kToshibaAcSwingStep = 0;       ///<            0b000
const uint8_t kToshibaAcSwingOn = 1;         ///<            0b001
const uint8_t kToshibaAcSwingOff = 2;        ///<            0b010
const uint8_t kToshibaAcSwingToggle = 4;     ///<            0b100

const uint8_t kToshibaAcMinTemp = 17;  ///< 17C
const uint8_t kToshibaAcMaxTemp = 30;  ///< 30C

const uint8_t kToshibaAcAuto = 0;                // 0b000
const uint8_t kToshibaAcCool = 1;                // 0b001
const uint8_t kToshibaAcDry =  2;                // 0b010
const uint8_t kToshibaAcHeat = 3;                // 0b011
const uint8_t kToshibaAcFan =  4;                // 0b100
const uint8_t kToshibaAcOff =  7;                // 0b111
const uint8_t kToshibaAcFanAuto = 0;   //      0b000
const uint8_t kToshibaAcFanMin =  1;   //      0b001
const uint8_t kToshibaAcFanMed =  3;   //      0b011
const uint8_t kToshibaAcFanMax =  5;   //      0b101

const uint8_t kToshibaAcTurboOn = 1;       //            0b01
const uint8_t kToshibaAcEconoOn = 3;       //            0b11

// Legacy defines. (Deprecated)
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
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setFilter(const bool on);
  bool getFilter(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(const bool raw = false) const;
  void setRaw(const uint8_t newState[],
              const uint16_t length = kToshibaACStateLength);
  uint8_t* getRaw(void);
  static uint16_t getInternalStateLength(const uint8_t state[],
                                         const uint16_t size);
  uint16_t getStateLength(void) const;
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kToshibaACStateLength);
  uint8_t getSwing(const bool raw = true) const;
  void setSwing(const uint8_t setting);
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
  ToshibaProtocol _;
  uint8_t backup[kToshibaACStateLengthLong];  ///< A backup copy of the state.
  uint8_t _prev_mode;  ///< Store of the previously set mode.
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
