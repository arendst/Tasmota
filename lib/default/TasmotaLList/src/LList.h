/*
  LList.h - Lightweight Linked List for simple objects - optimized for low code size and low memory

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __LLIST__
#define __LLIST__

#include <cstddef>

/*********************************************************************************************\
 *
 * private class for Linked List element
 *
\*********************************************************************************************/
template <typename T>
class LList;

template <typename T>
class LList_elt {
public:

  LList_elt() : _next(nullptr), _val() {}

  inline T & val(void) { return _val; }
  inline LList_elt<T> * next(void) { return _next; }
  inline void next(LList_elt<T> * next) { _next = next; }

  friend class LList<T>;

protected:
  LList_elt<T> * _next;
  T _val;
};

/*********************************************************************************************\
 *
 * Lightweight Linked List - optimized for low code size
 *
\*********************************************************************************************/
template <typename T>
class LList {
public:
  LList() : _head(nullptr) {}
  ~LList() { reset(); }

  // remove elements
  void removeHead(void);      // remove first element
  void reset(void);           // remove all elements
  void remove(const T * val);

  // read the list
  inline bool isEmpty(void) const { return (_head == nullptr) ? true : false; }
  size_t length(void) const;
  inline T * head(void) { return _head ? &_head->_val : nullptr; }
  inline const T * head(void) const { return _head ? &_head->_val : nullptr; }
  const T * at(size_t index) const ;
  // non-const variants
  // not very academic cast but reduces code size
  inline T * at(size_t index) { return (T*) ((const LList<T>*)this)->at(index); }

  // adding elements
  T & addHead(void);
  T & addHead(const T &val);
  T & addToLast(void);
  // add an element allocated externally
  // memory is free by us now -- don't free it!
  T & addHead(LList_elt<T> * elt);
  T & addToLast(LList_elt<T> * elt);

  // iterator
  // see https://stackoverflow.com/questions/8164567/how-to-make-my-custom-type-to-work-with-range-based-for-loops
  class iterator {
  public:
    iterator(LList_elt<T> *_cur): cur(_cur), next(nullptr) { if (cur) { next = cur->_next; } }
    iterator operator++() { cur = next; if (cur) { next = cur->_next;} return *this; }
    bool operator!=(const iterator & other) const { return cur != other.cur; }
    T & operator*() const { return cur->_val; }
  private:
    LList_elt<T> *cur;
    LList_elt<T> *next;    // we need to keep next pointer in case the current attribute gets deleted
  };
  iterator begin() { return iterator(this->_head); }      // start with 'head'
  iterator end() { return iterator(nullptr); }      // end with null pointer

  // const iterator
  class const_iterator {
    public:
      const_iterator(const LList_elt<T> *_cur): cur(_cur), next(nullptr) { if (cur) { next = cur->_next; } }
      const_iterator operator++() { cur = next; if (cur) { next = cur->_next;} return *this; }
      bool operator!=(const_iterator & other) const { return cur != other.cur; }
      const T & operator*() const { return cur->_val; }
    private:
      const LList_elt<T> *cur;
      const LList_elt<T> *next;    // we need to keep next pointer in case the current attribute gets deleted
    };
  const_iterator begin() const { return const_iterator(this->_head); }      // start with 'head'
  const_iterator end() const { return const_iterator(nullptr); }      // end with null pointer

protected:
  LList_elt<T> * _head;
};

template <typename T>
size_t LList<T>::length(void) const {
  size_t count = 0;
  for (auto & elt : *this) { (void)elt; count++; }
  return count;
}


template <typename T>
const T * LList<T>::at(size_t index) const {
  size_t count = 0;
  for (const auto & elt : *this) {
    if (index == count++) { return &elt; }
  }
  return nullptr;
}

template <typename T>
void LList<T>::reset(void) {
  while (_head) {
    LList_elt<T> * next = _head->next();
    delete _head;
    _head = next;
  }
}

template <typename T>
void LList<T>::removeHead(void) {
  if (_head) {
    LList_elt<T> * next = _head->next();
    delete _head;
    _head = next;
  }
}

template <typename T>
void LList<T>::remove(const T * val) {
  if (nullptr == val) { return; }
  // find element in chain and find pointer before
  LList_elt<T> **curr_ptr = &_head;
  while (*curr_ptr) {
    LList_elt<T> * curr_elt = *curr_ptr;
    if ( &(curr_elt->_val) == val) {
      *curr_ptr = curr_elt->_next;   // update previous pointer to next element
      delete curr_elt;
      break;                        // stop iteration now
    }
    curr_ptr = &((*curr_ptr)->_next);   // move to next element
  }
}

template <typename T>
T & LList<T>::addHead(void) {
  LList_elt<T> * elt = new LList_elt<T>();  // create element
  elt->next(_head);      // insert at the head
  _head = elt;
  return elt->_val;
}

template <typename T>
T & LList<T>::addHead(const T &val) {
  LList_elt<T> * elt = new LList_elt<T>();  // create element
  elt->next(_head);      // insert at the head
  elt->_val = val;
  _head = elt;
  return elt->_val;
}

template <typename T>
T & LList<T>::addHead(LList_elt<T> * elt) {
  elt->next(_head);      // insert at the head
  _head = elt;
  return elt->_val;
}

template <typename T>
T & LList<T>::addToLast(void) {
  LList_elt<T> **curr_ptr = &_head;
  while (*curr_ptr) {
    curr_ptr = &((*curr_ptr)->_next);
  }
  LList_elt<T> * elt = new LList_elt<T>();  // create element
  *curr_ptr = elt;
  return elt->_val;
}

template <typename T>
T & LList<T>::addToLast(LList_elt<T> * elt) {
  LList_elt<T> **curr_ptr = &_head;
  while (*curr_ptr) {
    curr_ptr = &((*curr_ptr)->_next);
  }
  *curr_ptr = elt;
  elt->_next = nullptr;
  return elt->_val;
}

#endif // __LLIST__