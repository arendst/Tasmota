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
#ifdef USE_ALEXA_AVS

#include <berry.h>
#include "be_mem.h"

/*********************************************************************************************\
 * AES class
 * 
\*********************************************************************************************/
extern "C" {

  int free_br_obj(bvm* vm) {
    int argc = be_top(vm);
    if (argc > 0) {
      void * obj = be_tocomptr(vm, 1);
      if (obj != NULL) { be_os_free(obj); }
    }
    be_return_nil(vm);
  }

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
        be_newcomobj(vm, ctr_ctx, &free_br_obj);
        be_setmember(vm, 1, ".p1");

        // Initialize an AES GCM structure based on this CTR engine
        br_gcm_context * gcm_ctx = (br_gcm_context *) be_os_malloc(sizeof(br_gcm_context));
        if (!gcm_ctx) { be_throw(vm, BE_MALLOC_FAIL); }
		    br_gcm_init(gcm_ctx, &ctr_ctx->vtable, &br_ghash_ctmul32);
        be_newcomobj(vm, gcm_ctx, &free_br_obj);
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
          be_raise(vm, "internal_error", "internal bearssl error in 519_m15.mul()");
        }
      }
      be_raise(vm, "value_error", "invalid input");
    }
    be_raise(vm, kTypeError, nullptr);
  }
}

#endif // USE_ALEXA_AVS
#endif  // USE_BERRY
