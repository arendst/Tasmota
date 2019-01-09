// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "ListConstIterator.hpp"
#include "ListNode.hpp"

namespace ArduinoJson {
namespace Internals {

template <typename T>
class List;

// A read-write forward iterator for List<T>
template <typename T>
class ListIterator {
  friend class List<T>;

 public:
  explicit ListIterator(ListNode<T> *node = NULL) : _node(node) {}

  T &operator*() const {
    return _node->content;
  }
  T *operator->() {
    return &_node->content;
  }

  bool operator==(const ListIterator<T> &other) const {
    return _node == other._node;
  }

  bool operator!=(const ListIterator<T> &other) const {
    return _node != other._node;
  }

  ListIterator<T> &operator++() {
    if (_node) _node = _node->next;
    return *this;
  }

  ListIterator<T> &operator+=(size_t distance) {
    while (_node && distance) {
      _node = _node->next;
      --distance;
    }
    return *this;
  }

  operator ListConstIterator<T>() const {
    return ListConstIterator<T>(_node);
  }

 private:
  ListNode<T> *_node;
};
}
}
