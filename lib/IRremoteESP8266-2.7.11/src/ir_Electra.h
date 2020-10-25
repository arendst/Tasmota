// Copyright 2019 David Conran
/// @file
/// @brief Support for Electra A/C protocols.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/AUXHeatpumpIR.cpp

// Supports:
//   Brand: AUX,  Model: KFR-35GW/BpNFW=3 A/C
//   Brand: AUX,  Model: YKR-T/011 remote
//   Brand: Electra,  Model: Classic INV 17 / AXW12DCS A/C
//   Brand: Electra,  Model: YKR-M/003E remote

#ifndef IR_ELECTRA_H_
#define IR_ELECTRA_H_

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
// state[1]
//                             Temp  0b11111000
const uint8_t kElectraAcTempOffset = 3;
const uint8_t kElectraAcTempSize = 5;   // Bits
const uint8_t kElectraAcMinTemp = 16;   // 16C
const uint8_t kElectraAcMaxTemp = 32;   // 32C
const uint8_t kElectraAcTempDelta = 8;
const uint8_t kElectraAcSwingSize = 3;  // Bits
const uint8_t kElectraAcSwingOn =  0b000;
const uint8_t kElectraAcSwingOff = 0b111;
//                      SwingVMask = 0b00000111;
const uint8_t kElectraAcSwingVOffset = 0;
// state[2]
//                      SwingHMask = 0b11100000;
const uint8_t kElectraAcSwingHOffset = 5;
// state[4]
//                      FanMask =    0b11100000;
const uint8_t kElectraAcFanOffset = 5;
const uint8_t kElectraAcFanSize = 3;  // Bits

const uint8_t kElectraAcFanAuto =    0b101;
const uint8_t kElectraAcFanLow =     0b011;
const uint8_t kElectraAcFanMed =     0b010;
const uint8_t kElectraAcFanHigh =    0b001;
// state[5]
//                      TurboMask =    0b01000000;
const uint8_t kElectraAcTurboOffset = 6;

// state[6]
//                            Mode 0b11100000
const uint8_t kElectraAcModeOffset = 5;
const uint8_t kElectraAcAuto =     0b000;
const uint8_t kElectraAcCool =     0b001;
const uint8_t kElectraAcDry =      0b010;
const uint8_t kElectraAcHeat =     0b100;
const uint8_t kElectraAcFan =      0b110;
// state[9]
//
const uint8_t kElectraAcCleanOffset = 2;  // Bit 0b00000100
const uint8_t kElectraAcPowerOffset = 5;  // Bit 0b00100000
// state[11]
//
const uint8_t kElectraAcLightToggleOn  = 0x15;
// Light has known ON values of 0x15 (0b00010101) or 0x19 (0b00011001)
//   Thus common bits ON are: 0b00010001 (0x11)
// We will use this for the getLightToggle() test.
const uint8_t kElectraAcLightToggleMask = 0x11;
// and known OFF values of 0x08 (0b00001000) & 0x05 (0x00000101)
const uint8_t kElectraAcLightToggleOff = 0x08;


// Classes
/// Class for handling detailed Electra A/C messages.
class IRElectraAc {
 public:
  explicit IRElectraAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset(void);
#if SEND_ELECTRA_AC
  void send(const uint16_t repeat = kElectraAcMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_ELECTRA_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setSwingV(const bool on);
  bool getSwingV(void);
  void setSwingH(const bool on);
  bool getSwingH(void);
  void setClean(const bool on);
  bool getClean(void);
  void setLightToggle(const bool on);
  bool getLightToggle(void);
  void setTurbo(const bool on);
  bool getTurbo(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kElectraAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kElectraAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kElectraAcStateLength);
  String toString(void);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  uint8_t remote_state[kElectraAcStateLength];   ///< The state of the IR remote
  void checksum(const uint16_t length = kElectraAcStateLength);
};
#endif  // IR_ELECTRA_H_
