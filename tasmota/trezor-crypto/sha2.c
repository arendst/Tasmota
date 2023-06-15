/**
 * Copyright (c) 2000-2001 Aaron D. Gifford
 * Copyright (c) 2013-2014 Pavol Rusnak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <stdint.h>
#include "sha2.h"

/*
 * ASSERT NOTE:
 * Some sanity checking code is included using assert().  On my FreeBSD
 * system, this additional code can be removed by compiling with NDEBUG
 * defined.  Check your own systems manpage on assert() to see how to
 * compile WITHOUT the sanity checking code on your system.
 *
 * UNROLLED TRANSFORM LOOP NOTE:
 * You can define SHA2_UNROLL_TRANSFORM to use the unrolled transform
 * loop version for the hash transform rounds (defined using macros
 * later in this file).  Either define on the command line, for example:
 *
 *   cc -DSHA2_UNROLL_TRANSFORM -o sha2 sha2.c sha2prog.c
 *
 * or define below:
 *
 *   #define SHA2_UNROLL_TRANSFORM
 *
 */


/*** SHA-256/384/512 Machine Architecture Definitions *****************/
/*
 * BYTE_ORDER NOTE:
 *
 * Please make sure that your system defines BYTE_ORDER.  If your
 * architecture is little-endian, make sure it also defines
 * LITTLE_ENDIAN and that the two (BYTE_ORDER and LITTLE_ENDIAN) are
 * equivilent.
 *
 * If your system does not define the above, then you can do so by
 * hand like this:
 *
 *   #define LITTLE_ENDIAN 1234
 *   #define BIG_ENDIAN    4321
 *
 * And for little-endian machines, add:
 *
 *   #define BYTE_ORDER LITTLE_ENDIAN 
 *
 * Or for big-endian machines:
 *
 *   #define BYTE_ORDER BIG_ENDIAN
 *
 * The FreeBSD machine this was written on defines BYTE_ORDER
 * appropriately by including <sys/types.h> (which in turn includes
 * <machine/endian.h> where the appropriate definitions are actually
 * made).
 */

#if !defined(BYTE_ORDER) || (BYTE_ORDER != LITTLE_ENDIAN && BYTE_ORDER != BIG_ENDIAN)
#error Define BYTE_ORDER to be equal to either LITTLE_ENDIAN or BIG_ENDIAN
#endif

typedef uint8_t  sha2_byte;	/* Exactly 1 byte */
typedef uint32_t sha2_word32;	/* Exactly 4 bytes */
typedef uint64_t sha2_word64;	/* Exactly 8 bytes */

/*** SHA-256/384/512 Various Length Definitions ***********************/
/* NOTE: Most of these are in sha2.h */
#define   SHA1_SHORT_BLOCK_LENGTH	(SHA1_BLOCK_LENGTH - 8)
#define SHA256_SHORT_BLOCK_LENGTH	(SHA256_BLOCK_LENGTH - 8)
#define SHA512_SHORT_BLOCK_LENGTH	(SHA512_BLOCK_LENGTH - 16)

/*
 * Macro for incrementally adding the unsigned 64-bit integer n to the
 * unsigned 128-bit integer (represented using a two-element array of
 * 64-bit words):
 */
#define ADDINC128(w,n)	{ \
	(w)[0] += (sha2_word64)(n); \
	if ((w)[0] < (n)) { \
		(w)[1]++; \
	} \
}

#define MEMSET_BZERO(p,l)	memset((p), 0, (l))
#define MEMCPY_BCOPY(d,s,l)	memcpy((d), (s), (l))

/*** THE SIX LOGICAL FUNCTIONS ****************************************/
/*
 * Bit shifting and rotation (used by the six SHA-XYZ logical functions:
 *
 *   NOTE:  In the original SHA-256/384/512 document, the shift-right
 *   function was named R and the rotate-right function was called S.
 *   (See: http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf on the
 *   web.)
 *
 *   The newer NIST FIPS 180-2 document uses a much clearer naming
 *   scheme, SHR for shift-right, ROTR for rotate-right, and ROTL for
 *   rotate-left.  (See:
 *   http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
 *   on the web.)
 *
 *   WARNING: These macros must be used cautiously, since they reference
 *   supplied parameters sometimes more than once, and thus could have
 *   unexpected side-effects if used without taking this into account.
 */

/* Shift-right (used in SHA-256, SHA-384, and SHA-512): */
#define SHR(b,x) 		((x) >> (b))
/* 32-bit Rotate-right (used in SHA-256): */
#define ROTR32(b,x)	(((x) >> (b)) | ((x) << (32 - (b))))
/* 64-bit Rotate-right (used in SHA-384 and SHA-512): */
#define ROTR64(b,x)	(((x) >> (b)) | ((x) << (64 - (b))))
/* 32-bit Rotate-left (used in SHA-1): */
#define ROTL32(b,x)	(((x) << (b)) | ((x) >> (32 - (b))))

/* Two of six logical functions used in SHA-1, SHA-256, SHA-384, and SHA-512: */
#define Ch(x,y,z)	(((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* Function used in SHA-1: */
#define Parity(x,y,z)	((x) ^ (y) ^ (z))

/* Four of six logical functions used in SHA-256: */
#define Sigma0_256(x)	(ROTR32(2,  (x)) ^ ROTR32(13, (x)) ^ ROTR32(22, (x)))
#define Sigma1_256(x)	(ROTR32(6,  (x)) ^ ROTR32(11, (x)) ^ ROTR32(25, (x)))
#define sigma0_256(x)	(ROTR32(7,  (x)) ^ ROTR32(18, (x)) ^ SHR(3 ,   (x)))
#define sigma1_256(x)	(ROTR32(17, (x)) ^ ROTR32(19, (x)) ^ SHR(10,   (x)))

/* Four of six logical functions used in SHA-384 and SHA-512: */
#define Sigma0_512(x)	(ROTR64(28, (x)) ^ ROTR64(34, (x)) ^ ROTR64(39, (x)))
#define Sigma1_512(x)	(ROTR64(14, (x)) ^ ROTR64(18, (x)) ^ ROTR64(41, (x)))
#define sigma0_512(x)	(ROTR64( 1, (x)) ^ ROTR64( 8, (x)) ^ SHR( 7,   (x)))
#define sigma1_512(x)	(ROTR64(19, (x)) ^ ROTR64(61, (x)) ^ SHR( 6,   (x)))

/*** INTERNAL FUNCTION PROTOTYPES *************************************/
/* NOTE: These should not be accessed directly from outside this
 * library -- they are intended for private internal visibility/use
 * only.
 */
static void sha512_Last(SHA512_CTX*);


/*** SHA-XYZ INITIAL HASH VALUES AND CONSTANTS ************************/

/* Hash constant words K for SHA-1: */
#define K1_0_TO_19	0x5a827999UL
#define K1_20_TO_39	0x6ed9eba1UL
#define K1_40_TO_59	0x8f1bbcdcUL
#define K1_60_TO_79	0xca62c1d6UL

/* Initial hash value H for SHA-1: */
const sha2_word32 sha1_initial_hash_value[SHA1_DIGEST_LENGTH / sizeof(sha2_word32)] = {
	0x67452301UL,
	0xefcdab89UL,
	0x98badcfeUL,
	0x10325476UL,
	0xc3d2e1f0UL
};

/* Hash constant words K for SHA-256: */
static const sha2_word32 K256[64] = {
	0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
	0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
	0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
	0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
	0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
	0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
	0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
	0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
	0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
	0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
	0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
	0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
	0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
	0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
	0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
	0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/* Initial hash value H for SHA-256: */
const sha2_word32 sha256_initial_hash_value[8] = {
	0x6a09e667UL,
	0xbb67ae85UL,
	0x3c6ef372UL,
	0xa54ff53aUL,
	0x510e527fUL,
	0x9b05688cUL,
	0x1f83d9abUL,
	0x5be0cd19UL
};

/* Hash constant words K for SHA-384 and SHA-512: */
static const sha2_word64 K512[80] = {
	0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
	0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
	0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
	0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
	0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
	0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
	0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
	0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
	0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
	0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
	0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
	0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
	0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
	0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
	0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
	0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
	0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
	0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
	0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
	0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
	0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
	0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
	0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
	0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
	0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
	0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
	0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
	0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
	0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
	0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
	0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
	0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
	0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
	0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
	0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
	0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
	0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
	0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
	0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
	0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

/* Initial hash value H for SHA-512 */
const sha2_word64 sha512_initial_hash_value[8] = {
	0x6a09e667f3bcc908ULL,
	0xbb67ae8584caa73bULL,
	0x3c6ef372fe94f82bULL,
	0xa54ff53a5f1d36f1ULL,
	0x510e527fade682d1ULL,
	0x9b05688c2b3e6c1fULL,
	0x1f83d9abfb41bd6bULL,
	0x5be0cd19137e2179ULL
};

/*
 * Constant used by SHA256/384/512_End() functions for converting the
 * digest to a readable hexadecimal character string:
 */
static const char *sha2_hex_digits = "0123456789abcdef";


/*** SHA-1: ***********************************************************/
void sha1_Init(SHA1_CTX* context) {
	MEMCPY_BCOPY(context->state, sha1_initial_hash_value, SHA1_DIGEST_LENGTH);
	MEMSET_BZERO(context->buffer, SHA1_BLOCK_LENGTH);
	context->bitcount = 0;
}

#ifdef SHA2_UNROLL_TRANSFORM

/* Unrolled SHA-1 round macros: */

#if BYTE_ORDER == LITTLE_ENDIAN

#define ROUND1_0_TO_15(a,b,c,d,e)				\
	REVERSE32(*data++, W1[j]);				\
	(e) = ROTL32(5, (a)) + Ch((b), (c), (d)) + (e) +	\
	     K1_0_TO_19 + W1[j];	\
	(b) = ROTL32(30, (b));		\
	j++;

#else /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND1_0_TO_15(a,b,c,d,e)				\
	(e) = ROTL32(5, (a)) + Ch((b), (c), (d)) + (e) +	\
	     K1_0_TO_19 + ( W1[j] = *data++ );		\
	(b) = ROTL32(30, (b));	\
	j++;

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND1_16_TO_19(a,b,c,d,e)	\
	T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];	\
	(e) = ROTL32(5, a) + Ch(b,c,d) + e + K1_0_TO_19 + ( W1[j&0x0f] = ROTL32(1, T1) );	\
	(b) = ROTL32(30, b);	\
	j++;

#define ROUND1_20_TO_39(a,b,c,d,e)	\
	T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];	\
	(e) = ROTL32(5, a) + Parity(b,c,d) + e + K1_20_TO_39 + ( W1[j&0x0f] = ROTL32(1, T1) );	\
	(b) = ROTL32(30, b);	\
	j++;

#define ROUND1_40_TO_59(a,b,c,d,e)	\
	T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];	\
	(e) = ROTL32(5, a) + Maj(b,c,d) + e + K1_40_TO_59 + ( W1[j&0x0f] = ROTL32(1, T1) );	\
	(b) = ROTL32(30, b);	\
	j++;

#define ROUND1_60_TO_79(a,b,c,d,e)	\
	T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];	\
	(e) = ROTL32(5, a) + Parity(b,c,d) + e + K1_60_TO_79 + ( W1[j&0x0f] = ROTL32(1, T1) );	\
	(b) = ROTL32(30, b);	\
	j++;

void sha1_Transform(const sha2_word32* state_in, const sha2_word32* data, sha2_word32* state_out) {
	sha2_word32	a, b, c, d, e;
	sha2_word32	T1;
	sha2_word32	W1[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];

	j = 0;

	/* Rounds 0 to 15 unrolled: */
	ROUND1_0_TO_15(a,b,c,d,e);
	ROUND1_0_TO_15(e,a,b,c,d);
	ROUND1_0_TO_15(d,e,a,b,c);
	ROUND1_0_TO_15(c,d,e,a,b);
	ROUND1_0_TO_15(b,c,d,e,a);
	ROUND1_0_TO_15(a,b,c,d,e);
	ROUND1_0_TO_15(e,a,b,c,d);
	ROUND1_0_TO_15(d,e,a,b,c);
	ROUND1_0_TO_15(c,d,e,a,b);
	ROUND1_0_TO_15(b,c,d,e,a);
	ROUND1_0_TO_15(a,b,c,d,e);
	ROUND1_0_TO_15(e,a,b,c,d);
	ROUND1_0_TO_15(d,e,a,b,c);
	ROUND1_0_TO_15(c,d,e,a,b);
	ROUND1_0_TO_15(b,c,d,e,a);
	ROUND1_0_TO_15(a,b,c,d,e);

	/* Rounds 16 to 19 unrolled: */
	ROUND1_16_TO_19(e,a,b,c,d);
	ROUND1_16_TO_19(d,e,a,b,c);
	ROUND1_16_TO_19(c,d,e,a,b);
	ROUND1_16_TO_19(b,c,d,e,a);

	/* Rounds 20 to 39 unrolled: */
	ROUND1_20_TO_39(a,b,c,d,e);
	ROUND1_20_TO_39(e,a,b,c,d);
	ROUND1_20_TO_39(d,e,a,b,c);
	ROUND1_20_TO_39(c,d,e,a,b);
	ROUND1_20_TO_39(b,c,d,e,a);
	ROUND1_20_TO_39(a,b,c,d,e);
	ROUND1_20_TO_39(e,a,b,c,d);
	ROUND1_20_TO_39(d,e,a,b,c);
	ROUND1_20_TO_39(c,d,e,a,b);
	ROUND1_20_TO_39(b,c,d,e,a);
	ROUND1_20_TO_39(a,b,c,d,e);
	ROUND1_20_TO_39(e,a,b,c,d);
	ROUND1_20_TO_39(d,e,a,b,c);
	ROUND1_20_TO_39(c,d,e,a,b);
	ROUND1_20_TO_39(b,c,d,e,a);
	ROUND1_20_TO_39(a,b,c,d,e);
	ROUND1_20_TO_39(e,a,b,c,d);
	ROUND1_20_TO_39(d,e,a,b,c);
	ROUND1_20_TO_39(c,d,e,a,b);
	ROUND1_20_TO_39(b,c,d,e,a);

	/* Rounds 40 to 59 unrolled: */
	ROUND1_40_TO_59(a,b,c,d,e);
	ROUND1_40_TO_59(e,a,b,c,d);
	ROUND1_40_TO_59(d,e,a,b,c);
	ROUND1_40_TO_59(c,d,e,a,b);
	ROUND1_40_TO_59(b,c,d,e,a);
	ROUND1_40_TO_59(a,b,c,d,e);
	ROUND1_40_TO_59(e,a,b,c,d);
	ROUND1_40_TO_59(d,e,a,b,c);
	ROUND1_40_TO_59(c,d,e,a,b);
	ROUND1_40_TO_59(b,c,d,e,a);
	ROUND1_40_TO_59(a,b,c,d,e);
	ROUND1_40_TO_59(e,a,b,c,d);
	ROUND1_40_TO_59(d,e,a,b,c);
	ROUND1_40_TO_59(c,d,e,a,b);
	ROUND1_40_TO_59(b,c,d,e,a);
	ROUND1_40_TO_59(a,b,c,d,e);
	ROUND1_40_TO_59(e,a,b,c,d);
	ROUND1_40_TO_59(d,e,a,b,c);
	ROUND1_40_TO_59(c,d,e,a,b);
	ROUND1_40_TO_59(b,c,d,e,a);

	/* Rounds 60 to 79 unrolled: */
	ROUND1_60_TO_79(a,b,c,d,e);
	ROUND1_60_TO_79(e,a,b,c,d);
	ROUND1_60_TO_79(d,e,a,b,c);
	ROUND1_60_TO_79(c,d,e,a,b);
	ROUND1_60_TO_79(b,c,d,e,a);
	ROUND1_60_TO_79(a,b,c,d,e);
	ROUND1_60_TO_79(e,a,b,c,d);
	ROUND1_60_TO_79(d,e,a,b,c);
	ROUND1_60_TO_79(c,d,e,a,b);
	ROUND1_60_TO_79(b,c,d,e,a);
	ROUND1_60_TO_79(a,b,c,d,e);
	ROUND1_60_TO_79(e,a,b,c,d);
	ROUND1_60_TO_79(d,e,a,b,c);
	ROUND1_60_TO_79(c,d,e,a,b);
	ROUND1_60_TO_79(b,c,d,e,a);
	ROUND1_60_TO_79(a,b,c,d,e);
	ROUND1_60_TO_79(e,a,b,c,d);
	ROUND1_60_TO_79(d,e,a,b,c);
	ROUND1_60_TO_79(c,d,e,a,b);
	ROUND1_60_TO_79(b,c,d,e,a);

	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;

	/* Clean up */
	a = b = c = d = e = T1 = 0;
}

#else  /* SHA2_UNROLL_TRANSFORM */

void sha1_Transform(const sha2_word32* state_in, const sha2_word32* data, sha2_word32* state_out) {
	sha2_word32	a, b, c, d, e;
	sha2_word32	T1;
	sha2_word32	W1[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];
	j = 0;
	do {
#if BYTE_ORDER == LITTLE_ENDIAN
		T1 = data[j];
		/* Copy data while converting to host byte order */
		REVERSE32(*data++, W1[j]);
		T1 = ROTL32(5, a) + Ch(b, c, d) + e + K1_0_TO_19 + W1[j];
#else /* BYTE_ORDER == LITTLE_ENDIAN */
		T1 = ROTL32(5, a) + Ch(b, c, d) + e + K1_0_TO_19 + (W1[j] = *data++);
#endif /* BYTE_ORDER == LITTLE_ENDIAN */
		e = d;
		d = c;
		c = ROTL32(30, b);
		b = a;
		a = T1;
		j++;
	} while (j < 16);

	do {
		T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];
		T1 = ROTL32(5, a) + Ch(b,c,d) + e + K1_0_TO_19 + (W1[j&0x0f] = ROTL32(1, T1));
		e = d;
		d = c;
		c = ROTL32(30, b);
		b = a;
		a = T1;
		j++;
	} while (j < 20);

	do {
		T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];
		T1 = ROTL32(5, a) + Parity(b,c,d) + e + K1_20_TO_39 + (W1[j&0x0f] = ROTL32(1, T1));
		e = d;
		d = c;
		c = ROTL32(30, b);
		b = a;
		a = T1;
		j++;
	} while (j < 40);

	do {
		T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];
		T1 = ROTL32(5, a) + Maj(b,c,d) + e + K1_40_TO_59 + (W1[j&0x0f] = ROTL32(1, T1));
		e = d;
		d = c;
		c = ROTL32(30, b);
		b = a;
		a = T1;
		j++;
	} while (j < 60);

	do {
		T1 = W1[(j+13)&0x0f] ^ W1[(j+8)&0x0f] ^ W1[(j+2)&0x0f] ^ W1[j&0x0f];
		T1 = ROTL32(5, a) + Parity(b,c,d) + e + K1_60_TO_79 + (W1[j&0x0f] = ROTL32(1, T1));
		e = d;
		d = c;
		c = ROTL32(30, b);
		b = a;
		a = T1;
		j++;
	} while (j < 80);


	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;

	/* Clean up */
	a = b = c = d = e = T1 = 0;
}

#endif /* SHA2_UNROLL_TRANSFORM */

void sha1_Update(SHA1_CTX* context, const sha2_byte *data, size_t len) {
	unsigned int	freespace, usedspace;
	if (len == 0) {
		/* Calling with no data is valid - we do nothing */
		return;
	}

	usedspace = (context->bitcount >> 3) % SHA1_BLOCK_LENGTH;
	if (usedspace > 0) {
		/* Calculate how much free space is available in the buffer */
		freespace = SHA1_BLOCK_LENGTH - usedspace;

		if (len >= freespace) {
			/* Fill the buffer completely and process it */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, freespace);
			context->bitcount += freespace << 3;
			len -= freespace;
			data += freespace;
			sha1_Transform(context->state, context->buffer, context->state);
		} else {
			/* The buffer is not yet full */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, len);
			context->bitcount += len << 3;
			/* Clean up: */
			usedspace = freespace = 0;
			return;
		}
	}
	while (len >= SHA1_BLOCK_LENGTH) {
		/* Process as many complete blocks as we can */
		sha1_Transform(context->state, (sha2_word32*)data, context->state);
		context->bitcount += SHA1_BLOCK_LENGTH << 3;
		len -= SHA1_BLOCK_LENGTH;
		data += SHA1_BLOCK_LENGTH;
	}
	if (len > 0) {
		/* There's left-overs, so save 'em */
		MEMCPY_BCOPY(context->buffer, data, len);
		context->bitcount += len << 3;
	}
	/* Clean up: */
	usedspace = freespace = 0;
}

void sha1_Final(SHA1_CTX* context, sha2_byte digest[]) {
	sha2_word32	*d = (sha2_word32*)digest;
	unsigned int	usedspace;

	if (digest == (sha2_byte*)0) {
		/*
		 * No digest buffer, so we can do nothing
		 * except clean up and go home
		 */
		MEMSET_BZERO(context, sizeof(SHA1_CTX));
		return;
	}

	usedspace = (context->bitcount >> 3) % SHA1_BLOCK_LENGTH;
	if (usedspace == 0) {
		/* Set-up for the last transform: */
		MEMSET_BZERO(context->buffer, SHA1_SHORT_BLOCK_LENGTH);

		/* Begin padding with a 1 bit: */
		*context->buffer = 0x80;
	} else {
		/* Begin padding with a 1 bit: */
		((uint8_t*)context->buffer)[usedspace++] = 0x80;

		if (usedspace <= 56) {
			/* Set-up for the last transform: */
			MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, 56 - usedspace);
		} else {
			if (usedspace < 64) {
				MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, 64 - usedspace);
			}
			/* Do second-to-last transform: */
			sha1_Transform(context->state, context->buffer, context->state);

			/* And set-up for the last transform: */
			MEMSET_BZERO(context->buffer, 56);
		}
		/* Clean up: */
		usedspace = 0;
	}
	/* Set the bit count: */
#if BYTE_ORDER == LITTLE_ENDIAN
	/* Convert FROM host byte order */
	REVERSE64(context->bitcount,context->bitcount);
#endif
	context->buffer[SHA1_SHORT_BLOCK_LENGTH >> 2]     = context->bitcount << 32;
	context->buffer[SHA1_SHORT_BLOCK_LENGTH >> 2 | 1] = context->bitcount >> 32;

	/* Final transform: */
	sha1_Transform(context->state, context->buffer, context->state);

	/* Save the hash data for output: */
#if BYTE_ORDER == LITTLE_ENDIAN
	{
		/* Convert TO host byte order */
		int	j;
		for (j = 0; j < (SHA1_DIGEST_LENGTH >> 2); j++) {
			REVERSE32(context->state[j],context->state[j]);
			*d++ = context->state[j];
		}
	}
#else
	MEMCPY_BCOPY(d, context->state, SHA1_DIGEST_LENGTH);
#endif

	/* Clean up: */
	MEMSET_BZERO(context, sizeof(SHA1_CTX));
}

char *sha1_End(SHA1_CTX* context, char buffer[]) {
	sha2_byte	digest[SHA1_DIGEST_LENGTH], *d = digest;
	int		i;

	if (buffer != (char*)0) {
		sha1_Final(context, digest);

		for (i = 0; i < SHA1_DIGEST_LENGTH; i++) {
			*buffer++ = sha2_hex_digits[(*d & 0xf0) >> 4];
			*buffer++ = sha2_hex_digits[*d & 0x0f];
			d++;
		}
		*buffer = (char)0;
	} else {
		MEMSET_BZERO(context, sizeof(SHA1_CTX));
	}
	MEMSET_BZERO(digest, SHA1_DIGEST_LENGTH);
	return buffer;
}

void sha1_Raw(const sha2_byte* data, size_t len, uint8_t digest[SHA1_DIGEST_LENGTH]) {
	SHA1_CTX	context;
	sha1_Init(&context);
	sha1_Update(&context, data, len);
	sha1_Final(&context, digest);
}

char* sha1_Data(const sha2_byte* data, size_t len, char digest[SHA1_DIGEST_STRING_LENGTH]) {
	SHA1_CTX	context;

	sha1_Init(&context);
	sha1_Update(&context, data, len);
	return sha1_End(&context, digest);
}

/*** SHA-256: *********************************************************/
void sha256_Init(SHA256_CTX* context) {
	if (context == (SHA256_CTX*)0) {
		return;
	}
	MEMCPY_BCOPY(context->state, sha256_initial_hash_value, SHA256_DIGEST_LENGTH);
	MEMSET_BZERO(context->buffer, SHA256_BLOCK_LENGTH);
	context->bitcount = 0;
}

#ifdef SHA2_UNROLL_TRANSFORM

/* Unrolled SHA-256 round macros: */

#if BYTE_ORDER == LITTLE_ENDIAN

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)	\
	W256[j] = *data++; \
	T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + \
             K256[j] + W256[j]; \
	(d) += T1; \
	(h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
	j++


#else /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)	\
	T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + \
	     K256[j] + (W256[j] = *data++); \
	(d) += T1; \
	(h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
	j++

#endif /* BYTE_ORDER == LITTLE_ENDIAN */

#define ROUND256(a,b,c,d,e,f,g,h)	\
	s0 = W256[(j+1)&0x0f]; \
	s0 = sigma0_256(s0); \
	s1 = W256[(j+14)&0x0f]; \
	s1 = sigma1_256(s1); \
	T1 = (h) + Sigma1_256(e) + Ch((e), (f), (g)) + K256[j] + \
	     (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0); \
	(d) += T1; \
	(h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
	j++

void sha256_Transform(const sha2_word32* state_in, const sha2_word32* data, sha2_word32* state_out) {
	sha2_word32	a, b, c, d, e, f, g, h, s0, s1;
	sha2_word32	T1;
	sha2_word32 W256[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];
	f = state_in[5];
	g = state_in[6];
	h = state_in[7];

	j = 0;
	do {
		/* Rounds 0 to 15 (unrolled): */
		ROUND256_0_TO_15(a,b,c,d,e,f,g,h);
		ROUND256_0_TO_15(h,a,b,c,d,e,f,g);
		ROUND256_0_TO_15(g,h,a,b,c,d,e,f);
		ROUND256_0_TO_15(f,g,h,a,b,c,d,e);
		ROUND256_0_TO_15(e,f,g,h,a,b,c,d);
		ROUND256_0_TO_15(d,e,f,g,h,a,b,c);
		ROUND256_0_TO_15(c,d,e,f,g,h,a,b);
		ROUND256_0_TO_15(b,c,d,e,f,g,h,a);
	} while (j < 16);

	/* Now for the remaining rounds to 64: */
	do {
		ROUND256(a,b,c,d,e,f,g,h);
		ROUND256(h,a,b,c,d,e,f,g);
		ROUND256(g,h,a,b,c,d,e,f);
		ROUND256(f,g,h,a,b,c,d,e);
		ROUND256(e,f,g,h,a,b,c,d);
		ROUND256(d,e,f,g,h,a,b,c);
		ROUND256(c,d,e,f,g,h,a,b);
		ROUND256(b,c,d,e,f,g,h,a);
	} while (j < 64);

	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;
	state_out[5] = state_in[5] + f;
	state_out[6] = state_in[6] + g;
	state_out[7] = state_in[7] + h;

	/* Clean up */
	a = b = c = d = e = f = g = h = T1 = 0;
}

#else /* SHA2_UNROLL_TRANSFORM */

void sha256_Transform(const sha2_word32* state_in, const sha2_word32* data, sha2_word32* state_out) {
	sha2_word32	a, b, c, d, e, f, g, h, s0, s1;
	sha2_word32	T1, T2, W256[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];
	f = state_in[5];
	g = state_in[6];
	h = state_in[7];

	j = 0;
	do {
		/* Apply the SHA-256 compression function to update a..h with copy */
		T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + (W256[j] = *data++);
		T2 = Sigma0_256(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 16);

	do {
		/* Part of the message block expansion: */
		s0 = W256[(j+1)&0x0f];
		s0 = sigma0_256(s0);
		s1 = W256[(j+14)&0x0f];	
		s1 = sigma1_256(s1);

		/* Apply the SHA-256 compression function to update a..h */
		T1 = h + Sigma1_256(e) + Ch(e, f, g) + K256[j] + 
		     (W256[j&0x0f] += s1 + W256[(j+9)&0x0f] + s0);
		T2 = Sigma0_256(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 64);

	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;
	state_out[5] = state_in[5] + f;
	state_out[6] = state_in[6] + g;
	state_out[7] = state_in[7] + h;

	/* Clean up */
	a = b = c = d = e = f = g = h = T1 = T2 = 0;
}

#endif /* SHA2_UNROLL_TRANSFORM */

void sha256_Update(SHA256_CTX* context, const sha2_byte *data, size_t len) {
	unsigned int	freespace, usedspace;

	if (len == 0) {
		/* Calling with no data is valid - we do nothing */
		return;
	}

	usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
	if (usedspace > 0) {
		/* Calculate how much free space is available in the buffer */
		freespace = SHA256_BLOCK_LENGTH - usedspace;

		if (len >= freespace) {
			/* Fill the buffer completely and process it */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, freespace);
			context->bitcount += freespace << 3;
			len -= freespace;
			data += freespace;
#if BYTE_ORDER == LITTLE_ENDIAN
			/* Convert TO host byte order */
			for (int j = 0; j < 16; j++) {
				REVERSE32(context->buffer[j],context->buffer[j]);
			}
#endif
			sha256_Transform(context->state, context->buffer, context->state);
		} else {
			/* The buffer is not yet full */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, len);
			context->bitcount += len << 3;
			/* Clean up: */
			usedspace = freespace = 0;
			return;
		}
	}
	while (len >= SHA256_BLOCK_LENGTH) {
		/* Process as many complete blocks as we can */
		MEMCPY_BCOPY(context->buffer, data, SHA256_BLOCK_LENGTH);
#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert TO host byte order */
		for (int j = 0; j < 16; j++) {
			REVERSE32(context->buffer[j],context->buffer[j]);
		}
#endif
		sha256_Transform(context->state, context->buffer, context->state);
		context->bitcount += SHA256_BLOCK_LENGTH << 3;
		len -= SHA256_BLOCK_LENGTH;
		data += SHA256_BLOCK_LENGTH;
	}
	if (len > 0) {
		/* There's left-overs, so save 'em */
		MEMCPY_BCOPY(context->buffer, data, len);
		context->bitcount += len << 3;
	}
	/* Clean up: */
	usedspace = freespace = 0;
}

void sha256_Final(SHA256_CTX* context, sha2_byte digest[]) {
	unsigned int	usedspace;

	/* If no digest buffer is passed, we don't bother doing this: */
	if (digest != (sha2_byte*)0) {
		usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
		/* Begin padding with a 1 bit: */
		((uint8_t*)context->buffer)[usedspace++] = 0x80;
		
		if (usedspace > SHA256_SHORT_BLOCK_LENGTH) {
			MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, SHA256_BLOCK_LENGTH - usedspace);

#if BYTE_ORDER == LITTLE_ENDIAN
			/* Convert TO host byte order */
			for (int j = 0; j < 16; j++) {
				REVERSE32(context->buffer[j],context->buffer[j]);
			}
#endif
			/* Do second-to-last transform: */
			sha256_Transform(context->state, context->buffer, context->state);
			
			/* And prepare the last transform: */
			usedspace = 0;
		}
		/* Set-up for the last transform: */
		MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, SHA256_SHORT_BLOCK_LENGTH - usedspace);

#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert TO host byte order */
		for (int j = 0; j < 14; j++) {
			REVERSE32(context->buffer[j],context->buffer[j]);
		}
#endif
		/* Set the bit count: */
		context->buffer[14] = context->bitcount >> 32;
		context->buffer[15] = context->bitcount & 0xffffffff;

		/* Final transform: */
		sha256_Transform(context->state, context->buffer, context->state);

#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert FROM host byte order */
		for (int j = 0; j < 8; j++) {
			REVERSE32(context->state[j],context->state[j]);
		}
#endif
		MEMCPY_BCOPY(digest, context->state, SHA256_DIGEST_LENGTH);
	}

	/* Clean up state data: */
	MEMSET_BZERO(context, sizeof(SHA256_CTX));
	usedspace = 0;
}

char *sha256_End(SHA256_CTX* context, char buffer[]) {
	sha2_byte	digest[SHA256_DIGEST_LENGTH], *d = digest;
	int		i;

	if (buffer != (char*)0) {
		sha256_Final(context, digest);

		for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
			*buffer++ = sha2_hex_digits[(*d & 0xf0) >> 4];
			*buffer++ = sha2_hex_digits[*d & 0x0f];
			d++;
		}
		*buffer = (char)0;
	} else {
		MEMSET_BZERO(context, sizeof(SHA256_CTX));
	}
	MEMSET_BZERO(digest, SHA256_DIGEST_LENGTH);
	return buffer;
}

void sha256_Raw(const sha2_byte* data, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH]) {
	SHA256_CTX	context;
	sha256_Init(&context);
	sha256_Update(&context, data, len);
	sha256_Final(&context, digest);
}

char* sha256_Data(const sha2_byte* data, size_t len, char digest[SHA256_DIGEST_STRING_LENGTH]) {
	SHA256_CTX	context;

	sha256_Init(&context);
	sha256_Update(&context, data, len);
	return sha256_End(&context, digest);
}


/*** SHA-512: *********************************************************/
void sha512_Init(SHA512_CTX* context) {
	if (context == (SHA512_CTX*)0) {
		return;
	}
	MEMCPY_BCOPY(context->state, sha512_initial_hash_value, SHA512_DIGEST_LENGTH);
	MEMSET_BZERO(context->buffer, SHA512_BLOCK_LENGTH);
	context->bitcount[0] = context->bitcount[1] =  0;
}

#ifdef SHA2_UNROLL_TRANSFORM

/* Unrolled SHA-512 round macros: */
#define ROUND512_0_TO_15(a,b,c,d,e,f,g,h)	\
	T1 = (h) + Sigma1_512(e) + Ch((e), (f), (g)) + \
             K512[j] + (W512[j] = *data++); \
	(d) += T1; \
	(h) = T1 + Sigma0_512(a) + Maj((a), (b), (c)); \
	j++

#define ROUND512(a,b,c,d,e,f,g,h)	\
	s0 = W512[(j+1)&0x0f]; \
	s0 = sigma0_512(s0); \
	s1 = W512[(j+14)&0x0f]; \
	s1 = sigma1_512(s1); \
	T1 = (h) + Sigma1_512(e) + Ch((e), (f), (g)) + K512[j] + \
             (W512[j&0x0f] += s1 + W512[(j+9)&0x0f] + s0); \
	(d) += T1; \
	(h) = T1 + Sigma0_512(a) + Maj((a), (b), (c)); \
	j++

void sha512_Transform(const sha2_word64* state_in, const sha2_word64* data, sha2_word64* state_out) {
	sha2_word64	a, b, c, d, e, f, g, h, s0, s1;
	sha2_word64	T1, W512[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];
	f = state_in[5];
	g = state_in[6];
	h = state_in[7];

	j = 0;
	do {
		ROUND512_0_TO_15(a,b,c,d,e,f,g,h);
		ROUND512_0_TO_15(h,a,b,c,d,e,f,g);
		ROUND512_0_TO_15(g,h,a,b,c,d,e,f);
		ROUND512_0_TO_15(f,g,h,a,b,c,d,e);
		ROUND512_0_TO_15(e,f,g,h,a,b,c,d);
		ROUND512_0_TO_15(d,e,f,g,h,a,b,c);
		ROUND512_0_TO_15(c,d,e,f,g,h,a,b);
		ROUND512_0_TO_15(b,c,d,e,f,g,h,a);
	} while (j < 16);

	/* Now for the remaining rounds up to 79: */
	do {
		ROUND512(a,b,c,d,e,f,g,h);
		ROUND512(h,a,b,c,d,e,f,g);
		ROUND512(g,h,a,b,c,d,e,f);
		ROUND512(f,g,h,a,b,c,d,e);
		ROUND512(e,f,g,h,a,b,c,d);
		ROUND512(d,e,f,g,h,a,b,c);
		ROUND512(c,d,e,f,g,h,a,b);
		ROUND512(b,c,d,e,f,g,h,a);
	} while (j < 80);

	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;
	state_out[5] = state_in[5] + f;
	state_out[6] = state_in[6] + g;
	state_out[7] = state_in[7] + h;

	/* Clean up */
	a = b = c = d = e = f = g = h = T1 = 0;
}

#else /* SHA2_UNROLL_TRANSFORM */

void sha512_Transform(const sha2_word64* state_in, const sha2_word64* data, sha2_word64* state_out) {
	sha2_word64	a, b, c, d, e, f, g, h, s0, s1;
	sha2_word64	T1, T2, W512[16];
	int		j;

	/* Initialize registers with the prev. intermediate value */
	a = state_in[0];
	b = state_in[1];
	c = state_in[2];
	d = state_in[3];
	e = state_in[4];
	f = state_in[5];
	g = state_in[6];
	h = state_in[7];

	j = 0;
	do {
		/* Apply the SHA-512 compression function to update a..h with copy */
		T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] + (W512[j] = *data++);
		T2 = Sigma0_512(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 16);

	do {
		/* Part of the message block expansion: */
		s0 = W512[(j+1)&0x0f];
		s0 = sigma0_512(s0);
		s1 = W512[(j+14)&0x0f];
		s1 =  sigma1_512(s1);

		/* Apply the SHA-512 compression function to update a..h */
		T1 = h + Sigma1_512(e) + Ch(e, f, g) + K512[j] +
		     (W512[j&0x0f] += s1 + W512[(j+9)&0x0f] + s0);
		T2 = Sigma0_512(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;

		j++;
	} while (j < 80);

	/* Compute the current intermediate hash value */
	state_out[0] = state_in[0] + a;
	state_out[1] = state_in[1] + b;
	state_out[2] = state_in[2] + c;
	state_out[3] = state_in[3] + d;
	state_out[4] = state_in[4] + e;
	state_out[5] = state_in[5] + f;
	state_out[6] = state_in[6] + g;
	state_out[7] = state_in[7] + h;

	/* Clean up */
	a = b = c = d = e = f = g = h = T1 = T2 = 0;
}

#endif /* SHA2_UNROLL_TRANSFORM */

void sha512_Update(SHA512_CTX* context, const sha2_byte *data, size_t len) {
	unsigned int	freespace, usedspace;

	if (len == 0) {
		/* Calling with no data is valid - we do nothing */
		return;
	}

	usedspace = (context->bitcount[0] >> 3) % SHA512_BLOCK_LENGTH;
	if (usedspace > 0) {
		/* Calculate how much free space is available in the buffer */
		freespace = SHA512_BLOCK_LENGTH - usedspace;

		if (len >= freespace) {
			/* Fill the buffer completely and process it */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, freespace);
			ADDINC128(context->bitcount, freespace << 3);
			len -= freespace;
			data += freespace;
#if BYTE_ORDER == LITTLE_ENDIAN
			/* Convert TO host byte order */
			for (int j = 0; j < 16; j++) {
				REVERSE64(context->buffer[j],context->buffer[j]);
			}
#endif
			sha512_Transform(context->state, context->buffer, context->state);
		} else {
			/* The buffer is not yet full */
			MEMCPY_BCOPY(((uint8_t*)context->buffer) + usedspace, data, len);
			ADDINC128(context->bitcount, len << 3);
			/* Clean up: */
			usedspace = freespace = 0;
			return;
		}
	}
	while (len >= SHA512_BLOCK_LENGTH) {
		/* Process as many complete blocks as we can */
		MEMCPY_BCOPY(context->buffer, data, SHA512_BLOCK_LENGTH);
#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert TO host byte order */
		for (int j = 0; j < 16; j++) {
			REVERSE64(context->buffer[j],context->buffer[j]);
		}
#endif
		sha512_Transform(context->state, context->buffer, context->state);
		ADDINC128(context->bitcount, SHA512_BLOCK_LENGTH << 3);
		len -= SHA512_BLOCK_LENGTH;
		data += SHA512_BLOCK_LENGTH;
	}
	if (len > 0) {
		/* There's left-overs, so save 'em */
		MEMCPY_BCOPY(context->buffer, data, len);
		ADDINC128(context->bitcount, len << 3);
	}
	/* Clean up: */
	usedspace = freespace = 0;
}

static void sha512_Last(SHA512_CTX* context) {
	unsigned int	usedspace;

	usedspace = (context->bitcount[0] >> 3) % SHA512_BLOCK_LENGTH;
	/* Begin padding with a 1 bit: */
	((uint8_t*)context->buffer)[usedspace++] = 0x80;
	
	if (usedspace > SHA512_SHORT_BLOCK_LENGTH) {
		MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, SHA512_BLOCK_LENGTH - usedspace);

#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert TO host byte order */
		for (int j = 0; j < 16; j++) {
			REVERSE64(context->buffer[j],context->buffer[j]);
		}
#endif
		/* Do second-to-last transform: */
		sha512_Transform(context->state, context->buffer, context->state);

		/* And prepare the last transform: */
		usedspace = 0;
	}
	/* Set-up for the last transform: */
	MEMSET_BZERO(((uint8_t*)context->buffer) + usedspace, SHA512_SHORT_BLOCK_LENGTH - usedspace);

#if BYTE_ORDER == LITTLE_ENDIAN
	/* Convert TO host byte order */
	for (int j = 0; j < 14; j++) {
		REVERSE64(context->buffer[j],context->buffer[j]);
	}
#endif
	/* Store the length of input data (in bits): */
	context->buffer[14] = context->bitcount[1];
	context->buffer[15] = context->bitcount[0];

	/* Final transform: */
	sha512_Transform(context->state, context->buffer, context->state);
}

void sha512_Final(SHA512_CTX* context, sha2_byte digest[]) {
	/* If no digest buffer is passed, we don't bother doing this: */
	if (digest != (sha2_byte*)0) {
		sha512_Last(context);

		/* Save the hash data for output: */
#if BYTE_ORDER == LITTLE_ENDIAN
		/* Convert FROM host byte order */
		for (int j = 0; j < 8; j++) {
			REVERSE64(context->state[j],context->state[j]);
		}
#endif
		MEMCPY_BCOPY(digest, context->state, SHA512_DIGEST_LENGTH);
	}

	/* Zero out state data */
	MEMSET_BZERO(context, sizeof(SHA512_CTX));
}

char *sha512_End(SHA512_CTX* context, char buffer[]) {
	sha2_byte	digest[SHA512_DIGEST_LENGTH], *d = digest;
	int		i;

	if (buffer != (char*)0) {
		sha512_Final(context, digest);

		for (i = 0; i < SHA512_DIGEST_LENGTH; i++) {
			*buffer++ = sha2_hex_digits[(*d & 0xf0) >> 4];
			*buffer++ = sha2_hex_digits[*d & 0x0f];
			d++;
		}
		*buffer = (char)0;
	} else {
		MEMSET_BZERO(context, sizeof(SHA512_CTX));
	}
	MEMSET_BZERO(digest, SHA512_DIGEST_LENGTH);
	return buffer;
}

void sha512_Raw(const sha2_byte* data, size_t len, uint8_t digest[SHA512_DIGEST_LENGTH]) {
	SHA512_CTX	context;
	sha512_Init(&context);
	sha512_Update(&context, data, len);
	sha512_Final(&context, digest);
}

char* sha512_Data(const sha2_byte* data, size_t len, char digest[SHA512_DIGEST_STRING_LENGTH]) {
	SHA512_CTX	context;

	sha512_Init(&context);
	sha512_Update(&context, data, len);
	return sha512_End(&context, digest);
}
