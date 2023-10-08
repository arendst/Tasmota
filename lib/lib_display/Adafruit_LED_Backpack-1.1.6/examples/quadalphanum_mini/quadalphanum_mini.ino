// Demo the quad alphanumeric display LED backpack kit
// Displays a short message and then scrolls through every character

// For use with Gemma or Trinket (Attiny85) 

#include <avr/power.h>
#include <Wire.h>


// Connect + pins to 3-5V
// Connect GND to ground
// Connect Data to #0
// Connect Clock to #2

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

char *message = "Hello world!     ";

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

void setup() {
  // This is the auto-speed doubler line, keep it in, it will
  // automatically double the speed when 16Mhz is selected!
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  
  alpha4.begin(0x70);  // pass in the address

  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  
  alpha4.clear();
  alpha4.writeDisplay();

  // send a message!
  for (uint8_t i=0; i<strlen(message)-4; i++) {
    alpha4.writeDigitAscii(0, message[i]);
    alpha4.writeDigitAscii(1, message[i+1]);
    alpha4.writeDigitAscii(2, message[i+2]);
    alpha4.writeDigitAscii(3, message[i+3]);
    alpha4.writeDisplay();
    
    delay(200);
  }
}

void loop() {
  // display every character, 
  for (uint8_t i='!'; i<='z'; i++) {
    alpha4.writeDigitAscii(0, i);
    alpha4.writeDigitAscii(1, i+1);
    alpha4.writeDigitAscii(2, i+2);
    alpha4.writeDigitAscii(3, i+3);
    alpha4.writeDisplay();
    
    delay(300);
  }
}