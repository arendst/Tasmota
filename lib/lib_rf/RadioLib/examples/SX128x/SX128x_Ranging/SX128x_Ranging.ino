/*
   RadioLib SX128x Ranging Example

   This example performs ranging exchange between two
   SX1280 LoRa radio modules. Ranging allows to measure
   distance between the modules using time-of-flight
   measurement.

   Only SX1280 and SX1282 without external RF switch support ranging!

   Note that to get accurate ranging results, calibration is needed!
   The process is described in Semtech SX1280 Application Note AN1200.29

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

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
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void loop() {
  Serial.print(F("[SX1280] Ranging ... "));

  // start ranging exchange
  // range as master:             true
  // slave address:               0x12345678
  int state = radio.range(true, 0x12345678);

  // the other module must be configured as slave with the same address
  /*
    int state = radio.range(false, 0x12345678);
  */

  // if ranging calibration is known, it can be provided
  // this should improve the accuracy and precision
  /*
    uint16_t calibration[3][6] = {
      { 10299, 10271, 10244, 10242, 10230, 10246 },
      { 11486, 11474, 11453, 11426, 11417, 11401 },
      { 13308, 13493, 13528, 13515, 13430, 13376 }
    };

    int state = radio.range(true, 0x12345678, calibration);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // ranging finished successfully
    Serial.println(F("success!"));
    Serial.print(F("[SX1280] Distance:\t\t\t"));
    Serial.print(radio.getRangingResult());
    Serial.println(F(" meters (raw)"));

  } else if (state == RADIOLIB_ERR_RANGING_TIMEOUT) {
    // timed out waiting for ranging packet
    Serial.println(F("timed out!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  // wait for a second before ranging again
  delay(1000);
}
