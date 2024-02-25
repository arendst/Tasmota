/*
   RadioLib CC1101 Transmit to Address Example

   This example transmits packets using CC1101 FSK radio
   module. Packets can have 1-byte address of the
   destination node. After setting node address, this node
   will automatically filter out any packets that do not
   contain either node address or broadcast addresses.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#cc1101

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// CC1101 has the following connections:
// CS pin:    10
// GDO0 pin:  2
// RST pin:   unused
// GDO2 pin:  3 (optional)
CC1101 radio = new Module(10, 2, RADIOLIB_NC, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//CC1101 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize CC1101 with default settings
  Serial.print(F("[CC1101] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set node address
  // NOTE: Calling this method will automatically enable
  //       address filtering. CC1101 also allows to set
  //       number of broadcast address (0/1/2).
  //       The following sets one broadcast address 0x00.
  //       When setting two broadcast addresses, 0x00 and
  //       0xFF will be used.
  Serial.print(F("[CC1101] Setting node address ... "));
  state = radio.setNodeAddress(0x01, 1);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // address filtering can also be disabled
  // NOTE: Calling this method will also erase previously
  //       set node address
  /*
    Serial.print(F("[CC1101] Disabling address filtering ... "));
    state == radio.disableAddressFiltering();
    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while(true);
    }
  */
}

void loop() {
  Serial.print(F("[CC1101] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to 63 characters long
  int state = radio.transmit("Hello World!");

  // you can also transmit byte array up to 63 bytes long
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 255 bytes
    Serial.println(F("too long!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(1000);
}
