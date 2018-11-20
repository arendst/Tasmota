/* Copyright 2017 Schmolders
// Adds support for Argo Ulisse 13 DCI Mobile Split ACs.
*/
#ifndef IR_ARGO_H_
#define IR_ARGO_H_

#include "IRremoteESP8266.h"
#include "IRsend.h"

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

const uint8_t kArgoCoolOn = 0;  // 0b000
const uint8_t kArgoCoolOff = 3;  // 0b110
const uint8_t kArgoCoolAuto = 2;  // 0b010
const uint8_t kArgoCoolHum = 1;  // 0b100
const uint8_t kArgoHeatOn = 0;  // 0b001
const uint8_t kArgoHeatAuto = 1;  // 0b101
const uint8_t kArgoHeatBlink = 2;  // 0b011  // ??no idea what mode that is
const uint8_t kArgoMinTemp = 10;  // Celsius offset +4
const uint8_t kArgoMaxTemp = 32;  // Celsius
const uint8_t kArgoFanAuto = 0;  // 0b00
const uint8_t kArgoFan3 = 3;  // 0b11
const uint8_t kArgoFan2 = 2;  // 0b01
const uint8_t kArgoFan1 = 1;  // 0b10
const uint8_t kArgoFlapAuto = 0;  // 0b000
const uint8_t kArgoFlap1 = 1;  // 0b100
const uint8_t kArgoFlap2 = 2;  // 0b010
const uint8_t kArgoFlap3 = 3;  // 0b110
const uint8_t kArgoFlap4 = 4;  // 0b001
const uint8_t kArgoFlap5 = 5;  // 0b101
const uint8_t kArgoFlap6 = 6;  // 0b011
const uint8_t kArgoFlapFull = 7;  // 0b111

// Legacy defines. (Deperecated)
#define ARGO_COOL_ON              kArgoCoolOn
#define ARGO_COOL_OFF             kArgoCoolOff
#define ARGO_COOL_AUTO            kArgoCoolAuto
#define ARGO_COOl_HUM             kArgoCoolHum
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
  explicit IRArgoAC(uint16_t pin);

#if SEND_ARGO
  void send();
#endif  // SEND_ARGO
  void begin();
  void on();
  void off();

  void setPower(bool state);
  uint8_t getPower();

  void setTemp(uint8_t temp);
  uint8_t getTemp();

  void setFan(uint8_t fan);
  uint8_t getFan();

  void setFlap(uint8_t flap);
  uint8_t getFlap();

  void setCoolMode(uint8_t mode);
  uint8_t getCoolMode();

  void setHeatMode(uint8_t mode);
  uint8_t getHeatMode();
  uint8_t getMode();

  void setMax(bool state);
  bool getMax();

  void setNight(bool state);
  bool getNight();

  void setiFeel(bool state);
  bool getiFeel();

  void setTime();
  void setRoomTemp(uint8_t temp);

  uint8_t* getRaw();

 private:
  // # of bytes per command
  uint8_t argo[kArgoStateLength];  // Defined in IRremoteESP8266.h
  void stateReset();
  void checksum();
  IRsend _irsend;  // instance of the IR send class

  // Attributes
  uint8_t set_temp;
  uint8_t fan_mode;
  uint8_t flap_mode;
  uint8_t ac_state;
  uint8_t ac_mode;  // heat 1, cool 0
  uint8_t heat_mode;
  uint8_t cool_mode;
  uint8_t night_mode;  // on/off
  uint8_t max_mode;  // on/off
  uint8_t ifeel_mode;  // on/off
};

#endif  // IR_ARGO_H_
