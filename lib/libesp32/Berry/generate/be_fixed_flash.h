#include "be_constobj.h"

static be_define_const_map_slots(m_libflash_map) {
    { be_const_key(read, -1), be_const_func(p_flash_read) },
    { be_const_key(erase, -1), be_const_func(p_flash_erase) },
    { be_const_key(write, -1), be_const_func(p_flash_write) },
};

static be_define_const_map(
    m_libflash_map,
    3
);

static be_define_const_module(
    m_libflash,
    "flash"
);

BE_EXPORT_VARIABLE be_define_const_native_module(flash, NULL);
