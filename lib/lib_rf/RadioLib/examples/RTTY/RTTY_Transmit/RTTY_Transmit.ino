/*
   RadioLib RTTY Transmit Example

   This example sends RTTY message using SX1278's
   FSK modem.

   Other modules that can be used for RTTY:
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

// create RTTY client instance using the FSK module
RTTYClient rtty(&radio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for RTTY
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize RTTY client
  // NOTE: RTTY frequency shift will be rounded
  //       to the nearest multiple of frequency step size.
  //       The exact value depends on the module:
  //         SX127x/RFM9x - 61 Hz
  //         RF69 - 61 Hz
  //         CC1101 - 397 Hz
  //         SX126x - 1 Hz
  //         nRF24 - 1000000 Hz
  //         Si443x/RFM2x - 156 Hz
  //         SX128x - 198 Hz
  Serial.print(F("[RTTY] Initializing ... "));
  // low ("space") frequency:     434.0 MHz
  // frequency shift:             183 Hz
  // baud rate:                   45 baud
  // encoding:                    ASCII (7-bit)
  // stop bits:                   1
  state = rtty.begin(434.0, 183, 45);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  /*
    // RadioLib also provides ITA2 ("Baudot") support
    rtty.begin(434, 183, 45, ITA2);

    // All transmissions in loop() (strings and numbers)
    // will now be encoded using ITA2 code

    // ASCII characters that do not have ITA2 equivalent
    // will be sent as NUL (including lower case letters!)
  */
}

void loop() {
  Serial.print(F("[RTTY] Sending RTTY data ... "));

  // send out idle condition for 500 ms
  rtty.idle();
  delay(500);

  // RTTYClient supports all methods of the Serial class

  // Arduino String class
  String aStr = "Arduino String";
  rtty.println(aStr);

  // character array (C-String)
  rtty.println("C-String");

  // string saved in flash
  rtty.println(F("Flash String"));

  // character
  rtty.println('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  rtty.println(255, HEX);

  // integer number
  int i = 1000;
  rtty.println(i);

  // floating point number
  float f = -3.1415;
  rtty.println(f, 3);

  // turn the transmitter off
  rtty.standby();

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
