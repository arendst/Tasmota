/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to ESP8266 pin 0.
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */
#include <IRremoteESP8266.h>
 
#define PanasonicAddress      0x4004     // Panasonic address (Pre data) 
#define PanasonicPower        0x100BCBD  // Panasonic Power button

#define JVCPower              0xC5E8

IRsend irsend(0); //an IR led is connected to GPIO pin 0

void setup()
{
   irsend.begin();
}

void loop() {
  irsend.sendPanasonic(PanasonicAddress,PanasonicPower); // This should turn your TV on and off
  
  irsend.sendJVC(JVCPower, 16,0); // hex value, 16 bits, no repeat
  delayMicroseconds(50); // see http://www.sbprojects.com/knowledge/ir/jvc.php for information
  irsend.sendJVC(JVCPower, 16,1); // hex value, 16 bits, repeat
  delayMicroseconds(50);
}
