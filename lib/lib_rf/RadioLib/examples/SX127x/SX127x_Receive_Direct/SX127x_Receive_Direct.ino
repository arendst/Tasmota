/*
   RadioLib SX127x Direct Receive Example

   This example shows how to receive FSK packets without using
   SX127x packet engine.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

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

// DIO2 pin:  5
const int pin = 5;

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with FSK modem at 9600 bps
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK(434.0, 9.6, 20.0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the direct mode sync word
  // the default RadioLib FSK sync word is 0x12AD
  // we can add in some preamble bits, to lower the chance
  // of false detection
  radio.setDirectSyncWord(0x555512AD, 32);

  // set function that will be called each time a bit is received
  radio.setDirectAction(readBit);

  // start direct mode reception
  radio.receiveDirect();
}

// this function is called when a new bit is received
void readBit(void) {
  // read the data bit
  radio.readBit(pin);
}

void loop() {
  // we expect the packet to contain the string "Hello World!",
  // a length byte and 2 CRC bytes, that's 15 bytes in total
  if(radio.available() >= 15) {
    Serial.println("[SX1278] Received packet in direct mode!");
    while(radio.available()) {
      // read a byte
      byte b = radio.read();
  
      // print it
      Serial.print(b, HEX);
      Serial.print('\t');
      Serial.write(b);
      Serial.println();
    }
  }
}
