/*
   RadioLib SX127x Morse Receive AM Example

   This example receives Morse code message using
   SX1278's FSK modem. The signal is expected to be
   modulated as OOK, to be demodulated in AM mode.

   Other modules that can be used for Morse Code
   with AFSK modulation:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - Si443x/RFM2x

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

// create AFSK client instance using the FSK module
// pin 5 is connected to SX1278 DIO2
AFSKClient audio(&radio, 5);

// create Morse client instance using the AFSK instance
MorseClient morse(&audio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // when using one of the non-LoRa modules for Morse code
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  // initialize Morse client
  Serial.print(F("[Morse] Initializing ... "));
  // AFSK tone frequency:         400 Hz
  // speed:                       20 words per minute
  state = morse.begin(400);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // after that, set mode to OOK to emulate AM modulation
  Serial.print(F("[SX1278] Switching to OOK ... "));
  state = radio.setOOK(true);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // start direct mode reception
  radio.receiveDirect();
}

// save symbol and length between loops
byte symbol = 0;
byte len = 0;

void loop() {
  // try to read a new symbol
  int state = morse.read(&symbol, &len);

  // check if we have something to decode
  if(state != RADIOLIB_MORSE_INTER_SYMBOL) {
    // decode and print
    Serial.print(MorseClient::decode(symbol, len));

    // reset the symbol buffer
    symbol = 0;
    len = 0;

    // check if we have a complete word
    if(state == RADIOLIB_MORSE_WORD_COMPLETE) {
      // inter-word space, interpret that as a new line
      Serial.println();
    }
  
  }

}
