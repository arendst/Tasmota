/*
   RadioLib RTTY Transmit AFSK Example

   This example sends RTTY message using SX1278's
   FSK modem. The data is modulated as AFSK.

   Other modules that can be used for RTTY:
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

// create RTTY client instance using the AFSK instance
RTTYClient rtty(&audio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules for RTTY
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize RTTY client
  // NOTE: Unlike FSK RTTY, AFSK requires no rounding of
  //       the frequency shift.
  Serial.print(F("[RTTY] Initializing ... "));
  // space frequency:             400 Hz
  // frequency shift:             170 Hz
  // baud rate:                   45 baud
  // encoding:                    ASCII (7-bit)
  // stop bits:                   1
  state = rtty.begin(400, 170, 45);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  /*
    // RadioLib also provides ITA2 ("Baudot") support
    rtty.begin(400, 170, 45, ITA2);

    // All transmissions in loop() (strings and numbers)
    // will now be encoded using ITA2 code

    // ASCII characters that do not have ITA2 equivalent
    // will be sent as NUL (including lower case letters!)
  */
}

void loop() {
  Serial.print(F("[RTTY] Sending RTTY data ... "));

  // send out idle condition for 500 ms
  rtty.idle();
  delay(500);

  // RTTYClient supports all methods of the Serial class

  // Arduino String class
  String aStr = "Arduino String";
  rtty.println(aStr);

  // character array (C-String)
  rtty.println("C-String");

  // string saved in flash
  rtty.println(F("Flash String"));

  // character
  rtty.println('c');

  // byte
  // formatting DEC/HEX/OCT/BIN is supported for
  // any integer type (byte/int/long)
  rtty.println(255, HEX);

  // integer number
  int i = 1000;
  rtty.println(i);

  // floating point number
  float f = -3.1415;
  rtty.println(f, 3);

  // turn the transmitter off
  rtty.standby();

  Serial.println(F("done!"));

  // wait for a second before transmitting again
  delay(1000);
}
