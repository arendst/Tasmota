// Copyright 2016 sillyfrog
// Copyright 2017 sillyfrog, crankyoldgit
// Copyright 2018-2019 crankyoldgit
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
#ifdef UNIT_TEST
#include "IRsend_test.h"
#endif

//                DDDDD     AAA   IIIII KK  KK IIIII NN   NN
//                DD  DD   AAAAA   III  KK KK   III  NNN  NN
//                DD   DD AA   AA  III  KKKK    III  NN N NN
//                DD   DD AAAAAAA  III  KK KK   III  NN  NNN
//                DDDDDD  AA   AA IIIII KK  KK IIIII NN   NN

/*
        Daikin AC map
        byte 6=
          b4:Comfort
        byte 7= checksum of the first part (and last byte before a 29ms pause)
        byte 13=Current time, mins past midnight, low bits
        byte 14
        b0-b3=Current time, mins past midnight, high bits
        byte 15= checksum of the second part (and last byte before a 29ms pause)
        byte 21=mode
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
        byte 22=temp*2   (Temp should be between 10 - 32)
        byte 24=Fan
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
        byte 25
                        Swing control left/right:
                        0000 = Swing left/right off
                        1111 = Swing left/right on
        byte 26=On timer mins past midnight, low bits
        byte 27
        b0-b3=On timer mins past midnight, high bits
        b4-b7=Off timer mins past midnight, low bits
        byte 28=Off timer mins past midnight, high bits
        byte 29=Aux  -> Powerful (bit 1), Silent (bit 5)
        byte 32=Aux2
        b1: Sensor
        b2: Econo mode
        b7: Intelligent eye on
        byte 33=Aux3
        b1: Mold Proof
        byte 34= checksum of the third part
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
const uint16_t kDaikinHeaderLength = 5;
const uint8_t kDaikinSections = 3;
const uint8_t kDaikinSection1Length = 8;
const uint8_t kDaikinSection2Length = 8;
const uint8_t kDaikinSection3Length =
    kDaikinStateLength - kDaikinSection1Length - kDaikinSection2Length;
const uint8_t kDaikinByteComfort = 6;
const uint8_t kDaikinByteChecksum1 = 7;
const uint8_t kDaikinBitComfort = 0b00010000;
const uint8_t kDaikinByteClockMinsLow = 13;
const uint8_t kDaikinByteClockMinsHigh = 14;
const uint8_t kDaikinByteChecksum2 = 15;
const uint8_t kDaikinBytePower = 21;
const uint8_t kDaikinBitPower = 0b00000001;
const uint8_t kDaikinByteTemp = 22;
const uint8_t kDaikinByteFan = 24;
const uint8_t kDaikinByteSwingH = 25;
const uint8_t kDaikinByteOnTimerMinsLow = 26;
const uint8_t kDaikinByteOnTimerMinsHigh = 27;
const uint8_t kDaikinByteOffTimerMinsLow = kDaikinByteOnTimerMinsHigh;
const uint8_t kDaikinByteOffTimerMinsHigh = 28;
const uint8_t kDaikinBytePowerful = 29;
const uint8_t kDaikinBitPowerful = 0b00000001;
const uint8_t kDaikinByteSilent = kDaikinBytePowerful;
const uint8_t kDaikinBitSilent = 0b00100000;
const uint8_t kDaikinByteSensor = 32;
const uint8_t kDaikinBitSensor = 0b00000010;
const uint8_t kDaikinByteEcono = kDaikinByteSensor;
const uint8_t kDaikinBitEcono = 0b00000100;
const uint8_t kDaikinByteEye = kDaikinByteSensor;
const uint8_t kDaikinBitEye = 0b10000000;
const uint8_t kDaikinByteMold = 33;
const uint8_t kDaikinBitMold = 0b00000010;
const uint8_t kDaikinByteOffTimer = kDaikinBytePower;
const uint8_t kDaikinBitOffTimer = 0b00000100;
const uint8_t kDaikinByteOnTimer = kDaikinByteOffTimer;
const uint8_t kDaikinBitOnTimer = 0b00000010;
const uint8_t kDaikinByteChecksum3 = kDaikinStateLength - 1;
const uint16_t kDaikinUnusedTime = 0x600;
const uint8_t kDaikinBeepQuiet = 1;
const uint8_t kDaikinBeepLoud = 2;
const uint8_t kDaikinBeepOff = 3;
const uint8_t kDaikinLightBright = 1;
const uint8_t kDaikinLightDim = 2;
const uint8_t kDaikinLightOff = 3;
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

// Another variant of the protocol for the Daikin ARC477A1 remote.
const uint16_t kDaikin2Freq = 36700;  // Modulation Frequency in Hz.
const uint16_t kDaikin2LeaderMark = 10024;
const uint16_t kDaikin2LeaderSpace = 25180;
const uint16_t kDaikin2Gap = kDaikin2LeaderMark + kDaikin2LeaderSpace;
const uint16_t kDaikin2HdrMark = 3500;
const uint16_t kDaikin2HdrSpace = 1728;
const uint16_t kDaikin2BitMark = 460;
const uint16_t kDaikin2OneSpace = 1270;
const uint16_t kDaikin2ZeroSpace = 420;
const uint16_t kDaikin2Sections = 2;
const uint16_t kDaikin2Section1Length = 20;
const uint16_t kDaikin2Section2Length = 19;
const uint8_t kDaikin2Tolerance = kTolerance + 5;

const uint8_t kDaikin2BitSleepTimer = 0b00100000;
const uint8_t kDaikin2BitPurify = 0b00010000;
const uint8_t kDaikin2BitEye = 0b00000010;
const uint8_t kDaikin2BitEyeAuto = 0b10000000;
const uint8_t kDaikin2BitMold = 0b00001000;
const uint8_t kDaikin2BitClean = 0b00100000;
const uint8_t kDaikin2BitFreshAir = 0b00000001;
const uint8_t kDaikin2BitFreshAirHigh = 0b10000000;
const uint8_t kDaikin2BitPower = 0b10000000;
const uint8_t kDaikin2LightMask = 0b00110000;
const uint8_t kDaikin2BeepMask = 0b11000000;
const uint8_t kDaikin2SwingVHigh = 0x1;
const uint8_t kDaikin2SwingVLow = 0x6;
const uint8_t kDaikin2SwingVBreeze = 0xC;
const uint8_t kDaikin2SwingVCirculate = 0xD;
const uint8_t kDaikin2SwingVAuto = 0xE;
const uint8_t kDaikin2SwingHAuto = 0xBE;
const uint8_t kDaikin2SwingHSwing = 0xBF;
const uint8_t kDaikin2MinCoolTemp = 18;  // Min temp (in C) when in Cool mode.

// Another variant of the protocol for the Daikin ARC433B69 remote.
const uint16_t kDaikin216Freq = 38000;  // Modulation Frequency in Hz.
const uint16_t kDaikin216HdrMark = 3400;
const uint16_t kDaikin216HdrSpace = 1800;
const uint16_t kDaikin216BitMark = 380;
const uint16_t kDaikin216OneSpace = 1350;
const uint16_t kDaikin216ZeroSpace = 480;
const uint16_t kDaikin216Gap = 29650;
const uint16_t kDaikin216Sections = 2;
const uint16_t kDaikin216Section1Length = 8;
const uint16_t kDaikin216Section2Length = kDaikin216StateLength -
                                          kDaikin216Section1Length;
const uint8_t kDaikin216BytePower = 13;
const uint8_t kDaikin216ByteMode = kDaikin216BytePower;
const uint8_t kDaikin216MaskMode = 0b01110000;
const uint8_t kDaikin216ByteTemp = 14;
const uint8_t kDaikin216MaskTemp = 0b01111110;
const uint8_t kDaikin216ByteFan = 16;
const uint8_t kDaikin216MaskFan = 0b11110000;
const uint8_t kDaikin216ByteSwingV = 16;
const uint8_t kDaikin216MaskSwingV = 0b00001111;
const uint8_t kDaikin216ByteSwingH = 17;
const uint8_t kDaikin216MaskSwingH = kDaikin216MaskSwingV;


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
  void send(const uint16_t repeat = kDaikinDefaultRepeat);
#endif
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  bool getQuiet(void);
  void setQuiet(const bool on);
  bool getPowerful(void);
  void setPowerful(const bool on);
  void setSensor(const bool on);
  bool getSensor(void);
  void setEcono(const bool on);
  bool getEcono(void);
  void setEye(const bool on);
  bool getEye(void);
  void setMold(const bool on);
  bool getMold(void);
  void setComfort(const bool on);
  bool getComfort(void);
  void enableOnTimer(const uint16_t starttime);
  void disableOnTimer(void);
  uint16_t getOnTime(void);
  bool getOnTimerEnabled();
  void enableOffTimer(const uint16_t endtime);
  void disableOffTimer(void);
  uint16_t getOffTime(void);
  bool getOffTimerEnabled(void);
  void setCurrentTime(const uint16_t mins_since_midnight);
  uint16_t getCurrentTime(void);
  uint8_t* getRaw(void);
  void setRaw(const uint8_t new_code[],
              const uint16_t length = kDaikinStateLength);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikinStateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
#ifdef ARDUINO
  String toString(void);
  static String renderTime(const uint16_t timemins);
#else
  std::string toString(void);
  static std::string renderTime(const uint16_t timemins);
#endif
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // # of bytes per command
  uint8_t remote[kDaikinStateLength];
  void stateReset(void);
  void checksum(void);
};

// Class to emulate a Daikin ARC477A1 remote.
class IRDaikin2 {
 public:
  explicit IRDaikin2(uint16_t pin);

#if SEND_DAIKIN2
  void send(const uint16_t repeat = kDaikin2DefaultRepeat);
#endif
  void begin();
  void on();
  void off();
  void setPower(const bool state);
  bool getPower();
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setFan(const uint8_t fan);
  uint8_t getFan();
  uint8_t getMode();
  void setMode(const uint8_t mode);
  void setSwingVertical(const uint8_t position);
  uint8_t getSwingVertical();
  void setSwingHorizontal(const uint8_t position);
  uint8_t getSwingHorizontal();
  bool getQuiet();
  void setQuiet(const bool on);
  bool getPowerful();
  void setPowerful(const bool on);
  void setSensor(const bool on);
  bool getSensor();
  void setEcono(const bool on);
  bool getEcono();
  void setEye(const bool on);
  bool getEye();
  void setEyeAuto(const bool on);
  bool getEyeAuto();
  void setPurify(const bool on);
  bool getPurify();
  void setMold(const bool on);
  bool getMold();
  void enableOnTimer(const uint16_t starttime);
  void disableOnTimer();
  uint16_t getOnTime();
  bool getOnTimerEnabled();
  void enableSleepTimer(const uint16_t sleeptime);
  void disableSleepTimer();
  uint16_t getSleepTime();
  bool getSleepTimerEnabled();
  void enableOffTimer(const uint16_t endtime);
  void disableOffTimer();
  uint16_t getOffTime();
  bool getOffTimerEnabled();
  void setCurrentTime(const uint16_t time);
  uint16_t getCurrentTime();
  void setBeep(const uint8_t beep);
  uint8_t getBeep();
  void setLight(const uint8_t light);
  uint8_t getLight();
  void setClean(const bool on);
  bool getClean();
  void setFreshAir(const bool on);
  bool getFreshAir();
  void setFreshAirHigh(const bool on);
  bool getFreshAirHigh();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  uint32_t getCommand();
  void setCommand(uint32_t value);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin2StateLength);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  uint8_t convertSwingV(const stdAc::swingv_t position);
#ifdef ARDUINO
  String toString();
  static String renderTime(uint16_t timemins);
#else
  std::string toString();
  static std::string renderTime(uint16_t timemins);
#endif
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin2StateLength];
  void stateReset();
  void checksum();
  void clearOnTimerFlag();
  void clearSleepTimerFlag();
};

// Class to emulate a Daikin ARC433B69 remote.
class IRDaikin216 {
 public:
  explicit IRDaikin216(uint16_t pin);

#if SEND_DAIKIN216
  void send(const uint16_t repeat = kDaikin216DefaultRepeat);
#endif
  void begin();
  uint8_t* getRaw();
  void setRaw(const uint8_t new_code[]);
  static bool validChecksum(uint8_t state[],
                            const uint16_t length = kDaikin216StateLength);
  void on(void);
  void off(void);
  void setPower(const bool on);
  bool getPower(void);
  void setTemp(const uint8_t temp);
  uint8_t getTemp();
  void setMode(const uint8_t mode);
  uint8_t getMode(void);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  void setFan(const uint8_t fan);
  uint8_t getFan(void);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  void setSwingVertical(const bool on);
  bool getSwingVertical(void);
  void setSwingHorizontal(const bool on);
  bool getSwingHorizontal(void);
  void setQuiet(const bool on);
  bool getQuiet(void);
#ifdef ARDUINO
  String toString(void);
  static String renderTime(const uint16_t timemins);
#else
  std::string toString(void);
  static std::string renderTime(const uint16_t timemins);
#endif
#ifndef UNIT_TEST

 private:
  IRsend _irsend;
#else
  IRsendTest _irsend;
#endif
  // # of bytes per command
  uint8_t remote_state[kDaikin216StateLength];
  void stateReset();
  void checksum();
};

#endif  // IR_DAIKIN_H_
