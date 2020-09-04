// Hitachi A/C
//
// Copyright 2018 David Conran

#ifndef IR_HITACHI_H_
#define IR_HITACHI_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
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
const uint8_t kHitachiAcFanHigh = 5;
const uint8_t kHitachiAcMinTemp = 16;   // 16C
const uint8_t kHitachiAcMaxTemp = 32;   // 32C
const uint8_t kHitachiAcAutoTemp = 23;  // 23C

// Classes
class IRHitachiAc {
 public:
  explicit IRHitachiAc(uint16_t pin);

  void stateReset();
#if SEND_HITACHI_AC
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif  // SEND_HITACHI_AC
  void begin();
  void on();
  void off();
  void setPower(const bool on);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwingVertical(const bool on);
  bool getSwingVertical();
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHitachiAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAcStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif
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
