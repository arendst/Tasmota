// Copyright 2017 Schmolders
// Copyright 2022 crankyoldgit
// Copyright 2022 Mateusz Bronk (mbronk)
/// @file
/// @brief Support for Argo Ulisse 13 DCI Mobile Split ACs.

// Supports:
//   Brand: Argo,  Model: Ulisse 13 DCI Mobile Split A/C [WREM2 remote]
//   Brand: Argo,  Model: Ulisse Eco Mobile Split A/C (Wifi) [WREM3 remote]

#ifndef IR_ARGO_H_
#define IR_ARGO_H_

#include <set>
#include <utility>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif


//  ARGO Ulisse DCI

/// Native representation of a Argo A/C message for WREM-2 remote.
union ArgoProtocol {
  uint8_t raw[kArgoStateLength];  ///< The state in native IR code form
  struct {
    // Byte 0
    uint64_t Pre1     :8;  // Typically 0b00110101
    // Byte 1
    uint64_t Pre2     :8;  // Typically 0b10101111
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
    uint32_t Post     :2;
    uint32_t Sum      :8;  // straddle byte 10 and 11
    uint32_t          :6;
  };
  struct {
    // Byte 0-1
    uint8_t           :8;
    uint8_t           :8;
    // Byte 2-3
    uint8_t CheckHi   :3;
    uint8_t SensorT   :5;
    uint8_t Fixed     :3;  // Typically 0b011
    uint8_t CheckLo   :5;
  };
};

/// Native representation of A/C IR message for WREM-3 remote
/// @note The remote sends 4 different IR command types, varying in length
///       and methods of checksum calculation
///       - [0b00] Regular A/C command (change operation mode)    - 6-byte
///       - [0b01] iFeel Temperature report                       - 2-byte
///       - [0b10] Timer command                                  - 9-byte
///       - [0b11] Config command                                 - 4-byte
/// @note The 1st 2 structures are unnamed for compat. with @c ArgoProtocol
///       1st byte definition is a header common across all commands though
union ArgoProtocolWREM3 {
  uint8_t raw[kArgoStateLength];  ///< The state in native IR code form
  struct {
    // Byte 0 (same definition across the union)
    uint8_t Pre1           :4;  /// Preamble: 0b1011 @ref kArgoWrem3Preamble
    uint8_t IrChannel      :2;  /// 0..3 range
    uint8_t IrCommandType  :2;  /// @ref argoIrMessageType_t
    // Byte 1
    uint8_t RoomTemp       :5;  // in Celsius, range:  4..35 (offset by -4[*C])
    uint8_t Mode           :3;  /// @ref argoMode_t
    // Byte 2
    uint8_t Temp           :5;  // in Celsius, range: 10..32 (offset by -4[*C])
    uint8_t Fan            :3;  /// @ref argoFan_t
    // Byte3
    uint8_t Flap           :3;  /// SwingV @ref argoFlap_t
    uint8_t Power          :1;
    uint8_t iFeel          :1;
    uint8_t Night          :1;
    uint8_t Eco            :1;
    uint8_t Max            :1;  ///< a.k.a. Turbo
    // Byte4
    uint8_t Filter         :1;
    uint8_t Light          :1;
    uint8_t Post1          :6;  /// Unknown, always 0b110000 (TempScale?)
    // Byte5
    uint8_t Sum            :8;  /// Checksum
  };
  struct {
    // Byte 0 (same definition across the union)
    uint8_t                :8;  // {Pre1 | IrChannel | IrCommandType}
    // Byte 1
    uint8_t SensorT        :5;  // in Celsius, range:  4..35 (offset by -4[*C])
    uint8_t CheckHi        :3;  // Checksum (short)
  };
  struct Timer {
    // Byte 0 (same definition across the union)
    uint8_t                   : 8;  // {Pre1 | IrChannel | IrCommandType}
    // Byte 1
    uint8_t IsOn              : 1;
    uint8_t TimerType         : 3;
    uint8_t CurrentTimeLo     : 4;
    // Byte 2
    uint8_t CurrentTimeHi     : 7;
    uint8_t CurrentWeekdayLo  : 1;
    // Byte 3
    uint8_t CurrentWeekdayHi  : 2;
    uint8_t DelayTimeLo       : 6;
    // Byte 4
    uint8_t DelayTimeHi       : 5;
    uint8_t TimerStartLo      : 3;
    // Byte 5
    uint8_t TimerStartHi      : 8;
    // Byte 6
    uint8_t TimerEndLo        : 8;
    // Byte 7
    uint8_t TimerEndHi        : 3;
    uint8_t TimerActiveDaysLo : 5;  // Bitmap (LSBit is Sunday)
    // Byte 8
    uint8_t TimerActiveDaysHi : 2;  // Bitmap (LSBit is Sunday)
    uint8_t Post1             : 1;  // Unknown, always 1
    uint8_t Checksum          : 5;
  } timer;
  struct Config {
    uint8_t           :8;  // Byte 0 {Pre1 | IrChannel | IrCommandType}
    uint8_t Key       :8;  // Byte 1
    uint8_t Value     :8;  // Byte 2
    uint8_t Checksum  :8;  // Byte 3
  } config;
};

// Constants (WREM-2). Store MSB left.
const uint8_t  kArgoHeatBit    = 0b00100000;
const uint8_t  kArgoPreamble1  = 0b10101100;
const uint8_t  kArgoPreamble2  = 0b11110101;
const uint8_t  kArgoPost       = 0b00000010;

// Constants (generic)
const uint16_t kArgoFrequency  = 38000;  // Hz
// Temp
const uint8_t kArgoTempDelta   = 4;
const uint8_t kArgoMaxRoomTemp = 35;  // Celsius
const uint8_t kArgoMinTemp     = 10;  // Celsius delta +4
const uint8_t kArgoMaxTemp     = 32;  // Celsius
const uint8_t kArgoMaxChannel  = 3;


/// @brief IR message type (determines the payload part of IR command)
/// @note Raw values match WREM-3 protocol, but the enum is used in generic
///       context
/// @note WREM-3 remote supports all commands separately, whereas
///       WREM-2 (allegedly) only has the @c AC_CONTROL and @c IFEEL_TEMP_REPORT
///       (timers are part of @c AC_CONTROL command), and there's no config.
enum class argoIrMessageType_t : uint8_t {
  AC_CONTROL = 0b00,
  IFEEL_TEMP_REPORT = 0b01,
  TIMER_COMMAND = 0b10,  // WREM-3 only (WREM-2 has it under AC_CONTROL)
  CONFIG_PARAM_SET = 0b11  // WREM-3 only
};

/// @brief A/C operation mode
/// @note Raw values match WREM-3 protocol, but the enum is used in generic
///       context
enum class argoMode_t : uint8_t {
  COOL = 0b001,
  DRY  = 0b010,
  HEAT = 0b011,
  FAN  = 0b100,
  AUTO = 0b101
};

// Raw mode definitions for WREM-2 remote
// (not wraped into a ns nor enum for backwards-compat.)
const uint8_t kArgoCool =           0b000;
const uint8_t kArgoDry =            0b001;
const uint8_t kArgoAuto =           0b010;
const uint8_t kArgoOff =            0b011;
const uint8_t kArgoHeat =           0b100;
const uint8_t kArgoHeatAuto =       0b101;
// ?no idea what mode that is
const uint8_t kArgoHeatBlink =      0b110;

/// @brief Fan speed
/// @note Raw values match WREM-3 protocol, but the enum is used in generic
///       context
enum class argoFan_t : uint8_t {
  FAN_AUTO = 0b000,
  FAN_LOWEST = 0b001,
  FAN_LOWER = 0b010,
  FAN_LOW  = 0b011,
  FAN_MEDIUM = 0b100,
  FAN_HIGH  = 0b101,
  FAN_HIGHEST  = 0b110
};

// Raw fan speed definitions for WREM-2 remote
// (not wraped into a ns nor enum for backwards-compat.)
const uint8_t kArgoFanAuto = 0;      // 0b00
const uint8_t kArgoFan1 = 1;         // 0b01
const uint8_t kArgoFan2 = 2;         // 0b10
const uint8_t kArgoFan3 = 3;         // 0b11

/// @brief Flap position (swing-V)
/// @note Raw values match WREM-3 protocol, but the enum is used in generic
///       context
enum class argoFlap_t : uint8_t {
  FLAP_AUTO = 0,
  FLAP_1 = 1,  // Highest
  FLAP_2 = 2,
  FLAP_3 = 3,
  FLAP_4 = 4,
  FLAP_5 = 5,
  FLAP_6 = 6,  // Lowest
  FLAP_FULL = 7
};

// Raw Flap/SwingV definitions for WREM-2 remote
// (not wraped into a ns nor enum for backwards-compat.)
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


/// @brief Timer type to set (for @c argoIrMessageType_t::TIMER_COMMAND)
/// @note Raw values match WREM-3 protocol
enum class argoTimerType_t : uint8_t {
  NO_TIMER         = 0b000,
  DELAY_TIMER      = 0b001,
  SCHEDULE_TIMER_1 = 0b010,
  SCHEDULE_TIMER_2 = 0b011,
  SCHEDULE_TIMER_3 = 0b100
};

/// @brief Day type to set (for @c argoIrMessageType_t::TIMER_COMMAND)
/// @note Raw values match WREM-3 protocol
enum class argoWeekday : uint8_t {
  SUNDAY    = 0b000,
  MONDAY    = 0b001,
  TUESDAY   = 0b010,
  WEDNESDAY = 0b011,
  THURSDAY  = 0b100,
  FRIDAY    = 0b101,
  SATURDAY  = 0b110
};



/// @brief Base class for handling *common* support for Argo remote protocols
///        (functionality is shared across WREM-2 and WREM-3 IR protocols)
/// @note This class uses static polymorphism and full template specializations
///       when required, to avoid a performance penalty of doing v-table lookup.
///       2 instantiations are forced in impl. file: for @c ArgoProtocol and
///       @c ArgoProtocolWREM3
/// @note This class is abstract (though does not declare a pure-virtual fn.
///       for abovementioned reasons), and instead declares protected c-tor
/// @tparam ARGO_PROTOCOL_T The Raw device protocol/message used
template <typename ARGO_PROTOCOL_T>
class IRArgoACBase {
#ifndef UNIT_TEST  // A less cloggy way of expressing FRIEND_TEST(...)

 protected:
#else

 public:
#endif
  explicit IRArgoACBase(const uint16_t pin, const bool inverted = false,
                    const bool use_modulation = true);

 public:
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

  void setSensorTemp(const uint8_t degrees);
  uint8_t getSensorTemp(void) const;

  void setFan(const argoFan_t fan);
  void setFanEx(const argoFan_t fan) { setFan(fan); }
  argoFan_t getFanEx(void) const;  ///< `-Ex` for backw. compat w/ @c IRArgoAC

  void setFlap(const argoFlap_t flap);
  void setFlapEx(const argoFlap_t flap) { setFlap(flap); }
  argoFlap_t getFlapEx(void) const;  ///< `-Ex` for backw. compat w/ @c IRArgoAC

  void setMode(const argoMode_t mode);
  void setModeEx(const argoMode_t mode) { setMode(mode); }
  argoMode_t getModeEx(void) const;  ///< `-Ex` for backw. compat w/ @c IRArgoAC

  void setMax(const bool on);
  bool getMax(void) const;

  void setNight(const bool on);
  bool getNight(void) const;

  void setiFeel(const bool on);
  bool getiFeel(void) const;

  void setMessageType(const argoIrMessageType_t msgType);
  argoIrMessageType_t getMessageType(void) const;
  static argoIrMessageType_t getMessageType(const uint8_t state[],
      const uint16_t length);

  uint8_t* getRaw(void);
  uint16_t getRawByteLength() const;
  static uint16_t getStateLengthForIrMsgType(argoIrMessageType_t type);
  void setRaw(const uint8_t state[], const uint16_t length);

  static bool validChecksum(const uint8_t state[], const uint16_t length);

  static argoMode_t convertMode(const stdAc::opmode_t mode);
  static argoFan_t convertFan(const stdAc::fanspeed_t speed);
  static argoFlap_t convertSwingV(const stdAc::swingv_t position);
  static argoIrMessageType_t convertCommand(const stdAc::ac_command_t command);

 protected:
  void _stateReset(ARGO_PROTOCOL_T *state, argoIrMessageType_t messageType
      = argoIrMessageType_t::AC_CONTROL);
  void stateReset(argoIrMessageType_t messageType
      = argoIrMessageType_t::AC_CONTROL);
  void _checksum(ARGO_PROTOCOL_T *state);
  void checksum(void);
  static uint16_t getRawByteLength(const ARGO_PROTOCOL_T& raw,
    argoIrMessageType_t messageTypeHint = argoIrMessageType_t::AC_CONTROL);
  static uint8_t calcChecksum(const uint8_t state[], const uint16_t length);
  static uint8_t getChecksum(const uint8_t state[], const uint16_t length);

  static stdAc::opmode_t toCommonMode(const argoMode_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const argoFan_t speed);
  static stdAc::swingv_t toCommonSwingV(const uint8_t position);
  static stdAc::ac_command_t toCommonCommand(const argoIrMessageType_t command);

  // Attributes
  ARGO_PROTOCOL_T _;  ///< The raw protocol data
  uint16_t _length = kArgoStateLength;
  argoIrMessageType_t _messageType = argoIrMessageType_t::AC_CONTROL;

#ifndef UNIT_TEST

 protected:
  IRsend _irsend;  ///< instance of the IR send class
#else

 public:
  /// @cond IGNORE
  IRsendTest _irsend;  ///< instance of the testing IR send class
  /// @endcond
#endif
};

/// @brief Supports Argo A/C SAC-WREM2 IR remote protocol
class IRArgoAC : public IRArgoACBase<ArgoProtocol> {
 public:
    explicit IRArgoAC(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);

  #if SEND_ARGO
    void sendSensorTemp(const uint8_t degrees,
                        const uint16_t repeat = kArgoDefaultRepeat);
  #endif  // SEND_ARGO

    String toString(void) const;
    stdAc::state_t toCommon(void) const;

    using IRArgoACBase<ArgoProtocol>::setMode;
    void setMode(const uint8_t mode);  /// @deprecated, for backwards-compat.
    uint8_t getMode(void) const;  /// @deprecated, for backwards-compat.

    using IRArgoACBase<ArgoProtocol>::setFan;
    void setFan(const uint8_t fan);  /// @deprecated, for backwards-compat.
    uint8_t getFan(void) const;  /// @deprecated, for backwards-compat.

    using IRArgoACBase<ArgoProtocol>::setFlap;
    void setFlap(const uint8_t flap);  /// @deprecated, for backwards-compat.
    uint8_t getFlap(void) const;  /// @deprecated, for backwards-compat.
};

/// @brief Supports Argo A/C SAC-WREM3 IR remote protocol
class IRArgoAC_WREM3 : public IRArgoACBase<ArgoProtocolWREM3> {
 public:
  explicit IRArgoAC_WREM3(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);

  #if SEND_ARGO
    void sendSensorTemp(const uint8_t degrees,
                        const uint16_t repeat = kArgoDefaultRepeat);
  #endif  // SEND_ARGO

  argo_ac_remote_model_t getModel(void) const;


  argoFan_t getFan(void) const;
  argoFlap_t getFlap(void) const;
  argoMode_t getMode(void) const;

  void setEco(const bool on);
  bool getEco(void) const;

  void setFilter(const bool on);
  bool getFilter(void) const;

  void setLight(const bool on);
  bool getLight(void) const;

  void setChannel(const uint8_t channel);
  uint8_t getChannel(void) const;

  void setConfigEntry(const uint8_t paramId, const uint8_t value);
  std::pair<uint8_t, uint8_t> getConfigEntry(void) const;

  void setCurrentTimeMinutes(uint16_t currentTimeMinutes);
  uint16_t getCurrentTimeMinutes(void) const;

  void setCurrentDayOfWeek(argoWeekday dayOfWeek);
  argoWeekday getCurrentDayOfWeek(void) const;

  void setTimerType(const argoTimerType_t timerType);
  argoTimerType_t getTimerType(void) const;

  void setDelayTimerMinutes(const uint16_t delayMinutes);
  uint16_t getDelayTimerMinutes(void) const;

  void setScheduleTimerStartMinutes(const uint16_t startTimeMinutes);
  uint16_t getScheduleTimerStartMinutes(void) const;
  // uint16_t getTimerXStartMinutes(void) const

  void setScheduleTimerStopMinutes(const uint16_t stopTimeMinutes);
  uint16_t getScheduleTimerStopMinutes(void) const;
  // uint16_t getTimerXStopMinutes(void) const;


  void setScheduleTimerActiveDays(const std::set<argoWeekday>& days);
  std::set<argoWeekday> getScheduleTimerActiveDays(void) const;
  uint8_t getTimerActiveDaysBitmap(void) const;

  using IRArgoACBase<ArgoProtocolWREM3>::getMessageType;
  static argoIrMessageType_t getMessageType(const ArgoProtocolWREM3& raw);

  String toString(void) const;
  stdAc::state_t toCommon(void) const;

  static bool hasValidPreamble(const uint8_t state[], const uint16_t length);

 public:
#if DECODE_ARGO
  static bool isValidWrem3Message(const uint8_t state[], const uint16_t nbits,
      bool verifyChecksum = true);
#endif
};

#endif  // IR_ARGO_H_
