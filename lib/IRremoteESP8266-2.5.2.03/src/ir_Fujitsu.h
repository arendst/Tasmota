// Copyright 2017 Jonny Graham
// Copyright 2018 David Conran
#ifndef IR_FUJITSU_H_
#define IR_FUJITSU_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

// FUJITSU A/C support added by Jonny Graham

// Constants
const uint8_t kFujitsuAcModeAuto = 0x00;
const uint8_t kFujitsuAcModeCool = 0x01;
const uint8_t kFujitsuAcModeDry = 0x02;
const uint8_t kFujitsuAcModeFan = 0x03;
const uint8_t kFujitsuAcModeHeat = 0x04;

const uint8_t kFujitsuAcCmdStayOn = 0x00;
const uint8_t kFujitsuAcCmdTurnOn = 0x01;
const uint8_t kFujitsuAcCmdTurnOff = 0x02;
const uint8_t kFujitsuAcCmdStepHoriz = 0x79;
const uint8_t kFujitsuAcCmdStepVert = 0x6C;

const uint8_t kFujitsuAcFanAuto = 0x00;
const uint8_t kFujitsuAcFanHigh = 0x01;
const uint8_t kFujitsuAcFanMed = 0x02;
const uint8_t kFujitsuAcFanLow = 0x03;
const uint8_t kFujitsuAcFanQuiet = 0x04;

const uint8_t kFujitsuAcMinTemp = 16;  // 16C
const uint8_t kFujitsuAcMaxTemp = 30;  // 30C

const uint8_t kFujitsuAcSwingOff = 0x00;
const uint8_t kFujitsuAcSwingVert = 0x01;
const uint8_t kFujitsuAcSwingHoriz = 0x02;
const uint8_t kFujitsuAcSwingBoth = 0x03;

// Legacy defines.
#define FUJITSU_AC_MODE_AUTO kFujitsuAcModeAuto
#define FUJITSU_AC_MODE_COOL kFujitsuAcModeCool
#define FUJITSU_AC_MODE_DRY kFujitsuAcModeDry
#define FUJITSU_AC_MODE_FAN kFujitsuAcModeFan
#define FUJITSU_AC_MODE_HEAT kFujitsuAcModeHeat
#define FUJITSU_AC_CMD_STAY_ON kFujitsuAcCmdStayOn
#define FUJITSU_AC_CMD_TURN_ON kFujitsuAcCmdTurnOn
#define FUJITSU_AC_CMD_TURN_OFF kFujitsuAcCmdTurnOff
#define FUJITSU_AC_CMD_STEP_HORIZ kFujitsuAcCmdStepHoriz
#define FUJITSU_AC_CMD_STEP_VERT kFujitsuAcCmdStepVert
#define FUJITSU_AC_FAN_AUTO kFujitsuAcFanAuto
#define FUJITSU_AC_FAN_HIGH kFujitsuAcFanHigh
#define FUJITSU_AC_FAN_MED kFujitsuAcFanMed
#define FUJITSU_AC_FAN_LOW kFujitsuAcFanLow
#define FUJITSU_AC_FAN_QUIET kFujitsuAcFanQuiet
#define FUJITSU_AC_MIN_TEMP kFujitsuAcMinTemp
#define FUJITSU_AC_MAX_TEMP kFujitsuAcMaxTemp
#define FUJITSU_AC_SWING_OFF kFujitsuAcSwingOff
#define FUJITSU_AC_SWING_VERT kFujitsuAcSwingVert
#define FUJITSU_AC_SWING_HORIZ kFujitsuAcSwingHoriz
#define FUJITSU_AC_SWING_BOTH kFujitsuAcSwingBoth

enum fujitsu_ac_remote_model_t {
  ARRAH2E = 1,
  ARDB1,
};

class IRFujitsuAC {
 public:
  explicit IRFujitsuAC(uint16_t pin, fujitsu_ac_remote_model_t model = ARRAH2E);

  void setModel(fujitsu_ac_remote_model_t model);
  void stateReset();
#if SEND_FUJITSU_AC
  void send();
#endif  // SEND_FUJITSU_AC
  void begin();
  void off();
  void stepHoriz();
  void stepVert();
  void setCmd(uint8_t cmd);
  uint8_t getCmd();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFanSpeed(uint8_t fan);
  uint8_t getFanSpeed();
  void setMode(uint8_t mode);
  uint8_t getMode();
  void setSwing(uint8_t mode);
  uint8_t getSwing();
  uint8_t* getRaw();
  bool setRaw(const uint8_t newState[], const uint16_t length);
  uint8_t getStateLength();
  static bool validChecksum(uint8_t* state, uint16_t length);
  bool getPower();
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  uint8_t remote_state[kFujitsuAcStateLength];
  IRsend _irsend;
  uint8_t _temp;
  uint8_t _fanSpeed;
  uint8_t _mode;
  uint8_t _swingMode;
  uint8_t _cmd;
  fujitsu_ac_remote_model_t _model;
  uint8_t _state_length;
  uint8_t _state_length_short;
  void buildState();
  void buildFromState(const uint16_t length);
};

#endif  // IR_FUJITSU_H_
