#include "be_constobj.h"

static be_define_const_map_slots(be_class_list_map) {
    { be_const_key(resize, 2), be_const_func(m_resize) },
    { be_const_key(find, -1), be_const_func(m_find) },
    { be_const_key(size, 16), be_const_func(m_size) },
    { be_const_key(_X2E_X2E, -1), be_const_func(m_connect) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(item, 10), be_const_func(m_item) },
    { be_const_key(_X2Ep, -1), be_const_var(0) },
    { be_const_key(concat, -1), be_const_func(m_concat) },
    { be_const_key(reverse, -1), be_const_func(m_reverse) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(_X3D_X3D, 21), be_const_func(m_equal) },
    { be_const_key(pop, -1), be_const_func(m_pop) },
    { be_const_key(insert, -1), be_const_func(m_insert) },
    { be_const_key(_X2B, -1), be_const_func(m_merge) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(copy, 3), be_const_func(m_copy) },
    { be_const_key(iter, -1), be_const_func(m_iter) },
    { be_const_key(tobool, -1), be_const_func(m_tobool) },
    { be_const_key(push, -1), be_const_func(m_push) },
    { be_const_key(_X21_X3D, -1), be_const_func(m_nequal) },
    { be_const_key(clear, 17), be_const_func(m_clear) },
    { be_const_key(keys, -1), be_const_func(m_keys) },
    { be_const_key(remove, -1), be_const_func(m_remove) },
};

static be_define_const_map(
    be_class_list_map,
    23
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_list,
    1,
    NULL,
    list
);
