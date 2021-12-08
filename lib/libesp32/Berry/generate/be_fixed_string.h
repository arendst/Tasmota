#include "be_constobj.h"

static be_define_const_map_slots(m_libstring_map) {
    { be_const_key(char, -1), be_const_func(str_char) },
    { be_const_key(tr, -1), be_const_func(str_tr) },
    { be_const_key(count, 4), be_const_func(str_count) },
    { be_const_key(format, 7), be_const_func(str_format) },
    { be_const_key(escape, -1), be_const_func(str_escape) },
    { be_const_key(byte, -1), be_const_func(str_byte) },
    { be_const_key(toupper, -1), be_const_func(str_toupper) },
    { be_const_key(hex, -1), be_const_func(str_i2hex) },
    { be_const_key(find, -1), be_const_func(str_find) },
    { be_const_key(split, 1), be_const_func(str_split) },
    { be_const_key(tolower, -1), be_const_func(str_tolower) },
};

static be_define_const_map(
    m_libstring_map,
    11
);

static be_define_const_module(
    m_libstring,
    "string"
);

BE_EXPORT_VARIABLE be_define_const_native_module(string);
