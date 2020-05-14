// Copyright 2019 David Conran

// Supports:
//   Brand: Leberg,  Model: LBS-TOR07 A/C

#ifndef IR_TCL_H_
#define IR_TCL_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRrecv.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

// Constants
const uint16_t kTcl112AcHdrMark = 3000;
const uint16_t kTcl112AcHdrSpace = 1650;
const uint16_t kTcl112AcBitMark = 500;
const uint16_t kTcl112AcOneSpace = 1050;
const uint16_t kTcl112AcZeroSpace = 325;
const uint32_t kTcl112AcGap = kDefaultMessageGap;  // Just a guess.
// Total tolerance percentage to use for matching the header mark.
const uint8_t kTcl112AcHdrMarkTolerance = 6;
const uint8_t kTcl112AcTolerance = 5;  // Extra Percentage for the rest.

const uint8_t kTcl112AcHeat = 1;
const uint8_t kTcl112AcDry =  2;
const uint8_t kTcl112AcCool = 3;
const uint8_t kTcl112AcFan =  7;
const uint8_t kTcl112AcAuto = 8;
const uint8_t kTcl112AcModeSize = 4;  // Nr. of Bits

const uint8_t kTcl112AcFanSize = 3;  // Nr. of Bits. Mask = 0b00000111
const uint8_t kTcl112AcFanAuto = 0b000;
const uint8_t kTcl112AcFanLow  = 0b010;
const uint8_t kTcl112AcFanMed  = 0b011;
const uint8_t kTcl112AcFanHigh = 0b101;

const uint8_t kTcl112AcHalfDegreeOffset = 5;
const float   kTcl112AcTempMax    = 31.0;
const float   kTcl112AcTempMin    = 16.0;

const uint8_t kTcl112AcPowerOffset = 2;
const uint8_t kTcl112AcBitEconoOffset = 7;
const uint8_t kTcl112AcBitLightOffset = 6;
const uint8_t kTcl112AcBitHealthOffset = 4;
const uint8_t kTcl112AcBitSwingHOffset = 3;
const uint8_t kTcl112AcSwingVOffset = 3;  // Mask 0b00111000
const uint8_t kTcl112AcSwingVSize = 3;  // Nr. of bits.
const uint8_t kTcl112AcSwingVOn =    0b111;
const uint8_t kTcl112AcSwingVOff =   0b000;
const uint8_t kTcl112AcBitTurboOffset = 6;


class IRTcl112Ac {
 public:
  explicit IRTcl112Ac(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);

#if SEND_TCL112AC
  void send(const uint16_t repeat = kTcl112AcDefaultRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TCL
  void begin(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kTcl112AcStateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const float celsius);  // Celsius in 0.5 increments
  float getTemp(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  static uint8_t calcChecksum(uint8_t state[],
                              const uint16_t length = kTcl112AcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kTcl112AcStateLength);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setEcono(const bool on);
  bool getEcono(void);
  void setHealth(const bool on);
  bool getHealth(void);
  void setLight(const bool on);
  bool getLight(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setTurbo(const bool on);
  bool getTurbo(void);
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
  uint8_t remote_state[kTcl112AcStateLength];
  void stateReset(void);
  void checksum(const uint16_t length = kTcl112AcStateLength);
};

#endif  // IR_TCL_H_
