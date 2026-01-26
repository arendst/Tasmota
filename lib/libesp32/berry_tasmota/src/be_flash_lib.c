/********************************************************************
 * Berry module `webserver`
 * 
 * To use: `import webserver`
 * 
 * Allows to respond to HTTP request
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#include "esp_partition.h"
#include "bootloader_flash.h"

// Forces the next restart to use the `factory` partition if any is present
extern void p_factory(bbool force_ota);
BE_FUNC_CTYPE_DECLARE(p_factory, "", "b");

// return current OTA partition
extern int p_cur_ota();
BE_FUNC_CTYPE_DECLARE(p_cur_ota, "i", "");

int32_t p_flashid(void) {
    uint32_t id = bootloader_read_flash_id();
    id = ((id & 0xff) << 16) | ((id >> 16) & 0xff) | (id & 0xff00);
    return id;
}
BE_FUNC_CTYPE_DECLARE(p_flashid, "i", "");

int32_t p_flashsize(void) {
    uint32_t id = (p_flashid() >> 16) & 0xFF;
    return 2 << (id - 1);
}
BE_FUNC_CTYPE_DECLARE(p_flashsize, "i", "");

extern int p_flash_read(bvm *vm);
extern int p_flash_write(bvm *vm);
extern int p_flash_erase(bvm *vm);

/* @const_object_info_begin
module flash (scope: global) {
    read, func(p_flash_read)
    write, func(p_flash_write)
    erase, func(p_flash_erase)

    factory, ctype_func(p_factory)

    id, ctype_func(p_flashid)
    size, ctype_func(p_flashsize)
    current_ota, ctype_func(p_cur_ota)
}
@const_object_info_end */
#include "be_fixed_flash.h"
