#ifndef ED25519_H
#define ED25519_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef unsigned char ed25519_signature[64];
typedef unsigned char ed25519_public_key[32];
typedef unsigned char ed25519_secret_key[32];

typedef unsigned char curve25519_key[32];

typedef unsigned char ed25519_cosi_signature[32];

void ed25519_publickey(const ed25519_secret_key sk, ed25519_public_key pk);

int ed25519_sign_open(const unsigned char *m, size_t mlen, const ed25519_public_key pk, const ed25519_signature RS);
void ed25519_sign(const unsigned char *m, size_t mlen, const ed25519_secret_key sk, const ed25519_public_key pk, ed25519_signature RS);

int ed25519_scalarmult(ed25519_public_key res, const ed25519_secret_key sk, const ed25519_public_key pk);

void curve25519_scalarmult(curve25519_key mypublic, const curve25519_key secret, const curve25519_key basepoint);
void curve25519_scalarmult_basepoint(curve25519_key mypublic, const curve25519_key secret);

#if !defined(__GNUC__) || __GNUC__ > 4
#define CONST const
#else
#define CONST
#endif

int ed25519_cosi_combine_publickeys(ed25519_public_key res, CONST ed25519_public_key *pks, size_t n);
void ed25519_cosi_combine_signatures(ed25519_signature res, const ed25519_public_key R, CONST ed25519_cosi_signature *sigs, size_t n);
void ed25519_cosi_sign(const unsigned char *m, size_t mlen, const ed25519_secret_key key, const ed25519_secret_key nonce, const ed25519_public_key R, const ed25519_public_key pk, ed25519_cosi_signature sig);

#if defined(__cplusplus)
}
#endif

#endif // ED25519_H
