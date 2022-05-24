#include "be_constobj.h"

static be_define_const_map_slots(m_libcrc_map) {
    { be_const_key(crc32, 1), be_const_ctype_func(c_crc32) },
    { be_const_key(sum, -1), be_const_ctype_func(c_sum) },
    { be_const_key(crc16, -1), be_const_ctype_func(c_crc16) },
    { be_const_key(crc8, -1), be_const_ctype_func(c_crc8) },
};

static be_define_const_map(
    m_libcrc_map,
    4
);

static be_define_const_module(
    m_libcrc,
    "crc"
);

BE_EXPORT_VARIABLE be_define_const_native_module(crc);
