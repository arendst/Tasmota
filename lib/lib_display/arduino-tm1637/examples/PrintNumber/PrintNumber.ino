/*
  Printing numbers example

  Demonstrate a couple different ways nubers can be printed: positive, negative,
  with padding and with rollover.


	The circuit:
  * connect TM1637 pin CLK to Arduino pin D4
  * connect TM1637 pin DIO to Arduino pin D5
  * connect TM1637 pin Vcc to Arduino pin 5V
  * connect TM1637 pin GND to Arduino pin GND

	Created 22 June 2020
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
SevenSegmentExtended    display(PIN_CLK, PIN_DIO);

// run setup code
void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.print("INIT");      // display INIT on the display
  delay(1000);                // wait 1000 ms
};

// run loop (forever)
void loop() {

    // print positive numbers
    display.print("PRINT POSITIVE NUMBERS");
    delay(1000);                          // wait 1000 ms
    for (int16_t number=0; number < 2000; number++)  {
        display.printNumber(number);
        delay(1);
    };

    // print negative numbers
    display.print("PRINT NEGATIVE NUMBERS");
    delay(1000);                          // wait 1000 ms
    for (int16_t number=0; number > -999; number--) {
        display.printNumber(number);
        delay(2);
    };

    // print with positive with zero padding
    display.print("PRINT POSITIVE NUMBERS WITH PADDING");
    delay(1000);                          // wait 1000 ms
    for (int16_t number=0; number < 2000; number++)  {
        display.printNumber(number, true);
        delay(1);
    };

    // print with rollover (e.g. 10000 -> 0, 10001 -> 1)
    display.print("PRINT POSITIVE NUMBERS WITH ROLLOVER");
    delay(1000);                          // wait 1000 ms
    for (int16_t number=0; number < 20000; number+=250)  {
        display.printNumber(number, false, true);
        delay(50);
    };
};
