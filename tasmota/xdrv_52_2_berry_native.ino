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
const char kInternalError[] PROGMEM = "intenal_error";

extern "C" {

  /*********************************************************************************************\
   * Support for Berry int constants
   * as virtual members
  \*********************************************************************************************/
  typedef struct be_constint_t {
      const char * name;
      int32_t      value;
  } be_constint_t;

}

/*********************************************************************************************\
 * LVGL top level virtual members
 * 
 * Responds to virtual constants
\*********************************************************************************************/
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

/*********************************************************************************************\
 * Binary search for dynamic attributes
 * 
 * Names need to be sorted
\*********************************************************************************************/
// binary search within an array of sorted strings
// the first 4 bytes are a pointer to a string
// returns 0..total_elements-1 or -1 if not found
extern "C" {
  int32_t bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements) {
    int32_t low = 0;
    int32_t high = total_elements - 1;
    int32_t mid = (low + high) / 2;
    // start a dissect
    while (low <= high) {
      const char * elt = *(const char **) ( ((uint8_t*)table) + mid * elt_size );
      int32_t comp = strcmp(needle, elt);
      if (comp < 0) {
        high = mid - 1;
      } else if (comp > 0) {
        low = mid + 1;
      } else {
        break;
      }
      mid = (low + high) / 2;
    }
    if (low <= high) {
      return mid;
    } else {
      return -1;
    }
  }
}

/*********************************************************************************************\
 * Generalized callbacks
 * 
\*********************************************************************************************/
extern "C" {
  
  typedef int32_t (*berry_callback_t)(int32_t v0, int32_t v1, int32_t v2, int32_t v3);

  int32_t call_berry_cb(int32_t num, int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
    // call berry cb dispatcher
    int32_t ret = 0;
    // get the 'tasmota' object (global) and call 'cb_dispatch'
    be_getglobal(berry.vm, PSTR("tasmota"));
    if (!be_isnil(berry.vm, -1)) {
      be_getmethod(berry.vm, -1, PSTR("cb_dispatch"));

      if (!be_isnil(berry.vm, -1)) {
        be_pushvalue(berry.vm, -2); // add instance as first arg
        // push all args as ints (may be revised)
        be_pushint(berry.vm, num);
        be_pushint(berry.vm, v0);
        be_pushint(berry.vm, v1);
        be_pushint(berry.vm, v2);
        be_pushint(berry.vm, v3);

        be_pcall(berry.vm, 6);   // 5 arguments
        be_pop(berry.vm, 6);

        if (be_isint(berry.vm, -1) || be_isnil(berry.vm, -1)) {  // sanity check
          if (be_isint(berry.vm, -1)) {
            ret = be_toint(berry.vm, -1);
          }
          // All good, we can proceed
          be_pop(berry.vm, 2);    // remove tasmota instance and result
          return ret;
        }
      }
      be_pop(berry.vm, 1);
    }
    be_pop(berry.vm, 1);
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BERRY "can't call 'tasmota.cb_dispatch'"));
    return 0;
  }

  #define BERRY_CB(n) int32_t berry_cb_##n(int32_t v0, int32_t v1, int32_t v2, int32_t v3) { return call_berry_cb(n, v0, v1, v2, v3); }
  // list the callbacks
  BERRY_CB(0);
  BERRY_CB(1);
  BERRY_CB(2);
  BERRY_CB(3);
  BERRY_CB(4);
  BERRY_CB(5);
  BERRY_CB(6);
  BERRY_CB(7);
  BERRY_CB(8);
  BERRY_CB(9);
  BERRY_CB(10);
  BERRY_CB(11);
  BERRY_CB(12);
  BERRY_CB(13);
  BERRY_CB(14);
  BERRY_CB(15);
  BERRY_CB(16);
  BERRY_CB(17);
  BERRY_CB(18);
  BERRY_CB(19);

  // array of callbacks
  berry_callback_t berry_callback_array[] {
    berry_cb_0,
    berry_cb_1,
    berry_cb_2,
    berry_cb_3,
    berry_cb_4,
    berry_cb_5,
    berry_cb_6,
    berry_cb_7,
    berry_cb_8,
    berry_cb_9,
    berry_cb_10,
    berry_cb_11,
    berry_cb_12,
    berry_cb_13,
    berry_cb_14,
    berry_cb_15,
    berry_cb_16,
    berry_cb_17,
    berry_cb_18,
    berry_cb_19,
  };
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

#define LVBE_MAX_CALLBACK     6   // max 6 callbackss
#define LVBE_LVGL_CB          "_lvgl_cb"
#define LVBE_LVGL_CB_OBJ      "_lvgl_cb_obj"        // remember which object it was linked to
#define LVBE_LVGL_CB_DISPATCH "_lvgl_cb_dispatch"

// General form of callback
typedef int32_t (*lvbe_callback)(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4);
int32_t lvbe_callback_x(uint32_t n, struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4);

// We define 6 callback vectors, this may need to be raised
int32_t lvbe_callback_0(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(0, obj, v1, v2, v3, v4);
}
int32_t lvbe_callback_1(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(1, obj, v1, v2, v3, v4);
}
int32_t lvbe_callback_2(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(2, obj, v1, v2, v3, v4);
}
int32_t lvbe_callback_3(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(3, obj, v1, v2, v3, v4);
}
int32_t lvbe_callback_4(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(4, obj, v1, v2, v3, v4);
}
int32_t lvbe_callback_5(struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  return lvbe_callback_x(5, obj, v1, v2, v3, v4);
}

const lvbe_callback lvbe_callbacks[LVBE_MAX_CALLBACK] = {
  lvbe_callback_0,
  lvbe_callback_1,
  lvbe_callback_2,
  lvbe_callback_3,
  lvbe_callback_4,
  lvbe_callback_5,
};

int32_t lvbe_callback_x(uint32_t n, struct _lv_obj_t * obj, int32_t v1, int32_t v2, int32_t v3, int32_t v4) {
  // berry_log_P(">>>: Callback called n=%i obj=0x%08X v1=%i v2=%i", n, obj, v1, v2);
  be_getglobal(berry.vm, LVBE_LVGL_CB_DISPATCH); // stack: List
  be_pushint(berry.vm, n);
  be_pushint(berry.vm, (int32_t) obj);
  be_pushint(berry.vm, v1);
  be_pushint(berry.vm, v2);
  be_pushint(berry.vm, v3);
  be_pushint(berry.vm, v4);
  be_pcall(berry.vm, 6);
  int32_t ret = be_toint(berry.vm, -7);
  be_pop(berry.vm, 7);
  // berry_log_P(">>>: Callback called out %d ret=%i", n, ret);
  return ret;
}

// read a single value at stack position idx, convert to int.
// if object instance, get `.p` member and convert it recursively
int32_t be_convert_single_elt(bvm *vm, int32_t idx, const char * arg_type = nullptr, int32_t lv_obj_cb = 0) {
  int32_t ret = 0;
  char provided_type = 0;
  idx = be_absindex(vm, idx);   // make sure we have an absolute index
  if (arg_type == nullptr) { arg_type = "."; }    // if no type provided, replace with wildchar
  size_t arg_type_len = strlen(arg_type);

  // handle callbacks first, since a wrong parameter will always yield to a crash
  if (arg_type_len == 1 && arg_type[0] >= '0' && arg_type[0] < '0' + LVBE_MAX_CALLBACK) {
    if (be_isclosure(vm, idx)) {
      // we're good
      // berry_log_P(">> closure found idx %d", idx);
      uint32_t cb_index = arg_type[0] - '0';
      lvbe_callback func = lvbe_callbacks[cb_index];
      // register the object

      // record the closure
      be_getglobal(vm, LVBE_LVGL_CB);
      be_getmember(vm, -1, ".p");
      be_pushint(vm, cb_index);
      be_getindex(vm, -2);
      // be_dumpstack(vm);
      // stack: _lvgl_cb, list.p, index, map
      be_moveto(vm, -1, -4);
      be_pop(vm, 3);
      // be_dumpstack(vm);
      // stack: map
      be_getmember(vm, -1, ".p");
      be_pushint(vm, lv_obj_cb);  // key - lv_obj
      be_pushvalue(vm, idx);      // value - closure
      // stack map, map.p, key, value
      be_setindex(vm, -3);
      // be_dumpstack(vm);
      // stack map, map.p, key, value
      be_pop(vm, 4);      // clean
      // be_dumpstack(vm);

      // record the object, it is always index #1
      be_getglobal(vm, LVBE_LVGL_CB_OBJ);
      be_getmember(vm, -1, ".p");
      be_pushint(vm, cb_index);
      be_getindex(vm, -2);
      be_moveto(vm, -1, -4);
      be_pop(vm, 3);
      // stack: map
      be_getmember(vm, -1, ".p");
      be_pushint(vm, lv_obj_cb);  // key - lv_obj as int
      be_pushvalue(vm, 1);  // value - lv_obj
      // stack map, map.p, key, value
      be_setindex(vm, -3);
      be_pop(vm, 4);      // clean


      return (int32_t) func;
    } else {
      be_raise(vm, kTypeError, "Closure expected for callback type");
    }
  }

  // first convert the value to int32
  if      (be_isint(vm, idx))     { ret = be_toint(vm, idx); provided_type = 'i'; }
  else if (be_isbool(vm, idx))    { ret = be_tobool(vm, idx); provided_type = 'b'; }
  else if (be_isstring(vm, idx))  { ret = (int32_t) be_tostring(vm, idx); provided_type = 's'; }
  else if (be_iscomptr(vm, idx))  { ret = (int32_t) be_tocomptr(vm, idx); provided_type = 'i'; }

  // check if simple type was a match
  if (provided_type) {
    bool type_ok = false;
    type_ok = (arg_type[0] == '.');                           // any type is accepted
    type_ok = type_ok || (arg_type[0] == provided_type);      // or type is a match
    type_ok = type_ok || (ret == 0 && arg_type_len != 1);    // or NULL is accepted for an instance
    
    if (!type_ok) {
      berry_log_P("Unexpected argument type '%c', expected '%s'", provided_type, arg_type);
    }
    return ret;
  }

  // non-simple type
  if (be_isinstance(vm, idx))  {
    be_getmember(vm, idx, ".p");
    int32_t ret = be_convert_single_elt(vm, -1, nullptr);   // recurse
    be_pop(vm, 1);

    if (arg_type_len > 1) {
      // Check type
      be_classof(vm, idx);
      bool class_found = be_getglobal(vm, arg_type);
      // Stack: class_of_idx, class_of_target (or nil)
      if (class_found) {
        if (!be_isderived(vm, -2)) {
          berry_log_P("Unexpected class type '%s', expected '%s'", be_classname(vm, idx), arg_type);
        }
      } else {
        berry_log_P("Unable to find class '%s' (%d)", arg_type, arg_type_len);
      }
      be_pop(vm, 2);
    } else if (arg_type[0] != '.') {
      berry_log_P("Unexpected instance type '%s', expected '%s'", be_classname(vm, idx), arg_type);
    }

    return ret;
  } else {
    be_raise(vm, kTypeError, nullptr);
  }

  // 

  return ret;
}


#endif  // USE_BERRY
