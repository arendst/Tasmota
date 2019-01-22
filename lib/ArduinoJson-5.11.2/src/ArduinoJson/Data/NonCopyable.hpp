// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

// A type that cannot be copied
class NonCopyable {
 protected:
  NonCopyable() {}

 private:
  // copy constructor is private
  NonCopyable(const NonCopyable&);

  // copy operator is private
  NonCopyable& operator=(const NonCopyable&);
};
}
}
