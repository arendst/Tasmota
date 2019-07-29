// Copyright 2017 stufisher
// Copyright 2019 crankyoldgit

#ifndef IR_TROTEC_H_
#define IR_TROTEC_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

// Constants
// Byte 0
const uint8_t kTrotecIntro1 = 0x12;

// Byte 1
const uint8_t kTrotecIntro2 = 0x34;

// Byte 2
const uint8_t kTrotecAuto = 0;
const uint8_t kTrotecCool = 1;
const uint8_t kTrotecDry = 2;
const uint8_t kTrotecFan = 3;

const uint8_t kTrotecPowerBit = 0b00001000;

const uint8_t kTrotecFanLow = 1;
const uint8_t kTrotecFanMed = 2;
const uint8_t kTrotecFanHigh = 3;

// Byte 3
const uint8_t kTrotecMinTemp = 18;
const uint8_t kTrotecDefTemp = 25;
const uint8_t kTrotecMaxTemp = 32;

const uint8_t kTrotecSleepBit = 0b10000000;

// Byte 5
const uint8_t kTrotecTimerBit = 0b01000000;

// Byte 6
const uint8_t kTrotecMaxTimer = 23;

// Legacy defines. (Deperecated)
#define TROTEC_AUTO kTrotecAuto
#define TROTEC_COOL kTrotecCool
#define TROTEC_DRY kTrotecDry
#define TROTEC_FAN kTrotecFan
#define TROTEC_FAN_LOW kTrotecFanLow
#define TROTEC_FAN_MED kTrotecFanMed
#define TROTEC_FAN_HIGH kTrotecFanHigh
#define TROTEC_MIN_TEMP kTrotecMinTemp
#define TROTEC_MAX_TEMP kTrotecMaxTemp
#define TROTEC_MAX_TIMER kTrotecMaxTimer

class IRTrotecESP {
 public:
  explicit IRTrotecESP(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);

#if SEND_TROTEC
  void send(const uint16_t repeat = kTrotecDefaultRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TROTEC
  void begin(void);

  void setPower(const bool state);
  bool getPower(void);

  void setTemp(const uint8_t celsius);
  uint8_t getTemp(void);

  void setSpeed(const uint8_t fan);
  uint8_t getSpeed(void);

  uint8_t getMode(void);
  void setMode(const uint8_t mode);

  bool getSleep(void);
  void setSleep(const bool on);

  uint8_t getTimer(void);
  void setTimer(const uint8_t timer);

  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kTrotecStateLength);
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
  uint8_t remote_state[kTrotecStateLength];
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kTrotecStateLength);
  void stateReset(void);
  void checksum(void);
};

#endif  // IR_TROTEC_H_
