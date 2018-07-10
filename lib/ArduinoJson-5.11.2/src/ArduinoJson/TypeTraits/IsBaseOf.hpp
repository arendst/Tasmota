// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

// A meta-function that returns true if Derived inherits from TBase is an
// integral type.
template <typename TBase, typename TDerived>
class IsBaseOf {
 protected:  // <- to avoid GCC's "all member functions in class are private"
  typedef char Yes[1];
  typedef char No[2];

  static Yes &probe(const TBase *);
  static No &probe(...);

 public:
  enum {
    value = sizeof(probe(reinterpret_cast<TDerived *>(0))) == sizeof(Yes)
  };
};
}
}
