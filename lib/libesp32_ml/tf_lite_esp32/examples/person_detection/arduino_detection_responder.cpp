/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include <cmath>

#include "Arduino.h"
#include "detection_responder.h"

// Flash the yellow (builtin) LED after each inference
void RespondToDetection(tflite::ErrorReporter* error_reporter,
                        float person_score, float no_person_score) {
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // Pins for the built-in RGB LEDs on the Arduino Nano 33 BLE Sense
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    // Switch the LEDs off
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, HIGH);
    is_initialized = true;
  }

  // Note: The RGB LEDs on the Arduino Nano 33 BLE
  // Sense are on when the pin is LOW, off when HIGH.

  // Switch on the green LED when a person is detected,
  // the blue when no person is detected
  if (person_score > no_person_score) {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, HIGH);
  } else {
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
  }

  // Flash the yellow LED after every inference.
  // The builtin LED is on when the pin is HIGH
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);

  float person_score_frac, person_score_int;
  float no_person_score_frac, no_person_score_int;
  person_score_frac = std::modf(person_score * 100, &person_score_int);
  no_person_score_frac = std::modf(no_person_score * 100, &no_person_score_int);
  TF_LITE_REPORT_ERROR(error_reporter,
                       "Person score: %d.%d%% No person score: %d.%d%%",
                       static_cast<int>(person_score_int),
                       static_cast<int>(person_score_frac * 100),
                       static_cast<int>(no_person_score_int),
                       static_cast<int>(no_person_score_frac * 100));
}

#endif  // ARDUINO_EXCLUDE_CODE
