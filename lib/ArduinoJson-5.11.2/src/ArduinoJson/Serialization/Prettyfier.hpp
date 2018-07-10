// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "IndentedPrint.hpp"

namespace ArduinoJson {
namespace Internals {

// Converts a compact JSON string into an indented one.
template <typename Print>
class Prettyfier {
 public:
  explicit Prettyfier(IndentedPrint<Print>& p) : _sink(p) {
    _previousChar = 0;
    _inString = false;
  }

  size_t print(char c) {
    size_t n = _inString ? handleStringChar(c) : handleMarkupChar(c);
    _previousChar = c;
    return n;
  }

  size_t print(const char* s) {
    // TODO: optimize
    size_t n = 0;
    while (*s) n += print(*s++);
    return n;
  }

 private:
  Prettyfier& operator=(const Prettyfier&);  // cannot be assigned

  bool inEmptyBlock() {
    return _previousChar == '{' || _previousChar == '[';
  }

  size_t handleStringChar(char c) {
    bool isQuote = c == '"' && _previousChar != '\\';

    if (isQuote) _inString = false;

    return _sink.print(c);
  }

  size_t handleMarkupChar(char c) {
    switch (c) {
      case '{':
      case '[':
        return writeBlockOpen(c);

      case '}':
      case ']':
        return writeBlockClose(c);

      case ':':
        return writeColon();

      case ',':
        return writeComma();

      case '"':
        return writeQuoteOpen();

      default:
        return writeNormalChar(c);
    }
  }

  size_t writeBlockClose(char c) {
    size_t n = 0;
    n += unindentIfNeeded();
    n += _sink.print(c);
    return n;
  }

  size_t writeBlockOpen(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += _sink.print(c);
    return n;
  }

  size_t writeColon() {
    size_t n = 0;
    n += _sink.print(": ");
    return n;
  }

  size_t writeComma() {
    size_t n = 0;
    n += _sink.print(",\r\n");
    return n;
  }

  size_t writeQuoteOpen() {
    _inString = true;
    size_t n = 0;
    n += indentIfNeeded();
    n += _sink.print('"');
    return n;
  }

  size_t writeNormalChar(char c) {
    size_t n = 0;
    n += indentIfNeeded();
    n += _sink.print(c);
    return n;
  }

  size_t indentIfNeeded() {
    if (!inEmptyBlock()) return 0;

    _sink.indent();
    return _sink.print("\r\n");
  }

  size_t unindentIfNeeded() {
    if (inEmptyBlock()) return 0;

    _sink.unindent();
    return _sink.print("\r\n");
  }

  char _previousChar;
  IndentedPrint<Print>& _sink;
  bool _inString;
};
}
}
