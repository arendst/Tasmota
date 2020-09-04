// Corona A/C
//
// Copyright 2020 Christian Nilsson

// Supports:
//   Brand: Corona,  Model: CSH-N2211 A/C
//   Brand: Corona,  Model: CSH-N2511 A/C
//   Brand: Corona,  Model: CSH-N2811 A/C
//   Brand: Corona,  Model: CSH-N4011 A/C
//   Brand: Corona,  Model: AR-01 remote
//
// Ref: https://docs.google.com/spreadsheets/d/1zzDEUQ52y7MZ7_xCU3pdjdqbRXOwZLsbTGvKWcicqCI/
// Ref: https://www.corona.co.jp/box/download.php?id=145060636229

#ifndef IR_CORONA_H_
#define IR_CORONA_H_

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

// Constants

// CORONA_AC
const uint8_t kCoronaAcSectionBytes = 7;  // kCoronaAcStateLengthShort
const uint8_t kCoronaAcSections = 3;
const uint8_t kCoronaAcSectionHeader0Pos = 0;
const uint8_t kCoronaAcSectionHeader0 = 0x28;
const uint8_t kCoronaAcSectionHeader1Pos = 1;
const uint8_t kCoronaAcSectionHeader1 = 0x61;
const uint8_t kCoronaAcSectionLabelPos = 2;
const uint8_t kCoronaAcSectionLabelBase = 0x0D;  // 0b1101
const uint8_t kCoronaAcSectionData0Pos = 3;
const uint8_t kCoronaAcSectionData0InvPos = 4;
const uint8_t kCoronaAcSectionData1Pos = 5;
const uint8_t kCoronaAcSectionData1InvPos = 6;
const uint8_t kCoronaAcSectionData0Base = 0x10;  // D0 Pos 4 always on

const uint8_t kCoronaAcSettingsSection = 0;
// D0
const uint8_t kCoronaAcFanOffset = 0;        // D0 LSB Pos 0-1
const uint8_t kCoronaAcFanSize = 2;
const uint8_t kCoronaAcFanAuto =    0b00;  // 0
const uint8_t kCoronaAcFanLow =     0b01;  // 1
const uint8_t kCoronaAcFanMedium =  0b10;  // 2
const uint8_t kCoronaAcFanHigh =    0b11;  // 3

// One bit unknown                           // D0 Pos 2
const uint8_t kCoronaAcPowerSaveOffset = 3;  // D0 Pos 3
// One bit unknown always on                 // D0 Pos 4
// One bit unknown                           // D0 Pos 5
const uint8_t kCoronaAcSwingVToggleOffset = 6;  // D0 Pos 6
// One bit unknown                           // D0 MSB Pos 7

// D1
/* full auto mode not supported by this code yet
const uint8_t kCoronaAcAutoD0 = 0b00010100; // only combined with power save
const uint8_t kCoronaAcAutoD1 = 0b10000011; // only combined with power
*/
const uint8_t kCoronaAcTempOffset = 0;       // D1 LSB Pos 0
const uint8_t kCoronaAcTempSize = 4;
const uint8_t kCoronaAcMinTemp = 17;  // Celsius = 0b0001
const uint8_t kCoronaAcMaxTemp = 30;  // Celsius = 0b1110
const uint8_t kCoronaAcPowerOffset =
    kCoronaAcTempOffset + kCoronaAcTempSize;  // D1 Pos 4
const uint8_t kCoronaAcPowerButtonOffset =
    kCoronaAcPowerOffset + 1;                // D1 Pos 5
const uint8_t kCoronaAcModeOffset =
    kCoronaAcPowerButtonOffset + 1;          // D1 MSB Pos 6-7
const uint8_t kCoronaAcModeSize = 2;
const uint8_t kCoronaAcModeHeat = 0b00;  // 0
const uint8_t kCoronaAcModeDry =  0b01;  // 1
const uint8_t kCoronaAcModeCool = 0b10;  // 2
const uint8_t kCoronaAcModeFan =  0b11;  // 3

const uint8_t kCoronaAcOnTimerSection = 1;
const uint8_t kCoronaAcOffTimerSection = 2;
const uint16_t kCoronaAcTimerMax = 12 * 60;  // 12H in Minutes
// Min value on remote is 1 hour, actual sent value can be 2 secs
const uint16_t kCoronaAcTimerOff = 0xffff;
const uint16_t kCoronaAcTimerUnitsPerMin = 30;  // 30 units = 1 minute

// Classes

/// Class for handling detailed Corona A/C messages.
class IRCoronaAc {
 public:
  explicit IRCoronaAc(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);

  void stateReset();
#if SEND_CORONA_AC
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_CORONA_AC
  void begin();
  static bool validSection(const uint8_t state[], const uint16_t pos,
                           const uint8_t section);
  void setPower(const bool on);
  bool getPower();
  bool getPowerButton();
  void on();
  void off();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setSwingVToggle(const bool on);
  bool getSwingVToggle(void);
  void setFan(const uint8_t speed);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setEcono(const bool on);
  bool getEcono(void);
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOnTimer(void);
  void setOffTimer(const uint16_t nr_of_mins);
  uint16_t getOffTimer(void);
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kCoronaAcStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon();
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  uint8_t remote_state[kCoronaAcStateLength];  ///< The state of the IR remote.
  static uint8_t getSectionByte(const uint8_t section);
  static void checksum(uint8_t* data);
  void setPowerButton(const bool on);
  void _setPower(const bool on);
  void _setTimer(const uint8_t section, const uint16_t nr_of_mins);
  uint16_t _getTimer(const uint8_t section);
};
#endif  // IR_CORONA_H_
