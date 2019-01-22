// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_ARDUINO_STREAM

#include <Stream.h>

namespace ArduinoJson {
namespace Internals {

struct ArduinoStreamTraits {
  class Reader {
    Stream& _stream;
    char _current, _next;

   public:
    Reader(Stream& stream) : _stream(stream), _current(0), _next(0) {}

    void move() {
      _current = _next;
      _next = 0;
    }

    char current() {
      if (!_current) _current = read();
      return _current;
    }

    char next() {
      // assumes that current() has been called
      if (!_next) _next = read();
      return _next;
    }

   private:
    char read() {
      // don't use _stream.read() as it ignores the timeout
      char c = 0;
      _stream.readBytes(&c, 1);
      return c;
    }
  };

  static const bool has_append = false;
  static const bool has_equals = false;
};

template <typename TStream>
struct StringTraits<
    TStream,
    // match any type that is derived from Stream:
    typename EnableIf<
        IsBaseOf<Stream, typename RemoveReference<TStream>::type>::value>::type>
    : ArduinoStreamTraits {};
}
}

#endif
