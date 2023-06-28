#ifndef RFC7539_H
#define RFC7539_H

#include "chacha20poly1305.h"

void rfc7539_init(chacha20poly1305_ctx *ctx, uint8_t key[32], uint8_t nonce[12]);
void rfc7539_auth(chacha20poly1305_ctx *ctx, uint8_t *in, size_t n);
void rfc7539_finish(chacha20poly1305_ctx *ctx, int64_t alen, int64_t plen, uint8_t mac[16]);

#endif // RFC7539_H
