/*
  Arduino.h - Host-side Arduino core shim for TasmotaPubSub host tests.

  Provides the Arduino primitives the *unmodified* PubSubClient library depends
  on when compiled on a host (g++/clang) instead of an ESP toolchain. This is the
  platform seam: the library `#include <Arduino.h>` resolves to this header
  because `tests/src/lib` is first on the include path.

  Time primitives (millis/delay/yield) are DECLARED here but intentionally NOT
  defined. They are wired to the virtual TestClock in a later task (3.2). The
  library links against these declarations.
*/

#ifndef TASMOTA_PUBSUB_TEST_ARDUINO_H
#define TASMOTA_PUBSUB_TEST_ARDUINO_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstddef>
#include <functional>

// The library selects the std::function callback signature only when building
// for an ESP target. The host tests must build with -DESP32 so lambdas with
// captures can be bound as callbacks. Make the requirement explicit.
#if !defined(ESP32) && !defined(ESP8266)
#error "Build the host shim with -DESP32 so MQTT_CALLBACK_SIGNATURE resolves to std::function"
#endif

// Arduino type aliases used by the library.
typedef bool    boolean;
typedef uint8_t byte;

// PROGMEM is a no-op on a host with a flat address space. publish_P therefore
// reads normal memory via pgm_read_byte_near.
#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef PGM_P
typedef const char* PGM_P;
#endif

#ifndef pgm_read_byte_near
#define pgm_read_byte_near(addr) (*reinterpret_cast<const uint8_t*>(addr))
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*reinterpret_cast<const uint8_t*>(addr))
#endif

// Time / scheduling primitives. Declared only; defined against TestClock later.
unsigned long millis();
void          delay(unsigned long ms);
void          delayMicroseconds(unsigned int us);
void          yield();

// Arduino String, provided by <Arduino.h> on the real platform.
#include "ArduinoString.h"

#endif  // TASMOTA_PUBSUB_TEST_ARDUINO_H
