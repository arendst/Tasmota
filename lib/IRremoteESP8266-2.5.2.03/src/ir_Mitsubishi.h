// Copyright 2009 Ken Shirriff
// Copyright 2017 David Conran
#ifndef IR_MITSUBISHI_H_
#define IR_MITSUBISHI_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

//    MMMMM  IIIII TTTTT   SSSS  U   U  BBBB   IIIII   SSSS  H   H  IIIII
//    M M M    I     T    S      U   U  B   B    I    S      H   H    I
//    M M M    I     T     SSS   U   U  BBBB     I     SSS   HHHHH    I
//    M   M    I     T        S  U   U  B   B    I        S  H   H    I
//    M   M  IIIII   T    SSSS    UUU   BBBBB  IIIII  SSSS   H   H  IIIII

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
const uint8_t kMitsubishiAcMinTemp = 16;  // 16C
const uint8_t kMitsubishiAcMaxTemp = 31;  // 31C
const uint8_t kMitsubishiAcVaneAuto = 0;
const uint8_t kMitsubishiAcVaneAutoMove = 7;
const uint8_t kMitsubishiAcNoTimer = 0;
const uint8_t kMitsubishiAcStartTimer = 5;
const uint8_t kMitsubishiAcStopTimer = 3;
const uint8_t kMitsubishiAcStartStopTimer = 7;

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
  explicit IRMitsubishiAC(uint16_t pin);

  static uint8_t calculateChecksum(uint8_t* data);

  void stateReset();
#if SEND_MITSUBISHI_AC
  void send();
#endif  // SEND_MITSUBISHI_AC
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFan(uint8_t fan);
  uint8_t getFan();
  void setMode(uint8_t mode);
  uint8_t getMode();
  void setVane(uint8_t mode);
  uint8_t getVane();
  uint8_t* getRaw();
  void setRaw(uint8_t* data);
  uint8_t getClock();
  void setClock(uint8_t clock);
  uint8_t getStartClock();
  void setStartClock(uint8_t clock);
  uint8_t getStopClock();
  void setStopClock(uint8_t clock);
  uint8_t getTimer();
  void setTimer(uint8_t timer);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
#ifdef ARDUINO
  String timeToString(uint64_t time);
#else
  std::string timeToString(uint64_t time);
#endif
  uint8_t remote_state[kMitsubishiACStateLength];
  void checksum();
  IRsend _irsend;
};

#endif  // IR_MITSUBISHI_H_
