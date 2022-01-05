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

int free_ctx(bvm* vm) {
  int argc = be_top(vm);
  if (argc > 0) {
    be_getmember(vm, 1, ".p");
    md5_context_t * ctx = (md5_context_t *) be_tocomptr(vm, -1);
    if (ctx != NULL) {
      be_os_free(ctx);
    }
  }
  be_return_nil(vm);
}

// `Md5.init() -> `
int32_t m_md5_init(struct bvm *vm);
int32_t m_md5_init(struct bvm *vm) {

  md5_context_t * ctx = (md5_context_t *) be_os_malloc(sizeof(md5_context_t));
  if (!ctx) {
    be_throw(vm, BE_MALLOC_FAIL);
  }
  esp_rom_md5_init(ctx);

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
      md5_context_t * ctx;
      ctx = (md5_context_t *) be_tocomptr(vm, -1);
      if (!ctx) break;

      if (length > 0) {
        esp_rom_md5_update(ctx, (const uint8_t*) bytes, length);
      }
      be_return_nil(vm);
      // success
    } while (0);
  }
  be_raise(vm, "value_error", NULL);
}

// `Md5.update(content:bytes()) -> nil`
//
// Add raw bytes to the MD5 calculation
int32_t m_md5_finish(struct bvm *vm);
int32_t m_md5_finish(struct bvm *vm) {
  be_getmember(vm, 1, ".p");
  md5_context_t * ctx;
  ctx = (md5_context_t *) be_tocomptr(vm, -1);

  uint8_t output[16];
  esp_rom_md5_final(output, ctx);
  be_pushbytes(vm, output, sizeof(output));
  be_return(vm);
}

#include "be_fixed_be_class_md5.h"

void be_load_md5_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_md5);
    be_setglobal(vm, "MD5");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_class_md5 (scope: global, name: MD5) {
    .p, var

    init, func(m_md5_init)
    update, func(m_md5_update)
    finish, func(m_md5_finish)
}
@const_object_info_end */
