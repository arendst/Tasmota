// Copyright 2019 David Conran
/// @file
/// @brief Support for Electra A/C protocols.
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/AUXHeatpumpIR.cpp

// Supports:
//   Brand: AUX,  Model: KFR-35GW/BpNFW=3 A/C
//   Brand: AUX,  Model: YKR-T/011 remote
//   Brand: Electra,  Model: Classic INV 17 / AXW12DCS A/C
//   Brand: Electra,  Model: YKR-M/003E remote
//   Brand: Frigidaire,  Model: FGPC102AB1 A/C

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

/// Native representation of a Electra A/C message.
union ElectraProtocol {
  uint8_t raw[kElectraAcStateLength];   ///< The state of the IR remote
  struct {
    // Byte 0
    uint8_t         :8;
    // Byte 1
    uint8_t SwingV  :3;
    uint8_t Temp    :5;
    // Byte 2
    uint8_t         :5;
    uint8_t SwingH  :3;
    // Byte 3
    uint8_t         :8;
    // Byte 4
    uint8_t         :5;
    uint8_t Fan     :3;
    // Byte 5
    uint8_t         :6;
    uint8_t Turbo   :1;
    uint8_t         :1;
    // Byte 6
    uint8_t         :5;
    uint8_t Mode    :3;
    // Byte 7
    uint8_t         :8;
    // Byte 8
    uint8_t         :8;
    // Byte 9
    uint8_t         :2;
    uint8_t Clean   :1;
    uint8_t         :2;
    uint8_t Power   :1;
    uint8_t         :2;
    // Byte 10
    uint8_t         :8;
    // Byte 11
    uint8_t LightToggle :8;
    // Byte 12
    uint8_t Sum     :8;
  };
};

// Constants
const uint8_t kElectraAcMinTemp = 16;   // 16C
const uint8_t kElectraAcMaxTemp = 32;   // 32C
const uint8_t kElectraAcTempDelta = 8;
const uint8_t kElectraAcSwingOn =  0b000;
const uint8_t kElectraAcSwingOff = 0b111;

const uint8_t kElectraAcFanAuto =    0b101;
const uint8_t kElectraAcFanLow =     0b011;
const uint8_t kElectraAcFanMed =     0b010;
const uint8_t kElectraAcFanHigh =    0b001;

const uint8_t kElectraAcAuto =     0b000;
const uint8_t kElectraAcCool =     0b001;
const uint8_t kElectraAcDry =      0b010;
const uint8_t kElectraAcHeat =     0b100;
const uint8_t kElectraAcFan =      0b110;

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
  bool getPower(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setSwingV(const bool on);
  bool getSwingV(void) const;
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  void setClean(const bool on);
  bool getClean(void) const;
  void setLightToggle(const bool on);
  bool getLightToggle(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kElectraAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kElectraAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kElectraAcStateLength);
  String toString(void) const;
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  ElectraProtocol _;
  void checksum(const uint16_t length = kElectraAcStateLength);
};
#endif  // IR_ELECTRA_H_
