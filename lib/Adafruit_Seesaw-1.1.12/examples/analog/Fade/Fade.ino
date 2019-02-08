/*
  Fade

  This example shows how to fade an LED on pin 6 of a seesaw board using the analogWrite()
  function.

  The analogWrite() function uses PWM, so if you want to change the pin you're
  using, be sure to use another PWM capable pin. On the SAMD09 breakout these are pins 5, 6, and 7
*/

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

int led = 6;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");
}

// the loop routine runs over and over again forever:
void loop() {
  // set the brightness of pin 9:
  ss.analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
