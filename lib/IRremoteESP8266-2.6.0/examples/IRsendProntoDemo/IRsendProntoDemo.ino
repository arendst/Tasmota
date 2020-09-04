/* IRremoteESP8266: IRsendProntoDemo
 * Copyright 2017 David Conran
 *
 * Demonstrates sending Pronto codes with IRsend.
 *
 * Version 1.0 June, 2017
 *
 * An IR LED circuit *MUST* be connected to ESP8266 pin 4 (D2), unless you
 * change the kIrLed value below.
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

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Panasonic Plasma TV Descrete code (Power On).
// Acquired from:
//   https://irdb.globalcache.com/
// e.g.
// 0000 006D 0000 0022 00ac 00ac 0016 0040 0016 0040 0016 0040 0016 0015 0016
// 0015 0016 0015 0016 0015 0016 0015 0016 0040 0016 0040 0016 0040 0016 0015
// 0016 0015 0016 0015 0016 0015 0016 0015 0016 0040 0016 0015 0016 0015 0016
// 0040 0016 0040 0016 0015 0016 0015 0016 0040 0016 0015 0016 0040 0016 0040
// 0016 0015 0016 0015 0016 0040 0016 0040 0016 0015 0016 071c
//
// Or the equiv. of sendSamsung(0xE0E09966);
uint16_t samsungProntoCode[72] = {
    0x0000, 0x006D, 0x0000, 0x0022,
    0x00ac, 0x00ac, 0x0016, 0x0040, 0x0016, 0x0040, 0x0016, 0x0040,
    0x0016, 0x0015, 0x0016, 0x0015, 0x0016, 0x0015, 0x0016, 0x0015,
    0x0016, 0x0015, 0x0016, 0x0040, 0x0016, 0x0040, 0x0016, 0x0040,
    0x0016, 0x0015, 0x0016, 0x0015, 0x0016, 0x0015, 0x0016, 0x0015,
    0x0016, 0x0015, 0x0016, 0x0040, 0x0016, 0x0015, 0x0016, 0x0015,
    0x0016, 0x0040, 0x0016, 0x0040, 0x0016, 0x0015, 0x0016, 0x0015,
    0x0016, 0x0040, 0x0016, 0x0015, 0x0016, 0x0040, 0x0016, 0x0040,
    0x0016, 0x0015, 0x0016, 0x0015, 0x0016, 0x0040, 0x0016, 0x0040,
    0x0016, 0x0015, 0x0016, 0x071c
};

// Panasonic Plasma TV Descrete code (Power On).
// Acquired from:
//   ftp://ftp.panasonic.com/pub/panasonic/drivers/monitors/Discrete-remote-control-codesProntoCCFformat.pdf
// e.g.
// 0000 0071 0000 0032 0080 003F 0010 0010 0010 0030 0010 0010 0010 0010 0010
// 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010
// 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010
// 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010 0010 0010 0010 0010 0010
// 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0010 0030 0010
// 0030 0010 0030 0010 0030 0010 0030 0010 0010 0010 0010 0010 0010 0010 0030
// 0010 0030 0010 0030 0010 0030 0010 0030 0010 0010 0010 0030 0010 0A98
//
// Or the equiv. of sendPanasonic64(0x400401007C7D);
uint16_t panasonicProntoCode[104] = {
    0x0000, 0x0071, 0x0000, 0x0032,
    0x0080, 0x003F, 0x0010, 0x0010, 0x0010, 0x0030, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0030, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0010, 0x0030,
    0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0010,
    0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0030, 0x0010, 0x0030,
    0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0030, 0x0010, 0x0010,
    0x0010, 0x0030, 0x0010, 0x0A98};

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
#if SEND_PRONTO
  Serial.println("Sending a Samsung TV 'on' command.");
  irsend.sendPronto(samsungProntoCode, 72);
  delay(2000);
  Serial.println("Sending a Panasonic Plasma TV 'on' command.");
  irsend.sendPronto(panasonicProntoCode, 104);
  delay(2000);
#else  // SEND_PRONTO
  Serial.println("Can't send because SEND_PRONTO has been disabled.");
  delay(10000);
#endif  // SEND_PRONTO
}
