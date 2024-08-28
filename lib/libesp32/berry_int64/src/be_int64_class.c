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

// is the int64 within int32 range?
bbool int64_isint(int64_t *i64) {
  return (*i64 >= INT32_MIN && *i64 <= INT32_MAX);
}
BE_FUNC_CTYPE_DECLARE(int64_isint, "b", ".")

int32_t int64_toint(int64_t *i64) {
  return (int32_t) *i64;
}
BE_FUNC_CTYPE_DECLARE(int64_toint, "i", ".")

int64_t* int64_fromu32(bvm *vm, uint32_t low, uint32_t high) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  *r64 = low | (((int64_t)high) << 32);
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_fromu32, "int64", "@i[i]")

int64_t* int64_add(bvm *vm, int64_t *i64, int64_t *j64) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = j64 ? *i64 + *j64 : *i64;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_add, "int64", "@(int64)(int64)")

int64_t* int64_add32(bvm *vm, int64_t *i64, int32_t j32) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  // it's possible that arg j64 is nullptr, since class type does allow NULLPTR to come through.
  *r64 = *i64 + j32;
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_add32, "int64", "@(int64)i")

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

bbool int64_tobool(int64_t *i64) {
  return *i64 != 0;
}
BE_FUNC_CTYPE_DECLARE(int64_tobool, "b", ".")

void* int64_tobytes(int64_t *i64, size_t *len) {
  if (len) { *len = sizeof(int64_t); }
  return i64;
}
BE_FUNC_CTYPE_DECLARE(int64_tobytes, "&", ".")

int64_t* int64_frombytes(bvm *vm, uint8_t* ptr, size_t len, int32_t idx) {
  int64_t* r64 = (int64_t*)be_malloc(vm, sizeof(int64_t));
  if (idx < 0) { idx = len + idx; }   // support negative index, counting from the end
  if (idx < 0) { idx = 0; }           // sanity check
  if (idx > len) { idx = len; }
  uint32_t usable_len = len - idx;
  if (usable_len > sizeof(int64_t)) { usable_len = sizeof(int64_t); }
  *r64 = 0;   // start with 0
  memmove(r64, ptr + idx, usable_len);
  return r64;
}
BE_FUNC_CTYPE_DECLARE(int64_frombytes, "int64", "@(bytes)~[i]")

int32_t int64_low32(int64_t *i64) {
  return (int32_t) *i64;
}
BE_FUNC_CTYPE_DECLARE(int64_low32, "i", "(int64)")

int32_t int64_high32(int64_t *i64) {
  return (int32_t) (*i64 >> 32);
}
BE_FUNC_CTYPE_DECLARE(int64_high32, "i", "(int64)")

/*

def toint64(i)
  if (type(i) == 'int') return int64.fromu32(i)   end
  if (type(i) == 'instance') && isinstance(i, int64) return i   end
  return nil
end

*/

/********************************************************************
** Solidified function: toint64
********************************************************************/
be_local_closure(toint64,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(int),
    /* K1   */  be_nested_str(int64),
    /* K2   */  be_nested_str(fromu32),
    /* K3   */  be_nested_str(instance),
    }),
    &be_const_str_to64,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60040004,  //  0000  GETGBL	R1	G4
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040300,  //  0003  EQ	R1	R1	K0
      0x78060004,  //  0004  JMPF	R1	#000A
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C040400,  //  0008  CALL	R1	2
      0x80040200,  //  0009  RET	1	R1
      0x60040004,  //  000A  GETGBL	R1	G4
      0x5C080000,  //  000B  MOVE	R2	R0
      0x7C040200,  //  000C  CALL	R1	1
      0x1C040303,  //  000D  EQ	R1	R1	K3
      0x78060005,  //  000E  JMPF	R1	#0015
      0x6004000F,  //  000F  GETGBL	R1	G15
      0x5C080000,  //  0010  MOVE	R2	R0
      0xB80E0200,  //  0011  GETNGBL	R3	K1
      0x7C040400,  //  0012  CALL	R1	2
      0x78060000,  //  0013  JMPF	R1	#0015
      0x80040000,  //  0014  RET	1	R0
      0x4C040000,  //  0015  LDNIL	R1
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


#include "be_fixed_be_class_int64.h"

/* @const_object_info_begin
class be_class_int64 (scope: global, name: int64) {
  _p, var
  init, ctype_func(int64_init)
  deinit, ctype_func(int64_deinit)
  fromu32, static_ctype_func(int64_fromu32)
  toint64, static_closure(toint64_closure)

  tostring, ctype_func(int64_tostring)
  fromstring, static_ctype_func(int64_fromstring)
  isint, ctype_func(int64_isint)
  toint, ctype_func(int64_toint)
  tobool, ctype_func(int64_tobool)

  add, ctype_func(int64_add32)
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
  frombytes, static_ctype_func(int64_frombytes)

  low32, ctype_func(int64_low32)        // return low 32 bits as int
  high32, ctype_func(int64_high32)      // return high 32 bits as int
}
@const_object_info_end */
