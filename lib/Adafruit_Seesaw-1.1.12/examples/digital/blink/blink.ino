/*
 * This example shows how to blink a pin on a seesaw.
 * Attach the positive (longer lead) of the LED to pin 15 on the seesaw, and
 * the negative lead of the LED to ground through a 1k ohm resistor.
 */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(9600);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

  ss.pinMode(15, OUTPUT);
}

void loop() {
  ss.digitalWrite(15, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  ss.digitalWrite(15, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
}