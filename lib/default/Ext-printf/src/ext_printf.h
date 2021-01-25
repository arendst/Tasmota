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

#ifndef EXT_PRINTF_H
#define EXT_PRINTF_H

#include <cstddef>
#include <cstdint>
#include <cstdarg>

int32_t ext_vsnprintf_P(char * buf, size_t buf_len, const char * fmt_P, va_list va);
int32_t ext_snprintf_P(char * buf, size_t buf_len, const char * fmt, ...);

char* ToHex_P(const unsigned char * in, size_t insz, char * out, size_t outsz, char inbetween);

// void test_ext_snprintf_P(void);

#endif // EXT_PRINTF_H