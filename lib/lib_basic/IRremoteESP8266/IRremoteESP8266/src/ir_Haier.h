// Copyright 2018-2021 crankyoldgit
/// @file
/// @brief Support for Haier A/C protocols.
/// The specifics of reverse engineering the protocols details:
/// * HSU07-HEA03 by kuzin2006.
/// * YR-W02/HSU-09HMC203 by non7top.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/404
/// @see https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/485
/// @see https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1804

// Supports:
//   Brand: Haier,  Model: HSU07-HEA03 remote (HAIER_AC)
//   Brand: Haier,  Model: YR-W02 remote (HAIER_AC_YRW02)
//   Brand: Haier,  Model: HSU-09HMC203 A/C (HAIER_AC_YRW02)
//   Brand: Haier,  Model: V9014557 M47 8D remote (HAIER_AC176)
//   Brand: Mabe,   Model: MMI18HDBWCA6MI8 A/C (HAIER_AC176)
//   Brand: Mabe,   Model: V12843 HJ200223 remote (HAIER_AC176)
//   Brand: Daichi, Model: D-H A/C (HAIER_AC176)
//   Brand: Haier,  Model: KFR-26GW/83@UI-Ge A/C (HAIER_AC160)

#ifndef IR_HAIER_H_
#define IR_HAIER_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a Haier HSU07-HEA03 A/C message.
union HaierProtocol{
  ///< The state in native IR code form
  uint8_t remote_state[kHaierACStateLength];
  struct {
    // Byte 0
    uint8_t Prefix;
    // Byte 1
    uint8_t Command:4;
    uint8_t Temp   :4;
    // Byte 2
    uint8_t CurrHours:5;
    uint8_t unknown  :1;  // value=1
    uint8_t SwingV   :2;
    // Byte 3
    uint8_t CurrMins:6;
    uint8_t OffTimer:1;
    uint8_t OnTimer :1;
    // Byte 4
    uint8_t OffHours:5;
    uint8_t Health  :1;
    uint8_t :0;
    // Byte 5
    uint8_t OffMins:6;
    uint8_t Fan    :2;
    // Byte 6
    uint8_t OnHours:5;
    uint8_t Mode   :3;
    // Byte 7
    uint8_t OnMins:6;
    uint8_t Sleep :1;
    uint8_t :0;
    // Byte 8
    uint8_t Sum;
  };
};

// Constants

const uint8_t kHaierAcPrefix = 0b10100101;

const uint8_t kHaierAcMinTemp = 16;
const uint8_t kHaierAcDefTemp = 25;
const uint8_t kHaierAcMaxTemp = 30;
const uint8_t kHaierAcCmdOff =         0b0000;
const uint8_t kHaierAcCmdOn =          0b0001;
const uint8_t kHaierAcCmdMode =        0b0010;
const uint8_t kHaierAcCmdFan =         0b0011;
const uint8_t kHaierAcCmdTempUp =      0b0110;
const uint8_t kHaierAcCmdTempDown =    0b0111;
const uint8_t kHaierAcCmdSleep =       0b1000;
const uint8_t kHaierAcCmdTimerSet =    0b1001;
const uint8_t kHaierAcCmdTimerCancel = 0b1010;
const uint8_t kHaierAcCmdHealth =      0b1100;
const uint8_t kHaierAcCmdSwing =       0b1101;

const uint8_t kHaierAcSwingVOff =  0b00;
const uint8_t kHaierAcSwingVUp =   0b01;
const uint8_t kHaierAcSwingVDown = 0b10;
const uint8_t kHaierAcSwingVChg =  0b11;

const uint8_t kHaierAcAuto = 0;
const uint8_t kHaierAcCool = 1;
const uint8_t kHaierAcDry = 2;
const uint8_t kHaierAcHeat = 3;
const uint8_t kHaierAcFan = 4;

const uint8_t kHaierAcFanAuto = 0;
const uint8_t kHaierAcFanLow = 1;
const uint8_t kHaierAcFanMed = 2;
const uint8_t kHaierAcFanHigh = 3;

const uint16_t kHaierAcMaxTime = (23 * 60) + 59;

const uint8_t kHaierAcSleepBit = 0b01000000;

// Legacy Haier AC defines.
#define HAIER_AC_MIN_TEMP kHaierAcMinTemp
#define HAIER_AC_DEF_TEMP kHaierAcDefTemp
#define HAIER_AC_MAX_TEMP kHaierAcMaxTemp
#define HAIER_AC_CMD_OFF kHaierAcCmdOff
#define HAIER_AC_CMD_ON kHaierAcCmdOn
#define HAIER_AC_CMD_MODE kHaierAcCmdMode
#define HAIER_AC_CMD_FAN kHaierAcCmdFan
#define HAIER_AC_CMD_TEMP_UP kHaierAcCmdTempUp
#define HAIER_AC_CMD_TEMP_DOWN kHaierAcCmdTempDown
#define HAIER_AC_CMD_SLEEP kHaierAcCmdSleep
#define HAIER_AC_CMD_TIMER_SET kHaierAcCmdTimerSet
#define HAIER_AC_CMD_TIMER_CANCEL kHaierAcCmdTimerCancel
#define HAIER_AC_CMD_HEALTH kHaierAcCmdHealth
#define HAIER_AC_CMD_SWINGV kHaierAcCmdSwing
#define HAIER_AC_SWINGV_OFF kHaierAcSwingVOff
#define HAIER_AC_SWINGV_UP kHaierAcSwingVUp
#define HAIER_AC_SWINGV_DOWN kHaierAcSwingVDown
#define HAIER_AC_SWINGV_CHG kHaierAcSwingVChg
#define HAIER_AC_AUTO kHaierAcAuto
#define HAIER_AC_COOL kHaierAcCool
#define HAIER_AC_DRY kHaierAcDry
#define HAIER_AC_HEAT kHaierAcHeat
#define HAIER_AC_FAN kHaierAcFan
#define HAIER_AC_FAN_AUTO kHaierAcFanAuto
#define HAIER_AC_FAN_LOW kHaierAcFanLow
#define HAIER_AC_FAN_MED kHaierAcFanMed
#define HAIER_AC_FAN_HIGH kHaierAcFanHigh

const uint8_t kHaierAcYrw02MinTempC = 16;
const uint8_t kHaierAcYrw02MaxTempC = 30;
const uint8_t kHaierAcYrw02MinTempF = 60;
const uint8_t kHaierAcYrw02MaxTempF = 86;
const uint8_t kHaierAcYrw02DefTempC = 25;

const uint8_t kHaierAcYrw02ModelA = 0xA6;
const uint8_t kHaierAcYrw02ModelB = 0x59;
const uint8_t kHaierAc176Prefix = 0xB7;
const uint8_t kHaierAc160Prefix = 0xB5;

const uint8_t kHaierAcYrw02SwingVOff = 0x0;
const uint8_t kHaierAcYrw02SwingVTop = 0x1;
const uint8_t kHaierAcYrw02SwingVMiddle = 0x2;  // Not available in heat mode.
const uint8_t kHaierAcYrw02SwingVBottom = 0x3;  // Only available in heat mode.
const uint8_t kHaierAcYrw02SwingVDown = 0xA;
const uint8_t kHaierAcYrw02SwingVAuto = 0xC;  // Airflow

const uint8_t kHaierAc160SwingVOff =     0b0000;
const uint8_t kHaierAc160SwingVTop =     0b0001;
const uint8_t kHaierAc160SwingVHighest = 0b0010;
const uint8_t kHaierAc160SwingVHigh =    0b0100;
const uint8_t kHaierAc160SwingVMiddle =  0b0110;
const uint8_t kHaierAc160SwingVLow =     0b1000;
const uint8_t kHaierAc160SwingVLowest =  0b0011;
const uint8_t kHaierAc160SwingVAuto =    0b1100;  // Airflow

const uint8_t kHaierAcYrw02SwingHMiddle = 0x0;
const uint8_t kHaierAcYrw02SwingHLeftMax = 0x3;
const uint8_t kHaierAcYrw02SwingHLeft = 0x4;
const uint8_t kHaierAcYrw02SwingHRight = 0x5;
const uint8_t kHaierAcYrw02SwingHRightMax = 0x6;
const uint8_t kHaierAcYrw02SwingHAuto = 0x7;

const uint8_t kHaierAcYrw02FanHigh = 0b001;
const uint8_t kHaierAcYrw02FanMed =  0b010;
const uint8_t kHaierAcYrw02FanLow =  0b011;
const uint8_t kHaierAcYrw02FanAuto = 0b101;  // HAIER_AC176 uses `0` in Fan2

const uint8_t kHaierAcYrw02Auto = 0b000;  // 0
const uint8_t kHaierAcYrw02Cool = 0b001;  // 1
const uint8_t kHaierAcYrw02Dry =  0b010;  // 2
const uint8_t kHaierAcYrw02Heat = 0b100;  // 4
const uint8_t kHaierAcYrw02Fan =  0b110;  // 5

const uint8_t kHaierAcYrw02ButtonTempUp =   0b00000;
const uint8_t kHaierAcYrw02ButtonTempDown = 0b00001;
const uint8_t kHaierAcYrw02ButtonSwingV =   0b00010;
const uint8_t kHaierAcYrw02ButtonSwingH =   0b00011;
const uint8_t kHaierAcYrw02ButtonFan =      0b00100;
const uint8_t kHaierAcYrw02ButtonPower =    0b00101;
const uint8_t kHaierAcYrw02ButtonMode =     0b00110;
const uint8_t kHaierAcYrw02ButtonHealth =   0b00111;
const uint8_t kHaierAcYrw02ButtonTurbo =    0b01000;
const uint8_t kHaierAcYrw02ButtonSleep =    0b01011;
const uint8_t kHaierAcYrw02ButtonTimer =    0b10000;
const uint8_t kHaierAcYrw02ButtonLock =     0b10100;
const uint8_t kHaierAc160ButtonLight =      0b10101;
const uint8_t kHaierAc160ButtonAuxHeating = 0b10110;
const uint8_t kHaierAc160ButtonClean =      0b11001;
const uint8_t kHaierAcYrw02ButtonCFAB =     0b11010;

const uint8_t kHaierAcYrw02NoTimers       = 0b000;
const uint8_t kHaierAcYrw02OffTimer       = 0b001;
const uint8_t kHaierAcYrw02OnTimer        = 0b010;
const uint8_t kHaierAcYrw02OnThenOffTimer = 0b100;
const uint8_t kHaierAcYrw02OffThenOnTimer = 0b101;

/// Native representation of a Haier 176 bit A/C message.
union HaierAc176Protocol{
  uint8_t raw[kHaierAC176StateLength];  ///< The state in native form
  struct {
    // Byte 0
    uint8_t Model       :8;
    // Byte 1
    uint8_t SwingV      :4;
    uint8_t Temp        :4;  // 16C~30C
    // Byte 2
    uint8_t             :5;
    uint8_t SwingH      :3;
    // Byte 3
    uint8_t             :1;
    uint8_t Health      :1;
    uint8_t             :3;
    uint8_t TimerMode   :3;
    // Byte 4
    uint8_t             :6;
    uint8_t Power       :1;
    uint8_t             :1;
    // Byte 5
    uint8_t OffTimerHrs :5;
    uint8_t Fan         :3;
    // Byte 6
    uint8_t OffTimerMins:6;
    uint8_t Turbo       :1;
    uint8_t Quiet       :1;
    // Byte 7
    uint8_t OnTimerHrs  :5;
    uint8_t Mode        :3;
    // Byte 8
    uint8_t OnTimerMins :6;
    uint8_t             :1;
    uint8_t Sleep       :1;
    // Byte 9
    uint8_t             :8;
    // Byte 10
    uint8_t ExtraDegreeF :1;
    uint8_t              :4;
    uint8_t UseFahrenheit:1;
    uint8_t              :2;
    // Byte 11
    uint8_t             :8;
    // Byte 12
    uint8_t Button      :5;
    uint8_t Lock        :1;
    uint8_t             :2;
    // Byte 13
    uint8_t Sum         :8;
    // Byte 14
    uint8_t Prefix2     :8;
    // Byte 15
    uint8_t             :8;
    // Byte 16
    uint8_t             :6;
    uint8_t Fan2        :2;
    // Byte 17
    uint8_t             :8;
    // Byte 18
    uint8_t             :8;
    // Byte 19
    uint8_t             :8;
    // Byte 20
    uint8_t             :8;
    // Byte 21
    uint8_t Sum2        :8;
  };
};

/// Native representation of a Haier 160 bit A/C message.
union HaierAc160Protocol{
  uint8_t raw[kHaierAC160StateLength];  ///< The state in native form
  struct {
    // Byte 0
    uint8_t Model       :8;
    // Byte 1
    uint8_t SwingV      :4;
    uint8_t Temp        :4;  // 16C~30C
    // Byte 2
    uint8_t             :5;
    uint8_t SwingH      :3;
    // Byte 3
    uint8_t             :1;
    uint8_t Health      :1;
    uint8_t             :3;
    uint8_t TimerMode   :3;
    // Byte 4
    uint8_t             :6;
    uint8_t Power       :1;
    uint8_t AuxHeating  :1;
    // Byte 5
    uint8_t OffTimerHrs :5;
    uint8_t Fan         :3;
    // Byte 6
    uint8_t OffTimerMins:6;
    uint8_t Turbo       :1;
    uint8_t Quiet       :1;
    // Byte 7
    uint8_t OnTimerHrs  :5;
    uint8_t Mode        :3;
    // Byte 8
    uint8_t OnTimerMins :6;
    uint8_t             :1;
    uint8_t Sleep       :1;
    // Byte 9
    uint8_t             :8;
    // Byte 10
    uint8_t ExtraDegreeF :1;
    uint8_t              :3;
    uint8_t Clean        :1;
    uint8_t UseFahrenheit:1;
    uint8_t              :2;
    // Byte 11
    uint8_t             :8;
    // Byte 12
    uint8_t Button      :5;
    uint8_t Lock        :1;
    uint8_t             :2;
    // Byte 13
    uint8_t Sum         :8;
    // Byte 14
    uint8_t Prefix     :8;
    // Byte 15
    uint8_t             :6;
    uint8_t Clean2      :1;
    uint8_t             :1;
    // Byte 16
    uint8_t             :5;
    uint8_t Fan2        :3;
    // Byte 17
    uint8_t             :8;
    // Byte 18
    uint8_t             :8;
    // Byte 19
    uint8_t Sum2        :8;
  };
};

// Legacy Haier YRW02 remote defines.
#define HAIER_AC_YRW02_SWING_OFF kHaierAcYrw02SwingOff
#define HAIER_AC_YRW02_SWING_TOP kHaierAcYrw02SwingTop
#define HAIER_AC_YRW02_SWING_MIDDLE kHaierAcYrw02SwingMiddle
#define HAIER_AC_YRW02_SWING_BOTTOM kHaierAcYrw02SwingBottom
#define HAIER_AC_YRW02_SWING_DOWN kHaierAcYrw02SwingDown
#define HAIER_AC_YRW02_SWING_AUTO kHaierAcYrw02SwingAuto
#define HAIER_AC_YRW02_FAN_HIGH kHaierAcYrw02FanHigh
#define HAIER_AC_YRW02_FAN_MED kHaierAcYrw02FanMed
#define HAIER_AC_YRW02_FAN_LOW kHaierAcYrw02FanLow
#define HAIER_AC_YRW02_FAN_AUTO kHaierAcYrw02FanAuto
#define HAIER_AC_YRW02_TURBO_OFF kHaierAcYrw02TurboOff
#define HAIER_AC_YRW02_AUTO kHaierAcYrw02Auto
#define HAIER_AC_YRW02_COOL kHaierAcYrw02Cool
#define HAIER_AC_YRW02_DRY kHaierAcYrw02Dry
#define HAIER_AC_YRW02_HEAT kHaierAcYrw02Heat
#define HAIER_AC_YRW02_FAN kHaierAcYrw02Fan
#define HAIER_AC_YRW02_BUTTON_TEMP_UP kHaierAcYrw02ButtonTempUp
#define HAIER_AC_YRW02_BUTTON_TEMP_DOWN kHaierAcYrw02ButtonTempDown
#define HAIER_AC_YRW02_BUTTON_SWING kHaierAcYrw02ButtonSwing
#define HAIER_AC_YRW02_BUTTON_FAN kHaierAcYrw02ButtonFan
#define HAIER_AC_YRW02_BUTTON_POWER kHaierAcYrw02ButtonPower
#define HAIER_AC_YRW02_BUTTON_MODE kHaierAcYrw02ButtonMode
#define HAIER_AC_YRW02_BUTTON_HEALTH kHaierAcYrw02ButtonHealth
#define HAIER_AC_YRW02_BUTTON_TURBO kHaierAcYrw02ButtonTurbo
#define HAIER_AC_YRW02_BUTTON_SLEEP kHaierAcYrw02ButtonSleep

// Classes
/// Class for handling detailed Haier A/C messages.
class IRHaierAC {
 public:
  explicit IRHaierAC(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
#if SEND_HAIER_AC
  void send(const uint16_t repeat = kHaierAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HAIER_AC
  void begin(void);
  void stateReset(void);

  void setCommand(const uint8_t command);
  uint8_t getCommand(void) const;

  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;

  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);

  bool getSleep(void) const;
  void setSleep(const bool on);
  bool getHealth(void) const;
  void setHealth(const bool on);

  int16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t mins);
  int16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t mins);
  void cancelTimers(void);

  uint16_t getCurrTime(void) const;
  void setCurrTime(const uint16_t mins);

  uint8_t getSwingV(void) const;
  void setSwingV(const uint8_t state);

  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kHaierACStateLength);
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
#endif
  HaierProtocol _;
  void checksum(void);
};

/// Class for handling detailed Haier 176 bit A/C messages.
class IRHaierAC176 {
  friend class IRHaierACYRW02;
 public:
  explicit IRHaierAC176(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);
#if SEND_HAIER_AC176
  virtual void send(const uint16_t repeat = kHaierAc176DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HAIER_AC176
  void begin(void);
  void stateReset(void);

  void setModel(const haier_ac176_remote_model_t model);
  haier_ac176_remote_model_t getModel(void) const;

  void setButton(const uint8_t button);
  uint8_t getButton(void) const;

  void setUseFahrenheit(const bool on);
  bool getUseFahrenheit(void) const;
  void setTemp(const uint8_t temp, const bool fahrenheit = false);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;

  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);

  bool getPower(void) const;
  void setPower(const bool on);
  void on(void);
  void off(void);

  bool getSleep(void) const;
  void setSleep(const bool on);
  bool getHealth(void) const;
  void setHealth(const bool on);

  bool getTurbo(void) const;
  void setTurbo(const bool on);
  bool getQuiet(void) const;
  void setQuiet(const bool on);

  uint8_t getSwingV(void) const;
  void setSwingV(const uint8_t pos);
  uint8_t getSwingH(void) const;
  void setSwingH(const uint8_t pos);

  /// These functions are for backward compatibility.
  /// Use getSwingV() and setSwingV() instead.
  uint8_t getSwing(void) const;
  void setSwing(const uint8_t pos);

  void setTimerMode(const uint8_t setting);
  uint8_t getTimerMode(void) const;
  void setOnTimer(const uint16_t mins);
  uint16_t getOnTimer(void) const;
  void setOffTimer(const uint16_t mins);
  uint16_t getOffTimer(void) const;

  bool getLock(void) const;
  void setLock(const bool on);

  uint8_t* getRaw(void);
  virtual void setRaw(const uint8_t new_code[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHaierAC176StateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
  static bool toCommonTurbo(const uint8_t speed);
  static bool toCommonQuiet(const uint8_t speed);
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
  HaierAc176Protocol _;
  void checksum(void);
};

/// Class for handling detailed Haier ACYRW02 A/C messages.
class IRHaierACYRW02 : public IRHaierAC176 {
 public:
  explicit IRHaierACYRW02(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);
#if SEND_HAIER_AC_YRW02
  void send(const uint16_t repeat = kHaierAcYrw02DefaultRepeat) override;
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HAIER_AC_YRW02
  void setRaw(const uint8_t new_code[]) override;
  static bool validChecksum(
      const uint8_t state[],
      const uint16_t length = kHaierACYRW02StateLength);
};

/// Class for handling detailed Haier 160 bit A/C messages.
class IRHaierAC160 {
 public:
  explicit IRHaierAC160(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);
#if SEND_HAIER_AC160
  virtual void send(const uint16_t repeat = kHaierAc160DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HAIER_AC160
  void begin(void);
  void stateReset(void);

  void setButton(const uint8_t button);
  uint8_t getButton(void) const;

  void setUseFahrenheit(const bool on);
  bool getUseFahrenheit(void) const;
  void setTemp(const uint8_t temp, const bool fahrenheit = false);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;

  uint8_t getMode(void) const;
  void setMode(const uint8_t mode);

  bool getPower(void) const;
  void setPower(const bool on);
  void on(void);
  void off(void);

  bool getSleep(void) const;
  void setSleep(const bool on);
  bool getClean(void) const;
  void setClean(const bool on);
  bool getLightToggle(void) const;
  void setLightToggle(const bool on);

  bool getTurbo(void) const;
  void setTurbo(const bool on);
  bool getQuiet(void) const;
  void setQuiet(const bool on);
  bool getAuxHeating(void) const;
  void setAuxHeating(const bool on);

  uint8_t getSwingV(void) const;
  void setSwingV(const uint8_t pos);

  void setTimerMode(const uint8_t setting);
  uint8_t getTimerMode(void) const;
  void setOnTimer(const uint16_t mins);
  uint16_t getOnTimer(void) const;
  void setOffTimer(const uint16_t mins);
  uint16_t getOffTimer(void) const;

  bool getLock(void) const;
  void setLock(const bool on);

  bool getHealth(void) const;
  void setHealth(const bool on);

  uint8_t* getRaw(void);
  virtual void setRaw(const uint8_t new_code[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHaierAC160StateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static bool toCommonTurbo(const uint8_t speed);
  static bool toCommonQuiet(const uint8_t speed);
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
  HaierAc160Protocol _;
  void checksum(void);
};
#endif  // IR_HAIER_H_
