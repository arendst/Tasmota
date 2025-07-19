/*
   RadioLib Pager (POCSAG) Receive Example

   This example shows how to receive FSK packets without using
   SX127x packet engine.

   This example receives POCSAG messages using SX1278's
   FSK modem in direct mode.

   Other modules that can be used to receive POCSAG:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - Si443x/RFM2x

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

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

// receiving packets requires connection
// to the module direct output pin,
// here connected to Arduino pin 5
// SX127x/RFM9x:  DIO2
// RF69:          DIO2
// SX1231:        DIO2
// CC1101:        GDO2
// Si443x/RFM2x:  GPIO
// SX126x/LLCC68: DIO2
const int pin = 5;

// create Pager client instance using the FSK module
PagerClient pager(&radio);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();

  // when using one of the non-LoRa modules
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // initialize Pager client
  Serial.print(F("[Pager] Initializing ... "));
  // base (center) frequency:     434.0 MHz
  // speed:                       1200 bps
  state = pager.begin(434.0, 1200);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // start receiving POCSAG messages
  Serial.print(F("[Pager] Starting to listen ... "));
  // address of this "pager":     1234567
  state = pager.startReceive(pin, 1234567);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

}

void loop() {
  // the number of batches to wait for
  // 2 batches will usually be enough to fit short and medium messages
  if (pager.available() >= 2) {
    Serial.print(F("[Pager] Received pager data, decoding ... "));

    // you can read the data as an Arduino String
    String str;
    int state = pager.readData(str);

    // you can also receive data as byte array
    /*
      byte byteArr[8];
      size_t numBytes = 0;
      int state = radio.receive(byteArr, &numBytes);
    */

    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));

      // print the received data
      Serial.print(F("[Pager] Data:\t"));
      Serial.println(str);

    } else {
      // some error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);

    }
  }
}
