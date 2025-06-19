/*
 * Copyright (C) 2019 Siara Logics (cc)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * @author Arundale R.
 *
 */

/*
 * 
 * This is a highly modified and optimized version of Unishox
 * for Tasmota, aimed at compressing `Rules` which are typically 
 * short strings from 50 to 500 bytes.
 * 
 * - moved to C++ (but still C-style)
 * - c_95[] and l_95[] are pre-computed
 * - all arrays in PROGMEM
 * - removed all Unicode specific code to get code smaller, Unicode is rare in rules and encoded as pure binary
 * - removed prev_lines management to reduce code size, we don't track previous encodings
 * - using C++ const instead of #define
 * - reusing the Unicode market to encode pure binary, which is 3 bits instead of 9
 * - reverse binary encoding to 255-byte, favoring short encoding for values above 127, typical of Unicode
 * - remove 2 bits encoding for Counts, since it could lead to a series of more than 8 consecutive 0-bits and output NULL char.
 *   Minimum encoding is 5 bits, which means spending 3+1=4 more bits for values in the range 0..3
 * - removed CRLF encoding and reusing entry for RPT, saving 3 bits for repeats. Note: any CR will be binary encded
 * - add safeguard to the output size (len_out), note that the compress buffer needs to be 4 bytes larger than actual compressed output.
 *   This is needed to avoid crash, since output can have ~30 bits
 * - combined c_95[] and l_95[] to a single array to save space
 * - Changed mapping of some characters in Set3, Set4 and Set4A, favoring frequent characters in rules and javascript
 * - Added escape mechanism to ensure we never output NULL char. The marker is 0x2A which looked rare in preliminary tests
 * 
 * @author Stephan Hadinger
 *
 */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#include <pgmspace.h>
#include "unishox.h"

typedef unsigned char byte;
// we squeeze both c_95[] and l_95[] in a sinle array.
// c_95[] uses only the 3 upper nibbles (or 12 most signifcant bits), while the last nibble encodes length (3..13)
// static uint16_t cl_95[95] PROGMEM = {0x4000 +  3, 0x3F80 + 11, 0x3D80 + 11, 0x3C80 + 10, 0x3BE0 + 12, 0x3E80 + 10, 0x3F40 + 11, 0x3EC0 + 10, 0x3BA0 + 11, 0x3BC0 + 11, 0x3D60 + 11, 0x3B60 + 11, 0x3A80 + 10, 0x3AC0 + 10, 0x3A00 +  9, 0x3B00 + 10, 0x38C0 + 10, 0x3900 + 10, 0x3940 + 11, 0x3960 + 11, 0x3980 + 11, 0x39A0 + 11, 0x39C0 + 11, 0x39E0 + 12, 0x39F0 + 12, 0x3880 + 10, 0x3CC0 + 10, 0x3C00 +  9, 0x3D00 + 10, 0x3E00 +  9, 0x3F00 + 10, 0x3B40 + 11, 0x3BF0 + 12, 0x2B00 +  8, 0x21C0 + 11, 0x20C0 + 10, 0x2100 + 10, 0x2600 +  7, 0x2300 + 11, 0x21E0 + 12, 0x2140 + 11, 0x2D00 +  8, 0x2358 + 13, 0x2340 + 12, 0x2080 + 10, 0x21A0 + 11, 0x2E00 +  8, 0x2C00 +  8, 0x2180 + 11, 0x2350 + 13, 0x2F80 +  9, 0x2F00 +  9, 0x2A00 +  8, 0x2160 + 11, 0x2330 + 12, 0x21F0 + 12, 0x2360 + 13, 0x2320 + 12, 0x2368 + 13, 0x3DE0 + 12, 0x3FA0 + 11, 0x3DF0 + 12, 0x3D40 + 11, 0x3F60 + 11, 0x3FF0 + 12, 0xB000 +  4, 0x1C00 +  7, 0x0C00 +  6, 0x1000 +  6, 0x6000 +  3, 0x3000 +  7, 0x1E00 +  8, 0x1400 +  7, 0xD000 +  4, 0x3580 +  9, 0x3400 +  8, 0x0800 +  6, 0x1A00 +  7, 0xE000 +  4, 0xC000 +  4, 0x1800 +  7, 0x3500 +  9, 0xF800 +  5, 0xF000 +  5, 0xA000 +  4, 0x1600 +  7, 0x3300 +  8, 0x1F00 +  8, 0x3600 +  9, 0x3200 +  8, 0x3680 +  9, 0x3DA0 + 11, 0x3FC0 + 11, 0x3DC0 + 11, 0x3FE0 + 12 };
// Patched, for len == 13, shift 1 bit right
static uint16_t cl_95[95] PROGMEM = {0x4000 +  3, 0x3F80 + 11, 0x3D80 + 11, 0x3C80 + 10, 0x3BE0 + 12, 0x3E80 + 10, 0x3F40 + 11, 0x3EC0 + 10, 0x3BA0 + 11, 0x3BC0 + 11, 0x3D60 + 11, 0x3B60 + 11, 0x3A80 + 10, 0x3AC0 + 10, 0x3A00 +  9, 0x3B00 + 10, 0x38C0 + 10, 0x3900 + 10, 0x3940 + 11, 0x3960 + 11, 0x3980 + 11, 0x39A0 + 11, 0x39C0 + 11, 0x39E0 + 12, 0x39F0 + 12, 0x3880 + 10, 0x3CC0 + 10, 0x3C00 +  9, 0x3D00 + 10, 0x3E00 +  9, 0x3F00 + 10, 0x3B40 + 11, 0x3BF0 + 12, 0x2B00 +  8, 0x21C0 + 11, 0x20C0 + 10, 0x2100 + 10, 0x2600 +  7, 0x2300 + 11, 0x21E0 + 12, 0x2140 + 11, 0x2D00 +  8, 0x46B0 + 13, 0x2340 + 12, 0x2080 + 10, 0x21A0 + 11, 0x2E00 +  8, 0x2C00 +  8, 0x2180 + 11, 0x46A0 + 13, 0x2F80 +  9, 0x2F00 +  9, 0x2A00 +  8, 0x2160 + 11, 0x2330 + 12, 0x21F0 + 12, 0x46C0 + 13, 0x2320 + 12, 0x46D0 + 13, 0x3DE0 + 12, 0x3FA0 + 11, 0x3DF0 + 12, 0x3D40 + 11, 0x3F60 + 11, 0x3FF0 + 12, 0xB000 +  4, 0x1C00 +  7, 0x0C00 +  6, 0x1000 +  6, 0x6000 +  3, 0x3000 +  7, 0x1E00 +  8, 0x1400 +  7, 0xD000 +  4, 0x3580 +  9, 0x3400 +  8, 0x0800 +  6, 0x1A00 +  7, 0xE000 +  4, 0xC000 +  4, 0x1800 +  7, 0x3500 +  9, 0xF800 +  5, 0xF000 +  5, 0xA000 +  4, 0x1600 +  7, 0x3300 +  8, 0x1F00 +  8, 0x3600 +  9, 0x3200 +  8, 0x3680 +  9, 0x3DA0 + 11, 0x3FC0 + 11, 0x3DC0 + 11, 0x3FE0 + 12 };
// Original version with c/l separate
// uint16_t c_95[95] PROGMEM = {0x4000, 0x3F80, 0x3D80, 0x3C80, 0x3BE0, 0x3E80, 0x3F40, 0x3EC0, 0x3BA0, 0x3BC0, 0x3D60, 0x3B60, 0x3A80, 0x3AC0, 0x3A00, 0x3B00, 0x38C0, 0x3900, 0x3940, 0x3960, 0x3980, 0x39A0, 0x39C0, 0x39E0, 0x39F0, 0x3880, 0x3CC0, 0x3C00, 0x3D00, 0x3E00, 0x3F00, 0x3B40, 0x3BF0, 0x2B00, 0x21C0, 0x20C0, 0x2100, 0x2600, 0x2300, 0x21E0, 0x2140, 0x2D00, 0x2358, 0x2340, 0x2080, 0x21A0, 0x2E00, 0x2C00, 0x2180, 0x2350, 0x2F80, 0x2F00, 0x2A00, 0x2160, 0x2330, 0x21F0, 0x2360, 0x2320, 0x2368, 0x3DE0, 0x3FA0, 0x3DF0, 0x3D40, 0x3F60, 0x3FF0, 0xB000, 0x1C00, 0x0C00, 0x1000, 0x6000, 0x3000, 0x1E00, 0x1400, 0xD000, 0x3580, 0x3400, 0x0800, 0x1A00, 0xE000, 0xC000, 0x1800, 0x3500, 0xF800, 0xF000, 0xA000, 0x1600, 0x3300, 0x1F00, 0x3600, 0x3200, 0x3680, 0x3DA0, 0x3FC0, 0x3DC0, 0x3FE0 };
// uint8_t  l_95[95] PROGMEM = {     3,     11,     11,     10,     12,     10,     11,     10,     11,     11,     11,     11,     10,     10,      9,     10,     10,     10,     11,     11,     11,     11,     11,     12,     12,     10,     10,      9,     10,      9,     10,     11,     12,      8,     11,     10,     10,      7,     11,     12,     11,      8,     13,     12,     10,     11,      8,      8,     11,     13,      9,      9,      8,     11,     12,     12,     13,     12,     13,     12,     11,     12,     11,     11,     12,      4,      7,      6,      6,      3,      7,      8,      7,      4,      9,      8,      6,      7,      4,      4,      7,      9,      5,      5,      4,      7,      8,      8,      9,      8,      9,     11,     11,     11,     12 };

enum {SHX_STATE_1 = 1, SHX_STATE_2};    // removed Unicode state

enum {SHX_SET1 = 0, SHX_SET1A, SHX_SET1B, SHX_SET2, SHX_SET3, SHX_SET4, SHX_SET4A};
// changed mapping in Set3, Set4, Set4A to accomodate frequencies in Rules and Javascript
static char sets[][11] PROGMEM = 
                  {{  0, ' ', 'e',   0, 't', 'a', 'o', 'i', 'n', 's', 'r'},
                   {  0, 'l', 'c', 'd', 'h', 'u', 'p', 'm', 'b', 'g', 'w'},
                   {'f', 'y', 'v', 'k', 'q', 'j', 'x', 'z',   0,   0,   0},
                   {  0, '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'},
                   {'.', ',', '-', '/', '?', '+', ' ', '(', ')', '$', '@'},
                   {';', '#', ':', '<', '^', '*', '"', '{', '}', '[', ']'},
                   {'=', '%', '\'', '>', '&', '_', '!', '\\', '|', '~', '`'}};

// Decoder is designed for using less memory, not speed
// Decode lookup table for code index and length
// First 2 bits 00, Next 3 bits indicate index of code from 0,
// last 3 bits indicate code length in bits
//                0,            1,            2,            3,            4,
static char us_vcode[32] PROGMEM = 
                 {2 + (0 << 3), 3 + (3 << 3), 3 + (1 << 3), 4 + (6 << 3), 0,
//                5,            6,            7,            8, 9, 10
                  4 + (4 << 3), 3 + (2 << 3), 4 + (8 << 3), 0, 0,  0,
//                11,          12, 13,            14, 15
                  4 + (7 << 3), 0,  4 + (5 << 3),  0,  5 + (9 << 3),
//                16, 17, 18, 19, 20, 21, 22, 23
                   0,  0,  0,  0,  0,  0,  0,  0,
//                24, 25, 26, 27, 28, 29, 30, 31
                   0, 0,  0,  0,  0,  0,  0,  5 + (10 << 3)};
//                0,            1,            2, 3,            4, 5, 6, 7,
static char us_hcode[32] PROGMEM =
                 {1 + (1 << 3), 2 + (0 << 3), 0, 3 + (2 << 3), 0, 0, 0, 5 + (3 << 3),
//                8, 9, 10, 11, 12, 13, 14, 15,
                  0, 0,  0,  0,  0,  0,  0,  5 + (5 << 3),
//                16, 17, 18, 19, 20, 21, 22, 23
                   0, 0,  0,  0,  0,  0,  0,  5 + (4 << 3),
//                24, 25, 26, 27, 28, 29, 30, 31
                   0, 0,  0,  0,  0,  0,  0,  5 + (6 << 3)};

static const char ESCAPE_MARKER = 0x2A;   // Escape any null char

static const uint16_t TERM_CODE = 0x37C0; // 0b0011011111000000
static const uint16_t TERM_CODE_LEN = 10;
static const uint16_t DICT_CODE = 0x0000;
static const uint16_t DICT_CODE_LEN = 5;
static const uint16_t DICT_OTHER_CODE = 0x0000; // not used
static const uint16_t DICT_OTHER_CODE_LEN = 6;
static const uint16_t RPT_CODE_TASMOTA = 0x3780;
static const uint16_t RPT_CODE_TASMOTA_LEN = 10;
static const uint16_t BACK2_STATE1_CODE = 0x2000;    // 0010 = back to lower case
static const uint16_t BACK2_STATE1_CODE_LEN = 4;
static const uint16_t BACK_FROM_UNI_CODE = 0xFE00;
static const uint16_t BACK_FROM_UNI_CODE_LEN = 8;
static const uint16_t LF_CODE = 0x3700;
static const uint16_t LF_CODE_LEN = 9;
static const uint16_t TAB_CODE = 0x2400;
static const uint16_t TAB_CODE_LEN = 7;
static const uint16_t ALL_UPPER_CODE = 0x2200;
static const uint16_t ALL_UPPER_CODE_LEN = 8;
static const uint16_t SW2_STATE2_CODE = 0x3800;
static const uint16_t SW2_STATE2_CODE_LEN = 7;
static const uint16_t ST2_SPC_CODE = 0x3B80;
static const uint16_t ST2_SPC_CODE_LEN = 11;
static const uint16_t BIN_CODE_TASMOTA = 0x8000;
static const uint16_t BIN_CODE_TASMOTA_LEN = 3;

#define NICE_LEN 5

// uint16_t mask[] PROGMEM = {0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00};
static const uint8_t mask[] PROGMEM = {0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};



void Unishox::append_bits(unsigned int code, int clen) {
// Serial.printf("append_bits code = 0x%08X, clen %d\n", code, clen);
  byte cur_bit;
  byte blen;
  unsigned char a_byte;

  if (state == SHX_STATE_2) {
    // remove change state prefix
    if ((code >> 9) == 0x1C) {
        code <<= 7;
        clen -= 7;
    }
  }
  while (clen > 0) {
    cur_bit = ol % 8;
    blen = (clen > 8 ? 8 : clen);
    a_byte = (code >> 8) & pgm_read_word(&mask[blen - 1]);
    a_byte >>= cur_bit;
    if (blen + cur_bit > 8)
      blen = (8 - cur_bit);
    if (out) {                // if out == nullptr, then we are in dry-run mode
      if (cur_bit == 0)
        out[ol >> 3] = a_byte;
      else
        out[ol >> 3] |= a_byte;
    }
    code <<= blen;
    ol += blen;
    if ((out) && (0 == ol % 8)) {           // if out == nullptr, dry-run mode. We miss the escaping of characters in the length
      // we completed a full byte
      char last_c = out[(ol / 8) - 1];
      if ((0 == last_c) || (ESCAPE_MARKER == last_c)) {
        out[ol >> 3] = 1 + last_c;           // increment to 0x01 or 0x2B
        out[(ol >>3) -1] = ESCAPE_MARKER;   // replace old value with marker
        ol += 8;   // add one full byte
      }
    }
    clen -= blen;
  }
}

// First five bits are code and Last three bits of codes represent length
// removing last 2 bytes, unused, we will never have values above 600 bytes
//   const byte codes[7] = {0x01, 0x82, 0xC3, 0xE5, 0xED, 0xF5, 0xFD};
//   const byte bit_len[7] =  {2, 5,  7,   9,  12,   16,  17};
//   const uint16_t adder[7] = {0, 4, 36, 164, 676, 4772,  0};
byte codes[] PROGMEM     = { 0x82, 0xC3, 0xE5, 0xED, 0xF5 };
byte bit_len[] PROGMEM   = {    5,    7,    9,   12,   16 };
// uint16_t adder[7] PROGMEM = {    0,   32,  160,  672, 4768 };  // no more used

void Unishox::encodeCount(int32_t count) {
  int till = 0;
  int base = 0;
  for (uint32_t i = 0; i < sizeof(bit_len); i++) {
    uint32_t bit_len_i = pgm_read_byte(&bit_len[i]);
    till += (1 << bit_len_i);
    if (count < till) {
      byte codes_i = pgm_read_byte(&codes[i]);
      append_bits((codes_i & 0xF8) << 8, codes_i & 0x07);
      // ol = append_bits(out, ol, (count - pgm_read_word(&adder[i])) << (16 - bit_len_i), bit_len_i, 1);
      append_bits((count - base) << (16 - bit_len_i), bit_len_i);
      return;
    }
    base = till;
  }
  return;
}

bool Unishox::matchOccurance(void) {
  int32_t j, k;
  uint32_t longest_dist = 0;
  uint32_t longest_len = 0;
  for (j = l - NICE_LEN; j >= 0; j--) {
    for (k = l; k < len && j + k - l < l; k++) {
      if (in[k] != in[j + k - l])
        break;
    }
    if (k - l > NICE_LEN - 1) {
      uint32_t match_len = k - l - NICE_LEN;
      uint32_t match_dist = l - j - NICE_LEN + 1;
      if (match_len > longest_len) {
        longest_len = match_len;
        longest_dist = match_dist;
      }
    }
  }
  if (longest_len) {
    if (state == SHX_STATE_2 || is_all_upper) {
      is_all_upper = 0;
      state = SHX_STATE_1;
      append_bits(BACK2_STATE1_CODE, BACK2_STATE1_CODE_LEN);
    }
    append_bits(DICT_CODE, DICT_CODE_LEN);
    encodeCount(longest_len);
    encodeCount(longest_dist);
    l += longest_len + NICE_LEN - 1;
    return true;
  }
  return false;
}

// Compress a buffer.
// Inputs:
//   - in: non-null pointer to a buffer of bytes to be compressed. Progmem is not valid. Null bytes are valid.
//   - len: size of the input buffer. 0 is valid for empty buffer
//   - out: pointer to output buffer. out is nullptr, the compressor does a dry-run and reports the compressed size without writing bytes
//   - len_out: length in bytes of the output buffer.
// Output:
//   - if >= 0: size of the compressed buffer. The output buffer does not contain NULL bytes, and it is not NULL terminated
//   - if < 0: an error occured, most certainly the output buffer was not large enough
int32_t Unishox::unishox_compress(const char *p_in, size_t p_len, char *p_out, size_t p_len_out) {
  in = p_in;
  len = p_len;
  out = p_out;
  len_out = p_len_out;

  byte bits;

  int ll;
  char c_in, c_next;
  byte is_upper;

  ol = 0;
  state = SHX_STATE_1;
  is_all_upper = 0;
  for (l=0; l<len; l++) {

    c_in = in[l];

    if (l && l < len - 4) {
      if (c_in == in[l - 1] && c_in == in[l + 1] && c_in == in[l + 2] && c_in == in[l + 3]) {   // check for repeat
        int rpt_count = l + 4;
        while (rpt_count < len && in[rpt_count] == c_in)
          rpt_count++;
        rpt_count -= l;
        
        if (state == SHX_STATE_2 || is_all_upper) {
          is_all_upper = 0;
          state = SHX_STATE_1;
          append_bits(BACK2_STATE1_CODE, BACK2_STATE1_CODE_LEN);   // back to lower case and Set1
        }
        // ol = append_bits(out, ol, RPT_CODE, RPT_CODE_LEN, 1);
        append_bits(RPT_CODE_TASMOTA, RPT_CODE_TASMOTA_LEN);     // reusing CRLF for RPT
        encodeCount(rpt_count - 4);
        l += rpt_count - 1;
        continue;
      }
    }

    if (l < (len - NICE_LEN + 1)) {
      if (matchOccurance()) {
        continue;
      }
    }
    if (state == SHX_STATE_2) {     // if Set2
      if ((c_in >= ' ' && c_in <= '@') ||
          (c_in >= '[' && c_in <= '`') ||
          (c_in >= '{' && c_in <= '~')) {
      } else {
        state = SHX_STATE_1;        // back to Set1 and lower case
        append_bits(BACK2_STATE1_CODE, BACK2_STATE1_CODE_LEN);
      }
    }

    is_upper = 0;
    if (c_in >= 'A' && c_in <= 'Z')
      is_upper = 1;
    else {
      if (is_all_upper) {
        is_all_upper = 0;
        append_bits(BACK2_STATE1_CODE, BACK2_STATE1_CODE_LEN);
      }
    }

    c_next = 0;
    if (l+1 < len)
      c_next = in[l+1];

    if (c_in >= 32 && c_in <= 126) {
      if (is_upper && !is_all_upper) {
        for (ll=l+5; ll>=l && ll<len; ll--) {
          if (in[ll] < 'A' || in[ll] > 'Z')
            break;
        }
        if (ll == l-1) {
          append_bits(ALL_UPPER_CODE, ALL_UPPER_CODE_LEN);   // CapsLock
          is_all_upper = 1;
        }
      }
      if (state == SHX_STATE_1 && c_in >= '0' && c_in <= '9') {
        append_bits(SW2_STATE2_CODE, SW2_STATE2_CODE_LEN);   // Switch to sticky Set2
        state = SHX_STATE_2;
      }
      c_in -= 32;
      if (is_all_upper && is_upper)
        c_in += 32;
      if (c_in == 0 && state == SHX_STATE_2)
        append_bits(ST2_SPC_CODE, ST2_SPC_CODE_LEN);       // space from Set2 ionstead of Set1
      else {
// Serial.printf("Encode %c %d\n", c_in + 32, c_in);
        uint16_t cl = pgm_read_word(&cl_95[c_in]);
        uint16_t cl_code = cl & 0xFFF0;
        uint8_t  cl_len = cl & 0x000F;
        if (13 == cl_len) {
          cl_code >>= 1;
        }
        append_bits(cl_code, cl_len);
      }
    } else if (c_in == 10) {
      append_bits(LF_CODE, LF_CODE_LEN);         // LF
    } else if (c_in == '\t') {
      append_bits(TAB_CODE, TAB_CODE_LEN);       // TAB
    } else {
      append_bits(BIN_CODE_TASMOTA, BIN_CODE_TASMOTA_LEN);       // Binary, we reuse the Unicode marker which 3 bits instead of 9
      encodeCount((unsigned char) 255 - c_in);
    }

    // check that we have some headroom in the output buffer
    if (ol / 8 >= len_out - 4) {
      return -1;      // we risk overflow and crash
    }
  }

  bits = ol % 8;
  if (bits) {
    state = SHX_STATE_1;
    append_bits(TERM_CODE, 8 - bits);   // 0011 0111 1100 0000 TERM = 0011 0111 11
  }
  return ol / 8;    // we already arrived to a byte boundary
  // return ol/8+(ol%8?1:0);
}

uint32_t Unishox::getNextBit(void) {
  if (8 == bit_no) {
    if (byte_no >= len) {
      in_eof = true;
      return 1;             // return only 1s, which appends 'r' in worst case
    }
    byte_in = pgm_read_byte(&in[byte_no++]);
    if (ESCAPE_MARKER == byte_in) {
      byte_in = pgm_read_byte(&in[byte_no++]) - 1;      // we shouldn't need to test if byte_no >= len, because it should not be possible to end with ESCAPE_MARKER
    }
    bit_no = 0;
  }
  // Serial.printf("getNextBit %d\n", byte_in & (0x80 >> bit_no) ? 1 : 0);
  return byte_in & (0x80 >> bit_no++) ? 1 : 0;
}

// Returns:
// 0..11
// or -1 if end of stream
int32_t Unishox::getCodeIdx(const char *code_type) {
  int32_t code = 0;
  int32_t count = 0;
  do {
    if (in_eof) return -1;           // invalid state
    code += getNextBit() << count;
    count++;
    uint8_t code_type_code = pgm_read_byte(&code_type[code]);
    if (code_type_code && (code_type_code & 0x07) == count) {
      return code_type_code >> 3;
    }
  } while (count < 5);
  return -1; // skip if code not found
}

int32_t Unishox::getNumFromBits(uint32_t count) {
  int ret = 0;
  while (count--) {
    ret += getNextBit() << count;
  }
  if (in_eof) return 0;
  return ret;
}

// const byte bit_len[7]   = {5, 2,  7,   9,  12,   16, 17};
// const uint16_t adder[7] = {4, 0, 36, 164, 676, 4772,  0};

// byte bit_len[7] PROGMEM   = {    5,    7,    9,   12,   16 };
// byte bit_len_read[7] PROGMEM = {5, 2,  7,   9,  12,   16 };
// uint16_t adder_read[7] PROGMEM = {4, 0, 36, 164, 676, 4772,  0};
// uint16_t adder_read[] PROGMEM = {0, 0, 32, 160, 672, 4768 };

// byte bit_len[7] PROGMEM   = {    5,    7,    9,   12,   16 };
// uint16_t adder_read[] PROGMEM = {0, 32, 160, 672, 4768 };

// Code size optimized, recalculate adder[] like in encodeCount
uint32_t Unishox::readCount(void) {
  int32_t idx = getCodeIdx(us_hcode);
  if ((1 == idx) || (idx >= sizeof(bit_len)) || (idx < 0)) return 0;  // unsupported or end of stream
  if (idx >= 1) idx--;    // we skip v = 1 (code '0') since we no more accept 2 bits encoding

  int base;
  int till = 0;
  byte bit_len_idx;   // bit_len[0]
  for (uint32_t i = 0; i <= idx; i++) {
    base = till;
    bit_len_idx = pgm_read_byte(&bit_len[i]);
    till += (1 << bit_len_idx);
  }
  int count = getNumFromBits(bit_len_idx) + base;

  return count;
}

void Unishox::decodeRepeat(void) {
  uint32_t dict_len = readCount() + NICE_LEN;
  uint32_t dist = readCount() + NICE_LEN - 1;
  if (ol + dict_len <= len_out) {
    memcpy(out + ol, out + ol - dist, dict_len);
    ol += dict_len;
  }
}

int32_t Unishox::unishox_decompress(const char *p_in, size_t p_len, char *p_out, size_t p_len_out) {
  in = p_in;
  len = p_len;
  out = p_out;
  len_out = p_len_out;

  in_eof = false;
  ol = 0;
  bit_no = 8;   // force load of first byte, pretending we expired the last one
  byte_no = 0;
  dstate = SHX_SET1;
  is_all_upper = 0;

  if (out) out[ol] = 0;
  // while ((byte_no << 3) + bit_no - 8 < len) {
  while (!in_eof) {
    if (out && ol >= len_out) {
      break;
    }
    int32_t h, v;
    char c = 0;
    byte is_upper = is_all_upper;
    v = getCodeIdx(us_vcode);    // read vCode
    if (v < 0) break;     // end of stream
    h = dstate;     // Set1 or Set2
    if (v == 0) {   // Switch which is common to Set1 and Set2, first entry
      h = getCodeIdx(us_hcode);    // read hCode
      if (h < 0) break;     // end of stream
      if (h == SHX_SET1) {          // target is Set1
         if (dstate == SHX_SET1) {  // Switch from Set1 to Set1 us UpperCase
            if (is_all_upper) {      // if CapsLock, then back to LowerCase
              is_upper = is_all_upper = 0;
              continue;
            }
            v = getCodeIdx(us_vcode);   // read again vCode
            if (v < 0) break;     // end of stream
            if (v == 0) {
              h = getCodeIdx(us_hcode);  // read second hCode
              if (h < 0) break;     // end of stream
              if (h == SHX_SET1) {  // If double Switch Set1, the CapsLock
                is_all_upper = 1;
                continue;
              }
            }
            is_upper = 1;      // anyways, still uppercase
         } else {
            dstate = SHX_SET1;  // if Set was not Set1, switch to Set1
            continue;
         }
      } else
      if (h == SHX_SET2) {    // If Set2, switch dstate to Set2
         if (dstate == SHX_SET1)    // TODO: is this test useful, there are only 2 states possible
           dstate = SHX_SET2;
         continue;
      }
      if (h != SHX_SET1) {    // all other Sets (why not else)
        v = getCodeIdx(us_vcode);    // we changed set, now read vCode for char
        if (v < 0) break;     // end of stream
      }
    }

    if (v == 0 && h == SHX_SET1A) {
      if (is_upper) {
        if (out) out[ol] = 255 - readCount();    // binary
        ol++;
      } else {
        decodeRepeat();   // dist
      }
      continue;
    }

    if (h == SHX_SET1 && v == 3) {
      // was Unicode, will do Binary instead
      if (out) out[ol] = 255 - readCount();    // binary
      ol++;
      continue;
    }
    if (h < 7 && v < 11)     // TODO: are these the actual limits? Not 11x7 ?
      c = pgm_read_byte(&sets[h][v]);
    if (c >= 'a' && c <= 'z') {
      if (is_upper)
        c -= 32;      // go to UpperCase for letters
    } else {          // handle all other cases
      if (is_upper && dstate == SHX_SET1 && v == 1)
        c = '\t';     // If UpperCase Space, change to TAB
      if (h == SHX_SET1B) {
        if (8 == v) {   // was LF or RPT, now only LF
          if (out) out[ol] = '\n';
          ol++;
          continue;
        }
        if (9 == v) {           // was CRLF, now RPT
          uint32_t count = readCount() + 4;
          if (out && ol + count >= len_out) {
            return -1;        // overflow
          }
          if (out) {
            char rpt_c = out[ol - 1];
            while (count--)
              out[ol++] = rpt_c;
          } else {
            ol += count;
          }
          continue;
        }
        if (10 == v) {
          break;          // TERM, stop decoding
        }
      }
    }
    // Serial.printf(">>>>>>>>>>>>>>>>>>>>>> Out = %c\n", c);
    if (out) out[ol] = c;
    ol++;
  }

  if (out && ol > len_out) {
    return -1;    // overflow
  } else {
    return ol;
  }
}
