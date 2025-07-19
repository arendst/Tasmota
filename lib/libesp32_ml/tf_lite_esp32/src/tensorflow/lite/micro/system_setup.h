/* Copyright 2021 The TensorFlow Authors. All Rights Reserved.

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
#ifndef TENSORFLOW_LITE_MICRO_SYSTEM_SETUP_H_
#define TENSORFLOW_LITE_MICRO_SYSTEM_SETUP_H_

#include <cstddef>
#include <utility>

namespace tflite {

// This should called during initialization of TFLM binaries and tests. It can
// be specialized if there is a need for custom target-specific intialization.
// For more information, see tensorflow/lite/micro/system_setup.cc.
void InitializeTarget();

}  // namespace tflite

namespace test_over_serial {

// computed for Arduino Nano 33 BLE Sense
constexpr size_t kSerialMaxInputLength = (64);

// Change baud rate on default serial port
void SerialChangeBaudRate(const int baud);

// SerialReadLine
// Read a set of ASCII characters from the default
// serial port.  Data is read up to the first newline ('\\n') character.
// This function uses an internal buffer which is automatically reset.
// The buffer will not contain the newline character.
// The buffer will be zero ('\\0') terminated.
// The <timeout> value is in milliseconds.  Any negative value means that
// the wait for data will be forever.
// Returns std::pair<size_t, char*>.
// The first pair element is the number of characters in buffer not including
// the newline character or zero terminator.
// Returns {0, NULL} if the timeout occurs.
std::pair<size_t, char*> SerialReadLine(int timeout);

// SerialWrite
// Write the ASCII characters in <buffer> to the default serial port.
// The <buffer> must be zero terminated.
void SerialWrite(const char* buffer);

}  // namespace test_over_serial

#endif  // TENSORFLOW_LITE_MICRO_SYSTEM_SETUP_H_
