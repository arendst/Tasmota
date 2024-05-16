/*
   RadioLib AFSK Imperial March Example

   This example shows how to EXECUTE ORDER 66

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

// include the melody
#include "melody.h"

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
  // (RF69, CC1101,, Si4432 etc.), use the basic begin() method
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
  Serial.print(F("[AFSK] Executing Order 66 ... "));

  // calculate whole note duration
  int wholenote = (60000 * 4) / 120;

  // iterate over the melody
  for(unsigned int note = 0; note < sizeof(melody) / sizeof(melody[0]); note += 2) {
    // calculate the duration of each note
    int noteDuration = 0;
    int divider = melody[note + 1];
    if(divider > 0) {
      // regular note, just proceed
      noteDuration = wholenote / divider;
    } else if(divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = wholenote / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    audio.tone(melody[note]);
    delay(noteDuration*0.9);
    audio.noTone();
    delay(noteDuration*0.1);
  }

  Serial.println(F("done!"));

  // wait for a second
  delay(1000);
}
