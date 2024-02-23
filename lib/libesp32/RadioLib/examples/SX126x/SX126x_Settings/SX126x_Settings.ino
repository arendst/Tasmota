/*
   RadioLib SX126x Settings Example

   This example shows how to change all the properties of LoRa transmission.
   RadioLib currently supports the following settings:
    - pins (SPI slave select, DIO1, DIO2, BUSY pin)
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word
    - output power during transmission
    - CRC
    - preamble length
    - TCXO voltage
    - DIO2 RF switch control

   Other modules from SX126x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// SX1262 has the following connections:
// NSS pin:   10
// DIO1 pin:  2
// NRST pin:  3
// BUSY pin:  9
SX1262 radio1 = new Module(10, 2, 3, 9);

// SX12628 has different connections:
// NSS pin:   8
// DIO1 pin:  4
// NRST pin:  5
// BUSY pin:  6
SX1268 radio2 = new Module(8, 4, 5, 6);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1261 radio3 = RadioShield.ModuleB;

// or using CubeCell
//SX1262 radio = new Module(RADIOLIB_BUILTIN_MODULE);

void setup() {
  Serial.begin(9600);

  // initialize SX1268 with default settings
  Serial.print(F("[SX1262] Initializing ... "));
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
  Serial.print(F("[SX1268] Initializing ... "));
  // carrier frequency:           915.0 MHz
  // bandwidth:                   500.0 kHz
  // spreading factor:            6
  // coding rate:                 5
  // sync word:                   0x34 (public network/LoRaWAN)
  // output power:                2 dBm
  // preamble length:             20 symbols
  state = radio2.begin(915.0, 500.0, 6, 5, 0x34, 2, 20);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // you can also change the settings at runtime
  // and check if the configuration was changed successfully

  // set carrier frequency to 433.5 MHz
  if (radio1.setFrequency(433.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true);
  }

  // set bandwidth to 250 kHz
  if (radio1.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
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

  // set LoRa sync word to 0xAB
  if (radio1.setSyncWord(0xAB) != RADIOLIB_ERR_NONE) {
    Serial.println(F("Unable to set sync word!"));
    while (true);
  }

  // set output power to 10 dBm (accepted range is -17 - 22 dBm)
  if (radio1.setOutputPower(10) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("Selected output power is invalid for this module!"));
    while (true);
  }

  // set over current protection limit to 80 mA (accepted range is 45 - 240 mA)
  // NOTE: set value to 0 to disable overcurrent protection
  if (radio1.setCurrentLimit(80) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
    Serial.println(F("Selected current limit is invalid for this module!"));
    while (true);
  }

  // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
  if (radio1.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
    Serial.println(F("Selected preamble length is invalid for this module!"));
    while (true);
  }

  // disable CRC
  if (radio1.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
    Serial.println(F("Selected CRC is invalid for this module!"));
    while (true);
  }

  // Some SX126x modules have TCXO (temperature compensated crystal
  // oscillator). To configure TCXO reference voltage,
  // the following method can be used.
  if (radio1.setTCXO(2.4) == RADIOLIB_ERR_INVALID_TCXO_VOLTAGE) {
    Serial.println(F("Selected TCXO voltage is invalid for this module!"));
    while (true);
  }

  // Some SX126x modules use DIO2 as RF switch. To enable
  // this feature, the following method can be used.
  // NOTE: As long as DIO2 is configured to control RF switch,
  //       it can't be used as interrupt pin!
  if (radio1.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE) {
    Serial.println(F("Failed to set DIO2 as RF switch!"));
    while (true);
  }

  Serial.println(F("All settings succesfully changed!"));
}

void loop() {
  // nothing here
}
