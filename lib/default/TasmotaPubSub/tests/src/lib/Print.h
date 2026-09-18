/*
  Print.h - Host-side Arduino `Print` shim for TasmotaPubSub host tests.

  This is NOT a test double of the library. It is a minimal reproduction of the
  Arduino `Print` base class so the *unmodified* PubSubClient library (which is
  `class PubSubClient : public Print`) compiles and links against a host platform.

  Only the surface the library depends on is provided:
    - `virtual size_t write(uint8_t)` (pure) and
      `virtual size_t write(const uint8_t*, size_t)` (default Arduino behavior:
      loop calling write(uint8_t)). The library overrides both.
  A few non-virtual print/println helpers are provided for parity with the real
  Arduino API; they are unused by the library but harmless.
*/

#ifndef TASMOTA_PUBSUB_TEST_PRINT_H
#define TASMOTA_PUBSUB_TEST_PRINT_H

#include <cstddef>
#include <cstdint>
#include <cstring>

class Print {
public:
    virtual ~Print() = default;

    // Core Arduino Print virtuals.
    virtual size_t write(uint8_t) = 0;

    // Standard Arduino default: write a buffer by looping over single-byte writes,
    // stopping early if a write reports failure (0 bytes written). Derived classes
    // (including the library under test) may override this.
    virtual size_t write(const uint8_t* buffer, size_t size) {
        size_t n = 0;
        while (size--) {
            if (write(*buffer++) == 0) {
                break;
            }
            n++;
        }
        return n;
    }

    // Convenience overload matching the Arduino API. Non-virtual and unused by the
    // library, but provided so this header is a faithful stand-in.
    size_t write(const char* str) {
        if (str == nullptr) {
            return 0;
        }
        return write(reinterpret_cast<const uint8_t*>(str), std::strlen(str));
    }

    // Harmless print/println helpers (unused by the library).
    size_t print(const char* str) {
        return write(str);
    }
    size_t println(const char* str) {
        size_t n = write(str);
        n += write(reinterpret_cast<const uint8_t*>("\r\n"), 2);
        return n;
    }
};

#endif  // TASMOTA_PUBSUB_TEST_PRINT_H
