/*
	Public domain by Andrew M. <liquidsun@gmail.com>

	Ed25519 reference implementation using Ed25519-donna
*/


/* define ED25519_SUFFIX to have it appended to the end of each public function */
#ifdef ED25519_SUFFIX
#define ED25519_FN3(fn,suffix) fn##suffix
#define ED25519_FN2(fn,suffix) ED25519_FN3(fn,suffix)
#define ED25519_FN(fn) ED25519_FN2(fn,ED25519_SUFFIX)
#else
#define ED25519_FN(fn) fn
#endif

#include "ed25519-donna.h"
#include "ed25519.h"

#include "ed25519-hash-custom.h"

/*
	Generates a (extsk[0..31]) and aExt (extsk[32..63])
*/
DONNA_INLINE static void
ed25519_extsk(hash_512bits extsk, const ed25519_secret_key sk) {
	ed25519_hash(extsk, sk, 32);
	extsk[0] &= 248;
	extsk[31] &= 127;
	extsk[31] |= 64;
}

static void
ed25519_hram(hash_512bits hram, const ed25519_signature RS, const ed25519_public_key pk, const unsigned char *m, size_t mlen) {
	ed25519_hash_context ctx;
	ed25519_hash_init(&ctx);
	ed25519_hash_update(&ctx, RS, 32);
	ed25519_hash_update(&ctx, pk, 32);
	ed25519_hash_update(&ctx, m, mlen);
	ed25519_hash_final(&ctx, hram);
}

void
ED25519_FN(ed25519_publickey) (const ed25519_secret_key sk, ed25519_public_key pk) {
	bignum256modm a;
	ge25519 ALIGN(16) A;
	hash_512bits extsk;

	/* A = aB */
	ed25519_extsk(extsk, sk);
	expand256_modm(a, extsk, 32);
	ge25519_scalarmult_base_niels(&A, ge25519_niels_base_multiples, a);
	ge25519_pack(pk, &A);
}

void
ED25519_FN(ed25519_cosi_sign) (const unsigned char *m, size_t mlen, const ed25519_secret_key sk, const ed25519_secret_key nonce, const ed25519_public_key R, const ed25519_public_key pk, ed25519_cosi_signature sig) {
	bignum256modm r, S, a;
	hash_512bits extsk, extnonce, hram;

	ed25519_extsk(extsk, sk);
	ed25519_extsk(extnonce, nonce);

	/* r = nonce */
	expand256_modm(r, extnonce, 32);

	/* S = H(R,A,m).. */
	ed25519_hram(hram, R, pk, m, mlen);
	expand256_modm(S, hram, 64);

	/* S = H(R,A,m)a */
	expand256_modm(a, extsk, 32);
	mul256_modm(S, S, a);

	/* S = (r + H(R,A,m)a) */
	add256_modm(S, S, r);

	/* S = (r + H(R,A,m)a) mod L */
	contract256_modm(sig, S);
}

void
ED25519_FN(ed25519_sign) (const unsigned char *m, size_t mlen, const ed25519_secret_key sk, const ed25519_public_key pk, ed25519_signature RS) {
	ed25519_hash_context ctx;
	bignum256modm r, S, a;
	ge25519 ALIGN(16) R;
	hash_512bits extsk, hashr, hram;

	ed25519_extsk(extsk, sk);

	/* r = H(aExt[32..64], m) */
	ed25519_hash_init(&ctx);
	ed25519_hash_update(&ctx, extsk + 32, 32);
	ed25519_hash_update(&ctx, m, mlen);
	ed25519_hash_final(&ctx, hashr);
	expand256_modm(r, hashr, 64);

	/* R = rB */
	ge25519_scalarmult_base_niels(&R, ge25519_niels_base_multiples, r);
	ge25519_pack(RS, &R);

	/* S = H(R,A,m).. */
	ed25519_hram(hram, RS, pk, m, mlen);
	expand256_modm(S, hram, 64);

	/* S = H(R,A,m)a */
	expand256_modm(a, extsk, 32);
	mul256_modm(S, S, a);

	/* S = (r + H(R,A,m)a) */
	add256_modm(S, S, r);

	/* S = (r + H(R,A,m)a) mod L */
	contract256_modm(RS + 32, S);
}

int
ED25519_FN(ed25519_sign_open) (const unsigned char *m, size_t mlen, const ed25519_public_key pk, const ed25519_signature RS) {
	ge25519 ALIGN(16) R, A;
	hash_512bits hash;
	bignum256modm hram, S;
	unsigned char checkR[32];

	if ((RS[63] & 224) || !ge25519_unpack_negative_vartime(&A, pk))
		return -1;

	/* hram = H(R,A,m) */
	ed25519_hram(hash, RS, pk, m, mlen);
	expand256_modm(hram, hash, 64);

	/* S */
	expand256_modm(S, RS + 32, 32);

	/* SB - H(R,A,m)A */
	ge25519_double_scalarmult_vartime(&R, &A, hram, S);
	ge25519_pack(checkR, &R);

	/* check that R = SB - H(R,A,m)A */
	return ed25519_verify(RS, checkR, 32) ? 0 : -1;
}

int
ED25519_FN(ed25519_scalarmult) (ed25519_public_key res, const ed25519_secret_key sk, const ed25519_public_key pk) {
	bignum256modm a;
	ge25519 ALIGN(16) A, P;
	hash_512bits extsk;

	ed25519_extsk(extsk, sk);
	expand256_modm(a, extsk, 32);

	if (!ge25519_unpack_negative_vartime(&P, pk)) {
		return -1;
	}

	ge25519_scalarmult(&A, &P, a);
	curve25519_neg(A.x, A.x);
	ge25519_pack(res, &A);
	return 0;
}


#ifndef ED25519_SUFFIX

#include "curve25519-donna-scalarmult-base.h"

int
ed25519_cosi_combine_publickeys(ed25519_public_key res, CONST ed25519_public_key *pks, size_t n) {
	size_t i = 0;
	ge25519 P;
	ge25519_pniels sump;
	ge25519_p1p1 sump1;

	if (n == 1) {
		memcpy(res, pks, sizeof(ed25519_public_key));
		return 0;
	}
	if (!ge25519_unpack_negative_vartime(&P, pks[i++])) {
		return -1;
	}
	ge25519_full_to_pniels(&sump, &P);
	while (i < n - 1) {
		if (!ge25519_unpack_negative_vartime(&P, pks[i++])) {
			return -1;
		}
		ge25519_pnielsadd(&sump, &P, &sump);
	}
	if (!ge25519_unpack_negative_vartime(&P, pks[i++])) {
		return -1;
	}
	ge25519_pnielsadd_p1p1(&sump1, &P, &sump, 0);
	ge25519_p1p1_to_partial(&P, &sump1);
	curve25519_neg(P.x, P.x);
	ge25519_pack(res, &P);
	return 0;
}

void
ed25519_cosi_combine_signatures(ed25519_signature res, const ed25519_public_key R, CONST ed25519_cosi_signature *sigs, size_t n) {
	bignum256modm s, t;
	size_t i = 0;

	expand256_modm(s, sigs[i++], 32);
	while (i < n) {
		expand256_modm(t, sigs[i++], 32);
		add256_modm(s, s, t);
	}
	memcpy(res, R, 32);
	contract256_modm(res + 32, s);
}

/*
	Fast Curve25519 basepoint scalar multiplication
*/
void
curve25519_scalarmult_basepoint(curve25519_key pk, const curve25519_key e) {
	curve25519_key ec;
	bignum256modm s;
	bignum25519 ALIGN(16) yplusz, zminusy;
	ge25519 ALIGN(16) p;
	size_t i;

	/* clamp */
	for (i = 0; i < 32; i++) ec[i] = e[i];
	ec[0] &= 248;
	ec[31] &= 127;
	ec[31] |= 64;

	expand_raw256_modm(s, ec);

	/* scalar * basepoint */
	ge25519_scalarmult_base_niels(&p, ge25519_niels_base_multiples, s);

	/* u = (y + z) / (z - y) */
	curve25519_add(yplusz, p.y, p.z);
	curve25519_sub(zminusy, p.z, p.y);
	curve25519_recip(zminusy, zminusy);
	curve25519_mul(yplusz, yplusz, zminusy);
	curve25519_contract(pk, yplusz);
}

void
curve25519_scalarmult(curve25519_key mypublic, const curve25519_key secret, const curve25519_key basepoint) {
	curve25519_key e;
	size_t i;

	for (i = 0;i < 32;++i) e[i] = secret[i];
	e[0] &= 0xf8;
	e[31] &= 0x7f;
	e[31] |= 0x40;
	curve25519_scalarmult_donna(mypublic, e, basepoint);
}

#endif // ED25519_SUFFIX
