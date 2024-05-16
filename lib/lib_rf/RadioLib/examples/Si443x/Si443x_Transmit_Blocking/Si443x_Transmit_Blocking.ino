/*
  RadioLib Si443x Blocking Transmit Example

  This example transmits packets using Si4432 FSK radio module.
  Each packet contains up to 64 bytes of data, in the form of:
  - Arduino String
  - null-terminated char array (C-string)
  - arbitrary binary data (byte array)

  Other modules from Si443x/RFM2x family can also be used.

  Using blocking transmit is not recommended, as it will lead
  to inefficient use of processor time!
  Instead, interrupt transmit is recommended.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#si443xrfm2x

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// Si4432 has the following connections:
// nSEL pin:  10
// nIRQ pin:  2
// SDN pin:   9
Si4432 radio = new Module(10, 2, 9);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//Si4432 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize Si4432 with default settings
  Serial.print(F("[Si4432] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

// counter to keep track of transmitted packets
int count = 0;

void loop() {
  Serial.print(F("[Si4432] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 64 characters long
  String str = "Hello World! #" + String(count++);
  int state = radio.transmit(str);

  // you can also transmit byte array up to 64 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F(" too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occured while transmitting packet
    Serial.println(F(" timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(1000);
}
