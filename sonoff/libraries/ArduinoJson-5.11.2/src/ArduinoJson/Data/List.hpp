// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#pragma once

#include "../JsonBuffer.hpp"
#include "ListConstIterator.hpp"
#include "ListIterator.hpp"

namespace ArduinoJson {
namespace Internals {

// A singly linked list of T.
// The linked list is composed of ListNode<T>.
// It is derived by JsonArray and JsonObject
template <typename T>
class List {
 public:
  typedef T value_type;
  typedef ListNode<T> node_type;
  typedef ListIterator<T> iterator;
  typedef ListConstIterator<T> const_iterator;

  // Creates an empty List<T> attached to a JsonBuffer.
  // The JsonBuffer allows to allocate new nodes.
  // When buffer is NULL, the List is not able to grow and success() returns
  // false. This is used to identify bad memory allocations and parsing
  // failures.
  explicit List(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  // Returns true if the object is valid
  // Would return false in the following situation:
  // - the memory allocation failed (StaticJsonBuffer was too small)
  // - the JSON parsing failed
  bool success() const {
    return _buffer != NULL;
  }

  // Returns the numbers of elements in the list.
  // For a JsonObject, it would return the number of key-value pairs
  size_t size() const {
    size_t nodeCount = 0;
    for (node_type *node = _firstNode; node; node = node->next) nodeCount++;
    return nodeCount;
  }

  iterator add() {
    node_type *newNode = new (_buffer) node_type();

    if (_firstNode) {
      node_type *lastNode = _firstNode;
      while (lastNode->next) lastNode = lastNode->next;
      lastNode->next = newNode;
    } else {
      _firstNode = newNode;
    }

    return iterator(newNode);
  }

  iterator begin() {
    return iterator(_firstNode);
  }
  iterator end() {
    return iterator(NULL);
  }

  const_iterator begin() const {
    return const_iterator(_firstNode);
  }
  const_iterator end() const {
    return const_iterator(NULL);
  }

  void remove(iterator it) {
    node_type *nodeToRemove = it._node;
    if (!nodeToRemove) return;
    if (nodeToRemove == _firstNode) {
      _firstNode = nodeToRemove->next;
    } else {
      for (node_type *node = _firstNode; node; node = node->next)
        if (node->next == nodeToRemove) node->next = nodeToRemove->next;
    }
  }

 protected:
  JsonBuffer *_buffer;

 private:
  node_type *_firstNode;
};
}
}
