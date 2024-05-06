/*
  ext_printf.ino - Extended printf for Arduino  objects

  Copyright (C) 2021  Stephan Hadinger

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

#include "ext_printf.h"
#include <Arduino.h>
#include <IPAddress.h>
#include <SBuffer.hpp>

/*********************************************************************************************\
 * va_list extended support
 *
 * va_list allows to get the next argument but not to get the address of this argument in the stack.
 *
 * We add `va_cur_ptr(va, TYPE)` to get a pointer to the current argument.
 * This will allow to modify it in place and call back printf with altered arguments
\*********************************************************************************************/

#if defined(__XTENSA__)    // this works only for xtensa, other platforms needs va_list to be adapted

// This code is heavily inspired by the gcc implementation of va_list
// https://github.com/gcc-mirror/gcc/blob/master/gcc/config/xtensa/xtensa.c

// Here is the va_list structure:
// struct va_list {
//   void *    __va_stk;       // offset 0 - pointer to arguments on the stack
//   void *    __va_reg;       // offset 4 - pointer to arguments from registers
//   uint32_t  __va_ndx;       // offset 8 - index in bytes of the argument (overshoot by sizeof(T))
// }
//
// When `va_start()` is called, the first 6 arguments are passed through registers r2-r7 and
// are saved on the stack like local variables

// The algorightm used by `va_arg()` is the following:
// /* Implement `va_arg'.  */
// /* First align __va_ndx if necessary for this arg:
//      orig_ndx = (AP).__va_ndx;
//      if (__alignof__ (TYPE) > 4 )
//        orig_ndx = ((orig_ndx + __alignof__ (TYPE) - 1)
// 			& -__alignof__ (TYPE)); */
// /* Increment __va_ndx to point past the argument:
//      (AP).__va_ndx = orig_ndx + __va_size (TYPE); */
// /* Check if the argument is in registers:
//      if ((AP).__va_ndx <= __MAX_ARGS_IN_REGISTERS * 4
//          && !must_pass_in_stack (type))
//         __array = (AP).__va_reg; */
// /* ...otherwise, the argument is on the stack (never split between
//      registers and the stack -- change __va_ndx if necessary):
//      else
//        {
// 	 if (orig_ndx <= __MAX_ARGS_IN_REGISTERS * 4)
// 	     (AP).__va_ndx = 32 + __va_size (TYPE);
//    __array = (AP).__va_stk;
//        } */
// /* Given the base array pointer (__array) and index to the subsequent
//      argument (__va_ndx), find the address:
//      __array + (AP).__va_ndx - (BYTES_BIG_ENDIAN && sizeof (TYPE) < 4
// 				? sizeof (TYPE)
// 				: __va_size (TYPE))
//      The results are endian-dependent because values smaller than one word
//      are aligned differently.  */

// So we can simply get the argument address
#define MAX_ARGS_IN_REGISTERS   6     // ESP8266 passes 6 arguments by register, then on stack

// #define va_cur_ptr(va,T) ( (T*) __va_cur_ptr(va,sizeof(T)) )   // we only support 4 bytes aligned arguments, so we don't need this one

// void * __va_cur_ptr(va_list &va, size_t size) {
//   size = (size + 3) & 0xFFFFFFFC;    // round to upper 4 bytes boundary

//   uintptr_t * va_stk =     (uintptr_t*) &va;
//   uintptr_t * va_reg = 1 + (uintptr_t*) &va;
//   uintptr_t * va_ndx = 2 + (uintptr_t*) &va;
//   uintptr_t   arr;

//   if (*va_ndx <= MAX_ARGS_IN_REGISTERS * 4) {
//     arr = *va_reg;
//   } else {
//     arr = *va_stk;
//   }
//   return (void*) (arr + *va_ndx - size);
// }

// reduced version when arguments are always 4 bytes
#define va_cur_ptr4(va,T) ( (T*) __va_cur_ptr4(va) )
void * __va_cur_ptr4(va_list &va) {
  uintptr_t * va_stk =     (uintptr_t*) &va;
  uintptr_t * va_reg = 1 + (uintptr_t*) &va;
  uintptr_t * va_ndx = 2 + (uintptr_t*) &va;
  uintptr_t   arr;

  if (*va_ndx <= MAX_ARGS_IN_REGISTERS * 4) {
    arr = *va_reg;
  } else {
    arr = *va_stk;
  }
  return (void*) (arr + *va_ndx - 4);
}

// Example of logs with 8 arguments (+1 static argument)
// We see that the first 5 are from low in the stack (local variables)
// while the last 8 are upper in the stack pushed by caller
//
// Note 64 bits arguments cannot be split between registers and stack
//
// >>> Reading a_ptr=0x3FFFFD44 *a_ptr=1
// >>> Reading a_ptr=0x3FFFFD48 *a_ptr=2
// >>> Reading a_ptr=0x3FFFFD4C *a_ptr=3
// >>> Reading a_ptr=0x3FFFFD50 *a_ptr=4
// >>> Reading a_ptr=0x3FFFFD54 *a_ptr=5
// >>> Reading a_ptr=0x3FFFFD70 *a_ptr=6
// >>> Reading a_ptr=0x3FFFFD74 *a_ptr=7
// >>> Reading a_ptr=0x3FFFFD78 *a_ptr=8

#elif defined(__riscv)
// #define __va_argsiz_tas(t)  	(((sizeof(t) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))
#define va_cur_ptr4(va,T) ( (T*) __va_cur_ptr4(va) )
void * __va_cur_ptr4(va_list &va) {
  uintptr_t * va_ptr = (uintptr_t*) &va;
  int32_t * cur_ptr = (int32_t*) *va_ptr;
  return (void*) (cur_ptr - 1);
}

#else   // __XTENSA__, __riscv
  #error "ext_printf is not suppoerted on this platform"
#endif  // __XTENSA__, __riscv

/*********************************************************************************************\
 * Genral function to convert u64 to hex
\*********************************************************************************************/
// Simple function to print a 64 bits unsigned int
/*
char * U64toHex(uint64_t value, char *str) {
  // str must be at least 17 bytes long
  str[16] = 0;    // end of string
  for (uint32_t i=0; i<16; i++) {       // 16 digits
    uint32_t n = value & 0x0F;
    str[15 - i] = (n < 10) ? (char)n+'0' : (char)n-10+'A';
    value = value >> 4;
  }
  return str;
}
*/

char * ToBinary(uint32_t value, char *str, int32_t digits) {
  if (digits > 32) { digits = 32; }
  if (digits < 1) { digits = 1; }
  int32_t digits_to_one = 1;               // how many digits until we find the last `1`
  str[32] = 0;    // end of string
  for (uint32_t i=0; i<32; i++) {       // 32 digits in uint32_t
    if ((value & 1) && (i+1 > digits_to_one)) {
      digits_to_one = i+1;
    }
    str[31 - i] = (char)(value & 1)+'0';
    value = value >> 1;
  }
  // adjust digits to always show the total value
  if (digits_to_one > digits) { digits = digits_to_one; }
  if (digits < 32) {
    memmove(str, str + 32 - digits, digits + 1);
  }
  return str;
}

char * U64toHex(uint64_t value, char *str, uint32_t zeroleads) {
  // str must be at least 17 bytes long
  str[16] = 0;    // end of string
  for (uint32_t i=0; i<16; i++) {       // 16 digits
    uint32_t n = value & 0x0F;
    str[15 - i] = (n < 10) ? (char)n+'0' : (char)n-10+'A';
    value = value >> 4;
  }
  if (zeroleads < 16) {
    uint32_t max_zeroes = 16 - zeroleads;
    while (max_zeroes) {
      if (str[0] == '0') {
        memmove(str, str +1, strlen(str));
      } else {
        break;
      }
      max_zeroes--;
    }
  }
  return str;
}

// see https://stackoverflow.com/questions/6357031/how-do-you-convert-a-byte-array-to-a-hexadecimal-string-in-c
// char* ToHex_P(unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0'); in tasmota_globals.h
char* ToHex_P(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween = '\0') {
  // ToHex_P(in, insz, out, outz)      -> "12345667"
  // ToHex_P(in, insz, out, outz, ' ') -> "12 34 56 67"
  // ToHex_P(in, insz, out, outz, ':') -> "12:34:56:67"
  static const char * hex PROGMEM = "0123456789ABCDEF";
  int between = (inbetween) ? 3 : 2;
  const unsigned char * pin = in;
  char * pout = out;
  for (; pin < in+insz; pout += between, pin++) {
    pout[0] = pgm_read_byte(&hex[(pgm_read_byte(pin)>>4) & 0xF]);
    pout[1] = pgm_read_byte(&hex[ pgm_read_byte(pin)     & 0xF]);
    if (inbetween) { pout[2] = inbetween; }
    if (pout + 3 - out > outsz) { break; }  // Better to truncate output string than overflow buffer
  }
  pout[(inbetween && insz) ? -1 : 0] = 0;   // Discard last inbetween if any input
  return out;
}

/*********************************************************************************************\
 * snprintf extended
 *
 * New: if the provided buffer is nullptr, a buffer is allocated on the heap (malloc)
 * and returned as a pointer instead of the length of the output (needs casting)
\*********************************************************************************************/

// get a fresh malloc allocated string based on the current pointer (can be in PROGMEM)
// It is the caller's responsibility to free the memory
//
// Returns nullptr if something went wrong
char * copyStr(const char * str) {
  if (str == nullptr) { return nullptr; }
  char * cpy = (char*) malloc(strlen_P(str) + 1);
  if (cpy == nullptr) { return nullptr; }     // something went wrong
  strcpy_P(cpy, str);
  return cpy;
}

const char ext_invalid_mem[] PROGMEM = "<--INVALID-->";
const uint32_t min_valid_ptr = 0x3F000000;    // addresses below this line are invalid

int32_t ext_vsnprintf_P(char * out_buf, size_t buf_len, const char * fmt_P, va_list va) {
  va_list va_cpy;
  va_copy(va_cpy, va);

  // iterate on fmt to extract arguments and patch them in place
  char * fmt_cpy = copyStr(fmt_P);
  if (fmt_cpy == nullptr) { return 0; }   // we couldn't copy the format, abort
  char * fmt = fmt_cpy;
  int32_t ret = 0;    // return 0 if unsuccessful
  bool aborted = true;   // did something went wrong?

  const uint32_t ALLOC_SIZE = 12;
  static const char * allocs[ALLOC_SIZE] = {};     // initialized to zeroes
  uint32_t alloc_idx = 0;
  static char hex[34];          // buffer used for 64 bits, favor RAM instead of stack to remove pressure

	for (; *fmt != 0; ++fmt) {
    int32_t decimals = -2;      // default to 2 decimals and remove trailing zeros
    int32_t * decimals_ptr = nullptr;
    if (alloc_idx >= ALLOC_SIZE) { break; }     // buffer is full, don't continue parsing
    if (*fmt == '%') {
      fmt++;
      char * fmt_start = fmt;
			if (*fmt == '\0') { break; }              // end of string
			if (*fmt == '%') { continue; }            // actual '%' char
			if (*fmt == '*') {
				decimals = va_arg(va, int32_t);   // skip width argument as int
        decimals_ptr = va_cur_ptr4(va, int32_t);    // pointer to value on stack
        fmt++;
        // Serial.printf("> decimals=%d, decimals_ptr=0x%08X\n", decimals, decimals_ptr);
			}
      if (*fmt < 'A') {
        decimals = strtol(fmt, nullptr, 10);
      }
      while (*fmt < 'A') {    // brutal way to munch anything that is not a letter or '-' (or anything else)
      // while ((*fmt >= '0' && *fmt <= '9') || (*fmt == '.') || (*fmt == '*') || (*fmt == '-' || (*fmt == ' ' || (*fmt == '+') || (*fmt == '#')))) {
        fmt++;
			}

      if (*fmt == '_') {      // extension
        if (decimals_ptr) {
          // Serial.printf(">2 decimals=%d, decimals_ptr=0x%08X\n", decimals, decimals_ptr);
          *decimals_ptr = 0;      // if '*' was used, make sure we replace the value with zero for snprintf()
          *(fmt_start++) = '-';   // in this case replace with `%-*s`
          *(fmt_start++) = '*';
        }
        for (; fmt_start <= fmt; fmt_start++) {
          *fmt_start = '0';
        }
        // *fmt = '0';
        fmt++;
        uint32_t cur_val = va_arg(va, uint32_t);              // current value
        const char ** cur_val_ptr = va_cur_ptr4(va, const char*);    // pointer to value on stack
        const char * new_val_str = "";
        switch (*fmt) {
          case 'H':     // Hex, decimals indicates the length, default 2
            {
              if (decimals < 0) { decimals = 0; }
              if (cur_val < min_valid_ptr) { new_val_str = ext_invalid_mem; }
              else if (decimals > 0) {
                char * hex_char = (char*) malloc(decimals*2 + 2);
                if (hex_char == nullptr) { goto free_allocs; }
                ToHex_P((const uint8_t *)cur_val, decimals, hex_char, decimals*2 + 2);
                new_val_str = hex_char;
                allocs[alloc_idx++] = new_val_str;
                // Serial.printf("> hex=%s\n", hex_char);
              }
            }
            break;
          case 'B':     // Pointer to SBuffer
            {
              if (cur_val < min_valid_ptr) { new_val_str = ext_invalid_mem; }
              else {
                const SBuffer & buf = *(const SBuffer*)cur_val;
                size_t buf_len = (&buf != nullptr) ? buf.len() : 0;
                if (buf_len) {
                  char * hex_char = (char*) malloc(buf_len*2 + 2);
                  if (hex_char == nullptr) { goto free_allocs; }
                  ToHex_P(buf.getBuffer(), buf_len, hex_char, buf_len*2 + 2);
                  new_val_str = hex_char;
                  allocs[alloc_idx++] = new_val_str;
                }
              }
            }
            break;
          // '%_b' outputs a uint32_t to binary
          // '%8_b' outputs a uint8_t to binary
          case 'b':     // Binary, decimals indicates the zero prefill
            {
                ToBinary(cur_val, hex, decimals);
                new_val_str = copyStr(hex);
                if (new_val_str == nullptr) { goto free_allocs; }
                allocs[alloc_idx++] = new_val_str;
              }
            break;
/*
          case 'V':     // 2-byte values, decimals indicates the length, default 2
            {
              if (decimals < 0) { decimals = 0; }
              if (cur_val < min_valid_ptr) { new_val_str = ext_invalid_mem; }
              else if (decimals > 0) {
                uint32_t val_size = decimals*6 + 2;
                char * val_char = (char*) malloc(val_size);
                if (val_char == nullptr) { goto free_allocs; }
                val_char[0] = '\0';
                for (uint32_t count = 0; count < decimals; count++) {
                  uint32_t value = pgm_read_byte((const uint8_t *)cur_val +1) << 8 | pgm_read_byte((const uint8_t *)cur_val);
                  snprintf_P(val_char, val_size, PSTR("%s%s%d"), val_char, (count)?",":"", value);
                  cur_val += 2;
                }
                new_val_str = val_char;
                allocs[alloc_idx++] = new_val_str;
                // Serial.printf("> values=%s\n", hex_char);
              }
            }
            break;
*/
          // case 'D':
          //   decimals = *(int32_t*)cur_val_ptr;
          //   break;

          // `%_I` ouputs an IPv4 32 bits address passed as u32 into a decimal dotted format
          case 'I':     // Input is `uint32_t` 32 bits IP address, output is decimal dotted address
            {
              char * ip_str = (char*) malloc(16);
              if (ip_str == nullptr) { goto free_allocs; }
              snprintf_P(ip_str, 16, PSTR("%u.%u.%u.%u"), cur_val & 0xFF, (cur_val >> 8) & 0xFF, (cur_val >> 16) & 0xFF, (cur_val >> 24) & 0xFF);
              new_val_str = ip_str;
              allocs[alloc_idx++] = new_val_str;
            }
            break;

          // `%_f` or `%*_f` outputs a float with optionan number of decimals passed as first argument if `*` is present
          // positive number of decimals means an exact number of decimals, can be `0` terminate
          // negative number of decimals will suppress
          // Ex:
          //    char c[128];
          //    float f = 3.141f;
          //    ext_vsnprintf_P(c; szeof(c), "%_f %*_f %*_f", &f, 4, 1f, -4, %f);
          //    --> c will be "3.14 3.1410 3.141"
          // Note: float MUST be passed by address, because C alsays promoted float to double when in vararg
          case 'f':     // input is `float`, printed to float with 2 decimals
            {
              if (cur_val < min_valid_ptr) { new_val_str = ext_invalid_mem; }
              else {
                bool truncate = false;
                if (decimals < 0) {
                  decimals = -decimals;
                  truncate = true;
                }
                float number = *(float*)cur_val;
                if (isnan(number) || isinf(number)) {
                  new_val_str = "null";
                } else {
                  uint32_t len = (decimals) ? decimals +2 : 1;
                  dtostrf(*(float*)cur_val, len, decimals, hex);

                  if (truncate) {
                    uint32_t last = strlen(hex) - 1;
                    // remove trailing zeros
                    while (hex[last] == '0') {
                      hex[last--] = 0;              // remove last char
                    }
                    // remove trailing dot
                    if (hex[last] == '.') {
                      hex[last] = 0;
                    }
                  }
                  new_val_str = copyStr(hex);
                  if (new_val_str == nullptr) { goto free_allocs; }
                  allocs[alloc_idx++] = new_val_str;
                }
              }
            }
            break;
          // '%_X' outputs a 64 bits unsigned int to uppercase HEX with 16 digits
          case 'X':     // input is `uint64_t*`, printed as 16 hex digits (no prefix 0x)
            {
              if (cur_val < min_valid_ptr) { new_val_str = ext_invalid_mem; }
              else {
                if ((decimals < 0) || (decimals > 16)) { decimals = 16; }
                U64toHex(*(uint64_t*)cur_val, hex, decimals);
                new_val_str = copyStr(hex);
                if (new_val_str == nullptr) { goto free_allocs; }
                allocs[alloc_idx++] = new_val_str;
              }
            }
            break;
        }
        *cur_val_ptr = new_val_str;
        *fmt = 's';     // replace `%_X` with `%0s` to display a string instead

      } else {
        va_arg(va, int32_t);      // munch one 32 bits argument and leave it unchanged
        // we take the hypothesis here that passing 64 bits arguments is always unsupported in ESP8266
      }
    }
  }
  // Serial.printf("> format_final=%s\n", fmt_cpy); Serial.flush();
  if (out_buf != nullptr) {
    ret = vsnprintf_P(out_buf, buf_len, fmt_cpy, va_cpy);
    aborted = false;    // we completed without malloc error
  } else {
    // if there is no output buffer, we allocate one on the heap
    // first we do a dry-run to know the target size
    char dummy[2];
    int32_t target_len = vsnprintf_P(dummy, 1, fmt_cpy, va_cpy);
    if (target_len >= 0) {
      // successful
      char * allocated_buf = (char*) malloc(target_len + 1);
      if (allocated_buf != nullptr) {
        allocated_buf[0] = 0;   // default to empty string
        vsnprintf_P(allocated_buf, target_len + 1, fmt_cpy, va_cpy);
        ret = (int32_t) allocated_buf;
        aborted = false;    // we completed without malloc error
      }
    }
  }

  va_end(va_cpy);

free_allocs:
  if (aborted && out_buf != nullptr) {    // if something went wrong, set output string to empty string to avoid corrupt data
    *out_buf = '\0';
  }

  // disallocated all temporary strings
  for (uint32_t i = 0; i < alloc_idx; i++) {
    free((void*)allocs[i]);      // it is ok to call free() on nullptr so we don't test for nullptr first
    allocs[i] = nullptr;
  }
  free(fmt_cpy);          // free the local copy of the format string
  return ret;
}

char * ext_vsnprintf_malloc_P(const char * fmt_P, va_list va) {
  int32_t ret = ext_vsnprintf_P(nullptr, 0, fmt_P, va);
  return (char*) ret;
}

int32_t ext_snprintf_P(char * out_buf, size_t buf_len, const char * fmt, ...) {
  va_list va;
  va_start(va, fmt);

  int32_t ret = ext_vsnprintf_P(out_buf, buf_len, fmt, va);
  va_end(va);
  return ret;
}

char * ext_snprintf_malloc_P(const char * fmt, ...) {
  va_list va;
  va_start(va, fmt);

  int32_t ret = ext_vsnprintf_P(nullptr, 0, fmt, va);
  va_end(va);
  return (char*) ret;
}
