/*
  be_matter_qrcode.cpp - implements Matter QRCode encoder as UTF8

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

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

#include <string.h>
#include "be_constobj.h"
#include "be_mapping.h"
#include "be_mem.h"
#include "be_exec.h"
#include "qrcodegen.h"

/******************************************************************************************************\
 * 
 * 
 * 
\******************************************************************************************************/

// `matter.QRCode.encode_str(content:string) -> map`
//
int qr_encode_str(bvm *vm) {
  int32_t argc = be_top(vm);
  if (argc >= 1 && be_isstring(vm, 1)) {
    const char * data_str = be_tostring(vm, 1);
    size_t data_len = strlen(data_str);

    int32_t qr_version = qrcodegen_getMinFitVersion(qrcodegen_Ecc_MEDIUM, data_len);
    if (qr_version <= 0) { be_return_nil(vm); }
    int32_t qr_size = qrcodegen_version2size(qr_version);
    if (qr_size <= 0) { be_return_nil(vm); }
    
    uint8_t * qr0 = (uint8_t *) be_os_malloc(qrcodegen_BUFFER_LEN_FOR_VERSION(qr_version));
    if (!qr0) { be_throw(vm, BE_MALLOC_FAIL); }
    uint8_t * data_tmp = (uint8_t *) be_os_malloc(qrcodegen_BUFFER_LEN_FOR_VERSION(qr_version));
    if (!qr0) { be_os_free(qr0); be_throw(vm, BE_MALLOC_FAIL); }

    bool ok = qrcodegen_encodeText(data_str, data_tmp, qr0, qrcodegen_Ecc_MEDIUM, qr_version, qr_version, qrcodegen_Mask_AUTO, true);

    if(!ok) {
      be_os_free(qr0);
      be_os_free(data_tmp);
      be_return_nil(vm);
    }

    qr_size = qrcodegen_getSize(qr0);
    size_t len = qr_size * qr_size;

    be_newobject(vm, "map");
    be_map_insert_int(vm, "size", qr_size);
    be_map_insert_int(vm, "version", qr_version);

    be_pushstring(vm, "bitmap");
    be_newobject(vm, "list");

    for (uint32_t i = 0; i < qr_size; i++) {
      char line[qr_size];

      for (uint32_t j = 0; j < qr_size; j++) {
        line[j] = qrcodegen_getModule(qr0, i, j) ? '*' : ' ';
      }

      be_pushnstring(vm, line, qr_size);
      be_data_push(vm, -2);
      be_pop(vm, 1);
    }

    be_pop(vm, 1);
    be_data_insert(vm, -3);
    be_pop(vm, 2);

    be_pop(vm, 1);

    be_os_free(qr0);
    be_os_free(data_tmp);

    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

#include "be_fixed_be_class_Matter_QRCode.h"

/* @const_object_info_begin
class be_class_Matter_QRCode (scope: global, name: Matter_QRCode, strings: weak) {
  encode_str, static_func(qr_encode_str)

  // UTF8 basic blocs for QR Codes
  // empty, str(" ")
  // lowhalf, str("\342\226\204")
  // uphalf, str("\342\226\200")
  // full, str("\342\226\210")
}
@const_object_info_end */
