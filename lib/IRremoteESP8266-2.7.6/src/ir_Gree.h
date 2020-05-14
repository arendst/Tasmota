// Copyright 2016 David Conran
// Gree A/C
//
// Supports:
//   Brand: Ultimate,  Model: Heat Pump
//   Brand: EKOKAI,  Model: A/C
//   Brand: RusClimate,  Model: EACS/I-09HAR_X/N3 A/C
//   Brand: RusClimate,  Model: YAW1F remote
//   Brand: Green,  Model: YBOFB remote
//   Brand: Green,  Model: YBOFB2 remote

#ifndef IR_GREE_H_
#define IR_GREE_H_

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

const uint8_t kGreeAuto = 0;
const uint8_t kGreeCool = 1;
const uint8_t kGreeDry = 2;
const uint8_t kGreeFan = 3;
const uint8_t kGreeHeat = 4;

// Byte 0
const uint8_t kGreePower1Offset = 3;
const uint8_t kGreeFanOffset = 4;
const uint8_t kGreeFanSize = 2;  // Bits
const uint8_t kGreeFanAuto = 0;
const uint8_t kGreeFanMin = 1;
const uint8_t kGreeFanMed = 2;
const uint8_t kGreeFanMax = 3;
const uint8_t kGreeSwingAutoOffset = 6;
const uint8_t kGreeSleepOffset = 7;
// Byte 1
const uint8_t kGreeTempSize = 4;
const uint8_t kGreeMinTemp = 16;  // Celsius
const uint8_t kGreeMaxTemp = 30;  // Celsius
const uint8_t kGreeTimerHalfHrOffset = 4;
const uint8_t kGreeTimerTensHrOffset = 5;
const uint8_t kGreeTimerTensHrSize = 2;  // Bits
const uint16_t kGreeTimerMax = 24 * 60;
const uint8_t kGreeTimerEnabledOffset = 7;

// Byte 2
const uint8_t kGreeTimerHoursOffset = 0;
const uint8_t kGreeTimerHoursSize = 4;  // Bits
const uint8_t kGreeTurboOffset = 4;
const uint8_t kGreeLightOffset = 5;
// This might not be used. See #814
const uint8_t kGreePower2Offset = 6;
const uint8_t kGreeXfanOffset = 7;
// Byte 4
const uint8_t kGreeSwingSize = 4;  // Bits
const uint8_t kGreeSwingLastPos =    0b0000;
const uint8_t kGreeSwingAuto =       0b0001;
const uint8_t kGreeSwingUp =         0b0010;
const uint8_t kGreeSwingMiddleUp =   0b0011;
const uint8_t kGreeSwingMiddle =     0b0100;
const uint8_t kGreeSwingMiddleDown = 0b0101;
const uint8_t kGreeSwingDown =       0b0110;
const uint8_t kGreeSwingDownAuto =   0b0111;
const uint8_t kGreeSwingMiddleAuto = 0b1001;
const uint8_t kGreeSwingUpAuto =     0b1011;
// byte 5
const uint8_t kGreeIFeelOffset = 2;
const uint8_t kGreeWiFiOffset = 6;

// Legacy defines.
#define GREE_AUTO kGreeAuto
#define GREE_COOL kGreeCool
#define GREE_DRY kGreeDry
#define GREE_FAN kGreeFan
#define GREE_HEAT kGreeHeat
#define GREE_MIN_TEMP kGreeMinTemp
#define GREE_MAX_TEMP kGreeMaxTemp
#define GREE_FAN_MAX kGreeFanMax
#define GREE_SWING_LAST_POS kGreeSwingLastPos
#define GREE_SWING_AUTO kGreeSwingAuto
#define GREE_SWING_UP kGreeSwingUp
#define GREE_SWING_MIDDLE_UP kGreeSwingMiddleUp
#define GREE_SWING_MIDDLE kGreeSwingMiddle
#define GREE_SWING_MIDDLE_DOWN kGreeSwingMiddleDown
#define GREE_SWING_DOWN kGreeSwingDown
#define GREE_SWING_DOWN_AUTO kGreeSwingDownAuto
#define GREE_SWING_MIDDLE_AUTO kGreeSwingMiddleAuto
#define GREE_SWING_UP_AUTO kGreeSwingUpAuto

// Classes
class IRGreeAC {
 public:
  explicit IRGreeAC(
      const uint16_t pin,
      const gree_ac_remote_model_t model = gree_ac_remote_model_t::YAW1F,
      const bool inverted = false, const bool use_modulation = true);

  void stateReset(void);
#if SEND_GREE
  void send(const uint16_t repeat = kGreeDefaultRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_GREE
  void begin(void);
  void on(void);
  void off(void);
  void setModel(const gree_ac_remote_model_t model);
  gree_ac_remote_model_t getModel(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setMode(const uint8_t new_mode);
  uint8_t getMode(void);
  void setLight(const bool on);
  bool getLight(void);
  void setXFan(const bool on);
  bool getXFan(void);
  void setSleep(const bool on);
  bool getSleep(void);
  void setTurbo(const bool on);
  bool getTurbo(void);
  void setIFeel(const bool on);
  bool getIFeel(void);
  void setWiFi(const bool on);
  bool getWiFi(void);
  void setSwingVertical(const bool automatic, const uint8_t position);
  bool getSwingVerticalAuto(void);
  uint8_t getSwingVerticalPosition(void);
  uint16_t getTimer(void);
  void setTimer(const uint16_t minutes);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  uint8_t convertSwingV(const stdAc::swingv_t swingv);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  stdAc::state_t toCommon(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kGreeStateLength);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else  // UNIT_TEST
  IRsendTest _irsend;
#endif  // UNIT_TEST
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kGreeStateLength];
  gree_ac_remote_model_t _model;
  void checksum(const uint16_t length = kGreeStateLength);
  void fixup(void);
  void setTimerEnabled(const bool on);
  bool getTimerEnabled(void);
};

#endif  // IR_GREE_H_
