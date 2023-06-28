// Implementations of the XChaCha20 + Poly1305 and ChaCha20 + Poly1305
// AEAD constructions with a goal of simplicity and correctness rather
// than performance.

#include "chacha20poly1305.h"

void hchacha20(ECRYPT_ctx *x,u8 *c);

// Initialize the XChaCha20 + Poly1305 context for encryption or decryption
// using a 32 byte key and 24 byte nonce. The key and the first 16 bytes of
// the nonce are used as input to HChaCha20 to derive the Chacha20 key.
void xchacha20poly1305_init(chacha20poly1305_ctx *ctx, uint8_t key[32], uint8_t nonce[24]) {
    unsigned char subkey[32] = {0};
    unsigned char block0[64] = {0};
    ECRYPT_ctx tmp;

    // Generate the Chacha20 key by applying HChaCha20 to the
    // original key and the first 16 bytes of the nonce.
    ECRYPT_keysetup(&tmp, key, 256, 16);
    tmp.input[12] = U8TO32_LITTLE(nonce + 0);
    tmp.input[13] = U8TO32_LITTLE(nonce + 4);
    tmp.input[14] = U8TO32_LITTLE(nonce + 8);
    tmp.input[15] = U8TO32_LITTLE(nonce + 12);
    hchacha20(&tmp, subkey);

    // Initialize Chacha20 with the newly generated key and
    // the last 8 bytes of the nonce.
    ECRYPT_keysetup(&ctx->chacha20, subkey, 256, 16);
    ECRYPT_ivsetup(&ctx->chacha20, nonce+16);

    // Encrypt 64 bytes of zeros and use the first 32 bytes
    // as the Poly1305 key.
    ECRYPT_encrypt_bytes(&ctx->chacha20, block0, block0, 64);
    poly1305_init(&ctx->poly1305, block0);
}

// Encrypt n bytes of plaintext where n must be evenly divisible by the
// Chacha20 blocksize of 64, except for the final n bytes of plaintext.
void chacha20poly1305_encrypt(chacha20poly1305_ctx *ctx, uint8_t *in, uint8_t *out, size_t n) {
    ECRYPT_encrypt_bytes(&ctx->chacha20, in, out, n);
    poly1305_update(&ctx->poly1305, out, n);
}

// Decrypt n bytes of ciphertext where n must be evenly divisible by the
// Chacha20 blocksize of 64, except for the final n bytes of ciphertext.
void chacha20poly1305_decrypt(chacha20poly1305_ctx *ctx, uint8_t *in, uint8_t *out, size_t n) {
    poly1305_update(&ctx->poly1305, in, n);
    ECRYPT_encrypt_bytes(&ctx->chacha20, in, out, n);
}

// Include authenticated data in the Poly1305 MAC.
void chacha20poly1305_auth(chacha20poly1305_ctx *ctx, uint8_t *in, size_t n) {
    poly1305_update(&ctx->poly1305, in, n);
}

// Compute NaCl secretbox-style Poly1305 MAC.
void chacha20poly1305_finish(chacha20poly1305_ctx *ctx, uint8_t mac[16]) {
    poly1305_finish(&ctx->poly1305, mac);
}
