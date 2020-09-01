// Copyright 2018-2020 David Conran
/// @file
/// @brief Support for Hitachi A/C protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/417
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/453
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/973
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1056
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1060
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1134

// Supports:
//   Brand: Hitachi,  Model: RAS-35THA6 remote
//   Brand: Hitachi,  Model: LT0541-HTA remote  (HITACHI_AC1)
//   Brand: Hitachi,  Model: Series VI A/C (Circa 2007) (HITACHI_AC1)
//   Brand: Hitachi,  Model: RAR-8P2 remote (HITACHI_AC424)
//   Brand: Hitachi,  Model: RAS-AJ25H A/C (HITACHI_AC424)
//   Brand: Hitachi,  Model: PC-LH3B (HITACHI_AC3)
//   Brand: Hitachi,  Model: KAZE-312KSDP A/C (HITACHI_AC1)
//   Brand: Hitachi,  Model: R-LT0541-HTA/Y.K.1.1-1 V2.3 remote (HITACHI_AC1)
//   Brand: Hitachi,  Model: RAS-22NK A/C (HITACHI_AC344)
//   Brand: Hitachi,  Model: RF11T1 remote (HITACHI_AC344)

#ifndef IR_HITACHI_H_
#define IR_HITACHI_H_

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
const uint16_t kHitachiAcFreq = 38000;  // Hz.
const uint8_t kHitachiAcAuto = 2;
const uint8_t kHitachiAcHeat = 3;
const uint8_t kHitachiAcCool = 4;
const uint8_t kHitachiAcDry = 5;
const uint8_t kHitachiAcFan = 0xC;
const uint8_t kHitachiAcFanAuto = 1;
const uint8_t kHitachiAcFanLow = 2;
const uint8_t kHitachiAcFanMed = 3;
const uint8_t kHitachiAcFanHigh = 5;
const uint8_t kHitachiAcMinTemp = 16;   // 16C
const uint8_t kHitachiAcMaxTemp = 32;   // 32C
const uint8_t kHitachiAcAutoTemp = 23;  // 23C
const uint8_t kHitachiAcPowerOffset = 0;
const uint8_t kHitachiAcSwingOffset = 7;

// HitachiAc424 & HitachiAc344
// Byte[11]
const uint8_t kHitachiAc424ButtonByte = 11;
const uint8_t kHitachiAc424ButtonPowerMode = 0x13;
const uint8_t kHitachiAc424ButtonFan = 0x42;
const uint8_t kHitachiAc424ButtonTempDown = 0x43;
const uint8_t kHitachiAc424ButtonTempUp = 0x44;
const uint8_t kHitachiAc424ButtonSwingV = 0x81;
const uint8_t kHitachiAc424ButtonSwingH = 0x8C;
const uint8_t kHitachiAc344ButtonPowerMode = kHitachiAc424ButtonPowerMode;
const uint8_t kHitachiAc344ButtonFan = kHitachiAc424ButtonFan;
const uint8_t kHitachiAc344ButtonTempDown = kHitachiAc424ButtonTempDown;
const uint8_t kHitachiAc344ButtonTempUp = kHitachiAc424ButtonTempUp;
const uint8_t kHitachiAc344ButtonSwingV = kHitachiAc424ButtonSwingV;
const uint8_t kHitachiAc344ButtonSwingH = kHitachiAc424ButtonSwingH;

// Byte[13]
const uint8_t kHitachiAc424TempByte = 13;
const uint8_t kHitachiAc424TempOffset = 2;
const uint8_t kHitachiAc424TempSize = 6;
const uint8_t kHitachiAc424MinTemp = 16;   // 16C
const uint8_t kHitachiAc424MaxTemp = 32;   // 32C
const uint8_t kHitachiAc344MinTemp = kHitachiAc424MinTemp;
const uint8_t kHitachiAc344MaxTemp = kHitachiAc424MaxTemp;
const uint8_t kHitachiAc424FanTemp = 27;   // 27C

// Byte[25]
const uint8_t kHitachiAc424ModeByte = 25;
const uint8_t kHitachiAc424Fan = 1;
const uint8_t kHitachiAc424Cool = 3;
const uint8_t kHitachiAc424Dry = 5;
const uint8_t kHitachiAc424Heat = 6;
const uint8_t kHitachiAc344Fan = kHitachiAc424Fan;
const uint8_t kHitachiAc344Cool = kHitachiAc424Cool;
const uint8_t kHitachiAc344Dry = kHitachiAc424Dry;
const uint8_t kHitachiAc344Heat = kHitachiAc424Heat;

const uint8_t kHitachiAc424FanByte = kHitachiAc424ModeByte;
const uint8_t kHitachiAc424FanMin = 1;
const uint8_t kHitachiAc424FanLow = 2;
const uint8_t kHitachiAc424FanMedium = 3;
const uint8_t kHitachiAc424FanHigh = 4;
const uint8_t kHitachiAc424FanAuto = 5;
const uint8_t kHitachiAc424FanMax = 6;
const uint8_t kHitachiAc424FanMaxDry = 2;
const uint8_t kHitachiAc344FanMin = kHitachiAc424FanMin;
const uint8_t kHitachiAc344FanLow = kHitachiAc424FanLow;
const uint8_t kHitachiAc344FanMedium = kHitachiAc424FanMedium;
const uint8_t kHitachiAc344FanHigh = kHitachiAc424FanHigh;
const uint8_t kHitachiAc344FanAuto = kHitachiAc424FanAuto;
const uint8_t kHitachiAc344FanMax = kHitachiAc424FanMax;

// Byte[27]
const uint8_t kHitachiAc424PowerByte = 27;
const uint8_t kHitachiAc424PowerOn = 0xF1;
const uint8_t kHitachiAc424PowerOff = 0xE1;

// Byte[35]
const uint8_t kHitachiAc344SwingHByte = 35;
const uint8_t kHitachiAc344SwingHOffset = 0;  // Mask 0b00000xxx
const uint8_t kHitachiAc344SwingHSize = 3;    // Mask 0b00000xxx
const uint8_t kHitachiAc344SwingHAuto = 0;              // 0b000
const uint8_t kHitachiAc344SwingHRightMax = 1;          // 0b001
const uint8_t kHitachiAc344SwingHRight = 2;             // 0b010
const uint8_t kHitachiAc344SwingHMiddle = 3;            // 0b011
const uint8_t kHitachiAc344SwingHLeft = 4;              // 0b100
const uint8_t kHitachiAc344SwingHLeftMax = 5;           // 0b101

// Byte[37]
const uint8_t kHitachiAc344SwingVByte = 37;
const uint8_t kHitachiAc344SwingVOffset = 5;  // Mask 0b00x00000

// HitachiAc1
// Byte[3] (Model)
const uint8_t kHitachiAc1ModelByte = 3;
const uint8_t kHitachiAc1ModelOffset = 6;  // Mask 0b11000000
const uint8_t kHitachiAc1Model_A =                 0b10;
const uint8_t kHitachiAc1Model_B =                 0b01;
const uint8_t kHitachiAc1ModelSize = 2;

// Byte[5] (Mode & Fan)
const uint8_t kHitachiAc1ModeByte = 5;
const uint8_t kHitachiAc1ModeOffset = 4;
const uint8_t kHitachiAc1ModeSize = 4;  // Mask 0b11110000
const uint8_t kHitachiAc1Dry =                  0b0010;  // 2
const uint8_t kHitachiAc1Fan =                  0b0100;  // 4
const uint8_t kHitachiAc1Cool =                 0b0110;  // 6
const uint8_t kHitachiAc1Heat =                 0b1001;  // 9
const uint8_t kHitachiAc1Auto =                 0b1110;  // 14
const uint8_t kHitachiAc1FanByte = kHitachiAc1ModeByte;
const uint8_t kHitachiAc1FanOffset = 0;
const uint8_t kHitachiAc1FanSize = 4;  // Mask 0b0001111
const uint8_t kHitachiAc1FanAuto = 1;          // 0b0001
const uint8_t kHitachiAc1FanHigh = 2;          // 0b0010
const uint8_t kHitachiAc1FanMed = 4;           // 0b0100
const uint8_t kHitachiAc1FanLow = 8;           // 0b1000
// Byte[6] (Temperature)
// Note: Temp is stored in LSB order.
const uint8_t kHitachiAc1TempByte = 6;
const uint8_t kHitachiAc1TempOffset = 2;
const uint8_t kHitachiAc1TempSize = 5;  // Mask 0b01111100
const uint8_t kHitachiAc1TempDelta = 7;
const uint8_t kHitachiAc1TempAuto = 25;  // Celsius
// Note: Timers are nr. of minutes & stored in LSB order.
// Byte[7-8] (Off Timer)
const uint8_t kHitachiAc1TimerSize = 16;  // Mask 0b1111111111111111
const uint8_t kHitachiAc1OffTimerLowByte = 7;
const uint8_t kHitachiAc1OffTimerHighByte = 8;
// Byte[9-10] (On Timer)
const uint8_t kHitachiAc1OnTimerLowByte = 9;
const uint8_t kHitachiAc1OnTimerHighByte = 10;
// Byte[11] (Power/Swing/Sleep)
const uint8_t kHitachiAc1PowerByte = 11;
const uint8_t kHitachiAc1PowerOffset = 5;        // Mask 0b00100000
const uint8_t kHitachiAc1PowerToggleOffset = 4;  // Mask 0b00010000
const uint8_t kHitachiAc1SwingByte = kHitachiAc1PowerByte;
const uint8_t kHitachiAc1SwingHOffset = 7;        // Mask 0b10000000
const uint8_t kHitachiAc1SwingVOffset = 6;        // Mask 0b01000000
const uint8_t kHitachiAc1SwingToggleOffset = 0;  // Mask 0b00000001
const uint8_t kHitachiAc1SleepByte = kHitachiAc1PowerByte;
const uint8_t kHitachiAc1SleepOffset = 1;        // Mask 0b00001110
const uint8_t kHitachiAc1SleepSize = 3;          // Mask 0b00001110
const uint8_t kHitachiAc1SleepOff =                          0b000;
const uint8_t kHitachiAc1Sleep1 =                            0b001;
const uint8_t kHitachiAc1Sleep2 =                            0b010;
const uint8_t kHitachiAc1Sleep3 =                            0b011;
const uint8_t kHitachiAc1Sleep4 =                            0b100;
// Byte[12] (Checksum)
const uint8_t kHitachiAc1ChecksumStartByte = 5;


// Classes
/// Class for handling detailed Hitachi 224-bit A/C messages.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/HitachiHeatpumpIR.cpp
class IRHitachiAc {
 public:
  explicit IRHitachiAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset(void);
#if SEND_HITACHI_AC
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HITACHI_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHitachiAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAcStateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint8_t remote_state[kHitachiAcStateLength];  ///< The state in native code.
  void checksum(const uint16_t length = kHitachiAcStateLength);
  uint8_t _previoustemp;
};

/// Class for handling detailed Hitachi 104-bit A/C messages.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1056
class IRHitachiAc1 {
 public:
  explicit IRHitachiAc1(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);

  void stateReset(void);
#if SEND_HITACHI_AC1
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HITACHI_AC1
  void begin(void);
  void on(void);
  void off(void);
  void setModel(const hitachi_ac1_remote_model_t model);
  hitachi_ac1_remote_model_t getModel(void);
  void setPower(const bool on);
  bool getPower(void);
  void setPowerToggle(const bool on);
  bool getPowerToggle(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed, const bool force = false);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingToggle(const bool toggle);
  bool getSwingToggle(void);
  void setSwingV(const bool on);
  bool getSwingV(void);
  void setSwingH(const bool on);
  bool getSwingH(void);
  void setSleep(const uint8_t mode);
  uint8_t getSleep(void);
  void setOnTimer(const uint16_t mins);
  uint16_t getOnTimer(void);
  void setOffTimer(const uint16_t mins);
  uint16_t getOffTimer(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAc1StateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kHitachiAc1StateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kHitachiAc1StateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint8_t remote_state[kHitachiAc1StateLength];  ///< The state in native code.
  void checksum(const uint16_t length = kHitachiAc1StateLength);
};

/// Class for handling detailed Hitachi 53-byte/424-bit A/C messages.
class IRHitachiAc424 {
  friend class IRHitachiAc344;
 public:
  explicit IRHitachiAc424(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  virtual void stateReset(void);
#if SEND_HITACHI_AC424
  virtual void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HITACHI_AC424
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp, bool setPrevious = true);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  uint8_t getButton(void);
  void setButton(const uint8_t button);
  void setSwingVToggle(const bool on);
  bool getSwingVToggle(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  uint8_t* getRaw(void);
  virtual void setRaw(const uint8_t new_code[],
                      const uint16_t length = kHitachiAc424StateLength);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  virtual stdAc::state_t toCommon(void);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint8_t remote_state[kHitachiAc424StateLength];  ///< The state in native code
  void setInvertedStates(void);
  String _toString(void);
  uint8_t _previoustemp;
};

/// Class for handling detailed Hitachi 15to27-byte/120to216-bit A/C messages.
class IRHitachiAc3 {
 public:
  explicit IRHitachiAc3(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);

  void stateReset(void);
#if SEND_HITACHI_AC3
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_HITACHI_AC3
  void begin(void);
  uint8_t getMode(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAc3StateLength);
  static bool hasInvertedStates(const uint8_t state[], const uint16_t length);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  uint8_t remote_state[kHitachiAc3StateLength];  ///< The state in native code.
  void setInvertedStates(const uint16_t length = kHitachiAc3StateLength);
};

/// Class for handling detailed Hitachi 344-bit A/C messages.
class IRHitachiAc344: public IRHitachiAc424 {
 public:
  explicit IRHitachiAc344(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);
  void stateReset(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kHitachiAc344StateLength);
  stdAc::state_t toCommon(void);
#if SEND_HITACHI_AC344
  void send(const uint16_t repeat = kHitachiAcDefaultRepeat);
#endif  // SEND_HITACHI_AC344
  void setSwingV(const bool on);
  bool getSwingV(void);
  void setSwingH(const uint8_t position);
  uint8_t getSwingH(void);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
  String toString(void);
};
#endif  // IR_HITACHI_H_
