#include "be_mapping.h"
#include <string.h>
#include <math.h>

/*********************************************************************************************\
 * Helper functions to create a map with single line calls
\*********************************************************************************************/
/* Insert an nil to a key */
void be_map_insert_nil(bvm *vm, const char *key)
{
  be_pushstring(vm, key);
  be_pushnil(vm);
  be_data_insert(vm, -3);
  be_pop(vm, 2);
}
/* Insert an int to a key */
void be_map_insert_int(bvm *vm, const char *key, bint value)
{
  be_pushstring(vm, key);
  be_pushint(vm, value);
  be_data_insert(vm, -3);
  be_pop(vm, 2);
}
/* Insert an bbool to a key */
void be_map_insert_bool(bvm *vm, const char *key, bbool value)
{
  be_pushstring(vm, key);
  be_pushbool(vm, value);
  be_data_insert(vm, -3);
  be_pop(vm, 2);
}
/* Insert an real to a key */
/* if value == NAN, ignore */
void be_map_insert_real(bvm *vm, const char *key, breal value)
{
  if (!isnan(value)) {
    be_pushstring(vm, key);
    be_pushreal(vm, value);
    be_data_insert(vm, -3);
    be_pop(vm, 2);
  }
}
/* Insert an C string to a key */
void be_map_insert_str(bvm *vm, const char *key, const char *value)
{
  be_pushstring(vm, key);
  be_pushstring(vm, value);
  be_data_insert(vm, -3);
  be_pop(vm, 2);
}
/* Insert list of bytes as individual integers to a key */
void be_map_insert_list_uint8(bvm *vm, const char *key, const uint8_t *value, size_t size)
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
int be_map_bin_search(const char * needle, const void * table, size_t elt_size, size_t total_elements) {
  int low = 0;
  int high = total_elements - 1;
  int mid = (low + high) / 2;
  // start a dissect
  while (low <= high) {
    const char * elt = *(const char **) ( ((uint8_t*)table) + mid * elt_size );
    char first_char = elt[0];
    if ( !(first_char >= 'a' && first_char <='z') && !(first_char >= 'A' && first_char <='Z') ) {
      elt++;  // skip first char
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