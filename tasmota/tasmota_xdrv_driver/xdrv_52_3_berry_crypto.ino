/*
  xdrv_52_3_berry_md5.ino - Berry scripting language, Md5 class

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_BERRY

#include <berry.h>
#include "be_mem.h"
#include "be_object.h"
#include "include/ed25519.h"
#include "crypto/refc/poly1305-donna.h"

/*********************************************************************************************\
 * members class
 * 
\*********************************************************************************************/
extern "C" {
  extern const be_const_member_t be_crypto_members[];
  extern const size_t be_crypto_members_size;
  // virtual member
  int be_class_crypto_member(bvm *vm);
  int be_class_crypto_member(bvm *vm) {
    be_const_module_member_raise(vm, be_crypto_members, be_crypto_members_size);
    be_return(vm);
  }
}

/*********************************************************************************************\
 * Random bytes generator
 * 
 * As long as Wifi or BLE is enable, it uses a hardware source for true randomnesss
 * 
\*********************************************************************************************/
extern "C" {
  // `crypto.random(num_bytes:int) -> bytes(num_bytes)`
  //
  // Generates a series of random bytes
  int m_crypto_random(bvm *vm);
  int m_crypto_random(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isint(vm, 1)) {
      int32_t n = be_toint(vm, 1);
      if (n < 0 || n > 4096) { be_raise(vm, "value_error", ""); }

      uint8_t rand_bytes[n];
      esp_fill_random(rand_bytes, n);
      be_pushbytes(vm, rand_bytes, n);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * RSA class
 * 
\*********************************************************************************************/
extern "C" {
  // crypto.RSA.rsassa_pkcs1_v1_5(private_key:bytes(), msg:bytes()) -> bytes()
  // Parses RSA private key from DER binary
  int32_t m_rsa_rsassa_pkcs1_v1_5(bvm *vm);
  int32_t m_rsa_rsassa_pkcs1_v1_5(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1)) {
      size_t sk_len = 0;
      uint8_t * sk_der = (uint8_t*) be_tobytes(vm, 1, &sk_len);
      
      // 1. decode the DER private key
      br_skey_decoder_context sdc;
      br_skey_decoder_init(&sdc);
      br_skey_decoder_push(&sdc, sk_der, sk_len);
      if (int ret = br_skey_decoder_last_error(&sdc)) {
        be_raisef(vm, "value_error", "invalid private key %i", ret);
      }
      if (br_skey_decoder_key_type(&sdc) != BR_KEYTYPE_RSA) {
        be_raise(vm, "value_error", "key is not RSA");
      }
      const br_rsa_private_key *sk = br_skey_decoder_get_rsa(&sdc);
      
      // 2. Hash the message with SHA
      size_t msg_len = 0;
      uint8_t * msg = (uint8_t*) be_tobytes(vm, 2, &msg_len);
      uint8_t hash[32];
      br_sha256_context ctx;
      br_sha256_init(&ctx);
      br_sha256_update(&ctx, msg, msg_len);
      br_sha256_out(&ctx, hash);

      // 3. sign the message
      size_t sign_len = (sk->n_bitlen + 7) / 8;
      uint8_t x[sign_len] = {};
      int err = br_rsa_i15_pkcs1_sign(BR_HASH_OID_SHA256,	hash, sizeof(hash), sk, x);
      if (err != 1) {
        be_raisef(vm, "value_error", "signature failed %i", err);
      }

      be_pushbytes(vm, x, sign_len);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * AES_GCM class
 * 
\*********************************************************************************************/
extern "C" {

  // `AES_GCM.init(secret_key:bytes(32), iv:bytes(12)) -> instance`
  int32_t m_aes_gcm_init(struct bvm *vm);
  int32_t m_aes_gcm_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3 && be_isinstance(vm, 2) && be_isinstance(vm, 3)) {
      do {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (!be_isderived(vm, 2)) break;
        size_t length = 0;
        const void * bytes = be_tobytes(vm, 2, &length);
        if (!bytes) break;
        if (length != 32) {
          be_raise(vm, "value_error", "Key size must be 32 bytes");
        }

        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (!be_isderived(vm, 3)) break;
        size_t iv_length = 0;
        const void * iv_bytes = be_tobytes(vm, 3, &iv_length);
        if (!iv_bytes) break;

        // Initialize an AES CTR structure with the secret key
        br_aes_small_ctr_keys * ctr_ctx = (br_aes_small_ctr_keys *) be_os_malloc(sizeof(br_aes_small_ctr_keys));
        if (!ctr_ctx) { be_throw(vm, BE_MALLOC_FAIL); }
        br_aes_small_ctr_init(ctr_ctx, bytes, length);
        be_newcomobj(vm, ctr_ctx, &be_commonobj_destroy_generic);
        be_setmember(vm, 1, ".p1");

        // Initialize an AES GCM structure based on this CTR engine
        br_gcm_context * gcm_ctx = (br_gcm_context *) be_os_malloc(sizeof(br_gcm_context));
        if (!gcm_ctx) { be_throw(vm, BE_MALLOC_FAIL); }
		    br_gcm_init(gcm_ctx, &ctr_ctx->vtable, &br_ghash_ctmul32);
        be_newcomobj(vm, gcm_ctx, &be_commonobj_destroy_generic);
        be_setmember(vm, 1, ".p2");

        // Reset GCM context with provided IV
        br_gcm_reset(gcm_ctx, iv_bytes, iv_length);

        // We don't have any additional authenticated data so we flip instantly
        br_gcm_flip(gcm_ctx);

        be_return_nil(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t m_aes_gcm_encryt(bvm *vm);
  int32_t m_aes_gcm_decryt(bvm *vm);
  int32_t m_aes_gcm_encrypt_or_decryt(bvm *vm, int encrypt) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isinstance(vm, 2)) {
      do {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (!be_isderived(vm, 2)) break;

        // get GCM context
        be_getmember(vm, 1, ".p2");
        br_gcm_context * gcm_ctx = (br_gcm_context *) be_tocomptr(vm, -1);
        be_pop(vm, 1);

        // copy the input buffer
        be_getmember(vm, 2, "copy");    // stack: bytes.copy()
        be_pushvalue(vm, 2);            // stack: bytes.copy(), bytes instance
        be_call(vm, 1);                 // call copy with self parameter
        be_pop(vm, 1);                  // stack: clone of input bytes

        size_t length = 0;
        // we are changing bytes in place
        void * bytes = (void*) be_tobytes(vm, -1, &length);
        if (!bytes) break;

        br_gcm_run(gcm_ctx, encrypt, bytes, length);

        be_return(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  int32_t m_aes_gcm_encryt(bvm *vm) {
    return m_aes_gcm_encrypt_or_decryt(vm, 1);
  }
  int32_t m_aes_gcm_decryt(bvm *vm) {
    return m_aes_gcm_encrypt_or_decryt(vm, 0);
  }

  int32_t m_aes_gcm_tag(bvm *vm) {
    do {
      be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */

      // get GCM context
      be_getmember(vm, 1, ".p2");
      br_gcm_context * gcm_ctx = (br_gcm_context *) be_tocomptr(vm, -1);
      be_pop(vm, 1);

      // create a bytes buffer of 16 bytes
      uint8_t tag[16] = {0};
      br_gcm_get_tag(gcm_ctx, tag);
      be_pushbytes(vm, tag, sizeof(tag));

      be_return(vm);
      // success
    } while (0);
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * AES_CCM class
 * 
\*********************************************************************************************/
extern "C" {

  // `AES_CCM.init(secret_key:bytes(16 or 32), iv:bytes(7..13), aad:bytes(), data_len:int, tag_len:int) -> instance`
  //
  int32_t m_aes_ccm_init(struct bvm *vm);
  int32_t m_aes_ccm_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 6 && be_isbytes(vm, 2) && be_isbytes(vm, 3) && be_isbytes(vm, 4) && be_isint(vm, 5) && be_isint(vm, 6)) {
      do {
        size_t key_len = 0;
        const void * key = be_tobytes(vm, 2, &key_len);
        if (key_len != 32 && key_len != 16) {
          be_raise(vm, "value_error", "Key size must be 16 or 32 bytes");
        }

        size_t nonce_len = 0;
        const void * nonce = be_tobytes(vm, 3, &nonce_len);
        if (nonce_len < 7 || nonce_len > 13) {
          be_raise(vm, "value_error", "Nonce size must be 7..13");
        }

        size_t aad_len = 0;
        const void * aad = be_tobytes(vm, 4, &aad_len);

        int32_t data_len = be_toint(vm, 5);

        int32_t tag_len = be_toint(vm, 6);
        if (tag_len < 4 || tag_len > 16) {
          be_raise(vm, "value_error", "Tag size must be 4..16");
        }

        // Initialize an AES CCM structure with the secret key
        br_ccm_context * ccm_ctx = (br_ccm_context *) be_os_malloc(sizeof(br_ccm_context));
        if (!ccm_ctx) { be_throw(vm, BE_MALLOC_FAIL); }

        be_newcomobj(vm, ccm_ctx, &be_commonobj_destroy_generic);
        be_setmember(vm, 1, ".p1");

        br_aes_small_ctrcbc_keys * key_ctx = (br_aes_small_ctrcbc_keys *) be_os_malloc(sizeof(br_aes_small_ctrcbc_keys));
        if (!key_ctx) { be_throw(vm, BE_MALLOC_FAIL); }
        br_aes_small_ctrcbc_init(key_ctx, key, key_len);
        be_newcomobj(vm, key_ctx, &be_commonobj_destroy_generic);
        be_setmember(vm, 1, ".p2");

        br_ccm_init(ccm_ctx, &key_ctx->vtable);
        int ret = br_ccm_reset(ccm_ctx, nonce, nonce_len, aad_len, data_len, tag_len);
        if (ret == 0) { be_raise(vm, "value_error", "br_ccm_reset failed"); }

        if (aad_len > 0) {
          br_ccm_aad_inject(ccm_ctx, aad, aad_len);
        }
        br_ccm_flip(ccm_ctx);

        be_return_nil(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // Finish injection of authentication data
  int32_t m_aes_ccm_encrypt_or_decryt(bvm *vm, int encrypt);
  int32_t m_aes_ccm_encrypt(bvm *vm) { return m_aes_ccm_encrypt_or_decryt(vm, 1); }
  int32_t m_aes_ccm_decrypt(bvm *vm) { return m_aes_ccm_encrypt_or_decryt(vm, 0); }
  int32_t m_aes_ccm_encrypt_or_decryt(bvm *vm, int encrypt) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 2)) {
      do {
        // get CCM context
        be_getmember(vm, 1, ".p1");
        br_ccm_context * ccm_ctx = (br_ccm_context *) be_tocomptr(vm, -1);
        be_pop(vm, 1);

        // copy the input buffer
        be_getmember(vm, 2, "copy");    // stack: bytes.copy()
        be_pushvalue(vm, 2);            // stack: bytes.copy(), bytes instance
        be_call(vm, 1);                 // call copy with self parameter
        be_pop(vm, 1);                  // stack: clone of input bytes

        size_t length = 0;
        // we are changing bytes in place
        void * bytes = (void*) be_tobytes(vm, -1, &length);
        if (!bytes) break;

        br_ccm_run(ccm_ctx, encrypt, bytes, length);

        be_return(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int32_t m_aes_ccm_tag(bvm *vm) {
    do {
      be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */

      // get CCM context
      be_getmember(vm, 1, ".p1");
      br_ccm_context * ccm_ctx = (br_ccm_context *) be_tocomptr(vm, -1);
      be_pop(vm, 1);

      // create a bytes buffer of 16 bytes
      uint8_t tag[16] = {};
      br_ccm_get_tag(ccm_ctx, tag);
      be_pushbytes(vm, tag, sizeof(tag));

      be_return(vm);
      // success
    } while (0);
    be_raise(vm, kTypeError, nullptr);
  }

  // `AES_CCM.decrypt1(
  //      secret_key:bytes(16 or 32),
  //      iv:bytes(), iv_start:int, iv_len:int (7..13),
  //      aad:bytes(), aad_start:int, aad_len:int,
  //      data:bytes(), data_start:int, data_len:int,
  //      tag:bytes(), tag_start:int, tag_len:int (4..16))
  //      -> bool (true if tag matches)
  //
  // all-in-one decrypt function
  // decryption in place
  //
  int32_t m_aes_ccm_encrypt1_or_decryt1(bvm *vm, int encrypt);
  int32_t m_aes_ccm_encrypt1(bvm *vm) { return m_aes_ccm_encrypt1_or_decryt1(vm, 1); }
  int32_t m_aes_ccm_decrypt1(bvm *vm) { return m_aes_ccm_encrypt1_or_decryt1(vm, 0); }
  int32_t m_aes_ccm_encrypt1_or_decryt1(bvm *vm, int encrypt) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 13 && be_isbytes(vm, 1)    // secret_key
                   && be_isbytes(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4) // iv, iv_start, iv_len
                   && be_isbytes(vm, 5) && be_isint(vm, 6) && be_isint(vm, 7) // aad, aad_start, aad_len
                   && be_isbytes(vm, 8) && be_isint(vm, 9) && be_isint(vm, 10) // data_start, data_len
                   && be_isbytes(vm, 11) && be_isint(vm, 12) && be_isint(vm, 13)) { // tag, tag_start, tag_len

      size_t key_len = 0;
      const void * key = be_tobytes(vm, 1, &key_len);
      if (key_len != 32 && key_len != 16) {
        be_raise(vm, "value_error", "Key size must be 16 or 32 bytes");
      }

      size_t nonce_len = 0;
      const uint8_t * nonce = (const uint8_t *) be_tobytes(vm, 2, &nonce_len);
      int32_t n_start = be_toint(vm, 3);
      int32_t n_len = be_toint(vm, 4);
      if (n_start < 0 || n_len < 0 || n_start > nonce_len || n_start+n_len > nonce_len) {
        be_raise(vm, "range_error", "out of range start/end");
      }
      nonce += n_start;
      nonce_len = n_len;
      if (nonce_len < 7 || nonce_len > 13) {
        be_raise(vm, "value_error", "Nonce size must be 7..13");
      }

      size_t aad_len = 0;
      const uint8_t * aad = (const uint8_t *) be_tobytes(vm, 5, &aad_len);
      int32_t a_start = be_toint(vm, 6);
      int32_t a_len = be_toint(vm, 7);
      if (a_start < 0 || a_len < 0 || a_start  > aad_len || a_start+a_len > aad_len) {
        be_raise(vm, "range_error", "out of range start/end");
      }
      aad += a_start;
      aad_len = a_len;

      size_t data_len = 0;
      uint8_t * data = (uint8_t *) be_tobytes(vm, 8, &data_len);
      int32_t d_start = be_toint(vm, 9);
      int32_t d_len = be_toint(vm, 10);
      if (d_start < 0 || d_len < 0 || d_start  > data_len || d_start+d_len > data_len) {
        be_raise(vm, "range_error", "out of range start/end");
      }
      data += d_start;
      data_len = d_len;

      size_t tag_len = 0;
      uint8_t * tag = (uint8_t *) be_tobytes(vm, 11, &tag_len);
      int32_t t_start = be_toint(vm, 12);
      int32_t t_len = be_toint(vm, 13);
      if (t_start < 0 || t_len < 0 || t_start  > tag_len || t_start+t_len > tag_len) {
        be_raise(vm, "range_error", "out of range start/end");
      }
      tag += t_start;
      tag_len = t_len;
      if (tag_len < 4 || tag_len > 16) {
        be_raise(vm, "value_error", "Tag size must be 4..16");
      }

      // Initialize an AES CCM structure with the secret key
      br_aes_small_ctrcbc_keys key_ctx;
      br_ccm_context ccm_ctx;
      br_aes_small_ctrcbc_init(&key_ctx, key, key_len);
      br_ccm_init(&ccm_ctx, &key_ctx.vtable);
      int ret = br_ccm_reset(&ccm_ctx, nonce, nonce_len, aad_len, data_len, tag_len);
      if (ret == 0) { be_raise(vm, "value_error", "br_ccm_reset failed"); }

      if (aad_len > 0) {
        br_ccm_aad_inject(&ccm_ctx, aad, aad_len);
      }
      br_ccm_flip(&ccm_ctx);

      br_ccm_run(&ccm_ctx, encrypt, data, data_len);  // decrypt in place

      // check tag
      // create a bytes buffer of 16 bytes
      uint8_t tag_computed[16] = {};
      br_ccm_get_tag(&ccm_ctx, tag_computed);

      if (encrypt) {
        // copy the tag back
        memcpy(tag, tag_computed, tag_len);
        be_pushbool(vm, btrue);
      } else {
        // check that the tag match
        if (memcmp(tag_computed, tag, tag_len) == 0) {
          be_pushbool(vm, btrue);
        } else {
          be_pushbool(vm, bfalse);
        }
      }

      // success
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * AES_CTR class
 * 
\*********************************************************************************************/
extern "C" {

  // `AES_CTR.init(secret_key:bytes(32)) -> instance`
  int32_t m_aes_ctr_init(struct bvm *vm);
  int32_t m_aes_ctr_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 2)) {
      do {
        size_t length = 0;
        const void * bytes = be_tobytes(vm, 2, &length);
        if (!bytes) break;
        if (length != 32) {
          be_raise(vm, "value_error", "Key size must be 32 bytes");
        }

        // Initialize an AES CTR structure with the secret key
        br_aes_small_ctr_keys * ctr_ctx = (br_aes_small_ctr_keys *) be_os_malloc(sizeof(br_aes_small_ctr_keys));
        if (!ctr_ctx) { be_throw(vm, BE_MALLOC_FAIL); }
        br_aes_small_ctr_init(ctr_ctx, bytes, length);
        be_newcomobj(vm, ctr_ctx, &be_commonobj_destroy_generic);
        be_setmember(vm, 1, ".p1");

        be_return_nil(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `<instance:AES_CTR>.encrypt(content:bytes(), in:bytes(12), counter:int) -> bytes()`
  // `<instance:AES_CTR>.decrypt(content:bytes(), in:bytes(12), counter:int) -> bytes()`
  int32_t m_aes_ctr_run(bvm *vm);
  int32_t m_aes_ctr_run(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isbytes(vm, 2) && be_isbytes(vm, 3) && be_isint(vm, 4)) {
      do {
        // get GCM context
        be_getmember(vm, 1, ".p1");
        br_aes_small_ctr_keys * ctx = (br_aes_small_ctr_keys *) be_tocomptr(vm, -1);
        be_pop(vm, 1);

        size_t iv_len;
        const void * iv = be_tobytes(vm, 3, &iv_len);
        if (iv_len != 12) { be_raise(vm, "value_error", "IV size must be 12 bytes"); }

        uint32_t counter = be_toint(vm, 4);

        // copy the input buffer
        be_getmember(vm, 2, "copy");    // stack: bytes.copy()
        be_pushvalue(vm, 2);            // stack: bytes.copy(), bytes instance
        be_call(vm, 1);                 // call copy with self parameter
        be_pop(vm, 1);                  // stack: clone of input bytes

        size_t length = 0;
        // we are changing bytes in place
        void * bytes = (void*) be_tobytes(vm, -1, &length);
        if (!bytes) break;

        uint32_t next_counter = br_aes_small_ctr_run(ctx, iv, counter, bytes, length);

        be_return(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * AES_CBC class
 * 
\*********************************************************************************************/
extern "C" {
  // `AES_CBC.encrypt1(secret_key:bytes(16),iv:bytes(16),data:bytes(n*16))-> bool (true)
  int32_t m_aes_cbc_encrypt1(bvm *vm);
  int32_t m_aes_cbc_encrypt1(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3  && be_isbytes(vm, 1)    // secret_key  - 16 bytes
                   && be_isbytes(vm, 2)    // iv          - 16 bytes
                   && be_isbytes(vm, 3)    // data/cipher - multiple 16 bytes
                   ) {

      size_t key_len = 0;
      const void * key = be_tobytes(vm, 1, &key_len);
      if (key_len != 16) {
        be_raise(vm, "value_error", "Key size must be 16 bytes");
      }

      size_t iv_len = 0;
      void * iv = (void *) be_tobytes(vm, 2, &iv_len);
      if (iv_len != 16) {
        be_raise(vm, "value_error", "IV size must be 16");
      }

      size_t data_len = 0;
      void * data = (void *) be_tobytes(vm, 3, &data_len);
      if (data_len%16 != 0) {
        be_raise(vm, "value_error", "Data size must be multiple of 16");
      }

      // Initialize an AES CBC encryption structure with the secret key, then run with IV and data
      br_aes_small_cbcenc_keys cbc_ctx;
      br_aes_small_cbcenc_init(&cbc_ctx, key, 16);
      br_aes_small_cbcenc_run( &cbc_ctx, iv, data, data_len );
      
      // (unchecked )success
      be_pushbool(vm, btrue);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  // `AES_CBC.decrypt1(secret_key:bytes(16),iv:bytes(16),cipher:bytes(n*16))-> bool (true)
  int32_t m_aes_cbc_decrypt1(bvm *vm);
  int32_t m_aes_cbc_decrypt1(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3  && be_isbytes(vm, 1)    // secret_key  - 16 bytes
                   && be_isbytes(vm, 2)    // iv          - 16 bytes
                   && be_isbytes(vm, 3)    // cipher/data - multiple 16 bytes
                   ) {

      size_t key_len = 0;
      const void * key = be_tobytes(vm, 1, &key_len);
      if (key_len != 16) {
        be_raise(vm, "value_error", "Key size must be 16 bytes");
      }

      size_t iv_len = 0;
      void * iv = (void *) be_tobytes(vm, 2, &iv_len);
      if (iv_len != 16) {
        be_raise(vm, "value_error", "IV size must be 16");
      }

      size_t data_len = 0;
      void * data = (void *) be_tobytes(vm, 3, &data_len);
      if (data_len%16 != 0) {
        be_raise(vm, "value_error", "Cipher size must be multiple of 16");
      }

      // Initialize an AES CBC decryption structure with the secret key, then run with IV and data
      br_aes_small_cbcdec_keys cbc_ctx;
      br_aes_small_cbcdec_init(&cbc_ctx, key, 16);
      br_aes_small_cbcdec_run( &cbc_ctx, iv, data, data_len );
      
      // (unchecked )success
      be_pushbool(vm, btrue);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * CHACHA20-POLY1305  class
 * 
\*********************************************************************************************/
extern "C" {

   // `chacha20_run(secret_key:bytes(32),iv:bytes(12),cipher:bytes(n*16),)-> int counter
  int32_t m_chacha20_run(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4  && be_isbytes(vm, 1)    // secret_key  - 32 bytes
                   && be_isbytes(vm, 2)    // iv/nonce    - 12 bytes
                   && be_isint(vm, 3)      // counter
                   && be_isbytes(vm, 4)    // data/cipher
                   ) {

      size_t key_len = 0;
      const void * key = be_tobytes(vm, 1, &key_len);
      if (key_len != 32) {
        AddLog(LOG_LEVEL_INFO, PSTR(" %d bytes"), key_len);
        be_raise(vm, "value_error", "Key size must be 32 bytes");
      }

      size_t iv_len = 0;
      void * iv = (void *) be_tobytes(vm, 2, &iv_len);
      if (iv_len != 12) {
        AddLog(LOG_LEVEL_INFO, PSTR(" %d bytes"), iv_len);
        be_raise(vm, "value_error", "IV size must be 12");
      }

      int32_t cc = be_toint(vm, 3);

      size_t data_len = 0;
      void * data = (void *) be_tobytes(vm, 4, &data_len);

      br_chacha20_run bc = br_chacha20_ct_run;
    
      int32_t new_cc = br_chacha20_ct_run(key, iv, cc, data, data_len);

      be_pushint(vm, new_cc);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `poly_run(data:bytes(),poly_key:bytes(32),)-> tag:bytes(16)`
  int m_poly1305_run(bvm *vm){
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1)    // data in
                  && be_isbytes(vm, 2)    // poly key
    ) {
      unsigned char tag[16];

      size_t data_len = 0;
      const unsigned char * data = (const unsigned char *) be_tobytes(vm, 1, &data_len);

      size_t polykey_len = 0;
      const unsigned char * polykey = (const unsigned char *) be_tobytes(vm, 2, &polykey_len);
      if (polykey_len != 32) {
        AddLog(LOG_LEVEL_INFO, PSTR(" %d bytes"), polykey_len);
        be_raise(vm, "value_error", "poly key size must be 32 bytes");
      }
      poly1305_context ctx;
      poly1305_init(&ctx, polykey);
      poly1305_update(&ctx, data, data_len);
      poly1305_finish(&ctx, tag);

      be_pushbytes(vm, tag, sizeof(tag));
      be_return(vm); 
    }
  be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * SHA256 class
 * 
\*********************************************************************************************/
extern "C" {

  // `SHA256.init() -> nil`
  int32_t m_hash_sha256_init(struct bvm *vm);
  int32_t m_hash_sha256_init(struct bvm *vm) {
    // Initialize a SHA256 context
    br_sha256_context * ctx = (br_sha256_context *) be_os_malloc(sizeof(br_sha256_context));
    if (!ctx) {
      be_throw(vm, BE_MALLOC_FAIL);
    }
    br_sha256_init(ctx);

    be_newcomobj(vm, ctx, &be_commonobj_destroy_generic);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  // `<instance:SHA256>.update(content:bytes()) -> self`
  //
  // Add raw bytes to the hash calculation
  int32_t m_hash_sha256_update(struct bvm *vm);
  int32_t m_hash_sha256_update(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isinstance(vm, 2)) {
      do {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (!be_isderived(vm, 2)) break;
        size_t length = 0;
        const void * bytes = be_tobytes(vm, 2, &length);
        if (!bytes) break;

        be_getmember(vm, 1, ".p");
        br_sha256_context * ctx;
        ctx = (br_sha256_context *) be_tocomptr(vm, -1);
        if (!ctx) break;

        if (length > 0) {
          br_sha256_update(ctx, bytes, length);
        }
        be_pushvalue(vm, 1);    // return self
        be_return(vm);
        // success
      } while (0);
    }
    be_raise(vm, "value_error", NULL);
  }

  // `<instance:SHA256>.finish() -> bytes()`
  //
  // Add raw bytes to the MD5 calculation
  int32_t m_hash_sha256_out(struct bvm *vm);
  int32_t m_hash_sha256_out(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    br_sha256_context * ctx;
    ctx = (br_sha256_context *) be_tocomptr(vm, -1);

    uint8_t output[32];
    br_sha256_out(ctx, output);
    be_pushbytes(vm, output, sizeof(output));
    be_return(vm);
  }
}

/*********************************************************************************************\
 * HMAC_SHA256 class
 * 
\*********************************************************************************************/
extern "C" {

  // `HMAC_SHA256.init(key:bytes) -> nil`
  int32_t m_hmac_sha256_init(struct bvm *vm);
  int32_t m_hmac_sha256_init(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 2)) {
      // Initialize a HMAC context
      br_hmac_context * ctx = (br_hmac_context *) be_os_malloc(sizeof(br_hmac_context));
      if (!ctx) {
        be_throw(vm, BE_MALLOC_FAIL);
      }
      br_hmac_key_context keyCtx;   // keyCtx can be allocated on stack, it is not needed after `br_hmac_init`
      size_t key_len;
      const void *key = be_tobytes(vm, 2, &key_len);
      br_hmac_key_init(&keyCtx, &br_sha256_vtable, key, key_len);
      br_hmac_init(ctx, &keyCtx, 0);    // 0 is "natural output length"

      be_newcomobj(vm, ctx, &be_commonobj_destroy_generic);
      be_setmember(vm, 1, ".p");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `<instance:HMAC_SHA256>.update(content:bytes()) -> self`
  //
  // Add raw bytes to the hash calculation
  int32_t m_hmac_sha256_update(struct bvm *vm);
  int32_t m_hmac_sha256_update(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isinstance(vm, 2)) {
      do {
        if (!be_isbytes(vm, 2)) break;
        size_t length = 0;
        const void * bytes = be_tobytes(vm, 2, &length);
        if (!bytes) break;

        be_getmember(vm, 1, ".p");
        br_hmac_context * ctx;
        ctx = (br_hmac_context *) be_tocomptr(vm, -1);
        if (!ctx) break;

        if (length > 0) {
          br_hmac_update(ctx, bytes, length);
        }
        be_pushvalue(vm, 1);    // return self
        be_return(vm);
        // success
      } while (0);
    }
    be_raise(vm, "value_error", NULL);
  }

  // `<instance:SHA256>.finish() -> bytes()`
  //
  // Add raw bytes to the MD5 calculation
  int32_t m_hmac_sha256_out(struct bvm *vm);
  int32_t m_hmac_sha256_out(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    br_hmac_context * ctx;
    ctx = (br_hmac_context *) be_tocomptr(vm, -1);

    uint8_t output[32];
    br_hmac_out(ctx, output);
    be_pushbytes(vm, output, sizeof(output));
    be_return(vm);
  }
}

/*********************************************************************************************\
 * EC_P256 class
 * 
\*********************************************************************************************/
#define BR_EC_P256_IMPL  br_ec_p256_m15  // BearSSL implementation for Curve P256
extern "C" {
  // crypto.EC_P256().public_key(private_key:bytes(32)) -> bytes(65)
  // Computes the public key from a completely random private key of 32 bytes
  int32_t m_ec_p256_pubkey(bvm *vm);
  int32_t m_ec_p256_pubkey(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isbytes(vm, 1)) {
      size_t sk_len = 0;
      uint8_t * sk = (uint8_t*) be_tobytes(vm, 1, &sk_len);
      if (sk_len == 32) {
        br_ec_private_key br_sk = { BR_EC_secp256r1, sk, 32 };   // TODO

        uint8_t pk_buf[80];
        size_t ret = br_ec_compute_pub(&BR_EC_P256_IMPL, nullptr, pk_buf, &br_sk);
        if (ret > 0) {
          be_pushbytes(vm, pk_buf, ret);
          be_return(vm);
        }
      }
      be_raise(vm, "value_error", "invalid input");
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // crypto.EC_P256().shared_key(my_private_key:bytes(32), their_public_key:bytes(65)) -> bytes(32)
  // Computes the shared pre-key. Normally this shared pre-key is hashed with another algorithm.
  int32_t m_ec_p256_sharedkey(bvm *vm);
  int32_t m_ec_p256_sharedkey(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1) && be_isbytes(vm, 2)) {
      size_t sk_len = 0;
      const uint8_t * sk = (const uint8_t*) be_tobytes(vm, 1, &sk_len);
      size_t pk_len = 0;
      const uint8_t * pk_const = (const uint8_t*) be_tobytes(vm, 2, &pk_len);
      if (sk_len != 32 || pk_len == 0 || pk_len > 65) {
        be_raise(vm, "value_error", "Key size invalid");
      }
      uint8_t pk[pk_len];
      memmove(pk, pk_const, pk_len);  /* copy to a non-const variable to receive the result */

      if (BR_EC_P256_IMPL.mul(pk, pk_len, sk, sk_len, BR_EC_secp256r1)) {
        /* return value (xoff is one, length is 32) */
        be_pushbytes(vm, pk + 1, 32);
        be_return(vm);
      } else {
        be_raise(vm, "internal_error", "internal bearssl error in mul()");
      }
      be_raise(vm, "value_error", "invalid input");
    }
    be_raise(vm, kTypeError, nullptr);
  }
  
  // crypto.EC_P256().ecdsa_sign_sha256(my_private_key:bytes(32), message:bytes()) -> bytes(64)
  // Sign with ECDSA SHA256
  int32_t m_ec_p256_ecdsa_sign_sha256(bvm *vm);
  int32_t m_ec_p256_ecdsa_sign_sha256(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1) && be_isbytes(vm, 2)) {
      size_t sk_len = 0;
      uint8_t * sk = (uint8_t*) be_tobytes(vm, 1, &sk_len);
      size_t msg_len = 0;
      const uint8_t * msg = (const uint8_t*) be_tobytes(vm, 2, &msg_len);
      if (sk_len != 32) {
        be_raise(vm, "value_error", "Key size invalid");
      }

      // first compute SHA-256 hash on the message
      uint8_t hash[32];
      br_sha256_context ctx;
      br_sha256_init(&ctx);
      br_sha256_update(&ctx, msg, msg_len);
      br_sha256_out(&ctx, hash);
 
      // run ECDSA on hash
      uint8_t sign[64];         // hard limit for ECDSA SHA256
      br_ec_private_key br_sk = { BR_EC_secp256r1, sk, 32 };
      size_t sign_len = br_ecdsa_i15_sign_raw(&BR_EC_P256_IMPL, &br_sha256_vtable, hash, &br_sk, sign);

      be_pushbytes(vm, sign, sign_len);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `crypto.EC_P256().ecdsa_verify_sha256(public_key:bytes(65), message:bytes(), hash:bytes()) -> bool`
  // Verify signature with ECDSA SHA256
  int32_t m_ec_p256_ecdsa_verify_sha256(bvm *vm);
  int32_t m_ec_p256_ecdsa_verify_sha256(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3 && be_isbytes(vm, 1) && be_isbytes(vm, 2) && be_isbytes(vm, 3)) {
      size_t pk_len = 0;
      uint8_t * pk = (uint8_t*) be_tobytes(vm, 1, &pk_len);
      size_t msg_len = 0;
      const uint8_t * msg = (const uint8_t*) be_tobytes(vm, 2, &msg_len);
      if (pk_len != 65) {
        be_raise(vm, "value_error", "Key size invalid");
      }
      size_t sig_len = 0;
      const uint8_t * sig = (const uint8_t*) be_tobytes(vm, 3, &sig_len);

      // first compute SHA-256 hash on the message
      uint8_t hash[32];
      br_sha256_context ctx;
      br_sha256_init(&ctx);
      br_sha256_update(&ctx, msg, msg_len);
      br_sha256_out(&ctx, hash);
 
      // run ECDSA on hash
      br_ec_public_key br_pk = { BR_EC_secp256r1, pk, pk_len };
      uint32_t ret = br_ecdsa_i15_vrfy_raw(&BR_EC_P256_IMPL, hash, sizeof(hash), &br_pk, sig, sig_len);

      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // crypto.EC_P256().ecdsa_sign_sha256_asn1(my_private_key:bytes(32), message:bytes()) -> bytes()
  // Sign with ECDSA SHA256, result in ASN.1 format for CSR and certificate
  int32_t m_ec_p256_ecdsa_sign_sha256_asn1(bvm *vm);
  int32_t m_ec_p256_ecdsa_sign_sha256_asn1(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1) && be_isbytes(vm, 2)) {
      size_t sk_len = 0;
      uint8_t * sk = (uint8_t*) be_tobytes(vm, 1, &sk_len);
      size_t msg_len = 0;
      const uint8_t * msg = (const uint8_t*) be_tobytes(vm, 2, &msg_len);
      if (sk_len != 32) {
        be_raise(vm, "value_error", "Key size invalid");
      }

      // first compute SHA-256 hash on the message
      uint8_t hash[32];
      br_sha256_context ctx;
      br_sha256_init(&ctx);
      br_sha256_update(&ctx, msg, msg_len);
      br_sha256_out(&ctx, hash);
 
      // run ECDSA on hash
      uint8_t sign[72];         // hard limit for ECDSA SHA256 ASN.1 as per bearssl documentation
      br_ec_private_key br_sk = { BR_EC_secp256r1, sk, 32 };
      size_t sign_len = br_ecdsa_i15_sign_asn1(&BR_EC_P256_IMPL, &br_sha256_vtable, hash, &br_sk, sign);

      be_pushbytes(vm, sign, sign_len);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `crypto.EC_P256().ecdsa_verify_sha256_asn1(public_key:bytes(65), message:bytes(), hash:bytes()) -> bool`
  // Verify signature with ECDSA SHA256 with signature in ASN.1 format
  int32_t m_ec_p256_ecdsa_verify_sha256_asn1(bvm *vm);
  int32_t m_ec_p256_ecdsa_verify_sha256_asn1(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3 && be_isbytes(vm, 1) && be_isbytes(vm, 2) && be_isbytes(vm, 3)) {
      size_t pk_len = 0;
      uint8_t * pk = (uint8_t*) be_tobytes(vm, 1, &pk_len);
      size_t msg_len = 0;
      const uint8_t * msg = (const uint8_t*) be_tobytes(vm, 2, &msg_len);
      if (pk_len != 65) {
        be_raise(vm, "value_error", "Key size invalid");
      }
      size_t sig_len = 0;
      const uint8_t * sig = (const uint8_t*) be_tobytes(vm, 3, &sig_len);

      // first compute SHA-256 hash on the message
      uint8_t hash[32];
      br_sha256_context ctx;
      br_sha256_init(&ctx);
      br_sha256_update(&ctx, msg, msg_len);
      br_sha256_out(&ctx, hash);
 
      // run ECDSA on hash
      br_ec_public_key br_pk = { BR_EC_secp256r1, pk, pk_len };
      uint32_t ret = br_ecdsa_i15_vrfy_asn1(&BR_EC_P256_IMPL, hash, sizeof(hash), &br_pk, sig, sig_len);

      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  /* Test values
  import crypto
  var priv = bytes('D42A43989B67211031FF194FBA791B5C3E03F9EC10ED561A4DEB2AA7BADB4772')
  # var priv = crypto.random(32)
  var pub = crypto.EC_P256().public_key(priv)
  var msg = bytes().fromstring("Tasmota crypto ECDSA SECP256R1 SHA256 test message")

  var sig = crypto.EC_P256().ecdsa_sign_sha256(priv, msg)
  var ok = crypto.EC_P256().ecdsa_verify_sha256(pub, msg, sig)
  assert(ok == true)
  */

  // We have generated the P256 order as a i15 encoding using 
  // static const unsigned char P256_N[] PROGMEM = {
  //   0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
  //   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  //   0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84,
  //   0xF3, 0xB9, 0xCA, 0xC2^, 0xFC, 0x63, 0x25, 0x51
  // };
  // uint16_t m[20] = {};
  // br_i15_decode(m, P256_N, sizeof(P256_N));
  // AddLog(LOG_LEVEL_INFO, ">>>: mod=%*_H", sizeof(m), m);
  // 11015125C6780B2BCE1D4F68F362692B7D73BC7FFF7FFF7FFF7FFF0F00000040FF7FFF7F0100

  // N=bytes('11015125C6780B2BCE1D4F68F362692B7D73BC7FFF7FFF7FFF7FFF0F00000040FF7FFF7F0100')
  // s = ''
  // while size(N) > 0
  //   var n = N.get(0, 2)
  //   s += format("0x%04X, ", n)
  //   N = N[2..]
  // end
  // print(s)
  static const uint16_t P256_N_I15[] PROGMEM = {
    0x0111,
    0x2551, 0x78C6, 0x2B0B, 0x1DCE, 0x684F, 0x62F3, 0x2B69, 0x737D,
    0x7FBC, 0x7FFF, 0x7FFF, 0x7FFF, 0x0FFF, 0x0000, 0x4000, 0x7FFF,
    0x7FFF, 0x0001,
  };
  extern void br_i15_decode(uint16_t *x, const void *src, size_t len);
  extern void br_i15_decode_reduce(uint16_t *x, const void *src, size_t len, const uint16_t *m);
  extern void br_i15_encode(void *dst, size_t len, const uint16_t *x);
  extern uint32_t br_i15_sub(uint16_t *a, const uint16_t *b, uint32_t ctl);
  // crypto.EC_P256().mod(data:bytes()) -> bytes(32)
  // Reduces the big int to the modulus of P256 curve
  int32_t m_ec_p256_mod(bvm *vm);
  int32_t m_ec_p256_mod(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isbytes(vm, 1)) {
      size_t data_len = 0;
      uint8_t * data = (uint8_t*) be_tobytes(vm, 1, &data_len);
      if (data_len == 0) { be_raise(vm, "value_error", "data must not be empty"); }

      uint16_t data0[20] = {};
      br_i15_decode_reduce(data0, data, data_len, P256_N_I15);
      // AddLog(LOG_LEVEL_INFO, ">>>: data0=%*_H", sizeof(data0), data0);

      uint8_t out[32] = {};
      br_i15_encode(out, sizeof(out), data0);
      // AddLog(LOG_LEVEL_INFO, ">>>: out=%*_H", sizeof(out), out);
      // void br_i15_encode(void *dst, size_t len, const uint16_t *x);

      be_pushbytes(vm, out, sizeof(out));
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // crypto.EC_P256().neg(data:bytes(32)) -> bytes(32)
  // Negate a point coordinate modulus the order
  int32_t m_ec_p256_neg(bvm *vm);
  int32_t m_ec_p256_neg(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 1 && be_isbytes(vm, 1)) {
      size_t data_len = 0;
      uint8_t * data = (uint8_t*) be_tobytes(vm, 1, &data_len);
      if (data_len == 0) { be_raise(vm, "value_error", "data must not be empty"); }

      uint16_t data0[20] = {};
      br_i15_decode_reduce(data0, data, data_len, P256_N_I15);
      // AddLog(LOG_LEVEL_INFO, ">>>: data0=%*_H", sizeof(data0), data0);

      uint16_t a[sizeof(P256_N_I15)/sizeof(uint16_t)];
      memcpy(a, P256_N_I15, sizeof(P256_N_I15));      // copy generator to a

      uint32_t carry = br_i15_sub(a, data0, 1);       // carry is always zero since the number if taken modulus the generator
      // AddLog(LOG_LEVEL_INFO, ">>>: carry=%i data0=%*_H", carry, sizeof(data0), data0);

      uint8_t out[32] = {};
      br_i15_encode(out, sizeof(out), a);
      // AddLog(LOG_LEVEL_INFO, ">>>: out=%*_H", sizeof(out), out);
      // void br_i15_encode(void *dst, size_t len, const uint16_t *x);

      be_pushbytes(vm, out, sizeof(out));
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // crypto.EC_P256().mul(x:bytes(), A:bytes(65)) -> bytes(65)`
  //
  // The point `x*A` is computed.
  // `x` is unsigned and MUST be lower than order (use mod if not sure)
  // `A` must be bytes(65) and unencoded (starting with 0x04)
  int32_t m_ec_p256_mul(bvm *vm);
  int32_t m_ec_p256_mul(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 1) && be_isbytes(vm, 2)) {
      size_t x_len = 0;
      const uint8_t * x = (const uint8_t*) be_tobytes(vm, 1, &x_len);
      if (x_len == 0) { be_raise(vm, "value_error", "x must not be empty"); }

      size_t A_len = 0;
      const uint8_t * A = (const uint8_t*) be_tobytes(vm, 2, &A_len);
      if (A_len != 65 || (A_len > 0 && A[0] != 0x04)) { be_raise(vm, "value_error", "invalid A point"); }

      uint8_t res[65];
      memcpy(res, A, sizeof(res));    // copy A to res which will hold the result
      uint32_t ret = BR_EC_P256_IMPL.mul(res, 65, x, x_len, BR_EC_secp256r1);
      if (ret == 0) { be_raise(vm, "value_error", "muladd failed"); }

      be_pushbytes(vm, res, sizeof(res));
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
  // crypto.EC_P256().muladd(x:bytes(), A:bytes(65), y:bytes(), B;bytes(65) or bytes(0)) -> bytes(65)`
  //
  // The point `x*A + y*B` is computed.
  // If `B` is empty, the Generator is taken instead.
  // `x` and `y` are unsigned and MUST be lower than order (use mod if not sure)
  // `A` and `B` must be bytes(65) and unencoded (starting with 0x04)
  int32_t m_ec_p256_muladd(bvm *vm);
  int32_t m_ec_p256_muladd(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isbytes(vm, 1) && be_isbytes(vm, 2) && be_isbytes(vm, 3) && be_isbytes(vm, 4)) {
      size_t x_len = 0;
      const uint8_t * x = (const uint8_t*) be_tobytes(vm, 1, &x_len);
      if (x_len == 0) { be_raise(vm, "value_error", "x must not be empty"); }
      size_t y_len = 0;
      const uint8_t * y = (const uint8_t*) be_tobytes(vm, 3, &y_len);
      if (y_len == 0) { be_raise(vm, "value_error", "y must not be empty"); }

      size_t A_len = 0;
      const uint8_t * A = (const uint8_t*) be_tobytes(vm, 2, &A_len);
      if (A_len != 65 || (A_len > 0 && A[0] != 0x04)) { be_raise(vm, "value_error", "invalid A point"); }
      size_t B_len = 0;
      const uint8_t * B = (const uint8_t*) be_tobytes(vm, 4, &B_len);
      if (B_len == 0) {
        B = nullptr;    // generator
      } else {
        if (B_len != 65 || (B_len > 0 && B[0] != 0x04)) { be_raise(vm, "value_error", "invalid A point"); }
      }

      uint8_t res[65];
      memcpy(res, A, sizeof(res));    // copy A to res which will hold the result
      uint32_t ret = BR_EC_P256_IMPL.muladd(res, B, 65, x, x_len, y, y_len, BR_EC_secp256r1);
      if (ret == 0) { be_raise(vm, "value_error", "muladd failed"); }

      be_pushbytes(vm, res, sizeof(res));
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * EC C25519 class
 * 
\*********************************************************************************************/
#define BR_EC25519_IMPL  br_ec_c25519_m15  // BearSSL implementation for Curve 25519

extern "C" {
  // crypto.EC_C25519().public_key(private_key:bytes(32)) -> bytes(32)
  // Computes the public key from a completely random private key of 32 bytes
  int32_t m_ec_c25519_pubkey(bvm *vm);
  int32_t m_ec_c25519_pubkey(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isbytes(vm, 2)) {
      size_t buf_len = 0;
      const uint8_t * buf = (const uint8_t*) be_tobytes(vm, 2, &buf_len);
      if (buf_len == 32) {
        /* create the private key structure */
        uint8_t sk[32];
        for (int32_t i=0; i<32; i++) {
          sk[i] = buf[31-i];
        }
        br_ec_private_key br_sk = { BR_EC_curve25519, sk, 32 };

        uint8_t pk_buf[32]; /* EC 25519 is 32 bytes */
        size_t ret = br_ec_compute_pub(&BR_EC25519_IMPL, nullptr, pk_buf, &br_sk);
        if (ret == 32) {
          be_pushbytes(vm, pk_buf, ret);
          be_return(vm);
        }
      }
      be_raise(vm, "value_error", "invalid input");
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // crypto.EC_C25519().shared_key(my_private_key:bytes(32), their_public_key:bytes(32)) -> bytes(32)
  // Computes the shared pre-key. Normally this shared pre-key is hashed with another algorithm.
  int32_t m_ec_c25519_sharedkey(bvm *vm);
  int32_t m_ec_c25519_sharedkey(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 3 && be_isbytes(vm, 2) && be_isbytes(vm, 3)) {
      size_t sk_len = 0;
      const uint8_t * sk_buf = (const uint8_t*) be_tobytes(vm, 2, &sk_len);
      uint8_t sk[32];
      for (int32_t i=0; i<32; i++) {
        sk[i] = sk_buf[31-i];
      }
      size_t pk_len = 0;
      const uint8_t * pk_const = (const uint8_t*) be_tobytes(vm, 3, &pk_len);
      uint8_t pk[32];
      memmove(pk, pk_const, sizeof(pk));  /* copy to a non-const variable to receive the result */

      if (sk_len == 32 && pk_len == 32) {
        if (BR_EC25519_IMPL.mul(pk, pk_len, sk, sk_len, BR_EC_curve25519)) {
          /* return value (xoff is zero so no offset) */
          be_pushbytes(vm, pk, pk_len);
          be_return(vm);
        } else {
          be_raise(vm, "internal_error", "internal bearssl error in mul()");
        }
      }
      be_raise(vm, "value_error", "invalid input");
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

  /*********************************************************************************************\
 * ED25519 class
 * 
\*********************************************************************************************/
extern "C" {
  /* internal function to generate a secret key from a seed
   * The secret key is 64 bytes long, the first 32 bytes are the seed
   * and the last 32 bytes are the public key.
   * The seed must be 32 bytes long.
  */
  void _ed25519_get_secret_key(unsigned char *private_key, const unsigned char *seed){
    ge_p3 A;
    unsigned char pub_key[32];

    br_sha512_context ctx;
    br_sha512_init(&ctx);
    br_sha512_update(&ctx, seed, 32);
    br_sha512_out(&ctx, private_key);
    private_key[0] &= 248;
    private_key[31] &= 63;
    private_key[31] |= 64;

    ge_scalarmult_base(&A, private_key);
    ge_p3_tobytes(pub_key, &A);
    memmove(private_key, seed, 32);
    memmove(private_key + 32, pub_key, 32);
  }

  /*crypto.ED25519().secret_key(seed:bytes(32)) -> bytes(64)*/
  int32_t m_ed25519_secret_key(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isbytes(vm, 2))  // seed
    {
      size_t seed_len = 0;
      const unsigned char * seed = (const unsigned char *) be_tobytes(vm, 2, &seed_len);
      if (seed_len != 32) {
        be_raise(vm, "value_error", "seed size must be 32");
      }
      unsigned char sec_key[64];

      _ed25519_get_secret_key(sec_key, seed);
      be_pushbytes(vm, sec_key, 64);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }


  // https://github.com/rdeker/ed25519/blob/13a0661670949bc2e1cfcd8720082d9670768041/src/sign.c
  /*crypto.ED25519().sign(message:bytes(), secret_key:bytes(64)) -> signature:bytes(64)*/
  int32_t m_ed25519_sign(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_isbytes(vm, 2)  // message
                  && be_isbytes(vm, 3))  // secret key
                  {
      size_t msg_len = 0;
      const void * msg = be_tobytes(vm, 2, &msg_len);

      size_t sec_key_len = 0;
      void * sec_key = (void *) be_tobytes(vm, 3, &sec_key_len);
      if (sec_key_len != 64) {
        be_raise(vm, "value_error", "Key size must be 64");
      }

      uint8_t sign[64];
      unsigned char hram[64];
      unsigned char r[64];
      unsigned char az[64];
      ge_p3 R;

      br_sha512_context ctx;

      br_sha512_init(&ctx);
      br_sha512_update(&ctx, (unsigned char*)sec_key, 32);
      br_sha512_out(&ctx, az);
      az[0] &= 248;
      az[31] &= 63;
      az[31] |= 64;

      br_sha512_init(&ctx);
      br_sha512_update(&ctx, az + 32, 32);
      br_sha512_update(&ctx, msg, msg_len);
      br_sha512_out(&ctx, r);

      memmove((unsigned char*)sign + 32, (unsigned char*)sec_key + 32, 32);
      // memmove((unsigned char*)sign + 32, (unsigned char*)pub_key, 32);
  
      sc_reduce(r);
      ge_scalarmult_base(&R, r);
      ge_p3_tobytes((unsigned char*)sign, &R);


      br_sha512_init(&ctx);
      br_sha512_update(&ctx, sign, 64);
      br_sha512_update(&ctx, msg, msg_len);
      br_sha512_out(&ctx, hram);
  
      sc_reduce(hram);
      sc_muladd((unsigned char*)sign + 32, hram, az, r);

      be_pushbytes(vm, sign, 64);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // https://github.com/rdeker/ed25519/blob/13a0661670949bc2e1cfcd8720082d9670768041/src/verify.c
  /*crypto.ED25519().verify(message:bytes(), signature:bytes(64), public_key:bytes(32)) -> bool*/
  int32_t m_ed25519_verify(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isbytes(vm, 2)  // message
                  && be_isbytes(vm, 3)  // signature
                  && be_isbytes(vm, 4)) // public key
                  {
      size_t message_len = 0;
      const void * message = be_tobytes(vm, 2, &message_len);

      size_t sign_len = 0;
      const unsigned char * signature = (const unsigned char *) be_tobytes(vm, 3, &sign_len);
      if (sign_len != 64) {
        be_raise(vm, "value_error", "signature size must be 64");
      }

      size_t pub_key_len = 0;
      const unsigned char * public_key = (const unsigned char *) be_tobytes(vm, 4, &pub_key_len);
      if (pub_key_len != 32) {
        be_raise(vm, "value_error", "key size must be 32");
      }

      unsigned char h[64];
      unsigned char checker[32];
      br_sha512_context ctx;

      ge_p3 A;
      ge_p2 R;
  
      bbool success = false;
      if (signature[63] & 224) {
        AddLog(LOG_LEVEL_INFO, PSTR(" signature[63] & 224"));
          goto eddsa_verify_exit;
      }
  
      if (ge_frombytes_negate_vartime(&A, public_key) != 0) {
        AddLog(LOG_LEVEL_INFO, PSTR(" ge_frombytes_negate_vartime(&A, public_key) != 0"));
          goto eddsa_verify_exit;
      }

      br_sha512_init(&ctx);
      br_sha512_update(&ctx, signature, 32);
      br_sha512_update(&ctx, public_key, 32);
      br_sha512_update(&ctx, message, message_len);
      br_sha512_out(&ctx, h);
  
      sc_reduce(h);
      ge_double_scalarmult_vartime(&R, h, &A, signature + 32);
      ge_tobytes(checker, &R);
  
      if (memcmp(checker, signature, 32) == 0) {
          success = true;
      }
eddsa_verify_exit:
      be_pushbool(vm, success);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

/*********************************************************************************************\
 * HKDF_SHA256
 * 
\*********************************************************************************************/
extern "C" {
  // crypto.HKDF_SHA256().derive(ikm:bytes(), salt:bytes(), info:bytes(), out_bytes:int) -> bytes(out_bytes)
  // Derive key with HKDF based on SHA256
  int32_t m_hkdf_sha256_derive(bvm *vm);
  int32_t m_hkdf_sha256_derive(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isbytes(vm, 1) && be_isbytes(vm, 2) && be_isbytes(vm, 3) && be_isint(vm, 4)) {
      size_t ikm_len;
      const void * ikm = be_tobytes(vm, 1, &ikm_len);
      if (ikm_len == 0) { be_raise(vm, "value_error", "ikm must not be empty"); }

      size_t salt_len;
      const void * salt = be_tobytes(vm, 2, &salt_len);
      if (salt_len == 0) { salt = &br_hkdf_no_salt; }

      size_t info_len;
      const void * info = be_tobytes(vm, 3, &info_len);

      int32_t out_bytes = be_toint(vm, 4);
      if (out_bytes < 1 || out_bytes > 256) { be_raise(vm, "value_error", "invalid out_bytes"); }

      br_hkdf_context hc;
      br_hkdf_init(&hc, &br_sha256_vtable, salt, salt_len);
      br_hkdf_inject(&hc, ikm, ikm_len);
      br_hkdf_flip(&hc);
      uint8_t out[out_bytes];
      br_hkdf_produce(&hc, info, info_len, out, out_bytes);

      be_pushbytes(vm, out, out_bytes);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
/* Test vectors
# https://www.rfc-editor.org/rfc/rfc5869

import crypto

# Test Case 1
hk = crypto.HKDF_SHA256()
ikm = bytes("0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B0B")
salt = bytes("000102030405060708090A0B0C")
info = bytes("F0F1F2F3F4F5F6F7F8F9")
k = hk.derive(ikm, salt, info, 42)
assert(k == bytes("3CB25F25FAACD57A90434F64D0362F2A2D2D0A90CF1A5A4C5DB02D56ECC4C5BF34007208D5B887185865"))

# Test Case 2
hk = crypto.HKDF_SHA256()
ikm  = bytes("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f")
salt = bytes("606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeaf")
info = bytes("b0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff")
k = hk.derive(ikm, salt, info, 82)
assert(k == bytes("b11e398dc80327a1c8e7f78c596a49344f012eda2d4efad8a050cc4c19afa97c59045a99cac7827271cb41c65e590e09da3275600c2f09b8367793a9aca3db71cc30c58179ec3e87c14c01d5c1f3434f1d87"))

# Test Case 3
hk = crypto.HKDF_SHA256()
ikm  = bytes("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b")
salt = bytes()
info = bytes()
k = hk.derive(ikm, salt, info, 42)
assert(k == bytes("8da4e775a563c18f715f802a063c5a31b8a11f5c5ee1879ec3454e5f3c738d2d9d201395faa4b61a96c8"))

*/
}

/*********************************************************************************************\
 * PBKDF2_HMAC_SHA256
 * 
 * accelerate _f function
\*********************************************************************************************/
extern "C" {
  // _f(password:bytes(), salt_i:bytes(), c:int, res:bytes(32)) -> nil
  int32_t m_pbkdf2_hmac_sha256_f(bvm *vm);
  int32_t m_pbkdf2_hmac_sha256_f(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 4 && be_isbytes(vm, 1) && be_isbytes(vm, 2) && be_isint(vm, 3) && be_isbytes(vm, 4)) {
      size_t passwd_len;
      const void * passwd = be_tobytes(vm, 1, &passwd_len);
      if (passwd_len == 0) { be_raise(vm, "value_error", "passwd must not be empty"); }

      size_t salt_len;
      const void * salt = be_tobytes(vm, 2, &salt_len);
      if (salt_len == 0) { be_raise(vm, "value_error", "salt must not be empty"); }

      int32_t count = be_toint(vm, 3);
      if (count < 1 || count > 10000) { be_raise(vm, "value_error", "invalid iterations number"); }

      size_t res_len;
      uint8_t * res = (uint8_t*) be_tobytes(vm, 4, &res_len);
      if (res_len != 32) { be_raise(vm, "value_error", "res must be 32 bytes"); }

      br_hmac_context ctx;
      br_hmac_key_context keyCtx;   // keyCtx can be allocated on stack, it is not needed after `br_hmac_init`
      br_hmac_key_init(&keyCtx, &br_sha256_vtable, passwd, passwd_len);
      br_hmac_init(&ctx, &keyCtx, 0);    // 0 is "natural output length"

      // iteration 1
      br_hmac_update(&ctx, salt, salt_len);
      br_hmac_out(&ctx, res);
      uint8_t u[32];                    // rolling buffer
      memcpy(u, res, 32);               // copy res into u

      // further iterations
      for (uint32_t i = 2; i <= count; i++) {
        br_hmac_init(&ctx, &keyCtx, 0);    // reinit HMAC
        br_hmac_update(&ctx, u, sizeof(u));
        br_hmac_out(&ctx, u);
        for (uint32_t j=0; j<32; j++) {
          res[j] = res[j] ^ u[j];
        }
      }
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
}
#endif  // USE_BERRY