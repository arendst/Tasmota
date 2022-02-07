/*********************************************************************************************\
 * Add const virtual members to classes and modules from C static tables
 * 
 * This allows to creates hundreds of constant members (integers, strings...)
 * stored in a C array instead of explicit berry members.
 * 
 * It has the following advantages:
 * - consumes much less flash memory, especially with hundreds of members
 * - allows C preprocessor to compute the value at compile time (instead of pure numerical numbers)
 * - allows to compute pointer addresses to C structures
 * 
 * Takes a pointer to be_const_member_t array and size
 * Returns true if a match was found. In such case the result is on Berry stack
 * 
 * Encoding depend on prefix (which is skipped when matching names):
 * - `COLOR_WHITE` int value
 * - `$SYMBOL_OK"` string pointer
 * - `&seg7_font` comptr
 * - `*my_func` native function - the function is called and return value passed back.
 *              This allows to create dynamic virtual members that are the result of a call.
\*********************************************************************************************/

#include "be_mapping.h"
#include "be_exec.h"
#include <string.h>
/*********************************************************************************************\
 * Takes a pointer to be_const_member_t array and size
 * Returns true if a match was found. In such case the result is on Berry stack
 * 
 * Can be called directly by `member()` function, takes the name of the member from
 * berry stack at position 1.
 * 
 * Encoding depend on prefix (which is skipped when matching names):
 * - `COLOR_WHITE` int value
 * - `$SYMBOL_OK"` string pointer
 * - `&seg7_font` comptr
 * - `*my_func` native function - the function is called and return value passed back.
 *              This allows to create dynamic virtual members that are the result of a call.
 * 
 * The array must be lexically sorted, but the sort function must ignore the prefix `$`, `&`, `*`
\*********************************************************************************************/
static bbool be_const_member_dual(bvm *vm, const be_const_member_t * definitions, size_t def_len, bbool is_method) {
  int32_t arg_idx = is_method ? 2 : 1;    // index for name argument, 1 if module, 2 if method since 1 is self.
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc == arg_idx && be_isstring(vm, arg_idx)) {
    const char * needle = be_tostring(vm, arg_idx);
    int32_t idx;

    idx = be_map_bin_search(needle, &definitions[0].name, sizeof(definitions[0]), def_len);
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
        case '*': // call to a native function
        {
          bntvfunc f = (bntvfunc) definitions[idx].value;
          int ret = f(vm);
          if ((ret == BE_OK) && !be_isnil(vm, -1)) {
            return btrue;
          } else {
            return bfalse;
          }
          break;
        }
        case '>': // call to a ctype function
        {
          be_ctype_var_args_t* args = (be_ctype_var_args_t*) definitions[idx].value;
          int ret = be_call_c_func(vm, args->func, args->return_type, NULL);
          if ((ret == BE_OK) && !be_isnil(vm, -1)) {
            return btrue;
          } else {
            return bfalse;
          }
          break;
        }
        default:  // int
          be_pushint(vm, definitions[idx].value);
          break;
      }
      return btrue;
    }
  }
  return bfalse;
}

bbool be_const_module_member(bvm *vm, const be_const_member_t * definitions, size_t def_len) {
  return be_const_member_dual(vm, definitions, def_len, bfalse);   // call for module, non-method
}

bbool be_const_class_member(bvm *vm, const be_const_member_t * definitions, size_t def_len) {
  return be_const_member_dual(vm, definitions, def_len, btrue);   // call for method
}
