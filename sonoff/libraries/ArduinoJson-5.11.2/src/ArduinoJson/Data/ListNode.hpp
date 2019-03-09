// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include <stddef.h>  // for NULL

#include "JsonBufferAllocated.hpp"

namespace ArduinoJson {
namespace Internals {

// A node for a singly-linked list.
// Used by List<T> and its iterators.
template <typename T>
struct ListNode : public Internals::JsonBufferAllocated {
  ListNode() throw() : next(NULL) {}

  ListNode<T> *next;
  T content;
};
}
}
