// Copyright 2019 crankyoldgit

/// @file
/// @brief Support for Sharp protocols.
/// @see http://www.sbprojects.net/knowledge/ir/sharp.htm
/// @see http://lirc.sourceforge.net/remotes/sharp/GA538WJSA
/// @see http://www.mwftr.com/ucF08/LEC14%20PIC%20IR.pdf
/// @see http://www.hifi-remote.com/johnsfine/DecodeIR.html#Sharp
/// @see GlobalCache's IR Control Tower data.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/638
/// @see https://github.com/ToniA/arduino-heatpumpir/blob/master/SharpHeatpumpIR.cpp
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1091
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1387

// Supports:
//   Brand: Sharp,  Model: LC-52D62U TV
//   Brand: Sharp,  Model: AY-ZP40KR A/C (A907)
//   Brand: Sharp,  Model: AH-AxSAY A/C (A907)
//   Brand: Sharp,  Model: CRMC-A907 JBEZ remote (A907)
//   Brand: Sharp,  Model: AH-PR13-GL A/C (A903)
//   Brand: Sharp,  Model: CRMC-A903JBEZ remote (A903)
//   Brand: Sharp,  Model: AH-XP10NRY A/C (A903)
//   Brand: Sharp,  Model: CRMC-820 JBEZ remote (A903)
//   Brand: Sharp,  Model: CRMC-A705 JBEZ remote (A705)
//   Brand: Sharp,  Model: AH-A12REVP-1 A/C (A903)
//   Brand: Sharp,  Model: CRMC-A863 JBEZ remote (A903)

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

/// Native representation of a Sharp A/C message.
union SharpProtocol{
  uint8_t raw[kSharpAcStateLength];  ///< State of the remote in IR code form
  struct {
    // Byte 0~3
    uint8_t pad[4];
    // Byte 4
    uint8_t Temp  :4;
    uint8_t Model :1;
    uint8_t       :3;
    // Byte 5
    uint8_t       :4;
    uint8_t PowerSpecial  :4;
    // Byte 6
    uint8_t Mode  :2;
    uint8_t       :1;
    uint8_t Clean :1;
    uint8_t Fan   :3;
    uint8_t       :1;
    // Byte 7
    uint8_t TimerHours  :4;
    uint8_t             :2;
    uint8_t TimerType   :1;
    uint8_t TimerEnabled:1;
    // Byte 8
    uint8_t Swing :3;
    uint8_t       :5;
    // Byte 9
    uint8_t       :8;
    // Byte 10
    uint8_t Special :8;
    // Byte 11
    uint8_t        :2;
    uint8_t Ion    :1;
    uint8_t        :1;
    uint8_t Model2 :1;
    uint8_t        :3;
    // Byte 12
    uint8_t     :4;
    uint8_t Sum :4;
  };
};

// Constants
const uint16_t kSharpAcHdrMark = 3800;
const uint16_t kSharpAcHdrSpace = 1900;
const uint16_t kSharpAcBitMark = 470;
const uint16_t kSharpAcZeroSpace = 500;
const uint16_t kSharpAcOneSpace = 1400;
const uint32_t kSharpAcGap = kDefaultMessageGap;

const uint8_t kSharpAcByteTemp = 4;
const uint8_t kSharpAcMinTemp = 15;  // Celsius
const uint8_t kSharpAcMaxTemp = 30;  // Celsius

const uint8_t kSharpAcPowerUnknown = 0;                     // 0b0000
const uint8_t kSharpAcPowerOnFromOff = 1;                   // 0b0001
const uint8_t kSharpAcPowerOff = 2;                         // 0b0010
const uint8_t kSharpAcPowerOn = 3;                          // 0b0011 (Normal)
const uint8_t kSharpAcPowerSetSpecialOn = 6;                // 0b0110
const uint8_t kSharpAcPowerSetSpecialOff = 7;               // 0b0111
const uint8_t kSharpAcPowerTimerSetting = 8;                // 0b1000

const uint8_t kSharpAcAuto =                             0b00;  // A907 only
const uint8_t kSharpAcFan =                              0b00;  // A705 only
const uint8_t kSharpAcDry =                              0b11;
const uint8_t kSharpAcCool =                             0b10;
const uint8_t kSharpAcHeat =                             0b01;  // A907 only
const uint8_t kSharpAcFanAuto =                     0b010;  // 2
const uint8_t kSharpAcFanMin =                      0b100;  // 4 (FAN1)
const uint8_t kSharpAcFanMed =                      0b011;  // 3 (FAN2)
const uint8_t kSharpAcFanA705Low =                  0b011;  // 3 (A903 too)
const uint8_t kSharpAcFanHigh =                     0b101;  // 5 (FAN3)
const uint8_t kSharpAcFanA705Med =                  0b101;  // 5 (A903 too)
const uint8_t kSharpAcFanMax =                      0b111;  // 7 (FAN4)

const uint8_t kSharpAcTimerIncrement = 30;  // Mins
const uint8_t kSharpAcTimerHoursOff =                             0b0000;
const uint8_t kSharpAcTimerHoursMax =                             0b1100;  // 12
const uint8_t kSharpAcOffTimerType =                           0b0;
const uint8_t kSharpAcOnTimerType =                            0b1;

const uint8_t kSharpAcSwingToggle =                0b111;
const uint8_t kSharpAcSwingNoToggle =              0b000;

const uint8_t kSharpAcSpecialPower =              0x00;
const uint8_t kSharpAcSpecialTurbo =              0x01;
const uint8_t kSharpAcSpecialTempEcono =          0x04;
const uint8_t kSharpAcSpecialFan =                0x05;
const uint8_t kSharpAcSpecialSwing =              0x06;
const uint8_t kSharpAcSpecialTimer =              0xC0;
const uint8_t kSharpAcSpecialTimerHalfHour =      0xDE;

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
  void setModel(const sharp_ac_remote_model_t model);
  sharp_ac_remote_model_t getModel(const bool raw = false) const;
  void on(void);
  void off(void);
  void setPower(const bool on, const bool prev_on = true);
  bool getPower(void) const;
  bool isPowerSpecial(void) const;
  void setTemp(const uint8_t temp, const bool save = true);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t fan, const bool save = true);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode, const bool save = true);
  uint8_t getMode(void) const;
  void setSpecial(const uint8_t mode);
  uint8_t getSpecial(void) const;
  bool getTurbo(void) const;
  void setTurbo(const bool on);
  bool getSwingToggle(void) const;
  void setSwingToggle(const bool on);
  bool getIon(void) const;
  void setIon(const bool on);
  bool getEconoToggle(void) const;
  void setEconoToggle(const bool on);
  bool getLightToggle(void) const;
  void setLightToggle(const bool on);
  uint16_t getTimerTime(void) const;
  bool getTimerEnabled(void) const;
  bool getTimerType(void) const;
  void setTimer(bool enable, bool timer_type, uint16_t mins);
  bool getClean(void) const;
  void setClean(const bool on);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kSharpAcStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kSharpAcStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed,
                            const sharp_ac_remote_model_t model =
                                sharp_ac_remote_model_t::A907);
  stdAc::opmode_t toCommonMode(const uint8_t mode) const;
  stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed) const;
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  SharpProtocol _;
  uint8_t _temp;  ///< Saved copy of the desired temp.
  uint8_t _mode;  ///< Saved copy of the desired mode.
  uint8_t _fan;  ///< Saved copy of the desired fan speed.
  sharp_ac_remote_model_t _model;  ///< Saved copy of the model.
  void stateReset(void);
  void checksum(void);
  static uint8_t calcChecksum(uint8_t state[],
                              const uint16_t length = kSharpAcStateLength);
  void setPowerSpecial(const uint8_t value);
  uint8_t getPowerSpecial(void) const;
  void clearPowerSpecial(void);
  bool _getEconoToggle(void) const;
  void _setEconoToggle(const bool on);
};

#endif  // IR_SHARP_H_
