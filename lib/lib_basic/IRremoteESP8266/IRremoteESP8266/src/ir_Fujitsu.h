// Copyright 2017 Jonny Graham
// Copyright 2018-2021 David Conran
// Copyright 2021 siriuslzx

/// @file
/// @brief Support for Fujitsu A/C protocols.
/// Fujitsu A/C support added by Jonny Graham
/// @warning Use of incorrect model may cause the A/C unit to lock up.
/// e.g. An A/C that uses an AR-RAH1U remote may lock up requiring a physical
///      power rest, if incorrect model (ARRAH2E) is used with a Swing command.
///      The correct model for it is ARREB1E.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1376

// Supports:
//   Brand: Fujitsu,  Model: AR-RAH2E remote (ARRAH2E)
//   Brand: Fujitsu,  Model: ASYG30LFCA A/C (ARRAH2E)
//   Brand: Fujitsu General,  Model: AR-RCE1E remote (ARRAH2E)
//   Brand: Fujitsu General,  Model: ASHG09LLCA A/C (ARRAH2E)
//   Brand: Fujitsu General,  Model: AOHG09LLC A/C (ARRAH2E)
//   Brand: Fujitsu,  Model: AR-DB1 remote (ARDB1)
//   Brand: Fujitsu,  Model: AST9RSGCW A/C (ARDB1)
//   Brand: Fujitsu,  Model: AR-REB1E remote (ARREB1E)
//   Brand: Fujitsu,  Model: ASYG7LMCA A/C (ARREB1E)
//   Brand: Fujitsu,  Model: AR-RAE1E remote (ARRAH2E)
//   Brand: Fujitsu,  Model: AGTV14LAC A/C (ARRAH2E)
//   Brand: Fujitsu,  Model: AR-RAC1E remote (ARRAH2E)
//   Brand: Fujitsu,  Model: ASTB09LBC A/C (ARRY4)
//   Brand: Fujitsu,  Model: AR-RY4 remote (ARRY4)
//   Brand: Fujitsu General,  Model: AR-JW2 remote (ARJW2)
//   Brand: Fujitsu,  Model: AR-DL10 remote (ARDB1)
//   Brand: Fujitsu,  Model: ASU30C1 A/C (ARDB1)
//   Brand: Fujitsu,  Model: AR-RAH1U remote (ARREB1E)
//   Brand: Fujitsu,  Model: ASU12RLF A/C (ARREB1E)
//   Brand: Fujitsu,  Model: AR-REW4E remote (ARREW4E)
//   Brand: Fujitsu,  Model: ASYG09KETA-B A/C (ARREW4E)

#ifndef IR_FUJITSU_H_
#define IR_FUJITSU_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifdef ARDUINO
#include <Arduino.h>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

/// Native representation of a Fujitsu A/C message.
union FujitsuProtocol {
  struct {
    uint8_t longcode[kFujitsuAcStateLength];  ///< The state of the IR remote.
    uint8_t shortcode[kFujitsuAcStateLengthShort];
  };
  struct {
    // Byte 0~1
    uint64_t           :16;  // Fixed header
    // Byte 2
    uint64_t            :4;
    uint64_t Id         :2;   // Device Number/Identifier
    uint64_t            :2;
    // Byte 3-4
    uint64_t            :16;
    // Byte 5
    uint64_t Cmd        :8;  // short codes:cmd; long codes:fixed value
    // Byte 6
    uint64_t RestLength :8;  // Nr. of bytes in the message after this byte.
    // Byte 7
    uint64_t Protocol   :8;  // Seems like a protocol version number. Not sure.
    // Byte 8
    uint64_t Power      :1;
    uint64_t Fahrenheit :1;
    uint64_t Temp       :6;  // Internal representation varies between models.
    // Byte 9
    uint64_t Mode       :3;
    uint64_t Clean      :1;  // Also 10C Heat in ARREW4E.
    uint64_t TimerType  :2;
    uint64_t            :2;
    // Byte 10
    uint64_t Fan    :3;
    uint64_t        :1;
    uint64_t Swing  :2;
    uint64_t        :2;
    // Byte 11~13
    uint64_t OffTimer       :11;  // Also is the sleep timer value
    uint64_t OffTimerEnable :1;
    uint64_t OnTimer        :11;
    uint64_t OnTimerEnable  :1;
    // Byte 14
    uint64_t              :3;
    uint64_t Filter       :1;
    uint64_t              :1;
    uint64_t unknown      :1;
    uint64_t              :1;
    uint64_t OutsideQuiet :1;
    // Byte 15
    uint64_t              :0;  // Checksum
  };
};

// Constants
const uint8_t kFujitsuAcModeAuto =  0x0;  // 0b000
const uint8_t kFujitsuAcModeCool =  0x1;  // 0b001
const uint8_t kFujitsuAcModeDry =   0x2;  // 0b010
const uint8_t kFujitsuAcModeFan =   0x3;  // 0b011
const uint8_t kFujitsuAcModeHeat =  0x4;  // 0b100

const uint8_t kFujitsuAcCmdStayOn = 0x00;            // b00000000
const uint8_t kFujitsuAcCmdTurnOn = 0x01;            // b00000001
const uint8_t kFujitsuAcCmdTurnOff = 0x02;           // b00000010
const uint8_t kFujitsuAcCmdEcono = 0x09;             // b00001001
const uint8_t kFujitsuAcCmdPowerful = 0x39;          // b00111001
const uint8_t kFujitsuAcCmdStepVert = 0x6C;          // b01101100
const uint8_t kFujitsuAcCmdToggleSwingVert = 0x6D;   // b01101101
const uint8_t kFujitsuAcCmdStepHoriz = 0x79;         // b01111001
const uint8_t kFujitsuAcCmdToggleSwingHoriz = 0x7A;  // b01111010

const uint8_t kFujitsuAcFanAuto = 0x00;
const uint8_t kFujitsuAcFanHigh = 0x01;
const uint8_t kFujitsuAcFanMed = 0x02;
const uint8_t kFujitsuAcFanLow = 0x03;
const uint8_t kFujitsuAcFanQuiet = 0x04;

const float   kFujitsuAcMinTemp =     16;  // 16C
const float   kFujitsuAcMaxTemp =     30;  // 30C
const uint8_t kFujitsuAcTempOffsetC = kFujitsuAcMinTemp;
const float   kFujitsuAcMinTempF =    60;  // 60F
const float   kFujitsuAcMaxTempF =    88;  // 88F
const uint8_t kFujitsuAcTempOffsetF = 44;

const uint8_t kFujitsuAcSwingOff = 0x00;
const uint8_t kFujitsuAcSwingVert = 0x01;
const uint8_t kFujitsuAcSwingHoriz = 0x02;
const uint8_t kFujitsuAcSwingBoth = 0x03;

const uint8_t kFujitsuAcStopTimers =                       0b00;  // 0
const uint8_t kFujitsuAcSleepTimer =                       0b01;  // 1
const uint8_t kFujitsuAcOffTimer =                         0b10;  // 2
const uint8_t kFujitsuAcOnTimer =                          0b11;  // 3
const uint16_t kFujitsuAcTimerMax = 12 * 60;  ///< Minutes.

// Legacy defines.
#define FUJITSU_AC_MODE_AUTO kFujitsuAcModeAuto
#define FUJITSU_AC_MODE_COOL kFujitsuAcModeCool
#define FUJITSU_AC_MODE_DRY kFujitsuAcModeDry
#define FUJITSU_AC_MODE_FAN kFujitsuAcModeFan
#define FUJITSU_AC_MODE_HEAT kFujitsuAcModeHeat
#define FUJITSU_AC_CMD_STAY_ON kFujitsuAcCmdStayOn
#define FUJITSU_AC_CMD_TURN_ON kFujitsuAcCmdTurnOn
#define FUJITSU_AC_CMD_TURN_OFF kFujitsuAcCmdTurnOff
#define FUJITSU_AC_CMD_STEP_HORIZ kFujitsuAcCmdStepHoriz
#define FUJITSU_AC_CMD_STEP_VERT kFujitsuAcCmdStepVert
#define FUJITSU_AC_FAN_AUTO kFujitsuAcFanAuto
#define FUJITSU_AC_FAN_HIGH kFujitsuAcFanHigh
#define FUJITSU_AC_FAN_MED kFujitsuAcFanMed
#define FUJITSU_AC_FAN_LOW kFujitsuAcFanLow
#define FUJITSU_AC_FAN_QUIET kFujitsuAcFanQuiet
#define FUJITSU_AC_MIN_TEMP kFujitsuAcMinTemp
#define FUJITSU_AC_MAX_TEMP kFujitsuAcMaxTemp
#define FUJITSU_AC_SWING_OFF kFujitsuAcSwingOff
#define FUJITSU_AC_SWING_VERT kFujitsuAcSwingVert
#define FUJITSU_AC_SWING_HORIZ kFujitsuAcSwingHoriz
#define FUJITSU_AC_SWING_BOTH kFujitsuAcSwingBoth

/// Class for handling detailed Fujitsu A/C messages.
class IRFujitsuAC {
 public:
  explicit IRFujitsuAC(const uint16_t pin,
                       const fujitsu_ac_remote_model_t model = ARRAH2E,
                       const bool inverted = false,
                       const bool use_modulation = true);
  void setModel(const fujitsu_ac_remote_model_t model);
  fujitsu_ac_remote_model_t getModel(void) const;
  void stateReset(void);
#if SEND_FUJITSU_AC
  void send(const uint16_t repeat = kFujitsuAcMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_FUJITSU_AC
  void begin(void);
  void stepHoriz(void);
  void toggleSwingHoriz(const bool update = true);
  void stepVert(void);
  void toggleSwingVert(const bool update = true);
  void setCmd(const uint8_t cmd);
  uint8_t getCmd(void) const;
  void setTemp(const float temp, const bool useCelsius = true);
  float getTemp(void) const;
  void setFanSpeed(const uint8_t fan);
  uint8_t getFanSpeed(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(const uint8_t mode);
  uint8_t getSwing(void) const;
  uint8_t* getRaw(void);
  bool setRaw(const uint8_t newState[], const uint16_t length);
  uint8_t getStateLength(void);
  static bool validChecksum(uint8_t* state, const uint16_t length);
  void setPower(const bool on);
  void off(void);
  void on(void);
  bool getPower(void) const;
  void setClean(const bool on);
  bool getClean(void) const;
  void setFilter(const bool on);
  bool getFilter(void) const;
  void set10CHeat(const bool on);
  bool get10CHeat(void) const;
  void setOutsideQuiet(const bool on);
  bool getOutsideQuiet(void) const;
  uint8_t getTimerType(void) const;
  void setTimerType(const uint8_t timertype);
  uint16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t nr_mins);
  uint16_t getOffSleepTimer(void) const;
  void setOffTimer(const uint16_t nr_mins);
  void setSleepTimer(const uint16_t nr_mins);
  void setId(const uint8_t num);
  uint8_t getId(void) const;
  void setCelsius(const bool on);
  bool getCelsius(void) const;
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  FujitsuProtocol _;
  uint8_t _cmd;
  fujitsu_ac_remote_model_t _model;
  uint8_t _state_length;
  uint8_t _state_length_short;
  void checkSum(void);
  bool updateUseLongOrShort(void);
  void buildFromState(const uint16_t length);
  void setOffSleepTimer(const uint16_t nr_mins);
};

#endif  // IR_FUJITSU_H_
