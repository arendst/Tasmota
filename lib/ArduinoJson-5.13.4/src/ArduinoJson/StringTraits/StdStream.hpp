// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <istream>

namespace ArduinoJson {
namespace Internals {

struct StdStreamTraits {
  class Reader {
    std::istream& _stream;
    char _current, _next;

   public:
    Reader(std::istream& stream) : _stream(stream), _current(0), _next(0) {}

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
    Reader& operator=(const Reader&);  // Visual Studio C4512

    char read() {
      return _stream.eof() ? '\0' : static_cast<char>(_stream.get());
    }
  };

  static const bool has_append = false;
  static const bool has_equals = false;
};

template <typename TStream>
struct StringTraits<
    TStream,
    // match any type that is derived from std::istream:
    typename EnableIf<IsBaseOf<
        std::istream, typename RemoveReference<TStream>::type>::value>::type>
    : StdStreamTraits {};
}
}

#endif
