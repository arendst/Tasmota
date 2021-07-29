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

//   // `Md5.update(content:bytes()) -> nil`
//   //
//   // Add raw bytes to the MD5 calculation
//   int32_t m_md5_update(struct bvm *vm);
//   int32_t m_md5_update(struct bvm *vm) {
//     int32_t argc = be_top(vm); // Get the number of arguments
//     if (argc >= 2 && be_isinstance(vm, 2)) {
//       do {
//         be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
//         if (!be_isderived(vm, 2)) break;
//         size_t length = 0;
//         const void * bytes = be_tobytes(vm, 2, &length);
//         if (!bytes) break;

//         be_getmember(vm, 1, ".p");
//         struct MD5Context * ctx;
//         ctx = (struct MD5Context *) be_tocomptr(vm, -1);
//         if (!ctx) break;

//         if (length > 0) {
//           MD5Update(ctx, (const uint8_t*) bytes, length);
//         }
//         be_return_nil(vm);
//         // success
//       } while (0);
//     }
//     be_raise(vm, kTypeError, nullptr);
//   }

//   // `Md5.update(content:bytes()) -> nil`
//   //
//   // Add raw bytes to the MD5 calculation
//   int32_t m_md5_finish(struct bvm *vm);
//   int32_t m_md5_finish(struct bvm *vm) {
//     be_getmember(vm, 1, ".p");
//     struct MD5Context * ctx;
//     ctx = (struct MD5Context *) be_tocomptr(vm, -1);

//     uint8_t output[16];
//     MD5Final(output, ctx);
//     be_pushbytes(vm, output, sizeof(output));
//     be_return(vm);
//   }
}

#endif // USE_ALEXA_AVS
#endif  // USE_BERRY
