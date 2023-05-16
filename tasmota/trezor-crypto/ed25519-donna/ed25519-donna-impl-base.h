/*
	Timing safe memory compare
*/
int ed25519_verify(const unsigned char *x, const unsigned char *y, size_t len);

/*
	conversions
*/

void ge25519_p1p1_to_partial(ge25519 *r, const ge25519_p1p1 *p);

void ge25519_p1p1_to_full(ge25519 *r, const ge25519_p1p1 *p);

void ge25519_full_to_pniels(ge25519_pniels *p, const ge25519 *r);

/*
	adding & doubling
*/

void ge25519_double_p1p1(ge25519_p1p1 *r, const ge25519 *p);

#ifndef ED25519_NO_PRECOMP
void ge25519_nielsadd2_p1p1(ge25519_p1p1 *r, const ge25519 *p, const ge25519_niels *q, unsigned char signbit);
#endif

void ge25519_pnielsadd_p1p1(ge25519_p1p1 *r, const ge25519 *p, const ge25519_pniels *q, unsigned char signbit);

void ge25519_double_partial(ge25519 *r, const ge25519 *p);

void ge25519_double(ge25519 *r, const ge25519 *p);

void ge25519_nielsadd2(ge25519 *r, const ge25519_niels *q);

void ge25519_pnielsadd(ge25519_pniels *r, const ge25519 *p, const ge25519_pniels *q);


/*
	pack & unpack
*/

void ge25519_pack(unsigned char r[32], const ge25519 *p);

int ge25519_unpack_negative_vartime(ge25519 *r, const unsigned char p[32]);

/*
	scalarmults
*/

void ge25519_set_neutral(ge25519 *r);

/* computes [s1]p1 + [s2]base */
void ge25519_double_scalarmult_vartime(ge25519 *r, const ge25519 *p1, const bignum256modm s1, const bignum256modm s2);

/* computes [s1]p1, constant time */
void ge25519_scalarmult(ge25519 *r, const ge25519 *p1, const bignum256modm s1);

void ge25519_scalarmult_base_choose_niels(ge25519_niels *t, const uint8_t table[256][96], uint32_t pos, signed char b);

/* computes [s]basepoint */
void ge25519_scalarmult_base_niels(ge25519 *r, const uint8_t basepoint_table[256][96], const bignum256modm s);
