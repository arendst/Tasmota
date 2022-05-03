// Copyright 2018-2021 David Conran
/// @file
/// @brief Support for Samsung protocols.
/// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1538 (Checksum)
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1277 (Timers)

// Supports:
//   Brand: Samsung,  Model: UA55H6300 TV (SAMSUNG)
//   Brand: Samsung,  Model: BN59-01178B TV remote (SAMSUNG)
//   Brand: Samsung,  Model: DB63-03556X003 remote
//   Brand: Samsung,  Model: DB93-16761C remote
//   Brand: Samsung,  Model: IEC-R03 remote
//   Brand: Samsung,  Model: AK59-00167A Bluray remote (SAMSUNG36)
//   Brand: Samsung,  Model: AH59-02692E Soundbar remote (SAMSUNG36)
//   Brand: Samsung,  Model: HW-J551 Soundbar (SAMSUNG36)
//   Brand: Samsung,  Model: AR09FSSDAWKNFA A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR09HSFSBWKN A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12KSFPEWQNET A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12HSSDBWKNEU A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12NXCXAWKXEU A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12TXEAAWKNEU A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: DB93-14195A remote (SAMSUNG_AC)
//   Brand: Samsung,  Model: DB96-24901C remote (SAMSUNG_AC)

#ifndef IR_SAMSUNG_H_
#define IR_SAMSUNG_H_

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

/// Native representation of a Samsung A/C message.
union SamsungProtocol{
  uint8_t raw[kSamsungAcExtendedStateLength];  ///< State in code form.
  struct {  // Standard message map
    // Byte 0
    uint8_t         :8;
    // Byte 1
    uint8_t         :4;
    uint8_t         :4;  // Sum1Lower
    // Byte 2
    uint8_t         :4;  // Sum1Upper
    uint8_t         :4;
    // Byte 3
    uint8_t         :8;
    // Byte 4
    uint8_t         :8;
    // Byte 5
    uint8_t         :4;
    uint8_t Sleep5  :1;
    uint8_t Quiet   :1;
    uint8_t         :2;
    // Byte 6
    uint8_t         :4;
    uint8_t Power1  :2;
    uint8_t         :2;
    // Byte 7
    uint8_t         :8;
    // Byte 8
    uint8_t         :4;
    uint8_t         :4;  // Sum2Lower
    // Byte 9
    uint8_t         :4;  // Sum1Upper
    uint8_t Swing   :3;
    uint8_t         :1;
    // Byte 10
    uint8_t               :1;
    uint8_t FanSpecial    :3;  // Powerful, Breeze/WindFree, Econo
    uint8_t Display       :1;
    uint8_t               :2;
    uint8_t CleanToggle10 :1;
    // Byte 11
    uint8_t Ion           :1;
    uint8_t CleanToggle11 :1;
    uint8_t               :2;
    uint8_t Temp          :4;
    // Byte 12
    uint8_t       :1;
    uint8_t Fan   :3;
    uint8_t Mode  :3;
    uint8_t       :1;
    // Byte 13
    uint8_t            :2;
    uint8_t BeepToggle :1;
    uint8_t            :1;
    uint8_t Power2     :2;
    uint8_t            :2;
  };
  struct {  // Extended message map
    // 1st Section
    // Byte 0
    uint8_t                :8;
    // Byte 1
    uint8_t                :4;
    uint8_t Sum1Lower      :4;
    // Byte 2
    uint8_t Sum1Upper      :4;
    uint8_t                :4;
    // Byte 3
    uint8_t                :8;
    // Byte 4
    uint8_t                :8;
    // Byte 5
    uint8_t                :8;
    // Byte 6
    uint8_t                :8;
    // 2nd Section
    // Byte 7
    uint8_t                :8;
    // Byte 8
    uint8_t                :4;
    uint8_t Sum2Lower      :4;
    // Byte 9
    uint8_t Sum2Upper      :4;
    uint8_t OffTimeMins    :3;  // In units of 10's of mins
    uint8_t OffTimeHrs1    :1;  // LSB of the number of hours.
    // Byte 10
    uint8_t OffTimeHrs2    :4;  // MSBs of the number of hours.
    uint8_t OnTimeMins     :3;  // In units of 10's of mins
    uint8_t OnTimeHrs1     :1;  // LSB of the number of hours.
    // Byte 11
    uint8_t OnTimeHrs2     :4;  // MSBs of the number of hours.
    uint8_t                :4;
    // Byte 12
    uint8_t OffTimeDay     :1;
    uint8_t OnTimerEnable  :1;
    uint8_t OffTimerEnable :1;
    uint8_t Sleep12        :1;
    uint8_t OnTimeDay      :1;
    uint8_t                :3;
    // Byte 13
    uint8_t                :8;
    // 3rd Section
    // Byte 14
    uint8_t                :8;
    // Byte 15
    uint8_t                :4;
    uint8_t Sum3Lower      :4;
    // Byte 16
    uint8_t Sum3Upper      :4;
    uint8_t                :4;
    // Byte 17
    uint8_t                :8;
    // Byte 18
    uint8_t                :8;
    // Byte 19
    uint8_t                :8;
    // Byte 20
    uint8_t                :8;
  };
};

// Constants
const uint8_t kSamsungAcMinTemp  = 16;  // C   Mask 0b11110000
const uint8_t kSamsungAcMaxTemp  = 30;  // C   Mask 0b11110000
const uint8_t kSamsungAcAutoTemp = 25;  // C   Mask 0b11110000
const uint8_t kSamsungAcAuto = 0;
const uint8_t kSamsungAcCool = 1;
const uint8_t kSamsungAcDry = 2;
const uint8_t kSamsungAcFan = 3;
const uint8_t kSamsungAcHeat = 4;
const uint8_t kSamsungAcFanAuto = 0;
const uint8_t kSamsungAcFanLow = 2;
const uint8_t kSamsungAcFanMed = 4;
const uint8_t kSamsungAcFanHigh = 5;
const uint8_t kSamsungAcFanAuto2 = 6;
const uint8_t kSamsungAcFanTurbo = 7;
const uint16_t kSamsungAcSectionLength = 7;
const uint64_t kSamsungAcPowerSection = 0x1D20F00000000;

// Classes
/// Class for handling detailed Samsung A/C messages.
class IRSamsungAc {
 public:
  explicit IRSamsungAc(const uint16_t pin, const bool inverted = false,
                       const bool use_modulation = true);
  void stateReset(const bool extended = true, const bool initialPower = true);
#if SEND_SAMSUNG_AC
  void send(const uint16_t repeat = kSamsungAcDefaultRepeat);
  void sendExtended(const uint16_t repeat = kSamsungAcDefaultRepeat);
  void sendOn(const uint16_t repeat = kSamsungAcDefaultRepeat);
  void sendOff(const uint16_t repeat = kSamsungAcDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_SAMSUNG_AC
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(const bool on);
  bool getSwing(void) const;
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  void setBeep(const bool on);
  bool getBeep(void) const;
  void setClean(const bool on);
  bool getClean(void) const;
  void setQuiet(const bool on);
  bool getQuiet(void) const;
  void setPowerful(const bool on);
  bool getPowerful(void) const;
  void setBreeze(const bool on);
  bool getBreeze(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setDisplay(const bool on);
  bool getDisplay(void) const;
  void setIon(const bool on);
  bool getIon(void) const;
  uint16_t getOnTimer(void) const;
  void setOnTimer(const uint16_t nr_of_mins);
  uint16_t getOffTimer(void) const;
  void setOffTimer(const uint16_t nr_of_mins);
  uint16_t getSleepTimer(void) const;
  void setSleepTimer(const uint16_t nr_of_mins);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kSamsungAcStateLength);
  static uint8_t calcSectionChecksum(const uint8_t *section);
  static uint8_t getSectionChecksum(const uint8_t *section);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kSamsungAcStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
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
  SamsungProtocol _;
  bool _forceextended;  ///< Flag to know when we need to send an extended mesg.
  bool _lastsentpowerstate;
  bool _OnTimerEnable;
  bool _OffTimerEnable;
  bool _Sleep;
  bool _lastSleep;
  uint16_t _OnTimer;
  uint16_t _OffTimer;
  uint16_t _lastOnTimer;
  uint16_t _lastOffTimer;
  void checksum(void);
  uint16_t _getOnTimer(void) const;
  uint16_t _getOffTimer(void) const;
  void _setOnTimer(void);
  void _setOffTimer(void);
  void _setSleepTimer(void);
};

#endif  // IR_SAMSUNG_H_
