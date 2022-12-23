/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"
#include "be_mem.h"
#include "be_exec.h"
#include "esp_rom_md5.h"

// `Md5.init() -> `
int m_md5_init(bvm *vm);
int m_md5_init(bvm *vm) {

  md5_context_t * ctx = (md5_context_t *) be_os_malloc(sizeof(md5_context_t));
  if (!ctx) {
    be_throw(vm, BE_MALLOC_FAIL);
  }
  esp_rom_md5_init(ctx);

  be_newcomobj(vm, ctx, &be_commonobj_destroy_generic);
  be_setmember(vm, 1, ".p");
  be_return_nil(vm);
}

// `Md5.update(content:bytes()) -> self`
//
// Add raw bytes to the MD5 calculation
int m_md5_update(bvm *vm);
int m_md5_update(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isbytes(vm, 2)) {
    do {
      size_t length = 0;
      const void * bytes = be_tobytes(vm, 2, &length);
      if (!bytes) break;

      be_getmember(vm, 1, ".p");
      md5_context_t * ctx;
      ctx = (md5_context_t *) be_tocomptr(vm, -1);
      if (!ctx) break;

      if (length > 0) {
        esp_rom_md5_update(ctx, (const uint8_t*) bytes, length);
      }
      be_pushvalue(vm, 1);
      be_return(vm);
      // success
    } while (0);
  }
  be_raise(vm, "value_error", NULL);
}

// `Md5.update(content:bytes()) -> nil`
//
// Add raw bytes to the MD5 calculation
int m_md5_finish(bvm *vm);
int m_md5_finish(bvm *vm) {
  be_getmember(vm, 1, ".p");
  md5_context_t * ctx;
  ctx = (md5_context_t *) be_tocomptr(vm, -1);

  uint8_t output[16];
  esp_rom_md5_final(output, ctx);
  be_pushbytes(vm, output, sizeof(output));
  be_return(vm);
}

#include "be_fixed_be_class_md5.h"

/* @const_object_info_begin

class be_class_md5 (scope: global, name: MD5) {
    .p, var

    init, func(m_md5_init)
    update, func(m_md5_update)
    finish, func(m_md5_finish)
}
@const_object_info_end */
