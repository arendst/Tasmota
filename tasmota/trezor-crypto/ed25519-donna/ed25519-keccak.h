#ifndef ED25519_KECCAK_H
#define ED25519_KECCAK_H

#include "ed25519.h"

#if defined(__cplusplus)
extern "C" {
#endif

void ed25519_publickey_keccak(const ed25519_secret_key sk, ed25519_public_key pk);

int ed25519_sign_open_keccak(const unsigned char *m, size_t mlen, const ed25519_public_key pk, const ed25519_signature RS);
void ed25519_sign_keccak(const unsigned char *m, size_t mlen, const ed25519_secret_key sk, const ed25519_public_key pk, ed25519_signature RS);

int ed25519_scalarmult_keccak(ed25519_public_key res, const ed25519_secret_key sk, const ed25519_public_key pk);

#if defined(__cplusplus)
}
#endif

#endif // ED25519_KECCAK_H
