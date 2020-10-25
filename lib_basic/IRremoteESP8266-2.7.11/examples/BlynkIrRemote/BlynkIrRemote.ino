/*************************************************************
  Emulate a physical remote via an iOS and Android App.
  Copyright Gaurav Barwalia 2020

  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

 /*
 // After decoding received below codes

 // Power button

18:12:33.993 -> Protocol  : NEC
18:12:33.993 -> Code      : 0x1FE50AF (32 Bits)
18:12:33.993 -> uint16_t rawData[71] = {9040, 4452,  606, 532,  606, 534,  630, 508,  604, 534,  604, 534,  604, 534,  630, 506,  606, 1646,  632, 1620,  606, 1646,  632, 1620,  630, 1620,  632, 1620,  630, 1620,  606, 1646,  632, 506,  632, 506,  632, 1620,  632, 506,  632, 1620,  632, 506,  632, 508,  632, 506,  632, 506,  632, 1620,  632, 506,  632, 1624,  628, 506,  632, 1620,  632, 1618,  632, 1620,  632, 1620,  632, 39016,  9040, 2216,  630};  // NEC 1FE50AF
18:12:34.027 -> uint32_t address = 0x80;
18:12:34.027 -> uint32_t command = 0xA;
18:12:34.027 -> uint64_t data = 0x1FE50AF;

//mute button

18:13:27.215 -> Protocol  : NEC
18:13:27.215 -> Code      : 0x1FE30CF (32 Bits)
18:13:27.215 -> uint16_t rawData[71] = {9094, 4398,  660, 478,  658, 480,  658, 480,  658, 480,  658, 480,  658, 480,  660, 480,  658, 1594,  658, 1594,  658, 1594,  658, 1594,  658, 1592,  658, 1594,  658, 1592,  658, 1594,  660, 480,  658, 480,  658, 480,  658, 1592,  658, 1592,  658, 480,  658, 480,  660, 478,  660, 478,  658, 1594,  658, 1592,  658, 480,  658, 480,  658, 1594,  658, 1592,  658, 1594,  658, 1594,  658, 38986,  9094, 2162,  658};  // NEC 1FE30CF
18:13:27.285 -> uint32_t address = 0x80;
18:13:27.285 -> uint32_t command = 0xC;
18:13:27.285 -> uint64_t data = 0x1FE30CF;

//Vol. low

18:14:44.427 -> Protocol  : NEC
18:14:44.427 -> Code      : 0x1FEC03F (32 Bits)
18:14:44.427 -> uint16_t rawData[71] = {9120, 4374,  658, 478,  658, 480,  658, 480,  658, 480,  658, 482,  658, 478,  658, 480,  658, 1594,  658, 1594,  658, 1592,  660, 1594,  658, 1592,  658, 1594,  658, 1594,  658, 1592,  660, 480,  658, 1594,  658, 1594,  658, 480,  658, 480,  660, 480,  658, 480,  658, 480,  658, 480,  658, 480,  658, 480,  658, 1594,  660, 1592,  658, 1594,  658, 1594,  658, 1592,  658, 1594,  658, 39002,  9094, 2162,  658};  // NEC 1FEC03F
18:14:44.497 -> uint32_t address = 0x80;
18:14:44.497 -> uint32_t command = 0x3;
18:14:44.497 -> uint64_t data = 0x1FEC03F;

//VOl. High

18:15:11.677 -> Protocol  : NEC
18:15:11.677 -> Code      : 0x1FE40BF (32 Bits)
18:15:11.677 -> uint16_t rawData[67] = {9068, 4426,  630, 506,  632, 508,  630, 508,  630, 508,  630, 508,  630, 508,  630, 508,  630, 1622,  630, 1622,  630, 1622,  630, 1622,  656, 1594,  630, 1622,  632, 1620,  630, 1622,  630, 508,  630, 508,  630, 1622,  630, 508,  630, 508,  630, 508,  630, 508,  630, 508,  630, 508,  630, 1622,  656, 482,  630, 1622,  630, 1622,  630, 1622,  630, 1622,  630, 1622,  632, 1620,  630};  // NEC 1FE40BF
18:15:11.747 -> uint32_t address = 0x80;
18:15:11.747 -> uint32_t command = 0x2;
18:15:11.747 -> uint64_t data = 0x1FE40BF;

//Play/Pause

18:15:38.529 -> Protocol  : NEC
18:15:38.529 -> Code      : 0x1FE32CD (32 Bits)
18:15:38.529 -> uint16_t rawData[71] = {9092, 4400,  632, 504,  658, 480,  658, 480,  632, 506,  658, 480,  658, 480,  658, 482,  632, 1620,  658, 1594,  658, 1594,  632, 1618,  658, 1594,  658, 1594,  632, 1620,  632, 1618,  634, 506,  658, 480,  658, 480,  632, 1620,  658, 1598,  656, 478,  658, 478,  658, 1594,  658, 482,  632, 1618,  632, 1618,  634, 506,  632, 506,  658, 1594,  632, 1620,  658, 480,  632, 1620,  658, 38998,  9094, 2162,  660};  // NEC 1FE32CD
18:15:38.564 -> uint32_t address = 0x80;
18:15:38.564 -> uint32_t command = 0x4C;
18:15:38.564 -> uint64_t data = 0x1FE32CD;

//Song Back

18:16:07.527 -> Protocol  : NEC
18:16:07.527 -> Code      : 0x1FEA05F (32 Bits)
18:16:07.562 -> uint16_t rawData[71] = {9590, 3902,  684, 452,  686, 456,  652, 480,  660, 480,  684, 456,  656, 480,  658, 480,  684, 1568,  658, 1594,  658, 1594,  686, 1566,  658, 1594,  684, 1568,  658, 1594,  658, 1594,  686, 454,  684, 1568,  686, 454,  658, 1594,  684, 454,  686, 454,  658, 480,  660, 480,  684, 454,  658, 482,  658, 1594,  682, 456,  658, 1596,  658, 1594,  686, 1568,  660, 1592,  684, 1568,  686, 38982,  9098, 2162,  684};  // NEC 1FEA05F
18:16:07.597 -> uint32_t address = 0x80;
18:16:07.597 -> uint32_t command = 0x5;
18:16:07.597 -> uint64_t data = 0x1FEA05F;

//Song Forward

18:17:20.541 -> Protocol  : NEC
18:17:20.541 -> Code      : 0x1FEE01F (32 Bits)
18:17:20.575 -> uint16_t rawData[71] = {9068, 4424,  632, 506,  630, 506,  632, 508,  606, 532,  632, 506,  630, 508,  630, 508,  632, 1620,  632, 1620,  632, 1620,  604, 1646,  606, 1646,  630, 1622,  604, 1646,  632, 1620,  606, 534,  630, 1622,  604, 1646,  630, 1622,  604, 534,  630, 508,  604, 534,  606, 534,  630, 508,  630, 508,  606, 534,  606, 532,  630, 1622,  604, 1646,  632, 1620,  604, 1648,  604, 1646,  604, 39040,  9040, 2216,  604};  // NEC 1FEE01F
18:17:20.610 -> uint32_t address = 0x80;
18:17:20.610 -> uint32_t command = 0x7;
18:17:20.610 -> uint64_t data = 0x1FEE01F;

 */

// check complete video tutorial here for program explanation https://www.youtube.com/watch?v=LqmkDKu54XY&t=17s

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#else
#include <WiFi.h>
#endif  // ESP8266
#if defined(ESP32)
#include <BlynkSimpleEsp32.h>
#endif  // ESP32

// IR library
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

  BLYNK_WRITE(V51) {  // Power button
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FE50AF);
    }
  }

  BLYNK_WRITE(V52) {  // Mute button
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FE30CF);
    }
  }

  BLYNK_WRITE(V53) {  // Song Forward
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FEE01F);
    }
  }

  BLYNK_WRITE(V54) {  // Song Backward
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FEA05F);
      delay(10);  // double tap back button to back one song
      irsend.sendNEC(0x1FEA05F);
    }
  }

  BLYNK_WRITE(V55) {  // Volume --
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FEC03F);
    }
  }

  BLYNK_WRITE(V56) {  // Volume ++
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FE40BF);
    }
  }

  BLYNK_WRITE(V57) {  // Play/Pause
    if (param.asInt() == 0) {
      // Serial.println("NEC");
      irsend.sendNEC(0x1FE32CD);
    }
  }

void setup() {
#if defined(BLYNK_PRINT)
  // Debug console
  Serial.begin(115200);
#endif  // BLYNK_PRINT

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}
