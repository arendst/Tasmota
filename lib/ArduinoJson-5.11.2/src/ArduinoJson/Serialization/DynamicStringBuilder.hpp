// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../StringTraits/StringTraits.hpp"

namespace ArduinoJson {
namespace Internals {

// A Print implementation that allows to write in a String
template <typename TString>
class DynamicStringBuilder {
 public:
  DynamicStringBuilder(TString &str) : _str(str) {}

  size_t print(char c) {
    StringTraits<TString>::append(_str, c);
    return 1;
  }

  size_t print(const char *s) {
    size_t initialLen = _str.length();
    StringTraits<TString>::append(_str, s);
    return _str.length() - initialLen;
  }

 private:
  DynamicStringBuilder &operator=(const DynamicStringBuilder &);

  TString &_str;
};
}
}
