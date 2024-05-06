/*
   RadioLib SX128x FLRC Modem Example

   This example shows how to use FLRC modem in SX128x chips.

   NOTE: The sketch below is just a guide on how to use
         FLRC modem, so this code should not be run directly!
         Instead, modify the other examples to use FLRC
         modem and use the appropriate configuration
         methods.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---flrc-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1280 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
SX1280 radio = new Module(10, 2, 3, 9);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1280 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1280 with default settings
  Serial.print(F("[SX1280] Initializing ... "));
  int state = radio.beginFLRC();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, you can switch between any of the modems
  //
  // radio.begin()       start LoRa modem (and disable FLRC)
  // radio.beginFLRC()   start FLRC modem (and disable LoRa)

  // the following settings can also
  // be modified at run-time
  state = radio.setFrequency(2410.5);
  state = radio.setBitRate(520);
  state = radio.setCodingRate(2);
  state = radio.setOutputPower(5);
  state = radio.setDataShaping(1.0);
  uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67};
  state = radio.setSyncWord(syncWord, 4);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

  #warning "This sketch is just an API guide! Read the note at line 6."
}

void loop() {
  // FLRC modem can use the same transmit/receive methods
  // as the LoRa modem, even their interrupt-driven versions

  // transmit FLRC packet
  int state = radio.transmit("Hello World!");
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1280] Packet transmitted successfully!"));
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println(F("[SX1280] Packet too long!"));
  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    Serial.println(F("[SX1280] Timed out while transmitting!"));
  } else {
    Serial.println(F("[SX1280] Failed to transmit packet, code "));
    Serial.println(state);
  }

  // receive FLRC packet
  String str;
  state = radio.receive(str);
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1280] Received packet!"));
    Serial.print(F("[SX1280] Data:\t"));
    Serial.println(str);
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("[SX1280] Timed out while waiting for packet!"));
  } else {
    Serial.print(F("[SX1280] Failed to receive packet, code "));
    Serial.println(state);
  }
}
