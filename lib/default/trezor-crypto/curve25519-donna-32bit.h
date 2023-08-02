/*
	Public domain by Andrew M. <liquidsun@gmail.com>
	See: https://github.com/floodyberry/curve25519-donna

	32 bit integer curve25519 implementation
*/

typedef uint32_t bignum25519[10];

/* out = in */
void curve25519_copy(bignum25519 out, const bignum25519 in);

/* out = a + b */
void curve25519_add(bignum25519 out, const bignum25519 a, const bignum25519 b);

void curve25519_add_after_basic(bignum25519 out, const bignum25519 a, const bignum25519 b);

void curve25519_add_reduce(bignum25519 out, const bignum25519 a, const bignum25519 b);

/* out = a - b */
void curve25519_sub(bignum25519 out, const bignum25519 a, const bignum25519 b);

/* out = in * scalar */
void curve25519_scalar_product(bignum25519 out, const bignum25519 in, const uint32_t scalar);

/* out = a - b, where a is the result of a basic op (add,sub) */
void curve25519_sub_after_basic(bignum25519 out, const bignum25519 a, const bignum25519 b);

void curve25519_sub_reduce(bignum25519 out, const bignum25519 a, const bignum25519 b);

/* out = -a */
void curve25519_neg(bignum25519 out, const bignum25519 a);

/* out = a * b */
#define curve25519_mul_noinline curve25519_mul
void curve25519_mul(bignum25519 out, const bignum25519 a, const bignum25519 b);

/* out = in * in */
void curve25519_square(bignum25519 out, const bignum25519 in);

/* out = in ^ (2 * count) */
void curve25519_square_times(bignum25519 out, const bignum25519 in, int count);

/* Take a little-endian, 32-byte number and expand it into polynomial form */
void curve25519_expand(bignum25519 out, const unsigned char in[32]);

/* Take a fully reduced polynomial form number and contract it into a
 * little-endian, 32-byte array
 */
void curve25519_contract(unsigned char out[32], const bignum25519 in);

/* if (iswap) swap(a, b) */
void curve25519_swap_conditional(bignum25519 a, bignum25519 b, uint32_t iswap);
