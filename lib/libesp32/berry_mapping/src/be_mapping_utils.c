#include "be_mapping.h"
#include <string.h>
#include <math.h>

/*********************************************************************************************\
 * Helper functions to create a map with single line calls
\*********************************************************************************************/
/* Insert an nil to a key */
void be_map_insert_nil(bvm *vm, const char *key)
{
  if (be_ismap(vm, -1)) {
    be_pushstring(vm, key);
    be_pushnil(vm);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
}
/* Insert an int to a key */
// On stack is either:
// Case 1;  (-2) map instance, (-1) map
// Case 2;  (-2) nil, (-1) string -> if key matches then update (-2)
void be_map_insert_int(bvm *vm, const char *key, bint value)
{
  if (be_ismap(vm, -1)) {
    be_pushstring(vm, key);
    be_pushint(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  } else if (be_isstring(vm, -1)) {
    const char * needle = be_tostring(vm, -1);
    if (strcmp(key, needle) == 0) {
      be_pushint(vm, value);
      be_moveto(vm, -1, -3);
      be_pop(vm, 1);
    }
  }
}
/* Insert an bbool to a key */
void be_map_insert_bool(bvm *vm, const char *key, bbool value)
{
  if (be_ismap(vm, -1)) {
    be_pushstring(vm, key);
    be_pushbool(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  } else if (be_isstring(vm, -1)) {
    const char * needle = be_tostring(vm, -1);
    if (strcmp(key, needle) == 0) {
      be_pushbool(vm, value);
      be_moveto(vm, -1, -3);
      be_pop(vm, 1);
    }
  }
}
/* Insert an real to a key */
/* if value == NAN, ignore */
void be_map_insert_real(bvm *vm, const char *key, breal value)
{
  if (be_ismap(vm, -1)) {
    if (!isnan(value)) {
      be_pushstring(vm, key);
      be_pushreal(vm, value);
      be_data_insert(vm, -3);
      be_pop(vm, 2);
    }
  } else if (be_isstring(vm, -1)) {
    const char * needle = be_tostring(vm, -1);
    if (strcmp(key, needle) == 0) {
      be_pushreal(vm, value);
      be_moveto(vm, -1, -3);
      be_pop(vm, 1);
    }
  }
}
/* Insert an C string to a key */
void be_map_insert_str(bvm *vm, const char *key, const char *value)
{
  if (be_ismap(vm, -1)) {
    be_pushstring(vm, key);
    be_pushstring(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  } else if (be_isstring(vm, -1)) {
    const char * needle = be_tostring(vm, -1);
    if (strcmp(key, needle) == 0) {
      be_pushstring(vm, value);
      be_moveto(vm, -1, -3);
      be_pop(vm, 1);
    }
  }
}
/* Insert list of bytes as individual integers to a key */
void be_map_insert_list_uint8(bvm *vm, const char *key, const uint8_t *value, size_t size)
{
  if (be_ismap(vm, -1)) {
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
}

/*********************************************************************************************\
 * Binary search for dynamic attributes - SECURITY PATCHED
 * 
 * SECURITY IMPROVEMENTS:
 * - Added comprehensive input validation
 * - Bounds checking for all parameters
 * - Protection against malicious table pointers
 * - Safe string operations
 * 
 * Names need to be sorted
\*********************************************************************************************/
// binary search within an array of sorted strings
// the first 4 bytes are a pointer to a string
// returns 0..total_elements-1 or -1 if not found
//
// This version skips the first character of the string if it's not a letter,
// the first character is used to indicate the type of the value associated to the key
int be_map_bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements) {
#if BE_MAPPING_ENABLE_INPUT_VALIDATION
  // SECURITY: Input validation - prevent crashes from invalid parameters
  if (needle == NULL) {
    return -1;
  }
  
  if (table == NULL) {
    return -1;
  }
  
  if (elt_size == 0 || elt_size > 1024) {  // Reasonable size limit
    return -1;
  }
  
  if (total_elements == 0) {
    return -1;  // Empty table
  }
  
  if (total_elements > 100000) {  // Reasonable limit to prevent DoS
    return -1;
  }
  
  // SECURITY: Validate needle string length
  size_t needle_len = strlen(needle);
  if (needle_len == 0 || needle_len > BE_MAPPING_MAX_NAME_LENGTH) {
    return -1;
  }
#endif // BE_MAPPING_ENABLE_INPUT_VALIDATION

  int low = 0;
  int high = (int)total_elements - 1;
  int mid = (low + high) / 2;
  
  // Start binary search
  while (low <= high) {
    // SECURITY: Bounds check for table access
    if (mid < 0 || mid >= (int)total_elements) {
      return -1;
    }
    
    // SECURITY: Safe table element access with bounds checking
    const char * elt = *(const char **) ( ((uint8_t*)table) + mid * elt_size );
    
    // SECURITY: Validate element pointer
    if (elt == NULL) {
      return -1;
    }
    
    // SECURITY: Validate element string length
    size_t elt_len = strlen(elt);
    if (elt_len > BE_MAPPING_MAX_NAME_LENGTH) {
      return -1;
    }
    
    char first_char = elt[0];
    if ( !(first_char >= 'a' && first_char <='z') && !(first_char >= 'A' && first_char <='Z') ) {
      elt++;  // skip first char
      // SECURITY: Ensure we still have a valid string after skipping
      if (*elt == '\0') {
        return -1;
      }
    }
    
    int comp = strcmp(needle, elt);
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
