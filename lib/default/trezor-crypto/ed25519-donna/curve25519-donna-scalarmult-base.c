#include "ed25519-donna.h"
#include "ed25519.h"

/* Calculates nQ where Q is the x-coordinate of a point on the curve
 *
 *   mypublic: the packed little endian x coordinate of the resulting curve point
 *   n: a little endian, 32-byte number
 *   basepoint: a packed little endian point of the curve
 */

void curve25519_scalarmult_donna(curve25519_key mypublic, const curve25519_key n, const curve25519_key basepoint) {
	bignum25519 nqpqx = {1}, nqpqz = {0}, nqz = {1}, nqx;
	bignum25519 q, qx, qpqx, qqx, zzz, zmone;
	size_t bit, lastbit;
	int32_t i;

	curve25519_expand(q, basepoint);
	curve25519_copy(nqx, q);

	/* bit 255 is always 0, and bit 254 is always 1, so skip bit 255 and
	   start pre-swapped on bit 254 */
	lastbit = 1;

	/* we are doing bits 254..3 in the loop, but are swapping in bits 253..2 */
	for (i = 253; i >= 2; i--) {
		curve25519_add(qx, nqx, nqz);
		curve25519_sub(nqz, nqx, nqz);
		curve25519_add(qpqx, nqpqx, nqpqz);
		curve25519_sub(nqpqz, nqpqx, nqpqz);
		curve25519_mul(nqpqx, qpqx, nqz);
		curve25519_mul(nqpqz, qx, nqpqz);
		curve25519_add(qqx, nqpqx, nqpqz);
		curve25519_sub(nqpqz, nqpqx, nqpqz);
		curve25519_square(nqpqz, nqpqz);
		curve25519_square(nqpqx, qqx);
		curve25519_mul(nqpqz, nqpqz, q);
		curve25519_square(qx, qx);
		curve25519_square(nqz, nqz);
		curve25519_mul(nqx, qx, nqz);
		curve25519_sub(nqz, qx, nqz);
		curve25519_scalar_product(zzz, nqz, 121665);
		curve25519_add(zzz, zzz, qx);
		curve25519_mul(nqz, nqz, zzz);

		bit = (n[i/8] >> (i & 7)) & 1;
		curve25519_swap_conditional(nqx, nqpqx, bit ^ lastbit);
		curve25519_swap_conditional(nqz, nqpqz, bit ^ lastbit);
		lastbit = bit;
	}

	/* the final 3 bits are always zero, so we only need to double */
	for (i = 0; i < 3; i++) {
		curve25519_add(qx, nqx, nqz);
		curve25519_sub(nqz, nqx, nqz);
		curve25519_square(qx, qx);
		curve25519_square(nqz, nqz);
		curve25519_mul(nqx, qx, nqz);
		curve25519_sub(nqz, qx, nqz);
		curve25519_scalar_product(zzz, nqz, 121665);
		curve25519_add(zzz, zzz, qx);
		curve25519_mul(nqz, nqz, zzz);
	}

	curve25519_recip(zmone, nqz);
	curve25519_mul(nqz, nqx, zmone);
	curve25519_contract(mypublic, nqz);
}
