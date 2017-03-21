// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Internals {

// A type that is meant to be used by reference only (JsonArray and JsonObject)
class ReferenceType {
 public:
  bool operator==(const ReferenceType& other) const {
    // two JsonArray are equal if they are the same instance
    // (we don't compare the content)
    return this == &other;
  }

  bool operator!=(const ReferenceType& other) const {
    return this != &other;
  }

 protected:
  ReferenceType() {}

 private:
  // copy constructor is private
  ReferenceType(const ReferenceType&);

  // copy operator is private
  ReferenceType& operator=(const ReferenceType&);
};
}
}
