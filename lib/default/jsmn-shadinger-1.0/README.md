# JSMN lightweight JSON parser for Tasmota

Intro: this library uses the JSMN in-place JSON parser.
See https://github.com/zserge/jsmn and https://zserge.com/jsmn/

It is proposed as a replacement for ArduinoJson. It has less features, does only parsing but does it in a very efficient way.

## Benefits

First, the memory impact is very low: 4 bytes per token and no need to add an extra buffer for values.
Second, the code is much smaller than ArduinoJson by 5-7KB.

## How to use

`{"Device":"0x1234","Power":true,"Temperature":25.5}`

The JSON above is split into 8 tokens, and requires 32 bytes of dynamic memory.

- Token 0: `OBJECT`, size=3: `{"Device":"0x1234","Power":true,"Temperature":25.5}`
- Token 1: `KEY`: `Device`
- Token 2: `STRING`: `0x1234`
- Token 3: `KEY`: `Power`
- Token 4: `BOOL_TRUE`: `true`
- Token 5: `KEY`: `Temperature`
- Token 6: `FLOAT`: `25.5`
- Token 7: `INVALID`

If what you need is to parse a JSON Object for values with default values:
```
#include "JsonParser.h"

char json_buffer[] = "{\"Device\":\"0x1234\",\"Power\":true,\"Temperature\":25.6}";
JsonParser parser(json_buffer);
JsonParserObject root = parser.getRootObject();
if (!root) { ResponseCmndChar_P(PSTR("Invalid JSON")); return; }

uint16_t d = root.getUInt(PSTR("DEVICE"), 0xFFFF);   // case insensitive
bool     b = root.getBool(PSTR("Power"), false);
float    f = root.getFloat(PSTR("Temperature), -100);
```

Alternative pattern, if you want to test the existence of the attribute first:
```
#include "JsonParser.h"

char json_buffer[] = "{\"Device\":\"0x1234\",\"Power\":true,\"Temperature\":25.6}";
JsonParser parser(json_buffer);
JsonParserObject root = parser.getRootObject();
if (!root) { ResponseCmndChar_P(PSTR("Invalid JSON")); return; }

JsonParserToken val = root[PSTR("DEVICE")];
if (val) {
    d = val.getUInt();
}
val = root[PSTR("Power")];
if (val) {
    b = val.getBool();
}
val = root[PSTR("Temperature)];
if (val) {
    f = val.getFloat();
}
```

WARNING: never use the following form:
```
JsonParserObject root = JsonParser(json_buffer).getRootObject();
```
In this case, the `JsonParser` object is temporary and destroyed at the end of the expression. Setting the JsonParser to a local variable ensures that the lifetime of the object is extended to the end of the scope.

## Types and conversion

JSMN relies on the concept of JSON Tokens `JsonParserToken`. Tokens do not hold any data, but point to token boundaries in JSON string instead. Every jsmn token has a type, which indicates the type of corresponding JSON token. JSMN for Tasmota extends the type from JSMN to ease further parsing.

Types are:
- `INVALID` invalid token or end of stream, see Error Handling below
- `OBJECT` a JSON sub-object, `size()` contains the number of key/values of the object
- `ARRAY` a JSON array, `size()` contains the number of sub values
- `STRING` a JSON string, return the sub-string, unescaped, without surrounding quotes. UTF-8 is supported.
- `PRIMITIVE` an unrecognized JSON token, consider as an error
- `KEY` a JSON key in an object as a string
- `NULL` a JSON `null` value, automatically converted to `0` or `""`
- `BOOL_FALSE` a JSON `false` value, automatically converted to `0` or `""`
- `BOOL_TRUE` a JSON `true` value, automatically converted to `1` or `""`
- `UINT` a JSON unsigned int
- `INT` a JSON negative int
- `FLOAT` a JSON floating point value, i.e. a numerical value containing a decimal ot `.`

Note: values are converted in this priority: 1/ `UINT`, 2/ `INT`, 3/ `FLOAT`.

`JsonParserToken` support the following getters:
- `getBool()`: returns true if 'true' or non-zero int (default false)
- `getUInt()`: converts to unsigned int (default 0), boolean true is 1
- `getInt()`: converts to signed int (default 0), boolean true is 1
- `getULong()`: converts to unsigned 64 bits (default 0), boolean is 1
- `getStr()`: converts to string (default "")

There are variants of the same function for which you can choose the default values. Remember that using a getter if the token type is INVALID returns the default value.

Conversion to `OBJECT` or `ARRAY`:
- `getObject()`: converts token to `OBJECT` or `INVALID`
- `getArray()`: converts token to `ARRAY` or `INVALID`

For `JsonParserKey`:
- `getValue()`: returns the value token associated to the key

## Checking Token types

Type checking for `JsonParserToken`:
- `isSingleToken()` returns `true` for a single level token, `false` for `OBJECT`, `ARRAY` and `INVALID`
- `isKey()` returns `true` for a `KEY` within an `OBJECT`
- `isStr()` returns `true` for `STRING` (note: other types can still be read as strings with `getStr()`)
- `isNull()` returns `true` for `NULL`
- `isBool()` returns `true` for `BOOL_FALSE` or `BOOL_TRUE`
- `isUInt()` returns `true` for `UINT` (see below for number conversions)
- `isInt()` returns `true` for `INT` (see below for number conversions)
- `isFloat()` returns `true` for `FLOAT` (see below for number conversions)
- `isNum()` returns `true` for any numerical value, i.e. `UINT`, `INT` or `FLOAT`
- `isObject()` returns `true` for `OBJECT`
- `isArray()` returns `true` for `ARRAY`
- `isValid()` returns `true`for any type except `INVALID`

JSMN for Tasmota provides sub-classes:
- `JsonParserKey` of type `KEY` or `INVALID`, used as a return value for Object iterators
- `JsonParserObject` of type `OBJECT` or `INVALID`, providing iterators
- `JsonParserArray` of type `ARRAY` or `INVALID`, providing iterators

Converting from Token to Object or Array is done with `token.getObject()` or `token.getArray()`. If the conversion is invalid, the resulting object has type `INVALID` (see Error Handling).

## Iterators and accessors for Objects and Arrays

The `JsonParserObject` provides an easy to use iterator:
```
JsonParserToken obj = <...>
for (auto key : obj) {
    // key is of type JsonParserKey
    JsonParserToken valie = key.getValue();   // retrieve the value associated to key
}
```

If the object contains only one element, you can retrieve it with `obj.getFirstElement()`.

You can access any key with `obj["Key"]`. Note: the search is on purpose **case insensitive** as it is the norm in Tasmota. The search string can be in PROGMEM. If the token is not found, it is of type `INVALID`.

The `JsonParserArray` provides an easy to use iterator:
```
JsonParserArray arr = <...>
for (auto elt : arr) {
    // elt is of type JsonParserToken
}
```

You can access any element in the array with the `[]` operator. Ex: `arr[0]` fof first element. If the index is invalid, the token has type `INVALID`.

## Memory

The `JsonParserToken` fits in 32 bits, so it can be freely returned or copied without any penalty of object copying. Hence it doesn't need the `const` modifier either, since it is always passed by value.

## Error handling

This library uses a `zero error` pattern. This means that calls never report any error nor throw exceptions. If something wrong happens (bad JSON, token not found...), function return an **Invalid Token**. You can call any function on an Invalid Token, they will always return the same Invalid Token (aka fixed point).

You can easily test if the current token is invalid with the following:

Short version:
```
if (token) { /* token is valid */ }
```

Long version:
```
if (token->isValiid()) { /* token is valid */ }
```

This pattern allows to cascade calls and check only the final result:
```
char json_buffer[] = "<json>";
JsonParserObject json = JsonParser(json_buffer).getRootObject();
JsonParserToken zbstatus_tok = json["ZbStatus"];
JsonParserObject zbstatus = zbstatus_tok.getObject();
if (zbstatus) { /* do some stuff */
    // reaching this point means: JSON is valid, there is a root object, there is a `ZbStatus` key and it contains a sub-object
}
```

Warning: there is an explicit convert to `bool` to allow the short version. Be careful, `(bool)token` is equivalent to `token->isValid()`, it is **NOT** equivalent to `token->getBool()`.

## Limits

Please keep in mind the current limits for this library:
- Maximum JSON buffer size 2047 bytes
- Maximum 63 JSON tokens
- No support for exponent in floats (i.e. `1.0e3` is invalid)

These limits shouldn't be a problem since buffers in Tasmota are limited to 1.2KB. The support for exponent in floats is commented out and can be added if needed (slight increase in Flash size)