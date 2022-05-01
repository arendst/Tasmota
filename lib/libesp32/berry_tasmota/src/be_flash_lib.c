/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#include "esp_ota_ops.h"

void p_rollback(void) {
    esp_ota_mark_app_invalid_rollback_and_reboot();
}
BE_FUNC_CTYPE_DECLARE(p_rollback, "", "");

extern int p_flash_read(bvm *vm);
extern int p_flash_write(bvm *vm);
extern int p_flash_erase(bvm *vm);

/* @const_object_info_begin
module flash (scope: global) {
    read, func(p_flash_read)
    write, func(p_flash_write)
    erase, func(p_flash_erase)

    rollback, ctype_func(p_rollback)
}
@const_object_info_end */
#include "be_fixed_flash.h"
