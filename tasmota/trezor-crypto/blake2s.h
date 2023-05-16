#ifndef __BLAKE2S_H__
#define __BLAKE2S_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

enum blake2s_constant
{
    BLAKE2S_BLOCKBYTES = 64,
    BLAKE2S_OUTBYTES   = 32,
    BLAKE2S_KEYBYTES   = 32,
    BLAKE2S_SALTBYTES  = 8,
    BLAKE2S_PERSONALBYTES = 8
};

typedef struct __blake2s_state
{
    uint32_t h[8];
    uint32_t t[2];
    uint32_t f[2];
    uint8_t  buf[BLAKE2S_BLOCKBYTES];
    uint32_t buflen;
    uint8_t  outlen;
    uint8_t  last_node;
} blake2s_state;

#define BLAKE2S_CTX blake2s_state
#define BLAKE2S_BLOCK_LENGTH   BLAKE2S_BLOCKBYTES
#define BLAKE2S_DIGEST_LENGTH  BLAKE2S_OUTBYTES
#define BLAKE2S_KEY_LENGTH     BLAKE2S_KEYBYTES

int blake2s_Init(blake2s_state *S, size_t outlen);
int blake2s_InitKey(blake2s_state *S, size_t outlen, const void *key, size_t keylen);
int blake2s_Update(blake2s_state *S, const void *pin, size_t inlen);
int blake2s_Final(blake2s_state *S, void *out, size_t outlen);

int blake2s(const uint8_t *msg, uint32_t msg_len, void *out, size_t outlen);
int blake2s_Key(const uint8_t *msg, uint32_t msg_len, const void *key, size_t keylen, void *out, size_t outlen);

#ifdef __cplusplus
} /* end of extern "C" */
#endif
    
#endif
