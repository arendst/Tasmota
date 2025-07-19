// Copyright 2020-2021 David Conran (crankyoldgit)
/// @file
/// @brief Support for Mirage protocol
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1289
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1573


// Supports:
//   Brand: Mirage,  Model: VLU series A/C
//   Brand: Maxell,  Model: MX-CH18CF A/C
//   Brand: Maxell,  Model: KKG9A-C1 remote
//   Brand: Tronitechnik,  Model: Reykir 9000 A/C
//   Brand: Tronitechnik,  Model: KKG29A-C1 remote

#ifndef IR_MIRAGE_H_
#define IR_MIRAGE_H_

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

/// Native representation of a Mirage 120-bit A/C message.
/// @see https://docs.google.com/spreadsheets/d/1Ucu9mOOIIJoWQjUJq_VCvwgV3EwKaRk8K2AuZgccYEk/edit#gid=0
union Mirage120Protocol{
  uint8_t raw[kMirageStateLength];  ///< The state in code form.
  struct {  // Common
    // Byte 0
    uint8_t Header        :8;  // Header. (0x56)
    // Byte 1
    uint8_t Temp          :8;  // Celsius minus 0x5C.
    // Byte 2
    uint8_t               :8;  // Unknown / Unused.
    // Byte 3
    uint8_t               :8;  // Unknown / Unused.
    // Byte 4
    uint8_t Fan           :2;  // Fan Speed.
    uint8_t               :2;  // Unknown / Unused.
    uint8_t Mode          :4;  // Cool, Heat, Dry, Fan, Recycle
    // Byte 5
    uint8_t               :8;
    // Byte 6
    uint8_t               :8;
    // Byte 7
    uint8_t               :8;
    // Byte 8
    uint8_t               :8;
    // Byte 9
    uint8_t               :8;
    // Byte 10
    uint8_t               :8;
    // Byte 11
    uint8_t               :8;
    // Byte 12
    uint8_t               :8;
    // Byte 13
    uint8_t               :8;
    // Byte 14
    uint8_t Sum           :8;  // Sum of all the previous nibbles.
  };
  struct {  // KKG9AC1 remote
    // Byte 0
    uint8_t               :8;  // Header
    // Byte 1
    uint8_t               :8;  // Temp
    // Byte 2
    uint8_t               :8;  // Unknown / Unused.
    // Byte 3
    uint8_t               :3;  // Unknown / Unused.
    uint8_t Light_Kkg9ac1 :1;  // Aka. Display. Seems linked to Sleep mode.
    uint8_t               :4;  // Unknown / Unused.
    // Byte 4
    uint8_t               :8;  // Fan & Mode
    // Byte 5
    uint8_t               :1;  // Unknown
    uint8_t SwingAndPower :7;
    // Byte 6
    uint8_t               :7;  // Unknown / Unused.
    uint8_t Sleep_Kkg9ac1 :1;  // Sleep mode on or off.
    // Byte 7
    uint8_t               :3;  // Unknown / Unused.
    uint8_t Turbo_Kkg9ac1 :1;  // Turbo mode on or off. Only works in Cool mode.
    uint8_t               :4;  // Unknown / Unused.
    // Byte 8
    uint8_t               :8;  // Unknown / Unused.
    // Byte 9
    uint8_t               :8;  // Unknown / Unused.
    // Byte 10
    uint8_t               :8;  // Unknown / Unused.
    // Byte 11
    uint8_t Seconds       :8;  // Nr. of Seconds in BCD.
    // Byte 12
    uint8_t Minutes       :8;  // Nr. of Minutes in BCD.
    // Byte 13
    uint8_t Hours         :8;  // Nr. of Hours in BCD.
    // Byte 14
    uint8_t               :8;  // Sum
  };
  struct {  // KKG29A-C1 remote
    // Byte 0
    uint8_t                :8;  // Header
    // Byte 1
    uint8_t                :8;  // Temp
    // Byte 2
    uint8_t                :8;
    // Byte 3
    uint8_t Quiet          :1;
    uint8_t                :7;
    // Byte 4
    uint8_t                :2;  // Fan
    uint8_t OffTimerEnable :1;
    uint8_t OnTimerEnable  :1;
    uint8_t                :3;  // Mode
    uint8_t                :1;
    // Byte 5
    uint8_t SwingH         :1;
    uint8_t SwingV         :1;
    uint8_t LightToggle_Kkg29ac1 :1;  // Aka. Display Toggle.
    uint8_t                :3;
    uint8_t Power          :2;
    // Byte 6
    uint8_t                :1;
    uint8_t Filter         :1;  // Aka. UVC
    uint8_t                :1;
    uint8_t Sleep_Kkg29ac1 :1;  // Sleep mode on or off.
    uint8_t                :2;
    uint8_t RecycleHeat    :1;
    uint8_t                :1;
    // Byte 7
    uint8_t SensorTemp     :6;  // Temperature at the remote
    uint8_t CleanToggle    :1;
    uint8_t IFeel          :1;
    // Byte 8
    uint8_t OnTimerHours   :5;
    uint8_t                :2;
    uint8_t Turbo_Kkg29ac1 :1;  // Turbo mode on or off.
    // Byte 9
    uint8_t OnTimerMins    :6;
    uint8_t                :2;
    // Byte 10
    uint8_t OffTimerHours  :5;
    uint8_t                :3;
    // Byte 11
    uint8_t OffTimerMins   :6;
    uint8_t                :2;
    // Byte 12
    uint8_t                :8;
    // Byte 13
    uint8_t                :8;
    // Byte 14
    uint8_t                :8;  // Sum
  };
};

// Constants
const uint8_t kMirageAcHeat =    0b001;  // 1
const uint8_t kMirageAcCool =    0b010;  // 2
const uint8_t kMirageAcDry =     0b011;  // 3
const uint8_t kMirageAcRecycle = 0b100;  // 4
const uint8_t kMirageAcFan =     0b101;  // 5

const uint8_t kMirageAcFanAuto = 0b00;  // 0
const uint8_t kMirageAcFanHigh = 0b01;  // 1
const uint8_t kMirageAcFanMed =  0b10;  // 2
const uint8_t kMirageAcFanLow =  0b11;  // 3
const uint8_t kMirageAcKKG29AC1FanAuto = 0b00;  // 0
const uint8_t kMirageAcKKG29AC1FanHigh = 0b01;  // 1
const uint8_t kMirageAcKKG29AC1FanLow =  0b10;  // 2
const uint8_t kMirageAcKKG29AC1FanMed =  0b11;  // 3

const uint8_t kMirageAcMinTemp = 16;  // 16C
const uint8_t kMirageAcMaxTemp = 32;  // 32C
const uint8_t kMirageAcTempOffset = 0x5C;
const uint8_t kMirageAcSensorTempOffset = 20;
const uint8_t kMirageAcSensorTempMax = 43;  // Celsius

const uint8_t kMirageAcPowerOff = 0x5F;
const uint8_t kMirageAcSwingVOff =      0b0000;  // 0
const uint8_t kMirageAcSwingVLowest =   0b0011;  // 3
const uint8_t kMirageAcSwingVLow =      0b0101;  // 5
const uint8_t kMirageAcSwingVMiddle =   0b0111;  // 7
const uint8_t kMirageAcSwingVHigh =     0b1001;  // 9
const uint8_t kMirageAcSwingVHighest =  0b1011;  // 11
const uint8_t kMirageAcSwingVAuto =     0b1101;  // 13


/// Class for handling detailed Mirage 120-bit A/C messages.
/// @note Inspired and derived from the work done at: https://github.com/r45635/HVAC-IR-Control
/// @warning Consider this very alpha code. Seems to work, but not validated.
class IRMirageAc {
 public:
  explicit IRMirageAc(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset(void);
#if SEND_MIRAGE
  void send(const uint16_t repeat = kMirageMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MIRAGE
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t* data);
  uint32_t getClock(void) const;
  void setClock(const uint32_t nr_of_seconds);
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setSwingV(const uint8_t position);
  uint8_t getSwingV(void) const;
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  void setQuiet(const bool on);
  bool getQuiet(void) const;
  void setCleanToggle(const bool on);
  bool getCleanToggle(void) const;
  void setFilter(const bool on);
  bool getFilter(void) const;
  void setIFeel(const bool on);
  bool getIFeel(void) const;
  void setSensorTemp(const uint8_t degrees);
  uint16_t getSensorTemp(void) const;
  uint16_t getOnTimer(void) const;
  uint16_t getOffTimer(void) const;
  void setOnTimer(const uint16_t nr_of_mins);
  void setOffTimer(const uint16_t nr_of_mins);
  mirage_ac_remote_model_t getModel(const bool useRaw = false) const;
  void setModel(const mirage_ac_remote_model_t model);
  static mirage_ac_remote_model_t getModel(const uint8_t *state);
  static bool validChecksum(const uint8_t* data);
  static uint8_t calculateChecksum(const uint8_t* data);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed,
      const mirage_ac_remote_model_t model = mirage_ac_remote_model_t::KKG9AC1);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed,
      const mirage_ac_remote_model_t model = mirage_ac_remote_model_t::KKG9AC1);
  static stdAc::swingv_t toCommonSwingV(const uint8_t pos);
  stdAc::state_t toCommon(void) const;
  void fromCommon(const stdAc::state_t state);
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  Mirage120Protocol _;
  mirage_ac_remote_model_t _model;
  void checksum(void);
};
#endif  // IR_MIRAGE_H_
