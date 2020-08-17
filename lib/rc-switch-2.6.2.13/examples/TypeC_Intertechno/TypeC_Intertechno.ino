/*
  Example for Intertechno outlets
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
}

void loop() {

  // Switch on:
  // The first parameter represents the familycode (a, b, c, ... f)
  // The second parameter represents the group number
  // The third parameter represents the device number
  // 
  // In this example it's family 'b', group #3, device #2 
  mySwitch.switchOn('b', 3, 2);

  // Wait a second
  delay(1000);
  
  // Switch off
  mySwitch.switchOff('b', 3, 2);
  
  // Wait another second
  delay(1000);
  
}