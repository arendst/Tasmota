/* Copyright 2016 sillyfrog */
#ifndef IR_DAIKIN_H_
#define IR_DAIKIN_H_

#include "IRremoteESP8266.h"
#include "IRsend.h"

//                DDDDD     AAA   IIIII KK  KK IIIII NN   NN
//                DD  DD   AAAAA   III  KK KK   III  NNN  NN
//                DD   DD AA   AA  III  KKKK    III  NN N NN
//                DD   DD AAAAAAA  III  KK KK   III  NN  NNN
//                DDDDDD  AA   AA IIIII KK  KK IIIII NN   NN

/*
	Daikin AC map
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
		b3 = 0
		b2 = OFF timer set
		b1 = ON timer set
		b0 = Air Conditioner ON
	byte 14=temp*2   (Temp should be between 18 - 32)
	byte 16=Fan
		FAN control
		b7+b6+b5+b4 = Fan speed
			Fan: b7+b6+b5+b4
			0×30 = 1 bar
			0×40 = 2 bar
			0×50 = 3 bar
			0×60 = 4 bar
			0×70 = 5 bar
			0xa0 = Auto
			0xb0 = Not auto, moon + tree
		b3+b2+b1+b0 = Swing control up/down
			Swing control up/down:
			0000 = Swing up/down off
			1111 = Swing up/down on
	byte 17
			Swing control left/right:
			0000 = Swing left/right off
			1111 = Swing left/right on
	byte 21=Aux  -> Powerful (bit 1), Silent (bit 5)
	byte 24=Aux2 -> Intelligent eye on (bit 7)
	byte 26= checksum of the second part
*/

// Constants
#define DAIKIN_COOL                0b011
#define DAIKIN_HEAT                0b100
#define DAIKIN_FAN                 0b110
#define DAIKIN_AUTO                0b000
#define DAIKIN_DRY                 0b010
#define DAIKIN_POWERFUL       0b00000010
#define DAIKIN_SILENT         0b00100000
#define DAIKIN_MIN_TEMP               18U  // Celsius
#define DAIKIN_MAX_TEMP               32U  // Celsius
#define DAIKIN_FAN_AUTO      (uint8_t) 0U
#define DAIKIN_FAN_MIN       (uint8_t) 1U
#define DAIKIN_FAN_MAX       (uint8_t) 5U

#if SEND_DAIKIN
class IRDaikinESP {
 public:
  explicit IRDaikinESP(uint16_t pin);

  void send();
  void begin();
  void on();
  void off();
  void setPower(bool state);
  uint8_t getPower();
  void setAux(uint8_t aux);
  uint8_t getAux();
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
  uint8_t* getRaw();

 private:
  // # of bytes per command
  uint8_t daikin[DAIKIN_COMMAND_LENGTH];
  void stateReset();
  void checksum();
  IRsend _irsend;
};
#endif

#endif  // IR_DAIKIN_H_
