/*
	Low level (advanced) example

  In this example the display is controlled by the low level command function.
  See SevenSegmentTM1637.h for more details on how the protocol actually works and which commands it accepts.

	The circuit:
  * connect TM1637 pin CLK to Arduino pin D4
  * connect TM1637 pin DIO to Arduino pin D5
  * connect TM1637 pin Vcc to Arduino pin 5V
  * connect TM1637 pin GND to Arduino pin GND

	Created 25 September 2015
	By Bram Harmsen

	https://github.com/bremme/arduino-tm1637

*/

// include the SevenSegmentTM1637 library
#include "SevenSegmentTM1637.h"

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 4;   // define CLK pin (any digital pin)
const byte PIN_DIO = 5;   // define DIO pin (any digital pin)
SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);

// run setup code
void setup() {
  Serial.begin(9600);     // initializes the Serial connection @ 9600 baud
};

void loop() {
  // set brightness
  uint8_t setDisplayCmd   = B10000000; // bit 6,7
  uint8_t brightnessBits  =      B111; // bit 0,1,2 (7 = max)
  uint8_t displayOnBit    =     B1000; // bit 3
  // construct complete command
  uint8_t command = setDisplayCmd | brightnessBits | displayOnBit;

  // turn display on and set brightness to max (7)
  bool ack = display.command(command);
  // print acknowledged?
  Serial.println(ack);

  // write init to display using automatic address
  uint8_t setDataCmd      = B01000000;  // bit 6,7
  ack = display.command(setDataCmd);
  Serial.println(ack);

  uint8_t setAddrCmd      = B11000000;  // bit 6,7 set addr to 0
  uint8_t dataI           = B00000110;  // I
  uint8_t dataN           = B00110111;  // N
  uint8_t dataT           = B01111000;  // T

  uint8_t commands[5];
  commands[0] = setAddrCmd;
  commands[1] = dataI;
  commands[2] = dataN;
  commands[3] = dataI;
  commands[4] = dataT;

  ack = display.command(commands, 5);
  Serial.println(ack);
  delay(1000);

  // dim display
  brightnessBits          = B001; // bit 0,1,2 level=1;
  command = setDisplayCmd | brightnessBits | displayOnBit;
  display.command(command);
  delay(1000);

  // set back to full brightness
  brightnessBits          = B111; // bit 0,1,2 (7 = max)
  command = setDisplayCmd | brightnessBits | displayOnBit;
  display.command(command);
  delay(1000);

  // write characters to display in non auto mode
  setDataCmd |= 1 << 2;      // bit 2 control auto/manual address
  ack = display.command(setDataCmd);
  Serial.println(ack);

  commands[1]   = B01110111;
  commands[2]   = B01111111;
  commands[3]   = B00111001;
  commands[4]   = B01011110;

  for (uint8_t i = 1; i < 5; i++) {
    commands[1] = commands[i];      // set character
    display.command(commands, 2);   // write character to display
    commands[0]++;                  // increase address
  }
  delay(1000);

  // set back to auto address mode
  setDataCmd &= ~(1 << 2);      // bit 2 control auto/manual address
  ack = display.command(setDataCmd);
  Serial.println(ack);

  // clear display
  commands[0] = setAddrCmd;
  commands[1] = 0; commands[2] = 0;commands[3] = 0; commands[4] = 0;

  // clear display
  display.command(commands, 5);
  delay(1000);
}
