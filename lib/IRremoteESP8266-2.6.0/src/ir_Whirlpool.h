// Whirlpool A/C
//
// Copyright 2018 David Conran

#ifndef IR_WHIRLPOOL_H_
#define IR_WHIRLPOOL_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
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

//    WW      WW HH   HH IIIII RRRRRR  LL      PPPPPP   OOOOO   OOOOO  LL
//    WW      WW HH   HH  III  RR   RR LL      PP   PP OO   OO OO   OO LL
//    WW   W  WW HHHHHHH  III  RRRRRR  LL      PPPPPP  OO   OO OO   OO LL
//     WW WWW WW HH   HH  III  RR  RR  LL      PP      OO   OO OO   OO LL
//      WW   WW  HH   HH IIIII RR   RR LLLLLLL PP       OOOO0   OOOO0  LLLLLLL

// Ref:
//   https://github.com/markszabo/IRremoteESP8266/issues/509

// Constants
const uint8_t kWhirlpoolAcChecksumByte1 = 13;
const uint8_t kWhirlpoolAcChecksumByte2 = kWhirlpoolAcStateLength - 1;
const uint8_t kWhirlpoolAcHeat = 0;
const uint8_t kWhirlpoolAcAuto = 1;
const uint8_t kWhirlpoolAcCool = 2;
const uint8_t kWhirlpoolAcDry = 3;
const uint8_t kWhirlpoolAcFan = 4;
const uint8_t kWhirlpoolAcModeMask = 0b00000111;
const uint8_t kWhirlpoolAcModePos = 3;
const uint8_t kWhirlpoolAcFanAuto = 0;
const uint8_t kWhirlpoolAcFanHigh = 1;
const uint8_t kWhirlpoolAcFanMedium = 2;
const uint8_t kWhirlpoolAcFanLow = 3;
const uint8_t kWhirlpoolAcFanMask = 0b00000011;
const uint8_t kWhirlpoolAcFanPos = 2;
const uint8_t kWhirlpoolAcMinTemp = 18;     // 18C (DG11J1-3A), 16C (DG11J1-91)
const uint8_t kWhirlpoolAcMaxTemp = 32;     // 32C (DG11J1-3A), 30C (DG11J1-91)
const uint8_t kWhirlpoolAcAutoTemp = 23;    // 23C
const uint8_t kWhirlpoolAcTempMask = 0b11110000;
const uint8_t kWhirlpoolAcTempPos = 3;
const uint8_t kWhirlpoolAcSwing1Mask = 0b10000000;
const uint8_t kWhirlpoolAcSwing2Mask = 0b01000000;
const uint8_t kWhirlpoolAcLightMask = 0b00100000;
const uint8_t kWhirlpoolAcPowerToggleMask = 0b00000100;
const uint8_t kWhirlpoolAcPowerTogglePos = 2;
const uint8_t kWhirlpoolAcSleepMask = 0b00001000;
const uint8_t kWhirlpoolAcSleepPos = 2;
const uint8_t kWhirlpoolAcSuperMask = 0b10010000;
const uint8_t kWhirlpoolAcSuperPos = 5;
const uint8_t kWhirlpoolAcHourMask = 0b00011111;
const uint8_t kWhirlpoolAcMinuteMask = 0b00111111;
const uint8_t kWhirlpoolAcTimerEnableMask = 0b10000000;
const uint8_t kWhirlpoolAcClockPos = 6;
const uint8_t kWhirlpoolAcOffTimerPos = 8;
const uint8_t kWhirlpoolAcOnTimerPos = 10;
const uint8_t kWhirlpoolAcCommandPos = 15;
const uint8_t kWhirlpoolAcCommandLight = 0x00;
const uint8_t kWhirlpoolAcCommandPower = 0x01;
const uint8_t kWhirlpoolAcCommandTemp = 0x02;
const uint8_t kWhirlpoolAcCommandSleep = 0x03;
const uint8_t kWhirlpoolAcCommandSuper = 0x04;
const uint8_t kWhirlpoolAcCommandOnTimer = 0x05;
const uint8_t kWhirlpoolAcCommandMode = 0x06;
const uint8_t kWhirlpoolAcCommandSwing = 0x07;
const uint8_t kWhirlpoolAcCommandIFeel = 0x0D;
const uint8_t kWhirlpoolAcCommandFanSpeed = 0x11;
const uint8_t kWhirlpoolAcCommand6thSense = 0x17;
const uint8_t kWhirlpoolAcCommandOffTimer = 0x1D;
const uint8_t kWhirlpoolAcAltTempMask = 0b00001000;
const uint8_t kWhirlpoolAcAltTempPos = 18;

enum whirlpool_ac_remote_model_t {
  DG11J13A = 1,  // DG11J1-04 too
  DG11J191,
};

// Classes
class IRWhirlpoolAc {
 public:
  explicit IRWhirlpoolAc(uint16_t pin);

  void stateReset();
#if SEND_WHIRLPOOL_AC
  void send(const uint16_t repeat = kWhirlpoolAcDefaultRepeat,
            const bool calcchecksum = true);
#endif  // SEND_WHIRLPOOL_AC
  void begin();
  void on();
  void off();
  void setPowerToggle(const bool on);
  bool getPowerToggle();
  void setSleep(const bool on);
  bool getSleep();
  void setSuper(const bool on);
  bool getSuper();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwing(const bool on);
  bool getSwing();
  void setLight(const bool on);
  bool getLight();
  uint16_t getClock();
  void setClock(const uint16_t minspastmidnight);
  uint16_t getOnTimer();
  void setOnTimer(const uint16_t minspastmidnight);
  void enableOnTimer(const bool state);
  bool isOnTimerEnabled();
  uint16_t getOffTimer();
  void setOffTimer(const uint16_t minspastmidnight);
  void enableOffTimer(const bool state);
  bool isOffTimerEnabled();
  void setCommand(const uint8_t code);
  uint8_t getCommand();
  whirlpool_ac_remote_model_t getModel();
  void setModel(const whirlpool_ac_remote_model_t model);
  uint8_t* getRaw(const bool calcchecksum = true);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kWhirlpoolAcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kWhirlpoolAcStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kWhirlpoolAcStateLength];
  uint8_t _desiredtemp;
  void checksum(const uint16_t length = kWhirlpoolAcStateLength);
  uint16_t getTime(const uint16_t pos);
  void setTime(const uint16_t pos, const uint16_t minspastmidnight);
  bool isTimerEnabled(const uint16_t pos);
  void enableTimer(const uint16_t pos, const bool state);
  void _setTemp(const uint8_t temp, const bool remember = true);
  void _setMode(const uint8_t mode);
  int8_t getTempOffset();
#ifdef ARDUINO
  String timeToString(uint16_t minspastmidnight);
#else
  std::string timeToString(uint16_t minspastmidnight);
#endif
};

#endif  // IR_WHIRLPOOL_H_
