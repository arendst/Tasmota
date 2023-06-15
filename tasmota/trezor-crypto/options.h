/**
 * Copyright (c) 2013-2014 Pavol Rusnak
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

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#ifdef __cplusplus
extern "C"
{
#endif

// use precomputed Curve Points (some scalar multiples of curve base point G)
#ifndef USE_PRECOMPUTED_CP
#define USE_PRECOMPUTED_CP 1
#endif

// use fast inverse method
#ifndef USE_INVERSE_FAST
#define USE_INVERSE_FAST 1
#endif

// support for printing bignum256 structures via printf
#ifndef USE_BN_PRINT
#define USE_BN_PRINT 0
#endif

// use deterministic signatures
#ifndef USE_RFC6979
#define USE_RFC6979 1
#endif

// implement BIP32 caching
#ifndef USE_BIP32_CACHE
#define USE_BIP32_CACHE 1
#define BIP32_CACHE_SIZE 10
#define BIP32_CACHE_MAXDEPTH 8
#endif

// implement BIP39 caching
#ifndef USE_BIP39_CACHE
#define USE_BIP39_CACHE 1
#define BIP39_CACHE_SIZE 4
#endif

// support Ethereum operations
#ifndef USE_ETHEREUM
#define USE_ETHEREUM 0
#endif

// support Graphene operations (STEEM, BitShares)
#ifndef USE_GRAPHENE
#define USE_GRAPHENE 0
#endif

// support NEM operations
#ifndef USE_NEM
#define USE_NEM 0
#endif

// support Keccak hashing
#ifndef USE_KECCAK
#define USE_KECCAK 1
#endif

// add way how to mark confidential data
#ifndef CONFIDENTIAL
#define CONFIDENTIAL
#endif

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
