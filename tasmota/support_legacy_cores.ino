/*
  support_legacy_cores.ino - Legacy arduino core support for Tasmota

  Copyright (C) 2020  Theo Arends

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

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
/*********************************************************************************************\
 * Functions not available in core 2.3.0
\*********************************************************************************************/

// http://clc-wiki.net/wiki/C_standard_library:string.h:memchr
void* memchr(const void* ptr, int value, size_t num)
{
  unsigned char *p = (unsigned char*)ptr;
  while (num--) {
    if (*p != (unsigned char)value) {
      p++;
    } else {
      return p;
    }
  }
  return 0;
}

// http://clc-wiki.net/wiki/C_standard_library:string.h:strcspn
// Get span until any character in string
size_t strcspn(const char *str1, const char *str2)
{
  size_t ret = 0;
  while (*str1) {
    if (strchr(str2, *str1)) {  // Slow
      return ret;
    } else {
      str1++;
      ret++;
    }
  }
  return ret;
}

// https://clc-wiki.net/wiki/C_standard_library:string.h:strpbrk
// Locate the first occurrence in the string pointed to by s1 of any character from the string pointed to by s2
char* strpbrk(const char *s1, const char *s2)
{
  while(*s1) {
    if (strchr(s2, *s1++)) {
      return (char*)--s1;
    }
  }
  return 0;
}

// https://opensource.apple.com/source/Libc/Libc-583/stdlib/FreeBSD/strtoull.c
// Convert a string to an unsigned long long integer
#ifndef __LONG_LONG_MAX__
#define __LONG_LONG_MAX__ 9223372036854775807LL
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX (__LONG_LONG_MAX__ * 2ULL + 1)
#endif

unsigned long long strtoull(const char *__restrict nptr, char **__restrict endptr, int base)
{
  const char *s = nptr;
  char c;
  do { c = *s++; } while (isspace((unsigned char)c));                         // Trim leading spaces

  int neg = 0;
  if (c == '-') {                                                             // Set minus flag and/or skip sign
    neg = 1;
    c = *s++;
  } else {
    if (c == '+') {
      c = *s++;
    }
  }

  if ((base == 0 || base == 16) && (c == '0') && (*s == 'x' || *s == 'X')) {  // Set Hexadecimal
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0) { base = (c == '0') ? 8 : 10; }                              // Set Octal or Decimal

  unsigned long long acc = 0;
  int any = 0;
  if (base > 1 && base < 37) {
    unsigned long long cutoff = ULLONG_MAX / base;
    int cutlim = ULLONG_MAX % base;
    for ( ; ; c = *s++) {
      if (c >= '0' && c <= '9')
        c -= '0';
      else if (c >= 'A' && c <= 'Z')
        c -= 'A' - 10;
      else if (c >= 'a' && c <= 'z')
        c -= 'a' - 10;
      else
        break;

      if (c >= base)
        break;

      if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
        any = -1;
      else {
        any = 1;
        acc *= base;
        acc += c;
      }
    }
    if (any < 0) {
      acc = ULLONG_MAX;                                                       // Range error
    }
    else if (any && neg) {
      acc = -acc;
    }
  }

  if (endptr != nullptr) { *endptr = (char *)(any ? s - 1 : nptr); }

  return acc;
}

#endif  // ARDUINO_ESP8266_RELEASE_2_3_0



#if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2) || defined(ARDUINO_ESP8266_RELEASE_2_5_0) || defined(ARDUINO_ESP8266_RELEASE_2_5_1) || defined(ARDUINO_ESP8266_RELEASE_2_5_2)
/*********************************************************************************************\
 * Functions not available in core before 2.6.0
\*********************************************************************************************/

// https://github.com/arendst/Tasmota/issues/6856#issuecomment-554258914
void* memmove_P(void *dest, const void *src, size_t n)
{
  if (src > (void*)0x40000000) {
    return memcpy_P(dest, src, n);
  } else {
    return memmove(dest, src, n);
  }
}

#endif  // ARDUINO_ESP8266_RELEASE < 2_6_0



/*********************************************************************************************\
 * Core overrides
\*********************************************************************************************/

// Add below line to tasmota_post.h
// extern "C" void resetPins();
void resetPins()
{
/*
  for (int i = 0; i <= 5; ++i) {
    pinMode(i, INPUT);
  }
  // pins 6-11 are used for the SPI flash interface
  for (int i = 12; i <= 16; ++i) {
    pinMode(i, INPUT);
  }
*/
}
