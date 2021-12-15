/*
  xdrv_52_1_berry_native.ino - Berry scripting language, native fucnctions

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
  #include "be_vm.h"
  // Call error handler and pop all from stack
  void be_error_pop_all(bvm *vm);
  void be_error_pop_all(bvm *vm) {
    if (vm->obshook != NULL) (*vm->obshook)(vm, BE_OBS_PCALL_ERROR);
    be_pop(vm, be_top(vm));       // clear Berry stack
  }

  #include "be_exec.h"
  #include "be_debug.h"
  void be_dumpstack(bvm *vm) {
    int32_t top = be_top(vm);
    AddLog(LOG_LEVEL_INFO, "BRY: top=%d", top);
    be_tracestack(vm);
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

  // variant of be_raise with string format
  [[ noreturn ]] void be_raisef(bvm *vm, const char *except, const char *msg, ...) {
    // To save stack space support logging for max text length of 128 characters
    char log_data[128];

    va_list arg;
    va_start(arg, msg);
    uint32_t len = ext_vsnprintf_P(log_data, sizeof(log_data)-3, msg, arg);
    va_end(arg);
    if (len+3 > sizeof(log_data)) { strcat(log_data, "..."); }  // Actual data is more
    be_raise(vm, except, log_data);
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

  extern void berry_log_C(const char * berry_buf, ...);
  // Create a class given a global name or a name within a module
  // Case 1: (no dot in name) `lv_wifi_bars` will look for a global variable `lv_wifi_bars`
  // Case 2: (dot in name) `lvgl.lv_obj` will import `lvgl` and look for `lv_obj` within this module
  // returns true if successful and result is top of stack, or false if not found and `nil` is at top of stack
  bbool be_find_class(bvm *vm, const char * cl_name);
  bbool be_find_class(bvm *vm, const char * cl_name) {
    char *saveptr;
    bbool ret = false;

    if (cl_name == NULL) {
      be_pushnil(vm);
      return ret;
    }
    // berry_log_C(">> be_find_class %s", cl_name);
    char cl_name_buf[strlen(cl_name)+1];
    strcpy(cl_name_buf, cl_name);

    char * prefix = strtok_r(cl_name_buf, ".", &saveptr);
    char * suffix = strtok_r(NULL, ".", &saveptr);
    if (suffix) {
    // berry_log_C(">> be_find_class %s - %s", prefix, suffix);
      be_getmodule(vm, prefix);
      ret = be_getmember(vm, -1, suffix);
      // berry_log_C(">> be_find_class ret=%i", ret);
      be_remove(vm, -2);
    } else {
      ret = be_getglobal(vm, prefix);
    }
    return ret;
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
//
// This version skips the first character of the string if it's not a letter,
// the first character is used to indicate the type of the value associated to the key
extern "C" {
  int32_t bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements);
  int32_t bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements) {
    int32_t low = 0;
    int32_t high = total_elements - 1;
    int32_t mid = (low + high) / 2;
    // start a dissect
    while (low <= high) {
      const char * elt = *(const char **) ( ((uint8_t*)table) + mid * elt_size );
      char first_char = elt[0];
      if ( !(first_char >= 'a' && first_char <='z') && !(first_char >= 'A' && first_char <='Z') ) {
        elt++;  // skip first char
      }
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
 * Warning, the following expect all parameters to be 32 bits wide
\*********************************************************************************************/

/*********************************************************************************************\
 * Automatically parse Berry stack and call the C function accordingly
 * 
 * This function takes the n incoming arguments and pushes them as arguments
 * on the stack for the C function:
 * - be_int -> int32_t
 * - be_bool -> int32_t with value 0/1
 * - be_string -> const char *
 * - be_instance -> gets the member "_p" and pushes as void*
 * 
 * This works because C silently ignores any unwanted arguments.
 * There is a strong requirements that all ints and pointers are 32 bits.
 * Float is not supported but could be added. Double cannot be supported because they are 64 bits
 * 
 * Optional argument:
 * - return_type: the C function return value is int32_t and is converted to the
 *   relevant Berry object depending on this char:
 *   '' (default): nil, no value
 *   'i' be_int
 *   'b' be_bool
 *   's' be_str
 * 
 * - arg_type: optionally check the types of input arguments, or throw an error
 *   string of argument types, '+' marks optional arguments
 *   '.' don't care
 *   'i' be_int
 *   'b' be_bool
 *   's' be_string
 *   'c' C callback
 *   'lv_obj' be_instance of type or subtype
 *   '^lv_event_cb' callback of a named class - will call `_lvgl.gen_cb(arg_type, closure, self, lv native pointer)` and expects a callback address in return
 * 
 * Ex: "oii+s" takes 3 mandatory arguments (obj_instance, int, int) and an optional fourth one [,string]
\*********************************************************************************************/
// general form of lv_obj_t* function, up to 4 parameters
// We can only send 32 bits arguments (no 64 bits nor double) and we expect pointers to be 32 bits

#define LVBE_LVGL_GLOB        "_lvgl"
#define LVBE_LVGL_CB_GEN      "gen_cb"

// read a single value at stack position idx, convert to int.
// if object instance, get `_p` member and convert it recursively
int32_t be_convert_single_elt(bvm *vm, int32_t idx, const char * arg_type = nullptr, void * lv_obj_cb = nullptr) {
  int32_t ret = 0;
  char provided_type = 0;
  idx = be_absindex(vm, idx);   // make sure we have an absolute index
  
  // berry_log_C(">> 0 idx=%i arg_type=%s", idx, arg_type ? arg_type : "NULL");
  if (arg_type == nullptr) { arg_type = "."; }    // if no type provided, replace with wildchar
  size_t arg_type_len = strlen(arg_type);

  // handle callbacks first, since a wrong parameter will always yield to a crash
  if (arg_type_len > 1 && arg_type[0] == '^') {     // it is a callback
    arg_type++;   // skip first character
    if (be_isclosure(vm, idx)) {
      be_getglobal(vm, LVBE_LVGL_GLOB);
      be_getmethod(vm, -1, LVBE_LVGL_CB_GEN);
      be_pushvalue(vm, -2);
      be_remove(vm, -3);  // stack contains method + instance
      be_pushstring(vm, arg_type);
      be_pushvalue(vm, idx);
      be_pushvalue(vm, 1);
      be_pushcomptr(vm, lv_obj_cb);
      be_call(vm, 5);
      const void * func = be_tocomptr(vm, -6);
      be_pop(vm, 6);

      // berry_log_C("func=%p", func);
      return (int32_t) func;
    } else {
      be_raise(vm, kTypeError, "Closure expected for callback type");
    }
  }

  // first convert the value to int32
  if      (be_isint(vm, idx))     { ret = be_toint(vm, idx); provided_type = 'i'; }
  else if (be_isbool(vm, idx))    { ret = be_tobool(vm, idx); provided_type = 'b'; }
  else if (be_isstring(vm, idx))  { ret = (int32_t) be_tostring(vm, idx); provided_type = 's'; }
  else if (be_iscomptr(vm, idx))  { ret = (int32_t) be_tocomptr(vm, idx); provided_type = 'c'; }

  // check if simple type was a match
  if (provided_type) {
    bool type_ok = false;
    type_ok = (arg_type[0] == '.');                           // any type is accepted
    type_ok = type_ok || (arg_type[0] == provided_type);      // or type is a match
    type_ok = type_ok || (ret == 0 && arg_type_len != 1);    // or NULL is accepted for an instance
    
    if (!type_ok) {
      berry_log_C("Unexpected argument type '%c', expected '%s'", provided_type, arg_type);
    }
    return ret;
  }

  // non-simple type
  if (be_isinstance(vm, idx))  {
    // check if the instance is a subclass of `bytes()``
    be_getbuiltin(vm, "bytes");    // add "list" class
    if (be_isderived(vm, idx)) {
      be_pop(vm, 1);
      be_getmember(vm, idx, "_buffer");
      be_pushvalue(vm, idx);
      be_call(vm, 1);
      int32_t ret = (int32_t) be_tocomptr(vm, -2);
      be_pop(vm, 2);
      return ret;
    } else {
      be_pop(vm, 1);
      be_getmember(vm, idx, "_p");
      int32_t ret = be_convert_single_elt(vm, -1, nullptr);   // recurse
      be_pop(vm, 1);

      if (arg_type_len > 1) {
        // Check type
        be_classof(vm, idx);
        bool class_found = be_find_class(vm, arg_type);
        // Stack: class_of_idx, class_of_target (or nil)
        if (class_found) {
          if (!be_isderived(vm, -2)) {
            berry_log_C("Unexpected class type '%s', expected '%s'", be_classname(vm, idx), arg_type);
          }
        } else {
          berry_log_C("Unable to find class '%s' (%d)", arg_type, arg_type_len);
        }
        be_pop(vm, 2);
      } else if (arg_type[0] != '.') {
        berry_log_C("Unexpected instance type '%s', expected '%s'", be_classname(vm, idx), arg_type);
      }

      return ret;
    }
  } else {
    be_raise(vm, kTypeError, nullptr);
  }

  return ret;
}

extern "C" {

  /*********************************************************************************************\
   * Generalized virtual members for modules
   * 
   * Takes a pointer to be_constint_t array and size
   * Returns true if a match was found. In such case the result is on Berry stack
   * 
   * Encoding depend on prefix (which is skipped when matching names):
   * 1. `COLOR_WHITE` int value
   * 3. `$SYMBOL_OK"` string pointer
   * 4. `&seg7_font` comptr
  \*********************************************************************************************/
  bool be_module_member(bvm *vm, const be_constint_t * definitions, size_t def_len);
  bool be_module_member(bvm *vm, const be_constint_t * definitions, size_t def_len) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * needle = be_tostring(vm, 1);
      int32_t idx;

      idx = bin_search(needle, &definitions[0].name, sizeof(definitions[0]), def_len);
      if (idx >= 0) {
        // we did have a match
        const char * key = definitions[idx].name;
        switch (key[0]) {
          // switch depending on the first char of the key, indicating the type
          case '$': // string
            be_pushstring(vm, (const char*) definitions[idx].value);
            break;
          case '&': // native function
            be_pushntvfunction(vm, (bntvfunc) definitions[idx].value);
            break;
          default:  // int
            be_pushint(vm, definitions[idx].value);
            break;
        }
        return true;
      }
    }
    return false;
  }
}

/*********************************************************************************************\
 * Manage timeout for Berry code
 *
\*********************************************************************************************/
void BrTimeoutStart(void)  {
  berry.timeout = millis() + USE_BERRY_TIMEOUT;
  if (0 == berry.timeout) {
    berry.timeout = 1;    // rare case when value accidentally computes to zero
  }

}

void BrTimeoutYield(void) {
  if (0 != berry.timeout) {
    BrTimeoutStart();
  }
}

void BrTimeoutReset(void)  {
  berry.timeout = 0;      // remove timer
}

#endif  // USE_BERRY
