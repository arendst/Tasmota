/*
 * This example shows how to read multiple pins at once on a seesaw.
 */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

//read pins A8, A9, A10
uint32_t mask = ((uint32_t)0b111 << 8);

void setup() {
  Serial.begin(9600);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");
  
  ss.pinModeBulk(mask, INPUT);
}

void loop() {
  Serial.println(ss.digitalReadBulk(mask), BIN);
  delay(500);
}
