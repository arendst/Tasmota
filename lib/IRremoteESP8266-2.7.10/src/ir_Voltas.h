// Copyright 2020 David Conran (crankyoldgit)
// Copyright 2020 manj9501
/// @file
/// @brief Support for Voltas A/C protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1238

// Supports:
//   Brand: Voltas,  Model: 122LZF 4011252 Window A/C
//
// Ref: https://docs.google.com/spreadsheets/d/1zzDEUQ52y7MZ7_xCU3pdjdqbRXOwZLsbTGvKWcicqCI/
// Ref: https://www.corona.co.jp/box/download.php?id=145060636229

#ifndef IR_VOLTAS_H_
#define IR_VOLTAS_H_

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


union VoltasProtocol {
  uint8_t raw[kVoltasStateLength];  ///< The state in native IR code form
  struct {
    // Byte 0
    uint8_t SwingH         :1;
    uint8_t SwingHChange   :7;
    // Byte 1
    uint8_t Mode           :4;
    uint8_t                :1;  // Unknown/Unused
    uint8_t FanSpeed       :3;
    // Byte 2
    uint8_t SwingV         :3;
    uint8_t Wifi           :1;
    uint8_t                :1;  // Unknown/Unused
    uint8_t Turbo          :1;
    uint8_t Sleep          :1;
    uint8_t Power          :1;
    // Byte 3
    uint8_t Temp           :4;
    uint8_t                :2;  // Typically 0b01
    uint8_t Econo          :1;
    uint8_t TempSet        :1;
    // Byte 4
    uint8_t OnTimerMins    :6;  // 0-59
    uint8_t                :1;  // Unknown/Unused
    uint8_t OnTimer12Hr    :1;  // (Nr of Hours + 1) % 12.
    // Byte 5
    uint8_t OffTimerMins   :6;  // 0-59
    uint8_t                :1;  // Unknown/Unused
    uint8_t OffTimer12Hr   :1;  // (Nr of Hours + 1) % 12.
    // Byte 6
    uint8_t                :8;  // Typically 0b00111011(0x3B)
    // Byte 7
    uint8_t OnTimerHrs     :4;  // (Nr of Hours + 1) % 12.
    uint8_t OffTimerHrs    :4;  // (Nr of Hours + 1) % 12.
    // Byte 8
    uint8_t                :5;  // Typically 0b00000
    uint8_t Light          :1;
    uint8_t OffTimerEnable :1;
    uint8_t OnTimerEnable  :1;
    // Byte 9
    uint8_t Checksum       :8;
  };
};

// Constants
const uint8_t kVoltasFan   = 0b0001;  ///< 1
const uint8_t kVoltasHeat  = 0b0010;  ///< 2
const uint8_t kVoltasDry   = 0b0100;  ///< 4
const uint8_t kVoltasCool  = 0b1000;  ///< 8
const uint8_t kVoltasMinTemp = 16;  ///< Celsius
const uint8_t kVoltasDryTemp = 24;  ///< Celsius
const uint8_t kVoltasMaxTemp = 30;  ///< Celsius
const uint8_t kVoltasFanHigh = 0b001;  ///< 1
const uint8_t kVoltasFanMed =  0b010;  ///< 2
const uint8_t kVoltasFanLow =  0b100;  ///< 4
const uint8_t kVoltasFanAuto = 0b111;  ///< 7
const uint8_t kVoltasSwingHChange =   0b1111100;  ///< 0x7D
const uint8_t kVoltasSwingHNoChange = 0b0011001;  ///< 0x19

// Classes
/// Class for handling detailed Voltas A/C messages.
class IRVoltas {
 public:
  explicit IRVoltas(const uint16_t pin, const bool inverted = false,
                    const bool use_modulation = true);
  void stateReset();
#if SEND_VOLTAS
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_VOLTAS
  void begin();
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kVoltasStateLength);
  void setModel(const voltas_ac_remote_model_t model);
  voltas_ac_remote_model_t getModel(const bool raw = false) const;
  void setPower(const bool on);
  bool getPower(void) const;
  void on(void);
  void off(void);
  void setWifi(const bool on);
  bool getWifi(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void);
  void setFan(const uint8_t speed);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  void setSwingHChange(const bool on);
  bool getSwingHChange(void) const;
  void setSwingV(const bool on);
  bool getSwingV(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  uint16_t getOnTime(void) const;
  void setOnTime(const uint16_t nr_of_mins);
  uint16_t getOffTime(void) const;
  void setOffTime(const uint16_t nr_of_mins);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  uint8_t convertMode(const stdAc::opmode_t mode);
  uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  VoltasProtocol _;  ///< The state of the IR remote.
  voltas_ac_remote_model_t _model;  ///< Model type.
  void checksum(void);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kVoltasStateLength);
};
#endif  // IR_VOLTAS_H_
