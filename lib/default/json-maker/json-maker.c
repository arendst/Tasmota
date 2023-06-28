
/*
<https://github.com/rafagafe/tiny-json>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <stddef.h> // For NULL
#include "json-maker.h"

/** Add a character at the end of a string.
  * @param dest Pointer to the null character of the string
  * @param ch Value to be added.
  * @return Pointer to the null character of the destination string. */
static char* chtoa( char* dest, char ch ) {
    *dest   = ch;
    *++dest = '\0';
    return dest;
}

/** Copy a null-terminated string.
  * @param dest Destination memory block.
  * @param src Source string.
  * @return Pointer to the null character of the destination string. */
static char* atoa( char* dest, char const* src ) {
    for( ; *src != '\0'; ++dest, ++src )
        *dest = *src;
    *dest = '\0';
    return dest;
}

/* Open a JSON object in a JSON string. */
char* json_objOpen( char* dest, char const* name ) {
    if ( NULL == name )
        dest = chtoa( dest, '{' );
    else {
        dest = chtoa( dest, '\"' );
        dest = atoa( dest, name );
        dest = atoa( dest, "\":{" );
    }
    return dest;
}

/* Close a JSON object in a JSON string. */
char* json_objClose( char* dest ) {
    if ( dest[-1] == ',' )
        --dest;
    return atoa( dest, "}," );
}

/* Open an array in a JSON string. */
char* json_arrOpen( char* dest, char const* name ) {
    if ( NULL == name )
        dest = chtoa( dest, '[' );
    else {
        dest = chtoa( dest, '\"' );
        dest = atoa( dest, name );
        dest = atoa( dest, "\":[" );
    }
    return dest;
}

/* Close an array in a JSON string. */
char* json_arrClose( char* dest ) {
    if ( dest[-1] == ',' )
        --dest;
    return atoa( dest, "]," );
}

/** Add the name of a text property.
  * @param dest Destination memory.
  * @param name The name of the property.
  * @return Pointer to the next char. */
static char* strname( char* dest, char const* name ) {
    dest = chtoa( dest, '\"' );
    if ( NULL != name ) {
        dest = atoa( dest, name );
        dest = atoa( dest, "\":\"" );
    }
    return dest;
}

/** Get the hexadecimal digit of the least significant nibble of a integer. */
static int nibbletoch( int nibble ) {
    return "0123456789ABCDEF"[ nibble % 16u ];
}

/** Get the escape character of a non-printable.
  * @param ch Character source.
  * @return The escape character or null character if error. */
static int escape( int ch ) {
    int i;
    static struct { char code; char ch; } const pair[] = {
        { '\"', '\"' }, { '\\', '\\' }, { '/',  '/'  }, { 'b',  '\b' },
        { 'f',  '\f' }, { 'n',  '\n' }, { 'r',  '\r' }, { 't',  '\t' },
    };
    for( i = 0; i < sizeof pair / sizeof *pair; ++i )
        if ( ch == pair[i].ch )
            return pair[i].code;
    return '\0';
}

/** Copy a null-terminated string inserting escape characters if needed.
  * @param dest Destination memory block.
  * @param src Source string.
  * @param len Max length of source. < 0 for unlimit.
  * @return Pointer to the null character of the destination string. */
static char* atoesc( char* dest, char const* src, int len ) {
    int i;
    for( i = 0; src[i] != '\0' && ( i < len || 0 > len ); ++dest, ++i ) {
        if ( src[i] >= ' ' && src[i] != '\"' && src[i] != '\\' && src[i] != '/' )
            *dest = src[i];
        else {
            *dest++ = '\\';
            int const esc = escape( src[i] );
            if ( esc )
                *dest = esc;
            else {
                *dest++ = 'u';
                *dest++ = '0';
                *dest++ = '0';
                *dest++ = nibbletoch( src[i] / 16 );
                *dest++ = nibbletoch( src[i] );
            }
        }
    }
    *dest = '\0';
    return dest;
}

/* Add a text property in a JSON string. */
char* json_nstr( char* dest, char const* name, char const* value, int len ) {
    dest = strname( dest, name );
    dest = atoesc( dest, value, len );
    dest = atoa( dest, "\"," );
    return dest;
}

/** Add the name of a primitive property.
  * @param dest Destination memory.
  * @param name The name of the property.
  * @return Pointer to the next char. */
static char* primitivename( char* dest, char const* name ) {
    if( NULL == name )
        return dest;
    dest = chtoa( dest, '\"' );
    dest = atoa( dest, name );
    dest = atoa( dest, "\":" );
    return dest;
}

/*  Add a boolean property in a JSON string. */
char* json_bool( char* dest, char const* name, int value ) {
    dest = primitivename( dest, name );
    dest = atoa( dest, value ? "true," : "false," );
    return dest;
}

/* Add a null property in a JSON string. */
char* json_null( char* dest, char const* name ) {
    dest = primitivename( dest, name );
    dest = atoa( dest, "null," );
    return dest;
}

/* Used to finish the root JSON object. After call json_objClose(). */
char* json_end( char* dest ) {
    if ( ',' == dest[-1] ) {
        dest[-1] = '\0';
        --dest;
    }
    return dest;
}

#ifdef NO_SPRINTF

static char* format( char* dest, int len, int isnegative ) {
    if ( isnegative )
        dest[ len++ ] = '-';
    dest[ len ] = '\0';
    int head = 0;
    int tail = len - 1;
    while( head < tail ) {
        char tmp = dest[ head ];
        dest[ head ] = dest[ tail ];
        dest[ tail ] = tmp;
        ++head;
        --tail;
    }
    return dest + len;
}

#define numtoa( func, type, utype )         \
static char* func( char* dest, type val ) { \
    enum { base = 10 };                     \
    if ( 0 == val )                         \
        return chtoa( dest, '0' );          \
    int const isnegative = 0 > val;         \
    utype num = isnegative ? -val : val;    \
    int len = 0;                            \
    while( 0 != num ) {                     \
        int rem = num % base;               \
        dest[ len++ ] = rem + '0';          \
        num = num / base;                   \
    }                                       \
    return format( dest, len, isnegative ); \
}                                           \

#define json_num( func, func2, type )                       \
char* func( char* dest, char const* name, type value ) {    \
    dest = primitivename( dest, name );                     \
    dest = func2( dest, value );                            \
    dest = chtoa( dest, ',' );                              \
    return dest;                                            \
}                                                           \

#define ALL_TYPES \
    X( int,      int,          unsigned int        ) \
    X( long,     long,         unsigned long       ) \
    X( uint,     unsigned int, unsigned int        ) \
    X( ulong,    unsigned      long, unsigned long ) \
    X( verylong, long long,    unsigned long long  ) \

#define X( name, type, utype ) numtoa( name##toa, type, utype )
ALL_TYPES
#undef X

#define X( name, type, utype ) json_num( json_##name, name##toa, type )
ALL_TYPES
#undef X

char* json_double( char* dest, char const* name, double value ) {
    return json_verylong( dest, name, value );
}

#else

#include <stdio.h>

#define ALL_TYPES \
    X( json_int,      int,           "%d"   ) \
    X( json_long,     long,          "%ld"  ) \
    X( json_uint,     unsigned int,  "%u"   ) \
    X( json_ulong,    unsigned long, "%lu"  ) \
    X( json_verylong, long long,     "%lld" ) \
    X( json_double,   double,        "%g"   ) \


#define json_num( funcname, type, fmt )                         \
char* funcname( char* dest, char const* name, type value ) {    \
    dest = primitivename( dest, name );                         \
    dest += sprintf( dest, fmt, value );                        \
    dest = chtoa( dest, ',' );                                  \
    return dest;                                                \
}

#define X( name, type, fmt ) json_num( name, type, fmt )
ALL_TYPES
#undef X


#endif
