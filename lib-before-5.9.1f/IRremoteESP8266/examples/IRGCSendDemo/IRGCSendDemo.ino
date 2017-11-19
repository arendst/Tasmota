/*
 * IRremoteESP8266: IRsendGCDemo - demonstrates sending Global Cache-formatted IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 30 March, 2016
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#include <IRremoteESP8266.h>

// Codes are in Global Cache format less the emitter ID and request ID. These codes can be found in GC's Control Tower database.

unsigned int Samsung_power_toggle[71] = {38000,1,1,170,170,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,63,20,63,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,20,20,20,20,20,20,20,20,20,20,63,20,20,20,63,20,63,20,63,20,63,20,63,20,63,20,1798};

IRsend irsend(4); //an IR emitter led is connected to GPIO pin 4

void setup()
{
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  Serial.println("Toggling power");
  irsend.sendGC(Samsung_power_toggle, 71);
  delay(10000);
}