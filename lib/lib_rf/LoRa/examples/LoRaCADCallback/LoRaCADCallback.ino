#include <SPI.h>
#include "LoRa.h"

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // register the channel activity dectection callback
  LoRa.onCadDone(onCadDone);
  // register the receive callback
  LoRa.onReceive(onReceive);
  // put the radio into CAD mode
  LoRa.channelActivityDetection();
}

void loop() {
  // do nothing
}

void onCadDone(boolean signalDetected) {
  // detect preamble
  if (signalDetected) {
    Serial.println("Signal detected");
    // put the radio into continuous receive mode
    LoRa.receive();
  } else {
    // try next activity dectection
    LoRa.channelActivityDetection();
  }
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());

  // put the radio into CAD mode
  LoRa.channelActivityDetection();
}
