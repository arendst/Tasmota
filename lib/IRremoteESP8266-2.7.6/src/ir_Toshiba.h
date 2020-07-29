// Copyright 2017 David Conran

// Toshiba A/C support added by David Conran

// Supports:
//   Brand: Toshiba,  Model: RAS-B13N3KV2
//   Brand: Toshiba,  Model: Akita EVO II
//   Brand: Toshiba,  Model: RAS-B13N3KVP-E
//   Brand: Toshiba,  Model: RAS 18SKP-ES
//   Brand: Toshiba,  Model: WH-TA04NE
//   Brand: Toshiba,  Model: WC-L03SE

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
const uint8_t kToshibaAcModeOffset = 0;
const uint8_t kToshibaAcModeSize = 2;  // Nr. of bits
const uint8_t kToshibaAcAuto = 0;
const uint8_t kToshibaAcCool = 1;
const uint8_t kToshibaAcDry = 2;
const uint8_t kToshibaAcHeat = 3;
const uint8_t kToshibaAcPowerOffset = 2;
const uint8_t kToshibaAcFanOffset = 5;
const uint8_t kToshibaAcFanSize = 3;  // Nr. of bits
const uint8_t kToshibaAcFanAuto = 0b000;
const uint8_t kToshibaAcFanMin =  0b001;
const uint8_t kToshibaAcFanMed =  0b011;
const uint8_t kToshibaAcFanMax =  0b101;
const uint8_t kToshibaAcTempOffset = 4;
const uint8_t kToshibaAcTempSize = 4;  // Nr. of bits
const uint8_t kToshibaAcMinTemp = 17;  // 17C
const uint8_t kToshibaAcMaxTemp = 30;  // 30C

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

class IRToshibaAC {
 public:
  explicit IRToshibaAC(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

  void stateReset(void);
#if SEND_TOSHIBA_AC
  void send(const uint16_t repeat = kToshibaACMinRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
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
  void setMode(const uint8_t mode);
  uint8_t getMode(const bool useRaw = false);
  void setRaw(const uint8_t newState[]);
  uint8_t* getRaw(void);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kToshibaACStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  uint8_t remote_state[kToshibaACStateLength];
  void checksum(const uint16_t length = kToshibaACStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kToshibaACStateLength);
  uint8_t mode_state;
};

#endif  // IR_TOSHIBA_H_
