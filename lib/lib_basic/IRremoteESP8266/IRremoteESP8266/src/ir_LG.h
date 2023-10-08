// Copyright 2017-2021 David Conran

/// @file
/// @brief Support for LG protocols.
/// @see https://github.com/arendst/Tasmota/blob/54c2eb283a02e4287640a4595e506bc6eadbd7f2/sonoff/xdrv_05_irremote.ino#L327-438
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1513

// Supports:
//   Brand: LG,  Model: 6711A20083V remote (LG - LG6711A20083V)
//   Brand: LG,  Model: TS-H122ERM1 remote (LG - LG6711A20083V)
//   Brand: LG,  Model: AKB74395308 remote (LG2)
//   Brand: LG,  Model: S4-W12JA3AA A/C (LG2)
//   Brand: LG,  Model: AKB75215403 remote (LG2)
//   Brand: LG,  Model: AKB74955603 remote (LG2 - AKB74955603)
//   Brand: LG,  Model: A4UW30GFA2 A/C (LG2 - AKB74955603 & AKB73757604)
//   Brand: LG,  Model: AMNW09GSJA0 A/C (LG2 - AKB74955603)
//   Brand: LG,  Model: AMNW24GTPA1 A/C (LG2 - AKB73757604)
//   Brand: LG,  Model: AKB73757604 remote (LG2 - AKB73757604)
//   Brand: LG,  Model: AKB73315611 remote (LG2 - AKB74955603)
//   Brand: LG,  Model: MS05SQ NW0 A/C (LG2 - AKB74955603)
//   Brand: General Electric,  Model: AG1BH09AW101 A/C (LG - GE6711AR2853M)
//   Brand: General Electric,  Model: 6711AR2853M Remote (LG - GE6711AR2853M)

#ifndef IR_LG_H_
#define IR_LG_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRutils.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a LG A/C message.
union LGProtocol{
  uint32_t raw;  ///< The state of the IR remote in IR code form.
  struct {
    uint32_t Sum  :4;
    uint32_t Fan  :4;
    uint32_t Temp :4;
    uint32_t Mode :3;
    uint32_t      :3;
    uint32_t Power:2;
    uint32_t Sign :8;
  };
};

const uint8_t kLgAcFanLowest = 0;  // 0b0000
const uint8_t kLgAcFanLow = 1;     // 0b0001
const uint8_t kLgAcFanMedium = 2;  // 0b0010
const uint8_t kLgAcFanMax = 4;     // 0b0100
const uint8_t kLgAcFanAuto = 5;    // 0b0101
const uint8_t kLgAcFanLowAlt = 9;  // 0b1001
const uint8_t kLgAcFanHigh = 10;   // 0b1010
// Nr. of slots in the look-up table
const uint8_t kLgAcFanEntries = kLgAcFanHigh + 1;
const uint8_t kLgAcTempAdjust = 15;
const uint8_t kLgAcMinTemp = 16;  // Celsius
const uint8_t kLgAcMaxTemp = 30;  // Celsius
const uint8_t kLgAcCool = 0;  // 0b000
const uint8_t kLgAcDry = 1;   // 0b001
const uint8_t kLgAcFan = 2;   // 0b010
const uint8_t kLgAcAuto = 3;  // 0b011
const uint8_t kLgAcHeat = 4;  // 0b100
const uint8_t kLgAcPowerOff = 3;  // 0b11
const uint8_t kLgAcPowerOn = 0;   // 0b00
const uint8_t kLgAcSignature = 0x88;

const uint32_t kLgAcOffCommand          = 0x88C0051;
const uint32_t kLgAcLightToggle         = 0x88C00A6;

const uint32_t kLgAcSwingVToggle        = 0x8810001;
const uint32_t kLgAcSwingSignature      = 0x8813;
const uint32_t kLgAcSwingVLowest        = 0x8813048;
const uint32_t kLgAcSwingVLow           = 0x8813059;
const uint32_t kLgAcSwingVMiddle        = 0x881306A;
const uint32_t kLgAcSwingVUpperMiddle   = 0x881307B;
const uint32_t kLgAcSwingVHigh          = 0x881308C;
const uint32_t kLgAcSwingVHighest       = 0x881309D;
const uint32_t kLgAcSwingVSwing         = 0x8813149;
const uint32_t kLgAcSwingVAuto          = kLgAcSwingVSwing;
const uint32_t kLgAcSwingVOff           = 0x881315A;
const uint8_t  kLgAcSwingVLowest_Short      = 0x04;
const uint8_t  kLgAcSwingVLow_Short         = 0x05;
const uint8_t  kLgAcSwingVMiddle_Short      = 0x06;
const uint8_t  kLgAcSwingVUpperMiddle_Short = 0x07;
const uint8_t  kLgAcSwingVHigh_Short        = 0x08;
const uint8_t  kLgAcSwingVHighest_Short     = 0x09;
const uint8_t  kLgAcSwingVSwing_Short       = 0x14;
const uint8_t  kLgAcSwingVAuto_Short        = kLgAcSwingVSwing_Short;
const uint8_t  kLgAcSwingVOff_Short         = 0x15;

// AKB73757604 Constants
// SwingH
const uint32_t kLgAcSwingHAuto            = 0x881316B;
const uint32_t kLgAcSwingHOff             = 0x881317C;
// SwingV
const uint8_t  kLgAcVaneSwingVHighest     = 1;  ///< 0b001
const uint8_t  kLgAcVaneSwingVHigh        = 2;  ///< 0b010
const uint8_t  kLgAcVaneSwingVUpperMiddle = 3;  ///< 0b011
const uint8_t  kLgAcVaneSwingVMiddle      = 4;  ///< 0b100
const uint8_t  kLgAcVaneSwingVLow         = 5;  ///< 0b101
const uint8_t  kLgAcVaneSwingVLowest      = 6;  ///< 0b110
const uint8_t  kLgAcVaneSwingVSize        = 8;
const uint8_t  kLgAcSwingVMaxVanes = 4;  ///< Max Nr. of Vanes

// Classes
/// Class for handling detailed LG A/C messages.
class IRLgAc {
 public:
  explicit IRLgAc(const uint16_t pin, const bool inverted = false,
                  const bool use_modulation = true);
  void stateReset(void);
  static uint8_t calcChecksum(const uint32_t state);
  static bool validChecksum(const uint32_t state);
  bool isValidLgAc(void) const;
#if SEND_LG
  void send(const uint16_t repeat = kLgDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_LG
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  bool isOffCommand(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  bool isLightToggle(void) const;
  bool isSwing(void) const;
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  bool isSwingV(void) const;
  bool isSwingVToggle(void) const;
  bool isVaneSwingV(void) const;
  void setSwingV(const uint32_t position);
  uint32_t getSwingV(void) const;
  void setVaneSwingV(const uint8_t vane, const uint8_t position);
  uint8_t getVaneSwingV(const uint8_t vane) const;
  static uint32_t calcVaneSwingV(const uint8_t vane, const uint8_t position);
  static uint8_t getVaneCode(const uint32_t raw);
  bool isSwingH(void) const;
  void updateSwingPrev(void);
  uint32_t getRaw(void);
  void setRaw(const uint32_t new_code,
              const decode_type_t protocol = decode_type_t::UNKNOWN);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint32_t code);
  static stdAc::swingv_t toCommonVaneSwingV(const uint8_t pos);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint32_t convertSwingV(const stdAc::swingv_t swingv);
  static uint8_t convertVaneSwingV(const stdAc::swingv_t swingv);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString(void) const;
  void setModel(const lg_ac_remote_model_t model);
  lg_ac_remote_model_t getModel(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  LGProtocol _;
  uint8_t _temp;
  bool _light;
  uint32_t _swingv;
  uint32_t _swingv_prev;
  uint8_t _vaneswingv[kLgAcSwingVMaxVanes];
  uint8_t _vaneswingv_prev[kLgAcSwingVMaxVanes];
  bool _swingh;
  bool _swingh_prev;
  decode_type_t _protocol;  ///< Protocol version
  lg_ac_remote_model_t _model;  ///< Model type
  void checksum(void);
  void _setTemp(const uint8_t value);
  bool _isAKB74955603(void) const;
  bool _isAKB73757604(void) const;
  bool _isLG6711A20083V(void) const;
  bool _isNormal(void) const;
};

#endif  // IR_LG_H_
