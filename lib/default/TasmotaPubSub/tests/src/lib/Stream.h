/*
  Stream.h - Host-side Arduino `Stream` shim for TasmotaPubSub host tests.

  Minimal reproduction of the Arduino `Stream` class: `Stream : public Print`
  adding the read side (available/read/peek/flush). The library holds an optional
  `Stream* stream;` and, in stream mode, calls `stream->write(digit)` (inherited
  from Print). MockStream subclasses this to record routed payload bytes.
*/

#ifndef TASMOTA_PUBSUB_TEST_STREAM_H
#define TASMOTA_PUBSUB_TEST_STREAM_H

#include "Print.h"

class Stream : public Print {
public:
    virtual ~Stream() = default;

    // Read-side interface added by Arduino `Stream`.
    virtual int  available() = 0;
    virtual int  read() = 0;
    virtual int  peek() = 0;
    virtual void flush();

    // Bring Print's write overloads into scope so a derived class that only
    // overrides write(uint8_t) does not hide write(const uint8_t*, size_t)
    // (avoids -Woverloaded-virtual under -Wextra).
    using Print::write;
};

#endif  // TASMOTA_PUBSUB_TEST_STREAM_H
