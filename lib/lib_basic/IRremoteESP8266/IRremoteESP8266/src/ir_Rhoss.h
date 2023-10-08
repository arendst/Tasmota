// Copyright 2021 Tom Rosenback

/// @file
/// @brief Support for Rhoss A/C protocol
// Supports:
//   Brand: Rhoss, Model: Idrowall MPCV 20-30-35-40

#ifndef IR_RHOSS_H_
#define IR_RHOSS_H_

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


/// Native representation of a Rhoss A/C message.
union RhossProtocol{
  uint8_t raw[kRhossStateLength];  // The state of the IR remote.
  struct {
    // Byte 0
    uint8_t       :8;  // Typically 0xAA
    // Byte 1
    uint8_t Temp  :4;
    uint8_t       :4;  // Typically 0x0
    // Byte 2
    uint8_t       :8;  // Typically 0x60
    // Byte 3
    uint8_t       :8;  // Typically 0x0
    // Byte 4
    uint8_t Fan   :2;
    uint8_t       :2;  // Typically 0x0
    uint8_t Mode  :4;
    // Byte 5
    uint8_t Swing :1;
    uint8_t       :5;  // Typically 0x0
    uint8_t Power :2;
    // Byte 6
    uint8_t       :8;  // Typically 0x54
    // Byte 7
    uint8_t       :8;  // Typically 0x0
    // Byte 8
    uint8_t       :8;  // Typically 0x0
    // Byte 9
    uint8_t       :8;  // Typically 0x0
    // Byte 10
    uint8_t       :8;  // Typically 0x0
    // Byte 11
    uint8_t Sum   :8;
  };
};

// Constants

// Fan Control
const uint8_t kRhossFanAuto =  0b00;
const uint8_t kRhossFanMin =   0b01;
const uint8_t kRhossFanMed =   0b10;
const uint8_t kRhossFanMax =   0b11;
// Modes
const uint8_t kRhossModeHeat = 0b0001;
const uint8_t kRhossModeCool = 0b0010;
const uint8_t kRhossModeDry =  0b0011;
const uint8_t kRhossModeFan =  0b0100;
const uint8_t kRhossModeAuto = 0b0101;

// Temperature
const uint8_t kRhossTempMin = 16;  // Celsius
const uint8_t kRhossTempMax = 30;  // Celsius

// Power
const uint8_t kRhossPowerOn =  0b10;  // 0x2
const uint8_t kRhossPowerOff = 0b01;  // 0x1

// Swing
const uint8_t kRhossSwingOn =   0b1;  // 0x1
const uint8_t kRhossSwingOff =  0b0;  // 0x0

const uint8_t kRhossDefaultFan = kRhossFanAuto;
const uint8_t kRhossDefaultMode = kRhossModeCool;
const uint8_t kRhossDefaultTemp = 21;  // Celsius
const bool kRhossDefaultPower = false;
const bool kRhossDefaultSwing = false;

// Classes

/// Class for handling detailed Rhoss A/C messages.
class IRRhossAc {
 public:
  explicit IRRhossAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);

  void stateReset();
#if SEND_RHOSS
  void send(const uint16_t repeat = kRhossDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_RHOSS
  void begin();
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kRhossStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kRhossStateLength);
  void setPower(const bool state);
  bool getPower(void) const;
  void on(void);
  void off(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setSwing(const bool state);
  uint8_t getSwing(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  /// @cond IGNORE
  IRsendTest _irsend;
  /// @endcond
#endif
  RhossProtocol _;
  void checksum(void);
};
#endif  // IR_RHOSS_H_
