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
  #include "be_exec.h"
  void be_dumpstack(bvm *vm) {
    int32_t top = be_top(vm);
    AddLog(LOG_LEVEL_INFO, "BRY: top=%d", top);
    for (uint32_t i = 1; i <= top; i++) {
      const char * tname = be_typename(vm, i);
      const char * cname = be_classname(vm, i);
      if (be_isstring(vm, i)) {
        cname = be_tostring(vm, i);
      }
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
  int32_t member_find(bvm *vm, const char *key, int32_t default_value) {
    int32_t ret = default_value;
    if (be_getmember(vm, -1, key)) {
      if (be_isint(vm, -1)) {
        ret = be_toint(vm, -1);
      }
    }
    be_pop(vm, 1);
    return ret;
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

  // create an object from the pointer and a class name
  // on return, instance is pushed on the stack
  void lv_create_object(bvm *vm, const char * class_name, void * ptr);
  void lv_create_object(bvm *vm, const char * class_name, void * ptr) {
    if (ptr == nullptr) {
        be_throw(vm, BE_MALLOC_FAIL);
    }

    be_getglobal(vm, class_name);   // stack = class
    be_call(vm, 0);                 // instanciate, stack = instance
    be_getmember(vm, -1, "init");   // stack = instance, init_func
    be_pushvalue(vm, -2);           // stack = instance, init_func, instance
    be_pushcomptr(vm, ptr);         // stack = instance, init_func, instance, ptr
    be_call(vm, 2);                 // stack = instance, ret, instance, ptr
    be_pop(vm, 3);                  // stack = instance
  }

}


#define LV_OBJ_CLASS    "lv_obj"

/*********************************************************************************************\
 * Automatically parse Berry stack and call the C function accordingly
 * 
 * This function takes the n incoming arguments and pushes them as arguments
 * on the stack for the C function:
 * - be_int -> int32_t
 * - be_bool -> int32_t with value 0/1
 * - be_string -> const char *
 * - be_instance -> gets the member ".p" and pushes as void*
 * 
 * This works because C silently ignores any unwanted arguments.
 * There is a strong requirements that all ints and pointers are 32 bits.
 * Float is not supported but could be added. Double cannot be supported because they are 64 bits
 * 
 * Optional argument:
 * - return_type: the C function return value is int32_t and is converted to the
 *   relevant Berry object depending on this char:
 *   '0' (default): nil, no value
 *   'i' be_int
 *   'b' be_boot
 *   's' be_str
 *   'o' instance of `lv_obj` (needs to be improved)
 * 
 * - arg_type: optionally check the types of input arguments, or throw an error
 *   string of argument types, '+' marks optional arguments
 *   '.' don't care
 *   'i' be_int
 *   'b' be_bool
 *   's' be_string
 *   'lv_obj' be_instance of type or subtype
 *   '0'..'5' callback
 * 
 * Ex: "oii+s" takes 3 mandatory arguments (obj_instance, int, int) and an optional fourth one [,string]
\*********************************************************************************************/
// general form of lv_obj_t* function, up to 4 parameters
// We can only send 32 bits arguments (no 64 bits nor double) and we expect pointers to be 32 bits

// read a single value at stack position idx, convert to int.
// if object instance, get `.p` member and convert it recursively
int32_t be_convert_single_elt(bvm *vm, int32_t idx, const char * arg_type = nullptr) {
  int32_t ret = 0;
  char provided_type = 0;

  // first convert the value to int32
  if      (be_isint(vm, idx))     { ret = be_toint(vm, idx); provided_type = 'i'; }
  else if (be_isbool(vm, idx))    { ret = be_tobool(vm, idx); provided_type = 'b'; }
  else if (be_isstring(vm, idx))  { ret = (int32_t) be_tostring(vm, idx); provided_type = 's'; }
  else if (be_iscomptr(vm, idx))  { ret = (int32_t) be_tocomptr(vm, idx); provided_type = 'i'; }

  // check if simple type was a match
  if (provided_type) {
    if (arg_type == nullptr) { return ret; }    // if no type check, return
    size_t arg_type_len = strlen(arg_type);
    if ((arg_type_len != 1) || ((arg_type[0] != provided_type) && arg_type[0] != '.') ) {
      berry_log_P("Unexpected argument type '%c', expected '%s'", provided_type, arg_type);
    }
    return ret;
  }

  // non-simple type
  if (be_isinstance(vm, idx))  {
    be_getmember(vm, idx, ".p");
    int32_t ret = be_convert_single_elt(vm, -1, nullptr);   // recurse
    be_pop(vm, 1);

    if (arg_type != nullptr) {
      // Check type
      be_classof(vm, idx);
      bool class_found = be_getglobal(vm, arg_type);
      // Stack: class_of_idx, class_of_target (or nil)
      if (class_found) {
        if (!be_isderived(vm, -2)) {
          berry_log_P("Unexpected class type '%s', expected '%s'", be_classname(vm, -2), arg_type);
        }
      } else {
        berry_log_P("Unable to find class '%s'", arg_type);
      }
      be_pop(vm, 2);
    }

    return ret;
  } else {
    be_raise(vm, kTypeError, nullptr);
  }

  // 

  return ret;
}


#endif  // USE_BERRY
