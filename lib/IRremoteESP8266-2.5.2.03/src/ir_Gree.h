// Kelvinator A/C
//
// Copyright 2016 David Conran

#ifndef IR_GREE_H_
#define IR_GREE_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//                      GGGG  RRRRRR  EEEEEEE EEEEEEE
//                     GG  GG RR   RR EE      EE
//                    GG      RRRRRR  EEEEE   EEEEE
//                    GG   GG RR  RR  EE      EE
//                     GGGGGG RR   RR EEEEEEE EEEEEEE

// Constants
const uint8_t kGreeAuto = 0;
const uint8_t kGreeCool = 1;
const uint8_t kGreeDry = 2;
const uint8_t kGreeFan = 3;
const uint8_t kGreeHeat = 4;

// Byte 0
const uint8_t kGreeModeMask = 0b00000111;
const uint8_t kGreePower1Mask = 0b00001000;
const uint8_t kGreeFanMask = 0b00110000;
const uint8_t kGreeSwingAutoMask = 0b01000000;
const uint8_t kGreeSleepMask = 0b10000000;
// Byte 2
const uint8_t kGreeTurboMask = 0b00010000;
const uint8_t kGreeLightMask = 0b00100000;
const uint8_t kGreePower2Mask = 0b01000000;
const uint8_t kGreeXfanMask = 0b10000000;
// Byte 4
const uint8_t kGreeSwingPosMask = 0b00001111;

const uint8_t kGreeMinTemp = 16;  // Celsius
const uint8_t kGreeMaxTemp = 30;  // Celsius
const uint8_t kGreeFanMax = 3;

const uint8_t kGreeSwingLastPos = 0b00000000;
const uint8_t kGreeSwingAuto = 0b00000001;
const uint8_t kGreeSwingUp = 0b00000010;
const uint8_t kGreeSwingMiddleUp = 0b00000011;
const uint8_t kGreeSwingMiddle = 0b00000100;
const uint8_t kGreeSwingMiddleDown = 0b00000101;
const uint8_t kGreeSwingDown = 0b00000110;
const uint8_t kGreeSwingDownAuto = 0b00000111;
const uint8_t kGreeSwingMiddleAuto = 0b00001001;
const uint8_t kGreeSwingUpAuto = 0b00001011;

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
  explicit IRGreeAC(uint16_t pin);

  void stateReset();
#if SEND_GREE
  void send();
#endif  // SEND_GREE
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t new_mode);
  uint8_t getMode();
  void setLight(const bool state);
  bool getLight();
  void setXFan(const bool state);
  bool getXFan();
  void setSleep(const bool state);
  bool getSleep();
  void setTurbo(const bool state);
  bool getTurbo();
  void setSwingVertical(const bool automatic, const uint8_t position);
  bool getSwingVerticalAuto();
  uint8_t getSwingVerticalPosition();

  uint8_t* getRaw();
  void setRaw(uint8_t new_code[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kGreeStateLength);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kGreeStateLength];
  void checksum(const uint16_t length = kGreeStateLength);
  void fixup();
  IRsend _irsend;
};

#endif  // IR_GREE_H_
