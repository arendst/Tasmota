// Delonghi A/C
//
// Copyright 2020 David Conran

#ifndef IR_DELONGHI_H_
#define IR_DELONGHI_H_

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

// Supports:
//   Brand: Delonghi,  Model: PAC A95

// Ref:
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/1096

// Kudos:
//   TheMaxxz: For the breakdown and mapping of the bit values.

/* State bit map:

+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------+
|     FIXED HEADER      | TEMPERATURE  | FAN |F or C|
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+------+
  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14   15

+--+--+--+--+-----+-----+
|ON|  MODE  |Boost|Sleep|
+--+--+--+--+-----+-----+
16 17 18 19    20    21

+--+--+------------+--+--+--+--+--+--+--+--+--+--+--+--+--+
| 0| 0|Timer Enable| ON TIME HOUR | 0  0|   ON TIME MIN   |
+--+--+------------+--+--+--+--+--+--+--+--+--+--+--+--+--+
 22 23           24 25 26 27 28 29 30 31 32 33 34 35 36 37

+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| 0  0|                   OFF TIMER                   |       CHECKSUM        |
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63

*/

// Constants
const uint8_t kDelonghiAcTempOffset = 8;
const uint8_t kDelonghiAcTempSize = 5;
const uint8_t kDelonghiAcTempMinC = 18;  // Deg C
const uint8_t kDelonghiAcTempMaxC = 32;  // Deg C
const uint8_t kDelonghiAcTempMinF = 64;  // Deg F
const uint8_t kDelonghiAcTempMaxF = 90;  // Deg F
const uint8_t kDelonghiAcTempAutoDryMode = 0;
const uint8_t kDelonghiAcTempFanMode = 0b00110;
const uint8_t kDelonghiAcFanOffset = kDelonghiAcTempOffset +
    kDelonghiAcTempSize;  // 13
const uint8_t kDelonghiAcFanSize = 2;
const uint8_t kDelonghiAcFanAuto =   0b00;
const uint8_t kDelonghiAcFanHigh =   0b01;
const uint8_t kDelonghiAcFanMedium = 0b10;
const uint8_t kDelonghiAcFanLow =    0b11;
const uint8_t kDelonghiAcTempUnitBit = kDelonghiAcFanOffset +
    kDelonghiAcFanSize;  // 15 (1 = Celsius, 0 = Fahrenheit)
const uint8_t kDelonghiAcPowerBit = kDelonghiAcTempUnitBit + 1;  // 16
const uint8_t kDelonghiAcModeOffset = kDelonghiAcPowerBit + 1;  // 17
const uint8_t kDelonghiAcModeSize = 3;
const uint8_t kDelonghiAcCool =   0b000;
const uint8_t kDelonghiAcDry =    0b001;
const uint8_t kDelonghiAcFan =    0b010;
const uint8_t kDelonghiAcAuto =   0b100;
const uint8_t kDelonghiAcBoostBit = kDelonghiAcModeOffset +
    kDelonghiAcModeSize;  // 20 (Aka Turbo)
const uint8_t kDelonghiAcSleepBit = kDelonghiAcBoostBit + 1;  // 21
// Two zero bits
const uint8_t kDelonghiAcOnTimerEnableBit = kDelonghiAcSleepBit + 3;  // 24
const uint8_t kDelonghiAcHoursSize = 5;  // Max 23 hrs
const uint8_t kDelonghiAcMinsSize = 6;  // Max 59 mins
const uint16_t kDelonghiAcTimerMax = 23 * 60 + 59;
const uint8_t kDelonghiAcOnTimerHoursOffset = kDelonghiAcOnTimerEnableBit +
    1;  // 25
const uint8_t kDelonghiAcOnTimerMinsOffset = kDelonghiAcOnTimerHoursOffset +
    kDelonghiAcHoursSize + 2;  // 32  (inc another two zero bits)
// Two zero bits
const uint8_t kDelonghiAcOffTimerEnableBit = kDelonghiAcOnTimerMinsOffset +
    kDelonghiAcMinsSize + 2;  // 40
const uint8_t kDelonghiAcOffTimerHoursOffset = kDelonghiAcOffTimerEnableBit +
    1;  // 41
const uint8_t kDelonghiAcOffTimerMinsOffset = kDelonghiAcOffTimerHoursOffset +
    kDelonghiAcHoursSize + 2;  // 48  (inc another two zero bits)
// Two zero bits
const uint8_t kDelonghiAcChecksumOffset = kDelonghiAcOffTimerMinsOffset +
    kDelonghiAcMinsSize + 2;  // 56
const uint8_t kDelonghiAcChecksumSize = 8;


// Classes
class IRDelonghiAc {
 public:
  explicit IRDelonghiAc(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);

  void stateReset();
#if SEND_DELONGHI_AC
  void send(const uint16_t repeat = kDelonghiAcDefaultRepeat);
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_DELONGHI_AC
  void begin();
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPower(const bool on);
  bool getPower();
  void on();
  void off();
  void setTempUnit(const bool celsius);
  bool getTempUnit(void);
  void setTemp(const uint8_t temp, const bool fahrenheit = false,
               const bool force = false);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setBoost(const bool on);  // Aka Turbo
  bool getBoost();  // Aka Turbo
  void setSleep(const bool on);
  bool getSleep();
  void setOnTimerEnabled(const bool on);
  bool getOnTimerEnabled(void);
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOnTimer(void);
  void setOffTimerEnabled(const bool on);
  bool getOffTimerEnabled(void);
  void setOffTimer(const uint16_t nr_of_mins);
  uint16_t getOffTimer(void);
  uint64_t getRaw();
  void setRaw(const uint64_t state);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  uint64_t remote_state;  // The state of the IR remote.
  uint8_t _saved_temp;  // The previously user requested temp value.
  uint8_t _saved_temp_units;  // The previously user requested temp units.
  void checksum(void);
};
#endif  // IR_DELONGHI_H_
