#ifndef __BLAKE2B_H__
#define __BLAKE2B_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

enum blake2b_constant
{
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
};

typedef struct __blake2b_state
{
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[BLAKE2B_BLOCKBYTES];
    size_t   buflen;
    size_t   outlen;
    uint8_t  last_node;
} blake2b_state;

#define BLAKE2B_CTX blake2b_state
#define BLAKE2B_BLOCK_LENGTH   BLAKE2B_BLOCKBYTES
#define BLAKE2B_DIGEST_LENGTH  BLAKE2B_OUTBYTES
#define BLAKE2B_KEY_LENGTH     BLAKE2B_KEYBYTES

int blake2b_Init(blake2b_state *S, size_t outlen);
int blake2b_InitKey(blake2b_state *S, size_t outlen, const void *key, size_t keylen);
int blake2b_Update(blake2b_state *S, const void *pin, size_t inlen);
int blake2b_Final(blake2b_state *S, void *out, size_t outlen);

int blake2b(const uint8_t *msg, uint32_t msg_len, void *out, size_t outlen);
int blake2b_Key(const uint8_t *msg, uint32_t msg_len, const void *key, size_t keylen, void *out, size_t outlen);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
