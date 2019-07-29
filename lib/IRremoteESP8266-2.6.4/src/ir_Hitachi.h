// Hitachi A/C
//
// Copyright 2018 David Conran

// Supports:
//   Brand: Hitachi,  Model: RAS-35THA6 remote
//   Brand: Hitachi,  Model: LT0541-HTA remote
//   Brand: Hitachi,  Model: Series VI A/C (Circa 2007)

#ifndef IR_HITACHI_H_
#define IR_HITACHI_H_

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
const uint8_t kHitachiAcAuto = 2;
const uint8_t kHitachiAcHeat = 3;
const uint8_t kHitachiAcCool = 4;
const uint8_t kHitachiAcDry = 5;
const uint8_t kHitachiAcFan = 0xC;
const uint8_t kHitachiAcFanAuto = 1;
const uint8_t kHitachiAcFanLow = 2;
const uint8_t kHitachiAcFanMed = 3;
const uint8_t kHitachiAcFanHigh = 5;
const uint8_t kHitachiAcMinTemp = 16;   // 16C
const uint8_t kHitachiAcMaxTemp = 32;   // 32C
const uint8_t kHitachiAcAutoTemp = 23;  // 23C

// Classes
class IRHitachiAc {
 public:
  explicit IRHitachiAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

  void stateReset(void);
#if SEND_HITACHI_AC
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HITACHI_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHitachiAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAcStateLength);
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
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kHitachiAcStateLength];
  void checksum(const uint16_t length = kHitachiAcStateLength);
  uint8_t _previoustemp;
};

#endif  // IR_HITACHI_H_
