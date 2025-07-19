#include "ITA2String.h"

#include <string.h>

ITA2String::ITA2String(char c) {
  asciiLen = 1;
  #if !RADIOLIB_STATIC_ONLY
  strAscii = new char[1];
  #endif
  strAscii[0] = c;
  ita2Len = 0;
}

ITA2String::ITA2String(const char* str) {
  asciiLen = strlen(str);
  #if !RADIOLIB_STATIC_ONLY
  strAscii = new char[asciiLen + 1];
  #endif
  strcpy(strAscii, str);
  ita2Len = 0;
}

ITA2String::~ITA2String() {
  #if !RADIOLIB_STATIC_ONLY
    delete[] strAscii;
  #endif
}

size_t ITA2String::length() {
  // length returned by this method is different than the length of ASCII-encoded strAscii
  // ITA2-encoded string length varies based on how many number and characters the string contains

  if(ita2Len == 0) {
    // ITA2 length wasn't calculated yet, call byteArr() to calculate it
    byteArr();
  }

  return(ita2Len);
}

uint8_t* ITA2String::byteArr() {
  // create temporary array 2x the string length (figures may be 3 bytes)
  #if RADIOLIB_STATIC_ONLY
    uint8_t temp[RADIOLIB_STATIC_ARRAY_SIZE*2 + 1];
  #else
    uint8_t* temp = new uint8_t[asciiLen*2 + 1];
  #endif

  size_t arrayLen = 0;
  bool flagFigure = false;
  for(size_t i = 0; i < asciiLen; i++) {
    uint16_t code = getBits(strAscii[i]);
    uint8_t shift = (code >> 5) & 0b11111;
    uint8_t character = code & 0b11111;
    // check if the code is letter or figure
    if(shift == RADIOLIB_ITA2_FIGS) {
      // check if this is the first figure in sequence
      if(!flagFigure) {
        flagFigure = true;
        temp[arrayLen++] = RADIOLIB_ITA2_FIGS;
      }

      // add the character code
      temp[arrayLen++] = character & 0b11111;

      // check the following character (skip for message end)
      if(i < (asciiLen - 1)) {
        uint16_t nextCode = getBits(strAscii[i+1]);
        uint8_t nextShift = (nextCode >> 5) & 0b11111;
        if(nextShift == RADIOLIB_ITA2_LTRS) {
          // next character is a letter, terminate figure shift
          temp[arrayLen++] = RADIOLIB_ITA2_LTRS;
          flagFigure = false;
        }
      } else {
        // reached the end of the message, terminate figure shift
        temp[arrayLen++] = RADIOLIB_ITA2_LTRS;
        flagFigure = false;
      }
    } else {
      temp[arrayLen++] = character & 0b11111;
    }
  }

  // save ITA2 string length
  ita2Len = arrayLen;

  uint8_t* arr = new uint8_t[arrayLen];
  memcpy(arr, temp, arrayLen);
  #if !RADIOLIB_STATIC_ONLY
    delete[] temp;
  #endif

  return(arr);
}

uint16_t ITA2String::getBits(char c) {
  // search ITA2 table
  uint16_t code = 0x0000;
  for(uint8_t i = 0; i < RADIOLIB_ITA2_LENGTH; i++) {
    if(RADIOLIB_NONVOLATILE_READ_BYTE(&ITA2Table[i][0]) == c) {
      // character is in letter shift
      code = (RADIOLIB_ITA2_LTRS << 5) | i;
      break;
    } else if(RADIOLIB_NONVOLATILE_READ_BYTE(&ITA2Table[i][1]) == c) {
      // character is in figures shift
      code = (RADIOLIB_ITA2_FIGS << 5) | i;
      break;
    }
  }

  return(code);
}
