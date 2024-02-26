/*
   RadioLib SX128x Settings Example

   This example shows how to change all the properties of LoRa transmission.
   RadioLib currently supports the following settings:
    - pins (SPI slave select, DIO1, DIO2, BUSY pin)
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - output power during transmission
    - CRC
    - preamble length

   Other modules from SX128x family can also be used.

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
SX1280 radio1 = new Module(10, 2, 3, 9);

// SX1280 has the following connections:
// NSS pin:   8
// DIO1 pin:  4
// NRST pin:  5
// BUSY pin:  6
SX1281 radio2 = new Module(8, 4, 5, 6);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1282 radio3 = RadioShield.ModuleB;

void setup() {
  Serial.begin(9600);

  // initialize SX1280 with default settings
  Serial.print(F("[SX1280] Initializing ... "));
  int state = radio1.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // initialize the second LoRa instance with
  // non-default settings
  // this LoRa link will have high data rate,
  // but lower range
  Serial.print(F("[SX1281] Initializing ... "));
  // carrier frequency:           2450.0 MHz
  // bandwidth:                   1625.0 kHz
  // spreading factor:            7
  // coding rate:                 5
  // sync word:                   0x12 (private network)
  // output power:                2 dBm
  // preamble length:             20 symbols
  state = radio2.begin(2450.0, 1625.0, 7, 5, 0x12, 2, 20);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // you can also change the settings at runtime
  // and check if the configuration was changed successfully

  // set carrier frequency to 2410.5 MHz
  if (radio1.setFrequency(2410.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true);
  }

  // set bandwidth to 203.125 kHz
  if (radio1.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    Serial.println(F("Selected bandwidth is invalid for this module!"));
    while (true);
  }

  // set spreading factor to 10
  if (radio1.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    while (true);
  }

  // set coding rate to 6
  if (radio1.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    while (true);
  }

  // set output power to -2 dBm
  if (radio1.setOutputPower(-2) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true);
  }

  // set LoRa preamble length to 16 symbols (accepted range is 2 - 65535)
  if (radio1.setPreambleLength(16) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
    Serial.println(F("Selected preamble length is invalid for this module!"));
    while (true);
  }

  // disable CRC
  if (radio1.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
    Serial.println(F("Selected CRC is invalid for this module!"));
    while (true);
  }

  Serial.println(F("All settings succesfully changed!"));
}

void loop() {
  // nothing here
}
