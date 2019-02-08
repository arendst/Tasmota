/*
 * This example shows how read the ADC on a seesaw. The default ADC pins on the SAMD09 Breakout are 2, 3, and 4.
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
}

void loop() {
  Serial.print(ss.analogRead(2));
  Serial.print(",");
  Serial.println(ss.analogRead(3));
  delay(50);  
}
