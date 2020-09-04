// Copyright 2019 Fabien Valthier

#ifndef IR_TECO_H_
#define IR_TECO_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

// Constants. Using LSB to be able to send only 35bits.
const uint8_t kTecoAuto = 0;  // 0b000
const uint8_t kTecoCool = 1;  // 0b001
const uint8_t kTecoDry = 2;  // 0b010
const uint8_t kTecoFan = 3;  // 0b110
const uint8_t kTecoHeat = 4;  // 0b001
const uint8_t kTecoFanAuto = 0;  // 0b00
const uint8_t kTecoFanHigh = 3;  // 0b11
const uint8_t kTecoFanMed = 2;  // 0b10
const uint8_t kTecoFanLow = 1;  // 0b01
const uint8_t kTecoMinTemp = 16;   // 16C
const uint8_t kTecoMaxTemp = 30;   // 30C

const uint64_t kTecoModeMask =   0b00000000000000000000000000000000111;
const uint64_t kTecoPower =      0b00000000000000000000000000000001000;
const uint64_t kTecoFanMask =    0b00000000000000000000000000000110000;
const uint64_t kTecoSwing =      0b00000000000000000000000000001000000;
const uint64_t kTecoSleep =      0b00000000000000000000000000010000000;
const uint64_t kTecoTempMask =   0b00000000000000000000000111100000000;
const uint64_t kTecoTimerHalfH = 0b00000000000000000000001000000000000;
const uint64_t kTecoTimerTenHr = 0b00000000000000000000110000000000000;
const uint64_t kTecoTimerOn =    0b00000000000000000001000000000000000;
const uint64_t kTecoTimerUniHr = 0b00000000000000011110000000000000000;
const uint64_t kTecoReset =      0b01001010000000000000010000000000000;
/*
  (header mark and space)
        Teco AC map read and to be sent in LSB with number of bits

  byte 0 = Cst 0x02
  byte 1 = Cst 0x50
  byte 2:
    b0-3 = 0b0000
    b4-7 = Timer hours (unit, not thenth)
      hours:
        0000 (0) = +0 hour
        0001 (1) = +1 hour
        ...
        1001 (9) = +9 hours
  byte 3: = timer and Temperature
    b0 = Timer (1 = On, 0 = Off)
    b1-2 = Timer - number of 10hours
      10Hours:
        00 = 0 * 10hours of timer
        01 = 1 * 10 hours of timer
        10 = 2 * 10hours of timer
    b3 = Timer - half hour (1=half hour on, 0 = round hour)
    b4-7: Degrees C.
      0000 (0) = 16C
      0001 (1) = 17C
      0010 (2) = 18C
      ...
      1101 (13) = 29C
      1110 (14) = 30C
  byte 4: Basics
    b0 = Sleep Mode (1 = On, 0 = Off)
    b1 = Vent swing (1 = On, 0 = Off)
    b2-3 = Fan
      Fan:
        00 = Auto
        01 = Fan 1
        10 = Fan 2
        11 = Fan 3 or higher
    b4 = Power Status (1 = On, 0 = Off)
    b5-7 = Modes LSB first
      Modes:
        000 = Auto (temp = 25C)
        001 = Cool
        010 = Dry (temp = 25C, but not shown)
        011 = Fan
        100 = Heat
*/

// Classes
class IRTecoAc {
 public:
  explicit IRTecoAc(const uint16_t pin);

  void stateReset(void);
#if SEND_TECO
  void send(const uint16_t repeat = kTecoDefaultRepeat);
#endif  // SEND_TECO
  void begin(void);
  void on(void);
  void off(void);

  void setPower(const bool on);
  bool getPower(void);

  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);

  void setFan(const uint8_t fan);
  uint8_t getFan(void);

  void setMode(const uint8_t mode);
  uint8_t getMode(void);

  void setSwing(const bool state);
  bool getSwing(void);

  void setSleep(const bool state);
  bool getSleep(void);

  // void setTimer(uint8_t time);  // To check unit
  // uint8_t getTimer(uint8_t);

  uint64_t getRaw(void);
  void setRaw(const uint64_t new_code);

  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
#ifdef ARDUINO
  String toString(void);
#else
  std::string toString(void);
#endif
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // The state of the IR remote in IR code form.
  uint64_t remote_state;
};

#endif  // IR_TECO_H_
