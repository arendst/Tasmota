// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"

namespace ArduinoJson {
namespace Internals {

class JsonBufferAllocated {
 public:
  void *operator new(size_t n, JsonBuffer *jsonBuffer) throw() {
    if (!jsonBuffer) return NULL;
    return jsonBuffer->alloc(n);
  }

  void operator delete(void *, JsonBuffer *)throw();
};
}
}
