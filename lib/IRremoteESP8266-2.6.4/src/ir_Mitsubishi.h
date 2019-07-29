// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran

// Mitsubishi

// Supports:
//   Brand: Mitsubishi,  Model: TV
//   Brand: Mitsubishi,  Model: HC3000 Projector

#ifndef IR_MITSUBISHI_H_
#define IR_MITSUBISHI_H_

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

// Mitsubishi (TV) decoding added from https://github.com/z3t0/Arduino-IRremote
// Mitsubishi (TV) sending & Mitsubishi A/C support added by David Conran

// Constants
const uint8_t kMitsubishiAcAuto = 0x20;
const uint8_t kMitsubishiAcCool = 0x18;
const uint8_t kMitsubishiAcDry = 0x10;
const uint8_t kMitsubishiAcHeat = 0x08;
const uint8_t kMitsubishiAcPower = 0x20;
const uint8_t kMitsubishiAcFanAuto = 0;
const uint8_t kMitsubishiAcFanMax = 5;
const uint8_t kMitsubishiAcFanRealMax = 4;
const uint8_t kMitsubishiAcFanSilent = 6;
const uint8_t kMitsubishiAcFanQuiet = kMitsubishiAcFanSilent;
const uint8_t kMitsubishiAcMinTemp = 16;  // 16C
const uint8_t kMitsubishiAcMaxTemp = 31;  // 31C
const uint8_t kMitsubishiAcVaneAuto = 0;
const uint8_t kMitsubishiAcVaneAutoMove = 7;
const uint8_t kMitsubishiAcNoTimer = 0;
const uint8_t kMitsubishiAcStartTimer = 5;
const uint8_t kMitsubishiAcStopTimer = 3;
const uint8_t kMitsubishiAcStartStopTimer = 7;
const uint8_t kMitsubishiAcWideVaneAuto = 8;

// Legacy defines (Deprecated)
#define MITSUBISHI_AC_VANE_AUTO_MOVE kMitsubishiAcVaneAutoMove
#define MITSUBISHI_AC_VANE_AUTO kMitsubishiAcVaneAuto
#define MITSUBISHI_AC_POWER kMitsubishiAcPower
#define MITSUBISHI_AC_MIN_TEMP kMitsubishiAcMinTemp
#define MITSUBISHI_AC_MAX_TEMP kMitsubishiAcMaxTemp
#define MITSUBISHI_AC_HEAT kMitsubishiAcHeat
#define MITSUBISHI_AC_FAN_SILENT kMitsubishiAcFanSilent
#define MITSUBISHI_AC_FAN_REAL_MAX kMitsubishiAcFanRealMax
#define MITSUBISHI_AC_FAN_MAX kMitsubishiAcFanMax
#define MITSUBISHI_AC_FAN_AUTO kMitsubishiAcFanAuto
#define MITSUBISHI_AC_DRY kMitsubishiAcDry
#define MITSUBISHI_AC_COOL kMitsubishiAcCool
#define MITSUBISHI_AC_AUTO kMitsubishiAcAuto

class IRMitsubishiAC {
 public:
  explicit IRMitsubishiAC(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);

  static uint8_t calculateChecksum(const uint8_t* data);

  void stateReset(void);
#if SEND_MITSUBISHI_AC
  void send(const uint16_t repeat = kMitsubishiACMinRepeat);
  uint8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MITSUBISHI_AC
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
  void setVane(const uint8_t position);
  void setWideVane(const uint8_t position);
  uint8_t getVane(void);
  uint8_t getWideVane(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t* data);
  uint8_t getClock(void);
  void setClock(const uint8_t clock);
  uint8_t getStartClock(void);
  void setStartClock(const uint8_t clock);
  uint8_t getStopClock(void);
  void setStopClock(const uint8_t clock);
  uint8_t getTimer(void);
  void setTimer(const uint8_t timer);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  uint8_t convertSwingV(const stdAc::swingv_t position);
  uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  uint8_t remote_state[kMitsubishiACStateLength];
  void checksum(void);
};

#endif  // IR_MITSUBISHI_H_
