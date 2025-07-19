/*
   RadioLib AFSK External Radio example

   This example shows how to use your Arduino
   as modulator for an external analogue FM radio.
   
   The example sends APRS position reports with
   audio modulated as AFSK at 1200 baud using
   Bell 202 tones. However, any other AFSK
   protocol (RTTY, SSTV, etc.) may be used as well.

   DO NOT transmit in APRS bands unless
   you have a ham radio license!

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// create a dummy radio module
ExternalRadio radio;

// create AFSK client instance using the external radio
// pin 5 is connected to the radio sound input
AFSKClient audio(&radio, 5);

// create AX.25 client instance using the AFSK instance
AX25Client ax25(&audio);

// create APRS client instance using the AX.25 client
APRSClient aprs(&ax25);

void setup() {
  Serial.begin(9600);

  // initialize AX.25 client
  Serial.print(F("[AX.25] Initializing ... "));
  // source station callsign:     "N7LEM"
  // source station SSID:         0
  // preamble length:             8 bytes
  int16_t state = ax25.begin("N7LEM");
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize APRS client
  Serial.print(F("[APRS] Initializing ... "));
  // symbol:                      '>' (car)
  state = aprs.begin('>');
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[APRS] Sending position ... "));
  
  // send a location without message or timestamp
  int state = aprs.sendPosition("N0CALL", 0, "4911.67N", "01635.96E");
  delay(500);
  
  // send a location with message and without timestamp
  state |= aprs.sendPosition("N0CALL", 0, "4911.67N", "01635.96E", "I'm here!");
  delay(500);
  
  // send a location with message and timestamp
  state |= aprs.sendPosition("N0CALL", 0, "4911.67N", "01635.96E", "I'm here!", "093045z");
  delay(500);

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait one minute before transmitting again
  delay(60000);
}
