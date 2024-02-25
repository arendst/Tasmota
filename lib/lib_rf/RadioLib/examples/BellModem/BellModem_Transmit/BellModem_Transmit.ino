/*
   RadioLib Bell Modem Transmit Example

   This example shows how to transmit binary data
   using audio Bell 202 tones.

   Other implemented Bell modems
    - Bell 101
    - Bell 103

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1278 has the following connections:
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
SX1278 radio = new Module(10, 2, 9, 3);

// create Bell modem instance using the FSK module
// this requires connection to the module direct
// input pin, here connected to Arduino pin 5
// SX127x/RFM9x:  DIO2
// RF69:          DIO2
// SX1231:        DIO2
// CC1101:        GDO2
// Si443x/RFM2x:  GPIO
// SX126x/LLCC68: DIO2 (only devices without TCXO!)
BellClient bell(&radio, 5);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for AFSK
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize Bell 202 modem
  Serial.print(F("[Bell 202] Initializing ... "));
  state = bell.begin(Bell202);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[Bell 202] Sending data ... "));

  // send out idle condition for 500 ms
  bell.idle();
  delay(500);

  // BellClient supports all methods of the Serial class

  // Arduino String class
  String aStr = "Arduino String";
  bell.println(aStr);

  // character array (C-String)
  bell.println("C-String");

  // string saved in flash
  bell.println(F("Flash String"));

  // character
  bell.println('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  bell.println(255, HEX);

  // integer number
  int i = 1000;
  bell.println(i);

  // floating point number
  float f = -3.1415;
  bell.println(f, 3);

  // ITA2-encoded string
  ITA2String str("HELLO WORLD!");
  bell.print(str);

  // turn the transmitter off
  bell.standby();

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
