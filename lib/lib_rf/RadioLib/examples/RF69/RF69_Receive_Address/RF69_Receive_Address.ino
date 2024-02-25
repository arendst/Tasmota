/*
   RadioLib RF69 Receive with Address Example

   This example receives packets using RF69 FSK radio module.
   Packets can have 1-byte address of the destination node.
   After setting node (or broadcast) address, this node will
   automatically filter out any packets that do not contain
   either node address or broadcast address.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#rf69sx1231

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// RF69 has the following connections:
// CS pin:    10
// DIO0 pin:  2
// RESET pin: 3
RF69 radio = new Module(10, 2, 3);

// or using RadioShield
// https://github.com/jgromes/RadioShield
//RF69 radio = RadioShield.ModuleA;

void setup() {
  Serial.begin(9600);

  // initialize RF69 with default settings
  Serial.print(F("[RF69] Initializing ... "));
  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set node address
  // NOTE: calling this method will automatically enable
  // address filtering (node address only)
  Serial.print(F("[RF69] Setting node address ... "));
  state = radio.setNodeAddress(0x02);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // set broadcast address
  // NOTE: calling this method will automatically enable
  //       address filtering (node or broadcast address)
  Serial.print(F("[RF69] Setting broadcast address ... "));
  state = radio.setBroadcastAddress(0xFF);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // address filtering can also be disabled
  // NOTE: calling this method will also erase previously set
  // node and broadcast address
  /*
    Serial.print(F("[RF69] Disabling address filtering ... "));
    state == radio.disableAddressFiltering();
    if(state == RADIOLIB_ERR_NONE) {
      Serial.println(F("success!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
      while(true);
    }
  */
}

void loop() {
  Serial.print(F("[RF69] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
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
    Serial.print(F("[RF69] Data:\t\t"));
    Serial.println(str);

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    Serial.println(F("timeout!"));

  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);

  }
}
