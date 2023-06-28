/* sha3.c - an implementation of Secure Hash Algorithm 3 (Keccak).
 * based on the
 * The Keccak SHA-3 submission. Submission to NIST (Round 3), 2011
 * by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche
 *
 * Copyright: 2013 Aleksey Kravchenko <rhash.admin@gmail.com>
 *
 * Permission is hereby granted,  free of charge,  to any person  obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,  including without limitation
 * the rights to  use, copy, modify,  merge, publish, distribute, sublicense,
 * and/or sell copies  of  the Software,  and to permit  persons  to whom the
 * Software is furnished to do so.
 *
 * This program  is  distributed  in  the  hope  that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  Use this program  at  your own risk!
 */

#include <assert.h>
#include <string.h>

#include "sha3.h"
#include "macros.h"

#define I64(x) x##LL
#define ROTL64(qword, n) ((qword) << (n) ^ ((qword) >> (64 - (n))))
#define le2me_64(x) (x)
#define IS_ALIGNED_64(p) (0 == (7 & ((const char*)(p) - (const char*)0)))
# define me64_to_le_str(to, from, length) memcpy((to), (from), (length))

/* constants */
#define NumberOfRounds 24

/* SHA3 (Keccak) constants for 24 rounds */
static uint64_t keccak_round_constants[NumberOfRounds] = {
	I64(0x0000000000000001), I64(0x0000000000008082), I64(0x800000000000808A), I64(0x8000000080008000),
	I64(0x000000000000808B), I64(0x0000000080000001), I64(0x8000000080008081), I64(0x8000000000008009),
	I64(0x000000000000008A), I64(0x0000000000000088), I64(0x0000000080008009), I64(0x000000008000000A),
	I64(0x000000008000808B), I64(0x800000000000008B), I64(0x8000000000008089), I64(0x8000000000008003),
	I64(0x8000000000008002), I64(0x8000000000000080), I64(0x000000000000800A), I64(0x800000008000000A),
	I64(0x8000000080008081), I64(0x8000000000008080), I64(0x0000000080000001), I64(0x8000000080008008)
};

/* Initializing a sha3 context for given number of output bits */
static void keccak_Init(SHA3_CTX *ctx, unsigned bits)
{
	/* NB: The Keccak capacity parameter = bits * 2 */
	unsigned rate = 1600 - bits * 2;

	memset(ctx, 0, sizeof(SHA3_CTX));
	ctx->block_size = rate / 8;
	assert(rate <= 1600 && (rate % 64) == 0);
}

/**
 * Initialize context before calculating hash.
 *
 * @param ctx context to initialize
 */
void sha3_224_Init(SHA3_CTX *ctx)
{
	keccak_Init(ctx, 224);
}

/**
 * Initialize context before calculating hash.
 *
 * @param ctx context to initialize
 */
void sha3_256_Init(SHA3_CTX *ctx)
{
	keccak_Init(ctx, 256);
}

/**
 * Initialize context before calculating hash.
 *
 * @param ctx context to initialize
 */
void sha3_384_Init(SHA3_CTX *ctx)
{
	keccak_Init(ctx, 384);
}

/**
 * Initialize context before calculating hash.
 *
 * @param ctx context to initialize
 */
void sha3_512_Init(SHA3_CTX *ctx)
{
	keccak_Init(ctx, 512);
}

/* Keccak theta() transformation */
static void keccak_theta(uint64_t *A)
{
	unsigned int x;
	uint64_t C[5], D[5];

	for (x = 0; x < 5; x++) {
		C[x] = A[x] ^ A[x + 5] ^ A[x + 10] ^ A[x + 15] ^ A[x + 20];
	}
	D[0] = ROTL64(C[1], 1) ^ C[4];
	D[1] = ROTL64(C[2], 1) ^ C[0];
	D[2] = ROTL64(C[3], 1) ^ C[1];
	D[3] = ROTL64(C[4], 1) ^ C[2];
	D[4] = ROTL64(C[0], 1) ^ C[3];

	for (x = 0; x < 5; x++) {
		A[x]      ^= D[x];
		A[x + 5]  ^= D[x];
		A[x + 10] ^= D[x];
		A[x + 15] ^= D[x];
		A[x + 20] ^= D[x];
	}
}

/* Keccak pi() transformation */
static void keccak_pi(uint64_t *A)
{
	uint64_t A1;
	A1 = A[1];
	A[ 1] = A[ 6];
	A[ 6] = A[ 9];
	A[ 9] = A[22];
	A[22] = A[14];
	A[14] = A[20];
	A[20] = A[ 2];
	A[ 2] = A[12];
	A[12] = A[13];
	A[13] = A[19];
	A[19] = A[23];
	A[23] = A[15];
	A[15] = A[ 4];
	A[ 4] = A[24];
	A[24] = A[21];
	A[21] = A[ 8];
	A[ 8] = A[16];
	A[16] = A[ 5];
	A[ 5] = A[ 3];
	A[ 3] = A[18];
	A[18] = A[17];
	A[17] = A[11];
	A[11] = A[ 7];
	A[ 7] = A[10];
	A[10] = A1;
	/* note: A[ 0] is left as is */
}

/* Keccak chi() transformation */
static void keccak_chi(uint64_t *A)
{
	int i;
	for (i = 0; i < 25; i += 5) {
		uint64_t A0 = A[0 + i], A1 = A[1 + i];
		A[0 + i] ^= ~A1 & A[2 + i];
		A[1 + i] ^= ~A[2 + i] & A[3 + i];
		A[2 + i] ^= ~A[3 + i] & A[4 + i];
		A[3 + i] ^= ~A[4 + i] & A0;
		A[4 + i] ^= ~A0 & A1;
	}
}

static void sha3_permutation(uint64_t *state)
{
	int round;
	for (round = 0; round < NumberOfRounds; round++)
	{
		keccak_theta(state);

		/* apply Keccak rho() transformation */
		state[ 1] = ROTL64(state[ 1],  1);
		state[ 2] = ROTL64(state[ 2], 62);
		state[ 3] = ROTL64(state[ 3], 28);
		state[ 4] = ROTL64(state[ 4], 27);
		state[ 5] = ROTL64(state[ 5], 36);
		state[ 6] = ROTL64(state[ 6], 44);
		state[ 7] = ROTL64(state[ 7],  6);
		state[ 8] = ROTL64(state[ 8], 55);
		state[ 9] = ROTL64(state[ 9], 20);
		state[10] = ROTL64(state[10],  3);
		state[11] = ROTL64(state[11], 10);
		state[12] = ROTL64(state[12], 43);
		state[13] = ROTL64(state[13], 25);
		state[14] = ROTL64(state[14], 39);
		state[15] = ROTL64(state[15], 41);
		state[16] = ROTL64(state[16], 45);
		state[17] = ROTL64(state[17], 15);
		state[18] = ROTL64(state[18], 21);
		state[19] = ROTL64(state[19],  8);
		state[20] = ROTL64(state[20], 18);
		state[21] = ROTL64(state[21],  2);
		state[22] = ROTL64(state[22], 61);
		state[23] = ROTL64(state[23], 56);
		state[24] = ROTL64(state[24], 14);

		keccak_pi(state);
		keccak_chi(state);

		/* apply iota(state, round) */
		*state ^= keccak_round_constants[round];
	}
}

/**
 * The core transformation. Process the specified block of data.
 *
 * @param hash the algorithm state
 * @param block the message block to process
 * @param block_size the size of the processed block in bytes
 */
static void sha3_process_block(uint64_t hash[25], const uint64_t *block, size_t block_size)
{
	/* expanded loop */
	hash[ 0] ^= le2me_64(block[ 0]);
	hash[ 1] ^= le2me_64(block[ 1]);
	hash[ 2] ^= le2me_64(block[ 2]);
	hash[ 3] ^= le2me_64(block[ 3]);
	hash[ 4] ^= le2me_64(block[ 4]);
	hash[ 5] ^= le2me_64(block[ 5]);
	hash[ 6] ^= le2me_64(block[ 6]);
	hash[ 7] ^= le2me_64(block[ 7]);
	hash[ 8] ^= le2me_64(block[ 8]);
	/* if not sha3-512 */
	if (block_size > 72) {
		hash[ 9] ^= le2me_64(block[ 9]);
		hash[10] ^= le2me_64(block[10]);
		hash[11] ^= le2me_64(block[11]);
		hash[12] ^= le2me_64(block[12]);
		/* if not sha3-384 */
		if (block_size > 104) {
			hash[13] ^= le2me_64(block[13]);
			hash[14] ^= le2me_64(block[14]);
			hash[15] ^= le2me_64(block[15]);
			hash[16] ^= le2me_64(block[16]);
			/* if not sha3-256 */
			if (block_size > 136) {
				hash[17] ^= le2me_64(block[17]);
#ifdef FULL_SHA3_FAMILY_SUPPORT
				/* if not sha3-224 */
				if (block_size > 144) {
					hash[18] ^= le2me_64(block[18]);
					hash[19] ^= le2me_64(block[19]);
					hash[20] ^= le2me_64(block[20]);
					hash[21] ^= le2me_64(block[21]);
					hash[22] ^= le2me_64(block[22]);
					hash[23] ^= le2me_64(block[23]);
					hash[24] ^= le2me_64(block[24]);
				}
#endif
			}
		}
	}
	/* make a permutation of the hash */
	sha3_permutation(hash);
}

#define SHA3_FINALIZED 0x80000000

/**
 * Calculate message hash.
 * Can be called repeatedly with chunks of the message to be hashed.
 *
 * @param ctx the algorithm context containing current hashing state
 * @param msg message chunk
 * @param size length of the message chunk
 */
void sha3_Update(SHA3_CTX *ctx, const unsigned char *msg, size_t size)
{
	size_t idx = (size_t)ctx->rest;
	size_t block_size = (size_t)ctx->block_size;

	if (ctx->rest & SHA3_FINALIZED) return; /* too late for additional input */
	ctx->rest = (unsigned)((ctx->rest + size) % block_size);

	/* fill partial block */
	if (idx) {
		size_t left = block_size - idx;
		memcpy((char*)ctx->message + idx, msg, (size < left ? size : left));
		if (size < left) return;

		/* process partial block */
		sha3_process_block(ctx->hash, ctx->message, block_size);
		msg  += left;
		size -= left;
	}
	while (size >= block_size) {
		uint64_t* aligned_message_block;
		if (IS_ALIGNED_64(msg)) {
			/* the most common case is processing of an already aligned message
			without copying it */
			aligned_message_block = (uint64_t*)(void*)msg;
		} else {
			memcpy(ctx->message, msg, block_size);
			aligned_message_block = ctx->message;
		}

		sha3_process_block(ctx->hash, aligned_message_block, block_size);
		msg  += block_size;
		size -= block_size;
	}
	if (size) {
		memcpy(ctx->message, msg, size); /* save leftovers */
	}
}

/**
 * Store calculated hash into the given array.
 *
 * @param ctx the algorithm context containing current hashing state
 * @param result calculated hash in binary form
 */
void sha3_Final(SHA3_CTX *ctx, unsigned char* result)
{
	size_t digest_length = 100 - ctx->block_size / 2;
	const size_t block_size = ctx->block_size;

	if (!(ctx->rest & SHA3_FINALIZED))
	{
		/* clear the rest of the data queue */
		memset((char*)ctx->message + ctx->rest, 0, block_size - ctx->rest);
		((char*)ctx->message)[ctx->rest] |= 0x06;
		((char*)ctx->message)[block_size - 1] |= 0x80;

		/* process final block */
		sha3_process_block(ctx->hash, ctx->message, block_size);
		ctx->rest = SHA3_FINALIZED; /* mark context as finalized */
	}

	assert(block_size > digest_length);
	if (result) me64_to_le_str(result, ctx->hash, digest_length);
	MEMSET_BZERO(ctx, sizeof(SHA3_CTX));
}

#if USE_KECCAK
/**
* Store calculated hash into the given array.
*
* @param ctx the algorithm context containing current hashing state
* @param result calculated hash in binary form
*/
void keccak_Final(SHA3_CTX *ctx, unsigned char* result)
{
	size_t digest_length = 100 - ctx->block_size / 2;
	const size_t block_size = ctx->block_size;

	if (!(ctx->rest & SHA3_FINALIZED))
	{
		/* clear the rest of the data queue */
		memset((char*)ctx->message + ctx->rest, 0, block_size - ctx->rest);
		((char*)ctx->message)[ctx->rest] |= 0x01;
		((char*)ctx->message)[block_size - 1] |= 0x80;

		/* process final block */
		sha3_process_block(ctx->hash, ctx->message, block_size);
		ctx->rest = SHA3_FINALIZED; /* mark context as finalized */
	}

	assert(block_size > digest_length);
	if (result) me64_to_le_str(result, ctx->hash, digest_length);
	MEMSET_BZERO(ctx, sizeof(SHA3_CTX));
}

void keccak_256(const unsigned char* data, size_t len, unsigned char* digest)
{
	SHA3_CTX ctx;
	keccak_256_Init(&ctx);
	keccak_Update(&ctx, data, len);
	keccak_Final(&ctx, digest);
}

void keccak_512(const unsigned char* data, size_t len, unsigned char* digest)
{
	SHA3_CTX ctx;
	keccak_512_Init(&ctx);
	keccak_Update(&ctx, data, len);
	keccak_Final(&ctx, digest);
}
#endif /* USE_KECCAK */

void sha3_256(const unsigned char* data, size_t len, unsigned char* digest)
{
	SHA3_CTX ctx;
	sha3_256_Init(&ctx);
	sha3_Update(&ctx, data, len);
	sha3_Final(&ctx, digest);
}

void sha3_512(const unsigned char* data, size_t len, unsigned char* digest)
{
	SHA3_CTX ctx;
	sha3_512_Init(&ctx);
	sha3_Update(&ctx, data, len);
	sha3_Final(&ctx, digest);
}
