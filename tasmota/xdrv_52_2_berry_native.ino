/*
  xdrv_52_3_berry_native.ino - Berry scripting language, native fucnctions

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

#include <berry.h>
#include <Wire.h>

const char kTypeError[] PROGMEM = "type_error";

extern "C" {
  void be_dumpstack(bvm *vm) {
    int32_t top = be_top(vm);
    AddLog(LOG_LEVEL_INFO, "BRY: top=%d", top);
    for (uint32_t i = 1; i <= top; i++) {
      const char * tname = be_typename(vm, i);
      const char * cname = be_classname(vm, i);
      AddLog(LOG_LEVEL_INFO, "BRY: stack[%d] = type='%s' (%s)", i, (tname != nullptr) ? tname : "", (cname != nullptr) ? cname : "");
    }
  }

  // convert to unsigned 8 bits
  static uint8_t to_u8(int32_t v) {
    if (v < 0) { return 0; }
    if (v > 0xFF) { return 0xFF; }
    return v;
  }
  static void map_insert_int(bvm *vm, const char *key, int value)
  {
    be_pushstring(vm, key);
    be_pushint(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
  static void map_insert_bool(bvm *vm, const char *key, bool value)
  {
    be_pushstring(vm, key);
    be_pushbool(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
  // if value == NAN, ignore
  static void map_insert_float(bvm *vm, const char *key, float value)
  {
    if (!isnan(value)) {
      be_pushstring(vm, key);
      be_pushreal(vm, value);
      be_data_insert(vm, -3);
      be_pop(vm, 2);
    }
  }
  static void map_insert_str(bvm *vm, const char *key, const char *value)
  {
    be_pushstring(vm, key);
    be_pushstring(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
  static void map_insert_list_uint8(bvm *vm, const char *key, const uint8_t *value, size_t size)
  {
    be_pushstring(vm, key);

    be_newobject(vm, "list");
    for (uint32_t i=0; i < size; i++) {
      be_pushint(vm, value[i]);
      be_data_push(vm, -2);
      be_pop(vm, 1);
    }
    be_pop(vm, 1);                  // now list is on top

    be_data_insert(vm, -3);         // insert into map, key/value
    be_pop(vm, 2);                  // pop both key and value
  }
  static bool map_find(bvm *vm, const char *key)
  {
    be_getmethod(vm, -1, "find");   // look for "find" method of "Map" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_pushstring(vm, key);         // push string as second argument
    be_call(vm, 2);                 // call wirn 2 parameters (implicit instance and key)
    be_pop(vm, 2);                  // pop 2 arguments, the function is replaced by result
    return !be_isnil(vm, -1);       // true if not 'nil'
  }
  static int32_t get_list_size(bvm *vm) {
    be_getmethod(vm, -1, "size");   // look for "size" method of "list" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_call(vm, 1);                 // call wirn 2 parameters (implicit instance and key)
    int32_t ret = be_toint(vm, -2);
    be_pop(vm, 2);                  // pop 1 argument and return value
    return ret;
  }
  // get item number `index` from list, index must be valid or raises an exception
  static void get_list_item(bvm *vm, int32_t index) {
    be_getmethod(vm, -1, "item");   // look for "size" method of "list" instance
    be_pushvalue(vm, -2);           // put back instance as first argument (implicit instance)
    be_pushint(vm, index);
    // be_dumpstack(vm);
    be_call(vm, 2);                 // call wirn 2 parameters (implicit instance and key)
    be_pop(vm, 2);                  // pop 2 arguments and return value
  }
}

#endif  // USE_BERRY
