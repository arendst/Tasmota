# TasmotaLList

A lightweight, header-only singly linked list template, optimized for low code size and low RAM usage on ESP8266/ESP32. It's the standard container Tasmota uses whenever it needs a dynamic, growable list of items (backlog commands, MQTT subscriptions, timed commands, Zigbee devices, etc).

## Why not `std::list` or `std::vector`?

`LList<T>` trades a richer API for a much smaller code footprint, which matters on constrained devices. It only allocates one node per element (no over-allocation like `std::vector`), and the whole implementation is a single header.

## Basic usage

```cpp
#include <LList.h>

typedef struct {
  uint32_t time;
  char*    command;
} tTimedCmnd;

LList<tTimedCmnd> timed_cmnd;   // declare a list of tTimedCmnd
```

### Adding elements

```cpp
// add an empty element at the end and fill it in
tTimedCmnd &elem = timed_cmnd.addToLast();
elem.time = millis() + 1000;
elem.command = strdup("some command");

// or add at the head
tTimedCmnd &head_elem = timed_cmnd.addHead();

// or insert a copy of an existing value
MyStruct val = {...};
my_list.addHead(val);

// or insert at an arbitrary position
my_list.insertAt(2);
```

`addHead()`, `addToLast()` and `insertAt()` all return a reference `T&` to the newly created element so you can fill it in directly.

### Iterating

Range-based `for` works out of the box (both mutable and `const`):

```cpp
for (auto &elem : timed_cmnd) {
  if (TimeReached(elem.time)) {
    // ...
  }
}
```

### Removing elements

Remove by pointer to the value (this is the normal pattern, safe to call from inside a `for` loop on the current element):

```cpp
for (auto &elem : timed_cmnd) {
  if (strcmp(command, elem.command) == 0) {
    free(elem.command);          // free any owned memory first
    timed_cmnd.remove(&elem);     // then remove the node
    break;                        // don't touch elem after this
  }
}
```

Other removal helpers:

```cpp
timed_cmnd.removeHead();   // remove first element
timed_cmnd.reset();        // remove all elements (also called by the destructor)
```

`removeHead()` and `remove()` return `bool` (`true` if something was actually removed). The pointer passed to `remove()` (and the node it referred to) must not be used afterwards.

### Reading

```cpp
timed_cmnd.isEmpty();   // true if list has no elements
timed_cmnd.length();    // number of elements (O(n))
timed_cmnd.head();      // pointer to first element's value, or nullptr
timed_cmnd.at(2);       // pointer to element at index 2, or nullptr (O(n))
```

## Using it as a base class

For a named, self-documenting list type, inherit from `LList<T>` instead of using it as a plain member:

```cpp
class Z_Data_Set : public LList<Z_Data> {
public:
  // add extra methods/state specific to this list
  Z_Data & getByType(uint8_t type);
  ...
};
```

This is how Tasmota implements `Z_attribute_list`, `Z_Data_Set` and `Z_plugin_templates` in the Zigbee driver.

## Gotchas

- **No copy semantics.** `LList` cannot be copied or assigned (copy constructor and copy assignment are deleted). A list owns its nodes and frees them in its destructor, so pass it by reference or pointer instead.
- **Element memory ownership.** If `T` contains a pointer to heap memory (e.g. `char* command`), the list does not manage that memory. You must `free()`/`delete` it yourself before removing/resetting the element.
- **Don't use a value after removing it.** `remove(&elem)` and `removeHead()` free the underlying node; don't dereference `elem` (or any pointer into it) afterwards.
