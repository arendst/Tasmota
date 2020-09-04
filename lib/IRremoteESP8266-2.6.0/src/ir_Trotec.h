// Copyright 2017 stufisher

#ifndef IR_TROTEC_H_
#define IR_TROTEC_H_

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
  explicit IRTrotecESP(uint16_t pin);

#if SEND_TROTEC
  void send(const uint16_t repeat = kTrotecDefaultRepeat);
#endif  // SEND_TROTEC
  void begin();

  void setPower(const bool state);
  bool getPower();

  void setTemp(const uint8_t celsius);
  uint8_t getTemp();

  void setSpeed(const uint8_t fan);
  uint8_t getSpeed();

  uint8_t getMode();
  void setMode(const uint8_t mode);

  bool getSleep();
  void setSleep(bool sleep);

  uint8_t getTimer();
  void setTimer(const uint8_t timer);

  uint8_t* getRaw();

  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  uint8_t remote_state[kTrotecStateLength];
  void stateReset();
  void checksum();
};

#endif  // IR_TROTEC_H_
