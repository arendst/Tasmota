// Copyright 2016 David Conran
/// @file
/// @brief Support for Kelvinator A/C protocols.

// Supports:
//   Brand: Kelvinator,  Model: YALIF Remote
//   Brand: Kelvinator,  Model: KSV26CRC A/C
//   Brand: Kelvinator,  Model: KSV26HRC A/C
//   Brand: Kelvinator,  Model: KSV35CRC A/C
//   Brand: Kelvinator,  Model: KSV35HRC A/C
//   Brand: Kelvinator,  Model: KSV53HRC A/C
//   Brand: Kelvinator,  Model: KSV62HRC A/C
//   Brand: Kelvinator,  Model: KSV70CRC A/C
//   Brand: Kelvinator,  Model: KSV70HRC A/C
//   Brand: Kelvinator,  Model: KSV80HRC A/C
//   Brand: Green,  Model: YAPOF3 remote
//   Brand: Sharp,  Model: YB1FA remote

#ifndef IR_KELVINATOR_H_
#define IR_KELVINATOR_H_

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

/// Native representation of a Kelvinator A/C message.
union KelvinatorProtocol{
  uint8_t raw[kKelvinatorStateLength];  ///< The state in IR code form.
  struct {
    // Byte 0
    uint8_t Mode      :3;
    uint8_t Power     :1;
    uint8_t BasicFan  :2;
    uint8_t VentSwing :1;
    uint8_t           :1;  // Sleep Modes 1 & 3 (1 = On, 0 = Off)
    // Byte 1
    uint8_t Temp  :4;  // Degrees C.
    uint8_t       :4;
    // Byte 2
    uint8_t           :4;
    uint8_t Turbo     :1;
    uint8_t Light     :1;
    uint8_t IonFilter :1;
    uint8_t XFan      :1;
    // Byte 3
    uint8_t :4;
    uint8_t :2;  // (possibly timer related) (Typically 0b01)
    uint8_t :2;  // End of command block (B01)
    // (B010 marker and a gap of 20ms)
    // Byte 4
    uint8_t SwingV  :1;
    uint8_t         :3;
    uint8_t SwingH  :1;
    uint8_t         :3;
    // Byte 5~6
    uint8_t pad0[2];  // Timer related. Typically 0 except when timer in use.
    // Byte 7
    uint8_t       :4;  // (Used in Timer mode)
    uint8_t Sum1  :4;  // checksum of the previous bytes (0-6)
    // (gap of 40ms)
    // (header mark and space)
    // Byte 8~10
    uint8_t pad1[3];  // Repeat of byte 0~2
    // Byte 11
    uint8_t :4;
    uint8_t :2;  // (possibly timer related) (Typically 0b11)
    uint8_t :2;  // End of command block (B01)
    // (B010 marker and a gap of 20ms)
    // Byte 12
    uint8_t       :1;  // Sleep mode 2 (1 = On, 0=Off)
    uint8_t       :6;  // (Used in Sleep Mode 3, Typically 0b000000)
    uint8_t Quiet :1;
    // Byte 13
    uint8_t     :8;  // (Sleep Mode 3 related, Typically 0x00)
    // Byte 14
    uint8_t     :4;  // (Sleep Mode 3 related, Typically 0b0000)
    uint8_t Fan :3;
    // Byte 15
    uint8_t       :4;
    uint8_t Sum2  :4;  // checksum of the previous bytes (8-14)
  };
};

// Constants
const uint8_t kKelvinatorAuto = 0;  // (temp = 25C)
const uint8_t kKelvinatorCool = 1;
const uint8_t kKelvinatorDry = 2;  // (temp = 25C, but not shown)
const uint8_t kKelvinatorFan = 3;
const uint8_t kKelvinatorHeat = 4;
const uint8_t kKelvinatorBasicFanMax = 3;
const uint8_t kKelvinatorFanAuto = 0;
const uint8_t kKelvinatorFanMin = 1;
const uint8_t kKelvinatorFanMax = 5;
const uint8_t kKelvinatorMinTemp = 16;   // 16C
const uint8_t kKelvinatorMaxTemp = 30;   // 30C
const uint8_t kKelvinatorAutoTemp = 25;  // 25C

// Legacy defines (Deprecated)
#define KELVINATOR_MIN_TEMP kKelvinatorMinTemp
#define KELVINATOR_MAX_TEMP kKelvinatorMaxTemp
#define KELVINATOR_HEAT kKelvinatorHeat
#define KELVINATOR_FAN_MAX kKelvinatorFanMax
#define KELVINATOR_FAN_AUTO kKelvinatorFanAuto
#define KELVINATOR_FAN kKelvinatorFan
#define KELVINATOR_DRY kKelvinatorDry
#define KELVINATOR_COOL kKelvinatorCool
#define KELVINATOR_BASIC_FAN_MAX kKelvinatorBasicFanMax
#define KELVINATOR_AUTO_TEMP kKelvinatorAutoTemp
#define KELVINATOR_AUTO kKelvinatorAuto

// Classes
/// Class for handling detailed Kelvinator A/C messages.
class IRKelvinatorAC {
 public:
  explicit IRKelvinatorAC(const uint16_t pin, const bool inverted = false,
                          const bool use_modulation = true);
  void stateReset(void);
#if SEND_KELVINATOR
  void send(const uint16_t repeat = kKelvinatorDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_KELVINATOR
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
  void setSwingVertical(const bool on);
  bool getSwingVertical(void) const;
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void) const;
  void setQuiet(const bool on);
  bool getQuiet(void) const;
  void setIonFilter(const bool on);
  bool getIonFilter(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  void setXFan(const bool on);
  bool getXFan(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[]);
  static uint8_t calcBlockChecksum(
      const uint8_t* block, const uint16_t length = kKelvinatorStateLength / 2);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kKelvinatorStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
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
  KelvinatorProtocol _;
  void checksum(void);
  void fixup(void);
};

#endif  // IR_KELVINATOR_H_
