// Copyright 2025 GottemHams
/// @file
/// @brief Support for Eurom A/C protocols.
/// @see https://eurom.nl/wp-content/uploads/2022/04/Polar-12C-16CH-v1.0.pdf

// Supports:
//   Brand: Eurom,  Model: Polar 16CH

#ifndef IR_EUROM_H_
#define IR_EUROM_H_

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

/// Native representation of a Eurom message.
union EuromProtocol {
  uint8_t raw[kEuromStateLength];  // The state of the IR remote
  struct {
    // Byte 0 is used as a negative offset for the checksum and is always 0x18
    uint8_t Sum1            :8;

    // Byte 1 is used as part of the checksum only and is always 0x27
    uint8_t Sum2            :8;

    // Byte 2 combines 2 functions and has some considerations:
    // 1. Cooling mode almost always has the lower nibble set to 0x1,
    //    e.g. 0x01 = 16 C, 0x11 = 17 C, 0xF1 = 31 C.
    //    Exception: 0x09 means 32 C (max temperature).
    // 2. Dehumidification doesn't support temperatures, so this is always 0x72.
    // 3. Same goes for fan mode, which is always 0x73.
    // 4. Heating mode almost always has the lower nibble set to 0x4,
    //    e.g. 0x04 = 16 C, 0x14 = 17 C, 0xF4 = 31 C.
    //    Exception: 0x0C means 32 C (max temperature).
    uint8_t Mode_Celsius    :8;

    // Byte 3 also combines 2 functions, with the values being OR'ed together:
    // 1. 0x00 means power off, swing off
    // 2. 0x40 means power off, swing on
    // 3. 0x80 means power on, swing off
    // 3. 0xC0 means power on, swing on
    uint8_t Power_Swing     :8;

    // Byte 4 is to track Fahrenheit separately, but note that it will always
    // reset to 0x00 if Celsius is used. On the other hand, Celsius moves along
    // with this, i.e. a change of +1/-1 C for roughly every 3 F. The base value
    // is 0x41 which corresponds to 61 F and increases by 0x01 for every degree.
    // This gives it a range of 0x41 - 0x5E (inclusive).
    uint8_t Fahrenheit      :8;

    // Byte 5 yet again combines functions:
    // 1. 0x00 for sleep mode disabled, 0x40 for enabled
    // 2. The timer duration is simply encoded as BCD and added to this, with a
    //    maximum of 24 hours
    uint8_t Sleep_OnTimer   :8;

    // Byte 6 seems to be truly unused, since it's always 0x00. We'll still
    // always use it in checksums though.
    uint8_t Sum3            :8;

    // Byte 7 is always at least 0x80, with the hours also being added as BCD,
    // e.g. 0x80 = 0 hours, 0x81 = 1 h, 0xA4 = 24 h.
    uint8_t OffTimer        :8;

    // Byte 8 doesn't really seem to matter, but it should be 0x00 or 0x80 for
    // off and on respectively. Apparently setting the **duration** alone is
    // already enough to set the timer?
    uint8_t OffTimerEnabled :8;

    // Byte 9 is used as part of the checksum only and is slways 0x80
    uint8_t Sum4            :8;

    // Byte 10 is simple: 0x10, 0x20, 0x40 for low, medium and high respectively
    uint8_t Fan             :8;

    // Byte 11 holds a funny checksum. =]
    // Add all nibbles beyond the first byte (excluding the checksum of course),
    // then subtract the first byte. The second byte should always be larger, so
    // this never results in sudden signedness (i.e. underflowing). It might be
    // pure coincidence that the first byte is always 0x18 and they could have
    // hardcoded that value elsewhere/otherwise.
    uint8_t Checksum        :8;
  };
};

// Constants

// IR signal information
const uint16_t kEuromHdrMark = 3257;
const uint16_t kEuromBitMark = 454;
const uint16_t kEuromHdrSpace = 3187;
const uint16_t kEuromOneSpace = 1162;
const uint16_t kEuromZeroSpace = 355;
const uint16_t kEuromSpaceGap = 50058;
const uint16_t kEuromFreq = 38000;

// Modes
const uint8_t kEuromCool = 0x01;  // Lowest possible value, 16 C
const uint8_t kEuromDehumidify = 0x72;
const uint8_t kEuromVentilate = 0x73;
const uint8_t kEuromHeat = 0x04;  // Also 16 C

// Reaching the highest temperature breaks the formula that is used otherwise,
// because we should basically just OR this flag to the above mode byte. It
// seems more like it indicates "max temp" instead of "32 C".
const uint8_t kEuromMaxTempFlag = 0x08;

// Temperatures
const uint8_t kEuromMinTempC = 16;
const uint8_t kEuromMaxTempC = 32;

const uint8_t kEuromMinTempF = 61;
const uint8_t kEuromMaxTempF = 90;

// The enabled flag will simply be added to chosen temperature
const uint8_t kEuromFahrenheitDisabled = 0x00;
const uint8_t kEuromFahrenheitEnabled = 0x04;

// Power and swing
const uint8_t kEuromPowerSwingDisabled = 0x00;
const uint8_t kEuromPowerOn = 0x80;
const uint8_t kEuromSwingOn = 0x40;

// Sleep mode and the "on timer"
const uint8_t kEuromSleepOnTimerDisabled = 0x00;
const uint8_t kEuromSleepEnabled = 0x40;

// The "off timer"
const uint8_t kEuromOffTimerDisabled = 0x00;
const uint8_t kEuromOffTimerEnabled = 0x80;
const uint8_t kEuromOffTimer = kEuromOffTimerEnabled;  // Corresponds to 0 hours

// Stuff for all timers
const uint8_t kEuromTimerMin = 0;
const uint8_t kEuromTimerMax = 24;

// Fan speeds
const uint8_t kEuromFanLow = 0x10;
const uint8_t kEuromFanMed = 0x20;
const uint8_t kEuromFanHigh = 0x40;

// Classes

/// Class for handling detailed Eurom A/C messages.
class IREuromAc {
 public:
  explicit IREuromAc(const uint16_t pin, const bool inverted = false,
                     const bool use_modulation = true);

  void stateReset();
#if SEND_EUROM
  void send(const uint16_t repeat = kNoRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65 ms IR signal pulse at 38 kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) {
    return _irsend.calibrate();
  }
#endif  // SEND_EUROM

  void begin(void);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kEuromStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kEuromStateLength);

  void setRaw(const uint8_t state[]);
  uint8_t *getRaw(void);

  void on(void);
  void off(void);

  void setPower(const bool state);
  bool getPower(void) const;

  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;

  void setTemp(const uint8_t degrees, const bool fahrenheit = false);
  uint8_t getTemp(void) const;
  bool getTempIsFahrenheit(void) const;

  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;

  void setSwing(const bool state);
  bool getSwing(void) const;

  void setSleep(const bool state);
  bool getSleep(void) const;

  void setOffTimer(const uint8_t duration);
  uint8_t getOffTimer(void) const;

  void setOnTimer(const uint8_t duration);
  uint8_t getOnTimer(void) const;

  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static bool convertSwing(const stdAc::swingv_t swing);

  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  static stdAc::swingv_t toCommonSwing(const bool swing);

  stdAc::state_t toCommon(void) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  /// @cond IGNORE
  IRsendTest _irsend;
  /// @endcond
#endif
  EuromProtocol _;

  // Due to some bytes combining multiple functions, we'll need to keep track of
  // some of the original values ourselves. Otherwise we wouldn't really be able
  // to e.g. return the current mode or temperature, or changing the sleep mode
  // without also messing with the timer hours.
  uint8_t state_mode_ = kEuromCool;
  uint8_t state_celsius_ = 23;
  bool state_sleep_ = false;
  uint8_t state_on_timer_ = kEuromTimerMin;

  // Some helper functions for reusing the above state variables depending on
  // context and returning the byte expected by the AC.
  uint8_t getModeCelsiusByte(const uint8_t mode, const uint8_t celsius) const;
  uint8_t getSleepOnTimerByte(const bool sleep, const uint8_t hours) const;

  void checksum(void);
};

#endif  // IR_EUROM_H_
