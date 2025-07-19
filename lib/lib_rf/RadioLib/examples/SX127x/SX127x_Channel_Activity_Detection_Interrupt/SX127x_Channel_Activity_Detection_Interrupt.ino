/*
   RadioLib SX127x Channel Activity Detection with Interrupts Example

   This example scans the current LoRa channel and detects
   valid LoRa preambles. Preamble is the first part of
   LoRa transmission, so this can be used to check
   if the LoRa channel is free, or if you should start
   receiving a message.

   Other modules from SX127x/RFM9x family can also be used.

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

// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;

void setup() {
  // Serial port speed must be high enough for this example
  Serial.begin(115200);

  // initialize SX1278 with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set the function that will be called
  // when LoRa preamble is not detected within CAD timeout period
  radio.setDio0Action(setFlagTimeout, RISING);

  // set the function that will be called
  // when LoRa preamble is detected
  radio.setDio1Action(setFlagDetected, RISING);

  // start scanning the channel
  Serial.print(F("[SX1278] Starting scan for LoRa preamble ... "));
  state = radio.startChannelScan();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}

// flag to indicate that a preamble was not detected
volatile bool timeoutFlag = false;

// flag to indicate that a preamble was detected
volatile bool detectedFlag = false;

// this function is called when no preamble
// is detected within timeout period
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlagTimeout(void) {
  // we timed out, set the flag
  timeoutFlag = true;
}

// this function is called when LoRa preamble
// is detected within timeout period
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlagDetected(void) {
  // we got a preamble, set the flag
  detectedFlag = true;
}

void loop() {
  // check if we need to restart channel activity detection
  if(detectedFlag || timeoutFlag) {
    // check if we got a preamble
    if(detectedFlag) {
      // LoRa preamble was detected
      Serial.println(F("[SX1278] Preamble detected!"));
    } else {
      // nothing was detected
      Serial.println(F("[SX1278] Channel free!"));
    }
    
    // start scanning the channel
    Serial.print(F("[SX1278] Starting scan for LoRa preamble ... "));

    // start scanning current channel
    int state = radio.startChannelScan();
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }

    // reset flags
    timeoutFlag = false;
    detectedFlag = false;
  }
}
