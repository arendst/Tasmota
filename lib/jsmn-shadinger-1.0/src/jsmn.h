/*
 * MIT License
 *
 * Copyright (c) 2010 Serge Zaitsev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef JSMN_H
#define JSMN_H

#include <stddef.h>
#include <stdint.h>

// #ifdef JSMN_STATIC
// #define JSMN_API static
// #else
#define JSMN_API extern
// #endif

/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
  // end market
  JSMN_INVALID = 0,     // type == 0 is invalid
  JSMN_OBJECT = 1,
  JSMN_ARRAY = 2,
  JSMN_STRING = 3,
  JSMN_PRIMITIVE = 4,
  // new types created during post-processing
  JSMN_KEY = 5,         // JSMN_STRING with size 1
  JSMN_NULL = 6,        // JSMN_PRIMITIVE starting with 'n'
  JSMN_BOOL_FALSE = 7,  // JSMN_PRIMITIVE starting with 'f' or 'F'
  JSMN_BOOL_TRUE = 8,   // JSMN_PRIMITIVE starting with 't' or 'T'
  JSMN_FLOAT = 9,       // JSMN_PRIMITIVE starting with '.', '-', '0-9' and containing a '.'
  JSMN_INT = 10,        // JSMN_PRIMITIVE starting with '-', '0-9' and not containing a '.'
  JSMN_UINT = 11,       // JSMN_PRIMITIVE starting with '0-9' and not containing a '.'
} jsmntype_t;

enum jsmnerr {
  /* Not enough tokens were provided */
  JSMN_ERROR_NOMEM = -1,
  /* Invalid character inside JSON string */
  JSMN_ERROR_INVAL = -2,
  /* The string is not a full JSON packet, more bytes expected */
  JSMN_ERROR_PART = -3
};

/**
 * JSON token description.
 * type		type (object, array, string etc.)
 * start	start position in JSON data string
 * end		end position in JSON data string
 */
// size of bitfield, sum is 32
#define JSMN_TYPE_B    4
#define JSMN_SIZE_B    6    // max 63 items per level (ex: max 63 keys per object)
#define JSMN_START_B  11    // max 2KB input buffer
#define JSMN_LEN_B    11    // max 2KB per item

typedef struct jsmntok {
  jsmntype_t type : JSMN_TYPE_B;
  unsigned int size : JSMN_SIZE_B;
  unsigned int start : JSMN_START_B;
  unsigned int len : JSMN_LEN_B;
} jsmntok_t;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string.
 */
typedef struct jsmn_parser {
  unsigned int pos;     /* offset in the JSON string */
  unsigned int toknext; /* next token to allocate */
  int toksuper;         /* superior token node, e.g. parent object or array */
} jsmn_parser;

/**
 * Create JSON parser over an array of tokens
 */
JSMN_API void jsmn_init(jsmn_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each
 * describing
 * a single JSON object.
 */
JSMN_API int jsmn_parse(jsmn_parser *parser, const char *js, const size_t len,
                        jsmntok_t *tokens, const unsigned int num_tokens);

/**
 * 
 * In-place json unescape
 * 
 */
void json_unescape(char* string);

#endif /* JSMN_H */
