#include "be_constobj.h"

static be_define_const_map_slots(be_class_range_map) {
    { be_const_key(setrange, -1), be_const_func(m_setrange) },
    { be_const_key(iter, -1), be_const_func(m_iter) },
    { be_const_key(lower, -1), be_const_func(m_lower) },
    { be_const_key(init, 4), be_const_func(m_init) },
    { be_const_key(__upper__, -1), be_const_int(0) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(__lower__, -1), be_const_int(1) },
    { be_const_key(upper, 1), be_const_func(m_upper) },
};

static be_define_const_map(
    be_class_range_map,
    8
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_range,
    2,
    NULL,
    range
);
