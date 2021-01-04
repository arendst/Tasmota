// Copyright 2018 David Conran
/// @file
/// @brief Support for Samsung protocols.
/// Samsung originally added from https://github.com/shirriff/Arduino-IRremote/
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/505
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/621
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1062
/// @see http://elektrolab.wz.cz/katalog/samsung_protocol.pdf

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
//   Brand: Samsung,  Model: AR12KSFPEWQNET A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12HSSDBWKNEU A/C (SAMSUNG_AC)
//   Brand: Samsung,  Model: AR12NXCXAWKXEU A/C (SAMSUNG_AC)

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
  struct {
    // Byte 0
    uint8_t :8;
    // Byte 1
    uint8_t         :4;
    uint8_t Quiet1  :1;
    uint8_t Power1  :1;
    uint8_t         :2;
    // Byte 2~4
    uint8_t pad0[3];
    // Byte 5
    uint8_t         :5;
    uint8_t Quiet5  :1;
    uint8_t         :2;
    // Byte 6
    uint8_t         :4;
    uint8_t Power6  :2;
    uint8_t         :2;
    // Byte 7
    uint8_t :8;
    // Byte 8
    uint8_t Powerful8 :8;
    // Byte 9
    uint8_t       :4;
    uint8_t Swing :3;
    uint8_t       :1;
    // Byte 10
    uint8_t             :1;
    uint8_t Powerful10  :3;
    uint8_t Display     :1;
    uint8_t             :2;
    uint8_t Clean10     :1;
    // Byte 11
    uint8_t Ion     :1;
    uint8_t Clean11 :1;
    uint8_t         :2;
    uint8_t Temp    :4;
    // Byte 12
    uint8_t       :1;
    uint8_t Fan   :3;
    uint8_t Mode  :3;
    uint8_t       :1;
    // Byte 13
    uint8_t       :1;
    uint8_t Beep  :1;
    uint8_t       :6;
  };
  struct {
    uint8_t :8;
    // Byte 1
    uint8_t       :4;
    uint8_t Sum1  :4;
    uint8_t pad1[6];
    // Byte 8
    uint8_t       :4;
    uint8_t Sum2  :4;
    uint8_t :8;
    // Byte 10
    uint8_t         :1;
    uint8_t Breeze  :3;  // WindFree
    uint8_t         :4;
  };
};

// Constants
const uint8_t kSamsungAcPowerfulMask8 = 0b01010000;
const uint8_t kSamsungAcSwingMove =                0b010;
const uint8_t kSamsungAcSwingStop =                0b111;
const uint8_t kSamsungAcPowerful10On =                     0b011;
const uint8_t kSamsungAcBreezeOn =                         0b101;
const uint8_t kSamsungAcMinTemp = 16;   // C   Mask 0b11110000
const uint8_t kSamsungAcMaxTemp = 30;   // C   Mask 0b11110000
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
  void stateReset(const bool forcepower = true, const bool initialPower = true);
#if SEND_SAMSUNG_AC
  void send(const uint16_t repeat = kSamsungAcDefaultRepeat,
            const bool calcchecksum = true);
  void sendExtended(const uint16_t repeat = kSamsungAcDefaultRepeat,
                    const bool calcchecksum = true);
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
  void setDisplay(const bool on);
  bool getDisplay(void) const;
  void setIon(const bool on);
  bool getIon(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kSamsungAcStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kSamsungAcStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
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
  bool _forcepower;  ///< Hack to know when we need to send a special power mesg
  bool _lastsentpowerstate;
  void checksum(const uint16_t length = kSamsungAcStateLength);
};

#endif  // IR_SAMSUNG_H_
