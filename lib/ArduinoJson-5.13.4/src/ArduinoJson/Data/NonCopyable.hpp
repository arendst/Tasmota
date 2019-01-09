// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

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
