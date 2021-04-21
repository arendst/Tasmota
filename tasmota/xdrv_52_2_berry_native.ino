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
 *   'o' be_instance
 * 
 * Ex: "oii+s" takes 3 mandatory arguments (obj_instance, int, int) and an optional fourth one [,string]
\*********************************************************************************************/
// general form of lv_obj_t* function, up to 4 parameters
// We can only send 32 bits arguments (no 64 bits nor double) and we expect pointers to be 32 bits

// read a single value at stack position idx, convert to int.
// if object instance, get `.p` member and convert it recursively
int32_t be_convert_single_elt(bvm *vm, int32_t idx);
int32_t be_convert_single_elt(bvm *vm, int32_t idx) {
  if      (be_isint(vm, idx))     { return be_toint(vm, idx); }
  else if (be_isbool(vm, idx))    { return be_tobool(vm, idx); }
  else if (be_isstring(vm, idx))  { return (int32_t) be_tostring(vm, idx); }
  else if (be_iscomptr(vm, idx))  { return (int32_t) be_tocomptr(vm, idx); }
  else if (be_isinstance(vm, idx))  {
    be_getmember(vm, idx, ".p");
    int32_t ret = be_convert_single_elt(vm, -1);   // recurse
    be_pop(vm, 1);
    return ret;
  } else {
    be_raise(vm, kTypeError, nullptr);
  }
}

typedef int32_t (*fn_any_callable)(int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4);
int be_call_c_func(bvm *vm, void * func, const char * return_type = nullptr, const char * arg_type = nullptr) {
  int32_t p[5] = {0,0,0,0,0};
  int32_t argc = be_top(vm); // Get the number of arguments
  // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func argc = %d", argc);
  for (uint32_t i = 0; i < argc; i++) {
    uint32_t idx = i+1;
    p[i] = be_convert_single_elt(vm, idx);
  }

  fn_any_callable f = (fn_any_callable) func;
  // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func(%p) - %p,%p,%p,%p,%p - %s", f, p[0], p[1], p[2], p[3], p[4], return_type);
  int32_t ret = (*f)(p[0], p[1], p[2], p[3], p[4]);
  // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, ret = %p", ret);
  if ((return_type == nullptr) || (strlen(return_type) == 0))       { be_return_nil(vm); }  // does not return
  else if (strlen(return_type) == 1) {
    switch (return_type[0]) {
      case 'i':   be_pushint(vm, ret); break;
      case 'b':   be_pushbool(vm, ret);  break;
      case 's':   be_pushstring(vm, (const char*) ret);  break;
      default:    be_raise(vm, "internal_error", "Unsupported return type"); break;
    }
    be_return(vm);
  } else { // class name
  // AddLog(LOG_LEVEL_INFO, ">> be_call_c_func, create_obj", ret);
    be_getglobal(vm, return_type);  // stack = class
    be_pushcomptr(vm, (void*) -1);         // stack = class, -1
    be_pushcomptr(vm, (void*) ret);         // stack = class, -1, ptr
    be_call(vm, 2);                 // instanciate with 2 arguments, stack = instance, -1, ptr
    be_pop(vm, 2);                  // stack = instance
    be_return(vm);
  }
}

#endif  // USE_BERRY
