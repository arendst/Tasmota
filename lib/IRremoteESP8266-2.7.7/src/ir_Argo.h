// Copyright 2017 Schmolders
// Adds support for Argo Ulisse 13 DCI Mobile Split ACs.

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

/*
	Protocol Description:
  All in LSB first as it is sent. argo message array will be stored MSB first!
  do LSB-MSB conversion in sendData
  Byte 0: const 0	0	1	1	0	1	0	1
  Byte 1: const 1	0	1	0	1	1	1	1
  Byte 2: 0 0 0, 3bit Cool/Heat Mode, 2bit start SetTemp LSB first
  Byte 3: 3bit End SetTemp, 2bit Fan Mode, 3bit RoomTemp LSB first
  Byte 4: 2bit RoomTemp, 3bit Flap Mode, 3bit OnTimer
  Byte 5: 8bit OnTimer
  Byte 6: 8Bit OffTimer
  Byte 7: 3bit OffTimer, 5bit Time
  Byte 8: 6bit Time, 1bit Timer On/Off, 1bit Timer Program
  Byte 9: 1bit Timer Program, 1bit Timer 1h, 1 bit Night Mode, 1bit Max Mode, 1bit Filter, 1bit on/off, 1bit const 0, 1bit iFeel
  Byte 10: 2bit const 0 1, 6bit Checksum
  Byte 11: 2bit Checksum
*/

// Constants. Store MSB left.

// byte[2]
const uint8_t kArgoHeatBit =      0b00100000;
//            kArgoTempLowMask =  0b11000000;
const uint8_t kArgoTempLowOffset = 5;
const uint8_t kArgoTempLowSize = 2;

// Mode                           0b00111000
const uint8_t kArgoModeOffset = 3;
const uint8_t kArgoModeSize = 3;
const uint8_t kArgoCool =           0b000;
const uint8_t kArgoDry =            0b001;
const uint8_t kArgoAuto =           0b010;
const uint8_t kArgoOff =            0b011;
const uint8_t kArgoHeat =           0b100;
const uint8_t kArgoHeatAuto =       0b101;
// ?no idea what mode that is
const uint8_t kArgoHeatBlink =      0b110;

// byte[3]
//            kArgoTempHighMask =    0b00000111;
const uint8_t kArgoTempHighOffset = 0;
const uint8_t kArgoTempHighSize = 3;
// Fan                               0b00011000
const uint8_t kArgoFanOffset = 3;
const uint8_t kArgoFanSize = 2;
const uint8_t kArgoFanAuto = 0;      // 0b00
const uint8_t kArgoFan1 = 1;         // 0b01
const uint8_t kArgoFan2 = 2;         // 0b10
const uint8_t kArgoFan3 = 3;         // 0b11
//            kArgoRoomTempLowMask = 0b11100000;
const uint8_t kArgoRoomTempLowOffset = 5;
const uint8_t kArgoRoomTempLowSize = 3;

// byte[4]
//            kArgoRoomTempHighMask = 0b00000011;
const uint8_t kArgoRoomTempHighOffset = 0;
const uint8_t kArgoRoomTempHighSize = 2;

const uint8_t kArgoTempDelta = 4;
const uint8_t kArgoMaxRoomTemp =
    ((1 << (kArgoRoomTempHighSize + kArgoRoomTempLowSize)) - 1) +
    kArgoTempDelta;  // 35C

// byte[9]
const uint8_t kArgoNightBitOffset = 2;
const uint8_t kArgoMaxBitOffset = 3;
const uint8_t kArgoPowerBitOffset = 5;
const uint8_t kArgoIFeelBitOffset = 7;

const uint8_t kArgoMinTemp = 10;  // Celsius delta +4
const uint8_t kArgoMaxTemp = 32;  // Celsius

const uint8_t kArgoFlapAuto = 0;
const uint8_t kArgoFlap1 = 1;
const uint8_t kArgoFlap2 = 2;
const uint8_t kArgoFlap3 = 3;
const uint8_t kArgoFlap4 = 4;
const uint8_t kArgoFlap5 = 5;
const uint8_t kArgoFlap6 = 6;
const uint8_t kArgoFlapFull = 7;

// Legacy defines. (Deperecated)
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


class IRArgoAC {
 public:
  explicit IRArgoAC(const uint16_t pin, const bool inverted = false,
                    const bool use_modulation = true);

#if SEND_ARGO
  void send(const uint16_t repeat = kArgoDefaultRepeat);
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_ARGO
  void begin(void);
  void on(void);
  void off(void);

  void setPower(const bool on);
  bool getPower(void);

  void setTemp(const uint8_t degrees);
  uint8_t getTemp(void);

  void setFan(const uint8_t fan);
  uint8_t getFan(void);

  void setFlap(const uint8_t flap);
  uint8_t getFlap(void);

  void setMode(const uint8_t mode);
  uint8_t getMode(void);

  void setMax(const bool on);
  bool getMax(void);

  void setNight(const bool on);
  bool getNight(void);

  void setiFeel(const bool on);
  bool getiFeel(void);

  void setTime(void);
  void setRoomTemp(const uint8_t degrees);
  uint8_t getRoomTemp(void);

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
  stdAc::state_t toCommon(void);
  String toString();
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  // instance of the IR send class
#else
  IRsendTest _irsend;  // instance of the testing IR send class
#endif
  // # of bytes per command
  uint8_t argo[kArgoStateLength];  // Defined in IRremoteESP8266.h
  void stateReset(void);
  void checksum(void);

  // Attributes
  uint8_t flap_mode;
  uint8_t heat_mode;
  uint8_t cool_mode;
};

#endif  // IR_ARGO_H_
