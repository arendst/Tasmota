# tiny-json

[![Build Status](https://travis-ci.org/rafagafe/tiny-json.svg?branch=master)](https://travis-ci.org/rafagafe/tiny-json) [![GitHub contributors](https://img.shields.io/github/contributors/rafagafe/tiny-json.svg)](https://github.com/rafagafe/tiny-json/graphs/contributors)

tiny-json is a versatile and easy to use json parser in C suitable for embedded systems. It is fast, robust and portable.

It is not only a tokenizer. You can get data in string format or get the primitives values in C type variables without performance loss.

You can get the JSON fields one on one. Or get their values by their names. This helps you to save a lot of source code lines and development time.

* It does not use recursivity.
* It does not use dynamic memory. The memory you use can be reserved statically.
* It has not limits in nested level in arrays or json objects.
* The JSON property number limit is determined by the size of a buffer that can be statically reserved.

If you need create JSON strings please visit: https://github.com/rafagafe/json-maker

# Philosophy

When parsing a JSON text string a tree is created by linking json_t structures. Navigating or querying this tree is very easy using the API functions.

To maintain reduced memory usage and fast processing the strings are not copied. When you request the value of a JSON element, a reference is returned within the original string with the JSON.

To facilitate the processing of the data the returned strings are null-terminated strings. This is achieved by setting the null character to JSON control characters such as commas, brackets, braces, and quotation marks.

# API
Two types are defined in tiny-json API. One is jsonType_t. It is an enumeration with the types of JSON fields. And the other is json_t. It is a structure that you don't need know its content.
```C
typedef enum {
    JSON_OBJ, JSON_ARRAY, JSON_TEXT, JSON_BOOLEAN,
    JSON_INTEGER, JSON_REAL, JSON_NULL
} jsonType_t;
```
To parse a JSON string we use the function json_create(). We pass it an array of json_t for it can allocate JSON fields.
If the JSON string is bad format or has more fields than the array it returns a null pointer.
```C
enum { MAX_FIELDS = 4 };
json_t pool[ MAX_FIELDS ];

char str[] = "{ \"name\": \"peter\", \"age\": 32 }";	

json_t const* parent = json_create( str, pool, MAX_FIELDS );
if ( parent == NULL ) return EXIT_FAILURE;
```
To get a field by its name we use the function json_getProperty. If the field does not exist it returns a null pointer.
And to get the type of a field we use the function json_getType ().
```C
json_t const* namefield = json_getProperty( parent, "name" );
if ( namefield == NULL ) return EXIT_FAILURE;
if ( json_getType( namefield ) != JSON_TEXT ) return EXIT_FAILURE;
```
To get the value of a filed in string format we use the function json_getValue(). It always returns a valid null-teminated string.
```C
char const* namevalue = json_getValue( namefield );
printf( "%s%s%s", "Name: '", namevalue, "'.\n" );
```
In primitive type fileds we can use a especific funtion to get its value in a C type like json_getInteger() or still use json_getValue() to get its value in text format.
```C
json_t const* agefield = json_getProperty( parent, "age" );
if ( agefield == NULL ) return EXIT_FAILURE;
if ( json_getType( agefield ) != JSON_INTEGER ) return EXIT_FAILURE;

int64_t agevalue = json_getInteger( agefield );
printf( "%s%lld%s", "Age: '", agevalue, "'.\n" );

char const* agetxt = json_getValue( agefield );
printf( "%s%s%s", "Age: '", agetxt, "'.\n" );
```
To see nested JSON objects and arrays please read example-01.c.
