// Copyright 2019 crankyoldgit

/// @file
/// @brief Support for Sharp protocols.
/// @see http://www.sbprojects.com/knowledge/ir/sharp.htm
/// @see http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
/// @see http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
/// @see http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
/// @see GlobalCache's IR Control Tower data.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp

// Supports:
//   Brand: Sharp,  Model: LC-52D62U TV
//   Brand: Sharp,  Model: AY-ZP40KR A/C
//   Brand: Sharp,  Model: AH-AxSAY A/C
//   Brand: Sharp,  Model: CRMC-A907 JBEZ remote
//   Brand: Sharp,  Model: AH-XP10NRY A/C
//   Brand: Sharp,  Model: CRMC-820JBEZ remote

#ifndef IR_SHARP_H_
#define IR_SHARP_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif
#include "IRutils.h"

// Constants
const uint16_t kSharpAcHdrMark = 3800;
const uint16_t kSharpAcHdrSpace = 1900;
const uint16_t kSharpAcBitMark = 470;
const uint16_t kSharpAcZeroSpace = 500;
const uint16_t kSharpAcOneSpace = 1400;
const uint32_t kSharpAcGap = kDefaultMessageGap;

// Byte[4]
const uint8_t kSharpAcByteTemp = 4;
const uint8_t kSharpAcMinTemp = 15;  // Celsius
const uint8_t kSharpAcMaxTemp = 30;  // Celsius
// Byte[5]
const uint8_t kSharpAcBytePowerSpecial = 5;
const uint8_t kSharpAcPowerSetSpecialOffset = kHighNibble;  // 0bxxxx0000
const uint8_t kSharpAcPowerSpecialSize = kNibbleSize;       // 0bxxxx0000
const uint8_t kSharpAcPowerUnknown = 0;                     // 0b0000
const uint8_t kSharpAcPowerOnFromOff = 1;                   // 0b0001
const uint8_t kSharpAcPowerOff = 2;                         // 0b0010
const uint8_t kSharpAcPowerOn = 3;                          // 0b0011 (Normal)
const uint8_t kSharpAcPowerSetSpecialOn = 6;                // 0b0110
const uint8_t kSharpAcPowerSetSpecialOff = 7;               // 0b0111
const uint8_t kSharpAcPowerTimerSetting = 8;                // 0b1000
// Byte[6]
const uint8_t kSharpAcByteMode = 6;
const uint8_t kSharpAcModeSize = 2;        // Mask 0b000000xx;
const uint8_t kSharpAcAuto =                             0b00;
const uint8_t kSharpAcDry =                              0b11;
const uint8_t kSharpAcCool =                             0b10;
const uint8_t kSharpAcHeat =                             0b01;
const uint8_t kSharpAcByteClean = kSharpAcByteMode;
const uint8_t kSharpAcBitCleanOffset = 3;  // Mask 0b0000x000
const uint8_t kSharpAcByteFan = kSharpAcByteMode;
const uint8_t kSharpAcFanOffset = 4;       // Mask 0b0xxx0000
const uint8_t kSharpAcFanSize = 3;  // Nr. of Bits
const uint8_t kSharpAcFanAuto =                     0b010;  // 2
const uint8_t kSharpAcFanMin =                      0b100;  // 4 (FAN1)
const uint8_t kSharpAcFanMed =                      0b011;  // 3 (FAN2)
const uint8_t kSharpAcFanHigh =                     0b101;  // 5 (FAN3)
const uint8_t kSharpAcFanMax =                      0b111;  // 7 (FAN4)
// Byte[7]
const uint8_t kSharpAcByteTimer = 7;
const uint8_t kSharpAcTimerIncrement = 30;  // Mins
const uint8_t kSharpAcTimerHoursOffset = kLowNibble;
const uint8_t kSharpAcTimerHoursSize = kNibbleSize;   // Mask 0b0000xxxx
const uint8_t kSharpAcTimerHoursOff =                             0b0000;
const uint8_t kSharpAcTimerHoursMax =                             0b1100;  // 12
const uint8_t kSharpAcBitTimerType = 6;               // Mask 0b0x000000
const uint8_t kSharpAcOffTimerType =                           0b0;
const uint8_t kSharpAcOnTimerType =                            0b1;
const uint8_t kSharpAcBitTimerEnabled = 7;            // Mask 0bx0000000
// Byte[8]
const uint8_t kSharpAcByteSwing = 8;
const uint8_t kSharpAcSwingOffset = 0;
const uint8_t kSharpAcSwingSize = 3;  // Mask 0b00000xxx
const uint8_t kSharpAcSwingToggle =                0b111;
const uint8_t kSharpAcSwingNoToggle =              0b000;
// Byte[10]
const uint8_t kSharpAcByteSpecial = 10;  // Mask 0bxxxxxxxx
const uint8_t kSharpAcSpecialPower =              0x00;
const uint8_t kSharpAcSpecialTurbo =              0x01;
const uint8_t kSharpAcSpecialTempEcono =          0x04;
const uint8_t kSharpAcSpecialFan =                0x05;
const uint8_t kSharpAcSpecialSwing =              0x06;
const uint8_t kSharpAcSpecialTimer =              0xC0;
const uint8_t kSharpAcSpecialTimerHalfHour =      0xDE;
// Byte[11]
const uint8_t kSharpAcByteIon = 11;
const uint8_t kSharpAcBitIonOffset = 2;  // Mask 0b00000x00
// Byte[12] (Checksum)

// Classes
/// Class for handling detailed Sharp A/C messages.
class IRSharpAc {
 public:
  explicit IRSharpAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
#if SEND_SHARP_AC
  void send(const uint16_t repeat = kSharpAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_SHARP_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on, const bool prev_on = true);
  bool getPower(void);
  bool isPowerSpecial(void);
  void setTemp(const uint8_t temp, const bool save = true);
  uint8_t getTemp(void);
  void setFan(const uint8_t fan, const bool save = true);
  uint8_t getFan(void);
  void setMode(const uint8_t mode, const bool save = true);
  uint8_t getMode(void);
  void setSpecial(const uint8_t mode);
  uint8_t getSpecial(void);
  bool getTurbo(void);
  void setTurbo(const bool on);
  bool getSwingToggle(void);
  void setSwingToggle(const bool on);
  bool getIon(void);
  void setIon(const bool on);
  bool getEconoToggle(void);
  void setEconoToggle(const bool on);
  uint16_t getTimerTime(void);
  bool getTimerEnabled(void);
  bool getTimerType(void);
  void setTimer(bool enable, bool timer_type, uint16_t mins);
  bool getClean(void);
  void setClean(const bool on);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kSharpAcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kSharpAcStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
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
  uint8_t remote[kSharpAcStateLength];  ///< State of the remote in IR code form
  uint8_t _temp;  ///< Saved copy of the desired temp.
  uint8_t _mode;  ///< Saved copy of the desired mode.
  uint8_t _fan;  ///< Saved copy of the desired fan speed.
  void stateReset(void);
  void checksum(void);
  static uint8_t calcChecksum(uint8_t state[],
                              const uint16_t length = kSharpAcStateLength);
  void setPowerSpecial(const uint8_t value);
  uint8_t getPowerSpecial(void);
  void clearPowerSpecial(void);
};

#endif  // IR_SHARP_H_
