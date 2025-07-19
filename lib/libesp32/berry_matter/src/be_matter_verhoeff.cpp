/*
  be_matter_verhoeff.cpp - implements the Verhoeff algorithm to compute a checksum digit for QR Code

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

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

#include <string.h>
#include <stdbool.h>
#include "be_constobj.h"
#include "be_mapping.h"

// code inspired from https://www.programmingalgorithms.com/algorithm/verhoeff-algorithm/c/

static const int8_t _multiplicationTable[10][10] = {
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
  { 1, 2, 3, 4, 0, 6, 7, 8, 9, 5 },
  { 2, 3, 4, 0, 1, 7, 8, 9, 5, 6 },
  { 3, 4, 0, 1, 2, 8, 9, 5, 6, 7 },
  { 4, 0, 1, 2, 3, 9, 5, 6, 7, 8 },
  { 5, 9, 8, 7, 6, 0, 4, 3, 2, 1 },
  { 6, 5, 9, 8, 7, 1, 0, 4, 3, 2 },
  { 7, 6, 5, 9, 8, 2, 1, 0, 4, 3 },
  { 8, 7, 6, 5, 9, 3, 2, 1, 0, 4 },
  { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }
};

static const int8_t _permutationTable[10][10] = {
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
  { 1, 5, 7, 6, 2, 8, 3, 0, 9, 4 },
  { 5, 8, 0, 3, 7, 9, 6, 1, 4, 2 },
  { 8, 9, 1, 6, 0, 4, 3, 5, 2, 7 },
  { 9, 4, 5, 3, 1, 2, 6, 8, 7, 0 },
  { 4, 2, 8, 6, 5, 7, 3, 9, 0, 1 },
  { 2, 7, 9, 3, 8, 0, 6, 4, 1, 5 },
  { 7, 0, 4, 6, 9, 1, 3, 2, 5, 8 }
};

static const int8_t _inverseTable[10] = { 0, 4, 3, 2, 1, 5, 6, 7, 8, 9 };

static char ret_digit[] = "0";       // used as a placeholder to return a single digit string

const char* vh_checksum(const char* number) {
  int32_t c = 0;
  int32_t len = strlen(number);

  for (int32_t i = 0; i < len; ++i) {
    int8_t digit = number[len - i - 1] - '0';
    if (digit < 0) { digit = 0; }
    if (digit > 9) { digit = 9; }
    c = _multiplicationTable[c][_permutationTable[((i + 1) % 8)][digit]];
  }
  
  ret_digit[0] = _inverseTable[c] + '0';
  return ret_digit;
}
BE_FUNC_CTYPE_DECLARE(vh_checksum, "s", "s")

static bbool vh_validate(char* number) {
  int32_t c = 0;
  int32_t len = strlen(number);

  for (int32_t i = 0; i < len; ++i) {
    int8_t digit = number[len - i - 1] - '0';
    if (digit < 0) { digit = 0; }
    if (digit > 9) { digit = 9; }
    c = _multiplicationTable[c][_permutationTable[(i % 8)][digit]];
  }
  return c == 0;
}
BE_FUNC_CTYPE_DECLARE(vh_validate, "b", "s")

#include "be_fixed_be_class_Matter_Verhoeff.h"

/* @const_object_info_begin
class be_class_Matter_Verhoeff (scope: global, name: Matter_Verhoeff, strings: weak) {
  checksum, static_ctype_func(vh_checksum)
  validate, static_ctype_func(vh_validate)
}
@const_object_info_end */


/*

# Unit tests
import matter

assert(matter.Verhoeff.checksum("236") == "3")
assert(matter.Verhoeff.validate("236"))
assert(matter.Verhoeff.checksum("58564") == "9")
assert(matter.Verhoeff.validate("585649"))

*/