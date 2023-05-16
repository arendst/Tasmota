/*
---------------------------------------------------------------------------
Copyright (c) 1998-2013, Brian Gladman, Worcester, UK. All rights reserved.

The redistribution and use of this software (with or without changes)
is allowed without the payment of fees or royalties provided that:

  source code distributions include the above copyright notice, this
  list of conditions and the following disclaimer;

  binary distributions include the above copyright notice, this list
  of conditions and the following disclaimer in their documentation.

This software is provided 'as is' with no explicit or implied warranties
in respect of its operation, including, but not limited to, correctness
and fitness for purpose.
---------------------------------------------------------------------------
Issue Date: 20/12/2007

 This file contains the definitions required to use AES in C. See aesopt.h
 for optimisation details.
*/

#ifndef _AES_H
#define _AES_H

#include <stdlib.h>

/*  This include is used to find 8 & 32 bit unsigned integer types  */
#include "./brg_types.h"

#if defined(__cplusplus)
extern "C"
{
#endif

// #define AES_128     /* if a fast 128 bit key scheduler is needed    */
// #define AES_192     /* if a fast 192 bit key scheduler is needed    */
#define AES_256     /* if a fast 256 bit key scheduler is needed    */
// #define AES_VAR     /* if variable key size scheduler is needed     */
#define AES_MODES   /* if support is needed for modes               */

/* The following must also be set in assembler files if being used  */

#define AES_ENCRYPT /* if support for encryption is needed          */
#define AES_DECRYPT /* if support for decryption is needed          */

#define AES_BLOCK_SIZE  16  /* the AES block size in bytes          */
#define N_COLS           4  /* the number of columns in the state   */

/* The key schedule length is 11, 13 or 15 16-byte blocks for 128,  */
/* 192 or 256-bit keys respectively. That is 176, 208 or 240 bytes  */
/* or 44, 52 or 60 32-bit words.                                    */

#if defined( AES_VAR ) || defined( AES_256 )
#define KS_LENGTH       60
#elif defined( AES_192 )
#define KS_LENGTH       52
#else
#define KS_LENGTH       44
#endif

#define AES_RETURN INT_RETURN

/* the character array 'inf' in the following structures is used    */
/* to hold AES context information. This AES code uses cx->inf.b[0] */
/* to hold the number of rounds multiplied by 16. The other three   */
/* elements can be used by code that implements additional modes    */

typedef union
{   uint32_t l;
    uint8_t b[4];
} aes_inf;

#ifdef _MSC_VER
#  pragma warning( disable : 4324 )
#endif

#if defined(_MSC_VER) && defined(_WIN64)
#define ALIGNED_(x) __declspec(align(x))
#elif defined(__GNUC__) && defined(__x86_64__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#else
#define ALIGNED_(x)
#endif

typedef struct ALIGNED_(16)
{   uint32_t ks[KS_LENGTH];
    aes_inf inf;
} trzr_aes_encrypt_ctx;

typedef struct ALIGNED_(16)
{   uint32_t ks[KS_LENGTH];
    aes_inf inf;
} trzr_aes_decrypt_ctx;

#ifdef _MSC_VER
#  pragma warning( default : 4324 )
#endif

/* This routine must be called before first use if non-static       */
/* tables are being used                                            */

AES_RETURN aes_init(void);

/* Key lengths in the range 16 <= key_len <= 32 are given in bytes, */
/* those in the range 128 <= key_len <= 256 are given in bits       */

#if defined( AES_ENCRYPT )

#if defined( AES_128 ) || defined( AES_VAR)
AES_RETURN trzr_aes_encrypt_key128(const unsigned char *key, trzr_aes_encrypt_ctx cx[1]);
#endif

#if defined( AES_192 ) || defined( AES_VAR)
AES_RETURN trzr_aes_encrypt_key192(const unsigned char *key, trzr_aes_encrypt_ctx cx[1]);
#endif

#if defined( AES_256 ) || defined( AES_VAR)
AES_RETURN trzr_aes_encrypt_key256(const unsigned char *key, trzr_aes_encrypt_ctx cx[1]);
#endif

#if defined( AES_VAR )
AES_RETURN trzr_aes_encrypt_key(const unsigned char *key, int key_len, trzr_aes_encrypt_ctx cx[1]);
#endif

AES_RETURN trzr_aes_encrypt(const unsigned char *in, unsigned char *out, const trzr_aes_encrypt_ctx cx[1]);

#endif

#if defined( AES_DECRYPT )

#if defined( AES_128 ) || defined( AES_VAR)
AES_RETURN trzr_aes_decrypt_key128(const unsigned char *key, trzr_aes_decrypt_ctx cx[1]);
#endif

#if defined( AES_192 ) || defined( AES_VAR)
AES_RETURN trzr_aes_decrypt_key192(const unsigned char *key, trzr_aes_decrypt_ctx cx[1]);
#endif

#if defined( AES_256 ) || defined( AES_VAR)
AES_RETURN trzr_aes_decrypt_key256(const unsigned char *key, trzr_aes_decrypt_ctx cx[1]);
#endif

#if defined( AES_VAR )
AES_RETURN trzr_aes_decrypt_key(const unsigned char *key, int key_len, trzr_aes_decrypt_ctx cx[1]);
#endif

AES_RETURN trzr_aes_decrypt(const unsigned char *in, unsigned char *out, const trzr_aes_decrypt_ctx cx[1]);

#endif

#if defined( AES_MODES )

/* Multiple calls to the following subroutines for multiple block   */
/* ECB, CBC, CFB, OFB and CTR mode encryption can be used to handle */
/* long messages incrementally provided that the context AND the iv */
/* are preserved between all such calls.  For the ECB and CBC modes */
/* each individual call within a series of incremental calls must   */
/* process only full blocks (i.e. len must be a multiple of 16) but */
/* the CFB, OFB and CTR mode calls can handle multiple incremental  */
/* calls of any length.  Each mode is reset when a new AES key is   */
/* set but ECB needs no reset and CBC can be reset without setting  */
/* a new key by setting a new IV value.  To reset CFB, OFB and CTR  */
/* without setting the key, aes_mode_reset() must be called and the */
/* IV must be set.  NOTE: All these calls update the IV on exit so  */
/* this has to be reset if a new operation with the same IV as the  */
/* previous one is required (or decryption follows encryption with  */
/* the same IV array).                                              */

AES_RETURN aes_test_alignment_detection(unsigned int n);

AES_RETURN aes_ecb_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, const trzr_aes_encrypt_ctx cx[1]);

AES_RETURN aes_ecb_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, const trzr_aes_decrypt_ctx cx[1]);

AES_RETURN aes_cbc_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, const trzr_aes_encrypt_ctx cx[1]);

AES_RETURN aes_cbc_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, const trzr_aes_decrypt_ctx cx[1]);

AES_RETURN aes_mode_reset(trzr_aes_encrypt_ctx cx[1]);

AES_RETURN aes_cfb_encrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, trzr_aes_encrypt_ctx cx[1]);

AES_RETURN aes_cfb_decrypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, trzr_aes_encrypt_ctx cx[1]);

#define aes_ofb_encrypt aes_ofb_crypt
#define aes_ofb_decrypt aes_ofb_crypt

AES_RETURN aes_ofb_crypt(const unsigned char *ibuf, unsigned char *obuf,
                    int len, unsigned char *iv, trzr_aes_encrypt_ctx cx[1]);

typedef void cbuf_inc(unsigned char *cbuf);

#define aes_ctr_encrypt aes_ctr_crypt
#define aes_ctr_decrypt aes_ctr_crypt

AES_RETURN aes_ctr_crypt(const unsigned char *ibuf, unsigned char *obuf,
            int len, unsigned char *cbuf, cbuf_inc ctr_inc, trzr_aes_encrypt_ctx cx[1]);

void aes_ctr_cbuf_inc(unsigned char *cbuf);

#endif

#if defined(__cplusplus)
}
#endif

#endif
