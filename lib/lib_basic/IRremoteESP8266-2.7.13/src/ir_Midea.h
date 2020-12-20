// Copyright 2017 David Conran

/// @file
/// @brief Support for Midea protocols.
/// Midea added by crankyoldgit & bwze
/// @see https://docs.google.com/spreadsheets/d/1TZh4jWrx4h9zzpYUI9aYXMl1fYOiqu-xVuOOMqagxrs/edit?usp=sharing

// Supports:
//   Brand: Pioneer System,  Model: RYBO12GMFILCAD A/C (12K BTU) (MIDEA)
//   Brand: Pioneer System,  Model: RUBO18GMFILCAD A/C (18K BTU) (MIDEA)
//   Brand: Comfee, Model: MPD1-12CRN7 A/C (MIDEA)
//   Brand: Kaysun, Model: Casual CF A/C (MIDEA)
//   Brand: Keystone, Model: RG57H4(B)BGEF remote (MIDEA)
//   Brand: MrCool,  Model: RG57A6/BGEFU1 remote (MIDEA)
//   Brand: Midea,  Model: FS40-7AR Stand Fan (MIDEA24)
//   Brand: Danby,  Model: DAC080BGUWDB (MIDEA)
//   Brand: Danby,  Model: DAC100BGUWDB (MIDEA)
//   Brand: Danby,  Model: DAC120BGUWDB (MIDEA)
//   Brand: Danby,  Model: R09C/BCGE remote (MIDEA)

#ifndef IR_MIDEA_H_
#define IR_MIDEA_H_

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

// Compile-time model specific overrides.
// Uncomment one of these if you have such a devices to better match your A/C.
// It changes some of the special commands/settings.
//
// #define DANBY_DAC true
// #define KAYSUN_AC true

/// Native representation of a Midea A/C message.
union MideaProtocol{
  uint64_t remote_state;  ///< The state in native IR code form
  // only use 48bits
  struct {
    // Byte 0
    uint8_t Sum;
    // Byte 1 (value=0xFF when not in use.)
    // This byte gets dual usage as Sensor Temp and On Timer
    // Depending on "Type" below.
    // When in "OnTimer", the nr of half hours is stored with mask 0b01111110
    // i.e.
    //  uint8_t :1;
    //  uint8_t OnTimerHalfHours:6;
    //  uint8_t :1;
    uint8_t SensorTemp:7;  ///< Degrees or OnTimer.
    uint8_t disableSensor:1;
    // Byte 2 (value=0xFF when not in use.)
    uint8_t :1;  // 0b1
    uint8_t OffTimer:6;  ///< Nr of Half hours. Off is 0b111111
    uint8_t BeepDisable:1;  ///< 0 = no beep in follow me messages, 1 = beep.
    // Byte 3
    uint8_t Temp:5;
    uint8_t useFahrenheit:1;
    uint8_t :0;
    // Byte 4
    uint8_t Mode:3;
    uint8_t Fan:2;
    uint8_t :1;
    uint8_t Sleep:1;
    uint8_t Power:1;
    // Byte 5
    uint8_t Type:3;    ///< Normal, Special, or FollowMe message type
    uint8_t Header:5;  ///< Typically 0b10100
  };
};

// Constants
const uint8_t kMideaACMinTempF = 62;        ///< Fahrenheit
const uint8_t kMideaACMaxTempF = 86;        ///< Fahrenheit
const uint8_t kMideaACMinTempC = 17;        ///< Celsius
const uint8_t kMideaACMaxTempC = 30;        ///< Celsius
const uint8_t kMideaACMinSensorTempC = 0;   ///< Celsius
const uint8_t kMideaACMaxSensorTempC = 37;  ///< Celsius
const uint8_t kMideaACMinSensorTempF = 32;  ///< Fahrenheit
const uint8_t kMideaACMaxSensorTempF = 99;  ///< Fahrenheit (Guess only!)
const uint8_t kMideaACSensorTempOnTimerOff = 0b1111111;
const uint8_t kMideaACTimerOff = 0b111111;
const uint8_t kMideaACCool = 0;     // 0b000
const uint8_t kMideaACDry = 1;      // 0b001
const uint8_t kMideaACAuto = 2;     // 0b010
const uint8_t kMideaACHeat = 3;     // 0b011
const uint8_t kMideaACFan = 4;      // 0b100
const uint8_t kMideaACFanAuto = 0;  // 0b00
const uint8_t kMideaACFanLow = 1;   // 0b01
const uint8_t kMideaACFanMed = 2;   // 0b10
const uint8_t kMideaACFanHigh = 3;  // 0b11
#if KAYSUN_AC
  // For Kaysun AC units, Toggle SwingV is 0xA202FFFFFF7E
  const uint64_t kMideaACToggleSwingV = 0xA202FFFFFF7E;
  const uint64_t kMideaACSwingVStep =   0xA201FFFFFF7C;
#else  // KAYSUN_AC
  const uint64_t kMideaACToggleSwingV = 0xA201FFFFFF7C;
#endif  // KAYSUN_AC
#if DANBY_DAC
  // For Danby DAC unit, the Ionizer toggle is the same as ToggleSwingV
  // const uint64_t kMideaACToggleIonizer = 0xA201FFFFFF7C;
  kSwingVToggleStr = kIonStr;
#endif  // DANBY_DAC
const uint64_t kMideaACToggleEcono = 0xA202FFFFFF7E;
const uint64_t kMideaACToggleLight = 0xA208FFFFFF75;
const uint64_t kMideaACToggleTurbo = 0xA209FFFFFF74;
const uint8_t kMideaACTypeCommand = 0b001;  ///< Message type
const uint8_t kMideaACTypeSpecial = 0b010;  ///< Message type
const uint8_t kMideaACTypeFollow =  0b100;  ///< Message type

// Legacy defines. (Deprecated)
#define MIDEA_AC_COOL kMideaACCool
#define MIDEA_AC_DRY kMideaACDry
#define MIDEA_AC_AUTO kMideaACAuto
#define MIDEA_AC_HEAT kMideaACHeat
#define MIDEA_AC_FAN kMideaACFan
#define MIDEA_AC_FAN_AUTO kMideaACFanAuto
#define MIDEA_AC_FAN_LOW kMideaACFanLow
#define MIDEA_AC_FAN_MED kMideaACFanMed
#define MIDEA_AC_FAN_HI kMideaACFanHigh
#define MIDEA_AC_POWER kMideaACPower
#define MIDEA_AC_SLEEP kMideaACSleep
#define MIDEA_AC_MIN_TEMP_F kMideaACMinTempF
#define MIDEA_AC_MAX_TEMP_F kMideaACMaxTempF
#define MIDEA_AC_MIN_TEMP_C kMideaACMinTempC
#define MIDEA_AC_MAX_TEMP_C kMideaACMaxTempC

// Classes
/// Class for handling detailed Midea A/C messages.
/// @warning Consider this very alpha code.
class IRMideaAC {
 public:
  explicit IRMideaAC(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);
  void stateReset(void);
#if SEND_MIDEA
  void send(const uint16_t repeat = kMideaMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_MIDEA
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  bool getUseCelsius(void) const;
  void setUseCelsius(const bool celsius);
  void setTemp(const uint8_t temp, const bool useCelsius = false);
  uint8_t getTemp(const bool useCelsius = false) const;
  void setSensorTemp(const uint8_t temp, const bool useCelsius = false);
  uint8_t getSensorTemp(const bool useCelsius = false) const;
  void setEnableSensorTemp(const bool on);
  bool getEnableSensorTemp(void) const;
  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setRaw(const uint64_t newState);
  uint64_t getRaw(void);
  static bool validChecksum(const uint64_t state);
  void setSleep(const bool on);
  bool getSleep(void) const;
  bool isSwingVToggle(void) const;
  void setSwingVToggle(const bool on);
  bool getSwingVToggle(void);
  #if KAYSUN_AC
  bool isSwingVStep(void) const;
  void setSwingVStep(const bool on);
  bool getSwingVStep(void);
  #endif  // KAYSUN_AC
  bool isEconoToggle(void) const;
  void setEconoToggle(const bool on);
  bool getEconoToggle(void);
  bool isTurboToggle(void) const;
  void setTurboToggle(const bool on);
  bool getTurboToggle(void);
  bool isLightToggle(void) const;
  void setLightToggle(const bool on);
  bool getLightToggle(void);
  uint8_t getType(void) const;
  bool isOnTimerEnabled(void) const;
  uint16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t mins);
  bool isOffTimerEnabled(void) const;
  uint16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t mins);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL);
  String toString(void);
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  MideaProtocol _;
  bool _SwingVToggle;
  #if KAYSUN_AC
  bool _SwingVStep;
  #endif  // KAYSUN_AC
  bool _EconoToggle;
  bool _TurboToggle;
  bool _LightToggle;
  void checksum(void);
  static uint8_t calcChecksum(const uint64_t state);
  void setType(const uint8_t type);
};

#endif  // IR_MIDEA_H_
