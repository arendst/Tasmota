/*
  RadioLib nRF24 Blocking Receive Example

  This example listens for FSK transmissions using nRF24 2.4 GHz radio module.
  To successfully receive data, the following settings have to be the same
  on both transmitter and receiver:
  - carrier frequency
  - data rate
  - transmit pipe on transmitter must match receive pipe on receiver

  Using blocking receive is not recommended, as it will lead
  to significant amount of timeouts, inefficient use of processor
  time and can some miss packets!
  Instead, interrupt receive is recommended.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#nrf24

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// nRF24 has the following connections:
// CS pin:    10
// IRQ pin:   2
// CE pin:    3
nRF24 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//nRF24 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize nRF24 with default settings
  Serial.print(F("[nRF24] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // set receive pipe 0 address
  // NOTE: address width in bytes MUST be equal to the
  //       width set in begin() or setAddressWidth()
  //       methods (5 by default)
  Serial.print(F("[nRF24] Setting address for receive pipe 0 ... "));
  byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
  state = radio.setReceivePipe(0, addr);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }
}

void loop() {
  Serial.print(F("[nRF24] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  int state = radio.receive(str);

  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */

  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));

    // print the data of the packet
    Serial.print(F("[nRF24] Data:\t\t"));
    Serial.println(str);

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
