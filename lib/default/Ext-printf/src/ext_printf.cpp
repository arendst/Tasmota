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

/*********************************************************************************************\
 * va_list extended support
 * 
 * va_list allows to get the next argument but not to get the address of this argument in the stack.
 * 
 * We add `va_cur_ptr(va, TYPE)` to get a pointer to the current argument.
 * This will allow to modify it in place and call back printf with altered arguments
\*********************************************************************************************/

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

/*********************************************************************************************\
 * Genral function to convert u64 to hex
\*********************************************************************************************/
// Simple function to print a 64 bits unsigned int
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

/*********************************************************************************************\
 * snprintf extended
 * 
\*********************************************************************************************/

// get a fresh malloc allocated string based on the current pointer (can be in PROGMEM)
// It is the caller's responsibility to free the memory
char * copyStr(const char * str) {
  if (str == nullptr) { return nullptr; }
  char * cpy = (char*) malloc(strlen_P(str) + 1);
  strcpy_P(cpy, str);
  return cpy;
}

int32_t ext_vsnprintf_P(char * buf, size_t buf_len, const char * fmt_P, va_list va) {
  va_list va_cpy;
  va_copy(va_cpy, va);

#if defined(ESP8266) || defined(ESP32)    // this works only for xtensa, other platforms needs va_list to be adapted
  // iterate on fmt to extract arguments and patch them in place
  char * fmt_cpy = copyStr(fmt_P);
  if (fmt_cpy == nullptr) { return 0; }
  char * fmt = fmt_cpy;

  const uint32_t ALLOC_SIZE = 12;
  static char * allocs[ALLOC_SIZE] = {};     // initialized to zeroes
  uint32_t alloc_idx = 0;
  int32_t decimals = -2;      // default to 2 decimals and remove trailing zeros
  static char hex[20];        // buffer used for 64 bits, favor RAM instead of stack to remove pressure

	for (; *fmt != 0; ++fmt) {
    if (alloc_idx >= ALLOC_SIZE) { break; }     // buffer is full, don't continue parsing
    if (*fmt == '%') {
      fmt++;
      char * fmt_start = fmt;
			if (*fmt == '\0') { break; }              // end of string
			if (*fmt == '%') { continue; }            // actual '%' char
			if (*fmt == '*') {
				va_arg(va, int32_t);   // skip width argument as int
        fmt++;
			}
      if (*fmt < 'A') {
        decimals = strtol(fmt, nullptr, 10);
      }
      while (*fmt < 'A') {    // brutal way to munch anything that is not a letter or '-' (or anything else)
      // while ((*fmt >= '0' && *fmt <= '9') || (*fmt == '.') || (*fmt == '*') || (*fmt == '-' || (*fmt == ' ' || (*fmt == '+') || (*fmt == '#')))) {
        fmt++;
			}
    
      if (*fmt == '_') {      // extension
        for (; fmt_start <= fmt; fmt_start++) {
          *fmt_start = '0';
        }
        // *fmt = '0';
        fmt++;
        uint32_t cur_val = va_arg(va, uint32_t);              // current value
        const char ** cur_val_ptr = va_cur_ptr4(va, const char*);    // pointer to value on stack
        char * new_val_str = (char*) "";
        switch (*fmt) {
          // case 'D':
          //   decimals = *(int32_t*)cur_val_ptr;
          //   break;
          
          // `%_I` ouputs an IPv4 32 bits address passed as u32 into a decimal dotted format
          case 'I':     // Input is `uint32_t` 32 bits IP address, output is decimal dotted address
            new_val_str = copyStr(IPAddress(cur_val).toString().c_str());
            allocs[alloc_idx++] = new_val_str;
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
              bool truncate = false;
              if (decimals < 0) {
                decimals = -decimals;
                truncate = true;
              }
              dtostrf(*(float*)cur_val, (decimals + 2), decimals, hex);

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
              allocs[alloc_idx++] = new_val_str;
            }
            break;
          // '%_X' outputs a 64 bits unsigned int to uppercase HEX with 16 digits
          case 'X':     // input is `uint64_t*`, printed as 16 hex digits (no prefix 0x)
            {
              U64toHex(*(uint64_t*)cur_val, hex);
              new_val_str = copyStr(hex);
              allocs[alloc_idx++] = new_val_str;
            }
            break;
          // Trying to do String allocation alternatives, but not as interesting as I thought in the beginning
          // case 's':
          //   {
          //     new_val_str = copyStr(((String*)cur_val)->c_str());
          //     allocs[alloc_idx++] = new_val_str;
          //   }
          //   break;
          // case 'S':
          //   {
          //     funcString_t * func_str = (funcString_t*) cur_val;
          //     new_val_str = copyStr((*func_str)().c_str());
          //     allocs[alloc_idx++] = new_val_str;
          //   }
          //   break;
        }
        *cur_val_ptr = new_val_str;
        *fmt = 's';     // replace `%_X` with `%0s` to display a string instead

      } else {
        va_arg(va, int32_t);      // munch one 32 bits argument and leave it unchanged
        // we take the hypothesis here that passing 64 bits arguments is always unsupported in ESP8266
      }
      fmt++;
    }
  }
#else // defined(ESP8266) || defined(ESP32)
  #error "ext_printf is not suppoerted on this platform" 
#endif // defined(ESP8266) || defined(ESP32)
  int32_t ret = vsnprintf_P(buf, buf_len, fmt_cpy, va_cpy);

  va_end(va_cpy);
  for (uint32_t i = 0; i < alloc_idx; i++) {
    free(allocs[i]);
    allocs[i] = nullptr;
  }
  free(fmt_cpy);
  return ret;
}

int32_t ext_snprintf_P(char * buf, size_t buf_len, const char * fmt, ...) {
  va_list va;
  va_start(va, fmt);  

  int32_t ret = ext_vsnprintf_P(buf, buf_len, fmt, va);
  va_end(va);
  return ret;
}
