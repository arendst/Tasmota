/*
   RadioLib SX127x Receive after Channel Activity Detection Example

   This example scans the current LoRa channel and detects
   valid LoRa preambles. Preamble is the first part of
   LoRa transmission, so this can be used to check
   if the LoRa channel is free, or if you should start
   receiving a message. If a preamble is detected,
   the module will switch to receive mode and receive the packet.
   
   For most use-cases, it should be enough to just use the
   interrupt-driven reception described in the example
   "SX127x_Receive_Interrupt".

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
  // or when a packet is received
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

// flag to indicate if we are currently receiving
bool receiving = false;

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
    int state = RADIOLIB_ERR_NONE;

    // check ongoing reception
    if(receiving) {
      // DIO triggered while reception is ongoing
      // that means we got a packet

      // reset flags first
      detectedFlag = false;
      timeoutFlag = false;

      // you can read received data as an Arduino String
      String str;
      state = radio.readData(str);
  
      // you can also read received data as byte array
      /*
        byte byteArr[8];
        state = radio.readData(byteArr, 8);
      */
    
      if (state == RADIOLIB_ERR_NONE) {
        // packet was successfully received
        Serial.println(F("[SX1278] Received packet!"));
  
        // print data of the packet
        Serial.print(F("[SX1278] Data:\t\t"));
        Serial.println(str);
  
        // print RSSI (Received Signal Strength Indicator)
        Serial.print(F("[SX1278] RSSI:\t\t"));
        Serial.print(radio.getRSSI());
        Serial.println(F(" dBm"));
  
        // print SNR (Signal-to-Noise Ratio)
        Serial.print(F("[SX1278] SNR:\t\t"));
        Serial.print(radio.getSNR());
        Serial.println(F(" dB"));
  
        // print frequency error
        Serial.print(F("[SX1278] Frequency error:\t"));
        Serial.print(radio.getFrequencyError());
        Serial.println(F(" Hz"));
  
      } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
        // packet was received, but is malformed
        Serial.println(F("[SX1278] CRC error!"));
  
      } else {
        // some other error occurred
        Serial.print(F("[SX1278] Failed, code "));
        Serial.println(state);
  
      }

      // reception is done now
      receiving = false;
      
    }

    // check if we got a preamble
    if(detectedFlag) {
      // LoRa preamble was detected
      Serial.print(F("[SX1278] Preamble detected, starting reception ... "));
      state = radio.startReceive(0, RADIOLIB_SX127X_RXSINGLE);
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
      } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
      }

      // set the flag for ongoing reception
      receiving = true;
    
    } else if(!receiving) {
      // nothing was detected
      // do not print anything, it just spams the console
    
    }

    // if we're not receiving, start scanning again
    if(!receiving) {
      int state = radio.startChannelScan();
      if (state != RADIOLIB_ERR_NONE) {
        Serial.print(F("[SX1278] Starting new scan failed, code "));
        Serial.println(state);
      }
    
    }

    // reset flags
    timeoutFlag = false;
    detectedFlag = false;
  }
}
