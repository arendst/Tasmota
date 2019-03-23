// Copyright 2019 David Conran

#ifndef IR_TCL_H_
#define IR_TCL_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

// Constants
const uint16_t kTcl112AcHdrMark = 3000;
const uint16_t kTcl112AcHdrSpace = 1650;
const uint16_t kTcl112AcBitMark = 500;
const uint16_t kTcl112AcOneSpace = 1050;
const uint16_t kTcl112AcZeroSpace = 325;
const uint32_t kTcl112AcGap = 100000;  // Just a guess.

const uint8_t kTcl112AcHeat = 1;
const uint8_t kTcl112AcDry =  2;
const uint8_t kTcl112AcCool = 3;
const uint8_t kTcl112AcFan =  7;
const uint8_t kTcl112AcAuto = 8;

const uint8_t kTcl112AcHalfDegree = 0b00100000;
const float   kTcl112AcTempMax = 31.0;
const float   kTcl112AcTempMin = 16.0;
const uint8_t kTcl112AcPowerMask = 0x04;

class IRTcl112Ac {
 public:
  explicit IRTcl112Ac(uint16_t pin);

#if SEND_TCL112AC
  void send(const uint16_t repeat = kTcl112AcDefaultRepeat);
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
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  uint8_t remote_state[kTcl112AcStateLength];
  void stateReset();
  void checksum(const uint16_t length = kTcl112AcStateLength);
  IRsend _irsend;
};

#endif  // IR_TCL_H_
