// Copyright 2017 David Conran
#ifndef IR_TOSHIBA_H_
#define IR_TOSHIBA_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//     TTTTTTT  OOOOO   SSSSS  HH   HH IIIII BBBBB     AAA
//       TTT   OO   OO SS      HH   HH  III  BB   B   AAAAA
//       TTT   OO   OO  SSSSS  HHHHHHH  III  BBBBBB  AA   AA
//       TTT   OO   OO      SS HH   HH  III  BB   BB AAAAAAA
//       TTT    OOOO0   SSSSS  HH   HH IIIII BBBBBB  AA   AA

// Toshiba A/C support added by David Conran

// Constants
const uint8_t kToshibaAcAuto = 0;
const uint8_t kToshibaAcCool = 1;
const uint8_t kToshibaAcDry = 2;
const uint8_t kToshibaAcHeat = 3;
const uint8_t kToshibaAcPower = 4;
const uint8_t kToshibaAcFanAuto = 0;
const uint8_t kToshibaAcFanMax = 5;
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
  explicit IRToshibaAC(uint16_t pin);

  void stateReset();
#if SEND_TOSHIBA_AC
  void send();
#endif  // SEND_TOSHIBA_AC
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFan(uint8_t fan);
  uint8_t getFan();
  void setMode(uint8_t mode);
  uint8_t getMode(bool useRaw = false);
  void setRaw(uint8_t newState[]);
  uint8_t* getRaw();
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kToshibaACStateLength);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif
#ifndef UNIT_TEST

 private:
#endif
  uint8_t remote_state[kToshibaACStateLength];
  void checksum(const uint16_t length = kToshibaACStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kToshibaACStateLength);
  uint8_t mode_state;
  IRsend _irsend;
};

#endif  // IR_TOSHIBA_H_
