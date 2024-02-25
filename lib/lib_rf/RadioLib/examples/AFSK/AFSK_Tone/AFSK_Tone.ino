/*
   RadioLib AFSK Example

   This example shows hot to send audio FSK tones
   using SX1278's FSK modem.

   Other modules that can be used for AFSK:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - Si443x/RFM2x
    - SX126x/LLCC68 (only devices without TCXO!)

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

// create AFSK client instance using the FSK module
// this requires connection to the module direct
// input pin, here connected to Arduino pin 5
// SX127x/RFM9x:  DIO2
// RF69:          DIO2
// SX1231:        DIO2
// CC1101:        GDO2
// Si443x/RFM2x:  GPIO
// SX126x/LLCC68: DIO2
AFSKClient audio(&radio, 5);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for AFSK
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize AFSK client
  Serial.print(F("[AFSK] Initializing ... "));
  state = audio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  // AFSKClient can be used to transmit tones,
  // same as Arduino tone() function
  
  // 400 Hz tone
  Serial.print(F("[AFSK] 400 Hz tone ... "));
  audio.tone(400);
  delay(1000);

  // silence
  Serial.println(F("done!"));
  audio.noTone();
  delay(1000);

  // AFSKClient can also be used to transmit HAM-friendly
  // RTTY, Morse code, Hellschreiber, SSTV and AX.25.
  // Details on how to use AFSK are in the example
  // folders for each of the above modes.
}
