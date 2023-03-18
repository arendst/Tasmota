/********************************************************************
 * int64 - support 64 bits int on 32 bits architecture
 * 
 *******************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_mem.h"

static void int64_toa(int64_t num, uint8_t* str) {
	uint64_t sum = num;
  if (num < 0) {
    sum = -num;
    str[0] = '-';
    str++;
  }
	int len = 0;
	do {
    str[len++] = '0' + sum % 10LL;
		sum /= 10LL;
	} while (sum);
	str[len] = '\0';
  /* strrev */
  int i, j;
	for (i = 0, j = len - 1; i < j; i++, j--){
		uint8_t a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

void* int64_init(bvm *vm, int32_t val) {
  int64_t *i64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  *i64 = (int64_t) val;
  // serial_debug("int64_init p=%p\n", i64);
  return i64;
}
BE_FUNC_CTYPE_DECLARE(int64_init, "+_p", "@[i]")

void int64_deinit(bvm *vm, int64_t *i64) {
  // serial_debug("int64_deinit p=%p\n", i64);
  be_free(vm, i64, sizeof(int64_t));
}
BE_FUNC_CTYPE_DECLARE(int64_deinit, "", "@.")

char* int64_tostring(int64_t *i64) {
  static char s[24];    /* enough to hold max value */
  int64_toa(*i64, s);
  return s;
}
BE_FUNC_CTYPE_DECLARE(int64_tostring, "s", ".")

int64_t* int64_fromstring(bvm *vm, const char* s) {
  int64_t *i64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  if (i64 == NULL) { be_raise(vm, "memory_error", "cannot allocate buffer"); }
  if (s) { *i64 = atoll(s); }
  else   { *i64 = 0; }
  return i64;
}
BE_FUNC_CTYPE_DECLARE(int64_fromstring, "int64", "@s")

int32_t int64_toint(int64_t *i64) {
  return (int32_t) *i64;
}
BE_FUNC_CTYPE_DECLARE(int64_toint, "i", ".")

void int64_set(int64_t *i64, int32_t high, int32_t low) {
  *i64 = ((int64_t)high << 32) | ((int64_t)low & 0xFFFFFFFF);
}
BE_FUNC_CTYPE_DECLARE(int64_set, "", ".ii")

int64_t* int64_fromu32(bvm *vm, uint32_t low) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  *r64 = low;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_fromu32, "int64", "@i")

int64_t* int64_add(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = j64 ? *i64 + *j64 : *i64;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_add, "int64", "@(int64)(int64)")

int64_t* int64_sub(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = j64 ? *i64 - *j64 : *i64;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_sub, "int64", "@(int64)(int64)")

int64_t* int64_neg(bvm *vm, int64_t *i64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = - *i64;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_neg, "int64", "@.")

int64_t* int64_mul(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = j64 ? *i64 * *j64 : 0;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_mul, "int64", "@(int64)(int64)")

int64_t* int64_mod(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = j64 ? *i64 % *j64 : 0;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_mod, "int64", "@(int64)(int64)")

int64_t* int64_div(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  if (j64 == NULL || *j64 == 0) {
    be_raise(vm, "divzero_error", "division by zero");
  } else {
    *r64 = *i64 / *j64;
  }
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_div, "int64", "@.(int64)")

bbool int64_equals(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 == j;
}
BE_FUNC_CTYPE_DECLARE(int64_equals, "b", ".(int64)")

bbool int64_nequals(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 != j;
}
BE_FUNC_CTYPE_DECLARE(int64_nequals, "b", ".(int64)")

bbool int64_gt(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 > j;
}
BE_FUNC_CTYPE_DECLARE(int64_gt, "b", ".(int64)")

bbool int64_gte(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 >= j;
}
BE_FUNC_CTYPE_DECLARE(int64_gte, "b", ".(int64)")

bbool int64_lt(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 < j;
}
BE_FUNC_CTYPE_DECLARE(int64_lt, "b", ".(int64)")

bbool int64_lte(int64_t *i64, int64_t *j64) {
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  int64_t j = 0;
  if (j64) { j = *j64; }
  return *i64 <= j;
}
BE_FUNC_CTYPE_DECLARE(int64_lte, "b", ".(int64)")

void* int64_tobytes(int64_t *i64, size_t *len) {
  if (len) { *len = sizeof(int64_t); }
  return i64;
}
BE_FUNC_CTYPE_DECLARE(int64_tobytes, "&", ".")

void int64_frombytes(int64_t *i64, uint8_t* ptr, size_t len, int32_t idx) {
  if (idx < 0) { idx = len + idx; }   // support negative index, counting from the end
  if (idx < 0) { idx = 0; }           // sanity check
  if (idx > len) { idx = len; }
  uint32_t usable_len = len - idx;
  if (usable_len > sizeof(int64_t)) { usable_len = sizeof(int64_t); }
  *i64 = 0;   // start with 0
  memmove(i64, ptr + idx, usable_len);
}
BE_FUNC_CTYPE_DECLARE(int64_frombytes, "", ".(bytes)~[i]")

#include "be_fixed_be_class_int64.h"

/* @const_object_info_begin
class be_class_int64 (scope: global, name: int64) {
  _p, var
  init, ctype_func(int64_init)
  deinit, ctype_func(int64_deinit)
  set, ctype_func(int64_set)
  fromu32, static_ctype_func(int64_fromu32)

  tostring, ctype_func(int64_tostring)
  fromstring, static_ctype_func(int64_fromstring)
  toint, ctype_func(int64_toint)

  +, ctype_func(int64_add)
  -, ctype_func(int64_sub)
  *, ctype_func(int64_mul)
  /, ctype_func(int64_div)
  %, ctype_func(int64_mod)
  -*, ctype_func(int64_neg)
  ==, ctype_func(int64_equals)
  !=, ctype_func(int64_nequals)
  >, ctype_func(int64_gt)
  >=, ctype_func(int64_gte)
  <, ctype_func(int64_lt)
  <=, ctype_func(int64_lte)

  tobytes, ctype_func(int64_tobytes)
  frombytes, ctype_func(int64_frombytes)
}
@const_object_info_end */
