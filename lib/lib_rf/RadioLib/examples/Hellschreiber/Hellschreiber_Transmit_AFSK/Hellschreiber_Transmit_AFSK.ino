/*
   RadioLib Hellschreiber Transmit AFSK Example

   This example sends Hellschreiber message using
   SX1278's FSK modem. The data is modulated
   as AFSK.

   Other modules that can be used for Hellschreiber
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

// create Hellschreiber client instance using the AFSK instance
HellClient hell(&audio);

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

  // initialize Hellschreiber client
  Serial.print(F("[Hell] Initializing ... "));
  // AFSK tone frequency:         400 Hz
  // speed:                       122.5 Baud ("Feld Hell")
  state = hell.begin(400);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[Hell] Sending Hellschreiber data ... "));

  // HellClient supports all methods of the Serial class
  // NOTE: Lower case letter will be capitalized.

  // Arduino String class
  String aStr = "Arduino String";
  hell.print(aStr);

  // character array (C-String)
  hell.print("C-String");

  // string saved in flash
  hell.print(F("Flash String"));

  // in AFSK mode, it is possible to invert the text colors
  // use white text on black background
  hell.setInversion(true);
  hell.print("Inverted String");
  hell.setInversion(false);

  // character
  hell.print('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  hell.print(255, HEX);

  // integer number
  int i = 1000;
  hell.print(i);

  // floating point number
  // NOTE: println() has no effect on the transmission,
  //       and is only kept for compatibility reasons.
  float f = -3.1415;
  hell.println(f, 3);

  // custom glyph - must be a 7 byte array of rows 7 pixels long
  uint8_t customGlyph[] = { 0b0000000, 0b0010100, 0b0010100, 0b0000000, 0b0100010, 0b0011100, 0b0000000 };
  hell.printGlyph(customGlyph);

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
