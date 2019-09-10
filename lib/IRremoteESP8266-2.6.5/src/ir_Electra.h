// Electra A/C
//
// Copyright 2019 David Conran

#ifndef IR_ELECTRA_H_
#define IR_ELECTRA_H_

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
//   Brand: AUX,  Model: KFR-35GW/BpNFW=3 A/C
//   Brand: AUX,  Model: YKR-T/011 remote

// Ref:
//  https://github.com/ToniA/arduino-heatpumpir/blob/master/AUXHeatpumpIR.cpp

// Constants
// state[1]
const uint8_t kElectraAcTempMask =   0b11111000;
const uint8_t kElectraAcMinTemp = 16;   // 16C
const uint8_t kElectraAcMaxTemp = 32;   // 32C
const uint8_t kElectraAcOffsetTemp = 8;
const uint8_t kElectraAcSwingVMask = 0b00000111;
// state[2]
const uint8_t kElectraAcSwingHMask = 0b11100000;
// state[4]
const uint8_t kElectraAcFanMask =    0b11100000;
const uint8_t kElectraAcFanAuto =    0b101;
const uint8_t kElectraAcFanLow =     0b011;
const uint8_t kElectraAcFanMed =     0b010;
const uint8_t kElectraAcFanHigh =    0b001;
// state[6]
const uint8_t kElectraAcModeMask = 0b11100000;
const uint8_t kElectraAcAuto =     0b000;
const uint8_t kElectraAcCool =     0b001;
const uint8_t kElectraAcDry =      0b010;
const uint8_t kElectraAcHeat =     0b100;
const uint8_t kElectraAcFan =      0b110;
// state[9]
const uint8_t kElectraAcPowerMask =  0b00100000;


// Classes
class IRElectraAc {
 public:
  explicit IRElectraAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

  void stateReset(void);
#if SEND_ELECTRA_AC
  void send(const uint16_t repeat = kElectraAcMinRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_ELECTRA_AC
  void begin(void);
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
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kElectraAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kElectraAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kElectraAcStateLength);
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
  uint8_t remote_state[kElectraAcStateLength];
  void checksum(const uint16_t length = kElectraAcStateLength);
};
#endif  // IR_ELECTRA_H_
