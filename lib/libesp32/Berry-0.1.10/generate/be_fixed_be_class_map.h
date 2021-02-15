#include "be_constobj.h"

static be_define_const_map_slots(be_class_map_map) {
    { be_const_key(dot_p, 2), be_const_int(0) },
    { be_const_key(insert, -1), be_const_func(m_insert) },
    { be_const_key(remove, -1), be_const_func(m_remove) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(iter, 5), be_const_func(m_iter) },
    { be_const_key(size, -1), be_const_func(m_size) },
    { be_const_key(keys, -1), be_const_func(m_keys) },
    { be_const_key(setitem, -1), be_const_func(m_setitem) },
    { be_const_key(find, 0), be_const_func(m_find) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(item, 1), be_const_func(m_item) },
};

static be_define_const_map(
    be_class_map_map,
    11
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_map,
    1,
    NULL,
    map
);
