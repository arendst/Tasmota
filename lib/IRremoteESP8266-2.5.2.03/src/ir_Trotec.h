// Copyright 2017 stufisher

#ifndef IR_TROTEC_H_
#define IR_TROTEC_H_

#include "IRremoteESP8266.h"
#include "IRsend.h"

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

const uint8_t kTrotecOn = 1;
const uint8_t kTrotecOff = 0;

const uint8_t kTrotecFanLow = 1;
const uint8_t kTrotecFanMed = 2;
const uint8_t kTrotecFanHigh = 3;

// Byte 3
const uint8_t kTrotecMinTemp = 18;
const uint8_t kTrotecDefTemp = 25;
const uint8_t kTrotecMaxTemp = 32;

const uint8_t kTrotecSleepOn = 1;

// Byte 5
const uint8_t kTrotecTimerOn = 1;

// Byte 6
const uint8_t kTrotecMinTimer = 0;
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
#define TROTEC_MIN_TIMER kTrotecMinTimer
#define TROTEC_MAX_TIMER kTrotecMaxTimer

class IRTrotecESP {
 public:
  explicit IRTrotecESP(uint16_t pin);

#if SEND_TROTEC
  void send();
#endif  // SEND_TROTEC
  void begin();

  void setPower(bool state);
  uint8_t getPower();

  void setTemp(uint8_t temp);
  uint8_t getTemp();

  void setSpeed(uint8_t fan);
  uint8_t getSpeed();

  uint8_t getMode();
  void setMode(uint8_t mode);

  bool getSleep();
  void setSleep(bool sleep);

  uint8_t getTimer();
  void setTimer(uint8_t timer);

  uint8_t* getRaw();

 private:
  uint8_t trotec[kTrotecStateLength];
  void stateReset();
  void checksum();
  IRsend _irsend;
};

#endif  // IR_TROTEC_H_
