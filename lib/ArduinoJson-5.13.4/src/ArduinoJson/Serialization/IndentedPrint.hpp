// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

namespace ArduinoJson {
namespace Internals {

// Decorator on top of Print to allow indented output.
// This class is used by JsonPrintable::prettyPrintTo() but can also be used
// for your own purpose, like logging.
template <typename Print>
class IndentedPrint {
 public:
  explicit IndentedPrint(Print &p) : sink(&p) {
    level = 0;
    tabSize = 2;
    isNewLine = true;
  }

  size_t print(char c) {
    size_t n = 0;
    if (isNewLine) n += writeTabs();
    n += sink->print(c);
    isNewLine = c == '\n';
    return n;
  }

  size_t print(const char *s) {
    // TODO: optimize
    size_t n = 0;
    while (*s) n += print(*s++);
    return n;
  }

  // Adds one level of indentation
  void indent() {
    if (level < MAX_LEVEL) level++;
  }

  // Removes one level of indentation
  void unindent() {
    if (level > 0) level--;
  }

  // Set the number of space printed for each level of indentation
  void setTabSize(uint8_t n) {
    if (n < MAX_TAB_SIZE) tabSize = n & MAX_TAB_SIZE;
  }

 private:
  Print *sink;
  uint8_t level : 4;
  uint8_t tabSize : 3;
  bool isNewLine : 1;

  size_t writeTabs() {
    size_t n = 0;
    for (int i = 0; i < level * tabSize; i++) n += sink->print(' ');
    return n;
  }

  static const int MAX_LEVEL = 15;    // because it's only 4 bits
  static const int MAX_TAB_SIZE = 7;  // because it's only 3 bits
};
}
}
