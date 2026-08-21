/*
 * Copyright (c) 2017 Thomas Pornin <pornin@bolet.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be 
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "t_inner.h"

static const unsigned char *
api_generator(int curve, size_t *len)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.generator(curve, len);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.generator(curve, len);
	default:
		return br_ec_prime_i15.generator(curve, len);
	}
}

static const unsigned char *
api_order(int curve, size_t *len)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.order(curve, len);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.order(curve, len);
	default:
		return br_ec_prime_i15.order(curve, len);
	}
}

static size_t
api_xoff(int curve, size_t *len)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.xoff(curve, len);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.xoff(curve, len);
	default:
		return br_ec_prime_i15.xoff(curve, len);
	}
}

static uint32_t
api_mul(unsigned char *G, size_t Glen,
	const unsigned char *kb, size_t kblen, int curve)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.mul(G, Glen, kb, kblen, curve);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.mul(G, Glen, kb, kblen, curve);
	default:
		return br_ec_prime_i15.mul(G, Glen, kb, kblen, curve);
	}
}

static size_t
api_mulgen(unsigned char *R,
	const unsigned char *x, size_t xlen, int curve)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.mulgen(R, x, xlen, curve);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.mulgen(R, x, xlen, curve);
	default:
		return br_ec_prime_i15.mulgen(R, x, xlen, curve);
	}
}

static uint32_t
api_muladd(unsigned char *A, const unsigned char *B, size_t len,
	const unsigned char *x, size_t xlen,
	const unsigned char *y, size_t ylen, int curve)
{
	switch (curve) {
	case BR_EC_secp256r1:
		return br_ec_p256_m15.muladd(A, B, len,
			x, xlen, y, ylen, curve);
	case BR_EC_curve25519:
		return br_ec_c25519_m15.muladd(A, B, len,
			x, xlen, y, ylen, curve);
	default:
		return br_ec_prime_i15.muladd(A, B, len,
			x, xlen, y, ylen, curve);
	}
}

/* see bearssl_ec.h */
/*
 * Tasmota: only advertise curves that fit in BR_MAX_EC_SIZE, since the
 * i15 big-integer buffers (ec_prime_i15, ecdsa_i15_*) are statically
 * sized from it. Advertising a larger curve would let the peer select
 * it and overflow those stack buffers.
 * Curve id bits: secp256r1=23, secp384r1=24, secp521r1=25, curve25519=29
 */
const br_ec_impl br_ec_all_m15 PROGMEM = {
#if BR_MAX_EC_SIZE >= 521
	(uint32_t)0x23800000,	/* P-256, P-384, P-521, Curve25519 */
#elif BR_MAX_EC_SIZE >= 384
	(uint32_t)0x21800000,	/* P-256, P-384, Curve25519 */
#else
	(uint32_t)0x20800000,	/* P-256, Curve25519 */
#endif
	&api_generator,
	&api_order,
	&api_xoff,
	&api_mul,
	&api_mulgen,
	&api_muladd
};
