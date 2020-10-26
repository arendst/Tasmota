/*
  JsonParser.h - lightweight JSON parser

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

#ifndef __JSON_PARSER__
#define __JSON_PARSER__

#include "jsmn.h"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

// #define strcmp_P(x, y) strcmp(x,y)
// #define strcasecmp_P(x,y) strcasecmp(x,y)
// #define pgm_read_byte(x)  (*(uint8_t*)(x))
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/*********************************************************************************************\
 * Utilities
\*********************************************************************************************/

// The code uses a zero-error approach. Functions never return an error code nor an exception.
// In case an operation fails, it returns an "Invalid Token".
// To know if a token is valid, use the `isValid()` method or just use it in an if statement.
//
// Internally, the bad token is a pointer to a constant token of type JSMN_INVALID
// fall-back token object when parsing failed
const extern jsmntok_t token_bad;

// To reduce code size, the current buffer is stored in a global variable.
// This prevents all calls to add this parameter on the stack and reduces code size.
// The caveat is that code is not re-entrant.
// If you ever intermix two or more JSON parsers, use `parser->setCurrent()` before further calls
//
// the current json buffer being used, for convenience
// Warning: this makes code non-reentrant.
extern const char * k_current_json_buffer;

/*********************************************************************************************\
 * Read-only JSON token object, fits in 32 bits
\*********************************************************************************************/
// forward class declarations
class JsonParserObject;
class JsonParserArray;

class JsonParserToken {
public:

  // constructor
  // If parameter is null, we use the Invalid Token instead.
  JsonParserToken(const jsmntok_t * token) : t(token) {
    if (nullptr == t) { t = &token_bad; }
  }
  JsonParserToken() : t(&token_bad) { }
  // no explicit destructor (not needed)

  inline bool isValid(void) const   { return t->type != JSMN_INVALID; }
  inline size_t size(void) const    { return t->size; }

  inline bool isSingleToken(void) const { return (t->type >= JSMN_STRING); }
  inline bool isKey(void) const     { return (t->type == JSMN_KEY); }
  inline bool isStr(void) const     { return (t->type == JSMN_STRING); }
  inline bool isNull(void) const    { return (t->type == JSMN_NULL); }
  inline bool isBool(void) const    { return (t->type == JSMN_BOOL_TRUE) || (t->type == JSMN_BOOL_FALSE); }
  inline bool isFloat(void) const   { return (t->type == JSMN_FLOAT); }
  inline bool isInt(void) const     { return (t->type == JSMN_INT); }
  inline bool isUint(void) const    { return (t->type == JSMN_UINT); }
  inline bool isNum(void) const     { return (t->type >= JSMN_FLOAT) && (t->type <= JSMN_UINT); }
  inline bool isObject(void) const  { return (t->type == JSMN_OBJECT); }
  inline bool isArray(void) const   { return (t->type == JSMN_ARRAY); }

  // move to token immediately after in the buffer
  void nextOne(void) { if (t->type != JSMN_INVALID) { t++; } }

  // conversion operators
  // Warning - bool does not test for Boolean value but for validity, i.e. equivalent to token.valid()
  inline explicit operator bool() const { return t->type != JSMN_INVALID; };

  // all the following conversion will try to give a meaninful value
  // if the content is not of the right type or the token is invalid, returns the 'default'
  bool getBool(void) const;         // true if 'true' or non-zero int (default false)
  int32_t getInt(void) const;       // convert to int (default 0)
  uint32_t getUInt(void) const;     // convert to unsigned int (default 0)
  uint64_t getULong(void) const;    // convert to unsigned 64 bits (default 0)
  float getFloat(void) const;       // convert to float (default 0), does not support exponent
  const char * getStr(void) const;  // convert to string (default "")

  // same as above, but you can choose the default value
  bool getBool(bool val) const;
  int32_t getInt(int32_t val) const;
  uint32_t getUInt(uint32_t val) const;
  uint64_t getULong(uint64_t val) const;
  float getFloat(float val) const;
  const char * getStr(const char * val) const;

  // convert to JsonParserObject or JsonParserArray, or Invalid Token if not allowed
  JsonParserObject getObject(void) const;
  JsonParserArray getArray(void) const;

public:
  // the following should be 'protected' but then it can't be accessed by iterators
  const jsmntok_t   * t;
  // skip the next Token as a whole (i.e. skip an entire array)
  void skipToken(void);

protected:

  // skip the next token knowing it's an array
  void skipArray(void);

  // skip the next token knowing it's an object
  void skipObject(void);
};

/*********************************************************************************************\
 * Subclass for Key
\*********************************************************************************************/
class JsonParserKey : public JsonParserToken {
public:
  JsonParserKey(const jsmntok_t * token);
  explicit JsonParserKey(const JsonParserToken token);

  // get the value token associated to the key
  JsonParserToken getValue(void) const;
};

/*********************************************************************************************\
 * Subclass for Object
\*********************************************************************************************/
class JsonParserObject : public JsonParserToken {
public:
  JsonParserObject(const jsmntok_t * token);
  JsonParserObject(const JsonParserToken token);
  JsonParserObject() : JsonParserToken() { }

  // find key with name, case-insensitive, '?' matches any key. Returns Invalid Token if not found
  JsonParserToken operator[](const char * needle) const;
  JsonParserToken operator[](const String & needle) const;
  // find a key starting with `needle`, case insensitive
  JsonParserToken findStartsWith(const char * needle) const;
  // find a key, case-insensitive, return nullptr if not found (instead of "")
  const char * findConstCharNull(const char * needle) const;

  // all-in-one methods: search for key (case insensitive), convert value and set default
  int32_t getInt(const char *, int32_t) const;
  uint32_t getUInt(const char *, uint32_t) const;
  uint64_t getULong(const char *, uint64_t) const;
  float getFloat(const char *, float) const;
  const char * getStr(const char *, const char *) const;
  const char * getStr(const char *) const;

  // get first element (key)
  JsonParserKey getFirstElement(void) const;

  //
  // const iterator
  //
  class const_iterator {
    public:
      const_iterator(const JsonParserObject t);
      const_iterator operator++();
      bool operator!=(const_iterator & other) const { return tok.t != other.tok.t; }
      const JsonParserKey operator*() const { return JsonParserKey(tok); }
    private:
      JsonParserToken tok;
      size_t remaining;
    };
  const_iterator begin() const { return const_iterator(*this); }      // start with 'head'
  const_iterator end() const { return const_iterator(JsonParserObject(&token_bad)); }        // end with null pointer
};

/*********************************************************************************************\
 * Subclass for Array
\*********************************************************************************************/
class JsonParserArray : public JsonParserToken {
public:
  JsonParserArray(const jsmntok_t * token);
  JsonParserArray(const JsonParserToken token);
  JsonParserArray() : JsonParserToken() { }

  // get the element if index `i` from 0 to `size() - 1`
  JsonParserToken operator[](int32_t i) const;

  //
  // const iterator
  //
  class const_iterator {
    public:
      const_iterator(const JsonParserArray t);
      const_iterator operator++();
      bool operator!=(const_iterator & other) const { return tok.t != other.tok.t; }
      const JsonParserToken operator*() const { return tok; }
    private:
      JsonParserToken tok;
      size_t remaining;
    };
  const_iterator begin() const { return const_iterator(*this); }      // start with 'head'
  const_iterator end() const { return const_iterator(JsonParserArray(&token_bad)); }        // end with null pointer
};

/*********************************************************************************************\
 * JSON Parser
\*********************************************************************************************/

class JsonParser {
public:
  // constructor, parse the json buffer
  // Warning: the buffer is modified in the process (in-place parsing)
  // Input: `json_in` can be nullptr, but CANNOT be in PROGMEM (remember we need to change characters in-place)
  JsonParser(char * json_in);

  // destructor
  ~JsonParser();
  
  // set the current buffer for attribute access (i.e. set the global)
  void setCurrent(void) { k_current_json_buffer = _json; }

  // test if the parsing was successful
  inline explicit operator bool() const { return _token_len > 0; }

  const JsonParserToken getRoot(void) { return JsonParserToken(&_tokens[0]); }
  // const JsonParserObject getRootObject(void) { return JsonParserObject(&_tokens[0]); }
  const JsonParserObject getRootObject(void) const;

  // pointer arithmetic
  // ptrdiff_t index(JsonParserToken token) const;

protected:
  uint16_t    _size;          // size of tokens buffer
  int16_t    _token_len;      // how many tokens have been parsed
  jsmntok_t * _tokens;        // pointer to token buffer
  jsmn_parser _parser;        // jmsn_parser structure
  char      * _json;          // json buffer

  // disallocate token buffer
  void free(void);

  // allocate token buffer of size _size
  void allocate(void);

  // access tokens by index
  const JsonParserToken operator[](int32_t i) const;
  // parse
  void parse(char * json_in);
  // post-process parsing: insert NULL chars to split strings, compute a more precise token type
  void postProcess(size_t json_len);
};

#endif // __JSON_PARSER__