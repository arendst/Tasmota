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

// Pre-compute c_95[] and l_95[]

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

typedef unsigned char byte;

enum {SHX_SET1 = 0, SHX_SET1A, SHX_SET1B, SHX_SET2, SHX_SET3, SHX_SET4, SHX_SET4A};
char us_vcodes[] =     {0, 2, 3, 4, 10, 11, 12, 13, 14, 30, 31};
char us_vcode_lens[] = {2, 3, 3, 3,  4,  4,  4,  4,  4,  5,  5};
char us_sets[][11] = 
                  {{  0, ' ', 'e',   0, 't', 'a', 'o', 'i', 'n', 's', 'r'},
                   {  0, 'l', 'c', 'd', 'h', 'u', 'p', 'm', 'b', 'g', 'w'},
                   {'f', 'y', 'v', 'k', 'q', 'j', 'x', 'z',   0,   0,   0},
                   {  0, '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'},
                   {'.', ',', '-', '/', '?', '+', ' ', '(', ')', '$', '@'},
                   {';', '#', ':', '<', '^', '*', '"', '{', '}', '[', ']'},
                   {'=', '%', '\'', '>', '&', '_', '!', '\\', '|', '~', '`'}};
                  // {{  0, ' ', 'e',   0, 't', 'a', 'o', 'i', 'n', 's', 'r'},
                  //  {  0, 'l', 'c', 'd', 'h', 'u', 'p', 'm', 'b', 'g', 'w'},
                  //  {'f', 'y', 'v', 'k', 'q', 'j', 'x', 'z',   0,   0,   0},
                  //  {  0, '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'},
                  //  {'.', ',', '-', '/', '=', '+', ' ', '(', ')', '$', '%'},
                  //  {'&', ';', ':', '<', '>', '*', '"', '{', '}', '[', ']'},
                  //  {'@', '?', '\'', '^', '#', '_', '!', '\\', '|', '~', '`'}};

unsigned int  c_95[95] ;
unsigned char l_95[95] ;


void init_coder() {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 11; j++) {
      char c = us_sets[i][j];
      if (c != 0 && c != 32) {
        int ascii = c - 32;
        //int prev_code = c_95[ascii];
        //int prev_code_len = l_95[ascii];
        switch (i) {
          case SHX_SET1: // just us_vcode
            c_95[ascii] = (us_vcodes[j] << (16 - us_vcode_lens[j]));
            l_95[ascii] = us_vcode_lens[j];
            //checkPreus_vcodes(c, prev_code, prev_code_len, c_95[ascii], l_95[ascii]);
            if (c >= 'a' && c <= 'z') {
              ascii -= ('a' - 'A');
              //prev_code = c_95[ascii];
              //prev_code_len = l_95[ascii];
              c_95[ascii] = (2 << 12) + (us_vcodes[j] << (12 - us_vcode_lens[j]));
              l_95[ascii] = 4 + us_vcode_lens[j];
            }
            break;
          case SHX_SET1A: // 000 + us_vcode
            c_95[ascii] = 0 + (us_vcodes[j] << (13 - us_vcode_lens[j]));
            l_95[ascii] = 3 + us_vcode_lens[j];
            //checkPreus_vcodes(c, prev_code, prev_code_len, c_95[ascii], l_95[ascii]);
            if (c >= 'a' && c <= 'z') {
              ascii -= ('a' - 'A');
              //prev_code = c_95[ascii];
              //prev_code_len = l_95[ascii];
              c_95[ascii] = (2 << 12) + 0 + (us_vcodes[j] << (9 - us_vcode_lens[j]));
              l_95[ascii] = 4 + 3 + us_vcode_lens[j];
            }
            break;
          case SHX_SET1B: // 00110 + us_vcode
            c_95[ascii] = (6 << 11) + (us_vcodes[j] << (11 - us_vcode_lens[j]));
            l_95[ascii] = 5 + us_vcode_lens[j];
            //checkPreus_vcodes(c, prev_code, prev_code_len, c_95[ascii], l_95[ascii]);
            if (c >= 'a' && c <= 'z') {
              ascii -= ('a' - 'A');
              //prev_code = c_95[ascii];
              //prev_code_len = l_95[ascii];
              c_95[ascii] = (2 << 12) + (6 << 7) + (us_vcodes[j] << (7 - us_vcode_lens[j]));
              l_95[ascii] = 4 + 5 + us_vcode_lens[j];
            }
            break;
          case SHX_SET2: // 0011100 + us_vcode
            c_95[ascii] = (28 << 9) + (us_vcodes[j] << (9 - us_vcode_lens[j]));
            l_95[ascii] = 7 + us_vcode_lens[j];
            break;
          case SHX_SET3: // 0011101 + us_vcode
            c_95[ascii] = (29 << 9) + (us_vcodes[j] << (9 - us_vcode_lens[j]));
            l_95[ascii] = 7 + us_vcode_lens[j];
            break;
          case SHX_SET4: // 0011110 + us_vcode
            c_95[ascii] = (30 << 9) + (us_vcodes[j] << (9 - us_vcode_lens[j]));
            l_95[ascii] = 7 + us_vcode_lens[j];
            break;
          case SHX_SET4A: // 0011111 + us_vcode
            c_95[ascii] = (31 << 9) + (us_vcodes[j] << (9 - us_vcode_lens[j]));
            l_95[ascii] = 7 + us_vcode_lens[j];
        }
        //checkPreus_vcodes(c, prev_code, prev_code_len, c_95[ascii], l_95[ascii]);
      }
    }
  }
  c_95[0] = 16384;
  l_95[0] = 3;

}

int main(int argv, char *args[]) {
  init_coder();

  printf("uint16_t c_95[95] PROGMEM = {");
  for (uint8_t i = 0; i<95; i++) {
    if (i) { printf(", "); }
    printf("0x%04X", c_95[i]);
  }
  printf(" };\n");

  printf("uint8_t  l_95[95] PROGMEM = {");
  for (uint8_t i = 0; i<95; i++) {
    if (i) { printf(", "); }
    printf("%6d", l_95[i]);
  }
  printf(" };\n");

  printf("\n\n");

  printf("uint16_t c_95[95] PROGMEM = {");
  for (uint8_t i = 0; i<95; i++) {
    if (i) { printf(", "); }
    printf("%5d", c_95[i]);
  }
  printf(" };\n");

  printf("uint8_t  l_95[95] PROGMEM = {");
  for (uint8_t i = 0; i<95; i++) {
    if (i) { printf(", "); }
    printf("%5d", l_95[i]);
  }
  printf(" };\n");


  printf("uint16_t cl_95[95] PROGMEM = {");
  for (uint8_t i = 0; i<95; i++) {
    if (i) { printf(", "); }
    printf("0x%04X + %2d", c_95[i], l_95[i]);
  }
  printf(" };\n");

}