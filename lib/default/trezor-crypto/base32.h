/**
 * Copyright (c) 2017 Saleem Rashid
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __BASE32_H__
#define __BASE32_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const char *BASE32_ALPHABET_RFC4648;

char *base32_encode(const uint8_t *in, size_t inlen, char *out, size_t outlen, const char *alphabet);
void base32_encode_unsafe(const uint8_t *in, size_t inlen, uint8_t *out);

uint8_t *base32_decode(const char *in, size_t inlen, uint8_t *out, size_t outlen, const char *alphabet);
bool base32_decode_unsafe(const uint8_t *in, size_t inlen, uint8_t *out, const char *alphabet);

size_t base32_encoded_length(size_t inlen);
size_t base32_decoded_length(size_t inlen);
    
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
