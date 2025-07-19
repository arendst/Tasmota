/*
   RadioLib AX.25 Frame Example

   This example shows how to send various
   AX.25 frames using SX1278's FSK modem.

   Other modules that can be used for AX.25:
    - SX127x/RFM9x
    - RF69
    - SX1231
    - CC1101
    - SX126x
    - nRF24
    - Si443x/RFM2x

   Using raw AX.25 frames requires some
   knowledge of the protocol, refer to
   AX25_Transmit for basic operation.
   Frames shown in this example are not
   exhaustive; all possible AX.25 frames
   should be supported.

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

// create AX.25 client instance using the FSK module
AX25Client ax25(&radio);

void setup() {
  Serial.begin(9600);

  // initialize SX1278
  Serial.print(F("[SX1278] Initializing ... "));
  // carrier frequency:           434.0 MHz
  // bit rate:                    1.2 kbps (1200 baud 2-FSK AX.25)
  // frequency deviation:         0.5 kHz  (1200 baud 2-FSK AX.25)
  int state = radio.beginFSK(434.0, 1.2, 0.5);

  // when using one of the non-LoRa modules for AX.25
  // (RF69, CC1101, Si4432 etc.), use the basic begin() method
  // int state = radio.begin();

  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // initialize AX.25 client
  Serial.print(F("[AX.25] Initializing ... "));
  // source station callsign:     "N7LEM"
  // source station SSID:         0
  // preamble length:             8 bytes
  state = ax25.begin("N7LEM");
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  // create AX.25 Unnumbered Information frame
  // destination station callsign:     "NJ7P"
  // destination station SSID:         0
  // source station callsign:          "N7LEM"
  // source station SSID:              0
  // control field:                    UI, P/F not used, unnumbered frame
  // protocol identifier:              no layer 3 protocol implemented
  // information field:                "Hello World!"
  AX25Frame frameUI("NJ7P", 0, "N7LEM", 0, RADIOLIB_AX25_CONTROL_U_UNNUMBERED_INFORMATION |
                    RADIOLIB_AX25_CONTROL_POLL_FINAL_DISABLED | RADIOLIB_AX25_CONTROL_UNNUMBERED_FRAME,
                    RADIOLIB_AX25_PID_NO_LAYER_3, "Hello World (unnumbered)!");

  // send the frame
  Serial.print(F("[AX.25] Sending UI frame ... "));
  int state = ax25.sendFrame(&frameUI);
  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else {
    // some error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  delay(1000);

  // create AX.25 Receive Ready frame
  // destination station callsign:     "NJ7P"
  // destination station SSID:         0
  // source station callsign:          "N7LEM"
  // source station SSID:              0
  // control field:                    RR, P/F not used, supervisory frame
  AX25Frame frameRR("NJ7P", 0, "N7LEM", 0, RADIOLIB_AX25_CONTROL_S_RECEIVE_READY |
                    RADIOLIB_AX25_CONTROL_POLL_FINAL_DISABLED | RADIOLIB_AX25_CONTROL_SUPERVISORY_FRAME);

  // set receive sequence number (0 - 7)
  frameRR.setRecvSequence(0);

  // send the frame
  Serial.print(F("[AX.25] Sending RR frame ... "));
  state = ax25.sendFrame(&frameRR);
  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else {
    // some error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  delay(1000);

  // create AX.25 Information frame
  // destination station callsign:     "NJ7P"
  // destination station SSID:         0
  // source station callsign:          "N7LEM"
  // source station SSID:              0
  // control field:                    P/F not used, information frame
  // protocol identifier:              no layer 3 protocol implemented
  // information field:                "Hello World (numbered)!"
  AX25Frame frameI("NJ7P", 0, "N7LEM", 0, RADIOLIB_AX25_CONTROL_POLL_FINAL_DISABLED |
                   RADIOLIB_AX25_CONTROL_INFORMATION_FRAME, RADIOLIB_AX25_PID_NO_LAYER_3,
                   "Hello World (numbered)!");

  // set receive sequence number (0 - 7)
  frameI.setRecvSequence(0);

  // set send sequence number (0 - 7)
  frameI.setSendSequence(0);

  // send the frame
  Serial.print(F("[AX.25] Sending I frame ... "));
  state = ax25.sendFrame(&frameI);
  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else {
    // some error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }

  delay(1000);
}
