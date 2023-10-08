/*
 * IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend
 * Version 0.1 June, 2015
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
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

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kPanasonicAddress = 0x4004;   // Panasonic address (Pre data)
const uint32_t kPanasonicPower = 0x100BCBD;  // Panasonic Power button
const uint16_t kJVCPower = 0xC5E8;

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void setup() {
  irsend.begin();
}

void loop() {
  // This should turn your TV on and off
#if SEND_PANASONIC
  irsend.sendPanasonic(kPanasonicAddress, kPanasonicPower);
#else  // SEND_PANASONIC
  Serial.println("Can't send because SEND_PANASONIC has been disabled.");
#endif  // SEND_PANASONIC

#if SEND_JVC
  irsend.sendJVC(kJVCPower, 16, 1);  // hex value, 16 bits, single repeat
#else  // SEND_JVC
  Serial.println("Can't send because SEND_JVC has been disabled.");
#endif  // SEND_JVC
  delay(10000);  // Wait 10 seconds before we repeat everything.
}
