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
*/

#include "aesopt.h"
#include "aestab.h"

#if defined( USE_INTEL_AES_IF_PRESENT )
#  include "aes_ni.h"
#else
/* map names here to provide the external API ('name' -> 'aes_name') */
#  define aes_xi(x) aes_ ## x
#endif

#ifdef USE_VIA_ACE_IF_PRESENT
#  include "aes_via_ace.h"
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

/* Initialise the key schedule from the user supplied key. The key
   length can be specified in bytes, with legal values of 16, 24
   and 32, or in bits, with legal values of 128, 192 and 256. These
   values correspond with Nk values of 4, 6 and 8 respectively.

   The following macros implement a single cycle in the key
   schedule generation process. The number of cycles needed
   for each cx->n_col and nk value is:

    nk =             4  5  6  7  8
    ------------------------------
    cx->n_col = 4   10  9  8  7  7
    cx->n_col = 5   14 11 10  9  9
    cx->n_col = 6   19 15 12 11 11
    cx->n_col = 7   21 19 16 13 14
    cx->n_col = 8   29 23 19 17 14
*/

#if defined( REDUCE_CODE_SIZE )
#  define ls_box ls_sub
   uint32_t ls_sub(const uint32_t t, const uint32_t n);
#  define inv_mcol im_sub
   uint32_t im_sub(const uint32_t x);
#  ifdef ENC_KS_UNROLL
#    undef ENC_KS_UNROLL
#  endif
#  ifdef DEC_KS_UNROLL
#    undef DEC_KS_UNROLL
#  endif
#endif

#if (FUNCS_IN_C & ENC_KEYING_IN_C)

#if defined(AES_128) || defined( AES_VAR )

#define ke4(k,i) \
{   k[4*(i)+4] = ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; \
    k[4*(i)+5] = ss[1] ^= ss[0]; \
    k[4*(i)+6] = ss[2] ^= ss[1]; \
    k[4*(i)+7] = ss[3] ^= ss[2]; \
}

AES_RETURN aes_xi(encrypt_key128)(const unsigned char *key, trzr_aes_encrypt_ctx cx[1])
{   uint32_t    ss[4];

    cx->ks[0] = ss[0] = word_in(key, 0);
    cx->ks[1] = ss[1] = word_in(key, 1);
    cx->ks[2] = ss[2] = word_in(key, 2);
    cx->ks[3] = ss[3] = word_in(key, 3);

#ifdef ENC_KS_UNROLL
    ke4(cx->ks, 0);  ke4(cx->ks, 1);
    ke4(cx->ks, 2);  ke4(cx->ks, 3);
    ke4(cx->ks, 4);  ke4(cx->ks, 5);
    ke4(cx->ks, 6);  ke4(cx->ks, 7);
    ke4(cx->ks, 8);
#else
    {   uint32_t i;
        for(i = 0; i < 9; ++i)
            ke4(cx->ks, i);
    }
#endif
    ke4(cx->ks, 9);
    cx->inf.l = 0;
    cx->inf.b[0] = 10 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#if defined(AES_192) || defined( AES_VAR )

#define kef6(k,i) \
{   k[6*(i)+ 6] = ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; \
    k[6*(i)+ 7] = ss[1] ^= ss[0]; \
    k[6*(i)+ 8] = ss[2] ^= ss[1]; \
    k[6*(i)+ 9] = ss[3] ^= ss[2]; \
}

#define ke6(k,i) \
{   kef6(k,i); \
    k[6*(i)+10] = ss[4] ^= ss[3]; \
    k[6*(i)+11] = ss[5] ^= ss[4]; \
}

AES_RETURN aes_xi(encrypt_key192)(const unsigned char *key, trzr_aes_encrypt_ctx cx[1])
{   uint32_t    ss[6];

	cx->ks[0] = ss[0] = word_in(key, 0);
    cx->ks[1] = ss[1] = word_in(key, 1);
    cx->ks[2] = ss[2] = word_in(key, 2);
    cx->ks[3] = ss[3] = word_in(key, 3);
    cx->ks[4] = ss[4] = word_in(key, 4);
    cx->ks[5] = ss[5] = word_in(key, 5);

#ifdef ENC_KS_UNROLL
    ke6(cx->ks, 0);  ke6(cx->ks, 1);
    ke6(cx->ks, 2);  ke6(cx->ks, 3);
    ke6(cx->ks, 4);  ke6(cx->ks, 5);
    ke6(cx->ks, 6);
#else
    {   uint32_t i;
        for(i = 0; i < 7; ++i)
            ke6(cx->ks, i);
    }
#endif
    kef6(cx->ks, 7);
    cx->inf.l = 0;
    cx->inf.b[0] = 12 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#if defined(AES_256) || defined( AES_VAR )

#define kef8(k,i) \
{   k[8*(i)+ 8] = ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; \
    k[8*(i)+ 9] = ss[1] ^= ss[0]; \
    k[8*(i)+10] = ss[2] ^= ss[1]; \
    k[8*(i)+11] = ss[3] ^= ss[2]; \
}

#define ke8(k,i) \
{   kef8(k,i); \
    k[8*(i)+12] = ss[4] ^= ls_box(ss[3],0); \
    k[8*(i)+13] = ss[5] ^= ss[4]; \
    k[8*(i)+14] = ss[6] ^= ss[5]; \
    k[8*(i)+15] = ss[7] ^= ss[6]; \
}

AES_RETURN aes_xi(encrypt_key256)(const unsigned char *key, trzr_aes_encrypt_ctx cx[1])
{   uint32_t    ss[8];

    cx->ks[0] = ss[0] = word_in(key, 0);
    cx->ks[1] = ss[1] = word_in(key, 1);
    cx->ks[2] = ss[2] = word_in(key, 2);
    cx->ks[3] = ss[3] = word_in(key, 3);
    cx->ks[4] = ss[4] = word_in(key, 4);
    cx->ks[5] = ss[5] = word_in(key, 5);
    cx->ks[6] = ss[6] = word_in(key, 6);
    cx->ks[7] = ss[7] = word_in(key, 7);

#ifdef ENC_KS_UNROLL
    ke8(cx->ks, 0); ke8(cx->ks, 1);
    ke8(cx->ks, 2); ke8(cx->ks, 3);
    ke8(cx->ks, 4); ke8(cx->ks, 5);
#else
    {   uint32_t i;
        for(i = 0; i < 6; ++i)
            ke8(cx->ks,  i);
    }
#endif
    kef8(cx->ks, 6);
    cx->inf.l = 0;
    cx->inf.b[0] = 14 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#endif

#if (FUNCS_IN_C & DEC_KEYING_IN_C)

/* this is used to store the decryption round keys  */
/* in forward or reverse order                      */

#ifdef AES_REV_DKS
#define v(n,i)  ((n) - (i) + 2 * ((i) & 3))
#else
#define v(n,i)  (i)
#endif

#if DEC_ROUND == NO_TABLES
#define ff(x)   (x)
#else
#define ff(x)   inv_mcol(x)
#if defined( dec_imvars )
#define d_vars  dec_imvars
#endif
#endif

#if defined(AES_128) || defined( AES_VAR )

#define k4e(k,i) \
{   k[v(40,(4*(i))+4)] = ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; \
    k[v(40,(4*(i))+5)] = ss[1] ^= ss[0]; \
    k[v(40,(4*(i))+6)] = ss[2] ^= ss[1]; \
    k[v(40,(4*(i))+7)] = ss[3] ^= ss[2]; \
}

#if 1

#define kdf4(k,i) \
{   ss[0] = ss[0] ^ ss[2] ^ ss[1] ^ ss[3]; \
    ss[1] = ss[1] ^ ss[3]; \
    ss[2] = ss[2] ^ ss[3]; \
    ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; \
    ss[i % 4] ^= ss[4]; \
    ss[4] ^= k[v(40,(4*(i)))];   k[v(40,(4*(i))+4)] = ff(ss[4]); \
    ss[4] ^= k[v(40,(4*(i))+1)]; k[v(40,(4*(i))+5)] = ff(ss[4]); \
    ss[4] ^= k[v(40,(4*(i))+2)]; k[v(40,(4*(i))+6)] = ff(ss[4]); \
    ss[4] ^= k[v(40,(4*(i))+3)]; k[v(40,(4*(i))+7)] = ff(ss[4]); \
}

#define kd4(k,i) \
{   ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; \
    ss[i % 4] ^= ss[4]; ss[4] = ff(ss[4]); \
    k[v(40,(4*(i))+4)] = ss[4] ^= k[v(40,(4*(i)))]; \
    k[v(40,(4*(i))+5)] = ss[4] ^= k[v(40,(4*(i))+1)]; \
    k[v(40,(4*(i))+6)] = ss[4] ^= k[v(40,(4*(i))+2)]; \
    k[v(40,(4*(i))+7)] = ss[4] ^= k[v(40,(4*(i))+3)]; \
}

#define kdl4(k,i) \
{   ss[4] = ls_box(ss[(i+3) % 4], 3) ^ t_use(r,c)[i]; ss[i % 4] ^= ss[4]; \
    k[v(40,(4*(i))+4)] = (ss[0] ^= ss[1]) ^ ss[2] ^ ss[3]; \
    k[v(40,(4*(i))+5)] = ss[1] ^ ss[3]; \
    k[v(40,(4*(i))+6)] = ss[0]; \
    k[v(40,(4*(i))+7)] = ss[1]; \
}

#else

#define kdf4(k,i) \
{   ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[v(40,(4*(i))+ 4)] = ff(ss[0]); \
    ss[1] ^= ss[0]; k[v(40,(4*(i))+ 5)] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[v(40,(4*(i))+ 6)] = ff(ss[2]); \
    ss[3] ^= ss[2]; k[v(40,(4*(i))+ 7)] = ff(ss[3]); \
}

#define kd4(k,i) \
{   ss[4] = ls_box(ss[3],3) ^ t_use(r,c)[i]; \
    ss[0] ^= ss[4]; ss[4] = ff(ss[4]); k[v(40,(4*(i))+ 4)] = ss[4] ^= k[v(40,(4*(i)))]; \
    ss[1] ^= ss[0]; k[v(40,(4*(i))+ 5)] = ss[4] ^= k[v(40,(4*(i))+ 1)]; \
    ss[2] ^= ss[1]; k[v(40,(4*(i))+ 6)] = ss[4] ^= k[v(40,(4*(i))+ 2)]; \
    ss[3] ^= ss[2]; k[v(40,(4*(i))+ 7)] = ss[4] ^= k[v(40,(4*(i))+ 3)]; \
}

#define kdl4(k,i) \
{   ss[0] ^= ls_box(ss[3],3) ^ t_use(r,c)[i]; k[v(40,(4*(i))+ 4)] = ss[0]; \
    ss[1] ^= ss[0]; k[v(40,(4*(i))+ 5)] = ss[1]; \
    ss[2] ^= ss[1]; k[v(40,(4*(i))+ 6)] = ss[2]; \
    ss[3] ^= ss[2]; k[v(40,(4*(i))+ 7)] = ss[3]; \
}

#endif

AES_RETURN aes_xi(decrypt_key128)(const unsigned char *key, trzr_aes_decrypt_ctx cx[1])
{   uint32_t    ss[5];
#if defined( d_vars )
        d_vars;
#endif

	cx->ks[v(40,(0))] = ss[0] = word_in(key, 0);
    cx->ks[v(40,(1))] = ss[1] = word_in(key, 1);
    cx->ks[v(40,(2))] = ss[2] = word_in(key, 2);
    cx->ks[v(40,(3))] = ss[3] = word_in(key, 3);

#ifdef DEC_KS_UNROLL
     kdf4(cx->ks, 0); kd4(cx->ks, 1);
     kd4(cx->ks, 2);  kd4(cx->ks, 3);
     kd4(cx->ks, 4);  kd4(cx->ks, 5);
     kd4(cx->ks, 6);  kd4(cx->ks, 7);
     kd4(cx->ks, 8);  kdl4(cx->ks, 9);
#else
    {   uint32_t i;
        for(i = 0; i < 10; ++i)
            k4e(cx->ks, i);
#if !(DEC_ROUND == NO_TABLES)
        for(i = N_COLS; i < 10 * N_COLS; ++i)
            cx->ks[i] = inv_mcol(cx->ks[i]);
#endif
    }
#endif
    cx->inf.l = 0;
    cx->inf.b[0] = 10 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#if defined(AES_192) || defined( AES_VAR )

#define k6ef(k,i) \
{   k[v(48,(6*(i))+ 6)] = ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; \
    k[v(48,(6*(i))+ 7)] = ss[1] ^= ss[0]; \
    k[v(48,(6*(i))+ 8)] = ss[2] ^= ss[1]; \
    k[v(48,(6*(i))+ 9)] = ss[3] ^= ss[2]; \
}

#define k6e(k,i) \
{   k6ef(k,i); \
    k[v(48,(6*(i))+10)] = ss[4] ^= ss[3]; \
    k[v(48,(6*(i))+11)] = ss[5] ^= ss[4]; \
}

#define kdf6(k,i) \
{   ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[v(48,(6*(i))+ 6)] = ff(ss[0]); \
    ss[1] ^= ss[0]; k[v(48,(6*(i))+ 7)] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[v(48,(6*(i))+ 8)] = ff(ss[2]); \
    ss[3] ^= ss[2]; k[v(48,(6*(i))+ 9)] = ff(ss[3]); \
    ss[4] ^= ss[3]; k[v(48,(6*(i))+10)] = ff(ss[4]); \
    ss[5] ^= ss[4]; k[v(48,(6*(i))+11)] = ff(ss[5]); \
}

#define kd6(k,i) \
{   ss[6] = ls_box(ss[5],3) ^ t_use(r,c)[i]; \
    ss[0] ^= ss[6]; ss[6] = ff(ss[6]); k[v(48,(6*(i))+ 6)] = ss[6] ^= k[v(48,(6*(i)))]; \
    ss[1] ^= ss[0]; k[v(48,(6*(i))+ 7)] = ss[6] ^= k[v(48,(6*(i))+ 1)]; \
    ss[2] ^= ss[1]; k[v(48,(6*(i))+ 8)] = ss[6] ^= k[v(48,(6*(i))+ 2)]; \
    ss[3] ^= ss[2]; k[v(48,(6*(i))+ 9)] = ss[6] ^= k[v(48,(6*(i))+ 3)]; \
    ss[4] ^= ss[3]; k[v(48,(6*(i))+10)] = ss[6] ^= k[v(48,(6*(i))+ 4)]; \
    ss[5] ^= ss[4]; k[v(48,(6*(i))+11)] = ss[6] ^= k[v(48,(6*(i))+ 5)]; \
}

#define kdl6(k,i) \
{   ss[0] ^= ls_box(ss[5],3) ^ t_use(r,c)[i]; k[v(48,(6*(i))+ 6)] = ss[0]; \
    ss[1] ^= ss[0]; k[v(48,(6*(i))+ 7)] = ss[1]; \
    ss[2] ^= ss[1]; k[v(48,(6*(i))+ 8)] = ss[2]; \
    ss[3] ^= ss[2]; k[v(48,(6*(i))+ 9)] = ss[3]; \
}

AES_RETURN aes_xi(decrypt_key192)(const unsigned char *key, trzr_aes_decrypt_ctx cx[1])
{   uint32_t    ss[7];
#if defined( d_vars )
        d_vars;
#endif

    cx->ks[v(48,(0))] = ss[0] = word_in(key, 0);
    cx->ks[v(48,(1))] = ss[1] = word_in(key, 1);
    cx->ks[v(48,(2))] = ss[2] = word_in(key, 2);
    cx->ks[v(48,(3))] = ss[3] = word_in(key, 3);

#ifdef DEC_KS_UNROLL
    ss[4] = word_in(key, 4);
    ss[5] = word_in(key, 5);

    cx->ks[v(48,(4))] = ff(ss[4]);
    cx->ks[v(48,(5))] = ff(ss[5]);
    kdf6(cx->ks, 0); kd6(cx->ks, 1);
    kd6(cx->ks, 2);  kd6(cx->ks, 3);
    kd6(cx->ks, 4);  kd6(cx->ks, 5);
    kd6(cx->ks, 6);  kdl6(cx->ks, 7);
#else
    cx->ks[v(48,(4))] = ss[4] = word_in(key, 4);
    cx->ks[v(48,(5))] = ss[5] = word_in(key, 5);
    {   uint32_t i;

        for(i = 0; i < 7; ++i)
            k6e(cx->ks, i);
        k6ef(cx->ks, 7);
#if !(DEC_ROUND == NO_TABLES)
        for(i = N_COLS; i < 12 * N_COLS; ++i)
            cx->ks[i] = inv_mcol(cx->ks[i]);
#endif
    }
#endif
    cx->inf.l = 0;
    cx->inf.b[0] = 12 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#if defined(AES_256) || defined( AES_VAR )

#define k8ef(k,i) \
{   k[v(56,(8*(i))+ 8)] = ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; \
    k[v(56,(8*(i))+ 9)] = ss[1] ^= ss[0]; \
    k[v(56,(8*(i))+10)] = ss[2] ^= ss[1]; \
    k[v(56,(8*(i))+11)] = ss[3] ^= ss[2]; \
}

#define k8e(k,i) \
{   k8ef(k,i); \
    k[v(56,(8*(i))+12)] = ss[4] ^= ls_box(ss[3],0); \
    k[v(56,(8*(i))+13)] = ss[5] ^= ss[4]; \
    k[v(56,(8*(i))+14)] = ss[6] ^= ss[5]; \
    k[v(56,(8*(i))+15)] = ss[7] ^= ss[6]; \
}

#define kdf8(k,i) \
{   ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[v(56,(8*(i))+ 8)] = ff(ss[0]); \
    ss[1] ^= ss[0]; k[v(56,(8*(i))+ 9)] = ff(ss[1]); \
    ss[2] ^= ss[1]; k[v(56,(8*(i))+10)] = ff(ss[2]); \
    ss[3] ^= ss[2]; k[v(56,(8*(i))+11)] = ff(ss[3]); \
    ss[4] ^= ls_box(ss[3],0); k[v(56,(8*(i))+12)] = ff(ss[4]); \
    ss[5] ^= ss[4]; k[v(56,(8*(i))+13)] = ff(ss[5]); \
    ss[6] ^= ss[5]; k[v(56,(8*(i))+14)] = ff(ss[6]); \
    ss[7] ^= ss[6]; k[v(56,(8*(i))+15)] = ff(ss[7]); \
}

#define kd8(k,i) \
{   ss[8] = ls_box(ss[7],3) ^ t_use(r,c)[i]; \
    ss[0] ^= ss[8]; ss[8] = ff(ss[8]); k[v(56,(8*(i))+ 8)] = ss[8] ^= k[v(56,(8*(i)))]; \
    ss[1] ^= ss[0]; k[v(56,(8*(i))+ 9)] = ss[8] ^= k[v(56,(8*(i))+ 1)]; \
    ss[2] ^= ss[1]; k[v(56,(8*(i))+10)] = ss[8] ^= k[v(56,(8*(i))+ 2)]; \
    ss[3] ^= ss[2]; k[v(56,(8*(i))+11)] = ss[8] ^= k[v(56,(8*(i))+ 3)]; \
    ss[8] = ls_box(ss[3],0); \
    ss[4] ^= ss[8]; ss[8] = ff(ss[8]); k[v(56,(8*(i))+12)] = ss[8] ^= k[v(56,(8*(i))+ 4)]; \
    ss[5] ^= ss[4]; k[v(56,(8*(i))+13)] = ss[8] ^= k[v(56,(8*(i))+ 5)]; \
    ss[6] ^= ss[5]; k[v(56,(8*(i))+14)] = ss[8] ^= k[v(56,(8*(i))+ 6)]; \
    ss[7] ^= ss[6]; k[v(56,(8*(i))+15)] = ss[8] ^= k[v(56,(8*(i))+ 7)]; \
}

#define kdl8(k,i) \
{   ss[0] ^= ls_box(ss[7],3) ^ t_use(r,c)[i]; k[v(56,(8*(i))+ 8)] = ss[0]; \
    ss[1] ^= ss[0]; k[v(56,(8*(i))+ 9)] = ss[1]; \
    ss[2] ^= ss[1]; k[v(56,(8*(i))+10)] = ss[2]; \
    ss[3] ^= ss[2]; k[v(56,(8*(i))+11)] = ss[3]; \
}

AES_RETURN aes_xi(decrypt_key256)(const unsigned char *key, trzr_aes_decrypt_ctx cx[1])
{   uint32_t    ss[9];
#if defined( d_vars )
        d_vars;
#endif

    cx->ks[v(56,(0))] = ss[0] = word_in(key, 0);
    cx->ks[v(56,(1))] = ss[1] = word_in(key, 1);
    cx->ks[v(56,(2))] = ss[2] = word_in(key, 2);
    cx->ks[v(56,(3))] = ss[3] = word_in(key, 3);

#ifdef DEC_KS_UNROLL
    ss[4] = word_in(key, 4);
    ss[5] = word_in(key, 5);
    ss[6] = word_in(key, 6);
    ss[7] = word_in(key, 7);

    cx->ks[v(56,(4))] = ff(ss[4]);
    cx->ks[v(56,(5))] = ff(ss[5]);
    cx->ks[v(56,(6))] = ff(ss[6]);
    cx->ks[v(56,(7))] = ff(ss[7]);
    kdf8(cx->ks, 0); kd8(cx->ks, 1);
    kd8(cx->ks, 2);  kd8(cx->ks, 3);
    kd8(cx->ks, 4);  kd8(cx->ks, 5);
    kdl8(cx->ks, 6);
#else
    cx->ks[v(56,(4))] = ss[4] = word_in(key, 4);
    cx->ks[v(56,(5))] = ss[5] = word_in(key, 5);
    cx->ks[v(56,(6))] = ss[6] = word_in(key, 6);
    cx->ks[v(56,(7))] = ss[7] = word_in(key, 7);
    {   uint32_t i;

        for(i = 0; i < 6; ++i)
            k8e(cx->ks,  i);
        k8ef(cx->ks,  6);
#if !(DEC_ROUND == NO_TABLES)
        for(i = N_COLS; i < 14 * N_COLS; ++i)
            cx->ks[i] = inv_mcol(cx->ks[i]);
#endif
    }
#endif
    cx->inf.l = 0;
    cx->inf.b[0] = 14 * 16;

#ifdef USE_VIA_ACE_IF_PRESENT
    if(VIA_ACE_AVAILABLE)
        cx->inf.b[1] = 0xff;
#endif
    return EXIT_SUCCESS;
}

#endif

#endif

#if defined( AES_VAR )

AES_RETURN trzr_aes_encrypt_key(const unsigned char *key, int key_len, trzr_aes_encrypt_ctx cx[1])
{
	switch(key_len)
	{
	case 16: case 128: return trzr_aes_encrypt_key128(key, cx);
	case 24: case 192: return trzr_aes_encrypt_key192(key, cx);
	case 32: case 256: return trzr_aes_encrypt_key256(key, cx);
	default: return EXIT_FAILURE;
	}
}

AES_RETURN trzr_aes_decrypt_key(const unsigned char *key, int key_len, trzr_aes_decrypt_ctx cx[1])
{
	switch(key_len)
	{
	case 16: case 128: return trzr_aes_decrypt_key128(key, cx);
	case 24: case 192: return trzr_aes_decrypt_key192(key, cx);
	case 32: case 256: return trzr_aes_decrypt_key256(key, cx);
	default: return EXIT_FAILURE;
	}
}

#endif

#if defined(__cplusplus)
}
#endif
