/*
  JsonGenerator.h - lightweight JSON generator

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

#ifndef __JSON_GENERATOR__
#define __JSON_GENERATOR__

#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

extern String EscapeJSONString(const char *str);

/*********************************************************************************************\
 * JSON Generator for Arrays
\*********************************************************************************************/
class JsonGeneratorArray {
public:

  JsonGeneratorArray(): val("[]") {}     // start with empty array

  void add(uint32_t uval32);
  void add(int32_t uval32);
  void addStrRaw(const char * sval);
  void addStr(const char * sval);

  inline String &toString(void) { return val; }

protected:
  void pre(void);
  void post(void) { val += ']'; }
  String val;
};

/*********************************************************************************************\
 * JSON Generator for Objects
\*********************************************************************************************/
class JsonGeneratorObject {
public:

  JsonGeneratorObject(): val("{}") {}     // start with empty object

  void add(const char* key, uint32_t uval32);
  void add(const char* key, int32_t uval32);
  void add(const char* key, const String & str);
  void addStrRaw(const char* key, const char * sval);
  void addStr(const char* key, const char * sval);

  inline String &toString(void) { return val; }

protected:
  void pre(const char * key);
  void post(void) { val += '}'; }
  String val;
};

#endif // __JSON_PARSER__