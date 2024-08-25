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

#define U      (2 + ((BR_MAX_RSA_FACTOR + 14) / 15))
#define TLEN   (8 * U)

/* see bearssl_rsa.h */
uint32_t
br_rsa_i15_private(unsigned char *x, const br_rsa_private_key *sk)
{
	const unsigned char *p, *q;
	size_t plen, qlen;
	size_t fwlen;
	uint16_t p0i, q0i;
	size_t xlen, u;
	uint16_t tmp[1 + TLEN];
	long z;
	uint16_t *mp, *mq, *s1, *s2, *t1, *t2, *t3;
	uint32_t r;

	/*
	 * Compute the actual lengths of p and q, in bytes.
	 * These lengths are not considered secret (we cannot really hide
	 * them anyway in constant-time code).
	 */
	p = sk->p;
	plen = sk->plen;
	while (plen > 0 && *p == 0) {
		p ++;
		plen --;
	}
	q = sk->q;
	qlen = sk->qlen;
	while (qlen > 0 && *q == 0) {
		q ++;
		qlen --;
	}

	/*
	 * Compute the maximum factor length, in words.
	 */
	z = (long)(plen > qlen ? plen : qlen) << 3;
	fwlen = 1;
	while (z > 0) {
		z -= 15;
		fwlen ++;
	}
	/*
	 * Round up the word length to an even number.
	 */
	fwlen += (fwlen & 1);

	/*
	 * We need to fit at least 6 values in the stack buffer.
	 */
	if (6 * fwlen > TLEN) {
		return 0;
	}

	/*
	 * Compute signature length (in bytes).
	 */
	xlen = (sk->n_bitlen + 7) >> 3;

	/*
	 * Ensure 32-bit alignment for value words.
	 */
	mq = tmp;
	if (((uintptr_t)mq & 2) == 0) {
		mq ++;
	}

	/*
	 * Decode q.
	 */
	br_i15_decode(mq, q, qlen);

	/*
	 * Decode p.
	 */
	t1 = mq + fwlen;
	br_i15_decode(t1, p, plen);

	/*
	 * Compute the modulus (product of the two factors), to compare
	 * it with the source value. We use br_i15_mulacc(), since it's
	 * already used later on.
	 */
	t2 = mq + 2 * fwlen;
	br_i15_zero(t2, mq[0]);
	br_i15_mulacc(t2, mq, t1);

	/*
	 * We encode the modulus into bytes, to perform the comparison
	 * with bytes. We know that the product length, in bytes, is
	 * exactly xlen.
	 * The comparison actually computes the carry when subtracting
	 * the modulus from the source value; that carry must be 1 for
	 * a value in the correct range. We keep it in r, which is our
	 * accumulator for the error code.
	 */
	t3 = mq + 4 * fwlen;
	br_i15_encode(t3, xlen, t2);
	u = xlen;
	r = 0;
	while (u > 0) {
		uint32_t wn, wx;

		u --;
		wn = ((unsigned char *)t3)[u];
		wx = x[u];
		r = ((wx - (wn + r)) >> 8) & 1;
	}

	/*
	 * Move the decoded p to another temporary buffer.
	 */
	mp = mq + 2 * fwlen;
	memmove(mp, t1, fwlen * sizeof *t1);

	stack_thunk_yield();

	/*
	 * Compute s2 = x^dq mod q.
	 */
	q0i = br_i15_ninv15(mq[1]);
	s2 = mq + fwlen;
	br_i15_decode_reduce(s2, x, xlen, mq);
	r &= br_i15_modpow_opt(s2, sk->dq, sk->dqlen, mq, q0i,
		mq + 3 * fwlen, TLEN - 3 * fwlen);

	stack_thunk_yield();

	/*
	 * Compute s1 = x^dq mod q.
	 */
	p0i = br_i15_ninv15(mp[1]);
	s1 = mq + 3 * fwlen;
	br_i15_decode_reduce(s1, x, xlen, mp);
	r &= br_i15_modpow_opt(s1, sk->dp, sk->dplen, mp, p0i,
		mq + 4 * fwlen, TLEN - 4 * fwlen);

	/*
	 * Compute:
	 *   h = (s1 - s2)*(1/q) mod p
	 * s1 is an integer modulo p, but s2 is modulo q. PKCS#1 is
	 * unclear about whether p may be lower than q (some existing,
	 * widely deployed implementations of RSA don't tolerate p < q),
	 * but we want to support that occurrence, so we need to use the
	 * reduction function.
	 *
	 * Since we use br_i15_decode_reduce() for iq (purportedly, the
	 * inverse of q modulo p), we also tolerate improperly large
	 * values for this parameter.
	 */
	t1 = mq + 4 * fwlen;
	t2 = mq + 5 * fwlen;
	br_i15_reduce(t2, s2, mp);
	br_i15_add(s1, mp, br_i15_sub(s1, t2, 1));
	br_i15_to_monty(s1, mp);
	br_i15_decode_reduce(t1, sk->iq, sk->iqlen, mp);
	br_i15_montymul(t2, s1, t1, mp, p0i);

	stack_thunk_yield();

	/*
	 * h is now in t2. We compute the final result:
	 *   s = s2 + q*h
	 * All these operations are non-modular.
	 *
	 * We need mq, s2 and t2. We use the t3 buffer as destination.
	 * The buffers mp, s1 and t1 are no longer needed, so we can
	 * reuse them for t3. Moreover, the first step of the computation
	 * is to copy s2 into t3, after which s2 is not needed. Right
	 * now, mq is in slot 0, s2 is in slot 1, and t2 in slot 5.
	 * Therefore, we have ample room for t3 by simply using s2.
	 */
	t3 = s2;
	br_i15_mulacc(t3, mq, t2);

	/*
	 * Encode the result. Since we already checked the value of xlen,
	 * we can just use it right away.
	 */
	br_i15_encode(x, xlen, t3);

	/*
	 * The only error conditions remaining at that point are invalid
	 * values for p and q (even integers).
	 */
	return p0i & q0i & r;
}
