/*
 * This example shows how to read and write EEPROM data. Try writing
 * then removing power from both devices, commenting out the write, and
 * uploading again.
 */

#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

void setup() {
  Serial.begin(115200);
  //while(!Serial);
  
  if(!ss.begin()){
    Serial.println("ERROR!");
    while(1);
  }
  else Serial.println("seesaw started");

  Serial.println("writing 0x0D to register 0x02");
  ss.EEPROMWrite8(0x02, 0xD);

  delay(500);
  Serial.print("reading from register 0x02...0x");
  Serial.println(ss.EEPROMRead8(0x02), HEX);
}

void loop() {
  //DONT WRITE EEPROM IN A LOOP!!!! YOU WILL DESTROY YOUR FLASH!!!
}