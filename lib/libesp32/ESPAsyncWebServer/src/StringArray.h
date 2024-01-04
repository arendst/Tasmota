/*
  Asynchronous WebServer library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef STRINGARRAY_H_
#define STRINGARRAY_H_

#include "stddef.h"
#include "WString.h"

template <typename T>
class AsyncLinkedListNode {
    T _value;
  public:
    AsyncLinkedListNode<T>* next;
    AsyncLinkedListNode(const T val): _value(val), next(nullptr) {}
    ~AsyncLinkedListNode(){}
    const T& value() const { return _value; };
    T& value(){ return _value; }
};

template <typename T, template<typename> class Item = AsyncLinkedListNode>
class AsyncLinkedList {
  public:
    typedef Item<T> ItemType;
    typedef std::function<void(const T&)> OnRemove;
    typedef std::function<bool(const T&)> Predicate;
  private:
    ItemType* _root;
    OnRemove _onRemove;

    class Iterator {
      ItemType* _node;
    public:
      Iterator(ItemType* current = nullptr) : _node(current) {}
      Iterator(const Iterator& i) : _node(i._node) {}
      Iterator& operator ++() { _node = _node->next; return *this; }
      bool operator != (const Iterator& i) const { return _node != i._node; }
      const T& operator * () const { return _node->value(); }
      const T* operator -> () const { return &_node->value(); }
    };
    
  public:
    typedef const Iterator ConstIterator;
    ConstIterator begin() const { return ConstIterator(_root); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    AsyncLinkedList(OnRemove onRemove) : _root(nullptr), _onRemove(onRemove) {}
    ~AsyncLinkedList(){}
    void add(const T& t){
      auto it = new ItemType(t);
      if(!_root){
        _root = it;
      } else {
        auto i = _root;
        while(i->next) i = i->next;
        i->next = it;
      }
    }
    T& front() const {
      return _root->value();
    }
    
    bool isEmpty() const {
      return _root == nullptr;
    }
    size_t length() const {
      size_t i = 0;
      auto it = _root;
      while(it){
        i++;
        it = it->next;
      }
      return i;
    }
    size_t count_if(Predicate predicate) const {
      size_t i = 0;
      auto it = _root;
      while(it){
        if (!predicate){
          i++;
        }
        else if (predicate(it->value())) {
          i++;
        }
        it = it->next;
      }
      return i;
    }
    const T* nth(size_t N) const {
      size_t i = 0;
      auto it = _root;
      while(it){
        if(i++ == N)
          return &(it->value());
        it = it->next;
      }
      return nullptr;
    }
    bool remove(const T& t){
      auto it = _root;
      auto pit = _root;
      while(it){
        if(it->value() == t){
          if(it == _root){
            _root = _root->next;
          } else {
            pit->next = it->next;
          }
          
          if (_onRemove) {
            _onRemove(it->value());
          }
          
          delete it;
          return true;
        }
        pit = it;
        it = it->next;
      }
      return false;
    }
    bool remove_first(Predicate predicate){
      auto it = _root;
      auto pit = _root;
      while(it){
        if(predicate(it->value())){
          if(it == _root){
            _root = _root->next;
          } else {
            pit->next = it->next;
          }
          if (_onRemove) {
            _onRemove(it->value());
          }
          delete it;
          return true;
        }
        pit = it;
        it = it->next;
      }
      return false;
    }
    
    void free(){
      while(_root != nullptr){
        auto it = _root;
        _root = _root->next;
        if (_onRemove) {
          _onRemove(it->value());
        }
        delete it;
      }
      _root = nullptr;
    }
};


class StringArray : public AsyncLinkedList<String> {
public:
  
  StringArray() : AsyncLinkedList(nullptr) {}
  
  bool containsIgnoreCase(const String& str){
    for (const auto& s : *this) {
      if (str.equalsIgnoreCase(s)) {
        return true;
      }
    }
    return false;
  }
};




#endif /* STRINGARRAY_H_ */
