// Copyright 2020 Quentin Briollant

/// @file
/// @brief Support for Technibel protocol.

#ifndef IR_TECHNIBEL_H_
#define IR_TECHNIBEL_H_

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
//   Brand: Technibel,  Model: IRO PLUS


/* State bit map:

+--+--+--+--+--+--+--+--+--+------------+-----------+----------+--+--+--+--+
|     FIXED HEADER      |ON|TIMER CHANGE|TEMP CHANGE|FAN CHANGE|    MODE   |
+--+--+--+--+--+--+--+--+--+------------+-----------+----------+--+--+--+--+
 55 54 53 52 51 50 49 48 47      46           45         44     43 42 41 40

+-----+------+-----+-----+---+--+--+--+---+--+--+--+--+--+--+--+
|TIMER|C OR F|SWING|SLEEP| 0 |   FAN  | 0 |     TEMPERATURE    |
+-----+------+-----+-----+---+--+--+--+---+--+--+--+--+--+--+--+
   39    38     37    36   35 34 33 32  31 30 29 28 27 26 25 24

+---+---+---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| 0 | 0 | 0 | ON TIME HOUR |         FOOTER        |       CHECKSUM        |
+---+---+---+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 23  22  21  20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0


*/

// Constants
const uint8_t kTechnibelAcChecksumOffset = 0;
const uint8_t kTechnibelAcChecksumSize = 8;

const uint8_t kTechnibelAcFooterOffset = kTechnibelAcChecksumOffset +
    kTechnibelAcChecksumSize;
const uint8_t kTechnibelAcFooterSize = 8;

const uint8_t kTechnibelAcTimerHoursOffset = kTechnibelAcFooterOffset +
    kTechnibelAcFooterSize;
const uint8_t kTechnibelAcHoursSize = 8;  // Max 24 hrs
const uint8_t kTechnibelAcTimerMax = 24;

const uint8_t kTechnibelAcTempOffset = kTechnibelAcTimerHoursOffset +
    kTechnibelAcHoursSize;
const uint8_t kTechnibelAcTempSize = 8;
const uint8_t kTechnibelAcTempMinC = 16;  // Deg C
const uint8_t kTechnibelAcTempMaxC = 31;  // Deg C
const uint8_t kTechnibelAcTempMinF = 61;  // Deg F
const uint8_t kTechnibelAcTempMaxF = 88;  // Deg F

const uint8_t kTechnibelAcFanOffset = kTechnibelAcTempOffset
    + kTechnibelAcTempSize;
const uint8_t kTechnibelAcFanSize = 4;
const uint8_t kTechnibelAcFanLow =    0b0001;
const uint8_t kTechnibelAcFanMedium = 0b0010;
const uint8_t kTechnibelAcFanHigh =   0b0100;

const uint8_t kTechnibelAcSleepBit = kTechnibelAcFanOffset +
    kTechnibelAcFanSize;

const uint8_t kTechnibelAcSwingBit = kTechnibelAcSleepBit + 1;

// (0 = Celsius, 1 = Fahrenheit)
const uint8_t kTechnibelAcTempUnitBit = kTechnibelAcSwingBit + 1;

const uint8_t kTechnibelAcTimerEnableBit = kTechnibelAcTempUnitBit + 1;

const uint8_t kTechnibelAcModeOffset = kTechnibelAcTimerEnableBit + 1;
const uint8_t kTechnibelAcModeSize = 4;
const uint8_t kTechnibelAcCool = 0b0001;
const uint8_t kTechnibelAcDry =  0b0010;
const uint8_t kTechnibelAcFan =  0b0100;
const uint8_t kTechnibelAcHeat = 0b1000;

const uint8_t kTechnibelAcFanChangeBit = kTechnibelAcModeOffset +
    kTechnibelAcModeSize;

const uint8_t kTechnibelAcTempChangeBit = kTechnibelAcFanChangeBit + 1;

const uint8_t kTechnibelAcTimerChangeBit = kTechnibelAcTempChangeBit + 1;

const uint8_t kTechnibelAcPowerBit = kTechnibelAcTimerChangeBit + 1;

const uint8_t kTechnibelAcHeaderOffset = kTechnibelAcPowerBit + 1;
const uint8_t kTechnibelAcHeaderSize = 8;
const uint8_t kTechnibelAcHeader = 0b00011000;

const uint64_t kTechnibelAcResetState = 0x180101140000EA;  ///<
///< Mode:Cool, Power:Off, fan:Low, temp:20, swing:Off, sleep:Off


// Classes
/// Class for handling detailed Technibel A/C messages.
class IRTechnibelAc {
 public:
  explicit IRTechnibelAc(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);
  void stateReset();
#if SEND_TECHNIBEL_AC
  void send(const uint16_t repeat = kTechnibelAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TECHNIBEL_AC
  void begin();
  static uint8_t calcChecksum(const uint64_t state);
  static bool validChecksum(const uint64_t state);
  void setPower(const bool on);
  bool getPower();
  void on();
  void off();
  void setTempUnit(const bool celsius);
  bool getTempUnit(void);
  void setTemp(const uint8_t temp, const bool fahrenheit = false);
  uint8_t getTemp();
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwing(const bool on);
  bool getSwing();
  bool convertSwing(const stdAc::swingv_t swing);
  stdAc::swingv_t toCommonSwing(const bool swing);
  void setSleep(const bool on);
  bool getSleep();
  void setTimerEnabled(const bool on);
  bool getTimerEnabled(void);
  void setTimer(const uint16_t nr_of_mins);
  uint16_t getTimer(void);
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
#endif  // IR_TECHNIBEL_H_
