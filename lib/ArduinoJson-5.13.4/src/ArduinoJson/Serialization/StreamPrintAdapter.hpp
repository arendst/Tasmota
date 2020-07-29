// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Configuration.hpp"

#if ARDUINOJSON_ENABLE_STD_STREAM

#include <ostream>

namespace ArduinoJson {
namespace Internals {

class StreamPrintAdapter {
 public:
  explicit StreamPrintAdapter(std::ostream& os) : _os(os) {}

  size_t print(char c) {
    _os << c;
    return 1;
  }

  size_t print(const char* s) {
    _os << s;
    return strlen(s);
  }

 private:
  // cannot be assigned
  StreamPrintAdapter& operator=(const StreamPrintAdapter&);

  std::ostream& _os;
};
}
}

#endif  // ARDUINOJSON_ENABLE_STD_STREAM
