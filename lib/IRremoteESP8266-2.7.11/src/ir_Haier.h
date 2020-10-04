// Copyright 2018 crankyoldgit
/// @file
/// @brief Support for Haier A/C protocols.
/// The specifics of reverse engineering the protocols details:
/// * HSU07-HEA03 by kuzin2006.
/// * YR-W02/HSU-09HMC203 by non7top.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/404
/// @see https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/485
/// @see https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods

// Supports:
//   Brand: Haier,  Model: HSU07-HEA03 remote (HAIER_AC)
//   Brand: Haier,  Model: YR-W02 remote (HAIER_AC_YRW02)
//   Brand: Haier,  Model: HSU-09HMC203 A/C (HAIER_AC_YRW02)

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

// Haier HSU07-HEA03 remote
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
    uint8_t Swing    :2;
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

const uint8_t kHaierAcSwingOff =  0b00;
const uint8_t kHaierAcSwingUp =   0b01;
const uint8_t kHaierAcSwingDown = 0b10;
const uint8_t kHaierAcSwingChg =  0b11;

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
#define HAIER_AC_CMD_SWING kHaierAcCmdSwing
#define HAIER_AC_SWING_OFF kHaierAcSwingOff
#define HAIER_AC_SWING_UP kHaierAcSwingUp
#define HAIER_AC_SWING_DOWN kHaierAcSwingDown
#define HAIER_AC_SWING_CHG kHaierAcSwingChg
#define HAIER_AC_AUTO kHaierAcAuto
#define HAIER_AC_COOL kHaierAcCool
#define HAIER_AC_DRY kHaierAcDry
#define HAIER_AC_HEAT kHaierAcHeat
#define HAIER_AC_FAN kHaierAcFan
#define HAIER_AC_FAN_AUTO kHaierAcFanAuto
#define HAIER_AC_FAN_LOW kHaierAcFanLow
#define HAIER_AC_FAN_MED kHaierAcFanMed
#define HAIER_AC_FAN_HIGH kHaierAcFanHigh

// Haier YRW02 remote
union HaierYRW02Protocol{
  uint8_t raw[kHaierACYRW02StateLength];  ///< The state in native form
  struct {
    // Byte 0
    uint8_t Prefix;
    // Byte 1
    uint8_t Swing:4;
    uint8_t Temp :4;  // 16C~30C
    // Byte 2
    uint8_t :8;
    // Byte 3
    uint8_t       :1;
    uint8_t Health:1;
    uint8_t       :6;
    // Byte 4
    uint8_t      :6;
    uint8_t Power:1;
    uint8_t      :1;
    // Byte 5
    uint8_t    :5;
    uint8_t Fan:3;
    // Byte 6
    uint8_t      :6;
    uint8_t Turbo:2;
    // Byte 7
    uint8_t     :5;
    uint8_t Mode:3;
    // Byte 8
    uint8_t      :7;
    uint8_t Sleep:1;
    // Byte 9
    uint8_t :8;
    // Byte 10
    uint8_t :8;
    // Byte 11
    uint8_t :8;
    // Byte 12
    uint8_t Button:4;
    uint8_t       :4;
    // Byte 13
    uint8_t Sum;
  };
};

const uint8_t kHaierAcYrw02Prefix = 0xA6;

const uint8_t kHaierAcYrw02SwingOff = 0x0;
const uint8_t kHaierAcYrw02SwingTop = 0x1;
const uint8_t kHaierAcYrw02SwingMiddle = 0x2;  // Not available in heat mode.
const uint8_t kHaierAcYrw02SwingBottom = 0x3;  // Only available in heat mode.
const uint8_t kHaierAcYrw02SwingDown = 0xA;
const uint8_t kHaierAcYrw02SwingAuto = 0xC;  // Airflow

const uint8_t kHaierAcYrw02FanHigh = 0b001;
const uint8_t kHaierAcYrw02FanMed =  0b010;
const uint8_t kHaierAcYrw02FanLow =  0b011;
const uint8_t kHaierAcYrw02FanAuto = 0b101;

const uint8_t kHaierAcYrw02TurboOff = 0x0;
const uint8_t kHaierAcYrw02TurboHigh = 0x1;
const uint8_t kHaierAcYrw02TurboLow = 0x2;

const uint8_t kHaierAcYrw02Auto = 0b000;  // 0
const uint8_t kHaierAcYrw02Cool = 0b001;  // 1
const uint8_t kHaierAcYrw02Dry =  0b010;  // 2
const uint8_t kHaierAcYrw02Heat = 0b100;  // 4
const uint8_t kHaierAcYrw02Fan =  0b110;  // 5

const uint8_t kHaierAcYrw02ButtonTempUp = 0x0;
const uint8_t kHaierAcYrw02ButtonTempDown = 0x1;
const uint8_t kHaierAcYrw02ButtonSwing = 0x2;
const uint8_t kHaierAcYrw02ButtonFan = 0x4;
const uint8_t kHaierAcYrw02ButtonPower = 0x5;
const uint8_t kHaierAcYrw02ButtonMode = 0x6;
const uint8_t kHaierAcYrw02ButtonHealth = 0x7;
const uint8_t kHaierAcYrw02ButtonTurbo = 0x8;
const uint8_t kHaierAcYrw02ButtonSleep = 0xB;

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
#define HAIER_AC_YRW02_TURBO_HIGH kHaierAcYrw02TurboHigh
#define HAIER_AC_YRW02_TURBO_LOW kHaierAcYrw02TurboLow
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

  uint8_t getSwing(void) const;
  void setSwing(const uint8_t state);

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
  void stateReset(void);
  void checksum(void);
};

/// Class for handling detailed Haier ACYRW02 A/C messages.
class IRHaierACYRW02 {
 public:
  explicit IRHaierACYRW02(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);
#if SEND_HAIER_AC_YRW02
  void send(const uint16_t repeat = kHaierAcYrw02DefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HAIER_AC_YRW02
  void begin(void);

  void setButton(const uint8_t button);
  uint8_t getButton(void) const;

  void setTemp(const uint8_t temp);
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

  uint8_t getTurbo(void) const;
  void setTurbo(const uint8_t speed);

  uint8_t getSwing(void) const;
  void setSwing(const uint8_t pos);

  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kHaierACYRW02StateLength);
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
  HaierYRW02Protocol _;
  void stateReset(void);
  void checksum(void);
};
#endif  // IR_HAIER_H_
