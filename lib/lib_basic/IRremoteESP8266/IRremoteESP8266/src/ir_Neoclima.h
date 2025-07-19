// Copyright 2019 David Conran

/// @file
/// @brief Support for Neoclima protocols.
/// Analysis by crankyoldgit & AndreyShpilevoy
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/764
/// @see https://drive.google.com/file/d/1kjYk4zS9NQcMQhFkak-L4mp4UuaAIesW/view

// Supports:
//   Brand: Neoclima,  Model: NS-09AHTI A/C
//   Brand: Neoclima,  Model: ZH/TY-01 remote
//   Brand: Soleus Air,  Model: TTWM1-10-01 A/C
//   Brand: Soleus Air,  Model: ZCF/TL-05 remote

#ifndef IR_NEOCLIMA_H_
#define IR_NEOCLIMA_H_

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

/// Native representation of a Neoclima A/C message.
union NeoclimaProtocol {
  uint8_t raw[kNeoclimaStateLength];  ///< State of the remote in code.
  struct {
    // Byte 0
    uint8_t       :8;
    // Byte 1
    uint8_t       :1;
    uint8_t CHeat :1;
    uint8_t Ion   :1;
    uint8_t       :5;
    // Byte 2
    uint8_t       :8;
    // Byte 3
    uint8_t Light :1;
    uint8_t       :1;
    uint8_t Hold  :1;
    uint8_t Turbo :1;
    uint8_t Econo :1;
    uint8_t       :1;
    uint8_t Eye   :1;
    uint8_t       :1;
    // Byte 4
    uint8_t       :8;
    // Byte 5
    uint8_t Button  :5;
    uint8_t         :2;
    uint8_t Fresh   :1;
    // Byte 6
    uint8_t         :8;
    // Byte 7
    uint8_t Sleep   :1;
    uint8_t Power   :1;
    uint8_t SwingV  :2;
    uint8_t SwingH  :1;
    uint8_t Fan     :2;
    uint8_t UseFah  :1;
    // Byte 8
    uint8_t Follow  :8;
    // Byte 9
    uint8_t Temp  :5;
    uint8_t Mode  :3;
    // Byte 10
    uint8_t     :8;
    // Byte 11
    uint8_t Sum :8;
  };
};

// Constants

const uint8_t kNeoclimaButtonPower =    0x00;
const uint8_t kNeoclimaButtonMode =     0x01;
const uint8_t kNeoclimaButtonTempUp =   0x02;
const uint8_t kNeoclimaButtonTempDown = 0x03;
const uint8_t kNeoclimaButtonSwing =    0x04;
const uint8_t kNeoclimaButtonFanSpeed = 0x05;
const uint8_t kNeoclimaButtonAirFlow =  0x07;
const uint8_t kNeoclimaButtonHold =     0x08;
const uint8_t kNeoclimaButtonSleep =    0x09;
const uint8_t kNeoclimaButtonTurbo =    0x0A;
const uint8_t kNeoclimaButtonLight =    0x0B;
const uint8_t kNeoclimaButtonEcono =    0x0D;
const uint8_t kNeoclimaButtonEye =      0x0E;
const uint8_t kNeoclimaButtonFollow =   0x13;
const uint8_t kNeoclimaButtonIon =      0x14;
const uint8_t kNeoclimaButtonFresh =    0x15;
const uint8_t kNeoclimaButton8CHeat =   0x1D;
const uint8_t kNeoclimaButtonTempUnit = 0x1E;

const uint8_t kNeoclimaSwingVOn =                            0b01;
const uint8_t kNeoclimaSwingVOff =                           0b10;
const uint8_t kNeoclimaFanAuto =                          0b00;
const uint8_t kNeoclimaFanHigh =                          0b01;
const uint8_t kNeoclimaFanMed =                           0b10;
const uint8_t kNeoclimaFanLow =                           0b11;

const uint8_t kNeoclimaFollowMe = 0x5D;  // Also 0x5F

const uint8_t kNeoclimaMinTempC = 16;   // 16C
const uint8_t kNeoclimaMaxTempC = 32;   // 32C
const uint8_t kNeoclimaMinTempF = 61;   // 61F
const uint8_t kNeoclimaMaxTempF = 90;   // 90F
const uint8_t kNeoclimaAuto =     0b000;
const uint8_t kNeoclimaCool =     0b001;
const uint8_t kNeoclimaDry =      0b010;
const uint8_t kNeoclimaFan =      0b011;
const uint8_t kNeoclimaHeat =     0b100;

// Classes
/// Class for handling detailed Neoclima A/C messages.
class IRNeoclimaAc {
 public:
  explicit IRNeoclimaAc(const uint16_t pin, const bool inverted = false,
                        const bool use_modulation = true);
  void stateReset(void);
#if SEND_NEOCLIMA
  void send(const uint16_t repeat = kNeoclimaMinRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_NEOCLIMA
  void begin(void);
  void setButton(const uint8_t button);
  uint8_t getButton(void) const;
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setTemp(const uint8_t temp, const bool celsius = true);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed);
  uint8_t getFan(void) const;
  void setSwingV(const bool on);
  bool getSwingV(void) const;
  void setSwingH(const bool on);
  bool getSwingH(void) const;
  void setSleep(const bool on);
  bool getSleep(void) const;
  void setTurbo(const bool on);
  bool getTurbo(void) const;
  void setEcono(const bool on);
  bool getEcono(void) const;
  void setFresh(const bool on);
  bool getFresh(void) const;
  void setHold(const bool on);
  bool getHold(void) const;
  void setIon(const bool on);
  bool getIon(void) const;
  void setLight(const bool on);
  bool getLight(void) const;
  void set8CHeat(const bool on);
  bool get8CHeat(void) const;
  void setEye(const bool on);
  bool getEye(void) const;
  bool getTempUnits(void) const;
  // DISABLED: See TODO in ir_Neoclima.cpp
  // void setFollow(const bool on);
  bool getFollow(void) const;
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kNeoclimaStateLength);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kNeoclimaStateLength);
  static uint8_t calcChecksum(const uint8_t state[],
                              const uint16_t length = kNeoclimaStateLength);
  String toString(void) const;
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else  // UNIT_TEST
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif  // UNIT_TEST
  NeoclimaProtocol _;
  void checksum(const uint16_t length = kNeoclimaStateLength);
};

#endif  // IR_NEOCLIMA_H_
