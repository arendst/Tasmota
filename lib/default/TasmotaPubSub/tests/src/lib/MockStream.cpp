/*
  MockStream.cpp - Implementation of the host-side `Stream` test double.

  Records every byte the library writes to the stream (stream-mode PUBLISH
  payload routing) and provides an inert read side.
*/

#include "MockStream.h"

size_t MockStream::write(uint8_t b) {
    _written.push_back(b);
    return 1;
}

size_t MockStream::write(const uint8_t* buffer, size_t size) {
    if (buffer == nullptr) {
        return 0;
    }
    _written.insert(_written.end(), buffer, buffer + size);
    return size;
}

int MockStream::available() {
    // The library never reads from this stream; report "nothing to read".
    return 0;
}

int MockStream::read() {
    // Inert read side: no bytes available.
    return -1;
}

int MockStream::peek() {
    // Inert read side: no bytes available.
    return -1;
}
