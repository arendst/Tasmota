/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>

IRsend irsend(0); //an IR led is connected to GPIO pin 0

void setup()
{
  irsend.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.println("NEC");
  irsend.sendNEC(0x00FFE01F, 36);
  delay(2000);
  Serial.println("Sony");
  irsend.sendSony(0xa90, 12);
  delay(2000);
}
