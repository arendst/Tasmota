/*
  support_json.ino - JSON support functions

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

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

/*********************************************************************************************\
 * JSON parsing
\*********************************************************************************************/

// does the character needs to be escaped, and if so with which character
char EscapeJSONChar(char c) {
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
  String r("");
  if (nullptr == str) { return r; }

  bool needs_escape = false;
  size_t len_out = 1;
  const char * c = str;

  while (*c) {
    if (EscapeJSONChar(*c)) {
      len_out++;
      needs_escape = true;
    }
    c++;
    len_out++;
  }

  if (needs_escape) {
    // we need to escape some chars
    // allocate target buffer
    r.reserve(len_out);
    c = str;
    char *d = r.begin();
    while (*c) {
      char c2 = EscapeJSONChar(*c);
      if (c2) {
        c++;
        *d++ = '\\';
        *d++ = c2;
      } else {
        *d++ = *c++;
      }
    }
    *d = 0;   // add NULL terminator
    r = (char*) r.begin();      // assign the buffer to the string
  } else {
    r = str;
  }

  return r;
}

/*********************************************************************************************\
 * Find key - case insensitive
\*********************************************************************************************/

// Given a JsonObject, finds the value as JsonVariant for the key needle.
// The search is case-insensitive, and will find the first match in the order of keys in JSON
//
// If the key is not found, returns a nullptr
// Input: needle cannot be NULL but may be PROGMEM
const JsonVariant &GetCaseInsensitive(const JsonObject &json, const char *needle) {
  // key can be in PROGMEM
  // if needle == "?" then we return the first valid key
  bool wildcard = strcmp_P("?", needle) == 0;
  if ((nullptr == &json) || (nullptr == needle) || (0 == pgm_read_byte(needle))) {
    return *(JsonVariant*)nullptr;
  }

  for (JsonObject::const_iterator it=json.begin(); it!=json.end(); ++it) {
    const char *key = it->key;
    const JsonVariant &value = it->value;

    if (wildcard || (0 == strcasecmp_P(key, needle))) {
      return value;
    }
  }
  // if not found
  return *(JsonVariant*)nullptr;
}
