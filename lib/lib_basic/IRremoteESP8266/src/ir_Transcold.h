// Copyright 2020 Chandrashekar Shetty (iamDshetty)
// Copyright 2020 crankyoldgit
// Copyright 2021 siriuslzx

/// @file
/// @brief Support for Transcold A/C protocols.
/// @see https://github.com/crankyoldgit/IRremoteESP8266/issues/1256
/// @see https://docs.google.com/spreadsheets/d/1qdoyB0FyJm85HPP9oXcfui0n4ztXBFlik6kiNlkO2IM/edit?usp=sharing

// Supports:
//   Brand: Transcold,  Model: M1-F-NO-6 A/C

/***************************************************************************************************************

 Raw Data Calculation: (UR 12)
//ON button
ON 	24	Auto	cool	close (right)	111011110001000001100001100111100101010010101011

//OFF button
OFF	24	Auto	cool	close (right)	111011110001000001110001100011100101010010101011

// MODE
Hot mode	24	auto	hot	close (right)	111010010001011010100001010111100101010010101011
Fan mode	0 (prev24)	low	fan	close (right)	"11101001	0001011000100001110111100101010010101011"
Dry	mode  24	low	dry	close (right)	"11101001	0001011011000001	00111110	0101010010101011"
Auto Mode	0(prev24)	low	auto	close (right)	"11101001	0001011011100001	00011110	0101010010101011"
Cool Mode	24	low	cool	close (right)	"11101001	0001011001100001	10011110	0101010010101011"

//FAN SPEED
fan Speed low	24	low	cool	close (right)	"11101001	0001011001100001	10011110	0101010010101011"
fan Speed  medium	24	medium	cool	close (right)	"11101101	000100100110000110011110	0101010010101011"
fan Speed high	24	high	cool	close (right)	"11101011	000101000110000110011110	0101010010101011"
fan Speed auto	24	auto	cool	close (right)	"11101111	000100000110000110011110	0101010010101011"

//SWING
Swing open 	24	auto	cool	open (left)	"11110111	000010000110000110011110	0101010010101011"
Swing close	24	auto	cool	close (right)	"11101111	000100000110000110011110	0101010010101011"

//TEMPERATURE
temp 30degC	Auto	cool	close (right)	111011110001000001100100100100010101010010101011
temp 	29	Auto	cool	close (right)	111011110001000001101100100100010101010010101011
temp 	28	Auto	cool	close (right)	111011110001000001100010100100010101010010101011
temp 	27	Auto	cool	close (right)	111011110001000001101010100100010101010010101011
temp 	26	Auto	cool	close (right)	111011110001000001100110100100010101010010101011
temp 	25	Auto	cool	close (right)	111011110001000001101110100100010101010010101011
temp 	24	Auto	cool	close (right)	111011110001000001100001100111100101010010101011
temp 	23	Auto	cool	close (right)	111011110001000001101001100101100101010010101011
temp 	22	Auto	cool	close (right)	111011110001000001100101100101100101010010101011
temp 	21	Auto	cool	close (right)	111011110001000001101101100101100101010010101011
temp 	20	Auto	cool	close (right)	111011110001000001100011100101100101010010101011
temp 	19	Auto	cool	close (right)	111011110001000001101011100101100101010010101011
temp 	18	Auto	cool	close (right)	111011110001000001100111100110000101010010101011
temp 	17	Auto	cool	close (right)	111011110001000001100111100110000101010010101011
temp 	16	Auto	cool	close (right)	111011110001000001100111100110000101010010101011

 **************************************************************************************************************/

#ifndef IR_TRANSCOLD_H_
#define IR_TRANSCOLD_H_

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

/// Native representation of a Transcold A/C message.
union TranscoldProtocol{
  uint32_t raw;  ///< The state of the IR remote in IR code form.
  struct {
    uint8_t       :8;
    uint8_t Temp  :4;
    uint8_t Mode  :4;
    uint8_t Fan   :4;
    uint8_t       :4;
    uint8_t       :8;
  };
};

// Constants
// Modes
const uint8_t kTranscoldCool = 0b0110;
const uint8_t kTranscoldDry = 0b1100;
const uint8_t kTranscoldAuto = 0b1110;
const uint8_t kTranscoldHeat = 0b1010;
const uint8_t kTranscoldFan = 0b0010;

// Fan Control
const uint8_t kTranscoldFanMin = 0b1001;
const uint8_t kTranscoldFanMed = 0b1101;
const uint8_t kTranscoldFanMax = 0b1011;
const uint8_t kTranscoldFanAuto = 0b1111;
const uint8_t kTranscoldFanAuto0 = 0b0110;
const uint8_t kTranscoldFanZoneFollow = 0b0000;
const uint8_t kTranscoldFanFixed = 0b1100;

// Temperature
const uint8_t kTranscoldTempMin = 18;  // Celsius
const uint8_t kTranscoldTempMax = 30;  // Celsius
const uint8_t kTranscoldFanTempCode = 0b1111;  // Part of Fan Mode.
const uint8_t kTranscoldTempSize = 4;

const uint8_t kTranscoldPrefix = 0b0000;
const uint8_t kTranscoldUnknown = 0xFF;
const uint32_t kTranscoldOff    = 0b111011110111100101010100;
const uint32_t kTranscoldSwing  = 0b111001110110000101010100;
const uint32_t kTranscoldSwingH = 0b111101110110000101010100;  // NA
const uint32_t kTranscoldSwingV = 0b111001110110000101010100;  // NA
const uint32_t kTranscoldCmdFan = 0b111011110110000101010100;  // NA

const uint32_t kTranscoldKnownGoodState = 0xE96554;

// Classes
/// Class for handling detailed Transcold A/C messages.
class IRTranscoldAc {
 public:
  explicit IRTranscoldAc(const uint16_t pin, const bool inverted = false,
                      const bool use_modulation = true);
  void stateReset(void);
#if SEND_TRANSCOLD
  void send(const uint16_t repeat = kTranscoldDefaultRepeat);
  /// Run the calibration to calculate uSec timing offsets for this platform.
  /// @return The uSec timing offset needed per modulation of the IR Led.
  /// @note This will produce a 65ms IR signal pulse at 38kHz.
  ///   Only ever needs to be run once per object instantiation, if at all.
  int8_t calibrate(void) { return _irsend.calibrate(); }
#endif  // SEND_TRANSCOLD
  void begin(void);
  void on(void);
  void off(void);
  void setPower(const bool state);
  bool getPower(void) const;
  void setTemp(const uint8_t temp);
  uint8_t getTemp(void) const;
  void setFan(const uint8_t speed, const bool modecheck = true);
  uint8_t getFan(void) const;
  void setMode(const uint8_t mode);
  uint8_t getMode(void) const;
  void setSwing(void);
  bool getSwing(void) const;
  uint32_t getRaw(void) const;
  void setRaw(const uint32_t new_code);
  static uint8_t convertMode(const stdAc::opmode_t mode);
  static uint8_t convertFan(const stdAc::fanspeed_t speed);
  static stdAc::opmode_t toCommonMode(const uint8_t mode);
  static stdAc::fanspeed_t toCommonFanSpeed(const uint8_t speed);
  stdAc::state_t toCommon(const stdAc::state_t *prev = NULL) const;
  String toString(void) const;
#ifndef UNIT_TEST

 private:
  IRsend _irsend;  ///< Instance of the IR send class
#else
  /// @cond IGNORE
  IRsendTest _irsend;  ///< Instance of the testing IR send class
  /// @endcond
#endif
  // internal state
  bool    swingFlag;
  bool    swingHFlag;
  bool    swingVFlag;

  TranscoldProtocol _;
  uint32_t special_state;   ///< special mode.
  bool isSpecialState(void) const;
  bool handleSpecialState(const uint32_t data);
};

#endif  // IR_TRANSCOLD_H_
