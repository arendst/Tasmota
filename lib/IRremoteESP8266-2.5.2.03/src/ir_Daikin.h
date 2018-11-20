// Copyright 2016 sillyfrog
// Copyright 2017 sillyfrog, crankyoldgit
#ifndef IR_DAIKIN_H_
#define IR_DAIKIN_H_

#ifndef UNIT_TEST
#include <Arduino.h>
#else
#include <string>
#endif
#include "IRrecv.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

// Option to disable the additional Daikin debug info to conserve memory
#define DAIKIN_DEBUG false

//                DDDDD     AAA   IIIII KK  KK IIIII NN   NN
//                DD  DD   AAAAA   III  KK KK   III  NNN  NN
//                DD   DD AA   AA  III  KKKK    III  NN N NN
//                DD   DD AAAAAAA  III  KK KK   III  NN  NNN
//                DDDDDD  AA   AA IIIII KK  KK IIIII NN   NN

/*
        Daikin AC map
        byte 5=Current time, mins past midnight, low bits
        byte 6
        b0-b3=Current time, mins past midnight, high bits
        byte 7= checksum of the first part (and last byte before a 29ms pause)
        byte 13=mode
                b7 = 0
                b6+b5+b4 = Mode
                        Modes: b6+b5+b4
                        011 = Cool
                        100 = Heat (temp 23)
                        110 = FAN (temp not shown, but 25)
                        000 = Fully Automatic (temp 25)
                        010 = DRY (temp 0xc0 = 96 degrees c)
                b3 = 1
                b2 = OFF timer set
                b1 = ON timer set
                b0 = Air Conditioner ON
        byte 14=temp*2   (Temp should be between 10 - 32)
        byte 16=Fan
                FAN control
                b7+b6+b5+b4 = Fan speed
                        Fan: b7+b6+b5+b4
                        0×3 = 1 bar
                        0×4 = 2 bar
                        0×5 = 3 bar
                        0×6 = 4 bar
                        0×7 = 5 bar
                        0xa = Auto
                        0xb = Quite
                b3+b2+b1+b0 = Swing control up/down
                        Swing control up/down:
                        0000 = Swing up/down off
                        1111 = Swing up/down on
        byte 17
                        Swing control left/right:
                        0000 = Swing left/right off
                        1111 = Swing left/right on
        byte 18=On timer mins past midnight, low bits
        byte 19
        b0-b3=On timer mins past midnight, high bits
        b4-b7=Off timer mins past midnight, low bits
        byte 20=Off timer mins past midnight, high bits
        byte 21=Aux  -> Powerful (bit 1), Silent (bit 5)
        byte 24=Aux2
        b1: Sensor
        b2: Econo mode
        b7: Intelligent eye on
        byte 25=Aux3
        b1: Mold Proof
        byte 26= checksum of the second part
*/

// Constants
const uint8_t kDaikinAuto = 0b000;
const uint8_t kDaikinDry = 0b010;
const uint8_t kDaikinCool = 0b011;
const uint8_t kDaikinHeat = 0b100;
const uint8_t kDaikinFan = 0b110;
const uint8_t kDaikinMinTemp = 10;  // Celsius
const uint8_t kDaikinMaxTemp = 32;  // Celsius
const uint8_t kDaikinFanMin = 1;
const uint8_t kDaikinFanMax = 5;
const uint8_t kDaikinFanAuto = 0b1010;
const uint8_t kDaikinFanQuiet = 0b1011;
const uint8_t kDaikinBytePower = 13;
const uint8_t kDaikinBitPower = 0b00000001;
const uint8_t kDaikinBytePowerful = 21;
const uint8_t kDaikinBitPowerful = 0b00000001;
const uint8_t kDaikinByteSilent = 21;
const uint8_t kDaikinBitSilent = 0b00100000;
const uint8_t kDaikinByteSensor = 24;
const uint8_t kDaikinBitSensor = 0b00000010;
const uint8_t kDaikinByteEcono = 24;
const uint8_t kDaikinBitEcono = 0b00000100;
const uint8_t kDaikinByteEye = 24;
const uint8_t kDaikinBitEye = 0b10000000;
const uint8_t kDaikinByteMold = 25;
const uint8_t kDaikinBitMold = 0b00000010;
const uint8_t kDaikinByteOffTimer = 13;
const uint8_t kDaikinBitOffTimer = 0b00000100;
const uint8_t kDaikinByteOnTimer = 13;
const uint8_t kDaikinBitOnTimer = 0b00000010;
const uint8_t kDaikinCurBit = kDaikinStateLength;
const uint8_t kDaikinCurIndex = kDaikinStateLength + 1;
const uint8_t kDaikinTolerance = 35;
const uint16_t kDaikinMarkExcess = kMarkExcess;
const uint16_t kDaikinHdrMark = 3650;   // kDaikinBitMark * 8
const uint16_t kDaikinHdrSpace = 1623;  // kDaikinBitMark * 4
const uint16_t kDaikinBitMark = 428;
const uint16_t kDaikinZeroSpace = 428;
const uint16_t kDaikinOneSpace = 1280;
const uint16_t kDaikinGap = 29000;
// Note bits in each octet swapped so can be sent as a single value
const uint64_t kDaikinFirstHeader64 =
    0b1101011100000000000000001100010100000000001001111101101000010001;

// Legacy defines.
#define DAIKIN_COOL kDaikinCool
#define DAIKIN_HEAT kDaikinHeat
#define DAIKIN_FAN kDaikinFan
#define DAIKIN_AUTO kDaikinAuto
#define DAIKIN_DRY kDaikinDry
#define DAIKIN_MIN_TEMP kDaikinMinTemp
#define DAIKIN_MAX_TEMP kDaikinMaxTemp
#define DAIKIN_FAN_MIN kDaikinFanMin
#define DAIKIN_FAN_MAX kDaikinFanMax
#define DAIKIN_FAN_AUTO kDaikinFanAuto
#define DAIKIN_FAN_QUIET kDaikinFanQuiet

class IRDaikinESP {
 public:
  explicit IRDaikinESP(uint16_t pin);

#if SEND_DAIKIN
  void send();
#endif
  void begin();
  void on();
  void off();
  void setPower(bool state);
  bool getPower();
  void setTemp(uint8_t temp);
  uint8_t getTemp();
  void setFan(uint8_t fan);
  uint8_t getFan();
  uint8_t getMode();
  void setMode(uint8_t mode);
  void setSwingVertical(bool state);
  bool getSwingVertical();
  void setSwingHorizontal(bool state);
  bool getSwingHorizontal();
  bool getQuiet();
  void setQuiet(bool state);
  bool getPowerful();
  void setPowerful(bool state);
  void setSensor(bool state);
  bool getSensor();
  void setEcono(bool state);
  bool getEcono();
  void setEye(bool state);
  bool getEye();
  void setMold(bool state);
  bool getMold();
  void enableOnTimer(uint16_t starttime);
  void disableOnTimer();
  uint16_t getOnTime();
  bool getOnTimerEnabled();
  void enableOffTimer(uint16_t endtime);
  void disableOffTimer();
  uint16_t getOffTime();
  bool getOffTimerEnabled();
  void setCurrentTime(uint16_t time);
  uint16_t getCurrentTime();
  uint8_t* getRaw();
  void setRaw(uint8_t new_code[]);
#if DAIKIN_DEBUG
  void printState();
#endif  // DAIKIN_DEBUG
  uint32_t getCommand();
  void setCommand(uint32_t value);
  static bool validChecksum(const uint8_t state[],
                            const uint16_t length = kDaikinStateLength);
#ifdef ARDUINO
  String toString();
  static String renderTime(uint16_t timemins);
#else
  std::string toString();
  static std::string renderTime(uint16_t timemins);
#endif

 private:
  // # of bytes per command
  uint8_t daikin[kDaikinStateLength];
  void stateReset();
  static uint8_t calcBlockChecksum(const uint8_t* block, const uint16_t length);
  void checksum();
  void setBit(uint8_t byte, uint8_t bitmask);
  void clearBit(uint8_t byte, uint8_t bitmask);
  uint8_t getBit(uint8_t byte, uint8_t bitmask);
  IRsend _irsend;
};

#endif  // IR_DAIKIN_H_
