/*
  Client.h - Host-side Arduino `Client` shim for TasmotaPubSub host tests.

  Abstract reproduction of the Arduino network `Client` interface the library
  holds as `Client* _client`. It declares the full pure-virtual set the library
  actually exercises. MockClient (later task) subclasses this exactly as a real
  `WiFiClient` would be used.

  Inheritance mirrors Arduino: `Client : public Stream : public Print`, so the
  two `write` overloads originate in `Print`. They are re-declared here as pure
  virtual to express the complete transport contract the library relies on.
*/

#ifndef TASMOTA_PUBSUB_TEST_CLIENT_H
#define TASMOTA_PUBSUB_TEST_CLIENT_H

#include <cstddef>
#include <cstdint>

#include "IPAddress.h"
#include "Stream.h"

class Client : public Stream {
public:
    virtual ~Client() = default;

    virtual int connect(IPAddress ip, uint16_t port) = 0;
    virtual int connect(const char* host, uint16_t port) = 0;

    // Transport write side (declared in Print; the library calls both forms).
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t* buf, size_t size) = 0;

    // Read side.
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int read(uint8_t* buf, size_t size) = 0;
    virtual int peek() = 0;

    virtual void    flush() = 0;
    virtual void    stop() = 0;
    virtual uint8_t connected() = 0;
    virtual operator bool() = 0;
};

#endif  // TASMOTA_PUBSUB_TEST_CLIENT_H
