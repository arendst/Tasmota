// Copyright 2017 Schmolders
/// @file
/// @brief Support for Argo Ulisse 13 DCI Mobile Split ACs.

// Supports:
//   Brand: Argo,  Model: Ulisse 13 DCI Mobile Split A/C

#ifndef IR_ARGO_H_
#define IR_ARGO_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif


//  ARGO Ulisse DCI

/// Native representation of a Argo A/C message.
union ArgoProtocol {
  uint8_t raw[kArgoStateLength];  ///< The state in native IR code form
  struct {
    // Byte 0
    uint64_t          :8;  // Typically 0b00110101
    // Byte 1
    uint64_t          :8;  // Typically 0b10101111
    // Byte 2~4
    uint64_t          :3;
    uint64_t Mode     :3;
    uint64_t Temp     :5;  // straddle byte 2 and 3
    uint64_t Fan      :2;
    uint64_t RoomTemp :5;  // straddle byte 3 and 4
    uint64_t Flap     :3;  // SwingV
    uint64_t          :3;  // OnTimer, maybe hours
    // Byte 5
    uint64_t          :8;  // OnTimer, maybe minutes
    // Byte 6
    uint64_t          :8;  // OffTimer, maybe minutes
    // Byte 7
    uint64_t          :3;  // OffTimer, maybe hours
    uint64_t          :5;  // Time
    // Byte 8
    uint32_t          :6;  // Time
    uint32_t          :1;  // Timer On/Off
    uint32_t          :1;  // Timer Program
    // Byte 9
    uint32_t          :1;  // Timer Program
    uint32_t          :1;  // Timer 1h
    uint32_t Night    :1;
    uint32_t Max      :1;
    uint32_t          :1;  // Filter
    uint32_t Power    :1;
    uint32_t          :1;  // const 0
    uint32_t iFeel    :1;
    // Byte 10~11
    uint32_t          :2;  // const 01
    uint32_t Sum      :8;  // straddle byte 10 and 11
    uint32_t          :6;
  };
};

// Constants. Store MSB left.

const uint8_t kArgoHeatBit =      0b00100000;

// Mode                           0b00111000
const uint8_t kArgoCool =           0b000;
const uint8_t kArgoDry =            0b001;
const uint8_t kArgoAuto =           0b010;
const uint8_t kArgoOff =            0b011;
const uint8_t kArgoHeat =           0b100;
const uint8_t kArgoHeatAuto =       0b101;
// ?no idea what mode that is
const uint8_t kArgoHeatBlink =      0b110;

// Fan                               0b00011000
const uint8_t kArgoFanAuto = 0;      // 0b00
const uint8_t kArgoFan1 = 1;         // 0b01
const uint8_t kArgoFan2 = 2;         // 0b10
const uint8_t kArgoFan3 = 3;         // 0b11

// Temp
const uint8_t kArgoTempDelta = 4;
const uint8_t kArgoMaxRoomTemp = 35;  // Celsius
const uint8_t kArgoMinTemp = 10;  // Celsius delta +4
const uint8_t kArgoMaxTemp = 32;  // Celsius

// Flap/SwingV
const uint8_t kArgoFlapAuto = 0;
const uint8_t kArgoFlap1 = 1;
const uint8_t kArgoFlap2 = 2;
const uint8_t kArgoFlap3 = 3;
const uint8_t kArgoFlap4 = 4;
const uint8_t kArgoFlap5 = 5;
const uint8_t kArgoFlap6 = 6;
const uint8_t kArgoFlapFull = 7;

// Legacy defines. (Deprecated)
#define ARGO_COOL_ON              kArgoCoolOn
#define ARGO_COOL_OFF             kArgoCoolOff
#define ARGO_COOL_AUTO            kArgoCoolAuto
#define ARGO_COOL_HUM             kArgoCoolHum
#define ARGO_HEAT_ON              kArgoHeatOn
#define ARGO_HEAT_AUTO            kArgoHeatAuto
#define ARGO_HEAT_BLINK           kArgoHeatBlink
#define ARGO_MIN_TEMP             kArgoMinTemp
#define ARGO_MAX_TEMP             kArgoMaxTemp
#define ARGO_FAN_AUTO             kArgoFanAuto
#define ARGO_FAN_3                kArgoFan3
#define ARGO_FAN_2                kArgoFan2
#define ARGO_FAN_1                kArgoFan1
#define ARGO_FLAP_AUTO            kArgoFlapAuto
#define ARGO_FLAP_1               kArgoFlap1
#define ARGO_FLAP_2               kArgoFlap2
#define ARGO_FLAP_3               kArgoFlap3
#define ARGO_FLAP_4               kArgoFlap4
#define ARGO_FLAP_5               kArgoFlap5
#define ARGO_FLAP_6               kArgoFlap6
#define ARGO_FLAP_FULL            kArgoFlapFull


/// Class for handling detailed Argo A/C messages.
class IRArgoAC {
 public:
  explicit IRArgoAC(const uint16_t pin, const bool inverted = false,
                    const bool use_modulation = true);

#if SEND_ARGO
  void send(const uint16_t repeat = kArgoDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_ARGO
  void begin(void);
  void on(void);
  void off(void);

  void setPower(const bool on);
  bool getPower(void) const;

  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void) const;

  void setFan(const uint8_t fan);
  uint8_t getFan(void) const;

  void setFlap(const uint8_t flap);
  uint8_t getFlap(void) const;

  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;

  void setMax(const bool on);
  bool getMax(void) const;

  void setNight(const bool on);
  bool getNight(void) const;

  void setiFeel(const bool on);
  bool getiFeel(void) const;

  void setTime(void);
  void setRoomTemp(const uint8_t degrees);
  uint8_t getRoomTemp(void) const;

  uint8_t* getRaw(void);
  void setRaw(const uint8_t state[]);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kArgoStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kArgoStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static uint8_t convertSwingV(const stdAc::swingv_t position);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
  // # of bytes per command
  ArgoProtocol _;
  void stateReset(void);
  void checksum(void);

  // Attributes
  uint8_t flap_mode;
  uint8_t heat_mode;
  uint8_t cool_mode;
};

#endif  // IR_ARGO_H_
