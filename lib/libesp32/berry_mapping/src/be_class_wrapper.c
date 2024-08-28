/*********************************************************************************************\
 * Class wrappers for native objects
 * 
 * These class are simple wrappers (containers) for a pointer of an external object.
 * The pointer is stored interanlly by the class.
 * 
 * The constructor of this class must accept the first argument to be `comptr`,
 * in such case, the constructor must store the pointer.
 * The class is not supposed to free the object at `deinit` time.
\*********************************************************************************************/

#include "be_mapping.h"
#include "be_exec.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#if BE_USE_SINGLE_FLOAT
  #define mathfunc(func)        func##f
#else
  #define mathfunc(func)        func
#endif

/* Ubuntu 22.04 LTS seems to have an invalid or missing signature for strtok_r, forcing a correct one */
extern char *strtok_r(char *str, const char *delim, char **saveptr);

typedef intptr_t (*fn_any_callable)(intptr_t p0, intptr_t p1, intptr_t p2, intptr_t p3,
                                    intptr_t p4, intptr_t p5, intptr_t p6, intptr_t p7);

/*********************************************************************************************\
 * Converision from real <-> int
 * 
 * Warning, works only if sizeof(intptr_t) == sizeof(breal)
 * On ESP32, int=32bits, real=float (32bits)
\*********************************************************************************************/
static intptr_t realasint(breal v) {
  intptr_t i;
  i = *((intptr_t*) &v);
  return i;
}

static breal intasreal(intptr_t v) {
  breal r;
  r = *((breal*) &v);
  return r;
}

/*********************************************************************************************\
 * Create an object of `class_name` given an external poinrt `ptr`.
 * 
 * Instanciates the class and calls `init()` with `ptr` wrapped in `comptr` as single arg.
 * Both arguments but nost bu NULL.
 * 
 * On return, the created instance is top of stack.
\*********************************************************************************************/
void be_create_class_wrapper(bvm *vm, const char * class_name, void * ptr) {
  if (ptr == NULL) {
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


/*********************************************************************************************\
 * Find an object by global or composite name.
 * 
 * I.e. `lv.lv_object` will check for a global called `lv` and a member `lv_object`
 * 
 * Only supports one level of depth, meaning a class within a module.
 * Does not check the type of the object found.
 * 
 * Arguments:
 *   `name`: can be NULL, in such case considers the member as not found
 * 
 * Case 1: (no dot in name) `lv_wifi_bars` will look for a global variable `lv_wifi_bars`
 * Case 2: (dot in name) `lvgl.lv_obj` will get global `lvgl` and look for `lv_obj` within this module
 * 
 * Returns the number of elements pushed on the stack: 1 for module, 2 for instance method, 0 if not found
\*********************************************************************************************/
int be_find_global_or_module_member(bvm *vm, const char * name) {
  char *saveptr;

  if (name == NULL) {
    be_pushnil(vm);
    return 0;
  }
  char name_buf[strlen(name)+1];
  strcpy(name_buf, name);

  char * prefix = strtok_r(name_buf, ".", &saveptr);
  char * suffix = strtok_r(NULL, ".", &saveptr);
  if (suffix) {
    if (!be_getglobal(vm, prefix)) {
      // global not found, try module
      be_pop(vm, 1);
      if (!be_getmodule(vm, prefix)) {
        return 0;
      }
    }
    if (!be_isnil(vm, -1)) {
      if (be_getmember(vm, -1, suffix)) {
        if (be_isinstance(vm, -2)) {  // instance, so we need to push method + instance
          be_pushvalue(vm, -2);
          be_remove(vm, -3);
          return 2;
        } else {  // not instane, so keep only the top object
          be_remove(vm, -2);
          return 1;
        }
      } else {
        be_pop(vm, 2);
        return 0;
      }
    }
    be_pop(vm, 1);  // remove nil
    return 0;
  } else {  // no suffix, get the global object
    if (be_getglobal(vm, prefix)) {
      return 1;
    }
    be_pop(vm, 1);
    return 0;
  }
}


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
 *   'f' be_real (float)
 *   'b' be_bool
 *   's' be_str
 *   '$' be_str but the buffer must be `free()`ed
 *   '&' bytes() object, pointer to buffer returned, and size passed with an additional (size_t*) argument
 * 
 * - arg_type: optionally check the types of input arguments, or throw an error
 *   string of argument types, '[' indicates that the following parameters are optional
 *   '.' don't care
 *   'i' be_int
 *   'b' be_bool
 *   's' be_string
 *   'f' be_real (float)
 *   'c' C comptr (general pointer)
 *   '-': skip argument and ignore
 *   '~': send the length of the previous bytes() buffer (or raise an exception if no length known)
 *   'lv_obj' be_instance of type or subtype
 *   '^lv_event_cb^' callback of a named class - will call `_lvgl.gen_cb(arg_type, closure, self)` and expects a callback address in return
 *   '@': pass a pointer to the Berry VM (virtual parameter added, must be the first argument)
 * 
 * Ex: ".ii" takes 3 arguments, first one is any type, followed by 2 ints
\*********************************************************************************************/
// general form of lv_obj_t* function, up to 4 parameters
// We can only send 32 bits arguments (no 64 bits nor double) and we expect pointers to be 32 bits

// read a single value at stack position idx, convert to int.
// if object instance, get `_p` member and convert it recursively
intptr_t be_convert_single_elt(bvm *vm, int idx, const char * arg_type, int *buf_len) {
  // berry_log_C("be_convert_single_elt(idx=%i, argtype='%s', type=%s)", idx, arg_type ? arg_type : "", be_typename(vm, idx));
  intptr_t ret = 0;
  char provided_type = 0;
  idx = be_absindex(vm, idx);   // make sure we have an absolute index
  
  // berry_log_C(">> 0 idx=%i arg_type=%s", idx, arg_type ? arg_type : "NULL");
  if (arg_type == NULL) { arg_type = "."; }    // if no type provided, replace with wildchar
  size_t arg_type_len = strlen(arg_type);

  // handle callbacks first, since a wrong parameter will always yield to a crash
  if (arg_type_len > 1 && arg_type[0] == '^') {     // it is a callback
    arg_type++;   // skip first character
    if (be_isclosure(vm, idx)) {
      ret = be_find_global_or_module_member(vm, "cb.make_cb");
      // ret may 1 if direct function, or 2 if method+instance. 0 indicates an error
      if (ret) {
        be_pushvalue(vm, idx);            // push function/closure as arg1
        be_pushvalue(vm, 1);              // push `self` as arg2
        be_pushstring(vm, arg_type);      // push name of the callback type (string) as arg3
        be_call(vm, 2 + ret);
        const void * func = be_tocomptr(vm, -(3 + ret));
        be_pop(vm, 3 + ret);

        // berry_log_C("func=%p", func);
        return (intptr_t) func;
      } else {
        be_raisef(vm, "type_error", "Can't find callback generator: 'cb.make_cb'");
      }
    } else if (be_iscomptr(vm, idx)) {
      // if it's a pointer, just pass it without any change
      return (intptr_t) be_tocomptr(vm, idx);;
    } else {
      be_raise(vm, "type_error", "Closure expected for callback type");
    }
  }

  // first convert the value to int32
  if      (be_isint(vm, idx))     {
    if (arg_type[0] == 'f') {
      ret = realasint((float)be_toint(vm, idx)); provided_type = 'f';
    } else {
      ret = be_toint(vm, idx); provided_type = 'i'; }
  }
  else if (be_isbool(vm, idx))    { ret = be_tobool(vm, idx); provided_type = 'b'; }
  else if (be_isstring(vm, idx))  { ret = (intptr_t) be_tostring(vm, idx); provided_type = 's'; }
  else if (be_iscomptr(vm, idx))  { ret = (intptr_t) be_tocomptr(vm, idx); provided_type = 'c'; }
  else if (be_isnil(vm, idx))     { ret = 0; provided_type = 'c'; }
  else if (be_isreal(vm, idx))    { ret = realasint(be_toreal(vm, idx)); provided_type = 'f'; }

  // check if simple type was a match
  if (provided_type) {
    bbool type_ok = bfalse;
    type_ok = (arg_type[0] == '.');                           // any type is accepted
    type_ok = type_ok || (arg_type[0] == provided_type && arg_type[1] == 0);      // or type is a match (single char only)
    type_ok = type_ok || (ret == 0 && arg_type_len != 1);     // or NULL is accepted for an instance
    type_ok = type_ok || (ret == 0 && arg_type[0] == 's' && arg_type[1] == 0);  // accept nil for string, can be dangerous
    if (!type_ok) {
      if ((provided_type == 'f') && (arg_type[0] == 'i') && (arg_type[1] == 0)) {
        // special case: float is accepted as int
        breal v_real = be_toreal(vm, idx);
        ret = mathfunc(round)(v_real);
        provided_type = 'i';
        type_ok = btrue;
      }
    }
    if (!type_ok) {
      be_raisef(vm, "type_error", "Unexpected argument type '%c', expected '%s'", provided_type, arg_type);
    }
    // berry_log_C("be_convert_single_elt provided type=%i", ret);
    return ret;
  }

  // non-simple type
  if (be_isinstance(vm, idx))  {
    // check if the instance is a subclass of `bytes()``
    if (be_isbytes(vm, idx)) {
      size_t len;
      ret = (intptr_t) be_tobytes(vm, idx, &len);
      if (buf_len) { *buf_len = (int) len; }
      return ret;
    } else {
      // we accept either `_p` or `.p` attribute to retrieve a pointer
      if (!be_getmember(vm, idx, "_p")) {
        be_pop(vm, 1);    // remove `nil`
        be_getmember(vm, idx, ".p");
      } // else `nil` is on top of stack
      ret = be_convert_single_elt(vm, -1, NULL, NULL);   // recurse
      be_pop(vm, 1);

      if (arg_type_len > 1) {
        // Check type
        be_classof(vm, idx);
        int class_found = be_find_global_or_module_member(vm, arg_type);
        // Stack: class_of_idx, class_of_target (or nil)
        if (class_found) {
          if (!be_isderived(vm, -2)) {
            be_raisef(vm, "type_error", "Unexpected class type '%s', expected '%s'", be_classname(vm, idx), arg_type);
          }
        } else {
          be_raisef(vm, "value_error", "Unable to find class '%s' (%d)", arg_type, arg_type_len);
        }
        be_pop(vm, 2);
      } else if (arg_type[0] != '.') {
        be_raisef(vm, "value_error", "Unexpected instance type '%s', expected '%s'", be_classname(vm, idx), arg_type);
      }

      return ret;
    }
  } else {
    be_raisef(vm, "value_error", "Unexpected '%s'", be_typename(vm, idx));
  }

  return ret;
}

/*********************************************************************************************\
 * Calling any LVGL function with auto-mapping
 * 
\*********************************************************************************************/

// check input parameters, and create callbacks if needed
// change values in place
//
// Format:
// - either a lowercase character encoding for a simple type
//   - 'b': bool
//   - 'i': int (int32_t)
//   - 's': string (const char *)
//   - '.': any argument (no check)
//   - '-': skip argument and ignore
//   - '~': send the length of the previous bytes() buffer (or raise an exception if no length known)
//   - if return type is '&' (bytes), an implicit additional parameter is passed as (size_t*) to return the length in bytes
//
// - a class name surroungded by parenthesis
//   - '(lv_button)' -> lv_button class or derived
//   - '[lv_event_cb]' -> callback type, still prefixed with '^' to mark that it is cb
//
// Returns the number of parameters sent to the function
//
int be_check_arg_type(bvm *vm, int arg_start, int argc, const char * arg_type, intptr_t p[8]) {
  bbool arg_type_check = (arg_type != NULL);      // is type checking activated
  int32_t arg_idx = 0;              // position in arg_type string
  bbool arg_optional = bfalse;      // are remaining types optional?
  char type_short_name[32];

  uint32_t p_idx = 0; // index in p[], is incremented with each parameter except '-'
  int32_t buf_len = -1;   // stores the length of a bytes() buffer to be used as '~' attribute

  // special case when first parameter is '@', pass pointer to VM
  if (NULL != arg_type && arg_type[arg_idx] == '@') {
    arg_idx++;
    p[p_idx] = (intptr_t) vm;
    p_idx++;
  }

  for (int i = 0; i < argc; i++) {
    type_short_name[0] = 0;   // clear string
    // extract individual type
    if (arg_type) {
      if (arg_type[arg_idx] == '[' || arg_type[arg_idx] == ']') {   // '[' is a marker that following parameters are optional and default to NULL
        arg_optional = btrue;
        arg_idx++;
      }
      switch (arg_type[arg_idx]) {
        case '-':
          arg_idx++;
          continue;   // ignore current parameter and advance
        case '.':
        case 'a'...'z':
          type_short_name[0] = arg_type[arg_idx];
          type_short_name[1] = 0;
          arg_idx++;
          break;
        case '(':
        case '^':
          {
            uint32_t prefix = 0;
            if (arg_type[arg_idx] == '^') {
              type_short_name[0] = '^';
              type_short_name[1] = 0;
              prefix = 1;
            }
            uint32_t offset = 0;
            arg_idx++;
            while (arg_type[arg_idx + offset] != ')' && arg_type[arg_idx + offset] != '^' && arg_type[arg_idx + offset] != 0 && offset+prefix+1 < sizeof(type_short_name)) {
              type_short_name[offset+prefix] = arg_type[arg_idx + offset];
              type_short_name[offset+prefix+1] = 0;
              offset++;
            }
            if (arg_type[arg_idx + offset] == 0) {
              arg_type = NULL;   // no more parameters, stop iterations
            }
            arg_idx += offset + 1;
          }
          break;
        case 0:
          arg_type = NULL;   // stop iterations
          break;
      }
    }
    // berry_log_C(">> be_call_c_func arg %i, type %s", i, arg_type_check ? type_short_name : "<null>");
    if (arg_type_check && type_short_name[0] == 0) {
      be_raisef(vm, "value_error", "Too many arguments");
    }
    p[p_idx] = be_convert_single_elt(vm, i + arg_start, arg_type_check ? type_short_name : NULL, (int*)&buf_len);
    // berry_log_C("< ret[%i]=%i", p_idx, p[p_idx]);
    p_idx++;

    if (arg_type && arg_type[arg_idx] == '~') { // if next argument is virtual
      if (buf_len < 0) {
        be_raisef(vm, "value_error", "no bytes() length known");
      }
      p[p_idx] = buf_len; // add the previous buffer len
      p_idx++;
      arg_idx++; // skip this arg
    }
  }

  // check if we are missing arguments
  if (!arg_optional && arg_type && arg_type[arg_idx] != 0 && arg_type[arg_idx] != '[') {
    be_raisef(vm, "value_error", "Missing arguments, remaining type '%s'", &arg_type[arg_idx]);
  }
  return p_idx;
}

//
// Internal function
//
// Called for constructors, i.e. C function mapped to Berry `init()`
//
// Pre-conditions:
//   The instance must be at stack position `1` (default when calling `init()`)
//
// Arguments:
//   vm: point to Berry vm (as usual)
//   ptr: the C pointer for internal data (can be NULL), will be stored in an instance variable
//   name: name of instance variable to store the pointer as `comptr`.
//         If NULL, this function does nothing
//         the name can be prefixed with `+` or `=`, if so first char is ignored.
//         Ex: `+_p` stores in instance variable `_p`
//         `+` forbids any NULL value (raises an exception) while `=` allows a NULL value
static void be_set_ctor_ptr(bvm *vm, void * ptr, const char *name) {
  if (name == NULL) return;    // do nothing if no name of attribute
  if (name[0] == '+' && ptr == NULL)  { be_raise(vm, "value_error", "native constructor cannot return NULL"); }
  if (name[0] == '+' || name[0] == '=') { name++; }   // skip prefix '^' if any
  if (strlen(name) == 0) return;  // do nothing if name is empty

  be_pushcomptr(vm, ptr);
  if (be_setmember(vm, 1, name)) {
    be_pop(vm, 1);
  } else {
    be_raisef(vm, "attribute_error", "Missing member '%s' in ctor", name);
  }
}

/*********************************************************************************************\
 * CType handler for Berry
\*********************************************************************************************/
int be_call_ctype_func(bvm *vm, const void *definition) {
  be_ctype_args_t* args = (be_ctype_args_t*) definition;
  return be_call_c_func(vm, args->func, args->return_type, args->arg_type);
}

/*********************************************************************************************\
 * Call a C function with auto-mapping
 * 
 * Arguments:
 *   vm: pointer to Berry vm (as ususal)
 *   func: pointer to C function
 *   return_type: how to convert the result into a Berry type
 *   arg_type: string describing the optional and mandatory parameters
 * 
 * Note: the C function mapping supports max 8 arguments and does not directly support
 *       pointers to values (although it is possible to mimick with classes)
\*********************************************************************************************/
int be_call_c_func(bvm *vm, const void * func, const char * return_type, const char * arg_type) {
  intptr_t p[8] = {0,0,0,0,0,0,0,0};
  int argc = be_top(vm); // Get the number of arguments

  // the following describe the active payload for the C function (start and count)
  // this is because the `init()` constructor first arg is not passed to the C function
  int arg_start = 1;      // start with standard values
  int arg_count = argc;

  // check if we call a constructor, in this case we store the return type into the new object
  // check if we call a constructor with a comptr as first arg
  if (return_type && (return_type[0] == '+' || return_type[0] == '=')) {
    if (argc > 1 && be_iscomptr(vm, 2)) {
      void * obj = be_tocomptr(vm, 2);
      be_set_ctor_ptr(vm, obj, return_type);
      be_return_nil(vm);
    } else {
      // we need to discard the first arg
      arg_start++;
      arg_count--;
    }
  }

  fn_any_callable f = (fn_any_callable) func;
  size_t return_len = 0;   // when returning a bytes buffer, this holds the length of the buffer, while the return value of the function is `void*`
  int c_args = be_check_arg_type(vm, arg_start, arg_count, arg_type, p);
  if (return_type != NULL && return_type[0] == '&') {
    if (c_args < 8) { p[c_args] = (intptr_t) &return_len; }
  }
  intptr_t ret = 0;
  if (f) ret = (*f)(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
  // berry_log_C("be_call_c_func '%s' -> '%s': (%i,%i,%i,%i,%i,%i) -> %i", return_type, arg_type, p[0], p[1], p[2], p[3], p[4], p[5], ret);

  if ((return_type == NULL) || (strlen(return_type) == 0))       { be_return_nil(vm); }  // does not return
  else if (return_type[0] == '+' || return_type[0] == '=') {
    void * obj = (void*) ret;
    be_set_ctor_ptr(vm, obj, return_type);
    be_return_nil(vm);
  }
  else if (strlen(return_type) == 1) {
    switch (return_type[0]) {
      case '.':   // fallback next
      case 'i':   be_pushint(vm, ret); break;
      case 'f':   be_pushreal(vm, intasreal(ret)); break;
      case 'b':   be_pushbool(vm, ret);  break;
      case 'c':   be_pushcomptr(vm, (void*) ret); break;
      case 's':   if (ret) {be_pushstring(vm, (const char*) ret);} else {be_pushnil(vm);} break;  // push `nil` if no string
      case '$':   if (ret) {be_pushstring(vm, (const char*) ret);  free((void*)ret);} else {be_pushnil(vm);} break;
      case '&':   if (ret) {be_pushbytes(vm, (void*) ret, return_len);} else {be_pushnil(vm);} break;
      default:    be_raise(vm, "internal_error", "Unsupported return type"); break;
    }
    be_return(vm);
  } else { // class name
    be_find_global_or_module_member(vm, return_type);
    be_pushcomptr(vm, (void*) ret);         // stack = class, ptr
    be_call(vm, 1);                 // instanciate with 1 argument (ptr)
    be_pop(vm, 1);                  // stack = instance
    be_return(vm);
  }
}
