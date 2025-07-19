#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#ifdef __cplusplus
extern "C" {
#endif

// BLAKE2S IMPLEMENTATION
#include "crypto/refc/blake2s.h"
#define wireguard_blake2s_ctx blake2s_ctx
#define wireguard_blake2s_init(ctx,outlen,key,keylen) blake2s_init(ctx,outlen,key,keylen)
#define wireguard_blake2s_update(ctx,in,inlen) blake2s_update(ctx,in,inlen)
#define wireguard_blake2s_final(ctx,out) blake2s_final(ctx,out)
#define wireguard_blake2s(out,outlen,key,keylen,in,inlen) blake2s(out,outlen,key,keylen,in,inlen)

// X25519 IMPLEMENTATION
int crypto_scalarmult_curve25519(unsigned char *q, const unsigned char *n,const unsigned char *p);
#define wireguard_x25519(a,b,c) crypto_scalarmult_curve25519(a,b,c)

// CHACHA20POLY1305 IMPLEMENTATION
#include "crypto/refc/chacha20poly1305.h"
#define wireguard_aead_encrypt(dst,src,srclen,ad,adlen,nonce,key) chacha20poly1305_encrypt(dst,src,srclen,ad,adlen,nonce,key)
#define wireguard_aead_decrypt(dst,src,srclen,ad,adlen,nonce,key) chacha20poly1305_decrypt(dst,src,srclen,ad,adlen,nonce,key)
#define wireguard_xaead_encrypt(dst,src,srclen,ad,adlen,nonce,key) xchacha20poly1305_encrypt(dst,src,srclen,ad,adlen,nonce,key)
#define wireguard_xaead_decrypt(dst,src,srclen,ad,adlen,nonce,key) xchacha20poly1305_decrypt(dst,src,srclen,ad,adlen,nonce,key)


// Endian / unaligned helper macros
#define U8C(v) (v##U)
#define U32C(v) (v##U)

#define U8V(v) ((uint8_t)(v) & U8C(0xFF))
#define U32V(v) ((uint32_t)(v) & U32C(0xFFFFFFFF))

#define U8TO32_LITTLE(p) \
  (((uint32_t)((p)[0])      ) | \
   ((uint32_t)((p)[1]) <<  8) | \
   ((uint32_t)((p)[2]) << 16) | \
   ((uint32_t)((p)[3]) << 24))

#define U8TO64_LITTLE(p) \
  (((uint64_t)((p)[0])      ) | \
   ((uint64_t)((p)[1]) <<  8) | \
   ((uint64_t)((p)[2]) << 16) | \
   ((uint64_t)((p)[3]) << 24) | \
   ((uint64_t)((p)[4]) << 32) | \
   ((uint64_t)((p)[5]) << 40) | \
   ((uint64_t)((p)[6]) << 48) | \
   ((uint64_t)((p)[7]) << 56))

#define U16TO8_BIG(p, v) \
  do { \
    (p)[1] = U8V((v)      ); \
    (p)[0] = U8V((v) >>  8); \
  } while (0)

#define U32TO8_LITTLE(p, v) \
  do { \
    (p)[0] = U8V((v)      ); \
    (p)[1] = U8V((v) >>  8); \
    (p)[2] = U8V((v) >> 16); \
    (p)[3] = U8V((v) >> 24); \
  } while (0)

#define U32TO8_BIG(p, v) \
  do { \
    (p)[3] = U8V((v)      ); \
    (p)[2] = U8V((v) >>  8); \
    (p)[1] = U8V((v) >> 16); \
    (p)[0] = U8V((v) >> 24); \
  } while (0)

#define U64TO8_LITTLE(p, v) \
  do { \
    (p)[0] = U8V((v)      ); \
    (p)[1] = U8V((v) >>  8); \
    (p)[2] = U8V((v) >> 16); \
    (p)[3] = U8V((v) >> 24); \
    (p)[4] = U8V((v) >> 32); \
    (p)[5] = U8V((v) >> 40); \
    (p)[6] = U8V((v) >> 48); \
    (p)[7] = U8V((v) >> 56); \
} while (0)

#define U64TO8_BIG(p, v) \
  do { \
    (p)[7] = U8V((v)      ); \
    (p)[6] = U8V((v) >>  8); \
    (p)[5] = U8V((v) >> 16); \
    (p)[4] = U8V((v) >> 24); \
    (p)[3] = U8V((v) >> 32); \
    (p)[2] = U8V((v) >> 40); \
    (p)[1] = U8V((v) >> 48); \
    (p)[0] = U8V((v) >> 56); \
} while (0)


void crypto_zero(void *dest, size_t len);
bool crypto_equal(const void *a, const void *b, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPTO_H_ */

