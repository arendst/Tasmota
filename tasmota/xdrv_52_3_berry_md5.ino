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
#include "MD5Builder.h"
#include "be_mem.h"

/*********************************************************************************************\
 * Md5 class
 * 
\*********************************************************************************************/
extern "C" {

  int free_ctx(bvm* vm) {
    int argc = be_top(vm);
    if (argc > 0) {
      be_getmember(vm, 1, ".p");
      struct MD5Context * ctx = (struct MD5Context *) be_tocomptr(vm, -1);
      if (ctx != NULL) {
        be_os_free(ctx);
      }
    }
    be_return_nil(vm);
  }

  // `Md5.init() -> `
  int32_t m_md5_init(struct bvm *vm);
  int32_t m_md5_init(struct bvm *vm) {

    struct MD5Context * ctx = (struct MD5Context *) be_os_malloc(sizeof(struct MD5Context));
    if (!ctx) {
      be_throw(vm, BE_MALLOC_FAIL);
    }
    MD5Init(ctx);

    be_newcomobj(vm, ctx, &free_ctx);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  // `Md5.update(content:bytes()) -> nil`
  //
  // Add raw bytes to the MD5 calculation
  int32_t m_md5_update(struct bvm *vm);
  int32_t m_md5_update(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc >= 2 && be_isinstance(vm, 2)) {
      do {
        be_getglobal(vm, "bytes"); /* get the bytes class */ /* TODO eventually replace with be_getbuiltin */
        if (!be_isderived(vm, 2)) break;
        size_t length = 0;
        const void * bytes = be_tobytes(vm, 2, &length);
        if (!bytes) break;

        be_getmember(vm, 1, ".p");
        struct MD5Context * ctx;
        ctx = (struct MD5Context *) be_tocomptr(vm, -1);
        if (!ctx) break;

        if (length > 0) {
          MD5Update(ctx, (const uint8_t*) bytes, length);
        }
        be_return_nil(vm);
        // success
      } while (0);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  // `Md5.update(content:bytes()) -> nil`
  //
  // Add raw bytes to the MD5 calculation
  int32_t m_md5_finish(struct bvm *vm);
  int32_t m_md5_finish(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    struct MD5Context * ctx;
    ctx = (struct MD5Context *) be_tocomptr(vm, -1);

    uint8_t output[16];
    MD5Final(output, ctx);
    be_pushbytes(vm, output, sizeof(output));
    be_return(vm);
  }
}

#endif  // USE_BERRY
