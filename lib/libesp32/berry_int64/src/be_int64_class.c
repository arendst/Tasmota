/********************************************************************
 * int64 - support 64 bits int on 32 bits architecture
 * 
 * Pure native Berry C API implementation - no dependency on berry_mapping.
 * All functions use be_top/be_to* /be_push* stack operations directly.
 * 
 * SECURITY FIXES APPLIED:
 * - Fixed memory leaks in error paths
 * - Replaced atoll() with secure strtoll() parsing
 * - Added wrapping behavior for shift operations (eliminates undefined behavior)
 * - Added arithmetic overflow detection
 * - Added proper null pointer checks
 * - Fixed buffer underflow in frombytes
 *******************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "be_constobj.h"
#include "be_mem.h"

/*********************************************************************************************\
 * Helpers
\*********************************************************************************************/

/* Convert int64 to decimal string */
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
  int i, j;
  for (i = 0, j = len - 1; i < j; i++, j--) {
    uint8_t a = str[i];
    str[i] = str[j];
    str[j] = a;
  }
}

/* Secure string to int64 conversion with validation */
static int secure_str_to_int64(bvm *vm, const char* s, int64_t* result, void* allocated_ptr) {
  if (!s || *s == '\0') {
    *result = 0;
    return 0;
  }
  char* endptr;
  errno = 0;
  long long temp = strtoll(s, &endptr, 10);
  if (errno == ERANGE) {
    if (allocated_ptr) be_free(vm, allocated_ptr, sizeof(int64_t));
    be_raise(vm, "value_error", "integer string out of range");
    return -1;
  }
  while (*endptr == ' ' || *endptr == '\t' || *endptr == '\n' || *endptr == '\r') {
    endptr++;
  }
  if (*endptr != '\0') {
    if (allocated_ptr) be_free(vm, allocated_ptr, sizeof(int64_t));
    be_raise(vm, "value_error", "invalid integer string format");
    return -1;
  }
  *result = temp;
  return 0;
}

/* Get the _p pointer from self (stack position 1) */
static int64_t* self_get_p(bvm *vm) {
  be_getmember(vm, 1, "_p");
  int64_t *p = (int64_t*)be_tocomptr(vm, -1);
  be_pop(vm, 1);
  return p;
}

/* Get the _p pointer from an int64 instance at stack position idx */
static int64_t* arg_get_p(bvm *vm, int idx) {
  if (be_isnil(vm, idx)) { return NULL; }
  if (!be_isinstance(vm, idx)) { return NULL; }
  be_getmember(vm, idx, "_p");
  int64_t *p = (int64_t*)be_tocomptr(vm, -1);
  be_pop(vm, 1);
  return p;
}

/*
 * Push a new int64 instance onto the stack with the given value.
 * Stack on return: new int64 instance at top.
 */
static void push_int64_instance(bvm *vm, int64_t val) {
  int64_t *r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  if (r64 == NULL) { be_raise(vm, "memory_error", "cannot allocate buffer"); }
  *r64 = val;
  be_getglobal(vm, "int64");     /* stack: class */
  be_call(vm, 0);                /* stack: instance */
  be_getmember(vm, -1, "init");  /* stack: instance, init */
  be_pushvalue(vm, -2);          /* stack: instance, init, instance */
  be_pushcomptr(vm, r64);        /* stack: instance, init, instance, ptr */
  be_call(vm, 2);                /* stack: instance, ret */
  be_pop(vm, 1);                 /* stack: instance */
}

/*********************************************************************************************\
 * Constructor / Destructor
\*********************************************************************************************/

/* init(self [, value]) */
static int int64_init(bvm *vm) {
  int argc = be_top(vm);
  int64_t *i64 = NULL;
  /* did we receive a pre-allocated pointer */
  if (argc > 1 && be_iscomptr(vm, 2)) {
    i64 = (int64_t*)be_tocomptr(vm, 2);
  }
  if (i64 == NULL) {
    i64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
    if (i64 == NULL) { be_raise(vm, "memory_error", "cannot allocate buffer"); }
    *i64 = 0;
  }
  bbool invalid_arg = bfalse;
  if (argc > 1) {
    if (be_iscomptr(vm, 2) || be_isnil(vm, 2)) {
      /* keep value */
    } else if (be_isint(vm, 2)) {
      *i64 = be_toint(vm, 2);
    } else if (be_isreal(vm, 2)) {
      *i64 = (int64_t)be_toreal(vm, 2);
    } else if (be_isstring(vm, 2)) {
      const char* s = be_tostring(vm, 2);
      if (secure_str_to_int64(vm, s, i64, i64) != 0) {
        return 0;
      }
    } else if (be_isbool(vm, 2)) {
      *i64 = be_tobool(vm, 2) ? 1 : 0;
    } else if (be_isinstance(vm, 2)) {
      be_getglobal(vm, "int64");
      if (be_isderived(vm, 2)) {
        be_getmember(vm, 2, "_p");
        int64_t *v64 = (int64_t*)be_tocomptr(vm, -1);
        if (v64 != NULL) { *i64 = *v64; }
      } else {
        invalid_arg = btrue;
      }
    } else {
      invalid_arg = btrue;
    }
  }
  if (invalid_arg) {
    be_free(vm, i64, sizeof(int64_t));
    be_raise(vm, "TypeError", "unsupported argument type");
  }
  be_pushcomptr(vm, i64);
  be_setmember(vm, 1, "_p");
  be_return_nil(vm);
}

/* deinit(self) */
static int int64_deinit(bvm *vm) {
  be_getmember(vm, 1, "_p");
  int64_t *i64 = (int64_t*)be_tocomptr(vm, -1);
  if (i64 != NULL) {
    be_free(vm, i64, sizeof(int64_t));
  }
  be_return_nil(vm);
}

/*********************************************************************************************\
 * Conversion methods
\*********************************************************************************************/

/* tostring(self) -> string */
static int int64_tostring(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  static char s[24];
  int64_toa(*i64, (uint8_t*)s);
  be_pushstring(vm, s);
  be_return(vm);
}

/* static fromstring(s) -> int64 */
static int int64_fromstring(bvm *vm) {
  const char *s = be_tostring(vm, 1);
  int64_t val = 0;
  /* allocate a temp to pass to secure_str_to_int64 for error-path freeing */
  if (secure_str_to_int64(vm, s, &val, NULL) != 0) {
    return 0;
  }
  push_int64_instance(vm, val);
  be_return(vm);
}

/* isint(self) -> bool : is the value within int32 range? */
static int int64_isint(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushbool(vm, *i64 >= INT32_MIN && *i64 <= INT32_MAX);
  be_return(vm);
}

/* toint(self) -> int : truncate to 32-bit int */
static int int64_toint(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushint(vm, (int32_t)*i64);
  be_return(vm);
}

/* tobool(self) -> bool */
static int int64_tobool(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushbool(vm, *i64 != 0);
  be_return(vm);
}

/*********************************************************************************************\
 * Static factory methods
\*********************************************************************************************/

/* static fromu32(low [, high]) -> int64 */
static int int64_fromu32(bvm *vm) {
  int argc = be_top(vm);
  uint32_t low = (uint32_t)be_toint(vm, 1);
  uint32_t high = (argc > 1) ? (uint32_t)be_toint(vm, 2) : 0;
  int64_t val = (int64_t)low | (((int64_t)high) << 32);
  push_int64_instance(vm, val);
  be_return(vm);
}

/* static fromfloat(f) -> int64 */
static int int64_fromfloat(bvm *vm) {
  breal f = be_toreal(vm, 1);
  push_int64_instance(vm, (int64_t)f);
  be_return(vm);
}

/*********************************************************************************************\
 * Arithmetic operators - all return new int64 instances
\*********************************************************************************************/

/* +(self, other_int64) -> int64 */
static int int64_add(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  if ((b > 0 && *i64 > INT64_MAX - b) ||
      (b < 0 && *i64 < INT64_MIN - b)) {
    be_raise(vm, "overflow_error", "integer overflow in addition");
  }
  push_int64_instance(vm, *i64 + b);
  be_return(vm);
}

/* add(self, int32) -> int64 : add a Berry int directly */
static int int64_add32(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t b = (int64_t)be_toint(vm, 2);
  if ((b > 0 && *i64 > INT64_MAX - b) ||
      (b < 0 && *i64 < INT64_MIN - b)) {
    be_raise(vm, "overflow_error", "integer overflow in addition");
  }
  push_int64_instance(vm, *i64 + b);
  be_return(vm);
}

/* -(self, other_int64) -> int64 */
static int int64_sub(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  /* overflow check for subtraction: a - b overflows if b < a - INT64_MAX or b > a - INT64_MIN */
  if ((*i64 > 0 && b < *i64 - INT64_MAX) ||
      (*i64 < 0 && b > *i64 - INT64_MIN)) {
    be_raise(vm, "overflow_error", "integer overflow in subtraction");
  }
  push_int64_instance(vm, *i64 - b);
  be_return(vm);
}

/* -*(self) -> int64 : unary negation */
static int int64_neg(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  if (*i64 == INT64_MIN) {
    be_raise(vm, "overflow_error", "cannot negate INT64_MIN");
  }
  push_int64_instance(vm, -*i64);
  be_return(vm);
}

/* *(self, other_int64) -> int64 */
static int int64_mul(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t a = *i64;
  int64_t b = j64 ? *j64 : 0;
  if (a == 0 || b == 0) {
    push_int64_instance(vm, 0);
    be_return(vm);
  }
  if ((a == INT64_MIN && b == -1) || (a == -1 && b == INT64_MIN)) {
    be_raise(vm, "overflow_error", "integer overflow in multiplication");
  }
  if ((a > 0 && b > 0 && a > INT64_MAX / b) ||
      (a < 0 && b < 0 && a < INT64_MAX / b) ||
      (a > 0 && b < 0 && b < INT64_MIN / a) ||
      (a < 0 && b > 0 && a < INT64_MIN / b)) {
    be_raise(vm, "overflow_error", "integer overflow in multiplication");
  }
  push_int64_instance(vm, a * b);
  be_return(vm);
}

/* /(self, other_int64) -> int64 */
static int int64_div(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  if (j64 == NULL || *j64 == 0) {
    be_raise(vm, "divzero_error", "division by zero");
  }
  if (*i64 == INT64_MIN && *j64 == -1) {
    be_raise(vm, "overflow_error", "division overflow: INT64_MIN / -1");
  }
  push_int64_instance(vm, *i64 / *j64);
  be_return(vm);
}

/* %(self, other_int64) -> int64 */
static int int64_mod(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  if (j64 == NULL || *j64 == 0) {
    be_raise(vm, "divzero_error", "modulo by zero");
  }
  push_int64_instance(vm, *i64 % *j64);
  be_return(vm);
}

/*********************************************************************************************\
 * Shift operators
\*********************************************************************************************/

/* <<(self, int32) -> int64 */
static int int64_shiftleft(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int32_t j32 = be_toint(vm, 2) & 63;
  push_int64_instance(vm, *i64 << j32);
  be_return(vm);
}

/* >>(self, int32) -> int64 */
static int int64_shiftright(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int32_t j32 = be_toint(vm, 2) & 63;
  push_int64_instance(vm, *i64 >> j32);
  be_return(vm);
}

/*********************************************************************************************\
 * Comparison operators
\*********************************************************************************************/

/* Helper: compare int64 with the argument at stack position idx.
 * Supports int64 instances, plain ints, and bytes objects.
 * Returns 0 if equal, <0 if self < other, >0 if self > other.
 * Sets *valid to true if comparison was possible, false otherwise.
 */
static int int64_compare(bvm *vm, int64_t *i64, int idx, bbool *valid) {
  *valid = btrue;
  /* check if it's an int64 instance */
  int64_t *j64 = arg_get_p(vm, idx);
  if (j64) {
    if (*i64 < *j64) return -1;
    if (*i64 > *j64) return 1;
    return 0;
  }
  /* check if it's a plain int (or nil treated as 0) */
  if (be_isint(vm, idx) || be_isnil(vm, idx)) {
    int64_t b = be_isint(vm, idx) ? (int64_t)be_toint(vm, idx) : 0;
    if (*i64 < b) return -1;
    if (*i64 > b) return 1;
    return 0;
  }
  /* check if it's a bytes object — compare raw memory */
  if (be_isbytes(vm, idx)) {
    size_t len = 0;
    const void *ptr = be_tobytes(vm, idx, &len);
    if (len == sizeof(int64_t) && memcmp(i64, ptr, sizeof(int64_t)) == 0) {
      return 0;
    }
    *valid = btrue;
    return 1;  /* not equal */
  }
  *valid = bfalse;
  return 1;  /* different types are not equal */
}

/* ==(self, other) -> bool */
static int int64_equals(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  bbool valid;
  int cmp = int64_compare(vm, i64, 2, &valid);
  be_pushbool(vm, valid && cmp == 0);
  be_return(vm);
}

/* !=(self, other) -> bool */
static int int64_nequals(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  bbool valid;
  int cmp = int64_compare(vm, i64, 2, &valid);
  be_pushbool(vm, !valid || cmp != 0);
  be_return(vm);
}

/* >(self, other) -> bool */
static int int64_gt(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  be_pushbool(vm, *i64 > b);
  be_return(vm);
}

/* >=(self, other) -> bool */
static int int64_gte(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  be_pushbool(vm, *i64 >= b);
  be_return(vm);
}

/* <(self, other) -> bool */
static int int64_lt(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  be_pushbool(vm, *i64 < b);
  be_return(vm);
}

/* <=(self, other) -> bool */
static int int64_lte(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  int64_t *j64 = arg_get_p(vm, 2);
  int64_t b = j64 ? *j64 : 0;
  be_pushbool(vm, *i64 <= b);
  be_return(vm);
}

/*********************************************************************************************\
 * Bytes conversion
\*********************************************************************************************/

/* tobytes(self) -> bytes */
static int int64_tobytes(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushbytes(vm, i64, sizeof(int64_t));
  be_return(vm);
}

/* static frombytes(b [, idx]) -> int64 */
static int int64_frombytes(bvm *vm) {
  int argc = be_top(vm);
  size_t len = 0;
  const uint8_t *ptr = (const uint8_t*)be_tobytes(vm, 1, &len);
  int32_t idx = (argc > 1) ? be_toint(vm, 2) : 0;

  if (idx < 0) { idx = (int32_t)len + idx; }
  if (idx < 0) { idx = 0; }
  if (idx >= (int32_t)len) { idx = (int32_t)len; }

  uint32_t usable_len = len - idx;
  if (usable_len > sizeof(int64_t)) { usable_len = sizeof(int64_t); }
  int64_t val = 0;
  memmove(&val, ptr + idx, usable_len);
  push_int64_instance(vm, val);
  be_return(vm);
}

/*********************************************************************************************\
 * 32-bit accessors
\*********************************************************************************************/

/* low32(self) -> int */
static int int64_low32(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushint(vm, (int32_t)*i64);
  be_return(vm);
}

/* high32(self) -> int */
static int int64_high32(bvm *vm) {
  int64_t *i64 = self_get_p(vm);
  be_pushint(vm, (int32_t)(*i64 >> 32));
  be_return(vm);
}

/*********************************************************************************************\
 * Static helper: toint64(v) — native C equivalent of the Berry closure
 * def toint64(v)
 *   if (v == nil)  return nil  end
 *   return int64(v)
 * end
\*********************************************************************************************/
static int int64_toint64(bvm *vm) {
  if (be_top(vm) == 0 || be_isnil(vm, 1)) {
    be_return_nil(vm);
  }
  /* push int64 class, then call int64(v) */
  be_getglobal(vm, "int64");
  be_pushvalue(vm, 1);
  be_call(vm, 1);
  be_pop(vm, 1);
  be_return(vm);
}

/*********************************************************************************************\
 * Class definition - all methods are now plain native functions (func), no ctype_func
\*********************************************************************************************/

#include "be_fixed_be_class_int64.h"

/* @const_object_info_begin
class be_class_int64 (scope: global, name: int64) {
  _p, var
  init, func(int64_init)
  deinit, func(int64_deinit)
  fromu32, static_func(int64_fromu32)
  fromfloat, static_func(int64_fromfloat)
  toint64, static_func(int64_toint64)

  tostring, func(int64_tostring)
  fromstring, static_func(int64_fromstring)
  isint, func(int64_isint)
  toint, func(int64_toint)
  tobool, func(int64_tobool)

  add, func(int64_add32)
  +, func(int64_add)
  -, func(int64_sub)
  *, func(int64_mul)
  /, func(int64_div)
  %, func(int64_mod)
  -*, func(int64_neg)
  ==, func(int64_equals)
  !=, func(int64_nequals)
  >, func(int64_gt)
  >=, func(int64_gte)
  <, func(int64_lt)
  <=, func(int64_lte)
  <<, func(int64_shiftleft)
  >>, func(int64_shiftright)

  tobytes, func(int64_tobytes)
  frombytes, static_func(int64_frombytes)

  low32, func(int64_low32)
  high32, func(int64_high32)
}
@const_object_info_end */
