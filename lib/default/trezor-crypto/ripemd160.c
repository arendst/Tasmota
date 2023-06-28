/*
 *  RIPE MD-160 implementation
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

/*
 *  The RIPEMD-160 algorithm was designed by RIPE in 1996
 *  http://homes.esat.kuleuven.be/~bosselae/ripemd160.html
 *  http://ehash.iaik.tugraz.at/wiki/RIPEMD-160
 */

#include <string.h>

#include "ripemd160.h"
#include "macros.h"

/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_UINT32_LE
#define GET_UINT32_LE(n,b,i)                            \
{                                                       \
    (n) = ( (uint32_t) (b)[(i)    ]       )             \
        | ( (uint32_t) (b)[(i) + 1] <<  8 )             \
        | ( (uint32_t) (b)[(i) + 2] << 16 )             \
        | ( (uint32_t) (b)[(i) + 3] << 24 );            \
}
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                                    \
{                                                               \
    (b)[(i)    ] = (uint8_t) ( ( (n)       ) & 0xFF );    \
    (b)[(i) + 1] = (uint8_t) ( ( (n) >>  8 ) & 0xFF );    \
    (b)[(i) + 2] = (uint8_t) ( ( (n) >> 16 ) & 0xFF );    \
    (b)[(i) + 3] = (uint8_t) ( ( (n) >> 24 ) & 0xFF );    \
}
#endif

/*
 * RIPEMD-160 context setup
 */
void ripemd160_Init(RIPEMD160_CTX *ctx)
{
    memset(ctx, 0, sizeof(RIPEMD160_CTX));
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
}

#if !defined(MBEDTLS_RIPEMD160_PROCESS_ALT)
/*
 * Process one block
 */
void ripemd160_process( RIPEMD160_CTX *ctx, const uint8_t data[RIPEMD160_BLOCK_LENGTH] )
{
    uint32_t A, B, C, D, E, Ap, Bp, Cp, Dp, Ep, X[16];

    GET_UINT32_LE( X[ 0], data,  0 );
    GET_UINT32_LE( X[ 1], data,  4 );
    GET_UINT32_LE( X[ 2], data,  8 );
    GET_UINT32_LE( X[ 3], data, 12 );
    GET_UINT32_LE( X[ 4], data, 16 );
    GET_UINT32_LE( X[ 5], data, 20 );
    GET_UINT32_LE( X[ 6], data, 24 );
    GET_UINT32_LE( X[ 7], data, 28 );
    GET_UINT32_LE( X[ 8], data, 32 );
    GET_UINT32_LE( X[ 9], data, 36 );
    GET_UINT32_LE( X[10], data, 40 );
    GET_UINT32_LE( X[11], data, 44 );
    GET_UINT32_LE( X[12], data, 48 );
    GET_UINT32_LE( X[13], data, 52 );
    GET_UINT32_LE( X[14], data, 56 );
    GET_UINT32_LE( X[15], data, 60 );

    A = Ap = ctx->state[0];
    B = Bp = ctx->state[1];
    C = Cp = ctx->state[2];
    D = Dp = ctx->state[3];
    E = Ep = ctx->state[4];

#define F1( x, y, z )   ( x ^ y ^ z )
#define F2( x, y, z )   ( ( x & y ) | ( ~x & z ) )
#define F3( x, y, z )   ( ( x | ~y ) ^ z )
#define F4( x, y, z )   ( ( x & z ) | ( y & ~z ) )
#define F5( x, y, z )   ( x ^ ( y | ~z ) )

#define S( x, n ) ( ( x << n ) | ( x >> (32 - n) ) )

#define P( a, b, c, d, e, r, s, f, k )      \
    a += f( b, c, d ) + X[r] + k;           \
    a = S( a, s ) + e;                      \
    c = S( c, 10 );

#define P2( a, b, c, d, e, r, s, rp, sp )   \
    P( a, b, c, d, e, r, s, F, K );         \
    P( a ## p, b ## p, c ## p, d ## p, e ## p, rp, sp, Fp, Kp );

#define F   F1
#define K   0x00000000
#define Fp  F5
#define Kp  0x50A28BE6
    P2( A, B, C, D, E,  0, 11,  5,  8 );
    P2( E, A, B, C, D,  1, 14, 14,  9 );
    P2( D, E, A, B, C,  2, 15,  7,  9 );
    P2( C, D, E, A, B,  3, 12,  0, 11 );
    P2( B, C, D, E, A,  4,  5,  9, 13 );
    P2( A, B, C, D, E,  5,  8,  2, 15 );
    P2( E, A, B, C, D,  6,  7, 11, 15 );
    P2( D, E, A, B, C,  7,  9,  4,  5 );
    P2( C, D, E, A, B,  8, 11, 13,  7 );
    P2( B, C, D, E, A,  9, 13,  6,  7 );
    P2( A, B, C, D, E, 10, 14, 15,  8 );
    P2( E, A, B, C, D, 11, 15,  8, 11 );
    P2( D, E, A, B, C, 12,  6,  1, 14 );
    P2( C, D, E, A, B, 13,  7, 10, 14 );
    P2( B, C, D, E, A, 14,  9,  3, 12 );
    P2( A, B, C, D, E, 15,  8, 12,  6 );
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F2
#define K   0x5A827999
#define Fp  F4
#define Kp  0x5C4DD124
    P2( E, A, B, C, D,  7,  7,  6,  9 );
    P2( D, E, A, B, C,  4,  6, 11, 13 );
    P2( C, D, E, A, B, 13,  8,  3, 15 );
    P2( B, C, D, E, A,  1, 13,  7,  7 );
    P2( A, B, C, D, E, 10, 11,  0, 12 );
    P2( E, A, B, C, D,  6,  9, 13,  8 );
    P2( D, E, A, B, C, 15,  7,  5,  9 );
    P2( C, D, E, A, B,  3, 15, 10, 11 );
    P2( B, C, D, E, A, 12,  7, 14,  7 );
    P2( A, B, C, D, E,  0, 12, 15,  7 );
    P2( E, A, B, C, D,  9, 15,  8, 12 );
    P2( D, E, A, B, C,  5,  9, 12,  7 );
    P2( C, D, E, A, B,  2, 11,  4,  6 );
    P2( B, C, D, E, A, 14,  7,  9, 15 );
    P2( A, B, C, D, E, 11, 13,  1, 13 );
    P2( E, A, B, C, D,  8, 12,  2, 11 );
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F3
#define K   0x6ED9EBA1
#define Fp  F3
#define Kp  0x6D703EF3
    P2( D, E, A, B, C,  3, 11, 15,  9 );
    P2( C, D, E, A, B, 10, 13,  5,  7 );
    P2( B, C, D, E, A, 14,  6,  1, 15 );
    P2( A, B, C, D, E,  4,  7,  3, 11 );
    P2( E, A, B, C, D,  9, 14,  7,  8 );
    P2( D, E, A, B, C, 15,  9, 14,  6 );
    P2( C, D, E, A, B,  8, 13,  6,  6 );
    P2( B, C, D, E, A,  1, 15,  9, 14 );
    P2( A, B, C, D, E,  2, 14, 11, 12 );
    P2( E, A, B, C, D,  7,  8,  8, 13 );
    P2( D, E, A, B, C,  0, 13, 12,  5 );
    P2( C, D, E, A, B,  6,  6,  2, 14 );
    P2( B, C, D, E, A, 13,  5, 10, 13 );
    P2( A, B, C, D, E, 11, 12,  0, 13 );
    P2( E, A, B, C, D,  5,  7,  4,  7 );
    P2( D, E, A, B, C, 12,  5, 13,  5 );
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F4
#define K   0x8F1BBCDC
#define Fp  F2
#define Kp  0x7A6D76E9
    P2( C, D, E, A, B,  1, 11,  8, 15 );
    P2( B, C, D, E, A,  9, 12,  6,  5 );
    P2( A, B, C, D, E, 11, 14,  4,  8 );
    P2( E, A, B, C, D, 10, 15,  1, 11 );
    P2( D, E, A, B, C,  0, 14,  3, 14 );
    P2( C, D, E, A, B,  8, 15, 11, 14 );
    P2( B, C, D, E, A, 12,  9, 15,  6 );
    P2( A, B, C, D, E,  4,  8,  0, 14 );
    P2( E, A, B, C, D, 13,  9,  5,  6 );
    P2( D, E, A, B, C,  3, 14, 12,  9 );
    P2( C, D, E, A, B,  7,  5,  2, 12 );
    P2( B, C, D, E, A, 15,  6, 13,  9 );
    P2( A, B, C, D, E, 14,  8,  9, 12 );
    P2( E, A, B, C, D,  5,  6,  7,  5 );
    P2( D, E, A, B, C,  6,  5, 10, 15 );
    P2( C, D, E, A, B,  2, 12, 14,  8 );
#undef F
#undef K
#undef Fp
#undef Kp

#define F   F5
#define K   0xA953FD4E
#define Fp  F1
#define Kp  0x00000000
    P2( B, C, D, E, A,  4,  9, 12,  8 );
    P2( A, B, C, D, E,  0, 15, 15,  5 );
    P2( E, A, B, C, D,  5,  5, 10, 12 );
    P2( D, E, A, B, C,  9, 11,  4,  9 );
    P2( C, D, E, A, B,  7,  6,  1, 12 );
    P2( B, C, D, E, A, 12,  8,  5,  5 );
    P2( A, B, C, D, E,  2, 13,  8, 14 );
    P2( E, A, B, C, D, 10, 12,  7,  6 );
    P2( D, E, A, B, C, 14,  5,  6,  8 );
    P2( C, D, E, A, B,  1, 12,  2, 13 );
    P2( B, C, D, E, A,  3, 13, 13,  6 );
    P2( A, B, C, D, E,  8, 14, 14,  5 );
    P2( E, A, B, C, D, 11, 11,  0, 15 );
    P2( D, E, A, B, C,  6,  8,  3, 13 );
    P2( C, D, E, A, B, 15,  5,  9, 11 );
    P2( B, C, D, E, A, 13,  6, 11, 11 );
#undef F
#undef K
#undef Fp
#undef Kp

    C             = ctx->state[1] + C + Dp;
    ctx->state[1] = ctx->state[2] + D + Ep;
    ctx->state[2] = ctx->state[3] + E + Ap;
    ctx->state[3] = ctx->state[4] + A + Bp;
    ctx->state[4] = ctx->state[0] + B + Cp;
    ctx->state[0] = C;
}
#endif /* !MBEDTLS_RIPEMD160_PROCESS_ALT */

/*
 * RIPEMD-160 process buffer
 */
void ripemd160_Update( RIPEMD160_CTX *ctx, const uint8_t *input, uint32_t ilen )
{
    uint32_t fill;
    uint32_t left;

    if( ilen == 0 )
        return;

    left = ctx->total[0] & 0x3F;
    fill = RIPEMD160_BLOCK_LENGTH - left;

    ctx->total[0] += (uint32_t) ilen;
    ctx->total[0] &= 0xFFFFFFFF;

    if( ctx->total[0] < (uint32_t) ilen )
        ctx->total[1]++;

    if( left && ilen >= fill )
    {
        memcpy( (void *) (ctx->buffer + left), input, fill );
        ripemd160_process( ctx, ctx->buffer );
        input += fill;
        ilen  -= fill;
        left = 0;
    }

    while( ilen >= RIPEMD160_BLOCK_LENGTH )
    {
        ripemd160_process( ctx, input );
        input += RIPEMD160_BLOCK_LENGTH;
        ilen  -= RIPEMD160_BLOCK_LENGTH;
    }

    if( ilen > 0 )
    {
        memcpy( (void *) (ctx->buffer + left), input, ilen );
    }
}

static const uint8_t ripemd160_padding[RIPEMD160_BLOCK_LENGTH] =
{
 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * RIPEMD-160 final digest
 */
void ripemd160_Final( RIPEMD160_CTX *ctx, uint8_t output[RIPEMD160_DIGEST_LENGTH] )
{
    uint32_t last, padn;
    uint32_t high, low;
    uint8_t msglen[8];

    high = ( ctx->total[0] >> 29 )
         | ( ctx->total[1] <<  3 );
    low  = ( ctx->total[0] <<  3 );

    PUT_UINT32_LE( low,  msglen, 0 );
    PUT_UINT32_LE( high, msglen, 4 );

    last = ctx->total[0] & 0x3F;
    padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

    ripemd160_Update( ctx, ripemd160_padding, padn );
    ripemd160_Update( ctx, msglen, 8 );

    PUT_UINT32_LE( ctx->state[0], output,  0 );
    PUT_UINT32_LE( ctx->state[1], output,  4 );
    PUT_UINT32_LE( ctx->state[2], output,  8 );
    PUT_UINT32_LE( ctx->state[3], output, 12 );
    PUT_UINT32_LE( ctx->state[4], output, 16 );

    MEMSET_BZERO(ctx, sizeof(RIPEMD160_CTX));
}

/*
 * output = RIPEMD-160( input buffer )
 */
void ripemd160(const uint8_t *msg, uint32_t msg_len, uint8_t hash[RIPEMD160_DIGEST_LENGTH])
{
    RIPEMD160_CTX ctx;
    ripemd160_Init( &ctx );
    ripemd160_Update( &ctx, msg, msg_len );
    ripemd160_Final( &ctx, hash );
}
