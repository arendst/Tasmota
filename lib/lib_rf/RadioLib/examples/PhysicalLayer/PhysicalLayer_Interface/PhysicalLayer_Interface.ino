/*
   RadioLib PhysicalLayer Interface Example

   This example shows how to use the common PhysicalLayer
   to interface with different radio modules using the same
   methods.

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// select which radio to use
// this can be any radio supported by RadioLib!
#define RADIO_TYPE      SX1278

// set the pinout depending on the wiring and module type
// SPI NSS pin:               10
// interrupt pin:             2
// reset pin:                 9 (unused on some modules)
// extra GPIO/interrupt pin:  3 (unused on some modules)
RADIO_TYPE radio = new Module(10, 2, 9, 3);

// get pointer to the common layer
PhysicalLayer* phy = (PhysicalLayer*)&radio;

void dummyISR(void) {
  // nothing here, this example is just a showcase
}

void setup() {
  Serial.begin(9600);

  // now we can use "radio" to access the features
  // specific to that radio type, such as the begin() method
  Serial.print(F("[Radio] Initializing ... "));
  int state = radio.begin();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // or we can use the "phy" pointer to access the common layer
  // PhysicalLayer has some common configuration
  Serial.print(F("[PHY] Changing frequency ... "));
  state = phy->setFrequency(433.5);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // PhysicalLayer also contains basic functionality
  // like transmitting and receiving packets
  Serial.print(F("[PHY] Sending packet ... "));
  state = phy->transmit("Hello World!");
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // try to receive now - this will almost certainly timeout
  // unless by chance there is a transmitter nearby,
  // but the point of this example is to showcase the interface
  String str;
  Serial.print(F("[PHY] Listening for packets ... "));
  state = phy->receive(str);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else if(state == RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.println(F("timeout!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // interrupt-driven versions of Rx/Tx are supported as well
  // for these to work, you have to configure the interrupt actions
  phy->setPacketReceivedAction(dummyISR);
  phy->setPacketSentAction(dummyISR);

  // now you can use methods like startTransmit(), startReceive(),
  // readData() etc.

  // interrupt actions can be cleared as well
  phy->clearPacketReceivedAction();
  phy->clearPacketSentAction();

  // PhysicalLayer supports basic mode changes like sleep ...
  Serial.print(F("[PHY] Going to sleep ... "));
  state = phy->sleep();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // ... or standby
  Serial.print(F("[PHY] Going to standby ... "));
  state = phy->standby();
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while(true);
  }

  // there are also common SNR/RSSI measurement functions
  Serial.print(F("[PHY] Measured SNR = "));
  Serial.print(phy->getSNR());
  Serial.println(F(" dB"));
  Serial.print(F("[PHY] Measured RSSI = "));
  Serial.print(phy->getRSSI());
  Serial.println(F(" dBm"));

  // and also a true random number generator
  Serial.print(F("[PHY] Random number between 0 and 100 = "));
  Serial.println(phy->random(100));
}

void loop() {
  // nothing here, the example is just a showcase
}
