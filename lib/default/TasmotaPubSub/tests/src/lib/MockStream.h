/*
  MockStream.h - Host-side test double for the Arduino `Stream` used in
  stream-mode inbound PUBLISH (Mock_Transport stream variant).

  This is NOT part of the library. It is a test support type used when
  constructing `PubSubClient` with the optional `Stream&` argument. In stream
  mode the *unmodified* library routes each inbound PUBLISH payload byte to the
  attached stream via `stream->write(digit)` (inherited from Print) during
  `readPacket()`. MockStream records those written bytes, in order, so tests can:
    - assert stream-mode payload routing, and
    - exercise the stream branch of the DoS/trickle logic (F-03 stream mode).

  The read side is intentionally inert (available()==0, read()==-1, peek()==-1):
  the library never reads *from* this stream; it only writes routed payload
  digits *to* it. Capturing those written digits is the whole job of this class.
*/

#ifndef TASMOTA_PUBSUB_TEST_MOCK_STREAM_H
#define TASMOTA_PUBSUB_TEST_MOCK_STREAM_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Stream.h"

class MockStream : public Stream {
public:
    MockStream() = default;
    ~MockStream() override = default;

    // --- Print write side (what the library actually exercises) -------------
    // Record a single routed payload byte.
    size_t write(uint8_t b) override;

    // Record a buffer of routed payload bytes in order. Overridden (rather than
    // relying on Print's byte-at-a-time default) so bulk writes are captured
    // directly and efficiently; behavior is observably identical.
    size_t write(const uint8_t* buffer, size_t size) override;

    // Bring the non-virtual Print::write(const char*) helper into scope so it is
    // not hidden by the overrides above.
    using Stream::write;

    // --- Stream read side (inert; the library never reads from this) --------
    int  available() override;
    int  read() override;
    int  peek() override;

    // --- Test accessors -----------------------------------------------------
    // Bytes the library has written to this stream, in the order written.
    const std::vector<uint8_t>& written() const { return _written; }

    // Discard all recorded bytes (for reuse across sub-scenarios).
    void clear() { _written.clear(); }

private:
    std::vector<uint8_t> _written;
};

#endif  // TASMOTA_PUBSUB_TEST_MOCK_STREAM_H
