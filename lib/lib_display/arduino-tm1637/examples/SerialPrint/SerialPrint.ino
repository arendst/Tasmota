/*
	Serial Print example

  Prints text typed to the Serial Monitor on the display. Connect an Arduino to a TM1637 4 digit 7-segment display. Connect the wires as descibed below. Run the sketch and open the Serial Monitor. Set the the speed to 9600 baud, set the line ending to Newline. Now type some text and press enter, the text will be displayed on the 7-segment display.

  Note: make sure to set your serial monitor to line end: NEW LINE!

	The circuit:
  * connect TM1637 pin CLK to Arduino pin D4
  * connect TM1637 pin DIO to Arduino pin D5
  * connect TM1637 pin Vcc to Arduino pin 5V
  * connect TM1637 pin GND to Arduino pin GND

	Created 25 September 2015
	By Bram Harmsen

	http://url/of/online/tutorial.cc

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

// define a fixed buffer size for receiving characters via Serial
const byte BUFFER_SIZE = 128;
char serialBuffer[BUFFER_SIZE];   // initialize global serial buffer

// run setup code
void setup() {
  display.begin();      // initializes the display
  Serial.begin(9600);   // initializes the Serial connection @ 9600 baud
  Serial.println(F("Enter some text followed by ENTER"));
  Serial.println(F("also make sure to set the line ending to Newline (\\n)"));
};

// run loop (forever)
void loop() {
  // if received new serial data, print to display
  if ( receivedSerialString() ) {
    display.print(serialBuffer);    // Print received serial data to display
    Serial.print(F("Echo:\t"));     // Echo serial data back to Serial Monitor
    Serial.println(serialBuffer);
  };
};

// Read in bytes from Serial Monitor and return true is ether the serialBuffer is full or a Newline character is received
bool receivedSerialString() {
  static unsigned int i=0;          // init static counter to keep track of count

  while( Serial.available() ) {     // check if new data arrived

    if ( i == BUFFER_SIZE-1) {      // if buffer is full RETURN true
      serialBuffer[i] = '\0';       // add termination char
      i = 0;
      Serial.println(F("Buffer full!"));
      return true;
    };

    char c = Serial.read();         // read new char from serial port

    if ( c == '\n') {               // if new line RETURN true
      serialBuffer[i] = '\0';       // add termination char
      i = 0;
      return true;
    } else {
      serialBuffer[i] = c;          // add received character to buffer
    }
    i++;                            // increase counter
  };
  return false;                     // default RETURN false
};
