// Copyright 2018 crankyoldgit
// The specifics of reverse engineering the protocol details by kuzin2006

#ifndef IR_HAIER_H_
#define IR_HAIER_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//                      HH   HH   AAA   IIIII EEEEEEE RRRRRR
//                      HH   HH  AAAAA   III  EE      RR   RR
//                      HHHHHHH AA   AA  III  EEEEE   RRRRRR
//                      HH   HH AAAAAAA  III  EE      RR  RR
//                      HH   HH AA   AA IIIII EEEEEEE RR   RR

// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/404
//   https://www.dropbox.com/s/mecyib3lhdxc8c6/IR%20data%20reverse%20engineering.xlsx?dl=0
//   https://github.com/markszabo/IRremoteESP8266/issues/485
//   https://www.dropbox.com/sh/w0bt7egp0fjger5/AADRFV6Wg4wZskJVdFvzb8Z0a?dl=0&preview=haer2.ods

// Constants

// Haier HSU07-HEA03 remote
// Byte 0
const uint8_t kHaierAcPrefix = 0b10100101;

// Byte 1
const uint8_t kHaierAcMinTemp = 16;
const uint8_t kHaierAcDefTemp = 25;
const uint8_t kHaierAcMaxTemp = 30;
const uint8_t kHaierAcCmdOff = 0b00000000;
const uint8_t kHaierAcCmdOn = 0b00000001;
const uint8_t kHaierAcCmdMode = 0b00000010;
const uint8_t kHaierAcCmdFan = 0b00000011;
const uint8_t kHaierAcCmdTempUp = 0b00000110;
const uint8_t kHaierAcCmdTempDown = 0b00000111;
const uint8_t kHaierAcCmdSleep = 0b00001000;
const uint8_t kHaierAcCmdTimerSet = 0b00001001;
const uint8_t kHaierAcCmdTimerCancel = 0b00001010;
const uint8_t kHaierAcCmdHealth = 0b00001100;
const uint8_t kHaierAcCmdSwing = 0b00001101;

// Byte 2
const uint8_t kHaierAcSwingOff = 0b00000000;
const uint8_t kHaierAcSwingUp = 0b00000001;
const uint8_t kHaierAcSwingDown = 0b00000010;
const uint8_t kHaierAcSwingChg = 0b00000011;

// Byte 6
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
// Byte 0
const uint8_t kHaierAcYrw02Prefix = 0xA6;

// Byte 1
// Bits 0-3
// 0x0 = 16DegC, ... 0xE = 30DegC
// Bits 4-7 - Swing
const uint8_t kHaierAcYrw02SwingOff = 0x0;
const uint8_t kHaierAcYrw02SwingTop = 0x1;
const uint8_t kHaierAcYrw02SwingMiddle = 0x2;  // Not available in heat mode.
const uint8_t kHaierAcYrw02SwingBottom = 0x3;  // Only available in heat mode.
const uint8_t kHaierAcYrw02SwingDown = 0xA;
const uint8_t kHaierAcYrw02SwingAuto = 0xC;  // Airflow

// Byte 3
// Bit 7 - Health mode

// Byte 4
const uint8_t kHaierAcYrw02Power = 0b01000000;

// Byte 5
// Bits 0-3
const uint8_t kHaierAcYrw02FanHigh = 0x2;
const uint8_t kHaierAcYrw02FanMed = 0x4;
const uint8_t kHaierAcYrw02FanLow = 0x6;
const uint8_t kHaierAcYrw02FanAuto = 0xA;

// Byte 6
// Bits 0-1
const uint8_t kHaierAcYrw02TurboOff = 0x0;
const uint8_t kHaierAcYrw02TurboHigh = 0x1;
const uint8_t kHaierAcYrw02TurboLow = 0x2;

// Byte 7
// Bits 0-3
const uint8_t kHaierAcYrw02Auto = 0x0;
const uint8_t kHaierAcYrw02Cool = 0x2;
const uint8_t kHaierAcYrw02Dry = 0x4;
const uint8_t kHaierAcYrw02Heat = 0x8;
const uint8_t kHaierAcYrw02Fan = 0xC;

// Byte 8
const uint8_t kHaierAcYrw02Sleep = 0b10000000;

// Byte 12
// Bits 4-7
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

class IRHaierAC {
 public:
  explicit IRHaierAC(uint16_t pin);

#if SEND_HAIER_AC
  void send();
#endif  // SEND_HAIER_AC
  void begin();

  void setCommand(const uint8_t command);
  uint8_t getCommand();

  void setTemp(const uint8_t temp);
  uint8_t getTemp();

  void setFan(const uint8_t speed);
  uint8_t getFan();

  uint8_t getMode();
  void setMode(const uint8_t mode);

  bool getSleep();
  void setSleep(const bool state);
  bool getHealth();
  void setHealth(const bool state);

  int16_t getOnTimer();
  void setOnTimer(const uint16_t mins);
  int16_t getOffTimer();
  void setOffTimer(const uint16_t mins);
  void cancelTimers();

  uint16_t getCurrTime();
  void setCurrTime(const uint16_t mins);

  uint8_t getSwing();
  void setSwing(const uint8_t state);

  uint8_t* getRaw();
  void setRaw(uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kHaierACStateLength);
#ifdef ARDUINO
  String toString();
  static String timeToString(const uint16_t nr_mins);
#else
  std::string toString();
  static std::string timeToString(const uint16_t nr_mins);
#endif

 private:
  uint8_t remote_state[kHaierACStateLength];
  void stateReset();
  void checksum();
  static uint16_t getTime(const uint8_t ptr[]);
  static void setTime(uint8_t ptr[], const uint16_t nr_mins);
  IRsend _irsend;
};

class IRHaierACYRW02 {
 public:
  explicit IRHaierACYRW02(uint16_t pin);

#if SEND_HAIER_AC_YRW02
  void send();
#endif  // SEND_HAIER_AC_YRW02
  void begin();

  void setButton(const uint8_t button);
  uint8_t getButton();

  void setTemp(const uint8_t temp);
  uint8_t getTemp();

  void setFan(const uint8_t speed);
  uint8_t getFan();

  uint8_t getMode();
  void setMode(const uint8_t mode);

  bool getPower();
  void setPower(const bool state);
  void on();
  void off();

  bool getSleep();
  void setSleep(const bool state);
  bool getHealth();
  void setHealth(const bool state);

  uint8_t getTurbo();
  void setTurbo(const uint8_t speed);

  uint8_t getSwing();
  void setSwing(const uint8_t state);

  uint8_t* getRaw();
  void setRaw(uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kHaierACYRW02StateLength);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  uint8_t remote_state[kHaierACYRW02StateLength];
  void stateReset();
  void checksum();
  IRsend _irsend;
};

#endif  // IR_HAIER_H_
