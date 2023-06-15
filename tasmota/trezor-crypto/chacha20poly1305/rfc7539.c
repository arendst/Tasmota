// Implementation of the ChaCha20 + Poly1305 AEAD construction
// as described in RFC 7539.

#include <string.h>
#include "rfc7539.h"

// Initialize the ChaCha20 + Poly1305 context for encryption or decryption
// using a 32 byte key and 12 byte nonce as in the RFC 7539 style.
void rfc7539_init(chacha20poly1305_ctx *ctx, uint8_t key[32], uint8_t nonce[12]) {
    unsigned char block0[64] = {0};

    ECRYPT_keysetup(&ctx->chacha20, key, 256, 16);
    ctx->chacha20.input[13] = U8TO32_LITTLE(nonce + 0);
    ctx->chacha20.input[14] = U8TO32_LITTLE(nonce + 4);
    ctx->chacha20.input[15] = U8TO32_LITTLE(nonce + 8);

    // Encrypt 64 bytes of zeros and use the first 32 bytes
    // as the Poly1305 key.
    ECRYPT_encrypt_bytes(&ctx->chacha20, block0, block0, 64);
    poly1305_init(&ctx->poly1305, block0);
}

// Include authenticated data in the Poly1305 MAC using the RFC 7539
// style with 16 byte padding. This must only be called once and prior
// to encryption or decryption.
void rfc7539_auth(chacha20poly1305_ctx *ctx, uint8_t *in, size_t n) {
    uint8_t padding[16] = {0};
    poly1305_update(&ctx->poly1305, in, n);
    poly1305_update(&ctx->poly1305, padding, 16 - n%16);
}

// Compute RFC 7539-style Poly1305 MAC.
void rfc7539_finish(chacha20poly1305_ctx *ctx, int64_t alen, int64_t plen, uint8_t mac[16]) {
    uint8_t padding[16] = {0};
    uint8_t lengths[16] = {0};

    memcpy(lengths, &alen, sizeof(int64_t));
    memcpy(lengths + 8, &plen, sizeof(int64_t));

    poly1305_update(&ctx->poly1305, padding, 16 - plen%16);
    poly1305_update(&ctx->poly1305, lengths, 16);

    poly1305_finish(&ctx->poly1305, mac);
}
