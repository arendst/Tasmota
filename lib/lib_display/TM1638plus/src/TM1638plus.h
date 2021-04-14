/*
* Project Name: TM1638plus 
* File: TM1638plus.h
* Description: TM1638plus.h header file arduino library for TM1638 module(LED & KEY). Model 1 & Model 3
* Author: Gavin Lyons.
* Created May 2019
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/


#ifndef TM1638PLUS_H
#define TM1638PLUS_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "TM1638plus_common.h"

class TM1638plus  {

public:
	// Constructor 
	//Parameters 
	// 1. strobe = GPIO STB pin
	// 2. clock = GPIO CLK  pin
	// 3. data = GPIO DIO pin
	// 4. higfreq Changes the value of parameter _HIGH_FREQ which is default false
	// This is used when running high freq MCU CPU (~>100Mhz) because of issues with button function.
	// Pass true when running high freq MCU CPU (~>100Mhz).
	TM1638plus(uint8_t strobe, uint8_t clock, uint8_t data, bool highfreq = false);
	
	// Methods
	
	void displayBegin();	 // Begin method , sets pinmodes , Call in setup
	
	void reset(void);  // Reset / Clear module 
	
	//Sets the brightness level on a scale of brightness = 0 to 7.
	//0 is not turned off, it's just the lowest brightness.
	//If user wishes to change the default brightness at start-up change.
	 //The DEFAULT_BRIGHTNESS define in header file.
	void brightness(uint8_t brightness);

	//Read buttons returns a byte with value of buttons 1-8 b7b6b5b4b3b2b1b0
	// 1 pressed, zero not pressed. 
	//User may have to debounce buttons depending on application.
	//See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
	// for de-bonce example.
	uint8_t readButtons(void);

	// Send Text to Seven segments, passed char array pointer
	// dots are removed from string and dot on preceding digit switched on
	// "abc.def" will be shown as "abcdef" with c decimal point turned on.
	void displayText(const char *text);

	// Send ASCII value to seven segment, pass position 0-7 and ASCII value byte
	void displayASCII(uint8_t position, uint8_t ascii);

	// Same as displayASCII function but turns on dot/decimal point  as well 
	void displayASCIIwDot(uint8_t position, uint8_t ascii) ;

	// Send HEX value to seven segment, pass position 0-7 and hex value(DEC) 0-15
	void displayHex(uint8_t position, uint8_t hex);

	// Send seven segment value to seven segment
	//  pass position 0-7 byte of data corresponding to segments (dp)gfedcba
	// i.e 0b01000001 will set g and a on. 
	void display7Seg(uint8_t position, uint8_t value);
	
	//Display an integer and leading zeros optional
	void displayIntNum(unsigned long number, boolean leadingZeros = true);
	
	//Divides the display into two nibbles and displays a Decimal number in each.
	//takes in two numbers 0-9999 for each nibble ,  and byte for decimal point display,
	//and leading zeros optional
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, boolean leadingZeros = true);
	
	  // Set the LEDs. passed one  16bit integer.
	  // MODEL 3:
	  //MSB byte for the green LEDs, LS byte for the red LEDs (0xgreenred) 
	  //ie. 0xE007   1110 0000 0000 0111   results in L8-L0  GGGX XRRR, NOTE L8 is RHS on display
	  // MODEL 1:
	  // MSB byte 1 for  red LED , LSB byte n/a set to 0x00 (0xleds, 0xXX)
	  //i.e 0xF100  1111 0000 L8-L0 RRRRXXX0 NOTE  L8 is RHS on display
    void setLEDs(uint16_t greenred);
    
    // Set an LED, pass it LED position 0-7 and value 0 or 1 , L1-L8
	void setLED(uint8_t position, uint8_t value);
	
private:
		uint8_t _STROBE_IO;
		uint8_t _DATA_IO;
		uint8_t _CLOCK_IO;
		 void sendCommand(uint8_t value);
        void sendData(uint8_t  data);
		//This is used when running high freq CPU 
		bool _HIGH_FREQ = false;
		TM1638plus_common  TM_common;
};

#endif
