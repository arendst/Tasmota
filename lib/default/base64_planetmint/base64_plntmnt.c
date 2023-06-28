
/*
<https://github.com/rafagafe/base64>
     
  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
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

#include "base64_plntmnt.h"

/** Escape values. */
enum special_e {
    notabase64 = 64, /**< Value to return when a non base64 digit is found. */
    terminator = 65, /**< Value to return when the character '=' is found.  */
};

/** Lookup table that converts a base64 digit to integer. */
static char const digittobin[] = {
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 65, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};


/* Convert a base64 null-terminated string to binary format.*/
void* b64tobin( void* dest, char const* src ) {
    unsigned char const* s = (unsigned char*)src;
    char* p = dest;
    for(;;) {

        int const a = digittobin[ *s ];
        if ( a == notabase64 ) return p;
        if ( a == terminator ) return p;

        int const b = digittobin[ *++s ];
        if ( b == notabase64 ) return 0;
        if ( b == terminator ) return 0;

        *p++ = ( a << 2u ) | ( b >> 4u );

        int const c = digittobin[ *++s ];
        if ( c == notabase64 ) return 0;

        int const d = digittobin[ *++s ];
        if ( d == notabase64 ) return 0;
        if ( c == terminator ) {
            if ( d != terminator ) return 0;
            return p;
        }

        *p++ = ( b << 4u ) | ( c >> 2u );

        if ( d == terminator ) return p;

        *p++ = ( c << 6u ) | ( d >> 0u );
        ++s;
    }
    
    return p;
}

/** Lookup table that converts a integer to base64 digit. */
static char const bintodigit[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789"
                                 "+/";

/** Get the first base 64 digit of a block of 4.
  * @param a The first byte of the source block of 3.
  * @return A base 64 digit. */
static int get0( int a ) {
    int const index = a >> 2u;
    return bintodigit[ index ];
}

/** Get the second base 64 digit of a block of 4.
  * @param a The first byte of the source block of 3.
  * @param b The second byte of the source block of 3.
  * @return A base 64 digit. */
static int get1( int a, int b ) {
    int const indexA = ( a & 3 ) << 4u;
    int const indexB = b >> 4u;
    int const index  = indexA | indexB;
    return bintodigit[ index ];
}

/** Get the third base 64 digit of a block of 4.
  * @param b The second byte of the source block of 3.
  * @param c The third byte of the source block of 3.
  * @return A base 64 digit. */
static unsigned int get2( unsigned int b, unsigned int c ) {
    int const indexB = ( b & 15 ) << 2u;
    int const indexC = c >> 6u;
    int const index  = indexB | indexC;
    return bintodigit[ index ];
}

/** Get the fourth base 64 digit of a block of 4.
  * @param c The third byte of the source block of 3.
  * @return A base 64 digit. */
static int get3( int c ) {
    int const index = c & 0x3f;
    return bintodigit[ index ];
}

/* Convert a binary memory block in a base64 null-terminated string. */
char* bintob64( char* dest, void const* src, size_t size ) {

    typedef struct { unsigned char a; unsigned char b; unsigned char c; } block_t;
    block_t const* block = (block_t*)src;
    for( ; size >= sizeof( block_t ); size -= sizeof( block_t ), ++block ) {
        *dest++ = get0( block->a );
        *dest++ = get1( block->a, block->b );
        *dest++ = get2( block->b, block->c );
        *dest++ = get3( block->c );
    }

    if ( !size ) goto final;

    *dest++ = get0( block->a );
    if ( !--size ) {
        *dest++ = get1( block->a, 0 );
        *dest++ = '=';
        *dest++ = '=';
        goto final;
    }

    *dest++ = get1( block->a, block->b );
    *dest++ = get2( block->b, 0 );
    *dest++ = '=';

  final:
    *dest = '\0';
    return dest;
}
