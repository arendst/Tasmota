// Samsung A/C
//
// Copyright 2018 David Conran

#ifndef IR_SAMSUNG_H_
#define IR_SAMSUNG_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//              SSSS   AAA    MMM    SSSS  U   U  N   N   GGGG
//             S      A   A  M M M  S      U   U  NN  N  G
//              SSS   AAAAA  M M M   SSS   U   U  N N N  G  GG
//                 S  A   A  M   M      S  U   U  N  NN  G   G
//             SSSS   A   A  M   M  SSSS    UUU   N   N   GGG

// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/505

// Constants
const uint8_t kSamsungAcAuto = 0;
const uint8_t kSamsungAcCool = 1;
const uint8_t kSamsungAcDry = 2;
const uint8_t kSamsungAcFan = 3;
const uint8_t kSamsungAcHeat = 4;
const uint8_t kSamsungAcModeMask = 0x70;
const uint8_t kSamsungAcFanAuto = 0;
const uint8_t kSamsungAcFanLow = 2;
const uint8_t kSamsungAcFanMed = 4;
const uint8_t kSamsungAcFanHigh = 5;
const uint8_t kSamsungAcFanAuto2 = 6;
const uint8_t kSamsungAcFanTurbo = 7;
const uint8_t kSamsungAcMinTemp = 16;   // 16C
const uint8_t kSamsungAcMaxTemp = 30;   // 30C
const uint8_t kSamsungAcAutoTemp = 25;  // 25C
const uint8_t kSamsungAcTempMask = 0xF0;
const uint8_t kSamsungAcPowerMask1 = 0x20;
const uint8_t kSamsungAcPowerMask2 = 0x30;
const uint8_t kSamsungAcFanMask = 0x0E;
const uint8_t kSamsungAcSwingMask = 0x70;
const uint8_t kSamsungAcSwingMove = 0b010;
const uint8_t kSamsungAcSwingStop = 0b111;
const uint8_t kSamsungAcBeepMask = 0x02;
const uint8_t kSamsungAcCleanMask10 = 0x80;
const uint8_t kSamsungAcCleanMask11 = 0x02;
const uint8_t kSamsungAcQuietMask11 = 0x01;

const uint16_t kSamsungACSectionLength = 7;
const uint64_t kSamsungAcPowerSection = 0x1D20F00000000;

// Classes
class IRSamsungAc {
 public:
  explicit IRSamsungAc(uint16_t pin);

  void stateReset();
#if SEND_SAMSUNG_AC
  void send(const bool calcchecksum = true);
  void sendExtended(const bool calcchecksum = true);
#endif  // SEND_SAMSUNG_AC
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwing(const bool state);
  bool getSwing();
  void setBeep(const bool state);
  bool getBeep();
  void setClean(const bool state);
  bool getClean();
  void setQuiet(const bool state);
  bool getQuiet();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kSamsungAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kSamsungAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kSamsungAcStateLength);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kSamsungAcExtendedStateLength];
  void checksum(const uint16_t length = kSamsungAcStateLength);
  IRsend _irsend;
};

#endif  // IR_SAMSUNG_H_
