/*
   RadioLib SX127x FSK Modem Example

   This example shows how to use FSK modem in SX127x chips.

   NOTE: The sketch below is just a guide on how to use
         FSK modem, so this code should not be run directly!
         Instead, modify the other examples to use FSK
         modem and use the appropriate configuration
         methods.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---fsk-modem

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
  Serial.begin(9600);

  // initialize SX1278 FSK modem with default settings
  Serial.print(F("[SX1278] Initializing ... "));
  int state = radio.beginFSK();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }

  // if needed, you can switch between LoRa and FSK modes
  //
  // radio.begin()       start LoRa mode (and disable FSK)
  // radio.beginFSK()    start FSK mode (and disable LoRa)

  // the following settings can also
  // be modified at run-time
  state = radio.setFrequency(433.5);
  state = radio.setBitRate(100.0);
  state = radio.setFrequencyDeviation(10.0);
  state = radio.setRxBandwidth(250.0);
  state = radio.setOutputPower(10.0);
  state = radio.setCurrentLimit(100);
  state = radio.setDataShaping(RADIOLIB_SHAPING_0_5);
  uint8_t syncWord[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
  state = radio.setSyncWord(syncWord, 8);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Unable to set configuration, code "));
    Serial.println(state);
    while (true);
  }

  // FSK modulation can be changed to OOK
  // NOTE: When using OOK, the maximum bit rate is only 32.768 kbps!
  //       Also, data shaping changes from Gaussian filter to
  //       simple filter with cutoff frequency. Make sure to call
  //       setDataShapingOOK() to set the correct shaping!
  state = radio.setOOK(true);
  state = radio.setDataShapingOOK(1);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Unable to change modulation, code "));
    Serial.println(state);
    while (true);
  }

  #warning "This sketch is just an API guide! Read the note at line 6."
}

void loop() {
  // FSK modem can use the same transmit/receive methods
  // as the LoRa modem, even their interrupt-driven versions
  // NOTE: FSK modem maximum packet length is 63 bytes!

  // transmit FSK packet
  int state = radio.transmit("Hello World!");
  /*
    byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                      0x89, 0xAB, 0xCD, 0xEF};
    int state = radio.transmit(byteArr, 8);
  */
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Packet transmitted successfully!"));
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println(F("[SX1278] Packet too long!"));
  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    Serial.println(F("[SX1278] Timed out while transmitting!"));
  } else {
    Serial.println(F("[SX1278] Failed to transmit packet, code "));
    Serial.println(state);
  }

  // receive FSK packet
  String str;
  state = radio.receive(str);
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Received packet!"));
    Serial.print(F("[SX1278] Data:\t"));
    Serial.println(str);
  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("[SX1278] Timed out while waiting for packet!"));
  } else {
    Serial.println(F("[SX1278] Failed to receive packet, code "));
    Serial.println(state);
  }

  // FSK modem has built-in address filtering system
  // it can be enabled by setting node address, broadcast
  // address, or both
  //
  // to transmit packet to a particular address,
  // use the following methods:
  //
  // radio.transmit("Hello World!", address);
  // radio.startTransmit("Hello World!", address);

  // set node address to 0x02
  state = radio.setNodeAddress(0x02);
  // set broadcast address to 0xFF
  state = radio.setBroadcastAddress(0xFF);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Unable to set address filter, code "));
    Serial.println(state);
  }

  // address filtering can also be disabled
  // NOTE: calling this method will also erase previously set
  //       node and broadcast address
  /*
    state = radio.disableAddressFiltering();
    if (state != RADIOLIB_ERR_NONE) {
      Serial.println(F("Unable to remove address filter, code "));
    }
  */

  // FSK modem supports direct data transmission
  // in this mode, SX127x directly transmits any data
  // sent to DIO1 (data) and DIO2 (clock)

  // activate direct mode transmitter
  state = radio.transmitDirect();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Unable to start direct transmission mode, code "));
    Serial.println(state);
  }

  // using the direct mode, it is possible to transmit
  // FM notes with Arduino tone() function

  // it is recommended to set data shaping to 0
  // (no shaping) when transmitting audio
  state = radio.setDataShaping(0.0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Unable to set data shaping, code "));
    Serial.println(state);
  }

  // transmit FM tone at 1000 Hz for 1 second, then 500 Hz for 1 second
  // (DIO2 is connected to Arduino pin 4)
  // Note: tone() function is not available on Arduino Due and CubeCell
  //       on these platforms, the following will do nothing
  #if !defined(RADIOLIB_TONE_UNSUPPORTED)
  tone(4, 1000);
  delay(1000);
  tone(4, 500);
  delay(1000);
  noTone(4);
  #endif

  // NOTE: after calling transmitDirect(), SX127x will start
  // transmitting immediately! This signal can jam other
  // devices at the same frequency, it is up to the user
  // to disable it with standby() method!

  // direct mode transmissions can also be received
  // as bit stream on DIO1 (data) and DIO2 (clock)
  state = radio.receiveDirect();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.println(F("[SX1278] Unable to start direct reception mode, code "));
    Serial.println(state);
  }

  // NOTE: you will not be able to send or receive packets
  // while direct mode is active! to deactivate it, call method
  // radio.packetMode()
}
