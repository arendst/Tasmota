#include "be_constobj.h"

static be_define_const_map_slots(be_class_list_map) {
    { be_const_key(concat, -1), be_const_func(m_concat) },
    { be_const_key(push, -1), be_const_func(m_push) },
    { be_const_key(insert, 1), be_const_func(m_insert) },
    { be_const_key(find, -1), be_const_func(m_find) },
    { be_const_key(remove, 9), be_const_func(m_remove) },
    { be_const_key(clear, -1), be_const_func(m_clear) },
    { be_const_key(size, -1), be_const_func(m_size) },
    { be_const_key(resize, 13), be_const_func(m_resize) },
    { be_const_key(copy, -1), be_const_func(m_copy) },
    { be_const_key(pop, -1), be_const_func(m_pop) },
    { be_const_key(tostring, 3), be_const_func(m_tostring) },
    { be_const_key(opt_eq, -1), be_const_func(m_equal) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(dot_p, 17), be_const_int(0) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(opt_connect, 4), be_const_func(m_connect) },
    { be_const_key(opt_neq, -1), be_const_func(m_nequal) },
    { be_const_key(opt_add, 18), be_const_func(m_merge) },
    { be_const_key(iter, 20), be_const_func(m_iter) },
    { be_const_key(item, -1), be_const_func(m_item) },
    { be_const_key(reverse, -1), be_const_func(m_reverse) },
};

static be_define_const_map(
    be_class_list_map,
    21
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_list,
    1,
    NULL,
    list
);
