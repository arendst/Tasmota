/*
  xdrv_52_3_berry_unishox.ino - Berry scripting language, Unishox library

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

// Mappgin from internal light and a generic `light_state` Berry class

#ifdef USE_BERRY
#ifdef USE_UNISHOX_COMPRESSION

#include "be_mapping.h"
#include <string.h>
#include "unishox.h"

extern Unishox compressor;

extern "C" {
  /*********************************************************************************************\
   * Native functions mapped to Berry functions
   * 
   * import unishox
   * 
   * 
  \*********************************************************************************************/
  int be_ntv_unishox_compress(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isstring(vm, 1)) {
      const char * s = be_tostring(vm, 1);
      // do a dry-run to know the compressed size
      int32_t compressed_size = compressor.unishox_compress(s, strlen(s), (char*) nullptr, 0);
      if (compressed_size < 0) {
        be_raise(vm, "internal_error", nullptr);
      }
      void * buf = be_pushbytes(vm, NULL, compressed_size);
      if (compressed_size > 0) {
        int32_t ret = compressor.unishox_compress(s, strlen(s), (char*) buf, compressed_size+5);  // We expand by 4 the buffer size to avoid an error, but we are sure it will not overflow (see unishox implementation)
        if (ret < 0 || ret != compressed_size) {
          be_raisef(vm, "internal_error", "unishox size=%i ret=%i", compressed_size, ret);
        }
      }
      be_return(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }

  int be_ntv_unishox_decompress(bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1 && be_isbytes(vm, 1)) {
      size_t len;
      const void * buf = be_tobytes(vm, 1, &len);
      if (len == 0) {
        be_pushstring(vm, "");
      } else {
        int32_t decomp_size = compressor.unishox_decompress((const char*)buf, len, (char*) nullptr, 0);
        if (decomp_size < 0) {
          be_raise(vm, "internal_error", nullptr);
        }
        if (decomp_size == 0) {
          be_pushstring(vm, "");
        } else {
          void * buf_out = be_pushbuffer(vm, decomp_size);
          int32_t ret = compressor.unishox_decompress((const char*)buf, len, (char*) buf_out, decomp_size);
          if (ret < 0 || ret != decomp_size) {
            be_raisef(vm, "internal_error", "unishox size=%i ret=%i", decomp_size, ret);
          }
          be_pushnstring(vm, (const char*) buf_out, decomp_size);
        }
      }
      be_return(vm);
    }
    be_raise(vm, "type_error", nullptr);
  }
}

#endif // USE_UNISHOX_COMPRESSION
#endif  // USE_BERRY
