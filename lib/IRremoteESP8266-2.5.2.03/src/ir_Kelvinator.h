// Kelvinator A/C
//
// Copyright 2016 David Conran

#ifndef IR_KELVINATOR_H_
#define IR_KELVINATOR_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRremoteESP8266.h"
#include "IRsend.h"

// KK  KK EEEEEEE LL     VV     VV IIIII NN   NN   AAA   TTTTTTT  OOOOO  RRRRRR
// KK KK  EE      LL     VV     VV  III  NNN  NN  AAAAA    TTT   OO   OO RR   RR
// KKKK   EEEEE   LL      VV   VV   III  NN N NN AA   AA   TTT   OO   OO RRRRRR
// KK KK  EE      LL       VV VV    III  NN  NNN AAAAAAA   TTT   OO   OO RR  RR
// KK  KK EEEEEEE LLLLLLL   VVV    IIIII NN   NN AA   AA   TTT    OOOO0  RR   RR

// Constants
const uint8_t kKelvinatorAuto = 0;
const uint8_t kKelvinatorCool = 1;
const uint8_t kKelvinatorDry = 2;
const uint8_t kKelvinatorFan = 3;
const uint8_t kKelvinatorHeat = 4;
const uint8_t kKelvinatorBasicFanMax = 3;
const uint8_t kKelvinatorFanAuto = 0;
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

/*
        Kelvinator AC map

  (header mark and space)
  byte 0 = Basic Modes
    b2-0 = Modes
      Modes:
        000 = Auto (temp = 25C)
        001 = Cool
        010 = Dry (temp = 25C, but not shown)
        011 = Fan
        100 = Heat
    b3 = Power Status (1 = On, 0 = Off)
    b5-4 = Fan (Basic modes)
      Fan:
        00 = Auto
        01 = Fan 1
        10 = Fan 2
        11 = Fan 3 or higher (See byte 14)
    b6 = Vent swing (1 = On, 0 = Off) (See byte 4)
    b7 = Sleep Modes 1 & 3 (1 = On, 0 = Off)
  byte 1 = Temperature
    b3-0: Degrees C.
      0000 (0) = 16C
      0001 (1) = 17C
      0010 (2) = 18C
      ...
      1101 (13) = 29C
      1110 (14) = 30C
  byte 2 = Extras
    b3-0 = UNKNOWN, typically 0.
    b4 = Turbo Fan (1 = On, 0 = Off)
    b5 = Light (Display) (1 = On, 0 = Off)
    b6 = Ion Filter (1 = On, 0 = Off)
    b7 = X-Fan (Fan runs for a while after power off) (1 = On, 0 = Off)
  byte 3 = Section Indicator
    b3-0 = Unused (Typically 0)
    b5-4 = Unknown (possibly timer related) (Typically 0b01)
    b7-6 = End of command block (B01)
  (B010 marker and a gap of 20ms)
  byte 4 = Extended options
    b0 = Swing Vent Vertical (1 = On, 0 = Off)
    b4 = Swing Vent Horizontal (1 = On, 0 = Off)
  byte 5-6 = Timer related. Typically 0 except when timer in use.
  byte 7 = checksum
    b3-0 = Unknown (Used in Timer mode)
    b7-4 = checksum of the previous bytes (0-6)
  (gap of 40ms)
  (header mark and space)
  byte 8 = Repeat of byte 0
  byte 9 = Repeat of byte 1
  byte 10 = Repeat of byte 2
  byte 11 = Section Indicator
    b3-0 = Unused (Typically 0)
    b5-4 = Unknown (possibly timer related) (Typically 0b11)
    b7-6 = End of command block (B01)
  (B010 marker and a gap of 20ms)
  byte 12 = Extended options
    b0 = Sleep mode 2 (1 = On, 0=Off)
    b6-1 = Unknown (Used in Sleep Mode 3, Typically 0b000000)
    b7 = Quiet Mode (1 = On, 0=Off)
  byte 13 = Unknown (Sleep Mode 3 related, Typically 0x00)
  byte 14 = Fan control
    b3-0 = Unknown (Sleep Mode 3 related, Typically 0b0000)
    b6-4 = Fan speed
       0b000 (0) = Automatic
       0b001 (1) = Fan 1
       0b010 (2) = Fan 2
       0b011 (3) = Fan 3
       0b100 (4) = Fan 4
       0b101 (5) = Fan 5
  byte 15 = checksum
    b3-0 = Unknown (Typically 0b0000)
    b7-4 = checksum of the previous bytes (8-14)
*/

// Classes
class IRKelvinatorAC {
 public:
  explicit IRKelvinatorAC(uint16_t pin);

  void stateReset();
#if SEND_KELVINATOR
  void send();
#endif  // SEND_KELVINATOR
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFan(uint8_t fan);
  uint8_t getFan();
  void setMode(uint8_t mode);
  uint8_t getMode();
  void setSwingVertical(bool state);
  bool getSwingVertical();
  void setSwingHorizontal(bool state);
  bool getSwingHorizontal();
  void setQuiet(bool state);
  bool getQuiet();
  void setIonFilter(bool state);
  bool getIonFilter();
  void setLight(bool state);
  bool getLight();
  void setXFan(bool state);
  bool getXFan();
  void setTurbo(bool state);
  bool getTurbo();
  uint8_t* getRaw();
  void setRaw(uint8_t new_code[]);
  static uint8_t calcBlockChecksum(
      const uint8_t* block, const uint16_t length = kKelvinatorStateLength / 2);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kKelvinatorStateLength);
#ifdef ARDUINO
  String toString();
#else
  std::string toString();
#endif

 private:
  // The state of the IR remote in IR code form.
  uint8_t remote_state[kKelvinatorStateLength];
  void checksum(const uint16_t length = kKelvinatorStateLength);
  void fixup();
  IRsend _irsend;
};

#endif  // IR_KELVINATOR_H_
