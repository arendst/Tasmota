/* Copyright 2019 David Conran
*
* An IR LED circuit *MUST* be connected to the ESP8266 on a pin
* as specified by kIrLed below.
*
* TL;DR: The IR LED needs to be driven by a transistor for a good result.
*
* Suggested circuit:
*     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
*
* Common mistakes & tips:
*   * Don't just connect the IR LED directly to the pin, it won't
*     have enough current to drive the IR LED effectively.
*   * Make sure you have the IR LED polarity correct.
*     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
*   * Typical digital camera/phones can be used to see if the IR LED is flashed.
*     Replace the IR LED with a normal LED if you don't have a digital camera
*     when debugging.
*   * Avoid using the following pins unless you really know what you are doing:
*     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
*     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
*     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
*   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
*     for your first time. e.g. ESP-12 etc.
*/
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Samsung.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRSamsungAc ac(kIrLed);     // Set the GPIO used for sending messages.

void printState() {
  // Display the settings.
  Serial.println("Samsung A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
}

void setup() {
  ac.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See ir_Samsung.cpp for all the options.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting initial state for A/C.");
  ac.off();
  ac.setFan(kSamsungAcFanLow);
  ac.setMode(kSamsungAcCool);
  ac.setTemp(25);
  ac.setSwing(false);
  printState();
}

void loop() {
  // Turn the A/C unit on and set to cooling mode.
  // Power changes require we send an extended message.
  Serial.println("Sending an extended IR command to A/C ...");
  ac.on();
  ac.setMode(kSamsungAcCool);
  ac.sendExtended();
  printState();
  delay(15000);  // wait 15 seconds

  // Increase the fan speed.
  Serial.println("Sending a normal IR command to A/C ...");
  ac.setFan(kSamsungAcFanHigh);
  ac.send();
  printState();
  delay(15000);

  // Change to swing the fan.
  Serial.println("Sending a normal IR command to A/C ...");
  ac.setSwing(true);
  ac.send();
  printState();
  delay(15000);

  // Change to Fan mode, lower the speed, and stop the swing.
  Serial.println("Sending a normal IR command to A/C ...");
  ac.setSwing(false);
  ac.setMode(kSamsungAcFan);
  ac.setFan(kSamsungAcFanLow);
  ac.send();
  printState();
  delay(15000);

  // Turn the A/C unit off.
  // Power changes require we send an extended message.
  Serial.println("Sending an extended IR command to A/C ...");
  ac.off();
  ac.sendExtended();
  printState();
  delay(15000);  // wait 15 seconds
}
