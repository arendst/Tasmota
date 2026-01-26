/*
   RadioLib Morse Transmit AFSK Example

   This example sends Morse code message using
   SX1278's FSK modem. The signal is modulated
   as AFSK, and may be demodulated in FM mode.

   Other modules that can be used for Morse Code
   with AFSK modulation:
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

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

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

// create Morse client instance using the AFSK instance
MorseClient morse(&audio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for Morse code
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize Morse client
  Serial.print(F("[Morse] Initializing ... "));
  // tone frequency:              400 Hz
  // speed:                       20 words per minute
  state = morse.begin(400);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[Morse] Sending Morse data ... "));

  // MorseClient supports all methods of the Serial class
  // NOTE: Characters that do not have ITU-R M.1677-1
  //       representation will not be sent! Lower case
  //       letters will be capitalized.

  // send start signal first
  morse.startSignal();

  // Arduino String class
  String aStr = "Arduino String";
  morse.print(aStr);

  // character array (C-String)
  morse.print("C-String");

  // string saved in flash
  morse.print(F("Flash String"));

  // character
  morse.print('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  morse.print(255, HEX);

  // integer number
  int i = 1000;
  morse.print(i);

  // floating point number
  // NOTE: When using println(), the transmission will be
  //       terminated with end-of-work signal (...-.-).
  float f = -3.1415;
  morse.println(f, 3);

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
