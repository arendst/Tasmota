/*
 * This example shows how to blink multiple pins at once on a seesaw.
 * pin 13 is attached to the LED on the samd11 xplained board
 */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

//blink pins PA11, PA12, PA13
uint32_t mask = ((uint32_t)0b111 << 11);

void setup() {
  Serial.begin(9600);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

  ss.pinModeBulk(mask, OUTPUT); //set pin modes
}

void loop() {
  ss.digitalWriteBulk(mask, HIGH);   //set pins
  delay(1000);                       // wait for a second
  ss.digitalWriteBulk(mask, LOW);   //clear pins
  delay(1000);  
}
