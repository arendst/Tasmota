// Whirlpool A/C
//
// Copyright 2018 David Conran

// Supports:
//   Brand: Whirlpool,  Model: DG11J1-3A remote
//   Brand: Whirlpool,  Model: DG11J1-04 remote
//   Brand: Whirlpool,  Model: DG11J1-91 remote
//   Brand: Whirlpool,  Model: SPIS409L A/C
//   Brand: Whirlpool,  Model: SPIS412L A/C
//   Brand: Whirlpool,  Model: SPIW409L A/C
//   Brand: Whirlpool,  Model: SPIW412L A/C
//   Brand: Whirlpool,  Model: SPIW418L A/C

#ifndef IR_WHIRLPOOL_H_
#define IR_WHIRLPOOL_H_

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

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/509

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
  explicit IRWhirlpoolAc(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);

  void stateReset(void);
#if SEND_WHIRLPOOL_AC
  void send(const uint16_t repeat = kWhirlpoolAcDefaultRepeat,
            const bool calcchecksum = true);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_WHIRLPOOL_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPowerToggle(const bool on);
  bool getPowerToggle(void);
  void setSleep(const bool on);
  bool getSleep(void);
  void setSuper(const bool on);
  bool getSuper(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwing(const bool on);
  bool getSwing(void);
  void setLight(const bool on);
  bool getLight(void);
  uint16_t getClock(void);
  void setClock(const uint16_t minspastmidnight);
  uint16_t getOnTimer(void);
  void setOnTimer(const uint16_t minspastmidnight);
  void enableOnTimer(const bool on);
  bool isOnTimerEnabled(void);
  uint16_t getOffTimer(void);
  void setOffTimer(const uint16_t minspastmidnight);
  void enableOffTimer(const bool on);
  bool isOffTimerEnabled(void);
  void setCommand(const uint8_t code);
  uint8_t getCommand(void);
  whirlpool_ac_remote_model_t getModel(void);
  void setModel(const whirlpool_ac_remote_model_t model);
  uint8_t* getRaw(const bool calcchecksum = true);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kWhirlpoolAcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kWhirlpoolAcStateLength);
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
  int8_t getTempOffset(void);
};

#endif  // IR_WHIRLPOOL_H_
