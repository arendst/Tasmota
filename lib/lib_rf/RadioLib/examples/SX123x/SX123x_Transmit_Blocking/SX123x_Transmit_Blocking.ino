/*
  RadioLib SX123x Blocking Transmit Example

  This example transmits packets using SX1231 FSK radio module.
  Other modules from SX123x family can also be used.

  NOTE: SX123x modules offer the same features as RF69 and have the same
        interface. Please see RF69 examples for examples on AES,
        address filtering, interrupts and settings.

  Using blocking transmit is not recommended, as it will lead
  to inefficient use of processor time!
  Instead, interrupt transmit is recommended.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#rf69sx1231

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1231 has the following connections:
// CS pin:    10
// DIO0 pin:  2
// RESET pin: 3
SX1231 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1231 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1231 with default settings
  Serial.print(F("[SX1231] Initializing ... "));
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
  Serial.print(F("[SX1231] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to 256 characters long
  String str = "Hello World! #" + String(count++);
  int state = radio.transmit(str);

  // you can also transmit byte array up to 256 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  }

  // wait for a second before transmitting again
  delay(1000);
}
