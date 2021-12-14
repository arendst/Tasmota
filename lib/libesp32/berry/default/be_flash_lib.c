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

/* @const_object_info_begin
module flash (scope: global) {
    read, func(p_flash_read)
    write, func(p_flash_write)
    erase, func(p_flash_erase)
}
@const_object_info_end */
#include "../generate/be_fixed_flash.h"
