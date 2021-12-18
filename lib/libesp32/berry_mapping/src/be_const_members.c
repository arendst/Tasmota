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
 * 1. `COLOR_WHITE` int value
 * 3. `$SYMBOL_OK"` string pointer
 * 4. `&seg7_font` comptr
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
 * 1. `COLOR_WHITE` int value
 * 3. `$SYMBOL_OK"` string pointer
 * 4. `&seg7_font` comptr
 * 
 * The array must be lexically sorted, but the sort function must ignore the prefix `$` or `&`
\*********************************************************************************************/
bbool be_const_member(bvm *vm, const be_const_member_t * definitions, size_t def_len) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc == 1 && be_isstring(vm, 1)) {
    const char * needle = be_tostring(vm, 1);
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
        default:  // int
          be_pushint(vm, definitions[idx].value);
          break;
      }
      return btrue;
    }
  }
  return bfalse;
}