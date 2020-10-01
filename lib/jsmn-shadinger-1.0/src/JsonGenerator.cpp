/*
  JsonGenerator.cpp - lightweight JSON parser

  Copyright (C) 2020  Stephan Hadinger

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

#include "JsonGenerator.h"

/*********************************************************************************************\
 * JSON Generator for Arrays
\*********************************************************************************************/
void JsonGeneratorArray::pre(void) {
  // remove trailing ']'
  val.remove(val.length()-1);
  if (val.length() > 1) {      // if not empty, prefix with comma
    val += ',';
  }
}

// void JsonGeneratorArray::post(void) {
//   val += ']';
// }

// Add signed int (32 bits)
void JsonGeneratorArray::add(int32_t uval32) {
  pre();
  val += uval32;
  post();
}

// Add unsigned int (32 bits)
void JsonGeneratorArray::add(uint32_t uval32) {
  pre();
  val += uval32;
  post();
}

// Add a raw string, that will not be escaped.
// Can be used to add a sub-array, sub-object or 'null', 'true', 'false' values
void JsonGeneratorArray::addStrRaw(const char * sval) {
  pre();
  val += sval;
  post();
}

// Add a JSON String (will be escaped)
void JsonGeneratorArray::addStr(const char * sval) {
  pre();
  val += '"';
  val += EscapeJSONString(sval).c_str();
  val += '"';
  post();
}

/*********************************************************************************************\
 * JSON Generator for Objects
\*********************************************************************************************/
void JsonGeneratorObject::pre(const char * key) {
  // remove trailing '}'
  val.remove(val.length()-1);
  if (val.length() > 1) {      // if not empty, prefix with comma
    val += ',';
  }
  val += '"';
  val += EscapeJSONString(key);
  val += '"';
  val += ':';
}

// void JsonGeneratorObject::post(void) {
//   val += '}';
// }

// Add signed int (32 bits)
void JsonGeneratorObject::add(const char* key, int32_t uval32) {
  pre(key);
  val += uval32;
  post();
}

// Add unsigned int (32 bits)
void JsonGeneratorObject::add(const char* key, uint32_t uval32) {
  pre(key);
  val += uval32;
  post();
}

void JsonGeneratorObject::add(const char* key, const String & str) {
  pre(key);
  val += '"';
  val += EscapeJSONString(str.c_str()).c_str();
  val += '"';
  post();
}

// Add a raw string, that will not be escaped.
// Can be used to add a sub-array, sub-object or 'null', 'true', 'false' values
void JsonGeneratorObject::addStrRaw(const char* key, const char * sval) {
  pre(key);
  val += sval;
  post();
}

// Add a JSON String (will be escaped)
void JsonGeneratorObject::addStr(const char* key, const char * sval) {
  pre(key);
  val += '"';
  val += EscapeJSONString(sval).c_str();
  val += '"';
  post();
}

/*********************************************************************************************\
 * JSON Generator for Arrays
\*********************************************************************************************/
// does the character needs to be escaped, and if so with which character
static char EscapeJSONChar(char c) {
  if ((c == '\"') || (c == '\\')) {
    return c;
  }
  if (c == '\n') { return 'n'; }
  if (c == '\t') { return 't'; }
  if (c == '\r') { return 'r'; }
  if (c == '\f') { return 'f'; }
  if (c == '\b') { return 'b'; }
  return 0;
}

String EscapeJSONString(const char *str) {
  // As this function is used in ResponseCmndChar() and ResponseCmndIdxChar()
  // it needs to be PROGMEM safe!
  String r("");
  if (nullptr == str) { return r; }

  bool needs_escape = false;
  size_t len_out = 1;
  const char* c = str;
  char ch = '.';
  while (ch != '\0') {
    ch = pgm_read_byte(c++);
    if (EscapeJSONChar(ch)) {
      len_out++;
      needs_escape = true;
    }
    len_out++;
  }

  if (needs_escape) {
    // we need to escape some chars
    // allocate target buffer
    r.reserve(len_out);
    c = str;
    char *d = r.begin();
    char ch = '.';
    while (ch != '\0') {
      ch = pgm_read_byte(c++);
      char c2 = EscapeJSONChar(ch);
      if (c2) {
        *d++ = '\\';
        *d++ = c2;
      } else {
        *d++ = ch;
      }
    }
    *d = 0;   // add NULL terminator
    r = (char*) r.begin();      // assign the buffer to the string
  } else {
    r = FPSTR(str);
  }

  return r;
}