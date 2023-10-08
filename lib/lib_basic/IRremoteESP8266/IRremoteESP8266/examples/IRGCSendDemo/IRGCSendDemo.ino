/*
 * IRremoteESP8266: IRsendGCDemo
 * demonstrates sending Global Cache-formatted IR codes with IRsend
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 *
 * Version 0.2 June, 2017
 *   Added helpful comments
 *   Better includes files.
 * Version 0.1 30 March, 2016
 *   Based on Ken Shirriff's IrsendDemo
 *   Version 0.1 July, 2009
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by IR_LED below.
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
 *   * Typical digital camera/phones can be used to see if the IR LED is
 * flashed. Replace the IR LED with a normal LED if you don't have a digital
 * camera when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will
 * interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Codes are in Global Cache format less the emitter ID and request ID.
// These codes can be found in GC's Control Tower database.

uint16_t Samsung_power_toggle[71] = {
    38000, 1,  1,  170, 170, 20, 63, 20, 63, 20, 63,  20, 20, 20, 20,
    20,    20, 20, 20,  20,  20, 20, 63, 20, 63, 20,  63, 20, 20, 20,
    20,    20, 20, 20,  20,  20, 20, 20, 20, 20, 63,  20, 20, 20, 20,
    20,    20, 20, 20,  20,  20, 20, 20, 20, 63, 20,  20, 20, 63, 20,
    63,    20, 63, 20,  63,  20, 63, 20, 63, 20, 1798};

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

void setup() {
  irsend.begin();
  Serial.begin(115200);
}

void loop() {
  Serial.println("Toggling power");
#if SEND_GLOBALCACHE
  irsend.sendGC(Samsung_power_toggle, 71);
#else   // SEND_GLOBALCACHE
  Serial.println("Can't send because SEND_GLOBALCACHE has been disabled.");
#endif  // SEND_GLOBALCACHE
  delay(10000);
}
