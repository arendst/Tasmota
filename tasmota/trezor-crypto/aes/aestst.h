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

// The following definitions are required for testing only, They are not needed
// for AES (Rijndael) implementation.  They are used to allow C, C++ and DLL
// data access and subroutine calls to be expressed in the same form in the
// testing code.

#ifndef AESTST_H
#define AESTST_H

#define f_info(x)               (x)->inf.b[2]
#define f_ectx                  trzr_aes_encrypt_ctx
#define f_enc_key128(a,b)       trzr_aes_encrypt_key128((b),(a))
#define f_enc_key192(a,b)       trzr_aes_encrypt_key192((b),(a))
#define f_enc_key256(a,b)       trzr_aes_encrypt_key256((b),(a))
#define f_enc_key(a,b,c)        trzr_aes_encrypt_key((b),(c),(a))
#define f_enc_blk(a,b,c)        trzr_aes_encrypt((b),(c),(a))

#define f_dctx                  trzr_aes_decrypt_ctx
#define f_dec_key128(a,b)       trzr_aes_decrypt_key128((b),(a))
#define f_dec_key192(a,b)       trzr_aes_decrypt_key192((b),(a))
#define f_dec_key256(a,b)       trzr_aes_decrypt_key256((b),(a))
#define f_dec_key(a,b,c)        trzr_aes_decrypt_key((b),(c),(a))
#define f_dec_blk(a,b,c)        trzr_aes_decrypt((b),(c),(a))

#define f_talign(a,b)			aes_test_alignment_detection(b)
#define f_mode_reset(a)         aes_mode_reset(a)
#define f_ecb_enc(a,b,c,d)      aes_ecb_encrypt((b),(c),(d),(a))
#define f_ecb_dec(a,b,c,d)      aes_ecb_decrypt((b),(c),(d),(a))
#define f_cbc_enc(a,b,c,d,e)    aes_cbc_encrypt((b),(c),(d),(e),(a))
#define f_cbc_dec(a,b,c,d,e)    aes_cbc_decrypt((b),(c),(d),(e),(a))
#define f_cfb_enc(a,b,c,d,e)    aes_cfb_encrypt((b),(c),(d),(e),(a))
#define f_cfb_dec(a,b,c,d,e)    aes_cfb_decrypt((b),(c),(d),(e),(a))
#define f_ofb_cry(a,b,c,d,e)    aes_ofb_crypt((b),(c),(d),(e),(a))
#define f_ctr_cry(a,b,c,d,e,f)  aes_ctr_crypt((b),(c),(d),(e),(f),(a))

#define ek_name128          "trzr_aes_encrypt_key128"
#define ek_name192          "trzr_aes_encrypt_key192"
#define ek_name256          "trzr_aes_encrypt_key256"
#define ek_name             "trzr_aes_encrypt_key"
#define eb_name             "trzr_aes_encrypt"

#define dk_name128          "trzr_aes_decrypt_key128"
#define dk_name192          "trzr_aes_decrypt_key192"
#define dk_name256          "trzr_aes_decrypt_key256"
#define dk_name             "trzr_aes_decrypt_key"
#define db_name             "trzr_aes_decrypt"

#define eres_name           "aes_mode_reset"
#define ecbe_name           "aes_ecb_encrypt"
#define ecbd_name           "aes_ecb_decrypt"
#define cbce_name           "aes_cbc_encrypt"
#define cbcd_name           "aes_cbc_decrypt"
#define cfbe_name           "aes_cfb_encrypt"
#define cfbd_name           "aes_cfb_decrypt"
#define ofb_name            "aes_ofb_crypt"
#define ctr_name            "aes_ctr_crypt"

#ifndef AES_N_BLOCK
#define do_enc(a,b,c,d) f_enc_blk(a, b, c)
#define do_dec(a,b,c,d) f_dec_blk(a, b, c)
#else
#define do_enc(a,b,c,d) f_ecb_enc(a, b, c, AES_BLOCK_SIZE)
#define do_dec(a,b,c,d) f_ecb_dec(a, b, c, AES_BLOCK_SIZE)
#endif

#endif
