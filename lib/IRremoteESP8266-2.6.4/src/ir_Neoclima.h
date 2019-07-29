// Neoclima A/C
//
// Copyright 2019 David Conran

// Analysis by crankyoldgit & AndreyShpilevoy

#ifndef IR_NEOCLIMA_H_
#define IR_NEOCLIMA_H_

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

// Supports:
//   Brand: Neoclima,  Model: NS-09AHTI A/C
//   Brand: Neoclima,  Model: ZH/TY-01 remote

// Ref:
//  https://github.com/crankyoldgit/IRremoteESP8266/issues/764
//  https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view

// Constants
// state[1]
const uint8_t kNeoclima8CHeatMask = 0b00000010;
const uint8_t kNeoclimaIonMask =    0b00000100;
// state[3]
const uint8_t kNeoclimaLightMask =  0b00000001;
const uint8_t kNeoclimaHoldMask =   0b00000100;
const uint8_t kNeoclimaTurboMask =  0b00001000;
const uint8_t kNeoclimaEyeMask =    0b01000000;
// state[5]
const uint8_t kNeoclimaFreshMask =  0b10000000;
const uint8_t kNeoclimaButtonMask = 0b00011111;
const uint8_t kNeoclimaButtonPower =    0x00;
const uint8_t kNeoclimaButtonMode =     0x01;
const uint8_t kNeoclimaButtonTempUp =   0x02;
const uint8_t kNeoclimaButtonTempDown = 0x03;
const uint8_t kNeoclimaButtonSwing =    0x04;
const uint8_t kNeoclimaButtonFanSpeed = 0x05;
const uint8_t kNeoclimaButtonAirFlow =  0x07;
const uint8_t kNeoclimaButtonHold =     0x08;
const uint8_t kNeoclimaButtonSleep =    0x09;
const uint8_t kNeoclimaButtonTurbo =    0x0A;
const uint8_t kNeoclimaButtonLight =    0x0B;
const uint8_t kNeoclimaButtonEye =      0x0E;
const uint8_t kNeoclimaButtonFollow =   0x13;
const uint8_t kNeoclimaButtonIon =      0x14;
const uint8_t kNeoclimaButtonFresh =    0x15;
const uint8_t kNeoclimaButton8CHeat =   0x1D;
// state[7]
const uint8_t kNeoclimaSleepMask =  0b00000001;
const uint8_t kNeoclimaPowerMask =  0b00000010;
const uint8_t kNeoclimaSwingVMask = 0b00001100;
const uint8_t kNeoclimaSwingVOn =   0b00000100;
const uint8_t kNeoclimaSwingVOff =  0b00001000;
const uint8_t kNeoclimaSwingHMask = 0b00010000;
const uint8_t kNeoclimaFanMask =    0b01100000;
const uint8_t kNeoclimaFanAuto =     0b00;
const uint8_t kNeoclimaFanHigh =     0b01;
const uint8_t kNeoclimaFanMed =      0b10;
const uint8_t kNeoclimaFanLow =      0b11;
// state[8]
const uint8_t kNeoclimaFollowMe = 0x5D;  // Also 0x5F
// state[9]
const uint8_t kNeoclimaTempMask = 0b00011111;
const uint8_t kNeoclimaMinTemp = 16;   // 16C
const uint8_t kNeoclimaMaxTemp = 32;   // 32C
const uint8_t kNeoclimaModeMask = 0b11100000;
const uint8_t kNeoclimaAuto =     0b000;
const uint8_t kNeoclimaCool =     0b001;
const uint8_t kNeoclimaDry =      0b010;
const uint8_t kNeoclimaFan =      0b011;
const uint8_t kNeoclimaHeat =     0b100;

// Classes
class IRNeoclimaAc {
 public:
  explicit IRNeoclimaAc(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);

  void stateReset(void);
#if SEND_NEOCLIMA
  void send(const uint16_t repeat = kNeoclimaMinRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_NEOCLIMA
  void begin(void);
  void setButton(const uint8_t button);
  uint8_t getButton(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setSwingV(const bool on);
  bool getSwingV(void);
  void setSwingH(const bool on);
  bool getSwingH(void);
  void setSleep(const bool on);
  bool getSleep(void);
  void setTurbo(const bool on);
  bool getTurbo(void);
  void setFresh(const bool on);
  bool getFresh(void);
  void setHold(const bool on);
  bool getHold(void);
  void setIon(const bool on);
  bool getIon(void);
  void setLight(const bool on);
  bool getLight(void);
  void set8CHeat(const bool on);
  bool get8CHeat(void);
  void setEye(const bool on);
  bool getEye(void);
  // DISABLED: See TODO in ir_Neoclima.cpp
  // void setFollow(const bool on);
  bool getFollow(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kNeoclimaStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kNeoclimaStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kNeoclimaStateLength);
  String toString(void);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kNeoclimaStateLength];
  void checksum(const uint16_t length = kNeoclimaStateLength);
};

#endif  // IR_NEOCLIMA_H_
