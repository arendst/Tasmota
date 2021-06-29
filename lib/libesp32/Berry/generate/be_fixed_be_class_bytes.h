#include "be_constobj.h"

static be_define_const_map_slots(be_class_bytes_map) {
    { be_const_key(opt_add, -1), be_const_func(m_merge) },
    { be_const_key(set, -1), be_const_func(m_set) },
    { be_const_key(opt_connect, -1), be_const_func(m_connect) },
    { be_const_key(tostring, 2), be_const_func(m_tostring) },
    { be_const_key(size, -1), be_const_func(m_size) },
    { be_const_key(get, -1), be_const_func(m_getu) },
    { be_const_key(asstring, -1), be_const_func(m_asstring) },
    { be_const_key(fromstring, -1), be_const_func(m_fromstring) },
    { be_const_key(_buffer, -1), be_const_func(m_buffer) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(item, 12), be_const_func(m_item) },
    { be_const_key(getbits, -1), be_const_closure(getbits_closure) },
    { be_const_key(add, -1), be_const_func(m_add) },
    { be_const_key(setbits, -1), be_const_closure(setbits_closure) },
    { be_const_key(copy, 7), be_const_func(m_copy) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(geti, -1), be_const_func(m_geti) },
    { be_const_key(resize, -1), be_const_func(m_resize) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(opt_neq, 15), be_const_func(m_nequal) },
    { be_const_key(clear, -1), be_const_func(m_clear) },
    { be_const_key(opt_eq, -1), be_const_func(m_equal) },
};

static be_define_const_map(
    be_class_bytes_map,
    22
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_bytes,
    1,
    NULL,
    bytes
);
