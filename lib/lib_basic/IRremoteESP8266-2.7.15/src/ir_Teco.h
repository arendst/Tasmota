// Copyright 2019 Fabien Valthier

/// @file
/// @brief Support for Teco protocols.

// Supports:
//   Brand: Alaska,  Model: SAC9010QC A/C
//   Brand: Alaska,  Model: SAC9010QC remote

#ifndef IR_TECO_H_
#define IR_TECO_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a Teco A/C message.
union TecoProtocol{
  uint64_t raw;  ///< The state of the IR remote in IR code form.
  struct {
    uint8_t Mode  :3;
    uint8_t Power :1;
    uint8_t Fan   :2;
    uint8_t Swing :1;
    uint8_t Sleep :1;
    uint8_t Temp  :4;
    uint8_t HalfHour  :1;
    uint8_t TensHours :2;  // number of 10hours
    uint8_t TimerOn   :1;
    uint8_t UnitHours :4;  // unit, not thenth
    uint8_t Humid :1;
    uint8_t Light :1;
    uint8_t :1;  // "Tree with bubbles" / Filter?? (Not Implemented)
    uint8_t Save  :1;
    uint8_t :8;  // Cst 0x50
    uint8_t :8;  // Cst 0x02
  };
};

// Constants.
const uint8_t kTecoAuto = 0;  // temp = 25C
const uint8_t kTecoCool = 1;
const uint8_t kTecoDry = 2;  // temp = 25C, but not shown
const uint8_t kTecoFan = 3;
const uint8_t kTecoHeat = 4;
const uint8_t kTecoFanAuto = 0;  // 0b00
const uint8_t kTecoFanLow = 1;   // 0b01
const uint8_t kTecoFanMed = 2;   // 0b10
const uint8_t kTecoFanHigh = 3;  // 0b11
const uint8_t kTecoMinTemp = 16;   // 16C
const uint8_t kTecoMaxTemp = 30;   // 30C

const uint64_t kTecoReset =      0b01001010000000000000010000000000000;

// Classes
/// Class for handling detailed Teco A/C messages.
class IRTecoAc {
 public:
  explicit IRTecoAc(const uint16_t pin, const bool inverted = false,
                    const bool use_modulation = true);
  void stateReset(void);
#if SEND_TECO
  void send(const uint16_t repeat = kTecoDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TECO
  void begin(void);
  void on(void);
  void off(void);

  void setPower(const bool on);
  bool getPower(void) const;

  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;

  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;

  void setSwing(const bool on);
  bool getSwing(void) const;

  void setSleep(const bool on);
  bool getSleep(void) const;

  void setLight(const bool on);
  bool getLight(void) const;

  void setHumid(const bool on);
  bool getHumid(void) const;

  void setSave(const bool on);
  bool getSave(void) const;

  uint16_t getTimer(void) const;
  void setTimer(const uint16_t mins);

  uint64_t getRaw(void) const;
  void setRaw(const uint64_t new_code);

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
  TecoProtocol _;
  bool getTimerEnabled(void) const;
};

#endif  // IR_TECO_H_
