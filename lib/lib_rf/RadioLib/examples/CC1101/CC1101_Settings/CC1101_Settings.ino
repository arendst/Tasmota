/*
   RadioLib CC1101 Settings Example

   This example shows how to change all the properties of RF69 radio.
   RadioLib currently supports the following settings:
    - pins (SPI slave select, digital IO 0, digital IO 1)
    - carrier frequency
    - bit rate
    - receiver bandwidth
    - allowed frequency deviation
    - output power during transmission
    - sync word

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
CC1101 radio1 = new Module(10, 2, RADIOLIB_NC, 3);

// second CC1101 has different connections:
// CS pin:    9
// GDO0 pin:  4
// RST pin:   unused
// GDO2 pin:  5 (optional)
CC1101 radio2 = new Module(9, 4, RADIOLIB_NC, 5);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//CC1101 radio3 = RadioShield.ModuleB;

void setup() {
  Serial.begin(9600);

  // initialize CC1101 with default settings
  Serial.print(F("[CC1101] Initializing ... "));
  int state = radio1.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // initialize CC1101 with non-default settings
  Serial.print(F("[CC1101] Initializing ... "));
  // carrier frequency:                   434.0 MHz
  // bit rate:                            32.0 kbps
  // frequency deviation:                 60.0 kHz
  // Rx bandwidth:                        250.0 kHz
  // output power:                        7 dBm
  // preamble length:                     32 bits
  state = radio2.begin(434.0, 32.0, 60.0, 250.0, 7, 32);
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
    Serial.println(F("[CC1101] Selected frequency is invalid for this module!"));
    while (true);
  }

  // set bit rate to 100.0 kbps
  state = radio1.setBitRate(100.0);
  if (state == RADIOLIB_ERR_INVALID_BIT_RATE) {
    Serial.println(F("[CC1101] Selected bit rate is invalid for this module!"));
    while (true);
  } else if (state == RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO) {
    Serial.println(F("[CC1101] Selected bit rate to bandwidth ratio is invalid!"));
    Serial.println(F("[CC1101] Increase receiver bandwidth to set this bit rate."));
    while (true);
  }

  // set receiver bandwidth to 250.0 kHz
  if (radio1.setRxBandwidth(250.0) == RADIOLIB_ERR_INVALID_RX_BANDWIDTH) {
    Serial.println(F("[CC1101] Selected receiver bandwidth is invalid for this module!"));
    while (true);
  }

  // set allowed frequency deviation to 10.0 kHz
  if (radio1.setFrequencyDeviation(10.0) == RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION) {
    Serial.println(F("[CC1101] Selected frequency deviation is invalid for this module!"));
    while (true);
  }

  // set output power to 5 dBm
  if (radio1.setOutputPower(5) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("[CC1101] Selected output power is invalid for this module!"));
    while (true);
  }

  // 2 bytes can be set as sync word
  if (radio1.setSyncWord(0x01, 0x23) == RADIOLIB_ERR_INVALID_SYNC_WORD) {
    Serial.println(F("[CC1101] Selected sync word is invalid for this module!"));
    while (true);
  }

}

void loop() {
  // nothing here
}
