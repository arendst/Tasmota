// Copyright 2018 David Conran
/// @file
/// @brief Support for Coolix A/C protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/484
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1318
/// @note Kudos:
///   Hamper: For the breakdown and mapping of the bit values.
///   fraschizzato: For additional ZoneFollow & SwingVStep analysis.
/// @note Timers seem to use the `COOLIX48` protocol.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1694

// Supports:
//   Brand: Beko, Model: RG57K7(B)/BGEF Remote
//   Brand: Beko, Model: BINR 070/071 split-type A/C
//   Brand: Midea, Model: RG52D/BGE Remote
//   Brand: Midea, Model: MS12FU-10HRDN1-QRD0GW(B) A/C
//   Brand: Midea, Model: MSABAU-07HRFN1-QRD0GW A/C (circa 2016)
//   Brand: Tokio, Model: AATOEMF17-12CHR1SW split-type RG51|50/BGE Remote
//   Brand: Airwell, Model: RC08B remote
//   Brand: Kastron, Model: RG57A7/BGEF Inverter remote
//   Brand: Kaysun, Model: Casual CF A/C
//   Brand: Toshiba, Model: RAS-M10YKV-E A/C
//   Brand: Toshiba, Model: RAS-M13YKV-E A/C
//   Brand: Toshiba, Model: RAS-4M27YAV-E A/C
//   Brand: Toshiba, Model: WH-E1YE remote
//   Brand: Bosch, Model: RG36B4/BGE remote
//   Brand: Bosch, Model: B1ZAI2441W/B1ZAO2441W A/C

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

// Constants
// Modes
const uint8_t kCoolixCool = 0b000;
const uint8_t kCoolixDry = 0b001;
const uint8_t kCoolixAuto = 0b010;
const uint8_t kCoolixHeat = 0b011;
const uint8_t kCoolixFan = 0b100;                                 // Synthetic.
// const uint32_t kCoolixModeMask = 0b000000000000000000001100;  // 0xC
// const uint32_t kCoolixZoneFollowMask = 0b000010000000000000000010  0x80002
// Fan Control
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
const uint8_t kCoolixSensorTempMax = 30;  // Celsius
const uint8_t kCoolixSensorTempIgnoreCode = 0b11111;  // 0x1F / 31 (DEC)
// kCoolixSensorTempMask = 0b000000000000111100000000;  // 0xF00
// Fixed states/messages.
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

/// Native representation of a Coolix A/C message.
union CoolixProtocol {
  uint32_t raw;  ///< The state in IR code form.
  struct {  // Only 24 bits are used.
    // Byte
    uint32_t            :1;  // Unknown
    uint32_t ZoneFollow1:1;  ///< Control bit for Zone Follow mode.
    uint32_t Mode       :2;  ///< Operation mode.
    uint32_t Temp       :4;  ///< Desired temperature (Celsius)
    // Byte
    uint32_t SensorTemp :5;  ///< The temperature sensor in the IR remote.
    uint32_t Fan        :3;  ///< Fan speed
    // Byte
    uint32_t            :3;  // Unknown
    uint32_t ZoneFollow2:1;  ///< Additional control bit for Zone Follow mode.
    uint32_t            :4;  ///< Fixed value 0b1011 / 0xB.
  };
};

// Classes

/// Class for handling detailed Coolix A/C messages.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/484
class IRCoolixAC {
 public:
  explicit IRCoolixAC(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset(void);
#if SEND_COOLIX
  void send(const uint16_t repeat = kCoolixDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_COOLIX
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setSensorTemp(const uint8_t temp);
  uint8_t getSensorTemp(void) const;
  void clearSensorTemp(void);
  void setFan(const uint8_t speed, const bool modecheck = true);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(void);
  bool getSwing(void) const;
  void setSwingVStep(void);
  bool getSwingVStep(void) const;
  void setSleep(void);
  bool getSleep(void) const;
  void setTurbo(void);
  bool getTurbo(void) const;
  void setLed(void);
  bool getLed(void) const;
  void setClean(void);
  bool getClean(void) const;
  bool getZoneFollow(void) const;
  uint32_t getRaw(void) const;
  void setRaw(const uint32_t new_code);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString(void) const;
  void setZoneFollow(const bool on);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  CoolixProtocol _;  ///< The state of the IR remote in IR code form.
  CoolixProtocol _saved;   ///< Copy of the state if we required a special mode.

  // Internal State settings
  bool powerFlag;
  bool turboFlag;
  bool ledFlag;
  bool cleanFlag;
  bool sleepFlag;
  bool swingFlag;
  uint8_t savedFan;

  void setTempRaw(const uint8_t code);
  uint8_t getTempRaw(void) const;
  void setSensorTempRaw(const uint8_t code);
  bool isSpecialState(void) const;
  bool handleSpecialState(const uint32_t data);
  void updateAndSaveState(const uint32_t raw_state);
  void recoverSavedState(void);
  uint32_t getNormalState(void);
};

#endif  // IR_COOLIX_H_
