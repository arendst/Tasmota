// Coolix A/C
//
// Copyright 2018 David Conran

// Supports:
//   Brand: Beko, Model: RG57K7(B)/BGEF Remote
//   Brand: Beko, Model: BINR 070/071 split-type A/C
//   Brand: Midea, Model: RG52D/BGE Remote
//   Brand: Midea, Model: MS12FU-10HRDN1-QRD0GW(B) A/C
//   Brand: Midea, Model: MSABAU-07HRFN1-QRD0GW A/C (circa 2016)
//   Brand: Tokio, Model: AATOEMF17-12CHR1SW split-type RG51|50/BGE Remote
//   Brand: Airwell, Model: RC08B remote

#ifndef IR_COOLIX_H_
#define IR_COOLIX_H_

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
//   https://github.com/crankyoldgit/IRremoteESP8266/issues/484
// Kudos:
//   Hamper: For the breakdown and mapping of the bit values.

// Constants
// Modes
const uint8_t kCoolixCool = 0b000;
const uint8_t kCoolixDry = 0b001;
const uint8_t kCoolixAuto = 0b010;
const uint8_t kCoolixHeat = 0b011;
const uint8_t kCoolixFan = 0b100;                                 // Synthetic.
// const uint32_t kCoolixModeMask = 0b000000000000000000001100;  // 0xC
const uint8_t kCoolixModeOffset = 2;
const uint8_t kCoolixModeSize = 2;
// const uint32_t kCoolixZoneFollowMask = 0b000010000000000000000000  0x80000
const uint8_t kCoolixZoneFollowMaskOffset = 19;
// Fan Control
// const uint32_t kCoolixFanMask = 0b000000001110000000000000;  // 0x00E000
const uint8_t kCoolixFanOffset = 13;
const uint8_t kCoolixFanSize = 3;
const uint8_t kCoolixFanMin = 0b100;
const uint8_t kCoolixFanMed = 0b010;
const uint8_t kCoolixFanMax = 0b001;
const uint8_t kCoolixFanAuto = 0b101;
const uint8_t kCoolixFanAuto0 = 0b000;
const uint8_t kCoolixFanZoneFollow = 0b110;
const uint8_t kCoolixFanFixed = 0b111;
// Temperature
const uint8_t kCoolixTempMin = 17;  // Celsius
const uint8_t kCoolixTempMax = 30;  // Celsius
const uint8_t kCoolixTempRange = kCoolixTempMax - kCoolixTempMin + 1;
const uint8_t kCoolixFanTempCode = 0b1110;  // Part of Fan Mode.
// const uint32_t kCoolixTempMask = 0b11110000;
const uint8_t kCoolixTempOffset = 4;
const uint8_t kCoolixTempSize = 4;
const uint8_t kCoolixTempMap[kCoolixTempRange] = {
    0b0000,  // 17C
    0b0001,  // 18c
    0b0011,  // 19C
    0b0010,  // 20C
    0b0110,  // 21C
    0b0111,  // 22C
    0b0101,  // 23C
    0b0100,  // 24C
    0b1100,  // 25C
    0b1101,  // 26C
    0b1001,  // 27C
    0b1000,  // 28C
    0b1010,  // 29C
    0b1011   // 30C
};
const uint8_t kCoolixSensorTempMin = 16;  // Celsius
const uint8_t kCoolixSensorTempMax = 30;  // Celsius
const uint8_t kCoolixSensorTempIgnoreCode = 0b1111;
// kCoolixSensorTempMask = 0b000000000000111100000000;  // 0xF00
const uint8_t kCoolixSensorTempOffset = 8;
const uint8_t kCoolixSensorTempSize = 4;
// Fixed states/messages.
const uint8_t kCoolixPrefix = 0b1011;  // 0xB
const uint8_t kCoolixUnknown = 0xFF;
const uint32_t kCoolixOff    = 0b101100100111101111100000;  // 0xB27BE0
const uint32_t kCoolixSwing  = 0b101100100110101111100000;  // 0xB26BE0
const uint32_t kCoolixSwingH = 0b101100101111010110100010;  // 0xB5F5A2
const uint32_t kCoolixSwingV = 0b101100100000111111100000;  // 0xB20FE0
const uint32_t kCoolixSleep  = 0b101100101110000000000011;  // 0xB2E003
const uint32_t kCoolixTurbo  = 0b101101011111010110100010;  // 0xB5F5A2
const uint32_t kCoolixLed    = 0b101101011111010110100101;  // 0xB5F5A5
const uint32_t kCoolixClean  = 0b101101011111010110101010;  // 0xB5F5AA
const uint32_t kCoolixCmdFan = 0b101100101011111111100100;  // 0xB2BFE4
// On, 25C, Mode: Auto, Fan: Auto, Zone Follow: Off, Sensor Temp: Ignore.
const uint32_t kCoolixDefaultState = 0b101100100001111111001000;  // 0xB21FC8

// Classes

/// Class for handling detailed Coolix A/C messages.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/484
class IRCoolixAC {
 public:
  explicit IRCoolixAC(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset();
#if SEND_COOLIX
  void send(const uint16_t repeat = kCoolixDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_COOLIX
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setSensorTemp(const uint8_t desired);
  uint8_t getSensorTemp();
  void clearSensorTemp();
  void setFan(const uint8_t speed, const bool modecheck = true);
  uint8_t getFan();
  void setMode(const uint8_t mode);
  uint8_t getMode();
  void setSwing();
  bool getSwing();
  void setSleep();
  bool getSleep();
  void setTurbo();
  bool getTurbo();
  void setLed();
  bool getLed();
  void setClean();
  bool getClean();
  bool getZoneFollow();
  uint32_t getRaw();
  void setRaw(const uint32_t new_code);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  // internal state
  bool    powerFlag;
  bool    turboFlag;
  bool    ledFlag;
  bool    cleanFlag;
  bool    sleepFlag;
  bool    zoneFollowFlag;
  bool    swingFlag;
  bool    swingHFlag;
  bool    swingVFlag;

  uint32_t remote_state;  ///< The state of the IR remote in IR code form.
  uint32_t saved_state;   ///< Copy of the state if we required a special mode.
  void setTempRaw(const uint8_t code);
  uint8_t getTempRaw();
  void setSensorTempRaw(const uint8_t code);
  void setZoneFollow(const bool on);
  bool isSpecialState(void);
  bool handleSpecialState(const uint32_t data);
  void updateSavedState(void);
  void recoverSavedState(void);
  uint32_t getNormalState(void);
};

#endif  // IR_COOLIX_H_
