/*
  Example for REV outlets (e.g. 8342L)
  
  https://github.com/sui77/rc-switch/
  
  Need help? http://forum.ardumote.com
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // set pulse length.
  mySwitch.setPulseLength(360);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the channel (a, b, c, d)
  // The second parameter represents the device number
  // 
  // In this example it's family 'd', device #2 
  mySwitch.switchOn('d', 2);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff('d', 2);
  
  // Wait another second
  delay(1000);
  
}
