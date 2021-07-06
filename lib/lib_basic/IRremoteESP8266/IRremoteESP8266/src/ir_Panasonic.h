// Copyright 2018 David Conran

/// @file
/// @brief Support for Panasonic protocols.
/// @see Panasonic A/C support heavily influenced by https://github.com/ToniA/ESPEasy/blob/HeatpumpIR/lib/HeatpumpIR/PanasonicHeatpumpIR.cpp

// Supports:
//   Brand: Panasonic,  Model: TV (PANASONIC)
//   Brand: Panasonic,  Model: NKE series A/C (PANASONIC_AC NKE/2)
//   Brand: Panasonic,  Model: DKE series A/C (PANASONIC_AC DKE/3)
//   Brand: Panasonic,  Model: DKW series A/C (PANASONIC_AC DKE/3)
//   Brand: Panasonic,  Model: PKR series A/C (PANASONIC_AC DKE/3)
//   Brand: Panasonic,  Model: JKE series A/C (PANASONIC_AC JKE/4)
//   Brand: Panasonic,  Model: CKP series A/C (PANASONIC_AC CKP/5)
//   Brand: Panasonic,  Model: RKR series A/C (PANASONIC_AC RKR/6)
//   Brand: Panasonic,  Model: CS-ME10CKPG A/C (PANASONIC_AC CKP/5)
//   Brand: Panasonic,  Model: CS-ME12CKPG A/C (PANASONIC_AC CKP/5)
//   Brand: Panasonic,  Model: CS-ME14CKPG A/C (PANASONIC_AC CKP/5)
//   Brand: Panasonic,  Model: CS-E7PKR A/C (PANASONIC_AC DKE/2)
//   Brand: Panasonic,  Model: CS-Z9RKR A/C (PANASONIC_AC RKR/6)
//   Brand: Panasonic,  Model: CS-YW9MKD A/C (PANASONIC_AC JKE/4)
//   Brand: Panasonic,  Model: A75C2311 remote (PANASONIC_AC CKP/5)
//   Brand: Panasonic,  Model: A75C2616-1 remote (PANASONIC_AC DKE/3)
//   Brand: Panasonic,  Model: A75C3704 remote (PANASONIC_AC DKE/3)
//   Brand: Panasonic,  Model: A75C3747 remote (PANASONIC_AC JKE/4)
//   Brand: Panasonic,  Model: CS-E9CKP series A/C (PANASONIC_AC32)
//   Brand: Panasonic,  Model: A75C2295 remote (PANASONIC_AC32)

#ifndef IR_PANASONIC_H_
#define IR_PANASONIC_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

// Constants
const uint16_t kPanasonicFreq = 36700;
const uint16_t kPanasonicAcExcess = 0;
// Much higher than usual. See issue #540.
const uint16_t kPanasonicAcTolerance = 40;

const uint8_t kPanasonicAcAuto = 0;  // 0b000
const uint8_t kPanasonicAcDry = 2;   // 0b010
const uint8_t kPanasonicAcCool = 3;  // 0b011
const uint8_t kPanasonicAcHeat = 4;  // 0b010
const uint8_t kPanasonicAcFan = 6;   // 0b110
const uint8_t kPanasonicAcFanMin = 0;
const uint8_t kPanasonicAcFanMed = 2;
const uint8_t kPanasonicAcFanMax = 4;
const uint8_t kPanasonicAcFanAuto = 7;
const uint8_t kPanasonicAcFanDelta = 3;
const uint8_t kPanasonicAcPowerOffset = 0;
const uint8_t kPanasonicAcTempOffset = 1;  // Bits
const uint8_t kPanasonicAcTempSize = 5;  // Bits
const uint8_t kPanasonicAcMinTemp = 16;      // Celsius
const uint8_t kPanasonicAcMaxTemp = 30;      // Celsius
const uint8_t kPanasonicAcFanModeTemp = 27;  // Celsius
const uint8_t kPanasonicAcQuietOffset = 0;
const uint8_t kPanasonicAcPowerfulOffset = 5;   // 0b100000
// CKP & RKR models have Powerful and Quiet bits swapped.
const uint8_t kPanasonicAcQuietCkpOffset = kPanasonicAcPowerfulOffset;
const uint8_t kPanasonicAcPowerfulCkpOffset = kPanasonicAcQuietOffset;
const uint8_t kPanasonicAcSwingVHighest = 0x1;  // 0b0001
const uint8_t kPanasonicAcSwingVHigh = 0x2;     // 0b0010
const uint8_t kPanasonicAcSwingVMiddle = 0x3;   // 0b0011
const uint8_t kPanasonicAcSwingVLow = 0x4;      // 0b0100
const uint8_t kPanasonicAcSwingVLowest = 0x5;   // 0b0101
const uint8_t kPanasonicAcSwingVAuto = 0xF;     // 0b1111

const uint8_t kPanasonicAcSwingHMiddle = 0x6;     // 0b0110
const uint8_t kPanasonicAcSwingHFullLeft = 0x9;   // 0b1001
const uint8_t kPanasonicAcSwingHLeft = 0xA;       // 0b1010
const uint8_t kPanasonicAcSwingHRight = 0xB;      // 0b1011
const uint8_t kPanasonicAcSwingHFullRight = 0xC;  // 0b1100
const uint8_t kPanasonicAcSwingHAuto = 0xD;       // 0b1101
const uint8_t kPanasonicAcChecksumInit = 0xF4;
const uint8_t kPanasonicAcOnTimerOffset = 1;
const uint8_t kPanasonicAcOffTimerOffset = 2;
const uint8_t kPanasonicAcTimeSize = 11;  // Bits
const uint8_t kPanasonicAcTimeOverflowSize = 3;  // Bits
const uint16_t kPanasonicAcTimeMax = 23 * 60 + 59;  // Mins since midnight.
const uint16_t kPanasonicAcTimeSpecial = 0x600;

const uint8_t kPanasonicAcIonFilterByte = 22;  // Byte
const uint8_t kPanasonicAcIonFilterOffset = 0;  // Bit

const uint8_t kPanasonicKnownGoodState[kPanasonicAcStateLength] = {
    0x02, 0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x06, 0x02,
    0x20, 0xE0, 0x04, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
    0x00, 0x0E, 0xE0, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00};

/// Class for handling detailed Panasonic A/C messages.
class IRPanasonicAc {
 public:
  explicit IRPanasonicAc(const uint16_t pin, const bool inverted = false,
                         const bool use_modulation = true);
  void stateReset(void);
#if SEND_PANASONIC_AC
  void send(const uint16_t repeat = kPanasonicAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_PANASONIC_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp, const bool remember = true);
  uint8_t getTemp(void);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setRaw(const uint8_t state[]);
  uint8_t *getRaw(void);
  static bool validChecksum(const uint8_t *state,
                            const uint16_t length = kPanasonicAcStateLength);
  static uint8_t calcChecksum(const uint8_t *state,
                              const uint16_t length = kPanasonicAcStateLength);
  void setQuiet(const bool on);
  bool getQuiet(void);
  void setPowerful(const bool on);
  bool getPowerful(void);
  void setIon(const bool on);
  bool getIon(void);
  void setModel(const panasonic_ac_remote_model_t model);
  panasonic_ac_remote_model_t getModel(void);
  void setSwingVertical(const uint8_t elevation);
  uint8_t getSwingVertical(void);
  void setSwingHorizontal(const uint8_t direction);
  uint8_t getSwingHorizontal(void);
  static uint16_t encodeTime(const uint8_t hours, const uint8_t mins);
  uint16_t getClock(void);
  void setClock(const uint16_t mins_since_midnight);
  uint16_t getOnTimer(void);
  void setOnTimer(const uint16_t mins_since_midnight, const bool enable = true);
  void cancelOnTimer(void);
  bool isOnTimerEnabled(void);
  uint16_t getOffTimer(void);
  void setOffTimer(const uint16_t mins_since_midnight,
                   const bool enable = true);
  void cancelOffTimer(void);
  bool isOffTimerEnabled(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static uint8_t convertSwingH(const stdAc::swingh_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  static stdAc::swingh_t toCommonSwingH(const uint8_t pos);
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
  uint8_t remote_state[kPanasonicAcStateLength];  ///< The state in code form.
  uint8_t _swingh;
  uint8_t _temp;
  void fixChecksum(const uint16_t length = kPanasonicAcStateLength);
  static uint16_t _getTime(const uint8_t ptr[]);
  static void _setTime(uint8_t * const ptr, const uint16_t mins_since_midnight,
                       const bool round_down);
};

/// Native representation of a Panasonic 32-bit A/C message.
union PanasonicAc32Protocol {
  uint32_t raw;  ///< The state in IR code form.
  struct {
    // Byte 0
    uint8_t             :3;
    uint8_t SwingH      :1;
    uint8_t SwingV      :3;
    uint8_t             :1;  ///< Always appears to be set. (1)
    // Byte 1
    uint8_t             :8;  // Always seems to be 0x36.
    // Byte 2
    uint8_t Temp        :4;
    uint8_t Fan         :4;
    // Byte 3
    uint8_t Mode        :3;
    uint8_t PowerToggle :1;  // 0 means toggle, 1 = keep the same.
    uint8_t             :4;
  };
};

const uint8_t kPanasonicAc32Fan =  1;  // 0b001
const uint8_t kPanasonicAc32Cool = 2;  // 0b010
const uint8_t kPanasonicAc32Dry =  3;  // 0b011
const uint8_t kPanasonicAc32Heat = 4;  // 0b010
const uint8_t kPanasonicAc32Auto = 6;  // 0b110

const uint8_t kPanasonicAc32FanMin =  2;
const uint8_t kPanasonicAc32FanLow =  3;
const uint8_t kPanasonicAc32FanMed =  4;
const uint8_t kPanasonicAc32FanHigh = 5;
const uint8_t kPanasonicAc32FanMax =  6;
const uint8_t kPanasonicAc32FanAuto = 0xF;
const uint8_t kPanasonicAc32SwingVAuto = 0x7;   // 0b111
const uint32_t kPanasonicAc32KnownGood = 0x0AF136FC;  ///< Cool, Auto, 16C

/// Class for handling detailed Panasonic 32bit A/C messages.
class IRPanasonicAc32 {
 public:
  explicit IRPanasonicAc32(const uint16_t pin, const bool inverted = false,
                           const bool use_modulation = true);
  void stateReset(void);
#if SEND_PANASONIC_AC32
  void send(const uint16_t repeat = kPanasonicAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_PANASONIC_AC32
  void begin(void);
  void setPowerToggle(const bool on);
  bool getPowerToggle(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setRaw(const uint32_t state);
  uint32_t getRaw(void) const;
  void setSwingVertical(const uint8_t pos);
  uint8_t getSwingVertical(void) const;
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void) const;
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  PanasonicAc32Protocol _;  ///< The state in code form.
};

#endif  // IR_PANASONIC_H_
