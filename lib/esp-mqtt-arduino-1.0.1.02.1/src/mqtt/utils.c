/*
* Copyright (c) 2014, Tuan PM
* Email: tuanpm@live.com
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include "utils.h"


uint8_t ICACHE_FLASH_ATTR UTILS_IsIPV4 (int8_t *str)
{
  uint8_t segs = 0;   /* Segment count. */
  uint8_t chcnt = 0;  /* Character count within segment. */
  uint8_t accum = 0;  /* Accumulator for segment. */
  /* Catch NULL pointer. */
  if (str == 0)
    return 0;
  /* Process every character in string. */

  while (*str != '\0') {
    /* Segment changeover. */

    if (*str == '.') {
      /* Must have some digits in segment. */
      if (chcnt == 0)
        return 0;
      /* Limit number of segments. */
      if (++segs == 4)
        return 0;
      /* Reset segment values and restart loop. */
      chcnt = accum = 0;
      str++;
      continue;
    }

    /* Check numeric. */
    if ((*str < '0') || (*str > '9'))
      return 0;

    /* Accumulate and check segment. */

    if ((accum = accum * 10 + *str - '0') > 255)
      return 0;
    /* Advance other segment specific stuff and continue loop. */

    chcnt++;
    str++;
  }

  /* Check enough segments and enough characters in last segment. */

  if (segs != 3)
    return 0;
  if (chcnt == 0)
    return 0;
  /* Address okay. */

  return 1;
}
uint8_t ICACHE_FLASH_ATTR UTILS_StrToIP(const int8_t* str, void *ip)
{

  /* The count of the number of bytes processed. */
  int i;
  /* A pointer to the next digit to process. */
  const char * start;

  start = str;
  for (i = 0; i < 4; i++) {
    /* The digit being processed. */
    char c;
    /* The value of this byte. */
    int n = 0;
    while (1) {
      c = * start;
      start++;
      if (c >= '0' && c <= '9') {
        n *= 10;
        n += c - '0';
      }
      /* We insist on stopping at "." if we are still parsing
         the first, second, or third numbers. If we have reached
         the end of the numbers, we will allow any character. */
      else if ((i < 3 && c == '.') || i == 3) {
        break;
      }
      else {
        return 0;
      }
    }
    if (n >= 256) {
      return 0;
    }
    ((uint8_t*)ip)[i] = n;
  }
  return 1;

}
uint32_t ICACHE_FLASH_ATTR UTILS_Atoh(const int8_t *s)
{
  uint32_t value = 0, digit;
  int8_t c;

  while ((c = *s++)) {
    if ('0' <= c && c <= '9')
      digit = c - '0';
    else if ('A' <= c && c <= 'F')
      digit = c - 'A' + 10;
    else if ('a' <= c && c <= 'f')
      digit = c - 'a' + 10;
    else break;

    value = (value << 4) | digit;
  }

  return value;
}

