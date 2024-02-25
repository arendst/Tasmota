/*
   RadioLib RF69 Settings Example

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
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#rf69sx1231

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// RF69 has the following connections:
// CS pin:    10
// DIO0 pin:  2
// RESET pin: 3
RF69 radio1 = new Module(10, 2, 3);

// second CC1101 has different connections:
// CS pin:    9
// DIO0 pin:  4
// RESET pin: 5
RF69 radio2 = new Module(9, 4, 5);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//RF69 radio3 = RadioShield.ModuleB;

void setup() {
  Serial.begin(9600);

  // initialize RF69 with default settings
  Serial.print(F("[RF69] Initializing ... "));
  int state = radio1.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // initialize RF69 with non-default settings
  Serial.print(F("[RF69] Initializing ... "));
  // carrier frequency:                   868.0 MHz
  // bit rate:                            300.0 kbps
  // frequency deviation:                 60.0 kHz
  // Rx bandwidth:                        250.0 kHz
  // output power:                        17 dBm
  // preamble length:                     32 bits
  state = radio2.begin(868.0, 300.0, 60.0, 250.0, 17, 32);
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
    Serial.println(F("[RF69] Selected frequency is invalid for this module!"));
    while (true);
  }

  // set bit rate to 100.0 kbps
  state = radio1.setBitRate(100.0);
  if (state == RADIOLIB_ERR_INVALID_BIT_RATE) {
    Serial.println(F("[RF69] Selected bit rate is invalid for this module!"));
    while (true);
  } else if (state == RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO) {
    Serial.println(F("[RF69] Selected bit rate to bandwidth ratio is invalid!"));
    Serial.println(F("[RF69] Increase receiver bandwidth to set this bit rate."));
    while (true);
  }

  // set receiver bandwidth to 250.0 kHz
  state = radio1.setRxBandwidth(250.0);
  if (state == RADIOLIB_ERR_INVALID_RX_BANDWIDTH) {
    Serial.println(F("[RF69] Selected receiver bandwidth is invalid for this module!"));
    while (true);
  } else if (state == RADIOLIB_ERR_INVALID_BIT_RATE_BW_RATIO) {
    Serial.println(F("[RF69] Selected bit rate to bandwidth ratio is invalid!"));
    Serial.println(F("[RF69] Decrease bit rate to set this receiver bandwidth."));
    while (true);
  }

  // set allowed frequency deviation to 10.0 kHz
  if (radio1.setFrequencyDeviation(10.0) == RADIOLIB_ERR_INVALID_FREQUENCY_DEVIATION) {
    Serial.println(F("[RF69] Selected frequency deviation is invalid for this module!"));
    while (true);
  }

  // set output power to 2 dBm
  if (radio1.setOutputPower(2) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.println(F("[RF69] Selected output power is invalid for this module!"));
    while (true);
  }

  // up to 8 bytes can be set as sync word
  // NOTE: sync word must not contain any zero bytes
  // set sync word to 0x0123456789ABCDEF
  uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  if (radio1.setSyncWord(syncWord, 8) == RADIOLIB_ERR_INVALID_SYNC_WORD) {
    Serial.println(F("[RF69] Selected sync word is invalid for this module!"));
    while (true);
  }

  Serial.println(F("[RF69] All settings changed successfully!"));

  // RF69 can also measure temperature (roughly)
  // to get correct temperature measurements, the sensor must be calibrated
  // at ambient temperature
  radio1.setAmbientTemperature(25); // replace 25 with your ambient temperature
}

void loop() {
  // measure temperature
  Serial.print(F("[RF69] Measured temperature: "));
  Serial.print(radio1.getTemperature());
  Serial.println(F(" deg C"));

  // wait 100 ms before the next measurement
  delay(100);
}
