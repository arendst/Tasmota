// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <istream>
#include "../TypeTraits/EnableIf.hpp"
#include "../TypeTraits/IsBaseOf.hpp"
#include "../TypeTraits/RemoveReference.hpp"

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
};

template <typename TStream>
struct StringTraits<TStream,
                    // match any type that is derived from std::istream:
                    typename TypeTraits::EnableIf<TypeTraits::IsBaseOf<
                        std::istream, typename TypeTraits::RemoveReference<
                                          TStream>::type>::value>::type>
    : StdStreamTraits {};
}
}
