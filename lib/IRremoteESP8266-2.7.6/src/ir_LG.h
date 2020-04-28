// Copyright 2017, 2019 David Conran

// Supports:
//   Brand: LG,  Model: 6711A20083V remote
//   Brand: LG,  Model: AKB74395308 remote
//   Brand: LG,  Model: S4-W12JA3AA A/C (LG2)
//   Brand: LG,  Model: AKB75215403 remote (LG2)
//   Brand: General Electric,  Model: AG1BH09AW101 Split A/C
//   Brand: General Electric,  Model: 6711AR2853M A/C Remote

#ifndef IR_LG_H_
#define IR_LG_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

const uint8_t kLgAcChecksumOffset = 0;  // Nr. of bits
const uint8_t kLgAcChecksumSize = kNibbleSize;  // Nr. of bits
const uint8_t kLgAcFanOffset = 4;  // Nr. of bits
const uint8_t kLgAcFanSize = 3;  // Nr. of bits
const uint8_t kLgAcFanLow = 0;     // 0b000
const uint8_t kLgAcFanMedium = 2;  // 0b010
const uint8_t kLgAcFanHigh = 4;    // 0b100
const uint8_t kLgAcFanAuto = 5;    // 0b101
const uint8_t kLgAcTempOffset = 8;  // Nr. of bits
const uint8_t kLgAcTempSize = 4;  // Nr. of bits
const uint8_t kLgAcTempAdjust = 15;
const uint8_t kLgAcMinTemp = 16;  // Celsius
const uint8_t kLgAcMaxTemp = 30;  // Celsius
const uint8_t kLgAcModeOffset = 12;  // Nr. of bits
const uint8_t kLgAcModeSize = 3;  // Nr. of bits
const uint8_t kLgAcCool = 0;  // 0b000
const uint8_t kLgAcDry = 1;   // 0b001
const uint8_t kLgAcFan = 2;   // 0b010
const uint8_t kLgAcAuto = 3;  // 0b011
const uint8_t kLgAcHeat = 4;  // 0b100
const uint8_t kLgAcPowerOffset = 18;  // Nr. of bits
const uint8_t kLgAcPowerSize = 2;  // Nr. of bits
const uint8_t kLgAcPowerOff = 3;  // 0b11
const uint8_t kLgAcPowerOn = 0;   // 0b00
const uint8_t kLgAcSignatureOffset = 20;  // Nr. of bits
const uint8_t kLgAcSignatureSize = 8;  // Nr. of bits
const uint8_t kLgAcSignature = 0x88;

const uint32_t kLgAcOffCommand = 0x88C0051;

uint8_t calcLGChecksum(uint16_t data);

// Classes
class IRLgAc {
 public:
  explicit IRLgAc(const uint16_t pin, const bool inverted = false,
                  const bool use_modulation = true);

  void stateReset(void);
  static uint8_t calcChecksum(const uint32_t state);
  static bool validChecksum(const uint32_t state);
  bool isValidLgAc(void);
#if SEND_LG
  void send(const uint16_t repeat = kLgDefaultRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_LG
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
  uint8_t getMode(void);
  uint32_t getRaw(void);
  void setRaw(const uint32_t new_code);
  uint8_t convertMode(const stdAc::opmode_t mode);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
  void setModel(const lg_ac_remote_model_t model);
  lg_ac_remote_model_t getModel(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // The state of the IR remote in IR code form.
  uint32_t remote_state;
  uint8_t _temp;
  decode_type_t _protocol;
  void checksum(void);
  void _setTemp(const uint8_t value);
};

#endif  // IR_LG_H_
