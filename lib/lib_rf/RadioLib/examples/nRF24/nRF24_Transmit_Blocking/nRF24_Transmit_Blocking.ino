/*
   RadioLib nRF24 Transmit Example

   This example transmits packets using nRF24 2.4 GHz radio module.
   Each packet contains up to 32 bytes of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Packet delivery is automatically acknowledged by the receiver.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#nrf24

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3
nRF24 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//nRF24 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize nRF24 with default settings
  Serial.print(F("[nRF24] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // set transmit address
  // NOTE: address width in bytes MUST be equal to the
  //       width set in begin() or setAddressWidth()
  //       methods (5 by default)
  byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
  Serial.print(F("[nRF24] Setting transmit pipe ... "));
  state = radio.setTransmitPipe(addr);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

// counter to keep track of transmitted packets
int count = 0;

void loop() {
  Serial.print(F("[nRF24] Transmitting packet ... "));

  // you can transmit C-string or Arduino string up to
  // 32 characters long
  String str = "Hello World! #" + String(count++);
  int state = radio.transmit(str);

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 32 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_ACK_NOT_RECEIVED) {
    // acknowledge from destination module
    // was not received within 15 retries
    Serial.println(F("ACK not received!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timed out while transmitting
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before transmitting again
  delay(1000);
}
