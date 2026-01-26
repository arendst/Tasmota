// Taken from https://github.com/floodyberry/poly1305-donna - public domain or MIT
#ifndef POLY1305_DONNA_H
#define POLY1305_DONNA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct poly1305_context {
	size_t aligner;
	unsigned char opaque[136];
} poly1305_context;

void poly1305_init(poly1305_context *ctx, const unsigned char key[32]);
void poly1305_update(poly1305_context *ctx, const unsigned char *m, size_t bytes);
void poly1305_finish(poly1305_context *ctx, unsigned char mac[16]);

#ifdef __cplusplus
}
#endif

#endif /* POLY1305_DONNA_H */
