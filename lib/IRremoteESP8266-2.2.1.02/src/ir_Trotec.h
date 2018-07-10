// Copyright 2017 stufisher

#ifndef IR_TROTEC_H_
#define IR_TROTEC_H_

#include "IRremoteESP8266.h"
#include "IRsend.h"

// Constants
// Byte 0
#define TROTEC_INTRO1      0x12

// Byte 1
#define TROTEC_INTRO2      0x34

// Byte 2
#define TROTEC_AUTO           0
#define TROTEC_COOL           1
#define TROTEC_DRY            2
#define TROTEC_FAN            3

#define TROTEC_ON             1
#define TROTEC_OFF            0

#define TROTEC_FAN_LOW        1
#define TROTEC_FAN_MED        2
#define TROTEC_FAN_HIGH       3

// Byte 3
#define TROTEC_MIN_TEMP      18
#define TROTEC_MAX_TEMP      32
#define TROTEC_DEF_TEMP      25

#define TROTEC_SLEEP_ON       1

// Byte 5
#define TROTEC_TIMER_ON       1

// Byte 6
#define TROTEC_MIN_TIMER      0
#define TROTEC_MAX_TIMER     23

#if SEND_TROTEC

class IRTrotecESP {
 public:
  explicit IRTrotecESP(uint16_t pin);

  void send();
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
  uint8_t trotec[TROTEC_COMMAND_LENGTH];
  void stateReset();
  void checksum();
  IRsend _irsend;
};
#endif

#endif  // IR_TROTEC_H_
