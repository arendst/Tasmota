
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

#ifndef _BASE64_MX_H_
#define	_BASE64_MX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/** @defgroup base64 Base64 Converter.
  * @{ */

/** Convert a binary memory block in a base64 null-terminated string.
  * @param dest Destination memory wher to put the base64 null-terminated string.
  * @param src Source binary memory block.
  * @param size Size in bytes of source binary memory block.
  * @return A pointer to the null character of the base64 null-terminated string. */
char* bintob64( char* dest, void const* src, size_t size );

/** Convert a base64 string to binary format.
  * @param dest Destination memory block.
  * @param src Source base64 string.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
void* b64tobin( void* dest, char const* src );

/** Convert a base64 string to binary format.
  * @param p Source base64 string and destination memory block.
  * @return If success a pointer to the next byte in memory block.
  *         Null if string has a bad format.  */
static inline void* b64decode( void* p ) {
    return b64tobin( p, (char*)p );
}


/** @ } */

#ifdef __cplusplus
}
#endif

#endif	/* _BASE64_H_ */
