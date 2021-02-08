#include "be_constobj.h"

static be_define_const_map_slots(m_libstring_map) {
    { be_const_key(hex, 2), be_const_func(str_i2hex) },
    { be_const_key(byte, -1), be_const_func(str_byte) },
    { be_const_key(format, 5), be_const_func(str_format) },
    { be_const_key(find, -1), be_const_func(str_find) },
    { be_const_key(char, 1), be_const_func(str_char) },
    { be_const_key(split, -1), be_const_func(str_split) },
    { be_const_key(count, -1), be_const_func(str_count) },
};

static be_define_const_map(
    m_libstring_map,
    7
);

static be_define_const_module(
    m_libstring,
    "string"
);

BE_EXPORT_VARIABLE be_define_const_native_module(string, NULL);
