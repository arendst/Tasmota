/*
	Public domain by Andrew M. <liquidsun@gmail.com>
	See: https://github.com/floodyberry/curve25519-donna

	Curve25519 implementation agnostic helpers
*/

/*
 * In:  b =   2^5 - 2^0
 * Out: b = 2^250 - 2^0
 */
void curve25519_pow_two5mtwo0_two250mtwo0(bignum25519 b);

/*
 * z^(p - 2) = z(2^255 - 21)
 */
void curve25519_recip(bignum25519 out, const bignum25519 z);

/*
 * z^((p-5)/8) = z^(2^252 - 3)
 */
void curve25519_pow_two252m3(bignum25519 two252m3, const bignum25519 z);
