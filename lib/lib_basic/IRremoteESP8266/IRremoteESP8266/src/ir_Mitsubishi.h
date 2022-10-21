// Copyright 2009 Ken Shirriff
// Copyright 2017-2021 David Conran
// Copyright 2019 Mark Kuchel

/// @file
/// @brief Support for Mitsubishi protocols.
/// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
/// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran
/// @see GlobalCache's Control Tower's Mitsubishi TV data.
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Mitsubishi.cpp
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/441
/// @see https://github.com/r45635/HVAC-IR-Control/blob/master/HVAC_ESP8266/HVAC_ESP8266.ino#L84
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/619
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/888
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/947
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1398
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1399
/// @see https://github.com/kuchel77

// Supports:
//   Brand: Mitsubishi,  Model: TV (MITSUBISHI)
//   Brand: Mitsubishi,  Model: HC3000 Projector (MITSUBISHI2)
//   Brand: Mitsubishi,  Model: MS-GK24VA A/C
//   Brand: Mitsubishi,  Model: KM14A 0179213 remote
//   Brand: Mitsubishi Electric,  Model: PEAD-RP71JAA Ducted A/C (MITSUBISHI136)
//   Brand: Mitsubishi Electric,  Model: 001CP T7WE10714 remote (MITSUBISHI136)
//   Brand: Mitsubishi Electric,  Model: MSH-A24WV A/C (MITSUBISHI112)
//   Brand: Mitsubishi Electric,  Model: MUH-A24WV A/C (MITSUBISHI112)
//   Brand: Mitsubishi Electric,  Model: KPOA remote (MITSUBISHI112)
//   Brand: Mitsubishi Electric,  Model: MLZ-RX5017AS A/C (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: SG153/M21EDF426 remote (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: MSZ-GV2519 A/C (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: RH151/M21ED6426 remote (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: MSZ-SF25VE3 A/C (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: SG15D remote (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: MSZ-ZW4017S A/C (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: MSZ-FHnnVE A/C (MITSUBISHI_AC)
//   Brand: Mitsubishi Electric,  Model: RH151 remote (MITSUBISHI_AC)

#ifndef IR_MITSUBISHI_H_
#define IR_MITSUBISHI_H_

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

/// Native representation of a Mitsubishi 144-bit A/C message.
union Mitsubishi144Protocol{
  uint8_t raw[kMitsubishiACStateLength];  ///< The state in code form.
  struct {
    // Byte 0~4
    uint8_t pad0[5];
    // Byte 5
    uint8_t       :5;
    uint8_t Power :1;
    uint8_t       :2;
    // Byte 6
    uint8_t       :3;
    uint8_t Mode  :3;
    uint8_t ISee : 1;
    uint8_t       :1;
    // Byte 7
    uint8_t Temp       :4;
    uint8_t HalfDegree :1;
    uint8_t            :3;
    // Byte 8
    uint8_t         :4;
    uint8_t WideVane:4;  // SwingH
    // Byte 9
    uint8_t Fan     :3;
    uint8_t Vane    :3;  // SwingV or VaneRight
    uint8_t VaneBit :1;
    uint8_t FanAuto :1;
    // Byte 10
    uint8_t Clock   :8;
    // Byte 11
    uint8_t StopClock :8;
    // Byte 12
    uint8_t StartClock:8;
    // Byte 13
    uint8_t Timer       :3;
    uint8_t WeeklyTimer :1;
    uint8_t             :4;
    // Byte 14
    uint8_t               :5;
    uint8_t Ecocool       :1;
    uint8_t               :2;
    // Byte 15
    uint8_t DirectIndirect:2;
    uint8_t AbsenseDetect :1;
    uint8_t               :2;
    uint8_t iSave10C      :1;  // i-SAVE:mode=Heat & iSave=on AND 10C on remote
    uint8_t               :2;
    // Byte 16
    uint8_t               :1;
    uint8_t NaturalFlow   :1;
    uint8_t               :1;
    uint8_t VaneLeft      :3;  // SwingV(Left)
    uint8_t               :2;
    // Byte 17
    uint8_t Sum   :8;
  };
};

// Constants
const uint8_t kMitsubishiAcAuto = 0b100;
const uint8_t kMitsubishiAcCool = 0b011;
const uint8_t kMitsubishiAcDry =  0b010;
const uint8_t kMitsubishiAcHeat = 0b001;
const uint8_t kMitsubishiAcFan  = 0b111;
const uint8_t kMitsubishiAcFanAuto = 0;
const uint8_t kMitsubishiAcFanMax = 5;
const uint8_t kMitsubishiAcFanRealMax = 4;
const uint8_t kMitsubishiAcFanSilent = 6;
const uint8_t kMitsubishiAcFanQuiet = kMitsubishiAcFanSilent;
const float   kMitsubishiAcMinTemp = 16.0;  // 16C
const float   kMitsubishiAcMaxTemp = 31.0;  // 31C
const uint8_t kMitsubishiAcVaneAuto    = 0b000;  // Vanes move when AC wants to.
const uint8_t kMitsubishiAcVaneHighest = 0b001;
const uint8_t kMitsubishiAcVaneHigh    = 0b010;
const uint8_t kMitsubishiAcVaneMiddle  = 0b011;
const uint8_t kMitsubishiAcVaneLow     = 0b100;
const uint8_t kMitsubishiAcVaneLowest  = 0b101;
const uint8_t kMitsubishiAcVaneSwing   = 0b111;  // Vanes move all the time.
const uint8_t kMitsubishiAcVaneAutoMove = kMitsubishiAcVaneSwing;  // Deprecated
const uint8_t kMitsubishiAcWideVaneLeftMax  = 0b0001;  // 1
const uint8_t kMitsubishiAcWideVaneLeft     = 0b0010;  // 2
const uint8_t kMitsubishiAcWideVaneMiddle   = 0b0011;  // 3
const uint8_t kMitsubishiAcWideVaneRight    = 0b0100;  // 4
const uint8_t kMitsubishiAcWideVaneRightMax = 0b0101;  // 5
const uint8_t kMitsubishiAcWideVaneWide     = 0b0110;  // 6
const uint8_t kMitsubishiAcWideVaneAuto     = 0b1000;  // 8
const uint8_t kMitsubishiAcDirectOff    = 0b00;  // Vanes move when AC wants to.
const uint8_t kMitsubishiAcIndirect     = 0b01;
const uint8_t kMitsubishiAcDirect       = 0b11;
const uint8_t kMitsubishiAcNoTimer = 0;
const uint8_t kMitsubishiAcStartTimer = 5;
const uint8_t kMitsubishiAcStopTimer = 3;
const uint8_t kMitsubishiAcStartStopTimer = 7;

/// Native representation of a Mitsubishi 136-bit A/C message.
union Mitsubishi136Protocol{
  uint8_t raw[kMitsubishi136StateLength];  ///< The state in code form.
  struct {
    // Byte 0~4
    uint8_t pad[5];
    // Byte 5
    uint8_t       :6;
    uint8_t Power :1;
    uint8_t       :1;
    // Byte 6
    uint8_t Mode  :3;
    uint8_t       :1;
    uint8_t Temp  :4;
    // Byte 7
    uint8_t         :1;
    uint8_t Fan     :2;
    uint8_t         :1;
    uint8_t SwingV  :4;
  };
};

const uint8_t kMitsubishi136PowerByte = 5;
const uint8_t kMitsubishi136MinTemp = 17;  // 17C
const uint8_t kMitsubishi136MaxTemp = 30;  // 30C
const uint8_t kMitsubishi136Fan =             0b000;
const uint8_t kMitsubishi136Cool =            0b001;
const uint8_t kMitsubishi136Heat =            0b010;
const uint8_t kMitsubishi136Auto =            0b011;
const uint8_t kMitsubishi136Dry =             0b101;
const uint8_t kMitsubishi136SwingVLowest =   0b0000;
const uint8_t kMitsubishi136SwingVLow =      0b0001;
const uint8_t kMitsubishi136SwingVHigh =     0b0010;
const uint8_t kMitsubishi136SwingVHighest =  0b0011;
const uint8_t kMitsubishi136SwingVAuto =     0b1100;
const uint8_t kMitsubishi136FanMin =          0b00;
const uint8_t kMitsubishi136FanLow =          0b01;
const uint8_t kMitsubishi136FanMed =          0b10;
const uint8_t kMitsubishi136FanMax =          0b11;
const uint8_t kMitsubishi136FanQuiet = kMitsubishi136FanMin;

/// Native representation of a Mitsubishi 112-bit A/C message.
union Mitsubishi112Protocol{
  uint8_t raw[kMitsubishi112StateLength];  ///< The state in code form.
  struct {
    // Byte 0~4
    uint8_t pad0[5];
    // Byte 5
    uint8_t       :2;
    uint8_t Power :1;
    uint8_t       :5;
    // Byte 6
    uint8_t Mode  :3;
    uint8_t       :5;
    // Byte 7
    uint8_t Temp  :4;
    uint8_t       :4;
    // Byte 8
    uint8_t Fan     :3;
    uint8_t SwingV  :3;
    uint8_t         :2;
    // Byte 9~11
    uint8_t pad1[3];
    // Byte 12
    uint8_t         :2;
    uint8_t SwingH  :4;
    uint8_t         :2;
    // Byte 13
    uint8_t Sum :8;
  };
};

const uint8_t kMitsubishi112Cool =                        0b011;
const uint8_t kMitsubishi112Heat =                        0b001;
const uint8_t kMitsubishi112Auto =                        0b111;
const uint8_t kMitsubishi112Dry =                         0b010;

const uint8_t kMitsubishi112MinTemp = 16;  // 16C
const uint8_t kMitsubishi112MaxTemp = 31;  // 31C

const uint8_t kMitsubishi112FanMin =                     0b010;
const uint8_t kMitsubishi112FanLow =                     0b011;
const uint8_t kMitsubishi112FanMed =                     0b101;
const uint8_t kMitsubishi112FanMax =                     0b000;
const uint8_t kMitsubishi112FanQuiet = kMitsubishi112FanMin;
const uint8_t kMitsubishi112SwingVLowest =               0b101;
const uint8_t kMitsubishi112SwingVLow =                  0b100;
const uint8_t kMitsubishi112SwingVMiddle =               0b011;
const uint8_t kMitsubishi112SwingVHigh =                 0b010;
const uint8_t kMitsubishi112SwingVHighest =              0b001;
const uint8_t kMitsubishi112SwingVAuto =                 0b111;

const uint8_t kMitsubishi112SwingHLeftMax =              0b0001;
const uint8_t kMitsubishi112SwingHLeft =                 0b0010;
const uint8_t kMitsubishi112SwingHMiddle =               0b0011;
const uint8_t kMitsubishi112SwingHRight =                0b0100;
const uint8_t kMitsubishi112SwingHRightMax =             0b0101;
const uint8_t kMitsubishi112SwingHWide =                 0b1000;
const uint8_t kMitsubishi112SwingHAuto =                 0b1100;

// Legacy defines (Deprecated)
#define MITSUBISHI_AC_VANE_AUTO_MOVE kMitsubishiAcVaneAutoMove
#define MITSUBISHI_AC_VANE_AUTO kMitsubishiAcVaneAuto
#define MITSUBISHI_AC_MIN_TEMP kMitsubishiAcMinTemp
#define MITSUBISHI_AC_MAX_TEMP kMitsubishiAcMaxTemp
#define MITSUBISHI_AC_HEAT kMitsubishiAcHeat
#define MITSUBISHI_AC_FAN_SILENT kMitsubishiAcFanSilent
#define MITSUBISHI_AC_FAN_REAL_MAX kMitsubishiAcFanRealMax
#define MITSUBISHI_AC_FAN_MAX kMitsubishiAcFanMax
#define MITSUBISHI_AC_FAN_AUTO kMitsubishiAcFanAuto
#define MITSUBISHI_AC_DRY kMitsubishiAcDry
#define MITSUBISHI_AC_COOL kMitsubishiAcCool
#define MITSUBISHI_AC_AUTO kMitsubishiAcAuto


/// Class for handling detailed Mitsubishi 144-bit A/C messages.
/// @note Inspired and derived from the work done at: https://github.com/r45635/HVAC-IR-Control
/// @warning Consider this very alpha code. Seems to work, but not validated.
class IRMitsubishiAC {
 public:
  explicit IRMitsubishiAC(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);
  void stateReset(void);
  static bool validChecksum(const uint8_t* data);
#if SEND_MITSUBISHI_AC
  void send(const uint16_t repeat = kMitsubishiACMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MITSUBISHI_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const float degrees);
  float getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setISave10C(const bool state);
  bool getISave10C(void) const;
  void setISee(const bool state);
  bool getISee(void) const;
  void setDirectIndirect(const uint8_t position);
  uint8_t getDirectIndirect(void) const;
  void setEcocool(const bool state);
  bool getEcocool(void) const;
  void setAbsenseDetect(const bool state);
  bool getAbsenseDetect(void) const;
  void setNaturalFlow(const bool state);
  bool getNaturalFlow(void) const;
  void setVane(const uint8_t position);  // controls RIGHT vane on some models
  uint8_t getVane(void) const;
  void setVaneLeft(const uint8_t position);
  uint8_t getVaneLeft(void) const;
  void setWideVane(const uint8_t position);
  uint8_t getWideVane(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t* data);
  uint8_t getClock(void) const;
  void setClock(const uint8_t clock);
  uint8_t getStartClock(void) const;
  void setStartClock(const uint8_t clock);
  uint8_t getStopClock(void) const;
  void setStopClock(const uint8_t clock);
  uint8_t getTimer(void) const;
  void setTimer(const uint8_t timer);
  bool getWeeklyTimerEnabled(void) const;
  void setWeeklyTimerEnabled(const bool on);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
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
  Mitsubishi144Protocol _;
  void checksum(void);
  static uint8_t calculateChecksum(const uint8_t* data);
};

/// Class for handling detailed Mitsubishi 136-bit A/C messages.
class IRMitsubishi136 {
 public:
  explicit IRMitsubishi136(const uint16_t pin, const bool inverted = false,
                           const bool use_modulation = true);
  void stateReset(void);
#if SEND_MITSUBISHI136
  void send(const uint16_t repeat = kMitsubishi136MinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MITSUBISHI136
  void begin(void);
  static bool validChecksum(const uint8_t* data,
                            const uint16_t len = kMitsubishi136StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwingV(const uint8_t position);
  uint8_t getSwingV(void) const;
  void setQuiet(const bool on);
  bool getQuiet(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t* data);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
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
  Mitsubishi136Protocol _;
  void checksum(void);
};

/// Class for handling detailed Mitsubishi 122-bit A/C messages.
class IRMitsubishi112 {
 public:
  explicit IRMitsubishi112(const uint16_t pin, const bool inverted = false,
                           const bool use_modulation = true);
  void stateReset(void);
#if SEND_MITSUBISHI112
  void send(const uint16_t repeat = kMitsubishi112MinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MITSUBISHI112
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwingV(const uint8_t position);
  uint8_t getSwingV(void) const;
  void setSwingH(const uint8_t position);
  uint8_t getSwingH(void) const;
  void setQuiet(const bool on);
  bool getQuiet(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t* data);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
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
  Mitsubishi112Protocol _;
  void checksum(void);
};

#endif  // IR_MITSUBISHI_H_
