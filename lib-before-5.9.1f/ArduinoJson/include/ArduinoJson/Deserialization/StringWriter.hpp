// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

template <typename TChar>
class StringWriter {
 public:
  class String {
   public:
    String(TChar** ptr) : _writePtr(ptr), _startPtr(*ptr) {}

    void append(TChar c) {
      *(*_writePtr)++ = c;
    }

    const char* c_str() const {
      *(*_writePtr)++ = 0;
      return reinterpret_cast<const char*>(_startPtr);
    }

   private:
    TChar** _writePtr;
    TChar* _startPtr;
  };

  StringWriter(TChar* buffer) : _ptr(buffer) {}

  String startString() {
    return String(&_ptr);
  }

 private:
  TChar* _ptr;
};
}
}
