#include <Wire.h>
#include "Adafruit_MCP23017.h"

// Basic pin reading and pullup test for the MCP23017 I/O expander
// public domain!

// Connect pin #12 of the expander to Analog 5 (i2c clock)
// Connect pin #13 of the expander to Analog 4 (i2c data)
// Connect pins #15, 16 and 17 of the expander to ground (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Input #0 is on pin 21 so connect a button or switch from there to ground

Adafruit_MCP23017 mcp;
  
void setup() {  
  mcp.begin();      // use default address 0

  mcp.pinMode(0, INPUT);
  mcp.pullUp(0, HIGH);  // turn on a 100K pullup internally

  pinMode(13, OUTPUT);  // use the p13 LED as debugging
}



void loop() {
  // The LED will 'echo' the button
  digitalWrite(13, mcp.digitalRead(0));
}