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

// Forces the next restart to use the `factory` partition if any is present
void p_factory(void) {
    const esp_partition_t *otadata_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_OTA, NULL);
    if (otadata_partition) {
        esp_partition_erase_range(otadata_partition, 0, SPI_FLASH_SEC_SIZE * 2);
    }
}
BE_FUNC_CTYPE_DECLARE(p_factory, "", "");

extern int p_flash_read(bvm *vm);
extern int p_flash_write(bvm *vm);
extern int p_flash_erase(bvm *vm);

/* @const_object_info_begin
module flash (scope: global) {
    read, func(p_flash_read)
    write, func(p_flash_write)
    erase, func(p_flash_erase)

    factory, ctype_func(p_factory)
}
@const_object_info_end */
#include "be_fixed_flash.h"
