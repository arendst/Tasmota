#include "be_constobj.h"

static be_define_const_map_slots(be_class_bytes_map) {
    { be_const_key(deinit, -1), be_const_func(m_deinit) },
    { be_const_key(get, 5), be_const_func(m_getu) },
    { be_const_key(fromb64, 18), be_const_func(m_fromb64) },
    { be_const_key(copy, -1), be_const_func(m_copy) },
    { be_const_key(_buffer, -1), be_const_func(m_buffer) },
    { be_const_key(getfloat, -1), be_const_func(m_getfloat) },
    { be_const_key(getbits, -1), be_const_closure(getbits_closure) },
    { be_const_key(tob64, -1), be_const_func(m_tob64) },
    { be_const_key(item, -1), be_const_func(m_item) },
    { be_const_key(_X21_X3D, -1), be_const_func(m_nequal) },
    { be_const_key(clear, 28), be_const_func(m_clear) },
    { be_const_key(size, 4), be_const_func(m_size) },
    { be_const_key(fromstring, -1), be_const_func(m_fromstring) },
    { be_const_key(_X2Elen, 21), be_const_var(0) },
    { be_const_key(seti, -1), be_const_func(m_set) },
    { be_const_key(_X2B, 9), be_const_func(m_merge) },
    { be_const_key(_X2Esize, 13), be_const_var(1) },
    { be_const_key(setfloat, 6), be_const_func(m_setfloat) },
    { be_const_key(_X2Ep, -1), be_const_var(2) },
    { be_const_key(set, -1), be_const_func(m_set) },
    { be_const_key(asstring, 0), be_const_func(m_asstring) },
    { be_const_key(add, 24), be_const_func(m_add) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(_change_buffer, -1), be_const_func(m_change_buffer) },
    { be_const_key(_X3D_X3D, 15), be_const_func(m_equal) },
    { be_const_key(resize, 2), be_const_func(m_resize) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(setbits, -1), be_const_closure(setbits_closure) },
    { be_const_key(_X2E_X2E, -1), be_const_func(m_connect) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(geti, -1), be_const_func(m_geti) },
};

static be_define_const_map(
    be_class_bytes_map,
    31
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_bytes,
    3,
    NULL,
    bytes
);
