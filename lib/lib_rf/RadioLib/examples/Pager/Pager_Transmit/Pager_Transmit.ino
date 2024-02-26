/*
   RadioLib Pager (POCSAG) Transmit Example

   This example sends POCSAG messages using SX1278's
   FSK modem.

   Other modules that can be used to send POCSAG:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - SX126x
    - nRF24
    - Si443x/RFM2x
    - SX128x

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

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

// create Pager client instance using the FSK module
PagerClient pager(&radio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize Pager client
  Serial.print(F("[Pager] Initializing ... "));
  // base (center) frequency:     434.0 MHz
  // speed:                       1200 bps
  state = pager.begin(434.0, 1200);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[Pager] Transmitting messages ... "));

  // the simples form of "message" is just a tone on the destination pager
  int state = pager.sendTone(1234567);
  delay(500);

  // next, transmit numeric (BCD) message to the destination pager
  // NOTE: Only characters 0123456789*U-() and space
  //       can be sent in a BCD message!
  state |= pager.transmit("0123456789*U -()", 1234567);
  delay(500);

  // finally, let's transmit an ASCII message now
  state |= pager.transmit("Hello World!", 1234567, RADIOLIB_PAGER_ASCII);
  delay(500);

  // we can also send only a tone

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait for a second before transmitting again
  delay(3000);
}
