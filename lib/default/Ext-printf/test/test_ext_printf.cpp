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

// DEBUG only

// String test_string(void) {
//   String s("This is the string");

//   return s;
// }

// String f_str(void) { return String("foobar"); }
// String k_str("foobar");

// void f1(String s) {
//   Serial.printf("> %s\n", s.c_str());
// }
// void f2(void) {
//   f1(f_str());
// }

// void test_snprintf1(void) {
//   char c[100];
//   snprintf_P(c, sizeof(c), PSTR("s1=%s, s2=%s"), k_str.c_str(), f_str().c_str());
// }
// void test_snprintf2(void) {
//   char c[100];
//   ext_snprintf_P(c, sizeof(c), PSTR("s1=%_s, s2=%_S"), &k_str, &f_str, &ResponseAppendTHD);
// }
void test_ext_snprintf_P(void) {
//   test_snprintf1();
//   test_snprintf2();
//   if (0) {
  // // testVarArg2("", 1, 2, 3, 4, 5, 6, 7, 8);

  char c[128];
  float fpi=-3333.1415926535f;
  float f3 = 3333;
  float f31 = 3333.1;
  ext_snprintf_P(c, sizeof(c), "Int1 = %d, ip=%_I", 1, 0x10203040);
  Serial.printf("--> out=%s\n", c);

  ext_snprintf_P(c, sizeof(c), "Float default=%_f %_f", &f3, &fpi);
  Serial.printf("--> out=%s\n", c);

  ext_snprintf_P(c, sizeof(c), "Float default=%1_f, int(3)=%4_f, int(3)=%-4_f, int(3)=%-4_f, 6dec=%-8_f", &fpi, &f3, &f3, &f31, &fpi);
  Serial.printf("--> out=%s\n", c);
  ext_snprintf_P(c, sizeof(c), "Float default=%*_f, int(3)=%*_f, int(3)=%*_f, int(3)=%*_f, 6dec=%*_f", 1, &fpi, 4, &f3, -4, &f3, -4, &f31, -8, &fpi);
  Serial.printf("--> out=%s\n", c);
  uint64_t u641 = 0x1122334455667788LL;
  uint64_t u642 = 0x0123456789ABCDEFLL;
  uint64_t u643 = 0xFEDCBA9876543210LL;
  ext_snprintf_P(c, sizeof(c), "Int64 0x%_X 0x%_X 0x%_X", &u641, &u642, &u643);
  Serial.printf("--> out=%s\n", c);

  // ext_snprintf_P(c, sizeof(c), "Float default=%*_f, int(3)=%*_f, int(3)=%*_f, int(3)=%*_f, 6dec=%*_f", &fpi, &f3, &f3, &f31, &fpi);

  // String string("Foobar");
  // ext_snprintf_P(c, sizeof(c), "String 0x%08X %_s", &string, &string);
  // Serial.printf("--> out=%s\n", c);
  
  // ext_snprintf_P(c, sizeof(c), "StringFunc 0x%08X %_S", &test_string, &test_string);
  // Serial.printf("--> out=%s\n", c);

  // uint64_t u64 = 0x123456789ABCDEFLL;
  // testVarArg2("", u64, 2, 3, 4, 5, 6, 7, 8);

  // // Serial.printf("+++ ld=%ld, lld=%lld\n", 1,2,3,4);
  // // testVarArg("", 1, 2, 3, 4, 5, 6, 7, 8);
  // }
  // tprintf("%s", 12, "14");
}


// void tprintf(const char* format) // base function
// {
//   Serial.printf("%s\n", format);
// }
 
// template<typename T, typename... Targs>
// void tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
// {
//   for ( ; *format != '\0'; format++ ) {
//     if ( *format == '%' ) {
//       Serial.printf("%d", (uint32_t) value);
//       tprintf(format+1, Fargs...); // recursive call
//       return;
//     }
//     Serial.printf("%s", format);
//   }
// }
