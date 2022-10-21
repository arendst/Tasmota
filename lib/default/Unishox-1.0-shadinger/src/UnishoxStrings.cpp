/*
  UnishoxStrings.c - support library for compressed strings in Flash

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

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

#include "UnishoxStrings.h"
#include <pgmspace.h>

extern int32_t DecompressNoAlloc(const char * compressed, size_t uncompressed_size, String & content);

//
// If first byte is NULL, then the string is compressed
//
UnishoxStrings::UnishoxStrings(const char *k_msg) {
  if (pgm_read_byte(k_msg) == 0) {
    size_t len = pgm_read_byte(k_msg + 1) * 16;
    DecompressNoAlloc(k_msg + 2, len, _decompressed);
    _msg = _decompressed.c_str();
  } else {
    _msg = k_msg;
  }
}
