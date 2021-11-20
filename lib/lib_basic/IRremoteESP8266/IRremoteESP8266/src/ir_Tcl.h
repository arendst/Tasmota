// Copyright 2019, 2021 David Conran

/// @file
/// @brief Support for TCL protocols.

// Supports:
//   Brand: Leberg,  Model: LBS-TOR07 A/C (TAC09CHSD)
//   Brand: TCL,  Model: TAC-09CHSD/XA31I A/C (TAC09CHSD)
//   Brand: Teknopoint,  Model: Allegro SSA-09H A/C (GZ055BE1)
//   Brand: Teknopoint,  Model: GZ-055B-E1 remote (GZ055BE1)

#ifndef IR_TCL_H_
#define IR_TCL_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRrecv.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a TCL 112 A/C message.
union Tcl112Protocol{
  uint8_t raw[kTcl112AcStateLength];  ///< The State in IR code form.
  struct {
    // Byte 0~2
    uint8_t                 :8;
    uint8_t                 :8;
    uint8_t                 :8;
    // Byte 3
    uint8_t MsgType         :2;
    uint8_t                 :6;
    // Byte 4
    uint8_t                 :8;
    // Byte 5
    uint8_t                 :2;
    uint8_t Power           :1;
    uint8_t OffTimerEnabled :1;
    uint8_t OnTimerEnabled  :1;
    uint8_t Quiet           :1;
    uint8_t Light           :1;
    uint8_t Econo           :1;
    // Byte 6
    uint8_t Mode            :4;
    uint8_t Health          :1;
    uint8_t Turbo           :1;
    uint8_t                 :2;
    // Byte 7
    uint8_t Temp            :4;
    uint8_t                 :4;
    // Byte 8
    uint8_t Fan             :3;
    uint8_t SwingV          :3;
    uint8_t TimerIndicator  :1;
    uint8_t                 :1;
    // Byte 9
    uint8_t                 :1;  // 0
    uint8_t OffTimer        :6;
    uint8_t                 :1;  // 0
    // Byte 10
    uint8_t                 :1;  // 0
    uint8_t OnTimer         :6;
    uint8_t                 :1;  // 0
    // Byte 11
    uint8_t                 :8;  // 00000000
    // Byte 12
    uint8_t                 :3;
    uint8_t SwingH          :1;
    uint8_t                 :1;
    uint8_t HalfDegree      :1;
    uint8_t                 :1;
    uint8_t isTcl           :1;
    // Byte 13
    uint8_t Sum             :8;
  };
};

// Constants
const uint16_t kTcl112AcHdrMark = 3000;
const uint16_t kTcl112AcHdrSpace = 1650;
const uint16_t kTcl112AcBitMark = 500;
const uint16_t kTcl112AcOneSpace = 1050;
const uint16_t kTcl112AcZeroSpace = 325;
const uint32_t kTcl112AcGap = kDefaultMessageGap;  // Just a guess.
// Total tolerance percentage to use for matching the header mark.
const uint8_t kTcl112AcHdrMarkTolerance = 6;
const uint8_t kTcl112AcTolerance = 5;  // Extra Percentage for the rest.

const uint8_t kTcl112AcHeat = 1;
const uint8_t kTcl112AcDry =  2;
const uint8_t kTcl112AcCool = 3;
const uint8_t kTcl112AcFan =  7;
const uint8_t kTcl112AcAuto = 8;

const uint8_t kTcl112AcFanAuto = 0b000;
const uint8_t kTcl112AcFanMin  = 0b001;  // Aka. "Night"
const uint8_t kTcl112AcFanLow  = 0b010;
const uint8_t kTcl112AcFanMed  = 0b011;
const uint8_t kTcl112AcFanHigh = 0b101;
const uint8_t kTcl112AcFanNight = kTcl112AcFanMin;
const uint8_t kTcl112AcFanQuiet = kTcl112AcFanMin;

const float   kTcl112AcTempMax    = 31.0;
const float   kTcl112AcTempMin    = 16.0;

const uint8_t kTcl112AcSwingVOff =     0b000;
const uint8_t kTcl112AcSwingVHighest = 0b001;
const uint8_t kTcl112AcSwingVHigh =    0b010;
const uint8_t kTcl112AcSwingVMiddle =  0b011;
const uint8_t kTcl112AcSwingVLow =     0b100;
const uint8_t kTcl112AcSwingVLowest =  0b101;
const uint8_t kTcl112AcSwingVOn =      0b111;
// MsgType
const uint8_t kTcl112AcNormal  = 0b01;
const uint8_t kTcl112AcSpecial = 0b10;

// Classes
/// Class for handling detailed TCL A/C messages.
class IRTcl112Ac {
 public:
  explicit IRTcl112Ac(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
#if SEND_TCL112AC
  void send(const uint16_t repeat = kTcl112AcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TCL
  void begin(void);
  void stateReset(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kTcl112AcStateLength);
  tcl_ac_remote_model_t getModel(void) const;
  void setModel(const tcl_ac_remote_model_t model);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const float celsius);  // Celsius in 0.5 increments
  float getTemp(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  static uint8_t calcChecksum(uint8_t state[],
                              const uint16_t length = kTcl112AcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kTcl112AcStateLength);
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setHealth(const bool on);
  bool getHealth(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void) const;
  void setSwingVertical(const uint8_t setting);
  uint8_t getSwingVertical(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setQuiet(const bool on);
  bool getQuiet(const bool def = false) const;
  uint16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t mins);
  uint16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t mins);
  static bool isTcl(const uint8_t state[]);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t setting);
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
  Tcl112Protocol _;
  bool _quiet_prev;
  bool _quiet;
  bool _quiet_explictly_set;
  void checksum(const uint16_t length = kTcl112AcStateLength);
};

#endif  // IR_TCL_H_
