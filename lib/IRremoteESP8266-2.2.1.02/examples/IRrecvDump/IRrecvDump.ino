/*
 * IRremoteESP8266: IRrecvDump - dump details of IR codes with IRrecv
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 *
 ***** DEPRECATED - DO NOT USE *****
 * Unless you know what you are doing, you should be using the
 * IRrecvDumpV2.ino sketch/example instead for capturing & decoding IR messages.
 * In almost ALL ways it is BETTER, FASTER, and MORE DETAILED.
 *
 * This code is left only for legacy reasons, and as another simple example of
 * how to use the IRremoteESP8266 library.
 *
 * As of November 2017 it will no longer be updated or supported.
 * You have been warned.
 ***** DEPRECATED - DO NOT USE *****
 *
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.2 Oct 2017
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * JVC and Panasonic protocol added by Kristian Lauszus
 *   (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// an IR detector/demodulator is connected to GPIO pin 2
uint16_t RECV_PIN = 2;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
}

void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  uint16_t count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } else if (results->decode_type == RC5X) {
    Serial.print("Decoded RC5X: ");
  } else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  } else if (results->decode_type == RCMM) {
    Serial.print("Decoded RCMM: ");
  } else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  } else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  } else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  } else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  } else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  } else if (results->decode_type == NIKAI) {
    Serial.print("Decoded Nikai: ");
  }
  serialPrintUint64(results->value, 16);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): {");

  for (uint16_t i = 1; i < count; i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    if (i & 1) {
      Serial.print(results->rawbuf[i] * RAWTICK, DEC);
    } else {
      Serial.print(", ");
      Serial.print((uint32_t) results->rawbuf[i] * RAWTICK, DEC);
    }
  }
  Serial.println("};");
}

void loop() {
  if (irrecv.decode(&results)) {
    dump(&results);
    Serial.println("DEPRECATED: Please use IRrecvDumpV2.ino instead!");
    irrecv.resume();  // Receive the next value
  }
}
