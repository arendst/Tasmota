/*
	Clock example example (Extended class example)

  Display a clock on the display. For this demo you can add a speed multiplier to make the clock run faster. For a real clock you want to use a delay of 1 min or even use a Real Time Clock module (RTC)

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
#include "SevenSegmentExtended.h"

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 4;   // define CLK pin (any digital pin)
const byte PIN_DIO = 5;   // define DIO pin (any digital pin)
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);

const unsigned int clockSpeed = 10000;    // speed up clock for demo

// run setup code
void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  delay(1000);                // wait 1000 ms
};

// run loop (forever)
void loop() {

  byte hours    = 14;                           // initialize hours
  byte minutes  = 39;                           // initialize minutes

  for ( ; hours < 24; hours++) {                // count hours   up to 24
    for ( ; minutes < 60; minutes++) {          // count minutes up to 59
      display.printTime(hours, minutes, true);  // display time
      delay(60000 / clockSpeed);                // clock delay ms
    };
    minutes = 0;                                // reset minutes
  };
};
