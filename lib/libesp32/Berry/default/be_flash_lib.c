/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"

extern int p_flash_read(bvm *vm);
extern int p_flash_write(bvm *vm);
extern int p_flash_erase(bvm *vm);

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(flash) {
    be_native_module_function("read", p_flash_read),
    be_native_module_function("write", p_flash_write),
    be_native_module_function("erase", p_flash_erase),

};

be_define_native_module(flash, NULL);
#else
/* @const_object_info_begin
module flash (scope: global) {
    read, func(p_flash_read)
    write, func(p_flash_write)
    erase, func(p_flash_erase)
}
@const_object_info_end */
#include "../generate/be_fixed_flash.h"
#endif
