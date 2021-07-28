// Copyright 2020 David Conran

/// @file
/// @brief Support for Sanyo protocols.
/// Sanyo LC7461 support originally by marcosamarinho
/// Sanyo SA 8650B originally added from
///   https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Sanyo.cpp
/// @see http://pdf.datasheetcatalog.com/datasheet/sanyo/LC7461.pdf
/// @see https://github.com/marcosamarinho/IRremoteESP8266/blob/master/ir_Sanyo.cpp
/// @see http://slydiman.narod.ru/scr/kb/sanyo.htm
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1211
/// @see https://docs.google.com/spreadsheets/d/1dYfLsnYvpjV-SgO8pdinpfuBIpSzm8Q1R5SabrLeskw/edit?usp=sharing

// Supports:
//   Brand: Sanyo,  Model: SA 8650B - disabled
//   Brand: Sanyo,  Model: LC7461 transmitter IC (SANYO_LC7461)
//   Brand: Sanyo,  Model: SAP-K121AHA A/C (SANYO_AC)
//   Brand: Sanyo,  Model: RCS-2HS4E remote (SANYO_AC)
//   Brand: Sanyo,  Model: SAP-K242AH A/C (SANYO_AC)
//   Brand: Sanyo,  Model: RCS-2S4E remote (SANYO_AC)

#ifndef IR_SANYO_H_
#define IR_SANYO_H_

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

/// Native representation of a Sanyo A/C message.
union SanyoProtocol{
  uint8_t raw[kSanyoAcStateLength];  ///< The state in IR code form.
  // Ref: https://docs.google.com/spreadsheets/d/1dYfLsnYvpjV-SgO8pdinpfuBIpSzm8Q1R5SabrLeskw/edit?usp=sharing
  struct {
    // Byte 0
    uint8_t :8;  // 0x6A (Fixed?)
    // Byte 1
    uint8_t Temp :5;
    uint8_t      :3;
    // Byte 2
    uint8_t SensorTemp :5;
    uint8_t Sensor     :1;  ///< Sensor location (0 = remote, 1 = A/C)
    uint8_t Beep       :1;
    uint8_t            :1;
    // Byte 3
    uint8_t OffHour :4;
    uint8_t         :4;
    // Byte 4
    uint8_t Fan      :2;
    uint8_t OffTimer :1;
    uint8_t          :1;
    uint8_t Mode     :3;
    uint8_t          :1;
    // Byte 5
    uint8_t SwingV :3;
    uint8_t        :3;
    uint8_t Power  :2;
    // Byte 6
    uint8_t       :3;
    uint8_t Sleep :1;
    uint8_t       :4;
    // Byte 7
    uint8_t :8;
    // Byte 8
    uint8_t Sum :8;
  };
};

// Constants

const uint8_t kSanyoAcTempMin = 16;    ///< Celsius
const uint8_t kSanyoAcTempMax = 30;    ///< Celsius
const uint8_t kSanyoAcTempDelta = 4;   ///< Celsius to Native Temp difference.

const uint8_t kSanyoAcHourMax = 15;       ///<          0b1111

const uint8_t kSanyoAcHeat = 1;        ///<       0b001
const uint8_t kSanyoAcCool = 2;        ///<       0b010
const uint8_t kSanyoAcDry =  3;        ///<       0b011
const uint8_t kSanyoAcAuto = 4;        ///<       0b100
const uint8_t kSanyoAcFanAuto =   0;   ///<            0b00
const uint8_t kSanyoAcFanHigh =   1;   ///<            0b01
const uint8_t kSanyoAcFanLow =    2;   ///<            0b10
const uint8_t kSanyoAcFanMedium = 3;   ///<            0b11

// const uint8_t kSanyoAcPowerStandby =           0b00;  ///< Standby?
const uint8_t kSanyoAcPowerOff =                  0b01;  ///< Off
const uint8_t kSanyoAcPowerOn =                   0b10;  ///< On
const uint8_t kSanyoAcSwingVAuto =         0;  ///<     0b000
const uint8_t kSanyoAcSwingVLowest =       2;  ///<     0b010
const uint8_t kSanyoAcSwingVLow =          3;  ///<     0b011
const uint8_t kSanyoAcSwingVLowerMiddle =  4;  ///<     0b100
const uint8_t kSanyoAcSwingVUpperMiddle =  5;  ///<     0b101
const uint8_t kSanyoAcSwingVHigh =         6;  ///<     0b110
const uint8_t kSanyoAcSwingVHighest =      7;  ///<     0b111

// Classes
/// Class for handling detailed Sanyo A/C messages.
class IRSanyoAc {
 public:
  explicit IRSanyoAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
  void stateReset(void);
#if SEND_SANYO_AC
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_SANYO_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setSensorTemp(const uint8_t degrees);
  uint8_t getSensorTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setSensor(const bool location);
  bool getSensor(void) const;
  void setBeep(const bool on);
  bool getBeep(void) const;
  void setSwingV(const uint8_t setting);
  uint8_t getSwingV(void) const;
  void setRaw(const uint8_t newState[]);
  uint8_t* getRaw(void);
  uint16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t mins);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kSanyoAcStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t setting);
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
  SanyoProtocol _;
  void checksum(void);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kSanyoAcStateLength);
};

#endif  // IR_SANYO_H_
