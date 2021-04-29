#include "be_constobj.h"

static be_define_const_map_slots(be_class_bytes_map) {
    { be_const_key(copy, -1), be_const_func(m_copy) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(item, -1), be_const_func(m_item) },
    { be_const_key(init, 8), be_const_func(m_init) },
    { be_const_key(size, 6), be_const_func(m_size) },
    { be_const_key(opt_connect, 7), be_const_func(m_connect) },
    { be_const_key(opt_add, -1), be_const_func(m_merge) },
    { be_const_key(fromstring, -1), be_const_func(m_fromstring) },
    { be_const_key(opt_eq, 14), be_const_func(m_equal) },
    { be_const_key(get, 1), be_const_func(m_get) },
    { be_const_key(asstring, 5), be_const_func(m_asstring) },
    { be_const_key(add, -1), be_const_func(m_add) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(clear, -1), be_const_func(m_clear) },
    { be_const_key(opt_neq, 4), be_const_func(m_nequal) },
    { be_const_key(resize, 9), be_const_func(m_resize) },
};

static be_define_const_map(
    be_class_bytes_map,
    17
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_bytes,
    1,
    NULL,
    bytes
);
